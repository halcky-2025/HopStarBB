// ============================================================
// obj2b2.h - B解析パス (vtable dispatch版) - Block 2
// Word, Value, Class, Type, Var, Generic, Number, FloatVal,
// SqlString, StrObj, BoolVal, VoiVal, Null, ArrayConstructor,
// Constructor, Base, Variable, Function, If, While, For, Switch,
// Break, Continue, Return, Goto, Print, Iterator,
// GenericFunction, Model, ModelObj, ModelVal, Gene, GeneObj,
// GeneChild, GeneMutate, GeneNew, GeneCross, GeneVal,
// GeneStore, GeneSelect, GeneSort, GeneLabel, GeneLabelValue,
// NewBlock, CrossBlock, MigrateBlock
// ============================================================

// 前方宣言
Obj* Word_SelfB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Word_GetterB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Word_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Word_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Value_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Class_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Class_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Type_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Var_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Generic_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Number_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Number_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Number_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
// Number B-pass opeB 前方宣言
Obj* Number_PlusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MinusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MulB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_DivB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MoreThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_LessThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MoreEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_LessEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_NotEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_EqualEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* FloatVal_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* FloatVal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* FloatVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
// FloatVal B-pass opeB 前方宣言
Obj* FloatVal_PlusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MinusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MulB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_DivB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MoreThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_LessThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MoreEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_LessEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_NotEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_EqualEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* SqlString_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* SqlString_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* SqlString_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* SqlString_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* StrObj_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* StrObj_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* StrObj_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* BoolVal_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* BoolVal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* BoolVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* VoiVal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* VoiVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Null_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Null_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Null_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ArrayConstructor_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Constructor_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Base_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Variable_GetterB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Variable_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
// Variable B-pass opeB 前方宣言
Obj* Variable_OpeB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Variable_InB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Variable_EqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Variable_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* Function_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* If_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* If_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* While_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* While_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* For_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* For_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Switch_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Switch_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Break_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Break_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Continue_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Continue_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Return_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Return_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Goto_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Goto_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Print_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Print_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Iterator_GetterB(ThreadGC* thgc, Obj* self, Master* local);
Obj* GenericFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Model_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Model_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ModelObj_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* ModelVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
// Stock 前方宣言
Obj* Stock_SelectB(ThreadGC* thgc, Obj* self, Obj* model, Master* local, Obj* func);
Obj* Stock_StoreB(ThreadGC* thgc, Obj* self, Obj* val, Master* local);
Obj* Gene_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Gene_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneChild_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneMutate_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneNew_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneCross_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneStore_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneSelect_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneSort_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneLabel_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* GeneLabelValue_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* NewBlock_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* CrossBlock_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* MigrateBlock_exeB(ThreadGC* thgc, Obj* self, Master* local);
// GeneObj::DotB 前方宣言
Obj* GeneObj_DotB(ThreadGC* thgc, Obj* self, String* name);
Obj* GeneObj_exeB(ThreadGC* thgc, Obj* self, Master* local);

// ============================================================
// Block_Sort: C# Block.Sort(Function func, Local local)
// ============================================================
Block* Block_Sort(ThreadGC* thgc, Block* arr, Function* func, Master* local) {
	for (int i = 0; i < arr->rets->size; i++) {
		for (int j = i + 1; j < arr->rets->size; j++) {
			local->blockslist = func->blocks;
			Block* block1 = (Block*)vt_Clone(thgc, *(Obj**)get_list(func->draw->children, 0));
			add_list(thgc, local->blockslist, (char*)block1);
			GetterA_vt(thgc, vt_exeA(thgc, (Obj*)block1, local), local);
			int k = 0;
			for (int vi = 0; vi < block1->vmapA->kvs->size; vi++) {
				KV* kv = (KV*)get_list(block1->vmapA->kvs, vi);
				Obj* val = (Obj*)kv->value;
				if (val->type == LetterType::OT_Variable) {
					if (k == 0) vt_opeA(thgc, val, LetterType::_Equal, local, *(Obj**)get_list(arr->rets, i));
					else if (k == 1) vt_opeA(thgc, val, LetterType::_Equal, local, *(Obj**)get_list(arr->rets, j));
				}
				k++;
			}
			Block* block2 = (Block*)vt_Clone(thgc, *(Obj**)get_list(func->draw->children, 1));
			add_list(thgc, local->blockslist, (char*)block2);
			GetterA_vt(thgc, vt_exeA(thgc, (Obj*)block2, local), local);
			if (block2->rets->size > 0) {
				Obj* last = *(Obj**)get_list(block2->rets, block2->rets->size - 1);
				if (last->type == LetterType::OT_BoolVal) {
					if (!((BoolVal*)last)->value) {
						char* tmp = *(char**)get_list(arr->rets, i);
						*(char**)get_list(arr->rets, i) = *(char**)get_list(arr->rets, j);
						*(char**)get_list(arr->rets, j) = tmp;
					}
				}
			}
			local->blockslist->size -= 3;
		}
	}
	return arr;
}

// ============================================================
// Word::SelfB
// ============================================================
Obj* Word_SelfB(ThreadGC* thgc, Obj* self, Master* local) {
	// C#: var obj = local.getB(name, letter).Clone(); obj.letter = letter; return obj;
	Obj* obj = vt_Clone(thgc, getB(thgc, local, self->letter->text, self->letter));
	obj->letter = self->letter;
	return obj;
}

// ============================================================
// Word::GetterB
// ============================================================
Obj* Word_GetterB(ThreadGC* thgc, Obj* self, Master* local) {
	// C#: var obj = local.getB(name, letter).GetterB(local).Clone(); obj.letter = letter; return obj;
	Obj* obj = getB(thgc, local, self->letter->text, self->letter);
	obj = vt_Clone(thgc, GetterB_vt(thgc, obj, local));
	obj->letter = self->letter;
	return obj;
}

// ============================================================
// Word::exepB
// ============================================================
Obj* Word_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Word::PrimaryB
// ============================================================
Obj* Word_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	(*n)++;
	return self;
}

// ============================================================
// Value::PrimaryB
// ============================================================
Obj* Value_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV), CType::_CObj);
				Map* vmap = ((Val*)self)->vmap;
				if (vmap != NULL) {
					for (int i = 0; i < vmap->kvs->size; i++) {
						KV* kv = (KV*)get_list(vmap->kvs, i);
						add_kouho(thgc, local->kouhos, kv->key, (Obj*)kv->value);
					}
				}
			}
			(*n)++;
			String* name = val2->letter->text;
			Value* vself = (Value*)self;
			if (vself->vmap0 != NULL) {
				char* found0 = get_mapy(vself->vmap0, name);
				if (found0 != NULL) {
					Obj* obj = vt_Clone(thgc, (Obj*)found0);
					obj->letter = self->letter;
					return obj;
				}
			}
			if (((Val*)self)->vmap != NULL) {
				char* found = get_mapy(((Val*)self)->vmap, name);
				if (found != NULL) {
					Obj* obj = vt_Clone(thgc, (Obj*)found);
					obj->letter = self->letter;
					return obj;
				}
			}
		}
	} else if (val2->type == LetterType::OT_Bracket) {
		Obj* ret0 = ((Val*)self)->cls;
		if (ret0->type == LetterType::OT_Var) ret0 = ((Var*)ret0)->cls;
		if (ret0->type != LetterType::OT_FuncType)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "()を引数に取るには関数型でなければなりません");
		FuncTypeC* functype = (FuncTypeC*)ret0;
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (IS_WEG(val)) return val;
		Block* blk = (Block*)val;
		if (functype->draws->size != blk->rets->size)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "関数の引数の数が合っていません");
		for (int i = 0; i < blk->rets->size; i++) {
			Obj* ret = CheckCVB_TypeCheck(thgc, *(Obj**)get_list(functype->draws, i), *(Obj**)get_list(blk->rets, i), CheckType_Setter, local);
			if (IS_WEG(ret)) return ret;
		}
		(*n)++;
		return New_Value(thgc, functype->cls, local, self->letter);
	} else if (val2->type == LetterType::OT_Block) {
		(*n)++;
		Obj* ret0 = ((Val*)self)->cls;
		if (ret0->type == LetterType::OT_Var) ret0 = ((Var*)ret0)->cls;
		if (ret0->type != LetterType::OT_ArrayType && ret0 != (Obj*)local->StrT)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "[]を引数に取るには配列型でなければなりません");
		ArrType* arrtype = (ArrType*)ret0;
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (IS_WEG(val)) return val;
		Block* blk = (Block*)val;
		if (ret0 == (Obj*)local->StrT) {
			if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "List配列の引数は1つだけです");
			Obj* ret = CheckCVB_TypeCheck(thgc, (Obj*)local->Int, *(Obj**)get_list(blk->rets, 0), CheckType_Setter, local);
			if (IS_WEG(ret)) return ret;
			return New_Value(thgc, (Obj*)local->StrT, local, self->letter);
		} else if (arrtype->list == NULL) {
			if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "List配列の引数は1つだけです");
			Obj* ret = CheckCVB_TypeCheck(thgc, (Obj*)local->Int, *(Obj**)get_list(blk->rets, 0), CheckType_Setter, local);
			if (ret->type == LetterType::OT_Wait) return ret;
			else if (ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) {
				ret = CheckCVB_TypeCheck(thgc, (Obj*)local->StrT, *(Obj**)get_list(blk->rets, 0), CheckType_Setter, local);
				if (IS_WEG(ret)) return ret;
			}
			return New_Value(thgc, arrtype->cls, local, self->letter);
		} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "コンパイルエラー");
	}
	// fallback
	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSet2(thgc, local);
	}
	Obj* val0 = NULL;
	Obj* ret00 = ((Val*)self)->cls;
	if (ret00->type == LetterType::OT_Var) ret00 = ((Var*)ret00)->cls;
	if (ret00->type != LetterType::OT_FuncType)
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "()を引数に取るには関数型でなければなりません");
	FuncTypeC* functype0 = (FuncTypeC*)ret00;
	if (functype0->draws->size != 1)
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "関数の引数の数が合っていません");
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val0);
		if (val0 != NULL && IS_WEG(val0)) return val0;
	}
	if (val0 != NULL) {
		val2 = GetterB_vt(thgc, val0, local);
		if (IS_WEG(val2)) return val2;
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj
			|| val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
			Obj* ret = CheckCVB_TypeCheck(thgc, *(Obj**)get_list(functype0->draws, 0), val2, CheckType_Setter, local);
			if (IS_WEG(ret)) return ret;
			(*n)++;
			return New_Value(thgc, (Obj*)ret, local, self->letter);
		}
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "値の後続が適切ではないです");
}

// ============================================================
// Class::exepB
// ============================================================
Obj* Class_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Class::PrimaryB
// ============================================================
Obj* Class_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Word) {
		Obj* word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Block) {
			Generic* generic = (Generic*)getB(thgc, local, word->letter->text, word->letter);
			Block* block = (Block*)vt_exeB(thgc, val2, local);
			List* blocks = get_blocks(local);
			add_list(thgc, blocks, (char*)block);
			for (int i = 0; i < block->rets->size; i++) {
				val2 = *(Obj**)get_list(block->rets, i);
			}
			blocks->size--;
			generic->block = block;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
		}
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Block) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					Block* blk = (Block*)val2;
					if (blk->children->size == 1) {
						Obj* ch0 = *(Obj**)get_list(blk->children, 0);
						if (ch0->type == LetterType::OT_FloatVal) {
						} else if (ch0->type == LetterType::OT_Number) {
						} else return ErrorA(thgc, LetterType::OT_Error, ch0->letter, "versionの引数は小数か整数だけです");
					} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "versionの引数は小数か整数だけです");
				}
			} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "テーブルを宣言してください。");
		}
	}
head:
	if (val2->type == LetterType::_Left) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Bracket) {
			add_list(thgc, local->calls, (char*)KouhoSetClaGen);
			Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
			local->calls->size--;
			if (IS_WEG(val)) return val;
			Block* blk = (Block*)val;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* reti = *(Obj**)get_list(blk->rets, i);
				if (reti->type == LetterType::OT_ClassObj || reti->type == LetterType::OT_GenericObj) {
					goto head;
				} else return ErrorA(thgc, LetterType::OT_Error, reti->letter, "クラスの継承元はクラスでなければなりません");
			}
		} else {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				KouhoSetClaGen(thgc, local);
			}
			val2 = GetterB_vt(thgc, val2, local);
			if (IS_WEG(val2)) return val2;
			if (val2->type == LetterType::OT_ClassObj) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head;
			} else if (val2->type == LetterType::OT_Generic) {
				Generic* generic = (Generic*)val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Block) {
					add_list(thgc, local->calls, (char*)KouhoSetType);
					val2 = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
					local->calls->size--;
					if (IS_WEG(val2)) return val2;
					Block* block = (Block*)val2;
					Obj* genericobj = (Obj*)make_genericobj(thgc, (Obj*)generic);
					((GenericObj*)genericobj)->gene = generic;
					for (int i = 0; i < block->rets->size; i++) {
						Obj* reti = *(Obj**)get_list(block->rets, i);
						if (isTypeObj(reti)) {
							add_list(thgc, ((GenericObj*)genericobj)->draws, (char*)reti);
						} else return ErrorA(thgc, LetterType::OT_Error, reti->letter, "ジェネリックスクラスの引数は型でなければならない");
					}
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					if (generic->vmap->kvs->size != ((GenericObj*)genericobj)->draws->size)
						return ErrorA(thgc, LetterType::OT_Error, block->letter, "ジェネリックスクラスの引数の数が違います");
					goto head;
				} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリッククラスは[]で引数をもたなければならないです");
			} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "クラスの継承元はクラスでなければなりません");
		}
	}
	if (val2->type == LetterType::OT_CallBlock) {
		(*n)++;
		return getB2(local, (CallBlock*)val2);
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "クラスの宣言の{|}が抜けています");
}

// ============================================================
// Type::PrimaryB
// ============================================================
Obj* Type_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Obj* type = self;
head:
	if (val2->type == LetterType::OT_Block) {
		Block* blk = (Block*)val2;
		if (blk->children->size == 0) {
			ArrType* arrtype = make_arrtype(thgc, type);
			((Obj*)arrtype)->letter = val2->letter;
			type = (Obj*)arrtype;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			goto head;
		} else {
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列の宣言は引数が0です。");
		}
	}
	else if (val2->type == LetterType::_Left) {
		FuncTypeC* functype = make_functype(thgc, type);
		type = (Obj*)functype;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Bracket) {
			add_list(thgc, local->calls, (char*)KouhoSetType);
			val2 = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
			local->calls->size--;
			if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
			Block* blk2 = (Block*)val2;
			for (int i = 0; i < blk2->rets->size; i++) {
				val2 = *(Obj**)get_list(blk2->rets, i);
				if (isTypeObj(val2)) {
					add_list(thgc, functype->draws, (char*)val2);
				} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "継承元はクラスでなければならない");
			}
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
		} else {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				KouhoSetType(thgc, local);
			}
			val2 = GetterB_vt(thgc, val2, local);
			if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
			if (isTypeObj(val2)) {
				add_list(thgc, functype->draws, (char*)val2);
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head;
			} else if (val2->type == LetterType::OT_Generic) {
				Generic* generic = (Generic*)val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Block) {
					add_list(thgc, local->calls, (char*)KouhoSetType);
					Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
					local->calls->size--;
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					Block* blk3 = (Block*)val;
					GenericObj* genericobj = make_genericobj(thgc, (Obj*)generic);
					for (int i = 0; i < blk3->rets->size; i++) {
						val2 = *(Obj**)get_list(blk3->rets, i);
						if (isTypeObj(val2)) {
							add_list(thgc, genericobj->draws, (char*)val2);
						} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数は型でなければいけません");
					}
					if (generic->vmap->kvs->size != genericobj->draws->size)
						return ErrorA(thgc, LetterType::OT_Error, ((Block*)val)->letter, "ジェネリックスクラスの引数の型の数が合っていません");
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					add_list(thgc, functype->draws, (char*)genericobj);
					goto head;
				} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数として[]をお願いします");
			} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "継承元はクラスでなければならない");
		}
	}
	Word* word = NULL;
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				KV* kv = (KV*)GC_alloc(thgc, CType::_KV);
				kv->key = createString(thgc, (char*)"new", 3, 1);
				kv->value = NULL;
				add_list(thgc, local->kouhos, (char*)kv);
			}
			Word* word2 = (Word*)val2;
			(*n)++;
			if (type->type == LetterType::OT_ArrType) {
				if (str_equals(word2->name, "new")) {
					Obj* ret = (Obj*)make_arrayconstructor(thgc, type);
					ret->letter = val2->letter;
					return ret;
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "は後続にこれません");
			}
			else if (type->type == LetterType::OT_ClassObj || type->type == LetterType::OT_ArrayType) {
				if (str_equals(word2->name, "new")) return (Obj*)make_constructor(thgc, type);
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "は後続にこれません");
			}
			else if (type->type == LetterType::OT_ModelObj || type->type == LetterType::OT_GenericObj) {
				ModelObj* model = (ModelObj*)type;
				if (((ClassObj*)model)->draw) {
					Block* blk0 = (Block*)*(Obj**)get_list(((ClassObj*)model)->draw->children, 0);
					if (blk0->vmapA != NULL) {
						char* v = get_mapy(blk0->vmapA, word2->name);
						if (v != NULL && ((Obj*)v)->type == LetterType::OT_Variable) {
							SqlString* ss = make_sqlstring(thgc, word2->name, val2->letter);
							ss->cls = ((Variable*)v)->cls;
							return (Obj*)ss;
						}
					}
					Block* blk1 = (Block*)*(Obj**)get_list(((ClassObj*)model)->draw->children, 1);
					if (blk1->vmapA != NULL) {
						char* v = get_mapy(blk1->vmapA, word2->name);
						if (v != NULL && ((Obj*)v)->type == LetterType::OT_Variable) {
							SqlString* ss = make_sqlstring(thgc, word2->name, val2->letter);
							ss->cls = ((Variable*)v)->cls;
							return (Obj*)ss;
						}
					}
				}
				if (str_equals(word2->name, "new")) {
					Obj* ret = (Obj*)make_constructor(thgc, type);
					ret->letter = val2->letter;
					return ret;
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "は後続にこれません");
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "関数型は.が来れません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には単語が必要です");
	}
	else if (val2->type == LetterType::OT_Dolor) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Word* word2 = (Word*)val2;
			(*n)++;
			if (local->comments->size > 0) {
				Comment* lastcomment = *(Comment**)get_list(local->comments, local->comments->size - 1);
				ATSSpan* cletter = comment_value_add(thgc, lastcomment, word2->name);
				Variable* variable = make_variable(thgc, type);
				variable->comment = cletter;
				return type;
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "コメント変数はコメントの中に宣言してください。");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "コメント変数の名前を指定してください。");
	}
	else if (val2->type == LetterType::OT_Word) {
		word = (Word*)val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		List* blocks = get_blocks(local);
		Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
		Obj* last_obj = lastblock->obj;
		if (last_obj != NULL && (last_obj->type == LetterType::OT_ModelObj || last_obj->type == LetterType::OT_GenericObj)) {
			Obj* obj2 = type;
		head2:
			if (obj2->type == LetterType::OT_ClassObj || obj2->type == LetterType::OT_FuncType)
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "model,geneの中ではmodelかgeneの型しか宣言できません");
			else if (obj2->type == LetterType::OT_ArrType) {
				obj2 = ((Type*)obj2)->cls;
				goto head2;
			}
		}
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "カラム名には名前が必要です");
		}
	}
	else if (val2->type == LetterType::OT_Bracket) {
		Block* blk = (Block*)vt_exeB(thgc, val2, local);
		(*n)++;
		for (int i = 0; i < blk->rets->size; i++) {
			val2 = *(Obj**)get_list(blk->rets, i);
			if (val2->type == LetterType::OT_Word) {
				Variable* variable = make_variable(thgc, type);
				declareB(thgc, local, ((Word*)val2)->name, (Obj*)variable);
				((Obj*)variable)->letter = val2->letter;
			}
			else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, i), "変数宣言は名前だけです");
		}
		return (Obj*)blk;
	}
	if (val2->type == LetterType::OT_Block) {
		Block* blk = (Block*)vt_exeB(thgc, vt_Clone(thgc, val2), local);
		for (int i = 0; i < blk->rets->size; i++) {
			if ((*(Obj**)get_list(blk->rets, i))->type == LetterType::OT_Word) {
				// Word* word2 = (Word*)*(Obj**)get_list(blk->rets, i);
			}
			else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, i), "ジェネリック関数の宣言は名前しか引数をとれません");
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_CallBlock) {
			Obj* func = getB2(local, (CallBlock*)val2);
			(*n)++;
			if (word != NULL) declareB(thgc, local, word->name, func);
			return func;
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリック関数の{|}が指定されていません");
	}
	else if (val2->type == LetterType::OT_CallBlock) {
		Obj* func = getB2(local, (CallBlock*)val2);
		(*n)++;
		if (word != NULL) {
			declareB(thgc, local, word->name, func);
			func->letter = word->letter;
		}
		return func;
	}
	else {
		if (word == NULL) return type;
		else {
			Variable* variable = make_variable(thgc, type);
			declareB(thgc, local, word->name, (Obj*)variable);
			((Obj*)variable)->letter = word->letter;
			return (Obj*)variable;
		}
	}
}

// ============================================================
// Var::exepB
// ============================================================
Obj* Var_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Generic::PrimaryB
// ============================================================
Obj* Generic_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Block) {
		add_list(thgc, local->calls, (char*)KouhoSetType);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* block = (Block*)val;
		Generic* generic = (Generic*)self;
		GenericObj* geneobj = make_genericobj(thgc, self);
		if (generic->block->vmapA->kvs->size != block->rets->size)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数があっていません。");
		for (int i = 0; i < block->rets->size; i++) {
			Obj* reti = *(Obj**)get_list(block->rets, i);
			if (isTypeObj(reti)) {
				add_list(thgc, geneobj->draws, (char*)reti);
			}
			else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(block->letters, i), "ジェネリックスクラスは型を引数にとる必要があります。");
		}
		// C# has: var list = new List<Obj>(this.block.vmap.Values); (unused result)
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "ジェネリッククラスは[]を引数にとります");
}

// ============================================================
// Number::exeB, exepB, PrimaryB
// ============================================================
Obj* Number_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* Number_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* Number_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				KV* kv = (KV*)GC_alloc(thgc, CType::_KV);
				kv->key = createString(thgc, (char*)"random", 6, 1);
				kv->value = NULL;
				add_list(thgc, local->kouhos, (char*)kv);
			}
			Word* word = (Word*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (str_equals(word->name, "random")) {
				if (val2->type == LetterType::OT_Bracket) {
					(*n)++;
					if (((Block*)val2)->children->size != 0)
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "randomの引数は0個です");
					Obj* ret = make_number(thgc, NULL);
					((Number*)ret)->value = 0;
					((Val*)ret)->cls = (Obj*)local->Int;
					return ret;
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "randomの()が指定されていません");
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "は適切な後続の単語ではありません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後に単語が来ていません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が来ていません");
}

// ============================================================
// Number B-pass arithmetic/comparison (obj1b.hから移動)
// ============================================================
Obj* Number_PlusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) return self;
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_number(thgc, self->letter);
		((Number*)ret)->value = ((Number*)self)->value + ((Number*)val2)->value;
		((Val*)ret)->cls = local->Int;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = (float)((Number*)self)->value + ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	else if (val2->type == LetterType::OT_StrObj) {
		StrObj* ret = make_strobj(thgc, self->letter);
		((Val*)ret)->cls = local->StrT;
		return (Obj*)ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の+は整数か小数か文字列です");
}

Obj* Number_MinusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) {
		Obj* ret = make_number(thgc, self->letter);
		((Number*)ret)->value = -((Number*)self)->value;
		((Val*)ret)->cls = local->Int;
		return ret;
	}
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_number(thgc, self->letter);
		((Number*)ret)->value = ((Number*)self)->value - ((Number*)val2)->value;
		((Val*)ret)->cls = local->Int;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = (float)((Number*)self)->value - ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の-は整数か小数です");
}

Obj* Number_MulB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_number(thgc, self->letter);
		((Number*)ret)->value = ((Number*)self)->value * ((Number*)val2)->value;
		((Val*)ret)->cls = local->Int;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = (float)((Number*)self)->value * ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の*は整数か小数です");
}

Obj* Number_DivB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_number(thgc, self->letter);
		((Number*)ret)->value = ((Number*)self)->value / ((Number*)val2)->value;
		((Val*)ret)->cls = local->Int;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = (float)((Number*)self)->value / ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の/は整数か小数です");
}

Obj* Number_MoreThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((Number*)self)->value > ((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = ((float)((Number*)self)->value > ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の>は整数か小数です");
}

Obj* Number_LessThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((Number*)self)->value < ((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = ((float)((Number*)self)->value < ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の<は整数か小数です");
}

Obj* Number_MoreEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((Number*)self)->value >= ((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = ((float)((Number*)self)->value >= ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の>=は整数か小数です");
}

Obj* Number_LessEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((Number*)self)->value <= ((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = ((float)((Number*)self)->value <= ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の<=は整数か小数です");
}

Obj* Number_NotEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((Number*)self)->value != ((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = ((float)((Number*)self)->value != ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の!=は整数か小数です");
}

Obj* Number_EqualEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((Number*)self)->value == ((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = ((float)((Number*)self)->value == ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "整数の==は整数か小数です");
}

// Number::opeB - dispatch to individual operators
Obj* Number_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if (key == LetterType::_Plus) return Number_PlusB(thgc, self, key, local, val2);
	if (key == LetterType::_Minus) return Number_MinusB(thgc, self, key, local, val2);
	if (key == LetterType::_Mul) return Number_MulB(thgc, self, key, local, val2);
	if (key == LetterType::_Div) return Number_DivB(thgc, self, key, local, val2);
	if (key == LetterType::_MoreThan) return Number_MoreThanB(thgc, self, key, local, val2);
	if (key == LetterType::_LessThan) return Number_LessThanB(thgc, self, key, local, val2);
	if (key == LetterType::_MoreEqual) return Number_MoreEqualB(thgc, self, key, local, val2);
	if (key == LetterType::_LessEqual) return Number_LessEqualB(thgc, self, key, local, val2);
	if (key == LetterType::_NotEqual) return Number_NotEqualB(thgc, self, key, local, val2);
	if (key == LetterType::_EqualEqual) return Number_EqualEqualB(thgc, self, key, local, val2);
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "整数にこの演算子は登録されていません");
}

// ============================================================
// FloatVal::exeB, exepB, PrimaryB
// ============================================================
Obj* FloatVal_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* FloatVal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* FloatVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "小数には後続が来れません");
}

// ============================================================
// FloatVal B-pass arithmetic/comparison (obj1b.hから移動)
// ============================================================
Obj* FloatVal_PlusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) return self;
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value + (float)((Number*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value + ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	else if (val2->type == LetterType::OT_StrObj) {
		StrObj* ret = make_strobj(thgc, self->letter);
		((Val*)ret)->cls = local->StrT;
		return (Obj*)ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の+は整数か小数か文字列です");
}

Obj* FloatVal_MinusB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = -((FloatVal*)self)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value - (float)((Number*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value - ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の-は整数か小数です");
}

Obj* FloatVal_MulB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value * (float)((Number*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value * ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の*は整数か小数です");
}

Obj* FloatVal_DivB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value / (float)((Number*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_floatval(thgc, self->letter);
		((FloatVal*)ret)->value = ((FloatVal*)self)->value / ((FloatVal*)val2)->value;
		((FloatVal*)ret)->cls = local->FloatT;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の/は整数か小数です");
}

Obj* FloatVal_MoreThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value > (float)((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value > ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の>は整数か小数です");
}

Obj* FloatVal_LessThanB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value < (float)((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value < ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の<は整数か小数です");
}

Obj* FloatVal_MoreEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value >= (float)((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value >= ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の>=は整数か小数です");
}

Obj* FloatVal_LessEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value <= (float)((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value <= ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の<=は整数か小数です");
}

Obj* FloatVal_NotEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value != (float)((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value != ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の!=は整数か小数です");
}

Obj* FloatVal_EqualEqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value == (float)((Number*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* ret = make_boolval(thgc, self->letter);
		ret->number_val = (((FloatVal*)self)->value == ((FloatVal*)val2)->value) ? 1 : 0;
		((Val*)ret)->cls = local->Bool;
		return ret;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "小数の==は整数か小数です");
}

// FloatVal::opeB - dispatch to individual operators
Obj* FloatVal_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if (key == LetterType::_Plus) return FloatVal_PlusB(thgc, self, key, local, val2);
	if (key == LetterType::_Minus) return FloatVal_MinusB(thgc, self, key, local, val2);
	if (key == LetterType::_Mul) return FloatVal_MulB(thgc, self, key, local, val2);
	if (key == LetterType::_Div) return FloatVal_DivB(thgc, self, key, local, val2);
	if (key == LetterType::_MoreThan) return FloatVal_MoreThanB(thgc, self, key, local, val2);
	if (key == LetterType::_LessThan) return FloatVal_LessThanB(thgc, self, key, local, val2);
	if (key == LetterType::_MoreEqual) return FloatVal_MoreEqualB(thgc, self, key, local, val2);
	if (key == LetterType::_LessEqual) return FloatVal_LessEqualB(thgc, self, key, local, val2);
	if (key == LetterType::_NotEqual) return FloatVal_NotEqualB(thgc, self, key, local, val2);
	if (key == LetterType::_EqualEqual) return FloatVal_EqualEqualB(thgc, self, key, local, val2);
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "小数にこの演算子は登録されていません");
}

// ============================================================
// SqlString::exeB, exepB, PrimaryB, opeB
// ============================================================
Obj* SqlString_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* SqlString_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* SqlString_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "SQL文字列には後続が来れません");
}
Obj* SqlString_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if (key == LetterType::_Plus || key == LetterType::_Minus || key == LetterType::_Mul ||
		key == LetterType::_Div || key == LetterType::_Equal) {
		if (((Val*)self)->cls != (Obj*)local->Int && ((Val*)self)->cls != (Obj*)local->FloatT && ((Val*)self)->cls != (Obj*)local->Short)
			return ErrorA(thgc, LetterType::OT_Error, self->letter, "SQLの演算子は整数か小数にしか作用しません。");
		Obj* ret = CheckCVB_TypeCheck(thgc, ((Val*)self)->cls, val2, CheckType_Setter, local);
		if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
		return self;
	}
	if (key == LetterType::_EqualEqual || key == LetterType::_NotEqual || key == LetterType::_MoreThan ||
		key == LetterType::_LessThan || key == LetterType::_MoreEqual || key == LetterType::_LessEqual) {
		Obj* ret = CheckCVB_TypeCheck(thgc, ((Val*)self)->cls, val2, CheckType_Setter, local);
		if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
		SqlString* ss = make_sqlstring(thgc, ((SqlString*)self)->value, self->letter);
		((Val*)ss)->cls = (Obj*)local->Bool;
		return (Obj*)ss;
	}
	if (key == LetterType::_And || key == LetterType::_Or) {
		if (((Val*)self)->cls != (Obj*)local->Bool)
			return ErrorA(thgc, LetterType::OT_Error, self->letter, "SQL文字列のand/orはbool値でなければなりません");
		Obj* ret = CheckCVB_TypeCheck(thgc, ((Val*)self)->cls, val2, CheckType_Setter, local);
		SqlString* ss = make_sqlstring(thgc, ((SqlString*)self)->value, self->letter);
		((Val*)ss)->cls = (Obj*)local->Bool;
		return (Obj*)ss;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "SQLで不適切な演算子");
}

// ============================================================
// StrObj::exeB, exepB, PrimaryB
// ============================================================
Obj* StrObj_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* StrObj_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return New_Value(thgc, (Obj*)local->StrT, local, self->letter);
}
Obj* StrObj_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "文字列には後続が来れません");
}

// ============================================================
// BoolVal::exeB, exepB, PrimaryB
// ============================================================
Obj* BoolVal_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* BoolVal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* BoolVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "bool値には後続がこれません");
}

// ============================================================
// VoiVal::exepB, PrimaryB
// ============================================================
Obj* VoiVal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "void値には後続がこれません");
}
Obj* VoiVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "void値には後続がこれません");
}

// ============================================================
// Null::exeB, exepB, PrimaryB
// ============================================================
Obj* Null_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* Null_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* Null_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "nullには後続が来ません");
}

// ============================================================
// ArrayConstructor::PrimaryB
// ============================================================
Obj* ArrayConstructor_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型のコンストラクタの引数は1です");
		(*n)++;
		if ((*(Obj**)get_list(blk->rets, 0))->type == LetterType::OT_Number) {
			Obj* value = New_Value(thgc, (Obj*)((ArrayConstructor*)self)->cls, local, self->letter);
			if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
			Block* arrblk = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
			arrblk->letter = self->letter;
			arrblk->letters = create_list(thgc, sizeof(ATSSpan*), CType::_ATSSpan);
			add_list(thgc, arrblk->rets, (char*)value);
			add_list(thgc, arrblk->letters, (char*)self->letter);
			return (Obj*)arrblk;
		}
		else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, 0), "配列型のコンストラクタの引数は1です");
	}
	Obj* val3 = val2->vt->GetterA ? val2->vt->GetterA(thgc, val2, local) : val2;
	if (val3->type == LetterType::OT_Wait || val3->type == LetterType::OT_Error || val3->type == LetterType::OT_NG) return val3;
	if (val3->type == LetterType::OT_Number) {
		Obj* value = New_Value(thgc, (Obj*)((ArrayConstructor*)self)->cls, local, self->letter);
		if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
		Block* arrblk = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
		arrblk->letter = self->letter;
		arrblk->letters = create_list(thgc, sizeof(ATSSpan*), CType::_ATSSpan);
		add_list(thgc, arrblk->rets, (char*)value);
		add_list(thgc, arrblk->letters, (char*)self->letter);
		return (Obj*)arrblk;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "コンストラクタの引数が適切ではありません");
}

// ============================================================
// Constructor::PrimaryB
// ============================================================
Obj* Constructor_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Constructor* ctor = (Constructor*)self;
	if (val2->type == LetterType::OT_Bracket) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		Block* block1 = (Block*)*(Obj**)get_list(ctor->cls->draw->children, 0);
		int varr_count = block1->vmapA->kvs->size;
		if (varr_count != blk->rets->size)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "コンストラクタの引数がありません");
		for (int i = 0; i < varr_count; i++) {
			KV* kv = *(KV**)get_list(block1->vmapA->kvs, i);
			Obj* varobj = (Obj*)kv->value;
			varobj->vt->opeB(thgc, varobj, LetterType::_Equal, local, *(Obj**)get_list(blk->rets, i));
		}
		(*n)++;
		Obj* value = New_Value(thgc, (Obj*)ctor->cls, local, self->letter);
		value->letter = self->letter;
		return value;
	}
	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSet2(thgc, local);
	}
	Obj* val0 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val0);
		if ((val0->type == LetterType::OT_Wait || val0->type == LetterType::OT_Error || val0->type == LetterType::OT_NG)) return val0;
	}
	if ((val0->type == LetterType::OT_Number || val0->type == LetterType::OT_StrObj ||
		val0->type == LetterType::OT_BoolVal || val0->type == LetterType::OT_Value)) {
		Block* block1 = (Block*)*(Obj**)get_list(((Block*)vt_Clone(thgc, (Obj*)ctor->cls->draw))->children, 0);
		int varr_count = block1->vmapA->kvs->size;
		if (varr_count != 1)
			return ErrorA(thgc, LetterType::OT_Error, val0->letter, "コンストラクタの引数が1個ではありません");
		for (int i = 0; i < varr_count; i++) {
			KV* kv = *(KV**)get_list(block1->vmapA->kvs, i);
			Obj* varobj = (Obj*)kv->value;
			varobj->vt->opeB(thgc, varobj, LetterType::_Equal, local, val0);
		}
		(*n)++;
		Obj* value = New_Value(thgc, (Obj*)ctor->cls, local, self->letter);
		value->letter = self->letter;
		return value;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "コンストラクタの引数が適切ではありません");
}

// ============================================================
// Base::PrimaryB
// ============================================================
Obj* Base_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Base* base = (Base*)self;
	int m = 0;
	if (val2->type == LetterType::OT_Block) {
		if (((Block*)val2)->children->size == 1 && (*(Obj**)get_list(((Block*)val2)->children, 0))->type == LetterType::OT_Number) {
			m = ((Number*)*(Obj**)get_list(((Block*)val2)->children, 0))->value;
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	}
	ClassObj* cls = (ClassObj*)*(Obj**)get_list(base->extends_list, m);
	if (val2->type == LetterType::OT_Bracket) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		Block* block1 = (Block*)*(Obj**)get_list(cls->draw->children, 0);
		int varr_count = block1->vmapA->kvs->size;
		if (varr_count != blk->rets->size)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "baseの引数があっていません");
		for (int i = 0; i < varr_count; i++) {
			KV* kv = *(KV**)get_list(block1->vmapA->kvs, i);
			Obj* varobj = (Obj*)kv->value;
			varobj->vt->opeB(thgc, varobj, LetterType::_Equal, local, *(Obj**)get_list(blk->rets, i));
		}
		return make_voival(thgc);
	}
	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSet2(thgc, local);
	}
	val2 = GetterB_vt(thgc, val2, local);
	if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
		val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
		Block* block1 = (Block*)*(Obj**)get_list(cls->draw->children, 0);
		int varr_count = block1->vmapA->kvs->size;
		if (varr_count != 1)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "base元のクラスの引数は1個ではありません");
		for (int i = 0; i < varr_count; i++) {
			KV* kv = *(KV**)get_list(block1->vmapA->kvs, i);
			Obj* varobj = (Obj*)kv->value;
			varobj->vt->opeB(thgc, varobj, LetterType::_Equal, local, val2);
		}
		(*n)++;
		return make_voival(thgc);
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "baseの引数が適切に設定されていません");
}

// ============================================================
// Variable::GetterB, PrimaryB
// ============================================================
Obj* Variable_GetterB(ThreadGC* thgc, Obj* self, Master* local) {
	Obj* val = New_Value(thgc, ((Variable*)self)->cls, local, self->letter);
	return GetterB_vt(thgc, val, local);
}
Obj* Variable_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Obj* val = New_Value(thgc, ((Variable*)self)->cls, local, self->letter);
	return val->vt->PrimaryB(thgc, val, n, local, primary, val2);
}

// ============================================================
// Variable B-pass methods (obj1b.hから移動)
// ============================================================
Obj* Variable_OpeB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	Variable* var = (Variable*)self;
	if (var->cls->type == LetterType::OT_Var) {
		Var* varobj = (Var*)var->cls;
		if (varobj->cls == NULL) return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
	}
	Obj* valobj = New_Value(thgc, var->cls, local, self->letter);
	if (valobj->vt && valobj->vt->opeB) {
		return valobj->vt->opeB(thgc, valobj, op, local, val2);
	}
	return valobj;
}

Obj* Variable_InB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	Variable* var = (Variable*)self;
	if (var->cls->type == LetterType::OT_Var) {
		Var* varobj = (Var*)var->cls;
		if (varobj->cls == NULL) return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
	}
head:
	if (val2->type == LetterType::OT_Bracket) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* block = (Block*)val;
		if (block->rets->size == 1) {
			val2 = *(Obj**)get_list(block->rets, 0);
			goto head;
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "()の中の数が間違っています。");
	}
	val2 = GetterB_vt(thgc, val2, local);
	if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	if (val2->type == LetterType::OT_Block) {
		Obj* it = make_iterator(thgc, self->letter);
		((Iterator*)it)->value = (Block*)val2;
		var->value = it;
		return it;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Iteratorの代入が不正です");
}

Obj* Variable_EqualB(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	Variable* var = (Variable*)self;
	if (var->value != NULL && var->value->type == LetterType::OT_Iterator)
		return ErrorA(thgc, LetterType::OT_Error, self->letter, "イテレーターは値を変更できません");
	Obj* ret = CheckB_TypeCheck(thgc, self, val2, CheckType_Setter, local);
	if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
	var->value = val2;
	if (var->comment != NULL) {
		if (val2->type == LetterType::OT_Number) {
			char buf[32]; snprintf(buf, sizeof(buf), "%d", ((Number*)val2)->value);
			var->comment->text = createString(thgc, buf, (int)strlen(buf), 1);
		}
		else if (val2->type == LetterType::OT_FloatVal) {
			char buf[64]; snprintf(buf, sizeof(buf), "%f", ((FloatVal*)val2)->value);
			var->comment->text = createString(thgc, buf, (int)strlen(buf), 1);
		}
		else if (val2->type == LetterType::OT_StrObj) {
			var->comment->text = ((StrObj*)val2)->value;
		}
	}
	return self;
}

// Variable::opeB - dispatch =, :, in, and arithmetic operators
Obj* Variable_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if (key == LetterType::_EqualEqual || key == LetterType::_NotEqual ||
		key == LetterType::_MoreEqual || key == LetterType::_LessEqual)
		return Variable_OpeB(thgc, self, key, local, val2);
	if (key == LetterType::_Equal || key == LetterType::_Colon)
		return Variable_EqualB(thgc, self, key, local, val2);
	if (key == LetterType::_In)
		return Variable_InB(thgc, self, key, local, val2);
	return Variable_OpeB(thgc, self, key, local, val2);
}

// ============================================================
// Function::PrimaryB
// ============================================================
Obj* Function_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Function* func = (Function*)self;
	if (val2->type == LetterType::OT_Bracket) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		Block* blk0 = (Block*)*(Obj**)get_list(func->draw->children, 0);
		int vs1_count = blk0->vmapA->kvs->size;
		if (vs1_count != blk->rets->size)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "関数の引数の数が合っていません");
		for (int i = 0; i < vs1_count; i++) {
			KV* kv = *(KV**)get_list(blk0->vmapA->kvs, i);
			Obj* ret = CheckB_TypeCheck(thgc, (Obj*)kv->value, *(Obj**)get_list(blk->rets, i), CheckType_Setter, local);
			if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
		}
		(*n)++;
		return New_Value(thgc, func->rettype, local, self->letter);
	}
	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSet2(thgc, local);
	}
	Obj* val0 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val0);
		if (val0 != NULL && (val0->type == LetterType::OT_Wait || val0->type == LetterType::OT_Error || val0->type == LetterType::OT_NG)) return val0;
	}
	if (val0 != NULL) {
		val0 = GetterB_vt(thgc, val0, local);
		if (val0->type == LetterType::OT_Wait || val0->type == LetterType::OT_Error || val0->type == LetterType::OT_NG) return val0;
		if (val0->type == LetterType::OT_Number || val0->type == LetterType::OT_StrObj ||
			val0->type == LetterType::OT_BoolVal || val0->type == LetterType::OT_Value) {
			Block* blk0 = (Block*)*(Obj**)get_list(func->draw->children, 0);
			int vs1_count = blk0->vmapA->kvs->size;
			if (vs1_count != 1)
				return ErrorA(thgc, LetterType::OT_Error, val0->letter, "関数の引数の数が合っていません");
			Obj* ret = CheckB_TypeCheck(thgc, (Obj*)(*(KV**)get_list(blk0->vmapA->kvs, 0))->value, val0, CheckType_Setter, local);
			if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
			(*n)++;
			return New_Value(thgc, func->rettype, local, self->letter);
		}
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "()のような関数に適切な引数が来ていません");
}

// ============================================================
// If::exepB, PrimaryB
// ============================================================
Obj* If_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* If_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Obj* ret = self->value;
	if (val2->type == LetterType::OT_CallBlock) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		IfBlock* ifb = *(IfBlock**)get_list(self->calls_list, 0);
		if (((Type*)ret)->cls == NULL)
			return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
		int m = 1;
	head:
		if (self->letter == local->letter && local->kouhos == NULL) {
			call_last(local->calls, thgc, local);
		}
		if (val2->type == LetterType::OT_Elif) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				ifb = *(IfBlock**)get_list(self->calls_list, m);
				m++;
				if (((Type*)ret)->cls == NULL)
					return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head;
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "elif文の中身が宣言されていません。");
		}
		else if (val2->type == LetterType::OT_Else) {
			Obj* elseobj = val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				ifb = *(IfBlock**)get_list(self->calls_list, m);
				if (((Type*)ret)->cls == NULL)
					return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
				(*n)++;
				return New_Value(thgc, ret, local, elseobj->letter);
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "else文の中身が宣言されていません。");
		}
		else return New_Value(thgc, ret, local, self->letter);
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "if文の中身が宣言されていません");
}

// ============================================================
// While::exepB, PrimaryB
// ============================================================
Obj* While_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* While_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (self->value == NULL) self->value = make_var(thgc, NULL);
	Obj* ret = self->value;
	if (val2->type == LetterType::OT_CallBlock) {
		if (((Type*)ret)->cls == NULL)
			return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
		(*n)++;
		return New_Value(thgc, ret, local, self->letter);
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "while文の中身が宣言されていません");
}

// ============================================================
// For::exepB, PrimaryB
// ============================================================
Obj* For_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* For_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (self->value == NULL) self->value = make_var(thgc, NULL);
	Obj* ret = self->value;
	if (val2->type == LetterType::OT_CallBlock) {
		if (((Type*)ret)->cls == NULL)
			return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
		(*n)++;
		return New_Value(thgc, ret, local, self->letter);
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "for文の中身が宣言されていません。");
}

// ============================================================
// Switch::exepB, PrimaryB
// ============================================================
Obj* Switch_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* Switch_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (self->value == NULL) self->value = make_var(thgc, NULL);
	Obj* ret = self->value;
	if (val2->type == LetterType::OT_CallBlock) {
		if (((Type*)ret)->cls == NULL)
			return ErrorA(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
		(*n)++;
		return New_Value(thgc, ret, local, self->letter);
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "switch文の中身が宣言されていません");
}

// ============================================================
// Break::exepB, PrimaryB
// ============================================================
static Obj* _breakB_search_blocks(ThreadGC* thgc, Obj* self, Master* local, Obj** val2, int* n) {
	List* blocks = get_blocks(local);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->obj != NULL) {
			if (block->obj->type == LetterType::OT_While || block->obj->type == LetterType::OT_For || block->obj->type == LetterType::OT_Switch) {
				if (block->obj_n == 0)
					return ErrorA(thgc, LetterType::OT_Error, self->letter, "while,for,switchの第1ブロックではbreakは使えません");
				if (*val2 == NULL) {
					*val2 = make_voival(thgc);
				}
				if ((*val2)->type == LetterType::OT_Number || (*val2)->type == LetterType::OT_StrObj ||
					(*val2)->type == LetterType::OT_BoolVal || (*val2)->type == LetterType::OT_Value || (*val2)->type == LetterType::OT_Block) {
					self->value = *val2;
				}
				if (!local->secondB) add_list(thgc, block->returns_list, (char*)*val2);
				if (n != NULL) (*n)--;
				return *val2;
			}
			else if (block->obj->type == LetterType::OT_Function || block->obj->type == LetterType::OT_ClassObj ||
				block->obj->type == LetterType::OT_ModelObj || block->obj->type == LetterType::OT_GenericObj ||
				block->obj->type == LetterType::OT_GenericFunction) {
				return ErrorA(thgc, LetterType::OT_Error, self->letter, "while,for,switchの中ではありません");
			}
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "while,for,switchの中ではありません");
}

Obj* Break_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) val2 = GetterB_vt(thgc, val2, local);
	return _breakB_search_blocks(thgc, self, local, &val2, n);
}
Obj* Break_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	val2 = NULL;
	(*n)++;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) val2 = GetterB_vt(thgc, val2, local);
	return _breakB_search_blocks(thgc, self, local, &val2, NULL);
}

// ============================================================
// Continue::exepB, PrimaryB
// ============================================================
static Obj* _continueB_search_blocks(ThreadGC* thgc, Obj* self, Master* local, Obj** val2, int* n) {
	List* blocks = get_blocks(local);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->obj != NULL) {
			if (block->obj->type == LetterType::OT_While || block->obj->type == LetterType::OT_For) {
				if (block->obj_n == 0)
					return ErrorA(thgc, LetterType::OT_Error, self->letter, "while,forの第1ブロックではcontinueは使えません");
				if (*val2 == NULL) {
					*val2 = make_voival(thgc);
				}
				if ((*val2)->type == LetterType::OT_Number || (*val2)->type == LetterType::OT_StrObj ||
					(*val2)->type == LetterType::OT_BoolVal || (*val2)->type == LetterType::OT_Value || (*val2)->type == LetterType::OT_Block) {
					self->value = *val2;
				}
				if (!local->secondB) add_list(thgc, block->returns_list, (char*)*val2);
				if (n != NULL) (*n)--;
				return (n != NULL) ? self : *val2;
			}
			else if (block->obj->type == LetterType::OT_Function || block->obj->type == LetterType::OT_ClassObj ||
				block->obj->type == LetterType::OT_ModelObj || block->obj->type == LetterType::OT_GenericObj ||
				block->obj->type == LetterType::OT_GenericFunction) {
				return ErrorA(thgc, LetterType::OT_Error, self->letter, "while,forの中ではありません");
			}
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "while,forの中ではありません");
}

Obj* Continue_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val2);
		if (val2 != NULL && IS_WEG(val2)) return val2;
	}
	return _continueB_search_blocks(thgc, self, local, &val2, n);
}
Obj* Continue_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	val2 = NULL;
	(*n)++;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val2);
		if (*n == primary->children->size - 1)
		if (val2 != NULL && IS_WEG(val2)) return val2;
	}
	if (val2 != NULL) val2 = GetterB_vt(thgc, val2, local);
	return _continueB_search_blocks(thgc, self, local, &val2, NULL);
}

// ============================================================
// Return::exepB, PrimaryB
// ============================================================
static Obj* _returnB_search_blocks(ThreadGC* thgc, Obj* self, Master* local, Obj** val2, int* n) {
	List* blocks = get_blocks(local);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->obj != NULL) {
			if (block->obj->type == LetterType::OT_Function || block->obj->type == LetterType::OT_GenericFunction ||
				block->obj->type == LetterType::OT_ServerFunction || block->obj->type == LetterType::OT_SignalFunction) {
				if (block->obj_n == 0)
					return ErrorA(thgc, LetterType::OT_Error, self->letter, "関数の第1ブロックではreturnは使えません");
				if (*val2 == NULL) {
					*val2 = make_voival(thgc);
				}
				if ((*val2)->type == LetterType::OT_Number || (*val2)->type == LetterType::OT_StrObj ||
					(*val2)->type == LetterType::OT_BoolVal || (*val2)->type == LetterType::OT_Value || (*val2)->type == LetterType::OT_Block) {
					self->value = *val2;
				}
				if (!local->secondB) add_list(thgc, block->returns_list, (char*)*val2);
				if (n != NULL) (*n)--;
				return *val2;
			}
			else if (block->obj->type == LetterType::OT_ClassObj) {
				return ErrorA(thgc, LetterType::OT_Error, self->letter, "関数の中ではありません");
			}
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "return文は関数の中でしか使えません");
}

Obj* Return_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val2);
		if (val2 != NULL && IS_WEG(val2)) return val2;
	}
	if (val2 != NULL) val2 = GetterB_vt(thgc, val2, local);
	return _returnB_search_blocks(thgc, self, local, &val2, n);
}
Obj* Return_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	val2 = NULL;
	(*n)++;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val2);
		if (val2 != NULL && IS_WEG(val2)) return val2;
	}
	if (val2 != NULL) val2 = GetterB_vt(thgc, val2, local);
	return _returnB_search_blocks(thgc, self, local, &val2, NULL);
}

// ============================================================
// Goto::exepB, PrimaryB
// ============================================================
Obj* Goto_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* Goto_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSetLabel(thgc, local);
	}
	Goto* gt = (Goto*)self;
	if (val2->type == LetterType::OT_Number) {
		gt->value = NumberString(thgc, ((Number*)val2)->value);
		return self;
	}
	else if (val2->type == LetterType::OT_StrObj) {
		gt->value = ((StrObj*)val2)->value;
		return self;
	}
	else if (val2->type == LetterType::OT_Word) {
		gt->value = ((Word*)val2)->name;
		return self;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "goto文の行き先を指定してください。");
}

// ============================================================
// Print::exepB, PrimaryB
// ============================================================
Obj* Print_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* Print_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket || val2->type == LetterType::OT_Block) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		remove_list(thgc, local->calls, local->calls->size - 1);
		if (IS_WEG(val)) return val;
		Block* block = (Block*)val;
		if (val2->type == LetterType::OT_Bracket) printf("(");
		else printf("[");
		for (int i = 0; i < block->rets->size; i++) {
			if (i != 0) printf(",");
			Obj* val3 = *(Obj**)get_list(block->rets, i);
			if (val3->type == LetterType::OT_Number) {
				printf("%d", ((Number*)val3)->value);
			}
			else if (val3->type == LetterType::OT_StrObj) {
				printf("%.*s", ((StrObj*)val3)->value->size, ((StrObj*)val3)->value->data);
			}
			else return ErrorA(thgc, LetterType::OT_Error, val3->letter, "printできない変数です");
		}
		if (val2->type == LetterType::OT_Bracket) printf(")\n");
		else printf("]\n");
		(*n)++;
		return make_voival(thgc);
	}
	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSet2(thgc, local);
	}
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextB(thgc, primary, local, n, &val2);
		if (val2 != NULL && IS_WEG(val2)) return val2;
	}
	if (val2 != NULL) {
		val2 = GetterB_vt(thgc, val2, local);
		if (IS_WEG(val2)) return val2;
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
			val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
			(*n)++;
			return make_voival(thgc);
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "()でprintの引数を指定してください");
}

// ============================================================
// Iterator::GetterB
// ============================================================
Obj* Iterator_GetterB(ThreadGC* thgc, Obj* self, Master* local) {
	Iterator* it = (Iterator*)self;
	if (it->m == -1) return *(Obj**)get_list(it->value->rets, it->n);
	else return *(Obj**)get_list(((Block*)*(Obj**)get_list(it->value->rets, it->n))->rets, it->m);
}

// ============================================================
// GenericFunction::PrimaryB
// ============================================================
Obj* GenericFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Block) {
		add_list(thgc, local->calls, (char*)KouhoSetType);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		remove_list(thgc, local->calls, local->calls->size - 1);
		if (IS_WEG(val)) return val;
		Block* blk = (Block*)val;
		GenericFunction* gf = (GenericFunction*)self;
		if (blk->rets->size != gf->vmap->kvs->size) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "引数の数があっていません。");
		for (int i = 0; i < blk->rets->size; i++) {
			if (!isTypeObj(*(Obj**)get_list(blk->rets, i)))
				return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, i), "ジェネリック関数には型を引数にしてください");
		}
		(*n)++;
		Obj* ch = *(Obj**)get_list(primary->children, *n);
		Function* func = make_function(thgc, ((Function*)self)->rettype);
		func->blocks = ((Function*)self)->blocks;
		func->draw = ((Function*)self)->draw;
		func->bracket = ((Function*)self)->bracket;
		func->block = blk;
		return (Obj*)func;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "[]でジェリック関数の引数を指定してください");
}

// ============================================================
// Model::exepB, PrimaryB
// ============================================================
Obj* Model_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* Model_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Block) {
		val2 = (Obj*)GetterB_vt(thgc, (Obj*)Block_exeRangeB(thgc, (Block*)val2, 0, val2->children->size, local, true, false), local);
		if (IS_WEG(val2)) return val2;
		Block* blk0 = (Block*)val2;
		if (blk0->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Modelブロックの引数は1つです");
		Obj* val = *(Obj**)get_list(blk0->rets, 0);
		if (val->type != LetterType::OT_ModelObj && val->type != LetterType::OT_GenericObj)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Modelブロックの引数はModelクラスかGeneクラスです");
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	head:
		if (val2->type == LetterType::_Dot) {
			ModelObj* modelobj = (ModelObj*)val;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				if (val2->letter == local->letter && local->kouhos == NULL) {
					local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
					Obj* none = make_cobj(thgc, CType::_CObj, LetterType::_None, NULL);
					add_kouho(thgc, local->kouhos, createString(thgc, (char*)"Where", 5, 1), none);
					add_kouho(thgc, local->kouhos, createString(thgc, (char*)"FirstAwait", 10, 1), none);
					add_kouho(thgc, local->kouhos, createString(thgc, (char*)"Select", 6, 1), none);
					add_kouho(thgc, local->kouhos, createString(thgc, (char*)"Sort", 4, 1), none);
				}
				Word* word2 = (Word*)val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (str_equals(word2->name, "Where")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++;
						val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
						if (IS_WEG(val)) return val;
						Block* blk = (Block*)val;
						if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Whereの引数は1です");
						Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
						if (ret0->type == LetterType::OT_SqlString) {
							goto head;
						}
						else if (ret0->type == LetterType::OT_Value) {
							Value* value = (Value*)ret0;
							if (value->cls == (Obj*)local->StrT) {
								goto head;
							}
							else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Whereの引数はString型かSqlString型です");
						}
						else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Whereの引数はString型かSqlString型です");
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Whereの()が指定されていません");
				}
				else if (str_equals(word2->name, "FirstAwait")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++;
						val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
						if (IS_WEG(val)) return val;
						Block* blk = (Block*)val;
						if (blk->rets->size != 0) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Firstの引数は0です");
						return New_Value(thgc, (Obj*)modelobj, local, word2->letter);
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Firstの()が指定されていません");
				}
				else if (str_equals(word2->name, "Select")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++;
						val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
						if (IS_WEG(val)) return val;
						Block* blk = (Block*)val;
						if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Selectの引数は1です");
						Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
						if (ret0->type != LetterType::OT_Function) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Selectの引数は関数です");
						return Stock_SelectB(thgc, (Obj*)local->db, (Obj*)modelobj, local, (Obj*)(Function*)ret0);
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Selectの()が指定されていません");
				}
				else if (str_equals(word2->name, "Sort")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++;
						val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
						if (IS_WEG(val)) return val;
						Block* blk = (Block*)val;
						if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Sortの引数は1です");
						Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
						if (ret0->type != LetterType::OT_Function) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Sortの引数は関数です");
						FuncTypeC* functype = make_functype(thgc, (Obj*)local->Bool);
						functype->nochange = true;
						add_list(thgc, functype->draws, (char*)modelobj);
						add_list(thgc, functype->draws, (char*)modelobj);
						Obj* ret = CheckCVB_TypeCheck(thgc, (Obj*)functype, ret0, CheckType_Setter, local);
						if (IS_WEG(ret)) return ret;
						Obj* value = New_Value(thgc, (Obj*)modelobj, local, word2->letter);
						if (IS_WEG(value)) return value;
						Block* arr = (Block*)GC_alloc(thgc, CType::_Block);
						arr->type = LetterType::OT_Array;
						arr->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
						add_list(thgc, arr->rets, (char*)value);
						arr->letter = word2->letter;
						return (Obj*)Block_Sort(thgc, arr, (Function*)ret0, local);
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Sortの()が指定されていません");
				}
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が来ます");
		}
		else return make_voival(thgc);
	}
	else if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				KouhoSetModelGene(thgc, local);
				Obj* none = make_cobj(thgc, CType::_CObj, LetterType::_None, NULL);
				add_kouho(thgc, local->kouhos, createString(thgc, (char*)"Store", 5, 1), none);
			}
			Word* word = (Word*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (str_equals(word->name, "Store")) {
				if (val2->type == LetterType::_Dot) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					if (val2->type == LetterType::OT_Word) {
						(*n)++;
						Word* word2 = (Word*)val2;
						val2 = *(Obj**)get_list(primary->children, *n);
						if (!str_equals(word2->name, "await")) return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後にはawaitが来ます");
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後にはawaitが来ます");
				}
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
					if (IS_WEG(val)) return val;
					Block* blk = (Block*)val;
					for (int i = 0; i < blk->rets->size; i++) {
						Obj* ri = *(Obj**)get_list(blk->rets, i);
						if (IS_WEG(ri)) return *(Obj**)get_list(blk->rets, 0);
						if (ri->type != LetterType::OT_Number && ri->type != LetterType::OT_FloatVal &&
							ri->type != LetterType::OT_StrObj && ri->type != LetterType::OT_BoolVal &&
							ri->type != LetterType::OT_Value && ri->type != LetterType::OT_ModelVal &&
							ri->type != LetterType::OT_SqlString && ri->type != LetterType::OT_GeneVal)
							return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, i), "Storeの引数には値を指定してください");
						Stock_StoreB(thgc, (Obj*)local->db, ri, local);
					}
					(*n)++;
					return make_voival(thgc);
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Storeの後に()がありません");
			}
			else if (str_equals(word->name, "await")) {
				if (val2->type == LetterType::OT_CallBlock) {
					(*n)++;
					return make_voival(thgc);
				}
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "modelにはその名前は宣言されていません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の次には名前が来ます");
	}
	else if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		ModelObj* gj = (ModelObj*)getB(thgc, local, word->name, word->letter);
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				((Obj*)gj)->rename = ((Word*)val2)->name;
				if (val2->type == LetterType::OT_Block) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					Block* blk = (Block*)val2;
					if (blk->children->size == 1) {
						Obj* ch0 = *(Obj**)get_list(blk->children, 0);
						if (ch0->type == LetterType::OT_FloatVal) {
							((Obj*)gj)->version = ch0->letter->text;
						}
						else if (ch0->type == LetterType::OT_Number) {
							((Obj*)gj)->version = ch0->letter->text;
						}
						else return ErrorA(thgc, LetterType::OT_Error, ch0->letter, "versionの引数は小数か整数だけです");
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "versionの引数は小数か整数だけです");
				}
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "テーブルを宣言してください。");
		}
		if (val2->type == LetterType::OT_CallBlock) {
			(*n)++;
			return make_voival(thgc);
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "modelの内容を{}で宣言してください。");
	}
	else if (val2->type == LetterType::OT_CallBlock) {
		(*n)++;
		return make_voival(thgc);
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "model名を宣言してください。");
}

// ============================================================
// ModelObj::exeB
// ============================================================
Obj* ModelObj_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }

// ============================================================
// ModelVal::PrimaryB
// ============================================================
Obj* ModelVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				Map* vmap = ((Val*)self)->vmap;
				if (vmap != NULL) {
					for (int i = 0; i < vmap->kvs->size; i++) {
						KV* kv = (KV*)get_list(vmap->kvs, i);
						add_kouho(thgc, local->kouhos, kv->key, (Obj*)kv->value);
					}
				}
				add_kouho(thgc, local->kouhos, createString(thgc, (char*)"update", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::_None, NULL));
			}
			Word* word = (Word*)val2;
			(*n)++;
			if (str_equals(word->name, "update")) {
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
					if (IS_WEG(val)) return val;
					Block* block = (Block*)val;
					(*n)++;
					if (block->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "updateの引数は1です");
					Obj* last = *(Obj**)get_list(block->rets, 0);
					if (last->type == LetterType::OT_Stock) {
						return self;
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "updateの引数はDataStockを取ります");
				}
				else if (val2->type == LetterType::OT_Stock) {
					return self;
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "updateの引数はDataStockを取ります");
			}
			else {
				Map* vmap = ((Val*)self)->vmap;
				if (vmap != NULL) {
					char* found = get_mapy(vmap, word->name);
					if (found != NULL) return (Obj*)found;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "この変数にはその名前は宣言されていません");
			}
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の次は名前です");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "次は.ではありませんか");
}

// ============================================================
// Stock::SelectB (obj1b.hから移動)
// ============================================================
Obj* Stock_SelectB(ThreadGC* thgc, Obj* self, Obj* model, Master* local, Obj* func) {
	Function* f = (Function*)func;
	Block* blk = (Block*)*(Obj**)get_list(f->draw->children, 0);
	if (blk->vmapA->kvs->size != 1) return ErrorA(thgc, LetterType::OT_Error, blk->letter, "Selectの引数は1つです");
	FuncTypeC* ftype = make_functype(thgc, (Obj*)local->Bool);
	add_list(thgc, ftype->draws, (char*)model);
	Obj* ret = CheckCVB_TypeCheck(thgc, (Obj*)ftype, func, CheckType_Setter, local);
	if (IS_WEG(ret)) return ret;
	Block* result = make_block(thgc, LetterType::OT_Array);
	result->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	add_list(thgc, result->rets, (char*)model);
	return (Obj*)result;
}

// ============================================================
// Stock::StoreB (obj1b.hから移動)
// ============================================================
Obj* Stock_StoreB(ThreadGC* thgc, Obj* self, Obj* val, Master* local) {
	return make_voival(thgc);
}

// ============================================================
// Gene::exepB, PrimaryB
// ============================================================
Obj* Gene_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}
Obj* Gene_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		GeneObj* gj = (GeneObj*)getB(thgc, local, word->name, word->letter);
		((Obj*)gj)->letter = self->letter;
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				((Obj*)gj)->rename = ((Word*)val2)->name;
				if (val2->type == LetterType::OT_Block) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					Block* blk = (Block*)val2;
					if (blk->children->size == 1) {
						Obj* ch0 = *(Obj**)get_list(blk->children, 0);
						if (ch0->type == LetterType::OT_FloatVal) {
							((Obj*)gj)->version = ch0->letter->text;
						}
						else if (ch0->type == LetterType::OT_Number) {
							((Obj*)gj)->version = ch0->letter->text;
						}
						else return ErrorA(thgc, LetterType::OT_Error, ch0->letter, "versionの引数は小数か整数だけです");
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "versionの引数は小数か整数だけです");
				}
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "テーブルを宣言してください。");
			if (val2->type == LetterType::OT_CallBlock) {
				Block* blk1 = (Block*)*(Obj**)get_list(val2->children, 1);
				((Type*)gj)->letter2 = blk1->letter2;
				gj->call_blk = (CallBlock*)val2;
				Block* blk1b = (Block*)*(Obj**)get_list(gj->call_blk->children, 1);
				blk1b->obj = (Obj*)make_objblock(thgc, (Obj*)gj, 1);
				List* blocks = get_blocks(local);
				for (int i = 0; i < blocks->size; i++) {
					add_list(thgc, gj->geneblocks, *(char**)get_list(blocks, i));
				}
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
			}
			return (Obj*)gj;
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "{|}を宣言してください。");
	}
	else if (val2->type == LetterType::OT_CallBlock) {
		String* emptyKey = createString(thgc, (char*)"", 0, 1);
		GeneObj* gj = (GeneObj*)get_mapy(local->gene->vmap, emptyKey);
		gj->call_blk = (CallBlock*)val2;
		Block* blk1 = (Block*)*(Obj**)get_list(gj->call_blk->children, 1);
		blk1->obj = (Obj*)make_objblock(thgc, (Obj*)gj, 1);
		List* blocks = get_blocks(local);
		for (int i = 0; i < blocks->size; i++) {
			add_list(thgc, gj->geneblocks, *(char**)get_list(blocks, i));
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	head:
		if (val2->type == LetterType::_Left) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				gj->left = (CallBlock*)val2;
				(*n)++;
				goto head;
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
		}
		else if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				gj->right = (CallBlock*)val2;
				(*n)++;
				goto head;
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
		}
		return (Obj*)gj;
	}
	else if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Word* word = (Word*)val2;
			(*n)++;
			char* found = get_mapy(local->gene->vmap, word->name);
			if (found != NULL) {
				return (Obj*)found;
			}
			String* emptyKey = createString(thgc, (char*)"", 0, 1);
			char* found2 = get_mapy(local->gene->vmap, emptyKey);
			if (found2 != NULL) {
				GeneObj* gj = (GeneObj*)found2;
				return GeneObj_DotB(thgc, (Obj*)gj, word->name);
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "無名Geneクラスは宣言されていません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が必要です");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "geneの後に適切な単語が来ていません");
}

// ============================================================
// GeneChild, GeneMutate, GeneNew, GeneCross, GeneVal,
// GeneStore, GeneSelect, GeneSort PrimaryB
// ============================================================
Obj* GeneChild_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return New_Value(thgc, (Obj*)((GeneChild*)self)->gj, local, self->letter);
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Childの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}
Obj* GeneMutate_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return New_Value(thgc, (Obj*)((GeneMutate*)self)->gj, local, self->letter);
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Mutateの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}
Obj* GeneNew_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return New_Value(thgc, (Obj*)((GeneNew*)self)->gj, local, self->letter);
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Newの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}
Obj* GeneCross_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 2) {
			Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
			if (IS_WEG(val)) return val;
			(*n)++;
			return New_Value(thgc, (Obj*)((GeneCross*)self)->gj, local, self->letter);
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Crossの引数は2です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}
Obj* GeneVal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Word* word = (Word*)val2;
			(*n)++;
			GeneObj* gobj = (GeneObj*)((Val*)self)->cls;
			Map* vmap = ((Block*)*(Obj**)get_list(gobj->call_blk->children, 1))->vmapA;
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				if (vmap != NULL) {
					for (int i = 0; i < vmap->kvs->size; i++) {
						KV* kv = (KV*)get_list(vmap->kvs, i);
						add_kouho(thgc, local->kouhos, kv->key, (Obj*)kv->value);
					}
				}
			}
			char* found = get_mapy(vmap, word->name);
			if (found != NULL) return (Obj*)found;
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "その名前は宣言されていません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後は名前でないといけません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "変数の後には.ではありませんか");
}
Obj* GeneStore_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		if (val2->children->size == 0) {
			Obj* value = New_Value(thgc, (Obj*)((GeneStore*)self)->gj, local, self->letter);
			if (IS_WEG(value)) return value;
			Stock_StoreB(thgc, (Obj*)local->db, value, local);
			return value;
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Storeの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}
Obj* GeneSelect_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		GeneObj* gj = ((GeneSelect*)self)->gj;
		if (val2->children->size == 0) {
			Obj* value = New_Value(thgc, (Obj*)gj, local, self->letter);
			if (IS_WEG(value)) return value;
			(*n)++;
			Block* result = make_block(thgc, LetterType::OT_Block);
			result->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
			add_list(thgc, result->rets, (char*)value);
			return (Obj*)result;
		}
		else if (val2->children->size == 1) {
			Obj* value = New_Value(thgc, (Obj*)gj, local, self->letter);
			if (IS_WEG(value)) return value;
			add_list(thgc, local->calls, (char*)KouhoSet2);
			Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
			local->calls->size--;
			if (IS_WEG(val)) return val;
			Block* blk = (Block*)val;
			Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
			if (ret0->type == LetterType::OT_Function) {
				Stock_SelectB(thgc, (Obj*)local->db, (Obj*)gj, local, ret0);
				(*n)++;
				Block* result = make_block(thgc, LetterType::OT_Array);
				result->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
				add_list(thgc, result->rets, (char*)value);
				return (Obj*)result;
			}
			else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, 0), "Selectは関数を引数にとります");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Selectの引数があっていません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}
Obj* GeneSort_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		GeneObj* gj = ((GeneSort*)self)->gj;
		if (val2->children->size == 0) {
			Obj* value = New_Value(thgc, (Obj*)gj, local, self->letter);
			if (IS_WEG(value)) return value;
			(*n)++;
			Block* result = make_block(thgc, LetterType::OT_Block);
			result->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
			add_list(thgc, result->rets, (char*)value);
			return (Obj*)result;
		}
		else if (val2->children->size == 1) {
			add_list(thgc, local->calls, (char*)KouhoSet2);
			Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
			local->calls->size--;
			if (IS_WEG(val)) return val;
			Block* blk = (Block*)val;
			(*n)++;
			Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
			if (ret0->type == LetterType::OT_Function) {
				Obj* value = New_Value(thgc, (Obj*)gj, local, self->letter);
				if (IS_WEG(value)) return value;
				Block* arr = make_block(thgc, LetterType::OT_Array);
				arr->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
				add_list(thgc, arr->rets, (char*)value);
				return (Obj*)Block_Sort(thgc, arr, (Function*)ret0, local);
			}
			else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, 0), "Sortは関数を引数にとります");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Sortの引数があっていません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneLabel::exepB, GeneLabelValue::PrimaryB
// ============================================================
Obj* GeneLabel_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* GeneLabelValue_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				add_kouho(thgc, local->kouhos, createString(thgc, (char*)"length", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::_None, NULL));
			}
			Word* word = (Word*)val2;
			(*n)++;
			if (str_equals(word->name, "length")) {
				GeneLabelValue* glv = (GeneLabelValue*)self;
				GeneLabel* id = glv->id;
				int count = id->children ? id->children->size : 0;
				Obj* num = make_number(thgc, NULL);
				((Number*)num)->value = count;
				((Val*)num)->cls = (Obj*)local->Int;
				return num;
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "このGeneLabelにはその名前は宣言されていません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後は名前でないといけません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "変数の後には.ではありませんか");
}

// ============================================================
// NewBlock, CrossBlock, MigrateBlock, GeneObj exeB
// ============================================================
Obj* NewBlock_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	Obj* num = make_number(thgc, NULL);
	((Number*)num)->value = 0;
	add_list(thgc, blk->rets, (char*)num);
	return self;
}
Obj* CrossBlock_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	CrossBlock* cb = (CrossBlock*)self;
	return New_Value(thgc, (Obj*)cb->gj, local, self->letter);
}
Obj* MigrateBlock_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	Obj* fv = make_floatval(thgc, NULL);
	((FloatVal*)fv)->value = 0.1f;
	((Val*)fv)->cls = (Obj*)local->FloatT;
	add_list(thgc, blk->rets, (char*)fv);
	return self;
}

// ============================================================
// GeneObj::DotB (obj1b.hから移動)
// ============================================================
Obj* GeneObj_DotB(ThreadGC* thgc, Obj* self, String* name) {
	GeneObj* gj = (GeneObj*)self;
	char* found = get_mapy(gj->vmap, name);
	if (found != NULL) return (Obj*)found;
	if (gj->gv != NULL) {
		GeneObj* gv = (GeneObj*)gj->gv;
		char* found2 = get_mapy(gv->vmap, name);
		if (found2 != NULL) return (Obj*)found2;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "GeneObjにこの名前はありません");
}

Obj* GeneObj_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }

// ============================================================
// B2_initVTables - B-pass vtable初期化 (Block 2)
// ============================================================
void B2_initVTables() {
	// --- Word ---
	vt_Word.SelfB = Word_SelfB;
	vt_Word.GetterB = Word_GetterB;
	vt_Word.exepB = Word_exepB;
	vt_Word.PrimaryB = Word_PrimaryB;

	// --- Number ---
	vt_Number.exeB = Number_exeB;
	vt_Number.exepB = Number_exepB;
	vt_Number.PrimaryB = Number_PrimaryB;
	vt_Number.opeB = Number_opeB;

	// --- FloatVal ---
	vt_FloatVal.exeB = FloatVal_exeB;
	vt_FloatVal.exepB = FloatVal_exepB;
	vt_FloatVal.PrimaryB = FloatVal_PrimaryB;
	vt_FloatVal.opeB = FloatVal_opeB;

	// --- SqlString ---
	vt_SqlString.exeB = SqlString_exeB;
	vt_SqlString.exepB = SqlString_exepB;
	vt_SqlString.PrimaryB = SqlString_PrimaryB;
	vt_SqlString.opeB = SqlString_opeB;

	// --- StrObj ---
	vt_StrObj.exeB = StrObj_exeB;
	vt_StrObj.exepB = StrObj_exepB;
	vt_StrObj.PrimaryB = StrObj_PrimaryB;

	// --- BoolVal ---
	vt_BoolVal.exeB = BoolVal_exeB;
	vt_BoolVal.exepB = BoolVal_exepB;
	vt_BoolVal.PrimaryB = BoolVal_PrimaryB;

	// --- VoiVal ---
	vt_VoiVal.exepB = VoiVal_exepB;
	vt_VoiVal.PrimaryB = VoiVal_PrimaryB;

	// --- Value ---
	vt_Value.PrimaryB = Value_PrimaryB;

	// --- Null ---
	vt_Null.exeB = Null_exeB;
	vt_Null.exepB = Null_exepB;
	vt_Null.PrimaryB = Null_PrimaryB;

	// --- Class ---
	vt_Class.exepB = Class_exepB;
	vt_Class.PrimaryB = Class_PrimaryB;

	// --- Type ---
	vt_Type.PrimaryB = Type_PrimaryB;
	// C# ClassObj extends Type — inherits Type.PrimaryB
	vt_ClassObj.PrimaryB = Type_PrimaryB;
	// C# ArrType/FuncType extend Type — inherit Type.PrimaryB
	vt_ArrType.PrimaryB = Type_PrimaryB;
	vt_FuncType.PrimaryB = Type_PrimaryB;

	// --- Var (inherits Type) ---
	vt_Var.exepB = Var_exepB;
	vt_Var.PrimaryB = Type_PrimaryB;

	// --- Generic ---
	vt_Generic.PrimaryB = Generic_PrimaryB;

	// --- Variable ---
	vt_Variable.GetterB = Variable_GetterB;
	vt_Variable.PrimaryB = Variable_PrimaryB;
	vt_Variable.opeB = Variable_opeB;

	// --- Function ---
	vt_Function.PrimaryB = Function_PrimaryB;

	// --- GenericFunction ---
	vt_GenericFunction.PrimaryB = GenericFunction_PrimaryB;

	// --- ArrayConstructor ---
	vt_ArrayConstructor.PrimaryB = ArrayConstructor_PrimaryB;

	// --- Constructor ---
	vt_Constructor.PrimaryB = Constructor_PrimaryB;

	// --- Base ---
	vt_Base.PrimaryB = Base_PrimaryB;

	// --- If ---
	vt_If.exepB = If_exepB;
	vt_If.PrimaryB = If_PrimaryB;

	// --- While ---
	vt_While.exepB = While_exepB;
	vt_While.PrimaryB = While_PrimaryB;

	// --- For ---
	vt_For.exepB = For_exepB;
	vt_For.PrimaryB = For_PrimaryB;

	// --- Switch ---
	vt_Switch.exepB = Switch_exepB;
	vt_Switch.PrimaryB = Switch_PrimaryB;

	// --- Break ---
	vt_Break.exepB = Break_exepB;
	vt_Break.PrimaryB = Break_PrimaryB;

	// --- Continue ---
	vt_Continue.exepB = Continue_exepB;
	vt_Continue.PrimaryB = Continue_PrimaryB;

	// --- Return ---
	vt_Return.exepB = Return_exepB;
	vt_Return.PrimaryB = Return_PrimaryB;

	// --- Goto ---
	vt_Goto.exepB = Goto_exepB;
	vt_Goto.PrimaryB = Goto_PrimaryB;

	// --- Print ---
	vt_Print.exepB = Print_exepB;
	vt_Print.PrimaryB = Print_PrimaryB;

	// --- Model ---
	vt_Model.exepB = Model_exepB;
	vt_Model.PrimaryB = Model_PrimaryB;

	// --- ModelObj ---
	// C# ModelObj extends ClassObj extends Type — inherits Type.PrimaryB
	vt_ModelObj.PrimaryB = Type_PrimaryB;
	vt_ModelObj.exeB = ModelObj_exeB;

	// --- ModelVal ---
	vt_ModelVal.PrimaryB = ModelVal_PrimaryB;

	// --- Gene ---
	vt_Gene.exepB = Gene_exepB;
	vt_Gene.PrimaryB = Gene_PrimaryB;

	// --- GeneObj ---
	// C# GeneObj extends ModelObj extends ClassObj extends Type — inherits Type.PrimaryB
	vt_GeneObj.PrimaryB = Type_PrimaryB;
	vt_GeneObj.exeB = GeneObj_exeB;

	// --- GeneChild ---
	vt_GeneChild.PrimaryB = GeneChild_PrimaryB;

	// --- GeneMutate ---
	vt_GeneMutate.PrimaryB = GeneMutate_PrimaryB;

	// --- GeneNew ---
	vt_GeneNew.PrimaryB = GeneNew_PrimaryB;

	// --- GeneCross ---
	vt_GeneCross.PrimaryB = GeneCross_PrimaryB;

	// --- GeneVal ---
	vt_GeneVal.PrimaryB = GeneVal_PrimaryB;

	// --- GeneStore ---
	vt_GeneStore.PrimaryB = GeneStore_PrimaryB;

	// --- GeneSelect ---
	vt_GeneSelect.PrimaryB = GeneSelect_PrimaryB;

	// --- GeneSort ---
	vt_GeneSort.PrimaryB = GeneSort_PrimaryB;

	// --- GeneLabel ---
	// C# GeneLabel extends Type — inherits Type.PrimaryB
	vt_GeneLabel.PrimaryB = Type_PrimaryB;
	vt_GeneLabel.exepB = GeneLabel_exepB;

	// --- GeneLabelValue ---
	vt_GeneLabelValue.PrimaryB = GeneLabelValue_PrimaryB;

	// --- Iterator ---
	vt_Iterator.GetterB = Iterator_GetterB;

	// --- NewBlock (inherits Block slots except exeB) ---
	vt_NewBlock.exeB = NewBlock_exeB;
	vt_NewBlock.exepB = Block_exepB;
	vt_NewBlock.GetterB = Block_GetterB;
	vt_NewBlock.SelfB = Block_SelfB;
	vt_NewBlock.PrimaryB = Block_PrimaryB;

	// --- CrossBlock (inherits Block slots except exeB) ---
	vt_CrossBlock.exeB = CrossBlock_exeB;
	vt_CrossBlock.exepB = Block_exepB;
	vt_CrossBlock.GetterB = Block_GetterB;
	vt_CrossBlock.SelfB = Block_SelfB;
	vt_CrossBlock.PrimaryB = Block_PrimaryB;

	// --- MigrateBlock (inherits Block slots except exeB) ---
	vt_MigrateBlock.exeB = MigrateBlock_exeB;
	vt_MigrateBlock.exepB = Block_exepB;
	vt_MigrateBlock.GetterB = Block_GetterB;
	vt_MigrateBlock.SelfB = Block_SelfB;
	vt_MigrateBlock.PrimaryB = Block_PrimaryB;
}

