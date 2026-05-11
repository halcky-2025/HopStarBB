#pragma once
#include "shader2.h"
#include <cfloat>
#include <unordered_map>

// imas.h はここから直接 include せず (依存型の循環があるため)、
// タイル lookup を関数で抽象化する。実装は ugui.h にて。
class ImageMaster;
struct TiledTextureInfo;
TiledTextureInfo* shader_lookupTiledTexture(ImageMaster& master, ImageId id);
// lazy 確保 forward (= ImageMaster::ensureTile を呼ぶ wrapper)。shader.h は imas.h
// を直接 include しないので関数経由でアクセス。実装は ugui.h。
bool              shader_ensureTile        (ImageMaster& master, ImageId id, int tileIdx);

// ============================================================
// LayerInfo（レイヤー管理）
// ============================================================


void LayerInfo::push(DrawCommand* cmd) {
    // 現在 LayerInfo に設定中のシザーを cmd に自動付与 (cmd 側が明示設定済みならそちら優先)。
    //   タイル展開で複製される tilecmd には UnifiedDrawCommand コピーコンストラクタ経由で伝播する。
    if (cmd->scissor == nullptr) cmd->scissor = currentScissor;
    // タイルFBO描画状態が有効で、このコマンドがタイル宛の場合、タイルごとに展開
    if (tiledTarget && cmd->targetFBO == tiledPlaceholderFBO) {
        auto* ucmd = static_cast<UnifiedDrawCommand*>(cmd);
        float cmdX = ucmd->x, cmdY = ucmd->y;
        // ── 単一 tile (page) 指定モード ──
        // tiledTargetPageIndex >= 0 のとき、auto-expand せずに該当 tile 1 個だけに push する。
        // 呼び出し側 (例: Table の Phase A per-tile heldGen) が「この tile を更新する」と
        // 明示的に指示した場合に使う。tile 境界跨ぎ cell を複数 region iteration で多重 push
        // することを防ぎつつ、submit 時 resolve (= dangling 回避) は維持。
        if (tiledTargetPageIndex >= 0) {
            int ti = tiledTargetPageIndex;
            if ((unsigned)ti < (unsigned)tiledTarget->tiles.size()) {
                auto& tile = tiledTarget->tiles[ti];
                UnifiedDrawCommand* tilecmd = new UnifiedDrawCommand(*ucmd);
                tilecmd->x = cmdX - (float)tile.x;
                tilecmd->y = cmdY - (float)tile.y;
                tilecmd->targetFBO    = nullptr;
                tilecmd->fbsize       = nullptr;
                tilecmd->tiledImageId = tiledTargetImageId;
                tilecmd->tileIndex    = ti;
                tilecmd->viewId       = tiledBaseViewId + (uint64_t)ti;
                cmds.push_back(tilecmd);
            }
            delete cmd;
            return;
        }
        float cmdW = ucmd->width, cmdH = ucmd->height;

        // 影によるはみ出し（影の方向＋ブラー半径分だけ各辺に拡張）
        float sBlur = ucmd->shadowBlur;
        float shadowLeft   = std::max(0.0f, -ucmd->shadowX + sBlur);
        float shadowRight  = std::max(0.0f,  ucmd->shadowX + sBlur);
        float shadowTop    = std::max(0.0f, -ucmd->shadowY + sBlur);
        float shadowBottom = std::max(0.0f,  ucmd->shadowY + sBlur);

        // cornerPatternに応じたborderRadiusの外側はみ出し
        // 弧の突出方向（シェーダーの凹角実装に基づく）:
        //   1(上),3(下): 弧が左右に突出
        //   2(右),4(左): 弧が上下に突出
        float maxR = std::max({ucmd->radiusTL, ucmd->radiusTR,
                               ucmd->radiusBR, ucmd->radiusBL});
        float mLeft = shadowLeft, mRight = shadowRight;
        float mTop = shadowTop, mBottom = shadowBottom;
        switch ((int)ucmd->cornerPattern) {
        case 1: mLeft  = std::max(mLeft,  maxR); mRight  = std::max(mRight,  maxR); break; // 上辺なし→左右突出
        case 2: mTop   = std::max(mTop,   maxR); mBottom = std::max(mBottom, maxR); break; // 右辺なし→上下突出
        case 3: mLeft  = std::max(mLeft,  maxR); mRight  = std::max(mRight,  maxR); break; // 下辺なし→左右突出
        case 4: mTop   = std::max(mTop,   maxR); mBottom = std::max(mBottom, maxR); break; // 左辺なし→上下突出
        case 5: mLeft  = std::max(mLeft,  maxR); mBottom = std::max(mBottom, maxR); break; // 上+右なし→左+下突出
        case 6: mTop   = std::max(mTop,   maxR); mLeft   = std::max(mLeft,   maxR); break; // 右+下なし→上+左突出
        case 7: mTop   = std::max(mTop,   maxR); mRight  = std::max(mRight,  maxR); break; // 下+左なし→上+右突出
        case 8: mRight = std::max(mRight, maxR); mBottom = std::max(mBottom, maxR); break; // 左+上なし→右+下突出
        }

        // マージン込みの拡張矩形からグリッド列・行範囲を O(1) で算出 (X/Y 別辺長)
        int tsx = (int)tiledTarget->tileSize;
        int tsy = (int)tiledTarget->tileSizeY;
        float expandL = cmdX - mLeft, expandT = cmdY - mTop;
        float expandR = cmdX + cmdW + mRight, expandB = cmdY + cmdH + mBottom;
        int col0 = std::clamp((int)std::floor(expandL / tsx),       0, tiledTarget->tilesX - 1);
        int row0 = std::clamp((int)std::floor(expandT / tsy),       0, tiledTarget->tilesY - 1);
        int col1 = std::clamp((int)std::floor((expandR - 1) / tsx), 0, tiledTarget->tilesX - 1);
        int row1 = std::clamp((int)std::floor((expandB - 1) / tsy), 0, tiledTarget->tilesY - 1);
        for (int row = row0; row <= row1; ++row) {
            for (int col = col0; col <= col1; ++col) {
                int ti = row * tiledTarget->tilesX + col;
                auto& tile = tiledTarget->tiles[ti];

                UnifiedDrawCommand* tilecmd = new UnifiedDrawCommand(*ucmd);
                tilecmd->x = cmdX - (float)tile.x;
                tilecmd->y = cmdY - (float)tile.y;
                // 「タイル時のFBO/fbsize」は submit 時に ImageMaster からルックアップする。
                // 現在のベクタ要素アドレス (&tile.fbo) を保存すると resize 時に dangling するため。
                tilecmd->targetFBO = nullptr;
                tilecmd->fbsize = nullptr;
                tilecmd->tiledImageId = tiledTargetImageId;
                tilecmd->tileIndex    = ti;
                tilecmd->viewId = tiledBaseViewId + (uint64_t)ti;
                cmds.push_back(tilecmd);
            }
        }
        delete cmd;
        return;
    }
    cmds.push_back(cmd);
}

// ============================================================
// Fill
// ============================================================

void LayerInfo::pushFill(float x, float y, float width, float height,
    float radiusTL, float radiusTR, float radiusBR, float radiusBL,
    float borderTop, float borderRight, float borderBottom, float borderLeft,
    float aaPixels,
    uint32_t fillColor, uint32_t borderColor,
    float shadowX, float shadowY, float shadowBlur,
    uint32_t shadowColor, float zIndex,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
    float blendMode,
    float cornerPattern)
{
    UnifiedDrawCommand* cmd = new UnifiedDrawCommand();
    cmd->type = DrawCommandType::Fill;
    cmd->blendMode = blendMode;
    cmd->targetFBO = targetFBO;
    cmd->viewId = viewId;
    cmd->x = x; cmd->y = y;
    cmd->width = width; cmd->height = height;
    cmd->aa = aaPixels;
    cmd->radiusTL = radiusTL;
    cmd->radiusTR = radiusTR;
    cmd->radiusBR = radiusBR;
    cmd->radiusBL = radiusBL;
    cmd->cornerPattern = cornerPattern;
    cmd->shadowX = shadowX; cmd->shadowY = shadowY;
    cmd->shadowBlur = shadowBlur;
    cmd->borderTop = borderTop;
    cmd->borderRight = borderRight;
    cmd->borderBottom = borderBottom;
    cmd->borderLeft = borderLeft;
    cmd->shadowColor = shadowColor;   // i_data4.x
    cmd->fillColor = fillColor;       // i_data4.y
    cmd->borderColor = borderColor;   // i_data4.z
    cmd->zIndex = zIndex;
    cmd->fbsize = fbsize;
    cmd->texture = &nulltex;
    cmd->texture2 = &nulltex;
    push(cmd);
}

// ============================================================
// PageCurl (UnifiedDrawCommandに統合)
// ============================================================
// レイアウト:
//   i_data1.x: uvMin (16bit pack)
//   i_data1.y: backUVMin (16bit pack)
//   i_data1.z: curlAngle
//   i_data1.w: curlRadius
//   i_data2.x: uvSize.x
//   i_data2.y: uvSize.y
//   i_data2.z: progress
//   i_data2.w: aa

void LayerInfo::pushPageCurl(float x, float y, float width, float height,
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
    float blendMode, bool vertical, bool spread)
{
    UnifiedDrawCommand* cmd = new UnifiedDrawCommand();
    cmd->type = DrawCommandType::PageCurl;
    cmd->blendMode = blendMode;
    cmd->targetFBO = targetFBO;
    cmd->viewId = viewId;
    cmd->x = x; cmd->y = y;
    cmd->width = width; cmd->height = height;

    // i_data1: uvMin, backUVMin をpack用に格納
    cmd->colorCount = uvMinX;         // packInstance で 16bit pack
    cmd->dataOffset = uvMinY;         // packInstance で 16bit pack
    cmd->angle = curlAngle;           // i_data1.z
    cmd->curlRadius = curlRadius;     // i_data1.w (新フィールド)

    // i_data1.y 用: backUVMin
    cmd->backUVMinX = backUVMinX;     // packInstance で 16bit pack (新フィールド)
    cmd->backUVMinY = backUVMinY;     // packInstance で 16bit pack (新フィールド)

    // i_data2: aa = 1横片面, 2縦片面, 3横両開き, 4縦両開き
    cmd->scrollX = uvSizeX;   // uvSize.x
    cmd->scrollY = uvSizeY;   // uvSize.y
    cmd->radius = progress;           // progress (0〜1)
    cmd->aa = (vertical ? 2.0f : 1.0f) + (spread ? 2.0f : 0.0f);

    // colors
    cmd->fillColor = fillColor;
    cmd->borderColor = borderColor;
    cmd->shadowColor = shadowColor;
    cmd->borderTop = borderTop;
    cmd->borderRight = borderRight;
    cmd->borderBottom = borderBottom;
    cmd->borderLeft = borderLeft;
    cmd->shadowX = shadowX; cmd->shadowY = shadowY; cmd->shadowBlur = shadowBlur;

    cmd->zIndex = zIndex;
    cmd->texture = tex1;
    cmd->texture2 = tex2;      // 新フィールド: 裏面テクスチャ
    cmd->fbsize = fbsize;
    push(cmd);
}

// ============================================================
// Image
// ============================================================
// 新レイアウト (float精度、packなし):
//   i_data1.z (angle)      = uvMin.x = atlasX
//   i_data1.w (dataOffset) = uvMin.y = atlasY
//   i_data2.x (scrollX)    = uvMax.x = atlasX + atlasW
//   i_data2.y (scrollY)    = uvMax.y = atlasY + atlasH

void LayerInfo::pushImage(float x, float y, float width, float height,
    float atlasX, float atlasY, float atlasW, float atlasH,
    float radiusTL, float radiusTR, float radiusBR, float radiusBL,
    float aaPixels,
    float borderTop, float borderRight, float borderBottom, float borderLeft,
    uint32_t borderColor,
    float shadowX, float shadowY, float shadowBlur,
    uint32_t shadowColor, uint32_t modulate,
    float zIndex, bgfx::TextureHandle* tex1,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
    float blendMode,
    float cornerPattern,
    float backUVMinX,
    bgfx::TextureHandle* tex2,
    bgfx::TextureHandle* tex3,
    bgfx::TextureHandle* tex4)
{
    UnifiedDrawCommand* cmd = new UnifiedDrawCommand();
    cmd->type = DrawCommandType::Image;
    cmd->blendMode = blendMode;
    cmd->targetFBO = targetFBO;
    cmd->viewId = viewId;
    // ジグソータブ突起分の描画領域拡張
    float tabR = backUVMinX;
    int tabPat = (int)cornerPattern;
    float tabExt = tabR * 1.5f;
    if (tabR > 0.0f && tabPat >= 9 && tabPat <= 12) {
        if (tabPat == 9)       { y -= tabExt; height += tabExt; }
        else if (tabPat == 10) { width += tabExt; }
        else if (tabPat == 11) { height += tabExt; }
        else                   { x -= tabExt; width += tabExt; }
    }
    cmd->x = x; cmd->y = y;
    cmd->width = width; cmd->height = height;
    cmd->dataOffset = 0; cmd->colorCount = 0;
    // 新レイアウト: float精度でUV座標を渡す
    cmd->angle = atlasX;              // uvMin.x (i_data1.z)
    cmd->curlRadius = atlasY;         // uvMin.y (i_data1.w に転送)
    cmd->scrollX = atlasX + atlasW;   // uvMax.x (i_data2.x)
    cmd->scrollY = atlasY + atlasH;   // uvMax.y (i_data2.y)

    cmd->radiusTL = radiusTL; cmd->radiusTR = radiusTR;
    cmd->radiusBR = radiusBR; cmd->radiusBL = radiusBL;
    cmd->cornerPattern = cornerPattern;
    cmd->backUVMinX = backUVMinX;
    cmd->aa = aaPixels;
    cmd->shadowX = shadowX; cmd->shadowY = shadowY;
    cmd->shadowBlur = shadowBlur;
    cmd->borderTop = borderTop;
    cmd->borderRight = borderRight;
    cmd->borderBottom = borderBottom;
    cmd->borderLeft = borderLeft;
    cmd->shadowColor = shadowColor;   // i_data4.x
    cmd->fillColor = modulate;        // i_data4.y
    cmd->borderColor = borderColor;   // i_data4.z
    cmd->zIndex = zIndex;
    cmd->texture = tex1;
    cmd->texture2 = tex2 ? tex2 : &nulltex;
    cmd->texture3 = tex3 ? tex3 : &nulltex;
    cmd->texture4 = tex4 ? tex4 : &nulltex;
    cmd->fbsize = fbsize;
    push(cmd);
}

// ============================================================
// RawImage (純粋テクスチャ描画、SDF/影/ボーダーなし)
// ============================================================

void LayerInfo::pushRawImage(float x, float y, float width, float height,
    float atlasX, float atlasY, float atlasW, float atlasH,
    uint32_t modulate, float zIndex, bgfx::TextureHandle* tex1,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
    float blendMode)
{
    UnifiedDrawCommand* cmd = new UnifiedDrawCommand();
    cmd->type = DrawCommandType::RawImage;
    cmd->blendMode = blendMode;
    cmd->targetFBO = targetFBO;
    cmd->viewId = viewId;
    cmd->x = x; cmd->y = y;
    cmd->width = width; cmd->height = height;
    cmd->dataOffset = 0; cmd->colorCount = 0;
    cmd->angle = atlasX;              // uvMin.x
    cmd->curlRadius = atlasY;         // uvMin.y
    cmd->scrollX = atlasX + atlasW;   // uvMax.x
    cmd->scrollY = atlasY + atlasH;   // uvMax.y
    cmd->radius = 0; cmd->aa = 0;
    cmd->shadowX = 0; cmd->shadowY = 0;
    cmd->shadowBlur = 0;
    cmd->shadowColor = 0;
    cmd->fillColor = modulate;
    cmd->borderColor = 0;
    cmd->zIndex = zIndex;
    cmd->texture = tex1;
    cmd->texture2 = &nulltex;
    cmd->fbsize = fbsize;
    push(cmd);
}

// ============================================================
// Pattern
// ============================================================

void LayerInfo::pushPattern(enum DrawCommandType patternMode,
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
    float blendMode,
    float cornerPattern)
{
    UnifiedDrawCommand* cmd = new UnifiedDrawCommand();
    cmd->type = patternMode;
    cmd->blendMode = blendMode;
    cmd->targetFBO = targetFBO;
    cmd->viewId = viewId;
    cmd->x = x; cmd->y = y;
    cmd->width = width; cmd->height = height;
    cmd->colorCount = colorCount;
    cmd->angle = angle;
    cmd->scrollX = scrollX; cmd->scrollY = scrollY;
    cmd->radiusTL = radiusTL; cmd->radiusTR = radiusTR;
    cmd->radiusBR = radiusBR; cmd->radiusBL = radiusBL;
    cmd->cornerPattern = cornerPattern;
    cmd->aa = aaPixels;
    cmd->shadowX = shadowX; cmd->shadowY = shadowY;
    cmd->shadowBlur = shadowBlur;
    cmd->borderTop = borderTop;
    cmd->borderRight = borderRight;
    cmd->borderBottom = borderBottom;
    cmd->borderLeft = borderLeft;
    cmd->shadowColor = shadowColor;   // i_data4.x
    cmd->fillColor = 0xFFFFFFFF;      // i_data4.y (白)
    cmd->borderColor = borderColor;   // i_data4.z
    cmd->dataOffset = static_cast<float>(dataOffset);
    cmd->zIndex = zIndex;
    cmd->fbsize = fbsize;
    cmd->texture = &nulltex;
    cmd->texture2 = &nulltex;
    push(cmd);
}

// ============================================================
// Text (Imageと同じレイアウト)
// ============================================================

void LayerInfo::pushText(float x, float y, float width, float height,
    float atlasX, float atlasY, float atlasW, float atlasH,
    uint32_t color, float zIndex, bgfx::TextureHandle* tex1,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
    float blendMode)
{
    UnifiedDrawCommand* cmd = new UnifiedDrawCommand();
    cmd->type = DrawCommandType::RawImage;
    cmd->blendMode = blendMode;
    cmd->targetFBO = targetFBO;
    cmd->viewId = viewId;
    cmd->x = x; cmd->y = y;
    cmd->width = width; cmd->height = height;
    cmd->dataOffset = 0; cmd->colorCount = 0;
    // 新レイアウト: float精度でUV座標を渡す
    cmd->angle = atlasX;              // uvMin.x (i_data1.z)
    cmd->curlRadius = atlasY;         // uvMin.y (i_data1.w に転送)
    cmd->scrollX = atlasX + atlasW;   // uvMax.x (i_data2.x)
    cmd->scrollY = atlasY + atlasH;   // uvMax.y (i_data2.y)
    cmd->borderTop = 0;
    cmd->borderRight = 0;
    cmd->borderBottom = 0;
    cmd->borderLeft = 0;
    cmd->radiusTL = 0; cmd->radiusTR = 0;
    cmd->radiusBL = 0; cmd->radiusBR = 0;
    cmd->radius = 0;
    cmd->aa = 0.0f;
    cmd->shadowX = 0;
    cmd->shadowY = 0;
    cmd->shadowBlur = 0;
    cmd->cornerPattern = 0;
    cmd->shadowColor = 0;             // i_data4.x
    cmd->fillColor = color;           // i_data4.y
    cmd->borderColor = 0;             // i_data4.z
    cmd->zIndex = zIndex;
    cmd->texture = tex1;
    cmd->texture2 = &nulltex;
    cmd->fbsize = fbsize;
    push(cmd);
}
// ============================================================
// Background
// ============================================================

void LayerInfo::pushBackground(Background* back, float x, float y, float width, float height,
    float zIndex, bgfx::TextureHandle* tex1, bgfx::TextureHandle* tex2,
    bgfx::TextureHandle* tex3, bgfx::TextureHandle* tex4,
    bgfx::FrameBufferHandle* targetFBO, PointI* fbsize, uint64_t viewId,
    float blendMode, const float* boWidthes, BorderShadow* bs)
{
    UnifiedDrawCommand* cmd = new UnifiedDrawCommand();
    cmd->type = back->type;
    cmd->blendMode = blendMode;
    cmd->targetFBO = targetFBO;
    cmd->viewId = viewId;
    // ジグソータブ突起分の描画領域拡張
    float tabR = back->backUVMinX;
    int tabPat = (int)back->cornerPattern;
    float tabExt = tabR * 1.5f;  // offset(R/2) + radius(R)
    if (tabR > 0.0f && tabPat >= 9 && tabPat <= 12) {
        if (tabPat == 9)       { y -= tabExt; height += tabExt; }  // 上辺タブ
        else if (tabPat == 10) { width += tabExt; }                // 右辺タブ
        else if (tabPat == 11) { height += tabExt; }               // 下辺タブ
        else                   { x -= tabExt; width += tabExt; }   // 左辺タブ
    }
    cmd->x = x; cmd->y = y;
    cmd->width = width; cmd->height = height;
    cmd->dataOffset = back->offset; cmd->colorCount = back->count;
    // 新レイアウト: float精度でUV座標を渡す
    cmd->angle = back->angle;              // uvMin.x (i_data1.z)
    cmd->curlRadius = back->curl;         // Pattern: uvMin.y (i_data1.w)
    cmd->backUVMinX = back->backUVMinX;   // Fill/Pattern: data1[1] (タブ半径等)
    cmd->backUVMinY = back->backUVMinY;
    cmd->scrollX = back->scrollX;   // uvMax.x (i_data2.x)
    cmd->scrollY = back->scrollY;   // uvMax.y (i_data2.y)

    cmd->radiusTL = bs->boRadiuses[0]; cmd->radiusTR = bs->boRadiuses[1];
    cmd->radiusBR = bs->boRadiuses[2]; cmd->radiusBL = bs->boRadiuses[3];
    cmd->cornerPattern = back->cornerPattern;
    cmd->aa = back->aa;
    cmd->shadowX = bs->shadowX; cmd->shadowY = bs->shadowY;
    cmd->shadowBlur = bs->shadowBlur;
    cmd->borderTop = boWidthes[0];
    cmd->borderRight = boWidthes[1];
    cmd->borderBottom = boWidthes[2];
    cmd->borderLeft = boWidthes[3];
    cmd->shadowColor = bs->shadowColor;             // i_data4.x
    cmd->fillColor = back->fillcolor;           // i_data4.y
    cmd->borderColor = bs->borderColor;             // i_data4.z
    cmd->zIndex = zIndex;
    cmd->texture = tex1;
    cmd->texture2 = tex2;
    cmd->texture3 = tex3;
    cmd->texture4 = tex4;
    cmd->fbsize = fbsize;
    push(cmd);
}
// ============================================================
// ユーティリティ
// ============================================================
LayerInfo& LayerInfo::setUniform(const std::string& name, const std::vector<float>& data) {
    defaultUniforms[name] = data;
    return *this;
}

LayerInfo& LayerInfo::setTexture(const std::string& uniformName, bgfx::TextureHandle texture) {
    defaultTextures[uniformName] = texture;
    return *this;
}

void LayerInfo::pushFBO(NewElement* elem, int fboWidth, int fboHeight,
    bgfx::TextureHandle* texture, bgfx::FrameBufferHandle* fbo, PointI* fbsize, bool resized) {
    FBODrawCommand* cmd = new FBODrawCommand();
    cmd->type = DrawCommandType::FBO;
    cmd->elem = elem;
    cmd->fboWidth = fboWidth;
    cmd->fboHeight = fboHeight;
    cmd->texture = texture;
    cmd->targetFBO = NULL;
    cmd->fbo = fbo;
    cmd->resized = resized;
    cmd->zIndex = -100000.0f;
    cmd->fbsize = fbsize;
    cmds.push_back((DrawCommand*)cmd);
}
void LayerInfo::pushDelFBO(bgfx::FrameBufferHandle fbo) {
    DelFBODrawCommand* cmd = new DelFBODrawCommand();
    cmd->type = DrawCommandType::DelFBO;
    cmd->fbo = fbo;
    cmd->zIndex = -100000.0f;
    cmds.push_back((DrawCommand*)cmd);
}
void LayerInfo::clear() {
    cmds.clear();
    // cmds が消えれば scissor pointer の参照元も無くなるので pool もリセット。
    scissorPool.clear();
}
// ============================================================
// グローバルリソース（実際のプロジェクトでは適切なクラスで管理）
// ============================================================



static RenderResources g_resources;

// ============================================================
// Pattern用アトラスビルダー（フレームごとに再構築）
// ============================================================

class PatternAtlasBuilder {
public:
    struct PatternEntry {
        int offset;
        int colorCount;
    };

    // 色パターンのキー（色データ + 幅データのハッシュ）
    struct PatternKey {
        std::vector<float> colors;  // RGBA × colorCount
        std::vector<float> widths;  // colorCount個

        bool operator==(const PatternKey& other) const {
            return colors == other.colors && widths == other.widths;
        }
    };

    struct PatternKeyHash {
        size_t operator()(const PatternKey& key) const {
            size_t hash = 0;
            for (float v : key.colors) {
                hash ^= std::hash<float>{}(v)+0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            for (float v : key.widths) {
                hash ^= std::hash<float>{}(v)+0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };
    mutable std::mutex m;

private:
    std::vector<float> paletteData_;
    std::vector<float> widthsData_;
    int currentOffset_ = 0;

    bgfx::TextureHandle paletteTexture_ = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle widthsTexture_ = BGFX_INVALID_HANDLE;

    // 色パターンをキーにしたキャッシュ
    std::unordered_map<PatternKey, PatternEntry, PatternKeyHash> patternCache_;
    bool dirty_ = false;

public:
    void reset() {
    }

    // 色を0xRRGGBBAA形式の整数で指定して追加（内部で0.0〜1.0に変換）
    int addPattern(const std::vector<uint32_t>& colors32, const std::vector<float>& widths) {
        std::vector<float> colors(colors32.size() * 4);
        for (size_t i = 0; i < colors32.size(); i++) {
            uint32_t c = colors32[i];
            colors[i * 4 + 0] = static_cast<float>((c >> 24) & 0xFF) / 255.0f;
            colors[i * 4 + 1] = static_cast<float>((c >> 16) & 0xFF) / 255.0f;
            colors[i * 4 + 2] = static_cast<float>((c >> 8) & 0xFF) / 255.0f;
            colors[i * 4 + 3] = static_cast<float>(c & 0xFF) / 255.0f;
        }
        return addPattern(colors, widths);
    }

    // 色パターンを直接指定して追加
    int addPattern(const std::vector<float>& colors, const std::vector<float>& widths) {
        std::lock_guard lock(m);
        int colorCount = static_cast<int>(widths.size());

        // 入力検証
        if (colors.size() != colorCount * 4) {
            SDL_Log("PatternAtlasBuilder: colors.size() must be widths.size() * 4");
            return -1;
        }

        // キーを作成
        PatternKey key;
        key.colors = colors;
        key.widths = widths;

        // キャッシュにあれば再利用
        auto it = patternCache_.find(key);
        if (it != patternCache_.end()) {
            return it->second.offset;
        }

        // 新規追加
        int newOffset = currentOffset_;

        paletteData_.insert(paletteData_.end(), colors.begin(), colors.end());
        widthsData_.insert(widthsData_.end(), widths.begin(), widths.end());

        currentOffset_ += colorCount;

        PatternEntry entry = { newOffset, colorCount };
        patternCache_[key] = entry;
        dirty_ = true;

        return newOffset;
    }


    // パターンが既に存在するか確認
    int findPattern(const std::vector<float>& colors, const std::vector<float>& widths) const {
        PatternKey key;
        key.colors = colors;
        key.widths = widths;

        auto it = patternCache_.find(key);
        if (it != patternCache_.end()) {
            return it->second.offset;
        }
        return -1;
    }

    // パターン情報を取得
    bool getPatternInfo(int offset, int& outColorCount) const {
        for (const auto& [key, entry] : patternCache_) {
            if (entry.offset == offset) {
                outColorCount = entry.colorCount;
                return true;
            }
        }
        return false;
    }

    void buildTextures() {
        if (!dirty_) return;
        dirty_ = false;

        if (bgfx::isValid(paletteTexture_)) {
            bgfx::destroy(paletteTexture_);
            paletteTexture_ = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(widthsTexture_)) {
            bgfx::destroy(widthsTexture_);
            widthsTexture_ = BGFX_INVALID_HANDLE;
        }

        if (currentOffset_ == 0) return;

        const int ATLAS_SIZE = 1024;  // 固定サイズ

        // パレットテクスチャ（1024幅で作成）
        std::vector<uint8_t> paletteBytes(ATLAS_SIZE * 4, 0);  // 0で初期化
        for (size_t i = 0; i < paletteData_.size(); ++i) {
            paletteBytes[i] = static_cast<uint8_t>(paletteData_[i] * 255.0f);
        }

        const bgfx::Memory* paletteMem = bgfx::copy(
            paletteBytes.data(),
            static_cast<uint32_t>(paletteBytes.size()));
        paletteTexture_ = bgfx::createTexture2D(
            ATLAS_SIZE, 1, false, 1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
            paletteMem
        );

        // 幅テクスチャ（1024幅で作成）
        // パターン境界のオフセット一覧を作成
        std::vector<int> boundaries;
        for (const auto& [key, entry] : patternCache_) {
            boundaries.push_back(entry.offset);
        }
        std::sort(boundaries.begin(), boundaries.end());
        boundaries.push_back(currentOffset_); // 番兵

        std::vector<float> widthsBytes(ATLAS_SIZE, 0.0f);
        float cumulative = 0.0f;
        size_t nextBoundary = 1; // boundaries[0]は最初のパターンの開始
        for (size_t i = 0; i < widthsData_.size(); ++i) {
            if (nextBoundary < boundaries.size() && (int)i == boundaries[nextBoundary]) {
                cumulative = 0.0f;
                nextBoundary++;
            }
            cumulative += widthsData_[i];
            widthsBytes[i] = cumulative;
        }

        const bgfx::Memory* widthsMem = bgfx::copy(
            widthsBytes.data(),
            static_cast<uint32_t>(ATLAS_SIZE * sizeof(float)));
        widthsTexture_ = bgfx::createTexture2D(
            ATLAS_SIZE, 1, false, 1,
            bgfx::TextureFormat::R32F,
            BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT, // POINTサンプリング
            widthsMem
        );
    }
    bgfx::TextureHandle getPaletteTexture() const { return paletteTexture_; }
    bgfx::TextureHandle getWidthsTexture() const { return widthsTexture_; }
    bool hasData() const { return currentOffset_ > 0; }
};

// ============================================================
// シェーダータイプのカテゴリ判定
// ============================================================

// ============================================================
// 描画バッチ
// ============================================================

struct DrawBatch {
    bgfx::TextureHandle *texture = NULL;
    bgfx::TextureHandle* texture2 = NULL;
    bgfx::TextureHandle* texture3 = NULL;
    bgfx::TextureHandle* texture4 = NULL;
    // scissor は per-submit (bgfx::setScissor) で設定する。view 単位ではなく
    // batch 単位 (= 1 submit = 1 scissor) なのでここに持つ。
    const ScissorRect* scissor = nullptr;
    float blendMode = 0.0f;
    float cornerPattern = 0.0f;
    float dctype = 0.0f;
    float minZIndex = 0;
    std::vector<DrawCommand*> commands;
};

// ============================================================
// レンダリングパス
// ============================================================

struct RenderPass {
    bgfx::FrameBufferHandle* fbo = nullptr;
    PointI* fbsize = nullptr;
    // タイル時の情報 (tileIndex >= 0 なら submit 時に master からルックアップする)
    ImageId tiledImageId = 0;
    int     tileIndex    = -1;
    uint64_t viewId = 0;  // 論理viewId
    float minZIndex = 0;
    float maxZIndex = 0;
    // 1 pass = 1 bgfx view。FBO/viewport/projection 単位で分割。texture/blend/scissor の
    // 違いは batch 単位で per-submit 設定する。
    std::vector<DrawBatch> batches;
};

// ============================================================
// レンダリングパスキー
// ============================================================

// pass key: view (= FBO/viewport/projection/zIndexBucket) 単位の grouping。
// 整数 bucket ごとに別 view = depth clear 単位、で旧設計の bucket 間 layering を保つ。
// scissor / texture / blend は batch (per-submit) で分けるので含めない。
struct RenderPassKey {
    bgfx::FrameBufferHandle* fbo;
    PointI* fbsize;
    ImageId tiledImageId;
    int     tileIndex;
    uint64_t viewId;       // 論理viewId
    int     zIndexBucket;  // floor(zIndex)、bucket ごとに別 view

    bool operator<(const RenderPassKey& other) const {
        if (viewId != other.viewId) return viewId < other.viewId;
        if (zIndexBucket != other.zIndexBucket) return zIndexBucket < other.zIndexBucket;
        if (fbo != other.fbo) return fbo < other.fbo;
        if (tiledImageId != other.tiledImageId) return tiledImageId < other.tiledImageId;
        return tileIndex < other.tileIndex;
    }
};

// batch key: 同一 view 内で per-submit (scissor / texture / blend / dctype) 単位の grouping。
struct RenderBatchKey {
    bgfx::TextureHandle* texture;
    bgfx::TextureHandle* texture2;
    bgfx::TextureHandle* texture3;
    bgfx::TextureHandle* texture4;
    float blendMode;
    float cornerPattern;
    float dctype;
    const ScissorRect* scissor;

    bool operator<(const RenderBatchKey& other) const {
        if (dctype != other.dctype) return dctype < other.dctype;
        if (blendMode != other.blendMode) return blendMode < other.blendMode;
        if (cornerPattern != other.cornerPattern) return cornerPattern < other.cornerPattern;
        if (texture != other.texture) return texture < other.texture;
        if (texture2 != other.texture2) return texture2 < other.texture2;
        if (texture3 != other.texture3) return texture3 < other.texture3;
        if (texture4 != other.texture4) return texture4 < other.texture4;
        return scissor < other.scissor;
    }
};

// ============================================================
// コマンドグループ化とアトラスビルド
// ============================================================

struct GroupedCommands {
    std::vector<RenderPass> passes;
    PatternAtlasBuilder* patternAtlas;
};

GroupedCommands groupDrawCommands(
    std::vector<DrawCommand*>& commands,
    PatternAtlasBuilder& sourcePatternAtlas)
{
    GroupedCommands result;
    result.patternAtlas = &sourcePatternAtlas;

    // zIndexでソートし、同一zIndexの後続コマンドを微小に増やす
    std::stable_sort(commands.begin(), commands.end(),
        [](DrawCommand* a, DrawCommand* b) { return a->zIndex < b->zIndex; });
    for (size_t i = 0; i < commands.size(); ) {
        float curZ = commands[i]->zIndex;
        // 同じzIndexが続く範囲の末尾を探す
        size_t j = i + 1;
        while (j < commands.size() && commands[j]->zIndex == curZ) ++j;
        if (j - i > 1) {
            // 次の異なるzIndexを探す
            float nextZ = (j < commands.size()) ? commands[j]->zIndex : curZ + 1.0f;
            float gap = nextZ - curZ;
            if (gap > 1.0f) gap = 1.0f;
            float step = gap / (float)(j - i);
            for (size_t k = i + 1; k < j; ++k) {
                commands[k]->zIndex = curZ + step * (k - i);
            }
        }
        i = j;
    }

    // 二段グループ化:
    //   1) pass = (viewId, fbo, tiledImageId, tileIndex)  ← bgfx view 単位
    //   2) batch = (texture, blend, scissor, zIndexBucket, ...)  ← per-submit 単位
    std::map<RenderPassKey, std::map<RenderBatchKey, std::vector<DrawCommand*>>> passGroups;

    for (auto* cmd : commands) {
        RenderPassKey passKey;
        passKey.fbo = cmd->targetFBO;
        passKey.fbsize = cmd->fbsize;
        passKey.tiledImageId = cmd->tiledImageId;
        passKey.tileIndex    = cmd->tileIndex;
        passKey.viewId = cmd->viewId;
        passKey.zIndexBucket = static_cast<int>(cmd->zIndex);  // bucket ごとに別 view

        RenderBatchKey batchKey;
        batchKey.texture = cmd->texture;
        batchKey.texture2 = cmd->texture2;
        batchKey.texture3 = cmd->texture3;
        batchKey.texture4 = cmd->texture4;
        batchKey.blendMode = cmd->blendMode;
        batchKey.cornerPattern = cmd->cornerPattern;
        batchKey.dctype = cmd->type;
        batchKey.scissor = cmd->scissor;

        passGroups[passKey][batchKey].push_back(cmd);
    }

    // パターンテクスチャをビルド
    if (result.patternAtlas->hasData()) {
        result.patternAtlas->buildTextures();
    }

    // RenderPass 配列に変換 (1 pass = 1 bucket 1 view)
    for (auto& [passKey, batchGroups] : passGroups) {
        RenderPass pass;
        pass.fbo = passKey.fbo;
        pass.viewId = passKey.viewId;
        pass.fbsize = passKey.fbsize;
        pass.tiledImageId = passKey.tiledImageId;
        pass.tileIndex    = passKey.tileIndex;
        pass.minZIndex = (float)passKey.zIndexBucket;
        pass.maxZIndex = (float)passKey.zIndexBucket;

        for (auto& [batchKey, cmds] : batchGroups) {
            DrawBatch batch;
            batch.texture = batchKey.texture;
            batch.texture2 = batchKey.texture2;
            batch.texture3 = batchKey.texture3;
            batch.texture4 = batchKey.texture4;
            batch.blendMode = batchKey.blendMode;
            batch.cornerPattern = batchKey.cornerPattern;
            batch.dctype = batchKey.dctype;
            batch.minZIndex = (float)passKey.zIndexBucket;
            batch.scissor = batchKey.scissor;
            batch.commands = std::move(cmds);
            pass.batches.push_back(std::move(batch));
        }
        result.passes.push_back(std::move(pass));
    }

    // パス全体をソート (viewId → zIndexBucket 順)。
    // bgfx は viewId 昇順で render するので、低 bucket = 低 viewId 割当 = 先に描画 = 奥。
    std::sort(result.passes.begin(), result.passes.end(),
        [](const RenderPass& a, const RenderPass& b) {
            if (a.viewId != b.viewId) return a.viewId < b.viewId;
            return a.minZIndex < b.minZIndex;
        });

    return result;
}

// ============================================================
// コマンドベクタ抽出ヘルパー
// ============================================================

template<typename T>
std::vector<T> extractCommands(const std::vector<DrawCommand*>& ptrs) {
    std::vector<T> result;
    result.reserve(ptrs.size());
    for (auto* ptr : ptrs) {
        result.push_back(*static_cast<T*>(ptr));
    }
    return result;
}

auto frameStart = std::chrono::high_resolution_clock::now();
// ============================================================
// メイン描画関数
// ============================================================

void renderAllCommands(
    NewLocal* local,
    GroupedCommands& grouped,
    RenderResources& resources, ImageMaster& master)
{
    // 1 pass = 1 bgfx view、順番に消費する。
    uint8_t nextBgfxViewId = 0;
    bool firstView = true;

    for (auto& pass : grouped.passes) {
        // 1 pass = 1 bgfx view。FBO/viewport/projection が違うときのみ view を消費する。
        // scissor / texture / blend / dctype の違いは batch (per-submit) で吸収する。
        uint8_t bgfxId = nextBgfxViewId++;
        uint16_t clearFlags = BGFX_CLEAR_DEPTH;
        if (firstView) { clearFlags |= BGFX_CLEAR_COLOR; firstView = false; }

        // pass の FBO/サイズ解決
        bgfx::FrameBufferHandle resolvedFbo = BGFX_INVALID_HANDLE;
        int resolvedW = 0, resolvedH = 0;
        bool isTilePass = (pass.tileIndex >= 0);
        bool fboValid = false;
        if (isTilePass) {
            TiledTextureInfo* ti = shader_lookupTiledTexture(master, pass.tiledImageId);
            if (ti && pass.tileIndex < (int)ti->tiles.size()) {
                // lazy 確保: tile.fbo が INVALID なら今ここで bgfx リソースを生成。
                // (= createTiledOffscreenInternal が tile を pre-allocate しなくなったので、
                //    描画直前にこの pass で初めて作る形)
                shader_ensureTile(master, pass.tiledImageId, pass.tileIndex);
                auto& tile = ti->tiles[pass.tileIndex];
                if (bgfx::isValid(tile.fbo)) {
                    resolvedFbo = tile.fbo;
                    resolvedW = tile.size.x;
                    resolvedH = tile.size.y;
                    fboValid = true;
                }
            }
            if (!fboValid) continue;
        }

        if (isTilePass) {
            bgfx::setViewFrameBuffer(bgfxId, resolvedFbo);
            bgfx::setViewClear(bgfxId, clearFlags, 0x808080ff, 1.0f, 0);
            bgfx::setViewRect(bgfxId, 0, 0, resolvedW, resolvedH);
            float orthoProj[16];
            bx::mtxOrtho(orthoProj, 0.0f, (float)resolvedW, (float)resolvedH, 0.0f, 0.0f, 10000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(bgfxId, NULL, orthoProj);
        }
        else if (pass.fbo && bgfx::isValid(*pass.fbo)) {
            bgfx::setViewFrameBuffer(bgfxId, *pass.fbo);
            bgfx::setViewClear(bgfxId, clearFlags, 0x808080ff, 1.0f, 0);
            bgfx::setViewRect(bgfxId, 0, 0, pass.fbsize->x, pass.fbsize->y);
            float orthoProj[16];
            bx::mtxOrtho(orthoProj, 0.0f, pass.fbsize->x, pass.fbsize->y, 0.0f, 0.0f, 10000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(bgfxId, NULL, orthoProj);
        }
        else {
            bgfx::setViewFrameBuffer(bgfxId, BGFX_INVALID_HANDLE);
            bgfx::setViewClear(bgfxId, clearFlags, 0xa0a0a0ff, 1.0f, 0);
            bgfx::setViewRect(bgfxId, 0, 0, pass.fbsize->x, pass.fbsize->y);
            float orthoProj[16];
            bx::mtxOrtho(orthoProj, 0.0f, pass.fbsize->x, pass.fbsize->y, 0.0f, 0.0f, 10000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(bgfxId, NULL, orthoProj);
        }
        // view scissor は使わない (clear 状態 = no clip)
        bgfx::setViewScissor(bgfxId, 0, 0, 0, 0);

        // batch ごとに per-submit scissor + drawUnifiedBatch
        for (auto& batch : pass.batches) {
            if (batch.commands.empty()) continue;
            // per-submit scissor
            if (batch.scissor && batch.scissor->w > 0 && batch.scissor->h > 0) {
                int sx = (int)batch.scissor->x;
                int sy = (int)batch.scissor->y;
                int sw = (int)batch.scissor->w;
                int sh = (int)batch.scissor->h;
                if (sx < 0) { sw += sx; sx = 0; }
                if (sy < 0) { sh += sy; sy = 0; }
                if (sw < 0) sw = 0;
                if (sh < 0) sh = 0;
                bgfx::setScissor((uint16_t)sx, (uint16_t)sy, (uint16_t)sw, (uint16_t)sh);
            }
            // (scissor==NULL 時は per-submit setScissor を呼ばない → view default = no clip)
            bgfx::TextureHandle tex = BGFX_INVALID_HANDLE;
            bgfx::TextureHandle wid = BGFX_INVALID_HANDLE;
            if (DrawCommandType::Gradient <= batch.dctype && batch.dctype <= DrawCommandType::GradientChecker) {
                tex = grouped.patternAtlas->getPaletteTexture();
                wid = grouped.patternAtlas->getWidthsTexture();
            }
            drawUnifiedBatch(reinterpret_cast<std::vector<UnifiedDrawCommand*>&>(batch.commands), resources, tex, wid, bgfxId);
        }
    }
}

// ============================================================
// LayerInfo用の描画エントリポイント
// ============================================================

void renderLayer(NewLocal* local, LayerInfo& layer,
    RenderResources& resources, ImageMaster& master,
    PatternAtlasBuilder& patternAtlas)
{
    if (layer.cmds.empty()) return;

    // コマンドポインタのベクタを作成
    std::vector<DrawCommand*> cmdPtrs;
    cmdPtrs.reserve(layer.cmds.size());
    for (auto cmd : layer.cmds) {
        cmdPtrs.push_back(cmd);
    }

    // グループ化
    auto grouped = groupDrawCommands(cmdPtrs, patternAtlas);
    // 描画
    renderAllCommands(local, grouped, resources, master);
}

// ============================================================
// 複数レイヤーの描画
// ============================================================

void renderAllLayers(NewLocal* local, std::vector<LayerInfo>& layers,
    RenderResources& resources, ImageMaster& master,
    PatternAtlasBuilder& patternAtlas)
{
    // 全レイヤーのコマンドを収集
    std::vector<DrawCommand*> allCommands;

    for (auto& layer : layers) {
        if (!layer.visible) continue;

        for (auto cmd : layer.cmds) {
            allCommands.push_back(cmd);
        }
    }

    if (allCommands.empty()) return;
    // グループ化
    auto grouped = groupDrawCommands(allCommands, patternAtlas);

    // 描画
    renderAllCommands(local, grouped, resources, master);
    // scissorPool のクリアは LayerInfo::clear() (= cmds.clear() のタイミング) と一緒に
    // 行う。ここでは cmds が cleared されないので pool も触らない。
}

// ============================================================
// 初期化・終了処理
// ============================================================
// シェーダーバイナリ読み込み (PlatformIO unified)
#include "platform_io.h"

// Get platform suffix for shader files
// Windows: no suffix (DirectX), macOS/iOS: _mtl (Metal), Android/Linux: _spv (Vulkan/SPIRV)
inline const char* getShaderPlatformSuffix() {
#if defined(_WIN32)
    return "";  // Windows DirectX shaders have no suffix
#elif defined(__ANDROID__)
    return "_a";  // Android Vulkan shaders
#elif TARGET_OS_IOS || TARGET_OS_SIMULATOR
    return "_mtl";  // iOS uses Metal
#elif TARGET_OS_MAC
    return "_mtl";  // macOS uses Metal
#elif defined(__linux__)
    return "_spv";  // Linux uses Vulkan/SPIRV
#else
    return "";
#endif
}

// Load shader with automatic platform suffix
// e.g., loadShader("shaders/vs_unite2", location) loads "shaders/vs_unite2_w.bin" on Windows
bgfx::ShaderHandle loadShader(const char* baseName, HopStarIO::Location location) {
    // Build platform-specific filename: baseName + platform_suffix + .bin
    std::string filename = std::string(baseName) + getShaderPlatformSuffix() + ".bin";

    auto data = PlatformIO::readFile(filename, location);
    if (data.empty()) {
        SDL_Log("loadShader: Failed to load shader file: %s", filename.c_str());
        return BGFX_INVALID_HANDLE;
    }

    SDL_Log("loadShader: Loaded %s (%zu bytes)", filename.c_str(), data.size());
    const bgfx::Memory* mem = bgfx::copy(data.data(), static_cast<uint32_t>(data.size()));
    return bgfx::createShader(mem);
}

void initRenderResources(RenderResources& resources) {
    createUnitQuad(resources.quadVB, resources.quadIB);
    resources.paletteUniform = bgfx::createUniform("s_palette", bgfx::UniformType::Sampler);
    resources.widthsUniform = bgfx::createUniform("s_widths", bgfx::UniformType::Sampler);
    resources.tex2Uniform = bgfx::createUniform("s_tex2", bgfx::UniformType::Sampler);
    resources.tex3Uniform = bgfx::createUniform("s_tex3", bgfx::UniformType::Sampler);
    resources.param1Uniform = bgfx::createUniform("u_param1", bgfx::UniformType::Vec4);

    // Create 1x1 white placeholder texture (required for Metal - must have valid texture bindings)
    uint32_t whitePixel = 0xFFFFFFFF;
    resources.placeholderTexture = bgfx::createTexture2D(
        1, 1, false, 1, bgfx::TextureFormat::RGBA8,
        BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
        bgfx::copy(&whitePixel, sizeof(whitePixel)));

    // Load shaders with automatic platform suffix
    // Windows: vs_unite2.bin, macOS/iOS: shaders/vs_unite2_mtl.bin, Linux/Android: shaders/vs_unite2_spv.bin
#if defined(_WIN32)
    bgfx::ShaderHandle vsu = loadShader("shaders/vs_unite2", HopStarIO::Location::Resource);
    bgfx::ShaderHandle fsu = loadShader("shaders/fs_unite2", HopStarIO::Location::Resource);
#else
    bgfx::ShaderHandle vsu = loadShader("shaders/vs_unite2", HopStarIO::Location::Resource);
    bgfx::ShaderHandle fsu = loadShader("shaders/fs_unite2", HopStarIO::Location::Resource);
#endif
    resources.uniteProgram = bgfx::createProgram(vsu, fsu, true);
    // シェーダープログラムの登録は外部で行う
    // resources.programs[DrawCommandType::Rectangle] = loadProgram(...);
}

void destroyRenderResources(RenderResources& resources) {
    if (bgfx::isValid(resources.quadVB)) bgfx::destroy(resources.quadVB);
    if (bgfx::isValid(resources.quadIB)) bgfx::destroy(resources.quadIB);
    if (bgfx::isValid(resources.paletteUniform)) bgfx::destroy(resources.paletteUniform);
    if (bgfx::isValid(resources.widthsUniform)) bgfx::destroy(resources.widthsUniform);
    if (bgfx::isValid(resources.tex2Uniform)) bgfx::destroy(resources.tex2Uniform);
    if (bgfx::isValid(resources.tex3Uniform)) bgfx::destroy(resources.tex3Uniform);
    if (bgfx::isValid(resources.param1Uniform)) bgfx::destroy(resources.param1Uniform);
    if (bgfx::isValid(resources.uniteProgram)) bgfx::destroy(resources.uniteProgram);
    if (bgfx::isValid(resources.pageProgram)) bgfx::destroy(resources.pageProgram);
    if (bgfx::isValid(resources.placeholderTexture)) bgfx::destroy(resources.placeholderTexture);
}