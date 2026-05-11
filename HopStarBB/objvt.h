// ============================================================
// objvt.h - 仮想関数テーブル (Virtual Table) 管理
// ObjVTの初期化とディスパッチヘルパー
// ============================================================

// ============================================================
// デフォルト関数 (C#のObj基底クラスに対応)
// ============================================================
Obj* _vt_exeZ_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_exepZ_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* _vt_GetterZ_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_SelfZ_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_opeZ_default(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) { return self; }
Obj* _vt_PrimaryZ_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) { (*n)++; return self; }
Obj* _vt_exeA_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_exepA_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* _vt_GetterA_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_SelfA_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_opeA_default(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) { return self; }
Obj* _vt_PrimaryA_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) { (*n)++; return self; }
Obj* _vt_exeB_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_exepB_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* _vt_GetterB_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_SelfB_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_opeB_default(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) { return self; }
Obj* _vt_PrimaryB_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) { (*n)++; return self; }
Obj* _vt_exeC_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_exepC_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* _vt_GetterC_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_SelfC_default(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* _vt_opeC_default(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) { return self; }
Obj* _vt_PrimaryC_default(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) { (*n)++; return self; }
void _vt_SetIdentity_default(ThreadGC* thgc, Obj* self) { /* no-op — types without identity logic */ }
Obj* _vt_Clone_default(ThreadGC* thgc, Obj* self) { return self; }  // C# base: public virtual Obj Clone() { return this; }

// ============================================================
// デフォルトVTable
// ============================================================
ObjVT vt_Obj_default = {
	_vt_exeZ_default, _vt_exepZ_default, _vt_GetterZ_default, _vt_SelfZ_default, _vt_opeZ_default, _vt_PrimaryZ_default,
	_vt_exeA_default, _vt_exepA_default, _vt_GetterA_default, _vt_SelfA_default, _vt_opeA_default, _vt_PrimaryA_default,
	_vt_exeB_default, _vt_exepB_default, _vt_GetterB_default, _vt_SelfB_default, _vt_opeB_default, _vt_PrimaryB_default,
	_vt_exeC_default, _vt_exepC_default, _vt_GetterC_default, _vt_SelfC_default, _vt_opeC_default, _vt_PrimaryC_default,
	_vt_SetIdentity_default, _vt_Clone_default
};

// ============================================================
// 各型用VTableインスタンス
// ============================================================
ObjVT vt_Operator;
ObjVT vt_Primary;
ObjVT vt_Block;
ObjVT vt_Block_Array;
ObjVT vt_CallBlock;
ObjVT vt_Word;
ObjVT vt_Number;
ObjVT vt_FloatVal;
ObjVT vt_StrObj;
ObjVT vt_BoolVal;
ObjVT vt_VoiVal;
ObjVT vt_Value;
ObjVT vt_Variable;
ObjVT vt_Function;
ObjVT vt_GenericFunction;
ObjVT vt_Comment;
ObjVT vt_Comment2;
ObjVT vt_TagBlock;
ObjVT vt_Signal;
ObjVT vt_SignalFunction;
ObjVT vt_ServerClient;
ObjVT vt_ServerFunction;
ObjVT vt_Dolor;
ObjVT vt_Mountain;
ObjVT vt_HtmObj;
ObjVT vt_ElemType;
ObjVT vt_ElemObj;
ObjVT vt_LinearFunction;
ObjVT vt_AnimationFunction;
ObjVT vt_Connect;
ObjVT vt_ConnectStock;
ObjVT vt_Address;
ObjVT vt_Sum;
ObjVT vt_Question;
ObjVT vt_Clones;
ObjVT vt_Class;
ObjVT vt_Type;
ObjVT vt_ArrType;
ObjVT vt_FuncType;
ObjVT vt_Var;
ObjVT vt_ClassObj;
ObjVT vt_Generic;
ObjVT vt_Model;
ObjVT vt_ModelObj;
ObjVT vt_ModelVal;
ObjVT vt_Gene;
ObjVT vt_GeneObj;
ObjVT vt_GeneLabel;
ObjVT vt_If;
ObjVT vt_While;
ObjVT vt_For;
ObjVT vt_Switch;
ObjVT vt_Break;
ObjVT vt_Continue;
ObjVT vt_Return;
ObjVT vt_Goto;
ObjVT vt_Print;
ObjVT vt_SqlString;
ObjVT vt_Null;
ObjVT vt_ArrayConstructor;
ObjVT vt_Constructor;
ObjVT vt_Base;
ObjVT vt_GeneChild;
ObjVT vt_GeneMutate;
ObjVT vt_GeneNew;
ObjVT vt_GeneCross;
ObjVT vt_GeneVal;
ObjVT vt_GeneStore;
ObjVT vt_GeneSelect;
ObjVT vt_GeneSort;
ObjVT vt_GeneLabelValue;
ObjVT vt_IfBlock;
ObjVT vt_Iterator;
ObjVT vt_NewBlock;
ObjVT vt_CrossBlock;
ObjVT vt_MigrateBlock;

// ============================================================
// LetterType → VTable ルックアップテーブル (objtypeから引く)
// LetterType は _* (~67) の後ろに OT_* (~80) が並ぶ並び。
// 最大 idx ~ 150。256 で余裕を見る。
// ============================================================
#ifndef VT_LOOKUP_SIZE
#define VT_LOOKUP_SIZE 256
#endif
ObjVT* vt_lookup[VT_LOOKUP_SIZE];

// ============================================================
// ヘルパー: vtを安全に呼ぶマクロ
// ============================================================
#define VT_CALL(self, fn, ...) \
	((self)->vt ? (self)->vt->fn(__VA_ARGS__) : _vt_##fn##_default(__VA_ARGS__))

// ============================================================
// ディスパッチ関数 (self->vtがNULLでも安全に動く)
// ============================================================
// lazy vtable assignment: vtがNULLならvt_lookupから自動割り当て
static inline void _vt_lazy_assign(Obj* self) {
	if (self->vt == NULL) {
		int idx = (int)self->type;
		if (idx >= 0 && idx < VT_LOOKUP_SIZE) self->vt = vt_lookup[idx];
	}
}

Obj* vt_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->exeA(thgc, self, local);
}
Obj* vt_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	_vt_lazy_assign(self);
	return self->vt->exepA(thgc, self, n, local, primary);
}
Obj* GetterA_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->GetterA(thgc, self, local);
}
Obj* SelfA_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->SelfA(thgc, self, local);
}
Obj* vt_opeA(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	_vt_lazy_assign(self);
	return self->vt->opeA(thgc, self, key, local, val2);
}
Obj* PrimaryA_vt(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	_vt_lazy_assign(self);
	return self->vt->PrimaryA(thgc, self, n, local, primary, val2);
}
Obj* vt_exeZ(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->exeZ(thgc, self, local);
}
Obj* vt_exepZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	_vt_lazy_assign(self);
	return self->vt->exepZ(thgc, self, n, local, primary);
}
Obj* GetterZ_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->GetterZ(thgc, self, local);
}
Obj* SelfZ_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->SelfZ(thgc, self, local);
}
Obj* PrimaryZ_vt(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	_vt_lazy_assign(self);
	return self->vt->PrimaryZ(thgc, self, n, local, primary, val2);
}
Obj* vt_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->exeB(thgc, self, local);
}
Obj* vt_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	_vt_lazy_assign(self);
	return self->vt->exepB(thgc, self, n, local, primary);
}
Obj* GetterB_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->GetterB(thgc, self, local);
}
Obj* SelfB_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->SelfB(thgc, self, local);
}
Obj* vt_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	_vt_lazy_assign(self);
	return self->vt->opeB(thgc, self, key, local, val2);
}
Obj* PrimaryB_vt(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	_vt_lazy_assign(self);
	return self->vt->PrimaryB(thgc, self, n, local, primary, val2);
}

Obj* vt_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->exeC(thgc, self, local);
}
Obj* vt_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	_vt_lazy_assign(self);
	return self->vt->exepC(thgc, self, n, local, primary);
}
Obj* GetterC_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->GetterC(thgc, self, local);
}
Obj* SelfC_vt(ThreadGC* thgc, Obj* self, Master* local) {
	_vt_lazy_assign(self);
	return self->vt->SelfC(thgc, self, local);
}
Obj* vt_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	_vt_lazy_assign(self);
	return self->vt->opeC(thgc, self, key, local, val2);
}
Obj* PrimaryC_vt(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	_vt_lazy_assign(self);
	return self->vt->PrimaryC(thgc, self, n, local, primary, val2);
}

Obj* vt_Clone(ThreadGC* thgc, Obj* self) {
	_vt_lazy_assign(self);
	return self->vt->Clone(thgc, self);
}

// ============================================================
// VTable割り当てヘルパー
// ============================================================
void vt_assign(Obj* obj, ObjVT* vt) {
	if (obj) obj->vt = vt;
}
void vt_assign_block(Block* blk, ObjVT* vt) {
	if (blk) blk->vt = vt;
}

// ============================================================
// initVTables - 起動時にVTableルックアップを初期化
// (obj2a.h, obj2a2.h, obj2a3.h, objz.h の関数実装後に呼ぶ)
// ============================================================
void initVTables();
void A2_initVTables();
void A3_initVTables();
void B_initVTables();
void B2_initVTables();
void B3_initVTables();
void C_initVTables();
