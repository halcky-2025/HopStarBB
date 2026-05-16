#pragma once

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#include <algorithm>
#include "platform_io.h"

#ifdef __ANDROID__
#include <SDL3/SDL.h>
#endif

//=============================================================================
// ImageId
//=============================================================================



//=============================================================================
// ResolveStatus
//=============================================================================


enum class ResolveStatus {
    Success,        // 成功した
    Pending,        // ロード中（placeholder使用）
    NotFound,       // 見つからない
    Evicted,        // eviction済み（リロード可能性）
};

struct ResolveResult {
    ResolveStatus status = ResolveStatus::NotFound;
    ResolvedTexture resolved;
    ResolvedTexture placeholder;  // Pending時のフォールバック

    bool isReady() const { return status == ResolveStatus::Success; }

    // 描画用（成功ならresolved、Pendingならplaceholder）
    const ResolvedTexture& forDraw() const {
        return (status == ResolveStatus::Success) ? resolved : placeholder;
    }
};

//=============================================================================
// ImageLocation - ImageIdの配置情報
//=============================================================================

struct ImageLocation {
    enum class Type : uint8_t {
        None,
        Standalone,        // standaloneTextures_ に存在
        TiledStandalone,   // tiledTextures_ に存在（巨大画像タイル分割）
        FontAtlas,         // FontAtlas に存在
        GridAtlas,         // ThumbnailGridAtlas に存在
        ShelfAtlas,        // ThumbnailShelfAtlas に存在
    };

    Type type = Type::None;

    // ★全 placement で共通の stable storage。
    // - cmd push は &slot.handle / &slot.fbo / &slot.size のアドレスを捕まえる
    // - GoThread (loadSync) で entry を作った瞬間からアドレスは不変
    // - render thread が upload 完了時に slot.handle 等を埋める
    //   ・Standalone: createStandaloneInternal が atlas-less な実 handle を入れる
    //   ・GridAtlas:  uploadToGpuInternal が atlas page texture を入れる
    //   ・ShelfAtlas: 同上
    // - eviction / page GC で texture が動いた場合も、render thread が
    //   slot.handle を新しい値に書き換えるだけ。cmd 側ポインタは不変。
    TextureSlot slot;

    // 合計コンテンツ寸法は uint16_t を超え得るので int 化 (= ugui.h 67600px Y で 2064 truncate 防止)。
    int width = 0;
    int height = 0;
    bool isPending = false;

    // 各Atlas用の内部キー (placement 固有メタ。union で省サイズ)
    union {
        struct { uint64_t glyphKey; } font;           // FontId + codepoint
        struct { ThumbnailHandle handle; } thumbnail; // Grid/Shelf
    };

    ImageLocation() : font{ 0 } {}
};

//=============================================================================
// RenderGroup - 基本グループ
//=============================================================================


//=============================================================================
// ExtendedRenderGroup - ImageMaster 用拡張グループ
//=============================================================================
struct ExtendedRenderGroup : RenderGroup {
    std::unordered_set<uint64_t> usedGridThumbnails;
    std::unordered_set<uint64_t> usedShelfThumbnails;
    std::unordered_set<ImageId> usedStandaloneTextures;

    void trackGridThumbnail(uint64_t id) { usedGridThumbnails.insert(id); }
    void trackShelfThumbnail(uint64_t id) { usedShelfThumbnails.insert(id); }
    void trackStandaloneTexture(ImageId id) { usedStandaloneTextures.insert(id); }

    void clearAll() {
        clear();
        usedGridThumbnails.clear();
        usedShelfThumbnails.clear();
        usedStandaloneTextures.clear();
    }
};

//=============================================================================
// ImageMasterGroupManager - 前方宣言
//=============================================================================
class ImageMaster;  // 前方宣言

class ImageMasterGroupManager {
public:
    explicit ImageMasterGroupManager(ImageMaster& master) : master_(master) {}

    ExtendedRenderGroup& createGroup(SurfaceId surfaceId,
        RenderGroupFlags flags = RenderGroupFlags::None) {
        RenderGroupId id = nextId_++;
        auto& g = groups_[id];
        g.id = id;
        g.surfaceId = surfaceId;
        g.flags = flags;
        g.bornFrame = currentFrame_;
        g.lastUsedFrame = currentFrame_;
        surfaceGroups_[surfaceId].push_back(id);
        return g;
    }

    ExtendedRenderGroup* getGroup(RenderGroupId id) {
        auto it = groups_.find(id);
        return (it != groups_.end()) ? &it->second : nullptr;
    }

    void beginFrame() { ++currentFrame_; }
    uint32_t currentFrame() const { return currentFrame_; }

    size_t groupCount() const { return groups_.size(); }
    size_t surfaceCount() const { return surfaceGroups_.size(); }

private:
    friend class ImageMaster;

    ImageMaster& master_;
    RenderGroupId nextId_ = 1;
    uint32_t currentFrame_ = 0;
    std::unordered_map<RenderGroupId, ExtendedRenderGroup> groups_;
    std::unordered_map<SurfaceId, std::vector<RenderGroupId>> surfaceGroups_;
};

//=============================================================================
// ImageMaster
//=============================================================================

class ImageMaster {
public:
    struct Config {
        FontAtlas::Config fontConfig = {};
        ThumbnailGridAtlasConfig gridConfig = {};
        ThumbnailShelfAtlasConfig shelfConfig = {};
        uint32_t standaloneExpireFrames = 600;
        uint32_t groupExpireFrames = 300;
    };

    ImageMaster()
        : config_()
        , fontAtlas_(config_.fontConfig)
        , gridAtlas_(config_.gridConfig)
        , shelfAtlas_(config_.shelfConfig)
        , groupManager_(*this) {
    }
    explicit ImageMaster(const Config& cfg)
        : config_(cfg)
        , fontAtlas_(cfg.fontConfig)
        , gridAtlas_(cfg.gridConfig)
        , shelfAtlas_(cfg.shelfConfig)
        , groupManager_(*this) {
    }

    ~ImageMaster() { shutdown(); }

    bool initialize() {
        fontAtlas_.init();
        if (!gridAtlas_.initialize()) return false;
        if (!shelfAtlas_.initialize()) return false;
        // GPU最大テクスチャサイズを取得
        const bgfx::Caps* caps = bgfx::getCaps();
        if (caps) {
            maxTileSize_ = std::min((uint16_t)caps->limits.maxTextureSize, (uint16_t)8192);
        }
        return true;
    }

    void shutdown() {
        // Standalone textures 解放
        {
            std::lock_guard lock(standaloneMutex_);
            for (auto& [id, info] : standaloneTextures_) {
                if (bgfx::isValid(info.fbo)) {
                    bgfx::destroy(info.fbo);
                }
                else if (bgfx::isValid(info.handle)) {
                    bgfx::destroy(info.handle);
                }
            }
            standaloneTextures_.clear();
        }

        // Tiled textures 解放
        {
            std::lock_guard lock(tiledMutex_);
            for (auto& [id, info] : tiledTextures_) {
                for (auto& tile : info.tiles) {
                    if (bgfx::isValid(tile.fbo)) {
                        bgfx::destroy(tile.fbo);
                    }
                    else if (bgfx::isValid(tile.handle)) {
                        bgfx::destroy(tile.handle);
                    }
                }
            }
            tiledTextures_.clear();
        }

        // 配置情報クリア
        {
            std::lock_guard lock(locationMutex_);
            imageLocations_.clear();
        }

        gridAtlas_.shutdown();
        shelfAtlas_.shutdown();
    }

    void beginFrame() {
        ++currentFrame_;
        // ★ GoThread が createNewPage で作った未 init の atlas page に対し、
        //   render thread 上で bgfx::createTexture2D を実行 (= placeholder slot を
        //   実 GPU テクスチャに昇格)。これより前の段階では page->m_texture は
        //   INVALID。imageLocations_[id].slot.handle (cmd 側の捕まえポインタ) は
        //   registerGridImage / registerShelfImage 時に atlas page texture を
        //   コピーするので、ensure → register の順で実 handle 値が伝播する。
        gridAtlas_.ensureAllBgfxTextures();
        shelfAtlas_.ensureAllBgfxTextures();
        fontAtlas_.beginFrame();
        gridAtlas_.beginFrame();
        shelfAtlas_.beginFrame();
        groupManager_.beginFrame();
        processOffscreenQueue();
    }

    void collectGarbage() {
        // 期限切れグループの解放
        collectExpiredGroups(config_.groupExpireFrames);

        // FontAtlas GC
        fontAtlas_.collectGarbage();

        // ThumbnailAtlas は内部LRUで管理
        gridAtlas_.evictLRU(16);
        shelfAtlas_.evictLRU(16);

        // Standalone texture GC
        collectStandaloneGarbage();

        // Tiled texture GC
        collectTiledGarbage();
    }

    void collectStandaloneGarbage() {
        std::lock_guard lock(standaloneMutex_);

        std::vector<ImageId> toRemove;
        for (auto& [id, info] : standaloneTextures_) {
            if (info.persistent || info.refCount > 0) continue;

            if (currentFrame_ - info.lastUsedFrame > config_.standaloneExpireFrames) {
                toRemove.push_back(id);
            }
        }

        for (ImageId id : toRemove) {
            auto it = standaloneTextures_.find(id);
            if (it != standaloneTextures_.end()) {
                if (bgfx::isValid(it->second.fbo)) {
                    bgfx::destroy(it->second.fbo);
                }
                else if (bgfx::isValid(it->second.handle)) {
                    bgfx::destroy(it->second.handle);
                }
                standaloneTextures_.erase(it);

                // 配置情報を削除
                std::lock_guard locLock(locationMutex_);
                imageLocations_.erase(id);
            }
        }
    }

    void collectTiledGarbage() {
        std::lock_guard lock(tiledMutex_);

        std::vector<ImageId> toRemove;
        for (auto& [id, info] : tiledTextures_) {
            if (info.persistent || info.refCount > 0) continue;

            if (currentFrame_ - info.lastUsedFrame > config_.standaloneExpireFrames) {
                toRemove.push_back(id);
            }
        }

        for (ImageId id : toRemove) {
            auto it = tiledTextures_.find(id);
            if (it != tiledTextures_.end()) {
                for (auto& tile : it->second.tiles) {
                    if (bgfx::isValid(tile.fbo)) {
                        bgfx::destroy(tile.fbo);
                    }
                    else if (bgfx::isValid(tile.handle)) {
                        bgfx::destroy(tile.handle);
                    }
                }
                tiledTextures_.erase(it);

                // 配置情報を削除
                std::lock_guard locLock(locationMutex_);
                imageLocations_.erase(id);
            }
        }
    }

    uint64_t currentFrame() const { return currentFrame_; }

    //=========================================================================
    // 統計情報
    //=========================================================================
    struct Stats {
        // FontAtlas
        size_t fontPageCount;
        size_t glyphCount;
        size_t imageCount;
        float fontAvgUtilization;

        // ThumbnailGridAtlas
        uint32_t gridPageCount;
        uint32_t gridUsedRects;
        uint32_t gridPendingUploads;

        // ThumbnailShelfAtlas
        uint32_t shelfPageCount;
        uint32_t shelfUsedShelves;
        uint32_t shelfPendingUploads;

        // Standalone Textures
        size_t standaloneCount;
        size_t standalonePersistentCount;
        uint64_t standaloneTotalBytes;

        // Tiled Textures
        size_t tiledCount;
        size_t tiledTotalTiles;
        uint64_t tiledTotalBytes;

        // ImageLocations
        size_t totalRegisteredImages;
        size_t pendingImages;

        uint64_t currentFrame;
    };

    Stats getStats() const {
        Stats s{};

        auto fontStats = fontAtlas_.getStats();
        s.fontPageCount = fontStats.pageCount;
        s.glyphCount = fontStats.glyphCount;
        s.imageCount = fontStats.imageCount;
        s.fontAvgUtilization = fontStats.avgUtilization;

        auto gridStats = gridAtlas_.getStats();
        s.gridPageCount = gridStats.pageCount;
        s.gridUsedRects = gridStats.usedRects;
        s.gridPendingUploads = gridStats.pendingUploads;

        auto shelfStats = shelfAtlas_.getStats();
        s.shelfPageCount = shelfStats.pageCount;
        s.shelfUsedShelves = shelfStats.usedShelves;
        s.shelfPendingUploads = shelfStats.pendingUploads;

        {
            std::lock_guard lock(standaloneMutex_);
            s.standaloneCount = standaloneTextures_.size();
            s.standaloneTotalBytes = 0;
            s.standalonePersistentCount = 0;
            for (const auto& [id, info] : standaloneTextures_) {
                s.standaloneTotalBytes += (uint64_t)info.size.x * info.size.y * 4;
                if (info.persistent) ++s.standalonePersistentCount;
            }
        }

        {
            std::lock_guard lock(tiledMutex_);
            s.tiledCount = tiledTextures_.size();
            s.tiledTotalTiles = 0;
            s.tiledTotalBytes = 0;
            for (const auto& [id, info] : tiledTextures_) {
                s.tiledTotalTiles += info.tiles.size();
                for (const auto& tile : info.tiles) {
                    s.tiledTotalBytes += (uint64_t)tile.w * tile.h * 4;
                }
            }
        }

        {
            std::lock_guard lock(locationMutex_);
            s.totalRegisteredImages = imageLocations_.size();
            s.pendingImages = 0;
            for (const auto& [id, loc] : imageLocations_) {
                if (loc.isPending) ++s.pendingImages;
            }
        }

        s.currentFrame = currentFrame_;
        return s;
    }

    //=========================================================================
    // 【Layer A】解決系API（UI/Scene向け）
    //=========================================================================
    ResolveResult resolveFontGlyph(ImageId id) {
        ResolveResult result;

        const GlyphInfo* gi = fontAtlas_.findByImageId(id);
        if (!gi || gi->width == 0) {
            result.status = ResolveStatus::NotFound;
            return result;
        }

        TextureSlot* slot = fontAtlas_.getPageSlot(gi->pageIndex);
        if (!slot || !bgfx::isValid(slot->handle)) {
            result.status = ResolveStatus::Evicted;
            return result;
        }

        result.status = ResolveStatus::Success;
        result.resolved.slot = slot;
        result.resolved.u0 = gi->u0;
        result.resolved.v0 = gi->v0;
        result.resolved.u1 = gi->u1;
        result.resolved.v1 = gi->v1;
        result.resolved.width = gi->width;
        result.resolved.height = gi->height;
        return result;
    }
    ResolveResult resolveThumbnail(ImageId id) {
        ResolveResult result;
        uint64_t contentId = getImageIdLocal(id);

        // slot は imageLocations_[id].slot を使う (= GoThread が事前確保した stable storage)。
        // UV / size は atlas 側 (page 内 sub-rect) が真値だが、atlas 登録前でも slot さえ
        // 存在すれば Success を返す方針 (= cmd は &slot->handle を捕まえれば render thread
        // が後で handle を埋める。UV はデフォルト 0..1 で取り敢えず返し、registerGridImage
        // 等で正値に更新される)。
        TextureSlot* slot = nullptr;
        int locW = 0, locH = 0;
        {
            std::lock_guard lock(locationMutex_);
            auto it = imageLocations_.find(id);
            if (it != imageLocations_.end()) {
                slot = &it->second.slot;
                locW = it->second.width;
                locH = it->second.height;
            }
        }

        // slot 無し = imageLocations_[id] 未登録 → 正真正銘の未知 id。
        if (!slot) {
            result.status = ResolveStatus::NotFound;
            return result;
        }

        // UV は atlas が知ってればそれを使う、無ければ 0..1 で返す。
        // ★ computeUVAnyState を使う: 通常の getUV は state==Ready を要求するが、
        //   reserveGridPlaceholder 直後は state==Loading なので false 返してしまい
        //   UV が 0..1 (全表示) になっていた。位置 (r.x/r.y) は acquire 時に確定済
        //   なので state 無視で UV 計算しても正しい。
        float u0 = 0, v0 = 0, u1 = 1, v1 = 1;
        auto gridHandle = gridAtlas_.find(contentId);
        if (gridHandle) {
            gridAtlas_.computeUVAnyState(*gridHandle, u0, v0, u1, v1);
        } else {
            auto shelfHandle = shelfAtlas_.find(contentId);
            if (shelfHandle) {
                shelfAtlas_.computeUVAnyState(*shelfHandle, u0, v0, u1, v1);
            }
        }

        result.status = ResolveStatus::Success;
        result.resolved.slot = slot;
        result.resolved.u0 = u0;
        result.resolved.v0 = v0;
        result.resolved.u1 = u1;
        result.resolved.v1 = v1;
        result.resolved.width = (uint16_t)locW;
        result.resolved.height = (uint16_t)locH;
        return result;
    }
    // ImageId → 描画可能なテクスチャに解決
    ResolveResult resolve(ImageId id) {
        ResolveResult result;

        if (id == 0) {
            result.status = ResolveStatus::NotFound;
            return result;
        }

        ImageIdDomain domain = getImageIdDomain(id);

        switch (domain) {
        case ImageIdDomain::FontGlyph:
            return resolveFontGlyph(id);

        case ImageIdDomain::Thumbnail:
            return resolveThumbnail(id);  // Grid/Shelf 両方

        case ImageIdDomain::Offscreen:
        case ImageIdDomain::Memory:
        case ImageIdDomain::Generated:
        case ImageIdDomain::File: {
            // Check imageLocations_ to determine actual placement.
            // ★ type を見るためだけに lock を取り、type だけ snapshot して即解放。
            //   その後 resolveGridAtlas / resolveShelfAtlas / resolveStandalone を
            //   再度 lock 経由で呼ぶ (各 resolver が内部で imageLocations_[id].slot
            //   の stable address を取得して返す)。re-lookup によるオーバーヘッドは
            //   軽微、deadlock 回避が優先。
            ImageLocation::Type loctype = ImageLocation::Type::None;
            {
                std::lock_guard lock(locationMutex_);
                auto it = imageLocations_.find(id);
                if (it != imageLocations_.end()) loctype = it->second.type;
            }
            switch (loctype) {
            case ImageLocation::Type::GridAtlas:
            case ImageLocation::Type::ShelfAtlas:
                return resolveThumbnail(id);
            case ImageLocation::Type::TiledStandalone:
                return resolveTiledFirstTile(id);
            default:
                break;
            }
            return resolveStandalone(id);
        }

        default:
            result.status = ResolveStatus::NotFound;
            return result;
        }
    }

    // 簡易版（成功時のみテクスチャ情報を返す）
    bool resolve(ImageId id, bgfx::TextureHandle& outTex,
        float& u0, float& v0, float& u1, float& v1) {
        auto result = resolve(id);
        if (!result.isReady()) return false;

        if (auto* tp = result.resolved.texture()) {
            outTex = *tp;
        }
        u0 = result.resolved.u0;
        v0 = result.resolved.v0;
        u1 = result.resolved.u1;
        v1 = result.resolved.v1;
        return true;
    }

    // 描画用（Pending時はplaceholder）
    ResolvedTexture resolveForDraw(ImageId id) {
        return resolve(id).forDraw();
    }

    //=========================================================================
    // 【Layer A】ImageId 登録/管理
    //=========================================================================

    // ImageIdの状態を確認
    bool exists(ImageId id) const {
        std::lock_guard lock(locationMutex_);
        return imageLocations_.find(id) != imageLocations_.end();
    }

    bool isPending(ImageId id) const {
        std::lock_guard lock(locationMutex_);
        auto it = imageLocations_.find(id);
        return it != imageLocations_.end() && it->second.isPending;
    }

    // Pending状態で登録（ロード開始時）
    void registerPending(ImageId id) {
        std::lock_guard lock(locationMutex_);
        auto& loc = imageLocations_[id];
        loc.type = ImageLocation::Type::None;
        loc.isPending = true;
    }

    // Pending状態 + 配置タイプを事前登録（GPU uploadキューイング前）
    void registerPendingWithType(ImageId id, ImageLocation::Type placementType) {
        std::lock_guard lock(locationMutex_);
        auto& loc = imageLocations_[id];
        loc.type = placementType;
        loc.isPending = true;
    }

    // touch（使用マーク）
    void touch(ImageId id) {
        ImageIdDomain domain = getImageIdDomain(id);

        // Standalone系
        if (domain == ImageIdDomain::Offscreen ||
            domain == ImageIdDomain::Memory ||
            domain == ImageIdDomain::Generated ||
            domain == ImageIdDomain::File) {
            // まずstandaloneを探す
            {
                std::lock_guard lock(standaloneMutex_);
                auto it = standaloneTextures_.find(id);
                if (it != standaloneTextures_.end()) {
                    it->second.lastUsedFrame = currentFrame_;
                    return;
                }
            }
            // tiledを探す
            {
                std::lock_guard lock(tiledMutex_);
                auto it = tiledTextures_.find(id);
                if (it != tiledTextures_.end()) {
                    it->second.lastUsedFrame = currentFrame_;
                    return;
                }
            }
        }

        // Grid/Shelf は内部ハンドル経由で touch
        std::lock_guard lock(locationMutex_);
        auto it = imageLocations_.find(id);
        if (it != imageLocations_.end()) {
            if (it->second.type == ImageLocation::Type::GridAtlas) {
                gridAtlas_.touch(it->second.thumbnail.handle);
            }
            else if (it->second.type == ImageLocation::Type::ShelfAtlas) {
                shelfAtlas_.touch(it->second.thumbnail.handle);
            }
        }
    }

    // 参照カウント
    void retain(ImageId id) {
        ImageIdDomain domain = getImageIdDomain(id);
        if (domain == ImageIdDomain::Offscreen ||
            domain == ImageIdDomain::Memory ||
            domain == ImageIdDomain::Generated ||
            domain == ImageIdDomain::File) {
            {
                std::lock_guard lock(standaloneMutex_);
                auto it = standaloneTextures_.find(id);
                if (it != standaloneTextures_.end()) {
                    ++it->second.refCount;
                    it->second.lastUsedFrame = currentFrame_;
                    return;
                }
            }
            {
                std::lock_guard lock(tiledMutex_);
                auto it = tiledTextures_.find(id);
                if (it != tiledTextures_.end()) {
                    ++it->second.refCount;
                    it->second.lastUsedFrame = currentFrame_;
                    return;
                }
            }
        }
        // Grid/Shelfは内部LRUで管理
    }

    void release(ImageId id) {
        ImageIdDomain domain = getImageIdDomain(id);
        if (domain == ImageIdDomain::Offscreen ||
            domain == ImageIdDomain::Memory ||
            domain == ImageIdDomain::Generated ||
            domain == ImageIdDomain::File) {
            {
                std::lock_guard lock(standaloneMutex_);
                auto it = standaloneTextures_.find(id);
                if (it != standaloneTextures_.end() && it->second.refCount > 0) {
                    --it->second.refCount;
                    return;
                }
            }
            {
                std::lock_guard lock(tiledMutex_);
                auto it = tiledTextures_.find(id);
                if (it != tiledTextures_.end() && it->second.refCount > 0) {
                    --it->second.refCount;
                    return;
                }
            }
        }
    }

    //=========================================================================
    // 【Layer B】Standalone テクスチャ（低レベル）
    //=========================================================================

    // 作成（ピクセルデータから）
    ImageId createStandaloneTexture(const void* pixels, uint16_t w, uint16_t h,
        int pitch, bool persistent = false) {
        ImageId id = ImageIdGenerator::fromMemory();
        if (!createStandaloneInternal(id, pixels, w, h, pitch, persistent,
            ImageOrigin::Memory, false)) {
            return 0;
        }
        return id;
    }

    // 作成（SDL_Surface から）
    ImageId createStandaloneTexture(SDL_Surface* surface, bool persistent = false) {
        if (!surface) return 0;

        SDL_Surface* s = surface;
        SDL_Surface* conv = nullptr;
        if (s->format != SDL_PIXELFORMAT_RGBA8888) {
            conv = SDL_ConvertSurface(s, SDL_PIXELFORMAT_RGBA8888);
            s = conv;
        }

        ImageId id = createStandaloneTexture(s->pixels, s->w, s->h, s->pitch, persistent);

        if (conv) SDL_DestroySurface(conv);
        return id;
    }

    // 作成（空テクスチャ）
    ImageId createEmptyStandaloneTexture(uint16_t w, uint16_t h, bool persistent = false) {
        if (w == 0 || h == 0) return 0;

        // 巨大空テクスチャ → タイル分割パス
        if (shouldTile(w, h)) {
            ImageId id = ImageIdGenerator::fromMemory();
            if (createTiledEmptyInternal(id, w, h, persistent)) return id;
            return 0;
        }

        ImageId id = ImageIdGenerator::fromMemory();

        std::lock_guard lock(standaloneMutex_);

        bgfx::TextureHandle handle = bgfx::createTexture2D(
            w, h, false, 1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
            nullptr);

        if (!bgfx::isValid(handle)) return 0;

        auto& info = standaloneTextures_[id];
        info.handle = handle;
        info.size.x = w;
        info.size.y = h;
        info.refCount = 1;
        info.lastUsedFrame = currentFrame_;
        info.persistent = persistent;
        info.origin = ImageOrigin::Memory;
        info.isRenderTarget = false;

        // 配置情報を登録
        {
            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::Standalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = false;
        }

        return id;
    }
    //=========================================================================
    // テクスチャポインタ取得API（追加）
    //=========================================================================

    // Standalone テクスチャへのポインタ取得
    bgfx::TextureHandle* getStandaloneTexturePtr(ImageId id) {
        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        if (it != standaloneTextures_.end()) {
            return &it->second.handle;
        }
        return nullptr;
    }

    // FBO へのポインタ取得
    bgfx::FrameBufferHandle* getOffscreenFBOPtr(ImageId id) {
        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        if (it != standaloneTextures_.end() && it->second.isRenderTarget) {
            return &it->second.fbo;
        }
        return nullptr;
    }

    // 旧API互換
    ImageId reserveOffscreenTexture(int w, int h,
        bool persistent = false) {
        ImageId id = ImageIdGenerator::forOffscreen();

        // 先にエントリを作成（テクスチャは後から）
        // shouldTile(w,h) で willBeTiled を判定しておき、巨大なときは Standalone エントリを
        // 作らずに Tiled エントリ側に登録する (= 後の queueOffscreenResize で wasTiled が
        // 正しく拾える)。今は queueOffscreenNew(thgc, 1, 1) で初期化することが多いので
        // ここに来た段階ではほぼ Standalone (1x1) パス。
        {
            std::lock_guard lock(standaloneMutex_);
            auto [it, inserted] = standaloneTextures_.emplace(id, StandaloneTextureInfo{});
            auto& info = it->second;
            info.size.x = w;
            info.size.y = h;
            info.persistent = persistent;
            info.isRenderTarget = true;
            info.origin = ImageOrigin::Offscreen;
            info.refCount = 1;
            {
                std::lock_guard lock(locationMutex_);
                auto& loc = imageLocations_[id];
                loc.type = ImageLocation::Type::Standalone;
                loc.width = w;
                loc.height = h;
                loc.isPending = true;
            }

            // 作成キューに追加
            {
                std::lock_guard lock(offscreenQueueMutex_);
                OffscreenCreateRequest req{};
                req.id = id; req.width = w; req.height = h; req.persistent = persistent;
                offscreenCreateQueue_.push_back(req);
            }

            return id;
        }

        return id;
    }
    // queueOffscreenResize: 戻り値は「以後参照すべき ImageId」。
    //
    //   ・サイズが変わるだけ (Standalone↔Standalone or Tiled↔Tiled): 同じ id を返す
    //   ・Standalone ↔ Tiled の TYPE 遷移が起きるケースでは
    //     新しい id を発行 (これが返り値) し、メタデータを eager に登録する。
    //     - 旧 id は destroy キューへ
    //     - 新 id は create キューへ
    //     - 同フレーム内の producer が新 id を mygetTiledTextureInfo で
    //       「ちゃんとタイル」として参照できるよう、tiles ベクタには
    //       BGFX_INVALID_HANDLE のプレースホルダを並べておく
    //       (実体 FBO は次の processOffscreenQueue で埋まる)
    //   呼び出し側は必ず戻り値で自身の参照 (例: elem->offscreen->imPing) を更新すること。
    // 合計寸法は int で受ける (= 巨大コンテンツの uint16_t truncate を防ぐ)。
    // 例: newH = 67600 を uint16_t で受けると 2064 (= 67600-65536) に縮んで shouldTile が
    // false になり、Tiled 遷移しないまま Standalone 固着するバグの修正。
    ImageId queueOffscreenResize(ImageId id, int newW, int newH,
                                 uint16_t pageW = 0, uint16_t pageH = 0) {
        bool wasTiled    = isTiledTexture(id);
        bool willBeTiled = shouldTile(newW, newH);

        if (wasTiled == willBeTiled) {
            // 同タイプ → 同 id でリサイズキューに積む。
            // 実 FBO の差し替えは processOffscreenQueue で非同期に行うが、
            // メタデータ (sti->size / tti->totalSize / tti->tiles) はここで同期更新しておく。
            // = producer 側 (Phase A の writeCellsInRect 等) がサイズを参照する時点で
            //   既に新サイズが見えている必要があるため。
            //   失敗時の rollback 用に旧値を残す動機はない (= queueOffscreenResize 自体が
            //   失敗しない設計、push されたら確定)。
            if (willBeTiled) {
                std::lock_guard tlock(tiledMutex_);
                auto it = tiledTextures_.find(id);
                if (it != tiledTextures_.end()) {
                    auto& info = it->second;
                    uint16_t tsx, tsy;
                    computeEffectiveTileSize(newW, newH, tsx, tsy, /*adjustX=*/true, pageW, pageH);
                    int tilesX = (int)((newW + tsx - 1) / tsx);
                    int tilesY = (int)((newH + tsy - 1) / tsy);
                    info.totalSize = { (int)newW, (int)newH };
                    info.tileSize  = tsx;
                    info.tileSizeY = tsy;
                    info.tilesX    = tilesX;
                    info.tilesY    = tilesY;
                    // ★ producer が `&tile.handle` をポインタ保持しているので tiles ベクタの
                    //   バッファアドレスを保つ in-place 更新を優先。タイル数が違うときだけ
                    //   resize する (= 既存ポインタが dangling になる経路)。
                    size_t expected = (size_t)tilesX * (size_t)tilesY;
                    if (info.tiles.size() != expected) {
                        info.tiles.clear();
                        info.tiles.reserve(expected);
                        info.tiles.resize(expected);
                    }
                    for (int ty = 0; ty < tilesY; ++ty) {
                        for (int tx = 0; tx < tilesX; ++tx) {
                            int tileX = tx * (int)tsx;
                            int tileY = ty * (int)tsy;
                            uint16_t tileW = (uint16_t)std::min(newW - tileX, (int)tsx);
                            uint16_t tileH = (uint16_t)std::min(newH - tileY, (int)tsy);
                            size_t idx = (size_t)ty * (size_t)tilesX + (size_t)tx;
                            auto& tile = info.tiles[idx];
                            // 既存 valid な fbo/handle は destroy してから INVALID に戻す
                            // (resize 等で実 FBO が古いサイズで残っているので解放が必要)。
                            if (bgfx::isValid(tile.fbo)) {
                                bgfx::destroy(tile.fbo);
                                tile.fbo    = BGFX_INVALID_HANDLE;
                                tile.handle = BGFX_INVALID_HANDLE;
                            } else if (bgfx::isValid(tile.handle)) {
                                bgfx::destroy(tile.handle);
                                tile.handle = BGFX_INVALID_HANDLE;
                            }
                            tile.x = tileX;  tile.y = tileY;
                            tile.w = tileW;  tile.h = tileH;
                            tile.size = { (int)tileW, (int)tileH };
                        }
                    }
                }
            } else {
                std::lock_guard slock(standaloneMutex_);
                auto it = standaloneTextures_.find(id);
                if (it != standaloneTextures_.end()) {
                    it->second.size = { (int)newW, (int)newH };
                }
            }
            std::lock_guard lock(offscreenQueueMutex_);
            OffscreenResizeRequest req{};
            req.id = id; req.width = newW; req.height = newH;
            req.pageW = pageW; req.pageH = pageH;
            offscreenResizeQueue_.push_back(std::move(req));
            return id;
        }

        // タイプ遷移 → 新 id を発行
        ImageId newId = ImageIdGenerator::forOffscreen();

        if (willBeTiled) {
            // Standalone → Tiled (pageW/pageH があればタイル辺長をその倍数に丸める)
            uint16_t tsx, tsy;
            computeEffectiveTileSize(newW, newH, tsx, tsy, /*adjustX=*/true, pageW, pageH);
            int tilesX = (int)((newW + tsx - 1) / tsx);
            int tilesY = (int)((newH + tsy - 1) / tsy);

            std::lock_guard lock(tiledMutex_);
            TiledTextureInfo info;
            info.totalSize     = { (int)newW, (int)newH };
            info.tileSize      = tsx;
            info.tileSizeY     = tsy;
            info.tilesX        = tilesX;
            info.tilesY        = tilesY;
            info.isRenderTarget = true;
            info.refCount      = 1;
            info.lastUsedFrame = currentFrame_;
            // tiles ベクタは「形だけ」並べておく (実体 FBO は processOffscreenQueue が埋める)
            // producer が tiles.size() / tile.x / tile.y / tile.w / tile.h は読める必要がある
            for (int ty = 0; ty < tilesY; ++ty) {
                for (int tx = 0; tx < tilesX; ++tx) {
                    int tileX = tx * (int)tsx;
                    int tileY = ty * (int)tsy;
                    uint16_t tileW = (uint16_t)std::min(newW - tileX, (int)tsx);
                    uint16_t tileH = (uint16_t)std::min(newH - tileY, (int)tsy);
                    TiledTextureInfo::Tile tile{};
                    tile.handle = BGFX_INVALID_HANDLE;
                    tile.fbo    = BGFX_INVALID_HANDLE;
                    tile.x = tileX;  tile.y = tileY;
                    tile.w = tileW;  tile.h = tileH;
                    tile.size = { (int)tileW, (int)tileH };
                    info.tiles.push_back(tile);
                }
            }
            tiledTextures_[newId] = std::move(info);

            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[newId];
            loc.type   = ImageLocation::Type::TiledStandalone;
            loc.width  = newW;
            loc.height = newH;
            loc.isPending = true;
        } else {
            // Tiled → Standalone
            std::lock_guard lock(standaloneMutex_);
            StandaloneTextureInfo info{};
            info.size           = { (int)newW, (int)newH };
            info.isRenderTarget = true;
            info.refCount       = 1;
            info.lastUsedFrame  = currentFrame_;
            info.handle = BGFX_INVALID_HANDLE;
            info.fbo    = BGFX_INVALID_HANDLE;
            standaloneTextures_[newId] = info;

            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[newId];
            loc.type   = ImageLocation::Type::Standalone;
            loc.width  = newW;
            loc.height = newH;
            loc.isPending = true;
        }

        // 旧 id 破棄 + 新 id 作成 をキューへ
        {
            std::lock_guard lock(offscreenQueueMutex_);
            offscreenDestroyQueue_.push_back(id);
            OffscreenCreateRequest req{};
            req.id = newId; req.width = newW; req.height = newH;
            req.persistent = false;
            req.pageW = pageW; req.pageH = pageH;
            offscreenCreateQueue_.push_back(std::move(req));
        }
        return newId;
    }
    void queueOffscreenDestroy(ImageId id) {
        std::lock_guard lock(offscreenQueueMutex_);
        offscreenDestroyQueue_.push_back(id);
    }
    // 通常テクスチャ予約（ロード前にポインタ取得）
    ImageId reserveTexture(bgfx::TextureHandle** outTexPtr, uint16_t w, uint16_t h) {
        ImageId id = ImageIdGenerator::fromMemory();

        {
            std::lock_guard lock(standaloneMutex_);
            auto [it, inserted] = standaloneTextures_.emplace(id, StandaloneTextureInfo{});
            auto& info = it->second;
            info.size.x = w;
            info.size.y = h;
            info.origin = ImageOrigin::Memory;

            if (outTexPtr) *outTexPtr = &info.handle;
        }

        {
            std::lock_guard lock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::Standalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = true;
        }

        return id;
    }
    // テクスチャデータをアップロード（予約済みIDに対して）
    bool uploadTexture(ImageId id, const void* pixels, uint16_t w, uint16_t h, int pitch) {
        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        if (it == standaloneTextures_.end()) return false;

        auto& info = it->second;

        // テクスチャ作成
        const bgfx::Memory* mem = bgfx::copy(pixels, w * h * 4);
        info.handle = bgfx::createTexture2D(
            w, h, false, 1,
            bgfx::TextureFormat::RGBA8,
            BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
            mem);

        if (!bgfx::isValid(info.handle)) return false;

        info.size.x = w;
        info.size.y = h;
        info.lastUsedFrame = currentFrame_;

        // Pending解除
        {
            std::lock_guard locLock(locationMutex_);
            auto locIt = imageLocations_.find(id);
            if (locIt != imageLocations_.end()) {
                locIt->second.isPending = false;
            }
        }

        return true;
    }

    //=========================================================================
    // FontAtlas テクスチャポインタ取得
    //=========================================================================

    bgfx::TextureHandle* getFontAtlasTexturePtr(uint16_t pageIndex) {
        return &fontAtlas_.getPageTexture(pageIndex);
    }

    // グリフ取得（ポインタ付き）
    struct GlyphResult {
        const GlyphInfo* info = nullptr;
        bgfx::TextureHandle* texturePtr = nullptr;
    };

    GlyphResult getOrAddGlyphWithPtr(FontId font, uint32_t codepoint,
        RenderGroup* group = nullptr,
        SDL_Color color = { 255,255,255,255 }) {
        GlyphResult result;
        const auto& gi = fontAtlas_.getOrAddGlyph(font, codepoint, group, color);
        result.info = &gi;
        if (gi.width > 0) {
            result.texturePtr = &fontAtlas_.getPageTexture(gi.pageIndex);
        }
        return result;
    }

    //=========================================================================
    // Thumbnail テクスチャポインタ取得
    //=========================================================================

    bgfx::TextureHandle* getGridAtlasTexturePtr(uint16_t pageIndex) {
        return (bgfx::TextureHandle*)(&gridAtlas_.getTexture(pageIndex));
    }

    bgfx::TextureHandle* getShelfAtlasTexturePtr(uint16_t pageIndex) {
        return (bgfx::TextureHandle*)(&shelfAtlas_.getTexture(pageIndex));
    }

    // ImageId → TextureSlot* を取得 (= handle / fbo / size の stable な置き場).
    //
    // 描画 cmd はこのポインタを捕まえる → render thread がそのまま slot の
    // 中の `handle` / `fbo` 値を埋めるので、cmd 側は同じポインタを deref する
    // だけで最新値を見られる (アドレス不変、値だけ INVALID → valid に変化)。
    //
    // 全 placement (Standalone / GridAtlas / ShelfAtlas / FontAtlas) で
    // `imageLocations_[id].slot` を返す。これが「**imageId ごとの stable storage**」。
    // 各 placement の render thread upload path が `loc.slot.handle` / `loc.slot.fbo`
    // / `loc.slot.size` を埋める責任を持つ。
    //
    // Tiled は tile 単位の表現なのでここでは扱わない (=
    // mygetTiledTextureInfo + tile index 経由が必要)。
    TextureSlot* resolveSlot(ImageId id) {
        if (id == 0) return nullptr;
        std::lock_guard lock(locationMutex_);
        auto it = imageLocations_.find(id);
        if (it == imageLocations_.end()) return nullptr;
        return &it->second.slot;
    }

    // GoThread から呼ばれる Standalone slot の事前確保 (= placeholder)。
    // bgfx は呼ばない。unordered_map に slot を入れて handle = INVALID のまま
    // 戻すだけ。実 bgfx テクスチャは後で render thread の uploadToGpuInternal が
    // info.handle に値を書き込んで完成させる。
    // cmd push が `&info.handle` をこの時点で捕まえてもアドレスは不変なので
    // 安全 (cmd は描画時点では valid な handle を見る)。
    void reserveStandalonePlaceholder(ImageId id, uint16_t w, uint16_t h,
                                       ImageUsage usage, bool persistent) {
        {
            std::lock_guard lock(standaloneMutex_);
            auto [it, inserted] = standaloneTextures_.emplace(id, StandaloneTextureInfo{});
            auto& info = it->second;
            if (inserted) {
                info.handle = BGFX_INVALID_HANDLE;
                info.fbo    = BGFX_INVALID_HANDLE;
                info.size   = { w, h };
                info.persistent = persistent;
                info.origin = ImageOrigin::File;
                info.refCount = 1;
                info.lastUsedFrame = currentFrame_;
            }
            // 既に entry がある場合は touch だけ
            info.lastUsedFrame = currentFrame_;
        }
        // ImageLocation 側の slot にも placeholder (INVALID + size) を入れておく。
        // cmd は &imageLocations_[id].slot.handle を握る (resolveSlot 経由)。
        {
            std::lock_guard lock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::Standalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = true;
            loc.slot.handle = BGFX_INVALID_HANDLE;
            loc.slot.fbo    = BGFX_INVALID_HANDLE;
            loc.slot.size   = { w, h };
        }
    }

    // GoThread から呼ばれる GridAtlas slot の事前確保。
    // gridAtlas_.acquire() はページが既に存在する状態では bgfx を呼ばず slot 索引
    // を返すだけなので GoThread 安全 (ImageMaster::initialize で page 0 は事前生成
    // 済み = 既に valid な page texture が存在)。
    // slot.handle には page texture handle (= gridAtlas_.getTexture(pageIndex)) を
    // 入れる。値そのもののコピーで OK で、page が evict されない限り変わらない。
    bool reserveGridPlaceholder(ImageId id, uint64_t contentId, uint16_t w, uint16_t h) {
        bool needsLoad = false;
        ThumbnailHandle handle = gridAtlas_.acquire(contentId, needsLoad);
        if (!handle.isValid()) return false;

        std::lock_guard lock(locationMutex_);
        auto& loc = imageLocations_[id];
        loc.type = ImageLocation::Type::GridAtlas;
        loc.thumbnail.handle = handle;
        loc.width = w;
        loc.height = h;
        loc.isPending = needsLoad;
        // ★ page texture を slot.handle にコピー。cmd の捕まえたポインタはここを指す。
        loc.slot.handle = gridAtlas_.getTexture(handle.pageIndex);
        loc.slot.fbo    = BGFX_INVALID_HANDLE;
        loc.slot.size   = { w, h };
        return true;
    }

    // GoThread から呼ばれる ShelfAtlas slot の事前確保 (Grid と同じ理屈)。
    bool reserveShelfPlaceholder(ImageId id, uint64_t contentId, uint16_t w, uint16_t h) {
        bool needsLoad = false;
        ThumbnailHandle handle = shelfAtlas_.acquire(contentId, w, h, needsLoad);
        if (!handle.isValid()) return false;

        std::lock_guard lock(locationMutex_);
        auto& loc = imageLocations_[id];
        loc.type = ImageLocation::Type::ShelfAtlas;
        loc.thumbnail.handle = handle;
        loc.width = w;
        loc.height = h;
        loc.isPending = needsLoad;
        loc.slot.handle = shelfAtlas_.getTexture(handle.pageIndex);
        loc.slot.fbo    = BGFX_INVALID_HANDLE;
        loc.slot.size   = { w, h };
        return true;
    }

    // ImageId → 永続的なTextureHandleポインタを取得（配置タイプに応じて）
    bgfx::TextureHandle* resolveTexturePtr(ImageId id) {
        if (id == 0) return nullptr;

        // imageLocations_ で配置タイプを確認
        {
            std::lock_guard lock(locationMutex_);
            auto it = imageLocations_.find(id);
            if (it != imageLocations_.end()) {
                switch (it->second.type) {
                case ImageLocation::Type::GridAtlas:
                    return &gridAtlas_.getTexture(it->second.thumbnail.handle.pageIndex);
                case ImageLocation::Type::ShelfAtlas:
                    return &shelfAtlas_.getTexture(it->second.thumbnail.handle.pageIndex);
                default:
                    break;
                }
            }
        }

        // Standalone fallback
        {
            std::lock_guard lock(standaloneMutex_);
            auto sit = standaloneTextures_.find(id);
            if (sit != standaloneTextures_.end() && bgfx::isValid(sit->second.handle)) {
                return &sit->second.handle;
            }
        }
        return nullptr;
    }

    // Thumbnail 取得（ポインタ付き）
    struct ThumbnailResult {
        ThumbnailHandle handle;
        bgfx::TextureHandle* texturePtr = nullptr;
        float u0 = 0, v0 = 0, u1 = 1, v1 = 1;
        bool needsLoad = false;
        bool found = false;
    };

    ThumbnailResult acquireGridThumbnailWithPtr(uint64_t contentId) {
        ThumbnailResult result;
        result.handle = gridAtlas_.acquire(contentId, result.needsLoad);
        if (result.handle.isValid()) {
            result.found = true;
            result.texturePtr = getGridAtlasTexturePtr(result.handle.pageIndex);
            gridAtlas_.getUV(result.handle, result.u0, result.v0, result.u1, result.v1);
        }
        return result;
    }

    ThumbnailResult acquireShelfThumbnailWithPtr(uint64_t contentId, uint16_t w, uint16_t h) {
        ThumbnailResult result;
        result.handle = shelfAtlas_.acquire(contentId, w, h, result.needsLoad);
        if (result.handle.isValid()) {
            result.found = true;
            result.texturePtr = getShelfAtlasTexturePtr(result.handle.pageIndex);
            shelfAtlas_.getUV(result.handle, result.u0, result.v0, result.u1, result.v1);
        }
        return result;
    }

    //=========================================================================
    // Placeholder テクスチャポインタ
    //=========================================================================

    bgfx::TextureHandle* getPlaceholderTexturePtr() {
        const auto& white = fontAtlas_.getWhitePixel();
        return &fontAtlas_.getPageTexture(white.pageIndex);
    }


    bool resizeOffscreenInternal(ImageId id, int newW, int newH,
                                 uint16_t pageW = 0, uint16_t pageH = 0) {
        if (newW <= 0 || newH <= 0) return false;

        // タイル画像のリサイズ → 破棄して再作成
        if (isTiledTexture(id)) {
            return resizeTiledOffscreenInternal(id, newW, newH, pageW, pageH);
        }

        // 新しいサイズがタイル分割が必要な場合 → Standalone→Tiled への移行
        if (shouldTile(newW, newH)) {
            // 既存のStandaloneを破棄してTiledで再作成
            destroyStandalone(id);
            return createTiledOffscreenInternal(id, newW, newH, false, pageW, pageH);
        }
        // ここに来た時点で shouldTile=false → 両軸 maxTileSize_ ≤ 8192 で uint16_t に収まる
        uint16_t newW16 = (uint16_t)newW;
        uint16_t newH16 = (uint16_t)newH;

        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        if (it == standaloneTextures_.end() || !it->second.isRenderTarget) {
            return false;
        }

        auto& info = it->second;

        // info.size は queueOffscreenResize で予測値 (= newW/newH) に同期更新済みなので、
        // info.size との比較で early-out しても「実 FBO が target サイズで存在する」ことの
        // 証明にはならない (古い FBO が valid なまま info.size だけ新サイズに置き換わっている
        // ケースが普通)。caller (NewTableDraw/ElementDraw) 側で実テクスチャサイズと比較して
        // 必要時のみ queueOffscreenResize を呼んでいるので、ここで early-out せず常に再生成する。

        // 古いリソース破棄
        if (bgfx::isValid(info.fbo)) {
            bgfx::destroy(info.fbo);
            info.fbo = BGFX_INVALID_HANDLE;
            info.handle = BGFX_INVALID_HANDLE;
        }

        // 新しいテクスチャ作成 (BGRA8 for Metal compatibility)
        bgfx::TextureHandle newHandle = bgfx::createTexture2D(
            newW16, newH16, false, 1,
            bgfx::TextureFormat::BGRA8,
            BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
            nullptr);

        if (!bgfx::isValid(newHandle)) return false;

        // Create depth texture for offscreen framebuffer (required for Metal depth testing)
        bgfx::TextureHandle newDepthHandle = bgfx::createTexture2D(
            newW16, newH16, false, 1,
            bgfx::TextureFormat::D24S8,
            BGFX_TEXTURE_RT, nullptr);

        if (!bgfx::isValid(newDepthHandle)) {
            bgfx::destroy(newHandle);
            return false;
        }

        // Use bgfx::Attachment to properly attach color and depth
        bgfx::Attachment att[2];
        att[0].init(newHandle);      // color attachment
        att[1].init(newDepthHandle); // depth attachment

        bgfx::FrameBufferHandle newFbo = bgfx::createFrameBuffer(2, att, true);
        if (!bgfx::isValid(newFbo)) {
            bgfx::destroy(newHandle);
            bgfx::destroy(newDepthHandle);
            return false;
        }

        info.handle = newHandle;
        info.fbo = newFbo;
        info.size.x = newW16;
        info.size.y = newH16;
        info.lastUsedFrame = currentFrame_;
        initFreshRenderTargetLayout(newFbo, newW16, newH16);

        // 配置情報を更新
        {
            std::lock_guard locLock(locationMutex_);
            auto locIt = imageLocations_.find(id);
            if (locIt != imageLocations_.end()) {
                locIt->second.width = newW16;
                locIt->second.height = newH16;
            }
        }

        return true;
    }
    struct OffscreenCreateRequest {
        ImageId id;
        // 合計寸法は int (= 巨大コンテンツの uint16_t truncate 防止)。
        // 実際の bgfx テクスチャ生成は per-tile (≤ maxTileSize_ ≤ 8192) なので uint16_t に収まる。
        int width = 0, height = 0;
        bool persistent = false;
        bgfx::FrameBufferHandle* dest = nullptr;
        PointI* fbsize = nullptr;
        // ページめくり用のページサイズヒント (0 なら無し)。
        // タイル分割時、tile.w / tile.h を pageW / pageH の倍数に丸めることで
        // 任意のページが単一タイル内に収まるようにする。
        uint16_t pageW = 0;
        uint16_t pageH = 0;
    };

    struct OffscreenResizeRequest {
        ImageId id;
        int width = 0, height = 0;
        bgfx::FrameBufferHandle* dest = nullptr;
        PointI* fbsize = nullptr;
        uint16_t pageW = 0;
        uint16_t pageH = 0;
    };

    std::mutex offscreenQueueMutex_;
    std::vector<OffscreenCreateRequest> offscreenCreateQueue_;
    std::vector<OffscreenResizeRequest> offscreenResizeQueue_;
    std::vector<ImageId> offscreenDestroyQueue_;

    void processOffscreenQueue() {
        std::vector<OffscreenCreateRequest> creates;
        std::vector<OffscreenResizeRequest> resizes;
        std::vector<ImageId> destroys;
        {
            std::lock_guard lock(offscreenQueueMutex_);
            creates.swap(offscreenCreateQueue_);
            resizes.swap(offscreenResizeQueue_);
            destroys.swap(offscreenDestroyQueue_);
        }

        // 破棄対象のIDを集めて、create/resizeから除外する
        std::unordered_set<ImageId> destroySet(destroys.begin(), destroys.end());

        int size = creates.size() + resizes.size() + destroys.size();

        // 破棄（レンダースレッドで安全に実行）
        for (auto& id : destroys) {
            if (isTiledTexture(id)) {
                destroyTiled(id);
                continue;
            }
            std::lock_guard lock(standaloneMutex_);
            auto it = standaloneTextures_.find(id);
            if (it != standaloneTextures_.end()) {
                if (bgfx::isValid(it->second.fbo)) {
                    bgfx::destroy(it->second.fbo);
                }
                else if (bgfx::isValid(it->second.handle)) {
                    bgfx::destroy(it->second.handle);
                }
                standaloneTextures_.erase(it);
            }
            {
                std::lock_guard locLock(locationMutex_);
                imageLocations_.erase(id);
            }
        }

        // 作成（破棄対象はスキップ）
        for (auto& req : creates) {
            if (destroySet.count(req.id)) continue;
            bool ok = createOffscreenInternal(req.id, req.width, req.height, req.persistent, req.pageW, req.pageH);
            if (ok && req.dest) {
                auto textureInfo = getStandaloneTexture(req.id);
            }
        }

        // リサイズ（破棄対象はスキップ）
        for (auto& req : resizes) {
            if (destroySet.count(req.id)) continue;
            bool ok = resizeOffscreenInternal(req.id, req.width, req.height, req.pageW, req.pageH);
            if (ok && req.dest) {
                auto textureInfo = getStandaloneTexture(req.id);
            }
        }
        if (size != 0)bgfx::frame();
    }
    void destroyStandalone(ImageId id) {
        // タイル画像の場合はそちらへ委譲
        if (isTiledTexture(id)) {
            destroyTiled(id);
            return;
        }
        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        if (it != standaloneTextures_.end()) {
            if (bgfx::isValid(it->second.fbo)) {
                bgfx::destroy(it->second.fbo);
            }
            else if (bgfx::isValid(it->second.handle)) {
                bgfx::destroy(it->second.handle);
            }
            standaloneTextures_.erase(it);

            // 配置情報を削除
            std::lock_guard locLock(locationMutex_);
            imageLocations_.erase(id);
        }
    }

    // 旧API互換
    void touchStandaloneTexture(ImageId id) {
        touch(id);
    }

    void retainStandaloneTexture(ImageId id) {
        retain(id);
    }

    void releaseStandaloneTexture(ImageId id) {
        release(id);
    }

    void destroyStandaloneTexture(ImageId id) {
        destroyStandalone(id);
    }

    // Standalone テクスチャ情報取得
    StandaloneTextureInfo* getStandaloneTexture(ImageId id) {
        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        return (it != standaloneTextures_.end()) ? &it->second : nullptr;
    }

    // Standalone テクスチャ更新（タイル画像は自動分配）
    bool updateStandaloneTexture(ImageId id, const void* pixels,
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, int pitch) {
        // タイル画像の場合はそちらへ委譲
        if (isTiledTexture(id)) {
            return updateTiledTexture(id, pixels, x, y, w, h, pitch);
        }

        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        if (it == standaloneTextures_.end()) return false;
        if (!bgfx::isValid(it->second.handle)) return false;

        std::vector<uint8_t> data(w * h * 4);
        const uint8_t* src = (const uint8_t*)pixels;
        for (uint16_t row = 0; row < h; ++row) {
            std::memcpy(data.data() + row * w * 4, src + row * pitch, w * 4);
        }

        bgfx::updateTexture2D(it->second.handle, 0, 0, x, y, w, h,
            bgfx::copy(data.data(), (uint32_t)data.size()));

        it->second.lastUsedFrame = currentFrame_;
        return true;
    }

    //=========================================================================
    // 【Layer B-2】Tiled テクスチャ（巨大画像タイル分割）
    //=========================================================================

    // Tiled テクスチャ情報取得
    TiledTextureInfo* getTiledTexture(ImageId id) {
        std::lock_guard lock(tiledMutex_);
        auto it = tiledTextures_.find(id);
        return (it != tiledTextures_.end()) ? &it->second : nullptr;
    }

    // Tiled テクスチャ更新（矩形領域を更新、重なるタイルに自動分配）
    bool updateTiledTexture(ImageId id, const void* pixels,
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, int pitch) {
        std::lock_guard lock(tiledMutex_);
        auto it = tiledTextures_.find(id);
        if (it == tiledTextures_.end()) return false;

        auto& info = it->second;
        const uint8_t* src = (const uint8_t*)pixels;

        // 更新矩形と重なるタイルに自動分配（O(1) グリッド検索）
        info.forEachTileInRect(x, y, w, h, [&](TiledTextureInfo::Tile& tile,
            int ix, int iy, int iw, int ih) {
            std::vector<uint8_t> subData(iw * ih * 4);
            for (int row = 0; row < ih; ++row) {
                std::memcpy(
                    subData.data() + row * iw * 4,
                    src + ((iy - y) + row) * pitch + (ix - x) * 4,
                    iw * 4);
            }
            bgfx::updateTexture2D(tile.handle, 0, 0,
                (uint16_t)(ix - tile.x), (uint16_t)(iy - tile.y),
                (uint16_t)iw, (uint16_t)ih,
                bgfx::copy(subData.data(), (uint32_t)subData.size()));
        });

        info.lastUsedFrame = currentFrame_;
        return true;
    }

    // Tiled テクスチャ破棄
    void destroyTiled(ImageId id) {
        std::lock_guard lock(tiledMutex_);
        auto it = tiledTextures_.find(id);
        if (it != tiledTextures_.end()) {
            for (auto& tile : it->second.tiles) {
                if (bgfx::isValid(tile.fbo)) {
                    bgfx::destroy(tile.fbo);  // FBO破棄でattachmentも解放される
                }
                else if (bgfx::isValid(tile.handle)) {
                    bgfx::destroy(tile.handle);
                }
            }
            tiledTextures_.erase(it);

            // 配置情報を削除
            std::lock_guard locLock(locationMutex_);
            imageLocations_.erase(id);
        }
    }

    // Tiled テクスチャの描画解決（全タイル返却）
    TiledResolveResult resolveTiledForDraw(ImageId id) {
        TiledResolveResult result;
        std::lock_guard lock(tiledMutex_);
        auto it = tiledTextures_.find(id);
        if (it == tiledTextures_.end()) return result;

        auto& info = it->second;
        info.lastUsedFrame = currentFrame_;
        result.totalWidth = info.totalSize.x;
        result.totalHeight = info.totalSize.y;

        for (auto& tile : info.tiles) {
            if (!bgfx::isValid(tile.handle)) continue;
            result.tiles.push_back({
                &tile.handle,
                (float)tile.x, (float)tile.y,
                (float)tile.w, (float)tile.h
            });
        }
        return result;
    }

    // タイル画像かどうか判定
    bool isTiledTexture(ImageId id) const {
        std::lock_guard lock(locationMutex_);
        auto it = imageLocations_.find(id);
        return it != imageLocations_.end() &&
               it->second.type == ImageLocation::Type::TiledStandalone;
    }

    //=========================================================================
    // 【Layer B】FontAtlas（低レベル）
    //=========================================================================

    FontAtlas& fontAtlas() { return fontAtlas_; }
    const FontAtlas& fontAtlas() const { return fontAtlas_; }

    // FontAtlas 便利メソッド（委譲）
    // 入力 size は論理ピクセル。getFont と同じ g_uiScale を掛けて物理ピクセルに
    // 揃え、getFont("sans",16) → FontId("sans", scaled) の lookup が一致する
    // フォントを atlas に登録する。
    FontId registerFont(const char* name, const std::string& fontPath, int size) {
        int physSize = (int)((float)size * GetUiScale() + 0.5f);
        if (physSize < 1) physSize = 1;
        return fontAtlas_.registerFont(name, fontPath, physSize);
    }

    void setFallbackChain(FontId primary, std::initializer_list<FontId> fallbacks) {
        fontAtlas_.setFallbackChain(primary, fallbacks);
    }

    const GlyphInfo& getWhitePixel() const {
        return fontAtlas_.getWhitePixel();
    }

    const GlyphInfo& getOrAddGlyph(FontId font, uint32_t codepoint,
        RenderGroup* group = nullptr,
        SDL_Color color = { 255, 255, 255, 255 }) {
        return fontAtlas_.getOrAddGlyph(font, codepoint, group, color);
    }

    const GlyphInfo* getImage(ImageId id) const {
        return fontAtlas_.getImage(id);
    }

    bgfx::TextureHandle getFontTexture(uint16_t pageIndex) {
        return fontAtlas_.getPageTexture(pageIndex);
    }

    // グリフ追加してImageIdを返す
    ImageId addGlyph(FontId font, uint32_t codepoint, SDL_Color color = { 255,255,255,255 }) {
        // FontIndex を取得（衝突なしの連番）
        uint32_t fontIndex = fontAtlas_.getFontIndex(font);
        if (fontIndex == 0) return 0;  // 未登録フォント

        const auto& gi = fontAtlas_.getOrAddGlyph(font, codepoint, nullptr, color);
        if (gi.width == 0) return 0;

        // localId = fontIndex(24bit) + codepoint(24bit) で一意
        // codepoint は Unicode で最大 0x10FFFF (21bit) なので 24bit で十分
        uint64_t localId = (uint64_t(fontIndex) << 24) | (codepoint & 0x00FFFFFF);
        ImageId id = ImageIdGenerator::forFontGlyph(localId);

        // 配置情報を登録
        {
            std::lock_guard lock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::FontAtlas;
            loc.font.glyphKey = localId;
            loc.width = gi.width;
            loc.height = gi.height;
            loc.isPending = false;
        }

        return id;
    }

    // 画像追加してImageIdを返す
    ImageId addImageToFontAtlas(SDL_Surface* surface, bool pinned = false) {
        ImageId id = ImageIdGenerator::forFontGlyph(nextFontAtlasImageId_++);
        const auto& gi = fontAtlas_.addImage(id, surface, nullptr, pinned);
        if (gi.width == 0) return 0;

        {
            std::lock_guard lock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::FontAtlas;
            loc.font.glyphKey = id;
            loc.width = gi.width;
            loc.height = gi.height;
            loc.isPending = false;
        }

        return id;
    }

    // 直接アクセス
    bgfx::TextureHandle getFontAtlasTexture(uint16_t pageIndex) {
        return fontAtlas_.getPageTexture(pageIndex);
    }

    //=========================================================================
    // 【Layer B】ThumbnailGridAtlas（低レベル）
    //=========================================================================

    ThumbnailGridAtlas& gridAtlas() { return gridAtlas_; }
    const ThumbnailGridAtlas& gridAtlas() const { return gridAtlas_; }

    // Grid 便利メソッド（委譲）
    bool getGridUV(ThumbnailHandle handle, float& u0, float& v0, float& u1, float& v1) const {
        return gridAtlas_.getUV(handle, u0, v0, u1, v1);
    }

    void getGridPlaceholderUV(float& u0, float& v0, float& u1, float& v1) const {
        gridAtlas_.getPlaceholderUV(u0, v0, u1, v1);
    }

    bgfx::TextureHandle getGridTexture(ThumbnailHandle handle) {
        return gridAtlas_.getTexture(handle);
    }

    bgfx::TextureHandle getGridTexture(uint16_t pageIndex) {
        return gridAtlas_.getTexture(pageIndex);
    }

    void touchGrid(ThumbnailHandle handle) {
        gridAtlas_.touch(handle);
    }

    bool isGridValid(ThumbnailHandle handle) const {
        return gridAtlas_.isValid(handle);
    }

    // サムネイル取得/作成してImageIdを返す
    ImageId acquireGridThumbnail(uint64_t contentId, bool& outNeedsLoad) {
        ImageId id = ImageIdGenerator::forThumbnail(contentId);
        ThumbnailHandle handle = gridAtlas_.acquire(contentId, outNeedsLoad);

        if (!handle.isValid()) return 0;

        {
            std::lock_guard lock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::GridAtlas;
            loc.thumbnail.handle = handle;
            loc.isPending = outNeedsLoad;
        }

        return id;
    }

    // アップロード完了通知
    void onGridUploadComplete(ImageId id) {
        std::lock_guard lock(locationMutex_);
        auto it = imageLocations_.find(id);
        if (it != imageLocations_.end()) {
            it->second.isPending = false;
        }
    }

    // 直接アクセス
    bool queueGridUpload(ImageId id, const void* data, uint16_t w, uint16_t h) {
        std::lock_guard lock(locationMutex_);
        auto it = imageLocations_.find(id);
        if (it == imageLocations_.end() || it->second.type != ImageLocation::Type::GridAtlas) {
            return false;
        }
        bool result = gridAtlas_.queueUpload(it->second.thumbnail.handle, data, w, h);
        if (result) {
            it->second.isPending = false;
            it->second.width = w;
            it->second.height = h;
        }
        return result;
    }

    bgfx::TextureHandle getGridAtlasTexture(uint16_t pageIndex) {
        return gridAtlas_.getTexture(pageIndex);
    }

    //=========================================================================
    // 【Layer B】ThumbnailShelfAtlas（低レベル）
    //=========================================================================

    ThumbnailShelfAtlas& shelfAtlas() { return shelfAtlas_; }
    const ThumbnailShelfAtlas& shelfAtlas() const { return shelfAtlas_; }

    // Shelf 便利メソッド（委譲）
    bool getShelfUV(ThumbnailHandle handle, float& u0, float& v0, float& u1, float& v1) const {
        return shelfAtlas_.getUV(handle, u0, v0, u1, v1);
    }

    void getShelfPlaceholderUV(float& u0, float& v0, float& u1, float& v1) const {
        shelfAtlas_.getPlaceholderUV(u0, v0, u1, v1);
    }

    bgfx::TextureHandle getShelfTexture(ThumbnailHandle handle) {
        return shelfAtlas_.getTexture(handle);
    }

    bgfx::TextureHandle getShelfTexture(uint16_t pageIndex) {
        return shelfAtlas_.getTexture(pageIndex);
    }

    void touchShelf(ThumbnailHandle handle) {
        shelfAtlas_.touch(handle);
    }

    bool isShelfValid(ThumbnailHandle handle) const {
        return shelfAtlas_.isValid(handle);
    }

    ImageId acquireShelfThumbnail(uint64_t contentId, uint16_t w, uint16_t h,
        bool& outNeedsLoad) {
        ImageId id = ImageIdGenerator::forThumbnail(contentId);
        ThumbnailHandle handle = shelfAtlas_.acquire(contentId, w, h, outNeedsLoad);

        if (!handle.isValid()) return 0;

        {
            std::lock_guard lock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::ShelfAtlas;
            loc.thumbnail.handle = handle;
            loc.width = w;
            loc.height = h;
            loc.isPending = outNeedsLoad;
        }

        return id;
    }

    bool queueShelfUpload(ImageId id, const void* data, uint16_t w, uint16_t h) {
        std::lock_guard lock(locationMutex_);
        auto it = imageLocations_.find(id);
        if (it == imageLocations_.end() || it->second.type != ImageLocation::Type::ShelfAtlas) {
            return false;
        }
        bool result = shelfAtlas_.queueUpload(it->second.thumbnail.handle, data, w, h);
        if (result) {
            it->second.isPending = false;
        }
        return result;
    }

    bgfx::TextureHandle getShelfAtlasTexture(uint16_t pageIndex) {
        return shelfAtlas_.getTexture(pageIndex);
    }

    //=========================================================================
    // ImageLoader から呼ばれる登録メソッド（ID指定で）
    //=========================================================================

    // FontAtlas画像を登録（ImageLoader用）
    void registerFontAtlasImage(ImageId id, uint16_t width, uint16_t height) {
        std::lock_guard lock(locationMutex_);
        auto& loc = imageLocations_[id];
        loc.type = ImageLocation::Type::FontAtlas;
        loc.font.glyphKey = id;
        loc.width = width;
        loc.height = height;
        loc.isPending = false;
    }

    // Grid画像を登録（ImageLoader用）
    void registerGridImage(ImageId id, ThumbnailHandle handle, uint16_t width, uint16_t height) {
        std::lock_guard lock(locationMutex_);
        auto& loc = imageLocations_[id];
        loc.type = ImageLocation::Type::GridAtlas;
        loc.thumbnail.handle = handle;
        loc.width = width;
        loc.height = height;
        loc.isPending = false;
        // ★ slot を atlas page texture で埋める。cmd が捕まえた &loc.slot.handle が有効化。
        //   atlas page handle 自体は安定 (page 構造体内の m_texture 値) なのでコピーで OK。
        //   evict / page 切り替え時はその上書きが必要 (TODO)。
        loc.slot.handle = gridAtlas_.getTexture(handle.pageIndex);
        loc.slot.size = { (int)width, (int)height };
        loc.slot.fbo = BGFX_INVALID_HANDLE;
    }

    // Shelf画像を登録（ImageLoader用）
    void registerShelfImage(ImageId id, ThumbnailHandle handle, uint16_t width, uint16_t height) {
        std::lock_guard lock(locationMutex_);
        auto& loc = imageLocations_[id];
        loc.type = ImageLocation::Type::ShelfAtlas;
        loc.thumbnail.handle = handle;
        loc.width = width;
        loc.height = height;
        loc.isPending = false;
        // ★ slot を atlas page texture で埋める (Grid と同様)。
        loc.slot.handle = shelfAtlas_.getTexture(handle.pageIndex);
        loc.slot.size = { (int)width, (int)height };
        loc.slot.fbo = BGFX_INVALID_HANDLE;
    }

    // Standalone作成（ID指定で、ImageLoader用）
    bool createStandaloneWithId(ImageId id, const void* pixels, uint16_t w, uint16_t h,
        int pitch, bool persistent) {
        return createStandaloneInternal(id, pixels, w, h, pitch, persistent,
            ImageOrigin::File, false);
    }

    //=========================================================================
    // Placeholder
    //=========================================================================

    ResolvedTexture getPlaceholder() {
        const auto& white = fontAtlas_.getWhitePixel();
        ResolvedTexture p;
        p.slot = fontAtlas_.getPageSlot(white.pageIndex);
        p.u0 = white.u0; p.v0 = white.v0;
        p.u1 = white.u1; p.v1 = white.v1;
        p.width = white.width;
        p.height = white.height;
        return p;
    }
public:
    // ensureTile: tiled offscreen の指定 tile のみを lazy 生成 (= 必要時に呼ぶ)。
    //   tile.handle / tile.fbo が既に valid なら何もせず true。
    //   render into / sample 直前に呼んで初めて bgfx リソースを確保することで、
    //   N ファイル open 時の「100+ tile 一斉確保 → bgfx 立て込み」を回避する。
    bool ensureTile(ImageId id, int tileIdx) {
        std::lock_guard lock(tiledMutex_);
        auto it = tiledTextures_.find(id);
        if (it == tiledTextures_.end()) return false;
        auto& info = it->second;
        if (tileIdx < 0 || tileIdx >= (int)info.tiles.size()) return false;
        auto& tile = info.tiles[tileIdx];
        if (bgfx::isValid(tile.handle) && bgfx::isValid(tile.fbo)) return true;
        if (tile.w == 0 || tile.h == 0) return false;
        bgfx::TextureHandle colorHandle = bgfx::createTexture2D(
            tile.w, tile.h, false, 1, bgfx::TextureFormat::BGRA8,
            BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
            nullptr);
        if (!bgfx::isValid(colorHandle)) return false;
        bgfx::TextureHandle depthHandle = bgfx::createTexture2D(
            tile.w, tile.h, false, 1, bgfx::TextureFormat::D24S8,
            BGFX_TEXTURE_RT, nullptr);
        if (!bgfx::isValid(depthHandle)) {
            bgfx::destroy(colorHandle);
            return false;
        }
        bgfx::Attachment att[2];
        att[0].init(colorHandle);
        att[1].init(depthHandle);
        bgfx::FrameBufferHandle fbo = bgfx::createFrameBuffer(2, att, true);
        if (!bgfx::isValid(fbo)) {
            bgfx::destroy(colorHandle);
            bgfx::destroy(depthHandle);
            return false;
        }
        tile.handle = colorHandle;
        tile.fbo    = fbo;
        initFreshRenderTargetLayout(fbo, tile.w, tile.h);
        return true;
    }

    // 新規 lazy 生成された RT FBO は Vulkan 上 VK_IMAGE_LAYOUT_UNDEFINED のまま。
    // producer pass がそのフレームで走らないと、consumer 側が UNDEFINED の
    // テクスチャを sample しようとして renderer_vk が assert する。
    // 初期化専用 view を1つ消費して clear+touch しておくと、bgfx が必ず1度
    // レンダーパスを走らせ、layout が SHADER_READ_ONLY_OPTIMAL へ遷移する。
    //
    // 通常描画 view (= renderAllCommands で 0 から積む) と衝突しないよう、
    // 初期化用 view は上位 (255 から下) を round-robin で消費する。
    void initFreshRenderTargetLayout(bgfx::FrameBufferHandle fbo, uint16_t w, uint16_t h) {
#if defined(__ANDROID__) || (defined(__linux__) && !defined(__APPLE__))
        // Vulkan only: VK_IMAGE_LAYOUT_UNDEFINED workaround. On D3D/Metal the
        // clear here runs AFTER actual rendering views (view id 255 > the low
        // ids the UI uses), so it overwrites the drawn pixels with gray. Only
        // apply on Vulkan platforms where the layout transition is needed.
        if (!bgfx::isValid(fbo) || w == 0 || h == 0) return;
        uint16_t initViewId = initViewCounter_.fetch_sub(1, std::memory_order_relaxed);
        if (initViewId < 224) {
            initViewCounter_.store(255, std::memory_order_relaxed);
            initViewId = 255;
        }
        bgfx::setViewFrameBuffer(initViewId, fbo);
        bgfx::setViewClear(initViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x808080ff, 1.0f, 0);
        bgfx::setViewRect(initViewId, 0, 0, w, h);
        bgfx::setViewScissor(initViewId, 0, 0, 0, 0);
        bgfx::touch(initViewId);
#else
        (void)fbo; (void)w; (void)h;
#endif
    }

    void destroyOffscreenInternal(ImageId id) {
        // タイル画像の場合はそちらへ委譲
        if (isTiledTexture(id)) {
            destroyTiled(id);
            return;
        }
        std::lock_guard lock(standaloneMutex_);
        auto it = standaloneTextures_.find(id);
        if (it != standaloneTextures_.end()) {
            if (bgfx::isValid(it->second.fbo)) {
                bgfx::destroy(it->second.fbo);
            }
            else if (bgfx::isValid(it->second.handle)) {
                bgfx::destroy(it->second.handle);
            }
            standaloneTextures_.erase(it);
        }

        std::lock_guard locLock(locationMutex_);
        imageLocations_.erase(id);
    }
private:
    //=========================================================================
    // 内部resolve関数
    //=========================================================================

    ResolveResult resolveStandalone(ImageId id) {
        ResolveResult result;
        std::lock_guard lock(standaloneMutex_);

        auto it = standaloneTextures_.find(id);
        if (it == standaloneTextures_.end()) {
            // entry そのものが無いときだけ NotFound。
            result.status = ResolveStatus::NotFound;
            return result;
        }

        it->second.lastUsedFrame = currentFrame_;

        // ★ handle が INVALID (= placeholder 段階、render thread が upload 途中) でも
        //   Success を返す。cmd は &it->second.handle を捕まえれば良くて、
        //   render thread が後で handle 値を埋める。
        result.status = ResolveStatus::Success;
        // StandaloneTextureInfo は TextureSlot を継承しているので暗黙 upcast 可能
        result.resolved.slot = &it->second;
        result.resolved.u0 = 0; result.resolved.v0 = 0;
        result.resolved.u1 = 1; result.resolved.v1 = 1;
        result.resolved.width = it->second.size.x;
        result.resolved.height = it->second.size.y;
        return result;
    }

    // タイル画像の互換resolve（最初のタイルを返す）
    ResolveResult resolveTiledFirstTile(ImageId id) {
        ResolveResult result;
        std::lock_guard lock(tiledMutex_);
        auto it = tiledTextures_.find(id);
        if (it == tiledTextures_.end() || it->second.tiles.empty()) {
            result.status = ResolveStatus::NotFound;
            return result;
        }

        auto& info = it->second;
        info.lastUsedFrame = currentFrame_;

        // 最初のタイルを返す（互換用、完全な描画には resolveTiledForDraw を使用）
        auto& firstTile = info.tiles[0];
        result.status = ResolveStatus::Success;
        // Tile は TextureSlot を継承しているので暗黙 upcast
        result.resolved.slot = &firstTile;
        result.resolved.u0 = 0; result.resolved.v0 = 0;
        result.resolved.u1 = 1; result.resolved.v1 = 1;
        result.resolved.width = info.totalSize.x;
        result.resolved.height = info.totalSize.y;
        return result;
    }

    ResolveResult resolveGridAtlas(ImageId id, const ImageLocation& loc) {
        ResolveResult result;

        float u0, v0, u1, v1;
        if (!gridAtlas_.getUV(loc.thumbnail.handle, u0, v0, u1, v1)) {
            // eviction されている可能性
            result.status = ResolveStatus::Evicted;
            return result;
        }

        gridAtlas_.touch(loc.thumbnail.handle);

        result.status = ResolveStatus::Success;
        // imageLocations_[id].slot を返す (= 既に loc 参照を渡されてるのでアドレスが取れる)
        result.resolved.slot = const_cast<TextureSlot*>(&loc.slot);
        result.resolved.u0 = u0; result.resolved.v0 = v0;
        result.resolved.u1 = u1; result.resolved.v1 = v1;
        result.resolved.width = loc.width;
        result.resolved.height = loc.height;
        return result;
    }

    ResolveResult resolveShelfAtlas(ImageId id, const ImageLocation& loc) {
        ResolveResult result;

        float u0, v0, u1, v1;
        if (!shelfAtlas_.getUV(loc.thumbnail.handle, u0, v0, u1, v1)) {
            result.status = ResolveStatus::Evicted;
            return result;
        }

        shelfAtlas_.touch(loc.thumbnail.handle);

        result.status = ResolveStatus::Success;
        result.resolved.slot = const_cast<TextureSlot*>(&loc.slot);
        result.resolved.u0 = u0; result.resolved.v0 = v0;
        result.resolved.u1 = u1; result.resolved.v1 = v1;
        result.resolved.width = loc.width;
        result.resolved.height = loc.height;
        return result;
    }

    //=========================================================================
    // タイル分割判定ヘルパー
    //=========================================================================

    // 辺長超過 or 面積超過でタイル分割が必要か判定
    // 入力は int (= 合計コンテンツ寸法は uint16_t を超え得る; uint16_t 受けだと
    // 67600 → 2064 のように truncate されて「タイル不要」と誤判定される)。
    // 各タイル単辺は maxTileSize_ ≤ 8192 で uint16_t に収まるので
    // タイル分割側のメタ計算は引き続き uint16_t で OK。
    bool shouldTile(int w, int h) const {
        if (w > (int)maxTileSize_ || h > (int)maxTileSize_) return true;
        if ((uint64_t)w * (uint64_t)h > maxTextureArea_) return true;
        return false;
    }

    // タイル辺長を計算 (片軸を均等分割、もう片軸は面積予算ぶん伸長)。
    //
    //   方針 (シンプル化):
    //     1. 「調整する軸」だけを均等分割 (default: X 軸 = width)
    //        → nx = ceil(w / maxTileSize_)、tw = ceil(w / nx)
    //     2. もう片軸は減った tw に応じて、面積予算 (maxTextureArea_) と
    //        maxTileSize_ の範囲で目一杯まで伸長
    //        → th = min(maxTileSize_, h, maxTextureArea_ / tw)
    //
    //   maxTileSize_ は initialize() で min(GPU上限, 8192) に設定される。
    //   PC では大抵 8192 (≈8000) まで使える。コードは maxTileSize_ を参照するので
    //   (4096 のような) ハードコード上限ではなく、ランタイム値で動く。
    //   maxTextureArea_ = 16M (64MB / RGBA8) なので、片辺 8192 のときは
    //   もう片辺は 16M/8192 = 2048 まで。
    //
    //   例 (post-initialize: maxTileSize_=8192, maxTextureArea_=16M):
    //     w=4999, h=4999  → nx=1, tw=4999, th=min(8192, 4999, 16M/4999=3200)=3200
    //                       → 1×2、各 4999×3200 (h 方向に分割)
    //     w=1000, h=10000 → nx=1, tw=1000, th=min(8192, 10000, 16M/1000=16000)=8192
    //                       → 1×2、各 1000×8192 (h 方向に最大限伸長 = 8192)
    //     w=10000, h=1000 → nx=2, tw=5000, th=min(8192, 1000, 16M/5000=3200)=1000
    //                       → 2×1、各 5000×1000
    //     w=12000, h=12000 → nx=2, tw=6000, th=min(8192, 12000, 16M/6000=2730)=2730
    //                       → 2×5 (12000/2730=4.4→5)、各 6000×2730
    //
    //   adjustX=false にすると軸を逆転 (Y を分割、X を伸長)。
    // 入力 w/h は **合計コンテンツ寸法** で int (= uint16_t を超える可能性あり)。
    // 出力 tw/th は **タイル単辺長** で uint16_t (= maxTileSize_ ≤ 8192 で必ず収まる)。
    void computeEffectiveTileSize(int w, int h, uint16_t& tw, uint16_t& th,
                                  bool adjustX = true,
                                  uint16_t pageW = 0, uint16_t pageH = 0) const {
        if (w <= 0 || h <= 0) { tw = (uint16_t)std::max(0, w); th = (uint16_t)std::max(0, h); return; }
        if (adjustX) {
            // X 軸を均等分割
            int nx = (w + (int)maxTileSize_ - 1) / (int)maxTileSize_;
            if (nx < 1) nx = 1;
            int twi = (w + nx - 1) / nx;
            tw = (uint16_t)std::min(twi, (int)maxTileSize_);
            // Y 軸は面積予算ぶん伸長
            uint64_t maxThByArea = (tw > 0) ? (maxTextureArea_ / tw) : (uint64_t)maxTileSize_;
            uint64_t cap = std::min({ (uint64_t)maxTileSize_, (uint64_t)h, maxThByArea });
            th = (cap > 0) ? (uint16_t)cap : (uint16_t)std::min(h, (int)maxTileSize_);
        } else {
            // Y 軸を均等分割
            int ny = (h + (int)maxTileSize_ - 1) / (int)maxTileSize_;
            if (ny < 1) ny = 1;
            int thi = (h + ny - 1) / ny;
            th = (uint16_t)std::min(thi, (int)maxTileSize_);
            // X 軸は面積予算ぶん伸長
            uint64_t maxTwByArea = (th > 0) ? (maxTextureArea_ / th) : (uint64_t)maxTileSize_;
            uint64_t cap = std::min({ (uint64_t)maxTileSize_, (uint64_t)w, maxTwByArea });
            tw = (cap > 0) ? (uint16_t)cap : (uint16_t)std::min(w, (int)maxTileSize_);
        }
        // ページめくり用: タイル辺長をページサイズの倍数に丸める。
        // これで任意のページ (pos = K * pageW, pos = K * pageH) が
        // 必ず単一タイル内に収まり、めくり時に 2 タイル間で完結できる。
        // ページがタイル上限より大きい異常ケースは丸めず素通り (curl 時はフォールバック判定)。
        if (pageW > 0 && tw >= pageW) tw = (uint16_t)((tw / pageW) * pageW);
        if (pageH > 0 && th >= pageH) th = (uint16_t)((th / pageH) * pageH);
    }
    // 旧 API 互換 (正方形タイル想定の場所が残っていたら使う)。新規利用は非推奨。
    uint16_t computeEffectiveTileSize(int w, int h) const {
        uint16_t tw, th;
        computeEffectiveTileSize(w, h, tw, th);
        return std::min(tw, th);
    }

    //=========================================================================
    // 内部作成関数
    //=========================================================================

    bool createStandaloneInternal(ImageId id, const void* pixels, uint16_t w, uint16_t h,
        int pitch, bool persistent, ImageOrigin origin,
        bool isRenderTarget) {
        // 巨大画像 → タイル分割パスへ（辺長超過 or 面積超過）
        if (shouldTile(w, h)) {
            if (isRenderTarget) {
                return createTiledOffscreenInternal(id, w, h, persistent);
            }
            return createTiledInternal(id, pixels, w, h, pitch, persistent, origin);
        }
        std::lock_guard lock(standaloneMutex_);

        std::vector<uint8_t> data(w * h * 4);
        const uint8_t* src = (const uint8_t*)pixels;
        for (uint16_t y = 0; y < h; ++y) {
            std::memcpy(data.data() + y * w * 4, src + y * pitch, w * 4);
        }

        uint64_t flags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;
        if (isRenderTarget) flags |= BGFX_TEXTURE_RT;

        bgfx::TextureHandle handle = bgfx::createTexture2D(
            w, h, false, 1, bgfx::TextureFormat::RGBA8, flags,
            bgfx::copy(data.data(), (uint32_t)data.size()));

        if (!bgfx::isValid(handle)) return false;

        auto& info = standaloneTextures_[id];
        info.handle = handle;
        info.size.x = w;
        info.size.y = h;
        info.refCount = 1;
        info.lastUsedFrame = currentFrame_;
        info.persistent = persistent;
        info.origin = origin;
        info.isRenderTarget = isRenderTarget;

        // 配置情報を登録 + slot 同期 (cmd 側が &loc.slot.handle を握ってる前提)
        {
            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::Standalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = false;
            // ★ stable slot を実 handle で埋める。cmd が捕まえた &loc.slot.handle が
            //   有効になる瞬間。size / fbo もここで同期 (Standalone は fbo INVALID)。
            loc.slot.handle = handle;
            loc.slot.size = { (int)w, (int)h };
            loc.slot.fbo = info.fbo;
        }

        return true;
    }
    //=========================================================================
    // タイル分割作成関数（巨大画像用）
    //=========================================================================
    bool createTiledInternal(ImageId id, const void* pixels, uint16_t w, uint16_t h,
        int pitch, bool persistent, ImageOrigin origin) {
        std::lock_guard lock(tiledMutex_);

        uint16_t tsx, tsy;
        computeEffectiveTileSize(w, h, tsx, tsy);
        TiledTextureInfo info;
        info.tileSize  = tsx;
        info.tileSizeY = tsy;
        info.tilesX = (w + tsx - 1) / tsx;
        info.tilesY = (h + tsy - 1) / tsy;
        info.totalSize = { w, h };
        info.persistent = persistent;
        info.origin = origin;

        const uint8_t* src = (const uint8_t*)pixels;

        for (int ty = 0; ty < info.tilesY; ++ty) {
            for (int tx = 0; tx < info.tilesX; ++tx) {
                uint16_t tileX = (uint16_t)(tx * tsx);
                uint16_t tileY = (uint16_t)(ty * tsy);
                uint16_t tileW = std::min((uint16_t)(w - tileX), tsx);
                uint16_t tileH = std::min((uint16_t)(h - tileY), tsy);

                // このタイル領域のピクセルをコピー
                std::vector<uint8_t> tileData(tileW * tileH * 4);
                for (uint16_t row = 0; row < tileH; ++row) {
                    std::memcpy(
                        tileData.data() + row * tileW * 4,
                        src + (tileY + row) * pitch + tileX * 4,
                        tileW * 4);
                }

                bgfx::TextureHandle handle = bgfx::createTexture2D(
                    tileW, tileH, false, 1, bgfx::TextureFormat::RGBA8,
                    BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
                    bgfx::copy(tileData.data(), (uint32_t)tileData.size()));

                if (!bgfx::isValid(handle)) {
                    // ロールバック: 作成済みタイルを全破棄
                    for (auto& t : info.tiles) bgfx::destroy(t.handle);
                    return false;
                }

                info.tiles.push_back({ handle, BGFX_INVALID_HANDLE, tileX, tileY, tileW, tileH, {(int)tileW, (int)tileH} });
            }
        }

        info.refCount = 1;
        info.lastUsedFrame = currentFrame_;
        tiledTextures_[id] = std::move(info);

        // 配置情報を登録
        {
            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::TiledStandalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = false;
        }

        return true;
    }

    //=========================================================================
    // タイル分割空テクスチャ作成関数
    //=========================================================================
    bool createTiledEmptyInternal(ImageId id, uint16_t w, uint16_t h, bool persistent) {
        std::lock_guard lock(tiledMutex_);

        uint16_t tsx, tsy;
        computeEffectiveTileSize(w, h, tsx, tsy);
        TiledTextureInfo info;
        info.tileSize  = tsx;
        info.tileSizeY = tsy;
        info.tilesX = (w + tsx - 1) / tsx;
        info.tilesY = (h + tsy - 1) / tsy;
        info.totalSize = { w, h };
        info.persistent = persistent;
        info.origin = ImageOrigin::Memory;

        for (int ty = 0; ty < info.tilesY; ++ty) {
            for (int tx = 0; tx < info.tilesX; ++tx) {
                uint16_t tileX = (uint16_t)(tx * tsx);
                uint16_t tileY = (uint16_t)(ty * tsy);
                uint16_t tileW = std::min((uint16_t)(w - tileX), tsx);
                uint16_t tileH = std::min((uint16_t)(h - tileY), tsy);

                bgfx::TextureHandle handle = bgfx::createTexture2D(
                    tileW, tileH, false, 1, bgfx::TextureFormat::RGBA8,
                    BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
                    nullptr);

                if (!bgfx::isValid(handle)) {
                    for (auto& t : info.tiles) bgfx::destroy(t.handle);
                    return false;
                }

                info.tiles.push_back({ handle, BGFX_INVALID_HANDLE, tileX, tileY, tileW, tileH, {(int)tileW, (int)tileH} });
            }
        }

        info.refCount = 1;
        info.lastUsedFrame = currentFrame_;
        tiledTextures_[id] = std::move(info);

        {
            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::TiledStandalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = false;
        }

        return true;
    }

    //=========================================================================
    // タイル分割オフスクリーン作成関数（巨大FBO用）
    //=========================================================================
    bool createTiledOffscreenInternal(ImageId id, int w, int h, bool persistent,
                                      uint16_t pageW = 0, uint16_t pageH = 0) {
        std::lock_guard lock(tiledMutex_);

        uint16_t tsx, tsy;
        // pageW/pageH > 0 のとき、tile.w/tile.h はその倍数に丸まる
        // → ページめくり時に 1 ページが必ず単一タイル内に収まる
        computeEffectiveTileSize(w, h, tsx, tsy, /*adjustX=*/true, pageW, pageH);
        int tilesX_ = (w + tsx - 1) / tsx;
        int tilesY_ = (h + tsy - 1) / tsy;

        // ★ producer 側 (Table の pushDisplay 等) が `&tile.handle` をポインタとして
        //   cmd に保存しているので、tiles ベクタを std::move で別バッファに置換すると
        //   保存ポインタが dangling になる。既存 entry がある場合は **in-place 更新** し、
        //   tiles ベクタのバッファアドレスを保つ。
        //   queueOffscreenResize / type 遷移経路で予め placeholder tiles が入っているのが普通だが、
        //   無い場合 (= 完全新規) は通常通り emplace する。
        TiledTextureInfo& info = tiledTextures_[id];  // 無ければ default-構築で挿入される
        info.tileSize  = tsx;
        info.tileSizeY = tsy;
        info.tilesX = tilesX_;
        info.tilesY = tilesY_;
        info.totalSize = { w, h };
        info.persistent = persistent;
        info.origin = ImageOrigin::Offscreen;
        info.isRenderTarget = true;
        info.refCount = 1;
        info.lastUsedFrame = currentFrame_;

        // 既存タイル数が一致しないなら resize する (= バッファアドレスは変わるが、
        // この経路は producer が古いポインタを保持していない初回 fresh ケースのみ)。
        // 一致するなら resize しないで in-place 更新 → producer のポインタが安定。
        size_t expectedTiles = (size_t)tilesX_ * (size_t)tilesY_;
        if (info.tiles.size() != expectedTiles) {
            info.tiles.clear();
            info.tiles.reserve(expectedTiles);
            info.tiles.resize(expectedTiles);
        }

        // 各タイルの **メタ情報だけ** 設定し、bgfx テクスチャは作らない (= lazy)。
        // tile.handle / tile.fbo は INVALID のまま → ensureTile() で初回利用時に生成。
        // これで「3 ファイルを開いた途端に 100+ タイルの GPU 確保が立て込む」問題を回避。
        for (int ty = 0; ty < tilesY_; ++ty) {
            for (int tx = 0; tx < tilesX_; ++tx) {
                int tileX = tx * (int)tsx;
                int tileY = ty * (int)tsy;
                uint16_t tileW = (uint16_t)std::min(w - tileX, (int)tsx);
                uint16_t tileH = (uint16_t)std::min(h - tileY, (int)tsy);
                size_t idx = (size_t)ty * (size_t)tilesX_ + (size_t)tx;
                auto& tile = info.tiles[idx];
                // 既存 fbo/handle が valid なら sizes が変わっていれば破棄、変わってなければ流用。
                bool sizeMatch = (tile.w == tileW && tile.h == tileH);
                if (!sizeMatch) {
                    if (bgfx::isValid(tile.fbo))    { bgfx::destroy(tile.fbo);    tile.fbo    = BGFX_INVALID_HANDLE; }
                    if (bgfx::isValid(tile.handle)) { bgfx::destroy(tile.handle); tile.handle = BGFX_INVALID_HANDLE; }
                }
                tile.x      = tileX;
                tile.y      = tileY;
                tile.w      = tileW;
                tile.h      = tileH;
                tile.size   = { (int)tileW, (int)tileH };
                // handle / fbo は INVALID のまま放置。ensureTile() で必要時に生成。
            }
        }

        // 配置情報を登録
        {
            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::TiledStandalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = false;
        }

        return true;
    }

    //=========================================================================
    // タイル分割オフスクリーンリサイズ（破棄→再作成）
    //=========================================================================
    bool resizeTiledOffscreenInternal(ImageId id, int newW, int newH,
                                      uint16_t pageW = 0, uint16_t pageH = 0) {
        // 現在のサイズチェック (= isRenderTarget の確認のみ)。
        // totalSize / tiles[].handle ともに queueOffscreenResize で予測値に置き換わっているので、
        // 「同サイズだから skip」と判定すると古い FBO が残ったまま新サイズで使われ事故になる。
        // caller 側で必要時のみ呼ばれている前提で、ここでは early-out せず常に再生成する。
        {
            std::lock_guard lock(tiledMutex_);
            auto it = tiledTextures_.find(id);
            if (it == tiledTextures_.end() || !it->second.isRenderTarget) return false;
        }

        // ★ destroyTiled(id) は entry を erase する → producer が cmd に保存した
        //   `&tile.handle` ポインタが dangling になる。代わりに createTiledOffscreenInternal
        //   の in-place 更新に任せる (queueOffscreenResize 側で古い fbo/handle は既に
        //   destroy + INVALID 化済なので二重 destroy にはならない)。
        bool persistent = false;
        {
            std::lock_guard lock(tiledMutex_);
            auto it = tiledTextures_.find(id);
            if (it != tiledTextures_.end()) persistent = it->second.persistent;
        }

        if (shouldTile(newW, newH)) {
            return createTiledOffscreenInternal(id, newW, newH, persistent, pageW, pageH);
        }
        // 新サイズが小さくなってタイル不要 → 通常Offscreenに戻る (= 型遷移なので erase 必要)
        destroyTiled(id);
        return createOffscreenInternal(id, newW, newH, persistent, pageW, pageH);
    }

    bool createOffscreenInternal(ImageId id, int w, int h, bool persistent,
                                 uint16_t pageW = 0, uint16_t pageH = 0) {
        // 巨大オフスクリーン → タイル分割パスへ
        if (shouldTile(w, h)) {
            return createTiledOffscreenInternal(id, w, h, persistent, pageW, pageH);
        }
        // ここに到達した時点で shouldTile=false → 両軸とも maxTileSize_ ≤ 8192 以下なので
        // uint16_t に収まる。bgfx 直 API 用に narrow する。
        uint16_t w16 = (uint16_t)w;
        uint16_t h16 = (uint16_t)h;
        std::lock_guard lock(standaloneMutex_);

        // Use BGRA8 for Metal compatibility
        bgfx::TextureHandle handle = bgfx::createTexture2D(
            w16, h16, false, 1, bgfx::TextureFormat::BGRA8,
            BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
            nullptr);

        if (!bgfx::isValid(handle)) return false;

        // Create depth texture for offscreen framebuffer (required for Metal depth testing)
        bgfx::TextureHandle depthHandle = bgfx::createTexture2D(
            w16, h16, false, 1, bgfx::TextureFormat::D24S8,
            BGFX_TEXTURE_RT, nullptr);

        if (!bgfx::isValid(depthHandle)) {
            bgfx::destroy(handle);
            return false;
        }

        // Use bgfx::Attachment to properly attach color and depth
        bgfx::Attachment att[2];
        att[0].init(handle);      // color attachment
        att[1].init(depthHandle); // depth attachment

        bgfx::FrameBufferHandle fbo = bgfx::createFrameBuffer(2, att, true);
        if (!bgfx::isValid(fbo)) {
            bgfx::destroy(handle);
            bgfx::destroy(depthHandle);
            return false;
        }

        auto& info = standaloneTextures_[id];
        info.handle = handle;
        info.fbo = fbo;
        info.size.x = w;
        info.size.y = h;
        info.refCount = 1;
        info.lastUsedFrame = currentFrame_;
        info.persistent = persistent;
        info.origin = ImageOrigin::Offscreen;
        info.isRenderTarget = true;
        initFreshRenderTargetLayout(fbo, (uint16_t)w, (uint16_t)h);

        {
            std::lock_guard locLock(locationMutex_);
            auto& loc = imageLocations_[id];
            loc.type = ImageLocation::Type::Standalone;
            loc.width = w;
            loc.height = h;
            loc.isPending = false;
        }

        return true;
    }

    //=========================================================================
    // メンバ
    //=========================================================================

    Config config_;
    uint64_t currentFrame_ = 0;

    // Atlas
    FontAtlas fontAtlas_;
    ThumbnailGridAtlas gridAtlas_;
    ThumbnailShelfAtlas shelfAtlas_;

    // Standalone textures
    mutable std::mutex standaloneMutex_;
    std::unordered_map<ImageId, StandaloneTextureInfo> standaloneTextures_;

    // Tiled textures（巨大画像タイル分割）
    mutable std::mutex tiledMutex_;
    std::unordered_map<ImageId, TiledTextureInfo> tiledTextures_;
    // タイル 1 枚の最大辺長 (GPU 限界から設定)。
    // 端末ごとに大きく異なる:
    //   PC / iOS 近年: 8192 (initialize() で min(caps, 8192) に補正される)
    //   古い Android  : 2048〜4096
    // initialize() 前にも create が走り得るので、安全側のデフォルト=2048。
    uint16_t maxTileSize_ = 2048;
    // 面積閾値 (1 タイルあたりのピクセル予算)。
    //   16M px = 64MB RGBA8。8192*2048 / 4096*4096 がちょうど 16M。
    //   モバイルでも余裕。PC で広く取りたければ initialize() で引き上げ可。
    uint64_t maxTextureArea_ = 16 * 1024 * 1024;

    // Group Manager
    ImageMasterGroupManager groupManager_;

public:
    //=========================================================================
    // RenderGroup 管理（委譲 + 拡張）
    //=========================================================================

    ExtendedRenderGroup& createGroup(SurfaceId surfaceId,
        RenderGroupFlags flags = RenderGroupFlags::None) {
        return groupManager_.createGroup(surfaceId, flags);
    }

    ExtendedRenderGroup* getGroup(RenderGroupId id) {
        return groupManager_.getGroup(id);
    }

    void destroyGroup(RenderGroupId id) {
        auto* g = groupManager_.getGroup(id);
        if (!g) return;

        // FontAtlas への通知
        fontAtlas_.onGroupDestroyed(*g);

        // Standalone texture の参照解放
        for (auto texId : g->usedStandaloneTextures) {
            release(texId);
        }

        // Surface リストから削除
        auto& sv = groupManager_.surfaceGroups_[g->surfaceId];
        sv.erase(std::remove(sv.begin(), sv.end(), id), sv.end());

        groupManager_.groups_.erase(id);
    }

    void destroySurfaceGroups(SurfaceId surfaceId) {
        auto it = groupManager_.surfaceGroups_.find(surfaceId);
        if (it == groupManager_.surfaceGroups_.end()) return;

        for (RenderGroupId gid : it->second) {
            auto git = groupManager_.groups_.find(gid);
            if (git != groupManager_.groups_.end()) {
                fontAtlas_.onGroupDestroyed(git->second);
                for (auto texId : git->second.usedStandaloneTextures) {
                    release(texId);
                }
                groupManager_.groups_.erase(git);
            }
        }
        groupManager_.surfaceGroups_.erase(it);
    }

    void collectExpiredGroups(uint32_t expireFrames = 300) {
        std::vector<RenderGroupId> expired;
        for (auto& [id, g] : groupManager_.groups_) {
            if (g.isPinned()) continue;
            if (groupManager_.currentFrame_ - g.lastUsedFrame > expireFrames) {
                expired.push_back(id);
            }
        }
        for (RenderGroupId id : expired) {
            destroyGroup(id);
        }
    }

    ImageMasterGroupManager& groupManager() { return groupManager_; }

    // ImageId → 配置情報のマッピング
    mutable std::mutex locationMutex_;
    std::unordered_map<ImageId, ImageLocation> imageLocations_;

    std::atomic<uint64_t> nextFontAtlasImageId_{ 0x1000000 };

    // 新規 lazy 確保したタイル/standalone FBO の Vulkan layout 初期化用 view 枠
    // (= 上位から下に消費)。renderAllCommands は 0 から積み上げるので
    // 224..255 を初期化専用に使う想定。
    std::atomic<uint16_t> initViewCounter_{ 255 };
};



//=============================================================================
// ImageSource - 画像ソース
//=============================================================================
struct ImageSource {
    enum class Type { File, Memory, SDL_Surface };

    Type type = Type::File;
    std::string path;
    const void* data = nullptr;
    size_t dataSize = 0;
    SDL_Surface* surface = nullptr;

    static ImageSource fromFile(const std::string& path) {
        return { Type::File, path, nullptr, 0, nullptr };
    }
    static ImageSource fromMemory(const void* data, size_t size) {
        return { Type::Memory, "", data, size, nullptr };
    }
    static ImageSource fromSurface(SDL_Surface* surface) {
        return { Type::SDL_Surface, "", nullptr, 0, surface };
    }
};

//=============================================================================
// ImageUsage - 配置ヒント
//=============================================================================


//=============================================================================
// DecodedImage
//=============================================================================
struct DecodedImage {
    std::vector<uint8_t> pixels;  // RGBA8888
    uint16_t width = 0;
    uint16_t height = 0;

    bool isValid() const { return !pixels.empty() && width > 0 && height > 0; }
};

using FileDecoder = std::function<DecodedImage(const std::string& path)>;
using MemoryDecoder = std::function<DecodedImage(const void* data, size_t size)>;

//=============================================================================
// PlacementPolicy - 配置ポリシー
//=============================================================================
struct PlacementPolicy {
    uint16_t gridMaxSize = 128;
    uint16_t shelfMaxHeight = 160;
    uint16_t shelfMaxWidth = 512;
    float shelfMinAspect = 1.2f;
    uint16_t fontAtlasMaxSize = 64;
    size_t maxCpuCacheBytes = 128 * 1024 * 1024;  // 128MB
    uint32_t cacheExpireFrames = 600;
};

//=============================================================================
// CachedImage - CPUキャッシュエントリ
//=============================================================================
struct CachedImage {
    ImageId imageId = 0;
    std::vector<uint8_t> pixels;  // RGBA
    uint16_t width = 0;
    uint16_t height = 0;
    ImageUsage usage = ImageUsage::Auto;
    bool persistent = false;
    uint64_t lastUsedFrame = 0;
    uint32_t refCount = 0;
    bool isUploaded = false;

    size_t byteSize() const { return pixels.size(); }
};

//=============================================================================
// LoadRequest - 非同期ロードリクエスト
//=============================================================================
struct LoadRequest {
    ImageId imageId;              // ※ 変更: 事前に確定されたID
    ImageSource source;
    ImageUsage usage;
    bool persistent;
    std::function<void(ImageId, bool success)> callback;  // ※ 変更: ImageId を返す
};

//=============================================================================
// ImageLoader
//=============================================================================
class ImageLoader {
public:
    explicit ImageLoader(ImageMaster& master, const PlacementPolicy& policy = {})
        : master_(master), policy_(policy) {
        setDefaultDecoders();
    }

    ~ImageLoader() { shutdown(); }

    //=========================================================================
    // デコーダー設定
    //=========================================================================
    void setFileDecoder(FileDecoder decoder) {
        fileDecoder_ = std::move(decoder);
    }

    void setMemoryDecoder(MemoryDecoder decoder) {
        memoryDecoder_ = std::move(decoder);
    }

    //=========================================================================
    // 初期化・終了
    //=========================================================================
    bool initialize(int workerThreads = 2) {
        if (running_) return true;

        running_ = true;
        for (int i = 0; i < workerThreads; ++i) {
            workers_.emplace_back(&ImageLoader::workerThread, this);
        }
        return true;
    }

    void shutdown() {
        {
            std::lock_guard lock(queueMutex_);
            running_ = false;
        }
        queueCv_.notify_all();

        for (auto& t : workers_) {
            if (t.joinable()) t.join();
        }
        workers_.clear();

        std::lock_guard lock(cacheMutex_);
        cache_.clear();
    }

    //=========================================================================
    // 同期ロード - ImageId を返す（変更）
    //=========================================================================
    ImageId loadSync(const ImageSource& source, ImageUsage usage = ImageUsage::Auto,
        bool persistent = false) {
        // ImageId を事前に決定
        ImageId imageId = computeImageId(source);

        // 既にアップロード済みなら即返す
        if (master_.exists(imageId) && !master_.isPending(imageId)) {
            // touch して返す
            master_.touch(imageId);
            std::lock_guard lock(cacheMutex_);
            auto it = cache_.find(imageId);
            if (it != cache_.end()) {
                it->second.lastUsedFrame = currentFrame_;
            }
            return imageId;
        }

        // デコード
        auto decoded = decodeImage(source);
        if (!decoded.isValid()) {
            return 0;
        }

        // キャッシュに登録
        {
            std::lock_guard lock(cacheMutex_);
            auto& cached = cache_[imageId];
            cached.imageId = imageId;
            cached.pixels = std::move(decoded.pixels);
            cached.width = decoded.width;
            cached.height = decoded.height;
            cached.usage = usage;
            cached.persistent = persistent;
            cached.lastUsedFrame = currentFrame_;
            cached.refCount = 1;
        }

        // 配置タイプを事前登録（resolve()がGPUアップロード前でもルーティングできるように）
        auto placementType = decidePlacement(decoded.width, decoded.height, usage);
        master_.registerPendingWithType(imageId, placementType);

        // ★ placement に応じて slot を GoThread で先に確保する (= 初回描画でも
        //   cmd が掴むポインタアドレスを安定化させる)。bgfx は呼ばない:
        //   ・Standalone: standaloneTextures_[id] + imageLocations_[id].slot を作る
        //   ・GridAtlas:  gridAtlas_.acquire (page 0 は init 済みなので bgfx 不要)
        //                + imageLocations_[id].slot.handle に page texture を代入
        //   ・ShelfAtlas: 同上
        //   ・FontAtlas:  glyph 単位なのでここでは扱わない (drawText 経路で別途処理)
        if (placementType == ImageLocation::Type::Standalone) {
            master_.reserveStandalonePlaceholder(imageId, decoded.width, decoded.height,
                                                  usage, persistent);
        }
        else if (placementType == ImageLocation::Type::GridAtlas) {
            uint64_t contentId = getImageIdLocal(imageId);
            master_.reserveGridPlaceholder(imageId, contentId,
                                            decoded.width, decoded.height);
        }
        else if (placementType == ImageLocation::Type::ShelfAtlas) {
            uint64_t contentId = getImageIdLocal(imageId);
            master_.reserveShelfPlaceholder(imageId, contentId,
                                             decoded.width, decoded.height);
        }

        // GPU にアップロード（レンダースレッドにキューイング）
        if (!uploadToGpu(imageId)) {
            return 0;
        }

        return imageId;
    }

    // ファイルから - ImageId を返す（変更）
    ImageId loadFromFile(const std::string& path, ImageUsage usage = ImageUsage::Auto,
        bool persistent = false) {
        return loadSync(ImageSource::fromFile(path), usage, persistent);
    }

    // メモリから - ImageId を返す（変更）
    ImageId loadFromMemory(const void* data, size_t size,
        ImageUsage usage = ImageUsage::Auto,
        bool persistent = false) {
        return loadSync(ImageSource::fromMemory(data, size), usage, persistent);
    }

    // SDL_Surface から - ImageId を返す（変更）
    ImageId loadFromSurface(SDL_Surface* surface, ImageUsage usage = ImageUsage::Auto,
        bool persistent = false) {
        return loadSync(ImageSource::fromSurface(surface), usage, persistent);
    }

    //=========================================================================
    // 非同期ロード - ImageId を返す（変更: Pending状態で即座に返す）
    //=========================================================================
    ImageId loadAsync(const ImageSource& source, ImageUsage usage = ImageUsage::Auto,
        bool persistent = false,
        std::function<void(ImageId, bool success)> callback = nullptr) {
        // ImageId を事前に決定
        ImageId imageId = computeImageId(source);

        // 既にアップロード済みなら即返却
        if (master_.exists(imageId) && !master_.isPending(imageId)) {
            if (callback) callback(imageId, true);
            return imageId;
        }

        // 既に Pending なら重複ロードしない
        if (master_.isPending(imageId)) {
            // コールバックだけ登録
            if (callback) {
                std::lock_guard lock(pendingCallbacksMutex_);
                pendingCallbacks_[imageId].push_back(callback);
            }
            return imageId;
        }

        // Pending として ImageMaster に登録
        master_.registerPending(imageId);

        LoadRequest req;
        req.imageId = imageId;
        req.source = source;
        req.usage = usage;
        req.persistent = persistent;
        req.callback = callback;

        {
            std::lock_guard lock(queueMutex_);
            loadQueue_.push(std::move(req));
        }
        queueCv_.notify_one();

        return imageId;  // Pending 状態の ImageId を即座に返す
    }

    //=========================================================================
    // ImageId 事前計算（新規）
    //=========================================================================
    static ImageId computeImageId(const ImageSource& source) {
        switch (source.type) {
        case ImageSource::Type::File:
            return ImageIdGenerator::fromFile(source.path);
        case ImageSource::Type::Memory:
        case ImageSource::Type::SDL_Surface:
            return ImageIdGenerator::fromMemory();
        default:
            return 0;
        }
    }

    //=========================================================================
    // 状態確認（変更: ImageMaster に委譲）
    //=========================================================================
    bool isLoaded(ImageId imageId) const {
        return master_.exists(imageId) && !master_.isPending(imageId);
    }

    bool isPending(ImageId imageId) const {
        return master_.isPending(imageId);
    }

    // 旧API互換
    bool isUploaded(ImageId imageId) const {
        return isLoaded(imageId);
    }

    //=========================================================================
    // getDrawInfo は削除 - ImageMaster.resolve() を使用
    //=========================================================================
    //
    // 【旧】
    // bgfx::TextureHandle tex;
    // float u0, v0, u1, v1;
    // if (loader.getDrawInfo(handle, tex, u0, v0, u1, v1)) { ... }
    //
    // 【新】
    // auto resolved = master.resolveForDraw(imageId);
    // if (resolved.isValid()) {
    //     // resolved.texture, resolved.u0, v0, u1, v1
    // }
    //

    //=========================================================================
    // 参照カウント
    //=========================================================================
    void retain(ImageId imageId) {
        master_.retain(imageId);
        std::lock_guard lock(cacheMutex_);
        auto it = cache_.find(imageId);
        if (it != cache_.end()) {
            ++it->second.refCount;
        }
    }

    void release(ImageId imageId) {
        master_.release(imageId);
        std::lock_guard lock(cacheMutex_);
        auto it = cache_.find(imageId);
        if (it != cache_.end() && it->second.refCount > 0) {
            --it->second.refCount;
        }
    }

    //=========================================================================
    // eviction 通知（ImageMaster から呼ばれる）
    //=========================================================================
    void onEvicted(ImageId id) {
        std::lock_guard lock(cacheMutex_);
        auto it = cache_.find(id);
        if (it != cache_.end()) {
            it->second.isUploaded = false;
        }
    }

    //=========================================================================
    // 再アップロード（eviction 後に再度必要になった場合）
    //=========================================================================
    bool reupload(ImageId imageId) {
        std::lock_guard lock(cacheMutex_);
        auto it = cache_.find(imageId);
        if (it == cache_.end() || it->second.pixels.empty()) {
            return false;
        }
        return uploadToGpuInternal(it->second);
    }

    //=========================================================================
    // フレーム処理
    //=========================================================================
    void beginFrame() {
        ++currentFrame_;
        processCompletedLoads();
    }

    //=========================================================================
    // CPUキャッシュ管理
    //=========================================================================
    void collectGarbage() {
        std::lock_guard lock(cacheMutex_);

        std::vector<ImageId> toRemove;
        size_t totalBytes = 0;

        for (auto& [id, cached] : cache_) {
            totalBytes += cached.byteSize();

            if (cached.persistent || cached.refCount > 0) continue;
            if (currentFrame_ - cached.lastUsedFrame > policy_.cacheExpireFrames) {
                toRemove.push_back(id);
            }
        }

        if (totalBytes > policy_.maxCpuCacheBytes) {
            std::vector<std::pair<uint64_t, ImageId>> byAge;
            for (auto& [id, cached] : cache_) {
                if (cached.persistent || cached.refCount > 0) continue;
                byAge.emplace_back(cached.lastUsedFrame, id);
            }
            std::sort(byAge.begin(), byAge.end());

            size_t freed = 0;
            for (auto& [_, id] : byAge) {
                if (totalBytes - freed <= policy_.maxCpuCacheBytes) break;
                auto it = cache_.find(id);
                if (it != cache_.end()) {
                    freed += it->second.byteSize();
                    toRemove.push_back(id);
                }
            }
        }

        for (ImageId id : toRemove) {
            cache_.erase(id);
        }
    }

    void evictFromCpuCache(ImageId imageId) {
        std::lock_guard lock(cacheMutex_);
        auto it = cache_.find(imageId);
        if (it != cache_.end()) {
            it->second.pixels.clear();
            it->second.pixels.shrink_to_fit();
        }
    }

    //=========================================================================
    // 統計情報
    //=========================================================================
    struct Stats {
        size_t cacheEntries;
        size_t cpuCacheBytes;
        size_t uploadedCount;
        size_t pendingLoads;
        uint64_t currentFrame;
    };

    Stats getStats() const {
        std::lock_guard lock(cacheMutex_);
        Stats s{};
        s.cacheEntries = cache_.size();
        s.currentFrame = currentFrame_;

        for (const auto& [id, cached] : cache_) {
            s.cpuCacheBytes += cached.byteSize();
            if (cached.isUploaded) ++s.uploadedCount;
        }

        {
            std::lock_guard qlock(queueMutex_);
            s.pendingLoads = loadQueue_.size();
        }

        return s;
    }

    const PlacementPolicy& policy() const { return policy_; }
    PlacementPolicy& policy() { return policy_; }

    //=========================================================================
    // Process queued GPU uploads on render thread
    // Must be called from render thread (bgfx API calls require it)
    //=========================================================================
    void processGpuUploadsMainThread() {
        // ★ Atlas page の bgfx::createTexture2D を先に走らせる (= GoThread が
        //   reserveGridPlaceholder/reserveShelfPlaceholder で metadata-only な
        //   page を作っていた場合、ここで初めて実 GPU テクスチャを生成する)。
        //   この後の uploadToGpuInternal → registerGridImage で
        //   loc.slot.handle に valid な page texture をコピーするため、
        //   必ず ensure → register の順で。
        master_.gridAtlas().ensureAllBgfxTextures();
        master_.shelfAtlas().ensureAllBgfxTextures();

        std::vector<ImageId> toProcess;
        {
            std::lock_guard lock(gpuUploadQueueMutex_);
            toProcess.swap(gpuUploadQueue_);
        }

        for (ImageId imageId : toProcess) {
            std::lock_guard lock(cacheMutex_);
            auto it = cache_.find(imageId);
            if (it != cache_.end()) {
                uploadToGpuInternal(it->second);
            }
        }
    }

private:
    //=========================================================================
    // 配置先決定
    //=========================================================================
    ImageLocation::Type decidePlacement(uint16_t w, uint16_t h, ImageUsage usage) {
        switch (usage) {
        case ImageUsage::Icon:
            return ImageLocation::Type::GridAtlas;

        case ImageUsage::VideoThumb:
            return ImageLocation::Type::ShelfAtlas;

        case ImageUsage::Background:
        case ImageUsage::Dynamic:
            return ImageLocation::Type::Standalone;

        case ImageUsage::UIImage:
            if (w <= policy_.fontAtlasMaxSize && h <= policy_.fontAtlasMaxSize) {
                return ImageLocation::Type::FontAtlas;
            }
            return ImageLocation::Type::Standalone;

        case ImageUsage::Thumbnail:
        case ImageUsage::Auto:
        default:
            break;
        }

        uint16_t maxDim = std::max(w, h);
        float aspect = (h > 0) ? (float)w / h : 1.0f;

        if (maxDim <= policy_.fontAtlasMaxSize) {
            return ImageLocation::Type::FontAtlas;
        }

        if (maxDim <= policy_.gridMaxSize && aspect >= 0.8f && aspect <= 1.25f) {
            return ImageLocation::Type::GridAtlas;
        }

        if (h <= policy_.shelfMaxHeight && w <= policy_.shelfMaxWidth && aspect >= policy_.shelfMinAspect) {
            return ImageLocation::Type::ShelfAtlas;
        }

        return ImageLocation::Type::Standalone;
    }

    //=========================================================================
    // 画像デコード
    //=========================================================================
    DecodedImage decodeImage(const ImageSource& source) {
        DecodedImage decoded;

        switch (source.type) {
        case ImageSource::Type::File:
            if (fileDecoder_) {
                decoded = fileDecoder_(source.path);
            }
            break;

        case ImageSource::Type::Memory:
            if (memoryDecoder_ && source.data && source.dataSize > 0) {
                decoded = memoryDecoder_(source.data, source.dataSize);
            }
            break;

        case ImageSource::Type::SDL_Surface:
            if (source.surface) {
                decoded = decodeSdlSurface(source.surface);
            }
            break;
        }

        return decoded;
    }

    DecodedImage decodeSdlSurface(SDL_Surface* surface) {
        DecodedImage result;
        if (!surface) return result;

        SDL_Surface* rgba = surface;
        bool ownsRgba = false;
        if (surface->format != SDL_PIXELFORMAT_RGBA8888) {
            rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
            ownsRgba = true;
        }

        if (!rgba) return result;

        result.width = (uint16_t)rgba->w;
        result.height = (uint16_t)rgba->h;

        size_t rowBytes = (size_t)rgba->w * 4;
        result.pixels.resize(rowBytes * rgba->h);
        const uint8_t* src = (const uint8_t*)rgba->pixels;
        for (int y = 0; y < rgba->h; ++y) {
            std::memcpy(result.pixels.data() + y * rowBytes,
                src + y * rgba->pitch, rowBytes);
        }

        if (ownsRgba) SDL_DestroySurface(rgba);
        return result;
    }

    void setDefaultDecoders() {
        fileDecoder_ = [](const std::string& path) -> DecodedImage {
            int w, h, channels;
            unsigned char* pixels = nullptr;

            // Use PlatformIO/FileEngine for unified cross-platform file access
            auto fileData = PlatformIO::readFile(path, HopStarIO::Location::Resource);
            if (fileData.empty()) {
                SDL_Log("loadImage: Failed to read file: %s", path.c_str());
                return DecodedImage{};
            }

            pixels = stbi_load_from_memory(fileData.data(), (int)fileData.size(), &w, &h, &channels, 4);

#if TARGET_OS_IOS || TARGET_OS_SIMULATOR
            // iOS only: Swap R and B channels (RGBA -> BGRA) for Metal compatibility
            if (pixels) {
                for (int i = 0; i < w * h; ++i) {
                    std::swap(pixels[i * 4 + 0], pixels[i * 4 + 2]);
                }
            }
#endif

            if (!pixels) return DecodedImage{};

            DecodedImage result;
            result.width = (uint16_t)w;
            result.height = (uint16_t)h;
            result.pixels.assign(pixels, pixels + (size_t)w * h * 4);
            stbi_image_free(pixels);
            return result;
            };

        memoryDecoder_ = [](const void* data, size_t size) -> DecodedImage {
            int w, h, channels;
            unsigned char* pixels = stbi_load_from_memory(
                (const stbi_uc*)data, (int)size, &w, &h, &channels, 4);
            if (!pixels) return DecodedImage{};

#if TARGET_OS_IOS || TARGET_OS_SIMULATOR
            // iOS only: Swap R and B channels (RGBA -> BGRA) for Metal compatibility
            for (int i = 0; i < w * h; ++i) {
                std::swap(pixels[i * 4 + 0], pixels[i * 4 + 2]);
            }
#endif

            DecodedImage result;
            result.width = (uint16_t)w;
            result.height = (uint16_t)h;
            result.pixels.assign(pixels, pixels + (size_t)w * h * 4);
            stbi_image_free(pixels);
            return result;
            };
    }

    //=========================================================================
    // GPU アップロード - ImageMaster に登録
    //=========================================================================
    bool uploadToGpu(ImageId imageId) {
        // Queue the upload for render thread processing
        // bgfx API calls must happen on the render thread
        {
            std::lock_guard lock(gpuUploadQueueMutex_);
            gpuUploadQueue_.push_back(imageId);
        }
        return true;  // Will be processed on render thread
    }

    bool uploadToGpuInternal(CachedImage& cached) {
        if (cached.pixels.empty()) return false;

        auto placement = decidePlacement(cached.width, cached.height, cached.usage);
        bool success = false;

        switch (placement) {
        case ImageLocation::Type::FontAtlas: {
            SDL_Surface* temp = SDL_CreateSurfaceFrom(
                cached.width, cached.height,
                SDL_PIXELFORMAT_RGBA8888,
                cached.pixels.data(), cached.width * 4);
            if (temp) {
                const auto& gi = master_.fontAtlas().addImage(
                    cached.imageId, temp, nullptr, cached.persistent);
                SDL_DestroySurface(temp);
                if (gi.width > 0) {
                    master_.registerFontAtlasImage(cached.imageId, cached.width, cached.height);
                    success = true;
                }
            }
            break;
        }

        case ImageLocation::Type::GridAtlas: {
            bool needsLoad = false;
            uint64_t contentId = getImageIdLocal(cached.imageId);
            auto handle = master_.gridAtlas().acquire(contentId, needsLoad);
            if (handle.isValid()) {
                if (needsLoad) {
                    master_.gridAtlas().queueUpload(handle, cached.pixels.data(),
                        cached.width, cached.height);
                }
                master_.registerGridImage(cached.imageId, handle, cached.width, cached.height);
                success = true;
            }
            break;
        }

        case ImageLocation::Type::ShelfAtlas: {
            bool needsLoad = false;
            uint64_t contentId = getImageIdLocal(cached.imageId);
            auto handle = master_.shelfAtlas().acquire(contentId, cached.width, cached.height, needsLoad);
            if (handle.isValid()) {
                if (needsLoad) {
                    master_.shelfAtlas().queueUpload(handle, cached.pixels.data(),
                        cached.width, cached.height);
                }
                master_.registerShelfImage(cached.imageId, handle, cached.width, cached.height);
                success = true;
            }
            break;
        }

        case ImageLocation::Type::Standalone: {
            success = master_.createStandaloneWithId(
                cached.imageId, cached.pixels.data(),
                cached.width, cached.height, cached.width * 4,
                cached.persistent);
            break;
        }

        default:
            break;
        }

        cached.isUploaded = success;
        cached.lastUsedFrame = currentFrame_;

        return success;
    }

    //=========================================================================
    // ワーカースレッド
    //=========================================================================
    void workerThread() {
        while (running_) {
            LoadRequest req;
            {
                std::unique_lock lock(queueMutex_);
                queueCv_.wait(lock, [this] {
                    return !running_ || !loadQueue_.empty();
                    });

                if (!running_ && loadQueue_.empty()) break;
                if (loadQueue_.empty()) continue;

                req = std::move(loadQueue_.front());
                loadQueue_.pop();
            }

            // デコード（ロック外）
            auto decoded = decodeImage(req.source);
            bool success = decoded.isValid();

            if (success) {
                // キャッシュに登録
                {
                    std::lock_guard lock(cacheMutex_);
                    auto& cached = cache_[req.imageId];
                    cached.imageId = req.imageId;
                    cached.pixels = std::move(decoded.pixels);
                    cached.width = decoded.width;
                    cached.height = decoded.height;
                    cached.usage = req.usage;
                    cached.persistent = req.persistent;
                    cached.lastUsedFrame = currentFrame_;
                    cached.refCount = 1;
                }
            }

            // 完了キューに追加
            {
                std::lock_guard lock(completedMutex_);
                completedLoads_.push_back({ req.imageId, req.callback, success });
            }
        }
    }

    //=========================================================================
    // 完了処理（メインスレッドで呼ばれる）
    //=========================================================================
    void processCompletedLoads() {
        std::vector<CompletedLoad> completed;
        {
            std::lock_guard lock(completedMutex_);
            completed.swap(completedLoads_);
        }

        for (auto& c : completed) {
            if (c.success && c.imageId != 0) {
                // GPUアップロード
                uploadToGpu(c.imageId);
            }

            // メインコールバック
            if (c.callback) {
                c.callback(c.imageId, c.success);
            }

            // 追加コールバック（重複ロード時後に登録されたもの）
            {
                std::lock_guard lock(pendingCallbacksMutex_);
                auto it = pendingCallbacks_.find(c.imageId);
                if (it != pendingCallbacks_.end()) {
                    for (auto& cb : it->second) {
                        if (cb) cb(c.imageId, c.success);
                    }
                    pendingCallbacks_.erase(it);
                }
            }
        }
    }

    struct CompletedLoad {
        ImageId imageId;
        std::function<void(ImageId, bool)> callback;
        bool success;
    };

    //=========================================================================
    // メンバ
    //=========================================================================
    ImageMaster& master_;
    PlacementPolicy policy_;
    uint64_t currentFrame_ = 0;

    FileDecoder fileDecoder_;
    MemoryDecoder memoryDecoder_;

    mutable std::mutex cacheMutex_;
    std::unordered_map<ImageId, CachedImage> cache_;

    std::atomic<bool> running_{ false };
    std::vector<std::thread> workers_;

    mutable std::mutex queueMutex_;
    std::queue<LoadRequest> loadQueue_;
    std::condition_variable queueCv_;

    std::mutex completedMutex_;
    std::vector<CompletedLoad> completedLoads_;

    // 重複ロード時用コールバック
    std::mutex pendingCallbacksMutex_;
    std::unordered_map<ImageId, std::vector<std::function<void(ImageId, bool)>>> pendingCallbacks_;

    // Queue for deferred GPU uploads (must be processed on render thread)
    std::mutex gpuUploadQueueMutex_;
    std::vector<ImageId> gpuUploadQueue_;
};
