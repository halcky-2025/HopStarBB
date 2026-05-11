#pragma once
// ============================================================
// lldb.h - LLDBClient (= UI 親プロセス側、driver 子プロセスと対話する受信/送信クライアント)
//
// 設計:
//   * GC 管理オブジェクト (= GC_alloc で確保、std STL 型は使わない)
//   * thgc->map に格納される (= キー "lldb" / "service:lldb" 等で取得)
//   * メソッドは自由関数で提供 (= struct はメンバ関数を持たない、純粋データ)
//   * 同期プリミティブは arr.h の AtomicInt / AtomicPtr / Mutex を使用
//   * 文字列は String*、リストは List*、map は Map* (= 全部 GC 管理)
//
// 立ち位置:
//   送信側 = driver 子プロセス (別 exe で `--lldb-driver` mode、SBDebugger 等を所有)
//   受信側 = ここ LLDBClient。UI スレッドから使う窓口。
//            * driver の stdout (`<<<VAR>>>`/`<<<FRAME>>>`/`<<<PAUSE>>>` 等) を
//              reader thread でパースして自分のメンバ List* に貯める
//            * BP リスト authoritative。setBreakpoint() で更新、稼働中なら IPC で driver に通知
//            * resume / stop は named event 発火で driver に伝わる
//
// このヘッダは liblldb (= SB API) を一切 include しない。SB API は lldbdriver.h 専用。
// ============================================================

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <thread>
#include <windows.h>

// ------------------------------------------------------------
// BpEntry: ブレークポイント 1 件 (= file:line + enabled)
// GC オブジェクト。CType::_BpEntry で登録。tracer は file (String*) のみ。
// ------------------------------------------------------------
struct BpEntry {
	String*  file;
	uint32_t line;
	int      enabled;   // 0 = disabled, 1 = enabled
};
inline void BpEntryCheck(ThreadGC* thgc, char* self) {
	BpEntry* b = (BpEntry*)self;
	GC_UPDATE_PTR_C(thgc, b->file, String*);
}

// ------------------------------------------------------------
// VarRow: BP HIT 時の変数 1 行 (= driver から `<<<VAR>>>depth|name|type|value`)
// ------------------------------------------------------------
struct VarRow {
	int      depth;
	String*  name;
	String*  type;
	String*  value;
	int      expand;        // 1 = 初期 expanded 状態 (= MightHaveChildren = 子要素を遅延ロード可能)
	uint32_t nextOffset;    // > 0 なら "more" sentinel 行 (= click 時にこの offset で次バッチ要求)
};
inline void VarRowCheck(ThreadGC* thgc, char* self) {
	VarRow* v = (VarRow*)self;
	GC_UPDATE_PTR_C(thgc, v->name,  String*);
	GC_UPDATE_PTR_C(thgc, v->type,  String*);
	GC_UPDATE_PTR_C(thgc, v->value, String*);
}

// ------------------------------------------------------------
// FrameRow: backtrace 1 行 (= driver から `<<<FRAME>>>func|file|line`)
// ------------------------------------------------------------
struct FrameRow {
	String* func;
	String* file;
	String* line;   // 文字列のまま保持 (= "?" 等が入る可能性があるため)
};
inline void FrameRowCheck(ThreadGC* thgc, char* self) {
	FrameRow* f = (FrameRow*)self;
	GC_UPDATE_PTR_C(thgc, f->func, String*);
	GC_UPDATE_PTR_C(thgc, f->file, String*);
	GC_UPDATE_PTR_C(thgc, f->line, String*);
}

// ------------------------------------------------------------
// LLDBClient: UI 側の状態 + IPC ハンドル一式。GC 管理。
//   thgc->map に "lldb" キーで保存される (= getLLDBClient で取得)。
// ------------------------------------------------------------
struct LLDBClient {
	// === 状態 atomic (= reader thread が更新、UI スレッドが読む) ===
	AtomicInt paused;        // 1 = BP HIT 中
	AtomicInt running;       // 1 = driver subprocess 稼働中
	AtomicInt varsReady;     // 1 = ENDVARS 受信済 → drain 可能
	AtomicInt framesReady;   // 1 = ENDFRAMES 受信済 → drain 可能
	AtomicInt expandReady;   // 1 = ENDEXPAND 受信済 → drain 可能 (= 1 件分)

	// === BP リスト (= UI authoritative) ===
	Mutex   bpsMutex;
	List*   breakpoints;     // List<BpEntry*>

	// === 受信バッファ (= driver の stdout から reader thread が貯める) ===
	Mutex   varsMutex;
	List*   pendingVars;     // List<VarRow*>
	Mutex   framesMutex;
	List*   pendingFrames;   // List<FrameRow*>
	// expand: 1 件のみ in-flight (= 連射防止は UI 側で onExpand 発火時 expandReady チェック)。
	Mutex   expandMutex;
	String* expandResultPath;  // どの path に対する応答か
	List*   expandResultRows;  // List<VarRow*> (= 子要素群、depth は親 row 直下に揃える)
	// 「ユーザが開いた path」の記憶 (= 各 BP HIT 後に自動的に再展開する)。
	// path String* を持つ List。重複追加は handler 側で防ぐ (= 線形検索で十分、要素数は数十)。
	List*   expandedPaths;     // List<String*>
	// 1 stop 中に処理待ちの path キュー。drain で 1 件ずつ送って ENDEXPAND を待つ。
	List*   expandRequestQueue; // List<String*>
	AtomicInt expandInFlight;  // 1 = 1 件送信済で応答待ち

	// === IPC ハンドル (= POD、GC trace 不要、ゼロ初期化で NULL) ===
	HANDLE evResume;
	HANDLE evStop;
	HANDLE evBpUpdate;
	HANDLE evStepIn;     // StepInto 要求 (= driver 側で thread.StepInto)
	HANDLE evStepOver;   // StepOver 要求 (= driver 側で thread.StepOver)
	HANDLE evStepOut;    // StepOut 要求 (= driver 側で thread.StepOut)
	HANDLE evExpand;     // 変数展開要求 (= driver 側で SBValue 子取得)
	HANDLE bpShmHandle;
	void*  bpShmView;
	HANDLE expandShmHandle;
	void*  expandShmView;
	HANDLE driverProcess;
	HANDLE driverJob;

	// === 停止行ハイライト用 (= 黄色塗り替え) ===
	// drainExtorchFrames が新しい停止行を受け取るたびに、
	//   (1) highlightCell があれば元の fillcolor に戻す
	//   (2) 新しい stop 行の content cell を yellow にして highlightCell に保存
	// driver 終了時に clear。
	struct ::NewElement* highlightCell;   // 黄色化中の cell (NULL = なし)
	uint32_t   highlightOldFill;          // 元の fillcolor (= 復元用)
	int        highlightHadBackground;    // 0=元々 bg なし、1=あり
};

// 共有メモリのサイズ (= 64KB)
constexpr size_t kLldbBpShmSize     = 64 * 1024;
constexpr size_t kLldbExpandShmSize = 64 * 1024;
// path 区切り (= 識別子に出ない ASCII Unit Separator)
constexpr char   kLldbPathSep = '\x1F';

// LLDBClient の GC tracer: List* メンバを更新
inline void LLDBClientCheck(ThreadGC* thgc, char* self) {
	LLDBClient* c = (LLDBClient*)self;
	GC_UPDATE_PTR_C(thgc, c->breakpoints,   List*);
	GC_UPDATE_PTR_C(thgc, c->pendingVars,   List*);
	GC_UPDATE_PTR_C(thgc, c->pendingFrames, List*);
	if (c->expandResultPath)    GC_UPDATE_PTR_C(thgc, c->expandResultPath,    String*);
	if (c->expandResultRows)    GC_UPDATE_PTR_C(thgc, c->expandResultRows,    List*);
	if (c->expandedPaths)       GC_UPDATE_PTR_C(thgc, c->expandedPaths,       List*);
	if (c->expandRequestQueue)  GC_UPDATE_PTR_C(thgc, c->expandRequestQueue,  List*);
	if (c->highlightCell) {
		GC_UPDATE_PTR_C(thgc, c->highlightCell, struct NewElement*);
	}
}

// LLDBClient の Finalizer: GC 回収時に driver subprocess を終了し、HANDLE / 共有メモリを解放。
//   driverJob は JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE 付きで作成されるため、
//   driverJob の HANDLE を閉じれば driver subprocess も自動的に kill される。
//   保険として TerminateProcess(driverProcess) も呼ぶ (= job 設定に失敗していたケース対応)。
inline void LLDBClientFinalize(ThreadGC* thgc, char* self) {
	LLDBClient* c = (LLDBClient*)self;
	(void)thgc;

	// 1) driver にまず evStop で穏やかに終了通知 (= reader thread が ENDDRIVER を流して脱出)
	if (c->evStop)         SetEvent(c->evStop);

	// 2) サブプロセスを確実に kill (= job HANDLE close でも kill されるが先に明示)
	if (c->driverProcess && c->driverProcess != INVALID_HANDLE_VALUE) {
		TerminateProcess(c->driverProcess, 0);
	}

	// 3) Job Object を閉じる (= KILL_ON_JOB_CLOSE フラグで未 kill のプロセスも kill)
	if (c->driverJob)      { CloseHandle(c->driverJob);      c->driverJob = NULL; }
	if (c->driverProcess && c->driverProcess != INVALID_HANDLE_VALUE) {
		CloseHandle(c->driverProcess);
		c->driverProcess = NULL;
	}

	// 4) 共有メモリ (= view を unmap してから handle を close)
	if (c->bpShmView)      { UnmapViewOfFile(c->bpShmView);      c->bpShmView = NULL; }
	if (c->bpShmHandle)    { CloseHandle(c->bpShmHandle);        c->bpShmHandle = NULL; }
	if (c->expandShmView)  { UnmapViewOfFile(c->expandShmView);  c->expandShmView = NULL; }
	if (c->expandShmHandle){ CloseHandle(c->expandShmHandle);    c->expandShmHandle = NULL; }

	// 5) named event HANDLE を順次 close
	if (c->evResume)       { CloseHandle(c->evResume);       c->evResume = NULL; }
	if (c->evStop)         { CloseHandle(c->evStop);         c->evStop = NULL; }
	if (c->evBpUpdate)     { CloseHandle(c->evBpUpdate);     c->evBpUpdate = NULL; }
	if (c->evStepIn)       { CloseHandle(c->evStepIn);       c->evStepIn = NULL; }
	if (c->evStepOver)     { CloseHandle(c->evStepOver);     c->evStepOver = NULL; }
	if (c->evStepOut)      { CloseHandle(c->evStepOut);      c->evStepOut = NULL; }
	if (c->evExpand)       { CloseHandle(c->evExpand);       c->evExpand = NULL; }

	// running フラグも下ろす (= reader thread 側の goto 脱出条件)
	atomic_store(&c->running, 0);
}

// ------------------------------------------------------------
// LLDBClient の生成 / thgc->map への登録
//   キーは "debug" 固定。同じ thgc で複数 LLDB セッションは想定しない。
//   既存エントリが LLDBClient 型でない場合は新規 alloc + 上書き登録 (= type 衝突対策)。
//   SetRichText / OpenFile 時点で先行呼び出しすれば、UI スレッドのどこからでも
//   `getLLDBClient(thgc)` で確実に LLDBClient が取れる状態を作れる。
// ------------------------------------------------------------
inline String* lldbClientKey(ThreadGC* thgc) {
	static const char kKey[] = "debug";
	return createString(thgc, (char*)kKey, (int)(sizeof(kKey) - 1), 1);
}

inline LLDBClient* getLLDBClient(ThreadGC* thgc) {
	String* key = lldbClientKey(thgc);
	LLDBClient* c = (LLDBClient*)get_mapy(thgc->map, key);
	// NULL or 別型なら新規作成 (= GC_typeOf で型チェック)
	if (c && GC_typeOf(c) == _LLDBClient) { return c; }

	c = (LLDBClient*)GC_alloc(thgc, _LLDBClient);
	// ゼロ初期化済 (= AtomicInt v=0, Mutex state=0, HANDLE=NULL)。List* は明示作成。
	c->breakpoints        = create_list(thgc, sizeof(BpEntry*),  CType::_BpEntry);
	c->pendingVars        = create_list(thgc, sizeof(VarRow*),   CType::_VarRow);
	c->pendingFrames      = create_list(thgc, sizeof(FrameRow*), CType::_FrameRow);
	c->expandedPaths      = create_list(thgc, sizeof(String*),   CType::_String);
	c->expandRequestQueue = create_list(thgc, sizeof(String*),   CType::_String);
	add_mapy(thgc, thgc->map, key, (char*)c);
	return c;
}

// 後方で実装される expand 要求関数の前方宣言
inline bool lldb_requestExpand(LLDBClient* c, const char* path, int pathLen, uint32_t offset);

// ------------------------------------------------------------
// expandedPaths にユニークに追加 (= 重複線形チェック)。
// ------------------------------------------------------------
inline void lldb_rememberExpandPath(ThreadGC* thgc, LLDBClient* c, String* path) {
	if (!path || path->size <= 0) return;
	for (int i = 0; i < c->expandedPaths->size; i++) {
		String* p = *(String**)get_list(c->expandedPaths, i);
		if (p && p->size == path->size && memcmp(p->data, path->data, p->size) == 0) return;
	}
	add_list(thgc, c->expandedPaths, (char*)path);
}

// ------------------------------------------------------------
// expandRequestQueue から 1 件取り出して driver に送る (= 既に in-flight なら no-op)。
// 送信失敗 (= driver 未起動 / shm 無効) は inFlight を立てずに諦める。
// ------------------------------------------------------------
inline void lldb_pumpExpandQueue(LLDBClient* c) {
	if (atomic_load(&c->expandInFlight) != 0) return;
	if (!c->expandRequestQueue || c->expandRequestQueue->size <= 0) return;
	String* path = *(String**)get_list(c->expandRequestQueue, 0);
	// 先頭を pop (= swap with last + size--)
	int last = c->expandRequestQueue->size - 1;
	if (last > 0) {
		String* lastP = *(String**)get_list(c->expandRequestQueue, last);
		((String**)c->expandRequestQueue->data)[0] = lastP;
	}
	c->expandRequestQueue->size--;
	if (!path || path->size <= 0) return;
	// queue 上のリクエストは offset=0 (= replay 用、初回バッチ)。
	// "more" バッチは UI 側で onExpand が直接 lldb_requestExpand(path, offset) を呼ぶ。
	if (lldb_requestExpand(c, path->data, path->size, 0)) {
		atomic_store(&c->expandInFlight, 1);
	}
}

// ------------------------------------------------------------
// expandRequestQueue を expandedPaths で再構築 (= 各 BP HIT 後に呼ぶ)。
// ------------------------------------------------------------
inline void lldb_replayExpansions(ThreadGC* thgc, LLDBClient* c) {
	if (!c->expandedPaths || !c->expandRequestQueue) return;
	c->expandRequestQueue->size = 0;
	for (int i = 0; i < c->expandedPaths->size; i++) {
		String* p = *(String**)get_list(c->expandedPaths, i);
		if (p) add_list(thgc, c->expandRequestQueue, (char*)p);
	}
}

// ------------------------------------------------------------
// 状態 query (= UI ボタンの enabled 判定等から呼ぶ)
// ------------------------------------------------------------
inline bool lldb_isPaused (LLDBClient* c) { return atomic_load(&c->paused)  != 0; }
inline bool lldb_isRunning(LLDBClient* c) { return atomic_load(&c->running) != 0; }

// ------------------------------------------------------------
// resume / stop / step (= driver に named event 発火)
//   step は driver 側の event スレッドが受信して stepMode_ を立てつつ
//   shouldResume_ も立てる → handleStop の wait ループから抜けて stepMode_ で
//   分岐ディスパッチ (StepInto / StepOver / StepOut)。
// ------------------------------------------------------------
inline void lldb_resume  (LLDBClient* c) { if (c->evResume)   SetEvent(c->evResume); }
inline void lldb_stop    (LLDBClient* c) { if (c->evStop)     SetEvent(c->evStop); }
inline void lldb_stepIn  (LLDBClient* c) { if (c->evStepIn)   SetEvent(c->evStepIn); }
inline void lldb_stepOver(LLDBClient* c) { if (c->evStepOver) SetEvent(c->evStepOver); }
inline void lldb_stepOut (LLDBClient* c) { if (c->evStepOut)  SetEvent(c->evStepOut); }

// ------------------------------------------------------------
// 共有メモリへの BP リスト書き込み
// layout: count (uint32) + { fileLen(uint32) + file(bytes) + line(uint32) + enabled(uint8) }*
// ------------------------------------------------------------
inline void lldb_writeBpShm(LLDBClient* c) {
	if (!c->bpShmView) return;
	MutexGuard g(&c->bpsMutex);
	uint8_t* p = (uint8_t*)c->bpShmView;
	uint32_t count = (uint32_t)c->breakpoints->size;
	std::memcpy(p, &count, sizeof(count)); p += sizeof(count);
	for (int i = 0; i < c->breakpoints->size; i++) {
		BpEntry* bp = *(BpEntry**)get_list(c->breakpoints, i);
		uint32_t fl = bp->file ? (uint32_t)bp->file->size : 0;
		std::memcpy(p, &fl, sizeof(fl)); p += sizeof(fl);
		if (fl > 0) { std::memcpy(p, bp->file->data, fl); p += fl; }
		std::memcpy(p, &bp->line, sizeof(bp->line)); p += sizeof(bp->line);
		uint8_t en = (uint8_t)(bp->enabled ? 1 : 0);
		std::memcpy(p, &en, sizeof(en)); p += sizeof(en);
	}
}

inline void lldb_pushBreakpointsToDriver(LLDBClient* c) {
	if (!c->bpShmView || !c->evBpUpdate) return;
	lldb_writeBpShm(c);
	SetEvent(c->evBpUpdate);
}

// ------------------------------------------------------------
// 変数展開要求: path + offset を expandShm に書いて evExpand 発火。
//   path = "tb\x1Fparent\x1Fsize" のような root → leaf 名連結。
//   "..." セグメントは driver 側で skip される (= more sentinel の印)。
//   offset > 0 で「more バッチ」要求 (= 子要素の [offset, offset+kMaxNch))。
//   layout: pathLen (uint32) + path bytes (no null term) + offset (uint32)
// ------------------------------------------------------------
inline bool lldb_requestExpand(LLDBClient* c, const char* path, int pathLen, uint32_t offset) {
	if (!c->expandShmView || !c->evExpand || !path || pathLen <= 0) return false;
	if ((size_t)(sizeof(uint32_t) + pathLen + sizeof(uint32_t)) > kLldbExpandShmSize) return false;
	uint8_t* p = (uint8_t*)c->expandShmView;
	uint32_t len = (uint32_t)pathLen;
	std::memcpy(p, &len, sizeof(len)); p += sizeof(len);
	std::memcpy(p, path, pathLen); p += pathLen;
	std::memcpy(p, &offset, sizeof(offset));
	SetEvent(c->evExpand);
	return true;
}

// ------------------------------------------------------------
// BP の追加 / 削除
//   親側 BP リストに反映 + 走行中なら共有メモリに書き直して g_evBpUpdate を発火。
// ------------------------------------------------------------
inline void lldb_setBreakpoint(ThreadGC* thgc, LLDBClient* c,
	const char* file, uint32_t line, bool enabled)
{
	if (!file) return;
	int fileLen = (int)strlen(file);
	{
		MutexGuard g(&c->bpsMutex);
		// 既存項目を file+line で検索
		int foundIdx = -1;
		for (int i = 0; i < c->breakpoints->size; i++) {
			BpEntry* bp = *(BpEntry**)get_list(c->breakpoints, i);
			if (!bp || !bp->file) continue;
			if ((int)bp->file->size == fileLen &&
				bp->line == line &&
				memcmp(bp->file->data, file, fileLen) == 0) {
				foundIdx = i;
				break;
			}
		}
		if (enabled) {
			if (foundIdx < 0) {
				BpEntry* bp = (BpEntry*)GC_alloc(thgc, _BpEntry);
				bp->file = createString(thgc, (char*)file, fileLen, 1);
				bp->line = line;
				bp->enabled = 1;
				add_list(thgc, c->breakpoints, (char*)bp);
			} else {
				BpEntry* bp = *(BpEntry**)get_list(c->breakpoints, foundIdx);
				bp->enabled = 1;
			}
		} else {
			if (foundIdx >= 0) {
				// 末尾と swap して pop (= 順序保たない簡易削除)
				int last = c->breakpoints->size - 1;
				if (foundIdx != last) {
					BpEntry* lastBp = *(BpEntry**)get_list(c->breakpoints, last);
					((BpEntry**)c->breakpoints->data)[foundIdx] = lastBp;
				}
				c->breakpoints->size--;
			}
		}
	}
	if (lldb_isRunning(c)) lldb_pushBreakpointsToDriver(c);
}

// ------------------------------------------------------------
// driver stdout reader thread のラインハンドラ
// ------------------------------------------------------------
inline void lldb_handleLine(ThreadGC* thgc, LLDBClient* c, const char* line, int len) {
	auto sw = [&](const char* prefix, int plen) -> bool {
		return len >= plen && memcmp(line, prefix, plen) == 0;
	};
	if (len == 11 && memcmp(line, "<<<PAUSE>>>", 11) == 0) {
		atomic_store(&c->paused, 1);
	}
	else if (len == 12 && memcmp(line, "<<<RESUME>>>", 12) == 0) {
		atomic_store(&c->paused, 0);
	}
	else if (len == 10 && memcmp(line, "<<<EXIT>>>", 10) == 0) {
		atomic_store(&c->running, 0);
	}
	else if (sw("<<<VAR>>>", 9)) {
		// body = "depth|name|type|value|expand|nextOffset"
		const char* body = line + 9;
		int blen = len - 9;
		int sep[5] = {-1,-1,-1,-1,-1};
		int found = 0;
		for (int i = 0; i < blen && found < 5; i++) {
			if (body[i] == '|') sep[found++] = i;
		}
		if (found < 5) {
			fprintf(stderr, "[UI] VAR parse 失敗: '|' 区切り %d 個 (期待 5)、line=%.*s\n",
				found, len, line);
			fflush(stderr);
			return;
		}
		VarRow* vr = (VarRow*)GC_alloc(thgc, _VarRow);
		char buf16[16];
		int dl = sep[0]; if (dl > 15) dl = 15;
		memcpy(buf16, body, dl); buf16[dl] = '\0';
		vr->depth = atoi(buf16);
		vr->name  = createString(thgc, (char*)(body + sep[0] + 1),
			sep[1] - sep[0] - 1, 1);
		vr->type  = createString(thgc, (char*)(body + sep[1] + 1),
			sep[2] - sep[1] - 1, 1);
		vr->value = createString(thgc, (char*)(body + sep[2] + 1),
			sep[3] - sep[2] - 1, 1);
		vr->expand = (sep[3] + 1 < blen && body[sep[3] + 1] == '1') ? 1 : 0;
		// nextOffset (= sep[4]+1 .. blen)
		int ol = blen - sep[4] - 1; if (ol > 15) ol = 15; if (ol < 0) ol = 0;
		memcpy(buf16, body + sep[4] + 1, ol); buf16[ol] = '\0';
		vr->nextOffset = (uint32_t)strtoul(buf16, NULL, 10);
		fprintf(stderr, "[UI] VAR ok d=%d name=%.*s\n",
			vr->depth, vr->name ? vr->name->size : 0,
			vr->name ? vr->name->data : "");
		fflush(stderr);
		{
			MutexGuard g(&c->varsMutex);
			add_list(thgc, c->pendingVars, (char*)vr);
		}
	}
	else if (len == 13 && memcmp(line, "<<<ENDVARS>>>", 13) == 0) {
		fprintf(stderr, "[UI] ENDVARS 受信、pendingVars=%d\n",
			c->pendingVars ? c->pendingVars->size : -1);
		fflush(stderr);
		atomic_store(&c->varsReady, 1);
	}
	else if (sw("<<<FRAME>>>", 11)) {
		// body = "func|file|line"
		const char* body = line + 11;
		int blen = len - 11;
		int sep[2] = {-1,-1};
		int found = 0;
		for (int i = 0; i < blen && found < 2; i++) {
			if (body[i] == '|') sep[found++] = i;
		}
		if (found < 2) return;
		FrameRow* fr = (FrameRow*)GC_alloc(thgc, _FrameRow);
		fr->func = createString(thgc, (char*)body,                  sep[0], 1);
		fr->file = createString(thgc, (char*)(body + sep[0] + 1),   sep[1] - sep[0] - 1, 1);
		fr->line = createString(thgc, (char*)(body + sep[1] + 1),   blen   - sep[1] - 1, 1);
		{
			MutexGuard g(&c->framesMutex);
			add_list(thgc, c->pendingFrames, (char*)fr);
		}
	}
	else if (len == 15 && memcmp(line, "<<<ENDFRAMES>>>", 15) == 0) {
		atomic_store(&c->framesReady, 1);
	}
	else if (sw("<<<EXPANDVAR>>>", 15)) {
		// body = "path|depth|name|type|value|expand|nextOffset"
		const char* body = line + 15;
		int blen = len - 15;
		int sep[6] = { -1,-1,-1,-1,-1,-1 };
		int found = 0;
		for (int i = 0; i < blen && found < 6; i++) {
			if (body[i] == '|') sep[found++] = i;
		}
		if (found < 6) return;
		// path = body[0..sep[0]]
		String* pathS = createString(thgc, (char*)body, sep[0], 1);
		VarRow* vr = (VarRow*)GC_alloc(thgc, _VarRow);
		char buf16[16];
		int dl = sep[1] - sep[0] - 1; if (dl > 15) dl = 15; if (dl < 0) dl = 0;
		memcpy(buf16, body + sep[0] + 1, dl); buf16[dl] = '\0';
		vr->depth = atoi(buf16);
		vr->name  = createString(thgc, (char*)(body + sep[1] + 1),
			sep[2] - sep[1] - 1, 1);
		vr->type  = createString(thgc, (char*)(body + sep[2] + 1),
			sep[3] - sep[2] - 1, 1);
		vr->value = createString(thgc, (char*)(body + sep[3] + 1),
			sep[4] - sep[3] - 1, 1);
		vr->expand = (sep[4] + 1 < blen && body[sep[4] + 1] == '1') ? 1 : 0;
		int ol = blen - sep[5] - 1; if (ol > 15) ol = 15; if (ol < 0) ol = 0;
		memcpy(buf16, body + sep[5] + 1, ol); buf16[ol] = '\0';
		vr->nextOffset = (uint32_t)strtoul(buf16, NULL, 10);
		{
			MutexGuard g(&c->expandMutex);
			// path 単位で List を貯める。連続 EXPANDVAR は同じ path のはず。
			// 異なる path に切り替わったら前回ぶんを破棄 (= 古い応答は無効)。
			if (!c->expandResultPath
				|| c->expandResultPath->size != pathS->size
				|| memcmp(c->expandResultPath->data, pathS->data, pathS->size) != 0)
			{
				c->expandResultPath = pathS;
				c->expandResultRows = create_list(thgc, sizeof(VarRow*), CType::_VarRow);
			}
			add_list(thgc, c->expandResultRows, (char*)vr);
		}
	}
	else if (sw("<<<ENDEXPAND>>>", 15)) {
		// body = path
		const char* body = line + 15;
		int blen = len - 15;
		String* pathS = createString(thgc, (char*)body, blen, 1);
		{
			MutexGuard g(&c->expandMutex);
			// expandResultPath が一致する場合のみ ready 化 (= path 不一致 = stale 応答は捨てる)
			if (c->expandResultPath
				&& c->expandResultPath->size == pathS->size
				&& memcmp(c->expandResultPath->data, pathS->data, pathS->size) == 0)
			{
				atomic_store(&c->expandReady, 1);
			}
		}
		// in-flight は応答到着で解除 (= drainer が結果を取り込んでから次を pump する)
		atomic_store(&c->expandInFlight, 0);
	}
	else {
		// それ以外 = stderr に流す (= driver からの diagnostic)
		fwrite(line, 1, len, stderr);
		fputc('\n', stderr);
		fflush(stderr);
	}
	// 13 / 13 の補正: ENDVARS は実は 13 文字
	(void)sw;
}

// ------------------------------------------------------------
// driver stdout reader thread 本体
// ------------------------------------------------------------
inline void lldb_readerLoop(ThreadGC* thgc, LLDBClient* c, HANDLE childStdoutRd) {
	// std::string を使わない簡易バッファ (= GC 管理にする必然性は低いので OS heap)
	const int kBufCap = 65536;
	char* buf = (char*)malloc(kBufCap);
	int   bufLen = 0;
	char readBuf[4096];
	while (true) {
		DWORD n = 0;
		if (!ReadFile(childStdoutRd, readBuf, sizeof(readBuf), &n, NULL) || n == 0) break;
		// append
		if (bufLen + (int)n > kBufCap) {
			// 異常に長い行 = drop
			bufLen = 0;
			continue;
		}
		memcpy(buf + bufLen, readBuf, n);
		bufLen += (int)n;
		// '\n' 区切りで処理
		while (true) {
			int nl = -1;
			for (int i = 0; i < bufLen; i++) {
				if (buf[i] == '\n') { nl = i; break; }
			}
			if (nl < 0) break;
			int lineLen = nl;
			if (lineLen > 0 && buf[lineLen - 1] == '\r') lineLen--;
			lldb_handleLine(thgc, c, buf, lineLen);
			// 残りを前に詰める
			int remain = bufLen - nl - 1;
			if (remain > 0) memmove(buf, buf + nl + 1, remain);
			bufLen = remain;
		}
	}
	free(buf);
	CloseHandle(childStdoutRd);
	atomic_store(&c->running, 0);
	atomic_store(&c->paused,  0);
}

// ------------------------------------------------------------
// セッション開始 (= driver subprocess を spawn して reader thread を立てる)
// ------------------------------------------------------------
inline void lldb_startDebug(ThreadGC* thgc, LLDBClient* c,
	const char* exePath,
	const char* breakpointFile,
	uint32_t    breakpointLine)
{
	fprintf(stderr, "[LLDB] startDebug ENTER  exe=%s  bp=%s:%u\n",
		exePath ? exePath : "(NULL)",
		breakpointFile ? breakpointFile : "(NULL)",
		breakpointLine); fflush(stderr);
	int prev = atomic_exchange(&c->running, 1);
	if (prev != 0) {
		fprintf(stderr, "[LLDB] 既にデバッグ中\n");
		return;
	}
	atomic_store(&c->paused, 0);

	char selfPath[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(NULL, selfPath, sizeof(selfPath)) == 0) {
		fprintf(stderr, "[LLDB] GetModuleFileNameA 失敗 GLE=%lu\n", GetLastError());
		atomic_store(&c->running, 0);
		return;
	}

	HANDLE childStdoutRd = NULL, childStdoutWr = NULL;
	SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
	if (!CreatePipe(&childStdoutRd, &childStdoutWr, &sa, 0)) {
		fprintf(stderr, "[LLDB] CreatePipe 失敗 GLE=%lu\n", GetLastError());
		atomic_store(&c->running, 0);
		return;
	}
	SetHandleInformation(childStdoutRd, HANDLE_FLAG_INHERIT, 0);
	HANDLE nulRead = CreateFileA("NUL", GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
		OPEN_EXISTING, 0, NULL);
	if (nulRead == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "[LLDB] CreateFile(NUL) 失敗 GLE=%lu\n", GetLastError());
		CloseHandle(childStdoutRd); CloseHandle(childStdoutWr);
		atomic_store(&c->running, 0);
		return;
	}

	DWORD parentPid = GetCurrentProcessId();
	char evResumeName[64], evStopName[64], evBpName[64], shmName[64];
	char evStepInName[64], evStepOverName[64], evStepOutName[64];
	char evExpandName[64], expandShmName[64];
	snprintf(evResumeName,   sizeof(evResumeName),   "HopStarBB_LLDBResume_%lu",   parentPid);
	snprintf(evStopName,     sizeof(evStopName),     "HopStarBB_LLDBStop_%lu",     parentPid);
	snprintf(evBpName,       sizeof(evBpName),       "HopStarBB_LLDBBpUpdate_%lu", parentPid);
	snprintf(evStepInName,   sizeof(evStepInName),   "HopStarBB_LLDBStepIn_%lu",   parentPid);
	snprintf(evStepOverName, sizeof(evStepOverName), "HopStarBB_LLDBStepOver_%lu", parentPid);
	snprintf(evStepOutName,  sizeof(evStepOutName),  "HopStarBB_LLDBStepOut_%lu",  parentPid);
	snprintf(shmName,        sizeof(shmName),        "HopStarBB_LLDBBpShm_%lu",    parentPid);
	snprintf(evExpandName,   sizeof(evExpandName),   "HopStarBB_LLDBExpand_%lu",   parentPid);
	snprintf(expandShmName,  sizeof(expandShmName),  "HopStarBB_LLDBExpandShm_%lu", parentPid);

	c->evResume   = CreateEventA(NULL, FALSE, FALSE, evResumeName);
	c->evStop     = CreateEventA(NULL, FALSE, FALSE, evStopName);
	c->evBpUpdate = CreateEventA(NULL, FALSE, FALSE, evBpName);
	c->evStepIn   = CreateEventA(NULL, FALSE, FALSE, evStepInName);
	c->evStepOver = CreateEventA(NULL, FALSE, FALSE, evStepOverName);
	c->evStepOut  = CreateEventA(NULL, FALSE, FALSE, evStepOutName);
	c->evExpand   = CreateEventA(NULL, FALSE, FALSE, evExpandName);
	if (!c->evResume || !c->evStop || !c->evBpUpdate ||
	    !c->evStepIn || !c->evStepOver || !c->evStepOut || !c->evExpand) {
		fprintf(stderr, "[LLDB] CreateEventA 失敗 GLE=%lu\n", GetLastError());
		CloseHandle(childStdoutRd); CloseHandle(childStdoutWr); CloseHandle(nulRead);
		atomic_store(&c->running, 0);
		return;
	}

	c->bpShmHandle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE, 0, (DWORD)kLldbBpShmSize, shmName);
	if (!c->bpShmHandle) {
		fprintf(stderr, "[LLDB] CreateFileMappingA 失敗 GLE=%lu\n", GetLastError());
		CloseHandle(childStdoutRd); CloseHandle(childStdoutWr); CloseHandle(nulRead);
		atomic_store(&c->running, 0);
		return;
	}
	c->bpShmView = MapViewOfFile(c->bpShmHandle, FILE_MAP_ALL_ACCESS, 0, 0, kLldbBpShmSize);
	if (!c->bpShmView) {
		fprintf(stderr, "[LLDB] MapViewOfFile 失敗 GLE=%lu\n", GetLastError());
		CloseHandle(c->bpShmHandle); c->bpShmHandle = NULL;
		CloseHandle(childStdoutRd); CloseHandle(childStdoutWr); CloseHandle(nulRead);
		atomic_store(&c->running, 0);
		return;
	}
	// 既存 BP リストを初期 payload として書き込み
	lldb_writeBpShm(c);

	c->expandShmHandle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE, 0, (DWORD)kLldbExpandShmSize, expandShmName);
	if (c->expandShmHandle) {
		c->expandShmView = MapViewOfFile(c->expandShmHandle, FILE_MAP_ALL_ACCESS, 0, 0, kLldbExpandShmSize);
	}
	if (!c->expandShmView) {
		fprintf(stderr, "[LLDB] expand CreateFileMappingA/MapViewOfFile 失敗 GLE=%lu\n", GetLastError());
		// expand が無くても本体デバッグは続行可能なので fatal にしない
	}

	char cmdLine[2048];
	snprintf(cmdLine, sizeof(cmdLine),
		"\"%s\" --lldb-driver \"%s\" \"%s\" %u %s %s %s %s %s %s %s %s %s",
		selfPath, exePath ? exePath : "", breakpointFile ? breakpointFile : "",
		breakpointLine, evResumeName, evStopName, evBpName, shmName,
		evStepInName, evStepOverName, evStepOutName,
		evExpandName, expandShmName);
	fprintf(stderr, "[LLDB] spawning driver: %s\n", cmdLine); fflush(stderr);

	STARTUPINFOA si = { sizeof(si) };
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = childStdoutWr;
	si.hStdError  = childStdoutWr;
	si.hStdInput  = nulRead;
	PROCESS_INFORMATION pi = { 0 };
	BOOL ok = CreateProcessA(NULL, cmdLine, NULL, NULL,
		/*bInheritHandles=*/TRUE,
		CREATE_BREAKAWAY_FROM_JOB | DETACHED_PROCESS,
		NULL, NULL, &si, &pi);
	CloseHandle(childStdoutWr);
	CloseHandle(nulRead);

	if (!ok) {
		fprintf(stderr, "[LLDB] CreateProcessA 失敗 GLE=%lu\n", GetLastError());
		CloseHandle(childStdoutRd);
		atomic_store(&c->running, 0);
		return;
	}
	fprintf(stderr, "[LLDB] driver pid=%lu\n", pi.dwProcessId);
	c->driverProcess = pi.hProcess;
	CloseHandle(pi.hThread);

	if (!c->driverJob) c->driverJob = CreateJobObjectA(NULL, NULL);
	if (c->driverJob) {
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION info = {};
		info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
		SetInformationJobObject(c->driverJob, JobObjectExtendedLimitInformation,
			&info, sizeof(info));
		if (!AssignProcessToJobObject(c->driverJob, pi.hProcess)) {
			fprintf(stderr, "[LLDB] AssignProcessToJobObject 失敗 GLE=%lu\n", GetLastError());
		}
	}

	std::thread([thgc, c, childStdoutRd]() {
		lldb_readerLoop(thgc, c, childStdoutRd);
	}).detach();
}

// ------------------------------------------------------------
// build ヘルパ (= UI 側ビルドコマンド + デバッグ起動)
// ------------------------------------------------------------
inline bool lldb_buildIRDll(const char* irName) {
	char cmd[1024];
	snprintf(cmd, sizeof(cmd),
		"cd /d clang3 && clang++ -std=c++20 -g -gdwarf-4 -O0 -fuse-ld=lld -Wl,/DEBUG:DWARF -shared "
		"\"%s.ll\" -o \"%s.dll\" -Wl,/IMPLIB:%s",
		irName, irName, irName);
	fprintf(stderr, "[IR ] %s\n", cmd);
	int rc = system(cmd);
	if (rc != 0) {
		fprintf(stderr, "[IR ] DLL ビルド失敗 (exit=%d)\n", rc);
		return false;
	}
	char src[512], dst[512];
	snprintf(src, sizeof(src), "clang3\\%s.dll", irName);
	snprintf(dst, sizeof(dst), "%s.dll", irName);
	if (!CopyFileA(src, dst, FALSE)) {
		fprintf(stderr, "[IR ] DLL コピー失敗 (%s → %s, GLE=%lu)\n",
			src, dst, GetLastError());
		return false;
	}
	fprintf(stderr, "[IR ] OK: %s\n", dst);
	return true;
}

inline bool lldb_buildMainExe() {
	DeleteFileA("main.exe");
	DeleteFileA("main.pdb");
	DeleteFileA("main.exp");
	DeleteFileA("main.lib");
	fprintf(stderr, "[CC ] 古い main.* を削除済み、clang++ を起動します。\n");
	static const char* kCmd =
		"clang3\\clang++ main.cpp sqlite3.o -g -gdwarf-4 -std=c++20 -O0 -fuse-ld=lld "
		"-Wl,/DEBUG:DWARF "
		"-D_DEBUG -D_ITERATOR_DEBUG_LEVEL=2 -D_DLL -D_MT "
		"-Xclang --dependent-lib=msvcrtd "
		"-DBX_CONFIG_DEBUG=1 "
		"-Wno-ignored-attributes -Wno-tautological-compare -Wno-format "
		"-Wno-invalid-source-encoding -Wno-parentheses "
		"-resource-dir C:\\Users\\hayah\\Downloads\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\lib\\clang\\21 "
		"-D__PRFCHWINTRIN_H=1 "
		"-D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS "
		"-I C:\\C2\\include "
		"-I C:\\Users\\hayah\\Downloads\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\include "
		"C:\\Users\\hayah\\Downloads\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\clang+llvm-21.1.5-x86_64-pc-windows-msvc\\lib\\liblldb.lib "
		"-I C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\include "
		"-I C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\include\\torch\\csrc\\api\\include "
		"C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\torch.lib "
		"C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\torch_cpu.lib "
		"C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\torch_cuda.lib "
		"C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\c10.lib "
		"C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\c10_cuda.lib "
		"C:\\C2\\onnxruntime.lib C:\\C2\\onnxruntime_providers_cuda.lib "
		"C:\\C2\\onnxruntime_providers_shared.lib C:\\C2\\onnxruntime_providers_tensorrt.lib "
		"C:\\C2\\avcodec.lib C:\\C2\\avdevice.lib C:\\C2\\avfilter.lib C:\\C2\\avformat.lib "
		"C:\\C2\\avutil.lib C:\\C2\\postproc.lib C:\\C2\\swresample.lib C:\\C2\\swscale.lib "
		"C:\\C2\\SDL3.lib C:\\C2\\SDL3_ttf.lib C:\\C2\\utf8proc_static.lib C:\\C2\\libcoro.lib "
		"C:\\C2\\bgfxDebug.lib C:\\C2\\bimg_decodeDebug.lib C:\\C2\\bxDebug.lib "
		"C:\\C2\\bimgDebug.lib C:\\C2\\portaudio_x64.lib "
		"-lmsvcrtd.lib -lucrtd.lib -lvcruntimed.lib "
		"-lgdi32.lib -luser32.lib -lkernel32.lib -lopengl32.lib "
		"-ld3d11.lib -ldxgi.lib -ldxguid.lib -ld3dcompiler.lib "
		"-lShell32 -lShlwapi -march=native -o main.exe";
	fprintf(stderr, "[CC ] %s\n", kCmd);
	int rc = system(kCmd);
	if (rc != 0) {
		fprintf(stderr, "[CC ] main.exe ビルド失敗 (exit=%d)\n", rc);
		return false;
	}
	fprintf(stderr, "[CC ] OK: main.exe\n");

	struct DllCopy { const char* src; const char* dst; };
	const DllCopy dlls[] = {
		{ "clang3\\liblldb.dll", "liblldb.dll" },
		{ "C:\\Users\\hayah\\AppData\\Local\\Programs\\Python\\Python310\\python310.dll",
		  "python310.dll" },
	};
	for (const auto& d : dlls) {
		if (!CopyFileA(d.src, d.dst, FALSE)) {
			DWORD gle = GetLastError();
			if (gle != ERROR_SHARING_VIOLATION) {
				fprintf(stderr, "[CC ] %s コピー失敗 GLE=%lu\n", d.dst, gle);
			}
		}
	}
	return true;
}

// breakpointFile / breakpointLine はデフォルト無効 (= NULL / 0)。
// 旧版は main.cpp:218 を強制 BP していたが、ユーザ BP (= ガター click → state DB
// → 共有メモリ経由) に任せるため廃止。明示的に渡された場合のみ追加 BP として使う。
inline bool lldb_compileAndDebug(ThreadGC* thgc, LLDBClient* c,
	const char* irName,
	const char* breakpointFile = NULL,
	uint32_t    breakpointLine = 0)
{
	if (irName && !lldb_buildIRDll(irName)) return false;
	if (!lldb_buildMainExe()) return false;

	char mainExeAbs[MAX_PATH] = { 0 };
	DWORD n = GetFullPathNameA("main.exe", (DWORD)sizeof(mainExeAbs), mainExeAbs, NULL);
	if (n == 0 || n >= sizeof(mainExeAbs)) {
		fprintf(stderr, "[LLDB] GetFullPathNameA(main.exe) 失敗 GLE=%lu\n", GetLastError());
		return false;
	}
	fprintf(stderr, "[LLDB] inferior = %s\n", mainExeAbs);
	lldb_startDebug(thgc, c, mainExeAbs, breakpointFile, breakpointLine);
	return true;
}
