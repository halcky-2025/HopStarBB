#define _CRT_SECURE_NO_WARNINGS

// =============================================================================
// Platform-specific heavy deps (Windows: PyTorch / ONNX / FFmpeg / Win32 API)
// Android 版は別途 jniLibs に配置した同等ライブラリを順次有効化していく。
// =============================================================================
#ifdef _WIN32
#include <torch/torch.h>
#include <windows.h>
#undef max
#undef min
#include <onnxruntime_c_api.h>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libswresample/swresample.h>
}
#endif

#ifdef __linux__
#include <unistd.h>
#include <dlfcn.h>
#include <linux/limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <pwd.h>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libswresample/swresample.h>
}
#endif

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>          // SDL3 のエントリポイントマクロ (Android では必須)
#include <SDL3_ttf/SDL_ttf.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "threed.h"
#include <cstdint>

#if defined(_WIN32) || defined(__linux__)
#include "miniaudio.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#ifdef _WIN32
#define UTF8PROC_STATIC
#include <utf8proc.h>
#pragma comment(lib, "utf8proc_static.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

#include "file_engine.h"

#ifdef _WIN32
#include "lldbdriver.h"   // RunLldbDriverMode 用 (= --lldb-driver 早期分岐で呼ぶ)
namespace F = torch::nn::functional;
#endif

#include "gc.h"
#include "arr.h"
#include "db2.h"
#include "state.h"     // SqlPool ベースの UI 状態永続化 (= db2.h の後、othelem.h より前)
#include <portaudio.h>   // audio.h より前にグローバルに入れる (audio.h は namespace nle 内で再 include するためヘッダガードでスキップ → 型は global に存在することになる)
#include "audio.h"
#include "elem.h"

#if defined(__ANDROID__) || defined(__linux__)
#include <thread>
#include <chrono>
#endif

#ifdef _WIN32
bool cuda = false;
HMODULE torch_cuda_dll;
#endif

// ============================================================================
// Global FileEngine instance
// ============================================================================
static std::unique_ptr<HopStarIO::FileEngine> g_fileEngine;

HopStarIO::FileEngine* getFileEngine() {
    if (!g_fileEngine) {
        g_fileEngine = HopStarIO::FileEngineFactory::createDefault();
    }
    return g_fileEngine.get();
}

// ============================================================================
// FileEngine-based file helpers
// ============================================================================

// Read file into memory using FileEngine
std::vector<uint8_t> readFileToMemory(const std::string& path) {
    auto* engine = getFileEngine();
    auto desc = engine->fromExternalPath(path, HopStarIO::Access::Read);
    auto result = engine->read(desc);
    if (result.success) {
        return std::move(result.data);
    }
    return {};
}

// Read file from URL or local path
std::vector<uint8_t> readFileFromAddress(const std::string& address) {
    auto* engine = getFileEngine();

    // Check if it's a URL or local path
    std::string scheme = HopStarIO::parseAddressScheme(address);
    if (scheme.empty()) {
        // Local file
        return readFileToMemory(address);
    }

    // Remote file
    auto desc = engine->createDescriptor(address, HopStarIO::Location::Server,
                                         HopStarIO::Access::Read, address);
    auto result = engine->read(desc);
    if (result.success) {
        return std::move(result.data);
    }
    return {};
}

// Write file using FileEngine
bool writeFileFromMemory(const std::string& path, const uint8_t* data, size_t length) {
    auto* engine = getFileEngine();
    auto desc = engine->fromExternalPath(path, HopStarIO::Access::Write);
    auto result = engine->write(desc, data, length);
    return result.success;
}

// Load image using FileEngine + stbi
unsigned char* loadImageViaFileEngine(const std::string& path, int* width, int* height, int* channels, int desired_channels) {
    auto data = readFileFromAddress(path);
    if (data.empty()) {
        return nullptr;
    }
    return stbi_load_from_memory(data.data(), static_cast<int>(data.size()),
                                  width, height, channels, desired_channels);
}

// Check if file exists - delegates to PlatformIO
bool fileExists(const std::string& path, HopStarIO::Location location) {
    return PlatformIO::fileExists(path, location);
}

// List files in directory - delegates to PlatformIO
std::vector<std::string> listDirectory(const std::string& dirPath, HopStarIO::Location location, const std::string& extension = "") {
    return PlatformIO::listDirectory(dirPath, location, extension);
}

// Create directory - delegates to PlatformIO
bool createDirectory(const std::string& path, HopStarIO::Location location) {
    return PlatformIO::createDirectory(path, location);
}
#include "opt.h"
#include "ugui.h"
#include "extorch.h"
//#include "db2.h"
SDL_HitTestResult
hit_test_cb(SDL_Window* win, const SDL_Point* pt, void* data)
{
    int w, h;
    const int border = 8;  // ���T�C�Y�\�G���A�̌���(px)
    SDL_GetWindowSize(win, &w, &h);

    // �l��
    if (pt->x < border && pt->y < border)               return SDL_HITTEST_RESIZE_TOPLEFT;
    if (pt->x > w - border && pt->y < border)           return SDL_HITTEST_RESIZE_TOPRIGHT;
    if (pt->x < border && pt->y > h - border)           return SDL_HITTEST_RESIZE_BOTTOMLEFT;
    if (pt->x > w - border && pt->y > h - border)       return SDL_HITTEST_RESIZE_BOTTOMRIGHT;

    // ��
    if (pt->y < border)                                 return SDL_HITTEST_RESIZE_TOP;
    if (pt->y > h - border)                             return SDL_HITTEST_RESIZE_BOTTOM;
    if (pt->x < border)                                 return SDL_HITTEST_RESIZE_LEFT;
    if (pt->x > w - border)                             return SDL_HITTEST_RESIZE_RIGHT;

    // タブ領域はドラッグでウィンドウ移動可能にする。
    // SDL_HITTEST_DRAGGABLE はドラッグ判定を超えた時だけウィンドウ移動として扱われ、
    // ただのクリックは通常通り SDL イベントが流れる → タブ切替も並立する。
    // 座標は RenderThread が atomic snapshot (= tabhit::g_left/top/right/bottom)
    // に書き込む。ここでは parent chain を辿らず snapshot の load だけ。
    if (tabhit::g_valid.load(std::memory_order_acquire)) {
        float left   = tabhit::g_left  .load(std::memory_order_relaxed);
        float top    = tabhit::g_top   .load(std::memory_order_relaxed);
        float right  = tabhit::g_right .load(std::memory_order_relaxed);
        float bottom = tabhit::g_bottom.load(std::memory_order_relaxed);
        if (pt->x >= left && pt->x < right &&
            pt->y >= top && pt->y < bottom) {
            return SDL_HITTEST_DRAGGABLE;
        }
    }

    // その他の領域は通常のクライアント領域として扱う
    return SDL_HITTEST_NORMAL;
}
#ifdef _WIN32
struct SimpleCNNImpl : torch::nn::Module {
public:
    torch::nn::Sequential net;
    SimpleCNNImpl() {
        if (!cuda) {
            torch_cuda_dll = LoadLibraryA("torch_cuda.dll");
            cuda = true;
        }
        net = torch::nn::Sequential(
            torch::nn::Conv2d(3, 16, 3),
            torch::nn::ReLU(),
            torch::nn::MaxPool2d(2),
            torch::nn::Conv2d(16, 32, 3),
            torch::nn::ReLU(),
            torch::nn::MaxPool2d(2),
            torch::nn::Flatten(),
            torch::nn::Linear(32 * 14 * 10, 64),
            torch::nn::ReLU(),
            torch::nn::Linear(64, 2)
        );
		register_module("net", net);
    }
	torch::Tensor forward(torch::Tensor x) {
		return net->forward(x);
	}
};
TORCH_MODULE(SimpleCNN);
size_t utf32_to_utf16(const utf8proc_int32_t* utf32, size_t len, uint16_t* utf16_out) {
    size_t j = 0;
    for (size_t i = 0; i < len; ++i) {
        uint32_t cp = utf32[i];
        if (cp <= 0xFFFF) {
            utf16_out[j++] = (uint16_t)cp;
        }
        else {
            cp -= 0x10000;
            utf16_out[j++] = (uint16_t)(0xD800 + (cp >> 10));
            utf16_out[j++] = (uint16_t)(0xDC00 + (cp & 0x3FF));
        }
    }
    return j;
}
#endif // _WIN32
int main(int argc, char** argv) {
#ifdef _WIN32
    // === driver mode 早期分岐 (C 案: double-init 回避) ===
    // 親 (= primary HopStarBB.exe) からの subprocess。SDL / bgfx を含む全 UI
    // 初期化を完全にスキップして、LLDB driver だけ走らせる。これで inferior
    // (= main.exe) の親が bgfx を init していない状態になり、孫の bgfx D3D11
    // 初期化が NVIDIA driver の per-process state と衝突しなくなる。
    if (argc >= 2 && strcmp(argv[1], "--lldb-driver") == 0) {
        return RunLldbDriverMode(argc, argv);
    }
#endif

#ifdef __ANDROID__
    SDL_Log("=== HopStarBB Android Start ===");
#endif

    // Initialize FileEngine (lazy init on first use)
    auto* fileEngine = getFileEngine();
    std::cout << "FileEngine initialized" << std::endl;

    magc = GC_init(100 * 1000 * 1000, 50 * 1000 * 1000);
    initVTables();

    // Use FileEngine's internal storage path for database
    std::string dbPath = fileEngine->getInternalPath("test.db");
    sqlite3* db;
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }
    else {
        std::cout << "Opened database successfully at: " << dbPath << std::endl;
    }
    const char* sql = "create table if not exists users(id integer primary key, name text);";
    char* err = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << err << std::endl;
        sqlite3_free(err);
    }
    else {
        std::cout << "Table created successfully" << std::endl;
    }
    sqlite3_close(db);
    try {
        /*
        torch::manual_seed(1);
        int ne = 0;
        if (ne == 0) {
            model->to(torch::kCUDA);
            model->train();
            torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(1e-3));
            createDirectory("inputs/faces", HopStarIO::Location::External);
            createDirectory("inputs/nonfaces", HopStarIO::Location::External);
            std::vector<torch::Tensor> input_tensors;
            std::vector<torch::Tensor> goal_tensors;

            // Load face images via FileEngine
            for (const auto& filename : listDirectory("inputs/faces", HopStarIO::Location::External, "png")) {
                int width, height, channels;
                unsigned char* data = loadImageViaFileEngine(filename, &width, &height, &channels, 3);
                if (data) {
                    torch::Tensor img_tensor = torch::from_blob(data, { height, width, 3 }, torch::kUInt8)
                        .permute({ 2, 0, 1 }).to(torch::kFloat32).div(255.0).unsqueeze(0);
                    img_tensor = F::interpolate(img_tensor, F::InterpolateFuncOptions().size(std::vector<int64_t>({ 64, 48 })).mode(torch::kBilinear).align_corners(false));
                    img_tensor = img_tensor.squeeze(0).clone();
                    input_tensors.push_back(img_tensor);
                    torch::Tensor label = torch::tensor(1, torch::kLong);
                    goal_tensors.push_back(label);
                    stbi_image_free(data);
                }
            }

            // Load non-face images via FileEngine
            for (const auto& filename : listDirectory("inputs/nonfaces", HopStarIO::Location::External, "png")) {
                int width, height, channels;
                unsigned char* data = loadImageViaFileEngine(filename, &width, &height, &channels, 3);
                if (data) {
                    torch::Tensor img_tensor = torch::from_blob(data, { height, width, 3 }, torch::kUInt8)
                        .permute({ 2, 0, 1 }).to(torch::kFloat32).div(255.0).unsqueeze(0);
                    img_tensor = F::interpolate(img_tensor, F::InterpolateFuncOptions().size(std::vector<int64_t>({ 64, 48 })).mode(torch::kBilinear).align_corners(false));
                    img_tensor = img_tensor.squeeze(0).clone();
                    input_tensors.push_back(img_tensor);
                    torch::Tensor label = torch::tensor(0, torch::kLong);
                    goal_tensors.push_back(label);
                    stbi_image_free(data);
                }
            }

            torch::Tensor loss;
            for (int epoch = 0; epoch < 100; epoch++) {
                size_t batch_index = 0;
                for (int i = 0; i < input_tensors.size(); i++) {
                }
            }
            //torch::save(model, "model.pt");
            model->eval();
        }
        else {
            //torch::load(model, "model.pt");
            model->to(torch::kCUDA);
            model->eval();
        }*/

        // SDL3 の初期化
#ifdef __ANDROID__
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
#else
        if (!SDL_Init(SDL_INIT_VIDEO)) {
#endif
            fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
            return 1;
        }
        if (!TTF_Init()) {
            fprintf(stderr, "ttferror");
        }
        // ウィンドウの作成
#ifdef __ANDROID__
        // SDL_WINDOW_FULLSCREEN を外す → immersive mode 解除 →
        // システム UI (ホーム/バック) が表示される。代わりに UI 描画は safe area
        // 内に閉じる (viewport offset + size を buildFrame で毎フレーム調整)。
        SDL_Window* window = SDL_CreateWindow("HopStarBB", 0, 0,
            SDL_WINDOW_VULKAN);
#elif defined(__linux__)
        SDL_Window* window = SDL_CreateWindow("HopStarBB",
            1200, 800,
            SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
#else
        SDL_Window* window = SDL_CreateWindow("SDL3 Example",
            1200, 800,
            SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE);
#endif
        if (!window) {
            fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }

#ifdef __ANDROID__
        // Android: ANativeWindow の準備完了を待つ (= SHOWN/EXPOSED/DISPLAY_CHANGED)
        {
            bool windowReady = false;
            int waitAttempts = 0;
            const int maxWaitAttempts = 100;  // 10秒上限
            while (!windowReady && waitAttempts < maxWaitAttempts) {
                SDL_Event ev;
                while (SDL_PollEvent(&ev)) {
                    if (ev.type == SDL_EVENT_WINDOW_SHOWN ||
                        ev.type == SDL_EVENT_WINDOW_EXPOSED ||
                        ev.type == SDL_EVENT_WINDOW_DISPLAY_CHANGED) {
                        windowReady = true;
                        break;
                    }
                    if (ev.type == SDL_EVENT_QUIT || ev.type == SDL_EVENT_TERMINATING) {
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 0;
                    }
                }
                if (!windowReady) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    waitAttempts++;
                }
            }
            SDL_Log("Window ready=%d (attempts=%d)", windowReady ? 1 : 0, waitAttempts);

            // ★ insets が安定するまでもう少しイベント pump する。
            // SDL_EVENT_WINDOW_SAFE_AREA_CHANGED が届いたら break、または最大
            // 30 回 (= 約 3 秒) で諦める。これにより最初の buildFrame が走る前に
            // SDL の window->safe_rect が確定し、初フレームから safe area サイズで
            // layout される。
            {
                SDL_Rect safe;
                int safeAttempts = 0;
                const int maxSafeAttempts = 30;
                bool safeReady = false;
                while (!safeReady && safeAttempts < maxSafeAttempts) {
                    SDL_Event ev;
                    while (SDL_PollEvent(&ev)) {
                        if (ev.type == SDL_EVENT_WINDOW_SAFE_AREA_CHANGED) {
                            safeReady = true;
                        }
                    }
                    if (SDL_GetWindowSafeArea(window, &safe) && safe.w > 0 && safe.h > 0) {
                        int fw = 0, fh = 0;
                        SDL_GetWindowSize(window, &fw, &fh);
                        if (safe.x != 0 || safe.y != 0 || safe.w != fw || safe.h != fh) {
                            // 実 inset が反映された (= safe rect が full size と違う) → 確定
                            safeReady = true;
                        }
                    }
                    if (!safeReady) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        safeAttempts++;
                    }
                }
                SDL_Log("[Android] safe area settled (attempts=%d)", safeAttempts);
            }
        }
#endif

        


        // �����_�����O�p�� SDL �O���t�B�b�N�R���e�L�X�g���쐬
        /*SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
        if (!renderer) {
            fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        TTF_Font* font = TTF_OpenFont("Gen.ttf", 16);  // 48px �̃T�C�Y
        if (!font) {
            printf("Failed to load font: \n");
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }*/
        SDL_Event event;
        SDL_FRect rect;
        SDL_FRect rect2;
        float mean[3] = { 123.675f, 116.28f, 103.53f };
        float std[3] = { 58.395f, 57.12f, 57.375f };

        int running = 1;
        int record = 0;
        int count = 0;
        /*Camera camera = Camera();
        AVFrame* frameRGB = av_frame_alloc();
        frameRGB->width = 640;
        frameRGB->height = 480;
        frameRGB->format = AV_PIX_FMT_RGB24;
        av_frame_get_buffer(frameRGB, 0);*/
        SDL_StartTextInput(window);
        // SDL_SetWindowHitTest は hoppy が作られた後 (= 下の HopStar* hoppy = new HopStar();
        // の後) で呼ばないと hoppy が未定義。↓に移動済み。
		/*Db* db = (Db*)GC_malloc(sizeof(Db));

        InitializeCriticalSection(&db->callback_mutex);
        for (int i = 0; i < WORKER_COUNT; i++) {
            char filename[32];
            sprintf(filename, "test.db", i);
            sqlite3_open(filename, &dbs[i]);
            sqlite3_exec(dbs[i], "CREATE TABLE IF NOT EXISTS logs(msg TEXT);", NULL, NULL, NULL);
            InitializeCriticalSection(&db->request_mutex[i]);
            db->request_event[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        }


        ThreadParam param[WORKER_COUNT];
        int ids[WORKER_COUNT];
        for (int i = 0; i < WORKER_COUNT; i++) {
			param[i].db = db;
            param[i].worker_id = i;
            ids[i] = i;
            db->threads[i] = CreateThread(NULL, 0, sqlite_thread, &param[i], 0, NULL);
        }*/
        /*GltfRenderer gltfRenderer;
        bool hasModel = false;

        // ���f�������[�h�i�R�}���h���C�������܂��̓f�t�H���g�j
        const char* modelPath = "CesiumMan.glb";
        float deltaTime = 1.0f / 100.0f;
        if (gltfRenderer.loadModel(modelPath, HopStarIO::Location::Resource)) {
            hasModel = true;
            printf("Loaded model: %s\n", modelPath);
        }
        else {
            printf("Warning: Could not load model, using fallback rendering\n");
        }*/
		int width, height;
        SDL_GetWindowSize(window, &width, &height);
#ifdef __ANDROID__
        // ★ Android: 初期 width/height は safe area サイズを使う (= ステータスバー /
        //    ナビゲーションバー / cutout を除いた領域)。これにより、最初の
        //    buildFrame が走る前から mainWin->size / mainLocal->size が safe area
        //    サイズになり、初回 layout から正しいサイズで組まれる
        //    (= 初フレーム描画が下に突き抜けない)。
        //    safe area の anchor (x, y) は g_backbufferViewOffsetX/Y で viewport
        //    offset として適用される (buildFrame で毎フレーム同期)。
        {
            SDL_Rect safe;
            if (SDL_GetWindowSafeArea(window, &safe) && safe.w > 0 && safe.h > 0) {
                SDL_Log("[Android-init] window=%dx%d safe=(%d,%d %dx%d)",
                        width, height, safe.x, safe.y, safe.w, safe.h);
                width = safe.w;
                height = safe.h;
            } else {
                SDL_Log("[Android-init] safe area not available, using window size %dx%d",
                        width, height);
            }
        }
#endif
        // 端末の content scale を UI / フォントサイズに反映。
        // SDL3 は high-DPI ディスプレイで 2.0 / Android 420dpi で 2.625 等を返す。
        // この時点で SDL_GetWindowDisplayScale は実ディスプレイに紐づき有効。
        {
            float dispScale = SDL_GetWindowDisplayScale(window);
            if (dispScale <= 0.0f) dispScale = 1.0f;
            SetUiScale(dispScale);
            SDL_Log("UI scale = %f (window %dx%d)", dispScale, width, height);
        }
        bool isDragging = false;
        int lastMouseX = 0, lastMouseY = 0;
        std::chrono::high_resolution_clock::time_point lastTime_ = std::chrono::high_resolution_clock::now();
        float time = 0;
		HopStar* hoppy = new HopStar();
        ThreadGC* thgc =hoppy->mainGC = GC_init(50 * 1024 * 1024);  // メインスレッド用GC (64MB)
        hoppy->mainGC->hoppy = hoppy;
        registerGCClasses(hoppy->mainGC);
		magc->hoppy = hoppy;
#ifndef __ANDROID__
        // hit test に hoppy を渡す (= タブ領域でウィンドウドラッグ移動を有効化)。
        // Android にはウィンドウドラッグ概念がないので無効。
        // Windows / Linux / macOS いずれもデスクトップ環境なので有効。
        SDL_SetWindowHitTest(window, hit_test_cb, hoppy);
#endif

        NativeWindow* mainWin = new NativeWindow();
        mainWin->sdlWindow = window;
        mainWin->size = { width, height };
        mainWin->viewId = hoppy->basicViewId++;
        mainWin->type = WindowType_Main;
        auto mainProps = SDL_GetWindowProperties(window);
#ifdef _WIN32
        mainWin->nwh = SDL_GetPointerProperty(mainProps, "SDL.window.win32.hwnd", nullptr);
#elif defined(__ANDROID__)
        mainWin->nwh = SDL_GetPointerProperty(mainProps, SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER, nullptr);
        SDL_Log("ANativeWindow nwh=%p", mainWin->nwh);
#elif defined(__linux__)
        // Linux: X11 window is a numeric ID (not a pointer)
        {
            uint64_t x11win = (uint64_t)SDL_GetNumberProperty(mainProps, "SDL.window.x11.window", 0);
            if (x11win) {
                mainWin->nwh = (void*)(uintptr_t)x11win;
            } else {
                // Wayland: wl_surface is a pointer
                mainWin->nwh = SDL_GetPointerProperty(mainProps, "SDL.window.wayland.surface", nullptr);
            }
        }
#else
        mainWin->nwh = nullptr;
#endif
        hoppy->mainGC->windows.push_back(mainWin);

        // mainGCのLocal初期化
        // mainLocal は cross-heap alias の指される側になる (= goLocal->parent = mainLocal)。
        // Old 確保で address を永続化し、cross-heap pointer を永久に valid にする。
        NewLocal* local = hoppy->mainLocal = (NewLocal*)GC_alloc_old(hoppy->mainGC, CType::_LocalC);
        initLocal(hoppy->mainGC, hoppy->mainLocal, mainWin);
        hoppy->mainLocal->font = getFont("sans", 16);
        local->xtype = SizeType::Range;
        local->ytype = SizeType::Range;
        mainWin->local = local;

        // 静的 root 登録 (= UI ツリー全体 + thgc 関連辞書 + main window)
        RootNode* rn = GC_add_root_node(thgc);
        GC_add_root(rn, (char**)&thgc->local);
        GC_add_root(rn, (char**)&thgc->map);
        GC_add_root(rn, (char**)&thgc->hoveredList);
        GC_add_root(rn, (char**)&thgc->hoveredSpanList);
        GC_add_root(rn, (char**)&mainWin->local);
        GC_add_root(rn, (char**)&mainWin->anchorElement);
        NewElement* elem = (NewElement*)GC_alloc(thgc, CType::_ElementC);
        initElement(thgc, elem);
        setPercentX(elem, 1.0, 0.0);
        elem->layout = _Zero;
        NewDirectAddLast(thgc, local, local, elem);
        // tab はクロスヒープ alias の登録元 (tab->linked->page を thgc の local 指す)。
        // tab->linked も Old 確保し、address 永続化する。
        NewTabBar* tab = hoppy->tab = (NewTabBar*)GC_alloc_old(thgc, CType::_TabC);
        initTab(thgc, tab);
        setPercentX(tab, 1.0, 0);
        tab->font = getFont("sans", 16);
        NewDirectAddLast(thgc, local, elem, tab);
        int height2 = Sc(24);
        NewElement* toolline = (NewElement*)GC_alloc(thgc, CType::_ElementC);
        initElement(thgc, toolline);
        toolline->orient = true;
        setPercentX(toolline, 1.0, 0.0);
        NewDirectAddLast(thgc, local, elem, toolline);
        NewImage* im1 = (NewImage*)GC_alloc(thgc, CType::_ImageC);
        initImage(thgc, im1);
        im1->background = (Background*)GC_alloc(thgc, CType::_Background);
        im1->background->tex1 = myloadTexture2D(thgc, "imgs/build.png", ImageUsage::Icon);
        im1->background->type = DrawCommandType::RawImage;
        im1->background->fillcolor = 0xFFFFFFFF;
        im1->size.x = height2;
        im1->size.y = height2;
        im1->hoverColor = 0x02AAFF20;
        NewElementAddLast(thgc, local, toolline, im1);
        NewImage* im2 = (NewImage*)GC_alloc(thgc, CType::_ImageC);
        initImage(thgc, im2);
        im2->background = (Background*)GC_alloc(thgc, CType::_Background);
        im2->background->tex1 = myloadTexture2D(thgc, "imgs/continue.png", ImageUsage::Icon);
        im2->background->type = DrawCommandType::RawImage;
        im2->background->fillcolor = 0xFFFFFFFF;
        im2->size.x = height2;
        im2->size.y = height2;
        NewElementAddLast(thgc, local, toolline, im2);
        NewDrop* drop = (NewDrop*)GC_alloc(thgc, CType::_DropC);
        drop->size.y = height2; drop->size.x = Sc(120);
        PopupWindow* pw = (PopupWindow*)GC_alloc(thgc, CType::_PopupC);
        pw->font = getFont("sans", 16);
        initPopup(thgc, local, pw, PopupAnchor::Anchor_Element, drop, 3, 0, 2);
        initDrop(thgc, drop, pw);
        NewElementAddLast(thgc, local, toolline, drop);
        NewDown* down1 = (NewDown*)GC_alloc(thgc, CType::_DownC);
        String* str1 = (String*)createString(thgc, "abcde", 5, 1);
        initDown(thgc, down1);
        NewDirectAddLast(thgc, local, pw, down1);
        reinitDown(thgc, down1, str1);
        NewLetter* let1 = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
        initLetter(thgc, let1, getFont("sans", 16), LetterType::_Letter);
        let1->text = str1;
        let1->color = 0x000000FF;
        NewDirectAddLast(thgc, local, down1, let1);
        NewDown* down2 = (NewDown*)GC_alloc(thgc, CType::_DownC);
        // ★ u"..." (char16_t = 2 byte 固定) で書く。L"..." は wchar_t で
        //   Windows は 2 byte、Android/Linux は 4 byte なので esize=2 と齟齬が出る。
        String* str2 = (String*)createString(thgc, (char*)u"あいうえお", 5, 2);
        initDown(thgc, down2);
        NewDirectAddLast(thgc, local, pw, down2);
        reinitDown(thgc, down2, str2);
        NewLetter* let2 = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
        initLetter(thgc, let2, getFont("sans", 16), LetterType::_Letter);
        let2->text = str2;
        let2->color = 0x000000FF;
        NewDirectAddLast(thgc, local, down2, let2);
        // le はクロスヒープ alias の登録元になる (le->page = local)。Old 直接確保で固定。
        LinkedElement* le = (LinkedElement*)GC_alloc_old(thgc, CType::_LinkedC);
        initLinkedElement(thgc, le);
        setPercentX((NewElement*)le, 1.0, 0);
        setPercentY((NewElement*)le, 1.0, 0);
        registerGCClasses(magc);
        // goLocal の parent などが mainGC->local (= mainLocal) を指す場合があり、
        // その field address を mainGC->local_aliases に登録するため Old 確保で固定。
        NewLocal* goLocal = (NewLocal*)GC_alloc_old(magc, CType::_LocalC);
        // goLocal は magc 用、メインの thgc root_stack には乗せない
        initLocal(magc, goLocal, mainWin);
        tab->linked = le;
        hoppy->push_tab(magc, goLocal);
        NewDirectAddLast(thgc, local, (NewElement*)local, le);
        // SDL User Event を登録（RenderThread → MainThread SDL委任用）
        //SDL_EVENT_SDL_REQUEST = SDL_RegisterEvents(1);
        // ファイルダイアログ結果用カスタムイベントを登録
        // (非同期ファイル読込は thgc->queue 経由なので SDL イベント不要)
        RegisterDialogEvents();

		hoppy->mainLocal->offscreen->markLayout(hoppy->mainLocal, hoppy->mainLocal);

        // リサイズ + Android lifecycle 用イベント watcher。
        // SDL3 は WILL_ENTER_BACKGROUND / DID_ENTER_FOREGROUND を
        // Java onPause / onResume の同期コールバック上で fire してくれるが、
        // SDL_PollEvent 経由だと処理が遅れて SurfaceView 破棄に間に合わず、
        // bgfx swapchain が dead nwh を掴んだまま再開して画面が止まる。
        // ここで同期的に render->pause() を呼んで Java onPause を blocking、
        // 安全に Android に surface 破棄させる。
        struct EventWatchCtx { HopStar* hoppy; RenderThread* render; };
        static EventWatchCtx s_eventWatchCtx;
        s_eventWatchCtx.hoppy = hoppy;
        s_eventWatchCtx.render = nullptr;
        SDL_AddEventWatch([](void* userdata, SDL_Event* event) -> bool {
            EventWatchCtx* ctx = (EventWatchCtx*)userdata;
            if (event->type == SDL_EVENT_WINDOW_RESIZED) {
                HopStar* hoppy = ctx->hoppy;
                NativeWindow* nw = hoppy->findWindowBySDLId(event->window.windowID);
                if (nw) {
                    int newW = (int)event->window.data1;
                    int newH = (int)event->window.data2;
                    nw->size = { newW, newH };
                    nw->needsResize = true;
                    auto wr = WindowResized(hoppy, nw);
                    hoppy->target->queue->push(wr.coro);
                    wr.coro = {};
                }
            }
#if defined(__ANDROID__)
            // WILL_ENTER_BACKGROUND / DID_ENTER_FOREGROUND + FOCUS_LOST / FOCUS_GAINED
            // の両方で検知する。File dialog 起動経路は端末によって WILL_ENTER_BACKGROUND が
            // 来ないので FOCUS_LOST を保険として入れる。
            // pause/resume が重複して呼ばれてもよいように冪等にする (内部で paused_ flag を
            // セット/リセットするだけなので既に冪等)。
            else if (event->type == SDL_EVENT_WILL_ENTER_BACKGROUND ||
                     event->type == SDL_EVENT_WINDOW_FOCUS_LOST) {
                SDL_Log("[EventWatch] pause-trigger type=%u", (unsigned)event->type);
                bgfx::PlatformData pdNull;
                memset(&pdNull, 0, sizeof(pdNull));
                pdNull.nwh = nullptr;
                bgfx::setPlatformData(pdNull);
                if (ctx->render) ctx->render->pauseAndWait();
            }
            else if (event->type == SDL_EVENT_DID_ENTER_FOREGROUND ||
                     event->type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
                SDL_Log("[EventWatch] resume-trigger type=%u", (unsigned)event->type);
                if (ctx->render) ctx->render->resume();
            }
#endif
            return true;
        }, &s_eventWatchCtx);

        RenderThread* render = new RenderThread(hoppy, window);
        render->start();
        s_eventWatchCtx.render = render;
		runGoThreadAsync(magc);
        SDL_Log("Entering main event loop");
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            /*for (int i = 0; i < WORKER_COUNT; i++) {
                SqlRequest* req = (SqlRequest*)malloc(sizeof(SqlRequest));
                snprintf(req->sql, sizeof(req->sql), "%s", "SELECT * FROM logs;");
                req->callback = print_result;
                req->user_data = NULL;
                enqueue_sql(db, req, i);
            }
            for (int i = 0; i < 20; i++) {
                poll_callbacks(db);
                Sleep(100);
            }*/

            while (SDL_PollEvent(&event)) {
                // ファイルダイアログ結果 (別スレッドから push される) はここで消費
                if (DispatchDialogEvent(&event)) continue;
                // 非同期ファイル読込完了は thgc->queue->resume_all() で処理されるので
                // ここでは何もしない (GoThread のメインループで毎フレーム回される)

                // イベントのウィンドウIDからターゲットウィンドウを特定
                SDL_WindowID eventWinId = 0;
                if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                    eventWinId = event.key.windowID;
                } else if (event.type == SDL_EVENT_TEXT_INPUT) {
                    eventWinId = event.text.windowID;
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    eventWinId = event.button.windowID;
                } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    eventWinId = event.motion.windowID;
                } else if (event.type == SDL_EVENT_WINDOW_RESIZED || event.type == SDL_EVENT_WINDOW_MOVED) {
                    eventWinId = event.window.windowID;
                }

                NativeWindow* targetNw = nullptr;
                if (eventWinId != 0) {
                    targetNw = hoppy->findWindowBySDLId(eventWinId);
                }
                bool isPopup = targetNw && targetNw->type != WindowType_Main && targetNw->local;

                if (event.type == SDL_EVENT_KEY_DOWN) {
                    if (event.key.key == SDLK_KP_ENTER || event.key.key == SDLK_RETURN || event.key.key == SDLK_DELETE || event.key.key == SDLK_BACKSPACE ||
                        event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT || event.key.key == SDLK_UP || event.key.key == SDLK_DOWN) {
                        bool select = false;
                        KeyEvent e = KeyEvent();
						e.key = event.key.key;
                        e.text = (String*)GC_alloc(hoppy->target, _String);
                        e.text->size = 0;
                        SDL_Keymod mod = event.key.mod;

                        e.shift = (mod & SDL_KMOD_SHIFT);
                        e.ctrl = (mod & SDL_KMOD_CTRL);
                        e.alt = (mod & SDL_KMOD_ALT);
                        if (isPopup) {
                            auto ke = PopupKeyButton(hoppy, targetNw, &e);
                            hoppy->target->queue->push(ke.coro);
                            ke.coro = {};
                        } else {
                            auto ke = KeyButton(hoppy, &e);
                            hoppy->target->queue->push(ke.coro);
                            ke.coro = {};
                        }
                    }
                }
                else if (event.type == SDL_EVENT_TEXT_INPUT) {
                    const char* text = event.text.text;
                    bool select = false;
                    KeyEvent e = KeyEvent();
                    e.key = -1;
                    String* str = (String*)GC_alloc(hoppy->target, _String);
#ifdef _WIN32
                    utf8proc_int32_t utf32Buf[256];
					utf8proc_ssize_t utf32len = utf8proc_decompose((const utf8proc_uint8_t*)text, -1, utf32Buf, 256, UTF8PROC_NULLTERM);
                    if (utf32len < 0) {
                        fprintf(stderr, "utf8proc_decompose_utf8 failed: %zd\n", utf32len);
                        continue;
                    }

                    uint16_t utf16Buf[512];
                    size_t utf16len = utf32_to_utf16(utf32Buf, (size_t)utf32len, utf16Buf);
                    str = createString(hoppy->target, (char*)utf16Buf, utf16len, 2);
#else
                    // Android/その他: utf8proc 未リンク。SDL3 が UTF-8 を渡してくるので
                    // 一旦そのまま UTF-8 で渡す (= encoding=1)。
                    // TODO: createString に渡す encoding 引数が UTF-16 専用なら、
                    // 将来 utf8proc を Android NDK ビルドに追加して同じパスにする。
                    size_t txtlen = strlen(text);
                    str = createString(hoppy->target, (char*)text, txtlen, 1);
#endif
                    e.text = str;
                    if (isPopup) {
                        auto ke = PopupKeyButton(hoppy, targetNw, &e);
                        hoppy->target->queue->push(ke.coro);
                        ke.coro = {};
                    } else {
                        auto ke = KeyButton(hoppy, &e);
                        hoppy->target->queue->push(ke.coro);
                        ke.coro = {};
                    }
				}
				else if (event.type == SDL_EVENT_QUIT) {
                    SDL_Log("SDL_EVENT_QUIT received, exiting");
					running = 0;
                }
#ifdef __ANDROID__
                else if (event.type == SDL_EVENT_TERMINATING) {
                    SDL_Log("SDL_EVENT_TERMINATING");
                    running = 0;
                }
                else if (event.type == SDL_EVENT_LOW_MEMORY) {
                    SDL_Log("SDL_EVENT_LOW_MEMORY");
                    // TODO: free non-essential resources (テクスチャキャッシュ等)
                }
                else if (event.type == SDL_EVENT_WILL_ENTER_BACKGROUND) {
                    SDL_Log("SDL_EVENT_WILL_ENTER_BACKGROUND");
                    // Pause render before Android destroys SurfaceView; otherwise
                    // bgfx (vulkan) hits vkCreateAndroidSurfaceKHR with a freed
                    // ANativeWindow* and SEGVs in libutils RefBase::incStrong on
                    // the swapchain recreate path.
                    if (render) render->pause();
                }
                else if (event.type == SDL_EVENT_DID_ENTER_BACKGROUND) {
                    SDL_Log("SDL_EVENT_DID_ENTER_BACKGROUND");
                }
                else if (event.type == SDL_EVENT_WILL_ENTER_FOREGROUND) {
                    SDL_Log("SDL_EVENT_WILL_ENTER_FOREGROUND");
                }
                else if (event.type == SDL_EVENT_DID_ENTER_FOREGROUND) {
                    SDL_Log("SDL_EVENT_DID_ENTER_FOREGROUND");
                    // SDL has a new ANativeWindow now; render thread refreshes
                    // bgfx::setPlatformData() before its next reset.
                    if (render) render->resume();
                }
                else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED ||
                         event.type == SDL_EVENT_WINDOW_DISPLAY_CHANGED) {
                    SDL_Log("Surface display/pixel-size change (%d)", event.type);
                    if (hoppy && hoppy->mainGC && !hoppy->mainGC->windows.empty()) {
                        NativeWindow* mn = hoppy->mainGC->windows[0];
                        int w = 0, h = 0;
                        SDL_GetWindowSize(window, &w, &h);
                        if (w > 0 && h > 0 && mn) {
                            mn->size = { w, h };
                            mn->needsFboReset = true;
                            {
                                std::lock_guard<std::mutex> lk(hoppy->m);
                                hoppy->render++;
                            }
                            hoppy->cv_.notify_all();
                        }
                    }
                }
                else if (event.type == SDL_EVENT_WINDOW_DESTROYED) {
                    SDL_Log("SDL_EVENT_WINDOW_DESTROYED");
                    running = 0;
                }
#endif
                else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    float tapX = (float)event.button.x;
                    float tapY = (float)event.button.y;
#ifdef __ANDROID__
                    // Android: touch 座標は raw SurfaceView 座標 (上 0 起点) で届くが、
                    // UI レイアウトは safe area 内座標 (上のステータスバー分シフト済み)。
                    // viewport offset (= g_backbufferViewOffsetX/Y) を引いて UI 座標に合わせる。
                    extern int g_backbufferViewOffsetX;
                    extern int g_backbufferViewOffsetY;
                    tapX -= (float)g_backbufferViewOffsetX;
                    tapY -= (float)g_backbufferViewOffsetY;
#endif
                    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                        SDL_Log("[Tap] DOWN raw=(%.0f,%.0f) ui=(%.0f,%.0f) winId=%u isPopup=%d targetNw=%p",
                                (float)event.button.x, (float)event.button.y,
                                tapX, tapY,
                                (unsigned)event.button.windowID, (int)isPopup, (void*)targetNw);
                    }
                    MouseEvent* e = new MouseEvent();
					e->x = tapX;
                    e->y = tapY;
                    e->action = event.type;
                    e->window = targetNw;
                    if (isPopup) {
                        auto me = PopupMouseButton(hoppy, targetNw, e);
                        hoppy->target->queue->push(me.coro);
                        me.coro = {};
                    }
                    else {
                        auto me = MouseMainButton(hoppy, e);
                        hoppy->mainGC->queue->push(me.coro);
                        me.coro = {};
                    }
                }
                else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    float mvX = event.motion.x;
                    float mvY = event.motion.y;
#ifdef __ANDROID__
                    extern int g_backbufferViewOffsetX;
                    extern int g_backbufferViewOffsetY;
                    mvX -= (float)g_backbufferViewOffsetX;
                    mvY -= (float)g_backbufferViewOffsetY;
#endif
                    MouseEvent* e = new MouseEvent();
                    e->x = mvX;
                    e->y = mvY;
                    e->window = targetNw;
                    SDL_MouseButtonFlags s = event.motion.state;

                    if (s & SDL_BUTTON_LMASK) {
                        e->click = true;
                    }
                    e->action = event.type;
                    if (isPopup) {
                        auto me = PopupMouseButton(hoppy, targetNw, e);
                        hoppy->target->queue->push(me.coro);
                        me.coro = {};
                    } else {
                        auto me = MouseMainButton(hoppy, e);
                        hoppy->target->queue->push(me.coro);
                        me.coro = {};
                    }
                }
                else if (event.type == SDL_EVENT_MOUSE_WHEEL) {

                }
                else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                    int newW = (int)event.window.data1;
                    int newH = (int)event.window.data2;
                    targetNw->size = { newW, newH };
                    targetNw->needsResize = true;
                    auto wr = WindowResized(hoppy, targetNw);
                    hoppy->target->queue->push(wr.coro);
                    wr.coro = {};
                }
                // === RenderThread からの SDL 委任リクエスト ===
                else if (event.type == SDL_EVENT_SDL_REQUEST) {
                    auto* req = (SDLRequest*)event.user.data1;
                    switch (req->type) {
                    case SDLRequest::CreateWindow: {
                        SDL_Window* sdlWin = nullptr;
                        SDL_WindowFlags hideFlag = req->visible ? 0 : SDL_WINDOW_HIDDEN;
                        switch (req->winType) {
                        case WindowType_Popup:
                            sdlWin = SDL_CreatePopupWindow(req->parentSDLWindow,
                                req->x, req->y, req->w, req->h, SDL_WINDOW_POPUP_MENU | hideFlag);
                            break;
                        case WindowType_AlwaysOnTop:
                            sdlWin = SDL_CreateWindow("", req->w, req->h,
                                SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP | hideFlag);
                            if (sdlWin) SDL_SetWindowPosition(sdlWin, req->x, req->y);
                            break;
                        case WindowType_Modal:
                            sdlWin = SDL_CreateWindow("", req->w, req->h, SDL_WINDOW_BORDERLESS | hideFlag);
                            if (sdlWin && req->parentSDLWindow) {
                                SDL_SetWindowParent(sdlWin, req->parentSDLWindow);
                                SDL_SetWindowModal(sdlWin, true);
                            }
                            if (sdlWin) SDL_SetWindowPosition(sdlWin, req->x, req->y);
                            break;
                        default:
                            break;
                        }
                        if (sdlWin) {
                            auto props = SDL_GetWindowProperties(sdlWin);
#ifdef _WIN32
                            req->resultNwh = SDL_GetPointerProperty(props, "SDL.window.win32.hwnd", nullptr);
                            // 角丸設定（Windows 11 DWM）
                            if (req->resultNwh && req->cornerRound > 0) {
                                DWORD pref = (DWORD)req->cornerRound;
                                DwmSetWindowAttribute((HWND)req->resultNwh,
                                    33 /*DWMWA_WINDOW_CORNER_PREFERENCE*/, &pref, sizeof(pref));
                            }
#elif defined(__ANDROID__)
                            req->resultNwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER, nullptr);
#elif defined(__linux__)
                            {
                                uint64_t x11win = (uint64_t)SDL_GetNumberProperty(props, "SDL.window.x11.window", 0);
                                if (x11win) {
                                    req->resultNwh = (void*)(uintptr_t)x11win;
                                } else {
                                    req->resultNwh = SDL_GetPointerProperty(props, "SDL.window.wayland.surface", nullptr);
                                }
                            }
#else
                            req->resultNwh = nullptr;
#endif
                            if (req->visible) SDL_ShowWindow(sdlWin);
                        }
                        req->resultSDLWindow = sdlWin;
                        req->success = (sdlWin != nullptr);
                        break;
                    }
                    case SDLRequest::ResizeWindow:
                        req->success = SDL_SetWindowSize(req->target->sdlWindow, req->newW, req->newH);
                        break;
                    case SDLRequest::DestroyWindow:
                        if (req->target && req->target->sdlWindow) {
                            SDL_DestroyWindow(req->target->sdlWindow);
                        }
                        req->success = true;
                        break;
                    case SDLRequest::ShowWindow:
                        if (req->target && req->target->sdlWindow) {
                            req->success = SDL_ShowWindow(req->target->sdlWindow);
                        }
                        break;
                    case SDLRequest::HideWindow:
                        if (req->target && req->target->sdlWindow) {
                            req->success = SDL_HideWindow(req->target->sdlWindow);
                        }
                        break;
                    case SDLRequest::MoveWindow:
                        if (req->target && req->target->sdlWindow) {
                            req->success = SDL_SetWindowPosition(req->target->sdlWindow, req->x, req->y);
                        }
                        break;
                    }
                    req->done.set_value();
                }
            }
        }

#ifdef _WIN32
        // 現在のデバイスを取得する別の方法
        c10::DeviceIndex device_idx = 0; // デフォルトの0番目のGPUを使用
        torch::Device device(torch::kCUDA, device_idx);
        std::cout << "Using CUDA device: " << device_idx << std::endl;

        // CPUテンソルを作成してからCUDAに転送（段階的に確認）
        torch::Tensor cpu_tensor = torch::rand({ 3, 4 });
        std::cout << "CPU Tensor created successfully" << std::endl;

        // 明示的なデバイス指定でGPUに転送
        torch::Tensor cuda_tensor = cpu_tensor.to(device);
        std::cout << "Tensor transferred to CUDA successfully" << std::endl;

        std::cout << cuda_tensor << std::endl;

        const OrtApi* ort = OrtGetApiBase()->GetApi(16);
        OrtEnv* env;
        ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "scrfd", &env);

        OrtSessionOptions* session_options;
        ort->CreateSessionOptions(&session_options);

        OrtSession* session;

        // Load ONNX model via FileEngine (supports local and remote)
        std::string onnxModelPath = "det_10g.onnx";
        std::string resolvedPath = PlatformIO::resolvePath(onnxModelPath, HopStarIO::Location::Resource);

        // Check if model exists in resources, otherwise use current directory
        if (!fileExists(resolvedPath, HopStarIO::Location::External)) {
            resolvedPath = onnxModelPath;
        }

        // Convert to wide string for ONNX Runtime
        std::wstring wModelPath(resolvedPath.begin(), resolvedPath.end());

        OrtStatus* status = ort->CreateSession(env, wModelPath.c_str(), session_options, &session);
        if (status != NULL) {
            const char* msg = ort->GetErrorMessage(status);
            fprintf(stderr, "ONNX session load error: %s (path: %s)\n", msg, resolvedPath.c_str());
            ort->ReleaseStatus(status);
            exit(1);
        }
        OrtAllocator* allocator;
        ort->GetAllocatorWithDefaultOptions(&allocator);
#endif // _WIN32 (PyTorch + ONNX Runtime block)



        /*for (int i = 0; i < WORKER_COUNT; i++) {
            sqlite3_close(dbs[i]);
        }*/

    }
#ifdef _WIN32
    catch (const c10::Error& e) {
        std::cerr << "LibTorch Error: " << e.what() << std::endl;
    }
#endif
    catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }

    // Cleanup FileEngine
    g_fileEngine.reset();
    std::cout << "FileEngine cleaned up" << std::endl;

    return 0;
}