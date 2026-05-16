#if defined(__ANDROID__) || defined(__APPLE__) || defined(__linux__)
#include <coroutine>
namespace coro = std;
#else
#include <coro/coro.hpp>
#endif

#include <SDL3/SDL_log.h>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <functional>
// =============================================================================
// 定数
// =============================================================================
constexpr int MAX_CLASSES = 256;
constexpr int MAX_ROOTS_PER_NODE = 64;
constexpr int PROMOTION_THRESHOLD = 3;
struct ThreadGC;
struct NewLocal;
class Transaction;
struct RootNode {
    ThreadGC* gc;
    int rn;
    Transaction* tx;
    char*** roots;
    RootNode* next;
    RootNode* prev;
    int count;
    int capacity;
};
struct ClassType;
class CoroutineQueue;
struct Frame;
struct CoroutineQueue;
// Generator の定義
class Generator {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    Generator(handle_type h) : coro(h) {}
    Generator(const Generator&) = delete;
    Generator(Generator&& other) noexcept : coro(other.coro) {
        other.coro = nullptr;
    }
    ~Generator() {
        if (coro) coro.destroy();
    }

    // ジェネレーターが await 可能になるために必要な 3つ
    bool await_ready() const noexcept { return false; }

    std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) {
        coro.promise().parent = h;  // 親のハンドルを保存
        coro.promise().queue = h.promise().queue;
        return coro;  // 子のコルーチンを実行
    }

    char* await_resume() noexcept {
        return coro.promise().current_value;
    }

    bool next() {
        if (!coro.done()) coro.resume();
        return !coro.done();
    }

    char* value() const {
        return coro.promise().current_value;
    }

    handle_type coro;

public:
    struct promise_type {
        void (*func)(Frame*);
        RootNode* root;
        CoroutineQueue* queue = nullptr;
        char* blockobj;
        std::coroutine_handle<Generator::promise_type> parent;
        // ★ state は必ず 0 で開始すること: -1 は final_suspend 完了マーカー (resume_all
        //   が skip 対象とする) のため、デフォルト未初期化だと前回 freed されたフレーム
        //   メモリが同アドレスで再アロケートされた時に「いきなり state==-1 で開始」
        //   する可能性がある。実際 Android で coroutine frame allocator が同アドレスを
        //   返してきて 2 個目以降の inner coroutine が resume されない bug を踏んだ。
        int state = 0;
        char* current_value = nullptr;
        char* ret_value = nullptr;
        int statefin = -1;
		std::suspend_always yield_value(char* val);

        void return_value(char* val) {
            current_value = val;
            if (parent != NULL) parent.promise().current_value = val;
        }

        std::suspend_always initial_suspend() { return {}; }

        // final_suspendの実装は後で定義（CoroutineQueueが完全型になってから）
        auto final_suspend() noexcept;

        void unhandled_exception() {
            auto eptr = std::current_exception();
            try {
                std::rethrow_exception(eptr); // 再スロー
            }
            catch (const std::exception& e) {
                std::cerr << "Exception caught in unhandled_exception: " << e.what() << "\n";
            }
            catch (...) {
                std::cerr << "Unknown exception caught in unhandled_exception\n";
            }
            current_value = 0;
            state = -5;
        }

        Generator get_return_object() {
            return Generator{ handle_type::from_promise(*this) };
        }
    };
};

// CoroutineQueue の完全定義
class CoroutineQueue {
public:
    std::queue<std::coroutine_handle<Generator::promise_type>> q;
    std::mutex m;

    void push(std::coroutine_handle<Generator::promise_type> h) {
        std::lock_guard lock(m);
        h.promise().queue = this;
        q.push(h);
    }

    void resume_all() {
        std::queue<std::coroutine_handle<Generator::promise_type>> local;
        {
            std::lock_guard lock(m);
            std::swap(local, q);
        }
        while (!local.empty()) {
            auto h = local.front();
            local.pop();
            if (h.promise().state == -1) {
                // 完了済みのコルーチンはスキップ
            }
            else {
                h.resume();
            }
        }
    }
    bool empty() {
        std::lock_guard lock(m);
        return q.empty();
    }
};

inline std::suspend_always Generator::promise_type::yield_value(char* val) {
    current_value = val;
    if (queue) queue->push(std::coroutine_handle<promise_type>::from_promise(*this));
    return {};
}

struct Frame {
    void (*resume)(Frame*);
    void (*destory)(Frame*);
    Generator::promise_type promise;
};
void ResumeFrame(Frame* frame) {
    frame->promise.func(frame);
}
void DestroyFrame(Frame* frame) {
    free(frame);
}

Frame* MakeFrame(RootNode* root, void (*func)(Frame*), char* blockobj) {
    Frame* frame = (Frame*)malloc(sizeof(Frame));
    frame->resume = ResumeFrame;
    frame->destory = DestroyFrame;
    frame->promise.func = func;
    frame->promise.root = root;
    frame->promise.blockobj = blockobj;
    frame->promise.state = 0;
    frame->promise.parent = NULL;
    frame->promise.current_value = nullptr;
    frame->promise.ret_value = nullptr;
    frame->promise.statefin = -1;
    return frame;
}
struct Obj;
struct FuncType {
    char* blk;
    char* (*drawcall)(RootNode*, char*);
	char* (*call)(RootNode*, char*);
    char* obj;
    Obj* cls;
    void* draws;
};
// FuncTypeRCheck は GC_clone が定義された後 (Compile.h 末尾付近) で定義する。
// final_suspendの実装（CoroutineQueueが完全型になってから）
inline auto Generator::promise_type::final_suspend() noexcept {
    struct awaiter {
        CoroutineQueue* q;
        std::coroutine_handle<Generator::promise_type> parent;

        bool await_ready() noexcept { return false; }        // ← 最終停止で“止まる”
        void await_suspend(std::coroutine_handle<>) const noexcept {
            if (q && parent) q->push(parent);                // ← 親をキューへ
        }
        void await_resume() noexcept {}
    };

    state = -1;
    return awaiter{ queue, parent };
}
class RenderCommandQueue; 
RenderCommandQueue* createRCQ();
class HopStar;
class SqlPool;
struct NativeWindow;
struct Map;
struct List;
struct String;
CoroutineQueue* queuefirst(ThreadGC* thgc);
void push_to_queue(CoroutineQueue* queue, char* raw) {
    queue->push(std::coroutine_handle<Generator::promise_type>::from_address(static_cast<void*>(raw)));  // グローバル or TLS な CoroutineQueue を使う
}
char* wait_handle(ThreadGC* thgc, char* raw) {
    auto h2 = std::coroutine_handle<Generator::promise_type>::from_address(static_cast<void*>(raw));
	auto queue = new CoroutineQueue();
	queue->push(h2);  // グローバル or TLS な CoroutineQueue を使う
    h2.promise().queue = queue;
    for (;;) {
        if (queuefirst(thgc) == NULL) break;
        else queuefirst(thgc)->resume_all();
    }
    for (;;) {
        std::queue<std::coroutine_handle<Generator::promise_type>> local;

        {
            std::lock_guard lock(queue->m);
            std::swap(local, queue->q);  // 🔄 ロック中に中身を移す
        }

        while (!local.empty()) {
            auto h = local.front();
            local.pop();
            h.resume();
            if (h == h2 && h.promise().state == -1) {
                return  h.promise().ret_value;
            }
        }
    }
}
enum CType {
	_Struct,
	_List, _String, _ElementC, _FRect, _LetterC, _TextBoxC, _ButtonC, _Detection, _LocalC, _PopupC, _Select, _State, _LineC, _VLineC, _ImageC, _TabC, _DropC, _DownC, _Background, _SIValues, _SIVal, _TabTitleC, _TabPageC, _LinkedC, _SideletC,
    _MapData, _Map, _KV, _EndC, _CloneElemC, _HoppyWindow, _ColumnMeta, _Table, _Column, _DivC, _SheetC, _TableCell, _NewTable, _ColumnInfo, _RowInfo,
    _MemTable, _MouseEvent, _KeyEvent, _MemFunc, _MemObj, _FuncTypeR, _Offscreen, _OffscreenEnd,
    _ATSSpan, _StyleSpan, _RenderSpan, _HoveredSpan, _BreakLetterC,
    _CObj, _Block, _CallBlock, _TagBlock,
    _Primary, _COperator, _Word, _CNumber,
    _FloatVal, _StrObj, _Address, _Comment,
    _Comment2, _HtmObj, _CMountain, _CQuestion,
    _CDolor, _CClones, _CAnimationFunction, _PrimOp, _SingleOp, _Label,
    _Master, _Operator,
    _VariClass, _ArrType, _FuncTypeC, _Variable, _Function, _GenericFunction,
    _ClassObj, _ModelObj, _GeneObj, _GenericObj, _ObjBlock, _GenericType, _Base,
    _CVariable, _CBlock,
    _CClass, _CVar, _CIf, _CIfBlock, _CElif, _CElse, _CWhile, _CFor, _CSwitch,
    _CReturn, _CGoto, _CContinue, _CSignal, _CPrint, _CModel, _CGene,
    _CGeneLabel, _CBoolVal, _CServerClient,
    _SB, _LComp, _LVari, _LStrV, _LLab, _LIfValue, _LPhi, _LTypeDec, _LTypeVal,
    _LFunc, _LGete, _LCast, _LCall, _LAlloca, _LStore, _LLoad, _LBr,
    _LSwitchComp, _LBinOp, _LCmp, _LLocationMarker, _LRet,
    _LDebugInfo, _LDebugVariable, _LStructField, _LStructTypeInfo, _LLLVM,
    _GeneChild, _GeneMutate, _GeneNew, _GeneCross, _GeneStore, _GeneSelect, _GeneSort,
    _Stock,
    _Constructor, _ArrayConstructor, _SqlString,
    _CVal, _CValue, _CIterator,
    _CommentLet,
    _CommentType,
    _CElemObj,
    _CConnectStock,
    _Migrate, _OneMigrate,
    // === LLDB クライアント (= UI 親側、driver と対話) ===
    _LLDBClient, _BpEntry, _VarRow, _FrameRow
};
// =============================================================================
// クラス定義
// =============================================================================
using GCCheckFunc = void (*)(ThreadGC*, char*);
using GCFinalizeFunc = void (*)(ThreadGC*, char*);
struct ClassType {
    String* name;
    uint32_t size;
    GCCheckFunc gc_check;       // 子オブジェクトのスキャン
    GCFinalizeFunc gc_finalize; // ファイナライザ
    bool has_io;                // I/Oリソースを持つか（trueならOld直行）
};
ThreadGC* magc = NULL;
// gc.hpp
// 世代別GC - リアルタイムアプリケーション向け
// Young (Eden + Survivor0 + Survivor1) + Old + Finalizer Queue





// =============================================================================
// オブジェクトヘッダ
// =============================================================================
enum class Generation : uint8_t {
    Young = 0,
    Old = 1
};

struct Object {
    uint32_t type;              // クラスID (65536以上はサイズ埋め込み)
    uint8_t survival_count;     // 生存回数（昇格判定用）
    uint8_t generation;         // 0=Young, 1=Old
    uint8_t forwarded;          // GC世代カウンタと一致なら転送済み
    uint8_t flags;              // 予約
    char* fwd;                  // 転送先ポインタ
};

static_assert(sizeof(Object) == 16, "Object header should be 16 bytes");

// GC オブジェクトの型 (= CType) を取得。NULL なら 0 を返す。
// 値ポインタ (= GC_alloc が返したアドレス) から sizeof(Object) 戻った位置に
// Object ヘッダがあり、obj->type が CType。65536 以上は GC_alloc_size の生バッファ
// (= サイズ埋め込み) で、CType ではないので呼び出し側でチェック必要。
inline uint32_t GC_typeOf(void* value) {
    if (!value) return 0;
    Object* obj = (Object*)((char*)value - sizeof(Object));
    return obj->type;
}







// =============================================================================
// Finalizerキュー（非同期実行用）
// =============================================================================
class FinalizerQueue {
public:
    struct FinalizerTask {
        GCFinalizeFunc func;
        char* data;             // オブジェクトデータのコピー
        uint32_t size;
    };

    FinalizerQueue() : running_(true) {
        worker_ = std::thread([this]() { worker_loop(); });
    }

    ~FinalizerQueue() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            running_ = false;
        }
        cv_.notify_one();
        if (worker_.joinable()) {
            worker_.join();
        }
    }

    void push(GCFinalizeFunc func, char* data, uint32_t size) {
        // データをコピー（元オブジェクトはGCで消える可能性）
        char* copy = (char*)malloc(size);
        memcpy(copy, data, size);

        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push({ func, copy, size });
        }
        cv_.notify_one();
    }

    void flush() {
        // 全Finalizerを同期実行（シャットダウン時用）
        std::unique_lock<std::mutex> lock(mutex_);
        while (!queue_.empty()) {
            auto task = queue_.front();
            queue_.pop();
            lock.unlock();

            task.func(nullptr, task.data);
            free(task.data);

            lock.lock();
        }
    }

private:
    void worker_loop() {
        while (true) {
            FinalizerTask task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [this]() { return !queue_.empty() || !running_; });

                if (!running_ && queue_.empty()) return;

                task = queue_.front();
                queue_.pop();
            }

            // GC外で実行（I/O可能）
            task.func(nullptr, task.data);
            free(task.data);
        }
    }

    std::queue<FinalizerTask> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread worker_;
    bool running_;
};

// =============================================================================
// Young世代ヒープ（Eden + Survivor0 + Survivor1）
// =============================================================================
// =============================================================================
// 大容量 alloc 閾値: これ以上のサイズは Eden を経由せず Old 直行。
//   - young GC で copy されないので高速 (= 大バッファの memcpy 回避)
//   - Eden / spill を圧迫しない (= 通常 alloc が小さくて済む)
//   - 適用例: 大文字列 data buffer、画像 bitmap、List 大配列
// 値は経験的に 64KB 程度。Eden サイズに対して数 % 程度を目安。
// =============================================================================
constexpr size_t kLargeAllocThreshold = 64 * 1024;

// =============================================================================
// YoungChunk: 1 領域 = base..end の連続バッファ + 必要時の next 連鎖
//   通常 next = NULL (= 単一バッファ運用)
//   alloc/copy 中に溢れたら malloc して next にぶら下げる (= overflow 退避)
//   GC 末尾の consolidation で next 連鎖を free + base を必要なら拡張
// =============================================================================
struct YoungChunk {
    char*       base;
    char*       end;
    char*       ptr;       // bump alloc 位置
    char*       scan_ptr;  // Cheney scan の進捗 (= GC 中のみ使用)
    YoungChunk* next;     // 通常 NULL、overflow 時に malloc 連鎖
};

inline YoungChunk* young_chunk_alloc(size_t size) {
    YoungChunk* c = (YoungChunk*)malloc(sizeof(YoungChunk));
    c->base = (char*)malloc(size);
    c->end  = c->base + size;
    c->ptr  = c->base;
    c->scan_ptr = c->base;
    c->next = nullptr;
    return c;
}

inline void young_chunk_init_inplace(YoungChunk* c, size_t size) {
    c->base = (char*)malloc(size);
    c->end  = c->base + size;
    c->ptr  = c->base;
    c->scan_ptr = c->base;
    c->next = nullptr;
}

// next 連鎖だけ free (= base は残す)、c->next = NULL に戻す
inline void young_chunk_free_chain(YoungChunk* c) {
    YoungChunk* p = c->next;
    while (p) {
        YoungChunk* nx = p->next;
        free(p->base);
        free(p);
        p = nx;
    }
    c->next = nullptr;
}

// base を捨てて新サイズで取り直す (= chunk が空であること前提)
inline void young_chunk_replace_base(YoungChunk* c, size_t new_size) {
    free(c->base);
    c->base = (char*)malloc(new_size);
    c->end  = c->base + new_size;
    c->ptr  = c->base;
    c->scan_ptr = c->base;
    // next は呼び出し側責任 (= 通常 free 済みで NULL)
}

inline void young_chunk_destroy(YoungChunk* c) {
    young_chunk_free_chain(c);
    free(c->base);
    c->base = c->end = c->ptr = nullptr;
}

inline bool young_chunk_in_chain(YoungChunk* c, char* ptr) {
    for (YoungChunk* p = c; p; p = p->next) {
        if (ptr >= p->base && ptr < p->end) return true;
    }
    return false;
}

// =============================================================================
// Young世代: Eden + Survivor[2] (= ping-pong)。各 chunk は overflow 連鎖を持てる。
// =============================================================================
struct YoungHeap {
    YoungChunk eden;
    YoungChunk survivor0;
    YoungChunk survivor1;

    int active_survivor;    // 0 or 1

    // ===== consolidation 用統計 =====
    bool last_gc_survivor_overflowed;   // 直前 GC で survivor[to] に next 連鎖した
    int  no_overflow_streak;            // Survivor: 連続「overflow なし」GC 数
    int  cooldown_gcs;                  // base 再 malloc 直後のクールダウン
    bool eden_grew_this_period;         // GC 後に Eden が新 spill chunk を malloc したか
                                        // (= GC_check のトリガ用、true なら GC 発火)

    // 設定値 (= init で固定)
    size_t eden_size_initial;           // 縮小下限
    size_t survivor_size_initial;
    size_t eden_size_max;               // 拡張上限
    size_t survivor_size_max;

    static constexpr int kShrinkStreak = 8;    // 連続 8 GC overflow なしで縮小候補
    static constexpr int kCooldownGCs  = 4;    // 再 malloc 直後 4 GC は静観

    void init(size_t eden_bytes, size_t survivor_bytes) {
        young_chunk_init_inplace(&eden, eden_bytes);
        young_chunk_init_inplace(&survivor0, survivor_bytes);
        young_chunk_init_inplace(&survivor1, survivor_bytes);
        active_survivor = 0;

        last_gc_survivor_overflowed = false;
        no_overflow_streak = 0;
        cooldown_gcs = 0;
        eden_grew_this_period = false;
        eden_size_initial     = eden_bytes;
        survivor_size_initial = survivor_bytes;
        eden_size_max         = eden_bytes * 64;
        survivor_size_max     = survivor_bytes * 64;
    }
    void init(size_t total_size) {
        init(total_size / 2, total_size / 4);
    }

    void destroy() {
        young_chunk_destroy(&eden);
        young_chunk_destroy(&survivor0);
        young_chunk_destroy(&survivor1);
    }

    YoungChunk* active_from() { return active_survivor == 0 ? &survivor0 : &survivor1; }
    YoungChunk* active_to()   { return active_survivor == 0 ? &survivor1 : &survivor0; }

    // 互換ポインタアクセサ (= 既存コードの "Eden 先頭" / "to 先頭" 想定箇所向け)
    char* eden_ptr_get()      { return eden.ptr; }
    void  eden_ptr_reset()    { eden.ptr = eden.base; }
    char* survivor_ptr_get()  { return active_to()->ptr; }
    void  survivor_ptr_reset(){ active_to()->ptr = active_to()->base; }

    void swap_survivors() {
        active_survivor = 1 - active_survivor;
    }

    bool in_eden(char* ptr) const {
        return young_chunk_in_chain(const_cast<YoungChunk*>(&eden), ptr);
    }
    bool in_survivor(char* ptr) {
        return young_chunk_in_chain(active_from(), ptr);
    }
    bool in_young(char* ptr) {
        return in_eden(ptr) || in_survivor(ptr);
    }
};

// =============================================================================
// Old世代ヒープ（移動なし、リンクリスト管理）
// =============================================================================
struct OldBlock {
    OldBlock* next;
    OldBlock* prev;
    bool marked;
    // Object + data follows
};

struct OldHeap {
    OldBlock sentinel;      // ダミーヘッド
    size_t total_size;
    size_t used_size;

    void init() {
        sentinel.next = &sentinel;
        sentinel.prev = &sentinel;
        total_size = 0;
        used_size = 0;
    }

    void destroy() {
        OldBlock* block = sentinel.next;
        while (block != &sentinel) {
            OldBlock* next = block->next;
            free(block);
            block = next;
        }
    }

    char* alloc(uint32_t type, uint32_t size, uint8_t gc_epoch) {
        size_t block_size = sizeof(OldBlock) + sizeof(Object) + size;
        OldBlock* block = (OldBlock*)malloc(block_size);
        if (!block) return nullptr;

        // リンクリストに追加
        block->next = sentinel.next;
        block->prev = &sentinel;
        sentinel.next->prev = block;
        sentinel.next = block;
        block->marked = false;

        Object* obj = (Object*)(block + 1);
        obj->type = type;
        obj->survival_count = PROMOTION_THRESHOLD;  // 昇格済み
        obj->generation = (uint8_t)Generation::Old;
        obj->forwarded = gc_epoch;
        obj->flags = 0;
        obj->fwd = nullptr;

        char* data = (char*)(obj + 1);
        memset(data, 0, size);

        used_size += block_size;
        return data;
    }

    void sweep(ThreadGC* gc, FinalizerQueue* finalizer_queue);
};


struct LoadedDll {
    void* handle;                // HMODULE on Windows, void* elsewhere
    std::string name;            // e.g. "tes" for clang3/tes.dll
    int class_base;              // thgc->class_count snapshot before registering this DLL's classes
};

struct ThreadGC {
    CoroutineQueue* queue;
    // LLVM から (%RootNodeType**) オフセットで参照される想定なのでポインタに保持する。
    // 実体は GC_init で new RootNode() して紐付け。Destructor 等はここでは管理しない
    // (センチネルなので ThreadGC と同じライフタイム)。
    RootNode* root_sentinel;
    CoroutineQueue* first;
    YoungHeap young;
    OldHeap old;
    ClassType class_table[MAX_CLASSES];
    int class_count;
    FinalizerQueue* finalizer_queue;
    uint8_t gc_epoch;
    uint64_t young_gc_count;
    uint64_t old_gc_count;
    uint64_t promoted_count;
    std::mutex mutex;
    RenderCommandQueue* commandQueue;
    HopStar* hoppy;
    bool waitgc = false;
    Map* map;
    NewLocal* local = nullptr;  // このスレッドのUI要素ツリー
    List* hoveredList = nullptr; // 現在hovered中の要素リスト
    List* hoveredSpanList = nullptr; // 現在hovered中のRenderSpanリスト (HoveredSpan)
    int staticid = -1;
    std::vector<NativeWindow*> windows;
    std::vector<NativeWindow*> linkwins;// このタブが所有するウィンドウ (windows[0] = メイン)
    std::vector<NativeWindow*> openPopups;  // 現在開いているポップアップ
    CoroutineQueue* animQueue = nullptr;   // アニメーション用キュー (Measure直前に実行)
    float deltaTime = 0.0f;                // 前フレームからの経過時間(秒)

    // ReadDll / runCompiledDllAsync でロードされた DLL の情報。
    // GoThread からはこれを参照して DLL 固有のエントリ関数を呼び出すことができる。
    std::mutex loadedDllsMutex;
    std::vector<LoadedDll> loadedDlls;

    // Cheney scan-pointer GC で Old 昇格された data 先頭 pointer を貯める。
    // gc_young の scan loop で消費 (= tracer を流して子を forward)、終端で clear。
    // copy_object_young 内では tracer を再帰呼び出しせず、ここに push のみ。
    std::vector<char*> promoted_pending;
};

// =============================================================================
// 初期化・終了
// =============================================================================
// Eden / Survivor サイズ個別指定版。survivor_bytes は survivor0 / survivor1 共通サイズ
// (= from / to の到底容量、片方だけ違うサイズにしても小さい方に制限されるので同値想定)。
inline ThreadGC* GC_init(size_t eden_bytes, size_t survivor_bytes) {
    ThreadGC* gc = new ThreadGC();
	gc->queue = new CoroutineQueue();
	gc->animQueue = new CoroutineQueue();
    gc->young.init(eden_bytes, survivor_bytes);
    gc->old.init();

    gc->class_count = 128;
    gc->first = NULL;
    gc->commandQueue = createRCQ();

    // ルートセンチネル
    gc->root_sentinel = new RootNode();
    gc->root_sentinel->gc = gc;
    gc->root_sentinel->next = gc->root_sentinel;
    gc->root_sentinel->prev = gc->root_sentinel;
    gc->root_sentinel->roots = (char***)malloc(sizeof(char**) * MAX_ROOTS_PER_NODE);
    gc->root_sentinel->count = 0;
    gc->root_sentinel->capacity = MAX_ROOTS_PER_NODE;

    gc->finalizer_queue = new FinalizerQueue();
    gc->gc_epoch = 0;

    gc->young_gc_count = 0;
    gc->old_gc_count = 0;
    gc->promoted_count = 0;

    return gc;
}
// 旧 API: total_size から 50% / 25% / 25% で内部分配する互換形。
inline ThreadGC* GC_init(size_t young_size) {
    return GC_init(young_size / 2, young_size / 4);
}

inline void GC_shutdown(ThreadGC* gc) {
    // Finalizerをフラッシュ
    gc->finalizer_queue->flush();
    delete gc->finalizer_queue;

    // ヒープ解放
    gc->young.destroy();
    gc->old.destroy();

    // ルート解放
    RootNode* node = gc->root_sentinel->next;
    while (node != gc->root_sentinel) {
        RootNode* next = node->next;
        free(node->roots);
        free(node);
        node = next;
    }
    delete gc->root_sentinel;

    delete gc;
}

// =============================================================================
// クラス登録
// =============================================================================
String* createString(ThreadGC* thgc, char* tex, int size, int esize);
// Cheney scan-pointer は obj header (16B) + payload を順に走査するため、
// 各オブジェクトサイズを 8 バイト境界に揃える必要がある (= 揃えないと次の
// Object header を misaligned で読み、obj->type にゴミが入って scan が崩壊)。
static inline uint32_t gc_align_size(uint32_t s) {
    return (s + 7u) & ~7u;
}
inline int GC_register_class(ThreadGC* gc, int type, int size, const char* name,
    GCCheckFunc check, GCFinalizeFunc finalize,
    bool has_io = false) {
    gc->class_table[type].size = gc_align_size((uint32_t)size);
    gc->class_table[type].gc_check = check;
    gc->class_table[type].gc_finalize = finalize;
    gc->class_table[type].has_io = has_io;
    gc->class_table[type].name = createString(gc, (char*)name, (int)strlen(name), 1);
    return type;
}

// =============================================================================
// ルート管理
// =============================================================================
inline RootNode* GC_add_root_node(ThreadGC* gc) {
    RootNode* node = (RootNode*)malloc(sizeof(RootNode));
    node->gc = gc;
    node->roots = (char***)malloc(sizeof(char**) * MAX_ROOTS_PER_NODE);
    node->count = 0;
    node->capacity = MAX_ROOTS_PER_NODE;

    // リンクリストに追加
    node->next = gc->root_sentinel->next;
    node->prev = gc->root_sentinel;
    gc->root_sentinel->next->prev = node;
    gc->root_sentinel->next = node;

    return node;
}

inline void GC_release_root_node(RootNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node->roots);
    free(node);
}

inline void GC_add_root(RootNode* node, char** root) {
    if (node->count < node->capacity) {
        node->roots[node->count++] = root;
    }
}


// =============================================================================
// (2) 統一方式: GC は event loop の GC_check で唯一発火する。
//
//   設計原則:
//     - GC_alloc は決して gc_young を起こさない (= 常に spill 連鎖に逃がす)
//     - 任意のコード中で GC が走ることはない (= ローカルが move しない、stale 化しない)
//     - 安全点 = event loop が GC_check を呼ぶタイミング
//     - 安全点で生きるべきオブジェクトは静的 root (= thgc->map / thgc->local) 経由で reachable
//       (= UI ツリーに繋がっている、map に登録されている、coroutine root_node に積まれている)
//
//   使い方:
//     int main() {
//         ThreadGC* thgc = GC_init(...);
//         init local, map, etc.;
//         while (running) {
//             buildFrame(thgc);
//             GC_check(thgc);    // ← Eden / spill 状況見て gc_young 発火
//         }
//     }
// =============================================================================
inline void gc_young(ThreadGC* gc);

// GC_check: event loop から呼ぶ唯一の GC トリガ。
//   この期間中に Eden が新 spill chunk を malloc していたら gc_young を発火。
//   spill chain は GC 後も保持される (= ptr リセットのみ、free しない) ので、
//   warmup 後 Eden が安定サイズになったら新規 malloc は起きず GC も走らない。
//   malloc/free の churn を最小化する設計。
inline void GC_check(ThreadGC* gc) {
    if (gc->young.eden_grew_this_period) {
        std::lock_guard<std::mutex> lock(gc->mutex);
        gc_young(gc);
    }
}

inline void GC_pop_roots(RootNode* node, int n) {
    node->count -= n;
    if (node->count < 0) node->count = 0;
}

// =============================================================================
// オブジェクトコピー（Young GC用）
// =============================================================================
inline char* copy_object_young(ThreadGC* gc, char* value) {
    if (value == nullptr) return nullptr;

    Object* obj = (Object*)(value - sizeof(Object));

    // Old世代は移動しない
    if (obj->generation == (uint8_t)Generation::Old) {
        return value;
    }

    // クロスヒープガード: value がこの gc の Young heap 外なら触らない (= 別 thgc の object)。
    // 別 heap の pointer を memcpy + forward すると相手 heap を破壊する。
    // Young object が別 thgc 由来の場合、そのまま返す (= alias 機構などで別途同期される前提)。
    if (!gc->young.in_young(value)) {
        return value;
    }

    // 既に転送済み
    if (obj->forwarded == gc->gc_epoch && obj->fwd != nullptr) {
        return obj->fwd;
    }

    // サイズ取得
    uint32_t size;
    ClassType* cls = nullptr;
    if (obj->type >= 65536) {
        size = obj->type - 65536;
    }
    else {
        cls = &gc->class_table[obj->type];
        size = cls->size;
    }

    // 生存回数インクリメント
    obj->survival_count++;

    // 昇格判定
    if (obj->survival_count >= PROMOTION_THRESHOLD) {
        // Oldへ昇格
        char* new_data = gc->old.alloc(obj->type, size, gc->gc_epoch);
        memcpy(new_data, value, size);

        obj->forwarded = gc->gc_epoch;
        obj->fwd = new_data;

        gc->promoted_count++;

        // Cheney scan-pointer 方式: ここでは tracer を再帰呼び出ししない。
        // gc_young の scan loop で promoted_pending を消費する際に tracer を流す。
        if (cls && cls->gc_check) {
            gc->promoted_pending.push_back(new_data);
        }

        return new_data;
    }

    // Survivorにコピー: chain を歩いて最初に空きが見つかった chunk に alloc。
    //   前の chunk の末尾空きも活用して memory を無駄にしない。
    //   Cheney scan は chunk ごとの scan_ptr を見て新規 object も漏れなく拾う。
    YoungChunk* to_chunk = gc->young.active_to();
    size_t needed = sizeof(Object) + size;
    while (to_chunk->ptr + needed > to_chunk->end) {
        if (!to_chunk->next) {
            // overflow: doubling サイズで次の chunk を確保
            size_t cur_cap = (size_t)(to_chunk->end - to_chunk->base);
            size_t next_size = cur_cap * 2;
            if (next_size < needed) next_size = needed * 2;
            to_chunk->next = young_chunk_alloc(next_size);
            gc->young.last_gc_survivor_overflowed = true;
        }
        to_chunk = to_chunk->next;
    }

    Object* new_obj = (Object*)to_chunk->ptr;
    to_chunk->ptr += needed;

    memcpy(new_obj, obj, sizeof(Object) + size);
    new_obj->generation = (uint8_t)Generation::Young;
    new_obj->forwarded = gc->gc_epoch;
    new_obj->fwd = nullptr;

    char* new_data = (char*)(new_obj + 1);

    obj->forwarded = gc->gc_epoch;
    obj->fwd = new_data;

    // Cheney scan-pointer 方式: ここでは tracer を呼ばない。
    // gc_young の scan loop が to-space を base..ptr で線形に歩いて全オブジェクトの
    // tracer を実行する (= 関数 stack を消費せず、深いツリーでも安全)。
    return new_data;
}

// =============================================================================
// Consolidation: gc_young 末尾 (= swap 後) で呼ぶ。
//   swap 前の "to" (= live を書いた側) には next 連鎖がぶら下がっている可能性がある。
//   swap 後それは "from" となり、次回 GC まで触れない。
//   逆に swap 後の "to" (= 直前の from = 今 scan 終わった = 空) は安全に触れる。
//
//   方針:
//     - 直前 GC で survivor 連鎖が発生 (= overflow) → 「base が小さすぎ」のサイン
//        → swap 後 to の base を free + 大きいサイズで取り直す (= overflow 量を吸収)
//     - 連続 N GC overflow なし & base が初期サイズより大きい → 縮小候補
//        → swap 後 to の base を半減 (= 次第に縮む)
//     - swap 後 from (= live 持ち) は触らない
//     - 残り side (= 旧 from = 新 to) への適用は「次回 GC 末尾」で対称的に行う
//        (= overflow 状態は 1-2 GC かけて両側に伝播)
// =============================================================================
inline void gc_apply_consolidation(ThreadGC* gc) {
    YoungHeap& y = gc->young;

    if (y.cooldown_gcs > 0) {
        --y.cooldown_gcs;
        // クールダウン中も from 側の連鎖だけは free (= live を含まなければ)。
        // 安全のため何もしない。
        return;
    }

    // swap 後の to (= 旧 from、空) を取り直し対象にする
    YoungChunk* fresh = y.active_to();
    // この時点で fresh->next は無いはず (= 旧 from は連鎖を持たない、live は新 from に集約済み)
    // 念のため free。
    young_chunk_free_chain(fresh);

    if (y.last_gc_survivor_overflowed) {
        // overflow 観測 → 新 from 側 (= live 連鎖込み) の総使用量を見て base を大きく取り直す
        size_t total_used = 0;
        for (YoungChunk* p = y.active_from(); p; p = p->next) total_used += (size_t)(p->ptr - p->base);
        size_t cur_base   = (size_t)(fresh->end - fresh->base);
        // 推奨サイズ = total_used * 2 を最低保証 (= 次フレームの同等ピークも 1 chunk に収める)
        size_t want = total_used * 2;
        if (want < cur_base * 2) want = cur_base * 2;        // 最低でも倍増
        if (want > y.survivor_size_max) want = y.survivor_size_max;
        if (want > cur_base) {
            young_chunk_replace_base(fresh, want);
            y.cooldown_gcs = YoungHeap::kCooldownGCs;
        }
        y.no_overflow_streak = 0;
        y.last_gc_survivor_overflowed = false;
    } else {
        ++y.no_overflow_streak;
        size_t cur_base = (size_t)(fresh->end - fresh->base);
        if (y.no_overflow_streak >= YoungHeap::kShrinkStreak &&
            cur_base > y.survivor_size_initial) {
            size_t want = cur_base / 2;
            if (want < y.survivor_size_initial) want = y.survivor_size_initial;
            young_chunk_replace_base(fresh, want);
            y.no_overflow_streak = 0;
            y.cooldown_gcs = YoungHeap::kCooldownGCs;
        }
    }
}

// =============================================================================
// Young GC
// =============================================================================
inline void gc_young(ThreadGC* gc) {
    gc->gc_epoch = (gc->gc_epoch + 1) % 256;

    // To-Survivor の bump をリセット (= active_to の base 先頭から書き始める)
    YoungChunk* to_chunk = gc->young.active_to();
    to_chunk->ptr = to_chunk->base;
    to_chunk->scan_ptr = to_chunk->base;
    // 直前 GC でぶら下がった next 連鎖が残っているなら破棄 (= 内容は次回 from 側に移されているはず)
    young_chunk_free_chain(to_chunk);

    // ルートからスキャン (= 静的 root: thgc->map / local など長寿命の根)
    // copy_object_young は tracer を呼ばず memcpy + 転送 stub のみ。
    RootNode* node = gc->root_sentinel->next;
    while (node != gc->root_sentinel) {
        for (int i = 0; i < node->count; i++) {
            if (node->roots[i] && *node->roots[i]) {
                *node->roots[i] = copy_object_young(gc, *node->roots[i]);
            }
        }
        node = node->next;
    }

    // Old → Young 参照の更新 (= write barrier 代替)。
    // Old 世代のオブジェクトは Young GC では copy 対象外だが、Young 子を持っている
    // 可能性があるので、その子は forward しないと dangling pointer になる。
    // 全 Old を walk して tracer を実行 (= O(|Old|)、コストは Old サイズ次第)。
    {
        OldBlock* sentinel = &gc->old.sentinel;
        for (OldBlock* block = sentinel->next; block != sentinel; block = block->next) {
            Object* oobj = (Object*)(block + 1);
            char* odata = (char*)(oobj + 1);
            if (oobj->type < 65536) {
                ClassType* ocls = &gc->class_table[oobj->type];
                if (ocls->gc_check) {
                    ocls->gc_check(gc, odata);
                }
            }
        }
    }

    // Cheney scan: chain 全体を walk し、各 chunk の scan_ptr から ptr までを処理する。
    //   chunk ごとに scan_ptr を持たせることで、alloc が任意 chunk に入っても拾える
    //   (= 前の chunk の末尾空きを使って alloc しても、再度 chain walk で新規分を処理)。
    //   Old 昇格分は promoted_pending に貯まるので、各 chain walk 後にそちらも消化する。
    {
        size_t old_idx = 0;
        bool progress;
        do {
            progress = false;

            // (1) chain 全体を walk して各 chunk の未 scan 分を処理
            for (YoungChunk* c = gc->young.active_to(); c != nullptr; c = c->next) {
                while (c->scan_ptr < c->ptr) {
                    Object* obj = (Object*)c->scan_ptr;
                    char* dat = (char*)(obj + 1);
                    uint32_t sz;
                    if (obj->type >= 65536) {
                        sz = obj->type - 65536;
                    } else {
                        ClassType* cls2 = &gc->class_table[obj->type];
                        sz = cls2->size;
                        if (cls2->gc_check) {
                            cls2->gc_check(gc, dat);  // ← 子を forward
                            progress = true;
                        }
                    }
                    c->scan_ptr += sizeof(Object) + sz;
                }
            }

            // (2) Old 昇格 queue を消化 (= tracer は Young への forward を起こす可能性あり)
            while (old_idx < gc->promoted_pending.size()) {
                char* old_data = gc->promoted_pending[old_idx++];
                Object* oobj = (Object*)(old_data - sizeof(Object));
                if (oobj->type < 65536) {
                    ClassType* ocls = &gc->class_table[oobj->type];
                    if (ocls->gc_check) {
                        ocls->gc_check(gc, old_data);
                        progress = true;
                    }
                }
            }
        } while (progress);

        gc->promoted_pending.clear();
    }

    // Eden 整理: spill chain が発生していたら 1 枚に統合する (= 合計容量で再 malloc)。
    //   - フレーム中は何枚 spill chunk を作っても良い (= 任意サイズに対応)
    //   - GC 後に「合計容量の base 1 枚」に再構成して、次フレームは bump 1 枚で速い
    //   - 縮小はしない (= 一度大きくなったらそのまま維持)
    if (gc->young.eden.next != nullptr) {
        size_t total_cap = 0;
        for (YoungChunk* p = &gc->young.eden; p; p = p->next) total_cap += (size_t)(p->end - p->base);
        young_chunk_free_chain(&gc->young.eden);   // chain 全 free
        young_chunk_replace_base(&gc->young.eden, total_cap);  // 統合サイズで base 再 malloc
    } else {
        // 連鎖なし → ptr リセットのみ
        gc->young.eden.ptr = gc->young.eden.base;
    }
    // 次の期間の grow フラグをリセット
    gc->young.eden_grew_this_period = false;

    // Survivorをスワップ
    gc->young.swap_survivors();

    // Survivor base リサイズ判定 (= swap 後、to 側は空のため安全)
    gc_apply_consolidation(gc);

    gc->young_gc_count++;
}

// =============================================================================
// Old GC（Mark-Sweep）
// =============================================================================
inline void mark_object(ThreadGC* gc, char* value) {
    if (value == nullptr) return;

    Object* obj = (Object*)(value - sizeof(Object));

    // Young世代はスキップ（Young GCで処理済み）
    if (obj->generation == (uint8_t)Generation::Young) {
        return;
    }

    // Old世代のマーク
    OldBlock* block = (OldBlock*)((char*)obj - sizeof(OldBlock));
    if (block->marked) return;  // 既にマーク済み

    block->marked = true;

    // 子オブジェクトをスキャン
    if (obj->type < 65536) {
        ClassType* cls = &gc->class_table[obj->type];
        if (cls->gc_check) {
            cls->gc_check(gc, value);
        }
    }
}

inline void OldHeap::sweep(ThreadGC* gc, FinalizerQueue* finalizer_queue) {
    OldBlock* block = sentinel.next;
    while (block != &sentinel) {
        OldBlock* next = block->next;

        if (!block->marked) {
            // 未マーク = 回収対象
            Object* obj = (Object*)(block + 1);
            char* data = (char*)(obj + 1);

            // Finalizerをキューに積む（非同期実行）
            if (obj->type < 65536) {
                ClassType* cls = &gc->class_table[obj->type];
                if (cls->gc_finalize) {
                    uint32_t size = cls->size;
                    finalizer_queue->push(cls->gc_finalize, data, size);
                }
            }

            // リンクリストから削除
            block->prev->next = block->next;
            block->next->prev = block->prev;

            // ブロック解放
            free(block);
        }
        else {
            // マークをクリア（次回GC用）
            block->marked = false;
        }

        block = next;
    }
}

inline void gc_old(ThreadGC* gc) {
    // まずYoung GC
    gc_young(gc);

    // Mark phase (= 静的 root)
    RootNode* node = gc->root_sentinel->next;
    while (node != gc->root_sentinel) {
        for (int i = 0; i < node->count; i++) {
            if (node->roots[i] && *node->roots[i]) {
                mark_object(gc, *node->roots[i]);
            }
        }
        node = node->next;
    }

    // (起動時の GC_add_root_node + GC_add_root 登録分は root_sentinel walk で拾う。
    //  ここでは追加の mark は無し。)

    // Sweep phase
    gc->old.sweep(gc, gc->finalizer_queue);

    gc->old_gc_count++;
}

// =============================================================================
// メモリ割り当て
// =============================================================================

// Young世代に割り当て（通常のオブジェクト）
inline char* GC_alloc(ThreadGC* gc, int type) {
    std::lock_guard<std::mutex> lock(gc->mutex);

    ClassType* cls = &gc->class_table[type];

    // I/Oリソースを持つクラスはOld直行
    if (cls->has_io) {
        return gc->old.alloc(type, cls->size, gc->gc_epoch);
    }

    uint32_t size = cls->size;
    size_t needed = sizeof(Object) + size;

    // 大容量クラスも Old 直行 (= young GC で copy しない)
    if (size > kLargeAllocThreshold) {
        return gc->old.alloc(type, size, gc->gc_epoch);
    }

    // (2) 統一方式: GC_alloc は決して gc_young を起こさない。
    // Eden 満杯時は spill chunk (= eden.next 連鎖) に逃がす。
    // GC は event loop の GC_check で唯一発火する。
    YoungChunk* c = &gc->young.eden;
    while (c->ptr + needed > c->end) {
        if (!c->next) {
            size_t cur_cap = (size_t)(c->end - c->base);
            size_t next_size = cur_cap * 2;
            if (next_size < needed) next_size = needed * 2;
            c->next = young_chunk_alloc(next_size);
            gc->young.eden_grew_this_period = true;   // 新 chunk malloc → GC 必要
        }
        c = c->next;
    }
    Object* obj = (Object*)c->ptr;
    c->ptr += needed;

    obj->type = type;
    obj->survival_count = 0;
    obj->generation = (uint8_t)Generation::Young;
    obj->forwarded = gc->gc_epoch;
    obj->flags = 0;
    obj->fwd = nullptr;

    char* data = (char*)(obj + 1);
    memset(data, 0, size);

    return data;
}

// 可変サイズ割り当て（Young / Old は size 閾値で振り分け）
inline char* GC_alloc_size(ThreadGC* gc, uint32_t size) {
    std::lock_guard<std::mutex> lock(gc->mutex);

    uint32_t asize = gc_align_size(size);  // 8B 境界揃え (Cheney scan の整合性確保)

    // 大容量データは Old 直行 (= 文字列の data buffer、画像 bitmap、List data 等)
    // young GC で copy しないので高速 + Eden を圧迫しない。
    if (asize > kLargeAllocThreshold) {
        return gc->old.alloc(65536 + asize, asize, gc->gc_epoch);
    }

    size_t needed = sizeof(Object) + asize;
    // (2) 統一方式: 常に bump、満杯時は spill 連鎖に逃がす。
    YoungChunk* c = &gc->young.eden;
    while (c->ptr + needed > c->end) {
        if (!c->next) {
            size_t cur_cap = (size_t)(c->end - c->base);
            size_t next_size = cur_cap * 2;
            if (next_size < needed) next_size = needed * 2;
            c->next = young_chunk_alloc(next_size);
            gc->young.eden_grew_this_period = true;   // 新 chunk malloc → GC 必要
        }
        c = c->next;
    }
    Object* obj = (Object*)c->ptr;
    c->ptr += needed;

    obj->type = 65536 + asize;  // サイズ埋め込み (= aligned)
    obj->survival_count = 0;
    obj->generation = (uint8_t)Generation::Young;
    obj->forwarded = gc->gc_epoch;
    obj->flags = 0;
    obj->fwd = nullptr;

    char* data = (char*)(obj + 1);
    memset(data, 0, asize);

    return data;
}

// Old世代に直接割り当て（I/Oリソース用）
inline char* GC_alloc_old(ThreadGC* gc, int type) {
    std::lock_guard<std::mutex> lock(gc->mutex);

    ClassType* cls = &gc->class_table[type];
    return gc->old.alloc(type, cls->size, gc->gc_epoch);
}

// Old世代に可変サイズ割り当て
inline char* GC_alloc_old_size(ThreadGC* gc, uint32_t size) {
    std::lock_guard<std::mutex> lock(gc->mutex);
    uint32_t asize = gc_align_size(size);
    return gc->old.alloc(65536 + asize, asize, gc->gc_epoch);
}
// オブジェクトを複製（Young世代に作成）
inline char* GC_clone(ThreadGC* gc, char* value) {
    if (value == nullptr) return nullptr;

    std::lock_guard<std::mutex> lock(gc->mutex);

    Object* obj = (Object*)(value - sizeof(Object));

    uint32_t size;
    ClassType* cls = nullptr;
    if (obj->type >= 65536) {
        size = obj->type - 65536;
    }
    else {
        cls = &gc->class_table[obj->type];
        size = cls->size;
    }

    // I/Oリソースを持つならOldに複製
    if (cls && cls->has_io) {
        char* new_data = gc->old.alloc(obj->type, size, gc->gc_epoch);
        if (new_data) {
            memcpy(new_data, value, size);
        }
        return new_data;
    }

    // Edenに空きがあるか
    if (gc->young.eden.ptr + sizeof(Object) + size > gc->young.eden.end) {
        gc_young(gc);
    }

    // まだ足りなければOldに作成
    if (gc->young.eden.ptr + sizeof(Object) + size > gc->young.eden.end) {
        char* new_data = gc->old.alloc(obj->type, size, gc->gc_epoch);
        if (new_data) {
            memcpy(new_data, value, size);
        }
        return new_data;
    }

    // Edenに複製
    Object* new_obj = (Object*)gc->young.eden.ptr;
    gc->young.eden.ptr += sizeof(Object) + size;

    new_obj->type = obj->type;
    new_obj->survival_count = 0;
    new_obj->generation = (uint8_t)Generation::Young;
    new_obj->forwarded = gc->gc_epoch;
    new_obj->flags = 0;
    new_obj->fwd = nullptr;

    char* new_data = (char*)(new_obj + 1);
    memcpy(new_data, value, size);

    return new_data;
}

// FuncType (CType::_FuncTypeR) の GC tracer。FuncType は gc.h 上部で定義されているが、
// GC_clone の実装は本関数の直前にあるため、ここでまとめて定義する。
inline void FuncTypeRCheck(ThreadGC* thgc, char* data) {
    FuncType* f = (FuncType*)data;
    f->cls = (Obj*)copy_object_young(thgc, (char*)f->cls);
    // obj/draws are untyped; cannot trace generically
}

// オブジェクトを複製（Old世代に作成）
inline char* GC_clone_old(ThreadGC* gc, char* value) {
    if (value == nullptr) return nullptr;

    std::lock_guard<std::mutex> lock(gc->mutex);

    Object* obj = (Object*)(value - sizeof(Object));

    uint32_t size;
    if (obj->type >= 65536) {
        size = obj->type - 65536;
    }
    else {
        size = gc->class_table[obj->type].size;
    }

    char* new_data = gc->old.alloc(obj->type, size, gc->gc_epoch);
    if (new_data) {
        memcpy(new_data, value, size);
    }
    return new_data;
}

// ロックなし版（GC中やバッチ処理用）
inline char* GC_clone_unlocked(ThreadGC* gc, char* value) {
    if (value == nullptr) return nullptr;

    Object* obj = (Object*)(value - sizeof(Object));

    uint32_t size;
    ClassType* cls = nullptr;
    if (obj->type >= 65536) {
        size = obj->type - 65536;
    }
    else {
        cls = &gc->class_table[obj->type];
        size = cls->size;
    }

    // 空きチェック（足りなければnullptr）
    if (gc->young.eden.ptr + sizeof(Object) + size > gc->young.eden.end) {
        return nullptr;
    }

    Object* new_obj = (Object*)gc->young.eden.ptr;
    gc->young.eden.ptr += sizeof(Object) + size;

    new_obj->type = obj->type;
    new_obj->survival_count = 0;
    new_obj->generation = (uint8_t)Generation::Young;
    new_obj->forwarded = gc->gc_epoch;
    new_obj->flags = 0;
    new_obj->fwd = nullptr;

    char* new_data = (char*)(new_obj + 1);
    memcpy(new_data, value, size);

    return new_data;
}
// =============================================================================
// 明示的GC呼び出し
// =============================================================================
inline void GC_collect_young(ThreadGC* gc) {
    std::lock_guard<std::mutex> lock(gc->mutex);
    gc_young(gc);
}

inline void GC_collect_full(ThreadGC* gc) {
    std::lock_guard<std::mutex> lock(gc->mutex);
    gc_old(gc);
}

// =============================================================================
// ユーティリティ
// =============================================================================
inline Object* GC_get_header(char* data) {
    return (Object*)(data - sizeof(Object));
}

inline bool GC_is_young(char* data) {
    Object* obj = GC_get_header(data);
    return obj->generation == (uint8_t)Generation::Young;
}

inline bool GC_is_old(char* data) {
    Object* obj = GC_get_header(data);
    return obj->generation == (uint8_t)Generation::Old;
}


// =============================================================================
// ロックなし割り当て（別スレッド用 - オーディオコールバック等）
// GCを発動しない、空きがなければnullptrを返す
// =============================================================================

// ロックなしYoung割り当て
inline char* GC_alloc_ant(ThreadGC* gc, int type) {
    ClassType* cls = &gc->class_table[type];
    uint32_t size = cls->size;

    // 空きチェックのみ（GC発動しない）
    if (gc->young.eden.ptr + sizeof(Object) + size > gc->young.eden.end) {
        return nullptr;
    }

    Object* obj = (Object*)gc->young.eden.ptr;
    gc->young.eden.ptr += sizeof(Object) + size;

    obj->type = type;
    obj->survival_count = 0;
    obj->generation = (uint8_t)Generation::Young;
    obj->forwarded = gc->gc_epoch;
    obj->flags = 0;
    obj->fwd = nullptr;

    char* data = (char*)(obj + 1);
    memset(data, 0, size);

    return data;
}

// ロックなし可変サイズ割り当て
inline char* GC_alloc_size_ant(ThreadGC* gc, uint32_t size) {
    uint32_t asize = gc_align_size(size);
    if (gc->young.eden.ptr + sizeof(Object) + asize > gc->young.eden.end) {
        return nullptr;
    }

    Object* obj = (Object*)gc->young.eden.ptr;
    gc->young.eden.ptr += sizeof(Object) + asize;

    obj->type = 65536 + asize;
    obj->survival_count = 0;
    obj->generation = (uint8_t)Generation::Young;
    obj->forwarded = gc->gc_epoch;
    obj->flags = 0;
    obj->fwd = nullptr;

    char* data = (char*)(obj + 1);
    memset(data, 0, asize);

    return data;
}

// ロックなしOld割り当て（mallocするので注意）
inline char* GC_alloc_old_ant(ThreadGC* gc, int type) {
    ClassType* cls = &gc->class_table[type];
    return gc->old.alloc(type, cls->size, gc->gc_epoch);
}

// ロックなしOld可変サイズ割り当て
inline char* GC_alloc_old_size_ant(ThreadGC* gc, uint32_t size) {
    uint32_t asize = gc_align_size(size);
    return gc->old.alloc(65536 + asize, asize, gc->gc_epoch);
}
// 統計情報
inline void GC_print_stats(ThreadGC* gc) {
    std::cout << "=== GC Statistics ===" << std::endl;
    std::cout << "Young GC count: " << gc->young_gc_count << std::endl;
    std::cout << "Old GC count: " << gc->old_gc_count << std::endl;
    std::cout << "Promoted objects: " << gc->promoted_count << std::endl;
    std::cout << "Eden used: " << (gc->young.eden.ptr - gc->young.eden.base) << " / " << (gc->young.eden.end - gc->young.eden.base) << std::endl;
}

// =============================================================================
// ヘルパーマクロ
// =============================================================================
#define GC_UPDATE_PTR(gc, ptr) \
do { \
    if (ptr) ptr = copy_object_young(gc, ptr); \
} while(0)
#define GC_UPDATE_PTR_C(gc, ptr, type) \
do { \
    if (ptr) ptr = (type)copy_object_young(gc, (char*)(ptr)); \
} while(0)

CoroutineQueue* queuefirst(ThreadGC* thgc) {
    return thgc->first;
}
