using SQLitePCL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography.Xml;
using System.Text;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;

namespace Cyclon
{
    class Component
    {
        public int debugLocationId = -1;
        public List<Component> comps = new List<Component>();
        public virtual String output()
        {
            return "";
        }
        public string OutputDebugLoc()
        {
            if (debugLocationId >= 0)
                return ", !dbg !" + debugLocationId;
            return "";
        }
    }
    class LLVM : Component
    {
        public Local local;
        public List<StrV> strs = new List<StrV>();
        public List<TypeDec> types = new List<TypeDec>();
        public List<Func> funcs = new List<Func>();
        public Func main;
        public static int n = 0;
        public int cn = 0;
        public List<String> tagvars = new List<String>();
        public List<Vari> tagvarvars = new List<Vari>();
        public List<Vari> tagparents = new List<Vari>();
        public List<Vari> tagparenttes = new List<Vari>();
        public DebugInfo debugInfo = new DebugInfo();
        public string sourceFile = "foo.cy"; // ソースファイル名
        public string directory = (Directory.GetCurrentDirectory() + "\\clang2").Replace('\\', '/');
        public Func func
        {
            get { return funcs.Last(); }
        }
        public LLVM(Local local)
        {
            this.local = local;
            comps.Add(new Func(this, new Vari("%torch.nn.ModuleImpl*", "@test")));
            var func = new Func(this, new Vari("i8*", "@forward"), new Vari("i8*", "%obj"), new Vari("i8*", "%input"));
            comps.Add(func);
            var func2 = new Func(this,new Vari("i32", "@test2"));
            comps.Add(func2);
            funcs.Add(func2);
        }
        public override string output()
        {
            ExtractStructDebugInfo();
            String output = "source_filename = \"foo.ll\"\n";
            output += "target datalayout = \"e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n";
            output += "target triple = \"x86_64-pc-windows-msvc19.43.34808\"\n";
            output += "@.strnum = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\", align 1\n";
            output += "@.str.conv1 = private unnamed_addr constant [6 x i8] c\"conv1\\00\", align 1\n";
            output += "@.str.relu1 = private unnamed_addr constant [6 x i8] c\"relu1\\00\", align 1\n";
            output += "@.str.flat1 = private unnamed_addr constant [6 x i8] c\"flat1\\00\", align 1\n";
            output += "@.str.line1 = private unnamed_addr constant [6 x i8] c\"line1\\00\", align 1\n";
            for (var i = 0; i < strs.Count; i++)
            {
                output += strs[i].outputdec();
            }
            output += "declare void @llvm.dbg.declare(metadata, metadata, metadata)\n";
            output += "declare void @llvm.dbg.value(metadata, metadata, metadata)\n";
            output += "declare ptr @calloc(i64, i64)\n";
            output += "declare ptr @malloc(i64)\n";
            output += "declare void @free(ptr)\n";
            output += "declare i32 @printf(i8*, ...)\n";
            output += "declare i32 @putchar(i32)\n";
            output += "declare i32 @fflush(i8 *)\n";
            for (var i = 0; i < types.Count; i++)
            {
                output += types[i].output();
            }
            output += "@cnp = dllexport global i32 0\n";
            output += "@cnl = dllexport global i32 " + cn + "\n";
            output += "%SDL_Color = type {i8, i8, i8, i8}\n";
            output += "%MarginType = type {i32, i32, i32, i32}\n";
            output += "%PointType = type {i32, i32}\n";
            //                              nex0 bef1 par2 chi3 margins4     paddings5   pos,pos26     scroll7 size,size28  xt,yt,pos,lay9 selectable10 font1 signle2 update3 recompile4 color5      background6 type7 align8 index9 groups20 count1 measure2 draw3 mouse4 key5 gomd6 bkmd7 gokd8 bkkd9
            output += "%ElementType = type {ptr, ptr, ptr, ptr, %MarginType, %MarginType, %MarginType, %PointType,    %MarginType, %MarginType,   i1,          ptr,  i1     , i1    , i1,        %SDL_Color, %SDL_Color, i32,  i32,   i32,   ptr,    ptr,   ptr,      ptr,  ptr,   ptr, ptr,  ptr,  ptr,  ptr}\n";
            output += "%CoroFrameType = type {ptr, ptr,  ptr, ptr, ptr, ptr, ptr, i32, i8*, i8*}; resume:0, destroy:1, func:2; RootNode:3; CoroutineQueue:4; blockobj:5; parent-coroutine:6; state:7; current_value:8; return_value:9; statefin:10:\n";
            output += "%ThreadGCType = type {i8*, i8*}\n";
            output += "%torch.nn.Conv2dImpl = type opaque\n";
            output += "%CreateConv2dType = type %torch.nn.Conv2dImpl* (i64, i64, i64)*;\n";
            output += "%GetInChannelsType = type i64 (%torch.nn.Conv2dImpl*)*;\n";
            output += "%GetWeightDataType = type float* (%torch.nn.Conv2dImpl*)*;\n";
            output += "%ForwardConv2dType = type i8* (%torch.nn.Conv2dImpl*, i8*)*;\n";
            output += "@CreateConv2d = dllexport global %CreateConv2dType null\n";
            output += "@GetInChannels = dllexport global %GetInChannelsType null\n";
            output += "@GetOutChannels = dllexport global %GetInChannelsType null\n";
            output += "@GetKernelSize = dllexport global %GetInChannelsType null\n";
            output += "@GetWeightData = dllexport global %GetWeightDataType null\n";
            output += "@GetWeightSize = dllexport global %GetInChannelsType null\n";
            output += "@ForwardConv2d = dllexport global %ForwardConv2dType null\n";
            output += "%torch.nn.MaxPool2dImpl = type opaque\n";
            output += "%CreateMaxPool2dType = type %torch.nn.MaxPool2dImpl* (i64, i64, i64)*\n";
            output += "%GetMaxPoolKernelSizeType = type i64 (%torch.nn.MaxPool2dImpl*)*\n";
            output += "%ForwardMaxPool2dType = type i8* (%torch.nn.MaxPool2dImpl*, i8*)*\n";
            output += "@CreateMaxPool2d = dllexport global %CreateMaxPool2dType null\n";
            output += "@GetMaxPoolKernelSize = dllexport global %GetMaxPoolKernelSizeType null\n";
            output += "@GetMaxPoolStride = dllexport global %GetMaxPoolKernelSizeType null\n";
            output += "@GetMaxPoolPadding = dllexport global %GetMaxPoolKernelSizeType null\n";
            output += "@ForwardMaxPool2d = dllexport global %ForwardMaxPool2dType null\n";
            output += "%torch.nn.ReLUImpl = type opaque\n";
            output += "%CreateReLUType = type %torch.nn.ReLUImpl* (i1)*\n";
            output += "%GetReLUInplaceType = type i1 (%torch.nn.ReLUImpl*)*\n";
            output += "%ForwardReLUType = type i8* (%torch.nn.ReLUImpl*, i8*)*\n";
            output += "@CreateReLU = dllexport global %CreateReLUType null\n";
            output += "@GetReLUInplace = dllexport global %GetReLUInplaceType null\n";
            output += "@ForwardReLU = dllexport global %ForwardReLUType null\n";

            output += "%torch.nn.FlattenImpl = type opaque\n";
            output += "%CreateFlattenType = type %torch.nn.FlattenImpl* (i64, i64)*\n";
            output += "%GetFlattenStartDimType = type i64 (%torch.nn.FlattenImpl*)*\n";
            output += "%ForwardFlattenType = type i8* (%torch.nn.FlattenImpl*, i8*)*\n";
            output += "@CreateFlatten = dllexport global %CreateFlattenType null\n";
            output += "@GetFlattenStartDim = dllexport global %GetFlattenStartDimType null\n";
            output += "@GetFlattenEndDim = dllexport global %GetFlattenStartDimType null\n";
            output += "@ForwardFlatten = dllexport global %ForwardFlattenType null\n";
            output += "%torch.nn.LinearImpl = type opaque\n";
            output += "%CreateLinearType = type %torch.nn.LinearImpl* (i64, i64, i1)*\n";
            output += "%GetLinearInFeaturesType = type i64 (%torch.nn.LinearImpl*)*\n";
            output += "%GetLinearWeightDataType = type float* (%torch.nn.LinearImpl*)*\n";
            output += "%ForwardLinearType = type i8* (%torch.nn.LinearImpl*, i8*)*\n";
            output += "@CreateLinear = dllexport global %CreateLinearType null\n";
            output += "@GetLinearInFeatures = dllexport global %GetLinearInFeaturesType null\n";
            output += "@GetLinearOutFeatures = dllexport global %GetLinearInFeaturesType null\n";
            output += "@GetLinearBias = dllexport global i1 (%torch.nn.LinearImpl*)* null\n";
            output += "@GetLinearWeightData = dllexport global %GetLinearWeightDataType null\n";
            output += "@GetLinearWeightSize = dllexport global %GetLinearInFeaturesType null\n";
            output += "@GetLinearBiasData = dllexport global %GetLinearWeightDataType null\n";
            output += "@GetLinearBiasSize = dllexport global %GetLinearInFeaturesType null\n";
            output += "@ForwardLinear = dllexport global %ForwardLinearType null\n";
            output += "%torch.nn.SequentialImpl = type opaque\n";
            output += "%CreateSequentialType = type %torch.nn.SequentialImpl* ()*\n";
            output += "%AddModuleToSequentialType = type void (%torch.nn.SequentialImpl*, i8*, i8*)*\n";
            output += "%GetSequentialSizeType = type i64 (%torch.nn.SequentialImpl*)*\n";
            output += "%GetSequentialModuleType = type i8* (%torch.nn.SequentialImpl*, i64)*\n";
            output += "%ForwardSequentialType = type i8* (%torch.nn.SequentialImpl*, i8*)*\n";
            output += "@CreateSequential = dllexport global %CreateSequentialType null\n";
            output += "@AddModuleToSequential = dllexport global %AddModuleToSequentialType null\n";
            output += "@GetSequentialSize = dllexport global %GetSequentialSizeType null\n";
            output += "@GetSequentialModule = dllexport global %GetSequentialModuleType null\n";
            output += "@ForwardSequential = dllexport global %ForwardSequentialType null\n";
            output += "%torch.nn.ModuleImpl = type opaque\n";
            output += "%RegisterModuleType = type void (%torch.nn.ModuleImpl*, i8*, %torch.nn.ModuleImpl*)*\n";
            output += "@RegisterModule = dllexport global %RegisterModuleType null\n";
            output += "%ForwardType = type i8*(i8*, i8*)*\n";
            output += "%CreateTorchModuleType = type %torch.nn.ModuleImpl* (i8*, %ForwardType)*\n";
            //"%SetForwardFunctionType = type void (%torch.nn.ModuleImpl*, i8* (i8*, i8*)*)*\r\n";
            output += "@CreateTorchModule = dllexport global %CreateTorchModuleType null\n";
            //"@SetForwardFunction = dllexport global %SetForwardFunctionType null"
            output += "%GCObject = type {ptr}\n";
            output += "%GCModel = type {ptr, i32}\n";
            output += "%GCObjectPtr = type %GCObject*\n";
            output += "%GCCheckFuncType = type void (i8*)*\n";
            output += "%GCFinalizeFuncType = type void (i8*)*\n";
            output += "%GC_AddClassType = type i32 (%ThreadGCType*, i32, i32, i8*, %GCCheckFuncType, %GCFinalizeFuncType)*\n";
            output += "%RootNodeType = type {ptr, i32, ptr, ptr, ptr}\n";
            output += "%GC_AddRootType = type %RootNodeType* (%ThreadGCType*)*\n";
            output += "%GC_ReleaseRootType = type void ( %RootNodeType*)*\n";
            output += "%GC_SetRootType = type void (%RootNodeType*, i8**)*\n";
            output += "%GC_BackRootType = type void (%RootNodeType*, i32)*\n";
            output += "%GC_mallocType = type %GCObjectPtr (%ThreadGCType*, i32)*\n";
            output += "@GC_AddClass = dllexport global %GC_AddClassType null\n";
            output += "@GC_AddRoot = dllexport global %GC_AddRootType null\n";
            output += "@GC_ReleaseRoot = dllexport global %GC_ReleaseRootType null\n";
            output += "@GC_SetRoot = dllexport global %GC_SetRootType null\n";
            output += "@GC_BackRoot = dllexport global %GC_BackRootType null\n";
            output += "@GC_malloc = dllexport global %GC_mallocType null\n";
            output += "%TestType = type { %torch.nn.Conv2dImpl*, %torch.nn.ReLUImpl*, %torch.nn.FlattenImpl*, %torch.nn.LinearImpl*}\n";
            output += "%torch.optim.Adam = type opaque\n";
            output += "%AdamOptions = type opaque\n";
            output += "%CreateAdamType = type %torch.optim.Adam* (%torch.nn.ModuleImpl**, i64, %AdamOptions*)*\n";
            output += "@CreateAdam = dllexport global %CreateAdamType null\n";
            output += "%CreateAdamOptionsType = type %AdamOptions* (float)*\n";
            output += "@CreateAdamOptions = dllexport global %CreateAdamOptionsType null\n";
            output += "%OptimizerZeroGradType = type void (%torch.optim.Adam*)*\n";
            output += "@OptimizerZeroGrad = dllexport global %OptimizerZeroGradType null\n";
            output += "%TensorBackwardType = type void (i8*)*\n";
            output += "@TensorBackward = dllexport global %TensorBackwardType null\n";
            output += "%OptimizerStepType = type void (%torch.optim.Adam*)*\n";
            output += "@OptimizerStep = dllexport global %OptimizerStepType null\n";
            output += "%CrossEntropyType = type i8* (i8*, i8*)*\n";
            output += "@CrossEntropy = dllexport global %CrossEntropyType null\n";
            output += "%TensorToCUDAType = type i8* (i8*)*\n";
            output += "@TensorToCUDA = dllexport global %TensorToCUDAType null\n";
            output += "%TensorRequiresGradType = type i8* (i8*, i1)*\n";
            output += "@TensorRequiresGrad = dllexport global %TensorRequiresGradType null\n";
            output += "%StringType = type {i8**, i32, i32}\n";
            output += "%NumberStringType = type %StringType* (%ThreadGCType*, i32)*\n";
            output += "@NumberString = dllexport global %NumberStringType null\n";
            output += "%CreateStringType = type %StringType* (%ThreadGCType*, i8*, i32, i32)*\n";
            output += "@CreateString = dllexport global %CreateStringType null\n";
            output += "%AddStringType = type %StringType* (%ThreadGCType*, %StringType*, i8*, i32, i32)*\n";
            output += "@AddString = dllexport global %AddStringType null\n";
            output += "%AddStringType2 = type %StringType* (%ThreadGCType*, %StringType*, %StringType*)*\n";
            output += "@AddString2 = dllexport global %AddStringType2 null\n";
            output += "%RemoveStringType = type %StringType* (%ThreadGCType*, %StringType*, i32)*\n";
            output += "@RemoveString = dllexport global %RemoveStringType null\n";
            output += "%SubStringType = type %StringType* (%ThreadGCType*, %StringType*, i32, i32)*\n";
            output += "@SubString = dllexport global %SubStringType null\n";
            output += "%GetCharType = type i16 (%StringType*, i32)*\n";
            output += "@GetChar = dllexport global %GetCharType null\n";
            output += "%PrintStringType = type void (%StringType*)*\n";
            output += "@PrintString = dllexport global %PrintStringType null\n";
            output += "%CopyObjectType = type i8* (%ThreadGCType*, i8*)*\n";
            output += "@CopyObject = dllexport global %CopyObjectType null\n";
            output += "@CloneObject = dllexport global %CopyObjectType null\n";
            output += "@thgcp = dllexport global %ThreadGCType * null\n";
            output += "%GetHashValueType = type %StringType* (%StringType*, i32)*\n";
            output += "@GetHashValue = dllexport global %GetHashValueType null\n";
            output += "%FuncType = type {i8*, i8*, i8*, i8*}\n";//blk, drawcall(rn, blk, ..draws), call(rn, obj), obj
            output += "%MemFuncType = type {i8*, i8*, i8*}\n";//blk, call, obj;
            output += "%ListType = type {i8**, i32, i32, i32, i1}\n";
            output += "%CreateListType = type %ListType* (%ThreadGCType*, i32, i1)*\n";
            output += "@CreateList = dllexport global %CreateListType null\n";
            output += "%AddListType = type void (%ThreadGCType*, %ListType*, i8*)*\n";
            output += "@AddList = dllexport global %AddListType null\n";
            output += "%GetListType = type i8** (%ListType*, i32)*\n";
            output += "@GetList = dllexport global %GetListType null\n";
            output += "%GetLastType = type i8** (%ListType*)*\n";
            output += "@GetLast = dllexport global %GetLastType null\n";
            output += "%SetListType = type void (%ListType*, i32, i8*)*\n";
            output += "@SetList = dllexport global %SetListType null\n";
            output += "%SetLastType = type void (%ListType*, i8*)*\n";
            output += "@SetLast = dllexport global %SetLastType null\n";
            output += "%KeyValueType = type {i32, %StringType*, i8*, i1}\n";
            output += "%HashType = type {%ListType*, i8*, i1}\n";
            output += "%CreateHashType = type %HashType* (%ThreadGCType*, i1)*\n";
            output += "@CreateHash = dllexport global %CreateHashType null\n";
            output += "%AddHashType = type void (%ThreadGCType*, %HashType*, %StringType*, i8*)*\n";
            output += "@AddHash = dllexport global %AddHashType null\n";
            output += "%GetHashType = type i8* (%HashType*, %StringType*)*\n";
            output += "@GetHash = dllexport global %GetHashType null\n";
            output += "%PrintArrayType = type void (%HashType*, i32, i32)*\n";
            output += "@PrintArray = dllexport global %PrintArrayType null\n";
            output += "%DbInitType = type ptr (%ThreadGCType*, %StringType*, i32, i32, i32, %StringType)*\n";
            output += "@DbInit = dllexport global %DbInitType null\n";
            output += "%CoroutineQueueType = type {i8*}\n";
            output += "%PushQueueType = type void (%CoroutineQueueType*, i8*)*\n";
            output += "@PushQueue = dllexport global %PushQueueType null\n";
            output += "%WaitHandleType = type i8* (i8*)*\n";
            output += "@WaitHandle = dllexport global %WaitHandleType null\n";
            output += "@sqlp = dllexport global ptr null\n";
            output += "%BeginTransactionType = type void (ptr, %ThreadGCType*, ptr, %CoroutineQueueType*)*\n";
            output += "@BeginTransaction = dllexport global %BeginTransactionType null\n";
            output += "%StringUTF8Type = type i8* (%StringType*, i32*)*\n";
            output += "@StringUTF8 = dllexport global %StringUTF8Type null\n";
            output += "%TextType = type { i8*, i32 }\n";
            output += "%SqlParamType = type {i32, i32, [16 x i8]}\n";
            output += "%ExecSqlType = type void (ptr, i8*, %StringType, %SqlParamType*, i32, ptr, ptr)*\n";
            output += "@ExecSql = dllexport global %ExecSqlType null\n";
            output += "%TxFinishType = type void (ptr, i8*)*\n";
            output += "@TxCommit = dllexport global %TxFinishType null\n";
            output += "@TxRollback = dllexport global %TxFinishType null\n";
            output += "%SQP = type {%StringType*, %SqlParamType*}\n";
            output += "%Triptr = type {%StringType*, %SqlParamType*, ptr}\n";
            output += "%Vector = type {ptr, ptr, ptr}\n";
            output += "%RowSet = type {%ListType*, %ListType*, i64, i32, i8}\n";
            output += "%SqlIntType = type i32 (i8*, i32)*\n";
            output += "%SqlStringType = type i8* (i8*, i32)*\n";
            output += "@SqlInt = dllexport global %SqlIntType null\n";
            output += "@SqlString = dllexport global %SqlStringType null\n";
            output += "@SqlLength = dllexport global %SqlIntType null\n";
            output += "@CreateStringANT = dllexport global %CreateStringType null\n";
            output += "@CloneObjectANT = dllexport global %CopyObjectType null\n";
            output += "%GCANTType = type void(%ThreadGCType*, %CoroutineQueueType*, i8*)*\n";
            output += "@GCANT = dllexport global %GCANTType null\n";
            output += "%CreateDivType = type %ElementType* (%ThreadGCType*)*\n";
            output += "@CreateDiv = dllexport global %CreateDivType null\n";
            output += "@CreateSheet = dllexport global %CreateDivType null\n";
            // 旧 %TreeElementType は廃止。ElementType に tag* フィールドを統合し、
            // checkTreeElement は ElementType* を直接返すよう ABI 変更。
            output += "%CheckTreeElementType = type %ElementType* (%ThreadGCType*, %ElementType*, %ElementType*, %ListType*, %StringType*, i32, i32, ptr)*\n";
            output += "@CheckTreeElement = dllexport global %CheckTreeElementType null\n";
            output += "%ElementAddType = type %ElementType* (%ThreadGCType*, %ElementType*, %ElementType*)*\n";
            output += "@ElementAdd = dllexport global %ElementAddType null\n";
            output += "%CreateLetterType = type void (%ThreadGCType*, %ElementType*, %StringType*)*\n";
            output += "@CreateLetter = dllexport global %CreateLetterType null\n";
            output += "%CreateKaigyouType = type void (%ThreadGCType*, %ElementType*)*\n";
            output += "@CreateKaigyou = dllexport global %CreateKaigyouType null\n";
            output += "%CreateEndType = type void (%ThreadGCType*, %ElementType*)*\n";
            output += "@CreateEnd = dllexport global %CreateEndType null\n";
            output += "%MakeFrameType = type ptr (%RootNodeType*, i8*, i8*)*\n";
            output += "@MakeFrame = dllexport global %MakeFrameType null\n";
            output += "@client = dllexport global ptr null\n";
            output += "%CheckType = type void(ptr)*\n";
            output += "@Check = dllexport global %CheckType null\n";
            output += "define void @print_int(i32 %n) {\n";
            output += "entry:\n";
            output += "  %fmt_ptr = getelementptr [4 x i8], [4 x i8]* @.strnum, i32 0, i32 0\n";
            output += "  call i32 (i8*, ...) @printf(i8* %fmt_ptr, i32 %n)\n";
            output += "  call i32 @fflush(i8 * null)\n";
            output += "  ret void\n}\n";
            for (int i = 0; i < comps.Count; i++)
            {
                output += comps[i].output();
            }
            output += OutputDebugMetadata();
            output += OutputDebugMetadata2();
            return output;
        }
        public void ExtractStructDebugInfo()
        {
            int fileId = debugInfo.CreateFile(sourceFile, directory);

            // 1. まず全ての構造体IDを予約（前方宣言）
            foreach (var typeDec in types)
            {
                if (typeDec.func) continue;
                string structName = typeDec.name.TrimStart('%');
                debugInfo.GetOrCreateType(typeDec.name);
            }

            // 2. 依存関係を解決して定義
            var processedTypes = new HashSet<string>();
            foreach (var typeDec in types)
            {
                if (typeDec.func) continue;
                DefineStructTypeRecursive(typeDec, fileId, processedTypes);
            }
        }

        public void DefineStructTypeRecursive(TypeDec typeDec, int fileId,
                                              HashSet<string> processed)
        {
            string structName = typeDec.name.TrimStart('%');

            if (processed.Contains(structName))
                return;

            // 依存する型を先に定義
            foreach (var comp in typeDec.comps)
            {
                var typeVal = comp as TypeVal;
                string fieldType = typeVal.type;

                // フィールドが構造体型の場合、先に定義
                if (fieldType.StartsWith("%"))
                {
                    string depStructName = fieldType.TrimStart('%').TrimEnd('*');
                    var depTypeDec = types.FirstOrDefault(t =>
                        t.name.TrimStart('%') == depStructName && !t.func);

                    if (depTypeDec != null && !processed.Contains(depStructName))
                    {
                        DefineStructTypeRecursive(depTypeDec, fileId, processed);
                    }
                }
            }

            // 自分自身を定義
            var fields = new List<StructField>();
            int offsetBits = 0;

            for (int i = 0; i < typeDec.comps.Count; i++)
            {
                var typeVal = typeDec.comps[i] as TypeVal;
                string llvmType = typeVal.type;

                int sizeBits = GetTypeSize(llvmType);

                fields.Add(new StructField(
                    typeVal.name,
                    llvmType,
                    offsetBits,
                    sizeBits, 
                    debugInfo.metaId++
                ));

                offsetBits += sizeBits;
            }

            // 構造体情報を保存
            int structId = debugInfo.structTypeIds[structName];
            debugInfo.structTypes[structName] = new StructTypeInfo
            {
                structId = structId,
                fileId = fileId,
                fields = fields,
                name = structName
            };

            processed.Add(structName);
        }

        private int GetTypeSize(string llvmType)
        {
            // ポインタ型
            if (llvmType.Contains("*") || llvmType == "ptr")
                return 64;

            // 基本型
            if (llvmType == "i1") return 1;
            if (llvmType == "i8") return 8;
            if (llvmType == "i16") return 16;
            if (llvmType == "i32") return 32;
            if (llvmType == "i64") return 64;
            if (llvmType == "float") return 32;
            if (llvmType == "double") return 64;

            // 配列型 [N x type]
            if (llvmType.StartsWith("["))
            {
                var match = System.Text.RegularExpressions.Regex.Match(
                    llvmType, @"\[(\d+) x (.+)\]");
                if (match.Success)
                {
                    int count = int.Parse(match.Groups[1].Value);
                    string elementType = match.Groups[2].Value;
                    return count * GetTypeSize(elementType);
                }
            }

            // 構造体型 %TypeName
            if (llvmType.StartsWith("%"))
            {
                string typeName = llvmType.TrimStart('%');
                if (debugInfo.structTypes.ContainsKey(typeName))
                {
                    var structInfo = debugInfo.structTypes[typeName];
                    return structInfo.fields.Sum(f => f.sizeInBits);
                }
            }

            return 64; // デフォルト
        }
        private string OutputDebugMetadata()
        {
            var sb = new StringBuilder();

            // モジュールフラグ
            sb.AppendLine("!llvm.dbg.cu = !{!" + debugInfo.compileUnitId + "}");
            sb.AppendLine("!llvm.module.flags = !{!" + debugInfo.dwarfVersionId +
                          ", !" + debugInfo.debugInfoVersionId + "}");
            sb.AppendLine();

            // DWARF Version
            sb.AppendLine("!" + debugInfo.dwarfVersionId +
                          " = !{i32 2, !\"Dwarf Version\", i32 4}");

            // Debug Info Version
            sb.AppendLine("!" + debugInfo.debugInfoVersionId +
                          " = !{i32 2, !\"Debug Info Version\", i32 3}");
            sb.AppendLine();

            // DICompileUnit
            int mainFileId = debugInfo.CreateFile(sourceFile, directory);
            sb.AppendLine("!" + debugInfo.compileUnitId +
                          " = distinct !DICompileUnit(");
            sb.AppendLine("  language: DW_LANG_C99,");
            sb.AppendLine("  file: !" + mainFileId + ",");
            sb.AppendLine("  producer: \"Cyclon Compiler\",");
            sb.AppendLine("  isOptimized: false,");
            sb.AppendLine("  runtimeVersion: 0,");
            sb.AppendLine("  emissionKind: FullDebug)");
            sb.AppendLine();

            // DIFile
            foreach (var file in debugInfo.fileIds)
            {
                var parts = file.Key.Split('#');
                sb.AppendLine("!" + file.Value +
                              " = !DIFile(filename: \"" + parts[0] +
                              "\", directory: \"" + parts[1] + "\")");
            }
            sb.AppendLine();

            // DISubprogram (関数情報)
            foreach (var func in debugInfo.functionIds)
            {
                sb.AppendLine("!" + func.Value +
                              " = distinct !DISubprogram(");
                sb.AppendLine("  name: \"" + func.Key + "\",");
                sb.AppendLine("  scope: !" + mainFileId + ",");
                sb.AppendLine("  file: !" + mainFileId + ",");
                sb.AppendLine("  line: 1,");
                sb.AppendLine("  type: !99,"); // 簡略化のため固定型
                sb.AppendLine("  scopeLine: 1,");
                sb.AppendLine("  spFlags: DISPFlagDefinition,");
                sb.AppendLine("  unit: !" + debugInfo.compileUnitId + ")");
            }

            // 基本的な関数型（簡略化）
            sb.AppendLine("!99 = !DISubroutineType(types: !100)");
            sb.AppendLine("!100 = !{}");

            return sb.ToString();
        }
        private string OutputDebugMetadata2()
        {
            var sb = new StringBuilder();

            // ... 既存のメタデータ出力 ...
            foreach (var structType in debugInfo.structTypeIds)
            {
                sb.AppendLine(OutputStructType(structType.Key, structType.Value));
            }
            foreach (var ptr in debugInfo.pointerTypes)
            {
                sb.AppendLine("!" + ptr.Value +
                              " = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !" +
                              ptr.Key + ", size: 64)");
            }
            // 型情報の出力
            foreach (var type in debugInfo.typeIds)
            {
                sb.AppendLine(OutputBasicType(type.Key, type.Value));
            }
            sb.AppendLine();

            // 各関数の変数情報を出力
            foreach (var func in funcs)
            {
                foreach (var debugVar in func.debugVariables)
                {
                    sb.AppendLine(OutputLocalVariable(debugVar));
                }
            }


            // ... 既存のメタデータ ...

            // 構造体型の出力
            

            foreach (var loc in debugInfo.locations)
            {
                sb.AppendLine("!" + loc.Key + " = !DILocation(line: " +
                              loc.Value.line + ", column: " + loc.Value.column +
                              ", scope: !" + loc.Value.scopeId + ")");
            }


            return sb.ToString();
        }
        private string OutputBasicType(string typeKey, int typeId)
        {
            // ポインタ型
            if (typeKey.Contains("*"))
            {
                string baseTypeName = typeKey.TrimEnd('*').Trim();
                int baseTypeId = 0;
                if (debugInfo.typeIds.ContainsKey(baseTypeName))
                    baseTypeId = debugInfo.typeIds[baseTypeName];
                else
                    baseTypeId = debugInfo.GetOrCreateType(baseTypeName);

                return "!" + typeId +
                       " = !DIDerivedType(tag: DW_TAG_pointer_type, " +
                       "baseType: !" + baseTypeId + ", size: 64)";
            }

            // 配列型
            if (typeKey.StartsWith("["))
            {
                var match = System.Text.RegularExpressions.Regex.Match(
                    typeKey, @"\[(\d+) x (.+)\]");
                if (match.Success)
                {
                    int count = int.Parse(match.Groups[1].Value);
                    string elementType = match.Groups[2].Value;
                    int elementTypeId = debugInfo.GetOrCreateType(elementType);
                    int totalSize = count * GetTypeSize(elementType);

                    return "!" + typeId +
                           " = !DICompositeType(tag: DW_TAG_array_type, " +
                           "baseType: !" + elementTypeId + ", " +
                           "size: " + totalSize + ", elements: !{})";
                }
            }

            // 構造体型はスキップ（別途出力）
            if (typeKey.StartsWith("%"))
                return "";

            // 基本型
            int size = 0;
            int encoding = 0;
            string name = typeKey;

            switch (typeKey)
            {
                case "i1": size = 1; encoding = 2; name = "bool"; break;
                case "i8": size = 8; encoding = 6; name = "char"; break;
                case "i16": size = 16; encoding = 5; name = "short"; break;
                case "i32": size = 32; encoding = 5; name = "int"; break;
                case "i64": size = 64; encoding = 5; name = "long"; break;
                case "float": size = 32; encoding = 4; break;
                case "double": size = 64; encoding = 4; break;
                case "ptr": size = 64; encoding = 7; break;
                case "void": size = 0; encoding = 0; break;
            }

            return "!" + typeId +
                   " = !DIBasicType(name: \"" + name +
                   "\", size: " + size + ", encoding: " + encoding + ")";
        }
        private string OutputStructType(string structName, int structId)
        {
            if (!debugInfo.structTypes.ContainsKey(structName))
                return "";

            var structInfo = debugInfo.structTypes[structName];
            var sb = new StringBuilder();

            // DICompositeType
            int totalSize = structInfo.fields.Sum(f => f.sizeInBits);

            sb.AppendLine("!" + structId + " = distinct !DICompositeType(");
            sb.AppendLine("  tag: DW_TAG_structure_type,");
            sb.AppendLine("  name: \"" + structName + "\",");
            sb.AppendLine("  file: !" + structInfo.fileId + ",");
            sb.AppendLine("  line: 1,");
            sb.AppendLine("  size: " + totalSize + ",");
            sb.Append("  elements: !{");

            for (int i = 0; i < structInfo.fields.Count; i++)
            {
                if (i > 0) sb.Append(", ");
                sb.Append("!" + structInfo.fields[i].metaId);
            }
            sb.AppendLine("})");

            // 各フィールド
            foreach (var field in structInfo.fields)
            {
                int fieldTypeId = debugInfo.GetOrCreateType(field.llvmType);

                sb.AppendLine("!" + field.metaId + " = !DIDerivedType(");
                sb.AppendLine("  tag: DW_TAG_member,");
                sb.AppendLine("  name: \"" + field.name + "\",");
                sb.AppendLine("  scope: !" + structId + ",");
                sb.AppendLine("  file: !" + structInfo.fileId + ",");
                sb.AppendLine("  line: 1,");
                sb.AppendLine("  baseType: !" + fieldTypeId + ",");
                sb.AppendLine("  size: " + field.sizeInBits + ",");
                sb.AppendLine("  offset: " + field.offsetInBits + ")");
            }

            return sb.ToString();
        }

        private string OutputLocalVariable(DebugVariable debugVar)
        {
            var sb = new StringBuilder();
            sb.Append("!" + debugVar.metaId + " = !DILocalVariable(");
            sb.Append("name: \"" + debugVar.name + "\", ");

            if (debugVar.argNo > 0)
                sb.Append("arg: " + debugVar.argNo + ", ");

            sb.Append("scope: !" + debugVar.scopeId + ", ");
            sb.Append("file: !" + debugInfo.fileIds.First().Value + ", ");
            sb.Append("line: " + debugVar.line + ", ");
            sb.Append("type: !" + debugVar.typeId + ")");

            return sb.ToString();
        }
        public void Compile(String name)
        {
            StreamWriter sw = new StreamWriter("clang3/" + name + ".ll");
            sw.Write(output());
            sw.Close();
            var psi = new ProcessStartInfo
            {
                FileName = "clang3/clang++",
                Arguments = "-std=c++20 -g -gdwarf-4 -O0 -fuse-ld=lld -shared " +
            name + ".ll -o " + name + ".dll -Wl,/IMPLIB:" + name,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true,
                WorkingDirectory = @"clang3"
            };
            var p = new Process { StartInfo = psi };
            p.Start();
            var str = p.StandardOutput.ReadToEnd();
            var err = p.StandardError.ReadToEnd();
            p.WaitForExit();
            new FileInfo("clang3/" + name + ".dll").CopyTo(@"C:\Users\hayah\source\repos\Project2\Project2\" + name + ".dll", true);
            var psi2 = new ProcessStartInfo
            {
                FileName = "clang3/clang++",
                Arguments = "main.cpp sqlite3.o -g -gdwarf-4 -std=c++20 -O0  -fuse-ld=lld  -D_DEBUG    -D_ITERATOR_DEBUG_LEVEL=2    -D_DLL   -D_MT    -Xclang --dependent-lib=msvcrtd   -DBX_CONFIG_DEBUG=1   -Wno-ignored-attributes     -Wno-tautological-compare     -Wno-format    -Wno-invalid-source-encoding     -Wno-parentheses -D__STDC_LIMIT_MACROS   -D__STDC_FORMAT_MACROS   -D__STDC_CONSTANT_MACROS    -I C:\\C2\\include    -I C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\include    -I C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\include\\torch\\csrc\\api\\include     C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\torch.lib     C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\torch_cpu.lib     C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\torch_cuda.lib   C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\c10.lib    C:\\Users\\hayah\\Downloads\\libtorch-win-shared-with-deps-debug-2.6.0+cu126\\libtorch\\lib\\c10_cuda.lib    C:\\C2\\onnxruntime.lib C:\\C2\\onnxruntime_providers_cuda.lib C:\\C2\\onnxruntime_providers_shared.lib C:\\C2\\onnxruntime_providers_tensorrt.lib C:\\C2\\avcodec.lib C:\\C2\\avdevice.lib C:\\C2\\avfilter.lib  C:\\C2\\avformat.lib     C:\\C2\\avutil.lib C:\\C2\\postproc.lib C:\\C2\\swresample.lib C:\\C2\\swscale.lib  C:\\C2\\SDL3.lib C:\\C2\\SDL3_ttf.lib C:\\C2\\utf8proc_static.lib C:\\C2\\libcoro.lib  C:\\C2\\bgfxDebug.lib C:\\C2\\bimg_decodeDebug.lib    C:\\C2\\bxDebug.lib     C:\\C2\\bimgDebug.lib C:\\C2\\portaudio_x64.lib -lmsvcrtd.lib  -lucrtd.lib -lvcruntimed.lib -lgdi32.lib -luser32.lib  -lkernel32.lib  -lopengl32.lib -lShell32 -lShlwapi   -march=native   -o main.exe",
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true,
                WorkingDirectory = "C:\\Users\\hayah\\source\\repos\\Project2\\Project2"
            };
            var p2 = new Process { StartInfo = psi2 };
            p2.Start();
            var stdout = new StringBuilder();
            var stderr = new StringBuilder();

            p2.OutputDataReceived += (s, e) => { if (e.Data != null) stdout.AppendLine(e.Data); };
            p2.ErrorDataReceived += (s, e) => { if (e.Data != null) stderr.AppendLine(e.Data); };

            p2.Start();
            p2.BeginOutputReadLine();
            p2.BeginErrorReadLine();
            p2.WaitForExit();

            var str2 = stdout.ToString();
            var err2 = stderr.ToString();
            local.panel.form.A();
            /*

                var chkp = new Vari("%CheckType", "%chkp" + LLVM.n++);
                local.llvm.func.comps.Add(new Load(chkp, new Vari("%CheckType*", "@Check")));
                local.llvm.func.comps.Add(new Call(null, chkp, tx));

             */
        }
    }
    class StrV : Vari
    {
        public String value;
        public StrV(String name, String value, int length) : base(null, name)
        {

            this.type = "[" + (length + 1) + " x i8]*";
            this.value = value;
        }
        public string outputdec()
        {
            return name + " = private unnamed_addr constant " + type.Substring(0, type.Length - 1) + " c\"" + value + "\\00\", align 1\n";
        }
    }
    class Vari : Component
    {
        public String type;
        public String name;
        public Vari(String type, String name)
        {
            this.type = type;
            this.name = name;
        }
        public override string output()
        {
            return this.type + " " + this.name;
        }
    }
    class IfValue : Component
    {
        public Lab lab;
        public Vari v;
        public bool check = false;
        public IfValue(Lab lab, Vari v)
        {
            this.lab = lab; this.v = v;
        }
    }
    class Phi : Component
    {
        Vari y;
        Dictionary<String, IfValue> vmap;
        public Phi(Vari y, Dictionary<String, IfValue> vmap)
        {
            this.y = y;
            this.vmap = vmap;
        }
        public override string output()
        {
            String output = y.name + " = phi " + y.type;
            var values = vmap.Values.ToList();
            for (var i = 0; i < values.Count; i++)
            {
                var value = values[i];
                if (value.v == null) continue;
                if (i != 0) output += ",";
                output += " [" + value.v.name + ", %" + value.lab.name + "]";
            }
            output += OutputDebugLoc();
            return output + "\n";
        }
    }
    class TypeDec : Component
    {
        public String option;
        public bool func = false;
        public String name;
        public TypeDec(String name, String option = "", bool func = false, params TypeVal[] val)
        {
            this.name = name;
            this.option = "";
            this.func = func;
            for (var i = 0; i < val.Length; i++) comps.Add(val[i]);
        }
        public override string output()
        {
            String output = name + " = type ";
            if (!func)
            {
                output += "{";
                for (var i = 0; i < comps.Count; i++)
                {
                    if (i != 0) output += ",";
                    output += comps[i].output();
                }
                return output + "}" + option + "\n";
            }
            else
            {
                output += comps[0].output();
                output += "(";
                for (var i = 1; i < comps.Count; i++)
                {
                    if (i != 1) output += ",";
                    output += comps[i].output();
                }
                return output + ")" + option + "\n";
            }
        }
    }
    class TypeVal : Component
    {
        public String type;
        public String name;
        public TypeVal(String type, string name)
        {
            this.type = type;
            this.name = name;
        }
        public override string output()
        {
            return type;
        }
    }
    class Func : Component
    {
        public static int fn = 0;
        public Vari y;
        public List<Component> draws = new List<Component>();
        public List<Component> dbinits = new List<Component>();
        public Vari vari;
        public bool async = false;
        public bool optnone;
        public SwitchComp sc = null;
        public int debugId = -1;
        public int currentLine = 1;
        public LLVM llvm;
        public List<DebugVariable> debugVariables = new List<DebugVariable>();
        public Func(LLVM llvm, Vari y, params Vari[] xs)
        {
            this.y = y;
            this.llvm = llvm;
            int fileId = llvm.debugInfo.CreateFile(llvm.sourceFile, llvm.directory);
            debugId = llvm.debugInfo.CreateFunction(y.name, fileId, 1);
            for (var i = 0; i < xs.Length; i++)
            {
                draws.Add(xs[i]);
            }
            if (y.name == "@test2")
            {

            }
            else if (y.name == "@test")
            {
                var gfclass = new Vari("%GC_AddClassType*", "@GC_AddClass");
                var gfmalloc = new Vari("%GC_mallocType*", "@GC_malloc");
                var gfmodule = new Vari("%CreateTorchModuleType*", "@CreateTorchModule");
                var gfconv = new Vari("%CreateConv2dType*", "@CreateConv2d");
                var gfrel = new Vari("%CreateReLUType*", "@CreateReLU");
                var gffla = new Vari("%CreateFlattenType*", "@CreateFlatten");
                var gflin = new Vari("%CreateLinearType*", "@CreateLinear");
                var gfreg = new Vari("%RegisterModuleType*", "@RegisterModule");
                var lfclass = new Vari("%GC_AddClassType", "%lfclass");
                var lfmalloc = new Vari("%GC_mallocType", "%lfmalloc");
                var lfmodule = new Vari("%CreateTorchModuleType", "%lfmodule");
                var lfconv = new Vari("%CreateConv2dType", "%lfconv");
                var lfrel = new Vari("%CreateReLUType", "%lfrel");
                var lffla = new Vari("%CreateFlattenType", "%lffla");
                var lflin = new Vari("%CreateLinearType", "%lflin");
                var lfreg = new Vari("%RegisterModuleType", "%lfreg");
                var loadclass = new Load(lfclass, gfclass);
                comps.Add(loadclass);
                var cls = new Vari("i32", "%cls");
                var lfcl = new Call(cls, lfclass, new Vari("i32", "32"), new Vari("%GCCheckFuncType", "null"), new Vari("%GCFinalizeFuncType", "null"));
                comps.Add(lfcl);
                var loadmalloc = new Load(lfmalloc, gfmalloc);
                comps.Add(loadmalloc);
                var obj = new Vari("%GCObjectPtr", "%obj");
                var lfma = new Call(obj, lfmalloc, cls);
                comps.Add(lfma);
                var loadmodule = new Load(lfmodule, gfmodule);
                comps.Add(loadmodule);
                var val = new Vari("%torch.nn.ModuleImpl*", "%val");
                var lfm = new Call(val, lfmodule, obj, new Vari("%ForwardType", "@forward"));
                comps.Add(lfm);
                var loadconv = new Load(lfconv, gfconv);
                comps.Add(loadconv);
                var val2 = new Vari("%torch.nn.Conv2dImpl*", "%val2");
                var lfc = new Call(val2, lfconv, new Vari("i64", "3"), new Vari("i64", "16"), new Vari("i64", "3"));
                comps.Add(lfc);
                var valt = new Vari("%TestType*", "%tobj");
                var bitcast = new Bitcast(valt, obj);
                comps.Add(bitcast);
                var convptr = new Vari("%torch.nn.Conv2dImpl*", "%convptr");
                var geteconv = new Gete("%TestType", convptr, valt, new Vari("i32", "0"), new Vari("i32", "0"));
                comps.Add(geteconv);
                var storeconv = new Store(convptr, val2);
                comps.Add(storeconv);
                var loadreg = new Load(lfreg, gfreg);
                comps.Add(loadreg);
                var str1 = new Vari("i8*", "%str1");
                var gete = new Gete("[6 x i8]", str1, new Vari("[6 x i8]*", "@.str.conv1"), new Vari("i64", "0"), new Vari("i64", "0"));
                comps.Add(gete);
                var bit1 = new Vari("%torch.nn.ModuleImpl*", "%bit1");
                comps.Add(new Bitcast(bit1, val2));
                var lfrg1 = new Call(null, lfreg, val, str1, bit1);
                comps.Add(lfrg1);
                var loadrel = new Load(lfrel, gfrel);
                comps.Add(loadrel);
                var val3 = new Vari("%torch.nn.ReLUImpl*", "%val3");
                var lfr = new Call(val3, lfrel, new Vari("i1", "false"));
                comps.Add(lfr);
                var reluptr = new Vari("%torch.nn.ReLUImpl*", "%reluptr");
                var geterelu = new Gete("%TestType", reluptr, valt, new Vari("i32", "0"), new Vari("i32", "1"));
                comps.Add(geterelu);
                var storerelu = new Store(reluptr, val3);
                comps.Add(storerelu);
                var str2 = new Vari("i8*", "%str2");
                var bit2 = new Vari("%torch.nn.ModuleImpl*", "%bit2");
                comps.Add(new Bitcast(bit2, val3));
                var gete2 = new Gete("[6 x i8]", str2, new Vari("[6 x i8]*", "@.str.relu1"), new Vari("i64", "0"), new Vari("i64", "0"));
                comps.Add(gete2);
                var lfrg2 = new Call(null, lfreg, val, str2, bit2);
                comps.Add(lfrg2);
                var loadfla = new Load(lffla, gffla);
                comps.Add(loadfla);
                var val5 = new Vari("%torch.nn.FlattenImpl*", "%val5");
                var lff = new Call(val5, lffla, new Vari("i64", "1"), new Vari("i64", "-1"));
                comps.Add(lff);
                var flatptr = new Vari("%torch.nn.FlattenImpl*", "%flatptr");
                var geteflat = new Gete("%TestType", flatptr, valt, new Vari("i32", "0"), new Vari("i32", "2"));
                comps.Add(geteflat);
                var storeflat = new Store(flatptr, val5);
                comps.Add(storeflat);
                var str4 = new Vari("i8*", "%str4");
                var bit4 = new Vari("%torch.nn.ModuleImpl*", "%bit4");
                comps.Add(new Bitcast(bit4, val5));
                var gete4 = new Gete("[6 x i8]", str4, new Vari("[6 x i8]*", "@.str.flat1"), new Vari("i64", "0"), new Vari("i64", "0"));
                comps.Add(gete4);
                var lfrg4 = new Call(null, lfreg, val, str4, bit4);
                comps.Add(lfrg4);
                var loadlin = new Load(lflin, gflin);
                comps.Add(loadlin);
                var val4 = new Vari("%torch.nn.LinearImpl*", "%val4");
                var lfl = new Call(val4, lflin, new Vari("i64", (16 * 62 * 46).ToString()), new Vari("i64", "2"), new Vari("i1", "true"));
                comps.Add(lfl);
                var lineptr = new Vari("%torch.nn.LinearImpl*", "%lineptr");
                var geteline = new Gete("%TestType", lineptr, valt, new Vari("i32", "0"), new Vari("i32", "3"));
                comps.Add(geteline);
                var storeline = new Store(lineptr, val4);
                comps.Add(storeline);
                var str3 = new Vari("i8*", "%str3");
                var bit3 = new Vari("%torch.nn.ModuleImpl*", "%bit3");
                comps.Add(new Bitcast(bit3, val4));
                var gete3 = new Gete("[6 x i8]", str3, new Vari("[6 x i8]*", "@.str.line1"), new Vari("i64", "0"), new Vari("i64", "0"));
                comps.Add(gete3);
                var lfrg3 = new Call(null, lfreg, val, str3, bit3);
                comps.Add(lfrg3);
                var ret = new Ret(val);
                comps.Add(ret);
            }
            else if (y.name == "@forward")
            {
                var gfconv = new Vari("%ForwardConv2dType*", "@ForwardConv2d");
                var gfrelu = new Vari("%ForwardReLUType*", "@ForwardReLU");
                var gfflat = new Vari("%ForwardFlattenType*", "@ForwardFlatten");
                var gfline = new Vari("%ForwardLinearType*", "@ForwardLinear");
                var lfconv = new Vari("%ForwardConv2dType", "%lfconv");
                var lfrelu = new Vari("%ForwardReLUType", "%lfrelu");
                var lfflat = new Vari("%ForwardFlattenType", "%lfflat");
                var lfline = new Vari("%ForwardLinearType", "%lfline");
                var val = new Vari("%TestType*", "%tobj");
                var bitcast = new Bitcast(val, draws[0] as Vari);
                comps.Add(bitcast);
                var conv_ptr = new Vari("%torch.nn.Conv2dImpl*", "%conv_ptr");
                var geteconv = new Gete("%TestType", conv_ptr, val, new Vari("i32", "0"), new Vari("i32", "0"));
                comps.Add(geteconv);
                var conv = new Vari("%torch.nn.Conv2dImpl*", "%conv");
                var loadconv = new Load(conv, conv_ptr);
                comps.Add(loadconv);
                var localconv = new Load(lfconv, gfconv);
                comps.Add(localconv);
                var val2 = new Vari("i8*", "%x1");
                var forwardconv = new Call(val2, lfconv, conv, draws[1] as Vari);
                comps.Add(forwardconv);
                var relu_ptr = new Vari("%torch.nn.ReLUImpl*", "%relu_ptr");
                var geterelu = new Gete("%TestType", relu_ptr, val, new Vari("i32", "0"), new Vari("i32", "1"));
                comps.Add(geterelu);
                var relu = new Vari("%torch.nn.ReLUImpl*", "%relu");
                var loadrelu = new Load(relu, relu_ptr);
                comps.Add(loadrelu);
                var localrelu = new Load(lfrelu, gfrelu);
                comps.Add(localrelu);
                var val3 = new Vari("i8*", "%x2");
                var forwardrelu = new Call(val3, lfrelu, relu, val2);
                comps.Add(forwardrelu);
                var flat_ptr = new Vari("%torch.nn.FlattenImpl*", "%flat_ptr");
                var geteflat = new Gete("%TestType", flat_ptr, val, new Vari("i32", "0"), new Vari("i32", "2"));
                comps.Add(geteflat);
                var flat = new Vari("%torch.nn.FlattenImpl*", "%flat");
                var loadflat = new Load(flat, flat_ptr);
                comps.Add(loadflat);
                var localflat = new Load(lfflat, gfflat);
                comps.Add(localflat);
                var val5 = new Vari("i8*", "%x3");
                var forwardflat = new Call(val5, lfflat, flat, val3);
                comps.Add(forwardflat);
                var line_ptr = new Vari("%torch.nn.LinearImpl*", "%line_ptr");
                var geteline = new Gete("%TestType", line_ptr, val, new Vari("i32", "0"), new Vari("i32", "3"));
                comps.Add(geteline);
                var line = new Vari("%torch.nn.LinearImpl*", "%line");
                var loadline = new Load(line, line_ptr);
                comps.Add(loadline);
                var localline = new Load(lfline, gfline);
                comps.Add(localline);
                var val4 = new Vari("i8*", "%x4");
                var forwardline = new Call(val4, lfline, line, val5);
                comps.Add(forwardline);
                comps.Add(new Ret(val4));
            }
        }
        public void RegisterParameter(Vari param, int argNo, int line)
        {
            int typeId = llvm.debugInfo.GetTypeId(param.type.TrimEnd('*'));
            int varId = llvm.debugInfo.metaId++;

            var debugVar = new DebugVariable(
                param.name.TrimStart('%'),
                typeId,
                debugId,
                line,
                varId
            );
            debugVar.argNo = argNo; // パラメータ番号
            debugVariables.Add(debugVar);
        }
        public void RegisterLocalVariable(Vari var, int line)
        {
            int typeId = llvm.debugInfo.GetTypeId(var.type.TrimEnd('*'));
            int varId = llvm.debugInfo.metaId++;

            var debugVar = new DebugVariable(
                var.name.TrimStart('%'),
                typeId,
                debugId,
                line,
                varId
            );
            debugVariables.Add(debugVar);
        }
        public override String output()
        {
            String output = base.output();
            output += "define dllexport " + y.type + " " + y.name + "(";
            for (var i = 0; i < draws.Count; i++)
            {
                if (i != 0) output += ", ";
                output += draws[i].output();
            }
            output += ") ";
            if (optnone) output += "optnone noinline ";
            if (debugId >= 0)
            {
                output += "!dbg !" + debugId + " ";
            }
            output += "{\n";
            output += "entry:\n";
            int debugLocationId = -1;
            for (var i = 0; i < dbinits.Count; i++)
            {
                output += dbinits[i].output();
                if (i != dbinits.Count - 1)
                {
                    if (dbinits[i + 1] is Br || dbinits[i + 1] is SwitchComp || dbinits[i + 1] is Phi) continue;
                    if (debugLocationId >= 0) output += dbinits[i].OutputDebugLoc();
                    output += "\n";
                }
            }
            for (var i = 0; i < comps.Count; i++)
            {
                if (comps[i] is LocationMarker)
                {
                    var marker = comps[i] as LocationMarker;
                    debugLocationId = llvm.debugInfo.CreateLocation(marker);
                    continue;
                }
                else if (debugLocationId >= 0) comps[i].debugLocationId = debugLocationId;
                output += comps[i].output();
                if (comps[i] is Call && (comps[i] as Call).f.name == "@llvm.coro.save") continue;
                if (comps[i] is Br || comps[i] is SwitchComp || comps[i] is Ret) continue;
                if (i != comps.Count - 1)
                {
                    if (comps[i + 1] is Br || comps[i + 1] is SwitchComp || comps[i + 1] is Phi) continue;
                    output += "                                                                                                                       call void @print_int(i32 " + fn++ + ")";
                    if (debugLocationId >= 0) output += comps[i].OutputDebugLoc();
                    output += "\n";
                }
            }
            output += "}\n";
            return output;
        }
    }
    class Gete : Component
    {
        String type;
        Vari y;
        Vari x;
        public Gete(String type, Vari y, Vari x, params Vari[] xs)
        {
            this.type = type;
            this.y = y;
            this.x = x;
            for (var i = 0; i < xs.Length; i++) comps.Add(xs[i]);
        }
        public override string output()
        {
            String output = "";
            output += this.y.name + " = getelementptr " + type + "," + this.x.type + "" + " " + this.x.name;
            output += "\n";
            for (var i = 0; i < comps.Count; i++) output += ", " + (comps[i] as Vari).type + " " + (comps[i] as Vari).name;
            output += OutputDebugLoc();
            return output;
        }
    }
    class Bitcast : Component
    {
        Vari x;
        Vari y;
        public int debugVarId = -1;
        public Bitcast(Vari y, Vari x)
        {
            this.y = y;
            this.x = x;
        }
        public override string output()
        {
            String output = y.name + " = bitcast " + x.output() + " to " + y.type;
            if (debugVarId >= 0)
            {
                output += "\n";
                output += "call void @llvm.dbg.value("; // bitcastの結果はSSA値なのでvalueを使う
                output += "metadata " + y.type + " " + y.name + ", ";
                output += "metadata !" + debugVarId + ", ";
                output += "metadata !DIExpression())";
            }
            output += OutputDebugLoc() + "\n";
            return output;
        }
        public static Vari Cast(Local local, Type t1, Type t2, Vari vari)
        {
            if (t1 == t2) return vari;
            else
            {
                if (t1 == local.Int)
                {
                    var v = new Vari("i32", "%v" + LLVM.n++);
                    if (t2 == local.Short || t2 == local.Bool)
                    {
                        local.llvm.func.comps.Add(new Zext(v, vari));
                    }
                    else throw new Exception("");
                    return v;
                }
                else if (t1 == local.Short)
                {
                    var v = new Vari("i16", "%v" + LLVM.n++);
                    if (t2 == local.Int)
                    {
                        local.llvm.func.comps.Add(new Trunc(v, vari));

                    }
                    else if (t2 == local.Bool)
                    {
                        local.llvm.func.comps.Add(new Zext(v, vari));
                    }
                    else throw new Exception("");
                    return v;
                }
                else if (t1 == local.Bool)
                {
                    var v = new Vari("i1", "%v" + LLVM.n++);
                    if (t2 == local.Int || t2 == local.Bool)
                    {
                        local.llvm.func.comps.Add(new Trunc(v, vari));
                    }
                    else throw new Exception("");
                    return v;
                }
                else return vari;
            }
        }
    }
    class Zext : Component
    {
        Vari x;
        Vari y;
        public Zext(Vari y, Vari x)
        {
            this.y = y;
            this.x = x;
        }
        public override string output()
        {
            String output = y.name + " = zext " + x.output() + " to " + y.type;
            output += OutputDebugLoc();
            return output + "\n";
        }
    }
    class Sext : Component
    {
        Vari x;
        Vari y;
        public Sext(Vari y, Vari x)
        {
            this.y = y;
            this.x = x;
        }
        public override string output()
        {
            String output = y.name + " = sext " + x.output() + " to " + y.type;
            output += OutputDebugLoc();
            return output + "\n";
        }
    }
    class Trunc : Component
    {
        Vari x;
        Vari y;
        public Trunc(Vari y, Vari x)
        {
            this.y = y;
            this.x = x;
        }
        public override string output()
        {
            String output = y.name + " = trunc " + x.output() + " to " + y.type;
            output += OutputDebugLoc();
            return output + "\n";
        }
    }
    class PtrToInt : Component
    {
        Vari y, x;
        public PtrToInt(Vari y, Vari x)
        {
            this.y = y; this.x = x;
        }
        public override string output()
        {
            String output = y.name + " = ptrtoint " + x.output() + " to " + y.type + "\n";
            output += OutputDebugLoc();
            return output;
        }
    }
    class IntToPtr : Component
    {
        Vari y, x;
        public IntToPtr(Vari y, Vari x)
        {
            this.y = y; this.x = x;
        }
        public override string output()
        {
            String output = y.name + " = inttoptr " + x.output() + " to " + y.type + "\n";
            output += OutputDebugLoc();
            return output;
        }
    }
    class Lab : Component
    {
        public String name;
        public Lab(string name)
        {
            this.name = name;
        }
        public override string output()
        {
            return name + ":\n";
        }
    }
    class Call : Component
    {
        Vari y;
        public Vari f;
        public Call(Vari y, Vari f, params Vari[] xs)
        {
            this.y = y;
            this.f = f;
            for (var i = 0; i < xs.Length; i++)
            {
                comps.Add(xs[i]);
            }
        }
        public override string output()
        {
            String output = "";
            if (y != null)
            {
                if (y.name != null) output += this.y.name + " = call " + y.type + " " + f.name + "(";
                else output += "call " + y.type + " " + f.name + "(";
            }
            else output += "call void " + f.name + "(";
            for (var i = 0; i < comps.Count; i++)
            {
                if (i != 0) output += ", ";
                output += comps[i].output();
            }
            output += ")" + OutputDebugLoc() + "\n";
            return output;
        }
    }
    class Alloca : Component
    {
        Vari a;
        public Alloca(Vari a)
        {
            this.a = a;
        }
        public override string output()
        {
            return "" + this.a.name + " = alloca " + a.type.Substring(0, a.type.Length - 1) + OutputDebugLoc() + "\n";
        }
    }
    class Store : Component
    {
        Vari a, y;
        public Store(Vari y, Vari a)
        {
            this.y = y; this.a = a;
        }
        public override string output()
        {
            return "store " + a.output() + ", " + y.type + " " + y.name + OutputDebugLoc() + "\n";
        }
    }
    class Load : Component
    {
        Vari a, y;
        public bool vol;
        public Load(Vari y, Vari a, bool vol = false)
        {
            this.y = y; this.a = a; this.vol = vol;
        }
        public override string output()
        {
            var str = "load";
            if (vol) str += " volatile";
            return this.y.name + " = " + str + " " + y.type + ", " + a.type + " " + a.name + OutputDebugLoc() + "\n";
        }
    }
    class Br : Component
    {
        Vari a;
        public Br(Vari a, params Lab[] labs)
        {
            this.a = a;
            for (var i = 0; i < labs.Length; i++) comps.Add(labs[i]);
        }
        public override string output()
        {
            String output = "br ";
            if (a != null) output += a.output() + ", ";
            for (var i = 0; i < comps.Count; i++)
            {
                if (i != 0) output += ", ";
                output += "label %" + (comps[i] as Lab).name;
            }
            output += OutputDebugLoc();
            return output + "\n";
        }
    }
    class SwitchComp : Component
    {
        public Vari x;
        public Lab lab;
        public List<IfValue> values;
        public SwitchComp(Vari x, Lab lab, List<IfValue> values)
        {
            this.x = x;
            this.lab = lab;
            this.values = values;
        }
        override public string output()
        {
            String output = "switch " + x.output() + ", label %" + lab.name + " [\n";
            for (var i = 0; i < values.Count; i++)
            {
                output += values[i].v.output() + ", " + "label %" + values[i].lab.name + OutputDebugLoc() + "\n";
            }
            return output + "]\n";
        }
    }
    class Add : Component
    {
        Vari a, b, y;
        public Add(Vari y, Vari a, Vari b)
        {
            this.y = y; this.a = a; this.b = b;
        }
        public override string output()
        {
            return this.y.name + " = add " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class Sub : Component
    {
        Vari a, b, y;
        public Sub(Vari y, Vari a, Vari b)
        {
            this.y = y; this.a = a; this.b = b;
        }
        public override string output()
        {
            return this.y.name + " = sub " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class Mul : Component
    {
        Vari a, b, y;
        public Mul(Vari y, Vari a, Vari b)
        {
            this.y = y; this.a = a; this.b = b;
        }
        public override string output()
        {
            return this.y.name + " = mul " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class SDiv : Component
    {
        Vari a, b, y;
        public SDiv(Vari y, Vari a, Vari b)
        {
            this.y = y; this.a = a; this.b = b;
        }
        public override string output()
        {
            return this.y.name + " = sdiv " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class SRem : Component
    {
        Vari a, b, y;
        public SRem(Vari y, Vari a, Vari b)
        {
            this.y = y; this.a = a; this.b = b;
        }
        public override string output()
        {
            return this.y.name + " = srem " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class Eq : Component
    {
        Vari a, b, y;
        public Eq(Vari y, Vari a, Vari b)
        {
            this.a = a;
            this.b = b;
            this.y = y;
        }
        public override string output()
        {
            return this.y.name + " = icmp eq " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class Ne : Component
    {
        Vari a, b, y;
        public Ne(Vari y, Vari a, Vari b)
        {
            this.a = a;
            this.b = b;
            this.y = y;
        }
        public override string output()
        {
            return this.y.name + " = icmp ne " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class SLessThan : Component
    {
        Vari a, b, y;
        public SLessThan(Vari y, Vari a, Vari b)
        {
            this.a = a;
            this.b = b;
            this.y = y;
        }
        public override string output()
        {
            return this.y.name + " = icmp slt " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class SLessEqual : Component
    {
        Vari a, b, y;
        public SLessEqual(Vari y, Vari a, Vari b)
        {
            this.a = a;
            this.b = b;
            this.y = y;
        }
        public override string output()
        {
            return this.y.name + " = icmp sle " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class SMoreThan : Component
    {
        Vari a, b, y;
        public SMoreThan(Vari y, Vari a, Vari b)
        {
            this.a = a;
            this.b = b;
            this.y = y;
        }
        public override string output()
        {
            return this.y.name + " = icmp sgt " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class SMoreEqual : Component
    {
        Vari a, b, y;
        public SMoreEqual(Vari y, Vari a, Vari b)
        {
            this.a = a;
            this.b = b;
            this.y = y;
        }
        public override string output()
        {
            return this.y.name + " = icmp sge " + a.output() + ", " + b.name + OutputDebugLoc() + "\n";
        }
    }
    class LocationMarker : Component
    {
        public int line;
        public int column;
        public int scopeId;
        public int metaid;

        public LocationMarker(int line, int column, int scopeId)
        {
            this.line = line;
            this.column = column;
            this.scopeId = scopeId;
        }

        public override string output()
        {
            // LLVM IRには出力しない（マーカーとしてのみ機能）
            return "";
        }
    }
    class Ret : Component
    {
        Vari a;
        public Ret(Vari a)
        {
            this.a = a;
        }
        public override string output()
        {
            if (a.type == "void") return "ret void" + OutputDebugLoc() + "\n";
            return "ret " + a.type + " " + a.name + OutputDebugLoc() + "\n";
        }
    }
    class DebugInfo
    {
        public int metaId = 0;
        public Dictionary<string, int> fileIds = new Dictionary<string, int>();
        public Dictionary<string, int> functionIds = new Dictionary<string, int>();
        public int compileUnitId;
        public int dwarfVersionId;
        public int debugInfoVersionId;
        public Dictionary<string, int> typeIds = new Dictionary<string, int>();
        public Dictionary<string, int> structTypeIds = new Dictionary<string, int>();
        // 既存のフィールド...
        public Dictionary<string, StructTypeInfo> structTypes =
            new Dictionary<string, StructTypeInfo>();
        public Dictionary<int, LocationMarker> locations = new Dictionary<int, LocationMarker>();
        public Dictionary<int, int> pointerTypes = new Dictionary<int, int>(); // baseTypeId -> ptrTypeId

        public DebugInfo()
        {
            // モジュールフラグ用のIDを予約
            dwarfVersionId = metaId++;
            debugInfoVersionId = metaId++;
            compileUnitId = metaId++;
        }

        public int CreateFile(string filename, string directory)
        {
            var key = filename + "#" + directory;
            if (fileIds.ContainsKey(key)) return fileIds[key];

            int id = metaId++;
            fileIds[key] = id;
            return id;
        }
        private HashSet<string> definedTypes = new HashSet<string>();
        private List<string> typeDefinitionOrder = new List<string>();

        public int GetOrCreateType(string llvmType)
        {
            // 既に定義済みの場合
            string typeKey = NormalizeTypeKey(llvmType);
            if (typeIds.ContainsKey(typeKey))
                return typeIds[typeKey];

            // ポインタ型の処理
            if (llvmType.EndsWith("*"))
            {
                string baseType = llvmType.Substring(0, llvmType.Length - 1).Trim();
                int baseTypeId = GetOrCreateType(baseType);

                int ptrId = metaId++;
                typeIds[typeKey] = ptrId;
                return ptrId;
            }

            // 配列型の処理 [N x type]
            if (llvmType.StartsWith("["))
            {
                var match = System.Text.RegularExpressions.Regex.Match(
                    llvmType, @"\[(\d+) x (.+)\]");
                if (match.Success)
                {
                    int count = int.Parse(match.Groups[1].Value);
                    string elementType = match.Groups[2].Value;
                    int elementTypeId = GetOrCreateType(elementType);

                    int arrayId = metaId++;
                    typeIds[typeKey] = arrayId;
                    return arrayId;
                }
            }

            // 構造体型の処理 %TypeName
            if (llvmType.StartsWith("%"))
            {
                string structName = llvmType.TrimStart('%');
                if (structTypeIds.ContainsKey(structName))
                    return structTypeIds[structName];

                // まだ定義されていない構造体の場合は前方宣言
                int structId = metaId++;
                structTypeIds[structName] = structId;
                return structId;
            }

            // 基本型
            return CreateBasicType(llvmType);
        }

        private int CreateBasicType(string llvmType)
        {
            string typeKey = NormalizeTypeKey(llvmType);

            if (typeIds.ContainsKey(typeKey))
                return typeIds[typeKey];

            int id = metaId++;
            typeIds[typeKey] = id;
            definedTypes.Add(typeKey);

            return id;
        }
        public int GetTypeId(string llvmType)
        {
            // LLVM型からDIBasicTypeへのマッピング
            llvmType = llvmType.TrimEnd('*'); // ポインタを除去

            switch (llvmType)
            {
                case "i32": return GetOrCreateType("i32");
                case "i16": return GetOrCreateType("i16");
                case "i8": return GetOrCreateType("i8");
                case "i1": return GetOrCreateType("i1");
                case "i64": return GetOrCreateType("i64");
                case "float": return GetOrCreateType("float");
                case "double": return GetOrCreateType("double");
                case "ptr": return GetOrCreateType("ptr");
                default:
                    // 構造体型など
                    if (llvmType.StartsWith("%"))
                        return GetOrCreateType(llvmType);
                    return GetOrCreateType("ptr"); // デフォルトはポインタ
            }
        }
        public int GetPointerType(int baseTypeId)
        {
            if (pointerTypes.ContainsKey(baseTypeId))
                return pointerTypes[baseTypeId];

            int ptrTypeId = metaId++;
            pointerTypes[baseTypeId] = ptrTypeId;

            return ptrTypeId;
        }
        private string NormalizeTypeKey(string llvmType)
        {
            // 型キーの正規化
            return llvmType.Trim();
        }
        public int CreateFunction(string name, int fileId, int line)
        {
            int id = metaId++;
            functionIds[name] = id;
            return id;
        }

        public int CreateLocation(LocationMarker lm)
        {
            int id = metaId++;
            locations[id] = lm;
            lm.metaid = id;
            return id;
        }

    }
    class DebugVariable
    {
        public string name;
        public int typeId;
        public int scopeId;
        public int line;
        public int metaId;
        public int argNo = 0;

        public DebugVariable(string name, int typeId, int scopeId, int line, int metaId)
        {
            this.name = name;
            this.typeId = typeId;
            this.scopeId = scopeId;
            this.line = line;
            this.metaId = metaId;
        }
    }
    class StructField
    {
        public string name;
        public string llvmType;
        public int offsetInBits;
        public int sizeInBits;
        public int metaId;

        public StructField(string name, string llvmType, int offsetInBits, int sizeInBits, int metaId)
        {
            this.name = name;
            this.llvmType = llvmType;
            this.offsetInBits = offsetInBits;
            this.sizeInBits = sizeInBits;
            this.metaId = metaId;
        }
    }
    class StructTypeInfo
    {
        public int structId;
        public int fileId;
        public List<StructField> fields;
        public string name;
    }
}
