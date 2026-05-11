// ============================================================
// obj2a3.h - A解析パス (vtable dispatch版) 第3ブロック
// Value, Class, Type, If, While, For, Gene, Model 等
// ============================================================

// ============================================================
// 前方宣言
// ============================================================
Obj* Word_SelfA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Word_GetterA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Word_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Word_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Value_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Class_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Class_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Type_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Var_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Generic_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Number_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Number_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Number_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* FloatVal_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* FloatVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* SqlString_opeA(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* SqlString_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* StrObj_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* BoolVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* VoiVal_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* VoiVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Null_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ArrayConstructor_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Constructor_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Base_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Variable_GetterA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Variable_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Function_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* If_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* If_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* While_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* While_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* For_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* For_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Switch_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Switch_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Break_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Break_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Continue_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Return_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Return_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Goto_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Goto_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Print_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Print_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GenericFunction2_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Model_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Model_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ModelVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Gene_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Gene_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneChild_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneMutate_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneNew_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneCross_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneStore_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneSelect_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneSort_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneLabel_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* GeneLabelValue_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);

// ============================================================
// ヘルパー: CallBlockの2ブロックを実行する共通パターン
// ============================================================
static void _exeCallBlockPairA(ThreadGC* thgc, Obj* callblk, Obj* owner, Master* local) {
	List* blocks = get_blocks(local);
	Block* blk1 = (Block*)*(Obj**)get_list(callblk->children, 0);
	if (local->comments->size > 0) blk1->comment = true;
	blk1->obj = (Obj*)make_objblock(thgc, owner, 0);
	pushBlock(thgc, local, blk1);
	vt_exeA(thgc, (Obj*)blk1, local);

	Block* blk2 = (Block*)*(Obj**)get_list(callblk->children, 1);
	if (local->comments->size > 0) blk2->comment = true;
	blk2->obj = (Obj*)make_objblock(thgc, owner, 1);
	pushBlock(thgc, local, blk2);
	vt_exeA(thgc, (Obj*)blk2, local);

	if (blocks->size > 0) blocks->size--;
	if (blocks->size > 0) blocks->size--;
}

// ============================================================
// ヘルパー: Geneの追加CallBlock (left/right) を実行
// ============================================================
static void _exeGeneExtBlockA(ThreadGC* thgc, Obj* callblk, Obj* owner, Master* local) {
	List* blocks = get_blocks(local);
	Block* blk3 = (Block*)*(Obj**)get_list(callblk->children, 0);
	if (local->comments->size > 0) blk3->comment = true;
	blk3->obj = (Obj*)make_objblock(thgc, owner, 2);
	pushBlock(thgc, local, blk3);
	vt_exeA(thgc, (Obj*)blk3, local);

	Block* blk4 = (Block*)*(Obj**)get_list(callblk->children, 1);
	if (local->comments->size > 0) blk4->comment = true;
	blk4->obj = (Obj*)make_objblock(thgc, owner, 3);
	pushBlock(thgc, local, blk4);
	vt_exeA(thgc, (Obj*)blk4, local);

	if (blocks->size > 0) blocks->size--;
	if (blocks->size > 0) blocks->size--;
}

// ============================================================
// Word::SelfA - C# Word.SelfA: return local.getA(name);
// ============================================================
Obj* Word_SelfA(ThreadGC* thgc, Obj* self, Master* local) {
	return getA(thgc, local, self->letter->text);
}

// ============================================================
// Word::GetterA - C# Word.GetterA: return local.getA(name).GetterA(local);
// ============================================================
Obj* Word_GetterA(ThreadGC* thgc, Obj* self, Master* local) {
	return GetterA_vt(thgc, getA(thgc, local, self->letter->text), local);
}

// ============================================================
// Word::exepA - C# Word.exepA: return this;
// ============================================================
Obj* Word_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Word::PrimaryA - C# Word.PrimaryA: n++; return this;
// ============================================================
Obj* Word_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	(*n)++;
	return self;
}

// ============================================================
// Value::PrimaryA - 値型の後置演算
// ============================================================
Obj* Value_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			return self;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後は名前を指定してください");
	} else if (val2->type == LetterType::OT_Bracket) {
		vt_exeA(thgc, val2, local);
		(*n)++;
		return self;
	} else if (val2->type == LetterType::OT_Block) {
		vt_exeA(thgc, val2, local);
		(*n)++;
		return self;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "値の後続が適切ではないです");
}

// ============================================================
// Class::exepA
// ============================================================
Obj* Class_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Class::PrimaryA - クラス宣言
// ============================================================
Obj* Class_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	Obj* obj = make_classobj(thgc);

	if (val2->type == LetterType::OT_Word) {
		Obj* word = val2;
		obj = getA(thgc, local, word->letter->text);
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (local->comments->size > 0) {
			return ErrorA(thgc, LetterType::OT_Error, self->letter, "comment内でクラスを宣言できません。");
		}
		// Generic block handling
		if (val2->type == LetterType::OT_Block) {
			Generic* generic = (Generic*)obj;
			Obj* val = vt_exeA(thgc, val2, local);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			Block* block = (Block*)val;
			pushBlock(thgc, local, block);
			for (int i = 0; i < block->rets->size; i++) {
				Obj* ret_i = *(Obj**)get_list(block->rets, i);
				if (ret_i->type == LetterType::OT_Word) {
					VariClass* vc = make_variclass(thgc, i);
					add_mapy(thgc, generic->vmap, ret_i->letter->text, (char*)vc);
				} else {
					return ErrorA(thgc, LetterType::OT_Error, ret_i->letter, "ジェネリッククラスの引数が適切ではありません");
				}
			}
			if (blocks->size > 0) blocks->size--;
			generic->block = block;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			obj = (Obj*)generic;
		}
		obj->letter = word->letter;
		// Right arrow → rename
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				obj->rename = val2->letter->text;
				if (val2->type == LetterType::OT_Block) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					Block* blk = (Block*)val2;
					if (blk->children->size == 1) {
						Obj* ch0 = *(Obj**)get_list(blk->children, 0);
						if (ch0->type == LetterType::OT_FloatVal) {
							obj->version = ch0->letter->text;
						} else if (ch0->type == LetterType::OT_Number) {
							obj->version = ch0->letter->text;
						} else {
							return ErrorA(thgc, LetterType::OT_Error, ch0->letter, "versionの引数は小数か整数だけです");
						}
					} else {
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "versionの引数は小数か整数だけです");
					}
				} else {
					obj->version = createString(thgc, (char*)"0", 1, 1);
				}
			} else {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "テーブル名を指定してください。");
			}
		} else {
			obj->rename = word->letter->text;
			obj->version = createString(thgc, (char*)"0", 1, 1);
		}
		declareRA(thgc, local, obj->rename, obj);
	}
	// Left arrow → extends
head:
	if (val2->type == LetterType::_Left) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Bracket) {
			val2 = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
			if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
			Block* blk = (Block*)val2;
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* ret_i = *(Obj**)get_list(blk->rets, i);
				if (ret_i->type == LetterType::OT_ClassObj || ret_i->type == LetterType::OT_GenericObj) {
					if (obj->type == LetterType::OT_ClassObj) add_list(thgc, ((ClassObj*)obj)->extends_list, (char*)ret_i);
					else add_list(thgc, ((Generic*)obj)->extends_list, (char*)ret_i);
				} else {
					ATSSpan* eletter = (blk->letters && i < blk->letters->size) ? *(ATSSpan**)get_list(blk->letters, i) : blk->letter;
					return ErrorA(thgc, LetterType::OT_Error, eletter, "継承元はクラスかジェネリックスでなければなりません");
				}
			}
		}
		if (val2->type == LetterType::OT_ClassObj) {
			if (obj->type == LetterType::OT_ClassObj) add_list(thgc, ((ClassObj*)obj)->extends_list, (char*)val2);
			else if (obj->type == LetterType::OT_Generic) add_list(thgc, ((Generic*)obj)->extends_list, (char*)val2);
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "継承元のクラスはクラスとジェネリッククラスだけです");
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			goto head;
		} else if (val2->type == LetterType::OT_Generic) {
			Generic* generic_ext = (Generic*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Block) {
				val2 = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
				if (val2->type == LetterType::OT_NG || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
				Block* block = (Block*)val2;
				GenericObj* genericobj = make_genericobj(thgc, (Obj*)generic_ext);
				for (int i = 0; i < block->rets->size; i++) {
					Obj* rv = *(Obj**)get_list(block->rets, i);
					if (isTypeObj(rv)) {
						add_list(thgc, genericobj->draws, (char*)rv);
					} else {
						return ErrorA(thgc, LetterType::OT_Error, rv->letter, "ジェネリックスクラスの引数は型でなければならない");
					}
				}
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (generic_ext->vmap->kvs->size != genericobj->draws->size) {
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリッククラスの引数の数が合っていません");
				}
				if (obj->type == LetterType::OT_ClassObj) add_list(thgc, ((ClassObj*)obj)->extends_list, (char*)genericobj);
				else if (obj->type == LetterType::OT_Generic) add_list(thgc, ((Generic*)obj)->extends_list, (char*)genericobj);
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "継承はクラスかジェネリッククラスだけです。");
				goto head;
			} else {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリッククラスは[]で引数をもたなければならないです");
			}
		} else {
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "クラスの継承元はクラスでなければなりません");
		}
	}
	// CallBlock → class body
	if (val2->type == LetterType::OT_CallBlock) {
		if (obj->type == LetterType::OT_Generic) {
			Generic* generic = (Generic*)obj;
			declareA2(thgc, local, (CallBlock*)val2, obj);
			generic->letter2 = ((Block*)*(Obj**)get_list(val2->children, 1))->letter2;
			generic->draw = (CallBlock*)val2;
			Block* blk1 = (Block*)*(Obj**)get_list(val2->children, 0);
			if (local->comments->size > 0) blk1->comment = true;
			blk1->obj = (Obj*)make_objblock(thgc, obj, 0);
			pushBlock(thgc, local, blk1);
			vt_exeA(thgc, (Obj*)blk1, local);
			Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
			if (local->comments->size > 0) blk2->comment = true;
			if (((Generic*)obj)->extends_list->size > 0) {
				add_mapy(thgc, blk2->vmapA, createString(thgc, (char*)"base", 4, 1), (char*)make_base(thgc, NULL, ((Generic*)obj)->extends_list));
			}
			blk2->obj = (Obj*)make_objblock(thgc, obj, 1);
			pushBlock(thgc, local, blk2);
			vt_exeA(thgc, (Obj*)blk2, local);
			if (blocks->size > 0) blocks->size--;
			if (blocks->size > 0) blocks->size--;
		} else if (obj->type == LetterType::OT_ClassObj) {
			ClassObj* cbj = (ClassObj*)obj;
			declareA2(thgc, local, (CallBlock*)val2, obj);
			((Type*)obj)->letter2 = ((Block*)*(Obj**)get_list(val2->children, 1))->letter2;
			cbj->draw = (Block*)val2;
			Block* blk1 = (Block*)*(Obj**)get_list(val2->children, 0);
			if (local->comments->size > 0) blk1->comment = true;
			blk1->obj = (Obj*)make_objblock(thgc, (Obj*)cbj, 0);
			pushBlock(thgc, local, blk1);
			vt_exeA(thgc, (Obj*)blk1, local);
			Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
			if (local->comments->size > 0) blk2->comment = true;
			if (((ClassObj*)obj)->extends_list->size > 0) {
				add_mapy(thgc, blk2->vmapA, createString(thgc, (char*)"base", 4, 1), (char*)make_base(thgc, NULL, ((ClassObj*)obj)->extends_list));
			}
			blk2->obj = (Obj*)make_objblock(thgc, obj, 1);
			pushBlock(thgc, local, blk2);
			vt_exeA(thgc, (Obj*)blk2, local);
			if (blocks->size > 0) blocks->size--;
			if (blocks->size > 0) blocks->size--;
		}
		(*n)++;
		return obj;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "クラスの宣言の{|}が抜けています");
}

// ============================================================
// Type::PrimaryA - 型の後置演算 (変数・関数宣言)
// ============================================================
Obj* Type_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
head:
	// Array type: T[]
	if (val2->type == LetterType::OT_Block) {
		Block* blk = (Block*)val2;
		if (blk->children->size == 0) {
			ArrType* arrtype = make_arrtype(thgc, self);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			self = (Obj*)arrtype;
			goto head;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型は引数の数が0です");
	}
	// Function type: T(args)
	if (val2->type == LetterType::_Left) {
		FuncTypeC* functype = make_functype(thgc, self);
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Bracket) {
			val2 = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
			if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
			Block* blk = (Block*)val2;
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* rv = *(Obj**)get_list(blk->rets, i);
				if (rv->type == LetterType::OT_Wait || rv->type == LetterType::OT_Error || rv->type == LetterType::OT_NG) return rv;
				else if (rv->type == LetterType::OT_ClassObj || rv->type == LetterType::OT_GenericObj) {
					add_list(thgc, functype->draws, (char*)rv);
				}
				else return ErrorA(thgc, LetterType::OT_Error, rv->letter, "継承元はクラスでなければならない");
			}
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
		} else {
			Obj* gval = GetterA_vt(thgc, val2, local);
			if (gval->type == LetterType::OT_Var) {
				add_list(thgc, functype->draws, (char*)gval);
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				self = (Obj*)functype;
				goto head;
			} else if (gval->type == LetterType::OT_ClassObj) {
				add_list(thgc, functype->draws, (char*)gval);
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				self = (Obj*)functype;
				goto head;
			} else if (gval->type == LetterType::OT_Generic) {
				Generic* generic = (Generic*)gval;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Block) {
					val2 = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
					if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
					Block* blk = (Block*)val2;
					GenericObj* genericobj = make_genericobj(thgc, (Obj*)generic);
					for (int i = 0; i < blk->rets->size; i++) {
						Obj* rv = *(Obj**)get_list(blk->rets, i);
						if (isType(rv)) {
							add_list(thgc, genericobj->draws, (char*)rv);
						} else {
							return ErrorA(thgc, LetterType::OT_Error, rv->letter, "ジェネリックスクラスの引数は型でなければいけません");
						}
					}
					if (generic->vmap->kvs->size != genericobj->draws->size)
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数が合っていません");
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					add_list(thgc, functype->draws, (char*)genericobj);
					self = (Obj*)functype;
					goto head;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数として[]をお願いします");
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "継承元はクラスでなければならない");
		}
		self = (Obj*)functype;
	}
	// Dot → .new / .method
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			if (self->type == LetterType::OT_ArrType) {
				String* wordname = val2->letter ? val2->letter->text : NULL;
				if (wordname && str_matchA(wordname, "new", 3)) {
					ArrayConstructor* ctor = make_arrayconstructor(thgc, self);
					((Obj*)ctor)->letter = self->letter;
					return (Obj*)ctor;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型は.newのみ使えます");
			} else if (self->type == LetterType::OT_ClassObj) {
				String* wordname = val2->letter ? val2->letter->text : NULL;
				if (wordname && str_matchA(wordname, "new", 3)) {
					Constructor* ctor = make_constructor(thgc, self);
					((Obj*)ctor)->letter = self->letter;
					return (Obj*)ctor;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "クラス型は.newのみ使えます");
			} else if (self->type == LetterType::OT_ModelObj || self->type == LetterType::OT_GeneObj) {
				String* wordname = val2->letter ? val2->letter->text : NULL;
				ModelObj* model = (ModelObj*)self;
				Block* blk0 = (Block*)*(Obj**)get_list(((Obj*)model->draw)->children, 0);
				if (blk0->vmapA && wordname && get_mapy(blk0->vmapA, wordname)) {
					return (Obj*)make_sqlstring(thgc, wordname, val2->letter);
				}
				Block* blk1 = (Block*)*(Obj**)get_list(((Obj*)model->draw)->children, 1);
				if (blk1->vmapA && wordname && get_mapy(blk1->vmapA, wordname)) {
					return (Obj*)make_sqlstring(thgc, wordname, val2->letter);
				}
				if (wordname && str_matchA(wordname, "new", 3)) {
					Constructor* ctor = make_constructor(thgc, self);
					((Obj*)ctor)->letter = self->letter;
					return (Obj*)ctor;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "モデル型のメンバーが見つかりません");
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "関数型は.が来れません");
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には単語が必要です");
	}
	// Dollar → comment variable
	if (val2->type == LetterType::OT_Dolor) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			if (local->comments && local->comments->size > 0) {
				Comment* lastComment = *(Comment**)get_list(local->comments, local->comments->size - 1);
				ATSSpan* cletter = comment_value_add(thgc, lastComment, val2->letter->text);
				Variable* variable = make_variable(thgc, self);
				variable->comment = cletter;
				return self;
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "コメント変数はコメントの中に宣言してください。");
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "コメント変数の名前を指定してください。");
	}
	// Word → variable/function declaration
	Obj* word = NULL;
	Obj* wordre = NULL;
	if (val2->type == LetterType::OT_Word) {
		word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// model/gene内の型制約チェック (C# head2)
		{
			Block* lastblk = *(Block**)get_list(blocks, blocks->size - 1);
			Obj* lastobj = lastblk->obj;
			Obj* lastobj_inner = (lastobj && lastobj->type == LetterType::OT_ObjBlock) ? ((ObjBlock*)lastobj)->obj : lastobj;
			if (lastobj_inner && (lastobj_inner->type == LetterType::OT_ModelObj || lastobj_inner->type == LetterType::OT_GeneObj)) {
				Obj* obj = self;
			head2:
				if (obj->type == LetterType::OT_ClassObj || obj->type == LetterType::OT_FuncType)
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "model,geneの中ではmodelかgeneの型しか宣言できません");
				else if (obj->type == LetterType::OT_ArrayType) {
					obj = ((Type*)obj)->cls;
					goto head2;
				}
			}
		}
		// Right arrow → rename
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				wordre = val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
			} else {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "カラム名には名前が必要です");
			}
		}
	} else if (val2->type == LetterType::OT_Bracket) {
		Block* blk = (Block*)vt_exeA(thgc, val2, local);
		(*n)++;
		for (int i = 0; i < blk->rets->size; i++) {
			Obj* rv = *(Obj**)get_list(blk->rets, i);
			if (rv->type == LetterType::OT_Word) {
				Variable* variable = make_variable(thgc, self);
				declareA(thgc, local, rv->letter->text, (Obj*)variable);
				((Obj*)variable)->rename = rv->letter->text;
				declareRA(thgc, local, ((Obj*)variable)->rename, (Obj*)variable);
				((Obj*)variable)->letter = rv->letter;
			} else {
				ATSSpan* eletter = (blk->letters && i < blk->letters->size) ? *(ATSSpan**)get_list(blk->letters, i) : blk->letter;
				return ErrorA(thgc, LetterType::OT_Error, eletter, "変数宣言は名前だけです");
			}
		}
		return (Obj*)blk;
	}
	// GenericFunction: T [params] {|body}
	if (val2->type == LetterType::OT_Block) {
		GenericFunction* func = make_genericfunction(thgc, self);
		Obj* val = vt_exeA(thgc, val2, local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		for (int i = 0; i < blk->rets->size; i++) {
			Obj* ret_i = *(Obj**)get_list(blk->rets, i);
			if (ret_i->type == LetterType::OT_Word) {
				VariClass* vc = make_variclass(thgc, i);
				add_mapy(thgc, func->vmap, ret_i->letter->text, (char*)vc);
			} else {
				ATSSpan* eletter = (blk->letters && i < blk->letters->size) ? *(ATSSpan**)get_list(blk->letters, i) : blk->letter;
				return ErrorA(thgc, LetterType::OT_Error, eletter, "ジェネリック関数の宣言は名前しか引数をとれません");
			}
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_CallBlock) {
			if (local->comments->size > 0) {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "comment内で関数を宣言できません。");
			}
			func->draw = (Block*)val2;
			declareA2(thgc, local, (CallBlock*)val2, (Obj*)func);
			_exeCallBlockPairA(thgc, val2, (Obj*)func, local);
			(*n)++;
			if (word != NULL) {
				((Obj*)func)->rename = wordre ? wordre->letter->text : word->letter->text;
				declareA(thgc, local, word->letter->text, (Obj*)func);
				declareRA(thgc, local, ((Obj*)func)->rename, (Obj*)func);
			}
			return (Obj*)func;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリック関数の{|}が指定されていません");
	}
	// Function: T word {|body}
	else if (val2->type == LetterType::OT_CallBlock) {
		Function* func = make_function(thgc, self);
		func->draw = (Block*)val2;
		declareA2(thgc, local, (CallBlock*)val2, (Obj*)func);
		_exeCallBlockPairA(thgc, val2, (Obj*)func, local);
		for (int bi = 0; bi < blocks->size; bi++) {
			add_list(thgc, func->blocks, *get_list(blocks, bi));
		}
		(*n)++;
		if (word != NULL) {
			declareA(thgc, local, word->letter->text, (Obj*)func);
			((Obj*)func)->rename = wordre ? wordre->letter->text : word->letter->text;
			declareRA(thgc, local, ((Obj*)func)->rename, (Obj*)func);
			((Obj*)func)->letter = word->letter;
		} else {
			((Obj*)func)->letter = val2->letter;
		}
		return (Obj*)func;
	}
	// Variable declaration (no CallBlock)
	else if (word != NULL) {
		Variable* variable = make_variable(thgc, self);
		declareA(thgc, local, word->letter->text, (Obj*)variable);
		((Obj*)variable)->letter = word->letter;
		((Obj*)variable)->rename = wordre ? wordre->letter->text : word->letter->text;
		declareRA(thgc, local, ((Obj*)variable)->rename, (Obj*)variable);
		return (Obj*)variable;
	}
	return self;
}

// ============================================================
// Var::exepA
// ============================================================
Obj* Var_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Generic::PrimaryA
// ============================================================
Obj* Generic_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Generic* gen = (Generic*)self;
	if (val2->type == LetterType::OT_Block) {
		Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* block = (Block*)val;
		GenericObj* geneobj = make_genericobj(thgc, self);
		geneobj->extends_list = create_list(thgc, sizeof(Obj*), CType::_CObj);
		// C#: if (this.block.vmap.Count != block.rets.Count) return Error
		if (gen->block->vmapA->kvs->size != block->rets->size)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数があっていません。");
		for (int i = 0; i < block->rets->size; i++) {
			Obj* rv = *(Obj**)get_list(block->rets, i);
			// C#: if (block.rets[i] is Type) geneobj.draws.Add(...) else Error
			if (isTypeObj(rv)) {
				add_list(thgc, geneobj->draws, (char*)rv);
			} else {
				ATSSpan* eletter = *(ATSSpan**)get_list(block->letters, i);
				return ErrorA(thgc, LetterType::OT_Error, eletter, "ジェネリックスクラスは型を引数にとる必要があります。");
			}
		}
		// C#: var list = new List<Obj>(this.block.vmap.Values); (未使用)
		return (Obj*)geneobj;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "ジェネリッククラスは[]を引数にとります");
}

// ============================================================
// Number::exeA - C# Number.exeA: return this;
// ============================================================
Obj* Number_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// Number::exepA - C# Number.exepA: return this;
// ============================================================
Obj* Number_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Number::PrimaryA
// ============================================================
Obj* Number_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		Obj* v2 = *(Obj**)get_list(primary->children, *n);
		if (v2->type == LetterType::OT_Word) {
			Word* word = (Word*)v2;
			(*n)++;
			Obj* v3 = *(Obj**)get_list(primary->children, *n);
			if (str_matchA(word->letter->text, "random", 6)) {
				if (v3->type == LetterType::OT_Bracket) {
					(*n)++;
					// C#: if (val2.children.Count != 0) return Error
					if (((Block*)v3)->children->size != 0)
						return ErrorA(thgc, LetterType::OT_Error, v3->letter, "randomの引数は0個です");
					Obj* num = make_number(thgc, NULL);
					// C#: cls = local.Int
					((Val*)num)->cls = local->Int;
					return num;
				}
				else return ErrorA(thgc, LetterType::OT_Error, v3->letter, "randomの()が指定されていません");
			}
			else return ErrorA(thgc, LetterType::OT_Error, v3->letter, "は適切な後続の単語ではありません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, v2->letter, ".の後に単語が来ていません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が来ていません");
}

// ============================================================
// FloatVal::exepA / PrimaryA
// C# FloatVal.exeA: return this; → ReturnSelf_exeA (vt)
// ============================================================
Obj* FloatVal_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* FloatVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "小数には後続が来れません");
}

// ============================================================
// SqlString::exeA / exepA / opeA / PrimaryA
// C# SqlString.exeA: return this; → ReturnSelf_exeA (vt)
// C# SqlString.exepA: return this; → ReturnSelf_exepA (vt)
// ============================================================
Obj* SqlString_opeA(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if (key == LetterType::_Plus || key == LetterType::_Minus || key == LetterType::_Mul ||
		key == LetterType::_Div || key == LetterType::_Equal) return self;
	if (key == LetterType::_EqualEqual || key == LetterType::_NotEqual ||
		key == LetterType::_MoreEqual || key == LetterType::_LessEqual) return self;
	if (key == LetterType::_MoreThan || key == LetterType::_LessThan) return self;
	if (key == LetterType::_And || key == LetterType::_Or) return self;
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "SQLで不適切な演算子");
}
Obj* SqlString_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "SQL文字列には後続が来れません");
}

// ============================================================
// StrObj::PrimaryA
// C# StrObj.exeA: return this; → ReturnSelf_exeA (vt)
// C# StrObj.exepA: return this; → ReturnSelf_exepA (vt)
// ============================================================
Obj* StrObj_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "文字列には後続が来れません");
}

// ============================================================
// BoolVal::PrimaryA
// ============================================================
Obj* BoolVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "bool値には後続がこれません");
}

// ============================================================
// VoiVal::exepA / PrimaryA
// ============================================================
Obj* VoiVal_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return ErrorA(thgc, LetterType::OT_NG, self->letter, "void値には後続がこれません");
}
Obj* VoiVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "void値には後続がこれません");
}

// ============================================================
// Null::PrimaryA
// ============================================================
Obj* Null_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "nullには後続が来ません");
}

// ============================================================
// ArrayConstructor::PrimaryA
// ============================================================
Obj* ArrayConstructor_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型のコンストラクタの引数は1です");
		(*n)++;
		if ((*(Obj**)get_list(blk->rets, 0))->type == LetterType::OT_Number) {
			Obj* value = New_Value(thgc, (Obj*)((ArrayConstructor*)self)->cls, local, self->letter);
			if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
			Block* arr = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
			add_list(thgc, arr->rets, (char*)value);
			return (Obj*)arr;
		}
		else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, 0), "配列型のコンストラクタの引数は1です");
	}
	Obj* val3 = GetterA_vt(thgc, val2, local);
	if (val3->type == LetterType::OT_Wait || val3->type == LetterType::OT_Error || val3->type == LetterType::OT_NG) return val3;
	if (val3->type == LetterType::OT_Number) {
		Obj* value = New_Value(thgc, (Obj*)((ArrayConstructor*)self)->cls, local, self->letter);
		if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
		Block* arr = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
		add_list(thgc, arr->rets, (char*)value);
		return (Obj*)arr;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "コンストラクタの引数が適切ではありません");
}

// ============================================================
// Constructor::PrimaryA
// ============================================================
Obj* Constructor_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		val2 = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		Block* blk = (Block*)val2;
		// C#: var block1 = cls.draw.children[0] as Block;
		Block* block1 = (Block*)*(Obj**)get_list(((Obj*)((Constructor*)self)->cls->draw)->children, 0);
		Block_exeRangeA(thgc, block1, 0, block1->children->size, local, false);
		(*n)++;
		return New_Value(thgc, (Obj*)((Constructor*)self)->cls, local, self->letter);
	}
	Obj* val3 = GetterA_vt(thgc, val2, local);
	if (val3->type == LetterType::OT_Wait || val3->type == LetterType::OT_Error || val3->type == LetterType::OT_NG) return val3;
	if (val3->type == LetterType::OT_Number || val3->type == LetterType::OT_StrObj ||
		val3->type == LetterType::OT_BoolVal || val3->type == LetterType::OT_Value) {
		// C#: var block1 = cls.draw.children[0] as Block;
		Block* block1 = (Block*)*(Obj**)get_list(((Obj*)((Constructor*)self)->cls->draw)->children, 0);
		Block_exeRangeA(thgc, block1, 0, block1->children->size, local, false);
		(*n)++;
		return New_Value(thgc, (Obj*)((Constructor*)self)->cls, local, self->letter);
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "コンストラクタの引数が適切ではありません");
}

// ============================================================
// Base::PrimaryA
// ============================================================
Obj* Base_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	int m = 0;
	if (val2->type == LetterType::OT_Block) {
		// C#: if (val2.children.Count == 1 && val2.children[0].type == ObjType.Number) m = (val2.children[0] as Number).value;
		if (val2->children->size == 1 && (*(Obj**)get_list(val2->children, 0))->type == LetterType::OT_Number) {
			m = ((Number*)*(Obj**)get_list(val2->children, 0))->value;
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	}
	// C#: var cls = extends[m] as ClassObj;
	Obj* cls = *(Obj**)get_list(((Base*)self)->extends_list, m);
	if (val2->type == LetterType::OT_Bracket) {
		GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		(*n)++;
		return make_voival(thgc);
	}
	Obj* val3 = GetterA_vt(thgc, val2, local);
	if (val3->type == LetterType::OT_Wait || val3->type == LetterType::OT_Error || val3->type == LetterType::OT_NG) return val3;
	if (val3->type == LetterType::OT_Number || val3->type == LetterType::OT_StrObj ||
		val3->type == LetterType::OT_BoolVal || val3->type == LetterType::OT_Value) {
		(*n)++;
		return make_voival(thgc);
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "baseの引数が適切に設定されていません");
}

// ============================================================
// Variable::GetterA - C# Variable.GetterA: return Value.New(cls, local, letter);
// ============================================================
Obj* Variable_GetterA(ThreadGC* thgc, Obj* self, Master* local) {
	return New_Value(thgc, ((Variable*)self)->cls, local, self->letter);
}

// ============================================================
// Variable::PrimaryA - C# return new Value(cls).PrimaryA(ref n, local, primary, val2);
// ============================================================
Obj* Variable_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Obj* val = New_Value(thgc, ((Variable*)self)->cls, local, self->letter);
	if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
	return PrimaryA_vt(thgc, val, n, local, primary, val2);
}

// ============================================================
// Function::PrimaryA
// C#: if bracket!=null throw; if Bracket → exeA.GetterA → Value(ret); else NextA loop → Value(ret)
// ============================================================
Obj* Function_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	// C#: if (bracket != null) throw new Exception();
	if (((Function*)self)->bracket != NULL) return ErrorA(thgc, LetterType::OT_Error, self->letter, "bracket != null");
	if (val2->type == LetterType::OT_Bracket) {
		Obj* v = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		if (v->type == LetterType::OT_Wait || v->type == LetterType::OT_Error || v->type == LetterType::OT_NG) return v;
		return New_Value(thgc, ((Function*)self)->rettype, local, self->letter);
	}
	// consume remaining args via NextA loop
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextA(thgc, (Primary*)primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) {
		val2 = GetterA_vt(thgc, val2, local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
			val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
			(*n)++;
			return New_Value(thgc, ((Function*)self)->rettype, local, self->letter);
		}
	}
	return ErrorA(thgc, LetterType::OT_NG, val2 ? val2->letter : self->letter, "()のような関数に適切な引数が来ていません");
}

// ============================================================
// If::exepA
// ============================================================
Obj* If_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// If::PrimaryA - if/elif/else分岐
// ============================================================
Obj* If_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_CallBlock) {
		// C#: calls = new List<IfBlock>()
		self->calls_list = create_list(thgc, sizeof(IfBlock*), CType::_CIfBlock);
		IfBlock* ifb = make_ifblock(thgc, (CallBlock*)val2, self, self);
		add_list(thgc, self->calls_list, (char*)ifb);
		_exeCallBlockPairA(thgc, val2, (Obj*)ifb, local);
		// C#: ret = block1.rets.Last().GetterA(local)
		Block* block1 = (Block*)*(Obj**)get_list(val2->children, 0);
		Obj* ret = GetterA_vt(thgc, *(Obj**)get_list(block1->rets, block1->rets->size - 1), local);
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	head:
		if (val2->type == LetterType::OT_Elif) {
			Obj* iflabel = val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				ifb = make_ifblock(thgc, (CallBlock*)val2, iflabel, self);
				add_list(thgc, self->calls_list, (char*)ifb);
				_exeCallBlockPairA(thgc, val2, (Obj*)ifb, local);
				block1 = (Block*)*(Obj**)get_list(val2->children, 0);
				ret = GetterA_vt(thgc, *(Obj**)get_list(block1->rets, block1->rets->size - 1), local);
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head;
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "elif文の中身が宣言されていません。");
		} else if (val2->type == LetterType::OT_Else) {
			Obj* iflabel = val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				ifb = make_ifblock(thgc, (CallBlock*)val2, iflabel, self);
				add_list(thgc, self->calls_list, (char*)ifb);
				_exeCallBlockPairA(thgc, val2, (Obj*)ifb, local);
				(*n)++;
				return New_Value(thgc, local->Int, local, self->letter);
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "else文の中身が宣言されていません。");
		}
		return New_Value(thgc, local->Int, local, self->letter);
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "if文の中身が宣言されていません");
}

// ============================================================
// While::exepA / PrimaryA
// ============================================================
Obj* While_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* While_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_CallBlock) {
		// C#: local.declareA2(val2 as CallBlock, this);
		declareA2(thgc, local, (CallBlock*)val2, self);
		_exeCallBlockPairA(thgc, val2, self, local);
		// C#: var retobj = block2.rets.Last().Getter(local);
		Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
		Obj* retobj = GetterA_vt(thgc, *(Obj**)get_list(blk2->rets, blk2->rets->size - 1), local);
		(*n)++;
		return retobj;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "while文の中身が宣言されていません");
}

// ============================================================
// For::exepA / PrimaryA
// ============================================================
Obj* For_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* For_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_CallBlock) {
		_exeCallBlockPairA(thgc, val2, self, local);
		// C#: var retobj = block2.rets.Last().Getter(local);
		Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
		Obj* retobj = GetterA_vt(thgc, *(Obj**)get_list(blk2->rets, blk2->rets->size - 1), local);
		(*n)++;
		return retobj;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "for文の中身が宣言されていません。");
}

// ============================================================
// Switch::exepA / PrimaryA
// ============================================================
Obj* Switch_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* Switch_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_CallBlock) {
		_exeCallBlockPairA(thgc, val2, self, local);
		Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
		Obj* retobj = *(Obj**)get_list(blk2->rets, blk2->rets->size - 1);
		(*n)++;
		return retobj;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "switch文の中身が宣言されていません");
}

// ============================================================
// Break::exepA / PrimaryA - 残りのPrimary項目を実行
// ============================================================
Obj* Break_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextA(thgc, (Primary*)primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) val2 = GetterA_vt(thgc, val2, local);
	(*n)--;
	return val2 ? val2 : self;
}
Obj* Break_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextA(thgc, (Primary*)primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) val2 = GetterA_vt(thgc, val2, local);
	return val2 ? val2 : self;
}

// ============================================================
// Continue::exepA
// ============================================================
Obj* Continue_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Return::exepA / PrimaryA
// ============================================================
Obj* Return_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextA(thgc, (Primary*)primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) val2 = GetterA_vt(thgc, val2, local);
	(*n)--;
	return val2 ? val2 : self;
}
Obj* Return_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextA(thgc, (Primary*)primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) val2 = GetterA_vt(thgc, val2, local);
	return val2 ? val2 : self;
}

// ============================================================
// Goto::exepA / PrimaryA
// ============================================================
Obj* Goto_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* Goto_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Goto* gt = (Goto*)self;
	if (val2->type == LetterType::OT_Number) {
		gt->value = NumberString(thgc, ((Number*)val2)->value);
		return self;
	} else if (val2->type == LetterType::OT_StrObj) {
		gt->value = ((StrObj*)val2)->value;
		return self;
	} else if (val2->type == LetterType::OT_Word) {
		gt->value = ((Word*)val2)->name;
		return self;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "goto文の行き先を指定してください。");
}

// ============================================================
// Print::exepA / PrimaryA
// ============================================================
Obj* Print_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* Print_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket || val2->type == LetterType::OT_Block) {
		Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		// C#: if (val2.type == Bracket) local.console.Text += "("; else local.console.Text += "[";
		// C#: if (val2.type == Bracket) local.console.Text += ")\n"; else local.console.Text += "]\n";
		(*n)++;
		return make_voival(thgc);
	}
	val2 = GetterA_vt(thgc, val2, local);
	if (val2->type == LetterType::OT_Number) {
		// C#: local.console.Text += (val2 as Number).value.ToString() + "\n";
		(*n)++;
		return make_voival(thgc);
	}
	else if (val2->type == LetterType::OT_StrObj) {
		// C#: local.console.Text += (val2 as StrObj).value + "\n";
		(*n)++;
		return make_voival(thgc);
	}
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextA(thgc, (Primary*)primary, local, n, &val2);
		if (val2 != NULL && (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG)) return val2;
	}
	if (val2 != NULL) {
		val2 = GetterA_vt(thgc, val2, local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
			val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
			(*n)++;
			return make_voival(thgc);
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "()でprintの引数を指定してください");
}

// ============================================================
// GenericFunction::PrimaryA
// ============================================================
Obj* GenericFunction2_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GenericFunction* gf = (GenericFunction*)self;
	if (val2->type == LetterType::OT_Block) {
		Obj* val = GetterA_vt(thgc, val2, local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		if (blk->rets->size != gf->vmap->kvs->size)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "引数の数があっていません。");
		for (int i = 0; i < blk->rets->size; i++) {
			Obj* ri = *(Obj**)get_list(blk->rets, i);
			if (!isType(ri))
				return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, i), "ジェネリック関数には型を引数にしてください");
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// C#: return new Function(ret) { blocks = blocks, draw = draw, bracket = bracket, block = blk };
		Function* func = make_function(thgc, gf->rettype);
		func->blocks = gf->blocks;
		func->draw = gf->draw;
		func->bracket = gf->bracket;
		func->block = blk;
		return (Obj*)func;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "[]でジェリック関数の引数を指定してください");
}

// ============================================================
// Model::exepA / PrimaryA
// ============================================================
Obj* Model_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* Model_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	// model [block].method()
	if (val2->type == LetterType::OT_Block) {
		val2 = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	head_model:
		if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				Word* word2 = (Word*)val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					Block* blk = (Block*)val;
					(*n)++;
					if (str_matchA(word2->name, "Where", 5)) {
						if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, blk->letter, "Firstは引数が1つです");
						val2 = *(Obj**)get_list(primary->children, *n);
						goto head_model;
					}
					else if (str_matchA(word2->name, "FirstAwait", 10)) {
						if (blk->rets->size != 0) return ErrorA(thgc, LetterType::OT_Error, blk->letter, "Firstは引数が1つです");
						return (Obj*)make_block(thgc, LetterType::OT_Array);
					}
					else if (str_matchA(word2->name, "Select", 6)) {
						if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, blk->letter, "Selectの引数は1つです");
						return (Obj*)make_block(thgc, LetterType::OT_Array);
					}
					else if (str_matchA(word2->name, "Sort", 4)) {
						if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, blk->letter, "Sortの引数は1つです");
						return (Obj*)make_block(thgc, LetterType::OT_Array);
					}
					else return ErrorA(thgc, LetterType::OT_Error, ((Obj*)word2)->letter, "First,Select,Sortがきます");
				}
				else return ErrorA(thgc, LetterType::OT_NG, val2->letter, "()がきます");
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
		}
		else return (Obj*)make_voival(thgc);
	}
	// model.Store / model.await
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Word* word = (Word*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (str_matchA(word->name, "Store", 5)) {
				if (val2->type == LetterType::_Dot) {
					// .Store.await pattern
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					if (val2->type == LetterType::OT_Word) {
						(*n)++;
						val2 = *(Obj**)get_list(primary->children, *n);
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後にはawaitが来ます");
				}
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = vt_exeA(thgc, val2, local);
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					(*n)++;
					return (Obj*)make_voival(thgc);
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Storeの後に()がありません");
			}
			else if (str_matchA(word->name, "await", 5)) {
				if (val2->type == LetterType::OT_CallBlock) {
					_exeCallBlockPairA(thgc, val2, self, local);
					(*n)++;
					return (Obj*)make_voival(thgc);
				}
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "modelには%sは宣言されていません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の次には名前が来ます");
	}
	// model word {|body} → ModelObj instance
	if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		Obj* gj = getA(thgc, local, word->name);
		ModelObj* mobj = (ModelObj*)gj;
		// C#: (gj.draw.children[1] as Block).vmapA["id"] = new Variable(local.Int) { letter = val2.letter };
		Block* drawblk1 = (Block*)*(Obj**)get_list(((Obj*)mobj->draw)->children, 1);
		Variable* idvar = make_variable(thgc, (Obj*)local->Int);
		((Obj*)idvar)->letter = val2->letter;
		add_mapy(thgc, drawblk1->vmapA, createString(thgc, (char*)"id", 2, 1), (char*)idvar);
		// C#: local.declare(word.name, gj);
		declareA(thgc, local, word->name, gj);
		mobj->letter = ((Obj*)word)->letter;
		// C#: if (val2.type == ObjType.Right)
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			// C#: if (val2.type == ObjType.Word)
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				// C#: gj.rename = (val2 as Word).name;
				gj->rename = ((Word*)val2)->name;
				// C#: if (val2.type == ObjType.Block)
				if (val2->type == LetterType::OT_Block) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					Block* blk = (Block*)val2;
					// C#: if (blk.children.Count == 1)
					if (blk->children->size == 1) {
						Obj* child0 = *(Obj**)get_list(blk->children, 0);
						if (child0->type == LetterType::OT_FloatVal) {
							// C#: gj.version = blk.children[0].letter.name;
							((Obj*)mobj)->version = child0->letter->text;
						}
						else if (child0->type == LetterType::OT_Number) {
							// C#: gj.version = blk.children[0].letter.name;
							((Obj*)mobj)->version = child0->letter->text;
						}
						else return ErrorA(thgc, LetterType::OT_Error, child0->letter, "versionの引数は小数か整数だけです");
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "versionの引数は小数か整数だけです");
				}
				// C#: else gj.version = "0";
				else {
					((Obj*)mobj)->version = createString(thgc, (char*)"0", 1, 1);
				}
			}
			// C#: else return Obj.Error(ObjType.Error, val2.letter, "テーブルを宣言してください。");
			else {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "テーブルを宣言してください。");
			}
		}
		// C#: else { gj.rename = word.name; gj.version = "0"; }
		else {
			gj->rename = word->name;
			((Obj*)mobj)->version = createString(thgc, (char*)"0", 1, 1);
		}
		declareRA(thgc, local, gj->rename, gj);
		if (val2->type == LetterType::OT_CallBlock) {
			if (local->comments->size > 0) {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "comment内でmodelを宣言できません。");
			}
			// C#: gj.letter2 = (val2.children[1] as Block).letter2;
			((Type*)mobj)->letter2 = ((Block*)*(Obj**)get_list(val2->children, 1))->letter2;
			// C#: gj.draw = val2 as CallBlock;
			mobj->draw = (Block*)val2;
			_exeCallBlockPairA(thgc, val2, gj, local);
			(*n)++;
			return gj;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "modelの内容を{}で宣言してください。");
	}
	// model {|body} (anonymous)
	if (val2->type == LetterType::OT_CallBlock) {
		_exeCallBlockPairA(thgc, val2, self, local);
		(*n)++;
		return (Obj*)make_voival(thgc);
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "model名を宣言してください。");
}

// ============================================================
// ModelVal::PrimaryA
// ============================================================
Obj* ModelVal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	// C#: if (val2.type == ObjType.Dot)
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// C#: if (val2.type == ObjType.Word)
		if (val2->type == LetterType::OT_Word) {
			Word* word = (Word*)val2;
			(*n)++;
			// C#: if (word.name == "update")
			if (str_matchA(word->name, "update", 6)) {
				val2 = *(Obj**)get_list(primary->children, *n);
				// C#: if (val2.type == ObjType.Bracket)
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					Block* block = (Block*)val;
					(*n)++;
					// C#: if (block.rets.Count != 1) return Obj.Error(...)
					if (block->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "updateの引数は1です");
					return self;
				}
				// C#: else if (val2.type == ObjType.Stock)
				else if (val2->type == LetterType::OT_Stock) {
					return self;
				}
				// C#: else return Obj.Error(...)
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "updateの引数はDataStockを取ります");
			}
			// C#: else if (vmap.ContainsKey(word.name)) return vmap[word.name];
			else if (((Val*)self)->vmap && get_mapy(((Val*)self)->vmap, word->name) != NULL) {
				return (Obj*)get_mapy(((Val*)self)->vmap, word->name);
			}
			// C#: else return Obj.Error(ObjType.Error, word.letter, "その単語は変数として宣言されていません");
			else return ErrorA(thgc, LetterType::OT_Error, ((Obj*)word)->letter, "その単語は変数として宣言されていません");
		}
		// C#: else return Obj.Error(ObjType.Error, val2.letter, ".の後は単語を宣言してください。");
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後は単語を宣言してください。");
	}
	// C#: return Obj.Error(ObjType.NG, val2.let, ".が来ます");
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が来ます");
}

// ============================================================
// Gene::exepA / PrimaryA - 遺伝子型宣言
// ============================================================
Obj* Gene_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}
Obj* Gene_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	if (val2->type == LetterType::OT_Word) {
		Obj* word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		Obj* gj = getA(thgc, local, word->letter->text);
		GeneObj* gobj = (GeneObj*)gj;
		gobj->letter = word->letter;
		// Right arrow → rename
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				gj->rename = val2->letter->text;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
			} else {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "テーブルを宣言してください。");
			}
		} else {
			gj->rename = word->letter->text;
		}
		declareRA(thgc, local, gj->rename, gj);
		if (val2->type == LetterType::OT_CallBlock) {
			gobj->call_blk = (Block*)val2;
			gobj->geneblocks = create_list(thgc, sizeof(Block*), CType::_Block);
			for (int bi = 0; bi < blocks->size; bi++) {
				add_list(thgc, gobj->geneblocks, *get_list(blocks, bi));
			}
			_exeCallBlockPairA(thgc, val2, gj, local);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
		gene_head:
			if (val2->type == LetterType::_Left) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_CallBlock) {
					_exeGeneExtBlockA(thgc, val2, gj, local);
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					goto gene_head;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
			} else if (val2->type == LetterType::_Right) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_CallBlock) {
					_exeGeneExtBlockA(thgc, val2, gj, local);
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					goto gene_head;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
			}
			return gj;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "{|}を宣言してください。");
	}
	// Anonymous gene {|body}
	if (val2->type == LetterType::OT_CallBlock) {
		GeneObj* gj = make_geneobj(thgc, NULL, NULL);
		gj->call_blk = (CallBlock*)val2;
		for (int bi = 0; bi < blocks->size; bi++) {
			add_list(thgc, gj->geneblocks, *get_list(blocks, bi));
		}
		gj->vmap = create_mapy(thgc, CType::_List);
		populate_geneobj_vmap(thgc, gj);
		_exeCallBlockPairA(thgc, val2, (Obj*)gj, local);
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	gene_head2:
		if (val2->type == LetterType::_Left) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				_exeGeneExtBlockA(thgc, val2, (Obj*)gj, local);
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto gene_head2;
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
		} else if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				_exeGeneExtBlockA(thgc, val2, (Obj*)gj, local);
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto gene_head2;
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
		}
		return (Obj*)gj;
	}
	// gene.name → member access
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			if (local->gene != NULL) {
				Generic* gen = (Generic*)local->gene;
				char* found = get_mapy(gen->vmap, val2->letter->text);
				if (found != NULL) return (Obj*)found;
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "無名Geneクラスは宣言されていません");
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が必要です");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "geneの後に適切な単語が来ていません");
}

// ============================================================
// GeneChild::PrimaryA
// ============================================================
Obj* GeneChild_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return make_cobj(thgc, CType::_CObj, LetterType::_None, self->letter);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Childの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneMutate::PrimaryA
// ============================================================
Obj* GeneMutate_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return make_cobj(thgc, CType::_CObj, LetterType::_None, self->letter);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Mutateの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneNew::PrimaryA
// ============================================================
Obj* GeneNew_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return make_cobj(thgc, CType::_CObj, LetterType::_None, self->letter);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Newの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneCross::PrimaryA
// ============================================================
Obj* GeneCross_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 2) {
			Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			(*n)++;
			return make_cobj(thgc, CType::_CObj, LetterType::_None, self->letter);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Crossの引数は2です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneStore::PrimaryA
// ============================================================
Obj* GeneStore_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			return make_cobj(thgc, CType::_CObj, LetterType::_None, self->letter);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Storeの引数は0です。");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneSelect::PrimaryA
// ============================================================
Obj* GeneSelect_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = vt_exeA(thgc, val2, local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		if (val2->children->size <= 1) {
			(*n)++;
			return (Obj*)make_cblock(thgc, CType::_Block, LetterType::OT_Block);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Selectの引数があっていません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneSort::PrimaryA
// ============================================================
Obj* GeneSort_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = vt_exeA(thgc, val2, local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		if (val2->children->size <= 1) {
			(*n)++;
			return (Obj*)make_cblock(thgc, CType::_Block, LetterType::OT_Block);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Sortの引数があっていません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneLabel::exepA
// ============================================================
Obj* GeneLabel_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// GeneLabelValue::PrimaryA
// ============================================================
Obj* GeneLabelValue_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			if (str_matchA(val2->letter->text, "length", 6)) {
				return make_number(thgc, NULL);
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "このGeneLabelには宣言されていません");
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後は名前でないといけません");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が必要です。");
}

// C# ModelObj.exeA / GeneObj.exeA / SqlString.exeA / StrObj.exeA / BoolVal.exeA / Null.exeA: return this;
Obj* ReturnSelf_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// C# SqlString.exepA / StrObj.exepA / BoolVal.exepA / Null.exepA: return this;
Obj* ReturnSelf_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// GeneObj::DotA - vmap lookup
// C#: if (vmap.ContainsKey(name)) return vmap[name]; return gv.vmap[name];
// ============================================================
Obj* GeneObj_DotA(ThreadGC* thgc, Obj* self, String* name) {
	GeneObj* gobj = (GeneObj*)self;
	char* found = get_mapy(gobj->vmap, name);
	if (found) return (Obj*)found;
	// fallback to gv.vmap (gv is Obj* with vmapA)
	if (gobj->gv && ((Val*)gobj->gv)->vmap) {
		found = get_mapy(((Val*)gobj->gv)->vmap, name);
		if (found) return (Obj*)found;
	}
	return NULL;
}

// ============================================================
// A3_initVTables - obj2a3.h用VTable初期化
// ============================================================
void A3_initVTables() {
	// --- Word --- (Obj2A.cs: Word)
	vt_Word.SelfA = Word_SelfA;
	vt_Word.GetterA = Word_GetterA;
	vt_Word.exepA = Word_exepA;
	vt_Word.PrimaryA = Word_PrimaryA;
	vt_lookup[(int)LetterType::OT_Word] = &vt_Word;

	// --- Number --- (Obj2A.cs: Number)
	vt_Number.exeA = Number_exeA;
	vt_Number.exepA = Number_exepA;
	vt_Number.PrimaryA = Number_PrimaryA;
	vt_lookup[(int)LetterType::OT_Number] = &vt_Number;

	// --- Variable --- (Obj2A.cs: Variable)
	vt_Variable.GetterA = Variable_GetterA;
	vt_Variable.PrimaryA = Variable_PrimaryA;
	vt_lookup[(int)LetterType::OT_Variable] = &vt_Variable;

	// --- Function --- (Obj2A.cs: Function)
	vt_Function.PrimaryA = Function_PrimaryA;
	vt_lookup[(int)LetterType::OT_Function] = &vt_Function;

	// --- FloatVal ---
	vt_FloatVal.exepA = FloatVal_exepA;
	vt_FloatVal.PrimaryA = FloatVal_PrimaryA;
	vt_lookup[(int)LetterType::OT_FloatVal] = &vt_FloatVal;

	// --- StrObj ---
	vt_StrObj.exeA = ReturnSelf_exeA;
	vt_StrObj.exepA = ReturnSelf_exepA;
	vt_StrObj.PrimaryA = StrObj_PrimaryA;
	vt_lookup[(int)LetterType::OT_StrObj] = &vt_StrObj;

	// --- BoolVal ---
	vt_BoolVal.exeA = ReturnSelf_exeA;
	vt_BoolVal.exepA = ReturnSelf_exepA;
	vt_BoolVal.PrimaryA = BoolVal_PrimaryA;
	vt_lookup[(int)LetterType::OT_BoolVal] = &vt_BoolVal;

	// --- Value ---
	vt_Value.PrimaryA = Value_PrimaryA;
	vt_lookup[(int)LetterType::OT_Value] = &vt_Value;

	// --- Var (inherits Type.PrimaryA) ---
	vt_Var.exepA = Var_exepA;
	vt_Var.PrimaryA = Type_PrimaryA;
	vt_lookup[(int)LetterType::OT_Var] = &vt_Var;

	// --- Class ---
	vt_Class.exepZ = Class_exepZ;
	vt_Class.exepA = Class_exepA;
	vt_Class.PrimaryA = Class_PrimaryA;
	vt_lookup[(int)LetterType::OT_Class] = &vt_Class;

	// --- Generic ---
	vt_Generic.PrimaryA = Generic_PrimaryA;
	vt_lookup[(int)LetterType::OT_Generic] = &vt_Generic;

	// --- If ---
	vt_If.exepA = If_exepA;
	vt_If.PrimaryA = If_PrimaryA;
	vt_lookup[(int)LetterType::OT_If] = &vt_If;

	// --- While ---
	vt_While.exepA = While_exepA;
	vt_While.PrimaryA = While_PrimaryA;
	vt_lookup[(int)LetterType::OT_While] = &vt_While;

	// --- For ---
	vt_For.exepA = For_exepA;
	vt_For.PrimaryA = For_PrimaryA;
	vt_lookup[(int)LetterType::OT_For] = &vt_For;

	// --- Switch ---
	vt_Switch.exepA = Switch_exepA;
	vt_Switch.PrimaryA = Switch_PrimaryA;
	vt_lookup[(int)LetterType::OT_Switch] = &vt_Switch;

	// --- Break ---
	vt_Break.exepA = Break_exepA;
	vt_Break.PrimaryA = Break_PrimaryA;
	vt_lookup[(int)LetterType::OT_Break] = &vt_Break;

	// --- Continue ---
	vt_Continue.exepA = Continue_exepA;
	vt_lookup[(int)LetterType::OT_Continue] = &vt_Continue;

	// --- Return ---
	vt_Return.exepA = Return_exepA;
	vt_Return.PrimaryA = Return_PrimaryA;
	vt_lookup[(int)LetterType::OT_Return] = &vt_Return;

	// --- Goto ---
	vt_Goto.exepA = Goto_exepA;
	vt_Goto.PrimaryA = Goto_PrimaryA;
	vt_lookup[(int)LetterType::OT_Goto] = &vt_Goto;

	// --- Print ---
	vt_Print.exepA = Print_exepA;
	vt_Print.PrimaryA = Print_PrimaryA;
	vt_lookup[(int)LetterType::OT_Print] = &vt_Print;

	// --- GeneLabel (inherits Type.PrimaryA) ---
	vt_GeneLabel.exepA = GeneLabel_exepA;
	vt_GeneLabel.PrimaryA = Type_PrimaryA;
	vt_lookup[(int)LetterType::OT_GeneLabel] = &vt_GeneLabel;

	// --- Model ---
	vt_Model.exepZ = Model_exepZ;
	vt_Model.exepA = Model_exepA;
	vt_Model.PrimaryA = Model_PrimaryA;
	vt_lookup[(int)LetterType::OT_Model] = &vt_Model;

	// --- Gene ---
	vt_Gene.exepZ = Gene_exepZ;
	vt_Gene.exepA = Gene_exepA;
	vt_Gene.PrimaryA = Gene_PrimaryA;
	vt_lookup[(int)LetterType::OT_Gene] = &vt_Gene;

	// --- GenericFunction ---
	vt_GenericFunction.PrimaryA = GenericFunction2_PrimaryA;
	vt_lookup[(int)LetterType::OT_GenericFunction] = &vt_GenericFunction;

	// --- SqlString ---
	vt_SqlString.exeA = ReturnSelf_exeA;
	vt_SqlString.exepA = ReturnSelf_exepA;
	vt_SqlString.opeA = SqlString_opeA;
	vt_SqlString.PrimaryA = SqlString_PrimaryA;

	// --- Constructor ---
	vt_Constructor.PrimaryA = Constructor_PrimaryA;

	// --- ArrayConstructor ---
	vt_ArrayConstructor.PrimaryA = ArrayConstructor_PrimaryA;

	// --- Base ---
	vt_Base.PrimaryA = Base_PrimaryA;

	// --- GeneChild ---
	vt_GeneChild.PrimaryA = GeneChild_PrimaryA;

	// --- GeneMutate ---
	vt_GeneMutate.PrimaryA = GeneMutate_PrimaryA;

	// --- GeneNew ---
	vt_GeneNew.PrimaryA = GeneNew_PrimaryA;

	// --- GeneCross ---
	vt_GeneCross.PrimaryA = GeneCross_PrimaryA;

	// --- GeneStore ---
	vt_GeneStore.PrimaryA = GeneStore_PrimaryA;

	// --- GeneSelect ---
	vt_GeneSelect.PrimaryA = GeneSelect_PrimaryA;

	// --- GeneSort ---
	vt_GeneSort.PrimaryA = GeneSort_PrimaryA;

	// --- GeneLabelValue ---
	vt_GeneLabelValue.PrimaryA = GeneLabelValue_PrimaryA;

	// --- ModelVal ---
	vt_ModelVal.PrimaryA = ModelVal_PrimaryA;

	// --- VoiVal ---
	vt_VoiVal.exepA = VoiVal_exepA;
	vt_VoiVal.PrimaryA = VoiVal_PrimaryA;

	// --- Null ---
	vt_Null.exeA = ReturnSelf_exeA;
	vt_Null.exepA = ReturnSelf_exepA;
	vt_Null.PrimaryA = Null_PrimaryA;

	// --- Type ---
	vt_Type.PrimaryA = Type_PrimaryA;

	// --- ArrType (inherits Type.PrimaryA) ---
	vt_ArrType.PrimaryA = Type_PrimaryA;
	vt_lookup[(int)LetterType::OT_ArrType] = &vt_ArrType;

	// --- FuncType (inherits Type.PrimaryA) ---
	vt_FuncType.PrimaryA = Type_PrimaryA;
	vt_lookup[(int)LetterType::OT_FuncType] = &vt_FuncType;

	// --- ClassObj (inherits Type.PrimaryA) ---
	vt_ClassObj.PrimaryA = Type_PrimaryA;
	vt_lookup[(int)LetterType::OT_ClassObj] = &vt_ClassObj;

	// --- ModelObj (inherits Type.PrimaryA, overrides exeA) ---
	vt_ModelObj.PrimaryA = Type_PrimaryA;
	vt_ModelObj.exeA = ReturnSelf_exeA;
	vt_ModelObj.GetterA = _vt_GetterA_default;
	vt_lookup[(int)LetterType::OT_ModelObj] = &vt_ModelObj;

	// --- GeneObj (inherits Type.PrimaryA, overrides exeA) ---
	vt_GeneObj.PrimaryA = Type_PrimaryA;
	vt_GeneObj.exeA = ReturnSelf_exeA;
	vt_GeneObj.GetterA = _vt_GetterA_default;
	vt_lookup[(int)LetterType::OT_GeneObj] = &vt_GeneObj;
}
