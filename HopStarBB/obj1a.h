// ============================================================
// obj2a.h - A解析パス (vtable dispatch版)
// ============================================================
#include <string>

// 前方宣言 - vtable実装関数
Obj* Operator_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Primary_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Primary_NextA(ThreadGC* thgc, Primary* primary, Master* local, int* i, Obj** val1);
Obj* Block_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Block_exeRangeA(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id);
Obj* Block_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Block_SelfA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_GetterA(ThreadGC* thgc, Obj* self, Master* local);
Obj* CallBlock_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* CallBlock_exeA(ThreadGC* thgc, Obj* self, Master* local);
// Word, Variable, Function, Number の前方宣言は obj2a.h に移動済み

// ============================================================
// ヘルパー関数
// ============================================================

bool is_operator_typeA(LetterType t) {
	return t == LetterType::_Equal || t == LetterType::_Colon ||
		t == LetterType::_LessThan || t == LetterType::_LessEqual ||
		t == LetterType::_MoreThan || t == LetterType::_MoreEqual ||
		t == LetterType::_EqualEqual || t == LetterType::_NotEqual ||
		t == LetterType::_Plus || t == LetterType::_Minus ||
		t == LetterType::_Mul || t == LetterType::_Div ||
		t == LetterType::_Mod || t == LetterType::_And || t == LetterType::_Percent;
}

Obj* ErrorA(ThreadGC* thgc, LetterType type, ATSSpan* letter, const char* msg = NULL) {
	Obj* err = make_cobj(thgc, CType::_CObj, type, letter);
	if (msg && letter) {
		letter->error = createString(thgc, msg, (int)strlen(msg), 1);
		letter->underlineColor = 0xFF0000FF;
	}
	return err;
}

bool str_matchA(String* s, const char* lit, int len) {
	if (s == NULL) return false;
	if (s->size != len) return false;
	if (s->esize == 1) return memcmp(s->data, lit, len) == 0;
	return false;
}

// ============================================================
// pushBlock - ブロックをlocal.blocksに追加し、childblocks・blocksを設定
// C#: local.blocks[^1].childblocks.Add(blk);
//     local.blocks.Add(blk);
//     for (i = 0; i < local.blocks.Count; i++) blk.blocks.Add(local.blocks[i]);
// ============================================================
void pushBlock(ThreadGC* thgc, Master* local, Block* blk) {
	List* blocks = get_blocks(local);
	if (blocks->size > 0) {
		Block* parentBlk = *(Block**)get_list(blocks, blocks->size - 1);
		if (!parentBlk->childblocks) parentBlk->childblocks = create_list(thgc, sizeof(Block*), CType::_Block);
		add_list(thgc, parentBlk->childblocks, (char*)blk);
	}
	add_list(thgc, blocks, (char*)blk);
	blk->blocks = create_list(thgc, sizeof(Block*), CType::_Block);
	for (int bi = 0; bi < blocks->size; bi++) {
		add_list(thgc, blk->blocks, *get_list(blocks, bi));
	}
}

// ============================================================
// declareA
// ============================================================
Obj* declareA(ThreadGC* thgc, Master* local, String* name, Obj* obj) {
	List* blocks = get_blocks(local);
	Block* blk = *(Block**)get_list(blocks, blocks->size - 1);
	char* existing = get_mapy(blk->vmapA, name);
	if (existing != NULL) {
		return ErrorA(thgc, LetterType::OT_Error, ((Obj*)existing)->letter, "変数はすでに宣言されています");
	}
	add_mapy(thgc, blk->vmapA, name, (char*)obj);
	return obj;
}

// ============================================================
// declareA2 - C# Local.declareA2(CallBlock call, Obj obj)
// ============================================================
static int declareA2_N = 0;
void declareA2(ThreadGC* thgc, Master* local, CallBlock* call, Obj* obj) {
	((Obj*)call)->number_val = declareA2_N;
	add_mapyn(thgc, local->vmapA2, (char*)(intptr_t)declareA2_N, (char*)obj);
	declareA2_N++;
}

// ============================================================
// declareRA
// ============================================================
Obj* declareRA(ThreadGC* thgc, Master* local, String* name, Obj* obj) {
	List* blocks = get_blocks(local);
	Block* blk = *(Block**)get_list(blocks, blocks->size - 1);
	// C# writes to vmapR (separate from vmapA) for rename duplicate detection
	if (get_mapy(blk->vmapR, name) != NULL) {
		return ErrorA(thgc, LetterType::OT_Error, ((Obj*)get_mapy(blk->vmapR, name))->letter, "変数はすでに宣言されています");
	}
	add_mapy(thgc, blk->vmapR, name, (char*)obj);
	return obj;
}

// ============================================================
// getA
// ============================================================
Obj* getA(ThreadGC* thgc, Master* local, String* name) {
	List* blocks = get_blocks(local);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* blk = *(Block**)get_list(blocks, i);
		char* val = get_mapy(blk->vmapA, name);
		if (val != NULL) return (Obj*)val;
	}
	return make_cobj(thgc, CType::_CObj, LetterType::_None, NULL);
}

// ============================================================
// Word::GetterA
// ============================================================
// Word, Variable, Function, Number の A-pass関数は obj2a.h (Obj2A.cs対応) に移動済み

// ============================================================
// Operator::exeA
// ============================================================
Obj* Operator_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	LetterType op = self->type;
	if (op == LetterType::_Equal || op == LetterType::_Colon || op == LetterType::_In) {
		Obj* val2 = GetterA_vt(thgc, vt_exeA(thgc, *(Obj**)get_list(self->children, 1), local), local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		Obj* val1 = SelfA_vt(thgc, vt_exeA(thgc, *(Obj**)get_list(self->children, 0), local), local);
		if (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG) return val1;
		return val1;
	}
	{
		Obj* val1 = GetterA_vt(thgc, vt_exeA(thgc, *(Obj**)get_list(self->children, 0), local), local);
		if (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG) return val1;
		Obj* val2 = GetterA_vt(thgc, vt_exeA(thgc, *(Obj**)get_list(self->children, 1), local), local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		return val1;
	}
}

// ============================================================
// Primary::NextA
// ============================================================
Obj* Primary_NextA(ThreadGC* thgc, Primary* primary, Master* local, int* i, Obj** val1) {
	Obj* child = *(Obj**)get_list(primary->children, *i);
	switch (child->type) {
		case LetterType::OT_Comment2:
			(*i)++;
			break;
		case LetterType::OT_Htm:
		case LetterType::OT_TagBlock:
			vt_exepA(thgc, child, i, local, primary);
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
				*val1 = vt_exepA(thgc, child, i, local, primary);
				(*i)++;
			} else {
				*val1 = GetterA_vt(thgc, *val1, local);
				if ((*val1)->type == LetterType::OT_Wait || (*val1)->type == LetterType::OT_Error || (*val1)->type == LetterType::OT_NG) return *val1;
				*val1 = PrimaryA_vt(thgc, *val1, i, local, primary, vt_exepA(thgc, child, i, local, primary));
			}
			break;
		case LetterType::_Dot:
		case LetterType::_Left:
		case LetterType::_Right:
			*val1 = GetterA_vt(thgc, *val1, local);
			if ((*val1)->type == LetterType::OT_Wait || (*val1)->type == LetterType::OT_Error || (*val1)->type == LetterType::OT_NG) return *val1;
			*val1 = PrimaryA_vt(thgc, *val1, i, local, primary, child);
			break;
		case LetterType::OT_CallBlock:
		case LetterType::OT_Block:
			if (*val1 == NULL) {
				*val1 = vt_exepA(thgc, child, i, local, primary);
				(*i)++;
			} else {
				*val1 = GetterA_vt(thgc, *val1, local);
				if ((*val1)->type == LetterType::OT_Wait || (*val1)->type == LetterType::OT_Error || (*val1)->type == LetterType::OT_NG) return *val1;
				*val1 = PrimaryA_vt(thgc, *val1, i, local, primary, child);
			}
			break;
		default:
			break;
	}
	return *val1;
}

// ============================================================
// Primary::exeA (vtable signature: Obj* self)
// ============================================================
Obj* Primary_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	Primary* pself = (Primary*)self;
	Obj* val1 = NULL;
	for (int i = 0; i < pself->children->size - 1;) {
		Primary_NextA(thgc, pself, local, &i, &val1);
		if (val1 != NULL && (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG)) return val1;
	}
	if (val1 == NULL) val1 = *(Obj**)get_list(pself->children, pself->children->size - 1);

	// singleops処理
	for (int i = pself->singleops->size - 1; i >= 0; i--) {
		SingleOp* sop = *(SingleOp**)get_list(pself->singleops, i);
		if (sop->letter != NULL) {
			LetterType lt = sop->letter->t;
			if (lt == LetterType::_Dot) {
				// op = sop; (dot)
			} else if (lt == LetterType::_Mul || lt == LetterType::_Div || lt == LetterType::_RightRight) {
				Obj* last = *(Obj**)get_list(pself->children, pself->children->size - 1);
				if (val1 != last) val1 = vt_opeA(thgc, val1, lt, local, NULL);
			} else {
				val1 = GetterA_vt(thgc, val1, local);
				val1 = vt_opeA(thgc, val1, lt, local, NULL);
			}
		}
	}
	return val1;
}

// ============================================================
// Block::exeA (vtable signature: Obj* self)
// ============================================================
Obj* Block_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	if (blk->type == LetterType::OT_Array) return (Obj*)blk;
	return Block_exeRangeA(thgc, blk, 0, blk->children->size, local, true);
}

// ============================================================
// Block::exeRangeA (vtable dispatch で子を実行)
// ============================================================
Obj* Block_exeRangeA(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id) {
	int count = self->children->size;
	self->letters = create_list(thgc, sizeof(ATSSpan*), CType::_List);
	fill_list(thgc, self->letters, count, (char*)NULL);
	self->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	if (self->type == LetterType::OT_Call1 || self->type == LetterType::OT_Call2) {
		self->statuses = create_list(thgc, sizeof(CodeStatus), CType::_Struct);
		CodeStatus cs_init = {};
		fill_list_v(thgc, self->statuses, count, &cs_init);
		for (int i = 0; i < count; i++) {
			CodeStatus* cs = (CodeStatus*)get_list(self->statuses, i);
			Obj* child = *(Obj**)get_list(self->children, i);
			Obj* val = vt_exeA(thgc, child, local);
			if (val->type == LetterType::OT_Wait) {
				cs->status = FS_Yet;
			} else if (val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) {
				cs->status = FS_Error;
				cs->letter = val->letter;
				cs->error = val->letter ? val->letter->error : NULL;
			} else {
				cs->status = FS_Finish;
			}
			add_list(thgc, self->rets, (char*)val);
		}
	} else {
		for (int i = 0; i < count; i++) {
			Obj* child = *(Obj**)get_list(self->children, i);
			Obj* val = vt_exeA(thgc, child, local);
			if (val->letter == NULL)
				if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			set_list(self->letters, i, (char*)child->letter);
			add_list(thgc, self->rets, (char*)val);
		}
	}
	return (Obj*)self;
}

// ============================================================
// Block::exepA (vtable signature: Obj* self)
// ============================================================
Obj* Block_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Block* blk = (Block*)self;
	List* blocks = get_blocks(local);
	if (blk->type == LetterType::OT_Block) {
		int m = 0;
		if (blk->children->size != 0) {
			Obj* child_m = *(Obj**)get_list(blk->children, m);
			if (child_m->type == LetterType::OT_Word) {
				if (child_m->letter != NULL && child_m->letter->text != NULL &&
					child_m->letter->text->size == 1 && child_m->letter->text->data[0] == 'T') {
					m++;
					Obj* val2 = *(Obj**)get_list(blk->children, m);
					VariClass* type = make_variclass(thgc, 0);
				head:
					if (val2->type == LetterType::OT_Block) {
						Block* blk2 = (Block*)val2;
						if (blk2->children->size == 0) {
							ArrType* arrtype = make_arrtype(thgc, (Obj*)type);
							type = (VariClass*)arrtype;
							m++;
							val2 = *(Obj**)get_list(blk->children, m);
							goto head;
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
							Obj* val = vt_exeA(thgc, val2, local);
							if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
							Block* blk3 = (Block*)val;
							for (int i = 0; i < blk3->rets->size; i++) {
								Obj* ret_i = *(Obj**)get_list(blk3->rets, i);
								if (ret_i->type == LetterType::OT_Word) {
									VariClass* vc = make_variclass(thgc, i);
									add_mapy(thgc, func->vmap, ret_i->letter->text, (char*)vc);
								} else {
									return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスの引数は名前を指定してください");
								}
							}
							(*n)++;
							val2 = *(Obj**)get_list(primary->children, *n);
							if (val2->type == LetterType::OT_CallBlock) {
								func->draw = (Block*)val2;
								for (int bi = 0; bi < blocks->size; bi++) {
									add_list(thgc, func->blocks, *get_list(blocks, bi));
								}
								declareA(thgc, local, word->letter->text, (Obj*)func);
								return (Obj*)func;
							}
							return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックス関数の{|}が宣言されていません");
						} else if (val2->type == LetterType::OT_CallBlock) {
							Function* func = make_function(thgc, (Obj*)type);
							func->draw = (Block*)val2;
							for (int bi = 0; bi < blocks->size; bi++) {
								add_list(thgc, func->blocks, *get_list(blocks, bi));
							}
							declareA(thgc, local, word->letter->text, (Obj*)func);
							return (Obj*)func;
						}
					}
				}
			}
		}
	}
	return Block_exeRangeA(thgc, blk, 0, blk->children->size, local, true);
}

// ============================================================
// Block::PrimaryA (vtable signature: Obj* self)
// ============================================================
Obj* Block_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Block* blk = (Block*)self;
	if (blk->type == LetterType::OT_Bracket) {
		if (val2->type == LetterType::OT_Block) {
			if (blk->rets->size == 1) {
				Block* blk_v2 = (Block*)val2;
				if (blk_v2->children->size == 0) {
					Obj* cls = *(Obj**)get_list(blk->rets, 0);
					if (isTypeObj(cls)) {
						ArrType* typearray = make_arrtype(thgc, cls);
						return (Obj*)typearray;
					} else {
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型の宣言は型を元にしてください");
					}
				} else {
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型の宣言の[]は引数が0です");
				}
			} else {
				return ErrorA(thgc, LetterType::OT_Error, blk->letter, "配列方の宣言は1つの型を配列にしてください");
			}
		} else if (val2->type == LetterType::OT_Bracket) {
			Obj* val = vt_exeA(thgc, val2, local);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			Block* block = (Block*)val;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			for (int i = 0; i < block->rets->size; i++) {
				Obj* rv = *(Obj**)get_list(block->rets, i);
				if (rv->type == LetterType::OT_Word) {
					Obj* cls = *(Obj**)get_list(blk->rets, i % blk->rets->size);
					if (isTypeObj(cls)) {
						Variable* variable = make_variable(thgc, cls);
						declareA(thgc, local, rv->letter->text, (Obj*)variable);
						char** slot = get_list(block->rets, i);
						*slot = (char*)variable;
					} else {
						return ErrorA(thgc, LetterType::OT_Error, blk->letter, "複数型の変数宣言は型を指定してください");
					}
				} else {
					return ErrorA(thgc, LetterType::OT_Error, rv->letter, "複数の変数の宣言には名前を指定してください");
				}
			}
			return (Obj*)block;
		} else if (val2->type == LetterType::OT_Word) {
			Obj* word = val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			Variable* variable = make_variable(thgc, *(Obj**)get_list(blk->rets, 0));
			declareA(thgc, local, word->letter->text, (Obj*)variable);
			return (Obj*)variable;
		} else if (val2->type == LetterType::_Left) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (blk->rets->size == 1) {
				Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
				return PrimaryA_vt(thgc, ret0, n, local, primary, val2);
			} else {
				return ErrorA(thgc, LetterType::OT_Error, blk->letter, "この()は1つしかとれない");
			}
		} else if (val2->type == LetterType::_Dot) {
			if (blk->rets->size == 1) {
				Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
				return PrimaryA_vt(thgc, ret0, n, local, primary, val2);
			} else {
				return ErrorA(thgc, LetterType::OT_Error, blk->letter, "この()は1つしかとれない");
			}
		}
	} else if (blk->type == LetterType::OT_Block || blk->type == LetterType::OT_Array) {
		if (val2->type == LetterType::OT_Block) {
			Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			Block* block = (Block*)val;
			Obj* me = (Obj*)blk;
			for (int i = 0; i < block->rets->size; i++) {
				if (me->type == LetterType::OT_Block || me->type == LetterType::OT_Array) {
					Block* meblk = (Block*)me;
					Obj* ret_i = *(Obj**)get_list(block->rets, i);
					if (ret_i->type == LetterType::OT_Number) {
						Number* num = (Number*)ret_i;
						me = *(Obj**)get_list(meblk->rets, num->value);
					} else if (ret_i->type == LetterType::OT_StrObj) {
						Label* lab = (Label*)get_mapy(meblk->labelmap, ret_i->letter->text);
						me = *(Obj**)get_list(meblk->rets, lab->n);
					} else {
						return ErrorA(thgc, LetterType::OT_Error, ret_i->letter, "配列の引数は文字列か整数です");
					}
				} else {
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列の深さが足りません");
				}
			}
			(*n)++;
			return me;
		} else if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				Obj* word = val2;
				(*n)++;
				if (str_matchA(word->letter->text, "length", 6)) {
					Obj* num = make_number(thgc, NULL);
					return num;
				} else if (str_matchA(word->letter->text, "Sort", 4)) {
					Obj* v2 = *(Obj**)get_list(primary->children, *n);
					if (v2->type == LetterType::OT_Bracket) {
						Obj* val = GetterA_vt(thgc, vt_exeA(thgc, v2, local), local);
						if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
						(*n)++;
						return (Obj*)blk;
					} else {
						return ErrorA(thgc, LetterType::OT_Error, v2->letter, "Sortの()がありません");
					}
				} else if (str_matchA(word->letter->text, "Range", 5)) {
					Obj* v2 = *(Obj**)get_list(primary->children, *n);
					if (v2->type == LetterType::OT_Bracket) {
						Obj* val = GetterA_vt(thgc, vt_exeA(thgc, v2, local), local);
						if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
						(*n)++;
						return (Obj*)blk;
					} else {
						return ErrorA(thgc, LetterType::OT_Error, v2->letter, "Rangeの()がありません");
					}
				} else {
					return ErrorA(thgc, LetterType::OT_NG, val2->letter, "配列にはこの名前は宣言されていません");
				}
			} else {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が必要です");
			}
		} else {
			return ErrorA(thgc, LetterType::OT_NG, val2->letter, "[]の後が必要です");
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "不正なエラー");
}

// ============================================================
// Block::SelfA (vtable signature: Obj* self)
// ============================================================
Obj* Block_SelfA(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	for (int i = 0; i < blk->rets->size; i++) {
		Obj* rv = *(Obj**)get_list(blk->rets, i);
		rv = SelfA_vt(thgc, rv, local);
		char** slot = get_list(blk->rets, i);
		*slot = (char*)rv;
	}
	return (Obj*)blk;
}

// ============================================================
// Block::GetterA (vtable signature: Obj* self)
// ============================================================
Obj* Block_GetterA(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	for (int i = 0; i < blk->rets->size; i++) {
		Obj* rv = *(Obj**)get_list(blk->rets, i);
		rv = GetterA_vt(thgc, rv, local);
		char** slot = get_list(blk->rets, i);
		*slot = (char*)rv;
	}
	return (Obj*)blk;
}

// ============================================================
// CallBlock::exepA (vtable signature: Obj* self)
// ============================================================
Obj* CallBlock_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return (Obj*)self;
}

// ============================================================
// CallBlock::exeA (vtable signature: Obj* self)
// ============================================================
Obj* CallBlock_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	// C# local.gene.vmap = new Dictionary<string, Obj>()
	local->gene->vmap = create_mapy(thgc, CType::_List);
	Block* blk = (Block*)self;
	Block* block2 = (Block*)*(Obj**)get_list(blk->children, 1);
	block2->blocks = create_list(thgc, sizeof(Block*), CType::_Block);
	add_list(thgc, block2->blocks, (char*)block2);
	add_list(thgc, local->blockslist, (char *)create_list(thgc, sizeof(List*), CType::_List));
	List* blocks = get_blocks(local);
	add_list(thgc, blocks, (char*)block2);
	Block_exeA(thgc, (Obj*)block2, local);
	if (blocks->size > 0) blocks->size--;
	if (local->blockslist->size > 0) local->blockslist->size--;
	return (Obj*)blk;
}

// ============================================================
// LetterName_obj - C# Obj.LetterName() / Type.LetterName()
// ============================================================
const char* LetterName_obj(Obj* obj) {
	if (!obj) return "(null)";
	if (obj->letter && obj->letter->text) {
		// 簡易ASCII取得 (esize==1のみ)
		String* s = obj->letter->text;
		if (s->esize == 1) {
			static thread_local char buf[256];
			int len = s->size < 255 ? s->size : 255;
			memcpy(buf, s->data, len);
			buf[len] = '\0';
			return buf;
		}
		return "(utf16)";
	}
	return "(no-name)";
}

// ============================================================
// obj_Info - C# Obj.Info() の移植 (objtype別に分岐)
// ============================================================
std::string obj_Info(Obj* obj) {
	if (!obj) return "(null)";
	switch (obj->type) {
	case LetterType::OT_Value:
		return std::string("val:") + LetterName_obj(((Val*)obj)->cls);
	case LetterType::OT_Variable:
		return std::string("var:") + LetterName_obj(obj) + " " + LetterName_obj(((Variable*)obj)->cls);
	case LetterType::OT_Function:
		return std::string("fun:") + LetterName_obj(obj) + " " + LetterName_obj(((Function*)obj)->rettype);
	case LetterType::OT_If:
		return std::string("if :") + LetterName_obj(obj);
	case LetterType::OT_IfBlock:
		return std::string("ifb:");
	case LetterType::OT_While:
		return std::string("whi:") + LetterName_obj(obj);
	case LetterType::OT_For:
		return std::string("for:") + LetterName_obj(obj);
	case LetterType::OT_Switch:
		return std::string("swi:") + LetterName_obj(obj);
	case LetterType::OT_ClassObj:
		return std::string("cbj:") + LetterName_obj(obj);
	case LetterType::OT_Number:
		return "int **";
	case LetterType::OT_FloatVal:
		return "float **";
	case LetterType::OT_SqlString:
		return "sqlstr **";
	case LetterType::OT_StrObj:
		return "strobj **";
	case LetterType::OT_BoolVal:
		return "bool **";
	case LetterType::OT_VoiVal:
		return "void **";
	case LetterType::OT_Model:
		return "model";
	case LetterType::OT_ModelObj:
		return std::string("mod :") + LetterName_obj(obj);
	case LetterType::OT_ModelVal:
		return std::string("mval:") + LetterName_obj(((Val*)obj)->cls);
	case LetterType::OT_GeneObj:
		return std::string("gene:") + LetterName_obj(obj);
	case LetterType::OT_SignalFunction:
		return std::string("sigf:") + LetterName_obj(obj);
	case LetterType::OT_ServerFunction:
		return std::string("scf:") + LetterName_obj(obj);
	case LetterType::OT_Stock:
		return "stock";
	default:
		return std::string("obj:") + std::to_string((int)obj->type);
	}
}

// ============================================================
// Block_Show - C# Block.Show(space, ref error, local)
// ============================================================
String* Block_Show(ThreadGC* thgc, Block* self, const char* space, bool* error, Master* local) {
	std::string text;

	// obj情報の表示
	if (self->obj && self->obj->type != LetterType::OT_None) {
		text += space;
		text += "$";
		text += std::to_string(self->obj_n);
		text += ":";
		text += obj_Info(self->obj);
		text += "\n";
	}

	// vmapAの各エントリを表示
	if (self->vmapA && self->vmapA->kvs) {
		List* kvs = self->vmapA->kvs;
		for (int i = 0; i < kvs->size; i++) {
			KV* kv = *(KV**)get_list(kvs, i);
			if (kv && kv->value) {
				text += space;
				text += "  ";
				text += obj_Info((Obj*)kv->value);
				text += "\n";
			}
		}
	}

	// statuses表示
	if (self->statuses) {
		for (int i = 0; i < self->statuses->size; i++) {
			CodeStatus* cs = (CodeStatus*)get_list(self->statuses, i);
			if (cs->status == FS_Yet) {
				text += "Yet:";
				*error = true;
			}
			else if (cs->status == FS_Error) {
				text += "Error:";
				if (cs->letter) {
					cs->letter->error = cs->error;
				}
				if (cs->error) {
					String* errstr = cs->error;
					if (errstr->esize == 1) {
						char buf[512];
						int len = errstr->size < 511 ? errstr->size : 511;
						memcpy(buf, errstr->data, len);
						buf[len] = '\0';
						fprintf(stderr, "[ShowError] %s\n", buf);
					}
				}
				*error = true;
			}
			else if (cs->status == FS_Finish) {
				text += "Finish:";
			}
		}
		text += "\n";
	}

	// childblocks再帰表示
	if (self->childblocks) {
		std::string childspace = std::string("    ") + space;
		for (int i = 0; i < self->childblocks->size; i++) {
			Block* child = *(Block**)get_list(self->childblocks, i);
			String* childtext = Block_Show(thgc, child, childspace.c_str(), error, local);
			if (childtext && childtext->size > 0) {
				if (childtext->esize == 1) {
					text.append(childtext->data, childtext->size);
				}
			}
		}
	}

	return createString(thgc, text.c_str(), (int)text.size(), 1);
}

// ============================================================
// initVTables - VTable初期化
// ============================================================
void initVTables() {
	// デフォルトで全ルックアップを初期化
	for (int i = 0; i < VT_LOOKUP_SIZE; i++) {
		vt_lookup[i] = &vt_Obj_default;
	}

	// 各 vt_XXX を vt_Obj_default でコピー初期化する。
	// これで明示的に代入されないスロットも NULL にならず、_vt_XXX_default (return self) が入る。
	// C# 側で override されていない仮想メソッドのデフォルト挙動 (base は this を返す) と一致する。
	vt_Operator = vt_Obj_default;
	vt_Primary = vt_Obj_default;
	vt_Block = vt_Obj_default;
	vt_Block_Array = vt_Obj_default;
	vt_CallBlock = vt_Obj_default;
	vt_Word = vt_Obj_default;
	vt_Number = vt_Obj_default;
	vt_FloatVal = vt_Obj_default;
	vt_StrObj = vt_Obj_default;
	vt_BoolVal = vt_Obj_default;
	vt_VoiVal = vt_Obj_default;
	vt_Value = vt_Obj_default;
	vt_Variable = vt_Obj_default;
	vt_Function = vt_Obj_default;
	vt_GenericFunction = vt_Obj_default;
	vt_Comment = vt_Obj_default;
	vt_Comment2 = vt_Obj_default;
	vt_TagBlock = vt_Obj_default;
	vt_Signal = vt_Obj_default;
	vt_SignalFunction = vt_Obj_default;
	vt_ServerClient = vt_Obj_default;
	vt_ServerFunction = vt_Obj_default;
	vt_Dolor = vt_Obj_default;
	vt_Mountain = vt_Obj_default;
	vt_HtmObj = vt_Obj_default;
	vt_ElemType = vt_Obj_default;
	vt_ElemObj = vt_Obj_default;
	vt_LinearFunction = vt_Obj_default;
	vt_AnimationFunction = vt_Obj_default;
	vt_Connect = vt_Obj_default;
	vt_ConnectStock = vt_Obj_default;
	vt_Address = vt_Obj_default;
	vt_Sum = vt_Obj_default;
	vt_Question = vt_Obj_default;
	vt_Clones = vt_Obj_default;
	vt_Class = vt_Obj_default;
	vt_Type = vt_Obj_default;
	vt_ArrType = vt_Obj_default;
	vt_FuncType = vt_Obj_default;
	vt_Var = vt_Obj_default;
	vt_ClassObj = vt_Obj_default;
	vt_Generic = vt_Obj_default;
	vt_Model = vt_Obj_default;
	vt_ModelObj = vt_Obj_default;
	vt_ModelVal = vt_Obj_default;
	vt_Gene = vt_Obj_default;
	vt_GeneObj = vt_Obj_default;
	vt_GeneLabel = vt_Obj_default;
	vt_If = vt_Obj_default;
	vt_While = vt_Obj_default;
	vt_For = vt_Obj_default;
	vt_Switch = vt_Obj_default;
	vt_Break = vt_Obj_default;
	vt_Continue = vt_Obj_default;
	vt_Return = vt_Obj_default;
	vt_Goto = vt_Obj_default;
	vt_Print = vt_Obj_default;
	vt_SqlString = vt_Obj_default;
	vt_Null = vt_Obj_default;
	vt_ArrayConstructor = vt_Obj_default;
	vt_Constructor = vt_Obj_default;
	vt_Base = vt_Obj_default;
	vt_GeneChild = vt_Obj_default;
	vt_GeneMutate = vt_Obj_default;
	vt_GeneNew = vt_Obj_default;
	vt_GeneCross = vt_Obj_default;
	vt_GeneVal = vt_Obj_default;
	vt_GeneStore = vt_Obj_default;
	vt_GeneSelect = vt_Obj_default;
	vt_GeneSort = vt_Obj_default;
	vt_GeneLabelValue = vt_Obj_default;
	vt_IfBlock = vt_Obj_default;
	vt_Iterator = vt_Obj_default;
	vt_NewBlock = vt_Obj_default;
	vt_CrossBlock = vt_Obj_default;
	vt_MigrateBlock = vt_Obj_default;

	// --- Operator ---
	vt_Operator.exeZ = Operator_exeZ;
	vt_Operator.exeA = Operator_exeA;
	vt_lookup[(int)LetterType::_Equal] = &vt_Operator;
	vt_lookup[(int)LetterType::_Colon] = &vt_Operator;
	vt_lookup[(int)LetterType::_LessThan] = &vt_Operator;
	vt_lookup[(int)LetterType::_LessEqual] = &vt_Operator;
	vt_lookup[(int)LetterType::_MoreThan] = &vt_Operator;
	vt_lookup[(int)LetterType::_MoreEqual] = &vt_Operator;
	vt_lookup[(int)LetterType::_EqualEqual] = &vt_Operator;
	vt_lookup[(int)LetterType::_NotEqual] = &vt_Operator;
	vt_lookup[(int)LetterType::_Plus] = &vt_Operator;
	vt_lookup[(int)LetterType::_Minus] = &vt_Operator;
	vt_lookup[(int)LetterType::_Mul] = &vt_Operator;
	vt_lookup[(int)LetterType::_Div] = &vt_Operator;
	vt_lookup[(int)LetterType::_Mod] = &vt_Operator;
	vt_lookup[(int)LetterType::_And] = &vt_Operator;
	vt_lookup[(int)LetterType::_Percent] = &vt_Operator;

	// --- Primary ---
	vt_Primary.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Primary_exeZ;
	vt_Primary.exeA = Primary_exeA;
	// Primary uses OT_None but vt is set explicitly in ParsePrimary

	// --- Block (OT_Block, OT_Bracket, OT_Call1, OT_Call2) ---
	vt_Block.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_exeZ;
	vt_Block.exepZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*))Block_exepZ;
	vt_Block.GetterZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_GetterZ;
	vt_Block.SelfZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_SelfZ;
	vt_Block.PrimaryZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*))Block_PrimaryZ;
	vt_Block.exeA = Block_exeA;
	vt_Block.exepA = Block_exepA;
	vt_Block.GetterA = Block_GetterA;
	vt_Block.SelfA = Block_SelfA;
	vt_Block.PrimaryA = Block_PrimaryA;
	vt_lookup[(int)LetterType::OT_Block] = &vt_Block;
	vt_lookup[(int)LetterType::OT_Bracket] = &vt_Block;
	vt_lookup[(int)LetterType::OT_Call1] = &vt_Block;
	vt_lookup[(int)LetterType::OT_Call2] = &vt_Block;

	// --- Block_Array (OT_Array) - inherits Block at A-pass and Z-pass ---
	vt_Block_Array.exeA = Block_exeA;
	vt_Block_Array.exepA = Block_exepA;
	vt_Block_Array.GetterA = Block_GetterA;
	vt_Block_Array.SelfA = Block_SelfA;
	vt_Block_Array.PrimaryA = Block_PrimaryA;
	vt_Block_Array.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_exeZ;
	vt_Block_Array.exepZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*))Block_exepZ;
	vt_Block_Array.GetterZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_GetterZ;
	vt_Block_Array.SelfZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_SelfZ;
	vt_Block_Array.PrimaryZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*))Block_PrimaryZ;
	vt_lookup[(int)LetterType::OT_Array] = &vt_Block_Array;

	// --- NewBlock / CrossBlock / MigrateBlock (inherit Block at A-pass and Z-pass) ---
	vt_NewBlock.exeA = Block_exeA;
	vt_NewBlock.exepA = Block_exepA;
	vt_NewBlock.GetterA = Block_GetterA;
	vt_NewBlock.SelfA = Block_SelfA;
	vt_NewBlock.PrimaryA = Block_PrimaryA;
	vt_NewBlock.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_exeZ;
	vt_NewBlock.exepZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*))Block_exepZ;
	vt_NewBlock.GetterZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_GetterZ;
	vt_NewBlock.SelfZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_SelfZ;
	vt_NewBlock.PrimaryZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*))Block_PrimaryZ;
	vt_CrossBlock.exeA = Block_exeA;
	vt_CrossBlock.exepA = Block_exepA;
	vt_CrossBlock.GetterA = Block_GetterA;
	vt_CrossBlock.SelfA = Block_SelfA;
	vt_CrossBlock.PrimaryA = Block_PrimaryA;
	vt_CrossBlock.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_exeZ;
	vt_CrossBlock.exepZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*))Block_exepZ;
	vt_CrossBlock.GetterZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_GetterZ;
	vt_CrossBlock.SelfZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_SelfZ;
	vt_CrossBlock.PrimaryZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*))Block_PrimaryZ;
	vt_MigrateBlock.exeA = Block_exeA;
	vt_MigrateBlock.exepA = Block_exepA;
	vt_MigrateBlock.GetterA = Block_GetterA;
	vt_MigrateBlock.SelfA = Block_SelfA;
	vt_MigrateBlock.PrimaryA = Block_PrimaryA;
	vt_MigrateBlock.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_exeZ;
	vt_MigrateBlock.exepZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*))Block_exepZ;
	vt_MigrateBlock.GetterZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_GetterZ;
	vt_MigrateBlock.SelfZ = (Obj*(*)(ThreadGC*, Obj*, Master*))Block_SelfZ;
	vt_MigrateBlock.PrimaryZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*))Block_PrimaryZ;

	// --- CallBlock ---
	vt_CallBlock.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))CallBlock_exeZ;
	vt_CallBlock.exepZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*))CallBlock_exepZ;
	vt_CallBlock.exeA = CallBlock_exeA;
	vt_CallBlock.exepA = CallBlock_exepA;
	vt_lookup[(int)LetterType::OT_CallBlock] = &vt_CallBlock;

	// Word, Variable, Function, Number の登録は A3_initVTables (obj2a.h) に移動済み

	// obj2a2.h の型を初期化
	A2_initVTables();

	// obj2a3.h の型を初期化
	A3_initVTables();

	// obj2b.h, obj2b2.h, obj2b3.h の型を初期化 (B-pass)
	B_initVTables();
	B2_initVTables();
	B3_initVTables();

	// C-pass の型を初期化
	C_initVTables();
}
