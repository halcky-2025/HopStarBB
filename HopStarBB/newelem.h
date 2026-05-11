#pragma once
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <bgfx/bgfx.h>
#include <limits>
enum DrawCommandType {
	DelFBO = -2, FBO = -1, Fill = 0, Gradient = 1, Stripe = 2, Checker = 3, GradientChecker = 4,
	Image = 5, PageCurl = 6, RawImage = 7, End = 8
};
// シザー (クリップ) 矩形。スクリーン (or 描画先 fbo) ピクセル空間。
//   DrawCommand::scissor / LayerInfo::currentScissor で使う。NULL なら scissor 無効。
//   width / height <= 0 の場合は無効化扱い (= bgfx::setViewScissor の挙動に準拠)。
struct ScissorRect {
	float x = 0, y = 0, w = 0, h = 0;
};
struct NewElement;
struct DrawCommand;
struct RectDrawCommand;
struct BorderShadow;
struct Background;
struct BorderDrawCommand;
struct ShadowOnlyDrawCommand;
struct BorderShadowDrawCommand;
struct ImageDrawCommand;
struct ImageBorderDrawCommand;
struct ImageShadowDrawCommand;
struct ImageBorderShadowDrawCommand;
struct PatternDrawCommand;
struct PatternBorderDrawCommand;
struct PatternShadowDrawCommand;
struct PatternBorderShadowDrawCommand;
struct FBODrawCommand;
struct CopyDrawCommand;
void MeasureString(FontAtlas& atlas, FontId font, String* text, int n, float width, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group);
void MeasureStringRange(FontAtlas& atlas, FontId font, String* text, int offset, int n, float width, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group);
void MeasureStringVerticalRange(FontAtlas& atlas, FontId font, String* text, int offset, int n, float height, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group);
struct LayerInfo {
	~LayerInfo() {
		for (auto* cmd : cmds) {
			delete cmd;
		}
	}

	// �R�s�[���֎~�i�_�u�� delete �h�~�j
	LayerInfo(const LayerInfo&) = delete;
	LayerInfo& operator=(const LayerInfo&) = delete;

	// ���[�u�͋���
	LayerInfo(LayerInfo&& other) noexcept
		: cmds(std::move(other.cmds))
		// ... ���̃����o�[�����[�u ...
	{
	}

	LayerInfo& operator=(LayerInfo&& other) noexcept {
		if (this != &other) {
			// �����̃R�}���h�����
			for (auto* cmd : cmds) {
				delete cmd;
			}
			cmds = std::move(other.cmds);
			// ... ���̃����o�[�����[�u ...
		}
		return *this;
	}

	LayerInfo() = default;
	std::map<std::string, std::vector<float>> defaultUniforms;
	std::map<std::string, bgfx::TextureHandle> defaultTextures;

	float opacity = 1.0f;
	bool visible = true;
	bool needsUpdate = true;
	int width = 1200;
	int height = 800;
	std::vector<DrawCommand*> cmds;

	// タイルFBO描画状態: 設定されている間、push関数がタイルごとにコマンドを展開する
	TiledTextureInfo* tiledTarget = nullptr;       // 描画先のタイル情報 (push で参照、submit では使わない)
	ImageId tiledTargetImageId = 0;                 // タイルテクスチャの安定 ID (submit 時の lookup 用)
	// 「このコマンドはタイル展開対象」の識別用 sentinel。レイヤーが所有する安定アドレス。
	// 旧設計では std::vector<Tile> 要素アドレス (&firstTile.fbo) を使っていたが、
	// resize で tiledTextures_.erase されるとベクタごと破壊されるので不正だった。
	bgfx::FrameBufferHandle _tiledSentinel = BGFX_INVALID_HANDLE;
	bgfx::FrameBufferHandle* tiledPlaceholderFBO = nullptr;  // &_tiledSentinel または nullptr
	uint64_t tiledBaseViewId = 0;                   // タイル0の論理viewId
	// 単一 tile (= page) ターゲット指定。-1 = 通常 (auto-expand: 重なる全 tile に展開)。
	//   >= 0 のとき、push() は該当 tile 1 個分の cmd だけを生成し、auto-expand を行わない。
	//   呼び出し側が「この tile を更新する」ループの中で setting & 戻すことを想定。
	int tiledTargetPageIndex = -1;

	// シザー (= 現在 push 中の DrawCommand に自動付与する clip rect)。NULL ならクリップ無し。
	//   呼び出し側が「ここ以降の push をクリップしたい」期間だけ非 NULL にして元に戻す。
	//   ポインタ先のライフタイムは renderAllCommands 終了まで保持されている必要がある。
	const ScissorRect* currentScissor = nullptr;

	// per-frame の ScissorRect 用プール。NewTableDraw 等で push back し、cmd->scissor が
	// その要素を指す。renderAllCommands 終了 (= cmd 全消費) まで pointer が安定する必要があり、
	// 単一の thread_local pool だと「table A → table B」の連続描画で B 側の clear が
	// A の cmd の scissor pointer を壊す → A が描画されない事故を起こす。よって layer 単位で
	// 持って、renderAllLayers / renderAllCommands 直前または直後の専用タイミングで clear する。
	// reserve はあらかじめ大きく取って、フレーム中の push で再 allocate しないこと。
	std::vector<ScissorRect> scissorPool;



	void push(DrawCommand* cmd);
void pushFill(float x, float y, float width, float height,
	float radiusTL, float radiusTR, float radiusBR, float radiusBL,
	float borderTop, float borderRight, float borderBottom, float borderLeft,
	float aaPixels,
	uint32_t fillColor, uint32_t borderColor,
	float shadowX, float shadowY, float shadowBlur,
	uint32_t shadowColor, float zIndex,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
	float blendMode = 0.0f,
	float cornerPattern = 0.0f);
void  pushImage(float x, float y, float width, float height,
	float atlasX, float atlasY, float atlasW, float atlasH,
	float radiusTL, float radiusTR, float radiusBR, float radiusBL,
	float aaPixels,
	float borderTop, float borderRight, float borderBottom, float borderLeft,
	uint32_t borderColor,
	float shadowX, float shadowY, float shadowBlur,
	uint32_t shadowColor, uint32_t modulate,
	float zIndex, bgfx::TextureHandle* tex1,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
	float blendMode = 0.0f,
	float cornerPattern = 0.0f,
	float backUVMinX = 0.0f,
	bgfx::TextureHandle* tex2 = nullptr,
	bgfx::TextureHandle* tex3 = nullptr,
	bgfx::TextureHandle* tex4 = nullptr);
void  pushPattern(enum DrawCommandType patternMode,
	float x, float y, float width, float height,
	float colorCount, float angle,
	float scrollX, float scrollY,
	float radiusTL, float radiusTR, float radiusBR, float radiusBL,
	float aaPixels,
	float borderTop, float borderRight, float borderBottom, float borderLeft,
	uint32_t borderColor,
	float shadowX, float shadowY, float shadowBlur,
	uint32_t shadowColor,
	int dataOffset, float zIndex,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
	float blendMode = 0.0f,
	float cornerPattern = 0.0f);
void  pushText(float x, float y, float width, float height,
	float atlasX, float atlasY, float atlasW, float atlasH,
	uint32_t color, float zIndex, bgfx::TextureHandle* tex1,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
	float blendMode = 0.0f);
void pushPageCurl(float x, float y, float width, float height,
	float progress, float curlRadius, float curlAngle,
	uint32_t fillColor,
	float uvMinX, float uvMinY, float uvSizeX, float uvSizeY,
	float backUVMinX, float backUVMinY,
	float borderTop, float borderRight, float borderBottom, float borderLeft,
	uint32_t borderColor,
	float shadowX, float shadowY, float shadowBlur,
	uint32_t shadowColor,
	bgfx::TextureHandle* tex1, bgfx::TextureHandle* tex2, float zIndex,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
	float blendMode = 0.0f, bool vertical = false, bool spread = false);
void pushRawImage(float x, float y, float width, float height,
	float atlasX, float atlasY, float atlasW, float atlasH,
	uint32_t modulate, float zIndex, bgfx::TextureHandle* tex1,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
	float blendMode = 0.0f);
void pushBackground(Background* back, float x, float y, float width, float height,
	float zIndex, bgfx::TextureHandle* tex1, bgfx::TextureHandle* tex2,
	bgfx::TextureHandle* tex3, bgfx::TextureHandle* tex4,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
	float blendMode, const float* boWidthes, BorderShadow* bs);

	LayerInfo& setUniform(const std::string& name, const std::vector<float>& data);
	LayerInfo& setTexture(const std::string& uniformName, bgfx::TextureHandle texture);
	void pushFBO(NewElement* elem, int fboWidth, int fboHeight,
		bgfx::TextureHandle* texture, bgfx::FrameBufferHandle* fbo, PointI* fbsize, bool resized);
	void pushDelFBO(bgfx::FrameBufferHandle fbo);
	void clear();
};
enum BgType {
	BgFill, BgImage, BgGradient, BgStripe, BgCheck, BgGradientChecker
};
enum AlignType {
	ZeroAlign, CenterAlign, OverAlign
};
enum EditType {
	Editable, Selectable, NoSelect
};
struct BorderShadow {
	float boRadiuses[4] = {}; // [0]=TL, [1]=TR, [2]=BR, [3]=BL
	uint32_t borderColor = 0;
	float shadowX = 0, shadowY = 0, shadowBlur = 0;
	uint32_t shadowColor = 0;
};
static BorderShadow noBorderShadow;
struct Background {
	enum DrawCommandType type;
	uint32_t fillcolor;
	ImageId tex1, tex2, tex3, tex4;
	float aa;
	float curl;
	BorderShadow bs;
	float count, offset, angle;
	float scrollX, scrollY;
	float cornerPattern;
	float backUVMinX = 0.0f;
	float backUVMinY = 0.0f;
};
inline void BackgroundCheck(ThreadGC* /*thgc*/, char* /*data*/) {
	// Background は ImageId (= uint32) と enum / float のみで GC ptr 無し
}
struct Offscreen;
struct NewLocal;
struct SDL_Window;          // 前方宣言

enum PopupAnchor { Anchor_None, Anchor_Element, Anchor_ParentWindow, Anchor_Screen };

enum NativeWindowType {
	WindowType_Main,        // メインウィンドウ（bgfx init済み、FBO不要）
	WindowType_Popup,       // SDL_CreatePopupWindow（親の子、ドロップダウン等）
	WindowType_AlwaysOnTop, // SDL_CreateWindow + SDL_WINDOW_ALWAYS_ON_TOP（アラート等）
	WindowType_Modal,       // SDL_CreateWindow + SDL_WINDOW_MODAL（親操作ブロック）
};

struct NativeWindow {
    SDL_Window* sdlWindow = nullptr;
    NativeWindow* parent = nullptr;
	bool reset = false;
    void* nwh = nullptr;
    bgfx::FrameBufferHandle fbo = BGFX_INVALID_HANDLE;  // メインは INVALID、それ以外はスワップチェーンFBO
    uint64_t viewId = 0;  // 論理viewId
    PointI size = { 0, 0 };
    NewElement* local = nullptr;

    NativeWindowType type = WindowType_Main;

    // 位置指定
    PopupAnchor anchor = Anchor_None;
    int anchorX = 0, anchorY = 0;
    NewElement* anchorElement = nullptr;

    bool needsFboInit = false;
    bool needsFboReset = false;   // リサイズ時のFBO再作成フラグ
    bool needsResize = false;     // イベントウォッチャーからのリサイズフラグ
    bool visible = true;

    // GC root node (= local / anchorElement を root 登録するため、popup destroy 時に release する)。
    // 起動時の main window は別途 startup で登録するため使わない (= NULL のまま)。
    RootNode* rootNode = nullptr;
};

void myShowWindow(ThreadGC* thgc, NativeWindow* nw);
void myHideWindow(ThreadGC* thgc, NativeWindow* nw);
void myMoveWindow(ThreadGC* thgc, NativeWindow* nw, int x, int y);
struct PointF {
	float x;
	float y;
	// 軸 index でアクセス (0=x, 1=y)。汎用ループで X/Y を切り替える時に使う。
	float& operator[](int axis)       { return axis ? y : x; }
	float  operator[](int axis) const { return axis ? y : x; }
};
struct Action {
	char* blk;
	int id;
	ThreadGC* gc;
	NewElement* elem;
};
struct MouseEvent : Action {
	int x, y;
	Point basepos;
	int action;
	NativeWindow* window;
	bool click = false;
	ExtendedRenderGroup* group;
};

inline void traceAction(ThreadGC* thgc, Action* a) {
	a->elem = (NewElement*)copy_object_young(thgc, (char*)a->elem);
	// blk: char* — does not need tracing here
	// gc: ThreadGC* — heap-allocated, not GC managed
}

inline void MouseEventCheck(ThreadGC* thgc, char* data) {
	MouseEvent* m = (MouseEvent*)data;
	traceAction(thgc, (Action*)m);
}
struct Capture {
	MouseEvent* down;
	bool (*Capture)(struct Capture*, MouseEvent*);
};
struct KeyEvent : Action {
	String* text;
	int key;
	Uint8* keys;
	bool shift, ctrl, alt;
};

inline void KeyEventCheck(ThreadGC* thgc, char* data) {
	KeyEvent* k = (KeyEvent*)data;
	traceAction(thgc, (Action*)k);
	k->text = (String*)copy_object_young(thgc, (char*)k->text);
}
enum RetMeasureType {
	RetMeasure_None = 0,
	RetMeasure_Break = 1,  // 折り返しが発生した
};
struct NewMeasure {
	NewElement* base;
	PointF pos, size;
	PointF start;
	ExtendedRenderGroup* group;
	// X-axis float state (orient==true時に使用)
	NewElement* floatelem;
	float floatLeftX = 0;
	float floatRightX = 0;
	float floatLeftBottomX = -10000;
	float floatRightBottomX = -10000;
	// Y-axis float state (orient==false時に使用)
	float floatLeftY = 0;
	float floatRightY = 0;
	float floatLeftBottomY = -10000;
	float floatRightBottomY = -10000;
	float bx = 0, by = 0;
	NewElement* breakxelem = NULL, *breakyelem = NULL;
};
struct NewGraphic {
	LayerInfo* layer;
	NewElement* offscreen;
	NewElement* orient;
	PointF pos, size;
	PointF start, end;
	ImageId im;
	bgfx::FrameBufferHandle* fb;
	PointI* fbsize;
	uint64_t viewId = 0;   // 論理viewId（描画先）
	uint64_t viewId2 = 0;  // 論理viewId（オフスクリーン書き込み先）
	ExtendedRenderGroup* group;
	// ウィンドウFBO (スクロールバー等のオーバーレイ描画用)
	bgfx::FrameBufferHandle* winFb = nullptr;
	PointI* winFbsize = nullptr;
	uint64_t winViewId = 0;
	float deltaTime = 0;
	int paint = 2;  // Offscreen::PaintType: 0=None, 1=Offscreen, 2=Command
	float zIndex = 0;  // 親から累積されるzIndex
};
struct NewEndElement {
	struct NewElement* next, * before, * parent, * childend;
	enum LetterType type;
	PointF pos2;
	ThreadGC* gc;
	int (*Mouse)(ThreadGC*, NewElement*, MouseEvent*, PointF, NewLocal*);
	int (*Key)(ThreadGC*, NewElement*, int, int, KeyEvent*, NewLocal*);
	void (*DrawSelection) (ThreadGC*, NewLocal*, NewElement*, int, int, NewGraphic*, PointF pos, RenderCommandQueue* q);
	MemFunc* GoMouseDown; MemFunc* BackMouseDown; MemFunc* GoKeyDown; MemFunc* BackKeyDown;
	int (*len)(NewElement* elem);
	uint64_t id;
};
// 多くの UI struct の base。派生側からも使うので inline で公開。
// 関数ポインタ (Mouse/Key/DrawSelection/len) と ThreadGC* (= GC 対象外) は trace しない。
inline void traceNewEndElement(ThreadGC* thgc, NewEndElement* e) {
	e->next     = (NewElement*)copy_object_young(thgc, (char*)e->next);
	e->before   = (NewElement*)copy_object_young(thgc, (char*)e->before);
	e->parent   = (NewElement*)copy_object_young(thgc, (char*)e->parent);
	e->childend = (NewElement*)copy_object_young(thgc, (char*)e->childend);
	e->GoMouseDown   = (MemFunc*)copy_object_young(thgc, (char*)e->GoMouseDown);
	e->BackMouseDown = (MemFunc*)copy_object_young(thgc, (char*)e->BackMouseDown);
	e->GoKeyDown     = (MemFunc*)copy_object_young(thgc, (char*)e->GoKeyDown);
	e->BackKeyDown   = (MemFunc*)copy_object_young(thgc, (char*)e->BackKeyDown);
}
inline void NewEndElementCheck(ThreadGC* thgc, char* data) {
	traceNewEndElement(thgc, (NewEndElement*)data);
}
// CType::_EndC 用 (= NewEndElement と同じ tracer)
inline void EndCCheck(ThreadGC* thgc, char* data) {
	traceNewEndElement(thgc, (NewEndElement*)data);
}
int len1(NewElement* elem) {
	return 1;
}
int len0(NewElement* elem) {
	return 0;
}
// ============================================================
// SIVal / SIValues  (シンプル版)
//
// SIVal: 8 byte、上位 3 bit が type tag、下位 61 bit がペイロード。
//   ペイロードは値型のみ (UIntPair / UIntFloat / UInt64 / ChangeCallback)。
//   GC ポインタ (String* 等) は持たない。
//
// SIValues: { int n; SIVal values[]; } の素朴な flex array。
//   余計なメタデータ (capacity / strCap / strings 等) は持たない。
//   GC 連携は後で必要になったら別途設定する。
// ============================================================
// SIType は最初から「上位 3bit に位置決め済み」の固定値で持つ。
// こうすれば SIVal にセットするときも取り出すときもシフト演算が不要で、
// OR / AND だけで完結する。
constexpr int      SIV_TAG_SHIFT = 61;
constexpr uint64_t SIV_TAG_MASK  = 0xE000000000000000ULL;
constexpr uint64_t SIV_VAL_MASK  = 0x1FFFFFFFFFFFFFFFULL;

enum class SIType : uint64_t {
	None           = 0ULL << SIV_TAG_SHIFT,   // 0x0000000000000000
	UIntInt        = 1ULL << SIV_TAG_SHIFT,   // 0x2000000000000000  hi29 + lo32 (uint)
	UIntFloat      = 2ULL << SIV_TAG_SHIFT,   // 0x4000000000000000  hi29 + lo32 (float)
	UInt64         = 3ULL << SIV_TAG_SHIFT,   // 0x6000000000000000  61 bit unsigned
	ChangeCallback = 4ULL << SIV_TAG_SHIFT,   // 0x8000000000000000  関数ポインタ
	String         = 5ULL << SIV_TAG_SHIFT,   // 0xA000000000000000  String* (GC ポインタ)
	Element        = 6ULL << SIV_TAG_SHIFT,   // 0xA000000000000000  String* (GC ポインタ)
	// 6, 7 reserved
};
// SIValues の固定スロット数 + 末尾予約 (framework 用)。
//   [0..Bcount-3]    = ユーザーデータ (4 slots)
//   [Bcount-2]       = TABLE_POS_SLOT  (Table 登録セルの (rid, cid) UIntPair。未登録は None)
//   [Bcount-1]       = CALLBACK_SLOT   (値変更時に Setter が発火させる ChangeCallback)
#define Bcount 6
constexpr int TABLE_POS_SLOT = Bcount - 2;
constexpr int CALLBACK_SLOT  = Bcount - 1;
// ユーザースロット (= 0..Bcount-3) の slot 0: パス用 (= String*)。
// 同じ slot だが別エレメントタイプで使い分ける (= 衝突しない):
//   - viewer NewTable: FILE_PATH_SLOT として「開いたファイルパス」を保存
//   - 変数 inspector の col-0 cell (NewLine): VAR_PATH_SLOT として
//     「root → leaf 名連結 ('\x1F' 区切り) の変数 path」を保存
constexpr int FILE_PATH_SLOT = 0;
constexpr int VAR_PATH_SLOT  = 0;   // = FILE_PATH_SLOT (別エレメントなので OK)
// 「more」行用 offset (= UInt32)。click 時にこの offset で次バッチを driver に要求する。
//   通常の row (= データ持ち or primitive) は未設定 (None)。
//   「(...)」表示の more 行のみ設定 (= driver から nextOffset > 0 で送られた行)。
constexpr int MORE_OFFSET_SLOT = 1;

typedef void (*SIValChangeCallback)(NewElement*, int);

// SIVal: 8B union。元のフィールドアクセス (lo/hi/f/u64/callback/str) を残しつつ、
// bits で全体ビットも触れる。x64 LE 前提:
//   offset 0 → lo(uint32) / f(float)
//   offset 4 → hi(uint32, 上位 3bit が type tag)
// String* / 関数ポインタは x64 canonical address (上位 16bit ゼロ) なので、
// 上位 3bit に tag を載せても下位 48bit のポインタ実体は保てる。
// ただし str / callback の生フィールド読みは tag 込みのビットを返すので、
// 取り出しは SIVal_string / SIVal_callback 経由で SIV_VAL_MASK を必ずかける。
union SIVal {                           // 8B
	uint64_t bits;                      // 全体 (タグ込み)
	uint64_t u64;                       // UInt64 用
	SIValChangeCallback callback;       // ChangeCallback 用 (tag 込み生ビット)
	String*  str;
	NewElement* elm;
	struct { int32_t lo;  uint32_t hi; };   // UIntPair  : hi の上位 3bit が tag
	struct { float    f;   uint32_t fhi; };  // UIntFloat : f は値、fhi に tag + hi29
};

// 上位 3bit を AND で取り出すだけ。シフト不要。
inline SIType SIVal_type(SIVal v) {
	return (SIType)(v.bits & SIV_TAG_MASK);
}

inline SIValChangeCallback SIVal_callback(SIVal v) {
	return (SIValChangeCallback)(v.bits & SIV_VAL_MASK);
}

// ── SIValues コンテナ (n + flex array values) ──
struct SIValues {
	int   n;            // values の要素数
	SIVal values[1];    // 実体は makeSIValues で確保した n 個 (flex array)
};
// SIVal の中で String / Element 型のものは GC ptr (= 上位 3bit tag 付き)。
// trace 時は tag を保ったまま下位 ptr を更新する。
inline void SIValuesCheck(ThreadGC* thgc, char* data) {
	SIValues* sv = (SIValues*)data;
	for (int i = 0; i < sv->n; ++i) {
		SIVal& v = sv->values[i];
		SIType t = SIVal_type(v);
		if (t == SIType::String) {
			String* s  = (String*)(v.bits & SIV_VAL_MASK);
			String* cs = (String*)copy_object_young(thgc, (char*)s);
			v.bits = (uint64_t)SIType::String | ((uint64_t)cs & SIV_VAL_MASK);
		}
		else if (t == SIType::Element) {
			NewElement* e  = (NewElement*)(v.bits & SIV_VAL_MASK);
			NewElement* ce = (NewElement*)copy_object_young(thgc, (char*)e);
			v.bits = (uint64_t)SIType::Element | ((uint64_t)ce & SIV_VAL_MASK);
		}
	}
}

// ── 抽象 API ──
// SIValues 1 ブロックを GC_alloc_size で確保 (header + values[n])。
inline SIValues* makeSIValues(ThreadGC* thgc, int n) {
	uint32_t bytes = (uint32_t)(sizeof(SIValues) + (n > 0 ? n - 1 : 0) * sizeof(SIVal));
	SIValues* sv = (SIValues*)GC_alloc_size(thgc, bytes);
	sv->n = n;
	return sv;
}

// SIValues を Bcount 以上に確保 (なければ作成、足りなければクローンして拡張)。
// NewElement の定義を必要とするので forward 宣言、実装はもっと下 (NewElement 定義後)。
inline void ensureSIValues(ThreadGC* thgc, NewElement* cell);

// 取得: 期待 type を引数で受け取る。tag が一致しなければ assert (= 開発エラー)。
//   範囲外 n も assert で落とす想定。
//   返り値は tag を AND で消した純粋な value。
inline SIVal getSIValue(SIValues* values, int n, SIType expectedType) {
	assert((unsigned)n < (unsigned)values->n);
	SIVal v = values->values[n];
	assert(SIVal_type(v) == expectedType);
	v.bits &= SIV_VAL_MASK;
	return v;
}

// 代入 (素の Setter): 範囲外なら何もしない。
// SIType (既に上位 3bit に位置決め済み) と SIVal を OR/AND するだけ。
inline void setSIValue(SIValues* values, int n, SIType type, SIVal val) {
	if ((unsigned)n >= (unsigned)values->n) return;
	values->values[n].bits = (val.bits & SIV_VAL_MASK) | (uint64_t)type;
}

// Setter: setSIValue で書き込み → 固定 CALLBACK_SLOT が ChangeCallback なら発火。
//   - スロットレイアウト固定 (Bcount=6, [0..3]=user, [4]=TABLE_POS, [5]=CALLBACK) に合わせ、
//     callback の場所も [CALLBACK_SLOT] 固定。「次のスロット」では見ない。
//   - 予約スロット (TABLE_POS_SLOT / CALLBACK_SLOT) を Setter で書き換えても発火させない
//     (= 再帰防止 + 位置/コールバック自体の更新では何も鳴らない)。
//   - 範囲外 n は no-op。
inline void Setter(NewElement* elem, SIValues* values, int n, SIType type, SIVal val) {
	if ((unsigned)n >= (unsigned)values->n) return;
	setSIValue(values, n, type, val);
	if (n == TABLE_POS_SLOT || n == CALLBACK_SLOT) return;
	if (CALLBACK_SLOT >= values->n) return;
	SIVal cbSlot = values->values[CALLBACK_SLOT];
	if (SIVal_type(cbSlot) == SIType::ChangeCallback) {
		SIValChangeCallback cb = SIVal_callback(cbSlot);
		if (cb) cb(elem, n);
	}
}
struct NewLocal;
struct NewElement : NewEndElement {
	float margins[4];
	float mbefores[4];
	float paddings[4];
	float pbefores[4];
	float opacity;
	PointF pos, size, size2, scroll;
	float zIndex;
	SizeType xtype, ytype;
	AlignType xalign, yalign;
	Position position;
	Layout layout;
	Background* background;
	float boWidthes[4];  // [0]=top, [1]=right, [2]=bottom, [3]=left
	bool orient;
	RetMeasureType (*Measure)(ThreadGC*, struct NewElement*, NewMeasure*, NewLocal*, int*);
	void (*Draw)(ThreadGC*, struct NewElement*, NewGraphic*, NewLocal*, RenderCommandQueue* q);
	FontId font;
	bool offscreened = false;
	Offscreen* offscreen;
	SIValues* values;
	EditType editable;
	uint32_t hoverColor = 0;
	bool hovered = false;
	float animate = -1;
	bool disabled;
};
// NewElement = NewEndElement + bg/offscreen/values。派生からも使う。
inline void traceNewElement(ThreadGC* thgc, NewElement* e) {
	traceNewEndElement(thgc, (NewEndElement*)e);
	e->background = (Background*)copy_object_young(thgc, (char*)e->background);
	e->offscreen  = (Offscreen*) copy_object_young(thgc, (char*)e->offscreen);
	e->values     = (SIValues*)  copy_object_young(thgc, (char*)e->values);
}
inline void NewElementCheck(ThreadGC* thgc, char* data) {
	traceNewElement(thgc, (NewElement*)data);
}
// SIValues 確保 (forward 宣言済み): NewElement 定義後に実装。
inline void ensureSIValues(ThreadGC* thgc, NewElement* cell) {
	if (!cell->values) {
		cell->values = makeSIValues(thgc, Bcount);
		return;
	}
	if (cell->values->n >= Bcount) return;
	SIValues* sv = makeSIValues(thgc, Bcount);
	int oldN = cell->values->n;
	for (int k = 0; k < oldN; k++) sv->values[k] = cell->values->values[k];
	cell->values = sv;
}
// border半径ヘルパー: background==NULLなら0 (boRadiusesはBackgroundに所属)
inline float boRadius(NewElement* e, int i) { return e->background ? e->background->bs.boRadiuses[i] : 0.0f; }
// パーセントサイズ設定: 小数部分=比率、整数部分=Auto時フォールバック
// pct: 比率 (0.5 = 50%, 1.0 = 100%), autoSize: Auto親のフォールバックpx
// base: 合成するベースタイプ (Break, Scroll等)。省略時はPercent単体(=Visible)
inline void setPercentX(NewElement* elem, float pct, float autoSize = 0, SizeType base = SizeType::Auto) {
	elem->size.x = floorf(autoSize) + ((pct >= 1.0f) ? 0.0f : pct);
	elem->xtype = SizeType::Percent | base;
}
inline void setPercentY(NewElement* elem, float pct, float autoSize = 0, SizeType base = SizeType::Auto) {
	elem->size.y = floorf(autoSize) + ((pct >= 1.0f) ? 0.0f : pct);
	elem->ytype = SizeType::Percent | base;
}
enum DirtyType {
	None,
	Partial = 1,
	OffscreenLayout = 2,
	Rebuild = 4,
	RebuildValue = 7,
};
struct NewSelect {
	NewElement* from, * to;
	uint64_t fromid, toid;
	int m, n;
	NewElement* start, * end;
	int s, e;
	int count;
	NativeWindow* window = nullptr; // fromが属するウィンドウ
};
struct ScrollDrag {
	NewElement* elem = nullptr;  // ドラッグ中の要素 (NULLならドラッグなし)
	int axis = 0;                // 0=Y(右バー), 1=X(下バー)
	float offset = 0;            // つまみ内のクリック位置オフセット
};
struct NewLocal : NewElement {
	bool resetid;
	NativeWindow* window;
	DirtyType dirty;
	List* screens;
	Map* temap;
	Map* selects;
	NewSelect select;
	List* imageids;
	NewElement* linked;  // このLocalを表示しているLinkedElement (NULLなら直接配置)
	ScrollDrag scrollDrag;
};
inline void NewLocalCheck(ThreadGC* thgc, char* data) {
	NewLocal* l = (NewLocal*)data;
	traceNewElement(thgc, (NewElement*)l);
	l->screens   = (List*)copy_object_young(thgc, (char*)l->screens);
	l->temap     = (Map*) copy_object_young(thgc, (char*)l->temap);
	l->selects   = (Map*) copy_object_young(thgc, (char*)l->selects);
	l->imageids  = (List*)copy_object_young(thgc, (char*)l->imageids);
	l->linked    = (NewElement*)copy_object_young(thgc, (char*)l->linked);
	// select / scrollDrag 内の GC pointer も forward (= 漏れていると stale で render 異常)。
	l->select.from   = (NewElement*)copy_object_young(thgc, (char*)l->select.from);
	l->select.to     = (NewElement*)copy_object_young(thgc, (char*)l->select.to);
	l->select.start  = (NewElement*)copy_object_young(thgc, (char*)l->select.start);
	l->select.end    = (NewElement*)copy_object_young(thgc, (char*)l->select.end);
	l->scrollDrag.elem = (NewElement*)copy_object_young(thgc, (char*)l->scrollDrag.elem);
}
struct RenderGroup;
enum class OffscreenType {
	EndOff, Off, LinkedOff
};
struct OffscreenEnd {
	OffscreenType type;
	Offscreen* next, * before;
	Offscreen* child, * parent;
	int id;
};
inline void OffscreenEndCheck(ThreadGC* thgc, char* data) {
	OffscreenEnd* o = (OffscreenEnd*)data;
	o->next   = (Offscreen*)copy_object_young(thgc, (char*)o->next);
	o->before = (Offscreen*)copy_object_young(thgc, (char*)o->before);
	o->child  = (Offscreen*)copy_object_young(thgc, (char*)o->child);
	o->parent = (Offscreen*)copy_object_young(thgc, (char*)o->parent);
}
struct Offscreen : OffscreenEnd {
	ImageId imPing;
	ImageId imPong;
	bool ping = true;
	bool layout = true;
	enum PaintType { PaintNone = 0, PaintOffscreen = 1, PaintCommand = 2 };
	PaintType paint = PaintCommand;
	NewElement* elem;
	NativeWindow* window = nullptr;  // 所属するウィンドウ
	// source が指定された場合、その elem が「offscreen 持つ Table の直下 cell」なら
	// Table の dirtyCells に登録する (= cell-level 差分 measure 用)。それ以外は無視。
	// source = nullptr (default) で従来挙動。
	void markLayout(NewLocal* local, NewElement* source);
	static void markLayoutChildren(NewLocal* local, Offscreen* off);
	void markPaint(NewLocal* local);
	static void markPaintChildren(Offscreen* parent);
	uint64_t viewId = 0;  // 論理viewId
	ExtendedRenderGroup *group;
	bool dirty = false;
	// 更新世代 (markLayout で ++)。「欲しい版」。
	// fbo を持つ各 unit (StandaloneTextureInfo / TiledTextureInfo::Tile) の heldGen と比較される。
	uint32_t updateGen = 1;
	// ページめくりアニメーション
	//   curl は 1 回のアニメで fromPage → toPage を遷移する (= shader が両端の
	//   UV だけサンプルするため、N ページ離れていても中間ページは描画されない)。
	float curlProgress = -1.0f;  // -1=アニメなし, 0〜1=めくり中
	float curlFromPage = 0;      // めくり元ページ
	float curlToPage = 0;        // めくり先ページ (= currentPage との差は任意)
	int curlAxis = 0;            // 0=Y, 1=X
	bool spread = false;         // true=両開き, false=片面開き
};
inline void OffscreenCheck(ThreadGC* thgc, char* data) {
	Offscreen* o = (Offscreen*)data;
	o->next   = (Offscreen*) copy_object_young(thgc, (char*)o->next);
	o->before = (Offscreen*) copy_object_young(thgc, (char*)o->before);
	o->child  = (Offscreen*) copy_object_young(thgc, (char*)o->child);
	o->parent = (Offscreen*) copy_object_young(thgc, (char*)o->parent);
	o->elem   = (NewElement*)copy_object_young(thgc, (char*)o->elem);
}
static uint64_t viewId = 0;  // グローバル論理viewIdカウンタ

struct NewLine : NewElement {
	int line = 0; // C# Line.line — 行番号
};
inline void NewLineCheck(ThreadGC* thgc, char* data) {
	traceNewElement(thgc, (NewElement*)data);   // NewLine の追加 GC ptr 無し (= int のみ)
}
struct ATSSpan {
	int start, end;
	FontId font;
	uint32_t color;
	String* text;
	LetterType t;
	String* error;
	bool hovered = false;
	uint32_t hoverColor = 0;
	uint32_t underlineColor = 0;
	uint32_t hoverUnderlineColor = 0;
	NewElement* letter = nullptr; // C# letter → NewLetter element, set in LetterMeasure
};
inline void ATSSpanCheck(ThreadGC* thgc, char* data) {
	ATSSpan* a = (ATSSpan*)data;
	a->text   = (String*)copy_object_young(thgc, (char*)a->text);
	a->error  = (String*)copy_object_young(thgc, (char*)a->error);
	a->letter = (NewElement*)copy_object_young(thgc, (char*)a->letter);
}
struct StyleSpan {
	int start, end;
	FontId font;
	uint32_t color;
	bool hovered = false;
	uint32_t hoverColor = 0;
	uint32_t underlineColor = 0;
	uint32_t hoverUnderlineColor = 0;
};
inline void StyleSpanCheck(ThreadGC* /*thgc*/, char* /*data*/) {
	// StyleSpan は GC ptr 持たない (= 全部 POD)
}
struct RenderSpan {
	int start, end;
	FontId font;
	uint32_t color;
	float x, y, width, height;
	ATSSpan* atsspan = nullptr;
	StyleSpan* stylespan = nullptr;
};
inline void RenderSpanCheck(ThreadGC* thgc, char* data) {
	RenderSpan* r = (RenderSpan*)data;
	r->atsspan   = (ATSSpan*)  copy_object_young(thgc, (char*)r->atsspan);
	r->stylespan = (StyleSpan*)copy_object_young(thgc, (char*)r->stylespan);
}
struct Master;
struct TextBox : NewElement {
	//ReCompile* recompile;
	Master* master;
	bool recompile;
};
inline void TextBoxCheck(ThreadGC* thgc, char* data) {
	TextBox* tb = (TextBox*)data;
	traceNewElement(thgc, (NewElement*)tb);
	tb->master = (Master*)copy_object_young(thgc, (char*)tb->master);
}
struct BreakLetter;
struct NewLetter : NewElement {
	String* text;
	uint32_t color;
	List* atsspans;
	List* stylespans;
	List* renderspans;
	List* breakletters; // 生成したBreakLetterのリスト（次回Measureで削除用）
	bool recompile;
};
inline void NewLetterCheck(ThreadGC* thgc, char* data) {
	NewLetter* l = (NewLetter*)data;
	traceNewElement(thgc, (NewElement*)l);
	l->text         = (String*)copy_object_young(thgc, (char*)l->text);
	l->atsspans     = (List*)  copy_object_young(thgc, (char*)l->atsspans);
	l->stylespans   = (List*)  copy_object_young(thgc, (char*)l->stylespans);
	l->renderspans  = (List*)  copy_object_young(thgc, (char*)l->renderspans);
	l->breakletters = (List*)  copy_object_young(thgc, (char*)l->breakletters);
}
struct BreakLetter : NewElement {
	NewLetter* origin;    // 元のLetter
	List* renderspans;    // この視覚行のrenderspans
	List* boundaries;     // 境界リスト (origin由来)
	int charPos;          // 継続するテキスト位置
	bool measured = false; // LetterMeasureで生成/更新されたらtrue、BreakLetterMeasureでチェック
};
inline void BreakLetterCheck(ThreadGC* thgc, char* data) {
	BreakLetter* b = (BreakLetter*)data;
	traceNewElement(thgc, (NewElement*)b);
	b->origin      = (NewLetter*)copy_object_young(thgc, (char*)b->origin);
	b->renderspans = (List*)     copy_object_young(thgc, (char*)b->renderspans);
	b->boundaries  = (List*)     copy_object_young(thgc, (char*)b->boundaries);
}
struct Sidelet : NewElement {
	bool hide;
	int direction;
	float percent;
};
inline void SideletCheck(ThreadGC* thgc, char* data) {
	traceNewElement(thgc, (NewElement*)data);
}
struct HoveredSpan {
	NewLetter* letter;
	RenderSpan* span;
	ATSSpan* atsspan;
	StyleSpan* stylespan;
};
inline void HoveredSpanCheck(ThreadGC* thgc, char* data) {
	HoveredSpan* h = (HoveredSpan*)data;
	h->letter   = (NewLetter*) copy_object_young(thgc, (char*)h->letter);
	h->span     = (RenderSpan*)copy_object_young(thgc, (char*)h->span);
	h->atsspan  = (ATSSpan*)   copy_object_young(thgc, (char*)h->atsspan);
	h->stylespan= (StyleSpan*) copy_object_young(thgc, (char*)h->stylespan);
}
void initLetter(ThreadGC* thgc, NewLetter* letter, FontId font, enum LetterType type);
void initBreakLetter(ThreadGC* thgc, BreakLetter* bl, NewLetter* origin);
void BreakLetterDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q);
void BreakLetterDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q);
int BreakLetterMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local);
void ElementDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q);
void EndDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q);
void LetterDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q);
void FireEvent(ThreadGC* thgc, NewElement* elem, MemFunc* handler, CType eventType, void* eventData, size_t eventSize) {
	char* ecopy = GC_alloc(elem->gc, eventType);
	memcpy(ecopy, eventData, eventSize);
	Action* act = (Action*)ecopy;
	act->gc = elem->gc;
	act->elem = elem;
	MemObj* mo = (MemObj*)GC_clone(elem->gc, (char*)handler->obj);
	MemTable* res = (MemTable*)GC_alloc(elem->gc, _MemTable);
	res->table = (Map*)create_mapy(elem->gc, _List);
	MemInsert(elem->gc, res, eventType, (ModelVal*)ecopy);
	mo->req = NULL; mo->res = res;
	mo->action = act;
	auto rn = GC_add_root_node(elem->gc);
	auto h = std::coroutine_handle<Generator::promise_type>::from_address(static_cast<void*>(MakeFrame(rn, handler->func, (char*)mo)));
	h.promise().state = -2;
	elem->gc->queue->push(h);
}
int ElementKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local);
int EndKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local);
int LineMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local);
int ElementMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local);
int LetterMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local);
int LetterKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local);
Offscreen* FindOffscreen(NewElement* elem) {
	for (; elem != NULL; elem = elem->parent) {
		if (elem->type == LetterType::_ElemEnd) continue;
		if (elem->offscreen != NULL) return elem->offscreen;
	}
	return NULL;
}
// elem の祖先 offscreen に対して markLayout を呼ぶ (= source = elem を渡す)。
//   FindOffscreen が NULL を返すケースを安全に扱い、cell-level dirty 追跡 (= Table 親なら
//   dirtyCells 登録) も自動で発火する。`markLayoutOf(elem, local)` の置換用。
inline void markLayoutOf(NewElement* elem, NewLocal* local) {
	Offscreen* off = FindOffscreen(elem);
	off->markLayout(local, elem);
}
// BreakLetterをスキップして次/前の要素を取得
inline NewElement* skipBreakNext(NewElement* e) {
	while (e->type == LetterType::_BreakLetter) e = e->next;
	return e;
}
inline NewElement* skipBreakBefore(NewElement* e) {
	while (e->type == LetterType::_BreakLetter) e = e->before;
	return e;
}
// Measure / Draw 用の no-op スタブ。NewElement::Measure / Draw に渡して、
//   initEndBodyElement だけで初期化された (= 各要素固有 init を通っていない) NewElement の
//   Measure / Draw が呼ばれてもクラッシュしないことを保証する。各要素 (Table / Letter 等) は
//   後段で実装関数 (ElementMeasure / NewTableMeasure / ...) に上書きする。
inline RetMeasureType EndMeasure(ThreadGC*, NewElement*, NewMeasure*, NewLocal*, int*) {
	return RetMeasure_None;
}
inline void EndDraw(ThreadGC*, NewElement*, NewGraphic*, NewLocal*, RenderCommandQueue*) {
}

// === NewTable 行操作の forward 宣言 (実装は othelem.h) ===
//   refLine の親が _TableElem (= NewTable) のとき、refLine の行位置に新しい行を挿入し、
//   newLine を refLine と同じ列に配置する。親が Table 以外なら no-op。
//   UniteLine 等で行 elem を消す前に呼ぶことで、Table と Linked-list の両方が同期する。
void tableInsertRowForLine(ThreadGC* thgc, NewElement* refLine, NewElement* newLine);
void tableRemoveRowForLine(ThreadGC* thgc, NewElement* lineToRemove);

void initNewEndElement(ThreadGC* thgc, NewEndElement* end, NewElement* parent) {
	end->gc = thgc;
	end->next = end->before = (NewElement*)end;
	end->parent = parent;
	end->type = LetterType::_ElemEnd;
	end->id = 0;
	end->len = len1;
	end->Mouse = ElementMouse;
	end->DrawSelection = EndDrawSelect;
	end->Key = EndKey;
	end->len = len1;
}
// body content (= NewElement) 用の初期化。
//   EndElement 部分は initNewEndElement をそのまま流用し、
//   ここでは NewElement 拡張側の関数ポインタ (Measure / Draw) に no-op スタブをセットするだけ。
//   各要素固有の init は後段で実装関数に上書きする (例: NewTable は initTable で Measure = NewTableMeasure)。
void initEndBodyElement(ThreadGC* thgc, NewElement* elem) {
	initNewEndElement(thgc, (NewEndElement*)elem, NULL);
	elem->Measure = EndMeasure;
	elem->Draw    = EndDraw;
}
void RootResetId(NewElement* elem, int* n) {
	for (NewElement* child = elem->childend->next; child->type != LetterType::_ElemEnd; child = child->next) {
		child->id = (uint64_t)++(*n) * 256ULL * 65536ULL * 65536ULL;
		if (child->childend != NULL) RootResetId(child, n);
	}
	elem->childend->id = (uint64_t)++(*n) * 256ULL * 65536ULL * 65536ULL;
}
void AllResetId(NewLocal* local, NativeWindow* nw) {
	int n = 0;
	local->resetid = false;
	RootResetId(nw->local, &n);
}
void MarkResetId(NewLocal* local, NewElement* elem) {
	Offscreen* off = FindOffscreen(elem);
	local->resetid = true;
	off->window->reset = true;
}
void SetChildIds(NewLocal* local, NewElement* elem) {

	uint64_t startId = elem->id;
	uint64_t endId = elem->childend->id;

	// �q�v�f�̐��𐔂���
	int childCount = 0;
	for (auto el = elem->childend->next; el->type != LetterType::_ElemEnd; el = el->next) {
		childCount++;
		if (el->childend != NULL) {
			childCount++;  // childend����
		}
	}

	if (childCount == 0) return;

	// �e�q�v�f��ID�����蓖��
	uint64_t step = (endId - startId) / (childCount + 1);
	uint64_t currentId = startId;
	if (step == 0) {
		MarkResetId(local, elem);
	}
	for (auto el = elem->childend->next; el->type != LetterType::_ElemEnd; el = el->next) {
		currentId += step;
		el->id = currentId;

		if (el->childend != NULL) {
			currentId += step;
			el->childend->id = currentId;
			SetChildIds(local, el);  // �ċA
		}
	}
}
bool CheckOffscreen(NewElement* elem) {
	if (elem->offscreen || baseType(elem->xtype) == SizeType::Page || baseType(elem->xtype) == SizeType::Scroll || baseType(elem->ytype) == SizeType::Page || baseType(elem->ytype) == SizeType::Scroll) {
		return true;
	}
	return false;

}
//elem->id
//elem->child-id
//elem->childend->id
//next->id
void NewBefore(ThreadGC* thgc, NewLocal* local, NewElement* self, NewElement* elem) {
	NewElement* before;
	if (self->before->type == LetterType::_ElemEnd) {
		before = self->parent;
	}
	else {
		before = self->before;
		if (before->childend != NULL) before = before->childend;
	}
	elem->before->next = elem->next;
	elem->next->before = elem->before;
	elem->next = self;
	elem->before = self->before;
	self->before->next = elem;
	self->before = elem;
	elem->parent = self->parent;
	if (elem->childend != NULL) {
		if (self->id - before->id <= 2) MarkResetId(local, elem);
		else {
			elem->id = self->id - (self->id - before->id) * 2 / 3;
			elem->childend->id = self->id - (self ->id - before->id) / 3;
		}
		SetChildIds(local, elem);
	}
	else {
		if (self->id - before->id <= 1) MarkResetId(local, elem);
		else elem->id = (self->id + before->id) / 2;
	}
}
void NewNext(ThreadGC* thgc, NewLocal* local, NewElement* self, NewElement* elem) {
	NewElement* next = self->next;
	NewElement* before;
	if (self->type == LetterType::_ElemEnd) {
		before = self->parent;
	}
	else {
		before = self;
		if (next->childend != NULL) next = next->childend->next;
	}
	elem->before->next = elem->next;
	elem->next->before = elem->before;
	elem->before = self;
	elem->next = self->next;
	self->next->before = elem;
	self->next = elem;
	elem->parent = self->parent;
	if (elem->childend != NULL) {
		if (next->id - before->id <= 2) MarkResetId(local, elem);
		else {
			elem->id = before->id + (next->id - before->id) / 3;
			elem->childend->id = before->id + (next->id - before->id) / 3 * 2;
		}
		SetChildIds(local, elem);
	}
	else {
		if (next->id - before->id <= 1) MarkResetId(local, elem);
		else elem->id = (before->id + next->id) / 2;
	}
}
void NewRemove(ThreadGC* thgc, NewLocal* local, NewElement* elem) {
	elem->before->next = elem->next;
	elem->next->before = elem->before;
	//elem->parent = NULL;
}
void RebuildOffscreen(ThreadGC* thgc, Offscreen* off, NewElement* elem, int *n) {
	for(NewElement* child = elem->childend->next; child->type != LetterType::_ElemEnd; child = child->next) {
		child->id = ++(*n) * 65535ULL * 65535ULL * 65535ULL;
		if (child->offscreen != NULL) {
			child->offscreen->next = off;
			child->offscreen->before = off->before;
			off->before->next = child->offscreen;
			off->before = child->offscreen;
			child->offscreen->parent = off->parent;
			child->offscreen->child->next = child->offscreen->child->before = child->offscreen->child;
			child->offscreen->child->parent = child->offscreen;
			if (child->childend != NULL) RebuildOffscreen(thgc, child->offscreen->child, child, n);
		}
		else if (child->childend != NULL) {
			RebuildOffscreen(thgc, off, child, n);
		}
	}
	elem->childend->id = ++(*n) * 65535ULL * 65535ULL * 65535ULL;
}
void BeforeOffscreen(NewLocal* local, Offscreen* next, Offscreen* screen) {
	screen->next->before = screen->before;
	screen->before->next = screen->next;
	screen->before = next->before;
	screen->next = next;
	next->before->next = screen;
	next->before = screen;
	screen->paint = next->parent ? Offscreen::PaintCommand : Offscreen::PaintNone;
	screen->window = next->parent->window;
}
void NextOffscreen(NewLocal* local, Offscreen* before, Offscreen* screen) {
	screen->next->before = screen->before;
	screen->before->next = screen->next;
	screen->next = before->next;
	screen->before = before;
	before->next->before = screen;
	before->next = screen;
	screen->parent = before->parent;
	screen->window = before->parent->window;
}
void RemoveOffscreen(Offscreen* screen) {
	screen->next->before = screen->before;
	screen->before->next = screen->next;
	screen->next->before = screen;
	screen->parent = NULL;
}
Offscreen* FindNextOffscreen(NewElement* elem) {
	for (; elem->type != _ElemEnd; elem = elem->next) {
		if (elem->childend != NULL) {
			Offscreen* ret = FindNextOffscreen(elem->childend->next);
			if (ret != NULL) return ret;
		}
	}
	return NULL;
}
void RootBeforeOffscreen(ThreadGC* thgc, NewLocal* local, NewElement* elem, Offscreen* offscreen) {
	if (elem->offscreen != NULL) {
		BeforeOffscreen(local, offscreen, elem->offscreen);
		return;
	}
	if (elem->childend != NULL) {
		for (NewElement* e = elem->childend->next; e->type != _ElemEnd; e = e->next) {
			RootBeforeOffscreen(thgc, local, e, offscreen);
		}
	}
}
void RootBeforeOffscreen2(ThreadGC* thgc, NewLocal* local, NewElement* elem, Offscreen* offscreen) {
	if (CheckOffscreen(elem)) {
		elem->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
		elem->offscreen->type = OffscreenType::Off;
		elem->offscreen->group = &createGroup(thgc);
		elem->offscreen->next = elem->offscreen->before = elem->offscreen;
		elem->offscreen->elem = elem;
		elem->offscreen->type = elem->type == LetterType::_Linked?OffscreenType::LinkedOff:OffscreenType::Off;
		elem->offscreen->dirty = true;
		elem->offscreen->curlProgress = -1.0f;
		elem->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
		elem->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
		elem->offscreen->window = offscreen->parent ? offscreen->parent->window : offscreen->window;
		elem->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
		elem->offscreen->child->next = elem->offscreen->child->before = elem->offscreen->child;
		if (elem->background == NULL) {
			elem->background = (Background*)GC_alloc(thgc, CType::_Background);
			elem->background->type = DrawCommandType::Fill;
			elem->background->fillcolor = 0xffffffff;
		}
		BeforeOffscreen(local, offscreen, elem->offscreen);
		offscreen = elem->offscreen->child;
	}
	if (elem->childend != NULL) {
		for (NewElement* e = elem->childend->next; e->type != _ElemEnd; e = e->next) {
			RootBeforeOffscreen2(thgc, local, e, offscreen);
		}
	}
}
void RootBeforeOffscreen3(ThreadGC* thgc, NewLocal* local, NewElement* elem, Offscreen* offscreen) {
	if (elem->childend != NULL) {
		for (NewElement* e = elem->childend->next; e->type != _ElemEnd; e = e->next) {
			RootBeforeOffscreen(thgc, local, e, offscreen);
		}
	}
}
void RootDeleteOffscreen(ThreadGC* thgc, NewLocal* local, NewElement* elem) {
	if (elem->offscreen != NULL) {
		elem->offscreen = NULL;
		RemoveOffscreen(elem->offscreen);
	}
	if (elem->childend != NULL) {
		for (NewElement* e = elem->childend->next; e->type != _ElemEnd; e = e->next) {
			RootDeleteOffscreen(thgc, local, e);
		}
	}
}
void NewNextElement(ThreadGC* thgc, NewLocal* local, NewElement* before, NewElement* elem) {
	Offscreen* bscreen = FindOffscreen(elem);
	NewNext(thgc, local, before, elem);
	if (bscreen != NULL) {
		bscreen->markLayout(local, elem);
		Offscreen* ascreen = FindOffscreen(elem->parent);
		if (ascreen != NULL) {
			ascreen->markLayout(local, elem);
			Offscreen* screen = FindNextOffscreen(elem->next);
			if (screen == NULL) screen = ascreen->child;
			RootBeforeOffscreen(thgc, local, elem, screen);
		}
		else {
			RootDeleteOffscreen(thgc, local, elem);
		}
	}
	else {
		Offscreen* ascreen = FindOffscreen(elem->parent);
		if (ascreen != NULL) {
			ascreen->markLayout(local, elem);
			Offscreen* screen = FindNextOffscreen(elem->next);
			if (screen == NULL) screen = ascreen->child;
			RootBeforeOffscreen2(thgc, local, elem, screen);
		}
	}
}
void NewBeforeElement(ThreadGC* thgc, NewLocal* local, NewElement* before, NewElement* elem) {
	Offscreen* bscreen = FindOffscreen(elem);
	NewBefore(thgc, local, before, elem);
	if (bscreen != NULL) {
		bscreen->markLayout(local, elem);
		Offscreen* ascreen = FindOffscreen(elem->parent);
		if (ascreen != NULL) {
			ascreen->markLayout(local, elem);
			Offscreen* screen = FindNextOffscreen(elem->next);
			if (screen == NULL) screen = ascreen->child;
			RootBeforeOffscreen(thgc, local, elem, screen);
		}
		else {
			RootDeleteOffscreen(thgc, local, elem);
		}
	}
	else {
		Offscreen* ascreen = FindOffscreen(elem->parent);
		if (ascreen != NULL) {
			ascreen->markLayout(local, elem);
			Offscreen* screen = FindNextOffscreen(elem->next);
			if (screen == NULL) screen = ascreen->child;
			RootBeforeOffscreen2(thgc, local, elem, screen);
		}
	}
}
void NewRemoveElement(ThreadGC* thgc, NewLocal* local, NewElement* elem) {
	Offscreen* screen = FindOffscreen(elem);
	if (screen != NULL) {
		RootDeleteOffscreen(thgc, local, elem);
		screen->markLayout(local, elem);
	}
	NewRemove(thgc, local, elem);
}
void changeOrient(NewLocal* local, NewElement* elem, bool orient) {
	elem->orient = orient;
	Offscreen* screen = FindOffscreen(elem);
	if (screen != NULL) screen->markLayout(local, elem);
}
void changeXtype(ThreadGC* thgc, NewLocal* local, NewElement* elem, SizeType xtype) {
	if (elem->xtype == xtype) return;
	elem->xtype = xtype;
	Offscreen* off = FindOffscreen(elem);
	if (off != NULL) {
		off->markLayout(local, elem);
		if (elem->offscreen == NULL) {
			if (CheckOffscreen(elem)) {
				elem->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
				elem->offscreen->group = &createGroup(thgc);
				elem->offscreen->elem = elem;
				elem->offscreen->type = elem->type == LetterType::_Linked ? OffscreenType::LinkedOff : OffscreenType::Off;
				elem->offscreen->dirty = true;
				elem->offscreen->curlProgress = -1.0f;
				elem->offscreen->window = off->window;
				elem->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
				elem->offscreen->child->next = elem->offscreen->child->before = elem->offscreen->child;
				elem->offscreen->child->parent = elem->offscreen;
				RootBeforeOffscreen3(thgc, local, elem, elem->offscreen->child);
				Offscreen* next = FindNextOffscreen(elem->next);
				if (next == NULL) next = off->child;
				BeforeOffscreen(local, next, elem->offscreen);

			}
		}
		else {
			if (!CheckOffscreen(elem)) {
				for (Offscreen* offscreen = elem->offscreen->parent->child->before;offscreen != offscreen->parent->child; ) {
					Offscreen* before = offscreen->before;
					NextOffscreen(local, elem->offscreen, offscreen);
					offscreen = before;
				}
				RemoveOffscreen(elem->offscreen);
				elem->offscreen = NULL;
			}
		}
	}
}
void changeYType(ThreadGC* thgc, NewLocal* local, NewElement* elem, SizeType ytype) {
	if (elem->ytype == ytype) return;
	elem->ytype = ytype;
	Offscreen* off = FindOffscreen(elem);
	if (off != NULL) {
		off->markLayout(local, elem);
		if (elem->offscreen == NULL) {
			if (CheckOffscreen(elem)) {
				elem->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
				elem->offscreen->group = &createGroup(thgc);
				elem->offscreen->elem = elem;
				elem->offscreen->type = elem->type == LetterType::_Linked ? OffscreenType::LinkedOff : OffscreenType::Off;
				elem->offscreen->dirty = true;
				elem->offscreen->curlProgress = -1.0f;
				elem->offscreen->window = off->window;
				elem->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
				elem->offscreen->child->next = elem->offscreen->child->before = elem->offscreen->child;
				elem->offscreen->child->parent = elem->offscreen;
				RootBeforeOffscreen3(thgc, local, elem, elem->offscreen->child);
				Offscreen* next = FindNextOffscreen(elem->next);
				if (next == NULL) next = off->child;
				BeforeOffscreen(local, next, elem->offscreen);

			}
		}
		else {
			if (!CheckOffscreen(elem)) {
				for (Offscreen* offscreen = elem->offscreen->parent->child->before; offscreen != offscreen->parent->child; ) {
					Offscreen* before = offscreen->before;
					NextOffscreen(local, elem->offscreen, offscreen);
					offscreen = before;
				}
				RemoveOffscreen(elem->offscreen);
				elem->offscreen = NULL;
			}
		}
	}
}
void changeOffscreen(ThreadGC* thgc, NewLocal* local, NewElement* elem, bool offscreened) {
	if (elem->offscreened == offscreened) return;
	elem->offscreened = offscreened;
	Offscreen* off = FindOffscreen(elem);
	if (off != NULL) {
		off->markLayout(local, elem);
		if (elem->offscreen == NULL) {
			if (CheckOffscreen(elem)) {
				elem->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
				elem->offscreen->group = &createGroup(thgc);
				elem->offscreen->elem = elem;
				elem->offscreen->type = elem->type == LetterType::_Linked ? OffscreenType::LinkedOff : OffscreenType::Off;
				elem->offscreen->dirty = true;
				elem->offscreen->curlProgress = -1.0f;
				elem->offscreen->window = off->window;
				elem->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
				elem->offscreen->child->next = elem->offscreen->child->before = elem->offscreen->child;
				elem->offscreen->child->parent = elem->offscreen;
				RootBeforeOffscreen3(thgc, local, elem, elem->offscreen->child);
				Offscreen* next = FindNextOffscreen(elem->next);
				if (next == NULL) next = off->child;
				BeforeOffscreen(local, next, elem->offscreen);

			}
		}
		else {
			if (!CheckOffscreen(elem)) {
				for (Offscreen* offscreen = elem->offscreen->parent->child->before; offscreen != offscreen->parent->child; ) {
					Offscreen* before = offscreen->before;
					NextOffscreen(local, elem->offscreen, offscreen);
					offscreen = before;
				}
				RemoveOffscreen(elem->offscreen);
				elem->offscreen = NULL;
			}
		}
	}
}
void changeBackground(NewLocal* local, NewElement* elem, Background* back) {
	elem->background = back;
	FindOffscreen(elem)->markPaint(local);
}
void changeMargin(NewLocal* local, NewElement* elem, float top, float right, float down, float left) {
	elem->margins[0] = top;
	elem->margins[1] = right;
	elem->margins[2] = down;
	elem->margins[3] = left;
	markLayoutOf(elem, local);
}
void changePadding(NewLocal* local, NewElement* elem, float top, float right, float down, float left) {
	elem->paddings[0] = top;
	elem->paddings[1] = right;
	elem->paddings[2] = down;
	elem->paddings[3] = left;
	markLayoutOf(elem, local);
}
void changeWidth(NewLocal* local, NewElement* elem, float width) {
	elem->size.x = width;
	markLayoutOf(elem, local);
}
void changeHeight(NewLocal* local, NewElement* elem, float height) {
	elem->size.y = height;
	markLayoutOf(elem, local);
}
void setSize(NewLocal* local, NewElement* elem, float width, float height) {
	elem->size.x = width;
	elem->size.y = height;
	markLayoutOf(elem, local);
}
/*void setText(NewElement* elem, String* text) {
	if (node && node->text != text) { node->text = text; markLayoutDirty(node); }
}
void setFontSize(NewElement* elem, float size) {
	if (node && node->fontSize != size) {
		node->fontSize = size; markLayoutDirty(node);
	}
}
void setColor(NewElement* elem, uint32_t color) {
	if (node && node->color != color) {
		node->color = color; markPaintDirty(node);
	}
}// ========== �����p�����[�^�iDirty�ɂ��Ȃ��j ========== 
void setScroll(NewElement* elem, float x, float y) {
	elem->scroll.x = x;
	elem->scroll.y = y;
	//main dirty
}
void setOpacity(NewElement* elem, float opacity) {
	elem->opacity = opacity;
	//main dirty
}
void setTransform(NewElement* elem, float tx, float ty, float scale, float rot) {
	Surface* surface = findSurfaceFor(node);
	if (surface) { surface->composite.translateX = tx; surface->composite.translateY = ty; surface->composite.scaleX = scale; surface->composite.scaleY = scale; surface->composite.rotation = rot;
	}
}*/
PointF getOffscreenPosition(NewElement* elem) {
	PointF pos = { 0, 0 };
	for (NewElement* e = elem; e->parent != nullptr; e = e->parent) {
		pos.x += e->pos2.x - e->parent->scroll.x;
		pos.y += e->pos2.y - e->parent->scroll.y;
		if (e->parent->offscreen) break;
	}
	return pos;
}
// elem の絶対位置と (任意で) 累積 zIndex を返す。
// elem 自身の pos.x/pos.y も含めて積算する (= caller 側で `absPos.x + elem->pos.x + ...` する必要なし)。
// outZIndex は += で累積するので、caller 側で 0 初期化してから渡すこと
// (再帰呼び出し時はそのまま渡せて上書きされない)。
PointF getAbsolutePosition(NewElement* elem, float* outZIndex = nullptr) {
	PointF abs = { 0, 0 };
	NewElement* e = elem;
	for (; e->parent != nullptr; e = e->parent) {
		abs.x += e->pos.x + e->pos2.x - e->parent->scroll.x;
		abs.y += e->pos.y + e->pos2.y - e->parent->scroll.y;
		if (outZIndex) *outZIndex += e->zIndex;
	}
	// root: pos2.y のみ加算 (旧ロジック維持)
	abs.y += e->pos2.y;
	if (outZIndex) *outZIndex += e->zIndex;
	// LinkedElement経由で挿入されている場合、LinkedElementの絶対位置を加算
	if (e->type == LetterType::_Main && ((NewLocal*)e)->linked) {
		PointF linkedAbs = getAbsolutePosition(((NewLocal*)e)->linked, outZIndex);
		abs.x += linkedAbs.x;
		abs.y += linkedAbs.y;
	}
	return abs;
}
// スクロールバードラッグ中のマウス追従処理
void scrollDragUpdate(NewLocal* local, MouseEvent* mv) {
	ScrollDrag& drag = local->scrollDrag;
	NewElement* self = drag.elem;
	PointF absPos = getAbsolutePosition(self);
	float localX = mv->x - absPos.x;
	float localY = mv->y - absPos.y;

	float sbarx = 0, sbary = 0;
	if (baseType(self->xtype) == SizeType::Scroll) sbary += 5;
	else if (baseType(self->xtype) == SizeType::Page) sbary += 10;
	if (baseType(self->ytype) == SizeType::Scroll) sbarx += 5;
	else if (baseType(self->ytype) == SizeType::Page) sbarx += 10;
	float sizex = self->size.x + self->paddings[1] + self->paddings[3];
	float sizey = self->size.y + self->paddings[0] + self->paddings[2];

	if (drag.axis == 0) {
		// 右スクロールバー (Y軸)
		float barH = sizey - sbary;
		float maxScroll = self->size2.y - sizey;
		float thumbRatio = sizey / self->size2.y;
		float thumbH = std::max(barH * thumbRatio, 16.f);
		float newThumbY = localY - drag.offset;
		float newRatio = std::clamp(newThumbY / (barH - thumbH), 0.f, 1.f);
		self->scroll.y = newRatio * maxScroll;
	} else {
		// 下スクロールバー (X軸)
		float barW = sizex - sbarx;
		float maxScroll = self->size2.x - sizex;
		float thumbRatio = sizex / self->size2.x;
		float thumbW = std::max(barW * thumbRatio, 16.f);
		float newThumbX = localX - drag.offset;
		float newRatio = std::clamp(newThumbX / (barW - thumbW), 0.f, 1.f);
		self->scroll.x = newRatio * maxScroll;
	}
	if (self->offscreen) self->offscreen->markPaint(local);
}
void initLine(ThreadGC* thgc, NewLine* line);
void NewElementAddLast(ThreadGC* thgc, NewLocal* local, NewElement* parent, NewElement* child) {
	if (parent->childend->before->type != LetterType::_Line && parent->childend->before->type != LetterType::_Down) {
		auto line = (NewLine*)GC_alloc(thgc, CType::_LineC);
		initLine(thgc, line);
		NewBefore(thgc, local, parent->childend, line);
	}
	NewBeforeElement(thgc, local, parent->childend->before->childend, child);
}
void NewDirectAddLast(ThreadGC* thgc, NewLocal* local, NewElement* parent, NewElement* child) {
	NewBeforeElement(thgc, local, parent->childend, child);
}

// ポップアップウィンドウ管理ラッパー（GoThread → RenderThread キュー経由）
NativeWindow* myCreatePopupWindow(ThreadGC* thgc, NativeWindowType type, PopupAnchor anchor,
	int x, int y, int w, int h, int cornerRound, NewElement* anchorElem,
	bool visible);
void myResizePopupWindow(ThreadGC* thgc, NativeWindow* popup, int newW, int newH);
void myDestroyPopupWindow(ThreadGC* thgc, NativeWindow* popup);

// 旧 TreeElement struct は廃止。tag 管理フィールド (tagId / tagParent / tagChildren / tagDel)
// もすべて削除 ── thgc->map を generic な「id → NewElement*」registry として使うだけに。
// 親子追跡 / sweep / visited フラグは持たない (= レンダリング階層 (childend / next) で十分)。
#include "addelem.h"
// LineMeasure: 中間コンテナ。orient/xtype/ytype/xalign/yalignは親から取得。
RetMeasureType LineMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	NewElement* par = elem->parent;
	if (!par) return RetMeasure_None;
	bool orient = !par->orient;
	SizeType mainType = orient ? par->xtype : par->ytype;
	AlignType mainAlign = orient ? par->xalign : par->yalign;
	AlignType crossAlign = orient ? par->yalign : par->xalign;
	float mainLimit = orient ? par->size.x : par->size.y;

	NewMeasure newmeasure;
	newmeasure.pos.x = measure->pos.x + elem->margins[3] + elem->boWidthes[3] + elem->paddings[3];
	newmeasure.pos.y = measure->pos.y + elem->margins[0] + elem->boWidthes[0] + elem->paddings[0];
	newmeasure.start = measure->pos; newmeasure.group = measure->group;

	newmeasure.floatLeftX = measure->floatLeftX;
	newmeasure.floatRightX = measure->floatRightX;
	newmeasure.floatLeftBottomX = measure->floatLeftBottomX;
	newmeasure.floatRightBottomX = measure->floatRightBottomX;
	newmeasure.floatLeftY = measure->floatLeftY;
	newmeasure.floatRightY = measure->floatRightY;
	newmeasure.floatLeftBottomY = measure->floatLeftBottomY;
	newmeasure.floatRightBottomY = measure->floatRightBottomY;
	newmeasure.breakxelem = measure->breakxelem;
	newmeasure.breakyelem = measure->breakyelem;
	// orientに応じてX/Yのfloat stateを選択
	float floatLeftX, floatRightX, floatLeftBottom, floatRightBottom;
	if (orient) {
		floatLeftX = std::max(newmeasure.floatLeftX, newmeasure.pos.x);
		floatRightX = newmeasure.floatRightX;
		floatLeftBottom = newmeasure.floatLeftBottomX;
		floatRightBottom = newmeasure.floatRightBottomX;
	} else {
		floatLeftX = std::max(newmeasure.floatLeftY, newmeasure.pos.y);
		floatRightX = newmeasure.floatRightY;
		floatLeftBottom = newmeasure.floatLeftBottomY;
		floatRightBottom = newmeasure.floatRightBottomY;
	}
	newmeasure.bx = newmeasure.pos.x; newmeasure.by = newmeasure.pos.y;
	float sizex = 0, sizey = 0;
	float szx = 0, szy = 0;
	NewElement* first = elem->childend->next;
	for (NewElement* child = elem->childend->next; child != elem->childend; ) {
		// パーセントサイズ解決: float領域に対する比率
		if (isPercent(child->xtype) && par->xtype != SizeType::Auto) {
			float pct = fmodf(child->size.x, 1.0f);
			float mult = (pct == 0.0f) ? 1.0f : pct;
			float availX = newmeasure.floatRightX - newmeasure.floatLeftX;
			child->size.x = floorf(availX * mult) + pct;
		}
		if (isPercent(child->ytype) && par->ytype != SizeType::Auto) {
			float pct = fmodf(child->size.y, 1.0f);
			float mult = (pct == 0.0f) ? 1.0f : pct;
			float availY = newmeasure.floatRightY - newmeasure.floatLeftY;
			child->size.y = floorf(availY * mult) + pct;
		}
		auto rettype = child->Measure(child->gc, child, &newmeasure, child->gc->local, n);
		float sx = child->size.x + child->boWidthes[1] + child->boWidthes[3] + child->paddings[1] + child->paddings[3] + child->margins[1] + child->margins[3];
		float sy = child->size.y + child->boWidthes[0] + child->boWidthes[2] + child->paddings[0] + child->paddings[2] + child->margins[0] + child->margins[2];
		// float配置

		if (child->layout == Layout::_ZeroNone) {
			if (elem->orient) {
				child->pos2.x = floatLeftX - newmeasure.bx;
				child->pos2.y = szy;
			}
			else {
				child->pos2.y = floatLeftX - newmeasure.by;
				child->pos2.x = szx;
			}
			child = child->next;
			continue;
		}
		if (child->layout == Layout::_OverNone) {
			if (elem->orient) {
				child->pos2.x = floatRightX - sx - newmeasure.bx;
				child->pos2.y = szy;
			}
			else {
				child->pos2.y = floatRightX - sy - newmeasure.by;
				child->pos2.x = szx;
			}
			child = child->next;
			continue;
		}
		if (child->layout == Layout::_Zero || child->layout == Layout::_Over) {
			if (elem->orient) {
				// Page境界: floatがページに収まらない場合、次ページから配置
				if (baseType(elem->ytype) == SizeType::Page) {
					float ph = elem->size.y + elem->paddings[0] + elem->paddings[2];
					float pageBottom = (std::floor(szy / ph) + 1.0f) * ph;
					if (szy + sy > pageBottom) {
						szy = pageBottom;
						sizex = 0; sizey = 0;
						newmeasure.floatLeftX = measure->floatLeftX = floatLeftX = newmeasure.bx;
						newmeasure.floatRightX = floatRightX = newmeasure.bx + elem->size.x;
						if (elem->parent) measure->floatRightX = elem->parent->size.x;
						newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = newmeasure.by;
						newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = newmeasure.by;
						if (Layout::_Zero) newmeasure.pos.x = sx;
						else newmeasure.pos.x = 0;
						newmeasure.pos.y = szy;
					}
				}
				if (child->layout == Layout::_Zero) {
					child->pos2.x = floatLeftX - newmeasure.bx;
					child->pos2.y = szy;
					floatLeftX += sx;
					newmeasure.floatLeftX = measure->floatLeftX = floatLeftX;
					if (floatLeftBottom < newmeasure.by + szy + sy) newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = newmeasure.by + szy + sy;
					measure->floatelem = elem->parent;
				}
				else {
					floatRightX -= sx;
					child->pos2.x = floatRightX - newmeasure.bx;
					child->pos2.y = szy;
					newmeasure.floatRightX = measure->floatRightX = floatRightX;
					if (floatRightBottom < newmeasure.by + szy + sy) newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = newmeasure.by + szy + sy;
					measure->floatelem = elem->parent;
				}
			}
			else {
				// Page境界: floatがページに収まらない場合、次ページから配置
				if (baseType(elem->xtype) == SizeType::Page) {
					float pw = elem->size.x + elem->paddings[1] + elem->paddings[3];
					float pageRight = (std::floor(szx / pw) + 1.0f) * pw;
					if (szx + sx > pageRight) {
						szx = pageRight;
						sizex = 0; sizey = 0;
						newmeasure.floatLeftY = measure->floatLeftY = floatLeftX = newmeasure.by;
						newmeasure.floatRightY = floatRightX = newmeasure.by + elem->size.y;
						if (elem->parent) measure->floatRightY = elem->parent->size.y;
						newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = newmeasure.bx;
						newmeasure.floatRightBottomY = measure->floatRightBottomY = floatRightBottom = newmeasure.bx;
						if (Layout::_Zero) newmeasure.pos.y = sy;
						else newmeasure.pos.y = 0;
						newmeasure.pos.x = szx;
					}
				}
				if (child->layout == Layout::_Zero) {
					child->pos2.y = floatLeftX - newmeasure.by;
					child->pos2.x = szx;
					floatLeftX += sy;
					newmeasure.floatLeftY = measure->floatLeftY = floatLeftX;
					if (floatLeftBottom < newmeasure.bx + szx + sx) newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = newmeasure.bx + szx + sx;
					measure->floatelem = elem->parent;
				}
				else {
					floatRightX -= sy;
					child->pos2.y = floatRightX - newmeasure.by;
					child->pos2.x = szx;
					newmeasure.floatRightY = floatRightX;
					measure->floatRightY -= sy;
					if (floatRightBottom < newmeasure.bx + szx + sx) newmeasure.floatRightBottomY = measure->floatRightBottomY = floatRightBottom = newmeasure.bx + szx + sx;
					measure->floatelem = elem->parent;
				}
			}
			child = child->next;
			continue;
		}
		if (elem->layout == Layout::none) {
			if (elem->orient) {
				child->pos2.x = floatLeftX - newmeasure.bx + sizex + szx;
				child->pos2.y = szy;
			}
			else {
				child->pos2.x = szx;
				child->pos2.y = floatLeftX - newmeasure.by + sizey + szy;
			}
		}
		else if (elem->orient) {
			float availW = floatRightX - floatLeftX;
			if (newmeasure.breakxelem) {
				if (rettype == RetMeasure_Break) {
					if (sizey < sy) sizey = sy;
					child->pos2.x = floatLeftX + sizex + szx;
					child->pos2.y = szy;
					newmeasure.start.y += sizey;
					newmeasure.pos.x = floatLeftX;
					newmeasure.pos.y = newmeasure.start.y;
					szy += sizey;
					sizex = 0; sizey = 0;
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatLeftBottom) { newmeasure.floatLeftX = floatLeftX = std::max(newmeasure.bx, measure->floatLeftX = getOffscreenPosition(measure->floatelem).x); newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = -10000; }
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatRightBottom) { newmeasure.floatRightX = measure->floatRightX = floatRightX = getOffscreenPosition(measure->floatelem).x + measure->floatelem->size.x; newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = -10000; }
					if (par->xalign != AlignType::ZeroAlign || par->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (availW - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (availW - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (elem->size.y - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (elem->size.y - sizey);
						}
					}
					child = child->next;
					continue;
				}
				else if (sizex + sx > availW) {
					if (par->xalign != AlignType::ZeroAlign || par->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (availW - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (availW - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (elem->size.y - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (elem->size.y - sizey);
						}
					}
					newmeasure.pos.x = floatLeftX;
					newmeasure.start.y += sizey;
					szy += sizey;
					sizex = 0; sizey = 0;
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatLeftBottom) { newmeasure.floatLeftX = floatLeftX = std::max(newmeasure.bx, measure->floatLeftX = getOffscreenPosition(measure->floatelem).x); newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = -10000; }
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatRightBottom) { newmeasure.floatRightX = measure->floatRightX = floatRightX = getOffscreenPosition(measure->floatelem).x + measure->floatelem->size.x; newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = -10000; }
				}
			}
			if (sizey < sy) sizey = sy;
			newmeasure.pos.x += sx;
			newmeasure.pos.y = newmeasure.start.y;
			child->pos2.x = floatLeftX - newmeasure.bx + sizex + szx;
			child->pos2.y = szy;
			sizex += sx;
		}
		else
		{
			float availH = floatRightX - floatLeftX;
			if (newmeasure.breakyelem) {
				if (rettype == RetMeasure_Break) {
					if (sizex < sx) sizex = sx;
					child->pos2.x = 0;
					child->pos2.y = floatLeftX + sizey + szy;
					newmeasure.start.x += sizex;
					newmeasure.pos.y = floatLeftX;
					newmeasure.pos.x = newmeasure.start.x;
					szx += sizex;
					sizex = 0; sizey = 0;
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatLeftBottom) { newmeasure.floatLeftY = floatLeftX = std::max(newmeasure.by, measure->floatLeftY = getOffscreenPosition(measure->floatelem).y); newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = -10000; }
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatRightBottom) { newmeasure.floatRightY = measure->floatRightY = floatRightX = getOffscreenPosition(measure->floatelem).y + measure->floatelem->size.y; newmeasure.floatRightBottomY = measure->floatRightBottomX = floatRightBottom = -10000; }
					if (par->xalign != AlignType::ZeroAlign || par->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (elem->size.x - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (elem->size.x - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (availH - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (availH - sizey);
						}
					}
					child = child->next;
					continue;
				}
				else if (sizey + sy > availH) {
					if (par->xalign != AlignType::ZeroAlign || par->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (elem->size.x - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (elem->size.x - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (availH - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (availH - sizey);
						}
					}
					newmeasure.pos.y = floatLeftX;
					newmeasure.start.x += sizex;
					szx += sizex;
					sizex = 0; sizey = 0;
					if (floatLeftBottom > 0 && newmeasure.bx + szx >= floatLeftBottom) { newmeasure.floatLeftY = floatLeftX = std::max(newmeasure.by, measure->floatLeftY = getOffscreenPosition(measure->floatelem).y); newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = -10000; }
					if (floatLeftBottom > 0 && newmeasure.bx + szx >= floatRightBottom) { newmeasure.floatRightY = measure->floatRightY = floatRightX = getOffscreenPosition(measure->floatelem).y + measure->floatelem->size.y; newmeasure.floatRightBottomY = measure->floatRightBottomY = floatRightBottom = -10000; }
				}
			}
			if (sizex < sx) sizex = sx;
			newmeasure.pos.y += sy;
			newmeasure.pos.x = newmeasure.start.x;
			child->pos2.x = szx;
			child->pos2.y = floatLeftX - newmeasure.by + sizey + szy;
			sizey += sy;
		}
		child = child->next;
	}
	// childend (ElemEnd) の位置設定
	if (orient) {
		elem->childend->pos2.x = floatLeftX + sizex + szx;
		elem->childend->pos2.y = szy;
	} else {
		elem->childend->pos2.x = szx;
		elem->childend->pos2.y = floatLeftX + sizey + szy;
	}
	elem->size2.x = sizex + szx;
	elem->size2.y = sizey + szy;
	// スクロールバー分をsize2に加算
	if (baseType(elem->xtype) == SizeType::Scroll) elem->size2.y += 5;
	if (baseType(elem->ytype) == SizeType::Scroll) elem->size2.x += 5;
	// ページ単位で切り上げ
	if (baseType(elem->xtype) == SizeType::Page && elem->size.x > 0) {
		float pw = elem->size.x + elem->paddings[1] + elem->paddings[3];
		elem->size2.x = std::ceil(elem->size2.x / pw) * pw;
	}
	if (baseType(elem->ytype) == SizeType::Page && elem->size.y > 0) {
		float ph = elem->size.y + elem->paddings[0] + elem->paddings[2];
		elem->size2.y = std::ceil(elem->size2.y / ph) * ph;
	}
	if (elem->xtype == SizeType::Auto) { elem->size.x = elem->size2.x; }
	if (elem->ytype == SizeType::Auto) { elem->size.y = elem->size2.y; }
	// align末尾パス
	if (mainAlign != AlignType::ZeroAlign || crossAlign != AlignType::ZeroAlign) {
		for (; first != elem->childend; first = first->next) {
			if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
			if (orient) {
				float availW = measure->floatRightX - measure->floatLeftX;
				if (mainAlign == AlignType::CenterAlign) first->pos2.x += (availW - sizex) / 2;
				else if (mainAlign == AlignType::OverAlign) first->pos2.x += (availW - sizex);
				if (crossAlign == AlignType::CenterAlign) first->pos2.y += (par->size.y - sizey) / 2;
				else if (crossAlign == AlignType::OverAlign) first->pos2.y += (par->size.y - sizey);
			} else {
				if (mainAlign == AlignType::CenterAlign) first->pos2.y += (par->size.y - sizey) / 2;
				else if (mainAlign == AlignType::OverAlign) first->pos2.y += (par->size.y - sizey);
				if (crossAlign == AlignType::CenterAlign) first->pos2.x += (par->size.x - sizex) / 2;
				else if (crossAlign == AlignType::OverAlign) first->pos2.x += (par->size.x - sizex);
			}
		}
	}
	return RetMeasure_None;
}
RetMeasureType ElementMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	// LinkedElement経由の場合、サイズをLinkedElementから取得
	if (elem->type == LetterType::_Main && ((NewLocal*)elem)->linked) {
		elem->size = ((NewLocal*)elem)->linked->size;
	}
	NewMeasure newmeasure;
	float sbarX = 0.0f, sbarY = 0.0f;
	if (baseType(elem->xtype) == SizeType::Scroll) sbarY = 5;
	if (baseType(elem->ytype) == SizeType::Scroll) sbarX = 5;
	if (baseType(elem->xtype) == SizeType::Page) sbarY = 10;
	if (baseType(elem->ytype) == SizeType::Page) sbarX = 10;
	if (elem->offscreen != NULL) {
		if (!elem->offscreen->layout) return RetMeasure_None;
		newmeasure.pos = { 0, 0 };
		newmeasure.size = { elem->size.x, elem->size.y };
		newmeasure.start = { 0, 0 };
		newmeasure.group = elem->offscreen->group;
		newmeasure.floatRightX = elem->size.x - sbarX;
		newmeasure.floatRightY = elem->size.y - sbarY;
		elem->offscreen->layout = false;
	} else {
		newmeasure.pos.x = measure->pos.x + elem->margins[3] + elem->boWidthes[3] + elem->paddings[3];
		newmeasure.pos.y = measure->pos.y + elem->margins[0] + elem->boWidthes[0] + elem->paddings[0];
		newmeasure.start = measure->pos;
		newmeasure.group = measure->group;
		// Auto/Percentの場合のみ親からfloat stateを継承
		if (elem->xtype == SizeType::Auto) {
			newmeasure.floatLeftX = measure->floatLeftX;
			newmeasure.floatRightX = measure->floatRightX;
			newmeasure.floatLeftBottomX = measure->floatLeftBottomX;
			newmeasure.floatRightBottomX = measure->floatRightBottomX;
			newmeasure.breakxelem = measure->breakxelem;
		} else {
			newmeasure.breakxelem = NULL;
			newmeasure.floatLeftX = newmeasure.pos.x;
			newmeasure.floatRightX = newmeasure.pos.x + elem->size.x;
		}
		if (elem->ytype == SizeType::Auto) {
			newmeasure.floatLeftY = measure->floatLeftY;
			newmeasure.floatRightY = measure->floatRightY;
			newmeasure.floatLeftBottomY = measure->floatLeftBottomY;
			newmeasure.floatRightBottomY = measure->floatRightBottomY;
			newmeasure.breakyelem = measure->breakyelem;
		} else {
			newmeasure.breakyelem = NULL;
			newmeasure.floatLeftY = newmeasure.pos.y;
			newmeasure.floatRightY = elem->size.y;
		}
	}
	newmeasure.bx = newmeasure.pos.x; newmeasure.by = newmeasure.pos.y;
	float sizex = 0, sizey = 0;
	float szx = 0, szy = 0;
	// orient に応じてX/Yのfloat stateを選択
	float floatLeftX, floatRightX, floatLeftBottom, floatRightBottom;
	if (elem->orient) {
		floatLeftX = std::max(newmeasure.pos.x, newmeasure.floatLeftX);
		floatRightX = newmeasure.floatRightX;
		floatLeftBottom = newmeasure.floatLeftBottomX;
		floatRightBottom = newmeasure.floatRightBottomX;
	} else {
		floatLeftX = std::max(newmeasure.pos.y, newmeasure.floatLeftY);
		floatRightX = newmeasure.floatRightY;
		floatLeftBottom = newmeasure.floatLeftBottomY;
		floatRightBottom = newmeasure.floatRightBottomY;
	}
	NewElement* first = elem->childend->next;
	if (baseType(elem->xtype) == SizeType::Break) newmeasure.breakxelem = elem;
	if (baseType(elem->ytype) == SizeType::Break) newmeasure.breakyelem = elem;
	for (NewElement* child = elem->childend->next; child != elem->childend; ) {
		// パーセントサイズ解決: float領域に対する比率
		if (isPercent(child->xtype) && elem->xtype != SizeType::Auto) {
			float pct = fmodf(child->size.x, 1.0f);
			float mult = (pct == 0.0f) ? 1.0f : pct;
			float availX = newmeasure.floatRightX - newmeasure.floatLeftX;
			child->size.x = floorf(availX * mult) + pct;
		}
		if (isPercent(child->ytype) && elem->ytype != SizeType::Auto) {
			float pct = fmodf(child->size.y, 1.0f);
			float mult = (pct == 0.0f) ? 1.0f : pct;
			float availY = newmeasure.floatRightY - newmeasure.floatLeftY;
			child->size.y = floorf(availY * mult) + pct;
		}
		auto rettype = child->Measure(child->gc, child, &newmeasure, child->gc->local, n);
		float sx = child->size.x + child->boWidthes[1] + child->boWidthes[3] + child->paddings[1] + child->paddings[3] + child->margins[1] + child->margins[3];
		float sy = child->size.y + child->boWidthes[0] + child->boWidthes[2] + child->paddings[0] + child->paddings[2] + child->margins[0] + child->margins[2];
		if (child->layout == Layout::_ZeroNone) {
			if (elem->orient) {
				child->pos2.x = floatLeftX - newmeasure.bx;
				child->pos2.y = szy;
			}
			else {
				child->pos2.y = floatLeftX - newmeasure.by;
				child->pos2.x = szx;
			}
			child = child->next;
			continue;
		}
		else if (child->layout == Layout::_OverNone) {
			if (elem->orient) {
				child->pos2.x = floatRightX - sx - newmeasure.bx;
				child->pos2.y = szy;
			}
			else {
				child->pos2.y = floatRightX - sy - newmeasure.by;
				child->pos2.x = szx;
			}
			child = child->next;
			continue;
		}
		if (child->layout == Layout::_Zero || child->layout == Layout::_Over) {
			if (elem->orient) {
				// Page境界: floatがページに収まらない場合、次ページから配置
				if (baseType(elem->ytype) == SizeType::Page) {
					float ph = elem->size.y + elem->paddings[0] + elem->paddings[2];
					float pageBottom = (std::floor(szy / ph) + 1.0f) * ph;
					if (szy + sy > pageBottom) {
						szy = pageBottom;
						sizex = 0; sizey = 0;
						newmeasure.floatLeftX = measure->floatLeftX = floatLeftX = newmeasure.bx;
						newmeasure.floatRightX = floatRightX = newmeasure.bx + elem->size.x;
						if (elem->parent) measure->floatRightX = elem->parent->size.x;
						newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = newmeasure.by;
						newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = newmeasure.by;
						if (Layout::_Zero) newmeasure.pos.x = sx;
						else newmeasure.pos.x = 0;
						newmeasure.pos.y = szy;
					}
				}
				if (child->layout == Layout::_Zero) {
					child->pos2.x = floatLeftX - newmeasure.bx;
					child->pos2.y = szy;
					floatLeftX += sx;
					newmeasure.floatLeftX = measure->floatLeftX = floatLeftX;
					if (floatLeftBottom < newmeasure.by + szy + sy) newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = newmeasure.by + szy + sy;
					measure->floatelem = elem->parent;
				} else {
					floatRightX -= sx;
					child->pos2.x = floatRightX - newmeasure.bx;
					child->pos2.y = szy;
					newmeasure.floatRightX = measure->floatRightX = floatRightX;
					if (floatRightBottom < newmeasure.by + szy + sy) newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = newmeasure.by + szy + sy;
					measure->floatelem = elem->parent;
				}
			} else {
				// Page境界: floatがページに収まらない場合、次ページから配置
				if (baseType(elem->xtype) == SizeType::Page) {
					float pw = elem->size.x + elem->paddings[1] + elem->paddings[3];
					float pageRight = (std::floor(szx / pw) + 1.0f) * pw;
					if (szx + sx > pageRight) {
						szx = pageRight;
						sizex = 0; sizey = 0;
						newmeasure.floatLeftY = measure->floatLeftY = floatLeftX = newmeasure.by;
						newmeasure.floatRightY = floatRightX = newmeasure.by + elem->size.y;
						if (elem->parent) measure->floatRightY = elem->parent->size.y;
						newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = newmeasure.bx;
						newmeasure.floatRightBottomY = measure->floatRightBottomY = floatRightBottom = newmeasure.bx;
						if (Layout::_Zero) newmeasure.pos.y = sy;
						else newmeasure.pos.y = 0;
						newmeasure.pos.x = szx;
					}
				}
				if (child->layout == Layout::_Zero) {
					child->pos2.y = floatLeftX - newmeasure.by;
					child->pos2.x = szx;
					floatLeftX += sy;
					newmeasure.floatLeftY = measure->floatLeftY = floatLeftX;
					if (floatLeftBottom < newmeasure.bx + szx + sx) newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = newmeasure.bx + szx + sx;
					measure->floatelem = elem->parent;
				} else {
					floatRightX -= sy;
					child->pos2.y = floatRightX - newmeasure.by;
					child->pos2.x = szx;
					newmeasure.floatRightY = floatRightX;
					measure->floatRightY -= sy;
					if (floatRightBottom < newmeasure.bx + szx + sx) newmeasure.floatRightBottomY = measure->floatRightBottomY = floatRightBottom = newmeasure.bx + szx + sx;
					measure->floatelem = elem->parent;
				}
			}
			child = child->next;
			continue;
		}
		if (elem->layout == Layout::none) {
			if (elem->orient) {
				child->pos2.x = floatLeftX - newmeasure.bx + sizex + szx;
				child->pos2.y = szy;
			}
			else {
				child->pos2.x = szx;
				child->pos2.y = floatLeftX - newmeasure.by + sizey + szy;
			}
		}
		else if (elem->orient) {
			float availW = floatRightX - floatLeftX;
			if (newmeasure.breakxelem) {
				if (rettype == RetMeasure_Break) {
					if (sizey < sy) sizey = sy;
					child->pos2.x = floatLeftX + sizex + szx;
					child->pos2.y = szy;
					newmeasure.start.y += sizey;
					newmeasure.pos.x = floatLeftX;
					newmeasure.pos.y = newmeasure.start.y;
					szy += sizey;
					sizex = 0; sizey = 0;
					if (newmeasure.by + szy >= floatLeftBottom) { newmeasure.floatLeftX = floatLeftX = std::max(newmeasure.bx, measure->floatLeftX = getOffscreenPosition(measure->floatelem).x); newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = -10000; }
					if (newmeasure.by + szy >= floatRightBottom) { newmeasure.floatRightX = measure->floatRightX = floatRightX = getOffscreenPosition(measure->floatelem).x + measure->floatelem->size.x; newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = -10000; }
					if (elem->xalign != AlignType::ZeroAlign || elem->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (availW - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (availW - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (elem->size.y - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (elem->size.y - sizey);
						}
					}
				}
				else if (sizex + sx > availW) {
					if (elem->xalign != AlignType::ZeroAlign || elem->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (availW - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (availW - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (elem->size.y - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (elem->size.y - sizey);
						}
					}
					newmeasure.pos.x = floatLeftX;
					newmeasure.start.y += sizey;
					szy += sizey;
					sizex = 0; sizey = 0;
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatLeftBottom) { newmeasure.floatLeftX = floatLeftX = std::max(newmeasure.bx, measure->floatLeftX = getOffscreenPosition(measure->floatelem).x); newmeasure.floatLeftBottomX = measure->floatLeftBottomX = floatLeftBottom = -10000; }
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatRightBottom) { newmeasure.floatRightX = measure->floatRightX = floatRightX = getOffscreenPosition(measure->floatelem).x + measure->floatelem->size.x; newmeasure.floatRightBottomX = measure->floatRightBottomX = floatRightBottom = -10000; }
				}
			}
			if (sizey < sy) sizey = sy;
			newmeasure.pos.x += sx;
			newmeasure.pos.y = newmeasure.start.y;
			child->pos2.x = floatLeftX - newmeasure.bx + sizex + szx;
			child->pos2.y = szy;
			sizex += sx;
		}
		else
		{
			float availH = floatRightX - floatLeftX;
			if (newmeasure.breakyelem) {
				if (rettype == RetMeasure_Break) {
					if (sizex < sx) sizex = sx;
					child->pos2.x = 0;
					child->pos2.y = floatLeftX + sizey + szy;
					newmeasure.start.x += sizex;
					newmeasure.pos.y = floatLeftX;
					newmeasure.pos.x = newmeasure.start.x;
					szx += sizex;
					sizex = 0; sizey = 0;
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatLeftBottom) { newmeasure.floatLeftY = floatLeftX = std::max(newmeasure.by, measure->floatLeftY = getOffscreenPosition(measure->floatelem).y); newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = -10000; }
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatRightBottom) { newmeasure.floatRightY = measure->floatRightY = floatRightX = getOffscreenPosition(measure->floatelem).y + measure->floatelem->size.y; newmeasure.floatRightBottomY = measure->floatRightBottomX = floatRightBottom = -10000; }
					if (elem->xalign != AlignType::ZeroAlign || elem->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (elem->size.x - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (elem->size.x - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (availH - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (availH - sizey);
						}
					}
					child = child->next;
					continue;
				}
				else if (sizey + sy > availH) {
					if (elem->xalign != AlignType::ZeroAlign || elem->yalign != AlignType::ZeroAlign) {
						for (; first != child; first = first->next) {
							if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
							if (elem->xalign == AlignType::CenterAlign) first->pos2.x += (elem->size.x - sizex) / 2;
							else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (elem->size.x - sizex);
							if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (availH - sizey) / 2;
							else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (availH - sizey);
						}
					}
					newmeasure.pos.y = floatLeftX;
					newmeasure.start.x += sizex;
					szx += sizex;
					sizex = 0; sizey = 0;
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatLeftBottom) { newmeasure.floatLeftY = floatLeftX = std::max(newmeasure.by, measure->floatLeftY = getOffscreenPosition(measure->floatelem).y); newmeasure.floatLeftBottomY = measure->floatLeftBottomY = floatLeftBottom = -10000; }
					if (floatLeftBottom > 0 && newmeasure.by + szy >= floatRightBottom) { newmeasure.floatRightY = measure->floatRightY = floatRightX = getOffscreenPosition(measure->floatelem).y + measure->floatelem->size.y; newmeasure.floatRightBottomY = measure->floatRightBottomX = floatRightBottom = -10000; }
				}
			}
			if (sizex < sx) sizex = sx;
			newmeasure.pos.y += sy;
			newmeasure.pos.x = newmeasure.start.x;
			child->pos2.x = szx;
			child->pos2.y = floatLeftX - newmeasure.by + sizey + szy;
			sizey += sy;
		}
		child = child->next;
	}
	// childend (ElemEnd) の位置設定
	if (elem->orient) {
		elem->childend->pos2.x = floatLeftX + sizex + szx;
		elem->childend->pos2.y = szy;
	} else {
		elem->childend->pos2.x = szx;
		elem->childend->pos2.y = floatLeftX + sizey + szy;
	}
	elem->size2.x = sizex + szx;
	elem->size2.y = sizey + szy;
	if (elem == measure->floatelem) {
		if (elem->orient) {
			float floatMaxY = (measure->floatLeftBottomY > measure->floatRightBottomY) ? measure->floatLeftBottomY : measure->floatRightBottomY;
			if (elem->size2.y < floatMaxY) elem->size2.y = floatMaxY;
		}
		else {
			float floatMaxX = (measure->floatLeftBottomX > measure->floatRightBottomX) ? measure->floatLeftBottomX : measure->floatRightBottomX;
			if (elem->size2.x < floatMaxX) elem->size2.x = floatMaxX;
		}
	}
	// スクロールバー分をsize2に加算
	if (baseType(elem->xtype) == SizeType::Scroll) elem->size2.y += 5;
	if (baseType(elem->ytype) == SizeType::Scroll) elem->size2.x += 5;
	// ページ単位で切り上げ
	if (baseType(elem->xtype) == SizeType::Page && elem->size.x > 0) {
		float pw = elem->size.x + elem->paddings[1] + elem->paddings[3];
		elem->size2.x = std::ceil(elem->size2.x / pw) * pw;
	}
	if (baseType(elem->ytype) == SizeType::Page && elem->size.y > 0) {
		float ph = elem->size.y + elem->paddings[0] + elem->paddings[2];
		elem->size2.y = std::ceil(elem->size2.y / ph) * ph;
	}
	if (elem->xtype == SizeType::Auto) { elem->size.x = elem->size2.x; }
	if (elem->ytype == SizeType::Auto) { elem->size.y = elem->size2.y; }

	if (elem->xalign != AlignType::ZeroAlign || elem->yalign != AlignType::ZeroAlign) {
		for (; first != elem->childend; first = first->next) {
			if (first->layout == Layout::_Zero || first->layout == Layout::_Over || first->layout == Layout::_ZeroNone || first->layout == Layout::_OverNone) continue;
			float sy2 = first->size.y + first->boWidthes[0] + first->boWidthes[2] + first->paddings[0] + first->paddings[2] + first->margins[0] + first->margins[2];
			if (elem->xalign == AlignType::CenterAlign) {
				first->pos2.x += (elem->size.x - sizex) / 2;
			}
			else if (elem->xalign == AlignType::OverAlign) first->pos2.x += (elem->size.x - sizex);
			if (elem->yalign == AlignType::CenterAlign) first->pos2.y += (elem->size.y - sizey) / 2;
			else if (elem->yalign == AlignType::OverAlign) first->pos2.y += (elem->size.y - sizey);
		}
	}
	return RetMeasure_None;
}
float pro = 0;
int addPattern(ThreadGC* thgc, std::vector<float>& colors, std::vector<float>& widthes);
int addPattern(ThreadGC* thgc, std::vector<uint32_t>& colors, std::vector<float>& widthes);
void ElementDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	// 親要素のタイル状態を退避 (このフレームで自要素が新しいタイル描画を始めた場合、
	// 戻り際に親の状態を nullptr で潰してしまうと、続く兄弟要素のタイル展開が壊れる)
	TiledTextureInfo*        _saveTiledTarget        = g->layer->tiledTarget;
	ImageId                  _saveTiledTargetImageId = g->layer->tiledTargetImageId;
	bgfx::FrameBufferHandle* _saveTiledPlaceholderFBO = g->layer->tiledPlaceholderFBO;
	uint64_t                 _saveTiledBaseViewId    = g->layer->tiledBaseViewId;
	bool offscreened = false;
	float sbarx = 0, sbary = 0;
	if (baseType(elem->xtype) == SizeType::Scroll) {
		sbary += 5; offscreened = true;
	} else if (baseType(elem->xtype) == SizeType::Page) {
		sbary += 10; offscreened = true;
	}
	if (baseType(elem->ytype) == SizeType::Scroll) {
		sbarx += 5; offscreened = true;
	} else if (baseType(elem->ytype) == SizeType::Page) {
		sbarx += 10; offscreened = true;
	}
	if (elem->type == LetterType::_Main) {
		offscreened = true;
	}
	NewGraphic g2;
	float sizex = elem->size.x + elem->paddings[1] + elem->paddings[3]; float sizey = elem->size.y + elem->paddings[0] + elem->paddings[2];
	float viewSizex = sizex, viewSizey = sizey; // 要素の表示サイズ（バー描画用）
	float tabExtL = 0, tabExtR = 0, tabExtT = 0, tabExtB = 0;
	if (elem->offscreen != NULL) {
		if (elem->background) {
			float tabR = elem->background->backUVMinX;
			int tabPat = (int)elem->background->cornerPattern;
			float tabExt = tabR * 1.5f;
			if (tabR > 0.0f && tabPat >= 9 && tabPat <= 12) {
				if (tabPat == 9)       tabExtT = tabExt;
				else if (tabPat == 10) tabExtR = tabExt;
				else if (tabPat == 11) tabExtB = tabExt;
				else                   tabExtL = tabExt;
			}
		}
		float z = 0;
		PointF absPos = getAbsolutePosition(elem, &z);
		if (elem->size2.x >= sizex) sizex = elem->size2.x;
		if (elem->size2.y >= sizey) sizey = elem->size2.y;
		sizex += tabExtL + tabExtR;
		sizey += tabExtT + tabExtB;
		// 現サイズは実テクスチャから読む (Tiled なら totalSize、Standalone なら size)。
		// queueOffscreenResize が同期更新するので「自前 fbsize キャッシュ」と等価。
		PointI curFb = { 0, 0 };
		{
			TiledTextureInfo* curTti = mygetTiledTextureInfo(thgc, elem->offscreen->imPing);
			if (curTti) curFb = curTti->totalSize;
			else {
				StandaloneTextureInfo* curSti = mygetStandaloneTextureInfo(thgc, elem->offscreen->imPing);
				if (curSti) curFb = curSti->size;
			}
		}
		float size3x = curFb.x, size3y = curFb.y;
		bool resized = false;
		if (curFb.x < sizex) {
			size3x = sizex * 1.3;
			resized = true;
		}
		else if (sizex < curFb.x / 3) {
			size3x = sizex * 1.3;
			resized = true;
		}
		if (curFb.y < sizey) {
			size3y = sizey * 1.3;
			resized = true;
		}
		else if (sizey < curFb.y / 3) {
			size3y = sizey * 1.3;
			resized = true;
		}
		if (resized) {
			// ページめくり対象なら、タイル辺長をページサイズの倍数に丸めるヒントを渡す。
			// (1 ページが必ず単一タイル内に収まるようにして pushPageCurl の前提を満たす)
			int pageW = (baseType(elem->xtype) == SizeType::Page) ? (int)viewSizex : 0;
			int pageH = (baseType(elem->ytype) == SizeType::Page) ? (int)viewSizey : 0;
			// タイプ遷移 (Standalone↔Tiled) では新 id が返るので必ず受け取る
			elem->offscreen->imPing = queueOffscreenResize(thgc, elem->offscreen->imPing, size3x, size3y, pageW, pageH);
			elem->offscreen->imPong = queueOffscreenResize(thgc, elem->offscreen->imPong, size3x, size3y, pageW, pageH);
		}
		ImageId readId = elem->offscreen->ping ? elem->offscreen->imPing : elem->offscreen->imPong;
		ImageId writeId = elem->offscreen->ping ? elem->offscreen->imPong : elem->offscreen->imPing;

		// タイルFBOか通常FBOかで分岐
		TiledTextureInfo* tiledRead = mygetTiledTextureInfo(thgc, readId);
		if (tiledRead) {
			// 読み取り: 最大2×2タイルを1回のpushImageで描画
			// タイルは X/Y で別辺長 (tsx, tsy)。w が短いときは h 方向に伸ばす設計。
			int tsx = (int)tiledRead->tileSize;
			int tsy = (int)tiledRead->tileSizeY;

			// ===== ページめくり (タイル版) =====
			// queueOffscreenResize で pageW/pageH を渡しているので、tile.w/tile.h は
			// ページサイズの倍数になっている。1 ページは必ず単一タイル内に収まる。
			// from ページと to ページそれぞれを含むタイルを 1 枚ずつ取って pushPageCurl。
			if (tsx > 0 && tsy > 0
				&& elem->offscreen->curlProgress >= 0.0f && elem->offscreen->curlProgress <= 1.0f) {
				float fromPage = elem->offscreen->curlFromPage;
				float toPage   = elem->offscreen->curlToPage;
				float progress = elem->offscreen->curlProgress;
				int axis = elem->offscreen->curlAxis;

				// 各ページの (絶対) 位置 → どのタイルか
				int fromX = (axis == 0) ? 0 : (int)(fromPage * viewSizex);
				int fromY = (axis == 0) ? (int)(fromPage * viewSizey) : 0;
				int toX   = (axis == 0) ? 0 : (int)(toPage   * viewSizex);
				int toY   = (axis == 0) ? (int)(toPage   * viewSizey) : 0;

				int fromCol = std::clamp(fromX / tsx, 0, tiledRead->tilesX - 1);
				int fromRow = std::clamp(fromY / tsy, 0, tiledRead->tilesY - 1);
				int toCol   = std::clamp(toX   / tsx, 0, tiledRead->tilesX - 1);
				int toRow   = std::clamp(toY   / tsy, 0, tiledRead->tilesY - 1);

				auto& fromTile = tiledRead->tiles[fromRow * tiledRead->tilesX + fromCol];
				auto& toTile   = tiledRead->tiles[toRow   * tiledRead->tilesX + toCol];

				// タイル内 UV (実テクスチャ寸法で正規化)
				int fromTw = fromTile.w > 0 ? fromTile.w : tsx;
				int fromTh = fromTile.h > 0 ? fromTile.h : tsy;
				int toTw   = toTile.w   > 0 ? toTile.w   : tsx;
				int toTh   = toTile.h   > 0 ? toTile.h   : tsy;
				float fromUvX = (fromX - fromCol * tsx) / (float)fromTw;
				float fromUvY = (fromY - fromRow * tsy) / (float)fromTh;
				float toUvX   = (toX   - toCol   * tsx) / (float)toTw;
				float toUvY   = (toY   - toRow   * tsy) / (float)toTh;
				// uvW/uvH は from タイル基準 (to タイルも同サイズである前提 = ページが
				// タイル境界をまたがないように pageW/pageH 倍数化した恩恵)
				float uvW = viewSizex / (float)fromTw;
				float uvH = viewSizey / (float)fromTh;

				bool goingBack = toPage < fromPage;
				float curlAngle = (axis == 0)
					? (goingBack ? -1.5708f : 1.5708f)
					: (goingBack ?  3.14159f : 0.0f);
				float endProgress = elem->offscreen->spread ? 0.5f : 1.0f;
				float drawProgress = goingBack ? (endProgress - progress) : progress;
				float drawFromUvX = goingBack ? toUvX   : fromUvX;
				float drawFromUvY = goingBack ? toUvY   : fromUvY;
				float drawToUvX   = goingBack ? fromUvX : toUvX;
				float drawToUvY   = goingBack ? fromUvY : toUvY;

				bgfx::TextureHandle* texFrom =
					bgfx::isValid(fromTile.handle) ? &fromTile.handle : &nulltex;
				bgfx::TextureHandle* texTo =
					bgfx::isValid(toTile.handle)   ? &toTile.handle   : &nulltex;

				auto* bg = elem->background;
				g->layer->pushPageCurl(
					absPos.x + elem->margins[3],
					absPos.y + elem->margins[0],
					viewSizex, viewSizey,
					drawProgress, -30.0f, curlAngle,
					0xFFFFFFFF,
					drawFromUvX, drawFromUvY, uvW, uvH,
					drawToUvX, drawToUvY,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0,
					texFrom, texTo, 10000 + std::floor(z),
					g->fb, g->fbsize, g->viewId, 0.0f, axis == 0, elem->offscreen->spread);

				elem->offscreen->curlProgress += g->deltaTime * 2.0f;
				if (elem->offscreen->curlProgress > endProgress) {
					elem->offscreen->curlProgress = -1.0f;
					if (axis == 0) elem->scroll.y = toPage * viewSizey;
					else           elem->scroll.x = toPage * viewSizex;
				}
				elem->offscreen->markPaint(local);
			}
			else if (tsx > 0 && tsy > 0) {
				int srcX = (int)(elem->scroll.x);
				int srcY = (int)(elem->scroll.y);
				int srcW = (int)std::ceil(viewSizex + tabExtL + tabExtR);
				int srcH = (int)std::ceil(viewSizey + tabExtT + tabExtB);
				// タイル境界は (tsx, tsy) のグリッドに揃っている (tile.x = col * tsx, tile.y = row * tsy) ので
				// インデックス計算はそれぞれの軸の辺長で割る。実テクスチャ寸法 (tile.w/h) ではない点に注意。
				// 両端 clamp は必須: srcX/srcY が想定外でも tiles[] への OOB アクセスを防ぐ。
				int col0 = std::clamp(srcX / tsx,                0, tiledRead->tilesX - 1);
				int row0 = std::clamp(srcY / tsy,                0, tiledRead->tilesY - 1);
				int col1 = std::clamp((srcX + srcW - 1) / tsx,   0, tiledRead->tilesX - 1);
				int row1 = std::clamp((srcY + srcH - 1) / tsy,   0, tiledRead->tilesY - 1);
				// 最大2×2に制限
				if (col1 > col0 + 1) col1 = col0 + 1;
				if (row1 > row0 + 1) row1 = row0 + 1;
				// テクスチャハンドル収集 (左上, 右上, 左下, 右下)
				// ★ in-place 更新で tiles ベクタのアドレスは安定。producer 時点で
				//   handle が INVALID でも submit 時に valid 化されるので isValid 判定で
				//   &nulltex に倒さず無条件に &tile.handle を渡す。
				bgfx::TextureHandle* texSlots[4] = { &nulltex, &nulltex, &nulltex, &nulltex };
				// lazy 確保: sample される 4 隅 tile を必要時に生成
				int idxTL = row0 * tiledRead->tilesX + col0;
				mEnsureTile(thgc, readId, idxTL);
				auto& tl = tiledRead->tiles[idxTL];
				texSlots[0] = &tl.handle;
				if (col1 > col0) {
					int idxTR = row0 * tiledRead->tilesX + col1;
					mEnsureTile(thgc, readId, idxTR);
					auto& tr = tiledRead->tiles[idxTR];
					texSlots[1] = &tr.handle;
				}
				if (row1 > row0) {
					int idxBL = row1 * tiledRead->tilesX + col0;
					mEnsureTile(thgc, readId, idxBL);
					auto& bl = tiledRead->tiles[idxBL];
					texSlots[2] = &bl.handle;
				}
				if (col1 > col0 && row1 > row0) {
					int idxBR = row1 * tiledRead->tilesX + col1;
					mEnsureTile(thgc, readId, idxBR);
					auto& br = tiledRead->tiles[idxBR];
					texSlots[3] = &br.handle;
				}
				// 描画領域: 全タイルをカバーする矩形 (X/Y 別辺長)
				float tileStartX = col0 * tsx;
				float tileStartY = row0 * tsy;
				float tileEndX = (col1 + 1) * tsx;
				float tileEndY = (row1 + 1) * tsy;
				// 最終タイルの実サイズで補正
				if (col1 < tiledRead->tilesX) {
					auto& lastCol = tiledRead->tiles[row0 * tiledRead->tilesX + col1];
					tileEndX = lastCol.x + lastCol.w;
				}
				if (row1 < tiledRead->tilesY) {
					auto& lastRow = tiledRead->tiles[row1 * tiledRead->tilesX + col0];
					tileEndY = lastRow.y + lastRow.h;
				}
				float drawX = absPos.x + elem->margins[3] + (tileStartX - elem->scroll.x);
				float drawY = absPos.y + elem->margins[0] + (tileStartY - elem->scroll.y);
				float drawW = tileEndX - tileStartX;
				float drawH = tileEndY - tileStartY;
				// UV 計算: グリッド原点 (col0*tsx, row0*tsy) からのオフセットを
				//          実テクスチャ寸法 tl.w / tl.h で正規化 (0..1 範囲)。
				//          "ts/tsy で割る" のではなく "tile.w/tile.h で割る" のがポイント。
				//          (tile.w < tsx になりうるエッジタイル / 小さい Tiled に対応)
				int tw = tl.w > 0 ? tl.w : tsx;
				int th = tl.h > 0 ? tl.h : tsy;
				float uvX = (elem->scroll.x - col0 * tsx) / (float)tw;
				float uvY = (elem->scroll.y - row0 * tsy) / (float)th;
				float uvW = (viewSizex + tabExtL + tabExtR) / (float)tw;
				float uvH = (viewSizey + tabExtT + tabExtB) / (float)th;
				auto* bg = elem->background;
				g->layer->pushImage(
					absPos.x + elem->margins[3], absPos.y + elem->margins[0],
					viewSizex, viewSizey,
					uvX, uvY, uvW, uvH,
					bg ? bg->bs.boRadiuses[0] : 0, bg ? bg->bs.boRadiuses[1] : 0,
					bg ? bg->bs.boRadiuses[2] : 0, bg ? bg->bs.boRadiuses[3] : 0,
					bg ? bg->aa : 0,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0, 0xFFFFFFFF,
					10000 + std::floor(z), texSlots[0], g->fb, g->fbsize, g->viewId,
					0.0f,
					bg ? bg->cornerPattern : 0.0f,
					bg ? bg->backUVMinX : 0.0f,
					texSlots[1], texSlots[2], texSlots[3]);
			}
			if (g->paint == Offscreen::PaintOffscreen) return;
			// 書き込み: LayerInfoにタイル状態を設定し、push関数が自動的にタイル展開する
			TiledTextureInfo* tiledWrite = mygetTiledTextureInfo(thgc, writeId);
			if (tiledWrite && !tiledWrite->tiles.empty()) {
				auto& firstTile = tiledWrite->tiles[0];
				// タイル数分の論理viewIdをグローバルカウンタから確保
				uint64_t tileBaseId = ::viewId;
				::viewId += (uint64_t)tiledWrite->tiles.size();
				// graphic.fb は LayerInfo 所有の安定 sentinel を指す。
				// (旧設計の &firstTile.fbo はベクタ要素アドレスで resize 時に dangling する)
				g->layer->tiledTarget = tiledWrite;
				g->layer->tiledTargetImageId = writeId;            // 安定 ID
				g->layer->tiledPlaceholderFBO = &g->layer->_tiledSentinel; // 安定アドレス
				g->layer->tiledBaseViewId = tileBaseId;
				g2 = NewGraphic{ g->layer, elem, elem, {0, 0}, {elem->size2.x, elem->size2.y}, {0,0}, {0,0},
					writeId, g->layer->tiledPlaceholderFBO, &tiledWrite->totalSize,
					elem->offscreen->viewId = tileBaseId, 0, elem->offscreen->group };
			}
		}
		else {
			auto readInfo = mygetStandaloneTextureInfo(thgc, readId);
			// ページめくりアニメーション中
			if (elem->offscreen->curlProgress >= 0.0f && elem->offscreen->curlProgress <= 1.0f) {
				float fromPage = elem->offscreen->curlFromPage;
				float toPage = elem->offscreen->curlToPage;
				float progress = elem->offscreen->curlProgress;
				int axis = elem->offscreen->curlAxis;
				// 現在ページと次ページのUV
				float fromUvX, fromUvY, toUvX, toUvY;
				float uvW = viewSizex / size3x, uvH = viewSizey / size3y;
				if (axis == 0) {
					fromUvX = 0; fromUvY = fromPage * viewSizey / size3y;
					toUvX = 0;   toUvY = toPage * viewSizey / size3y;
				} else {
					fromUvX = fromPage * viewSizex / size3x; fromUvY = 0;
					toUvX = toPage * viewSizex / size3x;     toUvY = 0;
				}
				bool goingBack = toPage < fromPage;
				float curlAngle;
				if (axis == 0) {
					curlAngle = goingBack ? -1.5708f : 1.5708f;
				} else {
					curlAngle = goingBack ? 3.14159f : 0.0f;
				}
				// 戻る場合: progressを反転（両開き: 0.5→0, 片面: 1.0→0）
				float endProgress = elem->offscreen->spread ? 0.5f : 1.0f;
				float drawProgress = goingBack ? (endProgress - progress) : progress;
				float drawFromUvX = goingBack ? toUvX : fromUvX;
				float drawFromUvY = goingBack ? toUvY : fromUvY;
				float drawToUvX = goingBack ? fromUvX : toUvX;
				float drawToUvY = goingBack ? fromUvY : toUvY;
				auto* bg = elem->background;
				g->layer->pushPageCurl(absPos.x + elem->margins[3], absPos.y + elem->margins[0], viewSizex, viewSizey,
					drawProgress, -30.0f, curlAngle,
					0xFFFFFFFF,
					drawFromUvX, drawFromUvY, uvW, uvH,
					drawToUvX, drawToUvY,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0,
					&readInfo->handle, &readInfo->handle, 10000 + std::floor(z),
					g->fb, g->fbsize, g->viewId, 0.0f, axis == 0, elem->offscreen->spread);
				// アニメーション進行
				elem->offscreen->curlProgress += g->deltaTime * 2.0f;
				if (elem->offscreen->curlProgress > endProgress) {
					elem->offscreen->curlProgress = -1.0f;
					// アニメーション完了、scrollを更新
					if (axis == 0) elem->scroll.y = toPage * viewSizey;
					else elem->scroll.x = toPage * viewSizex;
				}
				elem->offscreen->markPaint(local);
			} else {
				auto* bg = elem->background;
				float imgX = absPos.x + elem->margins[3];
				float imgY = absPos.y + elem->margins[0];
				float imgW = viewSizex;
				float imgH = viewSizey;
				g->layer->pushImage(imgX, imgY, imgW, imgH,
					(elem->scroll.x) / size3x, (elem->scroll.y) / size3y, (imgW + tabExtL + tabExtR) / size3x, (imgH + tabExtT + tabExtB) / size3y,
					bg ? bg->bs.boRadiuses[0] : 0, bg ? bg->bs.boRadiuses[1] : 0, bg ? bg->bs.boRadiuses[2] : 0, bg ? bg->bs.boRadiuses[3] : 0,
					bg ? bg->aa : 0,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0, 0xFFFFFFFF,
					10000 + std::floor(z), &readInfo->handle, g->fb, g->fbsize, g->viewId,
					0.0f,
					bg ? bg->cornerPattern : 0.0f,
					bg ? bg->backUVMinX : 0.0f);
			}
			if (g->paint == Offscreen::PaintOffscreen) return;
			auto writeInfo = mygetStandaloneTextureInfo(thgc, writeId);
			uint64_t offscreenViewId = ::viewId++;
			g2 = NewGraphic{g->layer, elem, elem, {tabExtL, tabExtT}, {elem->size2.x + tabExtL + tabExtR, elem->size2.y + tabExtT + tabExtB}, {0,0}, {0,0},
				writeId, &writeInfo->fbo, &writeInfo->size, elem->offscreen->viewId = offscreenViewId, 0, elem->offscreen->group };
		}
		elem->offscreen->ping = !elem->offscreen->ping;
		g2.winFb = g->winFb; g2.winFbsize = g->winFbsize; g2.winViewId = g->winViewId;
		g2.zIndex = z;
	}
	else {
		g2 = NewGraphic{ g->layer, elem, elem, {g->pos.x, g->pos.y}, {elem->size2.x, elem->size2.y}, {0,0}, {0,0},
			g->im,  g->fb,  g->fbsize, g->viewId, 0, g->group };
		g2.pos.x += elem->margins[3];
		g2.pos.y += elem->margins[0];
		g2.zIndex = g->zIndex + elem->zIndex;
	}
	g2.start = g->pos;
	if (elem->background != NULL) {
		bgfx::TextureHandle* tex1, * tex2, * tex3, * tex4;
		if (isValidImageId(elem->background->tex1)) {
			auto info = mygetStandaloneTextureInfo(thgc, elem->background->tex1);
			tex1 = &info->handle;
		}
		else tex1 = &nulltex;
		if (isValidImageId(elem->background->tex2)) {
			auto info = mygetStandaloneTextureInfo(thgc, elem->background->tex2);
			tex2 = &info->handle;
		}
		else tex2 = &nulltex;
		if (isValidImageId(elem->background->tex3)) {
			auto info = mygetStandaloneTextureInfo(thgc, elem->background->tex3);
			tex3 = &info->handle;
		}
		else tex3 = &nulltex;
		if (isValidImageId(elem->background->tex4)) {
			auto info = mygetStandaloneTextureInfo(thgc, elem->background->tex4);
			tex4 = &info->handle;
		}
		else tex4 = &nulltex;
		bool hasOffscreen = elem->offscreen != NULL;
		float bgW = std::max(elem->size2.x, elem->size.x + elem->paddings[1] + elem->paddings[3]) + tabExtL + tabExtR;
		float bgH = std::max(elem->size2.y, elem->size.y + elem->paddings[0] + elem->paddings[2]) + tabExtT + tabExtB;
		float noBo[4] = {0, 0, 0, 0};
		g2.layer->pushBackground(elem->background, g2.pos.x - tabExtL, g2.pos.y - tabExtT, bgW, bgH, std::floor(g2.zIndex),
			tex1, tex2, tex3, tex4, g2.fb, g2.fbsize, g2.viewId, hasOffscreen ? 1.0f : 0.0f,
			hasOffscreen ? noBo : elem->boWidthes,
			hasOffscreen ? &noBorderShadow : &elem->background->bs);
		std::vector<float>* colors = new std::vector<float>{
			1.0f, 0.0f, 0.0f, 1.0f,  // ��
			0.0f, 1.0f, 0.0f, 1.0f,  // ��
			0.0f, 0.0f, 1.0f, 1.0f,  // ��
		};
	}
	g2.pos.x += elem->boWidthes[3] + elem->paddings[3];
	g2.pos.y += elem->boWidthes[0] + elem->paddings[0];
	float baseX = g2.pos.x, baseY = g2.pos.y;
	NewElement* start = elem->childend->next;
	//background.draw
	float szx = 0, szy = 0;
	for (NewElement* child = elem->childend->next; child != elem->childend; ) {
		if (child->offscreen != NULL) {
			child = child->next; continue;
		}
		g2.pos.x = baseX + child->pos2.x + child->pos.x;
		g2.pos.y = baseY + child->pos2.y + child->pos.y;
		child->Draw(child->gc, child, &g2, child->gc->local, q);
		child = child->next;
	}
	// スクロールバー / ページングバー描画 (ウィンドウFBOに絶対座標で直接描画)
	if (g2.winFb) {
		PointF absPos2 = getAbsolutePosition(elem);
		if (baseType(elem->ytype) == SizeType::Scroll) {
			drawRightScrollBar(g2.layer,
				absPos2.x + viewSizex - sbarx * 3 / 2, absPos2.y, sbarx, viewSizey - sbary, sbarx * 3 / 2,
				elem->scroll.y, viewSizey, elem->size2.y,
				10001.0f,
				g2.winFb, g2.winFbsize, g2.winViewId);
		}
		if (baseType(elem->xtype) == SizeType::Scroll) {
			drawUnderScrollBar(g2.layer,
				absPos2.x, absPos2.y + viewSizey - sbary * 3 / 2, viewSizex - sbarx, sbary, sbary * 3 / 2,
				elem->scroll.x, viewSizex, elem->size2.x,
				10001.0f,
				g2.winFb, g2.winFbsize, g2.winViewId);
		}
		if (baseType(elem->ytype) == SizeType::Page) {
			float totalPages = std::ceil(elem->size2.y / viewSizey);
			float currentPage = std::floor(elem->scroll.y / viewSizey);
			drawRightPagingBar(g2.layer, *getAtlas(thgc), getFont("sans", 16),
				absPos2.x + viewSizex - sbarx, absPos2.y, sbarx, viewSizey - sbary,
				currentPage, totalPages,
				10001.0f, g2.group,
				g2.winFb, g2.winFbsize, g2.winViewId);
		}
		if (baseType(elem->xtype) == SizeType::Page) {
			float totalPages = std::ceil(elem->size2.x / viewSizex);
			float currentPage = std::floor(elem->scroll.x / viewSizex);
			drawUnderPagingBar(g2.layer, *getAtlas(thgc), getFont("sans", 16),
				absPos2.x, absPos2.y + viewSizey - sbary, viewSizex - sbarx, sbary,
				currentPage, totalPages,
				10001.0f, g2.group,
				g2.winFb, g2.winFbsize, g2.winViewId);
		}
	}

	// タイル描画状態を親の値で復元 (nullptr で潰すと外側のタイル展開が壊れる)
	g->layer->tiledTarget         = _saveTiledTarget;
	g->layer->tiledTargetImageId  = _saveTiledTargetImageId;
	g->layer->tiledPlaceholderFBO = _saveTiledPlaceholderFBO;
	g->layer->tiledBaseViewId     = _saveTiledBaseViewId;
}
void initLocal(ThreadGC* thgc, NewLocal* local, NativeWindow* mainWindow = nullptr) {
	thgc->local = local;
	local->gc = thgc;
	local->next = local->before = (NewElement*)local;
	local->parent = NULL;
	NewEndElement* end = (NewEndElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, end, (NewElement*)local);
	end->id = std::numeric_limits<uint64_t>::max();
	local->childend = (NewElement*)end;
	local->margins[0] = local->margins[1] = local->margins[2] = local->margins[3] = 0;
	local->paddings[0] = local->paddings[1] = local->paddings[2] = local->paddings[3] = 0;
	local->pos.x = local->pos.y = 0;
	local->offscreen = new Offscreen();
	local->scroll.x = local->scroll.y = 0;
	local->size.x = 1200;
	local->size.y = 800;
	local->size2.x = local->size2.y = 0;
	local->xtype = local->ytype = SizeType::Scroll;
	local->position = Relative;
	local->layout = ocupay;
	local->type = LetterType::_Main;
	local->Measure = ElementMeasure;
	local->Draw = ElementDraw;
	local->Mouse = ElementMouse;
	local->Key = ElementKey;
	local->DrawSelection = ElementDrawSelect;
	local->len = len1;
	local->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
	local->offscreen->group = &createGroup(thgc);
	local->offscreen->elem = local;
	local->offscreen->type = OffscreenType::Off;
	local->offscreen->markLayout(local, NULL);
	local->offscreen->curlProgress = -1.0f;
	local->offscreen->window = mainWindow;
	local->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
	local->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
	local->offscreen->next = local->offscreen->before = local->offscreen;
	local->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
	local->offscreen->child->next = local->offscreen->child->before = local->offscreen->child;
	local->offscreen->child->parent = local->offscreen;
	local->offscreened = true;
	local->background = (Background*)GC_alloc(thgc, CType::_Background);
	local->background->tex1 = myloadTexture2D(thgc, "123.png", ImageUsage::Background);
	local->background->fillcolor = 0xffffffff;
	local->background->bs.borderColor = 0x000000FF;
	local->boWidthes[0] = local->boWidthes[1] = local->boWidthes[2] = local->boWidthes[3] = 0.0f;
	local->background->bs.shadowBlur = 1.0f;
	local->background->bs.shadowColor = 0x00000000;
	local->dirty = DirtyType::RebuildValue;
	local->screens = create_list(thgc, sizeof(Offscreen*), CType::_Offscreen);
	if (!thgc->hoveredList) thgc->hoveredList = create_list(thgc, sizeof(NewElement*), CType::_ElementC);
	if (!thgc->hoveredSpanList) thgc->hoveredSpanList = create_list(thgc, sizeof(HoveredSpan*), CType::_HoveredSpan);
	local->editable = Editable;
	local->select.from = NULL;
	local->linked = NULL;
	local->animate = -1;
}
#include "Compile.h"
void initElement(ThreadGC* thgc, NewElement* line) {
	line->gc = thgc;
	line->type = LetterType::_Element;
	line->next = line;
	line->before = line;
	line->parent = NULL;
	line->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, (NewEndElement*)line->childend, line);
	line->id = 0;
	line->Measure = ElementMeasure;
	line->Draw = ElementDraw;
	line->Mouse = ElementMouse;
	line->Key = ElementKey;
	line->DrawSelection = ElementDrawSelect;
	line->len = len1;
	line->animate = -1;
}
void initLine(ThreadGC* thgc, NewLine* line) {
	line->gc = thgc;
	line->type = LetterType::_Line;
	line->next = line;
	line->before = line;
	line->parent = NULL;
	line->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, (NewEndElement*)line->childend, line);
	line->orient = true;
	line->id = 0;
	line->Measure = LineMeasure;
	line->Draw = ElementDraw;
	line->Mouse = LineMouse;
	line->Key = ElementKey;
	line->DrawSelection = ElementDrawSelect;
	line->len = len1;
	line->animate = -1;
	// 作成直後から SIValues を確保しておく (TABLE_POS_SLOT / CALLBACK_SLOT 等を使うため)。
	ensureSIValues(thgc, line);
}
void MeasureStringVertical(FontAtlas& atlas, FontId font, String* text, int n, float height, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group);
void drawStringVertical(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
void drawStringRange(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, int offset, int count, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
void drawStringScaledRange(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, int offset, int count, float x, float y, float scaleX,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
void drawStringVerticalRange(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, int offset, int count, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
RetMeasureType LetterMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	NewLetter* letter = (NewLetter*)elem;
	if (letter->recompile) {
		List* list = Compile(thgc, letter->text, letter->font, elem);
		letter->atsspans = *(List**)get_list(list, 0);
		if (list->size > 1) {
			String* text = letter->text;
			NewElement* nowline = elem->parent;
			NewLetter* let = letter;
			int m = 0;
			for (int i = 1; ; i++) {
				if (i == list->size) {
					let->text = SubString(thgc, text, m, text->size - m);
					break;
				}
				List* list2 = *(List**)get_list(list, i);
				ATSSpan* atsspan = *(ATSSpan**)get_list(list2, 0);
				NewLine* line = (NewLine*)GC_alloc(thgc, CType::_LineC);
				let->text = SubString(thgc, text, m, atsspan->start - m);
				m = atsspan->end;
				initLine(thgc, line);
				NewNext(thgc, local, nowline, line);
				let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
				initLetter(thgc, let, letter->font, LetterType::_Letter);
				let->recompile = false;
				remove_list(thgc, list2, 0);
				let->atsspans = list2;
				for (int j = 0; j < list2->size; j++) {
					ATSSpan* a = *(ATSSpan**)get_list(list2, j);
					a->start -= atsspan->start;
					a->end -= atsspan->start;
					a->letter = let;
				}
				NewNext(thgc, local, line->childend, let);
				nowline = line;
			}
		}
		letter->recompile = false;
		if (letter->parent->parent->type == LetterType::_TextBox) ((TextBox*)letter->parent->parent)->recompile = true;
	}
	List* boundaries = create_list(thgc, sizeof(int), (CType)0);
	for (int i = 0; i < letter->atsspans->size; i++) {
		ATSSpan* a = *(ATSSpan**)get_list(letter->atsspans, i);
		add_list(thgc, boundaries, (char*)(intptr_t)a->start);
		add_list(thgc, boundaries, (char*)(intptr_t)a->end);
	}
	for (int i = 0; i < letter->stylespans->size; i++) {
		StyleSpan* s = *(StyleSpan**)get_list(letter->stylespans, i);
		add_list(thgc, boundaries, (char*)(intptr_t)s->start);
		add_list(thgc, boundaries, (char*)(intptr_t)s->end);
	}
	// sort + unique
	std::sort((int*)boundaries->data, (int*)boundaries->data + boundaries->size);
	int* newEnd = std::unique((int*)boundaries->data, (int*)boundaries->data + boundaries->size);
	boundaries->size = (int)(newEnd - (int*)boundaries->data);

	// linespans + renderspans を生成: 親のorientと折り返し設定で分岐
	// LetterのparentがLineの場合、Lineの親(grandparent)からorient/xtype/sizeを取得
	NewElement* parent = elem->parent;
	bool isLine = parent && parent->Measure == LineMeasure && parent->parent;
	NewElement* layoutParent = isLine ? parent->parent : parent;
	bool parentOrient = layoutParent ? (isLine ? !layoutParent->orient : layoutParent->orient) : true;
	bool wrapEnabled = (parentOrient ? measure->breakxelem : measure->breakyelem);
	float letterMBP = parentOrient
		? (elem->margins[1] + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3])
		: (elem->margins[0] + elem->margins[2] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2]);
	float parentLimit = wrapEnabled ? (parentOrient ? measure->floatRightX - measure->pos.x: measure->floatRightY - measure->pos.y) - letterMBP : 10000.0f;

	// measure->start は親のmbpより前の位置、measure->posは親content area内の累積位置
	// drawではg->pos(親のmbp+pos2+pos含む) + letter自身のmbp + letter->pos + rs->x
	// なのでrs->xの基準は letterのmbp+pos を差し引いた値にする
	float lineStart = 0;
	float curMain = 0;
	float startMain = curMain;
	float curCross = 0.0f;
	float lineSize = 0.0f;
	float maxMain = 0.0f;

	// 前回生成したBreakLetterを削除
	for (int i = 0; i < letter->breakletters->size; i++) {
		BreakLetter* oldBl = *(BreakLetter**)get_list(letter->breakletters, i);
		NewRemove(thgc, local, (NewElement*)oldBl);
	}
	letter->breakletters = create_list(thgc, sizeof(BreakLetter*), CType::_BreakLetterC);
	letter->renderspans = create_list(thgc, sizeof(RenderSpan*), CType::_RenderSpan);
	// currentSpans: 現在の視覚行に追加するrenderspanリスト (最初はletter->renderspans)
	List* currentSpans = letter->renderspans;
	NewElement* insertAfter = elem; // BreakLetterの挿入位置 (最後に挿入した要素の後)
	float firstLineMaxMain = 0.0f;
	for (int i = 0; i + 1 < boundaries->size; ++i) {
		int segStart = *(int*)get_list(boundaries, i);
		int segEnd = *(int*)get_list(boundaries, i + 1);

		uint32_t color = 0x000000FF;
		FontId* font = &elem->parent->parent->font;
		StyleSpan* segStyle = nullptr;
		ATSSpan* segAts = nullptr;

		bool applied = false;
		for (int j = 0; j < letter->stylespans->size; j++) {
			StyleSpan* s = *(StyleSpan**)get_list(letter->stylespans, j);
			if (s->start <= segStart && s->end >= segEnd) {
				color = s->color;
				font = &s->font;
				segStyle = s;
				applied = true;
				break;
			}
		}
		if (!applied) {
			for (int j = 0; j < letter->atsspans->size; j++) {
				ATSSpan* a = *(ATSSpan**)get_list(letter->atsspans, j);
				if (a->start <= segStart && a->end >= segEnd) {
					color = a->color;
					font = &a->font;
					segAts = a;
					break;
				}
			}
		}
		int curStart = segStart;
		while (curStart < segEnd) {
			float w, h;
			size_t n;
			float remainMain = parentLimit - curMain;
			if (remainMain < 0) remainMain = 0;
			if (parentOrient) {
				MeasureStringRange(*getAtlas(thgc), *font, letter->text, curStart, segEnd - curStart, wrapEnabled ? remainMain : 10000, &w, &h, &n, measure->group);
			} else {
				MeasureStringVerticalRange(*getAtlas(thgc), *font, letter->text, curStart, segEnd - curStart, wrapEnabled ? remainMain : 10000, &w, &h, &n, measure->group);
			}
			if (n < (size_t)(segEnd - curStart) && wrapEnabled) {
				// 折り返し発生: 収まる分を現在行に追加
				if (n != 0) {
					RenderSpan* rs0 = (RenderSpan*)GC_alloc(thgc, CType::_RenderSpan);
					rs0->font = *font;
					rs0->color = color;
					rs0->atsspan = segAts;
					rs0->stylespan = segStyle;
					rs0->start = curStart;
					rs0->end = curStart + (int)n;
					rs0->width = w;
					rs0->height = h;
					if (parentOrient) { rs0->x = curMain; rs0->y = curCross; }
					else { rs0->x = curCross; rs0->y = curMain; }
					add_list(thgc, currentSpans, (char*)rs0);
					curStart += (int)n;
					curMain += (parentOrient ? w : h);
				}
				float spanCross = parentOrient ? h : w;
				if (lineSize < spanCross) lineSize = spanCross;
				// 現在行のサイズ記録
				if (currentSpans == letter->renderspans) {
					// Letterのサイズ = 1行目
					firstLineMaxMain = curMain;
					if (parentOrient) {
						letter->size2.x = firstLineMaxMain; letter->size2.y = lineSize;
					} else {
						letter->size2.x = lineSize; letter->size2.y = firstLineMaxMain;
					}
					letter->size = letter->size2;
					letter->pos2.x = 0; letter->pos2.y = 0;
				} else {
					// 前のBreakLetterのサイズ確定
					BreakLetter* prevBl = *(BreakLetter**)get_list(letter->breakletters, letter->breakletters->size - 1);
					if (parentOrient) {
						prevBl->size2.x = curMain; prevBl->size2.y = lineSize;
					} else {
						prevBl->size2.x = lineSize; prevBl->size2.y = curMain;
					}
					prevBl->size = prevBl->size2;
				}
				if (parentOrient) insertAfter->size = { curMain - startMain, lineSize };
				else insertAfter->size = { lineSize, curMain - startMain };
				curCross += lineSize;
				lineSize = 0.0f;
				// BreakLetter生成: 次の視覚行、継続位置をセットしてreturn
				BreakLetter* bl = (BreakLetter*)GC_alloc(thgc, CType::_BreakLetterC);
				initBreakLetter(thgc, bl, letter);
				bl->boundaries = boundaries;
				bl->charPos = curStart;
				NewNext(thgc, local, insertAfter, (NewElement*)bl);
				add_list(thgc, letter->breakletters, (char*)bl);
				return RetMeasure_Break;
			}
			// renderspanを現在の視覚行に追加
			RenderSpan* r = (RenderSpan*)GC_alloc(thgc, CType::_RenderSpan);
			r->font = *font;
			r->color = color;
			r->atsspan = segAts;
			r->stylespan = segStyle;
			r->start = curStart;
			r->end = curStart + (int)n;
			r->width = w;
			r->height = h;
			float spanMain = parentOrient ? w : h;
			float spanCross = parentOrient ? h : w;
			if (parentOrient) { r->x = curMain; r->y = 0; }
			else { r->x = 0; r->y = curMain; }
			add_list(thgc, currentSpans, (char*)r);
			curMain += spanMain;
			if (lineSize < spanCross) lineSize = spanCross;
			curStart += (int)n;
		}
	}
	if (parentOrient) elem->size = { curMain - startMain, lineSize };
	else elem->size = { lineSize, curMain - startMain };
	return RetMeasure_None;
}
void SelectDraw(ThreadGC* thgc, NewLocal* local, NewGraphic* g, RenderCommandQueue* q) {
	if (local->select.m == local->select.from->len(local->select.from)) {
		local->select.from = local->select.from->next;
		local->select.fromid = local->select.from->id;
		local->select.m = 0;
	}
	if (local->select.n == local->select.to->len(local->select.to)) {
		local->select.to = local->select.to->next;
		local->select.toid = local->select.to->id;
		local->select.n = 0;
	}
	if (local->select.fromid == local->select.toid) {
		if (local->select.m <= local->select.n) {
			local->select.start = local->select.from;
			local->select.end = local->select.to;
			local->select.s = local->select.m;
			local->select.e = local->select.n;
		}
		else {
			local->select.start = local->select.to;
			local->select.end = local->select.from;
			local->select.s = local->select.n;
			local->select.e = local->select.m;
		}
	}
	else if (local->select.fromid < local->select.toid) {
		local->select.start = local->select.from;
		local->select.end = local->select.to;
		local->select.s = local->select.m;
		local->select.e = local->select.n;
	}
	else {
		local->select.start = local->select.to;
		local->select.end = local->select.from;
		local->select.s = local->select.n;
		local->select.e = local->select.m;
	}
	NewElement* start = local->select.start;
	int s = local->select.s;
	PointF pos = getAbsolutePosition(start->parent);
	std::vector<PointF> ps;
	ps.push_back(pos);
	for (;;) {
		pos.x += start->pos2.x - start->parent->scroll.x;
		pos.y += start->pos2.y - start->parent->scroll.y;
		if (start == local->select.end) {
			start->DrawSelection(start->gc, start->gc->local, start, s, local->select.e, g, pos, q);
			break;
		}
		start->DrawSelection(start->gc, start->gc->local, start, s, start->len(start), g, pos, q);
		s = 0;
		if (start->childend != NULL) {
			start = start->childend->next;
			ps.push_back(pos);
			continue;
		}
		else if (start == start->parent->childend) {
			start = start->parent->next;
			ps.pop_back();
			if (start == start->next) start = start->parent;
			if (ps.size() == 0) {
				if (start == NULL) break;
				pos = getAbsolutePosition(start->parent);
				ps.push_back(pos);
			}
			else pos = ps.back();
			continue;
		}
		start = start->next;
		pos = ps.back();
	}
}
void ElementDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q) {
	return;
}
void EndDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q) {
	if (self->parent && self->parent->parent &&
		self->parent->parent->editable == EditType::Editable) {
		FontId font = self->parent->parent->font;
		bool parentOrient = (self->parent->Measure == LineMeasure)
			? !self->parent->parent->orient
			: self->parent->orient;
		float x = pos.x + self->pos.x;
		float y = pos.y + self->pos.y;
		float w, h;
		size_t n2;
		String* ch = createString(thgc, "A", 1, 1);
		if (parentOrient) {
			MeasureString(*getAtlas(thgc), font, ch, 1, 10000, &w, &h, &n2, NULL);
			g->layer->pushFill(x, y, 1.0f, h,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0x4477ff66, 0, 0.0f, 0.0f, 1.0f, 0, 12000.0f,
				g->fb, g->fbsize, g->viewId);
		} else {
			MeasureStringVertical(*getAtlas(thgc), font, ch, 1, 10000, &w, &h, &n2, NULL);
			g->layer->pushFill(x, y, w, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0x4477ff66, 0, 0.0f, 0.0f, 1.0f, 0, 12000.0f,
				g->fb, g->fbsize, g->viewId);
		}
	}
}
void LetterDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q) {
	NewLetter* letter = (NewLetter*)self;
	int l = 0, r = letter->renderspans->size - 1;
	while (l <= r) {
		int m2 = (l + r) / 2;
		auto s = *(RenderSpan**)get_list(letter->renderspans, m2);
		if (m < s->start)
			r = m2 - 1;
		else if (m >= s->end)
			l = m2 + 1;
		else{
			float w0, h0;
			size_t n2;
			MeasureStringRange(*getAtlas(thgc), s->font, letter->text, s->start, m - s->start, 10000, &w0, &h0, &n2, NULL);
			for (;;) {
				if (n <= s->end) {
					float w, h;
					MeasureStringRange(*getAtlas(thgc), s->font, letter->text, s->start, n - s->start, 10000, &w, &h, &n2, NULL);
					g->layer->pushFill(pos.x + self->pos.x + s->x + w0, pos.y + self->pos.y + s->y, w - w0 + 1, s->height, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x4477ff66, 0, 0.0f, 0.0f, 1.0f, 0, 12000.0f, g->fb, g->fbsize, g->viewId);
					break;
				}
				else {
					g->layer->pushFill(pos.x + self->pos.x + s->x + w0, pos.y + self->pos.y + s->y, s->width - w0 + 1, s->height, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x4477ff66, 0, 0.0f, 0.0f, 1.0f, 0, 12000.0f, g->fb, g->fbsize, g->viewId);
				}
				w0 = 0.0f; h0 = 0.0f;
				m2++;
				s = *(RenderSpan**)get_list(letter->renderspans, m2);
			}
			break;
		}
	}
	return;
}
void BreakLetterDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q) {
	BreakLetter* bl = (BreakLetter*)self;
	NewLetter* letter = bl->origin;
	int l = 0, r = bl->renderspans->size - 1;
	while (l <= r) {
		int m2 = (l + r) / 2;
		auto s = *(RenderSpan**)get_list(bl->renderspans, m2);
		if (m < s->start)
			r = m2 - 1;
		else if (m >= s->end)
			l = m2 + 1;
		else{
			float w0, h0;
			size_t n2;
			MeasureStringRange(*getAtlas(thgc), s->font, letter->text, s->start, m - s->start, 10000, &w0, &h0, &n2, NULL);
			for (;;) {
				if (n <= s->end) {
					float w, h;
					MeasureStringRange(*getAtlas(thgc), s->font, letter->text, s->start, n - s->start, 10000, &w, &h, &n2, NULL);
					g->layer->pushFill(pos.x + self->pos.x + s->x + w0, pos.y + self->pos.y + s->y, w - w0 + 1, s->height, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x4477ff66, 0, 0.0f, 0.0f, 1.0f, 0, 12000.0f, g->fb, g->fbsize, g->viewId);
					break;
				}
				else {
					g->layer->pushFill(pos.x + self->pos.x + s->x + w0, pos.y + self->pos.y + s->y, s->width - w0 + 1, s->height, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x4477ff66, 0, 0.0f, 0.0f, 1.0f, 0, 12000.0f, g->fb, g->fbsize, g->viewId);
				}
				w0 = 0.0f; h0 = 0.0f;
				m2++;
				if (m2 >= bl->renderspans->size) break;
				s = *(RenderSpan**)get_list(bl->renderspans, m2);
			}
			break;
		}
	}
}
bgfx::FrameBufferHandle nullfb = BGFX_INVALID_HANDLE;
void LetterDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	NewLetter* letter = (NewLetter*)elem;
	bool parentOrient = elem->parent ? elem->parent->orient : true;
	for (int i = 0; i < letter->renderspans->size; i++) {
		auto rs = *(RenderSpan**)get_list(letter->renderspans, i);
		float drawX = g->pos.x + letter->margins[3] + letter->boWidthes[3] + letter->paddings[3] + letter->pos.x + rs->x;
		float drawY = g->pos.y + letter->margins[0] + letter->boWidthes[0] + letter->paddings[0] + letter->pos.y + rs->y;
		float z = std::floor(g->zIndex + elem->zIndex) + 0.9f;
		// hover状態ならhoverColorを優先
		bool isHovered = (rs->atsspan && rs->atsspan->hovered) || (rs->stylespan && rs->stylespan->hovered);
		uint32_t drawColor = rs->color;
		if (isHovered) {
			uint32_t hc = rs->stylespan && rs->stylespan->hoverColor ? rs->stylespan->hoverColor
			            : rs->atsspan && rs->atsspan->hoverColor ? rs->atsspan->hoverColor : 0;
			if (hc) drawColor = hc;
		}
		// underline色の決定
		uint32_t ulColor = 0;
		if (isHovered) {
			ulColor = rs->stylespan && rs->stylespan->hoverUnderlineColor ? rs->stylespan->hoverUnderlineColor
			        : rs->atsspan && rs->atsspan->hoverUnderlineColor ? rs->atsspan->hoverUnderlineColor : 0;
		}
		if (!ulColor) {
			ulColor = rs->stylespan && rs->stylespan->underlineColor ? rs->stylespan->underlineColor
			        : rs->atsspan && rs->atsspan->underlineColor ? rs->atsspan->underlineColor : 0;
		}
		if (parentOrient) {
			drawStringRange((LayerInfo*)g->layer, *getAtlas(thgc), rs->font, letter->text, rs->start, rs->end - rs->start, drawX, drawY, z,
				drawColor, g->group, g->fb, g->fbsize, g->viewId);
			if (ulColor) {
				g->layer->pushFill(drawX, drawY + rs->height - 1.0f, rs->width, 1.0f,
					0, 0, 0, 0, 0, 0, 0, 0, 0,
					ulColor, 0, 0, 0, 0, 0, z,
					g->fb, g->fbsize, g->viewId);
			}
		} else {
			drawStringVerticalRange((LayerInfo*)g->layer, *getAtlas(thgc), rs->font, letter->text, rs->start, rs->end - rs->start, drawX, drawY, z,
				drawColor, g->group, g->fb, g->fbsize, g->viewId);
			if (ulColor) {
				g->layer->pushFill(drawX + rs->width - 1.0f, drawY, 1.0f, rs->height,
					0, 0, 0, 0, 0, 0, 0, 0, 0,
					ulColor, 0, 0, 0, 0, 0, z,
					g->fb, g->fbsize, g->viewId);
			}
		}
	}
}
int LetterLen(NewElement* elem) {
	NewLetter* letter = (NewLetter*)elem;
	return letter->text->size;
}
void initLetter(ThreadGC* thgc, NewLetter* letter, FontId font, enum LetterType type) {
	letter->gc = thgc;
	letter->type = type;
	letter->next = letter->before = letter;
	letter->parent = NULL;
	letter->childend = NULL;
	letter->id = 0;
	letter->Measure = LetterMeasure;
	letter->Draw = LetterDraw;
	letter->Mouse = LetterMouse;
	letter->Key = LetterKey;
	letter->DrawSelection = LetterDrawSelect;
	letter->len = LetterLen;
	letter->text = NULL;
	letter->font = font;
	letter->atsspans = create_list(thgc, sizeof(ATSSpan*), CType::_ATSSpan);
	letter->stylespans = create_list(thgc, sizeof(StyleSpan*), CType::_StyleSpan);
	letter->renderspans = create_list(thgc, sizeof(RenderSpan*), CType::_RenderSpan);
	letter->breakletters = create_list(thgc, sizeof(BreakLetter*), CType::_BreakLetterC);
	letter->recompile = true;
	letter->animate = -1;
}
RetMeasureType BreakLetterMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	BreakLetter* bl = (BreakLetter*)elem;
	if (!bl->measured) {
		// 今回のサイクルでLetterMeasureが呼ばれていない → 不要
		NewRemove(thgc, local, elem);
		return RetMeasure_None;
	}
	bl->measured = false; // 次サイクル用にリセット
	NewLetter* letter = bl->origin;
	List* boundaries = bl->boundaries;

	// parentOrient/wrapEnabled/parentLimit を再計算
	NewElement* parent = elem->parent;
	bool isLine = parent && parent->Measure == LineMeasure && parent->parent;
	NewElement* layoutParent = isLine ? parent->parent : parent;
	bool parentOrient = layoutParent ? (isLine ? !layoutParent->orient : layoutParent->orient) : true;
	bool wrapEnabled = (parentOrient ? measure->breakxelem : measure->breakyelem);
	float letterMBP = parentOrient
		? (elem->margins[1] + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3])
		: (elem->margins[0] + elem->margins[2] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2]);
	float parentLimit = wrapEnabled ? (parentOrient ? measure->floatRightX - measure->pos.x : measure->floatRightY - measure->pos.y) - letterMBP : 10000.0f;

	float curMain = 0;
	float startMain = curMain;
	float lineSize = 0.0f;

	bl->renderspans = create_list(thgc, sizeof(RenderSpan*), CType::_RenderSpan);
	elem->pos2.x = 0;
	elem->pos2.y = 0;

	// charPosが属するセグメントを特定
	int startSeg = 0;
	for (int k = 0; k + 1 < boundaries->size; ++k) {
		if (*(int*)get_list(boundaries, k + 1) > bl->charPos) { startSeg = k; break; }
	}
	for (int i = startSeg; i + 1 < boundaries->size; ++i) {
		int segStart = *(int*)get_list(boundaries, i);
		int segEnd = *(int*)get_list(boundaries, i + 1);

		uint32_t color = 0x000000FF;
		FontId* font = &elem->parent->parent->font;
		StyleSpan* segStyle = nullptr;
		ATSSpan* segAts = nullptr;

		bool applied = false;
		for (int j = 0; j < letter->stylespans->size; j++) {
			StyleSpan* s = *(StyleSpan**)get_list(letter->stylespans, j);
			if (s->start <= segStart && s->end >= segEnd) {
				color = s->color;
				font = &s->font;
				segStyle = s;
				applied = true;
				break;
			}
		}
		if (!applied) {
			for (int j = 0; j < letter->atsspans->size; j++) {
				ATSSpan* a = *(ATSSpan**)get_list(letter->atsspans, j);
				if (a->start <= segStart && a->end >= segEnd) {
					color = a->color;
					font = &a->font;
					segAts = a;
					break;
				}
			}
		}
		int curStart = (i == startSeg) ? bl->charPos : segStart;
		while (curStart < segEnd) {
			float w, h;
			size_t nn;
			float remainMain = parentLimit - curMain;
			if (remainMain < 0) remainMain = 0;
			if (parentOrient) {
				MeasureStringRange(*getAtlas(thgc), *font, letter->text, curStart, segEnd - curStart, wrapEnabled ? remainMain : 10000, &w, &h, &nn, measure->group);
			} else {
				MeasureStringVerticalRange(*getAtlas(thgc), *font, letter->text, curStart, segEnd - curStart, wrapEnabled ? remainMain : 10000, &w, &h, &nn, measure->group);
			}
			if (nn < (size_t)(segEnd - curStart) && wrapEnabled) {
				// 折り返し発生: 収まる分を現在行に追加（最低1文字は強制）
				if (nn == 0 && curMain == startMain) {
					nn = 1;
					if (parentOrient) MeasureStringRange(*getAtlas(thgc), *font, letter->text, curStart, 1, 10000, &w, &h, &nn, measure->group);
					else MeasureStringVerticalRange(*getAtlas(thgc), *font, letter->text, curStart, 1, 10000, &w, &h, &nn, measure->group);
					nn = 1;
				}
				if (nn != 0) {
					RenderSpan* rs0 = (RenderSpan*)GC_alloc(thgc, CType::_RenderSpan);
					rs0->font = *font;
					rs0->color = color;
					rs0->atsspan = segAts;
					rs0->stylespan = segStyle;
					rs0->start = curStart;
					rs0->end = curStart + (int)nn;
					rs0->width = w;
					rs0->height = h;
					if (parentOrient) { rs0->x = curMain; rs0->y = 0; }
					else { rs0->x = 0; rs0->y = curMain; }
					add_list(thgc, bl->renderspans, (char*)rs0);
					curStart += (int)nn;
					curMain += (parentOrient ? w : h);
					if (h > lineSize) lineSize = h;
				}
				// 現在行のサイズ確定
				if (parentOrient) elem->size = { curMain - startMain, lineSize };
				else elem->size = { lineSize, curMain - startMain };
				// 次のBreakLetter生成、継続位置をセットしてreturn
				BreakLetter* newBl = (BreakLetter*)GC_alloc(thgc, CType::_BreakLetterC);
				initBreakLetter(thgc, newBl, letter);
				newBl->boundaries = boundaries;
				newBl->charPos = curStart;
				NewNext(thgc, local, elem, (NewElement*)newBl);
				add_list(thgc, letter->breakletters, (char*)newBl);
				return RetMeasure_Break;
			}
			// renderspanを現在の視覚行に追加
			RenderSpan* r = (RenderSpan*)GC_alloc(thgc, CType::_RenderSpan);
			r->font = *font;
			r->color = color;
			r->atsspan = segAts;
			r->stylespan = segStyle;
			r->start = curStart;
			r->end = curStart + (int)nn;
			r->width = w;
			r->height = h;
			float spanMain = parentOrient ? w : h;
			float spanCross = parentOrient ? h : w;
			if (parentOrient) { r->x = curMain; r->y = 0; }
			else { r->x = 0; r->y = curMain; }
			add_list(thgc, bl->renderspans, (char*)r);
			curMain += spanMain;
			if (lineSize < spanCross) lineSize = spanCross;
			curStart += (int)nn;
		}
	}
	if (parentOrient) elem->size = { curMain - startMain, lineSize };
	else elem->size = { lineSize, curMain - startMain };
	return RetMeasure_None;
}
void BreakLetterDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	BreakLetter* bl = (BreakLetter*)elem;
	NewLetter* letter = bl->origin;
	bool parentOrient = elem->parent ? elem->parent->orient : true;
	for (int i = 0; i < bl->renderspans->size; i++) {
		auto rs = *(RenderSpan**)get_list(bl->renderspans, i);
		float drawX = g->pos.x + elem->margins[3] + elem->boWidthes[3] + elem->paddings[3] + elem->pos.x + rs->x;
		float drawY = g->pos.y + elem->margins[0] + elem->boWidthes[0] + elem->paddings[0] + elem->pos.y + rs->y;
		float z = std::floor(g->zIndex + elem->zIndex) + 0.9f;
		bool isHovered = (rs->atsspan && rs->atsspan->hovered) || (rs->stylespan && rs->stylespan->hovered);
		uint32_t drawColor = rs->color;
		if (isHovered) {
			uint32_t hc = rs->stylespan && rs->stylespan->hoverColor ? rs->stylespan->hoverColor
			            : rs->atsspan && rs->atsspan->hoverColor ? rs->atsspan->hoverColor : 0;
			if (hc) drawColor = hc;
		}
		// underline色の決定
		uint32_t ulColor = 0;
		if (isHovered) {
			ulColor = rs->stylespan && rs->stylespan->hoverUnderlineColor ? rs->stylespan->hoverUnderlineColor
			        : rs->atsspan && rs->atsspan->hoverUnderlineColor ? rs->atsspan->hoverUnderlineColor : 0;
		}
		if (!ulColor) {
			ulColor = rs->stylespan && rs->stylespan->underlineColor ? rs->stylespan->underlineColor
			        : rs->atsspan && rs->atsspan->underlineColor ? rs->atsspan->underlineColor : 0;
		}
		if (parentOrient) {
			drawStringRange((LayerInfo*)g->layer, *getAtlas(thgc), rs->font, letter->text, rs->start, rs->end - rs->start, drawX, drawY, z,
				drawColor, g->group, g->fb, g->fbsize, g->viewId);
			if (ulColor) {
				g->layer->pushFill(drawX, drawY + rs->height - 1.0f, rs->width, 1.0f,
					0, 0, 0, 0, 0, 0, 0, 0, 0,
					ulColor, 0, 0, 0, 0, 0, z,
					g->fb, g->fbsize, g->viewId);
			}
		} else {
			drawStringVerticalRange((LayerInfo*)g->layer, *getAtlas(thgc), rs->font, letter->text, rs->start, rs->end - rs->start, drawX, drawY, z,
				drawColor, g->group, g->fb, g->fbsize, g->viewId);
			if (ulColor) {
				g->layer->pushFill(drawX + rs->width - 1.0f, drawY, 1.0f, rs->height,
					0, 0, 0, 0, 0, 0, 0, 0, 0,
					ulColor, 0, 0, 0, 0, 0, z,
					g->fb, g->fbsize, g->viewId);
			}
		}
	}
}
int BreakLetterMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	BreakLetter* bl = (BreakLetter*)self;
	NewLetter* letter = bl->origin;
	for (int i = 0; i < bl->renderspans->size; i++) {
		auto rs = *(RenderSpan**)get_list(bl->renderspans, i);
		if (self->pos.x + rs->x <= e->x - pos.x && e->x - pos.x < self->pos.x + rs->x + rs->width) {
			if (self->pos.y + rs->y <= e->y - pos.y && e->y - pos.y < self->pos.y + rs->y + rs->height) {
				// RenderSpanホバー登録
				if (thgc->hoveredSpanList) {
					HoveredSpan* hs = (HoveredSpan*)GC_alloc(thgc, CType::_HoveredSpan);
					hs->letter = letter;
					hs->span = rs;
					hs->atsspan = rs->atsspan;
					hs->stylespan = rs->stylespan;
					add_list(thgc, thgc->hoveredSpanList, (char*)hs);
				}
				float width, hei;
				String* str = SubString(thgc, letter->text, rs->start, rs->end - rs->start);
				size_t n;
				MeasureString(*getAtlas(thgc), rs->font, str, str->size, e->x - pos.x - self->pos.x - rs->x, &width, &hei, &n, e->group);
				if (self->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = (NewElement*)letter;
						local->select.fromid = local->select.toid = letter->id;
						local->select.m = local->select.n = rs->start + n;
						Offscreen* seloff = FindOffscreen(self);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(self);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = (NewElement*)letter;
							local->select.toid = letter->id;
							local->select.n = rs->start + n;
						}
					}
				}
				return n;
			}
		}
	}
	return 0;
}
void initBreakLetter(ThreadGC* thgc, BreakLetter* bl, NewLetter* origin) {
	bl->gc = thgc;
	bl->type = LetterType::_BreakLetter;
	bl->next = bl->before = bl;
	bl->parent = NULL;
	bl->childend = NULL;
	bl->id = 0;
	bl->Measure = BreakLetterMeasure;
	bl->Draw = BreakLetterDraw;
	bl->Mouse = BreakLetterMouse;
	bl->Key = ElementKey;
	bl->DrawSelection = BreakLetterDrawSelect;
	bl->len = len0;
	bl->origin = origin;
	bl->renderspans = create_list(thgc, sizeof(RenderSpan*), CType::_RenderSpan);
	bl->boundaries = NULL;
	bl->charPos = 0;
	bl->measured = true;
	bl->animate = -1;
}
int LineMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	if (self->GoMouseDown != NULL)
		FireEvent(thgc, self, self->GoMouseDown, _MouseEvent, e, sizeof(MouseEvent));
	pos.x += self->pos.x + self->margins[3] + self->boWidthes[3] + self->paddings[3] - self->scroll.x;
	pos.y += self->pos.y + self->margins[0] + self->boWidthes[0] + self->paddings[0] - self->scroll.y;
	if (self->childend != NULL) {
		bool par = self->parent ? !self->parent->orient : self->orient;
		NewElement* elem = self->childend->next;
		if (par) {
			if (e->x - pos.x < elem->pos.x + elem->pos2.x + elem->margins[3]) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->next);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = 0;
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = 0;
						}
					}
				}
			}
		}
		else {
			if (e->y - pos.y < elem->pos.y + elem->pos2.y + elem->margins[0]) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->next);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = 0;
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = 0;
						}
					}
				}
			}
		}
		for (; elem != self->childend; elem = elem->next) {
			float hitL = 0, hitR = 0, hitT = 0, hitB = 0;
			if (elem->type == _Sidelet && elem->background) {
				int dir = (int)(elem->background->cornerPattern - 9.0f);
				switch (dir) {
				case 0: hitT = 18; break;
				case 1: hitR = 18; break;
				case 2: hitB = 18; break;
				case 3: hitL = 18; break;
				}
			}
			if (par) {
				if (elem->pos.x + elem->pos2.x + elem->margins[3] - hitL <= e->x - pos.x && e->x - pos.x < elem->pos.x + elem->pos2.x + elem->size.x + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3] + hitR) {
					if (elem->hoverColor != 0) {
						add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
					}
					elem->Mouse(elem->gc, elem, e, { pos.x + elem->pos2.x, pos.y }, elem->gc->local);
				}
			}
			else {
				if (elem->pos.y + elem->pos2.y + elem->margins[0] - hitT <= e->y - pos.y && e->y - pos.y < elem->pos.y + elem->pos2.y + elem->size.y + elem->margins[0] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2] + hitB) {
					if (elem->hoverColor != 0) {
						add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
					}
					elem->Mouse(elem->gc, elem, e, { pos.x, pos.y + elem->pos2.y}, elem->gc->local);
				}
			}
		}
		if (par) {
			elem = elem->before;
			if (elem->pos.x + elem->pos2.x + elem->size.x + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3] <= e->x - pos.x) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->before);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = el->len(el);
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = el->len(el);
						}
					}
				}
			}
		}
		else {
			elem = elem->before;
			if (elem->pos.y + elem->pos2.y + elem->size.y + elem->margins[0] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2] <= e->y - pos.y) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->before);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = el->len(el);
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = el->len(el);
						}
					}
				}
			}
		}
	}
	if (self->BackMouseDown != NULL)
		FireEvent(thgc, self, self->BackMouseDown, _MouseEvent, e, sizeof(MouseEvent));
	return -1;
}
int ElementMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	if (self->offscreened) e->group = self->offscreen->group;
	if (self->GoMouseDown != NULL)
		FireEvent(thgc, self, self->GoMouseDown, _MouseEvent, e, sizeof(MouseEvent));

	// スクロールバー / ページングバーのクリック判定 (MouseDown時のみ開始)
	if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float sbarx = 0, sbary = 0;
		if (baseType(self->xtype) == SizeType::Scroll) sbary += 5 * 2;
		else if (baseType(self->xtype) == SizeType::Page) sbary += 15;
		if (baseType(self->ytype) == SizeType::Scroll) sbarx += 5 * 2;
		else if (baseType(self->ytype) == SizeType::Page) sbarx += 15;
		float sizex = self->size.x + self->paddings[1] + self->paddings[3];
		float sizey = self->size.y + self->paddings[0] + self->paddings[2];
		float localX = e->x - pos.x - self->pos.x - self->margins[3] - self->boWidthes[3];
		float localY = e->y - pos.y - self->pos.y - self->margins[0] - self->boWidthes[0];

		// 右スクロールバー (ytype == Scroll)
		if (baseType(self->ytype) == SizeType::Scroll && self->size2.y > sizey) {
			float barX = sizex - sbarx;
			float barH = sizey - sbary;
			if (localX >= barX && localX < sizex && localY >= 0 && localY < barH) {
				float maxScroll = self->size2.y - sizey;
				// つまみの位置とサイズを計算
				float thumbRatio = sizey / self->size2.y;
				float thumbH = std::max(barH * thumbRatio, 16.f);
				float scrollRatio = (maxScroll > 0) ? self->scroll.y / maxScroll : 0.f;
				float thumbY = (barH - thumbH) * scrollRatio;
				// つまみ上をクリックしたか
				if (localY >= thumbY && localY < thumbY + thumbH) {
					local->scrollDrag = { self, 0, localY - thumbY };
				} else if (localY < thumbY) {
					// つまみより上: つまみ上端がクリック位置に来る
					float newRatio = std::clamp(localY / (barH - thumbH), 0.f, 1.f);
					self->scroll.y = newRatio * maxScroll;
					local->scrollDrag = { self, 0, 0.f };
				} else {
					// つまみより下: つまみ下端がクリック位置に来る
					float newThumbY = localY - thumbH;
					float newRatio = std::clamp(newThumbY / (barH - thumbH), 0.f, 1.f);
					self->scroll.y = newRatio * maxScroll;
					local->scrollDrag = { self, 0, thumbH };
				}
				if (self->offscreen) self->offscreen->markPaint(local);
				return -1;
			}
		}
		// 下スクロールバー (xtype == Scroll)
		if (baseType(self->xtype) == SizeType::Scroll && self->size2.x > sizex) {
			float barY = sizey - sbary;
			float barW = sizex - sbarx;
			if (localY >= barY && localY < sizey && localX >= 0 && localX < barW) {
				float maxScroll = self->size2.x - sizex;
				// つまみの位置とサイズを計算
				float thumbRatio = sizex / self->size2.x;
				float thumbW = std::max(barW * thumbRatio, 16.f);
				float scrollRatio = (maxScroll > 0) ? self->scroll.x / maxScroll : 0.f;
				float thumbX = (barW - thumbW) * scrollRatio;
				// つまみ上をクリックしたか
				if (localX >= thumbX && localX < thumbX + thumbW) {
					local->scrollDrag = { self, 1, localX - thumbX };
				} else if (localX < thumbX) {
					// つまみより左: つまみ左端がクリック位置に来る
					float newRatio = std::clamp(localX / (barW - thumbW), 0.f, 1.f);
					self->scroll.x = newRatio * maxScroll;
					local->scrollDrag = { self, 1, 0.f };
				} else {
					// つまみより右: つまみ右端がクリック位置に来る
					float newThumbX = localX - thumbW;
					float newRatio = std::clamp(newThumbX / (barW - thumbW), 0.f, 1.f);
					self->scroll.x = newRatio * maxScroll;
					local->scrollDrag = { self, 1, thumbW };
				}
				if (self->offscreen) self->offscreen->markPaint(local);
				return -1;
			}
		}
		// 右ページングバー (ytype == Page)
		if (baseType(self->ytype) == SizeType::Page && self->size2.y > sizey) {
			float barX = sizex - sbarx;
			if (localX >= barX && localX < sizex && localY >= 0 && localY < sizey) {
				float totalPages = std::ceil(self->size2.y / sizey);
				float currentPage = std::floor(self->scroll.y / sizey);
				float toPage = currentPage;
				if (localY < sizey * 0.5f) {
					if (currentPage > 0) toPage = currentPage - 1;
				} else {
					if (currentPage < totalPages - 1) toPage = currentPage + 1;
				}
				if (toPage != currentPage && self->offscreen) {
					self->offscreen->curlFromPage = currentPage;
					self->offscreen->curlToPage = toPage;
					self->offscreen->curlProgress = 0.0f;
					self->offscreen->curlAxis = 0;
					self->offscreen->markPaint(local);
				}
				return -1;
			}
		}
		// 下ページングバー (xtype == Page)
		if (baseType(self->xtype) == SizeType::Page && self->size2.x > sizex) {
			float barY = sizey - sbary;
			if (localY >= barY && localY < sizey && localX >= 0 && localX < sizex) {
				float totalPages = std::ceil(self->size2.x / sizex);
				float currentPage = std::floor(self->scroll.x / sizex);
				float toPage = currentPage;
				if (localX < sizex * 0.5f) {
					if (currentPage > 0) toPage = currentPage - 1;
				} else {
					if (currentPage < totalPages - 1) toPage = currentPage + 1;
				}
				if (toPage != currentPage && self->offscreen) {
					self->offscreen->curlFromPage = currentPage;
					self->offscreen->curlToPage = toPage;
					self->offscreen->curlProgress = 0.0f;
					self->offscreen->curlAxis = 1;
					self->offscreen->markPaint(local);
				}
				return -1;
			}
		}
	}

	pos.x += self->pos.x + self->margins[3] + self->boWidthes[3] + self->paddings[3] - self->scroll.x;
	pos.y += self->pos.y + self->margins[0] + self->boWidthes[0] + self->paddings[0] - self->scroll.y;
	if (self->childend != NULL) {
		NewElement* elem = self->childend->next;
		if (self->orient) {
			if (e->x - pos.x < elem->pos.x + elem->pos2.x + elem->margins[3]) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->next);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = 0;
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = 0;
						}
					}
				}
			}
		}
		else {
			if (e->y - pos.y < elem->pos.y + elem->pos2.y + elem->margins[0]) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->next);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = 0;
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = 0;
						}
					}
				}
			}
		}
		for (; elem != self->childend; elem = elem->next) {
			float hitL = 0, hitR = 0, hitT = 0, hitB = 0;
			if (elem->type == _Sidelet && elem->background) {
				Sidelet* sidelet = (Sidelet*)elem;
				switch (sidelet->direction) {
				case 0: hitT = 18; break;
				case 1: hitR = 18; break;
				case 2: hitB = 18; break;
				case 3: hitL = 18; break;
				}
			}
			if (self->orient) {
				if (elem->pos.x + elem->pos2.x + elem->margins[3] - hitL <= e->x - pos.x && e->x - pos.x < elem->pos.x + elem->pos2.x + elem->size.x + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3] + hitR) {
					if (elem->hoverColor != 0) {
						add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
					}
					elem->Mouse(elem->gc, elem, e, { pos.x + elem->pos2.x, pos.y}, elem->gc->local);
				}
			}
			else {
				if (elem->pos.y + elem->pos2.y + elem->margins[0] - hitT <= e->y - pos.y && e->y - pos.y < elem->pos2.y + elem->size.y + elem->margins[0] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2] + hitB) {
					if (elem->hoverColor != 0) {
						add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
					}
					elem->Mouse(elem->gc, elem, e, { pos.x, pos.y + elem->pos2.y}, elem->gc->local);
				}
			}
		}
		if (self->orient) {
			elem = elem->before;
			if (elem->pos.x + elem->pos2.x + elem->size.x + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3] <= e->x - pos.x) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->before);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = el->len(el);
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = el->len(el);
						}
					}
				}
			}
		}
		else {
			elem = elem->before;
			if (elem->pos.y + elem->pos2.y + elem->size.y + elem->margins[0] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2] <= e->y - pos.y) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->before);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = el->len(el);
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = el->len(el);
						}
					}
				}
			}
		}
	}

	if (self->BackMouseDown != NULL)
		FireEvent(thgc, self, self->BackMouseDown, _MouseEvent, e, sizeof(MouseEvent));
	return -1;
}
void NewMoveElement(ThreadGC* thgc, NewLocal* local, NewElement* before, NewElement* from, NewElement* to) {
	if (from->type == LetterType::_ElemEnd || to->type == LetterType::_ElemEnd) return;
	NewElement* b, *a = before->next;
	if (before->type == _ElemEnd) {
		b = before->parent;
	}
	else b = before;
	from->before->next = to->next;
	to->next->before = from->before;
	to->next = before->next;
	before->next->before = to;
	before->next = from;
	from->before = before;
	int n = 2;
	for (NewElement* elem = from; elem != to; elem = elem->next) n++;
	int c = 1;
	for (; ; from = from->next) {
		from->id = b->id + (a->id - b->id) * c / n;
		from->parent = before->parent;
		if (from == to) break;
	}
	
}
void SelectKey(ThreadGC* thgc, NewLocal* local, KeyEvent* e) {
	if (local->select.from == NULL) return;
	if (local->select.m == local->select.from->len(local->select.from)) {
		local->select.from = local->select.from->next;
		local->select.fromid = local->select.from->id;
		local->select.m = 0;
	}
	if (local->select.n == local->select.to->len(local->select.to)) {
		local->select.to = local->select.to->next;
		local->select.toid = local->select.to->id;
		local->select.n = 0;
	}
	if (local->select.fromid == local->select.toid) {
		if (local->select.m <= local->select.n) {
			local->select.start = local->select.from;
			local->select.end = local->select.to;
			local->select.s = local->select.m;
			local->select.e = local->select.n;
		}
		else {
			local->select.start = local->select.to;
			local->select.end = local->select.from;
			local->select.s = local->select.n;
			local->select.e = local->select.m;
		}
	}
	else if (local->select.fromid < local->select.toid) {
		local->select.start = local->select.from;
		local->select.end = local->select.to;
		local->select.s = local->select.m;
		local->select.e = local->select.n;
	}
	else {
		local->select.start = local->select.to;
		local->select.end = local->select.from;
		local->select.s = local->select.n;
		local->select.e = local->select.m;
	}
	NewElement* start = local->select.start;
	std::vector<NewElement*> vec;
	for (; start != NULL; start = start->parent) vec.push_back(start);
	for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
		NewElement* el = *it;
		if (el->GoKeyDown != NULL)
			FireEvent(thgc, el, el->GoKeyDown, _KeyEvent, e, sizeof(KeyEvent));
	}
	start = local->select.start;
	int s = local->select.s;
	for (;;) {
		if (start == local->select.end) {
			start->Key(thgc, start, s, local->select.e, e, local);
			break;
		}
		NewElement* next = start->next;
		int n = start->Key(thgc, start, s, start->len(start), e, local);
		s = 0;
		if (n == 1) break;
		else if (n == -1) {
			start = next;
			break;
		}
		else if (n == -2) {
			start = start->before->next;
			continue;
		}
		if (start->childend != NULL) {
			start = start->childend->next;
			continue;
		}
		else if (start == start->parent->childend) {
			start = start->parent->next;
			continue;
		}
		start = start->next;
	}
	for (; start != NULL; start = start->parent) {
		if (start->BackKeyDown != NULL)
			FireEvent(thgc, start, start->BackKeyDown, _KeyEvent, e, sizeof(KeyEvent));
	}
}
int ElementKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local) {
	return 0;
}
int AddText(ThreadGC* thgc, NewLocal* local, NewLetter* letter, int m, int n, String* text) {
	if (m == 0 && n == letter->text->size && (text == NULL || text->size == 0)) {
		NewRemove(thgc, local, letter);
		return -1;
	}

	int deleteLen = n - m;
	int insertLen = (text != NULL) ? text->size : 0;
	int delta = insertLen - deleteLen;

	// stylespans を更新
	for (int i = letter->stylespans->size - 1; i >= 0; i--) {
		StyleSpan* s = *(StyleSpan**)get_list(letter->stylespans, i);

		// 削除範囲に完全に含まれるスパンは除去
		if (s->start >= m && s->end <= n) {
			remove_list(thgc, letter->stylespans, i);
			continue;
		}

		// 削除範囲と重なるスパンをクリップ
		if (s->start < n && s->end > m) {
			if (s->start < m) {
				// スパンの左側が残る
				if (s->end <= n) {
					s->end = m;
				}
				else {
					// スパンが削除範囲をまたぐ → 縮める
					s->end += delta;
				}
			}
			else {
				// スパンの右側が残る
				s->start = m + insertLen;
				s->end += delta;
			}
		}
		// 削除範囲より後のスパンはシフト
		else if (s->start >= n) {
			s->start += delta;
			s->end += delta;
		}
	}

	// テキスト更新
	if (text == NULL)
		letter->text = StringAdd2(thgc, SubString(thgc, letter->text, 0, m), SubString(thgc, letter->text, n, letter->text->size - n));
	else
		letter->text = StringAdd2(thgc, StringAdd2(thgc, SubString(thgc, letter->text, 0, m), text), SubString(thgc, letter->text, n, letter->text->size - n));

	letter->recompile = true;
	return 0;
}
void UniteText(ThreadGC* thgc, NewLocal* local, NewElement* before, NewElement* next) {
	if (before->type == LetterType::_Letter && next->type == LetterType::_Letter) {
		NewLetter* bef = (NewLetter*)before, * nex = (NewLetter*)next;
		AddText(thgc, local, bef, bef->len(bef), bef->len(bef), nex->text);
		NewRemove(thgc, local, nex);
	}

}
void UniteLine(ThreadGC* thgc, NewLocal* local, NewLine* before, NewLine* next) {
	for (NewElement* elem = before->childend->before; elem->type != _ElemEnd; ) {
		NewElement* bef = elem->before;
		NewNext(thgc, local, next->childend, elem);
		elem = bef;
	}
	if (local->select.from == before->childend) {
		local->select.from = next->childend;
	}
	else if (local->select.to == before->childend) {
		local->select.to = next->childend;
	}
	// Linked-list から before を消す前に、親が NewTable なら該当 Row も削除しておく
	// (順序: Table 同期 → 後で linked-list から削除)。before->parent が _TableElem なら効く。
	tableRemoveRowForLine(thgc, before);
	NewRemove(thgc, local, before);
}
int EndKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local) {
	if (self->parent->parent->editable == EditType::NoSelect) return 1;
	if (m == 1) return 0;
	if (e->key == SDLK_LEFT) {
		local->select.count = -1;
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (m == 0) {
				NewElement* prevReal = skipBreakBefore(self->before);
				if (prevReal->type == _ElemEnd) {
					if (self->parent->before->type == _ElemEnd) return 0;
					local->select.to = self->parent->before->childend;
					local->select.toid = local->select.to->id;
					local->select.n = 0;
				}
				else {
					local->select.to = prevReal;
					local->select.toid = prevReal->id;
					local->select.n = prevReal->len(prevReal) - 1;
				}
				if (!e->shift) {
					local->select.from = local->select.to;
					local->select.fromid = local->select.to->id;
					local->select.m = local->select.n;
				}
			}
			else {
				local->select.m = local->select.n = m - 1;
				if (!e->shift) {
					local->select.m = local->select.n;
				}
			}
			FindOffscreen(self)->markPaint(local);
			return 0;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (m == 0) {
					NewElement* prevReal = skipBreakBefore(self->before);
					if (prevReal->type == _ElemEnd) {
						if (self->parent->before->type == _ElemEnd) return 1;
						local->select.to = self->parent->before->childend;
						local->select.toid = local->select.to->id;
						local->select.n = 0;
					}
					else {
						local->select.to = prevReal;
						local->select.toid = prevReal->id;
						local->select.n = prevReal->len(prevReal) - 1;
					}
				}
				else {
					local->select.n--;
				}
				FindOffscreen(self)->markPaint(local);
				return 1;
			}
			else {
				local->select.from = local->select.to = local->select.start;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.s;
			}
			FindOffscreen(self)->markPaint(local);
			return 1;
		}
	}
	else if (e->key == SDLK_RIGHT) {
		local->select.count = -1;
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (self->parent->next->type == _ElemEnd) return 0;
			local->select.to = skipBreakNext(self->parent->next->childend->next);
			local->select.toid = local->select.to->id;
			local->select.n = 0;
			if (!e->shift) {
				local->select.from = local->select.to;
				local->select.fromid = local->select.from->id;
				local->select.m = local->select.n;
			}
			FindOffscreen(self)->markPaint(local);
			return 1;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (m == self->len(self)) {
					NewElement* nextReal = skipBreakNext(self->next);
					if (nextReal->type == _ElemEnd) {
						if (self->parent->next->type == _ElemEnd) return 0;
						local->select.to = skipBreakNext(self->parent->next->childend->next);
						local->select.toid = local->select.to->id;
						local->select.n = 0;
					}
					else {
						local->select.to = nextReal;
						local->select.toid = local->select.to->id;
						local->select.n = 1;
					}
				}
				else {
					if (self->type == _ElemEnd) {
						if (self->parent->next->type == _ElemEnd) return 0;
						local->select.to = skipBreakNext(self->parent->next->childend->next);
						local->select.toid = local->select.to->id;
						local->select.n = 0;
					}
					local->select.n++;
				}
				FindOffscreen(self)->markPaint(local);
				return 0;
			}
			else {
				local->select.from = local->select.to = local->select.end;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.e;
			}
			FindOffscreen(self)->markPaint(local);
			return 1;
		}
	}
	else if (e->key == SDLK_UP) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (self->parent->before->type == _ElemEnd) {
				local->select.to = skipBreakNext(self->parent->childend->next);
				local->select.toid = local->select.to->id;
				local->select.n = 0;
			}
			else {
				if (local->select.count < 0) {
					local->select.count = m;
					for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
						local->select.count += elem->len(elem);
					}
				}
				for (NewElement* elem = skipBreakNext(self->parent->before->childend->next); ; elem = skipBreakNext(elem->next)) {
					if (elem->type == _ElemEnd) {
						local->select.to = elem;
						local->select.toid = local->select.to->id;
						local->select.n = 0;
						break;
					}
					else if (local->select.count <= elem->len(elem)) {
						local->select.to = elem;
						local->select.toid = local->select.toid;
						local->select.n = local->select.count;
						break;
					}
				}
			}
			if (!e->shift) {
				local->select.from = local->select.to;
				local->select.fromid = local->select.from->id;
				local->select.m = local->select.n;
			}
			FindOffscreen(self)->markPaint(local);
			return 1;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (self->parent->before->type == _ElemEnd) {
					local->select.to = skipBreakNext(self->parent->childend->next);
					local->select.toid = local->select.to->id;
					local->select.n = 0;
				}
				else {
					if (local->select.count < 0) {
						local->select.count = local->select.n;
						for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
							local->select.count += elem->len(elem);
						}
					}
					for (NewElement* elem = skipBreakNext(self->parent->before->childend->next); ; elem = skipBreakNext(elem->next)) {
						if (elem->type == _ElemEnd) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = 0;
							break;
						}
						else if (local->select.count <= elem->len(elem)) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = local->select.count;
							break;
						}
					}
				}
				FindOffscreen(self)->markPaint(local);
				return 1;
			}
			else {
				local->select.from = local->select.to = local->select.start;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.s;
			}
			FindOffscreen(self)->markPaint(local);
			return 1;
		}
	}
	else if (e->key == SDLK_DOWN) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (self->parent->next->type == _ElemEnd) {
				local->select.to = self->parent->childend;
				local->select.toid = local->select.to->id;
				local->select.n = 0;
			}
			else {
				if (local->select.count < 0) {
					local->select.count = m;
					for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
						local->select.count += elem->len(elem);
					}
				}
				for (NewElement* elem = skipBreakNext(self->parent->next->childend->next); ; elem = skipBreakNext(elem->next)) {
					if (elem->type == _ElemEnd) {
						local->select.to = elem;
						local->select.toid = local->select.to->id;
						local->select.n = 0;
						break;
					}
					else if (local->select.count <= elem->len(elem)) {
						local->select.to = elem;
						local->select.toid = local->select.to->id;
						local->select.n = local->select.count;
						break;
					}
				}
			}
			if (!e->shift) {
				local->select.from = local->select.to;
				local->select.fromid = local->select.from->id;
				local->select.m = local->select.n;
			}
			FindOffscreen(self)->markPaint(local);
			return 1;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (self->parent->next->type == _ElemEnd) {
					local->select.to = self->parent->childend;
					local->select.toid = local->select.to->id;
					local->select.n = 0;
				}
				else {
					if (local->select.count < 0) {
						local->select.count = local->select.n;
						for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
							local->select.count += elem->len(elem);
						}
					}
					for (NewElement* elem = skipBreakNext(self->parent->next->childend->next); ; elem = skipBreakNext(elem->next)) {
						if (elem->type == _ElemEnd) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = 0;
							break;
						}
						else if (local->select.count <= elem->len(elem)) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = local->select.count;
							break;
						}
					}
				}
				FindOffscreen(self)->markPaint(local);
				return 1;
			}
			else {
				local->select.from = local->select.to = local->select.end;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.e;
				FindOffscreen(self)->markPaint(local);
			}
			return 1;
		}
	}
	if (self->parent->parent->editable != EditType::Editable) return 0;
	local->select.count = -1;
	if (e->key == SDLK_UNKNOWN) {
		markLayoutOf(self, local);
		UniteLine(thgc, local, (NewLine*)self->parent, (NewLine*)self->parent->next);
		return -2;
	}
	if (e->key == SDLK_KP_ENTER || e->key == SDLK_RETURN) {
		NewLine* newline = (NewLine*)GC_alloc(thgc, _LineC);
		initLine(thgc, newline);
		NewBefore(thgc, local, self->parent, newline);
		// 親が NewTable なら、newline の位置に対応する Row も挿入する。
		// NewMoveElement が moved elements の parent を newline に書き換えるので、
		// 先に Table 側に newline を登録 (= cells map / TABLE_POS_SLOT を設定) しておく。
		tableInsertRowForLine(thgc, self->parent, newline);
		NewMoveElement(thgc, local, newline->childend, self->parent->childend->next, skipBreakBefore(self->before));
		e->key = SDLK_UNKNOWN;
		markLayoutOf(newline->next, local);
		return -1;
	}
	else if (e->key == SDLK_BACKSPACE) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (skipBreakBefore(self->before)->type == _ElemEnd) {
				if (self->parent->before->type == _ElemEnd) return 0;
				NewElement* el = skipBreakBefore(self->parent->before->childend->before);
				local->select.from = local->select.to = el;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.from->len(local->select.from);
				UniteLine(thgc, local, (NewLine*)self->parent->before, (NewLine*)self->parent);
				UniteText(thgc, local, el, el->next);
			}
			else {
				NewElement* prev = skipBreakBefore(self->before);
				if (prev->type == LetterType::_Letter) {
					if (self->parent->parent->editable == Editable) {
						NewLetter* let = (NewLetter*)prev;
						NewElement* before = let->before;
						if (AddText(thgc, local, let, let->text->size - 1, let->text->size, NULL) == -1) {
							local->select.from = local->select.to = before;
							local->select.fromid = local->select.toid = before->id;
							local->select.m = local->select.n = before->len(before);
						}
						else {
							local->select.from = local->select.to = let;
							local->select.fromid = local->select.toid = local->select.from->id;
							local->select.m = local->select.n = let->text->size - 1;
						}
					}
				}
			}
		}
		else {
			local->select.from = local->select.to = skipBreakBefore(self->before);
			local->select.fromid = local->select.toid = local->select.from->id;
			local->select.m = local->select.n = local->select.from->len(local->select.from);
			UniteLine(thgc, local, (NewLine*)self->parent->before, (NewLine*)self->parent);
			e->key = SDLK_UNKNOWN;
			markLayoutOf(self, local);
			return -2;
		}
		markLayoutOf(self, local);
	}
	else if (e->key == SDLK_DELETE) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			NewElement* el = skipBreakBefore(self->parent->childend->before);
			local->select.from = local->select.to = el;
			local->select.fromid = local->select.toid = local->select.from->id;
			local->select.m = local->select.n = local->select.from->len(local->select.from);
			UniteLine(thgc, local, (NewLine*)self->parent, (NewLine*)self->parent->next);
			UniteText(thgc, local, el, el->next);
		}
		else {
			local->select.from = local->select.to = skipBreakBefore(self->parent->childend->before);
			local->select.fromid = local->select.toid = local->select.from->id;
			local->select.m = local->select.n = local->select.from->len(local->select.from);
			UniteLine(thgc, local, (NewLine*)self->parent->before, (NewLine*)self->parent);
			e->key = SDLK_UNKNOWN;
			markLayoutOf(self, local);
			return -2;
		}
		markLayoutOf(self, local);
	}
	else {
		NewElement* prev = skipBreakBefore(self->before);
		if (prev->type == LetterType::_Letter) {
			NewLetter* let = (NewLetter*)prev;
			NewElement* before = let->before;
			if (AddText(thgc, local, let, let->text->size, let->text->size, e->text) == -1) {
				local->select.from = local->select.to = before;
				local->select.fromid = local->select.toid = before->id;
				local->select.m = local->select.n = before->len(before);
			}
			else {
				local->select.from = local->select.to = let;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = let->text->size;
			}
		}
		else {
			NewLetter* let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
			initLetter(thgc, let, self->parent->parent->font, LetterType::_Letter);
			let->text = e->text;
			NewBefore(thgc, local, self, let);
		}
		if (n == 1) {
			local->select.from = local->select.to = skipBreakBefore(self->parent->before->childend->before);
			local->select.fromid = local->select.toid = local->select.from->id;
			local->select.m = local->select.n = local->select.from->len(local->select.from);
			UniteLine(thgc, local, (NewLine*)self->parent->before, (NewLine*)self->parent);
			e->key = SDLK_UNKNOWN;
			markLayoutOf(self, local);
			return -2;
		}
		markLayoutOf(self, local);
	}
	return -1;
}
int LetterMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	NewLetter* letter = (NewLetter*)self;
	for (int i = 0; i < letter->renderspans->size; i++) {
		auto rs = *(RenderSpan**)get_list(letter->renderspans, i);
		if (self->pos.x + rs->x <= e->x - pos.x && e->x - pos.x < self->pos.x + rs->x + rs->width) {
			if (self->pos.y + rs->y <= e->y - pos.y && e->y - pos.y < self->pos.y + rs->y + rs->height) {
				// RenderSpanホバー登録
				if (thgc->hoveredSpanList) {
					HoveredSpan* hs = (HoveredSpan*)GC_alloc(thgc, CType::_HoveredSpan);
					hs->letter = letter;
					hs->span = rs;
					hs->atsspan = rs->atsspan;
					hs->stylespan = rs->stylespan;
					add_list(thgc, thgc->hoveredSpanList, (char*)hs);
				}
				if (self->GoMouseDown != NULL)
					FireEvent(thgc, self, self->GoMouseDown, _MouseEvent, e, sizeof(MouseEvent));
				float width;
				String* str = SubString(thgc, letter->text, rs->start, rs->end - rs->start);
				size_t n;
				float hei;
				MeasureString(*getAtlas(thgc), rs->font, str, str->size, e->x - pos.x - self->pos.x - rs->x, &width, &hei, &n, e->group);
				if (self->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = self;
						local->select.fromid = local->select.toid = self->id;
						local->select.m = local->select.n = rs->start + n;
						Offscreen* seloff = FindOffscreen(self);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(self);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = self;
							local->select.toid = self->id;
							local->select.n = rs->start + n;
						}
					}
				}
				if (self->BackMouseDown != NULL)
					FireEvent(thgc, self, self->BackMouseDown, _MouseEvent, e, sizeof(MouseEvent));
				return n;
			}
		}
	}
	// フォールバック: Letterは1視覚行のみなので最後のrenderspanの末尾を選択
	if (letter->renderspans->size > 0) {
		RenderSpan* last = *(RenderSpan**)get_list(letter->renderspans, letter->renderspans->size - 1);
		int endPos = last->end;
		if (self->parent->parent->editable != EditType::NoSelect) {
			if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				local->select.from = local->select.to = self;
				local->select.fromid = local->select.toid = self->id;
				local->select.m = local->select.n = endPos;
				Offscreen* seloff = FindOffscreen(self);
				local->select.window = seloff ? seloff->window : nullptr;
			}
			else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
				Offscreen* tooff = FindOffscreen(self);
				NativeWindow* toWin = tooff ? tooff->window : nullptr;
				if (toWin == local->select.window) {
					local->select.to = self;
					local->select.toid = self->id;
					local->select.n = endPos;
				}
			}
		}
		return endPos;
	}
	return -1;
}
int LetterKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local) {
	if (self->parent->parent->editable == EditType::NoSelect) return 0;
	NewLetter* letter = (NewLetter*)self;
	if (e->key == SDLK_LEFT) {
		local->select.count = -1;
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (m == 0) {
				NewElement* prevReal = skipBreakBefore(self->before);
				if (prevReal->type == _ElemEnd) {
					if (self->parent->before->type == _ElemEnd) return 1;
					local->select.to = self->parent->before->childend;
					local->select.toid = local->select.to->id;
					local->select.n = 0;
				}
				else {
					local->select.to = prevReal;
					local->select.toid = prevReal->id;
					local->select.n = prevReal->len(prevReal) - 1;
				}
				if (!e->shift) {
					local->select.from = local->select.to;
					local->select.fromid = local->select.to->id;
					local->select.m = local->select.n;
				}
			}
			else {
				local->select.n = m - 1;
				if (!e->shift) {
					local->select.m = local->select.n;
				}
			}
			FindOffscreen(letter)->markPaint(local);
			return 1;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (local->select.n == 0) {
					NewElement* prevReal = skipBreakBefore(self->before);
					if (prevReal->type == _ElemEnd) {
						if (self->parent->before->type == _ElemEnd) return 1;
						local->select.to = self->parent->before->childend;
						local->select.toid = local->select.to->id;
						local->select.n = 0;
					}
					else {
						local->select.to = prevReal;
						local->select.toid = prevReal->id;
						local->select.n = prevReal->len(prevReal) - 1;
					}
				}
				else {
					local->select.n--;
				}
				FindOffscreen(letter)->markPaint(local);
				return 1;
			}
			else {
				local->select.from = local->select.to = local->select.start;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.s;
			}
			FindOffscreen(letter)->markPaint(local);
			return 1;
		}
	}
	else if (e->key == SDLK_RIGHT) {
		local->select.count = -1;
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (m == letter->text->size) {
				NewElement* nextReal = skipBreakNext(self->next);
				if (nextReal->type == _ElemEnd) {
					if (self->parent->before->type == _ElemEnd) return 1;
					local->select.to = skipBreakNext(self->parent->next->childend->next);
					local->select.toid = local->select.to->id;
					local->select.n = 0;
				}
				else {
					local->select.to = nextReal;
					local->select.toid = local->select.to->id;
					local->select.n = 1;
				}
				if (!e->shift) {
					local->select.from = local->select.to;
					local->select.fromid = local->select.to->id;
					local->select.m = local->select.n;
				}
			}
			else {
				local->select.n = m + 1;
				if (!e->shift) {
					local->select.m = local->select.n;
				}
			}
			FindOffscreen(letter)->markPaint(local);
			return 1;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (local->select.n == self->len(self)) {
					NewElement* nextReal = skipBreakNext(self->next);
					if (nextReal->type == _ElemEnd) {
						if (self->parent->next->type == _ElemEnd) return 0;
						local->select.to = skipBreakNext(self->parent->next->childend->next);
						local->select.toid = local->select.to->id;
						local->select.n = 0;
					}
					else {
						local->select.to = nextReal;
						local->select.toid = local->select.to->id;
						local->select.n = 1;
					}
				}
				else {
					if (self->type == _ElemEnd) {
						if (self->parent->next->type == _ElemEnd) return 0;
						local->select.to = skipBreakNext(self->parent->next->childend->next);
						local->select.toid = local->select.to->id;
						local->select.n = 0;
					}
					local->select.n++;
				}
				FindOffscreen(letter)->markPaint(local);
				return 1;
			}
			else {
				local->select.from = local->select.to = local->select.end;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.e;
			}
			FindOffscreen(letter)->markPaint(local);
			return 1;
		}
	}
	else if (e->key == SDLK_UP) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (self->parent->before->type == _ElemEnd) {
				local->select.to = skipBreakNext(self->parent->childend->next);
				local->select.toid = local->select.to->id;
				local->select.n = 0;
			}
			else {
				if (local->select.count < 0) {
					local->select.count = m;
					for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
						local->select.count += elem->len(elem);
					}
				}
				for (NewElement* elem = skipBreakNext(self->parent->before->childend->next); ; elem = skipBreakNext(elem->next)) {
					if (elem->type == _ElemEnd) {
						local->select.to = elem;
						local->select.toid = local->select.to->id;
						local->select.n = 0;
						break;
					}
					else if (local->select.count <= elem->len(elem)) {
						local->select.to = elem;
						local->select.toid = local->select.toid;;
						local->select.n = local->select.count;
						break;
					}
				}
			}
			if (!e->shift) {
				local->select.from = local->select.to;
				local->select.fromid = local->select.from->id;
				local->select.m = local->select.n;
			}
			FindOffscreen(letter)->markPaint(local);
			return 1;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (self->parent->before->type == _ElemEnd) {
					local->select.to = skipBreakNext(self->parent->childend->next);
					local->select.toid = local->select.to->id;
					local->select.n = 0;
				}
				else {
					if (local->select.count < 0) {
						local->select.count = local->select.n;
						for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
							local->select.count += elem->len(elem);
						}
					}
					for (NewElement* elem = skipBreakNext(self->parent->before->childend->next); ; elem = skipBreakNext(elem->next)) {
						if (elem->type == _ElemEnd) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = 0;
							break;
						}
						else if (local->select.count <= elem->len(elem)) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = local->select.count;
							break;
						}
					}
				}
				FindOffscreen(letter)->markPaint(local);
				return 1;
			}
			else {
				local->select.from = local->select.to = local->select.start;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.s;
			}
			FindOffscreen(letter)->markPaint(local);
			return 1;
		}
	}
	else if (e->key == SDLK_DOWN) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (self->parent->next->type == _ElemEnd) {
				local->select.to = self->parent->childend;
				local->select.toid = local->select.to->id;
				local->select.n = 0;
			}
			else {
				if (local->select.count < 0) {
					local->select.count = m;
					for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
						local->select.count += elem->len(elem);
					}
				}
				for (NewElement* elem = skipBreakNext(self->parent->next->childend->next); ; elem = skipBreakNext(elem->next)) {
					if (elem->type == _ElemEnd) {
						local->select.to = elem;
						local->select.toid = local->select.to->id;
						local->select.n = 0;
						break;
					}
					else if (local->select.count <= elem->len(elem)) {
						local->select.to = elem;
						local->select.toid = local->select.to->id;
						local->select.n = local->select.count;
						break;
					}
				}
			}
			if (!e->shift) {
				local->select.from = local->select.to;
				local->select.fromid = local->select.from->id;
				local->select.m = local->select.n;
			}
			FindOffscreen(letter)->markPaint(local);
			return 1;
		}
		else {
			if (e->shift) {
				self = local->select.to;
				if (self->parent->next->type == _ElemEnd) {
					local->select.to = self->parent->childend;
					local->select.toid = local->select.to->id;
					local->select.n = 0;
				}
				else {
					if (local->select.count < 0) {
						local->select.count = local->select.n;
						for (NewElement* elem = skipBreakBefore(self->before); elem->type != _ElemEnd; elem = skipBreakBefore(elem->before)) {
							local->select.count += elem->len(elem);
						}
					}
					for (NewElement* elem = skipBreakNext(self->parent->next->childend->next); ; elem = skipBreakNext(elem->next)) {
						if (elem->type == _ElemEnd) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = 0;
							break;
						}
						else if (local->select.count <= elem->len(elem)) {
							local->select.to = elem;
							local->select.toid = local->select.to->id;
							local->select.n = local->select.count;
							break;
						}
					}
				}
				FindOffscreen(letter)->markPaint(local);
				return 1;
			}
			else {
				local->select.from = local->select.to = local->select.end;
				local->select.fromid = local->select.toid = local->select.from->id;
				local->select.m = local->select.n = local->select.e;
				FindOffscreen(letter)->markPaint(local);
			}
			return 1;
		}
	}
	if (self->parent->parent->editable != EditType::Editable) return 0;
	local->select.count = -1;
	if (e->key == SDLK_UNKNOWN) {
		markLayoutOf(letter, local);
		int n2 = AddText(thgc, local, letter, m, n, NULL);
		if (n2 != -1) {
			UniteText(thgc, local, skipBreakBefore(letter->before), letter);
		}
		return n2;
	}
	else if (e->key == SDLK_KP_ENTER || e->key == SDLK_RETURN) {
		String* head = SubString(thgc, letter->text, 0, m);
		String* tail = SubString(thgc, letter->text, n, letter->text->size - n);
		NewElement* nowline = self->parent;
		NewLine* newline = (NewLine*)GC_alloc(thgc, _LineC);
		initLine(thgc, newline);
		NewBefore(thgc, local, self->parent, newline);
		// 親が NewTable なら、newline の位置に対応する Row を先に挿入する
		// (NewMoveElement が moved elements の parent を newline に書き換える前に登録しておく)。
		tableInsertRowForLine(thgc, self->parent, newline);
		NewElement* next = letter->next;
		NewMoveElement(thgc, local, newline->childend, self->parent->childend->next, letter);
		if (head->size == 0) {
			NewRemove(thgc, local, self);
		}
		else {
			letter->text = head;
			letter->recompile = true;
		}
		if (tail->size != 0) {
			NewLetter* let2 = (NewLetter*)GC_alloc(thgc, CType::_LetterC);	
			initLetter(thgc, let2, letter->font, LetterType::_Letter);
			NewNext(thgc, local, nowline->childend, let2);
			let2->text = tail;
			local->select.from = local->select.to = let2;
			local->select.fromid = local->select.toid = let2->id;
			local->select.m = local->select.n = 0;
		}
		else {
			local->select.from = local->select.to = nowline->childend;
			local->select.fromid = local->select.toid = nowline->id;
			local->select.m = local->select.n = 1;
		}
		e->key = SDLK_UNKNOWN;
		markLayoutOf(newline->next, local);
		return -1;
	}
	else if (e->key == SDLK_BACKSPACE) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (m == 0) {
				if (skipBreakBefore(letter->before)->type == _ElemEnd) {
					if (letter->parent->before->type == _ElemEnd) return 0;
					NewElement* el = skipBreakBefore(self->parent->before->childend->before);
					local->select.from = local->select.to = el;
					local->select.fromid = local->select.toid = local->select.from->id;
					local->select.m = local->select.n = local->select.from->len(local->select.from);
					UniteLine(thgc, local, (NewLine*)letter->parent->before, (NewLine*)letter->parent);
					UniteText(thgc, local, el, el->next);
				}
				else {
					NewElement* prevLet = skipBreakBefore(letter->before);
					if (prevLet->type == LetterType::_Letter) {
						if (letter->parent->parent->editable == Editable) {
							NewLetter* let = (NewLetter*)prevLet;
							NewElement* before = let->before;
							if (AddText(thgc, local, let, let->text->size - 1, let->text->size, NULL) == -1) {
								local->select.from = local->select.to = before;
								local->select.fromid = local->select.toid = before->id;
								local->select.m = local->select.n = before->len(before);
							}
							else {
								local->select.from = local->select.to = let;
								local->select.fromid = local->select.toid = local->select.from->id;
								local->select.m = local->select.n = let->text->size - 1;
							}
						}
					}
				}
			}
			else {
				NewElement* before = skipBreakBefore(self->before);
				if (AddText(thgc, local, letter, m - 1, m, NULL) == -1) {
					local->select.from = local->select.to = before;
					local->select.fromid = local->select.toid = before->id;
					local->select.m = local->select.n = before->len(before);
				}
				else local->select.m = local->select.n = m - 1;
			}
		}
		else {
			NewElement* before = skipBreakBefore(self->before);
			if (AddText(thgc, local, letter, m - 1, m, NULL) == -1) {
				local->select.from = local->select.to = before;
				local->select.fromid = local->select.toid = before->id;
				local->select.m = local->select.n = before->len(before);
			}
			else local->select.m = local->select.n = m;
			e->key = SDLK_UNKNOWN;
		}
		markLayoutOf(letter, local);
	}
	else if (e->key == SDLK_DELETE) {
		if (local->select.from == local->select.to && local->select.m == local->select.n) {
			if (m == letter->text->size) {
				NewElement* nextElem = skipBreakNext(letter->next);
				if (nextElem->type == _ElemEnd) {
					if (letter->parent->next->type == _ElemEnd) return 0;
					NewElement* el = skipBreakBefore(self->parent->childend->before);
					local->select.from = local->select.to = el;
					local->select.fromid = local->select.toid = local->select.from->id;
					local->select.m = local->select.n = local->select.from->len(local->select.from);
					UniteLine(thgc, local, (NewLine*)letter->parent, (NewLine*)letter->parent->next);
					UniteText(thgc, local, el, el->next);
				}
				else {
					if (nextElem->type == LetterType::_Letter) {
						if (letter->parent->parent->editable == Editable) {
							NewLetter* let = (NewLetter*)nextElem;
							NewElement* before = let->before;
							if (AddText(thgc, local, letter, m - 1, m, NULL) == -1) {
								local->select.from = local->select.to = before;
								local->select.fromid = local->select.toid = before->id;
								local->select.m = local->select.n = before->len(before);
								return -1;
							}
							else {
								local->select.from = local->select.to = let;
								local->select.fromid = local->select.toid = local->select.from->id;
								local->select.m = local->select.n = 0;
							}
						
						}
					}
				}
			}
			else {
				NewElement* before = skipBreakBefore(self->before);
				if (AddText(thgc, local, letter, m - 1, m, NULL) == -1) {
					local->select.from = local->select.to = before;
					local->select.fromid = local->select.toid = before->id;
					local->select.m = local->select.n = before->len(before);
					return -1;
				}
			}
		}
		else {
			AddText(thgc, local, letter, m, n, NULL);

			NewElement* before = skipBreakBefore(self->before);
			if (AddText(thgc, local, letter, m - 1, m, NULL) == -1) {
				local->select.from = local->select.to = before;
				local->select.fromid = local->select.toid = before->id;
				local->select.m = local->select.n = before->len(before);
				return -1;
			}
			else local->select.m = local->select.n = m;
			e->key = SDLK_UNKNOWN;
		}
		markLayoutOf(letter, local);
	}
	else {
		AddText(thgc, local, letter, m, n, e->text);
		local->select.from = local->select.to = self;
		local->select.fromid = local->select.toid = self->id;
		local->select.m = m + e->text->size;
		local->select.n = local->select.m;
		markLayoutOf(letter, local);
		e->key = SDLK_UNKNOWN;
		return 0;
	}
	return -1;
}
// othelem.h で使うポップアップ関数の前方宣言
NativeWindow* myCreatePopupWindow(ThreadGC* thgc, NativeWindowType type, PopupAnchor anchor,
                                   int x, int y, int w, int h, int cornerRound, NewElement* anchorElem,
                                   bool visible = true);
void myResizePopupWindow(ThreadGC* thgc, NativeWindow* popup, int newW, int newH);
void myDestroyPopupWindow(ThreadGC* thgc, NativeWindow* popup);
#include "othelem.h"


inline void Offscreen::markLayout(NewLocal* local, NewElement* source) {
	// 「offscreen の elem が Table」のとき、descendant から親を辿って Table 直下の cell を見つけて
	// dirtyCells に登録するヘルパ。
	auto registerCellIfTable = [&](Offscreen* off, NewElement* descendant) {
		if (off->elem->type != LetterType::_TableElem) return;
		NewTable* t = (NewTable*)off->elem;
		if (t == descendant) return;
		NewElement* cell = descendant;
		while (cell->parent != (NewElement*)t) cell = cell->parent;
		add_list(cell->gc, t->dirtyCells, (char*)cell);
	};
	// 自 offscreen の elem が Table なら、source の親を辿って cell 登録
	registerCellIfTable(this, source);
	// 親をたどり、Auto なら親もレイアウト対象。途中で Table を持つ offscreen を踏んだら
	// 「その 1 つ深い offscreen の elem」から親を辿って cell 登録 (= ネストした Table 階層に対応)。
	Offscreen* top = this;
	Offscreen* prev = top;
	while (top->parent && ((top->elem->xtype == baseType(SizeType::Auto) || top->elem->ytype == baseType(SizeType::Auto)))) {
		prev = top;
		top = top->parent;
		registerCellIfTable(top, prev->elem);
	}
	// topから下へ再帰的に伝播 (markLayoutChildren が各 Offscreen 訪問時に Table 判定する)
	local->dirty = (DirtyType)(local->dirty | DirtyType::Partial);
	markLayoutChildren(local, top);
}
inline void Offscreen::markLayoutChildren(NewLocal* local, Offscreen* off) {
	off->layout = true;
	off->paint  = PaintCommand;
	off->updateGen++;
	if (off->elem->type == LetterType::_Linked) {
		LinkedElement* linked = (LinkedElement*)off->elem;
		if (linked->page) markLayoutChildren(linked->page->gc->local, linked->page->offscreen);
	}
	else if (off->child) {
		for (Offscreen* c = off->child->next; c->type != OffscreenType::EndOff; c = c->next) {
			if (c->elem && (isPercent(c->elem->xtype) || isPercent(c->elem->ytype) || c->elem->xtype == baseType(SizeType::Auto) || c->elem->ytype == baseType(SizeType::Auto))) {
				markLayoutChildren(local, c);
			}
		}
	}
}
inline void Offscreen::markPaint(NewLocal* local) {
	if (elem->type == LetterType::_Linked) {
		LinkedElement* linked = (LinkedElement*)elem;
		markPaint(linked->page->gc->local);
	}
	else {
		paint = PaintCommand;
		updateGen++;
		local->dirty = (DirtyType)(local->dirty | DirtyType::Partial);
		markPaintChildren(this);
	}
}
inline void Offscreen::markPaintChildren(Offscreen* parent) {
	if (parent->elem->type == LetterType::_Linked) {
		LinkedElement* linked = (LinkedElement*)parent->elem;
		markPaintChildren(linked->page->offscreen);
	}
	else if (parent->child) {
		for (Offscreen* c = parent->child->next; c->type != OffscreenType::EndOff; c = c->next) {
			if (c->paint < PaintOffscreen) c->paint = PaintOffscreen;
			c->updateGen++;
			markPaintChildren(c);
		}
	}
}