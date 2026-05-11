// ============================================================
// obj2b.h - B解析パス (vtable dispatch版) - Block 1
// Operator, Primary, Block, CallBlock + TypeCheck
// ============================================================

// ヘルパーマクロ
#define IS_WEG(obj) ((obj)->type == LetterType::OT_Wait || (obj)->type == LetterType::OT_Error || (obj)->type == LetterType::OT_NG)

// 前方宣言
Obj* Operator_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Primary_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Primary_NextB(ThreadGC* thgc, Primary* primary, Master* local, int* i, Obj** val1);
Obj* Block_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Block_exeRangeB(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id, bool typecheck);
Obj* Block_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Block_SelfB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_GetterB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* CallBlock_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* CallBlock_exeB(ThreadGC* thgc, Obj* self, Master* local);

// ============================================================
// declareB
// ============================================================
Obj* declareB(ThreadGC* thgc, Master* local, String* name, Obj* obj) {
	return obj;
}

// ============================================================
// getB
// ============================================================
Obj* getB(ThreadGC* thgc, Master* local, String* name, ATSSpan* letter) {
	List* blocks = get_blocks(local);
 	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* blk = *(Block**)get_list(blocks, i);
		char* val = get_mapy(blk->vmapA, name);
		if (val != NULL) return (Obj*)val;
	}
	// sigmapA check
	if (local->sigmapA) {
		char* sig = get_mapy(local->sigmapA, name);
		if (sig != NULL) return (Obj*)sig;
	}
	return ErrorA(thgc, LetterType::OT_Error, letter, "variable not found");
}

// ============================================================
// Operator::exeB
// ============================================================
Obj* Operator_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	LetterType op = self->type;
	Obj* ch0 = *(Obj**)get_list(self->children, 0);
	Obj* ch1 = *(Obj**)get_list(self->children, 1);

	if (op == LetterType::_Equal || op == LetterType::_Colon || op == LetterType::_In) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val2 = GetterB_vt(thgc, vt_exeB(thgc, ch1, local), local);
		local->calls->size--;
		if (IS_WEG(val2)) return val2;
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val1 = SelfB_vt(thgc, vt_exeB(thgc, ch0, local), local);
		if (IS_WEG(val1)) return val1;
		return vt_opeB(thgc, val1, op, local, val2);
	} else {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val1 = GetterB_vt(thgc, vt_exeB(thgc, ch0, local), local);
		if (IS_WEG(val1)) { local->calls->size--; return val1; }
		Obj* val2 = GetterB_vt(thgc, vt_exeB(thgc, ch1, local), local);
		local->calls->size--;
		if (IS_WEG(val2)) return val2;
		return vt_opeB(thgc, val1, op, local, val2);
	}
}

// ============================================================
// Primary::NextB
// ============================================================
Obj* Primary_NextB(ThreadGC* thgc, Primary* primary, Master* local, int* i, Obj** val1) {
	Obj* ch = *(Obj**)get_list(primary->children, *i);
	LetterType t = ch->type;

	switch (t) {
	case LetterType::OT_Comment2:
		(*i)++;
		break;
	case LetterType::OT_Htm:
	case LetterType::OT_TagBlock:
		vt_exepB(thgc, ch, i, local, primary);
		(*i)++;
		*val1 = NULL;
		break;
	case LetterType::OT_Gene:
	case LetterType::OT_Model:
	case LetterType::OT_Class:
	case LetterType::OT_Word:
	case LetterType::OT_Number:
	case LetterType::OT_FloatVal:
	case LetterType::OT_BoolVal:
	case LetterType::OT_StrObj:
	case LetterType::OT_Bracket:
	case LetterType::OT_Print:
	case LetterType::OT_Return:
	case LetterType::OT_Goto:
	case LetterType::OT_Continue:
	case LetterType::OT_Var:
	case LetterType::OT_If:
	case LetterType::OT_While:
	case LetterType::OT_For:
	case LetterType::OT_Switch:
	case LetterType::OT_GeneLabel:
	case LetterType::OT_Comment:
	case LetterType::OT_Client:
	case LetterType::OT_Server:
	case LetterType::OT_Signal:
	case LetterType::OT_ServerClient:
	case LetterType::OT_Dolor:
	case LetterType::OT_Mountain:
		if (*val1 == NULL) {
			*val1 = vt_exepB(thgc, ch, i, local, primary);
			(*i)++;
		} else {
			*val1 = GetterB_vt(thgc, *val1, local);
			if (IS_WEG(*val1)) return *val1;
			*val1 = PrimaryB_vt(thgc, *val1, i, local, primary, vt_exepB(thgc, ch, i, local, primary));
		}
		break;
	case LetterType::_Dot:
	case LetterType::_Left:
	case LetterType::_Right:
		*val1 = GetterB_vt(thgc, *val1, local);
		if (IS_WEG(*val1)) return *val1;
		*val1 = PrimaryB_vt(thgc, *val1, i, local, primary, ch);
		break;
	case LetterType::OT_CallBlock:
	case LetterType::OT_Block:
		if (*val1 == NULL) {
			*val1 = vt_exepB(thgc, ch, i, local, primary);
			(*i)++;
		} else {
			*val1 = GetterB_vt(thgc, *val1, local);
			if (IS_WEG(*val1)) return *val1;
			*val1 = PrimaryB_vt(thgc, *val1, i, local, primary, ch);
		}
		break;
	default:
		return ErrorA(thgc, LetterType::OT_Error, ch->letter, "unexpected type in NextB");
	}
	return *val1;
}

// ============================================================
// Primary::exeB
// ============================================================
Obj* Primary_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	Primary* primary = (Primary*)self;
	if (local->comments != NULL && local->comments->size > 0) {
		CommentLet* cl = *(CommentLet**)get_list(local->comments, local->comments->size - 1);
		Start_CommentLet(thgc, cl);
	}
	Obj* val1 = NULL;
	int count = primary->children->size;
	for (int i = 0; i < count - 1;) {
		Primary_NextB(thgc, primary, local, &i, &val1);
		if (val1 != NULL && IS_WEG(val1)) return val1;
	}
	if (val1 == NULL) val1 = *(Obj**)get_list(primary->children, count - 1);

	// singleops processing
	SingleOp* op = NULL;
	if (primary->singleops != NULL) {
		for (int i = primary->singleops->size - 1; i >= 0; i--) {
			SingleOp* sop = *(SingleOp**)get_list(primary->singleops, i);
			if (sop->letter != NULL) {
				LetterType lt = sop->letter->t;
				if (lt == LetterType::_Dot) {
					op = sop;
				} else if (lt == LetterType::_Mul || lt == LetterType::_RightRight) {
					op = sop;
					Obj* last = *(Obj**)get_list(primary->children, count - 1);
					if (val1 != last) val1 = vt_opeB(thgc, val1, lt, local, NULL);
				} else {
					val1 = GetterB_vt(thgc, val1, local);
					val1 = vt_opeB(thgc, val1, lt, local, NULL);
				}
			}
			if (local->comments != NULL && local->comments->size > 0) {
				CommentLet* cl = *(CommentLet**)get_list(local->comments, local->comments->size - 1);
				SingleB_CommentLet(thgc, cl, op, local);
			}
		}
	}
	return val1;
}

// ============================================================
// Block::exeRangeB
// ============================================================
Obj* Block_exeRangeB(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id, bool typecheck) {
	// C#: letters = new Letter[children.Count];
	self->letters = create_list(thgc, sizeof(ATSSpan*), CType::_ATSSpan);
	fill_list(thgc, self->letters, self->children->size, NULL);
	// kouho check: if local->elem_flag is set and kouhos is null, iterate labelmap
	if (local->elem_flag && local->kouhos == NULL) {
		if (self->labelmap != NULL && self->labelmap->kvs != NULL) {
			for (int k = 0; k < self->labelmap->kvs->size; k++) {
				KV* kv = *(KV**)get_list(self->labelmap->kvs, k);
				Label* lab = (Label*)kv->value;
				if (lab != NULL && lab->letter == local->letter) {
					KouhoSetElem(thgc, local);
				}
			}
		}
	}
	self->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	for (int i = 0; i < self->children->size; i++) {
		Obj* ch = *(Obj**)get_list(self->children, i);
		Obj* val = vt_exeB(thgc, ch, local);
		add_list(thgc, self->rets, (char*)val);
		if (IS_WEG(val)) return val;
		// C#: letters[i] = children[i].letter;
		set_list(self->letters, i, (char*)ch->letter);
	}
	// C#: if (typecheck && type == ObjType.Block) { cls = new Var(); foreach(ret) CheckCVB(cls, ret, Setter); cls = (cls as Var).cls; }
	if (typecheck && self->type == LetterType::OT_Block) {
		Obj* vcls = make_var(thgc, NULL);
		for (int i = 0; i < self->rets->size; i++) {
			Obj* r = *(Obj**)get_list(self->rets, i);
			Obj* chk = CheckCVB_TypeCheck(thgc, vcls, r, CheckType_Setter, local);
			if (chk == NULL) {
				return ErrorA(thgc, LetterType::OT_Error, self->letter, "Block typecheck failed");
			}
		}
		self->cls = (Type*)((Var*)vcls)->cls;
	}
	return (Obj*)self;
}

// ============================================================
// Block::exepB
// ============================================================
Obj* Block_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Block* blk = (Block*)self;
	if (self->type == LetterType::OT_Block) {
		int m = 0;
		Obj* ch0 = *(Obj**)get_list(blk->children, m);
		if (ch0->type == LetterType::OT_Word) {
			if (str_matchA(ch0->letter->text, "T", 1)) {
				m++;
				Obj* val2 = *(Obj**)get_list(blk->children, m);
				Type* type = (Type*)make_variclass(thgc, 0);
			head_t:
				if (val2->type == LetterType::OT_Block) {
					Block* tblk = (Block*)val2;
					if (tblk->children->size == 0) {
						type = (Type*)make_arrtype(thgc, (Obj*)type);
						m++;
						val2 = *(Obj**)get_list(blk->children, m);
						goto head_t;
					} else {
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型は引数が0です");
					}
				}
				*n = 1;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Word) {
					Obj* word = val2;
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					if (val2->type == LetterType::OT_Block) {
						GenericFunction* func = make_genericfunction(thgc, (Obj*)type);
						Block* gblk = (Block*)vt_exeZ(thgc, val2, local);
						for (int i = 0; i < gblk->rets->size; i++) {
							Obj* ret_i = *(Obj**)get_list(gblk->rets, i);
							if (ret_i->type == LetterType::OT_Word) {
								add_mapy(thgc, func->vmap, ret_i->letter->text, (char*)make_variclass(thgc, i));
							} else {
								return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスの引数は名前を指定してください");
							}
						}
						(*n)++;
						val2 = *(Obj**)get_list(primary->children, *n);
						if (val2->type == LetterType::OT_CallBlock) {
							func->draw = (Block*)val2;
							List* blocks = get_blocks(local);
							if (blocks != NULL) {
								for (int i = 0; i < blocks->size; i++) {
									Block* b = *(Block**)get_list(blocks, i);
									add_list(thgc, func->blocks, (char*)b);
								}
							}
							// declareB is empty in C#
							return (Obj*)func;
						}
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックス関数の{|}が宣言されていません");
					} else if (val2->type == LetterType::OT_CallBlock) {
						Function* func = make_function(thgc, (Obj*)type);
						func->draw = (Block*)val2;
						List* blocks = get_blocks(local);
						if (blocks != NULL) {
							for (int i = 0; i < blocks->size; i++) {
								Block* b = *(Block**)get_list(blocks, i);
								add_list(thgc, func->blocks, (char*)b);
							}
						}
						// declareB is empty in C#
						return (Obj*)func;
					}
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックス関数の{|}が宣言されていません");
				}
			}
		}
	}
	Block* blk2 = (Block*)Block_exeRangeB(thgc, blk, 0, blk->children->size, local, true, true);
	if (self->type == LetterType::OT_Bracket) {
		if (blk2->rets != NULL && blk2->rets->size == 1) {
			return *(Obj**)get_list(blk2->rets, 0);
		}
	}
	return (Obj*)blk2;
}

// ============================================================
// Block::exeB
// ============================================================
Obj* Block_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	if (self->type == LetterType::OT_Array)
		return ErrorA(thgc, LetterType::OT_Error, self->letter, "Bコンパイルではこの項目を使いません");
	return Block_exeRangeB(thgc, blk, 0, blk->children->size, local, true, true);
}

// ============================================================
// Block::PrimaryB
// ============================================================
Obj* Block_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Block* blk = (Block*)self;
	if (self->type == LetterType::OT_Bracket) {
		if (val2->type == LetterType::OT_Block) {
			// 配列型宣言: (Type)[]
			if (blk->rets != NULL && blk->rets->size == 1) {
				if (((Block*)val2)->children->size == 0) {
					Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
					// C#: var cls = rets[0] as Type; if (cls != null) return new ArrType(cls);
					if (isTypeObj(ret0)) {
						return (Obj*)make_arrtype(thgc, ret0);
					} else {
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列方の宣言は型を元にしてください");
					}
				} else {
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型の宣言の[]は引数が0です");
				}
			} else {
				return ErrorA(thgc, LetterType::OT_Error, self->letter, "配列方の宣言は1つの型を配列にしてください");
			}
		} else if (val2->type == LetterType::OT_Bracket) {
			// 複数変数宣言: (Type, Type) (name, name) = ...
			Block* block = (Block*)vt_exeZ(thgc, val2, local);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			for (int i = 0; i < block->rets->size; i++) {
				Obj* ret_i = *(Obj**)get_list(block->rets, i);
				if (ret_i->type == LetterType::OT_Word) {
					Obj* cls_i = *(Obj**)get_list(blk->rets, i % blk->rets->size);
					// C#: var cls = rets[i % rets.Count] as Type; if (cls != null)
					if (cls_i->type == LetterType::OT_ModelObj || cls_i->type == LetterType::OT_ClassObj ||
						cls_i->type == LetterType::OT_Generic || cls_i->type == LetterType::OT_ArrType ||
						cls_i->type == LetterType::OT_Var || cls_i->type == LetterType::OT_GeneObj) {
						Variable* variable = make_variable(thgc, cls_i);
						// declareB is empty in C#
						set_list(block->rets, i, (char*)variable);
					} else {
						return ErrorA(thgc, LetterType::OT_Error, blk->letters != NULL ? *(ATSSpan**)get_list(blk->letters, i % blk->rets->size) : self->letter, "複数型の変数宣言は型を指定してください");
					}
				} else {
					return ErrorA(thgc, LetterType::OT_Error, ret_i->letter, "複数の変数の宣言には名前を指定してください");
				}
			}
			return (Obj*)block;
		} else if (val2->type == LetterType::OT_Word) {
			// 変数宣言: (Type) name
			(*n)++;
			Variable* variable = make_variable(thgc, *(Obj**)get_list(blk->rets, 0));
			// declareB is empty in C#
			return (Obj*)variable;
		} else if (val2->type == LetterType::_Left) {
			(*n)++;
			Obj* ch = *(Obj**)get_list(primary->children, *n);
			if (blk->rets != NULL && blk->rets->size == 1) {
				return PrimaryB_vt(thgc, *(Obj**)get_list(blk->rets, 0), n, local, primary, ch);
			}
			return ErrorA(thgc, LetterType::OT_Error, self->letter, "この()は1つしかとれない");
		} else if (val2->type == LetterType::_Dot) {
			if (blk->rets != NULL && blk->rets->size == 1) {
				return PrimaryB_vt(thgc, *(Obj**)get_list(blk->rets, 0), n, local, primary, val2);
			}
			return ErrorA(thgc, LetterType::OT_Error, self->letter, "この()は1つしかとれない");
		}
		return ErrorA(thgc, LetterType::OT_NG, val2->letter, "()の後が必要です");
	} else if (self->type == LetterType::OT_Block || self->type == LetterType::OT_Array) {
		if (val2->type == LetterType::OT_Block) {
			// 配列アクセス: arr[index]
			add_list(thgc, local->calls, (char*)KouhoSet2);
			Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
			local->calls->size--;
			if (IS_WEG(val)) return val;
			Block* block = (Block*)val;
			if (block->rets == NULL || block->rets->size != 1)
				return ErrorA(thgc, LetterType::OT_Error, block->letters != NULL && block->letters->size > 0 ? *(ATSSpan**)get_list(block->letters, 0) : val2->letter, "配列の引数は1つだけです");
			Obj* me = self;
			Obj* idx = *(Obj**)get_list(block->rets, 0);
			if (idx->type == LetterType::OT_Number) {
				Number* num = (Number*)idx;
				me = *(Obj**)get_list(blk->rets, num->value);
			} else if (idx->type == LetterType::OT_StrObj) {
				StrObj* str = (StrObj*)idx;
				char* labval = get_mapy(blk->labelmap, str->value);
				if (labval != NULL) {
					Label* lab = (Label*)labval;
					me = *(Obj**)get_list(blk->rets, lab->n);
				} else {
					return ErrorA(thgc, LetterType::OT_Error, block->letters != NULL && block->letters->size > 0 ? *(ATSSpan**)get_list(block->letters, 0) : val2->letter, "配列の引数は文字列か整数です");
				}
			} else {
				return ErrorA(thgc, LetterType::OT_Error, block->letters != NULL && block->letters->size > 0 ? *(ATSSpan**)get_list(block->letters, 0) : val2->letter, "配列の引数は文字列か整数です");
			}
			(*n)++;
			return me;
		} else if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				// kouho check
				if (val2->letter == local->letter && local->kouhos == NULL) {
					local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
					add_kouho(thgc, local->kouhos, createString(thgc, "length", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
					add_kouho(thgc, local->kouhos, createString(thgc, "Sort", 4, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
					add_kouho(thgc, local->kouhos, createString(thgc, "Range", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
				}
				String* wname = val2->letter->text;
				(*n)++;
				if (str_matchA(wname, "length", 6)) {
					// C#: return new Number(rets.Count) { cls = local.Int }
					Obj* num = make_number(thgc, val2->letter);
					((Number*)num)->value = blk->rets != NULL ? blk->rets->size : 0;
					((Val*)num)->cls = (Obj*)local->Int;
					return num;
				} else if (str_matchA(wname, "Sort", 4)) {
					Obj* val2b = *(Obj**)get_list(primary->children, *n);
					if (val2b->type == LetterType::OT_Bracket) {
						(*n)++;
						Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2b, local), local);
						if (IS_WEG(val)) return val;
						Block* blk1 = (Block*)val;
						Obj* ret0 = *(Obj**)get_list(blk1->rets, 0);
						if (ret0->type == LetterType::OT_Function) {
							// SortB: C# block.SortB(func, local)
							FuncTypeC* ftype = make_functype(thgc, local->Bool);
							Obj* ret0cls = ((Val*)*(Obj**)get_list(blk->rets, 0))->cls;
							add_list(thgc, ftype->draws, (char*)ret0cls);
							add_list(thgc, ftype->draws, (char*)ret0cls);
							Obj* ret = CheckCVB_Func_TypeCheck(thgc, ftype, (Function*)ret0, CheckType_Setter, local);
							if (IS_WEG(ret)) return ret;
							return (Obj*)blk;
						} else {
							return ErrorA(thgc, LetterType::OT_Error, val2b->letter, "Sortの引数は関数しかだめです");
						}
					} else {
						return ErrorA(thgc, LetterType::OT_Error, val2b->letter, "Sortの()がありません");
					}
				} else if (str_matchA(wname, "Range", 5)) {
					Obj* val2b = *(Obj**)get_list(primary->children, *n);
					if (val2b->type == LetterType::OT_Bracket) {
						Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2b, local), local);
						if (IS_WEG(val)) return val;
						Block* rblk = (Block*)val;
						if (rblk->rets->size == 0) {
							// ok
						} else if (rblk->rets->size == 1) {
							Obj* r0 = *(Obj**)get_list(rblk->rets, 0);
							if (r0->type != LetterType::OT_Number)
								return ErrorA(thgc, LetterType::OT_Error, rblk->letters != NULL ? *(ATSSpan**)get_list(rblk->letters, 0) : val2b->letter, "Rangeの引数1は整数です");
						} else if (rblk->rets->size == 2) {
							Obj* r0 = *(Obj**)get_list(rblk->rets, 0);
							Obj* r1 = *(Obj**)get_list(rblk->rets, 1);
							if (r0->type != LetterType::OT_Number)
								return ErrorA(thgc, LetterType::OT_Error, rblk->letters != NULL ? *(ATSSpan**)get_list(rblk->letters, 0) : val2b->letter, "Rangeの引数1は整数です");
							if (r1->type != LetterType::OT_Number)
								return ErrorA(thgc, LetterType::OT_Error, rblk->letters != NULL ? *(ATSSpan**)get_list(rblk->letters, 0) : val2b->letter, "Rangeの引数2は整数です");
						} else {
							return ErrorA(thgc, LetterType::OT_Error, val2b->letter, "Rangeの引数は0,1,2です");
						}
						(*n)++;
						return (Obj*)blk;
					} else {
						return ErrorA(thgc, LetterType::OT_Error, val2b->letter, "Rangeの()がありません");
					}
				}
				return ErrorA(thgc, LetterType::OT_NG, val2->letter, "配列には指定名は宣言されていません");
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が必要です");
		}
		return ErrorA(thgc, LetterType::OT_NG, val2->letter, "[]の後が必要です");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "不正なエラー");
}

// ============================================================
// Block::SelfB
// ============================================================
Obj* Block_SelfB(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	if (blk->rets == NULL) return self;
	for (int i = 0; i < blk->rets->size; i++) {
		Obj** pp = (Obj**)get_list(blk->rets, i);
		*pp = SelfB_vt(thgc, *pp, local);
		if (IS_WEG(*pp)) return *pp;
	}
	return self;
}

// ============================================================
// Block::GetterB
// ============================================================
Obj* Block_GetterB(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	if (blk->rets == NULL) return self;
	for (int i = 0; i < blk->rets->size; i++) {
		Obj** pp = (Obj**)get_list(blk->rets, i);
		*pp = GetterB_vt(thgc, GetterB_vt(thgc, *pp, local), local);
		if (IS_WEG(*pp)) return *pp;
	}
	return self;
}

// ============================================================
// Block::opeB — C# Block.EqualB / InB / OpeB
// ============================================================
Obj* Block_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	Block* blk = (Block*)self;
	if (key == LetterType::_Equal || key == LetterType::_Colon) {
		// EqualB
		if (val2->type == LetterType::OT_Block) {
			Block* blk2 = (Block*)val2;
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* ri = *(Obj**)get_list(blk->rets, i);
				((Variable*)ri)->value = *(Obj**)get_list(blk2->rets, i % blk2->rets->size);
			}
		} else {
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* ri = *(Obj**)get_list(blk->rets, i);
				vt_opeB(thgc, ri, key, local, val2);
			}
		}
		return self;
	}
	if (key == LetterType::_In) {
		// InB
		if (val2->type == LetterType::OT_Block) {
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* it = make_iterator(thgc);
				((Iterator*)it)->value = (Block*)val2;
				((Iterator*)it)->m = i;
				Obj* ri = *(Obj**)get_list(blk->rets, i);
				((Variable*)ri)->value = it;
			}
			return self;
		} else {
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "イテレーターには配列を代入してください");
		}
	}
	// OpeB
	if (self->type == LetterType::OT_Bracket) {
		if (blk->rets != NULL && blk->rets->size == 1) {
			Obj* r0 = *(Obj**)get_list(blk->rets, 0);
			return vt_opeB(thgc, r0, key, local, val2);
		} else {
			return ErrorA(thgc, LetterType::OT_Error, self->letter, "()は1つだけです");
		}
	} else {
		return ErrorA(thgc, LetterType::OT_Error, self->letter, "演算子は()だけです");
	}
}

// ============================================================
// CallBlock::exepB
// ============================================================
Obj* CallBlock_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Block::analyse - C# Block.analyse(Local)
// ============================================================
bool Block_analyse(ThreadGC* thgc, Block* self, Master* local) {
	local->comment_flag = self->comment;
	bool check = true;
	for (int i = 0; i < self->children->size; i++) {
		if (((CodeStatus*)get_list(self->statuses, i))->status == FS_Yet) {
			Obj* child = *(Obj**)get_list(self->children, i);
			Obj* val = vt_exeB(thgc, child, local);
			// TagBlock obj.obj.n==0: apply GetterB (C# obj.obj.n, not obj.n)
			if (self->obj && self->obj->type == LetterType::OT_TagBlock && self->obj->n_c == 0) {
				val = GetterB_vt(thgc, val, local);
			}
			if (val->type == LetterType::OT_Wait) {
				((CodeStatus*)get_list(self->statuses, i))->status = FS_Yet;
				check = false;
			}
			else if (val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) {
				((CodeStatus*)get_list(self->statuses, i))->status = FS_Error;
				((CodeStatus*)get_list(self->statuses, i))->letter = val->letter;
				((CodeStatus*)get_list(self->statuses, i))->error = NULL;
			}
			else {
				((CodeStatus*)get_list(self->statuses, i))->status = FS_Finish;
				if (self->obj_n == 0) {
					if (self->obj && self->obj->type == LetterType::OT_TagBlock) {
						if (i == 0) {
							if (val->type == LetterType::OT_ElemObj || self->obj->vt == &vt_ElemType) {
								continue;
							}
						}
						else {
							Label* lab = (Label*)get_mapyn(self->labelmapn, (char*)(intptr_t)i);
							if (lab != NULL) {
								Obj* ret = NULL;
								String* lname = lab->name;
								// label名でTypeCheckを分岐
								if (str_equals("left", lname) || str_equals("top", lname) ||
									str_equals("w", lname) || str_equals("width", lname) ||
									str_equals("h", lname) || str_equals("height", lname)) {
									ret = CheckCVB_TypeCheck(thgc, local->FloatT, val, CheckType_Setter, local);
								}
								else if (str_equals("z", lname)) {
									ret = CheckCVB_TypeCheck(thgc, local->Int, val, CheckType_Setter, local);
								}
								else if (str_equals("p", lname) || str_equals("size", lname)) {
									ret = CheckCVB_TypeCheck(thgc, (Obj*)make_arrtype(thgc, (Obj*)local->FloatT), val, CheckType_Setter, local);
								}
								else if (str_equals("gokeydown", lname) || str_equals("backkeydown", lname) ||
									str_equals("gomouseup", lname) || str_equals("gomousedown", lname) ||
									str_equals("gomousemove", lname) || str_equals("backmousemove", lname) ||
									str_equals("keydown", lname) || str_equals("keyup", lname) ||
									str_equals("keyinput", lname)) {
									// C#: ret = TypeCheck.CheckCVB(new CommentType(), val, Setter, local)
									// CommentType未ポート — stub
									ret = val;
								}
								else {
									// xtype, ytype, ptype, pos, position, c, color, b, background — no type check (break in C#)
								}
								if (ret != NULL && ret->type == LetterType::OT_Wait) {
									((CodeStatus*)get_list(self->statuses, i))->status = FS_Yet;
								}
								else if (ret != NULL && (ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG)) {
									((CodeStatus*)get_list(self->statuses, i))->status = FS_Error;
									((CodeStatus*)get_list(self->statuses, i))->letter = ret->letter;
									((CodeStatus*)get_list(self->statuses, i))->error = NULL;
									continue;
								}
							}
						}
					}
					else if (self->obj && self->obj->type == LetterType::OT_SignalFunction) {
						if (i == self->children->size - 1) {
							if (self->vmapA == NULL || self->vmapA->kvs == NULL || self->vmapA->kvs->size != 2) {
								((CodeStatus*)get_list(self->statuses, i))->status = FS_Error;
								((CodeStatus*)get_list(self->statuses, i))->letter = self->letter;
								((CodeStatus*)get_list(self->statuses, i))->error = createString(thgc, "signal\xe9\x96\xa2\xe6\x95\xb0\xe3\x81\xae\xe5\xbc\x95\xe6\x95\xb0\xe3\x81\xaf\xef\xbc\x92\xe3\x81\xa4\xe3\x81\xa7\xe3\x81\x99\xe3\x80\x82", 42, 1);
							}
							// vmapAの最初の2つをStockに初期化
							if (self->vmapA != NULL && self->vmapA->kvs != NULL && self->vmapA->kvs->size >= 2) {
								KV* kv0 = (KV*)*get_list2(self->vmapA->kvs, 0);
								kv0->value = (char*)make_stock(thgc);
								KV* kv1 = (KV*)*get_list2(self->vmapA->kvs, 1);
								kv1->value = (char*)make_stock(thgc);
							}
						}
					}
				}
				else if (self->obj_n == 1) {
					if (self->obj && self->obj->type == LetterType::OT_TagBlock) continue;
					if (self->obj && (self->obj->type == LetterType::OT_ClassObj ||
						self->obj->type == LetterType::OT_GeneObj ||
						self->obj->type == LetterType::OT_ModelObj ||
						self->obj->type == LetterType::OT_Model)) continue;
					if (self->obj && self->obj->type == LetterType::OT_Switch) {
						Label* nextlab = (Label*)get_mapyn(self->labelmapn, (char*)(intptr_t)(i + 1));
						if (i == self->children->size - 1 || nextlab != NULL) {
							Obj* ret = CheckB_TypeCheck(thgc, self->obj, val, CheckType_Finish, local);
							if (ret->type == LetterType::OT_Wait) ((CodeStatus*)get_list(self->statuses, i))->status = FS_Yet;
							else if (ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) {
								((CodeStatus*)get_list(self->statuses, i))->status = FS_Error;
								((CodeStatus*)get_list(self->statuses, i))->letter = ret->letter;
								continue;
							}
						}
						local->step++;
						continue;
					}
					// 通常のブロック終了チェック
					Label* endlab = (Label*)get_mapy(self->branchmap, createString(thgc, "end", 3, 1));
					bool no_end = (endlab == NULL || endlab->n == self->children->size);
					Label* nextbranch = (Label*)get_mapyn(self->branchmapn, (char*)(intptr_t)(i + 1));
					if (i == self->children->size - 1 || (no_end && nextbranch != NULL)) {
						Obj* ret = CheckB_TypeCheck(thgc, self->obj, val, CheckType_Finish, local);
						if (ret->type == LetterType::OT_Wait) ((CodeStatus*)get_list(self->statuses, i))->status = FS_Yet;
						else if (ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) {
							((CodeStatus*)get_list(self->statuses, i))->status = FS_Error;
							((CodeStatus*)get_list(self->statuses, i))->letter = ret->letter;
							continue;
						}
					}
				}
				local->step++;
			}
		}
	}
	return check;
}

// ============================================================
// allocate_statuses - CodeStatus配列を確保・初期化
// ============================================================
void allocate_statuses(ThreadGC* thgc, Block* blk) {
	int count = blk->children->size;
	CodeStatus cs_init = { FS_Yet, NULL, NULL };
	blk->statuses = create_list(thgc, sizeof(CodeStatus), CType::_Struct);
	fill_list_v(thgc, blk->statuses, count, &cs_init);
}

// ============================================================
// CallBlock::exeB
// ============================================================
Obj* CallBlock_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	// 第1パス: analblocks の各 block を analyse する
	for (int i = 0; i < local->analblocks->size; i++) {
		Block* blk = *(Block**)get_list(local->analblocks, i);
		allocate_statuses(thgc, blk);
	}
	for (;;) {
		local->step = 0;
		bool any_remaining = false;
		for (int i = 0; i < local->analblocks->size; i++) {
			Block* block = *(Block**)get_list(local->analblocks, i);
			if (block->finished) continue;
			if (block->blocks == NULL || block->blocks->size == 0) continue;
			any_remaining = true;
			set_blocks(thgc, local, block->blocks);
			local->obj = block->obj;
			bool ret = Block_analyse(thgc, block, local);
			// local->blockslist.RemoveAt(last)
			local->blockslist->size--;
			if (ret) {
				block->finished = true;
			}
		}
		if (!any_remaining || local->step == 0) break;
	}

	// 第2パス: secondB
	local->secondB = true;
	for (int i = 0; i < local->analblocks->size; i++) {
		Block* block = *(Block**)get_list(local->analblocks, i);
		if (block->blocks == NULL || block->blocks->size == 0) continue;
		allocate_statuses(thgc, block);
		set_blocks(thgc, local, block->blocks);
		local->obj = block->obj;
		// returns チェック
		for (int j = 0; j < block->returns_list->size; j++) {
			Obj* re_val = *(Obj**)get_list(block->returns_list, j);
			Obj* re = CheckB_TypeCheck(thgc, block->obj, re_val, CheckType_Return, local);
			if (re->type == LetterType::OT_Wait) {
				if (block->statuses && i < block->statuses->size) ((CodeStatus*)get_list(block->statuses, i))->status = FS_Yet;
			}
			else if (re->type == LetterType::OT_Error || re->type == LetterType::OT_NG) {
				if (block->statuses && i < block->statuses->size) {
					((CodeStatus*)get_list(block->statuses, i))->status = FS_Error;
					((CodeStatus*)get_list(block->statuses, i))->letter = re->letter;
				}
				continue;
			}
		}
		bool ret = Block_analyse(thgc, block, local);
		// local->blockslist.RemoveAt(last)
		local->blockslist->size--;
		// C#: if (block.obj.n == 0 || block.obj.obj.type == ObjType.TagBlock && local.kouhos == null)
		ObjBlock* objblk = (ObjBlock*)block->obj;
		if (objblk->n == 0 || (objblk->obj->type == LetterType::OT_TagBlock && local->kouhos == NULL)) {
			List* lvals = map_values_list(thgc, block->labelmap);
			for (int k = 0; k < lvals->size; k++) {
				Label* lab = *(Label**)get_list(lvals, k);
				if (lab->letter == local->letter) {
					KouhoSetElem(thgc, local);
				}
			}
		}
		if (ret) {
			remove_list(thgc, local->analblocks, i);
			i--;
		}
	}
	local->secondB = false;
	return self;
}

// Number, FloatVal, Variable B-pass演算、GeneObj.DotB、Stock は obj2b.h (Obj2B.cs対応) に移動済み

// ============================================================
// B_initVTables - B-pass vtable初期化 (Block 1)
// ============================================================
void B_initVTables() {
	// --- Operator ---
	vt_Operator.exeB = Operator_exeB;

	// --- Primary ---
	vt_Primary.exeB = Primary_exeB;

	// --- Block / Bracket / Call1 / Call2 ---
	vt_Block.exeB = Block_exeB;
	vt_Block.exepB = Block_exepB;
	vt_Block.PrimaryB = Block_PrimaryB;
	vt_Block.SelfB = Block_SelfB;
	vt_Block.GetterB = Block_GetterB;
	vt_Block.opeB = Block_opeB;
	// Block_Array uses same functions
	vt_Block_Array.exeB = Block_exeB;
	vt_Block_Array.exepB = Block_exepB;
	vt_Block_Array.PrimaryB = Block_PrimaryB;
	vt_Block_Array.SelfB = Block_SelfB;
	vt_Block_Array.GetterB = Block_GetterB;
	vt_Block_Array.opeB = Block_opeB;

	// --- CallBlock ---
	vt_CallBlock.exepB = CallBlock_exepB;
	vt_CallBlock.exeB = CallBlock_exeB;

	// Number.opeB, FloatVal.opeB, Variable.opeB, GeneObj.DotB → obj2b.h B2_initVTables に移動済み
}

// ============================================================
// kouho_contains: check if a key already exists in kouhos list
// ============================================================
inline bool kouho_contains(List* kouhos, String* key) {
	if (kouhos == NULL) return false;
	for (int i = 0; i < kouhos->size; i++) {
		KV* kv = *(KV**)get_list(kouhos, i);
		if (str_equals(kv->key, key)) return true;
	}
	return false;
}

// str_compare: lexicographic comparison of two String*. Returns <0, 0, >0.
inline int str_compare(String* a, String* b) {
	if (a == b) return 0;
	if (a == NULL) return -1;
	if (b == NULL) return 1;
	int minlen = a->size < b->size ? a->size : b->size;
	for (int i = 0; i < minlen; i++) {
		int ca = GetChar(a, i);
		int cb = GetChar(b, i);
		if (ca != cb) return ca - cb;
	}
	return a->size - b->size;
}

// ============================================================
// sort_kouhos: sort kouhos list by key (insertion sort for small lists)
// ============================================================
inline void sort_kouhos(List* kouhos) {
	if (kouhos == NULL || kouhos->size <= 1) return;
	for (int i = 1; i < kouhos->size; i++) {
		KV* tmp = *(KV**)get_list(kouhos, i);
		int j = i - 1;
		while (j >= 0) {
			KV* kj = *(KV**)get_list(kouhos, j);
			if (str_compare(kj->key, tmp->key) <= 0) break;
			set_list(kouhos, j + 1, (char*)kj);
			j--;
		}
		set_list(kouhos, j + 1, (char*)tmp);
	}
}

// ============================================================
// KouhoSet: C# local.KouhoSet()
// ============================================================
void KouhoSet(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	bool reach = false;
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->vmapA != NULL && block->vmapA->kvs != NULL) {
			for (int ki = 0; ki < block->vmapA->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->vmapA->kvs, ki);
				if (kouho_contains(local->kouhos, kv->key)) continue;
				add_kouho(thgc, local->kouhos, kv->key, (Obj*)kv->value);
			}
		}
		if (block->obj_n == 1) {
			if (block->obj != NULL && block->obj->type == LetterType::OT_Function) {
				reach = true;
				add_kouho(thgc, local->kouhos, createString(thgc, "return", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Return, NULL));
			} else if (!reach && block->obj != NULL &&
				(block->obj->type == LetterType::OT_For || block->obj->type == LetterType::OT_Switch || block->obj->type == LetterType::OT_While)) {
				add_kouho(thgc, local->kouhos, createString(thgc, "continue", 8, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Continue, NULL));
				add_kouho(thgc, local->kouhos, createString(thgc, "break", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Break, NULL));
			}
		}
	}
	add_kouho(thgc, local->kouhos, createString(thgc, "class", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Class, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "var", 3, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Class, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "if", 2, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_If, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "for", 3, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_For, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "while", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_While, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "switch", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Switch, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "goto", 4, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Goto, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "gene", 4, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Gene, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "model", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Model, NULL));
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSet2: C# local.KouhoSet2()
// ============================================================
void KouhoSet2(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	bool reach = false;
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->vmapA != NULL && block->vmapA->kvs != NULL) {
			for (int ki = 0; ki < block->vmapA->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->vmapA->kvs, ki);
				if (kouho_contains(local->kouhos, kv->key)) continue;
				add_kouho(thgc, local->kouhos, kv->key, (Obj*)kv->value);
			}
		}
		if (block->obj_n == 1) {
			if (block->obj != NULL && block->obj->type == LetterType::OT_Function) {
				reach = true;
				add_kouho(thgc, local->kouhos, createString(thgc, "return", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Return, NULL));
			} else if (!reach && block->obj != NULL &&
				(block->obj->type == LetterType::OT_For || block->obj->type == LetterType::OT_Switch || block->obj->type == LetterType::OT_While)) {
				add_kouho(thgc, local->kouhos, createString(thgc, "continue", 8, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Continue, NULL));
				add_kouho(thgc, local->kouhos, createString(thgc, "break", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_Break, NULL));
			}
		}
	}
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSetClaGen: C# local.KouhoSetClaGen()
// ============================================================
void KouhoSetClaGen(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->vmapA != NULL && block->vmapA->kvs != NULL) {
			for (int ki = 0; ki < block->vmapA->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->vmapA->kvs, ki);
				Obj* val = (Obj*)kv->value;
				if (val->type == LetterType::OT_ClassObj || val->type == LetterType::OT_Generic) {
					if (kouho_contains(local->kouhos, kv->key)) continue;
					add_kouho(thgc, local->kouhos, kv->key, val);
				}
			}
		}
	}
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSetType: C# local.KouhoSetType()
// ============================================================
void KouhoSetType(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->vmapA != NULL && block->vmapA->kvs != NULL) {
			for (int ki = 0; ki < block->vmapA->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->vmapA->kvs, ki);
				Obj* val = (Obj*)kv->value;
				// C#: kv.Value is Type || kv.Value.type == ObjType.Generic
				if (val->type == LetterType::OT_ModelObj || val->type == LetterType::OT_ClassObj || val->type == LetterType::OT_Generic) {
					if (kouho_contains(local->kouhos, kv->key)) continue;
					add_kouho(thgc, local->kouhos, kv->key, val);
				}
			}
		}
	}
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSetFunction: C# local.KouhoSetFunction()
// ============================================================
void KouhoSetFunction(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->vmapA != NULL && block->vmapA->kvs != NULL) {
			for (int ki = 0; ki < block->vmapA->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->vmapA->kvs, ki);
				Obj* val = (Obj*)kv->value;
				if (val->type == LetterType::OT_Function) {
					if (kouho_contains(local->kouhos, kv->key)) continue;
					add_kouho(thgc, local->kouhos, kv->key, val);
				}
			}
		}
	}
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSetLabel: C# local.KouhoSetLabel()
// ============================================================
void KouhoSetLabel(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->labelmap != NULL && block->labelmap->kvs != NULL) {
			for (int ki = 0; ki < block->labelmap->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->labelmap->kvs, ki);
				if (kouho_contains(local->kouhos, kv->key)) continue;
				add_kouho(thgc, local->kouhos, kv->key, make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
			}
		}
		// C#: if (obj.obj.type == ObjType.Function) return;
		if (local->obj != NULL && local->obj->type == LetterType::OT_Function) return;
	}
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSetModelGene: C# local.KouhoSetModelGene()
// ============================================================
void KouhoSetModelGene(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->vmapA != NULL && block->vmapA->kvs != NULL) {
			for (int ki = 0; ki < block->vmapA->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->vmapA->kvs, ki);
				Obj* val = (Obj*)kv->value;
				if (val->type == LetterType::OT_ModelObj || val->type == LetterType::OT_GeneObj) {
					if (kouho_contains(local->kouhos, kv->key)) continue;
					add_kouho(thgc, local->kouhos, kv->key, val);
				}
			}
		}
	}
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSetSignal: C# local.KouhoSetSignal()
// ============================================================
void KouhoSetSignal(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	List* blocks = get_blocks(local);
	if (blocks == NULL) return;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		if (block->vmapA != NULL && block->vmapA->kvs != NULL) {
			for (int ki = 0; ki < block->vmapA->kvs->size; ki++) {
				KV* kv = *(KV**)get_list(block->vmapA->kvs, ki);
				Obj* val = (Obj*)kv->value;
				if (val->type == LetterType::OT_ModelObj || val->type == LetterType::OT_GeneObj) {
					if (kouho_contains(local->kouhos, kv->key)) continue;
					add_kouho(thgc, local->kouhos, kv->key, val);
				}
			}
		}
	}
	sort_kouhos(local->kouhos);
}

// ============================================================
// KouhoSetElem: C# local.KouhoSetElem()
// ============================================================
void KouhoSetElem(ThreadGC* thgc, Master* local) {
	local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
	add_kouho(thgc, local->kouhos, createString(thgc, "left", 4, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "top", 3, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "z", 1, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "p", 1, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "point", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "w", 1, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "width", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "h", 1, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "height", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "size", 4, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "xtype", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "ytype", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "ptype", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "pos", 3, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "anim", 4, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "c", 1, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "color", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "b", 1, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "background", 10, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "a", 1, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "align", 5, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "gokeydown", 9, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "backkeydown", 11, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "gomousedown", 11, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "backmousedown", 13, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "gomousemove", 11, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	add_kouho(thgc, local->kouhos, createString(thgc, "backmousemove", 13, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
	sort_kouhos(local->kouhos);
}
