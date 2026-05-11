#include "shader.h"
#include <fstream>
#include <chrono>
#include <future>

// getBundlePath is now provided by platform_io.h (included via shader.h)
// For backwards compatibility, define it if not already defined
#ifndef BUNDLE_PATH_DEFINED
#define BUNDLE_PATH_DEFINED
inline std::string getBundlePath(const char* filename) {
    return PlatformIO::resolvePath(filename, HopStarIO::Location::Resource);
}
#endif

// 描画コマンド構造体

#pragma once
#include <portaudio.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <bgfx/bgfx.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "thumnailAtlas.h"
#include "imas.h"

class RenderCommandQueue {
public:
    // === Producer（タブ側スレッド）API ===
    // 1フレーム生成の開始（必要なら予約数を渡して再割当回数を減らす）
    void begin(uint64_t frameId, size_t reserveHint = 0) {
        auto& s = slots_[write_];
        s.frameId = frameId;
        s.layers.clear();
        if (reserveHint > 0) {
            s.layers.reserve(reserveHint);  // ★ 追加
        }
    }
    // フレーム公開 (= 最新スロットを front に切替)。データ更新だけ、wait しない。
    //   frame skip 防止 (= 必ず consumer が acquire する) は caller 側で waitAcquired() を呼ぶ。
    //   waitAcquired() を呼ぶ前に必ず consumer に通知 (= cv_.notify_one) しておくこと。
    //   そうしないと consumer が wait のままで spin が解けず deadlock する。
    void publish() {
        // 最新フレームの公開
        front_.store(write_, std::memory_order_release);
        publishCount_.fetch_add(1, std::memory_order_release);

        // 次に Producer が書くスロットを選ぶ (front と reading を避ける)。
        const int front = front_.load(std::memory_order_relaxed);
        const int reading = reading_.load(std::memory_order_acquire);
        for (int i = 0; i < 3; ++i) {
            if (i != front && i != reading) { write_ = i; break; }
        }
    }

    // 直前 publish() を consumer が acquire するまで待つ。
    //   caller は publish() → render++/notify_one → waitAcquired() の順で呼ぶ。
    //   notify_one 済み + consumer alive なら必ず acquireCount が追いついて exit する。
    //   consumer 死亡時の保険として max yield 数 + 強制整合 fallback を入れる。
    void waitAcquired() {
        const int maxSpins = 1000000;   // 適当な大きさで永久 spin 防止
        int spins = 0;
        while (publishCount_.load(std::memory_order_acquire)
               > acquireCount_.load(std::memory_order_acquire)) {
            std::this_thread::yield();
            if (++spins > maxSpins) {
                // consumer 復旧不能 → 諦めて整合だけ取る (= 古い slot は失われる)
                acquireCount_.store(publishCount_.load(std::memory_order_acquire),
                                    std::memory_order_release);
                break;
            }
        }
    }

    // Producer が現在の write スロットにレイヤー設定を書くためのユーティリティ
    LayerInfo& setCurrentSlotLayer(
        const std::vector<std::string>& uniformNames,
        float opacity = 1.0f,
        bool visible = true,
        bool needsUpdate = true,
        int w = 1200,
        int h = 800)
    {
        Slot& s = slots_[write_];
        s.layers.emplace_back();
        LayerInfo& layer = s.layers.back();

        layer.opacity = opacity;
        layer.visible = visible;
        layer.needsUpdate = needsUpdate;
        layer.width = w;
        layer.height = h;

        return layer;
    }

    // === Consumer（レンダースレッド）API ===
    // 最新フレームを取得（新規が無ければ false）: out にムーブし、token を返す
    bool acquire(std::vector<LayerInfo>** outLayers, uint64_t& outFrameId, int& token) {
        const int idx = front_.load(std::memory_order_acquire);
        if (idx == -1) return false;                              // まだ公開無し
        if (idx == reading_.load(std::memory_order_acquire)) return false; // 既に取得済み

        reading_.store(idx, std::memory_order_release);           // このスロットを“使用中”に
        // 「acquire したよ」を producer に通知 (publish() の block 解除条件)。
        acquireCount_.fetch_add(1, std::memory_order_release);
        auto& s = slots_[idx];
        *outLayers = &s.layers;                                    // ムーブしてコピー回避
        outFrameId = s.frameId;
        token = idx;                                              // release 用
        return true;
    }

    // レンダがスロットの使用を終えたら呼ぶ（書き込みに開放）
    void release(int token) {
        // token が現在の reading なら -1 に戻す
        int expected = token;
        reading_.compare_exchange_strong(expected, -1, std::memory_order_acq_rel);
    }

private:
    struct Slot {
        uint64_t frameId = 0;
        std::vector<LayerInfo> layers; // ← スロットにレイヤーを持たせる（トリプルバッファの各スロットがレイヤー）
    };
    Slot slots_[3];

    // Producer が書き込むスロット（Producer専有なので非atomicでOK）
    int write_ = 0;

    // 公開済みの最新スロット（-1 = なし）
    std::atomic<int> front_{ -1 };
    // Consumer が現在描画中のスロット（-1 = なし）
    std::atomic<int> reading_{ -1 };

    // publish/acquire の同期カウンタ。publishCount_ > acquireCount_ なら publish は待つ。
    std::atomic<uint64_t> publishCount_{ 0 };
    std::atomic<uint64_t> acquireCount_{ 0 };
};
struct FontKey {
    std::string family;  // "NotoSans", "Roboto", "Consolas" など
    int size;            // 24, 32, 48 など

    bool operator<(const FontKey& other) const {
        if (family != other.family) return family < other.family;
        return size < other.size;
    }
};
// === ヘルパー関数：テキスト描画 ===
void drawText(LayerInfo* layer, FontAtlas& atlas, FontId font,
    const char* text, float x, float y,
    float zIndex, uint32_t color, bgfx::FrameBufferHandle* targetFBO, RenderGroup* group, PointI* fbsize, uint64_t viewId) {
    float cursorX = x;
    float cursorY = y;

    const char* p = text;
    while (*p) {
        uint32_t codepoint = (uint32_t)(unsigned char)*p; // 簡易版（UTF-8対応は別途必要）
        p++;

        auto gi = atlas.getOrAddGlyph(font, codepoint, group);

        if (gi.width > 0 && gi.height > 0) {
            layer->pushText(cursorX + gi.bearingX, cursorY - gi.bearingY, gi.width, gi.height, gi.u0, gi.v0, gi.u1 - gi.u0, gi.v1 - gi.v0, color, zIndex, &atlas.getPageTexture(gi.pageIndex), targetFBO, fbsize, viewId);
        }

        cursorX += gi.advance;
    }
}
// 指定範囲 [offset, offset+count) を描画。Letter / BreakLetter で SubString を作らずに
// 元 String + RenderSpan の (start, end) をそのまま渡せるようにするため。
void drawStringRange(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, int offset, int count, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle * targetFBO, PointI* fbsize, uint64_t viewId) {
    float cursorX = x;
    float cursorY = y;
    for (int i = 0; i < count; i++) {
        uint32_t codepoint = (uint32_t)GetChar(text, offset + i);
        // CR (= U+000D, CRLF の \r) はフォントにグリフが無いので何もしない。
        if (codepoint == 0x0D) continue;
        if (codepoint == 0x09) {
            const auto& sp = atlas.getOrAddGlyph(font, ' ', group);
            cursorX += sp.advance * 4.0f;
            continue;
        }
        auto gi = atlas.getOrAddGlyph(font, codepoint, group);
        if (gi.width > 0 && gi.height > 0) {
            layer->pushText(cursorX + gi.bearingX, cursorY - gi.bearingY, gi.width, gi.height, gi.u0, gi.v0, gi.u1 - gi.u0, gi.v1 - gi.v0, color, zIndex, &atlas.getPageTexture(gi.pageIndex), targetFBO, fbsize, viewId);
        }
        cursorX += gi.advance;
    }
}
void drawString(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle * targetFBO, PointI* fbsize, uint64_t viewId) {
    drawStringRange(layer, atlas, font, text, 0, text->size, x, y, zIndex, color, group, targetFBO, fbsize, viewId);
}
// 横方向 scale 付きで描画 (= 列幅に収めるための圧縮表示)。scaleX < 1 なら glyph 幅 / advance を
// 縮めて描画する。glyph の高さ / Y 位置は変えない。タブは scaleX 適用後の幅でスペース 4 個分。
void drawStringScaledRange(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, int offset, int count, float x, float y, float scaleX,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId) {
    float cursorX = x;
    float cursorY = y;
    for (int i = 0; i < count; i++) {
        uint32_t codepoint = (uint32_t)GetChar(text, offset + i);
        if (codepoint == 0x0D) continue;
        if (codepoint == 0x09) {
            const auto& sp = atlas.getOrAddGlyph(font, ' ', group);
            cursorX += sp.advance * 4.0f * scaleX;
            continue;
        }
        auto gi = atlas.getOrAddGlyph(font, codepoint, group);
        if (gi.width > 0 && gi.height > 0) {
            layer->pushText(
                cursorX + gi.bearingX * scaleX, cursorY - gi.bearingY,
                gi.width * scaleX, gi.height,
                gi.u0, gi.v0, gi.u1 - gi.u0, gi.v1 - gi.v0,
                color, zIndex, &atlas.getPageTexture(gi.pageIndex),
                targetFBO, fbsize, viewId);
        }
        cursorX += gi.advance * scaleX;
    }
}
void drawStringVerticalRange(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, int offset, int count, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle * targetFBO, PointI* fbsize, uint64_t viewId) {
    float cursorX = x;
    float cursorY = y;
    for (int i = 0; i < count; i++) {
        uint32_t codepoint = (uint32_t)GetChar(text, offset + i);
        auto gi = atlas.getOrAddGlyph(font, codepoint, group);
        if (gi.width > 0 && gi.height > 0) {
            layer->pushText(cursorX + gi.bearingX, cursorY - gi.bearingY, gi.width, gi.height, gi.u0, gi.v0, gi.u1 - gi.u0, gi.v1 - gi.v0, color, zIndex, &atlas.getPageTexture(gi.pageIndex), targetFBO, fbsize, viewId);
        }
        cursorY += gi.height;
    }
}
void drawStringVertical(LayerInfo* layer, FontAtlas& atlas, FontId font,
    String* text, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle * targetFBO, PointI* fbsize, uint64_t viewId) {
    drawStringVerticalRange(layer, atlas, font, text, 0, text->size, x, y, zIndex, color, group, targetFBO, fbsize, viewId);
}
void drawChar16(LayerInfo* layer, FontAtlas& atlas, FontId font,
    uint32_t codepoint, float x, float y,
    float zIndex, uint32_t color, ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId) {
    float cursorX = x;
    float cursorY = y;


    auto gi = atlas.getOrAddGlyph(font, codepoint, group);

    if (gi.width > 0 && gi.height > 0) {
        layer->pushText(cursorX + gi.bearingX, cursorY - gi.bearingY, gi.width, gi.height, gi.u0, gi.v0, gi.u1 - gi.u0, gi.v1 - gi.v0, color, zIndex, &atlas.getPageTexture(gi.pageIndex), targetFBO, fbsize, viewId);
    }
}
void drawUnderPagingBar(LayerInfo* layer, FontAtlas& atlas, FontId font, float x, float y, float width, float height, float currentPage, float totalPages, float zIndex,
    ExtendedRenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId) {


    const uint32_t arrowColor = 0x00D0FFFF;       // 水色
    const uint32_t arrowDisabledColor = 0x808080FF; // 灰色
    const uint32_t trackColor = 0x40FFFFFF;
    const uint32_t thumbColor = 0xFFFFFFFF;

    // グリフサイズ取得
    const auto& leftGlyph = atlas.getOrAddGlyph(font, 0x25C0, group);   // ◀
    const auto& rightGlyph = atlas.getOrAddGlyph(font, 0x25B6, group); // ▶

    float arrowWidth = leftGlyph.width;
    float padding = 6.f;

    // レイアウト計算
    float leftArrowX = x;
    float rightArrowX = x + width - arrowWidth;
    float trackX = leftArrowX + arrowWidth + padding;
    float trackWidth = rightArrowX - trackX - padding;
    float trackHeight = 4.f;
    float thumbRadius = 6.f;

    float centerY = y + height * 0.5f;

    // 左矢印 ◀
    uint32_t leftColor = (currentPage > 0) ? arrowColor : arrowDisabledColor;
    drawChar16(layer, atlas,font,  0x25C0,
        leftArrowX,
        centerY - leftGlyph.height * 0.5f,
        zIndex, leftColor, group, targetFBO, fbsize, viewId);

    // 右矢印 ▶
    uint32_t rightColor = (currentPage < totalPages - 1) ? arrowColor : arrowDisabledColor;
    drawChar16(layer, atlas, font, 0x25B6,
        rightArrowX,
        centerY - rightGlyph.height * 0.5f,
        zIndex, rightColor, group, targetFBO, fbsize, viewId);

    // トラック（角丸矩形）
    // ※ layer->pushRectがあると仮定
    {float r = trackHeight * 0.5f;
    layer->pushFill(
        trackX, centerY - trackHeight * 0.5f,
        trackWidth,
        trackHeight,
        r, r, r, r, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        trackColor, 0x00000000,
        0.5f, 1.0f, 0.0f,
        0x000000AA,
        zIndex,
        targetFBO, fbsize, viewId);}

    // つまみ位置
    float progress = (totalPages > 1)
        ? (float)currentPage / (totalPages - 1)
        : 0.5f;
    float thumbX = trackX + trackWidth * progress;

    // つまみ（円）
    layer->pushFill(
        thumbX - thumbRadius / 2,
        centerY - thumbRadius,
        thumbRadius, thumbRadius * 2,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        thumbColor, 0x00000000,
        0.0f, 0.0f, 0.0f,
        0x000000000,
        zIndex + 0.1f,  // トラックより前
        targetFBO, fbsize, viewId);
}
void drawRightPagingBar(LayerInfo* layer, FontAtlas& atlas, FontId font,
    float x, float y, float width, float height,
	float currentPage, float totalPages, float zIndex, ExtendedRenderGroup* group,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId) {

    const uint32_t arrowColor = 0x00D0FFFF;       // 水色
    const uint32_t arrowDisabledColor = 0x808080FF; // 灰色
    const uint32_t trackColor = 0x40FFFFFF;
    const uint32_t thumbColor = 0xFFFFFFFF;

    // グリフサイズ取得
    const auto& upGlyph = atlas.getOrAddGlyph(font, 0x25B2, group);    // ▲
    const auto& downGlyph = atlas.getOrAddGlyph(font, 0x25BC, group);  // ▼

    float arrowHeight = upGlyph.height;
    float padding = 1.5f;

    // レイアウト計算
    float topArrowY = y;
    float bottomArrowY = y + height - arrowHeight;
    float trackY = topArrowY + arrowHeight + padding;
    float trackHeight = bottomArrowY - trackY - padding;
    float trackWidth = 4.f;
    float thumbRadius = 6.f;
    float centerX = x + width * 0.5f;

    // 上矢印 ▲
    uint32_t upColor = (currentPage > 0) ? arrowColor : arrowDisabledColor;
    drawChar16(layer, atlas, font, 0x25B2,
        centerX - upGlyph.width * 0.5f,
        topArrowY,
        zIndex, upColor, group, targetFBO, fbsize, viewId);

    // 下矢印 ▼
    uint32_t downColor = (currentPage < totalPages - 1) ? arrowColor : arrowDisabledColor;
    drawChar16(layer, atlas, font, 0x25BC,
        centerX - downGlyph.width * 0.5f,
        bottomArrowY,
        zIndex, downColor, group, targetFBO, fbsize, viewId);

    // トラック（縦の角丸矩形）
    {float r = trackWidth * 0.5f;
    layer->pushFill(
        centerX - trackWidth * 0.5f,
        trackY,
        trackWidth,
        trackHeight,
        r, r, r, r, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        trackColor, 0x00000000,
        0.5f, 1.0f, 0.0f,
        0x000000AA,
        zIndex,
        targetFBO, fbsize, viewId);}

    // つまみ位置
    float progress = (totalPages > 1)
        ? (float)currentPage / (totalPages - 1)
        : 0.5f;
    float thumbY = trackY + trackHeight * progress;

    // つまみ（縦長）
    layer->pushFill(
        centerX - thumbRadius,
        thumbY - thumbRadius / 2,
        thumbRadius * 2,
        thumbRadius,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        thumbColor, 0x00000000,
        0.0f, 0.0f, 0.0f,
        0x00000000,
        zIndex + 0.1f,
        targetFBO, fbsize, viewId);
}
void drawUnderScrollBar(LayerInfo* layer,
    float x, float y, float width, float height, float thumbHeight,
    float currentX, float pageWidth, float totalWidth,
    float zIndex,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId) {

    // スクロール不要
    if (totalWidth <= pageWidth) {
        return;
    }

    const uint32_t trackColor = 0x666666FF;
    const uint32_t thumbColor = 0x40FFFFFF;

    float centerY = y + height * 0.5f;

    // トラック（幅いっぱい）
    {float r = height * 0.5f;
    layer->pushFill(
        x, y,
        width,
        height,
        r, r, r, r, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        trackColor, 0x00000000,
        0.5f, 1.0f, 0.0f,
        0x000000AA,
        zIndex,
        targetFBO, fbsize, viewId);}

    // つまみサイズ（pageWidth / totalWidth の比率）
    float thumbRatio = pageWidth / totalWidth;
    float thumbWidth = width * thumbRatio;
    thumbWidth = std::max(thumbWidth, 16.f);  // 最小幅

    // つまみ位置
    float maxScroll = totalWidth - pageWidth;
    float scrollRatio = (maxScroll > 0) ? currentX / maxScroll : 0.f;
    scrollRatio = std::clamp(scrollRatio, 0.f, 1.f);
    float thumbX = x + (width - thumbWidth) * scrollRatio;

    // つまみ（角丸長方形）
    {float r = thumbHeight * 0.5f;
    layer->pushFill(
        thumbX,
        centerY - thumbHeight * 0.5f,
        thumbWidth,
        thumbHeight,
        r, r, r, r, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        thumbColor, 0x00000000,
        0.0f, 0.0f, 0.0f,
        0x00000000,
        zIndex + 0.1f,
        targetFBO, fbsize, viewId);}
}

void drawRightScrollBar(LayerInfo* layer,
    float x, float y, float width, float height, float thumbWidth,
    float currentY, float pageHeight, float totalHeight,
    float zIndex,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId) {

    // スクロール不要
    if (totalHeight <= pageHeight) {
        return;
    }

    const uint32_t trackColor = 0x666666FF;
    const uint32_t thumbColor = 0x40FFFFFF;

    float centerX = x + width * 0.5f;

    // トラック（高さいっぱい）
    {float r = width * 0.5f;
    layer->pushFill(
        x, y,
        width,
        height,
        r, r, r, r, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        trackColor, 0x00000000,
        0.5f, 1.0f, 0.0f,
        0x000000AA,
        zIndex,
        targetFBO, fbsize, viewId);}

    // つまみサイズ（pageHeight / totalHeight の比率）
    float thumbRatio = pageHeight / totalHeight;
    float thumbHeight = height * thumbRatio;
    thumbHeight = std::max(thumbHeight, 16.f);  // 最小高さ

    // つまみ位置
    float maxScroll = totalHeight - pageHeight;
    float scrollRatio = (maxScroll > 0) ? currentY / maxScroll : 0.f;
    scrollRatio = std::clamp(scrollRatio, 0.f, 1.f);
    float thumbY = y + (height - thumbHeight) * scrollRatio;

    // つまみ（角丸長方形）
    {float r = thumbWidth * 0.5f;
    layer->pushFill(
        centerX - thumbWidth * 0.5f,
        thumbY,
        thumbWidth,
        thumbHeight,
        r, r, r, r, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        thumbColor, 0x00000000,
        0.0f, 0.0f, 0.0f,
        0x00000000,
        zIndex + 0.1f,
        targetFBO, fbsize, viewId);}
}
// 指定範囲 [offset, offset+n) を計測。offset=0 のとき従来 MeasureString と同等。
void MeasureStringRange(FontAtlas& atlas, FontId font, String* text, int offset, int n, float width, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group) {
    *retwid = 0;
    *rethei = 0;
    for (*len = 0; *len < (size_t)n; (*len)++) {
        uint32_t codepoint = (uint32_t)GetChar(text, offset + (int)*len);
        // CR (= U+000D, CRLF の \r) はフォントにグリフが無いので何もしない (advance=0)。
        if (codepoint == 0x0D) continue;
        if (codepoint == 0x09) {
            const auto& sp = atlas.getOrAddGlyph(font, ' ', group);
            float adv = sp.advance * 4.0f;
            if (*retwid + adv > width) return;
            *retwid += adv;
            if (*rethei < sp.height) *rethei = sp.height;
            continue;
        }
        const auto& gi = atlas.getOrAddGlyph(font, codepoint, group);
        if (gi.advance > 0) {
            if (*retwid + gi.advance > width) return;
            *retwid += gi.advance;
            if (*rethei < gi.height) *rethei = gi.height;
        }
    }
}
// 既存 API: text の先頭から n 文字を計測 (= offset=0)。SubString 渡しの呼び出し互換用。
void MeasureString(FontAtlas& atlas, FontId font, String* text, int n, float width, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group) {
    MeasureStringRange(atlas, font, text, 0, n, width, retwid, rethei, len, group);
}
void MeasureStringVerticalRange(FontAtlas& atlas, FontId font, String* text, int offset, int n, float height, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group) {
    *retwid = 0;
    *rethei = 0;
    for (*len = 0; *len < (size_t)n; (*len)++) {
        uint32_t codepoint = (uint32_t)GetChar(text, offset + (int)*len);
        const auto& gi = atlas.getOrAddGlyph(font, codepoint, group);
        if (gi.advance > 0) {
            if (*rethei + gi.height > height) return;
            *rethei += gi.height;
            if (*retwid < gi.advance) *retwid = gi.advance;
        }
    }
}
void MeasureStringVertical(FontAtlas& atlas, FontId font, String* text, int n, float height, float* retwid, float* rethei, size_t* len, ExtendedRenderGroup* group) {
    MeasureStringVerticalRange(atlas, font, text, 0, n, height, retwid, rethei, len, group);
}
// UTF-8対応版（1文字ずつデコード）
uint32_t utf8Decode(const char*& p) {
    uint32_t c = (unsigned char)*p++;
    if (c < 0x80) return c;

    int extraBytes = 0;
    if ((c & 0xE0) == 0xC0) { extraBytes = 1; c &= 0x1F; }
    else if ((c & 0xF0) == 0xE0) { extraBytes = 2; c &= 0x0F; }
    else if ((c & 0xF8) == 0xF0) { extraBytes = 3; c &= 0x07; }

    for (int i = 0; i < extraBytes; i++) {
        c = (c << 6) | ((unsigned char)*p++ & 0x3F);
    }
    return c;
}

void drawTextUTF8(LayerInfo* layer, FontAtlas& atlas, FontId font,
    const char* text, int length, float x, float y,
    int zIndex, uint32_t color, RenderGroup* group, bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId) {
    float cursorX = x;
    float cursorY = y;

    const char* p = text;
    for (int i = 0; i < length; i++) {
        uint32_t codepoint = utf8Decode(p);
        const auto& gi = atlas.getOrAddGlyph(font, codepoint, group);

        if (gi.width > 0 && gi.height > 0) {
            layer->pushText(cursorX + gi.bearingX, cursorY - gi.bearingY, gi.width, gi.height, gi.u0, gi.v0, gi.u1 - gi.u0, gi.v1 - gi.v0, color, zIndex, &atlas.getPageTexture(gi.pageIndex), targetFBO, fbsize, viewId);
        }

        cursorX += gi.advance;
    }
}



// bgfx初期化
bool initBgfx(SDL_Window* window) {
    fprintf(stderr, "[BGFX] >>> initBgfx ENTER  window=%p\n", (void*)window);
    fflush(stderr);
    bgfx::PlatformData pd;
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    memset(&pd, 0, sizeof(pd));
    fprintf(stderr, "[BGFX] PlatformData zeroed, props=%llu\n",
        (unsigned long long)props);
    fflush(stderr);

#ifdef __APPLE__
    __block int width = 1200, height = 800;
#else
    int width = 1200, height = 800;
#endif

#if defined(__ANDROID__)
    void* nwh = nullptr;
    // Wait for ANativeWindow to be ready (up to 5 seconds)
    for (int retry = 0; retry < 50 && nwh == nullptr; retry++) {
        nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER, nullptr);
        if (nwh == nullptr) {
            SDL_Log("initBgfx: Waiting for ANativeWindow... (attempt %d)", retry + 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            props = SDL_GetWindowProperties(window);  // Refresh properties
        }
    }
    if (nwh == nullptr) {
        SDL_Log("initBgfx: FATAL - ANativeWindow is NULL after waiting!");
        return false;
    }
    pd.nwh = nwh;
    SDL_GetWindowSize(window, &width, &height);
    SDL_Log("initBgfx: Android nwh=%p, size=%dx%d", nwh, width, height);

#elif defined(_WIN32)
    void* hwnd = SDL_GetPointerProperty(props, "SDL.window.win32.hwnd", nullptr);
    pd.nwh = hwnd;
    SDL_GetWindowSize(window, &width, &height);
    fprintf(stderr, "[BGFX] Win32 hwnd=%p  size=%dx%d\n", hwnd, width, height);
    fflush(stderr);

#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IOS || TARGET_OS_SIMULATOR
        // iOS: Metal layer from SDL
        // bgfx init must be called from main thread for iOS
        // Store metalView as static to prevent deallocation
        static SDL_MetalView s_metalView = nullptr;
        __block void* metalLayer = nullptr;
        __block bool bgfxInitResult = false;

        auto initBlock = ^{
            s_metalView = SDL_Metal_CreateView(window);
            metalLayer = SDL_Metal_GetLayer(s_metalView);
            SDL_GetWindowSize(window, &width, &height);
            SDL_Log("initBgfx: iOS metalView=%p, metalLayer=%p, size=%dx%d",
                    s_metalView, metalLayer, width, height);

            if (!metalLayer) {
                SDL_Log("initBgfx: Failed to get Metal layer!");
                bgfxInitResult = false;
                return;
            }

            bgfx::PlatformData pdLocal;
            memset(&pdLocal, 0, sizeof(pdLocal));
            pdLocal.nwh = metalLayer;

            // iOS single-thread mode: call renderFrame() before init()
            // This prevents bgfx from creating internal render thread
            bgfx::renderFrame();

            bgfx::Init initLocal;
            initLocal.platformData = pdLocal;
            initLocal.type = bgfx::RendererType::Metal;
            initLocal.resolution.width = width;
            initLocal.resolution.height = height;
            initLocal.resolution.reset = BGFX_RESET_VSYNC;
            initLocal.resolution.formatDepthStencil = bgfx::TextureFormat::D24S8;
            initLocal.debug = true;

            bgfxInitResult = bgfx::init(initLocal);
            if (bgfxInitResult) {
                SDL_Log("bgfx initialized successfully");
                // Force depth buffer creation via reset
                bgfx::reset(width, height, BGFX_RESET_VSYNC, bgfx::TextureFormat::D24S8);
                SDL_Log("bgfx reset with depth buffer");
            } else {
                SDL_Log("bgfx::init failed");
            }
        };

        if ([NSThread isMainThread]) {
            initBlock();
        } else {
            dispatch_sync(dispatch_get_main_queue(), initBlock);
        }

        return bgfxInitResult;
    #else
        // macOS
        void* cocoa_win = SDL_GetPointerProperty(props, "SDL.window.cocoa.window", nullptr);
        pd.nwh = cocoa_win;
    #endif

#elif defined(__linux__)
    // X11
    void* x11_display = SDL_GetPointerProperty(props, "SDL.window.x11.display", nullptr);
    uint64_t x11_window = (uint64_t)SDL_GetNumberProperty(props, "SDL.window.x11.window", 0);
    // Wayland
    void* wl_display = SDL_GetPointerProperty(props, "SDL.window.wayland.display", nullptr);
    void* wl_surface = SDL_GetPointerProperty(props, "SDL.window.wayland.surface", nullptr);

    if (wl_surface) {            // Wayland 優先
        pd.ndt = wl_display;
        pd.nwh = wl_surface;
        pd.type = bgfx::NativeWindowHandleType::Wayland;
        SDL_Log("initBgfx: Using Wayland");
    }
    else {                     // X11
        pd.ndt = x11_display;
        pd.nwh = (void*)(uintptr_t)x11_window;
        pd.type = bgfx::NativeWindowHandleType::Default;
        SDL_Log("initBgfx: Using X11 display=%p window=0x%lx", x11_display, (unsigned long)x11_window);
    }
    SDL_GetWindowSize(window, &width, &height);
    SDL_Log("initBgfx: Linux window size=%dx%d", width, height);
#endif

    bgfx::Init init;
    init.platformData = pd;
#if defined(__ANDROID__) || defined(__linux__)
    init.type = bgfx::RendererType::Vulkan;
#elif TARGET_OS_IOS || TARGET_OS_SIMULATOR
    init.type = bgfx::RendererType::Metal;
#endif
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
#if defined(__linux__) && !defined(__ANDROID__)
    init.resolution.formatDepthStencil = bgfx::TextureFormat::D24S8;  // Enable depth buffer (Desktop Linux bgfx only)
#endif
    init.debug = true;  // デバッグ情報を有効

    fprintf(stderr, "[BGFX] >>> calling bgfx::init  type=%d  res=%ux%u  reset=0x%x  pd.nwh=%p\n",
        (int)init.type, init.resolution.width, init.resolution.height,
        init.resolution.reset, pd.nwh);
    fflush(stderr);
    bool initOk = bgfx::init(init);
    fprintf(stderr, "[BGFX] <<< bgfx::init returned: %s\n", initOk ? "true" : "false");
    fflush(stderr);
    if (!initOk) {
        SDL_Log("bgfx::init failed");
        return false;
    }
    SDL_Log("bgfx initialized successfully");

    fprintf(stderr, "[BGFX] >>> first-frame setup\n"); fflush(stderr);
    // Force first black frame to prevent flicker on X11
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, width, height);
    bgfx::touch(0);
    bgfx::frame();
    fprintf(stderr, "[BGFX] <<< first-frame done\n"); fflush(stderr);
    SDL_Log("First black frame sent");

    return true;
}
struct BatchCache {
    bgfx::DynamicVertexBufferHandle dvb = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle  dib = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;  // テク
    uint32_t numVertices = 0;
    uint32_t numIndices = 0;
    bool valid = false;
};
// NativeWindow は newelem.h で定義済み

// === メインスレッドへの SDL 委任用イベント（RenderThread → MainThread） ===
inline Uint32 SDL_EVENT_SDL_REQUEST = 0;

// SDL委任リクエスト（RenderThread → MainThread、SDL操作のみ）
struct SDLRequest {
    enum Type { CreateWindow, ResizeWindow, DestroyWindow, ShowWindow, HideWindow, MoveWindow };
    Type type;
    // CreateWindow 用
    NativeWindowType winType;
    SDL_Window* parentSDLWindow = nullptr;
    int x, y, w, h;
    bool visible = true;  // CreateWindow時に表示するか
    int cornerRound = 0;  // 0=デフォルト, 1=角丸なし, 2=角丸(大), 3=角丸(小)
    // ResizeWindow 用
    NativeWindow* target = nullptr;
    int newW, newH;
    // 結果
    SDL_Window* resultSDLWindow = nullptr;
    void* resultNwh = nullptr;
    bool success = false;
    std::promise<void> done;
};

// === ポップアップコマンド（GoThread → RenderThread、描画と同じキューで順序保証） ===
enum class PopupCmdType { Create, Resize, Destroy, Show, Hide, Move };

struct PopupCmd {
    PopupCmdType type;
    ThreadGC* thgc = nullptr;  // どのタブに属するか
    // Create 用
    NativeWindowType winType = WindowType_Popup;
    PopupAnchor anchor = Anchor_None;
    int x = 0, y = 0, w = 0, h = 0;
    NewElement* anchorElem = nullptr;
    bool visible = true;  // Create時に表示するか
    int cornerRound = 2;  // 0=デフォルト, 1=角丸なし, 2=角丸(大), 3=角丸(小)
    // Resize 用 / Move 用
    NativeWindow* target = nullptr;
    int newW = 0, newH = 0;
};

// HopStar::set_target を othelem.h 等から前方宣言ベースで呼ぶためのフリー関数
inline void HopStar_set_target(HopStar* hp, ThreadGC* thgc);

class HopStar {
public:
    ThreadGC* mainGC = nullptr;  // メインスレッド用GC
    NewLocal* mainLocal = nullptr;  // メインスレッドのUI要素ツリー
    std::vector<ThreadGC*> thgcs;
    int target_order = -1;
    ThreadGC* target;
    std::mutex m;
    // 「描画キュー有り」フラグ。producer が publish 後に ++、consumer が処理時に --。
    // m の ロック取り直しを避けるため atomic (= cv.wait の predicate からも安全に読める)。
    std::atomic<int> render{ 0 };
    std::condition_variable cv_;

    // === FontManagerで複数フォントを管理 ===
    PatternAtlasBuilder patternAtlas;
    ImageMaster master = ImageMaster{};
	ImageLoader loader = ImageLoader(master);

    // === ウィンドウ管理 ===
    // windows はタブ毎に ThreadGC::windows で管理
    uint64_t basicViewId = 100000;         // ウィンドウ用論理viewId（大きい値 = 後に描画 = オフスクリーン完了後）

    // ポップアップコマンドキュー（GoThread → RenderThread）
    std::mutex popupCmdMutex;
    std::vector<PopupCmd> popupCmdQueue;

    // --- SDL操作をメインスレッドに委任して同期待ち ---
    void requestSDL(SDLRequest& req) {
        std::future<void> f = req.done.get_future();
        SDL_Event ev = {};
        ev.type = SDL_EVENT_SDL_REQUEST;
        ev.user.data1 = &req;
        SDL_PushEvent(&ev);
        f.get();  // メインスレッドが処理するまでブロック（~1ms）
    }

    // --- RenderThread から呼ばれるポップアップ破棄 ---
    void destroyPopupWindow(ThreadGC* thgc, NativeWindow* popup) {
        if (!popup) return;
        // タブのウィンドウリストから削除
        for (auto it = thgc->windows.begin(); it != thgc->windows.end(); ++it) {
            if (*it == popup) {
                thgc->windows.erase(it);
                break;
            }
        }
        // bgfx FBO 解放（RenderThread 上なので直接OK）
        if (bgfx::isValid(popup->fbo)) {
            bgfx::destroy(popup->fbo);
            popup->fbo = BGFX_INVALID_HANDLE;
        }
        // SDL ウィンドウ破棄をメインスレッドに委任
        if (popup->sdlWindow) {
            SDLRequest req{};
            req.type = SDLRequest::DestroyWindow;
            req.target = popup;
            requestSDL(req);
            popup->sdlWindow = nullptr;
        }
        // GC root 解放 (= popup の local / anchorElement の登録を外す)
        if (popup->rootNode) {
            GC_release_root_node(popup->rootNode);
            popup->rootNode = nullptr;
        }
        delete popup;
    }

    void resolvePopupPosition(NativeWindow* popup) {
        if (!popup || !popup->sdlWindow) return;
        switch (popup->anchor) {
        case Anchor_Element: {
            auto* e = popup->anchorElement;
            if (!e) break;
            int relX = (int)(e->pos2.x + e->pos.x) + popup->anchorX;
            int relY = (int)(e->pos2.y + e->pos.y + e->size.y) + popup->anchorY;
            SDL_SetWindowPosition(popup->sdlWindow, relX, relY);
            break;
        }
        case Anchor_ParentWindow:
            SDL_SetWindowPosition(popup->sdlWindow, popup->anchorX, popup->anchorY);
            break;
        case Anchor_Screen: {
            if (popup->parent && popup->parent->sdlWindow) {
                int px, py;
                SDL_GetWindowPosition(popup->parent->sdlWindow, &px, &py);
                SDL_SetWindowPosition(popup->sdlWindow, popup->anchorX - px, popup->anchorY - py);
            } else {
                SDL_SetWindowPosition(popup->sdlWindow, popup->anchorX, popup->anchorY);
            }
            break;
        }
        default:
            break;
        }
    }

    // --- RenderThread から呼ばれるリサイズ処理（SDL部分はメインに委任） ---
    bool resizePopupWindow(NativeWindow* nw, int newW, int newH) {
        if (!nw || !nw->sdlWindow) return false;

        // SDL ウィンドウリサイズをメインスレッドに委任
        SDLRequest req{};
        req.type = SDLRequest::ResizeWindow;
        req.target = nw;
        req.newW = newW; req.newH = newH;
        requestSDL(req);

        if (!req.success) return false;

        // 内部サイズを更新
        nw->size = { newW, newH };

        // FBO を再作成（RenderThread 上なので直接OK）
        if (bgfx::isValid(nw->fbo)) {
            bgfx::destroy(nw->fbo);
        }
        nw->fbo = bgfx::createFrameBuffer(nw->nwh, (uint16_t)newW, (uint16_t)newH);
        return bgfx::isValid(nw->fbo);
    }

    NativeWindow* findWindowBySDLId(SDL_WindowID id) {
        for (auto* nw : mainGC->windows) {
            if (nw->sdlWindow && SDL_GetWindowID(nw->sdlWindow) == id) {
                return nw;
            }
        }
        for (auto* thgc : thgcs) {
            for (auto* nw : thgc->windows) {
                if (nw->sdlWindow && SDL_GetWindowID(nw->sdlWindow) == id) {
                    return nw;
                }
            }
        }
        return nullptr;
    }

    HopStar() {
	}
    void initFonts() {
        // TTF初期化
        if (!TTF_Init()) {
            SDL_Log("TTF_Init failed: %s", SDL_GetError());
        }
        // 複数のフォントを登録
        master.registerFont("sans", "fonts/NotoSansJP-Regular.ttf", 16);
        master.registerFont("sans", "fonts/NotoSansJP-Regular.ttf", 24);
        master.registerFont("sans", "fonts/NotoSansJP-Regular.ttf", 32);

        master.registerFont("noto", "fonts/NotoSerifJP-Regular.ttf", 16);
        master.registerFont("noto", "fonts/NotoSerifJP-Regular.ttf", 24);

        master.registerFont("robo", "fonts/RobotoMono-Regular.ttf", 14);
        master.registerFont("robo", "fonts/RobotoMono-Regular.ttf", 16);
    }
    void push_tab(ThreadGC* thgc, NewLocal* local) {
        std::lock_guard lock(m);
        target_order++;

        // title はクロスヒープ alias の登録元 (= title->page が thgc の local を指す)。
        // address を永続させるため Old 直接確保。
        NewTabTitle* title = (NewTabTitle*)GC_alloc_old(mainGC, CType::_TabTitleC);
        initTabTitle(mainGC, title);
        NewDirectAddLast(thgc, mainLocal, tab, title);
        NewLetter* l1 = (NewLetter*)GC_alloc(mainGC, CType::_LetterC);
        initLetter(mainGC, l1, getFont("sans", 16), LetterType::_Letter);
        l1->text = createString(mainGC, (char*)"letter1", 7, 1);
        l1->color = 0xFFFFFFFF;
        NewElementAddLast(mainGC, mainLocal, (NewElement*)title, (NewElement*)l1);
        thgcs.insert(thgcs.begin() + static_cast<std::ptrdiff_t>(target_order), thgc);
        tab->select = title;
        title->page = local;
        tab->linked->page = local;
        local->linked = tab->linked;
        target = thgc;

        // local (= UI local in thgc) の parent は linked (= LinkedElement、mainGC の Old)。
        // linked が画面上の placeholder で、その位置に goLocal の中身が描画される。
        // 絶対座標は parent chain で linked → ... と辿るので、parent を linked にすることで
        // goLocal の描画位置が linked の位置と一致する。
        // linked は Old 確保なので動かない (= 永久 valid)。
        local->parent = tab->linked;

        // 新しい ThreadGC (= magc / DLL タブ用 thgc) の thgc 関連 root をここで登録。
        // initLocal で thgc->local 等は既にセットされているが、root 登録はまだ。
        // push_tab を経由して新規 thgc が hoppy に登録される時に root 登録もまとめて行う。
        RootNode* threadRn = GC_add_root_node(thgc);
        GC_add_root(threadRn, (char**)&thgc->local);
        GC_add_root(threadRn, (char**)&thgc->map);
        GC_add_root(threadRn, (char**)&thgc->hoveredList);
        GC_add_root(threadRn, (char**)&thgc->hoveredSpanList);

        if (mainGC && !mainGC->windows.empty()) {
            NativeWindow* mainWin = mainGC->windows[0];
            NativeWindow* tabWin = new NativeWindow();
            tabWin->sdlWindow = mainWin->sdlWindow;       // 同じ物理ウィンドウ
            tabWin->nwh = mainWin->nwh;             // 同じ bgfx native ハンドル
            tabWin->viewId = mainWin->viewId;          // 同じ描画 pass
            tabWin->size = mainWin->size;            // 寸法スナップショット
            tabWin->type = mainWin->type;
            // local / fbo / parent / anchor: 既定値 (nullptr / INVALID) のまま
            thgc->windows.push_back(tabWin);
            // tabWin の GC ポインタ field を root 登録
            tabWin->rootNode = GC_add_root_node(thgc);
            GC_add_root(tabWin->rootNode, (char**)&tabWin->local);
            GC_add_root(tabWin->rootNode, (char**)&tabWin->anchorElement);
        }
    }

    // ThreadGC* を target に直接切り替える (TabTitle クリック時の Local ページ切替で使う)。
    // thgcs に含まれていない場合は何もしない。
    //
    // ロック注意: この関数は buildFrame の中 (= 既に m を保持している状態) から
    //   クリックハンドラ経由で呼ばれるので、m を再取得すると non-recursive mutex の
    //   self-lock で std::system_error("resource deadlock would occur") が飛ぶ。
    //   書き込み先は target (ポインタ) と target_order (int) のみ、thgcs は
    //   要素追加のみ (削除しない設計) なのでロック無しで安全に読める。
    void set_target(ThreadGC* thgc) {
        if (!thgc) return;
        for (size_t i = 0; i < thgcs.size(); i++) {
            if (thgcs[i] == thgc) {
                target = thgc;
                target_order = (int)i;
                return;
            }
        }
    }
    friend inline void HopStar_set_target(HopStar* hp, ThreadGC* thgc);

    // タブを削除
    void remove_tab(int n) {
        std::lock_guard lock(m);
        thgcs.erase(thgcs.begin() + n);
    }

    // タブを選択
    void select_tab(int n) {
        std::lock_guard lock(m);
        target = thgcs.begin()[n];
    }
    void RootOffscreen(ThreadGC* thgc, List* list, Offscreen* offscreen) {
        add_list(thgc, list, (char*)offscreen);
        for (Offscreen* off = offscreen->child->next; off != offscreen->child; off = off->next) {
            if (off->type == OffscreenType::LinkedOff) {
                LinkedElement* linked = (LinkedElement*)(off->elem);
                if (linked->page->type != LetterType::_Main) {
                    RootOffscreen(thgc, list, linked->page->offscreen);
                }
            }
            else {
                RootOffscreen(thgc, list, off);
            }
        }
    }
    void buildFrame(ThreadGC* thgc, uint64_t frameId) {
        if (invalidate == 0) {
            return;
        }
        // publish() で wait する前に必ず unlock する必要があるため unique_lock。
        // (= render thread が m を取れずに acquire 出来ない deadlock を防ぐ)
        std::unique_lock<std::mutex> lock2(m);
        if (target != thgc) return;
        mainGC->queue->resume_all();
        auto* q = target->commandQueue;

        auto currentTime = std::chrono::steady_clock::now();
        this->deltaTime = std::chrono::duration<float>(currentTime - lastTime_).count();
        lastTime_ = currentTime;
        time += this->deltaTime;
        mainGC->deltaTime = this->deltaTime;
        target->deltaTime = this->deltaTime;

        // 各ウィンドウのツリーに対してRebuild/OffscreenLayoutを実行
        NewLocal* local = target->local;
        if (mainLocal->dirty & DirtyType::Rebuild > 0) {
            int n = 0;
            for (auto* nw : mainGC->windows) {
                nw->local->offscreen->child->next = nw->local->offscreen->child->before = nw->local->offscreen->child;
                RebuildOffscreen(mainGC, nw->local->offscreen->child, nw->local, &n);
            }
        }
        if ((local->dirty & DirtyType::Rebuild) > 0) {
            int n = 0;
            for (auto* nw : target->windows) {
                if (!nw->local) continue;
                nw->local->offscreen->child->next = nw->local->offscreen->child->before = nw->local->offscreen->child;
                RebuildOffscreen(target, nw->local->offscreen->child, nw->local, &n);
            }
        }
        if (mainLocal->dirty & DirtyType::OffscreenLayout > 0) {
            mainLocal->screens = (List*)create_list(mainGC, sizeof(Offscreen*), CType::_Offscreen);
            for (auto* nw : mainGC->windows) {
                if (!nw->local) continue;
                RootOffscreen(mainGC, mainLocal->screens, nw->local->offscreen);
            }
        }
        if ((local->dirty & DirtyType::OffscreenLayout) > 0) {
            local->screens = (List*)create_list(target, sizeof(Offscreen*), CType::_Offscreen);
            for (auto* nw : target->windows) {
                if (!nw->local) continue;
                RootOffscreen(target, local->screens, nw->local->offscreen);
            }
        }

        if ((local->dirty & DirtyType::Partial) > 0) {
            // アニメーションキューをMeasure直前に実行
            if (mainGC->animQueue) mainGC->animQueue->resume_all();
            if (target->animQueue && target->animQueue != mainGC->animQueue) target->animQueue->resume_all();
            q->begin(frameId, 1024);
            ::viewId = 0;  // フレーム開始時に論理viewIdカウンタをリセット（オフスクリーン用: 小さい値から）
            auto& layer = q->setCurrentSlotLayer({}, 1.0f, true, true, 1200, 800);
            // 離れたオフスクリーン用: ルートから到達できなかったもの（layout==trueのまま）をMeasure
            for (int i = 0; i < mainLocal->screens->size; i++) {
                Offscreen* screen = (Offscreen*)*get_list(mainLocal->screens, i);
                if (screen->layout) {
                    NewMeasure measure;
                    int order = 0;
                    screen->elem->Measure(mainGC, (NewElement*)screen->elem, &measure, mainLocal, &order);
                }
            }
            for (int i = 0; i < local->screens->size; i++) {
                Offscreen* screen = (Offscreen*)*get_list(local->screens, i);
                if (screen->layout) {
                    NewMeasure measure;
                    int order = 0;
                    screen->elem->Measure(target, (NewElement*)screen->elem, &measure, local, &order);
                }
            }
            for (int i = 0; i < mainLocal->screens->size; i++) {
                Offscreen* screen = (Offscreen*)*get_list(mainLocal->screens, i);
                if (screen->paint != Offscreen::PaintNone && screen->window && screen->window->visible) {
                    NewGraphic graphic;
                    graphic.pos.x = 0; graphic.pos.y = 0; graphic.size.x = screen->window->size.x; graphic.size.y = screen->window->size.y;
                    graphic.start.x = 0; graphic.start.y = 0; graphic.end.x = screen->window->size.x; graphic.end.y = screen->window->size.y;
                    graphic.fb = &screen->window->fbo; graphic.viewId = screen->window->viewId; graphic.viewId2 = ::viewId++; graphic.layer = &layer;
                    graphic.group = NULL; graphic.fbsize = &screen->window->size;
                    graphic.winFb = &screen->window->fbo; graphic.winFbsize = &screen->window->size; graphic.winViewId = screen->window->viewId;
                    graphic.deltaTime = this->deltaTime;
                    graphic.paint = screen->paint;
                    screen->elem->Draw(target, (NewElement*)screen->elem, &graphic, mainLocal, q);
                }
            }
            for (int i = 0; i < local->screens->size; i++) {
                Offscreen* screen = (Offscreen*)*get_list(local->screens, i);
                if (screen->paint != Offscreen::PaintNone && screen->window && screen->window->visible) {
                    NewGraphic graphic;
                    graphic.pos.x = 0; graphic.pos.y = 0; graphic.size.x = screen->window->size.x; graphic.size.y = screen->window->size.y;
                    graphic.start.x = 0; graphic.start.y = 0; graphic.end.x = screen->window->size.x; graphic.end.y = screen->window->size.y;
                    graphic.fb = &screen->window->fbo; graphic.viewId = screen->window->viewId; graphic.viewId2 = ::viewId++; graphic.layer = &layer;
                    graphic.group = NULL; graphic.fbsize = &screen->window->size;
                    graphic.winFb = &screen->window->fbo; graphic.winFbsize = &screen->window->size; graphic.winViewId = screen->window->viewId;
                    graphic.deltaTime = this->deltaTime;
                    graphic.paint = screen->paint;
                    screen->elem->Draw(target, (NewElement*)screen->elem, &graphic, local, q);
                }
            }
            // Hover描画: 各要素のオフスクリーンのウィンドウに直接上書き
            auto drawHoverList = [&](List* hoveredList) {
                for (int i = 0; i < hoveredList->size; i++) {
                    NewElement* elem = (NewElement*)*get_list(hoveredList, i);
                    if (!elem->hovered) continue;
                    Offscreen* off = FindOffscreen(elem);
                    if (!off || !off->window) continue;
                    NativeWindow* win = off->window;
                    PointF absPos = getAbsolutePosition(elem);
                    float w = elem->size.x + elem->paddings[1] + elem->paddings[3];
                    float h = elem->size.y + elem->paddings[0] + elem->paddings[2];
                    float r0 = boRadius(elem, 0), r1 = boRadius(elem, 1), r2 = boRadius(elem, 2), r3 = boRadius(elem, 3);
                    layer.pushFill(absPos.x, absPos.y, w, h,
                        r0, r1, r2, r3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                        elem->hoverColor, 0,
                        0.0f, 0.0f, 0.0f, 0, 12000.0f,
                        &win->fbo, &win->size, win->viewId);
                }
            };
            drawHoverList(mainGC->hoveredList);
            drawHoverList(target->hoveredList);
            if (local->select.from != NULL && local->select.window != nullptr) {
                NativeWindow* selWin = local->select.window;
                NewGraphic graphic;
                graphic.pos.x = 0; graphic.pos.y = 0;
                graphic.size.x = (float)selWin->size.x; graphic.size.y = (float)selWin->size.y;
                graphic.start.x = 0; graphic.start.y = 0;
                graphic.end.x = (float)selWin->size.x; graphic.end.y = (float)selWin->size.y;
                graphic.fb = &selWin->fbo; graphic.fbsize = &selWin->size; graphic.viewId = selWin->viewId; graphic.layer = &layer;
                SelectDraw(target, local, &graphic, q);
            }
            // 順序が重要:
            //   1. unlock(m): consumer が m を取れるように (= cv_.wait 復帰時に acquireCount++ できる)。
            //   2. publish(): front_ を最新 slot に切替 (= consumer が次 acquire で見える状態)。
            //   3. render++ + notify_one: consumer を起こす (= wait predicate (render>0) を満たす)。
            //   4. waitAcquired(): consumer が acquire 完了するまで spin (= frame skip 防止)。
            //   ※ 旧設計は publish 内で spin していたため、(3) が呼ばれず consumer が
            //     起きないまま spin 続行 → deadlock。先に notify してから spin する必要あり。
            lock2.unlock();
            q->publish();
            render.fetch_add(1, std::memory_order_release);
            invalidate.store(1, std::memory_order_relaxed);
            cv_.notify_one();
            q->waitAcquired();
        }
    }
    NewTabBar* tab;
    std::atomic<int> invalidate{ 1 };
    bool update = true;
    float time = 0;
    float deltaTime = 0;
    std::chrono::steady_clock::time_point lastTime_ = std::chrono::steady_clock::now();
};

// HopStar の完全型定義が揃ったので、前方宣言したフリー関数をここで実装する。
// othelem.h など HopStar の詳細を知らないファイルから呼べるように提供する。
inline void HopStar_set_target(HopStar* hp, ThreadGC* thgc) {
    if (hp) hp->set_target(thgc);
}

int addPattern(ThreadGC* thgc, std::vector<float>& colors, std::vector<float>& widthes) {
    return thgc->hoppy->patternAtlas.addPattern(colors, widthes);
}
int addPattern(ThreadGC* thgc, std::vector<uint32_t>& colors, std::vector<float>& widthes) {
    return thgc->hoppy->patternAtlas.addPattern(colors, widthes);
}
static void spanEnterLeave(List* oldList, List* newList) {
    if (!oldList || !newList) return;
    for (int i = 0; i < oldList->size; i++) {
        HoveredSpan* old = *(HoveredSpan**)get_list(oldList, i);
        bool found = false;
        for (int j = 0; j < newList->size; j++) {
            HoveredSpan* cur = *(HoveredSpan**)get_list(newList, j);
            if (cur->atsspan == old->atsspan && cur->stylespan == old->stylespan) { found = true; break; }
        }
        if (!found) {
            if (old->atsspan) old->atsspan->hovered = false;
            if (old->stylespan) old->stylespan->hovered = false;
        }
    }
    for (int i = 0; i < newList->size; i++) {
        HoveredSpan* cur = *(HoveredSpan**)get_list(newList, i);
        bool found = false;
        for (int j = 0; j < oldList->size; j++) {
            HoveredSpan* old = *(HoveredSpan**)get_list(oldList, j);
            if (old->atsspan == cur->atsspan && old->stylespan == cur->stylespan) { found = true; break; }
        }
        if (!found) {
            if (cur->atsspan) cur->atsspan->hovered = true;
            if (cur->stylespan) cur->stylespan->hovered = true;
        }
    }
}
Generator MouseMainButton(HopStar* hoppy, MouseEvent* mv) {
    // メインウィンドウクリック時、開いているポップアップを閉じてイベント消費
    if (mv->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        auto closePopups = [&](ThreadGC* thgc) {
            if (!thgc->openPopups.empty()) {
                auto popups = thgc->openPopups;
                for (auto* nw : popups) {
                    if (nw->local && nw->local->type == LetterType::_Popup) {
                        ((PopupWindow*)nw->local)->visible = false;
                    }
                    myHideWindow(thgc, nw);
                }
                hoppy->invalidate = 1;
            }
        };
        closePopups(hoppy->target);
        closePopups(hoppy->mainGC);
    }

    // スクロールバードラッグ中の処理 (ウィンドウ外も追従)
    NewLocal* local = hoppy->target->local;
    if (local->scrollDrag.elem != nullptr) {
        if (mv->action == SDL_EVENT_MOUSE_MOTION) {
            if (mv->click) {
                scrollDragUpdate(local, mv);
            } else {
                local->scrollDrag.elem = nullptr;
                SDL_CaptureMouse(false);
            }
            hoppy->invalidate = 1;
            delete mv;
            co_return (char*)0;
        }
        if (mv->action == SDL_EVENT_MOUSE_BUTTON_UP) {
            local->scrollDrag.elem = nullptr;
            SDL_CaptureMouse(false);
            hoppy->invalidate = 1;
            delete mv;
            co_return (char*)0;
        }
    }

    // Hover処理: mainGCのhoveredListを初期化
    List* oldMainHovered = hoppy->mainGC->hoveredList;
    hoppy->mainGC->hoveredList = create_list(hoppy->target, sizeof(NewElement*), CType::_ElementC);
    List* oldMainHoveredSpan = hoppy->mainGC->hoveredSpanList;
    hoppy->mainGC->hoveredSpanList = create_list(hoppy->target, sizeof(HoveredSpan*), CType::_HoveredSpan);

    // Hover処理: targetのhoveredListを初期化
    List* oldHovered = hoppy->target->hoveredList;
    hoppy->target->hoveredList = create_list(hoppy->target, sizeof(NewElement*), CType::_ElementC);
    List* oldHoveredSpan = hoppy->target->hoveredSpanList;
    hoppy->target->hoveredSpanList = create_list(hoppy->target, sizeof(HoveredSpan*), CType::_HoveredSpan);

    hoppy->mainLocal->Mouse(hoppy->mainGC, hoppy->mainLocal, mv, { 0.0f, 0.0f }, hoppy->mainLocal);

    // mainGC Hover Enter/Leave判定
    for (int i = 0; i < oldMainHovered->size; i++) {
        NewElement* old = (NewElement*)*get_list(oldMainHovered, i);
        bool found = false;
        for (int j = 0; j < hoppy->mainGC->hoveredList->size; j++) {
            if ((NewElement*)*get_list(hoppy->mainGC->hoveredList, j) == old) { found = true; break; }
        }
        if (!found) old->hovered = false;
    }
    for (int i = 0; i < hoppy->mainGC->hoveredList->size; i++) {
        NewElement* cur = (NewElement*)*get_list(hoppy->mainGC->hoveredList, i);
        bool found = false;
        for (int j = 0; j < oldMainHovered->size; j++) {
            if ((NewElement*)*get_list(oldMainHovered, j) == cur) { found = true; break; }
        }
        if (!found) cur->hovered = true;
    }
    // mainGC HoveredSpan Enter/Leave判定
    spanEnterLeave(oldMainHoveredSpan, hoppy->mainGC->hoveredSpanList);

    // target Hover Enter/Leave判定
    for (int i = 0; i < oldHovered->size; i++) {
        NewElement* old = (NewElement*)*get_list(oldHovered, i);
        bool found = false;
        for (int j = 0; j < hoppy->target->hoveredList->size; j++) {
            if ((NewElement*)*get_list(hoppy->target->hoveredList, j) == old) { found = true; break; }
        }
        if (!found) old->hovered = false;
    }
    for (int i = 0; i < hoppy->target->hoveredList->size; i++) {
        NewElement* cur = (NewElement*)*get_list(hoppy->target->hoveredList, i);
        bool found = false;
        for (int j = 0; j < oldHovered->size; j++) {
            if ((NewElement*)*get_list(oldHovered, j) == cur) { found = true; break; }
        }
        if (!found) cur->hovered = true;
    }
    // target HoveredSpan Enter/Leave判定
    spanEnterLeave(oldHoveredSpan, hoppy->target->hoveredSpanList);

    // Mouse()内でスクロールバードラッグが開始された場合
    if (mv->action == SDL_EVENT_MOUSE_BUTTON_DOWN && local->scrollDrag.elem != nullptr) {
        SDL_CaptureMouse(true);
    }

    hoppy->invalidate = 1;
    delete mv;
    co_return (char*)0;

}
Generator KeyButton(HopStar* hoppy, KeyEvent* kv) {
    bool select = false;
    NewLocal* local = hoppy->target->local;
	SelectKey(hoppy->target, local, kv);
    hoppy->update = true;
    hoppy->invalidate = 1;
    co_return (char*)0;
}
// ポップアップ用マウスイベント
Generator PopupMouseButton(HopStar* hoppy, NativeWindow* nw, MouseEvent* mv) {
    if (nw && nw->local) {
        // Hover処理
        List* oldHovered = hoppy->target->hoveredList;
        hoppy->target->hoveredList = create_list(hoppy->target, sizeof(NewElement*), CType::_ElementC);
        List* oldHoveredSpan = hoppy->target->hoveredSpanList;
        hoppy->target->hoveredSpanList = create_list(hoppy->target, sizeof(HoveredSpan*), CType::_HoveredSpan);
        if (nw->local->hoverColor != 0) add_list(hoppy->target, hoppy->target->hoveredList, (char*)nw->local);

        ((NewElement*)nw->local)->Mouse(hoppy->target, nw->local, mv, { 0.0f, 0.0f }, hoppy->target->local);

        // Hover Enter/Leave判定
        for (int i = 0; i < oldHovered->size; i++) {
            NewElement* old = (NewElement*)*get_list(oldHovered, i);
            bool found = false;
            for (int j = 0; j < hoppy->target->hoveredList->size; j++) {
                if ((NewElement*)*get_list(hoppy->target->hoveredList, j) == old) { found = true; break; }
            }
            if (!found) old->hovered = false;
        }
        for (int i = 0; i < hoppy->target->hoveredList->size; i++) {
            NewElement* cur = (NewElement*)*get_list(hoppy->target->hoveredList, i);
            bool found = false;
            for (int j = 0; j < oldHovered->size; j++) {
                if ((NewElement*)*get_list(oldHovered, j) == cur) { found = true; break; }
            }
            if (!found) cur->hovered = true;
        }
        // Popup HoveredSpan Enter/Leave判定
        spanEnterLeave(oldHoveredSpan, hoppy->target->hoveredSpanList);

        hoppy->invalidate = 1;
    }
    delete mv;
    co_return (char*)0;
}
// ポップアップ用キーイベント
Generator PopupKeyButton(HopStar* hoppy, NativeWindow* nw, KeyEvent* kv) {
    if (nw && nw->local) {
        NewLocal* local = hoppy->target->local;
        SelectKey(hoppy->target, local, kv);
        hoppy->update = true;
        hoppy->invalidate = 1;
    }
    co_return (char*)0;
}
Generator WindowResized(HopStar* hoppy, NativeWindow* nw) {
    if (nw == NULL) co_return (char*)0;
    nw->local->size.x = nw->size.x - nw->local->paddings[1] - nw->local->paddings[3];
    nw->local->size.y = nw->size.y - nw->local->paddings[0] - nw->local->paddings[2];
    nw->needsFboReset = true;
    nw->local->offscreen->markLayout(nw->local->gc->local, nw->local);
    co_return (char*)0;
}
std::promise<void> initDone;

// === タブ領域のヒットテスト用 snapshot ===
// SDL の hit_test_cb は WM_NCHITTEST 経由で呼ばれる (main thread)。
// Layout 計算と並行して getAbsolutePosition を呼ぶと parent chain を辿る最中に
// 親子ポインタ書き換えと衝突して AV する可能性があるので、layout 完了側で
// 絶対座標を atomic にスナップショット → hit_test_cb はそれだけ読む形にする。
namespace tabhit {
	inline std::atomic<float> g_left  { 0.0f };
	inline std::atomic<float> g_top   { 0.0f };
	inline std::atomic<float> g_right { 0.0f };
	inline std::atomic<float> g_bottom{ 0.0f };
	inline std::atomic<bool>  g_valid { false };
}

class RenderThread {
public:
	RenderThread(HopStar* hoppy, SDL_Window* window) : hoppy_(hoppy), window_(window), running_(false) {
    }
    void start() {
        running_ = true;
#if TARGET_OS_IOS || TARGET_OS_SIMULATOR
        // iOS: Don't create thread, will be called from main thread via CADisplayLink
        // Initialize on main thread
        initOnMainThread();
#else
        thread_ = std::thread(&RenderThread::run, this);
#endif
    }

    void stop() {
        running_ = false;
#if !(TARGET_OS_IOS || TARGET_OS_SIMULATOR)
        if (thread_.joinable()) {
            thread_.join();
        }
#endif
    }

#if TARGET_OS_IOS || TARGET_OS_SIMULATOR
    bool initialized_ = false;
    std::unique_ptr<nle::AudioOutput> audio_output_ios_;

    void initOnMainThread() {
        initBgfx(window_);
        hoppy_->master.initialize();
        hoppy_->initFonts();
        initDone.set_value();

        auto video_track = timeline_.add_video_track();
        auto video_clip = video_track->add_clip();

        std::string videoPath = getBundlePath("pyonpyon.mp4");
        if (!video_clip->open(videoPath.c_str())) {
            SDL_Log("Video file not found: %s", videoPath.c_str());
            return;
        }

        video_clip->timeline_range.start = 0;
        video_clip->timeline_range.end = video_clip->source_range.duration();

        auto audio_track = timeline_.add_audio_track();
        auto audio_clip = audio_track->add_clip();

        if (audio_clip->open(videoPath.c_str())) {
            audio_clip->timeline_range.start = 0;
            audio_clip->timeline_range.end = audio_clip->source_range.duration();
        }

        audio_output_ios_ = std::make_unique<nle::AudioOutput>(timeline_);
        if (!audio_output_ios_->initialize()) {
            SDL_Log("Audio output init failed");
        }

        renderer_ = std::make_unique<nle::TimelineRenderer<ImageMaster>>(timeline_, hoppy_->master);
        timeline_.play();

        initialized_ = true;
        SDL_Log("Render thread started");
    }

    RenderResources resources_ios_{};
    bool resources_initialized_ = false;

    void renderOneFrame() {
        if (!running_ || !initialized_) return;

        // Initialize resources once
        if (!resources_initialized_) {
            QuadVertex::init();
            initRenderResources(resources_ios_);
            resources_initialized_ = true;
        }

        // Process deferred GPU uploads from other threads
        // Must happen on main thread in bgfx single-thread mode
        hoppy_->loader.processGpuUploadsMainThread();

        std::vector<LayerInfo>* layers = NULL;
        uint64_t frameId = 0;
        int token = -1;
        RenderCommandQueue* queue = NULL;

        // Non-blocking check for render data (iOS can't block main thread)
        {
            std::unique_lock<std::mutex> lock(hoppy_->m, std::try_to_lock);
            if (lock.owns_lock() && hoppy_->render > 0) {
                hoppy_->render--;
                queue = hoppy_->target->commandQueue;
                queue->acquire(&layers, frameId, token);
            }
        }

        {
            std::unique_lock<std::mutex> lock3(hoppy_->patternAtlas.m, std::try_to_lock);
            if (lock3.owns_lock()) {
                hoppy_->patternAtlas.reset();
                hoppy_->patternAtlas.buildTextures();
            }
        }

        if (layers != NULL) {
            hoppy_->master.beginFrame();
            for (auto& layer : *layers) {
                renderLayer(NULL, layer, resources_ios_, hoppy_->master, hoppy_->patternAtlas);
            }

            renderer_->update();
            auto render_items = renderer_->get_render_items();

            // Setup view 30 for video rendering (with depth buffer)
            if (!render_items.empty()) {
                int winW, winH;
                SDL_GetWindowSize(window_, &winW, &winH);
                // Update layout size to match actual window size
                width_ = winW;
                height_ = winH;
                bgfx::setViewFrameBuffer(30, BGFX_INVALID_HANDLE);
                bgfx::setViewClear(30, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
                bgfx::setViewRect(30, 0, 0, uint16_t(winW), uint16_t(winH));
                float orthoProj[16];
                bx::mtxOrtho(orthoProj, 0.0f, float(winW), float(winH), 0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
                bgfx::setViewTransform(30, NULL, orthoProj);
            }

            for (size_t i = 0; i < render_items.size(); i++) {
                std::vector<UnifiedDrawCommand> commands;
                commands.reserve(render_items.size());

                float base_z = 0.0f;
                for (size_t j = 0; j < render_items.size(); j++) {
                    const auto& item = render_items[j];
                    auto resolved = hoppy_->master.resolve(item.image_id);

                    if (!resolved.isReady()) continue;

                    float px, py, pw, ph;
                    calculate_layout_pixels(j, render_items.size(), item.clip, px, py, pw, ph);

                    UnifiedDrawCommand cmd{};
                    cmd.type = DrawCommandType::Image;
                    cmd.viewId = hoppy_->target->windows[0]->viewId;
                    cmd.zIndex = base_z + j * 0.001f;
                    cmd.texture = resolved.resolved.texture;
                    cmd.texture2 = &nulltex;

                    cmd.x = px;
                    cmd.y = py;
                    cmd.width = pw;
                    cmd.height = ph;

                    cmd.angle = 0.0f;
                    cmd.dataOffset = 0.0f;
                    cmd.scrollX = 1.0f;
                    cmd.scrollY = 1.0f;

                    cmd.radius = 0.0f;
                    cmd.aa = 1.0f;
                    cmd.shadowX = 0.0f;
                    cmd.shadowY = 0.0f;
                    cmd.shadowBlur = 0.0f;
                    cmd.borderTop = cmd.borderRight = cmd.borderBottom = cmd.borderLeft = 0.0f;

                    cmd.shadowColor = 0x00000000;
                    cmd.fillColor = 0xFFFFFFFF;
                    cmd.borderColor = 0x00000000;
                    std::vector<UnifiedDrawCommand*> single_batch = { &cmd };
                    drawUnifiedBatch(single_batch, resources_ios_,
                        BGFX_INVALID_HANDLE, BGFX_INVALID_HANDLE, 30);
                }
            }
        }

        bgfx::frame();
        // bgfx::renderFrame() は init前の1回だけ。毎フレームは呼ばない
    }
#endif
    enum class LayoutMode {
        Stack,   // PiP形式
        Grid,    // グリッド
        Single   // 単一表示
    };
	nle::Timeline timeline_;
    std::unique_ptr<nle::TimelineRenderer<ImageMaster>> renderer_;
    std::unique_ptr<nle::AudioOutput> audio_output_;
    size_t selected_track_ = 0;
    int width_ = 1280, height_ = 720;
    LayoutMode layout_mode_ = LayoutMode::Stack;
    private:void calculate_layout_pixels(size_t index, size_t total,
        std::shared_ptr<nle::VideoClip> clip,
        float& x, float& y, float& w, float& h) {
        float video_aspect = (float)clip->get_width() / clip->get_height();

        switch (layout_mode_) {
        case LayoutMode::Stack:
            if (index == 0) {
                // フルスクリーン（アスペクト比維持）
                float screen_aspect = (float)width_ / height_;
                if (video_aspect > screen_aspect) {
                    w = (float)width_;
                    h = w / video_aspect;
                    x = 0;
                    y = (height_ - h) * 0.5f;
                }
                else {
                    h = (float)height_;
                    w = h * video_aspect;
                    x = (width_ - w) * 0.5f;
                    y = 0;
                }
            }
            else {
                // PiP: 右上に並べる
                float pip_scale = 0.25f;
                w = width_ * pip_scale;
                h = w / video_aspect;
                float margin = 10.0f;
                x = width_ - w - margin;
                y = margin + (h + margin) * (index - 1);
            }
            break;

        case LayoutMode::Grid: {
            int cols = (int)std::ceil(std::sqrt((double)total));
            int rows = (int)std::ceil((double)total / cols);
            int col = index % cols;
            int row = index / cols;

            float cell_w = (float)width_ / cols;
            float cell_h = (float)height_ / rows;
            float cell_aspect = cell_w / cell_h;

            if (video_aspect > cell_aspect) {
                w = cell_w;
                h = w / video_aspect;
            }
            else {
                h = cell_h;
                w = h * video_aspect;
            }

            x = col * cell_w + (cell_w - w) * 0.5f;
            y = row * cell_h + (cell_h - h) * 0.5f;
            break;
        }

        case LayoutMode::Single:
        default:
            if (index == selected_track_) {
                float screen_aspect = (float)width_ / height_;
                if (video_aspect > screen_aspect) {
                    w = (float)width_;
                    h = w / video_aspect;
                    x = 0;
                    y = (height_ - h) * 0.5f;
                }
                else {
                    h = (float)height_;
                    w = h * video_aspect;
                    x = (width_ - w) * 0.5f;
                    y = 0;
                }
            }
            else {
                x = y = w = h = 0;  // 非表示
            }
            break;
        }
    }
    void run() {
        if (!initBgfx(window_)) {
            SDL_Log("RenderThread: bgfx initialization failed, aborting render thread");
            initDone.set_value();  // Signal anyway to unblock waiting threads
            return;
        }
        //bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_IFH);
        hoppy_->master.initialize();
        hoppy_->initFonts();
        initDone.set_value();
        videoLoaded_ = false;
        std::string videoPath = getBundlePath("pyonpyon.mp4");
        auto video_track = timeline_.add_video_track();
        auto video_clip = video_track->add_clip();

        if (video_clip->open(videoPath.c_str())) {
            video_clip->timeline_range.start = 0;
            video_clip->timeline_range.end = video_clip->source_range.duration();
            videoLoaded_ = true;
            SDL_Log("Video loaded successfully: %s", videoPath.c_str());
        } else {
            SDL_Log("Video file not found: %s (continuing without video)", videoPath.c_str());
        }

        // オーディオトラック（同じファイルから音声を抽出）
        auto audio_track = timeline_.add_audio_track();
        auto audio_clip = audio_track->add_clip();

        if (videoLoaded_ && audio_clip->open(videoPath.c_str())) {
            audio_clip->timeline_range.start = 0;
            audio_clip->timeline_range.end = audio_clip->source_range.duration();
            SDL_Log("Audio clip loaded successfully");
        } else {
            SDL_Log("Audio clip failed to load or video not loaded");
        }
        audio_output_ = std::make_unique<nle::AudioOutput>(timeline_);
        if (!audio_output_->initialize()) {
            SDL_Log("AudioOutput initialization failed");
        }
        renderer_ = std::make_unique <nle::TimelineRenderer<ImageMaster >>(timeline_, hoppy_->master);
        if (videoLoaded_) {
            timeline_.play();
        }
        /*auto id = engine.load("banban.mp3");
        engine.play(id);*/
        /*std::wstring wpath = L"おどれ！！マグロっ子.mp3";
        std::string utf8 = utf16_to_utf8(wpath);
		auto id2 = engine.load(utf8.c_str());
        engine.play(id2);*/
        QuadVertex::init();
        // シェーダー読み込み
        RenderResources resources{};
        initRenderResources(resources);
        // ビュー設定
        bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x808080ff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, 1200, 800);
        // 射影行列設定（2D用）
        float orthoProj[16];
        bx::mtxOrtho(orthoProj, 0.0f, 1200.0f, 800.0f, 0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, NULL, orthoProj);
        int frameCount = 0;
        auto lastFpsTime = std::chrono::high_resolution_clock::now();

        int scrollX = 0;
        int scrollY = 0;// ★ ビュー0の設定（スクリーン描画用）

        SDL_Log("Render thread started");
        bool rendered = false;
        NewLocal* local = NULL;
        int oldtoken = -1;
        while (running_) {
            // LLDB driver spawn 前に親の bgfx を shutdown して GPU state を
            // 解放するためのシグナル受信ポイント (旧 lldbctl 経由)。現状
            // 立てる側のコードは無いので no-op だが、将来のために残す。
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<LayerInfo>* layers = NULL;
            uint64_t frameId = 0;
            int token = -1;
            bool gotNew = false;
            RenderCommandQueue* queue = NULL;
            {
                std::unique_lock<std::mutex> lock(hoppy_->m);
                hoppy_->cv_.wait(lock, [this] {
                    return hoppy_->render > 0;
                });
                if (hoppy_->render > 0) hoppy_->render--;;
                bool gotNewLocal = false;
                queue = hoppy_->target->commandQueue;
                gotNew = hoppy_->target->commandQueue->acquire(&layers, frameId, token);
                NativeWindow* nw = hoppy_->mainGC->windows[0];
                if (nw->needsFboReset) {
                    bgfx::reset(nw->size.x, nw->size.y);
                    nw->needsFboReset = false;
                }
                for (int i = 1; i < hoppy_->mainGC->windows.size(); i++) {
                    nw = hoppy_->mainGC->windows[i];
                    if (nw->needsFboReset) {
                        nw->needsFboReset = false;
                        bgfx::destroy(nw->fbo);
                        nw->fbo = bgfx::createFrameBuffer(nw->nwh, (uint16_t)nw->size.x, (uint16_t)nw->size.y);
                    }
                }
                for (int i = 0; i < hoppy_->target->windows.size(); i++) {
                    nw = hoppy_->target->windows[i];
                    if (nw->needsFboReset && bgfx::isValid(nw->fbo)) {
                        nw->needsFboReset = false;
                        bgfx::destroy(nw->fbo);
                        nw->fbo = bgfx::createFrameBuffer(nw->nwh, (uint16_t)nw->size.x, (uint16_t)nw->size.y);
                    }
                }
            }
            // === ポップアップコマンドを処理（描画前に順序保証） ===
            {
                std::vector<PopupCmd> cmds;
                {
                    std::lock_guard<std::mutex> lock(hoppy_->popupCmdMutex);
                    cmds.swap(hoppy_->popupCmdQueue);
                }
                for (auto& cmd : cmds) {
                    switch (cmd.type) {
                    case PopupCmdType::Create: {
                        // GoThread が確保済みの NativeWindow に SDL/FBO を埋める
                        NativeWindow* nw = cmd.target;
                        ThreadGC* cmdThgc = cmd.thgc;
                        NativeWindow* parent = nullptr;
                        if (cmdThgc && !cmdThgc->windows.empty()) parent = cmdThgc->windows[0];
                        nw->parent = parent;

                        // SDL座標を計算
                        int relX = cmd.x, relY = cmd.y;
                        if (cmd.anchor == Anchor_Element && cmd.anchorElem) {
                            PointF p = getAbsolutePosition(cmd.anchorElem);
                            relX = p.x;
                            relY = p.y + cmd.anchorElem->size.y;
                        }

                        // SDL ウィンドウ作成をメインスレッドに委任
                        SDLRequest req{};
                        req.type = SDLRequest::CreateWindow;
                        req.winType = cmd.winType;
                        req.parentSDLWindow = parent ? parent->sdlWindow : nullptr;
                        req.x = relX; req.y = relY; req.w = cmd.w; req.h = cmd.h;
                        req.visible = cmd.visible;
                        req.cornerRound = cmd.cornerRound;
                        hoppy_->requestSDL(req);

                        if (req.success) {
                            nw->sdlWindow = req.resultSDLWindow;
                            nw->nwh = req.resultNwh;
                            nw->visible = cmd.visible;
                            // FBO作成（RenderThread上なので直接OK）
                            nw->fbo = bgfx::createFrameBuffer(nw->nwh, (uint16_t)cmd.w, (uint16_t)cmd.h);
                            nw->viewId = hoppy_->basicViewId++;  // 永続的な論理viewId（ウィンドウ用）
                            if (cmdThgc) cmdThgc->windows.push_back(nw);
                        } else {
                            SDL_Log("createPopupWindow failed via RenderThread");
                        }
                        break;
                    }
                    case PopupCmdType::Resize:
                        if (cmd.target) hoppy_->resizePopupWindow(cmd.target, cmd.newW, cmd.newH);
                        break;
                    case PopupCmdType::Destroy:
                        if (cmd.target && cmd.thgc) hoppy_->destroyPopupWindow(cmd.thgc, cmd.target);
                        break;
                    case PopupCmdType::Show:
                        if (cmd.target && cmd.target->sdlWindow) {
                            SDLRequest req{};
                            req.type = SDLRequest::ShowWindow;
                            req.target = cmd.target;
                            hoppy_->requestSDL(req);
                            cmd.target->visible = true;
                        }
                        break;
                    case PopupCmdType::Hide:
                        if (cmd.target && cmd.target->sdlWindow) {
                            SDLRequest req{};
                            req.type = SDLRequest::HideWindow;
                            req.target = cmd.target;
                            hoppy_->requestSDL(req);
                            cmd.target->visible = false;
                        }
                        break;
                    case PopupCmdType::Move:
                        if (cmd.target && cmd.target->sdlWindow) {
                            cmd.target->anchorX = cmd.x;
                            cmd.target->anchorY = cmd.y;
                            SDLRequest req{};
                            req.type = SDLRequest::MoveWindow;
                            req.target = cmd.target;
                            req.x = cmd.x; req.y = cmd.y;
                            hoppy_->requestSDL(req);
                        }
                        break;
                    }
                }
            }

            {
                std::lock_guard<std::mutex> lock3(hoppy_->patternAtlas.m);
                hoppy_->patternAtlas.reset();
                hoppy_->patternAtlas.buildTextures();
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            hoppy_->loader.processGpuUploadsMainThread();
            if (layers != NULL) {
                hoppy_->master.beginFrame();
                for (auto& layer : *layers) {
                    renderLayer(local, layer, resources, hoppy_->master, hoppy_->patternAtlas);
                }
                rendered = true;
                // layers構築処理
                auto end = std::chrono::high_resolution_clock::now();

                // Only process video renderer if video was loaded
                /*if (videoLoaded_) {
                    renderer_->update();
                    auto render_items = renderer_->get_render_items();

                    // レイヤー順に描画（下から上へ）
                    for (size_t i = 0; i < render_items.size(); i++) {
                        std::vector<UnifiedDrawCommand> commands;
                        commands.reserve(render_items.size());

                        float base_z = 0.0f;
                        for (size_t i = 0; i < render_items.size(); i++) {
                            const auto& item = render_items[i];
                            auto resolved = hoppy_->master.resolve(item.image_id);

                            if (!resolved.isReady()) continue;

                            // レイアウト計算（ピクセル座標）
                            float px, py, pw, ph;
                            calculate_layout_pixels(i, render_items.size(), item.clip, px, py, pw, ph);

                            UnifiedDrawCommand cmd{};
                            cmd.type = DrawCommandType::Image;
                            cmd.viewId = hoppy_->target->windows[0]->viewId;
                            cmd.zIndex = base_z + i * 0.001f;
                            cmd.texture = resolved.resolved.texture;
                            cmd.texture2 = &nulltex;

                            // 位置とサイズ（ピクセル座標）
                            cmd.x = px;
                            cmd.y = py;
                            cmd.width = pw;
                            cmd.height = ph;

                            // Image モード: UV座標
                            cmd.angle = 0.0f;      // uvMin.x
                            cmd.dataOffset = 0.0f; // uvMin.y
                            cmd.scrollX = 1.0f;    // uvMax.x
                            cmd.scrollY = 1.0f;    // uvMax.y

                            // 角丸、シャドウなし
                            cmd.radius = 0.0f;
                            cmd.aa = 1.0f;
                            cmd.shadowX = 0.0f;
                            cmd.shadowY = 0.0f;
                            cmd.shadowBlur = 0.0f;
                            cmd.borderTop = cmd.borderRight = cmd.borderBottom = cmd.borderLeft = 0.0f;

                            cmd.shadowColor = 0x00000000;
                            cmd.fillColor = 0xFFFFFFFF;
                            cmd.borderColor = 0x00000000;
                            std::vector<UnifiedDrawCommand*> single_batch = { &cmd };
                            drawUnifiedBatch(single_batch, resources,
                                BGFX_INVALID_HANDLE, BGFX_INVALID_HANDLE, 30);
                        }
                    }
                }*/

                frameN = bgfx::frame();
                // bgfx::renderFrame() は init前の1回だけ。毎フレームは呼ばない
            }
            // tab 領域の screen 絶対座標を atomic snapshot に保存
            // (= hit_test_cb は main thread からこの値だけ読む。parent chain
            //    辿りを避けて AV と torn read を防ぐ)
            if (hoppy_->tab) {
                PointF abs = getAbsolutePosition(hoppy_->tab);
                tabhit::g_left  .store(abs.x,                       std::memory_order_relaxed);
                tabhit::g_top   .store(abs.y,                       std::memory_order_relaxed);
                tabhit::g_right .store(abs.x + hoppy_->tab->size.x, std::memory_order_relaxed);
                tabhit::g_bottom.store(abs.y + hoppy_->tab->size.y, std::memory_order_relaxed);
                tabhit::g_valid .store(true, std::memory_order_release);
            }
        }
    }
    HopStar* hoppy_;
    SDL_Window* window_;
    std::thread thread_;
    std::atomic<bool> running_;
    bool videoLoaded_ = false;
};
FontAtlas* getAtlas(ThreadGC* thgc) {
    return &thgc->hoppy->master.fontAtlas();
}
int myGetFontHeight(ThreadGC* thgc, FontId font) {
    return thgc->hoppy->master.fontAtlas().getFontHeight(font);
}
RenderCommandQueue* createRCQ() {
	return new RenderCommandQueue();
}
ExtendedRenderGroup& createGroup(ThreadGC* thgc) {
    return thgc->hoppy->master.createGroup(0);  // SurfaceId = 0
}

ImageId myloadTexture2D(ThreadGC* thgc, const char* path, ImageUsage usage) {
    return thgc->hoppy->loader.loadFromFile(path, usage, false);
}
ImageId queueOffscreenNew(ThreadGC* thgc, int width, int height) {
    return thgc->hoppy->master.reserveOffscreenTexture(width, height, false);
}
// Standalone↔Tiled のタイプ遷移時は新しい ImageId が返ってくる。
// 呼び出し側は必ず戻り値で自身の参照を更新すること。
//
// pageW / pageH > 0 で「ページめくり用」を指示できる。タイル辺長がそれらの
// 倍数になるよう丸められ、任意のページが単一タイル内に収まるようになる
// (ページがタイル境界をまたいで pushPageCurl が破綻するのを防ぐ)。
ImageId queueOffscreenResize(ThreadGC* thgc, ImageId offscreenid, int width, int height,
                             int pageW, int pageH) {
    // master 側は (int newW, int newH, uint16_t pageW, uint16_t pageH)。
    // pageW/pageH は viewport サイズなので uint16_t に必ず収まる。width/height はそのまま int で渡す。
    return thgc->hoppy->master.queueOffscreenResize(offscreenid, width, height,
                                                    (uint16_t)pageW, (uint16_t)pageH);
}
ResolvedTexture myresolveTexture(ImageMaster& master, ImageId imageId) {
    return master.resolveForDraw(imageId);
}
bgfx::TextureHandle* myResolveTexturePtr(ThreadGC* thgc, ImageId imageId) {
    return thgc->hoppy->master.resolveTexturePtr(imageId);
}
ResolvedTexture myResolveForDraw(ThreadGC* thgc, ImageId imageId) {
    return thgc->hoppy->master.resolveForDraw(imageId);
}
StandaloneTextureInfo* mygetStandaloneTextureInfo(ThreadGC* thgc, ImageId imageId) {
    return thgc->hoppy->master.getStandaloneTexture(imageId);
}
// 指定 tile を lazy 生成 (= bgfx createTexture/createFrameBuffer)。
// 既に valid なら no-op。createTiledOffscreenInternal で全 tile を一括生成しなく
// なったので、render path は使用直前にこれを呼ぶ。
inline bool mEnsureTile(ThreadGC* thgc, ImageId id, int tileIdx) {
    return thgc->hoppy->master.ensureTile(id, tileIdx);
}

TiledTextureInfo* mygetTiledTextureInfo(ThreadGC* thgc, ImageId imageId) {
    return thgc->hoppy->master.getTiledTexture(imageId);
}
// shader.h からの前方宣言 (タイル lookup 関数) の実装
TiledTextureInfo* shader_lookupTiledTexture(ImageMaster& master, ImageId id) {
    return master.getTiledTexture(id);
}
bool shader_ensureTile(ImageMaster& master, ImageId id, int tileIdx) {
    return master.ensureTile(id, tileIdx);
}
TiledResolveResult myResolveTiledForDraw(ThreadGC* thgc, ImageId imageId) {
    return thgc->hoppy->master.resolveTiledForDraw(imageId);
}
bool myIsTiledTexture(ThreadGC* thgc, ImageId imageId) {
    return thgc->hoppy->master.isTiledTexture(imageId);
}
// === ポップアップウィンドウ管理ラッパー（GoThread → RenderThread キュー経由） ===

// ポップアップ生成: NativeWindow* を即座に返す（SDL/FBO は RenderThread が後で埋める）
NativeWindow* myCreatePopupWindow(ThreadGC* thgc, NativeWindowType type, PopupAnchor anchor,
                                   int x, int y, int w, int h, int cornerRound, NewElement* anchorElem,
                                   bool visible) {
    // GoThread側でNativeWindowを先に確保（ポインタを即座に返せる）
    NativeWindow* nw = new NativeWindow{};
    nw->type = type;
    nw->anchor = anchor;
    nw->anchorX = x; nw->anchorY = y;
    nw->size = { w, h };
    nw->anchorElement = anchorElem;
    nw->visible = visible;

    // 動的に確保した popup の GC ポインタ field を root 登録 (= GC が popup の UI ツリーを拾う)。
    // RootNode を popup ごとに作っておき、myDestroyPopupWindow で release する。
    nw->rootNode = GC_add_root_node(thgc);
    GC_add_root(nw->rootNode, (char**)&nw->local);
    GC_add_root(nw->rootNode, (char**)&nw->anchorElement);

    PopupCmd cmd{};
    cmd.type = PopupCmdType::Create;
    cmd.thgc = thgc;
    cmd.winType = type;
    cmd.anchor = anchor;
    cmd.x = x; cmd.y = y; cmd.w = w; cmd.h = h;
    cmd.anchorElem = anchorElem;
    cmd.cornerRound = cornerRound;
    cmd.visible = visible;
    cmd.target = nw;  // 先に確保したNativeWindowを渡す
    {
        std::lock_guard<std::mutex> lock(thgc->hoppy->popupCmdMutex);
        thgc->hoppy->popupCmdQueue.push_back(cmd);
    }
    return nw;  // 即座に返す。描画命令より先にRenderThreadが初期化する
}

// ポップアップリサイズ（キューに積むだけ、ブロックしない）
void myResizePopupWindow(ThreadGC* thgc, NativeWindow* popup, int newW, int newH) {
    if (!popup) return;
    PopupCmd cmd{};
    cmd.type = PopupCmdType::Resize;
    cmd.target = popup;
    cmd.newW = newW; cmd.newH = newH;
    {
        std::lock_guard<std::mutex> lock(thgc->hoppy->popupCmdMutex);
        thgc->hoppy->popupCmdQueue.push_back(cmd);
    }
}

// ポップアップ破棄（キューに積むだけ、ブロックしない）
void myDestroyPopupWindow(ThreadGC* thgc, NativeWindow* popup) {
    if (!popup) return;
    PopupCmd cmd{};
    cmd.type = PopupCmdType::Destroy;
    cmd.thgc = thgc;
    cmd.target = popup;
    {
        std::lock_guard<std::mutex> lock(thgc->hoppy->popupCmdMutex);
        thgc->hoppy->popupCmdQueue.push_back(cmd);
    }
}

// ウィンドウ表示（キューに積むだけ、ブロックしない）
void myShowWindow(ThreadGC* thgc, NativeWindow* nw) {
    if (!nw) return;
    {
        std::lock_guard<std::mutex> lock(thgc->hoppy->popupCmdMutex);
        // 同じウィンドウのHideが未処理なら相殺して何もしない
        auto& q = thgc->hoppy->popupCmdQueue;
        for (auto it = q.begin(); it != q.end(); ++it) {
            if (it->target == nw && it->type == PopupCmdType::Hide) {
                q.erase(it);
                goto done_show;
            }
        }
        {
            PopupCmd cmd{};
            cmd.type = PopupCmdType::Show;
            cmd.target = nw;
            q.push_back(cmd);
        }
        done_show:;
    }
    // openPopups に追加（重複防止）
    auto& op = thgc->openPopups;
    if (std::find(op.begin(), op.end(), nw) == op.end()) {
        op.push_back(nw);
    }
}

// ウィンドウ非表示（キューに積むだけ、ブロックしない）
void myHideWindow(ThreadGC* thgc, NativeWindow* nw) {
    if (!nw) return;
    {
        std::lock_guard<std::mutex> lock(thgc->hoppy->popupCmdMutex);
        // 同じウィンドウのShowが未処理なら相殺して何もしない
        auto& q = thgc->hoppy->popupCmdQueue;
        for (auto it = q.begin(); it != q.end(); ++it) {
            if (it->target == nw && it->type == PopupCmdType::Show) {
                q.erase(it);
                goto done_hide;
            }
        }
        {
            PopupCmd cmd{};
            cmd.type = PopupCmdType::Hide;
            cmd.target = nw;
            q.push_back(cmd);
        }
        done_hide:;
    }
    // openPopups から削除
    auto& op = thgc->openPopups;
    op.erase(std::remove(op.begin(), op.end(), nw), op.end());
}

// ウィンドウ位置変更（キューに積むだけ、ブロックしない）
void myMoveWindow(ThreadGC* thgc, NativeWindow* nw, int x, int y) {
    if (!nw) return;
    PopupCmd cmd{};
    cmd.type = PopupCmdType::Move;
    cmd.target = nw;
    cmd.x = x; cmd.y = y;
    {
        std::lock_guard<std::mutex> lock(thgc->hoppy->popupCmdMutex);
        thgc->hoppy->popupCmdQueue.push_back(cmd);
    }
}
