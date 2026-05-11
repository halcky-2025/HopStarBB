#pragma once
// ============================================================
// lldbdriver.h - driver 子プロセス側 (= 送信側)
//
// 立ち位置:
//   送信側 = ここ。`HopStarBB.exe --lldb-driver <args>` で再起動された **別プロセス**
//            で動く。SBDebugger / SBTarget / SBProcess を所有して inferior を回し、
//            BP HIT 時に変数 / backtrace を stdout に流す。親 UI から resume / stop /
//            BP更新を受信する (= named event + 共有メモリ)。
//   受信側 = lldb.h の LLDBClient (= 親 UI)
//
// このヘッダは liblldb (= SB API) を include する。UI 側 TU には流れない (= UI は
// lldb.h だけ include する想定)。main.cpp が `--lldb-driver` 分岐のため直接 include する。
// ============================================================

#include <lldb/API/LLDB.h>
#pragma comment(lib, "liblldb.lib")

#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <functional>
#include <cstring>
#include <cstdio>
#include <windows.h>

// ============================================================
// LldbDriverApp: driver 子プロセス内で SBDebugger を回すクラス。
//   1. initialize()      : SBDebugger::Initialize / Create
//   2. createTarget()    : exe を開いてシンボル読み込み
//   3. setBreakpoint()   : file:line に BP を追加 / 削除 (target 未生成でも pending 登録)
//   4. launch()          : inferior を起動 (環境 / cwd 整備込み)
//   5. runEventLoop()    : eStateStopped を受けて var/backtrace 出力 → resume 待ち
//   6. shutdown()        : SBDebugger::Destroy / Terminate
// ============================================================
class LldbDriverApp {
public:
	// === 制御 atomic (= 親からの event 受信スレッドが立てる) ===
	// === 親 → driver 制御イベント ===
	// handleStop が WaitForMultipleObjects で直接待つので、専用 listener thread
	// は持たない (= 旧 50ms ポーリング sleep を廃止して即応化)。
	HANDLE evResume   = NULL;
	HANDLE evStop     = NULL;
	HANDLE evStepIn   = NULL;
	HANDLE evStepOver = NULL;
	HANDLE evStepOut  = NULL;

	// ============================================================
	// 初期化 / 後始末
	// ============================================================
	bool initialize() {
		using namespace lldb;
		fprintf(stderr, "[LLDB] SBDebugger::Initialize()...\n"); fflush(stderr);
		SBDebugger::Initialize();
		debugger_ = SBDebugger::Create();
		if (!debugger_.IsValid()) {
			fprintf(stderr, "[LLDB] SBDebugger::Create 失敗\n");
			SBDebugger::Terminate();
			return false;
		}
		debugger_.SetAsync(true);
		listener_ = debugger_.GetListener();
		return true;
	}

	void shutdown() {
		using namespace lldb;
		if (debugger_.IsValid()) SBDebugger::Destroy(debugger_);
		SBDebugger::Terminate();
	}

	// ============================================================
	// Target 作成
	// ============================================================
	bool createTarget(const char* exePath) {
		using namespace lldb;
		fprintf(stderr, "[LLDB] CreateTarget(%s)...\n", exePath ? exePath : "(NULL)"); fflush(stderr);
		exePath_ = exePath ? exePath : "";
		const char* exeBase = strrchr(exePath_.c_str(), '\\');
		exeBase_ = exeBase ? (exeBase + 1) : exePath_.c_str();

		SBError error;
		target_ = debugger_.CreateTarget(exePath_.c_str(), nullptr, nullptr, true, error);
		if (!target_.IsValid() || !error.Success()) {
			dumpError("CreateTarget", error);
			return false;
		}
		return true;
	}

	// ============================================================
	// BP 全置換 (= 親から渡される「現在有効な BP リスト」を真とみなして同期)
	//
	// 親が BP を削除すると、共有メモリ上の list から entry が swap-pop で消える。
	// driver は新 list だけ受け取るので「前回あった entry」が消えても気付けない。
	// → ここで自前 bpIds_ と diff して、欠けている (file, line) を target から
	//   BreakpointDelete する。残り部分は setBreakpoint で再適用。
	// ============================================================
	void syncBreakpoints(const std::vector<std::pair<std::string, uint32_t>>& nowEnabled) {
		// 入力を set 化 (= O(1) lookup)
		std::set<std::pair<std::string, uint32_t>> nowSet(nowEnabled.begin(), nowEnabled.end());
		// bpIds_ を走査して set に居ない entry を delete
		for (auto fIt = bpIds_.begin(); fIt != bpIds_.end(); ) {
			auto& lineMap = fIt->second;
			for (auto lIt = lineMap.begin(); lIt != lineMap.end(); ) {
				if (nowSet.find({ fIt->first, lIt->first }) == nowSet.end()) {
					if (target_.IsValid() && lIt->second >= 0) {
						target_.BreakpointDelete(lIt->second);
					}
					fprintf(stderr, "[LLDB] BP 同期削除: %s:%u\n", fIt->first.c_str(), lIt->first);
					lIt = lineMap.erase(lIt);
				} else {
					++lIt;
				}
			}
			if (lineMap.empty()) fIt = bpIds_.erase(fIt);
			else                 ++fIt;
		}
	}

	// ============================================================
	// BP 追加 / 削除 (= file:line 単位)
	// ============================================================
	int setBreakpoint(const std::string& file, uint32_t line, bool enabled) {
		using namespace lldb;
		auto& lineMap = bpIds_[file];
		auto it = lineMap.find(line);
		if (it != lineMap.end()) {
			if (target_.IsValid()) target_.BreakpointDelete(it->second);
			lineMap.erase(it);
			fprintf(stderr, "[LLDB] BP 削除: %s:%u\n", file.c_str(), line);
		}
		if (!enabled) return -1;

		if (!target_.IsValid()) {
			lineMap[line] = -1;
			return -1;
		}

		SBFileSpec fileSpec(file.c_str(), false);
		SBFileSpecList moduleList;
		if (!exeBase_.empty()) moduleList.Append(SBFileSpec(exeBase_.c_str(), false));
		SBBreakpoint bp = target_.BreakpointCreateByLocation(fileSpec, line, 0, moduleList);
		if (!bp.IsValid()) {
			fprintf(stderr, "[LLDB] BP 作成失敗 %s:%u\n", file.c_str(), line);
			return -1;
		}
		// スライド検証: LLDB は実行コードの無い行 (= include / コメント / 空行)
		// を指定すると、最も近い実行可能行に勝手にスライドする。スライドした
		// 場合は「ユーザが意図しない位置で止まる」ので **完全一致以外は reject**。
		// (parent 側のリスト / state.db には残るが、driver 側で BP を立てない
		// = 停止が起きないので実害なし。掃除したければ state::clear_breakpoints)
		if (bp.GetNumLocations() > 0) {
			SBBreakpointLocation loc = bp.GetLocationAtIndex(0);
			SBLineEntry le = loc.GetAddress().GetLineEntry();
			uint32_t resolvedLine = le.GetLine();
			if (resolvedLine != 0 && resolvedLine != line) {
				fprintf(stderr, "[LLDB] BP スライド検出 (要求 %u → 解決 %u)、reject\n",
					line, resolvedLine);
				target_.BreakpointDelete(bp.GetID());
				return -1;
			}
		}
		int id = bp.GetID();
		lineMap[line] = id;
		fprintf(stderr, "[LLDB] BP 追加: %s:%u -> id=%d locs=%u\n",
			file.c_str(), line, id, bp.GetNumLocations());
		return id;
	}

	// ============================================================
	// inferior 起動
	// ============================================================
	bool launch() {
		using namespace lldb;
		SBLaunchInfo launchInfo(nullptr);
		launchInfo.SetLaunchFlags(eLaunchFlagStopAtEntry |
			eLaunchFlagDisableASLR |
			eLaunchFlagDisableSTDIO);

		// minimal env (= VS dev env を継承させない)
		char path[2048];
		snprintf(path, sizeof(path),
			"PATH=%s;"
			"C:\\C2;"
			"C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib;"
			"C:\\Users\\hayah\\Downloads\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\bin;"
			"C:\\Users\\hayah\\AppData\\Local\\Programs\\Python\\Python310;"
			"C:\\Windows\\System32;C:\\Windows",
			"C:\\Users\\hayah\\source\\repos\\HopStarBB\\HopStarBB");
		const char* envs[] = {
			path,
			"SystemRoot=C:\\Windows",
			"SystemDrive=C:",
			"TEMP=C:\\Users\\hayah\\AppData\\Local\\Temp",
			"TMP=C:\\Users\\hayah\\AppData\\Local\\Temp",
			"USERPROFILE=C:\\Users\\hayah",
			"USERNAME=hayah",
			"COMPUTERNAME=",
			"PROCESSOR_ARCHITECTURE=AMD64",
			"NUMBER_OF_PROCESSORS=8",
			"OS=Windows_NT",
			"WINDIR=C:\\Windows",
			"LOCALAPPDATA=C:\\Users\\hayah\\AppData\\Local",
			"APPDATA=C:\\Users\\hayah\\AppData\\Roaming",
			"PROGRAMDATA=C:\\ProgramData",
			"PROGRAMFILES=C:\\Program Files",
			nullptr
		};
		launchInfo.SetEnvironmentEntries(envs, false);
		SetEnvironmentVariableA("HOPSTARBB_LLDB_CHILD", "1");

		char workDir[MAX_PATH] = { 0 };
		strncpy(workDir, exePath_.c_str(), sizeof(workDir) - 1);
		char* lastSlash = strrchr(workDir, '\\');
		if (!lastSlash) lastSlash = strrchr(workDir, '/');
		if (lastSlash) *lastSlash = '\0';
		else GetCurrentDirectoryA((DWORD)sizeof(workDir), workDir);
		launchInfo.SetWorkingDirectory(workDir);

		SBError error;
		process_ = target_.Launch(launchInfo, error);
		SetEnvironmentVariableA("HOPSTARBB_LLDB_CHILD", NULL);
		if (!process_.IsValid() || !error.Success()) {
			dumpError("Launch", error);
			return false;
		}
		fprintf(stderr, "[LLDB] launched pid=%llu\n",
			(unsigned long long)process_.GetProcessID());

		SBCommandInterpreter ci = debugger_.GetCommandInterpreter();
		SBCommandReturnObject ret;
		ci.HandleCommand("settings set auto-confirm true", ret);
		ci.HandleCommand("process handle --pass true --stop false --notify false exception", ret);

		SBTarget tg = process_.GetTarget();
		uint32_t nm = tg.GetNumModules();
		fprintf(stderr, "[LLDB] modules loaded: %u\n", nm);
		for (uint32_t i = 0; i < nm && i < 32; ++i) {
			SBModule m = tg.GetModuleAtIndex(i);
			if (!m.IsValid()) continue;
			const char* name = m.GetFileSpec().GetFilename();
			SBFileSpec sym = m.GetSymbolFileSpec();
			const char* symName = sym.IsValid() ? sym.GetFilename() : nullptr;
			uint32_t numSyms = m.GetNumSymbols();
			uint32_t numCu = m.GetNumCompileUnits();
			fprintf(stderr, "  [%2u] %-32s syms=%5u  compile_units=%4u  pdb=%s\n",
				i, name ? name : "?", numSyms, numCu,
				symName ? symName : "(なし)");
		}
		return true;
	}

	// ============================================================
	// イベントループ (= eStateStopped で var / backtrace 出力 + resume 待ち)
	// ============================================================
	void runEventLoop() {
		using namespace lldb;
		bool running = true;
		firstExceptionConsumed_ = false;
		while (running) {
			SBEvent ev;
			if (!listener_.WaitForEvent(1, ev)) continue;
			if (!SBProcess::EventIsProcessEvent(ev)) continue;

			StateType state = SBProcess::GetStateFromEvent(ev);
			switch (state) {
			case eStateStopped:
				if (!handleStop()) running = false;
				break;
			case eStateExited:
				fprintf(stderr, "[LLDB] exited code=%d\n", process_.GetExitStatus());
				running = false;
				break;
			case eStateCrashed:
				fprintf(stderr, "[LLDB] crashed\n");
				running = false;
				break;
			case eStateDetached:
			case eStateInvalid:
				fprintf(stderr, "[LLDB] detached / invalid\n");
				running = false;
				break;
			default:
				break;
			}
		}
	}

public:
	// ============================================================
	// chain.txt 読み込み: 「(field 名, enum 名) → クラス名」マッピング。
	//
	//   形式: 1 行 = "<field_name> <enum_member_name> <class_name>" (スペース区切り)。
	//   コメント行 (= 先頭 #) と空行は無視。
	//   例:
	//       type     _TableElem  NewTable     # NewElement::type が _TableElem → NewTable
	//       objtype  OT_Function Function     # Obj::objtype が OT_Function → Function
	//
	//   用途: 変数 expand 時に struct に「指定フィールドが enum 型」を見つけたら、
	//   そこに記されている enum 名から実体クラスを引いて、その派生型として再解釈する。
	//   (= base class pointer 越しの実型を見せる)
	// ============================================================
	void loadChainTxt(const std::string& path) {
		FILE* f = std::fopen(path.c_str(), "rb");
		if (!f) {
			fprintf(stderr, "[LLDB] chain.txt 読み込み skip (= %s 無し)\n", path.c_str());
			return;
		}
		char line[512];
		int n = 0;
		while (std::fgets(line, sizeof(line), f)) {
			// 改行 / 末尾 ws trim
			size_t L = std::strlen(line);
			while (L > 0 && (line[L-1] == '\n' || line[L-1] == '\r' || line[L-1] == ' ' || line[L-1] == '\t')) line[--L] = '\0';
			char* p = line;
			while (*p == ' ' || *p == '\t') ++p;
			if (*p == '\0' || *p == '#') continue;
			// 3 token を順に切り出し
			auto nextTok = [&]() -> char* {
				char* tok = p;
				while (*p && *p != ' ' && *p != '\t') ++p;
				if (*p) { *p++ = '\0'; }
				while (*p == ' ' || *p == '\t') ++p;
				return tok;
			};
			char* fieldName = nextTok();
			if (!*fieldName) continue;
			char* enumName  = nextTok();
			if (!*enumName) continue;
			char* className = nextTok();
			if (!*className) continue;
			chainMap_[std::string(fieldName)][std::string(enumName)] = std::string(className);
			fieldNames_.insert(std::string(fieldName));
			++n;
		}
		std::fclose(f);
		fprintf(stderr, "[LLDB] chain.txt 読み込み完了: %d entries (%zu fields)\n",
			n, fieldNames_.size());
	}

	// ============================================================
	// applyChain: SBValue が chain.txt に登録された field 名を持ち、enum 値が
	// マッピングにあれば派生型に Cast した SBValue を返す。マッチなければ元の v。
	// ============================================================
	lldb::SBValue applyChain(lldb::SBValue v) {
		using namespace lldb;
		if (chainMap_.empty() || !v.IsValid()) return v;
		SBType t = v.GetType();
		if (!t.IsValid()) return v;
		SBValue body = t.IsPointerType() ? v.Dereference() : v;
		if (!body.IsValid()) return v;
		// 登録された全 field 名について順に試す (= 通常 1 個 = `type`)。
		// `objtype` は廃止 (= struct 側で `type` field に統合済み、chain.txt も `type` 行に統一)。
		// `dtype` は **List 要素型用の lookup 専用**で List 自体の Cast には使わないので skip。
		// 通すと List<Offscreen> が Offscreen* に化けて List のメンバ (= data/size 等) が見えなくなる。
		for (auto& fname : fieldNames_) {
			if (fname == "dtype") continue;
			SBValue tf = body.GetChildMemberWithName(fname.c_str());
			if (!tf.IsValid()) continue;
			const char* enumName = tf.GetValue();
			if (!enumName) continue;
			auto fit = chainMap_.find(fname);
			if (fit == chainMap_.end()) continue;
			auto eit = fit->second.find(enumName);
			if (eit == fit->second.end()) continue;
			SBType derivedType = target_.FindFirstType(eit->second.c_str());
			if (!derivedType.IsValid()) continue;
			SBValue casted;
			if (t.IsPointerType()) {
				SBType derivedPtr = derivedType.GetPointerType();
				if (!derivedPtr.IsValid()) continue;
				casted = v.Cast(derivedPtr);
			} else {
				casted = v.Cast(derivedType);
			}
			// Cast 失敗時 (= invalid SBValue) は素の v に戻す。
			// 失敗で invalid を返すと emit 側で type/value/children が全部空になり
			// 「変数が何も届かない」状態になる。
			if (!casted.IsValid()) return v;
			return casted;
		}
		return v;
	}

private:
	lldb::SBDebugger debugger_;
	lldb::SBTarget   target_;
	lldb::SBProcess  process_;
	lldb::SBListener listener_;
	std::string exePath_;
	std::string exeBase_;
	bool firstExceptionConsumed_ = false;
	std::map<std::string, std::map<uint32_t, int>> bpIds_;
	// chain.txt 由来: field 名 (= "type" / "objtype") → enum 名 → クラス名。
	// applyChain で base class ポインタを派生型に dynamic 解釈するのに使う。
	std::map<std::string, std::map<std::string, std::string>> chainMap_;
	std::set<std::string> fieldNames_;   // chain.txt に登場する全 field 名

	static void dumpError(const char* label, const lldb::SBError& e) {
		fprintf(stderr, "[LLDB] %s 失敗\n", label);
		fprintf(stderr, "       success=%d  fail=%d  type=%d  err=%u\n",
			e.Success() ? 1 : 0, e.Fail() ? 1 : 0, (int)e.GetType(),
			(unsigned)e.GetError());
		const char* s = e.GetCString();
		fprintf(stderr, "       msg: %s\n", (s && *s) ? s : "(なし)");
	}

	bool handleStop() {
		using namespace lldb;
		SBThread thread = process_.GetSelectedThread();
		StopReason reason = thread.GetStopReason();

		if (reason == eStopReasonException && !firstExceptionConsumed_) {
			firstExceptionConsumed_ = true;
			fprintf(stderr, "[LLDB] (skip first-chance exception, silently continuing)\n");
			process_.Continue();
			return true;
		}

		dumpStopHeader(thread, reason);
		// 順序重要: backtrace を先に emit (= 親 UI が file/line で即 scroll/highlight)。
		// 変数 dump (= LLDB SBValue evaluate が重く 100ms+ かかることもある) を後回しに。
		dumpBacktrace(thread);
		dumpStopVars(thread);

		// pause を親に通知 → resume / step* / stop イベントのいずれかを直接待つ。
		// ポーリング sleep を排除し、SetEvent からほぼ即時 (= microsecond 単位) で起動。
		fprintf(stderr, "<<<PAUSE>>>\n"); fflush(stderr);
		fprintf(stderr, "[LLDB] paused. resume/stepIn/stepOver/stepOut/stop を待機.\n");
		HANDLE waits[5] = { evResume, evStop, evStepIn, evStepOver, evStepOut };
		DWORD wr = WaitForMultipleObjects(5, waits, FALSE, INFINITE);
		fprintf(stderr, "<<<RESUME>>>\n"); fflush(stderr);

		int idx = (int)(wr - WAIT_OBJECT_0);
		switch (idx) {
		case 0:   // evResume
			process_.Continue();
			break;
		case 1:   // evStop
			fprintf(stderr, "[LLDB] stop 要求 → kill\n");
			process_.Kill();
			return false;
		case 2:   // evStepIn
			fprintf(stderr, "[LLDB] StepInto\n");
			thread.StepInto();
			break;
		case 3:   // evStepOver
			fprintf(stderr, "[LLDB] StepOver\n");
			thread.StepOver();
			break;
		case 4:   // evStepOut
			fprintf(stderr, "[LLDB] StepOut\n");
			thread.StepOut();
			break;
		default:
			fprintf(stderr, "[LLDB] WaitForMultipleObjects 異常 (wr=%lu)、Continue\n", wr);
			process_.Continue();
			break;
		}
		return true;
	}

	static void dumpStopHeader(lldb::SBThread thread, lldb::StopReason reason) {
		using namespace lldb;
		SBFrame frame = thread.GetSelectedFrame();
		SBLineEntry le = frame.GetLineEntry();
		const char* func = frame.GetFunctionName();
		const char* file = le.GetFileSpec().GetFilename();
		const char* dir  = le.GetFileSpec().GetDirectory();
		uint32_t line    = le.GetLine();
		uint32_t col     = le.GetColumn();
		addr_t   pc      = frame.GetPC();

		const char* rname =
			(reason == eStopReasonInvalid)        ? "Invalid" :
			(reason == eStopReasonNone)           ? "None" :
			(reason == eStopReasonTrace)          ? "Trace" :
			(reason == eStopReasonBreakpoint)     ? "Breakpoint" :
			(reason == eStopReasonWatchpoint)     ? "Watchpoint" :
			(reason == eStopReasonSignal)         ? "Signal" :
			(reason == eStopReasonException)      ? "Exception" :
			(reason == eStopReasonExec)           ? "Exec" :
			(reason == eStopReasonPlanComplete)   ? "PlanComplete" :
			(reason == eStopReasonThreadExiting)  ? "ThreadExiting" :
			(reason == eStopReasonInstrumentation)? "Instrumentation" :
			(reason == eStopReasonProcessorTrace) ? "ProcessorTrace" :
			(reason == eStopReasonFork)           ? "Fork" :
			(reason == eStopReasonVFork)          ? "VFork" :
			(reason == eStopReasonVForkDone)      ? "VForkDone" :
			(reason == eStopReasonInterrupt)      ? "Interrupt" : "?";

		char dataStr[128] = { 0 };
		size_t cnt = thread.GetStopReasonDataCount();
		int off = 0;
		for (size_t i = 0; i < cnt && off < (int)sizeof(dataStr) - 32; ++i) {
			off += snprintf(dataStr + off, sizeof(dataStr) - off,
				"%s0x%llx", i ? "," : "",
				(unsigned long long)thread.GetStopReasonDataAtIndex(i));
		}
		char desc[512] = { 0 };
		thread.GetStopDescription(desc, sizeof(desc));

		fprintf(stderr,
			"[LLDB] STOP reason=%s(%d)  data=[%s]  desc=%s\n"
			"       at %s%s%s:%u:%u  in %s  pc=0x%llx\n",
			rname, (int)reason, dataStr, desc,
			dir ? dir : "", dir ? "\\" : "",
			file ? file : "?",
			line, col,
			func ? func : "?",
			(unsigned long long)pc);
	}

	// ============================================================
	// 既知型のカスタムフォーマット (= 値表示を「ポインタ番地」より意味のある文字列に)
	//
	// String / String*:  size + data を読んで "abc" 形式に整形。
	// List / List*:      size + esize から "[N items × Mb]" を表示
	//                    (要素表示は子要素として後で expand される)。
	// 戻り値が空でない場合は emit 側で sDisp を上書きする。
	// ============================================================
	static std::string formatCustomValue(lldb::SBValue v, lldb::SBProcess proc) {
		using namespace lldb;
		if (!v.IsValid()) return "";
		SBType t = v.GetType();
		if (!t.IsValid()) return "";
		const char* tn = t.GetName();
		if (!tn) return "";
		// "String", "String *", "const String", "struct String" ... を網羅したいので
		// "String" を含むかをまず見て、後で正確に判定する。
		bool isString = (std::strstr(tn, "String") != nullptr);
		bool isList   = (std::strstr(tn, "List")   != nullptr);
		if (!isString && !isList) return "";
		// pointer なら deref
		SBValue deref = t.IsPointerType() ? v.Dereference() : v;
		if (!deref.IsValid()) return "";
		// null チェック (= ポインタが 0)
		if (t.IsPointerType() && v.GetValueAsUnsigned() == 0) return "(null)";

		if (isString) {
			SBValue szV   = deref.GetChildMemberWithName("size");
			SBValue dataV = deref.GetChildMemberWithName("data");
			if (!szV.IsValid() || !dataV.IsValid()) return "";
			int64_t sz = szV.GetValueAsSigned();
			if (sz < 0) return "";
			if (sz > 256) sz = 256;   // 表示上限 (= 長すぎる文字列は trim)
			addr_t addr = dataV.GetValueAsUnsigned();
			if (addr == 0) return "\"\"";
			std::vector<char> buf((size_t)sz + 1);
			SBError err;
			size_t nr = proc.ReadMemory(addr, buf.data(), (size_t)sz, err);
			if (!err.Success()) return "";
			std::string out = "\"";
			// 印字不可文字は \xHH に。長すぎたら "..." を末尾に付ける。
			for (size_t i = 0; i < nr; ++i) {
				unsigned char c = (unsigned char)buf[i];
				if (c >= 0x20 && c < 0x7F && c != '"' && c != '\\') out.push_back((char)c);
				else if (c == '"')  out += "\\\"";
				else if (c == '\\') out += "\\\\";
				else if (c == '\n') out += "\\n";
				else if (c == '\r') out += "\\r";
				else if (c == '\t') out += "\\t";
				else { char hex[8]; std::snprintf(hex, sizeof(hex), "\\x%02X", c); out += hex; }
			}
			out += "\"";
			int64_t fullSz = szV.GetValueAsSigned();
			if (fullSz > sz) {
				char tail[32]; std::snprintf(tail, sizeof(tail), " ...(%lld)", (long long)fullSz);
				out += tail;
			}
			return out;
		}
		if (isList) {
			SBValue szV    = deref.GetChildMemberWithName("size");
			SBValue esizeV = deref.GetChildMemberWithName("esize");
			if (!szV.IsValid()) return "";
			int64_t sz = szV.GetValueAsSigned();
			int64_t es = esizeV.IsValid() ? esizeV.GetValueAsSigned() : 0;
			char out[64];
			if (es > 0) std::snprintf(out, sizeof(out), "[%lld items, esize=%lld]", (long long)sz, (long long)es);
			else        std::snprintf(out, sizeof(out), "[%lld items]", (long long)sz);
			return out;
		}
		return "";
	}

	// applyChain (= chain.txt 由来の動的 Cast) を呼ぶため非 static にした。
	void dumpStopVars(lldb::SBThread thread) {
		using namespace lldb;
		SBFrame frame = thread.GetSelectedFrame();
		SBValueList vars = frame.GetVariables(true, true, false, true);
		SBProcess proc = thread.GetProcess();
		// 制限値: depth ≤ kMaxDepth、子は <= kMaxNch。
		// kMaxDepth=0 = top level のみ。子はユーザが click したときに dumpExpansion で取得する。
		const int      kMaxDepth = 0;
		const uint32_t kMaxNch   = 64;
		// バッファに溜めて 1 回の fwrite に (= stderr 多数 syscall 回避)
		std::string buf;
		buf.reserve(8192);

		// ベースクラス subobject 判定: lldb は親 struct を expand すると、
		// 派生 struct の場合 child[0] (など) が **基底クラス subobject** で、
		// その name は基底クラスの型名 (例: "NewElement") になる慣習。
		// → child.name == child.type.name なら基底とみなして「行として emit せず」
		//   その中身を親と同 depth で flatten する。
		std::function<void(SBValue, int)> emitVar;
		// flatten 専用 (= chain.txt 再適用なし)。base subobject だけ素直に降りる。
		std::function<void(SBValue, int)> walkFlatten;
		walkFlatten = [&](SBValue ch, int depth) {
			if (!ch.IsValid()) return;
			const char* chName = ch.GetName();
			const char* chType = ch.GetType().IsValid() ? ch.GetType().GetName() : nullptr;
			if (chName && chType && std::strcmp(chName, chType) == 0
				&& ch.GetNumChildren() > 0)
			{
				uint32_t inner = ch.GetNumChildren();
				if (inner > kMaxNch) inner = kMaxNch;
				for (uint32_t j = 0; j < inner; ++j) {
					walkFlatten(ch.GetChildAtIndex(j), depth);
				}
				return;
			}
			emitVar(ch, depth);
		};
		std::function<void(SBValue, int)> emitChildren = [&](SBValue parent, int depth) {
			// chain.txt は **外側 1 回だけ** 適用。base subobject の flatten で再帰的に
			// applyChain を呼ぶと「派生型 → base → 同じ type field → 派生型 → ...」の
			// 無限ループに入るので必ずここだけ。
			SBValue effective = applyChain(parent);
			if (!effective.IsValid()) effective = parent;
			uint32_t nch = effective.GetNumChildren();
			if (nch > kMaxNch) nch = kMaxNch;
			for (uint32_t k = 0; k < nch; ++k) {
				walkFlatten(effective.GetChildAtIndex(k), depth);
			}
		};
		emitVar = [&](SBValue v, int depth) {
			if (!v.IsValid() || depth > kMaxDepth) return;
			// chain.txt: type/objtype フィールドが既知 enum なら派生型に Cast。
			// row 自体の type 列にも反映するので、ここで早期に適用する。
			// Cast 失敗時のため effective.IsValid() を二重チェック。
			SBValue effective = applyChain(v);
			if (!effective.IsValid()) effective = v;
			const char* nm  = v.GetName();
			const char* val = effective.GetValue();
			const char* sm  = effective.GetSummary();
			const char* tn  = effective.GetType().IsValid() ? effective.GetType().GetName() : nullptr;
			std::string sNm  = nm  ? nm  : "";
			std::string sTn  = tn  ? tn  : "";
			std::string sVal = val ? val : "";
			std::string sSm  = sm  ? sm  : "";
			std::string sDisp = !sSm.empty() ? sSm : sVal;
			// 既知型 (String*, List*) はカスタムフォーマットで意味のある値表示に。
			std::string sCust = formatCustomValue(effective, proc);
			if (!sCust.empty()) sDisp = sCust;
			// 「子がいるか」フラグ (= UI 側の "+" アイコン表示判定)。
			// MightHaveChildren は cheap (= 中身は見ない、型情報で判定)。
			bool hasChildren = effective.MightHaveChildren();
			// 診断ログ: 読み込み成功した変数を 1 行に
			fprintf(stderr, "[LLDB] var d=%d %s : %s = %s%s\n",
				depth, sNm.c_str(), sTn.c_str(), sDisp.c_str(),
				hasChildren ? "  [+]" : "");
			fflush(stderr);
			buf += "<<<VAR>>>";
			buf += std::to_string(depth);
			buf += '|';
			buf += sNm;
			buf += '|';
			buf += sTn;
			buf += '|';
			buf += sDisp;
			buf += '|';
			buf += hasChildren ? "1" : "0";
			buf += '|';
			buf += "0";   // nextOffset = 0 (= 普通の行)
			buf += '\n';
			if (depth < kMaxDepth) {
				emitChildren(v, depth + 1);
			}
		};
		uint32_t nvAll = vars.GetSize();
		uint32_t nv = std::min<uint32_t>(nvAll, kMaxNch);
		for (uint32_t i = 0; i < nv; ++i) {
			emitVar(vars.GetValueAtIndex(i), 0);
		}
		// 上限超え → 末尾に "..." (more) sentinel を出して UI 側で展開可能化。
		if (nvAll > kMaxNch) {
			buf += "<<<VAR>>>0|...|||1|";   // depth=0, name=..., type/value 空, expand=1
			buf += std::to_string(kMaxNch); // nextOffset = 64
			buf += '\n';
		}
		buf += "<<<ENDVARS>>>\n";
		// 1 回の fwrite で送信
		fwrite(buf.data(), 1, buf.size(), stderr);
		fflush(stderr);
	}

public:
	// ============================================================
	// 変数展開: path (= "tb\x1Fparent\x1Fsize" のような root → leaf 名連結) を辿って
	// 該当 SBValue の子要素を 1 段だけ dump。
	// 出力: <<<EXPANDVAR>>>path|depth|name|type|value\n ... <<<ENDEXPAND>>>path\n
	//
	// 親 UI 側は path で応答を識別し、対応する RowInfo の childs に流し込む。
	// 子要素についても base class subobject (= name == type.name) は flatten する。
	// ============================================================
	void dumpExpansion(const std::string& path, uint32_t offset) {
		using namespace lldb;
		const uint32_t kMaxNch = 64;
		// 現在停止中の frame (= run loop が停止状態でなければ何も返せない)
		SBThread thread = process_.GetSelectedThread();
		if (!thread.IsValid()) {
			fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
			return;
		}
		SBFrame frame = thread.GetSelectedFrame();
		if (!frame.IsValid()) {
			fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
			return;
		}
		// path を '\x1F' で分割。"..." セグメントは "more" sentinel なので skip
		// (= 同じ親の別バッチを指す印で、実 SBValue ノードではない)。
		std::vector<std::string> segs;
		{
			std::string seg;
			for (char ch : path) {
				if (ch == '\x1F') {
					if (!seg.empty() && seg != "...") segs.push_back(seg);
					seg.clear();
				}
				else seg.push_back(ch);
			}
			if (!seg.empty() && seg != "...") segs.push_back(seg);
		}
		if (segs.empty()) {
			fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
			return;
		}
		// 最初の segment は frame の variable
		SBValueList vars = frame.GetVariables(true, true, false, true);
		SBValue cur;
		uint32_t nv = vars.GetSize();
		for (uint32_t i = 0; i < nv; ++i) {
			SBValue v = vars.GetValueAtIndex(i);
			const char* nm = v.GetName();
			if (nm && segs[0] == nm) { cur = v; break; }
		}
		if (!cur.IsValid()) {
			fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
			return;
		}
		// 残り segments を辿る (= 子検索)。"[N]" は List index アクセスとして特別処理。
		auto findChild = [](SBValue parent, const std::string& name) -> SBValue {
			uint32_t nch = parent.GetNumChildren();
			for (uint32_t k = 0; k < nch; ++k) {
				SBValue ch = parent.GetChildAtIndex(k);
				const char* cn = ch.GetName();
				if (cn && name == cn) return ch;
				const char* tn = ch.GetType().IsValid() ? ch.GetType().GetName() : nullptr;
				if (cn && tn && std::strcmp(cn, tn) == 0) {
					SBValue v2 = ch.GetValueForExpressionPath(("." + name).c_str());
					if (v2.IsValid()) return v2;
				}
			}
			return SBValue();
		};
		// "[N]" 形式の segment かどうか判定 + N を取り出す。
		auto parseIndex = [](const std::string& s, int64_t& out) -> bool {
			if (s.size() < 3 || s.front() != '[' || s.back() != ']') return false;
			std::string num = s.substr(1, s.size() - 2);
			for (char c : num) if (c < '0' || c > '9') return false;
			out = std::stoll(num);
			return true;
		};
		// Map の key lookup: cur が Map / Map* で seg がそのキーなら、該当 KV を探して
		// value (= dtype 由来 ptrType) の SBValue を返す。
		// keyIsInt = true なら seg を decimal int として比較、false なら String content 比較。
		// row name に `(@0x...)` suffix が付いていれば strip して使う (= addr 添え書き対応)。
		auto mapLookup = [&](SBValue mapCur, const std::string& keyIn) -> SBValue {
			// suffix " (@0x...)" を strip
			std::string key = keyIn;
			size_t at = key.rfind("(@0x");
			if (at != std::string::npos && key.back() == ')') {
				// 直前 space も取る
				size_t cut = at;
				while (cut > 0 && key[cut - 1] == ' ') --cut;
				key.resize(cut);
			}
			SBType ct = mapCur.GetType();
			if (!ct.IsValid()) return SBValue();
			const char* tn = ct.GetName();
			if (!tn || !std::strstr(tn, "Map")) return SBValue();
			SBValue body = ct.IsPointerType() ? mapCur.Dereference() : mapCur;
			if (!body.IsValid()) return SBValue();
			SBValue kvsV   = body.GetChildMemberWithName("kvs");
			SBValue dtypeV = body.GetChildMemberWithName("dtype");
			SBValue kintV  = body.GetChildMemberWithName("keyIsInt");
			bool keyIsInt = (kintV.IsValid() && kintV.GetValueAsUnsigned() != 0);
			if (!kvsV.IsValid()) return SBValue();
			SBValue kvsBody = kvsV.GetType().IsPointerType() ? kvsV.Dereference() : kvsV;
			SBValue szV   = kvsBody.GetChildMemberWithName("size");
			SBValue dataV = kvsBody.GetChildMemberWithName("data");
			if (!szV.IsValid() || !dataV.IsValid()) return SBValue();
			int64_t sz = szV.GetValueAsSigned();
			lldb::addr_t kvsDataAddr = dataV.GetValueAsUnsigned();
			SBType ptrType;
			if (dtypeV.IsValid()) {
				const char* dtName = dtypeV.GetValue();
				if (dtName) {
					auto fit = chainMap_.find("dtype");
					if (fit != chainMap_.end()) {
						auto eit = fit->second.find(dtName);
						if (eit != fit->second.end()) {
							SBType base = target_.FindFirstType(eit->second.c_str());
							if (base.IsValid()) ptrType = base.GetPointerType();
						}
					}
				}
			}
			// 整数 key の場合は seg を decimal parse
			int64_t intKey = 0;
			bool intKeyValid = false;
			if (keyIsInt) {
				if (!key.empty()) {
					char* endp = nullptr;
					intKey = std::strtoll(key.c_str(), &endp, 10);
					intKeyValid = (endp && *endp == '\0');
				}
				if (!intKeyValid) return SBValue();
			}
			SBType kvType = target_.FindFirstType("KV");
			for (int64_t k = 0; k < sz; ++k) {
				lldb::addr_t kvPtrSlot = kvsDataAddr + k * 8;
				uint64_t kvAddr = 0;
				lldb::SBError err;
				process_.ReadMemory(kvPtrSlot, &kvAddr, 8, err);
				if (!err.Success() || kvAddr == 0) continue;
				if (!kvType.IsValid()) continue;
				lldb::SBAddress kvAddrObj((lldb::addr_t)kvAddr, target_);
				SBValue kvV = target_.CreateValueFromAddress("kv", kvAddrObj, kvType);
				if (!kvV.IsValid()) continue;
				SBValue keyV = kvV.GetChildMemberWithName("key");
				if (!keyV.IsValid()) continue;
				bool match = false;
				if (keyIsInt) {
					int64_t kvKeyInt = (int64_t)keyV.GetValueAsUnsigned();
					match = (kvKeyInt == intKey);
				} else {
					SBValue keyBody = keyV.GetType().IsPointerType() ? keyV.Dereference() : keyV;
					if (!keyBody.IsValid()) continue;
					SBValue ksz   = keyBody.GetChildMemberWithName("size");
					SBValue kdata = keyBody.GetChildMemberWithName("data");
					if (!ksz.IsValid() || !kdata.IsValid()) continue;
					int64_t kszVal = ksz.GetValueAsSigned();
					lldb::addr_t kdataAddr = kdata.GetValueAsUnsigned();
					if (kszVal != (int64_t)key.size()) continue;
					std::vector<char> kb((size_t)kszVal);
					lldb::SBError err2;
					process_.ReadMemory(kdataAddr, kb.data(), (size_t)kszVal, err2);
					if (!err2.Success()) continue;
					match = (std::memcmp(kb.data(), key.data(), (size_t)kszVal) == 0);
				}
				if (!match) continue;
				// match → value field の SBValue を返す
				SBValue valueV = kvV.GetChildMemberWithName("value");
				if (!valueV.IsValid()) return SBValue();
				lldb::SBAddress va = valueV.GetAddress();
				if (!va.IsValid()) return SBValue();
				if (!ptrType.IsValid()) return SBValue();
				return target_.CreateValueFromAddress(key.c_str(), va, ptrType);
			}
			return SBValue();
		};
		for (size_t i = 1; i < segs.size(); ++i) {
			int64_t idx = 0;
			if (parseIndex(segs[i], idx)) {
				// List index アクセス: cur が List/List* であることを期待。
				// data + idx * esize の addr に dtype 由来の型で SBValue を作る。
				SBType ct = cur.GetType();
				const char* ctn = ct.IsValid() ? ct.GetName() : nullptr;
				if (!ctn || !std::strstr(ctn, "List")) {
					fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
					return;
				}
				SBValue body = ct.IsPointerType() ? cur.Dereference() : cur;
				SBValue szV    = body.GetChildMemberWithName("size");
				SBValue esizeV = body.GetChildMemberWithName("esize");
				SBValue dataV  = body.GetChildMemberWithName("data");
				SBValue dtypeV = body.GetChildMemberWithName("dtype");
				if (!szV.IsValid() || !esizeV.IsValid() || !dataV.IsValid()) {
					fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
					return;
				}
				int64_t es = esizeV.GetValueAsSigned();
				lldb::addr_t dataAddr = dataV.GetValueAsUnsigned();
				lldb::addr_t elemAddr = dataAddr + (lldb::addr_t)(idx * es);
				// dtype → 要素型
				SBType ptrType;
				if (dtypeV.IsValid()) {
					const char* dtName = dtypeV.GetValue();
					if (dtName) {
						auto fit = chainMap_.find("dtype");
						if (fit != chainMap_.end()) {
							auto eit = fit->second.find(dtName);
							if (eit != fit->second.end()) {
								SBType base = target_.FindFirstType(eit->second.c_str());
								if (base.IsValid()) ptrType = base.GetPointerType();
							}
						}
					}
				}
				if (!ptrType.IsValid() || es != 8) {
					fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
					return;
				}
				lldb::SBAddress addrObj(elemAddr, target_);
				cur = target_.CreateValueFromAddress(segs[i].c_str(), addrObj, ptrType);
				if (!cur.IsValid()) {
					fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
					return;
				}
				continue;
			}
			// Map のキー lookup を先に試す (= cur が Map で seg がそのキーなら value SBValue)
			{
				SBValue mv = mapLookup(cur, segs[i]);
				if (mv.IsValid()) { cur = mv; continue; }
			}
			cur = findChild(cur, segs[i]);
			if (!cur.IsValid()) {
				fprintf(stderr, "<<<ENDEXPAND>>>%s\n", path.c_str()); fflush(stderr);
				return;
			}
		}
		// chain.txt 適用: cur が type フィールド付きで chain にマッチすれば派生型に Cast。
		// Cast 失敗時は cur をそのまま使う (= 子要素 0 になって応答が空になるのを防ぐ)。
		{
			SBValue casted = applyChain(cur);
			if (casted.IsValid()) cur = casted;
		}
		uint32_t totalNch = cur.GetNumChildren();
		SBProcess proc = process_;
		std::string buf;
		buf.reserve(8192);
		// 1 行を出力 (= 通常 child)。base subobject の flatten では使わない。
		// chain.txt 適用で派生型に Cast してから type/value/MightHaveChildren を取る。
		auto emitOne = [&](SBValue ch) {
			SBValue effective = applyChain(ch);
			if (!effective.IsValid()) effective = ch;
			const char* cn = ch.GetName();
			const char* tn = effective.GetType().IsValid() ? effective.GetType().GetName() : nullptr;
			const char* val = effective.GetValue();
			const char* sm  = effective.GetSummary();
			std::string sNm  = cn  ? cn  : "";
			std::string sTn  = tn  ? tn  : "";
			std::string sVal = val ? val : "";
			std::string sSm  = sm  ? sm  : "";
			std::string sDisp = !sSm.empty() ? sSm : sVal;
			std::string sCust = formatCustomValue(effective, proc);
			if (!sCust.empty()) sDisp = sCust;
			bool hch = effective.MightHaveChildren();
			// 診断ログ: 読み込み成功した変数 (= expand 応答)
			fprintf(stderr, "[LLDB] expand %s/%s : %s = %s%s\n",
				path.c_str(), sNm.c_str(), sTn.c_str(), sDisp.c_str(),
				hch ? "  [+]" : "");
			fflush(stderr);
			buf += "<<<EXPANDVAR>>>"; buf += path; buf += '|';
			buf += "0"; buf += '|';
			buf += sNm; buf += '|';
			buf += sTn; buf += '|';
			buf += sDisp; buf += '|';
			buf += hch ? "1" : "0"; buf += '|';
			buf += "0"; buf += '\n';   // nextOffset = 0
		};
		// base-class subobject (= name == type.name) は再帰的に中身展開、それ以外は emitOne。
		// 再帰版にしないと NewLocal → NewElement → NewEndElement の多段継承が
		// 1 段しか flatten されず、深い base の field が表示されないバグになる。
		std::function<void(SBValue)> walkOne;
		walkOne = [&](SBValue ch) {
			if (!ch.IsValid()) return;
			const char* cn = ch.GetName();
			const char* tn = ch.GetType().IsValid() ? ch.GetType().GetName() : nullptr;
			if (cn && tn && std::strcmp(cn, tn) == 0 && ch.GetNumChildren() > 0) {
				uint32_t inner = ch.GetNumChildren();
				if (inner > kMaxNch) inner = kMaxNch;
				for (uint32_t j = 0; j < inner; ++j) {
					walkOne(ch.GetChildAtIndex(j));   // ← 再帰
				}
				return;
			}
			emitOne(ch);
		};

		// === List* / List 検出: 通常 field の後に配列要素 [i] を表示 ===
		// まず data/size/capa/esize/dtype 等の struct field を walkOne で通常出力、
		// 続けて要素 [0]..[N-1] を CreateValueFromAddress で構築して walkOne に流す。
		// 要素の型は List::dtype field (= CType enum) を chain.txt の `dtype` 行で引く。
		// (esize=8 のポインタ配列前提。それ以外は fallback で raw 値表示。)
		bool isListMode = false;
		uint64_t listSz = 0, listEs = 0;
		lldb::addr_t listDataAddr = 0;
		SBType listPtrType;
		// === Map* / Map 検出: 通常 field の後に KV 配列を表示 ===
		// Map.kvs (= List<KV*>) を辿り、各 KV の key/value を読む。
		// value は Map.dtype に従って `<T>*` 型で interpret する。
		// key は Map.keyIsInt が true なら (intptr_t)integer、false なら String*。
		bool isMapMode = false;
		bool mapKeyIsInt = false;
		uint64_t mapSz = 0;
		lldb::addr_t mapKvsDataAddr = 0;
		SBType mapPtrType;
		{
			const char* tn = cur.GetType().IsValid() ? cur.GetType().GetName() : nullptr;
			if (tn && std::strstr(tn, "List")) {
				SBValue body = cur.GetType().IsPointerType() ? cur.Dereference() : cur;
				if (body.IsValid()) {
					SBValue szV    = body.GetChildMemberWithName("size");
					SBValue esizeV = body.GetChildMemberWithName("esize");
					SBValue dataV  = body.GetChildMemberWithName("data");
					SBValue dtypeV = body.GetChildMemberWithName("dtype");
					if (szV.IsValid() && esizeV.IsValid() && dataV.IsValid()) {
						isListMode = true;
						listSz       = (uint64_t)szV.GetValueAsSigned();
						listEs       = (uint64_t)esizeV.GetValueAsSigned();
						listDataAddr = dataV.GetValueAsUnsigned();
						if (dtypeV.IsValid()) {
							const char* dtName = dtypeV.GetValue();
							if (dtName) {
								auto fit = chainMap_.find("dtype");
								if (fit != chainMap_.end()) {
									auto eit = fit->second.find(dtName);
									if (eit != fit->second.end()) {
										SBType base = target_.FindFirstType(eit->second.c_str());
										if (base.IsValid()) listPtrType = base.GetPointerType();
									}
								}
							}
						}
					}
				}
			}
			else if (tn && std::strstr(tn, "Map")) {
				SBValue body = cur.GetType().IsPointerType() ? cur.Dereference() : cur;
				if (body.IsValid()) {
					SBValue kvsV     = body.GetChildMemberWithName("kvs");
					SBValue dtypeV   = body.GetChildMemberWithName("dtype");
					SBValue kintV    = body.GetChildMemberWithName("keyIsInt");
					if (kintV.IsValid()) mapKeyIsInt = (kintV.GetValueAsUnsigned() != 0);
					if (kvsV.IsValid()) {
						SBValue kvsBody = kvsV.GetType().IsPointerType() ? kvsV.Dereference() : kvsV;
						if (kvsBody.IsValid()) {
							SBValue szV   = kvsBody.GetChildMemberWithName("size");
							SBValue dataV = kvsBody.GetChildMemberWithName("data");
							if (szV.IsValid() && dataV.IsValid()) {
								isMapMode = true;
								mapSz          = (uint64_t)szV.GetValueAsSigned();
								mapKvsDataAddr = dataV.GetValueAsUnsigned();
							}
						}
					}
					if (dtypeV.IsValid()) {
						const char* dtName = dtypeV.GetValue();
						if (dtName) {
							auto fit = chainMap_.find("dtype");
							if (fit != chainMap_.end()) {
								auto eit = fit->second.find(dtName);
								if (eit != fit->second.end()) {
									SBType base = target_.FindFirstType(eit->second.c_str());
									if (base.IsValid()) mapPtrType = base.GetPointerType();
								}
							}
						}
					}
				}
			}
		}

		std::function<void(SBValue)> emitChildren = [&](SBValue parent) {
			uint32_t nch = parent.GetNumChildren();
			uint32_t end = std::min<uint32_t>(nch, offset + kMaxNch);
			for (uint32_t k = offset; k < end; ++k) {
				walkOne(parent.GetChildAtIndex(k));
			}
			// このバッチを超える子要素が残っていれば末尾に "..." sentinel を追加
			if (nch > offset + kMaxNch) {
				buf += "<<<EXPANDVAR>>>"; buf += path; buf += '|';
				buf += "0|...|||1|";
				buf += std::to_string(offset + kMaxNch);
				buf += '\n';
			}
		};
		(void)totalNch;
		emitChildren(cur);

		// === List モード: 通常 field の後に配列要素を追加 ===
		if (isListMode) {
			uint64_t end = std::min<uint64_t>(listSz, offset + kMaxNch);
			for (uint64_t k = offset; k < end; ++k) {
				lldb::addr_t elemAddr = listDataAddr + k * listEs;
				std::string nm = "[" + std::to_string(k) + "]";
				SBValue elemV;
				if (listPtrType.IsValid() && listEs == 8) {
					lldb::SBAddress addrObj(elemAddr, target_);
					elemV = target_.CreateValueFromAddress(nm.c_str(), addrObj, listPtrType);
				}
				if (elemV.IsValid()) {
					walkOne(elemV);
					continue;
				}
				// fallback: ptrType 解決失敗 / esize ≠ 8
				std::string val;
				if (listEs == 8) {
					uint64_t ptr = 0;
					lldb::SBError err;
					proc.ReadMemory(elemAddr, &ptr, 8, err);
					char tmp[32];
					std::snprintf(tmp, sizeof(tmp), "0x%016llx", (unsigned long long)ptr);
					val = err.Success() ? tmp : "?";
				} else {
					char tmp[64];
					std::snprintf(tmp, sizeof(tmp), "addr=0x%llx esize=%llu",
						(unsigned long long)elemAddr, (unsigned long long)listEs);
					val = tmp;
				}
				buf += "<<<EXPANDVAR>>>"; buf += path; buf += '|';
				buf += "0"; buf += '|';
				buf += nm; buf += '|';
				buf += "void*"; buf += '|';
				buf += val; buf += '|';
				buf += "0"; buf += '|';
				buf += "0"; buf += '\n';
			}
			if (listSz > end) {
				buf += "<<<EXPANDVAR>>>"; buf += path; buf += '|';
				buf += "0|...|||1|";
				buf += std::to_string(end);
				buf += '\n';
			}
		}

		// === Map モード: 通常 field の後に KV 配列を追加 ===
		// 各 KV: key (String*) と value (char*, dtype 由来の型で interpret)。
		// kvs の data は KV** (= 8 byte ポインタの配列)。
		if (isMapMode) {
			SBType kvType = target_.FindFirstType("KV");
			uint64_t end = std::min<uint64_t>(mapSz, offset + kMaxNch);
			for (uint64_t k = offset; k < end; ++k) {
				lldb::addr_t kvPtrSlot = mapKvsDataAddr + k * 8;
				uint64_t kvAddr = 0;
				lldb::SBError err;
				proc.ReadMemory(kvPtrSlot, &kvAddr, 8, err);
				if (!err.Success() || kvAddr == 0) continue;
				// KV の field に load (= type 解決済なら lldb 経由、ダメなら手で offset)
				std::string keyStr;
				lldb::addr_t valueFieldAddr = 0;
				if (kvType.IsValid()) {
					lldb::SBAddress kvAddrObj((lldb::addr_t)kvAddr, target_);
					SBValue kvV = target_.CreateValueFromAddress("kv", kvAddrObj, kvType);
					if (kvV.IsValid()) {
						SBValue keyV   = kvV.GetChildMemberWithName("key");
						SBValue valueV = kvV.GetChildMemberWithName("value");
						if (valueV.IsValid()) {
							lldb::SBAddress va = valueV.GetAddress();
							if (va.IsValid()) valueFieldAddr = va.GetLoadAddress(target_);
						}
						if (keyV.IsValid()) {
							if (mapKeyIsInt) {
								// 整数 key (= (intptr_t) 型でスロットに詰めてある)
								int64_t kint = (int64_t)keyV.GetValueAsUnsigned();
								keyStr = std::to_string(kint);
							} else {
								// String* key。raw 内容を **先頭 8 char まで**読む。
								// (= 巨大な String を key にされたら何 MB も読んでしまうので cap)。
								// path segment との一致のため formatCustomValue (= quote 付き) は使わない。
								// 末尾に `kv->key` の生 8-byte 値も `(@0x...)` で添える。
								// → keyIsInt が間違って false に立っているケース (= 実際は整数 key) で
								//   表示が無意味な場合でもユーザが原因に気付ける。
								SBValue keyBody = keyV.GetType().IsPointerType() ? keyV.Dereference() : keyV;
								if (keyBody.IsValid()) {
									SBValue szV2   = keyBody.GetChildMemberWithName("size");
									SBValue dataV2 = keyBody.GetChildMemberWithName("data");
									if (szV2.IsValid() && dataV2.IsValid()) {
										int64_t ksz = szV2.GetValueAsSigned();
										lldb::addr_t kaddr = dataV2.GetValueAsUnsigned();
										if (ksz > 0 && kaddr != 0) {
											int64_t readSz = ksz > 8 ? 8 : ksz;
											std::vector<char> kb((size_t)readSz);
											lldb::SBError err2;
											size_t nr2 = proc.ReadMemory(kaddr, kb.data(), (size_t)readSz, err2);
											if (err2.Success()) keyStr.assign(kb.data(), nr2);
										}
									}
								}
								// `kv->key` の slot 値 (= String* として保存されているアドレス)
								uint64_t keySlotVal = keyV.GetValueAsUnsigned();
								char addrTag[32];
								std::snprintf(addrTag, sizeof(addrTag), "(@0x%llx)",
									(unsigned long long)keySlotVal);
								keyStr += addrTag;
							}
						}
					}
				}
				if (valueFieldAddr == 0) {
					// kvType 解決失敗 → offset 手計算 (= n(int)+pad+key(8)+value(8): value は ~16)
					valueFieldAddr = (lldb::addr_t)kvAddr + 16;
				}
				if (keyStr.empty()) keyStr = "[" + std::to_string(k) + "]";
				SBValue elemV;
				if (mapPtrType.IsValid()) {
					lldb::SBAddress addrObj(valueFieldAddr, target_);
					elemV = target_.CreateValueFromAddress(keyStr.c_str(), addrObj, mapPtrType);
				}
				if (elemV.IsValid()) {
					walkOne(elemV);
					continue;
				}
				// fallback: dtype 解決失敗 → 生アドレス表示
				uint64_t valPtr = 0;
				proc.ReadMemory(valueFieldAddr, &valPtr, 8, err);
				char tmp[32];
				std::snprintf(tmp, sizeof(tmp), "0x%016llx", (unsigned long long)valPtr);
				buf += "<<<EXPANDVAR>>>"; buf += path; buf += '|';
				buf += "0"; buf += '|';
				buf += keyStr; buf += '|';
				buf += "void*"; buf += '|';
				buf += err.Success() ? tmp : "?"; buf += '|';
				buf += "0"; buf += '|';
				buf += "0"; buf += '\n';
			}
			if (mapSz > end) {
				buf += "<<<EXPANDVAR>>>"; buf += path; buf += '|';
				buf += "0|...|||1|";
				buf += std::to_string(end);
				buf += '\n';
			}
		}

		buf += "<<<ENDEXPAND>>>";
		buf += path;
		buf += '\n';
		fwrite(buf.data(), 1, buf.size(), stderr);
		fflush(stderr);
	}

	// process_ 公開アクセサ (= run loop 外から expand thread が呼ぶ用)
	lldb::SBProcess getProcess() const { return process_; }

private:
	static void dumpBacktrace(lldb::SBThread thread) {
		using namespace lldb;
		uint32_t nf = thread.GetNumFrames();
		if (nf > 64) nf = 64;
		// バッファに溜めて 1 回の fwrite で送信 (= 多数 syscall 回避)
		std::string buf;
		buf.reserve(4096);
		for (uint32_t i = 0; i < nf; ++i) {
			SBFrame f = thread.GetFrameAtIndex(i);
			if (!f.IsValid()) continue;
			SBLineEntry fle = f.GetLineEntry();
			const char* ffunc = f.GetFunctionName();
			const char* ffile = fle.GetFileSpec().GetFilename();
			uint32_t fline    = fle.GetLine();
			std::string sFunc = ffunc ? ffunc : "";
			std::string sFile = ffile ? ffile : "";
			char lineBuf[32];
			snprintf(lineBuf, sizeof(lineBuf), "%u", fline);
			std::string fline_s = (fline == UINT32_MAX) ? "" : lineBuf;
			buf += "<<<FRAME>>>";
			buf += sFunc;
			buf += '|';
			buf += sFile;
			buf += '|';
			buf += fline_s;
			buf += '\n';
		}
		buf += "<<<ENDFRAMES>>>\n";
		fwrite(buf.data(), 1, buf.size(), stderr);
		fflush(stderr);
	}
};

// expand shm の最大 path 長 (= shm 64KB - 4 bytes header)
constexpr uint32_t kLldbExpandShmMax = 64 * 1024 - 16;

// ============================================================
// 共有メモリの BP リスト読み出しヘルパ
// layout は lldb.h の writeBpShm と同じ。
// ============================================================
struct LldbBpEntry {
	std::string file;
	uint32_t    line;
	bool        enabled;
};
inline void ReadBpShm(const void* view, std::vector<LldbBpEntry>& out) {
	const uint8_t* p = (const uint8_t*)view;
	uint32_t count = 0;
	std::memcpy(&count, p, sizeof(count)); p += sizeof(count);
	out.clear(); out.reserve(count);
	for (uint32_t i = 0; i < count; ++i) {
		uint32_t fl = 0;
		std::memcpy(&fl, p, sizeof(fl)); p += sizeof(fl);
		LldbBpEntry e;
		e.file.assign((const char*)p, fl); p += fl;
		std::memcpy(&e.line, p, sizeof(e.line)); p += sizeof(e.line);
		uint8_t en = 0; std::memcpy(&en, p, sizeof(en)); p += sizeof(en);
		e.enabled = (en != 0);
		out.push_back(std::move(e));
	}
}

// ============================================================
// driver mode entry: main() の冒頭 (`--lldb-driver` 検出時) から呼ぶ。
// argv: --lldb-driver <exe> <bpFile> <bpLine>
//        <evResume> <evStop> <evBp> <shmName>
//        <evStepIn> <evStepOver> <evStepOut>
//        <evExpand> <expandShmName>
// ============================================================
inline int RunLldbDriverMode(int argc, char** argv) {
	if (argc < 14) {
		fprintf(stderr, "[DRIVER] usage: --lldb-driver <exe> <bpFile> <bpLine> "
			"<evResume> <evStop> <evBp> <shmName> "
			"<evStepIn> <evStepOver> <evStepOut> "
			"<evExpand> <expandShmName>\n");
		return 1;
	}
	const char* exePath         = argv[2];
	const char* breakpointFile  = argv[3];
	uint32_t    breakpointLine  = (uint32_t)atoi(argv[4]);
	const char* evResumeName    = argv[5];
	const char* evStopName      = argv[6];
	const char* evBpName        = argv[7];
	const char* shmName         = argv[8];
	const char* evStepInName    = argv[9];
	const char* evStepOverName  = argv[10];
	const char* evStepOutName   = argv[11];
	const char* evExpandName    = argv[12];
	const char* expandShmName   = argv[13];
	fprintf(stderr, "[DRIVER] enter  exe=%s  bp=%s:%u\n", exePath, breakpointFile, breakpointLine);
	fprintf(stderr, "[DRIVER] events: %s / %s / %s / %s / %s / %s\n",
		evResumeName, evStopName, evBpName,
		evStepInName, evStepOverName, evStepOutName);
	fflush(stderr);

	SetConsoleOutputCP(CP_UTF8);

	SetEnvironmentVariableA("PYTHONHOME",
		"C:\\Users\\hayah\\AppData\\Local\\Programs\\Python\\Python310");
	SetEnvironmentVariableA("PYTHONPATH",
		"C:\\Users\\hayah\\Downloads\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\"
		"clang+llvm-21.1.5-x86_64-pc-windows-msvc\\lib\\site-packages");
	SetEnvironmentVariableA("PYTHONDONTWRITEBYTECODE", "1");
	SetEnvironmentVariableA("PYTHONNOUSERSITE", "1");
	SetEnvironmentVariableA("PYTHONIOENCODING", "utf-8");
	SetEnvironmentVariableA("LLDB_DISABLE_CRASH_DIALOG", "1");

	HANDLE evResume   = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, evResumeName);
	HANDLE evStop     = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, evStopName);
	HANDLE evBpUpdate = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, evBpName);
	HANDLE evStepIn   = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, evStepInName);
	HANDLE evStepOver = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, evStepOverName);
	HANDLE evStepOut  = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, evStepOutName);
	HANDLE evExpand   = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, evExpandName);
	if (!evResume || !evStop || !evBpUpdate || !evStepIn || !evStepOver || !evStepOut || !evExpand) {
		fprintf(stderr, "[DRIVER] OpenEventA 失敗 GLE=%lu\n", GetLastError());
		return 1;
	}
	// LldbDriverApp に保存 → handleStop が直接 wait できる
	// (= 旧 listener thread + 50ms ポーリングを廃止)
	HANDLE bpShmHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, shmName);
	void*  bpShmView   = bpShmHandle
		? MapViewOfFile(bpShmHandle, FILE_MAP_ALL_ACCESS, 0, 0, 64 * 1024)
		: NULL;
	if (!bpShmView) {
		fprintf(stderr, "[DRIVER] OpenFileMapping/MapViewOfFile 失敗 GLE=%lu\n", GetLastError());
	}
	HANDLE expandShmHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, expandShmName);
	void*  expandShmView   = expandShmHandle
		? MapViewOfFile(expandShmHandle, FILE_MAP_ALL_ACCESS, 0, 0, 64 * 1024)
		: NULL;
	if (!expandShmView) {
		fprintf(stderr, "[DRIVER] expand OpenFileMapping/MapViewOfFile 失敗 GLE=%lu\n", GetLastError());
	}

	LldbDriverApp app;
	app.evResume   = evResume;
	app.evStop     = evStop;
	app.evStepIn   = evStepIn;
	app.evStepOver = evStepOver;
	app.evStepOut  = evStepOut;
	if (!app.initialize()) return 1;
	if (!app.createTarget(exePath)) {
		app.shutdown();
		return 1;
	}
	// chain.txt (= base ポインタ→派生型マッピング) のロード。
	// 候補 1: CWD ./chain.txt
	// 候補 2: driver exe (= argv[0]) のディレクトリ ./chain.txt
	// どちらも見つからなければ skip。
	{
		FILE* tf = std::fopen("chain.txt", "rb");
		if (tf) {
			std::fclose(tf);
			app.loadChainTxt("chain.txt");
		} else {
			char selfPath[MAX_PATH] = { 0 };
			GetModuleFileNameA(NULL, selfPath, sizeof(selfPath));
			char* lastSlash = strrchr(selfPath, '\\');
			if (lastSlash) {
				*(lastSlash + 1) = '\0';
				std::string p = std::string(selfPath) + "chain.txt";
				app.loadChainTxt(p);
			} else {
				app.loadChainTxt("chain.txt");
			}
		}
	}

	// デフォルト BP (= driver 引数で渡された file:line)
	if (breakpointFile && *breakpointFile) {
		app.setBreakpoint(breakpointFile, breakpointLine, true);
	}
	// 親から渡された初期 BP リスト
	if (bpShmView) {
		std::vector<LldbBpEntry> bps;
		ReadBpShm(bpShmView, bps);
		for (auto& bp : bps) app.setBreakpoint(bp.file, bp.line, bp.enabled);
	}

	// resume / stop 受信スレッド (= app.shouldResume_ / shouldStop_ を立てる)
	// BP 更新だけは別スレッド (= 親が SetEvent(evBpUpdate) → 共有メモリ再読込 →
	// app.setBreakpoint)。inferior 走行中でも paused 中でも届くため。
	// resume/stop/step* は handleStop が WaitForMultipleObjects で直接受ける。
	//
	// INFINITE wait (= 0 wakes/sec when idle、省エネ)。shutdown 時は evRun=false +
	// SetEvent(evBpUpdate) で wake → evRun チェックで抜ける。
	std::atomic<bool> evRun{ true };
	std::thread tBp([&]() {
		while (evRun.load()) {
			DWORD r = WaitForSingleObject(evBpUpdate, INFINITE);
			if (!evRun.load()) break;
			if (r != WAIT_OBJECT_0 || !bpShmView) continue;
			std::vector<LldbBpEntry> bps;
			ReadBpShm(bpShmView, bps);
			// 1. 親から消えた BP を driver からも消す (= 親 list は削除を「entry 不在」で表す)
			std::vector<std::pair<std::string, uint32_t>> nowEnabled;
			for (auto& bp : bps) if (bp.enabled) nowEnabled.emplace_back(bp.file, bp.line);
			app.syncBreakpoints(nowEnabled);
			// 2. 残りを通常通り適用 (= 既存は no-op、新規は追加)
			for (auto& bp : bps) app.setBreakpoint(bp.file, bp.line, bp.enabled);
		}
	});

	// 変数展開要求スレッド (= 親が SetEvent(evExpand) → expandShm から path 読み出し →
	// app.dumpExpansion(path) で 1 段ぶんの子変数を stderr に流す)。
	// inferior が停止中 (= paused) の時のみ意味を持つ。走行中なら frame.Get... が無効を返し
	// dumpExpansion は ENDEXPAND だけ流して空応答を返す。
	std::thread tExpand([&]() {
		while (evRun.load()) {
			DWORD r = WaitForSingleObject(evExpand, INFINITE);
			if (!evRun.load()) break;
			if (r != WAIT_OBJECT_0 || !expandShmView) continue;
			// expandShm 読み取り (= layout: pathLen uint32 + path bytes + offset uint32)
			const uint8_t* p = (const uint8_t*)expandShmView;
			uint32_t pathLen = 0;
			std::memcpy(&pathLen, p, sizeof(pathLen)); p += sizeof(pathLen);
			if (pathLen == 0 || pathLen > kLldbExpandShmMax) continue;
			std::string path((const char*)p, pathLen); p += pathLen;
			uint32_t offset = 0;
			std::memcpy(&offset, p, sizeof(offset));
			app.dumpExpansion(path, offset);
		}
	});

	if (app.launch()) {
		app.runEventLoop();
	}

	evRun = false;
	SetEvent(evBpUpdate);   // wake bp thread
	SetEvent(evExpand);     // wake expand thread
	tBp.join();
	tExpand.join();
	app.shutdown();

	if (bpShmView)       UnmapViewOfFile(bpShmView);
	if (bpShmHandle)     CloseHandle(bpShmHandle);
	if (expandShmView)   UnmapViewOfFile(expandShmView);
	if (expandShmHandle) CloseHandle(expandShmHandle);
	CloseHandle(evResume); CloseHandle(evStop); CloseHandle(evBpUpdate);
	CloseHandle(evStepIn); CloseHandle(evStepOver); CloseHandle(evStepOut);
	CloseHandle(evExpand);

	fprintf(stdout, "<<<EXIT>>>\n"); fflush(stdout);
	fprintf(stderr, "[DRIVER] exit\n"); fflush(stderr);
	return 0;
}
