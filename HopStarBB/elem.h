// GlyphAtlas_bgfx.hpp
#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <bgfx/bgfx.h>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include <atomic>
#include <cstring>
#include <cassert>
#include "mem.h"



enum Position {
	Relative, Pos_Fixed, Absolute  // Fixed renamed to Pos_Fixed to avoid conflict with MacTypes.h
};
enum Layout {
    ocupay, none, _Zero, _Over, _ZeroNone, _OverNone
};
enum MouseCall {
	MouseDown, MouseUp, DoubleClick, MouseMove
};
enum KeyCall {
	KeyDown, KeyUp, KeyPress
};
enum Align {
    Left, Center, Right, Separate
};
enum SizeType {
    Auto = 0, Break = 1, Page = 2, Range = 3, Scroll = 4,
    Percent = 8,  // ビットフラグ: Break | Percent, Scroll | Percent 等で合成可能
};
inline bool isPercent(SizeType t) { return t & Percent; }
inline SizeType baseType(SizeType t) { return (SizeType)(t & ~Percent); }
inline SizeType operator|(SizeType a, SizeType b) { return (SizeType)((int)a | (int)b); }
inline SizeType operator&(SizeType a, SizeType b) { return (SizeType)((int)a & (int)b); }
enum LetterType {
	_None, _ElemEnd, _Main, _Sidelet, _Element, _Button, _Popup, _End, _Kaigyou, _Line, _VLine, _Letter, _BreakLetter, _CloneElem, _Image, _TabControl, _TabTitle, _TabPage, _Linked, _TextBox,
	_Name, _Number, _BracketL, _BracketR, _BlockL, _BlockR, _BraceL, _BraceR, _Semicolon, _Comma, _Plus, _Minus, _Mul, _Div, _Mod,
	_Space, _Tab, _Equal,
	_Str, _Decimal, _Sharp, _Dot, _Bou, _Not, _LessThan, _MoreThan,
	_Question, _Mountain, _Nyoro, _NyoroNyoro, _NyoroNyoroNyoro,
	_Dolor, _Left, _Right, _HLetter, _StringTag, _AtLetter, _RightRight,
	_Colon, _And, _Percent, _EqualEqual, _NotEqual, _LessEqual, _MoreEqual, _CommentSingle,
	_Obj,
	OT_Call1, OT_Call2, OT_Bracket, OT_Block, OT_Comment, OT_None,
	// 旧 OT_Dot / OT_Left / OT_Right は廃止。字句型 (_Dot / _Left / _Right) を
	// objtype にそのまま流用する設計に統一 (_Plus / _Mul / _RightRight 等と同じ扱い)。
	OT_Wait, OT_Error, OT_NG, OT_Word, OT_Htm, OT_Number, OT_FloatVal, OT_BoolVal, OT_StrObj,
	OT_Print, OT_Return, OT_Goto, OT_Continue, OT_Var, OT_If, OT_Elif, OT_Else,
	OT_While, OT_For, OT_Switch, OT_GeneLabel, OT_Comment2,
	OT_Client, OT_Server, OT_Signal, OT_ServerClient, OT_Dolor, OT_Mountain,
	OT_Model, OT_Gene, OT_Class, OT_TagBlock, OT_CallBlock, OT_Array,
	OT_ClassObj, OT_Generic, OT_GenericObj,
	OT_VariClass, OT_ArrType, OT_FuncType, OT_Variable, OT_Function, OT_GenericFunction, OT_ObjBlock,
	OT_Value, OT_ModelObj, OT_GeneObj, OT_VoiVal, OT_Constructor, OT_ArrayConstructor,
	OT_AnimationFunction, OT_ArrayType, OT_Br, OT_Clones, OT_ConnectStock,
	OT_Div, OT_ElemObj, OT_IfBlock, OT_Iterator, OT_ModelVal,
	OT_ServerFunction, OT_Sheet, OT_SignalFunction, OT_SqlString, OT_Stock,
	OT_Cell, OT_Span, OT_LinearFunction,
	OT_Break, OT_GeneVal, OT_Base, OT_CommentType,
	OT_AddressType, OT_StockType,
	_Drop, _Down, _In, _Or, _TableElem
};
class FontAtlas;
#ifndef __MACTYPES__  // Avoid conflict with MacTypes.h Point on Apple platforms
typedef struct {
    int x;
    int y;
} Point;
#endif
struct CloneElement;
struct Measure;
struct Graphic;
struct Local;
struct Select;
struct EventObj {
	char* blk;
	MemTable* table;
};
struct Action;
typedef struct MemObj {
	char* blk;
	Action* action;
	MemTable* req;
	MemTable* res;
};
inline void MemObjCheck(ThreadGC* thgc, char* data) {
	MemObj* m = (MemObj*)data;
	m->req = (MemTable*)copy_object_young(thgc, (char*)m->req);
	m->res = (MemTable*)copy_object_young(thgc, (char*)m->res);
}

typedef struct MemFunc {
	char* blk;
	MemObj* obj;
	void (*func)(Frame* frame);
};
inline void MemFuncCheck(ThreadGC* thgc, char* data) {
	MemFunc* m = (MemFunc*)data;
	m->obj = (MemObj*)copy_object_young(thgc, (char*)m->obj);
}

typedef struct PointI {
	int x;
	int y;
	int& operator[](int axis)       { return axis ? y : x; }
	int  operator[](int axis) const { return axis ? y : x; }
};
enum class RenderGroupFlags : uint8_t {
	None = 0,
	Pinned = 1 << 0,
	Transient = 1 << 1,
};
struct FontId {
	uint64_t name = 0;   // �t�H���g�� or �p�X
	int size = 0;    
	FontId(const char* n, int s) : size(s) {
		size_t len = strnlen(n, 8);
		memcpy(&name, n, len);
	} 

	// �L�����`�F�b�N
	bool valid() const { return name != 0 && size > 0; }

	// ��r���Z�q�istd::map�p�j
	bool operator<(const FontId& o) const {
		if (name != o.name) return name < o.name;
		return size < o.size;
	}

	// �������Z�q�istd::unordered_map�p�j
	bool operator==(const FontId& o) const {
		return name == o.name && size == o.size;
	}

	bool operator!=(const FontId& o) const {
		return !(*this == o);
	}

	// �f�o�b�O�p������
	std::string toString() const {
		return name + "@" + std::to_string(size);
	}
	uint64_t computeHash() const {
		// FNV-1a �n�b�V��
		uint64_t h = 14695981039346656037ULL;

		h ^= name;
		h *= 1099511628211ULL;

		// size��������
		h ^= static_cast<uint64_t>(size);
		h *= 1099511628211ULL;
		return h;
	}
};

// 描画 cmd が握る「テクスチャ参照の最小単位」。
// Standalone / Tile 等の placement 固有の info 構造体に **先頭メンバとして** 継承される。
// cmd は TextureSlot* を捕まえれば、placement 種別に関係なく
// handle / fbo / size を deref できる。アドレスは map slot ごとに stable
// (= producer がエントリを作った瞬間から render thread が値を埋めるまでの間、
//   ポインタ位置は不変。値だけが INVALID → valid に変化する)。
struct TextureSlot {
	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
	bgfx::FrameBufferHandle fbo = BGFX_INVALID_HANDLE;
	PointI size = {0, 0};
};

struct ResolvedTexture {
	// TextureSlot* に統一 (= cmd は &slot->handle を捕まえる)。
	// slot のアドレスは imageLocations_[id].slot / atlas page / tile 内
	// に保持されているもので、entry erase されない限り stable。
	TextureSlot* slot = nullptr;
	float u0 = 0, v0 = 0, u1 = 1, v1 = 1;
	uint16_t width = 0;
	uint16_t height = 0;

	// 旧 API 互換: texture() で &slot->handle を返す。
	// 旧 r.texture (field access) を呼んでた箇所は r.texture() (関数呼び出し) に
	// 置き換える必要あり。新コードは r.slot->handle を直接見るのが推奨。
	bgfx::TextureHandle* texture() const { return slot ? &slot->handle : nullptr; }

	// isValid は「**slot がある = 描画 cmd に渡せる (= ポインタとして掴める)**」を意味する。
	// slot->handle が現時点で INVALID でも true を返す: render thread が後で値を埋める
	// 設計 (placeholder + lazy upload) のため。cmd は &slot->handle を保持して、
	// 実描画時には valid handle になっている前提。
	// 「今この瞬間に valid な bgfx texture か」を知りたい場合は
	// `bgfx::isValid(slot->handle)` を直に見ること。
	bool isValid() const { return slot != nullptr; }
};
// グローバル UI スケール (= SDL_GetWindowDisplayScale で取得した端末の論理→物理
// 倍率)。Android 420dpi で 2.625 とか、Windows で 1.0、Win/Mac の Retina で 2.0
// 等になる。main 起動直後にウィンドウ作成後に SetUiScale() で設定。
// 0 以下は無効値扱いで 1.0 として扱う。
inline float g_uiScale = 1.0f;
inline void SetUiScale(float s) {
	if (s > 0.0f) g_uiScale = s;
}
inline float GetUiScale() {
#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR)
	// タッチ機器 (Android / iOS) は元 UI が 1200x800 デスクトップ前提で
	// 作られていて DPI 拡大しないと指で押せない → 自動スケール。
	return g_uiScale > 0.0f ? g_uiScale : 1.0f;
#else
	// Windows / Mac: 元 UI のピクセルサイズ感を維持したいので、
	// OS 側の DPI scaling (125% / 150% / Retina) は UI 要素に
	// 反映しない (= getFont の論理サイズもそのまま物理サイズ)。
	// ユーザが「Windows でも自動スケール欲しい」と言ったら別 API で
	// 強制有効化する。
	return 1.0f;
#endif
}

// 整数 / float の論理 dp → 物理 px 変換ヘルパ。
// UI 要素のマージン/ボーダー/シャドウ/アイコン辺長などのリテラル値を
//   margins = Sc(5);  /  radius = Sc(12);  /  iconSize = Sc(24);
// と書けば、Android 高 DPI 端末や HiDPI Mac で自動的に物理ピクセルへ拡大される。
// 切り捨てでなく四捨五入。最小 1px は保証。
inline int   Sc (int v)   { int r = (int)((float)v * GetUiScale() + 0.5f); return r < 1 ? (v == 0 ? 0 : 1) : r; }
inline float Scf(float v) { return v * GetUiScale(); }

// タッチ操作向けの倍率付きヘルパ。マウスポインタは小さくても問題ないが、
// 指タッチは Material design 推奨で 48dp 目安。
//   Sctf(12)        : デフォルト 1.5x  → 描画用 (knob, border 等)
//   Sctf(18, 2.0f)  : 明示 2.0x        → ヒット領域用 (見えてる範囲より少し外
//                                        も拾わせて「指がずれてもタップ成立」)
// デスクトップ (マウス) では mul 引数に関わらず 1.0x (touchScale なし)。
inline bool IsTouchPlatform() {
#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR)
	return true;
#else
	return false;
#endif
}
inline float Sctf(float v, float touchMul = 1.5f) {
	return v * GetUiScale() * (IsTouchPlatform() ? touchMul : 1.0f);
}
inline int   Sct (int v,   float touchMul = 1.5f) {
	int r = (int)(Sctf((float)v, touchMul) + 0.5f);
	return r < 1 ? (v == 0 ? 0 : 1) : r;
}

// フォントサイズは getFont 呼び出し時点でスケール適用。
// 呼び出し側がリテラル (e.g., getFont("sans", 16)) を渡しても、
// 高 DPI 端末では実物理サイズが拡大される。
FontId getFont(const char* name, int size) {
	int scaled = (int)((float)size * GetUiScale() + 0.5f);
	if (scaled < 1) scaled = 1;
	FontId id(name, scaled);
	return id;
}
enum class ImageIdDomain : uint8_t {
	Invalid = 0x00,
	File = 0x01,  // ファイルパスから生成
	Memory = 0x02,  // メモリデータから生成
	Offscreen = 0x03,  // オフスクリーンテクスチャ
	Generated = 0x04,  // 動的生成(procedural)
	FontGlyph = 0x05,  // FontAtlas のグリフ/画像
	Thumbnail = 0x06,  // サムネイル (Grid/Shelf)
};

using ImageId = uint64_t;

inline constexpr ImageId makeImageId(ImageIdDomain domain, uint64_t localId) {
	return (uint64_t(domain) << 56) | (localId & 0x00FFFFFFFFFFFFFFULL);
}

inline constexpr ImageIdDomain getImageIdDomain(ImageId id) {
	return static_cast<ImageIdDomain>((id >> 56) & 0xFF);
}

inline constexpr uint64_t getImageIdLocal(ImageId id) {
	return id & 0x00FFFFFFFFFFFFFFULL;
}

inline constexpr bool isValidImageId(ImageId id) {
	return id != 0 && getImageIdDomain(id) != ImageIdDomain::Invalid;
}

//=============================================================================
// ImageId ����
//=============================================================================

class ImageIdGenerator {
public:
	static ImageId fromFile(const std::string& path) {
		return makeImageId(ImageIdDomain::File, std::hash<std::string>{}(path));
	}

	static ImageId fromMemory() {
		static std::atomic<uint64_t> counter{ 0 };
		return makeImageId(ImageIdDomain::Memory, ++counter);
	}

	static ImageId forOffscreen() {
		static std::atomic<uint64_t> counter{ 0 };
		return makeImageId(ImageIdDomain::Offscreen, ++counter);
	}

	static ImageId forGenerated() {
		static std::atomic<uint64_t> counter{ 0 };
		return makeImageId(ImageIdDomain::Generated, ++counter);
	}

	static ImageId forFontGlyph(uint64_t localId) {
		return makeImageId(ImageIdDomain::FontGlyph, localId);
	}

	static ImageId forThumbnail(uint64_t contentId) {
		return makeImageId(ImageIdDomain::Thumbnail, contentId);
	}
};
//=============================================================================
// StandaloneTextureInfo - 1���G�e�N�X�`�����
//=============================================================================

enum class ImageOrigin : uint8_t {
	File,
	Memory,
	Offscreen,
	Generated,
};
enum class ImageUsage : uint8_t {
	Auto,           // サイズから自動判断
	Icon,           // 小さな正方形 → Grid
	Thumbnail,      // 通常サムネ → サイズ別 Grid or Shelf
	VideoThumb,     // 動画サムネ → Shelf
	Background,     // 背景画像 → Standalone (persistent)
	UIImage,        // UI画像 → Standalone or FontAtlas
	Dynamic,        // 動的更新 → Standalone
};
struct StandaloneTextureInfo : TextureSlot {
	// handle / fbo / size は TextureSlot から inherited
	uint32_t refCount = 0;
	uint64_t lastUsedFrame = 0;
	bool persistent = false;
	bool isRenderTarget = false;
	ImageOrigin origin = ImageOrigin::File;
	// fbo を持つ単位は heldGen を持つ (= Offscreen::updateGen と比較して再描画要否を判定)。
	// producer (UI thread) と render thread で読み書きするため atomic。x86/x64 では
	// aligned 32bit relaxed load/store は LOCK 命令不要の普通の MOV と同等コスト。
	std::atomic<uint32_t> heldGen{ 0 };

	// std::atomic は copy 不可なので explicit copy/move を定義 (=
	// std::unordered_map<ImageId, StandaloneTextureInfo> の operator[]= 代入が必要)
	StandaloneTextureInfo() = default;
	StandaloneTextureInfo(const StandaloneTextureInfo& o)
		: TextureSlot(o),  // handle / fbo / size を base class copy
		  refCount(o.refCount),
		  lastUsedFrame(o.lastUsedFrame), persistent(o.persistent),
		  isRenderTarget(o.isRenderTarget), origin(o.origin),
		  heldGen(o.heldGen.load(std::memory_order_relaxed)) {}
	StandaloneTextureInfo& operator=(const StandaloneTextureInfo& o) {
		if (this != &o) {
			TextureSlot::operator=(o);  // handle / fbo / size を base class assign
			refCount = o.refCount;
			lastUsedFrame = o.lastUsedFrame; persistent = o.persistent;
			isRenderTarget = o.isRenderTarget; origin = o.origin;
			heldGen.store(o.heldGen.load(std::memory_order_relaxed), std::memory_order_relaxed);
		}
		return *this;
	}
};

//=============================================================================
// TiledTextureInfo - 巨大テクスチャをタイル分割して管理
//=============================================================================
struct TiledTextureInfo {
	struct Tile : TextureSlot {
		// handle / fbo / size は TextureSlot から inherited
		// タイルの開始位置（元画像座標系）。総寸法 > 65535 なら必ず uint16_t を超えるので int。
		// 例: ugui.h で総高 67600 + maxTileSize 8192 → tilesY=9、最後のタイル y=65536。
		int x = 0, y = 0;
		uint16_t w = 0, h = 0;   // このタイルの実サイズ（端は小さくなる、maxTileSize_ ≤ 8192 で uint16 OK）
		// 世代カウンタ: tile ごとに「最新の中身が入っているか」を判定。
		//   描画側 (Table 等) が visible tile だけ heldGen を更新する想定。
		//   producer / render 両スレッドからアクセスするため atomic (x86 では追加コスト無し)。
		std::atomic<uint32_t> heldGen{ 0 };

		// std::atomic は copy 不可なので、Tile を std::vector に格納するための
		// explicit copy/move + 既存 brace-init 互換のフィールド ctor を定義する。
		Tile() = default;
		Tile(bgfx::TextureHandle h, bgfx::FrameBufferHandle f,
		     int x_, int y_, uint16_t w_, uint16_t h_, PointI s)
			: x(x_), y(y_), w(w_), h(h_) {
			handle = h; fbo = f; size = s;  // base class fields
		}
		Tile(const Tile& o)
			: TextureSlot(o),  // handle / fbo / size を base class copy
			  x(o.x), y(o.y), w(o.w), h(o.h),
			  heldGen(o.heldGen.load(std::memory_order_relaxed)) {}
		Tile& operator=(const Tile& o) {
			if (this != &o) {
				TextureSlot::operator=(o);  // handle / fbo / size を base class assign
				x = o.x; y = o.y; w = o.w; h = o.h;
				heldGen.store(o.heldGen.load(std::memory_order_relaxed), std::memory_order_relaxed);
			}
			return *this;
		}
	};
	std::vector<Tile> tiles;     // row-major 順
	PointI totalSize;            // 元画像の全体サイズ
	// タイルは「短辺をテクスチャに合わせ、長辺方向だけ分割」させるため、
	// X/Y 別々のタイル辺長を保持する (旧 tileSize を tileSize=X 軸として残し、Y 軸を追加)。
	uint16_t tileSize  = 0;     // X 方向のタイル辺長 (= tileSizeX)
	uint16_t tileSizeY = 0;     // Y 方向のタイル辺長
	int tilesX = 0, tilesY = 0; // グリッド次元
	uint32_t refCount = 0;
	uint64_t lastUsedFrame = 0;
	bool persistent = false;
	bool isRenderTarget = false;
	ImageOrigin origin = ImageOrigin::File;
	// updateGen は持たない (= Offscreen 側に持つ)。
	// 各 Tile::heldGen が Offscreen::updateGen と比較される。

	// 座標 (px, py) を含むタイルのインデックスを O(1) で取得
	int tileIndexAt(int px, int py) const {
		if (tileSize == 0 || tileSizeY == 0) return -1;
		int col = px / tileSize;
		int row = py / tileSizeY;
		if (col < 0 || col >= tilesX || row < 0 || row >= tilesY) return -1;
		return row * tilesX + col;
	}

	// 座標 (px, py) を含むタイルへのポインタを O(1) で取得
	Tile* tileAt(int px, int py) {
		int idx = tileIndexAt(px, py);
		return (idx >= 0) ? &tiles[idx] : nullptr;
	}
	const Tile* tileAt(int px, int py) const {
		int idx = tileIndexAt(px, py);
		return (idx >= 0) ? &tiles[idx] : nullptr;
	}

	// 矩形領域 (rx, ry, rw, rh) と重なるタイルを列挙
	// callback: void(Tile& tile, int intersectX, int intersectY, int intersectW, int intersectH)
	template<typename Func>
	void forEachTileInRect(int rx, int ry, int rw, int rh, Func&& callback) {
		if (tileSize == 0 || tileSizeY == 0) return;
		int col0 = std::clamp(rx / (int)tileSize,                0, tilesX - 1);
		int row0 = std::clamp(ry / (int)tileSizeY,               0, tilesY - 1);
		int col1 = std::clamp((rx + rw - 1) / (int)tileSize,     0, tilesX - 1);
		int row1 = std::clamp((ry + rh - 1) / (int)tileSizeY,    0, tilesY - 1);
		for (int row = row0; row <= row1; ++row) {
			for (int col = col0; col <= col1; ++col) {
				auto& tile = tiles[row * tilesX + col];
				int ix0 = std::max(rx, (int)tile.x);
				int iy0 = std::max(ry, (int)tile.y);
				int ix1 = std::min(rx + rw, (int)(tile.x + tile.w));
				int iy1 = std::min(ry + rh, (int)(tile.y + tile.h));
				if (ix0 < ix1 && iy0 < iy1) {
					callback(tile, ix0, iy0, ix1 - ix0, iy1 - iy0);
				}
			}
		}
	}
};

// タイル画像の描画用解決結果
struct TiledResolveResult {
	struct TileRegion {
		bgfx::TextureHandle* texture;
		float offsetX, offsetY;  // 元画像座標系でのオフセット
		float width, height;     // タイルの実サイズ
	};
	std::vector<TileRegion> tiles;
	int totalWidth = 0, totalHeight = 0;   // 合計コンテンツ寸法は uint16_t を超え得る
	bool isValid() const { return !tiles.empty(); }
};

//=============================================================================
// TiledRenderPass - タイルFBOへの描画ヘルパー
// 境界をまたぐ描画を各タイルに射影をずらして再発行する
//=============================================================================
//
// 使い方:
//   TiledTextureInfo* tiled = mygetTiledTextureInfo(thgc, offscreenId);
//   if (tiled && tiled->isRenderTarget) {
//       // ビューポート矩形（スクロール考慮）と重なるタイルだけ処理
//       tiled->forEachTileInRect(scrollX, scrollY, viewW, viewH,
//           [&](auto& tile, int ix, int iy, int iw, int ih) {
//               // 1) このタイルのFBOをレンダーターゲットに設定
//               bgfx::setViewFrameBuffer(viewId, tile.fbo);
//               bgfx::setViewRect(viewId, 0, 0, tile.w, tile.h);
//
//               // 2) 射影行列: タイルの原点 (tile.x, tile.y) → 画面 (0,0)
//               float proj[16];
//               bx::mtxOrtho(proj,
//                   (float)tile.x, (float)(tile.x + tile.w),   // left, right
//                   (float)(tile.y + tile.h), (float)tile.y,   // bottom, top
//                   0.0f, 10000.0f, 0.0f, caps->homogeneousDepth);
//               bgfx::setViewTransform(viewId, NULL, proj);
//
//               // 3) 同じ描画コマンドを発行（座標は元画像座標系のまま）
//               //    bgfxが射影で自動クリッピングするのでタイル外は描画されない
//               submitDrawCommands(layer, viewId);
//
//               viewId++;  // 次のタイルは別のviewIdを使用
//           });
//   }
//
// 原理:
//   通常: 射影 = (0, totalW) × (0, totalH) → 1つのFBOに全描画
//   タイル: 射影 = (tile.x, tile.x+tile.w) × (tile.y, tile.y+tile.h)
//           → 各タイルのFBOにはタイル領域内の描画だけが入る
//           → 同じ描画コマンド・同じ座標系で、射影だけ変える

bgfx::TextureHandle nulltex = BGFX_INVALID_HANDLE;
struct LayerInfo;
struct ExtendedRenderGroup;
struct ImageMaster;
struct FontAtlas;
StandaloneTextureInfo* mygetStandaloneTextureInfo(ThreadGC* thgc, ImageId imageId);
TiledTextureInfo* mygetTiledTextureInfo(ThreadGC* thgc, ImageId imageId);
// lazy 確保: tile.handle / tile.fbo が未生成なら bgfx リソースを今ここで作る (= 必要時のみ)
bool mEnsureTile(ThreadGC* thgc, ImageId imageId, int tileIdx);
TiledResolveResult myResolveTiledForDraw(ThreadGC* thgc, ImageId imageId);
bool myIsTiledTexture(ThreadGC* thgc, ImageId imageId);
ImageId queueOffscreenNew(ThreadGC* thgc, int width, int height);
// 戻り値: 以後使うべき ImageId (Standalone↔Tiled 遷移時に新規発行される)
// pageW/pageH > 0 でページめくり用 (タイル辺長をその倍数に丸める)
ImageId queueOffscreenResize(ThreadGC* thgc, ImageId offscreenid, int width, int height,
                             int pageW = 0, int pageH = 0);
ImageId myloadTexture2D(ThreadGC* thgc, const char* path, ImageUsage usage);
bgfx::TextureHandle* myResolveTexturePtr(ThreadGC* thgc, ImageId imageId);
ResolvedTexture myResolveForDraw(ThreadGC* thgc, ImageId imageId);
TextureSlot* myResolveSlot(ThreadGC* thgc, ImageId imageId);
ExtendedRenderGroup& createGroup(ThreadGC* thgc);
void drawTextUTF8(LayerInfo* layer, FontAtlas& atlas, FontId font,
	const char* text, int length, float x, float y,
	float zIndex, ExtendedRenderGroup* group, uint32_t color);
void drawString(LayerInfo* layer, FontAtlas& atlas, FontId font,
	String* text, float x, float y,
	float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI *fbsize, uint64_t viewId);
void drawUnderPagingBar(LayerInfo* layer, FontAtlas& atlas, FontId font, float x, float y, float width, float height, float currentPage, float totalPages, float zIndex,
	ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
void drawRightPagingBar(LayerInfo* layer, FontAtlas& atlas, FontId font,
	float x, float y, float width, float height,
	float currentPage, float totalPage,
	float zIndex, ExtendedRenderGroup* group,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
FontAtlas* getAtlas(ThreadGC* thgc);
TTF_Font* getFont(FontAtlas* atlas);
void drawUnderScrollBar(LayerInfo* layer,
	float x, float y, float width, float height, float thumbHeight,
	float currentX, float pageWidth, float totalWidth,
	float zIndex,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
void drawRightScrollBar(LayerInfo* layer,
	float x, float y, float width, float height, float thumbWidth,
	float currentY, float pageHeight, float totalHeight,
	float zIndex,
	bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId);
#include "newelem.h"


