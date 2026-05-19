#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#include <thread>
#include <chrono>
#include <iostream>

#if !TARGET_OS_IOS && !TARGET_OS_SIMULATOR && !defined(__ANDROID__) && !defined(__linux__) && !TARGET_OS_OSX

class CustomModuleImpl;


class TensorWrapper {
public:
    torch::Tensor tensor;

    // Use move semantics to preserve computation graph
    TensorWrapper(torch::Tensor&& t) : tensor(std::move(t)) {}

    // Copy constructor that maintains graph connectivity
    TensorWrapper(const torch::Tensor& t) : tensor(t) {}
};
torch::nn::Conv2dImpl* createConv2d(int64_t in_channels, int64_t out_channels, int64_t kernel_size) {
    try {
        auto options = torch::nn::Conv2dOptions(in_channels, out_channels, kernel_size)
            .stride(1)
            .padding(0)
            .bias(true);

        auto conv = new torch::nn::Conv2dImpl(options);
        conv->to(torch::kCUDA);
        return conv;
    }
    catch (...) {
        return nullptr;
    }
}

// Conv2d�̃v���p�e�B�ɃA�N�Z�X����֐�
int64_t getInChannels(torch::nn::Conv2dImpl* conv) {
    if (conv) {
        return conv->options.in_channels();
    }
    return 0;
}

int64_t getOutChannels(torch::nn::Conv2dImpl* conv) {
    if (conv) {
        return conv->options.out_channels();
    }
    return 0;
}

int64_t getKernelSize(torch::nn::Conv2dImpl* conv) {
    if (conv) {
        return conv->options.kernel_size()->at(0);
    }
    return 0;
}

// �E�F�C�g�f�[�^�ɃA�N�Z�X����֐�
float* getWeightData(torch::nn::Conv2dImpl* conv) {
    if (conv && conv->weight.defined()) {
        return conv->weight.data_ptr<float>();
    }
    return nullptr;
}

int64_t getWeightSize(torch::nn::Conv2dImpl* conv) {
    if (conv && conv->weight.defined()) {
        return conv->weight.numel();
    }
    return 0;
}


// �t�H���[�h�֐�
void* forwardConv2d(torch::nn::Conv2dImpl* conv, void* input_tensor) {
    if (conv && input_tensor) {
        try {
            TensorWrapper* input_wrapper = static_cast<TensorWrapper*>(input_tensor);
            torch::Tensor output = conv->forward(input_wrapper->tensor);
            // �v�Z�O���t��ێ����郉�b�p�[��Ԃ�
            return new TensorWrapper(output);
        }
        catch (const c10::Error& e) {
            std::cerr << "LibTorch Error: " << e.what() << std::endl;
            return nullptr;
        }
        catch (const std::exception& e) {
            std::cerr << "Standard Exception: " << e.what() << std::endl;
            return nullptr;
        }
        catch (...) {
            std::cerr << "Unknown exception occurred" << std::endl;
            return nullptr;
        }
    }
    return nullptr;
}torch::nn::MaxPool2dImpl* createMaxPool2d(int64_t kernel_size, int64_t stride, int64_t padding) {
    try {
        auto options = torch::nn::MaxPool2dOptions(kernel_size)
            .stride(stride)
            .padding(padding);

        auto pool = new torch::nn::MaxPool2dImpl(options);
        return pool;
    }
    catch (...) {
        return nullptr;
    }
}

int64_t getMaxPoolKernelSize(torch::nn::MaxPool2dImpl* pool) {
    if (pool) {
        return pool->options.kernel_size()->at(0);
    }
    return 0;
}

int64_t getMaxPoolStride(torch::nn::MaxPool2dImpl* pool) {
    if (pool) {
        return pool->options.stride()->at(0);
    }
    return 0;
}

int64_t getMaxPoolPadding(torch::nn::MaxPool2dImpl* pool) {
    if (pool) {
        return pool->options.padding()->at(0);
    }
    return 0;
}

void* forwardMaxPool2d(torch::nn::MaxPool2dImpl* pool, void* input_tensor) {
    if (pool && input_tensor) {
        try {
            torch::Tensor* input = static_cast<torch::Tensor*>(input_tensor);
            torch::Tensor output = pool->forward(*input);
            return new torch::Tensor(output);
        }
        catch (...) {
            return nullptr;
        }
    }
    return nullptr;
}

// ReLU�֘A�̎���
torch::nn::ReLUImpl* createReLU(bool inplace) {
    try {
        auto options = torch::nn::ReLUOptions().inplace(inplace);
        auto relu = new torch::nn::ReLUImpl(options);
        relu->to(torch::kCUDA);
        return relu;
    }
    catch (...) {
        return nullptr;
    }
}

bool getReLUInplace(torch::nn::ReLUImpl* relu) {
    if (relu) {
        return relu->options.inplace();
    }
    return false;
}

void* forwardReLU(torch::nn::ReLUImpl* relu, void* input_tensor) {
    if (relu && input_tensor) {
        try {
            TensorWrapper* input_wrapper = static_cast<TensorWrapper*>(input_tensor);
            torch::Tensor output = relu->forward(input_wrapper->tensor);
            // �v�Z�O���t��ێ����郉�b�p�[��Ԃ�
            return new TensorWrapper(output);
        }
        catch (const c10::Error& e) {
            std::cerr << "LibTorch Error: " << e.what() << std::endl;
            return nullptr;
        }
        catch (const std::exception& e) {
            std::cerr << "Standard Exception: " << e.what() << std::endl;
            return nullptr;
        }
        catch (...) {
            std::cerr << "Unknown exception occurred" << std::endl;
            return nullptr;
        }
    }
    return nullptr;
}

torch::nn::FlattenImpl* createFlatten(int64_t start_dim, int64_t end_dim) {
    try {
        auto options = torch::nn::FlattenOptions().start_dim(start_dim).end_dim(end_dim);
        auto flatten = new torch::nn::FlattenImpl(options);
        flatten->to(torch::kCUDA);
        return flatten;
    }
    catch (...) {
        return nullptr;
    }
}

int64_t getFlattenStartDim(torch::nn::FlattenImpl* flatten) {
    if (flatten) {
        return flatten->options.start_dim();
    }
    return 0;
}

int64_t getFlattenEndDim(torch::nn::FlattenImpl* flatten) {
    if (flatten) {
        return flatten->options.end_dim();
    }
    return -1;
}

void* forwardFlatten(torch::nn::FlattenImpl* flatten, void* input_tensor) {
    if (flatten && input_tensor) {
        try {
            torch::Tensor* input = static_cast<torch::Tensor*>(input_tensor);
            torch::Tensor output = flatten->forward(*input);
            return new torch::Tensor(std::move(output));
        }
        catch (...) {
            return nullptr;
        }
    }
    return nullptr;
}

// Linear �̎���
torch::nn::LinearImpl* createLinear(int64_t in_features, int64_t out_features, bool bias) {
    try {
        auto options = torch::nn::LinearOptions(in_features, out_features).bias(bias);
        auto linear = new torch::nn::LinearImpl(options);
        linear->to(torch::kCUDA);
        return linear;
    }
    catch (...) {
        return nullptr;
    }
}

int64_t getLinearInFeatures(torch::nn::LinearImpl* linear) {
    if (linear) {
        return linear->options.in_features();
    }
    return 0;
}

int64_t getLinearOutFeatures(torch::nn::LinearImpl* linear) {
    if (linear) {
        return linear->options.out_features();
    }
    return 0;
}

bool getLinearBias(torch::nn::LinearImpl* linear) {
    if (linear) {
        return linear->options.bias();
    }
    return false;
}

float* getLinearWeightData(torch::nn::LinearImpl* linear) {
    if (linear && linear->weight.defined()) {
        return linear->weight.data_ptr<float>();
    }
    return nullptr;
}

int64_t getLinearWeightSize(torch::nn::LinearImpl* linear) {
    if (linear && linear->weight.defined()) {
        return linear->weight.numel();
    }
    return 0;
}

float* getLinearBiasData(torch::nn::LinearImpl* linear) {
    if (linear && linear->bias.defined()) {
        return linear->bias.data_ptr<float>();
    }
    return nullptr;
}

int64_t getLinearBiasSize(torch::nn::LinearImpl* linear) {
    if (linear && linear->bias.defined()) {
        return linear->bias.numel();
    }
    return 0;
}

void* forwardLinear(torch::nn::LinearImpl* linear, void* input_tensor) {
    if (linear && input_tensor) {
        try {
            torch::Tensor* input = static_cast<torch::Tensor*>(input_tensor);
            torch::Tensor output = linear->forward(*input);
            return new torch::Tensor(std::move(output));
        }
        catch (const c10::Error& e) {
            std::cerr << "LibTorch Error: " << e.what() << std::endl;
            return nullptr;
        }
        catch (const std::exception& e) {
            std::cerr << "Standard Exception: " << e.what() << std::endl;
            return nullptr;
        }
        catch (...) {
            std::cerr << "Unknown exception occurred" << std::endl;
            return nullptr;
        }
    }
    return nullptr;
}

// Sequential �̎���
torch::nn::SequentialImpl* createSequential() {
    try {
        auto seq = new torch::nn::SequentialImpl();
        return seq;
    }
    catch (...) {
        return nullptr;
    }
}

void addModuleToSequential(torch::nn::SequentialImpl* seq, const char* name, void* module) {
    if (seq && module && name) {
        try {
            // module_handle �𐳂����^�ɃL���X�g
            auto* holder = static_cast<torch::nn::AnyModule*>(module);
            // AnyModule �̃N���[���i���[�u�\�ȃI�u�W�F�N�g�j�𓾂�
            torch::nn::AnyModule cloned = holder->clone();
            // push_back �Ƀ��[�u����
            seq->push_back(std::string(name), std::move(cloned));
        }
        catch (...) {
            // �G���[����
        }
    }
}

size_t getSequentialSize(torch::nn::SequentialImpl* seq) {
    if (seq) {
        return seq->size();
    }
    return 0;
}

void* getSequentialModule(torch::nn::SequentialImpl* seq, size_t index) {
    if (seq && index < seq->size()) {
        return (*seq)[index].get();
    }
    return nullptr;
}

void* forwardSequential(torch::nn::SequentialImpl* seq, void* input_tensor) {
    if (seq && input_tensor) {
        try {
            torch::Tensor* input = static_cast<torch::Tensor*>(input_tensor);
            torch::Tensor output = seq->forward(*input);
            return new torch::Tensor(output);
        }
        catch (...) {
            return nullptr;
        }
    }
    return nullptr;
}

void registerModule(torch::nn::Module* parent, const char* name, torch::nn::Module* child) {  
   if (parent && child && name) {  
       try {  
           // �C��: `register_module` �̑�2������ `t       orch::nn::Module` �̎Q�Ƃł͂Ȃ��A`std::shared_ptr<torch::nn::Module>` �����҂��܂�  
           parent->register_module(name, std::shared_ptr<torch::nn::Module>(child));  
       }  
       catch (...) {  
           // �G���[����  
       }  
   }  
}

using ForwardType = void* (*)(void*, void*);
// �J�X�^�����W���[���̃t�H���[�h�֐��̃R�[���o�b�N�^
using ForwardCallback = void* (*)(void*, void*);
class CustomModuleImpl : public torch::nn::Module {
public:
    CustomModuleImpl() {
        if (!cuda) {
            torch_cuda_dll = LoadLibraryA("torch_cuda.dll");
            cuda = true;
        }
        // �f�t�H���g�R���X�g���N�^
    }
    // �t�H���[�h�֐�
    torch::Tensor forward(torch::Tensor x) {
        // ���̓e���\�������b�p�[�ɓ����
        TensorWrapper* wrapper = new TensorWrapper(x);

        // ���������s
        void* result = forwardfunc(obj, wrapper);

        // ���b�p�[���猋�ʃe���\�������o��
        TensorWrapper* output_wrapper = static_cast<TensorWrapper*>(result);
        torch::Tensor output = output_wrapper->tensor;

        // ���������
        delete wrapper;
        delete output_wrapper;

        return output;
    }
    ForwardType forwardfunc;
    char* obj;
};

TORCH_MODULE(CustomModule);
CustomModuleImpl* createTorchModule(char* obj, ForwardType forward) {
    try {
        auto module = new CustomModuleImpl();
        module->obj = obj;
        module->forwardfunc = forward;
        return module;
    }
    catch (...) {
        return nullptr;
    }
}

torch::optim::Adam* createAdam(torch::nn::Module** params, int64_t num_params, torch::optim::AdamOptions* options) {
    std::vector<torch::Tensor> parameters;
    for (int64_t i = 0; i < num_params; i++) {
        auto module_params = params[i]->parameters();
        parameters.insert(parameters.end(), module_params.begin(), module_params.end());
    }
    return new torch::optim::Adam(parameters, *options);
}

torch::optim::AdamOptions* createAdamOptions(float learning_rate) {
    return new torch::optim::AdamOptions(learning_rate);
}

void* tensorToCUDA(void* tensor_ptr) {
    TensorWrapper* wrapper = static_cast<TensorWrapper*>(tensor_ptr);
    torch::Tensor cuda_tensor = wrapper->tensor.to(torch::kCUDA);
    return new TensorWrapper(std::move(cuda_tensor));
}

void* tensorRequiresGrad(void* tensor_ptr, bool requires_grad) {
    TensorWrapper* wrapper = static_cast<TensorWrapper*>(tensor_ptr);
    torch::Tensor tensor = wrapper->tensor.requires_grad_(requires_grad);
    return new TensorWrapper(std::move(tensor));
}

void optimizerZeroGrad(torch::optim::Adam* optimizer) {
    optimizer->zero_grad();
}

void tensorBackward(void* tensor_ptr) {
    TensorWrapper* wrapper = static_cast<TensorWrapper*>(tensor_ptr);
    wrapper->tensor.backward();
}

void optimizerStep(torch::optim::Adam* optimizer) {
    optimizer->step();
}

void* crossEntropy(void* output_ptr, void* target_ptr) {
    TensorWrapper* output_wrapper = static_cast<TensorWrapper*>(output_ptr);
    TensorWrapper* target_wrapper = static_cast<TensorWrapper*>(target_ptr);
    torch::Tensor loss = torch::nn::functional::cross_entropy(output_wrapper->tensor, target_wrapper->tensor);
    return new TensorWrapper(std::move(loss));
}

using CreateConv2dType = torch::nn::Conv2dImpl* (*)(int64_t, int64_t, int64_t);
using GetInChannelsType = int64_t(*)(torch::nn::Conv2dImpl* conv);
using GetWeightDataType = float* (*)(torch::nn::Conv2dImpl* conv);
using ForwardConv2dType = void* (*)(torch::nn::Conv2dImpl* conv, void* input_tensor);

using CreateMaxPool2dType = torch::nn::MaxPool2dImpl* (*)(int64_t, int64_t, int64_t);
using GetMaxPoolKernelSizeType = int64_t(*)(torch::nn::MaxPool2dImpl*);
using ForwardMaxPool2dType = void* (*)(torch::nn::MaxPool2dImpl*, void*);

// ReLU�p�̊֐��|�C���^�^
using CreateReLUType = torch::nn::ReLUImpl* (*)(bool);
using GetReLUInplaceType = bool(*)(torch::nn::ReLUImpl*);
using ForwardReLUType = void* (*)(torch::nn::ReLUImpl*, void*);

using CreateFlattenType = torch::nn::FlattenImpl* (*)(int64_t, int64_t);
using GetFlattenStartDimType = int64_t(*)(torch::nn::FlattenImpl*);
using ForwardFlattenType = void* (*)(torch::nn::FlattenImpl*, void*);

using CreateLinearType = torch::nn::LinearImpl* (*)(int64_t, int64_t, bool);
using GetLinearInFeaturesType = int64_t(*)(torch::nn::LinearImpl*);
using GetLinearBiasType = bool(*)(torch::nn::LinearImpl*);
using GetLinearWeightDataType = float* (*)(torch::nn::LinearImpl*);
using ForwardLinearType = void* (*)(torch::nn::LinearImpl*, void*);

using CreateSequentialType = torch::nn::SequentialImpl* (*)();
using AddModuleToSequentialType = void (*)(torch::nn::SequentialImpl*, const char*, void*);
using GetSequentialSizeType = size_t(*)(torch::nn::SequentialImpl*);
using GetSequentialModuleType = void* (*)(torch::nn::SequentialImpl*, size_t index);
using ForwardSequentialType = void* (*)(torch::nn::SequentialImpl*, void*);

using RegisterModuleType = void (*)(torch::nn::Module*, const char*, torch::nn::Module*);
using CreateTorchModuleType = CustomModuleImpl * (*)(char*, ForwardType);
using SetForwardFunctionType = void (*)(torch::nn::Module*, void* (*)(void*, void*));

using GC_AddClassType = int (*)(ThreadGC*, int, int, const char*, GCCheckFunc, GCFinalizeFunc, bool);
using GC_AddRootType = RootNode * (*)(ThreadGC*);
using GC_ReleaseRootType = void(*)(RootNode*);
using GC_SetRootType = void (*)(RootNode*, char**);
using GC_BackRootType = void (*)(RootNode*, int);
using GC_mallocType = char* (*)(ThreadGC*, int);

using CreateAdamType = torch::optim::Adam* (*)(torch::nn::Module**, int64_t, torch::optim::AdamOptions*);
using CreateAdamOptionsType = torch::optim::AdamOptions* (*)(float);
using OptimizerZeroGradType = void (*)(torch::optim::Adam*);
using OptimizerStepType = void (*)(torch::optim::Adam*);

// �e���\������֘A�̊֐��|�C���^�^
using TensorToCUDAType = void* (*)(void*);
using TensorRequiresGradType = void* (*)(void*, bool);
using TensorBackwardType = void (*)(void*);

// �����v�Z�֘A�̊֐��|�C���^�^
using CrossEntropyType = void* (*)(void*, void*);

// �e���\���A�N�Z�X�֘A�i�K�v�ɉ����Ēǉ��j
using TensorUnsqueezeType = void* (*)(void*, int64_t);
using TensorItemIntType = int (*)(void*);
using TensorItemFloatType = float (*)(void*);
using TensorCloneType = void* (*)(void*);

using CopyObjectType = char* (*)(ThreadGC*, char*);
using GetHashValueType = int (*) (String*, int);

using CreateListType = List * (*) (ThreadGC*, int, CType);
using AddListType = void (*)(ThreadGC*, List*, char*);
using GetListType = char** (*)(List*, int);
using GetLastType = char** (*)(List*);
using SetListType = void (*)(List*, int, char*);
using SetLastType = void (*)(List*, char*);

using CreateHashType = Map * (*) (ThreadGC*, CType);
using AddHashType = void (*)(ThreadGC*, Map*, String*, char*);
using GetHashType = char* (*)(Map*, String*);

using CreateStringType = String * (*) (ThreadGC*, char*, int, int);
using AddStringType = String * (*)(ThreadGC*, String*, char*, int, int);
using AddStringType2 = String * (*)(ThreadGC*, String*, String*);
using RemoveStringType = String * (*)(ThreadGC*, String*, int);
using SubStringType = String * (*)(ThreadGC*, String*, int, int);
using NumberStringType = String * (*)(ThreadGC*, int);
using GetCharType = wchar_t (*)(String*, int);
using PrintStringType = void (*)(String*);
using PrintArrayType = void (*)(Map*, int, int);

using DbInitType = SqlPool* (*)(ThreadGC*, String*, int, int, int, String*);
using PushQueueType = void (*)(CoroutineQueue*, char*);
using WaitHandleType = char* (*)(ThreadGC* thgc, char*);

using BeginTransactionType = std::coroutine_handle<Generator::promise_type>(*)(ThreadGC* thgc, SqlPool* sql, CoroutineQueue* queue);
using StringUTF8Type = char* (*)(String* str, int* outlen);
using ExecSqlType = std::coroutine_handle<Generator::promise_type>(*)(Transaction* tx, String* sql, SqlParam params[], int pcount, char* (*func)(ThreadGC*, CoroutineQueue*, std::unique_lock<std::mutex>*, sqlite3_stmt*, char*), char* obj);
using TxFinishType = std::coroutine_handle<Generator::promise_type>(*)(Transaction* tx);
using GCANTType = void(*) (ThreadGC* thgc, CoroutineQueue* queue, std::unique_lock<std::mutex>* lock);
using SqlIntType = int(*) (sqlite3_stmt* st, int n);
using SqlStringType = const unsigned char* (*) (sqlite3_stmt* st, int n);
using CreateDivType = NewElement * (*)(ThreadGC*);
using ElementAddType = void (*)(ThreadGC*, NewElement*, NewElement*);
using CreateLetterType = void (*) (ThreadGC*, NewElement*, String*);
using CreateKaigyouType = void (*) (ThreadGC*, NewElement*);
using CheckTreeElementType = NewElement * (*)(ThreadGC* thgc, NewLocal* local, NewElement* parentt, NewElement* parent, List* deletes, String* id, enum LetterType type, enum OpCode op, NewElement* (*func)(ThreadGC*));
using MakeFrameType = Frame * (*)(RootNode* root, void (*func)(Frame*), char* blockobj);
uint64_t now_us() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}
CustomModuleImpl* ReadDll(ThreadGC* thgc, const char* dll_path = "foo.dll") {
    // 「loadDll は一つだけ」ポリシー:
    //   各タブ (= ThreadGC) のライフタイム中、本関数 = LoadLibrary は 1 回のみ呼ばれる想定。
    //   タブごとに新しい ThreadGC を作るため、ここで過去の DLL を解放する必要はない。
    //   もしユーザーコードが追加の DLL (プラグインや依存ライブラリ) を必要とする場合は、
    //   この関数の内部で LoadLibraryA を追加して補助的にロードする。外側から ReadDll を
    //   何度も呼び直す設計にはしない。
    HMODULE h = LoadLibraryA(dll_path);
    if (!h) {
        std::cerr << "LoadLibrary " << dll_path << " failed: " << GetLastError() << "\n";
        return NULL;
    }

    auto conv2d = reinterpret_cast<CreateConv2dType*>(GetProcAddress(h, "CreateConv2d"));
    if (!conv2d) {
        std::cerr << "GetProcAddress for 'createconv2d' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    // 3) ���O�̊֐�����
    *conv2d = createConv2d;
    auto getinchannels = reinterpret_cast<GetInChannelsType*>(GetProcAddress(h, "GetInChannels"));
    if (!getinchannels) {
        std::cerr << "GetProcAddress for 'cgetinchannels' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    // 3) ���O�̊֐�����
    *getinchannels = getInChannels;

    auto getoutchannels = reinterpret_cast<GetInChannelsType*>(GetProcAddress(h, "GetOutChannels"));
    if (!getoutchannels) {
        std::cerr << "GetProcAddress for 'getoutchannels' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    // 3) ���O�̊֐�����
    *getinchannels = getOutChannels;
    auto getkernelsize = reinterpret_cast<GetInChannelsType*>(GetProcAddress(h, "GetKernelSize"));
    if (!getkernelsize) {
        std::cerr << "GetProcAddress for 'getkernelsize' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    // 3) ���O�̊֐�����
    *getkernelsize = getKernelSize;
    // Conv2d�̃E�F�C�g�f�[�^�ɃA�N�Z�X����֐�
    auto getweightdata = reinterpret_cast<GetWeightDataType*>(GetProcAddress(h, "GetWeightData"));
    if (!getweightdata) {
        std::cerr << "GetProcAddress for 'getweightdata' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    // 3) ���O�̊֐�����
    *getweightdata = getWeightData;
    auto getweightsize = reinterpret_cast<GetInChannelsType*>(GetProcAddress(h, "GetWeightSize"));
    if (!getweightsize) {
        std::cerr << "GetProcAddress for 'getweightdata' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    // 3) ���O�̊֐�����
    *getweightsize = getWeightSize;

    // Conv2d���t�H���[�h����֐�
    auto forwardconv2d = reinterpret_cast<ForwardConv2dType*>(GetProcAddress(h, "ForwardConv2d"));
    if (!forwardconv2d) {
        std::cerr << "GetProcAddress for 'forwardconv2d' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    // 3) ���O�̊֐�����
    *forwardconv2d = forwardConv2d;

    auto createMaxpool = reinterpret_cast<CreateMaxPool2dType*>(GetProcAddress(h, "CreateMaxPool2d"));
    if (!createMaxpool) {
        std::cerr << "GetProcAddress for 'createMaxpool' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createMaxpool = createMaxPool2d;

    auto getMaxpoolKernelSize = reinterpret_cast<GetMaxPoolKernelSizeType*>(GetProcAddress(h, "GetMaxPoolKernelSize"));
    if (!getMaxpoolKernelSize) {
        std::cerr << "GetProcAddress for 'getMaxpoolKernelSize' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getMaxpoolKernelSize = getMaxPoolKernelSize;

    auto getMaxpoolStride = reinterpret_cast<GetMaxPoolKernelSizeType*>(GetProcAddress(h, "GetMaxPoolStride"));
    if (!getMaxpoolStride) {
        std::cerr << "GetProcAddress for 'getMaxpoolStride' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getMaxpoolStride = getMaxPoolStride;

    auto getMaxpoolPadding = reinterpret_cast<GetMaxPoolKernelSizeType*>(GetProcAddress(h, "GetMaxPoolPadding"));
    if (!getMaxpoolPadding) {
        std::cerr << "GetProcAddress for 'getMaxpoolPadding' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getMaxpoolPadding = getMaxPoolPadding;

    auto forwardMaxpool = reinterpret_cast<ForwardMaxPool2dType*>(GetProcAddress(h, "ForwardMaxPool2d"));
    if (!forwardMaxpool) {
        std::cerr << "GetProcAddress for 'forwardMaxpool' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *forwardMaxpool = forwardMaxPool2d;

    // ReLU�֘A�̊֐��ݒ�
    auto createRelu = reinterpret_cast<CreateReLUType*>(GetProcAddress(h, "CreateReLU"));
    if (!createRelu) {
        std::cerr << "GetProcAddress for 'createRelu' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createRelu = createReLU;

    auto getReLUInplacePtr = reinterpret_cast<GetReLUInplaceType*>(GetProcAddress(h, "GetReLUInplace"));
    if (!getReLUInplacePtr) {
        std::cerr << "GetProcAddress for 'getReLUInplacePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getReLUInplacePtr = getReLUInplace;

    auto forwardReLUPtr = reinterpret_cast<ForwardReLUType*>(GetProcAddress(h, "ForwardReLU"));
    if (!forwardReLUPtr) {
        std::cerr << "GetProcAddress for 'forwardReLUPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *forwardReLUPtr = forwardReLU;

    auto createFlattenPtr = reinterpret_cast<CreateFlattenType*>(GetProcAddress(h, "CreateFlatten"));
    if (!createFlattenPtr) {
        std::cerr << "GetProcAddress for 'createFlattenPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createFlattenPtr = createFlatten;

    auto getFlattenStartDimPtr = reinterpret_cast<GetFlattenStartDimType*>(GetProcAddress(h, "GetFlattenStartDim"));
    if (!getFlattenStartDimPtr) {
        std::cerr << "GetProcAddress for 'getFlattenStartDimPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getFlattenStartDimPtr = getFlattenStartDim;

    auto getFlattenEndDimPtr = reinterpret_cast<GetFlattenStartDimType*>(GetProcAddress(h, "GetFlattenEndDim"));
    if (!getFlattenEndDimPtr) {
        std::cerr << "GetProcAddress for 'getFlattenEndDimPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getFlattenEndDimPtr = getFlattenEndDim;

    auto forwardFlattenPtr = reinterpret_cast<ForwardFlattenType*>(GetProcAddress(h, "ForwardFlatten"));
    if (!getFlattenEndDimPtr) {
        std::cerr << "GetProcAddress for 'forwardFlattenPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *forwardFlattenPtr = forwardFlatten;

    // Linear �֘A�̊֐��ݒ�
    auto createLinearPtr = reinterpret_cast<CreateLinearType*>(GetProcAddress(h, "CreateLinear"));
    if (!createLinearPtr) {
        std::cerr << "GetProcAddress for 'createLinearPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createLinearPtr = createLinear;

    auto getLinearInFeaturesPtr = reinterpret_cast<GetLinearInFeaturesType*>(GetProcAddress(h, "GetLinearInFeatures"));
    if (!getLinearInFeaturesPtr) {
        std::cerr << "GetProcAddress for 'getLinearInFeaturesPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLinearInFeaturesPtr = getLinearInFeatures;

    auto getLinearOutFeaturesPtr = reinterpret_cast<GetLinearInFeaturesType*>(GetProcAddress(h, "GetLinearOutFeatures"));
    if (!getLinearOutFeaturesPtr) {
        std::cerr << "GetProcAddress for 'getLinearOutFeaturesPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLinearOutFeaturesPtr = getLinearOutFeatures;

    auto getLinearBiasPtr = reinterpret_cast<GetLinearBiasType*>(GetProcAddress(h, "GetLinearBias"));
    if (!getLinearInFeaturesPtr) {
        std::cerr << "GetProcAddress for 'getLinearBias' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLinearBiasPtr = getLinearBias;

    auto getLinearWeightDataPtr = reinterpret_cast<GetLinearWeightDataType*>(GetProcAddress(h, "GetLinearInFeatures"));
    if (!getLinearInFeaturesPtr) {
        std::cerr << "GetProcAddress for 'getLinearInFeaturesPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLinearWeightDataPtr = getLinearWeightData;

    auto getLinearWeightSizePtr = reinterpret_cast<GetLinearInFeaturesType*>(GetProcAddress(h, "GetLinearInFeatures"));
    if (!getLinearInFeaturesPtr) {
        std::cerr << "GetProcAddress for 'getLinearInFeaturesPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLinearWeightSizePtr = getLinearWeightSize;

    auto getLinearBiasDataPtr = reinterpret_cast<GetLinearWeightDataType*>(GetProcAddress(h, "GetLinearInFeatures"));
    if (!getLinearInFeaturesPtr) {
        std::cerr << "GetProcAddress for 'getLinearInFeaturesPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLinearBiasDataPtr = getLinearBiasData;

    auto getLinearBiasSizePtr = reinterpret_cast<GetLinearInFeaturesType*>(GetProcAddress(h, "GetLinearInFeatures"));
    if (!getLinearInFeaturesPtr) {
        std::cerr << "GetProcAddress for 'getLinearInFeaturesPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLinearBiasSizePtr = getLinearBiasSize;

    auto forwardLinearPtr = reinterpret_cast<ForwardLinearType*>(GetProcAddress(h, "ForwardLinear"));
    if (!forwardLinearPtr) {
        std::cerr << "GetProcAddress for 'forwardLinearPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *forwardLinearPtr = forwardLinear;

    // ���̑��� Linear �֐������l�ɐݒ�...

    // Sequential �֘A�̊֐��ݒ�
    auto createSequentialPtr = reinterpret_cast<CreateSequentialType*>(GetProcAddress(h, "CreateSequential"));
    if (!createSequentialPtr) {
        std::cerr << "GetProcAddress for 'createSequentialPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createSequentialPtr = createSequential;

    auto addModuleToSequentialPtr = reinterpret_cast<AddModuleToSequentialType*>(GetProcAddress(h, "AddModuleToSequential"));
    if (!addModuleToSequentialPtr) {
        std::cerr << "GetProcAddress for 'addModuleToSequentialPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *addModuleToSequentialPtr = addModuleToSequential;

    auto getSequentialSizePtr = reinterpret_cast<GetSequentialSizeType*>(GetProcAddress(h, "AddModuleToSequential"));
    if (!getSequentialSizePtr) {
        std::cerr << "GetProcAddress for 'getSequentialSizePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getSequentialSizePtr = getSequentialSize;

    auto getSequentialModulePtr = reinterpret_cast<GetSequentialModuleType*>(GetProcAddress(h, "AddModuleToSequential"));
    if (!getSequentialModulePtr) {
        std::cerr << "GetProcAddress for 'getSequetialModulePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getSequentialModulePtr = getSequentialModule;

    auto forwardSequentialPtr = reinterpret_cast<ForwardSequentialType*>(GetProcAddress(h, "AddModuleToSequential"));
    if (!forwardSequentialPtr) {
        std::cerr << "GetProcAddress for 'forwardSequentialPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *forwardSequentialPtr = forwardSequential;

    // ���̑��� Sequential �֐������l�ɐݒ�...

    // Module �֘A�̔ėp�֐�
    auto registerModulePtr = reinterpret_cast<RegisterModuleType*>(GetProcAddress(h, "RegisterModule"));
    if (!registerModulePtr) {
        std::cerr << "GetProcAddress for 'registerModulePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *registerModulePtr = registerModule;

    // �J�X�^�����W���[���֘A
    auto createTorchModulePtr = reinterpret_cast<CreateTorchModuleType*>(GetProcAddress(h, "CreateTorchModule"));
    if (!createTorchModulePtr) {
        std::cerr << "GetProcAddress for 'createTorchModulePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createTorchModulePtr = createTorchModule;

    /*auto setForwardFunctionPtr = reinterpret_cast<SetForwardFunctionType*>(GetProcAddress(h, "SetForwardFunction"));
    if (!forwardReLUPtr) {
        std::cerr << "GetProcAddress for 'cgetinchannels' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return;
    }
    *setForwardFunctionPtr = setForwardFunction;*/

    auto gcAddClassPtr = reinterpret_cast<GC_AddClassType*>(GetProcAddress(h, "GC_AddClass"));
    if (!gcAddClassPtr) {
        std::cerr << "GetProcAddress for 'GC_AddClass' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *gcAddClassPtr = GC_register_class;

    auto gcAddRootPtr = reinterpret_cast<GC_AddRootType*>(GetProcAddress(h, "GC_AddRoot"));
    if (!gcAddRootPtr) {
        std::cerr << "GetProcAddress for 'GC_AddRoot' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *gcAddRootPtr = GC_add_root_node;

    auto gcReleaseRootPtr = reinterpret_cast<GC_ReleaseRootType*>(GetProcAddress(h, "GC_ReleaseRoot"));
    if (!gcReleaseRootPtr) {
        std::cerr << "GetProcAddress for 'GC_SetRoot' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *gcReleaseRootPtr = GC_release_root_node;

    auto gcSetRootPtr = reinterpret_cast<GC_SetRootType*>(GetProcAddress(h, "GC_SetRoot"));
    if (!gcSetRootPtr) {
        std::cerr << "GetProcAddress for 'GC_SetRoot' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *gcSetRootPtr = GC_add_root;

    auto gcBackRootPtr = reinterpret_cast<GC_BackRootType*>(GetProcAddress(h, "GC_BackRoot"));
    if (!gcBackRootPtr) {
        std::cerr << "GetProcAddress for 'rnptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *gcBackRootPtr = GC_pop_roots;

    auto gcMallocPtr = reinterpret_cast<GC_mallocType*>(GetProcAddress(h, "GC_malloc"));
    if (!gcMallocPtr) {
        std::cerr << "GetProcAddress for 'GC_malloc' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *gcMallocPtr = GC_alloc;

    auto createAdamPtr = reinterpret_cast<CreateAdamType*>(GetProcAddress(h, "CreateAdam"));
    if (!createAdamPtr) {
        std::cerr << "GetProcAddress for 'CreateAdam' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createAdamPtr = createAdam;

    auto createAdamOptionsPtr = reinterpret_cast<CreateAdamOptionsType*>(GetProcAddress(h, "CreateAdamOptions"));
    if (!createAdamOptionsPtr) {
        std::cerr << "GetProcAddress for 'CreateAdamOptions' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createAdamOptionsPtr = createAdamOptions;

    auto optimizerZeroGradPtr = reinterpret_cast<OptimizerZeroGradType*>(GetProcAddress(h, "OptimizerZeroGrad"));
    if (!optimizerZeroGradPtr) {
        std::cerr << "GetProcAddress for 'OptimizerZeroGrad' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *optimizerZeroGradPtr = optimizerZeroGrad;

    auto optimizerStepPtr = reinterpret_cast<OptimizerStepType*>(GetProcAddress(h, "OptimizerStep"));
    if (!optimizerStepPtr) {
        std::cerr << "GetProcAddress for 'OptimizerStep' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *optimizerStepPtr = optimizerStep;

    // �e���\������֘A
    auto tensorToCUDAPtr = reinterpret_cast<TensorToCUDAType*>(GetProcAddress(h, "TensorToCUDA"));
    if (!tensorToCUDAPtr) {
        std::cerr << "GetProcAddress for 'TensorToCUDA' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *tensorToCUDAPtr = tensorToCUDA;

    auto tensorRequiresGradPtr = reinterpret_cast<TensorRequiresGradType*>(GetProcAddress(h, "TensorRequiresGrad"));
    if (!tensorRequiresGradPtr) {
        std::cerr << "GetProcAddress for 'TensorRequiresGrad' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *tensorRequiresGradPtr = tensorRequiresGrad;

    auto tensorBackwardPtr = reinterpret_cast<TensorBackwardType*>(GetProcAddress(h, "TensorBackward"));
    if (!tensorBackwardPtr) {
        std::cerr << "GetProcAddress for 'TensorBackward' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *tensorBackwardPtr = tensorBackward;

    // �����v�Z�֘A
    auto crossEntropyPtr = reinterpret_cast<CrossEntropyType*>(GetProcAddress(h, "CrossEntropy"));
    if (!crossEntropyPtr) {
        std::cerr << "GetProcAddress for 'CrossEntropy' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *crossEntropyPtr = crossEntropy;

    auto copyObjectPtr = reinterpret_cast<CopyObjectType*>(GetProcAddress(h, "CopyObject"));
    if (!copyObjectPtr) {
        std::cerr << "GetProcAddress for 'CopyObject' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *copyObjectPtr = copy_object_young;

    auto cloneObjectPtr = reinterpret_cast<CopyObjectType*>(GetProcAddress(h, "CloneObject"));
    if (!cloneObjectPtr) {
        std::cerr << "GetProcAddress for 'CloneObject' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *cloneObjectPtr = GC_clone;

    auto getHashValuePtr = reinterpret_cast<GetHashValueType*>(GetProcAddress(h, "GetHashValue"));
    if (!getHashValuePtr) {
        std::cerr << "GetProcAddress for 'getHashValuePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getHashValuePtr = get_hashvalue;

    auto thgcp = (ThreadGC**)GetProcAddress(h, "thgcp");
    if (!thgcp) {
        std::cerr << "GetProcAddress for 'thgcp' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *thgcp = thgc;

    auto createListPtr = reinterpret_cast<CreateListType*>(GetProcAddress(h, "CreateList"));
    if (!createListPtr) {
        std::cerr << "GetProcAddress for 'createListPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createListPtr = create_list;

    auto addListPtr = reinterpret_cast<AddListType*>(GetProcAddress(h, "AddList"));
    if (!addListPtr) {
        std::cerr << "GetProcAddress for 'addListPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *addListPtr = add_list;

    auto getListPtr = reinterpret_cast<GetListType*>(GetProcAddress(h, "GetList"));
    if (!getListPtr) {
        std::cerr << "GetProcAddress for 'getListPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getListPtr = get_list;

    auto getLastPtr = reinterpret_cast<GetLastType*>(GetProcAddress(h, "GetLast"));
    if (!getLastPtr) {
        std::cerr << "GetProcAddress for 'getLastPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getLastPtr = get_last;

    auto setListPtr = reinterpret_cast<SetListType*>(GetProcAddress(h, "SetList"));
    if (!setListPtr) {
        std::cerr << "GetProcAddress for 'setListPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *setListPtr = set_list;

    auto setLastPtr = reinterpret_cast<SetLastType*>(GetProcAddress(h, "SetLast"));
    if (!setLastPtr) {
        std::cerr << "GetProcAddress for 'setLastPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *setLastPtr = set_last;

    auto createHashPtr = reinterpret_cast<CreateHashType*>(GetProcAddress(h, "CreateHash"));
    if (!createHashPtr) {
        std::cerr << "GetProcAddress for 'createHashPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createHashPtr = create_mapy;

    auto addHashPtr = reinterpret_cast<AddHashType*>(GetProcAddress(h, "AddHash"));
    if (!addHashPtr) {
        std::cerr << "GetProcAddress for 'addHashPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *addHashPtr = add_mapy;

    auto getHashPtr = reinterpret_cast<GetHashType*>(GetProcAddress(h, "GetHash"));
    if (!getHashPtr) {
        std::cerr << "GetProcAddress for 'getHashPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getHashPtr = get_mapy;

    auto createStringPtr = reinterpret_cast<CreateStringType*>(GetProcAddress(h, "CreateString"));
    if (!createStringPtr) {
        std::cerr << "GetProcAddress for 'createStringPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *createStringPtr = createString;

    auto addStringPtr = reinterpret_cast<AddStringType*>(GetProcAddress(h, "AddString"));
    if (!addStringPtr) {
        std::cerr << "GetProcAddress for 'addStringPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *addStringPtr = StringAdd;

    auto addStringPtr2 = reinterpret_cast<AddStringType2*>(GetProcAddress(h, "AddString2"));
    if (!addStringPtr2) {
        std::cerr << "GetProcAddress for 'addStringPtr2' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *addStringPtr2 = StringAdd2;

    auto removeStringPtr = reinterpret_cast<RemoveStringType*>(GetProcAddress(h, "RemoveString"));
    if (!removeStringPtr) {
        std::cerr << "GetProcAddress for 'removeStringPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *removeStringPtr = StringRemove;

    auto subStringPtr = reinterpret_cast<SubStringType*>(GetProcAddress(h, "SubString"));
    if (!subStringPtr) {
        std::cerr << "GetProcAddress for 'subStringPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *subStringPtr = SubString;

    auto getCharPtr = reinterpret_cast<GetCharType*>(GetProcAddress(h, "GetChar"));
    if (!getCharPtr) {
        std::cerr << "GetProcAddress for 'getCharPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *getCharPtr = GetChar;

    auto numberStringPtr = reinterpret_cast<NumberStringType*>(GetProcAddress(h, "NumberString"));
    if (!numberStringPtr) {
        std::cerr << "GetProcAddress for 'numberStringPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *numberStringPtr = NumberString;

    auto printStringPtr = reinterpret_cast<PrintStringType*>(GetProcAddress(h, "PrintString"));
    if (!printStringPtr) {
        std::cerr << "GetProcAddress for 'printStringPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *printStringPtr = PrintString;

    auto printArrayPtr = reinterpret_cast<PrintArrayType*>(GetProcAddress(h, "PrintArray"));
    if (!printArrayPtr) {
        std::cerr << "GetProcAddress for 'printArrayPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *printArrayPtr = PrintArray;

    auto dbInitPtr = reinterpret_cast<DbInitType*>(GetProcAddress(h, "DbInit"));
    if (!dbInitPtr) {
        std::cerr << "GetProcAddress for 'printArrayPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *dbInitPtr = DbInit;

    auto pushQueuePtr = reinterpret_cast<PushQueueType*>(GetProcAddress(h, "PushQueue"));
    if (!pushQueuePtr) {
        std::cerr << "GetProcAddress for 'pushQueuePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *pushQueuePtr = push_to_queue;

    auto waitHandlePtr = reinterpret_cast<WaitHandleType*>(GetProcAddress(h, "WaitHandle"));
    if (!waitHandlePtr) {
        std::cerr << "GetProcAddress for 'waitHandlePtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *waitHandlePtr = wait_handle;

    auto beginTransactionPtr = reinterpret_cast<BeginTransactionType*>(GetProcAddress(h, "BeginTransaction"));
    if (!beginTransactionPtr) {
        std::cerr << "GetProcAddress for 'beginTransactionPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *beginTransactionPtr = beginTransaction;

    auto execSqlPtr = reinterpret_cast<ExecSqlType*>(GetProcAddress(h, "ExecSql"));
    if (!execSqlPtr) {
        std::cerr << "GetProcAddress for 'execSqlPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *execSqlPtr = execSql;

    auto txCommitPtr = reinterpret_cast<TxFinishType*>(GetProcAddress(h, "TxCommit"));
    if (!txCommitPtr) {
        std::cerr << "GetProcAddress for 'txCommitPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *txCommitPtr = TxCommit;

    auto txRollbackPtr = reinterpret_cast<TxFinishType*>(GetProcAddress(h, "TxRollback"));
    if (!txCommitPtr) {
        std::cerr << "GetProcAddress for 'txRollbackPtr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *txRollbackPtr = TxRollback;

    auto stringUTF8Ptr = reinterpret_cast<StringUTF8Type*>(GetProcAddress(h, "StringUTF8"));
    if (!stringUTF8Ptr) {
        std::cerr << "GetProcAddress for 'stringUTF8' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *stringUTF8Ptr = StringUTF8;

    using CheckType = void (*) (Transaction*);
    auto checkPtr = reinterpret_cast<CheckType*>(GetProcAddress(h, "Check"));
    if (!checkPtr) {
        std::cerr << "GetProcAddress for 'check' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *checkPtr = Check;

    auto sqlIntPtr = reinterpret_cast<SqlIntType*>(GetProcAddress(h, "SqlInt"));
    if (!sqlIntPtr) {
        std::cerr << "GetProcAddress for 'sqlInt' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *sqlIntPtr = sqlite3_column_int;

    auto sqlStringPtr = reinterpret_cast<SqlStringType*>(GetProcAddress(h, "SqlString"));
    if (!sqlStringPtr) {
        std::cerr << "GetProcAddress for 'sqlString' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *sqlStringPtr = sqlite3_column_text;

    auto sqlLengthPtr = reinterpret_cast<SqlIntType*>(GetProcAddress(h, "SqlLength"));
    if (!sqlLengthPtr) {
        std::cerr << "GetProcAddress for 'sqlLength' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *sqlLengthPtr = sqlite3_column_int;

    auto gcANTPtr = reinterpret_cast<GCANTType*>(GetProcAddress(h, "GCANT"));
    if (!gcANTPtr) {
        std::cerr << "GetProcAddress for 'gcANT' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *gcANTPtr = CallGCant;

    auto crsANTPtr = reinterpret_cast<CreateStringType*>(GetProcAddress(h, "CreateStringANT"));
    if (!crsANTPtr) {
        std::cerr << "GetProcAddress for 'crsANT' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *crsANTPtr = createStringant;

    auto cloANTptr = reinterpret_cast<CopyObjectType*>(GetProcAddress(h, "CloneObjectANT"));
    if (!cloANTptr) {
        std::cerr << "GetProcAddress for 'cloANTptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *cloANTptr = GC_clone_unlocked;

    /*auto creDivptr = reinterpret_cast<CreateDivType*>(GetProcAddress(h, "CreateDiv"));
    if (!creDivptr) {
        std::cerr << "GetProcAddress for 'creDivptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *creDivptr = createDiv;

    auto creSheetptr = reinterpret_cast<CreateDivType*>(GetProcAddress(h, "CreateSheet"));
    if (!creSheetptr) {
        std::cerr << "GetProcAddress for 'creSheetptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *creSheetptr = createSheet;

    auto eleAddptr = reinterpret_cast<ElementAddType*>(GetProcAddress(h, "ElementAdd"));
    if (!eleAddptr) {
        std::cerr << "GetProcAddress for 'eleAddptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *eleAddptr = ElementAdd;

    auto creLetptr = reinterpret_cast<CreateLetterType*>(GetProcAddress(h, "CreateLetter"));
    if (!creLetptr) {
        std::cerr << "GetProcAddress for 'creLetptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *creLetptr = createLetter;

    auto creKaiptr = reinterpret_cast<CreateKaigyouType*>(GetProcAddress(h, "CreateKaigyou"));
    if (!creKaiptr) {
        std::cerr << "GetProcAddress for 'creKaiptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *creKaiptr = createKaigyou;

    auto creEndptr = reinterpret_cast<CreateKaigyouType*>(GetProcAddress(h, "CreateEnd"));
    if (!creEndptr) {
        std::cerr << "GetProcAddress for 'creEndptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *creEndptr = createEnd;*/

    auto cheTeptr = reinterpret_cast<CheckTreeElementType*>(GetProcAddress(h, "CheckTreeElement"));
    if (!cheTeptr) {
        std::cerr << "GetProcAddress for 'creTeptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *cheTeptr = checkTreeElement;

    auto mkfptr = reinterpret_cast<MakeFrameType*>(GetProcAddress(h, "MakeFrame"));
    if (!mkfptr) {
        std::cerr << "GetProcAddress for 'mkfptr' failed: " << GetLastError() << "\n";
        FreeLibrary(h);
        return NULL;
    }
    *mkfptr = MakeFrame;



    auto cnl = reinterpret_cast<int*>(GetProcAddress(h, "cnl"));
    int oldcn = thgc->class_count;
    thgc->class_count += *cnl;

    using VoiFun = void (*)(int);
    VoiFun mainF = reinterpret_cast<VoiFun>(GetProcAddress(h, "main"));
    mainF(oldcn);

    // GoThread 側から参照できるように thgc->loadedDlls へ登録する。
    {
        std::lock_guard<std::mutex> lk(thgc->loadedDllsMutex);
        LoadedDll info;
        info.handle = (void*)h;
        info.name = dll_path;
        info.class_base = oldcn;
        thgc->loadedDlls.push_back(std::move(info));
    }

    using Test2 = int (*)();
    Test2 test2 = reinterpret_cast<Test2>(
        GetProcAddress(h, "test2")
        );
    if (test2) {
        int nn = test2();
        std::cout << nn << std::endl;
    }
    return NULL;
}

#endif // !TARGET_OS_IOS && !TARGET_OS_SIMULATOR && !__ANDROID__ && !__linux__

// Timing function needed by GoThread - shared for all platforms
#if TARGET_OS_IOS || TARGET_OS_SIMULATOR || defined(__ANDROID__) || defined(__linux__) || TARGET_OS_OSX
uint64_t now_us() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}
#endif

// GCにクラス登録（GoThread / mainGC 共通）。
// 各 Check 関数の実装は **対応する struct 定義の直下**に置いてある (= newelem.h, othelem.h,
// elem.h, Compile.h を参照)。共通 helper も同様 (= traceNewEndElement / traceNewElement / traceObj)。
void registerGCClasses(ThreadGC* thgc) {
    GC_register_class(thgc, CType::_String,sizeof(String), "Str", StringCheck, NULL);
    GC_register_class(thgc, CType::_List,sizeof(List), "List", ListCheck, NULL);
    GC_register_class(thgc, CType::_ElementC,sizeof(NewElement), "Elem", NewElementCheck, NULL);
    GC_register_class(thgc, CType::_FRect,sizeof(SDL_FRect), "FRect", NULL, NULL);
    GC_register_class(thgc, CType::_LetterC,sizeof(NewLetter), "Letter", NewLetterCheck, NULL);
    GC_register_class(thgc, CType::_ButtonC,sizeof(Button), "Button", ButtonCheck, NULL);
    GC_register_class(thgc, CType::_LineC,sizeof(NewLine), "Line", NewLineCheck, NULL);
    GC_register_class(thgc, CType::_LocalC,sizeof(NewLocal), "Local", NewLocalCheck, NULL);
    GC_register_class(thgc, CType::_ImageC,sizeof(NewImage), "Image", NewImageCheck, NULL);
    GC_register_class(thgc, CType::_DropC,sizeof(NewDrop), "Drop", NewDropCheck, NULL);
    GC_register_class(thgc, CType::_DownC,sizeof(NewDown), "Down", NewDownCheck, NULL);
    GC_register_class(thgc, CType::_TabC,sizeof(NewTabBar), "Tab", NewTabBarCheck, NULL);
    GC_register_class(thgc, CType::_PopupC,sizeof(PopupWindow), "Popup", PopupWindowCheck, NULL);
    GC_register_class(thgc, CType::_Background,sizeof(Background), "Background", BackgroundCheck, NULL);
    // SIValues は GC_alloc_size で {int n; SIVal values[];} を 1 ブロックで確保する。
    // values[] は値型 (uint pair / uint+float / uint64 / function ptr) のみで GC ポインタを含まないため、
    // gc_check は不要。GC 連携は将来必要になったら別途設定する。
    // _SIVal / _SIValues の CType 登録は残しておくが、現状は未使用。
    GC_register_class(thgc, CType::_SIVal,    sizeof(SIVal),    "SIVal",    NULL, NULL);
    GC_register_class(thgc, CType::_SIValues, sizeof(SIValues), "SIValues", SIValuesCheck, NULL);
    GC_register_class(thgc, CType::_TabTitleC,sizeof(NewTabTitle), "TabTitle", NewTabTitleCheck, NULL);
    GC_register_class(thgc, CType::_TabPageC,sizeof(NewTabPage), "TabPage", NULL, NULL);
    GC_register_class(thgc, CType::_LinkedC,sizeof(LinkedElement), "Linked", LinkedElementCheck, NULL);
    GC_register_class(thgc, CType::_TextBoxC,sizeof(TextBox), "TextBox", TextBoxCheck, NULL);
    GC_register_class(thgc, CType::_SideletC,sizeof(Sidelet), "Sidelet", SideletCheck, NULL);
    GC_register_class(thgc, CType::_KV,sizeof(KV), "KeyValue", KVCheck, NULL);
    GC_register_class(thgc, CType::_MapData,sizeof(MapData), "MapData", MapDataCheck, NULL);
    GC_register_class(thgc, CType::_Map,sizeof(Map), "Map", MapCheck, NULL);
    GC_register_class(thgc, CType::_EndC,sizeof(NewEndElement), "End", EndCCheck, NULL);
    GC_register_class(thgc, CType::_ColumnMeta,sizeof(ColumnMeta), "ColumnMeta", ColumnMetaCheck, NULL);
    GC_register_class(thgc, CType::_Table,sizeof(Table), "Table", TableCheck, NULL);
    GC_register_class(thgc, CType::_NewTable,sizeof(NewTable), "NewTable", NewTableCheck, NULL);
    GC_register_class(thgc, CType::_ColumnInfo,sizeof(ColumnInfo), "ColumnInfo", RowInfoCheck, NULL);
    GC_register_class(thgc, CType::_RowInfo,sizeof(RowInfo), "RowInfo", RowInfoCheck, NULL);
    GC_register_class(thgc, CType::_Column,sizeof(Column), "Column", ColumnCheck, NULL);
    // _TreeElement は廃止 (= NewElement に tag* フィールドを統合済み)
    GC_register_class(thgc, CType::_MemTable,sizeof(MemTable), "MemTable", MemTableCheck, NULL);
    GC_register_class(thgc, CType::_MemFunc,sizeof(MemFunc), "MemFunc", MemFuncCheck, NULL);
    GC_register_class(thgc, CType::_MemObj,sizeof(MemObj), "MemObj", MemObjCheck, NULL);
    GC_register_class(thgc, CType::_MouseEvent,sizeof(MouseEvent), "MouseEvent", MouseEventCheck, NULL);
    GC_register_class(thgc, CType::_KeyEvent,sizeof(KeyEvent), "KeyEvent", KeyEventCheck, NULL);
    GC_register_class(thgc, CType::_FuncTypeR,sizeof(FuncType), "FuncTypeR", FuncTypeRCheck, NULL);
    GC_register_class(thgc, CType::_Offscreen,sizeof(Offscreen), "Offscreen", OffscreenCheck, NULL);
    GC_register_class(thgc, CType::_OffscreenEnd,sizeof(OffscreenEnd), "OffscreenEnd", OffscreenEndCheck, NULL);
    GC_register_class(thgc, CType::_ATSSpan,sizeof(ATSSpan), "ATSSpan", ATSSpanCheck, NULL);
    GC_register_class(thgc, CType::_StyleSpan,sizeof(StyleSpan), "StyleSpan", StyleSpanCheck, NULL);
    GC_register_class(thgc, CType::_RenderSpan,sizeof(RenderSpan), "RenderSpan", RenderSpanCheck, NULL);
    GC_register_class(thgc, CType::_HoveredSpan,sizeof(HoveredSpan), "HoveredSpan", HoveredSpanCheck, NULL);
    GC_register_class(thgc, CType::_BreakLetterC,sizeof(BreakLetter), "BreakLetter", BreakLetterCheck, NULL);
    GC_register_class(thgc, CType::_CObj,sizeof(Obj), "Obj", ObjCheck, NULL);
    GC_register_class(thgc, CType::_Block,sizeof(Block), "Block", BlockCheck, NULL);
    GC_register_class(thgc, CType::_CBlock,sizeof(Block), "CBlock", BlockCheck, NULL);
    GC_register_class(thgc, CType::_CallBlock,sizeof(Block), "CallBlock", BlockCheck, NULL);
    GC_register_class(thgc, CType::_TagBlock,sizeof(TagBlock), "TagBlock", TagBlockCheck, NULL);
    GC_register_class(thgc, CType::_Primary,sizeof(Primary), "Primary", PrimaryCheck, NULL);
    GC_register_class(thgc, CType::_COperator,sizeof(Operator), "COperator", OperatorCheck, NULL);
    GC_register_class(thgc, CType::_Word,sizeof(Word), "Word", WordCheck, NULL);
    GC_register_class(thgc, CType::_CNumber,sizeof(Number), "CNumber", NumberCheck, NULL);
    GC_register_class(thgc, CType::_FloatVal,sizeof(FloatVal), "FloatVal", FloatValCheck, NULL);
    GC_register_class(thgc, CType::_StrObj,sizeof(StrObj), "StrObj", StrObjCheck, NULL);
    GC_register_class(thgc, CType::_Address,sizeof(Address), "Address", AddressCheck, NULL);
    GC_register_class(thgc, CType::_Comment,sizeof(Comment), "Comment", CommentCheck, NULL);
    GC_register_class(thgc, CType::_CommentLet,sizeof(CommentLet), "CommentLet", CommentLetCheck, NULL);
    GC_register_class(thgc, CType::_Comment2,sizeof(Obj), "Comment2", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CommentType,sizeof(CommentType), "CommentType", CommentTypeCheck, NULL);
    GC_register_class(thgc, CType::_HtmObj,sizeof(Obj), "HtmObj", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CMountain,sizeof(Obj), "CMountain", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CQuestion,sizeof(Obj), "CQuestion", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CDolor,sizeof(Obj), "CDolor", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CClones,sizeof(Clones), "CClones", ClonesCheck, NULL);
    GC_register_class(thgc, CType::_CAnimationFunction,sizeof(AnimationFunction), "CAnimationFunction", AnimationFunctionCheck, NULL);
    GC_register_class(thgc, CType::_PrimOp,sizeof(PrimOp), "PrimOp", PrimOpCheck, NULL);
    GC_register_class(thgc, CType::_SingleOp,sizeof(SingleOp), "SingleOp", SingleOpCheck, NULL);
    GC_register_class(thgc, CType::_Label,sizeof(Label), "Label", LabelCheck, NULL);
    GC_register_class(thgc, CType::_Master,sizeof(Master), "Local", MasterCheck, NULL);
    GC_register_class(thgc, CType::_Operator,sizeof(Operator), "Operator", OperatorCheck, NULL);
    GC_register_class(thgc, CType::_CClass,sizeof(Obj), "CClass", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CVar,sizeof(Var), "CVar", VarCheck, NULL);
    GC_register_class(thgc, CType::_CIf,sizeof(Obj), "CIf", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CIfBlock,sizeof(IfBlock), "CIfBlock", IfBlockCheck, NULL);
    GC_register_class(thgc, CType::_CElif,sizeof(Obj), "CElif", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CElse,sizeof(Obj), "CElse", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CWhile,sizeof(Obj), "CWhile", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CFor,sizeof(Obj), "CFor", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CSwitch,sizeof(Obj), "CSwitch", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CReturn,sizeof(Return), "CReturn", ReturnCheck, NULL);
    GC_register_class(thgc, CType::_CGoto,sizeof(Goto), "CGoto", GotoCheck, NULL);
    GC_register_class(thgc, CType::_CContinue,sizeof(Continue), "CContinue", ContinueCheck, NULL);
    GC_register_class(thgc, CType::_CSignal,sizeof(Obj), "CSignal", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CPrint,sizeof(Obj), "CPrint", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CModel,sizeof(Obj), "CModel", ObjCheck, NULL);
    GC_register_class(thgc, CType::_ModelObj,sizeof(ModelObj), "ModelObj", ModelObjCheck, NULL);
    GC_register_class(thgc, CType::_CGene,sizeof(Generic), "CGene", GenericCheck, NULL);
    GC_register_class(thgc, CType::_CGeneLabel,sizeof(GeneLabel), "CGeneLabel", GeneLabelCheck, NULL);
    GC_register_class(thgc, CType::_CBoolVal,sizeof(Obj), "CBoolVal", ObjCheck, NULL);
    GC_register_class(thgc, CType::_CServerClient,sizeof(ServerClient), "CServerClient", ServerClientCheck, NULL);
    // Compile types
    GC_register_class(thgc, CType::_VariClass,sizeof(VariClass), "VariClass", VariClassCheck, NULL);
    GC_register_class(thgc, CType::_ArrType,sizeof(ArrType), "ArrType", ArrTypeCheck, NULL);
    GC_register_class(thgc, CType::_FuncTypeC,sizeof(FuncTypeC), "FuncType", FuncTypeCCheck, NULL);
    GC_register_class(thgc, CType::_Variable,sizeof(Variable), "Variable", VariableCheck, NULL);
    GC_register_class(thgc, CType::_CVariable,sizeof(Variable), "CVariable", VariableCheck, NULL);
    GC_register_class(thgc, CType::_Function,sizeof(Function), "Function", FunctionCheck, NULL);
    GC_register_class(thgc, CType::_GenericFunction,sizeof(GenericFunction), "GenericFunction", GenericFunctionCheck, NULL);
    GC_register_class(thgc, CType::_ClassObj,sizeof(ClassObj), "ClassObj", ClassObjCheck, NULL);
    GC_register_class(thgc, CType::_GeneObj,sizeof(GeneObj), "GeneObj", GeneObjCheck, NULL);
    GC_register_class(thgc, CType::_GenericObj,sizeof(GenericObj), "GenericObj", GenericObjCheck, NULL);
    GC_register_class(thgc, CType::_ObjBlock,sizeof(ObjBlock), "ObjBlock", ObjBlockCheck, NULL);
    GC_register_class(thgc, CType::_GenericType,sizeof(Generic), "GenericType", GenericCheck, NULL);
    GC_register_class(thgc, CType::_Base,sizeof(Base), "Base", BaseCheck, NULL);
    GC_register_class(thgc, CType::_Stock,sizeof(Stock), "Stock", StockCheck, NULL);
    GC_register_class(thgc, CType::_GeneChild,sizeof(GeneChild), "GeneChild", GeneChildCheck, NULL);
    GC_register_class(thgc, CType::_GeneMutate,sizeof(GeneMutate), "GeneMutate", GeneMutateCheck, NULL);
    GC_register_class(thgc, CType::_GeneNew,sizeof(GeneNew), "GeneNew", GeneNewCheck, NULL);
    GC_register_class(thgc, CType::_GeneCross,sizeof(GeneCross), "GeneCross", GeneCrossCheck, NULL);
    GC_register_class(thgc, CType::_GeneStore,sizeof(GeneStore), "GeneStore", GeneStoreCheck, NULL);
    GC_register_class(thgc, CType::_GeneSelect,sizeof(GeneSelect), "GeneSelect", GeneSelectCheck, NULL);
    GC_register_class(thgc, CType::_GeneSort,sizeof(GeneSort), "GeneSort", GeneSortCheck, NULL);
    GC_register_class(thgc, CType::_Constructor,sizeof(Constructor), "Constructor", ConstructorCheck, NULL);
    GC_register_class(thgc, CType::_ArrayConstructor,sizeof(ArrayConstructor), "ArrayConstructor", ArrayConstructorCheck, NULL);
    GC_register_class(thgc, CType::_SqlString,sizeof(SqlString), "SqlString", SqlStringCheck, NULL);
    GC_register_class(thgc, CType::_CVal,sizeof(Val), "Val", ValCheck, NULL);
    GC_register_class(thgc, CType::_CValue,sizeof(Value), "Value", ValueCheck, NULL);
    GC_register_class(thgc, CType::_CIterator,sizeof(Val), "Iterator", ValCheck, NULL);
    // LLVM types
    GC_register_class(thgc, CType::_SB,sizeof(SB), "SB", SBCheck, NULL);
    GC_register_class(thgc, CType::_LComp,sizeof(LComp), "LComp", LCompCheck, NULL);
    GC_register_class(thgc, CType::_LVari,sizeof(LVari), "LVari", LVariCheck, NULL);
    GC_register_class(thgc, CType::_LStrV,sizeof(LStrV), "LStrV", LStrVCheck, NULL);
    GC_register_class(thgc, CType::_LLab,sizeof(LLab), "LLab", LLabCheck, NULL);
    GC_register_class(thgc, CType::_LIfValue,sizeof(LIfValue), "LIfValue", LIfValueCheck, NULL);
    GC_register_class(thgc, CType::_LPhi,sizeof(LPhi), "LPhi", LPhiCheck, NULL);
    GC_register_class(thgc, CType::_LTypeDec,sizeof(LTypeDec), "LTypeDec", LTypeDecCheck, NULL);
    GC_register_class(thgc, CType::_LTypeVal,sizeof(LTypeVal), "LTypeVal", LTypeValCheck, NULL);
    GC_register_class(thgc, CType::_LFunc,sizeof(LFunc), "LFunc", LFuncCheck, NULL);
    GC_register_class(thgc, CType::_LGete,sizeof(LGete), "LGete", LGeteCheck, NULL);
    GC_register_class(thgc, CType::_LCast,sizeof(LCast), "LCast", LCastCheck, NULL);
    GC_register_class(thgc, CType::_LCall,sizeof(LCall), "LCall", LCallCheck, NULL);
    GC_register_class(thgc, CType::_LAlloca,sizeof(LAlloca), "LAlloca", LAllocaCheck, NULL);
    GC_register_class(thgc, CType::_LStore,sizeof(LStore), "LStore", LStoreCheck, NULL);
    GC_register_class(thgc, CType::_LLoad,sizeof(LLoad), "LLoad", LLoadCheck, NULL);
    GC_register_class(thgc, CType::_LBr,sizeof(LBr), "LBr", LBrCheck, NULL);
    GC_register_class(thgc, CType::_LSwitchComp,sizeof(LSwitchComp), "LSwitchComp", LSwitchCompCheck, NULL);
    GC_register_class(thgc, CType::_LBinOp,sizeof(LBinOp), "LBinOp", LBinOpCheck, NULL);
    GC_register_class(thgc, CType::_LCmp,sizeof(LCmp), "LCmp", LCmpCheck, NULL);
    GC_register_class(thgc, CType::_LLocationMarker,sizeof(LLocationMarker), "LLocationMarker", LLocationMarkerCheck, NULL);
    GC_register_class(thgc, CType::_LRet,sizeof(LRet), "LRet", LRetCheck, NULL);
    GC_register_class(thgc, CType::_LDebugInfo,sizeof(LDebugInfo), "LDebugInfo", LDebugInfoCheck, NULL);
    GC_register_class(thgc, CType::_LDebugVariable,sizeof(LDebugVariable), "LDebugVariable", LDebugVariableCheck, NULL);
    GC_register_class(thgc, CType::_LStructField,sizeof(LStructField), "LStructField", LStructFieldCheck, NULL);
    GC_register_class(thgc, CType::_LStructTypeInfo,sizeof(LStructTypeInfo), "LStructTypeInfo", LStructTypeInfoCheck, NULL);
    GC_register_class(thgc, CType::_LLLVM,sizeof(LLLVM), "LLLVM", LLLVMCheck, NULL);
    GC_register_class(thgc, CType::_CElemObj,sizeof(ElemObj), "ElemObj", ElemObjCheck, NULL);
    GC_register_class(thgc, CType::_CConnectStock,sizeof(ConnectStock), "ConnectStock", ConnectStockCheck, NULL);
    GC_register_class(thgc, CType::_Migrate,sizeof(MigrateObj), "Migrate", MigrateObjCheck, NULL);
    GC_register_class(thgc, CType::_OneMigrate,sizeof(OneMigrate), "OneMigrate", OneMigrateCheck, NULL);
    // LLDB クライアント (= lldb.h で定義)。tracer は List* メンバ更新。
    GC_register_class(thgc, CType::_LLDBClient,sizeof(LLDBClient),"LLDBClient",LLDBClientCheck, LLDBClientFinalize, /*has_io=*/true);
    GC_register_class(thgc, CType::_BpEntry,   sizeof(BpEntry),   "BpEntry",   BpEntryCheck,   NULL);
    GC_register_class(thgc, CType::_VarRow,    sizeof(VarRow),    "VarRow",    VarRowCheck,    NULL);
    GC_register_class(thgc, CType::_FrameRow,  sizeof(FrameRow),  "FrameRow",  FrameRowCheck,  NULL);
}

// GoThread - shared code for all platforms
#if TARGET_OS_IOS || TARGET_OS_SIMULATOR || defined(__ANDROID__) || defined(__linux__) || TARGET_OS_OSX
void* GoThread(ThreadGC* thgc) {
#else
CustomModuleImpl* GoThread(ThreadGC* thgc) {
#endif
    if (!thgc->local) {
        registerGCClasses(thgc);
        NewLocal* local = (NewLocal*)GC_alloc(thgc, CType::_LocalC);
        NativeWindow* mainWin = (!thgc->windows.empty()) ? thgc->windows[0] : nullptr;
        initLocal(thgc, local, mainWin);
        if (mainWin) mainWin->local = (NewElement*)local;

        // 静的 root 登録 (= GC 全期間生存させる thgc 関連 + NativeWindow 内 UI ツリー)
        RootNode* rn = GC_add_root_node(thgc);
        GC_add_root(rn, (char**)&thgc->local);
        GC_add_root(rn, (char**)&thgc->map);
        GC_add_root(rn, (char**)&thgc->hoveredList);
        GC_add_root(rn, (char**)&thgc->hoveredSpanList);
        for (NativeWindow* nw : thgc->windows) {
            if (nw) {
                GC_add_root(rn, (char**)&nw->local);
                GC_add_root(rn, (char**)&nw->anchorElement);
            }
        }
    }
    NewLocal* local = thgc->local;
    local->font = getFont("sans", 16);
    local->xtype = SizeType::Range;
    local->ytype = SizeType::Range;
    local->offscreen->spread = false;
    Sidelet* sidelet = (Sidelet*)GC_alloc(thgc, CType::_SideletC);
    sidelet->hide = true;
    sidelet->percent = 0.7;
    sidelet->direction = 0;
    initSidelet(thgc, sidelet);
    sidelet->size.y = 250; sidelet->ytype = SizeType::Range;
    setPercentX(sidelet, 1.0, 0);
    sidelet->layout = Layout::_OverNone;
	sidelet->animate = -1.0;
    sidelet->orient = true;
    sidelet->zIndex = 2.0f;
    sidelet->offscreen->markLayout(local, sidelet);
    NewDirectAddLast(thgc, local, local, sidelet);
	NewTable* table = (NewTable*)GC_alloc(thgc, CType::_NewTable);
    initTable(thgc, table);
    table->gridColor = 0x000000FF;
    table->treeView = true;
    table->treeIconSize = Scf(10);
    table->treeIndent = Scf(16);
    setPercentX(table, 0.5, 0, SizeType::Range);
	setPercentY(table, 1.0, 0, SizeType::Page);
	NewDirectAddLast(thgc, local, sidelet, table);
	NewLine* iline = (NewLine*)GC_alloc(thgc, CType::_LineC);
	initLine(thgc, iline);
	NewDirectAddLast(thgc, local, table, iline);
    setTableCell(thgc, table, 0, 0, iline);
	NewLetter* ilet = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
	initLetter(thgc, ilet, getFont("sans", 16), LetterType::_Letter);
	ilet->text = createString(thgc, "name", 4, 1);
	NewDirectAddLast(thgc, local, iline, ilet);
    iline = (NewLine*)GC_alloc(thgc, CType::_LineC);
    initLine(thgc, iline);
    NewDirectAddLast(thgc, local, table, iline);
    setTableCell(thgc, table, 0, 1, iline);
    ilet = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, ilet, getFont("sans", 16), LetterType::_Letter);
    ilet->text = createString(thgc, "value", 5, 1);
    NewDirectAddLast(thgc, local, iline, ilet);
    iline = (NewLine*)GC_alloc(thgc, CType::_LineC);
    initLine(thgc, iline);
    NewDirectAddLast(thgc, local, table, iline);
    setTableCell(thgc, table, 0, 2, iline);
    ilet = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, ilet, getFont("sans", 16), LetterType::_Letter);
    ilet->text = createString(thgc, "type", 4, 1);
    NewDirectAddLast(thgc, local, iline, ilet);
    auto c = tableCol(table, 0);
    c->mode = TableLineMode::Percent;
    c->width = 100.30;
    c = tableCol(table, 1);
    c->mode = TableLineMode::Percent;
    c->width = 100.35;
    c = tableCol(table, 2);
    c->mode = TableLineMode::Percent;
    c->width = 100.35;
    table->offscreen->markLayout(local, table);
    NewTable* table2 = (NewTable*)GC_alloc(thgc, CType::_NewTable);
    initTable(thgc, table2);
    table2->gridColor = 0x000000FF;
    setPercentX(table2, 0.5, 0, SizeType::Range);
    setPercentY(table2, 1.0, 0, SizeType::Scroll);
    NewDirectAddLast(thgc, local, sidelet, table2);
    iline = (NewLine*)GC_alloc(thgc, CType::_LineC);
    initLine(thgc, iline);
    NewDirectAddLast(thgc, local, table2, iline);
    setTableCell(thgc, table2, 0, 0, iline);
    ilet = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, ilet, getFont("sans", 16), LetterType::_Letter);
    ilet->text = createString(thgc, "func", 4, 1);
    NewDirectAddLast(thgc, local, iline, ilet);
    iline = (NewLine*)GC_alloc(thgc, CType::_LineC);
    initLine(thgc, iline);
    NewDirectAddLast(thgc, local, table2, iline);
    setTableCell(thgc, table2, 0, 1, iline);
    ilet = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, ilet, getFont("sans", 16), LetterType::_Letter);
    ilet->text = createString(thgc, "file", 4, 1);
    NewDirectAddLast(thgc, local, iline, ilet);
    iline = (NewLine*)GC_alloc(thgc, CType::_LineC);
    initLine(thgc, iline);
    NewDirectAddLast(thgc, local, table2, iline);
    setTableCell(thgc, table2, 0, 2, iline);
    ilet = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, ilet, getFont("sans", 16), LetterType::_Letter);
    ilet->text = createString(thgc, "line", 4, 1);
    NewDirectAddLast(thgc, local, iline, ilet);
    c = tableCol(table2, 0);
    c->mode = TableLineMode::Percent;
    c->width = 100.40;
    c = tableCol(table2, 1);
    c->mode = TableLineMode::Percent;
    c->width = 100.30;
    c = tableCol(table2, 2);
    c->mode = TableLineMode::Percent;
    c->width = 100.30;
    table2->offscreen->markLayout(local, table2);
    NewLine* line3 = (NewLine*)GC_alloc(thgc, CType::_LineC);
    initLine(thgc, line3);
    NewDirectAddLast(thgc, local, local, line3);
    Sidelet* sidelet2 = (Sidelet*)GC_alloc(thgc, CType::_SideletC);
    sidelet2->hide = true;
    sidelet2->percent = 0.7;
    sidelet2->direction = 1;
    sidelet2->zIndex = 1.0f;
    initSidelet(thgc, sidelet2);
    sidelet2->size.x = 150; sidelet2->xtype = SizeType::Range;
    setPercentY(sidelet2, 1.0, 0);
    sidelet2->layout = Layout::_ZeroNone;
    NewDirectAddLast(thgc, local, line3, sidelet2);
    Sidelet* sidelet3 = (Sidelet*)GC_alloc(thgc, CType::_SideletC);
    sidelet3->hide = true;
    sidelet3->percent = 0.7;
    sidelet3->direction = 3;
    sidelet3->zIndex = 1.0f;
    initSidelet(thgc, sidelet3);
    sidelet3->size.x = 200; sidelet3->xtype = SizeType::Range;
    setPercentY(sidelet3, 1.0, 0);
    sidelet3->layout = Layout::_OverNone;
    std::vector<uint32_t> colors = std::vector<uint32_t>{ 0x202F3CFF, 0x545C6F };
    std::vector<float> widths = std::vector<float>{ -1.0f, -0.2f };
    int n = addPattern(thgc, colors, widths);
    sidelet3->background->offset = n;
    sidelet3->background->angle = -3.14 / 2;
    int size = widths.size();
    sidelet3->background->count = size;
    sidelet3->background->type = DrawCommandType::Gradient;
    sidelet3->animate = -1.0;
    NewDirectAddLast(thgc, local, line3, sidelet3);
    Button* button = (Button*)GC_alloc(thgc, CType::_ButtonC);
    initButton(thgc, button);
    button->size.x = Sc(100); button->xtype = SizeType::Range;
    button->size.y = Sc(24);  button->ytype = SizeType::Range;
    button->GoMouseDown = (MemFunc*)GC_alloc(thgc, CType::_MemFunc);
    button->GoMouseDown->func = ButtonClick;
    button->GoMouseDown->obj = (MemObj*)GC_alloc(thgc, CType::_MemObj);
    NewDirectAddLast(thgc, local, sidelet3, button);
    NewLetter* let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, let, getFont("sans", 16), LetterType::_Letter);
    let->text = createString(thgc, "Run", 3, 1);
    NewDirectAddLast(thgc, local, button, let);
    Button* openfile = (Button*)GC_alloc(thgc, CType::_ButtonC);
    initButton(thgc, openfile);
    openfile->size.x = Sc(100); openfile->xtype = SizeType::Range;
    openfile->size.y = Sc(24); openfile->ytype = SizeType::Range;
    openfile->GoMouseDown = (MemFunc*)GC_alloc(thgc, CType::_MemFunc);
    openfile->GoMouseDown->func = OpenFile;
    openfile->GoMouseDown->obj = (MemObj*)GC_alloc(thgc, CType::_MemObj);
    NewDirectAddLast(thgc, local, sidelet3, openfile);
    let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, let, getFont("sans", 16), LetterType::_Letter);
    let->text = createString(thgc, "Open", 4, 1);
    NewDirectAddLast(thgc, local, openfile, let);
    Button* cont = (Button*)GC_alloc(thgc, CType::_ButtonC);
    initButton(thgc, cont);
    cont->size.x = Sc(100); cont->xtype = SizeType::Range;
    cont->size.y = Sc(24); cont->ytype = SizeType::Range;
    cont->GoMouseDown = (MemFunc*)GC_alloc(thgc, CType::_MemFunc);
    cont->GoMouseDown->func = ContinueClick;
    cont->GoMouseDown->obj = (MemObj*)GC_alloc(thgc, CType::_MemObj);
    NewDirectAddLast(thgc, local, sidelet3, cont);
    let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, let, getFont("sans", 16), LetterType::_Letter);
    let->text = createString(thgc, "Cotinue", 4, 1);
    NewDirectAddLast(thgc, local, cont, let);
    Button* stepin = (Button*)GC_alloc(thgc, CType::_ButtonC);
    initButton(thgc, stepin);
    stepin->size.x = Sc(100); stepin->xtype = SizeType::Range;
    stepin->size.y = Sc(24); stepin->ytype = SizeType::Range;
    stepin->GoMouseDown = (MemFunc*)GC_alloc(thgc, CType::_MemFunc);
    stepin->GoMouseDown->func = StepInClick;
    stepin->GoMouseDown->obj = (MemObj*)GC_alloc(thgc, CType::_MemObj);
    NewDirectAddLast(thgc, local, sidelet3, stepin);
    let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, let, getFont("sans", 16), LetterType::_Letter);
    let->text = createString(thgc, "Step In", 7, 1);
    NewDirectAddLast(thgc, local, stepin, let);
    Button* stepover = (Button*)GC_alloc(thgc, CType::_ButtonC);
    initButton(thgc, stepover);
    stepover->size.x = Sc(100); stepover->xtype = SizeType::Range;
    stepover->size.y = Sc(24); stepover->ytype = SizeType::Range;
    stepover->GoMouseDown = (MemFunc*)GC_alloc(thgc, CType::_MemFunc);
    stepover->GoMouseDown->func = StepOverClick;
    stepover->GoMouseDown->obj = (MemObj*)GC_alloc(thgc, CType::_MemObj);
    NewDirectAddLast(thgc, local, sidelet3, stepover);
    let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, let, getFont("sans", 16), LetterType::_Letter);
    let->text = createString(thgc, "Step Over", 9, 1);
    NewDirectAddLast(thgc, local, stepover, let);
    Button* stepout = (Button*)GC_alloc(thgc, CType::_ButtonC);
    initButton(thgc, stepout);
    stepout->size.x = Sc(100); stepout->xtype = SizeType::Range;
    stepout->size.y = Sc(24); stepout->ytype = SizeType::Range;
    stepout->GoMouseDown = (MemFunc*)GC_alloc(thgc, CType::_MemFunc);
    stepout->GoMouseDown->func = StepOutClick;
    stepout->GoMouseDown->obj = (MemObj*)GC_alloc(thgc, CType::_MemObj);
    NewDirectAddLast(thgc, local, sidelet3, stepout);
    let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, let, getFont("sans", 16), LetterType::_Letter);
    let->text = createString(thgc, "Step Out", 8, 1);
    NewDirectAddLast(thgc, local, stepout, let);
    NativeWindow* mainWin = (!thgc->windows.empty()) ? thgc->windows[0] : nullptr;
    if (mainWin) mainWin->local = (NewElement*)local;
    local->font = getFont("sans", 16);
	NewElement* elem = (NewElement*)GC_alloc(thgc, CType::_ElementC);
    initElement(thgc, elem);
    elem->zIndex = 0.0;
    setPercentX(elem, 1.0, 0, SizeType::Break);
    setPercentY(elem, 1.0, 0, SizeType::Range);
    NewDirectAddLast(thgc, local, line3, elem);
	// tab はクロスヒープ alias の登録元 (tab->linked->page = thgc の local)。Old 確保。
	NewTabBar* tab = (NewTabBar*)GC_alloc_old(thgc, CType::_TabC);
    tab->orient = true;
	initTab(thgc, tab);
    tab->layout = Layout::_Zero;
    NewDirectAddLast(thgc, local, elem, tab);
	NewTabTitle* title1 = (NewTabTitle*)GC_alloc(thgc, CType::_TabTitleC);
    initTabTitle(thgc, title1);
	NewDirectAddLast(thgc, local, tab, title1);
	NewLetter* titleLet1 = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
    initLetter(thgc, titleLet1, local->font, LetterType::_Letter);
	titleLet1->text = createString(thgc, "tes.cy", 6, 1);
	NewElementAddLast(thgc, local, title1, titleLet1);
	// linked は alias 登録元 (linked->page = local)。Old 確保で field address 永続化。
	LinkedElement* linked = (LinkedElement*)GC_alloc_old(thgc, CType::_LinkedC);
	initLinkedElement(thgc, linked);
    tab->linked = linked;
    setPercentX(linked, 1.0, 0, SizeType::Range);
    setPercentY(linked, 1.0, 0, SizeType::Range);
    NewDirectAddLast(thgc, local, elem, linked);
    NativeWindow* linkWin = new NativeWindow();
    linkWin->sdlWindow = mainWin->sdlWindow;
    linkWin->size = mainWin->size;
    linkWin->viewId = mainWin->viewId;
    linkWin->type = WindowType_Main;
    linkWin->nwh = mainWin->nwh;
	thgc->linkwins.push_back(linkWin);
    // linkWin の GC ポインタ field を root 登録
    linkWin->rootNode = GC_add_root_node(thgc);
    GC_add_root(linkWin->rootNode, (char**)&linkWin->local);
    GC_add_root(linkWin->rootNode, (char**)&linkWin->anchorElement);
    TextBox* tb = (TextBox*)GC_alloc(thgc, CType::_TextBoxC);
    linkWin->local = tb;
    initTb(thgc, tb);
    tb->childend->id = std::numeric_limits<uint64_t>::max();
    tb->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
    tb->offscreen->group = &createGroup(thgc);
    tb->offscreen->elem = tb;
    tb->offscreen->type = OffscreenType::Off;
    tb->offscreen->markLayout(local, tb);
    tb->offscreen->curlProgress = -1.0f;
    tb->offscreen->window = linkWin;
    tb->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
    tb->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
    tb->offscreen->next = local->offscreen->before = local->offscreen;
    tb->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
    tb->offscreen->child->next = tb->offscreen->child->before = tb->offscreen->child;
    tb->offscreen->child->parent = NULL;
    tb->offscreened = true;
    tb->zIndex = 0.0;
    tb->font = getFont("sans", 16);
	tb->xtype = SizeType::Break;
	tb->ytype = SizeType::Page;
    tb->background = (Background*)GC_alloc(thgc, CType::_Background);
    tb->background->fillcolor = 0xffffffff;
    tb->background->bs.borderColor = 0x000000FF;
    tb->boWidthes[0] = tb->boWidthes[1] = tb->boWidthes[2] = tb->boWidthes[3] = 0.0f;
    tb->background->bs.shadowBlur = 1.0f;
    tb->background->bs.shadowColor = 0x00000000;
    NewLine* line = (NewLine*)GC_alloc(thgc, CType::_LineC);
    initLine(thgc, line);
    NewDirectAddLast(thgc, local, tb, line);
    title1->page = tb;
    tab->select = title1;
    linked->page = title1->page;
    title1->page->parent = tab->linked;
    /*
    for (int i = 0; i < 3; i++) {
        NewLine* line = (NewLine*)GC_alloc(thgc, CType::_LineC);
        initLine(thgc, line);
        NewDirectAddLast(thgc, local, tb, line);
        NewLetter* letter = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
        initLetter(thgc, letter, getFont("sans", 16), LetterType::_Letter);
        letter->text = createString(thgc, (char*)"Hello,world!", 12, 1);
        letter->color = 0xFFFFFFFF;
        NewElementAddLast(thgc, local, tb, (NewElement*)letter);
    }
    int height = myGetFontHeight(thgc, getFont("sans", 16));
    for (int i = 0; i < 15; i++) {
        NewLine* line = (NewLine*)GC_alloc(thgc, CType::_LineC);
        initLine(thgc, line);
        NewDirectAddLast(thgc, local, tb, line);
        NewLetter* letter = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
        initLetter(thgc, letter, getFont("sans", 16), LetterType::_Letter);
        letter->text = createString(thgc, (char*)"Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!Hello,world!", 180, 1);
        letter->color = 0xFFFFFFFF;
        NewElementAddLast(thgc, local, tb, (NewElement*)letter);
    }
    */
    // thgc->map は generic な「id → 任意 GC オブジェクト」のレジストリ。
    // tag 系フィールド (= 旧 tagId 等) は NewElement から削除済みなので、
    // map 登録だけ行う (= 要素側に id を覚えさせない)。
    // dtype は非 0 (= GC trace 有効) にする。0 (= _Struct) だと KVCheck が
    // kv->value を GC_clone せず、登録した NewElement 等が GC で消える。
    // 実際の relocate は GC オブジェクトのヘッダ型で動くので、ここの値は「非 0 の
    // 任意 GC 型」で OK。代表として _ElementC を使う (= 主に NewElement 派生を入れるので)。
    thgc->map = create_mapy(magc, _ElementC);
    add_mapy(thgc, thgc->map, createString(thgc, (char*)"main",  4, 1), (char*)local);
    add_mapy(thgc, thgc->map, createString(thgc, (char*)"text",  4, 1), (char*)tb);
    add_mapy(thgc, thgc->map, createString(thgc, (char*)"tab",   3, 1), (char*)tab);
    add_mapy(thgc, thgc->map, createString(thgc, (char*)"table", 5, 1), (char*)table);
    /*ReadDll(thgc);*/

    // UI 状態 (= 開いたファイル / BP) 永続化用 SQLite を初期化。
    // FileEngine 経由でアプリ専用ストレージのパスを取って、SqlPool を 1 worker で開く。
    {
        auto* fe = getFileEngine();
        std::string statePath = fe ? fe->getInternalPath("state.db") : "state.db";
        state::init(thgc, statePath.c_str());
    }
    // 既存 state を replay: 前回終了時に開いていたファイルを再オープン、BP を再設定。
    // ファイル読み込みは LoadFileAsync 経路で UI を止めない。
    state::replay(thgc,
        // openCb: 1 ファイルごとに呼ばれる
        [](ThreadGC* thgc, const char* path) {
            std::string p(path);
            LoadFileAsync(thgc, p, [p](ThreadGC* thgc, Stream* s) {
                if (!s) {
                    // ファイル不在 / 読み取り失敗 → state.db から消す (= 次回 replay 対象外)。
                    SDL_Log("[state] replay open failed, removing from db: %s", p.c_str());
                    state::remove_open(thgc, p);
                    return;
                }
                NewElement* viewer = nullptr;
                switch (detectFileKind(p.c_str(), s)) {
                case FileKind::Text:
                case FileKind::SourceCode:
                    viewer = MakeRichTextBox(thgc, ToText(thgc, s, Encoding::Auto)); break;
                case FileKind::Image:
                    viewer = MakeImageView(thgc, ToImage(thgc, s)); break;
                case FileKind::Audio:
                    viewer = MakeAudioPlayer(thgc, ToSound(thgc, s)); break;
                case FileKind::Video:
                    viewer = MakeVideoPlayer(thgc, ToVideoSound(thgc, s)); break;
                default:
                    viewer = MakeTextBox(thgc, ToHex(thgc, s)); break;
                }
                if (!viewer) { return; }
                ensureSIValues(thgc, viewer);
                SIVal pv;
                String* pathStr2 = createString(thgc, (char*)p.c_str(), (int)p.size(), 1);
                pv.str = pathStr2;
                // setSIValue は bit 操作のみで GC を起こさない → pathStr2 は viewer->values に入った時点で
                // collection 防止 (viewer rooted)。add_mapy は内部で key を root するので move にも追従。
                // 以降 pathStr2 を caller 側で参照しないため、ここで GC_ROOT 不要。
                setSIValue(viewer->values, FILE_PATH_SLOT, SIType::String, pv);
                // path → viewer 逆引き登録 (= スタックフレームで viewer を引くため)
                add_mapy(thgc, thgc->map, pathStr2, (char*)viewer);
                // BP のガター赤丸を復元 (= LLDBClient.breakpoints は bpCb で先行充填済み)
                paintBreakpointsForFile(thgc, thgc->local, viewer, p.c_str());
                // タブ名 = ファイル表示名。Android SAF (content://) URI のときは
                // ContentResolver から DISPLAY_NAME を取得 (= "tone.xml" 等)。
                // 失敗時 / 通常 path は末尾セグメントをフォールバックで使う。
                std::string displayName;
#ifdef __ANDROID__
                if (p.rfind("content://", 0) == 0) {
                    displayName = androidDisplayNameForUri(p);
                }
#endif
                if (displayName.empty()) {
                    const char* bn = strrchr(p.c_str(), '/');
                    if (!bn) bn = strrchr(p.c_str(), '\\');
                    bn = bn ? bn + 1 : p.c_str();
                    displayName = bn;
                }
                AddViewerToTab(thgc, viewer, displayName.c_str());
            });
        },
        // bpCb: 1 BP ごとに呼ばれる (= LLDBClient のリストに復元)
        [](ThreadGC* thgc, const char* file, uint32_t line, bool enabled) {
            lldb_setBreakpoint(thgc, getLLDBClient(thgc), file, line, enabled);
        });

    // 既存の "table" の TreeElement とその NewTable* を取り出して保持。
    // BP HIT 時に driver から流れてくる変数列で table を更新するために使う。
    NewTable* extorchVarTable = (NewTable*)table;
    NewTable* extorchFrameTable = (NewTable*)table2;  // 呼出履歴用

    // GoThread の関数 local 変数で、while ループ内 (drainExtorchVars/Frames/Expand) から
    // 参照される GC ポインタは root 登録が必要 (= GC で move された時にローカルが stale 化しないため)。
    // local / table / table2 は while ループの lambda が [&] でキャプチャしている。
    RootNode* funcRn = GC_add_root_node(thgc);
    GC_add_root(funcRn, (char**)&local);
    GC_add_root(funcRn, (char**)&table);
    GC_add_root(funcRn, (char**)&table2);
    GC_add_root(funcRn, (char**)&extorchVarTable);
    GC_add_root(funcRn, (char**)&extorchFrameTable);

    // 「前回 BP HIT で何行追加したか」を覚えて、次の BP HIT で先に削除する。
    int prevVarRowCount = 0;
    int prevFrameRowCount = 0;

    // === 変数展開 onExpand ハンドラ (= NewTable::onExpand に 1 個セット) ===
    // row の col-0 cell (= NewLine NewElement) の values[VAR_PATH_SLOT] から
    // path String* を取り出して driver に送る。
    // values[MORE_OFFSET_SLOT] が UInt64 で入っていれば「more」行 (= 次バッチ要求)。
    // クリック展開時:
    //   - 通常行 (offset=0): expandedPaths に登録 + queue + pump (= replay 対象)
    //   - more 行 (offset>0): in-flight でなければ直接送信 (= 1 回限り、replay 対象外)
    static auto varExpandHandler = +[](ThreadGC* thgc, NewLocal* local, NewTable* t, LineInfo* row) {
        (void)local;
        if (!row || !t || !t->columns || t->columns->size <= 0) return;
        LineInfo* col0 = (LineInfo*)*get_list2(t->columns, 0);
        if (!col0) return;
        NewElement* cellElem = getTableCellById(t, row->id, col0->id);
        if (!cellElem || !cellElem->values || cellElem->values->n <= VAR_PATH_SLOT) return;
        SIVal pv = cellElem->values->values[VAR_PATH_SLOT];
        if (SIVal_type(pv) != SIType::String) return;
        String* path = (String*)(pv.bits & SIV_VAL_MASK);
        if (!path) return;
        // offset 取得 (= more 行ならスロットに UInt64 で入っている)
        uint32_t offset = 0;
        if (cellElem->values->n > MORE_OFFSET_SLOT) {
            SIVal ov = cellElem->values->values[MORE_OFFSET_SLOT];
            if (SIVal_type(ov) == SIType::UInt64) {
                offset = (uint32_t)(ov.u64 & SIV_VAL_MASK);
            }
        }
        LLDBClient* cli = getLLDBClient(thgc);
        if (!cli) return;
        if (offset > 0) {
            // more 行 → 直接送信 (= replay 不要、1 回限り)
            if (atomic_load(&cli->expandInFlight) == 0) {
                if (lldb_requestExpand(cli, path->data, path->size, offset)) {
                    atomic_store(&cli->expandInFlight, 1);
                }
            }
        } else {
            // 通常行 → 記憶 + queue + pump (= step in/out 後に再展開される)
            lldb_rememberExpandPath(thgc, cli, path);
            if (cli->expandRequestQueue) add_list(thgc, cli->expandRequestQueue, (char*)path);
            lldb_pumpExpandQueue(cli);
        }
    };
    // テーブルレベルで 1 度だけセット
    extorchVarTable->onExpand = varExpandHandler;

    auto drainExtorchVars = [&]() {
        LLDBClient* cli = getLLDBClient(thgc);
        if (atomic_exchange(&cli->varsReady, 0) == 0) return;
        // pending を取り出して空にする (= List* 自体は再利用、size=0 にするだけ)
        List* rows = NULL;
        {
            MutexGuard g(&cli->varsMutex);
            // 一旦別 List に move (= 描画中に reader thread が触れないように)
            rows = cli->pendingVars;
            cli->pendingVars = create_list(thgc, sizeof(VarRow*), CType::_VarRow);
        }
        if (!extorchVarTable) { return; }

        // (1) 前回の data rows のセル + child list を再帰的にクリア。
        //     RowInfo は t->rows (root) / parent->childs に残すが、cells map から外して
        //     children list を size=0 にする (= 描画には出ない、容器は次回再利用)。
        int colN = extorchVarTable->columns ? extorchVarTable->columns->size : 0;
        std::function<void(RowInfo*)> clearRowRec = [&](RowInfo* r) {
            if (!r) return;
            for (int c = 0; c < colN; ++c) {
                ColumnInfo* ci = tableCol(extorchVarTable, c);
                if (!ci) continue;
                remove_mapyn(thgc, extorchVarTable->cells, tableKey(r->id, ci->id));
            }
            if (r->childs) {
                for (int k = 0; k < r->childs->size; k++) {
                    clearRowRec((RowInfo*)*get_list2(r->childs, k));
                }
                r->childs->size = 0;
            }
        };
        for (int r = 1; r <= prevVarRowCount; ++r) {
            if (r >= (int)extorchVarTable->rows->size) break;
            clearRowRec(tableRow(extorchVarTable, r));
        }

        // (2) 新しい変数列を tree-aware に書き込む。
        //     depth==0 → t->rows[1..N] に追加。
        //     depth>0  → parent->childs に追加 (= t->rows には入れない)。
        //     stack[d] = 直近 depth d の row。stack[d-1] が新 row の親。
        std::vector<RowInfo*> rowStack;
        std::vector<int>      idxStack;   // 各 depth で次に追加する idx
        std::vector<String*>  pathStack;  // 各 depth の row の path String* (= "tb\x1Fparent" 等)
        for (int i = 0; i < rows->size; ++i) {
            VarRow* vr = *(VarRow**)get_list(rows, i);
            if (!vr) continue;
            // ターゲットリスト + idx 決定
            List* targetList = NULL;
            int targetIdx = 0;
            RowInfo* parentRow = NULL;
            if (vr->depth == 0) {
                targetList = extorchVarTable->rows;
                if ((int)idxStack.size() < 1) idxStack.resize(1, 0);
                targetIdx = ++idxStack[0];   // 1 から開始 (= 0 はヘッダ)
            } else {
                if ((int)rowStack.size() < vr->depth || !rowStack[vr->depth - 1]) continue;
                parentRow = rowStack[vr->depth - 1];
                if (!parentRow->childs) {
                    parentRow->childs = create_list(thgc, (int)sizeof(char*), CType::_RowInfo);
                }
                targetList = parentRow->childs;
                if ((int)idxStack.size() <= vr->depth) idxStack.resize(vr->depth + 1, 0);
                targetIdx = idxStack[vr->depth]++;   // child は 0 から
            }
            NewLine* col0Cell = NULL;
            auto putCell = [&](int col, String* s, RowInfo*& outRow) {
                NewLine* ln = (NewLine*)GC_alloc(thgc, CType::_LineC);
                initLine(thgc, ln);
                NewDirectAddLast(thgc, local, extorchVarTable, ln);
                setTableCell(thgc, extorchVarTable, targetList,
                             extorchVarTable->columns, targetIdx, col, ln);
                NewLetter* lt = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
                initLetter(thgc, lt, getFont("sans", 16), LetterType::_Letter);
                lt->text = s ? s : createString(thgc, (char*)"", 0, 1);
                lt->color = 0x000000FF;
                NewDirectAddLast(thgc, local, ln, lt);
                if (col == 0) {
                    col0Cell = ln;
                    if (targetIdx < targetList->size) {
                        outRow = (RowInfo*)*get_list2(targetList, targetIdx);
                    }
                }
            };
            RowInfo* curRow = NULL;
            putCell(0, vr->name,  curRow);
            putCell(1, vr->value, curRow);
            putCell(2, vr->type,  curRow);
            // 親 / depth / expanded の手当て
            if (curRow) {
                if (parentRow) {
                    curRow->parent = parentRow;
                    curRow->depth  = parentRow->depth + 1;
                }
                // expand=1 → 「展開状態だが子未ロード」、expand=0 → primitive (= 折り畳み相当)
                curRow->expanded = (vr->expand != 0);
                // path 構築: depth==0 なら name そのまま、>0 なら parent.path + sep + name
                String* myPath = vr->name;
                if (vr->depth > 0 && (int)pathStack.size() >= vr->depth && pathStack[vr->depth - 1]) {
                    String* pp = pathStack[vr->depth - 1];
                    int nlen = vr->name ? vr->name->size : 0;
                    int total = pp->size + 1 + nlen;
                    String* cat = createString(thgc, (char*)"", total, 1);
                    memcpy(cat->data, pp->data, pp->size);
                    cat->data[pp->size] = '\x1F';
                    if (nlen > 0) memcpy(cat->data + pp->size + 1, vr->name->data, nlen);
                    cat->size = total;
                    myPath = cat;
                }
                // path は col-0 cell の SIValues に保存 (= LineInfo に新フィールド足さない)
                if (col0Cell) {
                    ensureSIValues(thgc, col0Cell);
                    SIVal pv{}; pv.str = myPath;
                    setSIValue(col0Cell->values, VAR_PATH_SLOT, SIType::String, pv);
                    // more 行: nextOffset > 0 → MORE_OFFSET_SLOT に UInt64 でセット
                    if (vr->nextOffset > 0) {
                        SIVal ov{}; ov.u64 = vr->nextOffset;
                        setSIValue(col0Cell->values, MORE_OFFSET_SLOT, SIType::UInt64, ov);
                    }
                }
                if ((int)rowStack.size() <= vr->depth) rowStack.resize(vr->depth + 1, nullptr);
                if ((int)pathStack.size() <= vr->depth) pathStack.resize(vr->depth + 1, nullptr);
                rowStack[vr->depth] = curRow;
                pathStack[vr->depth] = myPath;
                for (int d = vr->depth + 1; d < (int)rowStack.size(); d++) {
                    rowStack[d] = nullptr;
                    if (d < (int)idxStack.size()) idxStack[d] = 0;
                }
                for (int d = vr->depth + 1; d < (int)pathStack.size(); d++) pathStack[d] = nullptr;
            }
        }
        rebuildLineVisibility(extorchVarTable->rows);
        prevVarRowCount = (idxStack.size() > 0) ? idxStack[0] : 0;

        // 再レイアウト要求
        markLayoutOf(table, local);

        // 記憶している展開 path を全部 queue に積み直して 1 件目 pump (= step 後の自動再展開)
        lldb_replayExpansions(thgc, cli);
        lldb_pumpExpandQueue(cli);
    };

    // === 展開結果ドレイン ===
    // <<<EXPANDVAR>>>...path... を expandResultRows に貯めて、ENDEXPAND で expandReady=1。
    // ここで対応する RowInfo を探して childs に流し込み、次の path を pump する。
    auto drainExtorchExpand = [&]() {
        LLDBClient* cli = getLLDBClient(thgc);
        if (atomic_exchange(&cli->expandReady, 0) == 0) return;
        if (!extorchVarTable) return;
        String* resultPath = NULL;
        List*   resultRows = NULL;
        {
            MutexGuard g(&cli->expandMutex);
            resultPath = cli->expandResultPath;
            resultRows = cli->expandResultRows;
            // 取り出して状態クリア (= 次の応答用)
            cli->expandResultPath = NULL;
            cli->expandResultRows = NULL;
        }
        if (!resultPath || !resultRows) {
            lldb_pumpExpandQueue(cli);
            return;
        }
        // path で RowInfo を線形検索 (= visible-DFS、要素数は数十)。
        // 各 row の path は col-0 cell (= NewLine) の values[VAR_PATH_SLOT] にある。
        LineInfo* col0 = (extorchVarTable->columns && extorchVarTable->columns->size > 0)
            ? (LineInfo*)*get_list2(extorchVarTable->columns, 0) : NULL;
        RowInfo* targetRow = NULL;
        std::function<void(List*)> walk = [&](List* lines) {
            if (!lines || targetRow || !col0) return;
            for (int i = 0; i < lines->size; i++) {
                LineInfo* l = (LineInfo*)*get_list2(lines, i);
                if (!l) continue;
                NewElement* cellElem = getTableCellById(extorchVarTable, l->id, col0->id);
                if (cellElem && cellElem->values && cellElem->values->n > VAR_PATH_SLOT) {
                    SIVal pv = cellElem->values->values[VAR_PATH_SLOT];
                    if (SIVal_type(pv) == SIType::String) {
                        String* p = (String*)(pv.bits & SIV_VAL_MASK);
                        if (p && p->size == resultPath->size
                            && memcmp(p->data, resultPath->data, p->size) == 0)
                        {
                            targetRow = (RowInfo*)l;
                            return;
                        }
                    }
                }
                if (l->childs) walk(l->childs);
                if (targetRow) return;
            }
        };
        walk(extorchVarTable->rows);
        if (!targetRow) {
            // path がもう存在しない (= スコープ外、別関数等)。捨てる。
            lldb_pumpExpandQueue(cli);
            return;
        }
        // childs を作り直して resultRows を流し込む
        if (!targetRow->childs) {
            targetRow->childs = create_list(thgc, (int)sizeof(char*), CType::_RowInfo);
        }
        targetRow->childs->size = 0;
        int colN = extorchVarTable->columns ? extorchVarTable->columns->size : 0;
        (void)colN;
        for (int i = 0; i < resultRows->size; i++) {
            VarRow* vr = *(VarRow**)get_list(resultRows, i);
            if (!vr) continue;
            int ci = i;
            // child 行作成: putCell 同等の処理を inline で
            NewLine* childCol0Cell = NULL;
            auto putCellAt = [&](List* lst, int idx, int col, String* s, RowInfo*& outRow) {
                NewLine* ln = (NewLine*)GC_alloc(thgc, CType::_LineC);
                initLine(thgc, ln);
                NewDirectAddLast(thgc, local, extorchVarTable, ln);
                setTableCell(thgc, extorchVarTable, lst,
                             extorchVarTable->columns, idx, col, ln);
                NewLetter* lt = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
                initLetter(thgc, lt, getFont("sans", 16), LetterType::_Letter);
                lt->text = s ? s : createString(thgc, (char*)"", 0, 1);
                lt->color = 0x000000FF;
                NewDirectAddLast(thgc, local, ln, lt);
                if (col == 0) {
                    childCol0Cell = ln;
                    if (idx < lst->size) outRow = (RowInfo*)*get_list2(lst, idx);
                }
            };
            RowInfo* childRow = NULL;
            putCellAt(targetRow->childs, ci, 0, vr->name,  childRow);
            putCellAt(targetRow->childs, ci, 1, vr->value, childRow);
            putCellAt(targetRow->childs, ci, 2, vr->type,  childRow);
            if (childRow) {
                childRow->parent   = targetRow;
                childRow->depth    = targetRow->depth + 1;
                // expand=1 → 「展開状態だが子未ロード」(= 再帰展開可)、expand=0 → primitive
                childRow->expanded = (vr->expand != 0);
                // child の path = parent.path + sep + name
                int nlen = vr->name ? vr->name->size : 0;
                int total = resultPath->size + 1 + nlen;
                String* cat = createString(thgc, (char*)"", total, 1);
                memcpy(cat->data, resultPath->data, resultPath->size);
                cat->data[resultPath->size] = '\x1F';
                if (nlen > 0) memcpy(cat->data + resultPath->size + 1, vr->name->data, nlen);
                cat->size = total;
                if (childCol0Cell) {
                    ensureSIValues(thgc, childCol0Cell);
                    SIVal pv{}; pv.str = cat;
                    setSIValue(childCol0Cell->values, VAR_PATH_SLOT, SIType::String, pv);
                    if (vr->nextOffset > 0) {
                        SIVal ov{}; ov.u64 = vr->nextOffset;
                        setSIValue(childCol0Cell->values, MORE_OFFSET_SLOT, SIType::UInt64, ov);
                    }
                }
            }
        }
        targetRow->expanded = true;
        rebuildLineVisibility(extorchVarTable->rows);
        markLayoutOf(table, local);
        // 次の path を pump
        lldb_pumpExpandQueue(cli);
    };

    // 呼出履歴 (backtrace) を table2 に反映するドレイン。drainExtorchVars と同形。
    // 列順は table2 ヘッダ (line 1655-1671) と一致: 0=func, 1=file, 2=line
    auto drainExtorchFrames = [&]() {
        LLDBClient* cli = getLLDBClient(thgc);
        if (atomic_exchange(&cli->framesReady, 0) == 0) return;
        List* rows = NULL;
        {
            MutexGuard g(&cli->framesMutex);
            rows = cli->pendingFrames;
            cli->pendingFrames = create_list(thgc, sizeof(FrameRow*), CType::_FrameRow);
        }

        // 先頭フレーム (= 現在の停止位置) でソースを scroll + 黄色ハイライト
        if (rows && rows->size > 0) {
            FrameRow* topFrame = *(FrameRow**)get_list(rows, 0);
            if (topFrame && topFrame->file && topFrame->line) {
                char lineBuf[32];
                int ll = topFrame->line->size;
                if (ll > (int)sizeof(lineBuf) - 1) ll = (int)sizeof(lineBuf) - 1;
                memcpy(lineBuf, topFrame->line->data, ll);
                lineBuf[ll] = '\0';
                uint32_t lineNo = (uint32_t)atoi(lineBuf);
                char fileBuf[1024];
                int fl = topFrame->file->size;
                if (fl > (int)sizeof(fileBuf) - 1) fl = (int)sizeof(fileBuf) - 1;
                memcpy(fileBuf, topFrame->file->data, fl);
                fileBuf[fl] = '\0';
                fprintf(stderr, "[scroll] target=%s:%u\n", fileBuf, lineNo); fflush(stderr);
                if (lineNo > 0) {
                    scrollSourceToFrame(thgc, local, fileBuf, lineNo);
                    // 黄色ハイライト (= 該当行を可視化)
                    NewElement* viewer = findViewerByPathOrBasename(thgc, fileBuf);
                    if (viewer) paintDebugStopLine(thgc, local, viewer, lineNo);
                }
            }
        }

        if (!extorchFrameTable) { return; }

        // 前回追加した row のセルを cells map から remove
        int colN = extorchFrameTable->columns ? extorchFrameTable->columns->size : 0;
        for (int r = 1; r <= prevFrameRowCount; ++r) {
            if (r >= (int)extorchFrameTable->rows->size) break;
            RowInfo* ri = tableRow(extorchFrameTable, r);
            if (!ri) continue;
            for (int c = 0; c < colN; ++c) {
                ColumnInfo* ci = tableCol(extorchFrameTable, c);
                if (!ci) continue;
                remove_mapyn(thgc, extorchFrameTable->cells,
                    tableKey(ri->id, ci->id));
            }
        }

        // 新しい frame 行を rows[1..N] に書き込む。
        for (int i = 0; i < rows->size; ++i) {
            FrameRow* fr = *(FrameRow**)get_list(rows, i);
            if (!fr) continue;
            int rowIdx = i + 1;
            auto putCell = [&](int col, String* s) {
                NewLine* ln = (NewLine*)GC_alloc(thgc, CType::_LineC);
                initLine(thgc, ln);
                NewDirectAddLast(thgc, local, extorchFrameTable, ln);
                setTableCell(thgc, extorchFrameTable, rowIdx, col, ln);
                NewLetter* lt = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
                initLetter(thgc, lt, getFont("sans", 16), LetterType::_Letter);
                lt->text = s ? s : createString(thgc, (char*)"", 0, 1);
                lt->color = 0x000000FF;
                NewDirectAddLast(thgc, local, ln, lt);
            };
            putCell(0, fr->func);
            putCell(1, fr->file);
            putCell(2, fr->line);
        }
        prevFrameRowCount = rows->size;

        markLayoutOf(table2, local);
    };

    SDL_Log("[GoThread] entering loop tid=%llu thgc=%p thgc->map=%p thgc->local=%p",
            (unsigned long long)std::hash<std::thread::id>{}(std::this_thread::get_id()),
            (void*)thgc, (void*)thgc->map, (void*)thgc->local);
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        /*while (thgc->first != NULL) {
            thgc->first->resume_all();
        }*/
        thgc->queue->resume_all();
        drainExtorchVars();
        drainExtorchExpand();
        drainExtorchFrames();
        if (local->resetid) {
            for (auto nw : thgc->windows) if (nw->reset) AllResetId(local, nw);
            for (auto nw : thgc->linkwins) if (nw->reset) AllResetId(local, nw);
            local->resetid = false;
        }
        thgc->hoppy->buildFrame(thgc, now_us());
        // (2) 統一方式の唯一の GC 安全点。Eden 高使用率 or spill 発生で gc_young 発火。
        GC_check(thgc);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration<double, std::milli>(end - start).count();
        // フレーム時間が 15ms 未満なら差分 sleep。15ms 以上でも最低 1ms は yield して
        // CPU を解放 (= busy-spin 防止 + 省エネ)。
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(15.0 - ms)));
    }
    // 2) �G�N�X�|�[�g�� "add" ���֐��|�C���^�Ƃ��Ď擾
    /*using BaseFunc = CustomModuleImpl * (*)();
    BaseFunc test = reinterpret_cast<BaseFunc>(
        GetProcAddress(h, "test")
        );
    return test();*/
    return NULL;
}
void runGoThreadAsync(ThreadGC* thgc) {
    std::thread([thgc]() {
        initDone.get_future().wait();
#if TARGET_OS_IOS || TARGET_OS_SIMULATOR || defined(__ANDROID__) || defined(__linux__) || TARGET_OS_OSX
        GoThread(thgc);
#else
        CustomModuleImpl* result = GoThread(thgc);
        if (result) {
            std::cout << "GoThread completed successfully" << std::endl;
        }
        else {
            std::cerr << "GoThread failed" << std::endl;
        }
#endif
        }).detach();
}

#if !TARGET_OS_IOS && !TARGET_OS_SIMULATOR && !defined(__ANDROID__) && !defined(__linux__) && !TARGET_OS_OSX
// ============================================================
// DllGoThread: 新規タブ用のイベントループ。
//   - 自タブ専用の ThreadGC (= hoppy->push_tab で登録済み) を使う
//   - 最小限の UI ルート (NewLocal) を初期化して現ウィンドウに紐付ける
//   - clang3/<name>.dll を ReadDll でロード → main() が実行される
//   - 既存 GoThread と同じく drainPendingTasks → queue->resume_all → buildFrame
//     のサイクルを回す
// ============================================================
void DllGoThread(ThreadGC* thgc, std::string dll_name) {
    // クラステーブルは親 (main) と独立なので再登録が必要
    registerGCClasses(thgc);

    // ※ thgc->local / map / hovered* と tabWin の root 登録は
    // 下方の hoppy->push_tab(thgc, local) で行われる。
    // push_tab 前に GC が走らないので、ここでは何もしない。

    // タブ用の UI ルートを用意。windows[0] は runCompiledDllAsync で
    // このタブ専用に new した NativeWindow なので、local を書き込んでも
    // 親タブ側には影響しない。
    NewLocal* local = (NewLocal*)GC_alloc(thgc, CType::_LocalC);
    NativeWindow* tabWin = (!thgc->windows.empty()) ? thgc->windows[0] : nullptr;
    initLocal(thgc, local, tabWin);
    if (tabWin) tabWin->local = (NewElement*)local;
    thgc->local = local;
    local->xtype = SizeType::Range;
    local->ytype = SizeType::Range;
	local->background = (Background*)GC_alloc(thgc, CType::_Background);
	local->background->fillcolor = 0x202020FF;
	local->background->type = DrawCommandType::Fill;
    local->offscreen->markLayout(local, local);
    thgc->hoppy->push_tab(thgc, local);

    // local は while ループで毎回参照される (= local->resetid 等)。
    // GC で move された時に stale 化しないよう root 登録。
    RootNode* funcRn = GC_add_root_node(thgc);
    GC_add_root(funcRn, (char**)&local);

    // DLL ロード + main() 実行 (この thgc 上で走るので競合なし)
    char path[512];
    snprintf(path, sizeof(path), "clang3/%s.dll", dll_name.c_str());
    ReadDll(thgc, path);

    // イベントループ (通常の GoThread と同じ構造)
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        thgc->queue->resume_all();
        if (local->resetid) {
            for (auto nw : thgc->windows) if (nw->reset) AllResetId(local, nw);
            for (auto nw : thgc->linkwins) if (nw->reset) AllResetId(local, nw);
            local->resetid = false;
        }
        thgc->hoppy->buildFrame(thgc, now_us());
        // (2) 統一方式の唯一の GC 安全点 (= DllGoThread 用)。
        GC_check(thgc);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration<double, std::milli>(end - start).count();
        if (15.0 - ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(15.0 - ms)));
    }
}

// ============================================================
// runCompiledDllAsync: main タブ (UIスレッド) から呼び出し、
//   1. 新しい ThreadGC を生成 (= 新タブの GC コンテキスト)
//   2. hoppy->push_tab で hoppy 管理下のタブ一覧に登録
//   3. そのタブ専用の std::thread を起動し DllGoThread を実行
// 呼び出し元 (UI スレッド) は即 return するので UI はブロックされない。
// hoppy が既にマルチスレッドタブ機構 (thgcs / target / push_tab) を
// 持っているので、それを利用する。
// ============================================================
void runCompiledDllAsync(HopStar* hoppy, const char* dll_name) {
    ThreadGC* tab_gc = GC_init(1ULL * 1024 * 1024 * 1024);  // 1GB/tab
    tab_gc->hoppy = hoppy;

    // タブ GC 所有の独立した NativeWindow を「新規作成」する。
    // 親 (mainGC) の NativeWindow ポインタは共有しない (クロス GC 参照回避)。
    // ただし同一の物理 OS ウィンドウに描画する設計なので、
    //   - sdlWindow / nwh (OS ハンドル)
    //   - viewId         (bgfx 的にも「このウィンドウへの描画パス」は同一)
    //   - size / type
    // は親からコピーして共有する。タブ固有の状態 (local / fbo / anchor 系) は
    // 既定値のままにしておき、DllGoThread 側の initLocal 等で自タブ用に設定する。
    

    std::string name_s = dll_name;
    std::thread([tab_gc, name_s]() {
        DllGoThread(tab_gc, name_s);
    }).detach();
}
#endif