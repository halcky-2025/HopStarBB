// ============================================================
// objz.h - Z解析パス (静的解析 / 型チェック)
// ============================================================

// isTypeObj - C#の "as Type" に対応するnullチェック
// Type継承: Type, ClassObj, ModelObj, GeneObj, Var, ArrType, FuncType, VariClass, GenericObj, GeneLabel, Signal, AddressType
static inline bool isTypeObj(Obj* obj) {
	if (obj == NULL) return false;
	switch (obj->type) {
	case LetterType::OT_ClassObj:
	case LetterType::OT_ModelObj:
	case LetterType::OT_GeneObj:
	case LetterType::OT_Var:
	case LetterType::OT_ArrType:
	case LetterType::OT_ArrayType:
	case LetterType::OT_FuncType:
	case LetterType::OT_VariClass:
	case LetterType::OT_GenericObj:
	case LetterType::OT_GeneLabel:
	case LetterType::OT_Signal:
		return true;
	default:
		return false;
	}
}

// ErrorZ - Z-pass用エラー生成
Obj* ErrorZ(ThreadGC* thgc, LetterType type, ATSSpan* letter, const char* msg = NULL) {
	Obj* err = make_cobj(thgc, CType::_CObj, type, letter);
	if (msg && letter) {
		letter->error = createString(thgc, msg, (int)strlen(msg), 1);
		letter->underlineColor = 0xFF0000FF;
	}
	return err;
}

// 前方宣言
Obj* exeZ(ThreadGC* thgc, Obj* self, Master* local);
Obj* exepZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* GetterZ(ThreadGC* thgc, Obj* self, Master* local);
Obj* SelfZ(ThreadGC* thgc, Obj* self, Master* local);
Obj* opeZ(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* PrimaryZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Block_exeZ(ThreadGC* thgc, Block* self, Master* local);
Obj* Block_exepZ(ThreadGC* thgc, Block* self, int* n, Master* local, Primary* primary);
Obj* exeRangeZ(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id);
Obj* Block_PrimaryZ(ThreadGC* thgc, Block* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Block_SelfZ(ThreadGC* thgc, Block* self, Master* local);
Obj* Block_GetterZ(ThreadGC* thgc, Block* self, Master* local);

// ============================================================
// declareZ
// ============================================================
void declareZ(ThreadGC* thgc, Master* local, String* name, Obj* obj) {
	List* blocks = get_blocks(local);
	Block* blk = *(Block**)get_list(blocks, blocks->size - 1);
	add_mapy(thgc, blk->vmapA, name, (char*)obj);
}

// ============================================================
// GetterZ (デフォルト: selfを返す)
// ============================================================
Obj* GetterZ(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// SelfZ (デフォルト: selfを返す)
// ============================================================
Obj* SelfZ(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// PrimaryZ (デフォルト: n++してselfを返す)
// ============================================================
Obj* PrimaryZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	(*n)++;
	return self;
}

// ============================================================
// exeZ (デフォルト: 例外)
// ============================================================
Obj* exeZ(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// Operator::exeZ
// ============================================================
Obj* Operator_exeZ(ThreadGC* thgc, Obj* self, Master* local) {
	LetterType op = self->type;
	if (op == LetterType::_Equal || op == LetterType::_Colon || op == LetterType::_In) {
		Obj* val1 = SelfZ(thgc, exeZ(thgc, *(Obj**)get_list(self->children, 0), local), local);
		if (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG) return val1;
		Obj* val2 = GetterZ(thgc, exeZ(thgc, *(Obj**)get_list(self->children, 1), local), local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		return self;
	}
	{
		Obj* val1 = GetterZ(thgc, exeZ(thgc, *(Obj**)get_list(self->children, 0), local), local);
		if (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG) return val1;
		Obj* val2 = GetterZ(thgc, exeZ(thgc, *(Obj**)get_list(self->children, 1), local), local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		return self;
	}
}

// ============================================================
// Primary::exeZ
// ============================================================
Obj* Primary_exeZ(ThreadGC* thgc, Primary* self, Master* local) {
	if (local->comments != NULL && local->comments->size > 0) {
	}
	Obj* val1 = NULL;
	for (int i = 0; i < self->children->size - 1;) {
		Obj* child = *(Obj**)get_list(self->children, i);
		switch (child->type) {
			case LetterType::OT_Comment2:
				i++;
				continue;
			case LetterType::OT_Htm:
			case LetterType::OT_Word:
			case LetterType::OT_Number:
			case LetterType::OT_FloatVal:
			case LetterType::OT_BoolVal:
			case LetterType::OT_StrObj:
			case LetterType::OT_Print:
			case LetterType::OT_Return:
			case LetterType::OT_Goto:
			case LetterType::OT_Continue:
			case LetterType::OT_Var:
			case LetterType::OT_If:
			case LetterType::OT_Elif:
			case LetterType::OT_Else:
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
				i++;
				break;
			case LetterType::OT_Bracket:
			case LetterType::OT_Block:
				val1 = Block_exepZ(thgc, (Block*)child, &i, local, self);
				i++;
				break;
			case LetterType::OT_Model:
			case LetterType::OT_Gene:
			case LetterType::OT_Class:
				val1 = exepZ(thgc, child, &i, local, self);
				break;
			case LetterType::_Dot:
			case LetterType::_Left:
			case LetterType::_Right:
				i++;
				break;
			case LetterType::OT_TagBlock:
			case LetterType::OT_CallBlock:
				i++;
				break;
			default:
				break;
		}
		if (val1 != NULL && (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG)) return val1;
	}
	if (val1 == NULL) val1 = *(Obj**)get_list(self->children, self->children->size - 1);
	return val1;
}

// ============================================================
// Obj::exepZ (TODO: 移植中)
// ============================================================
Obj* exepZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return (Obj*)self;
}

// ============================================================
// Block::exepZ
// ============================================================
Obj* Block_exepZ(ThreadGC* thgc, Block* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	if (self->type == LetterType::OT_Block) {
		int m = 0;
		if (self->children->size != 0) {
			Obj* child_m = *(Obj**)get_list(self->children, m);
			if (child_m->type == LetterType::OT_Word) {
				if (child_m->letter != NULL && child_m->letter->text != NULL &&
					child_m->letter->text->size == 1 && child_m->letter->text->data[0] == 'T') {
					m++;
					Obj* val2 = *(Obj**)get_list(self->children, m);
					VariClass* type = make_variclass(thgc, 0);
				head:
					if (val2->type == LetterType::OT_Block) {
						Block* blk = (Block*)val2;
						if (blk->children->size == 0) {
							ArrType* arrtype = make_arrtype(thgc, (Obj*)type);
							type = (VariClass*)arrtype;
							m++;
							val2 = *(Obj**)get_list(self->children, m);
							goto head;
						}
						else return ErrorZ(thgc, LetterType::OT_Error, val2->letter, "配列型は引数が0です");
					}
					*n = 1;
					val2 = *(Obj**)get_list(primary->children, *n);
					if (val2->type == LetterType::OT_Word) {
						Obj* word = val2;
						(*n)++;
						val2 = *(Obj**)get_list(primary->children, *n);
						bool gene = false;
						if (val2->type == LetterType::OT_Block) {
							GenericFunction* func = make_genericfunction(thgc, (Obj*)type);
							Block* blk = (Block*)Block_exeZ(thgc, (Block*)val2, local);
							for (int i = 0; i < blk->rets->size; i++) {
								Obj* ret_i = *(Obj**)get_list(blk->rets, i);
								if (ret_i->type == LetterType::OT_Word) {
									VariClass* vc = make_variclass(thgc, i);
									add_mapy(thgc, func->vmap, ret_i->letter->text, (char*)vc);
								}
							}
							(*n)++;
							val2 = *(Obj**)get_list(primary->children, *n);
							if (val2->type == LetterType::OT_CallBlock) {
								func->draw = (Block*)val2;
								for (int bi = 0; bi < blocks->size; bi++) {
									add_list(thgc, func->blocks, *get_list(blocks, bi));
								}
								declareZ(thgc, local, word->letter->text, (Obj*)func);
								return (Obj*)func;
							}
						} else if (val2->type == LetterType::OT_CallBlock) {
							Function* func = make_function(thgc, (Obj*)type);
							func->draw = (Block*)val2;
							for (int bi = 0; bi < blocks->size; bi++) {
								add_list(thgc, func->blocks, *get_list(blocks, bi));
							}
							declareZ(thgc, local, word->letter->text, (Obj*)func);
							return (Obj*)func;
						}
					}
				}
			}
		}
	}
	return exeRangeZ(thgc, self, 0, self->children->size, local, true);
}

// ============================================================
// Block::exeZ
// ============================================================
Obj* Block_exeZ(ThreadGC* thgc, Block* self, Master* local) {
	if (self->type == LetterType::OT_Array) return (Obj*)self;
	return exeRangeZ(thgc, self, 0, self->children->size, local, true);
}

// ============================================================
// Block::exeRangeZ
// ============================================================
Obj* exeRangeZ(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id) {
	for (int i = 0; i < self->children->size; i++) {
		Obj* child = *(Obj**)get_list(self->children, i);
		Obj* val = exeZ(thgc, child, local);
		if (self->type != LetterType::OT_Call1 && self->type != LetterType::OT_Call2) {
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		}
		add_list(thgc, self->rets, (char*)val);
	}
	return (Obj*)self;
}

// ============================================================
// Block::PrimaryZ
// ============================================================
Obj* Block_PrimaryZ(ThreadGC* thgc, Block* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (self->type == LetterType::OT_Bracket) {
		if (val2->type == LetterType::OT_Block) {
			Block* blk_v2 = (Block*)val2;
			if (self->rets->size == 1) {
				if (blk_v2->children->size == 0) {
					Obj* cls = *(Obj**)get_list(self->rets, 0);
					if (isTypeObj(cls)) {
						ArrType* typearray = make_arrtype(thgc, cls);
						return (Obj*)typearray;
					}
					else return ErrorZ(thgc, LetterType::OT_Error, val2->letter, "配列方の宣言は型を元にしてください");
				}
				else return ErrorZ(thgc, LetterType::OT_Error, val2->letter, "配列型の宣言の[]は引数が0です");
			}
			else return ErrorZ(thgc, LetterType::OT_Error, self->letter, "配列方の宣言は1つの型を配列にしてください");
		} else if (val2->type == LetterType::OT_Bracket) {
			Block* block = (Block*)Block_exeZ(thgc, (Block*)val2, local);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			for (int i = 0; i < block->rets->size; i++) {
				val2 = *(Obj**)get_list(block->rets, i);
				if (val2->type == LetterType::OT_Word) {
					Obj* cls = *(Obj**)get_list(self->rets, i % self->rets->size);
					if (isTypeObj(cls)) {
						Variable* variable = make_variable(thgc, cls);
						declareZ(thgc, local, val2->letter->text, (Obj*)variable);
						char** slot = get_list(block->rets, i);
						*slot = (char*)variable;
					}
					else return ErrorZ(thgc, LetterType::OT_Error, self->letter, "複数型の変数宣言は型を指定してください");
				}
				else return ErrorZ(thgc, LetterType::OT_Error, val2->letter, "複数の変数の宣言には名前を指定してください");
			}
			return (Obj*)block;
		} else if (val2->type == LetterType::OT_Word) {
			Obj* word = val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			Variable* variable = make_variable(thgc, *(Obj**)get_list(self->rets, 0));
			declareZ(thgc, local, word->letter->text, (Obj*)variable);
			return (Obj*)variable;
		} else if (val2->type == LetterType::_Left) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (self->rets->size == 1) {
				Obj* type = *(Obj**)get_list(self->rets, 0);
				if (type != NULL) {
					FuncTypeC* functype = make_functype(thgc, type);
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					GetterZ(thgc, exeZ(thgc, val2, local), local);
					if (val2->type == LetterType::OT_ClassObj) {
						add_list(thgc, functype->draws, (char*)val2);
						(*n)++;
						return (Obj*)functype;
					} else if (val2->type == LetterType::OT_Generic) {
						Obj* generic = val2;
						(*n)++;
						val2 = *(Obj**)get_list(primary->children, *n);
						if (val2->type == LetterType::OT_Block) {
							GetterZ(thgc, Block_exeZ(thgc, (Block*)val2, local), local);
							Block* blk = (Block*)val2;
							GenericObj* genericobj = make_genericobj(thgc, generic);
							genericobj->extends_list = create_list(thgc, sizeof(Obj*), CType::_CObj);
							for (int i = 0; i < blk->rets->size; i++) {
								Obj* rv = *(Obj**)get_list(blk->rets, i);
								if (rv->type == LetterType::OT_ClassObj || rv->type == LetterType::OT_GenericObj) {
									add_list(thgc, genericobj->draws, (char*)rv);
								}
							}
							Generic* gen = (Generic*)generic;
							if (gen->vmap->kvs->size != genericobj->draws->size) {
								return ErrorZ(thgc, LetterType::OT_Error, val2->letter, "ジェネリッククラスの引数の数が合っていません");
							}
							(*n)++;
							add_list(thgc, functype->draws, (char*)genericobj);
							return (Obj*)functype;
						}
					} else if (val2->type == LetterType::OT_Bracket) {
						Block* blk = (Block*)GetterZ(thgc, Block_exeZ(thgc, (Block*)val2, local), local);
						for (int i = 0; i < blk->rets->size; i++) {
							Obj* rv = *(Obj**)get_list(blk->rets, i);
							if (rv->type == LetterType::OT_ClassObj || rv->type == LetterType::OT_GenericObj) {
								add_list(thgc, functype->draws, (char*)rv);
							}
						}
						(*n)++;
						return (Obj*)functype;
					}
				}
			}
			else return ErrorZ(thgc, LetterType::OT_Error, self->letter, "この()は1つしかとれない");
		} else if (val2->type == LetterType::_Dot) {
			if (self->rets->size == 1) {
				return PrimaryZ(thgc, *(Obj**)get_list(self->rets, 0), n, local, primary, val2);
			}
			else return ErrorZ(thgc, LetterType::OT_Error, self->letter, "この()は1つしかとれない");
		}
	} else if (self->type == LetterType::OT_Block || self->type == LetterType::OT_Array) {
		if (val2->type == LetterType::OT_Block) {
			Block* block = (Block*)val2;
			Block_GetterZ(thgc, block, local);
			Obj* me = (Obj*)self;
			for (int i = 0; i < block->rets->size; i++) {
				Obj* ret_i = *(Obj**)get_list(block->rets, i);
				if (me->type == LetterType::OT_Block || me->type == LetterType::OT_Array) {
					Block* blk = (Block*)me;
					if (ret_i->type == LetterType::OT_Number) {
						Number* num = (Number*)ret_i;
						me = *(Obj**)get_list(blk->rets, num->value);
					} else if (ret_i->type == LetterType::OT_StrObj) {
						// String indexing via labelmap
					}
					else return ErrorZ(thgc, LetterType::OT_Error, ret_i->letter, "配列の引数は文字列か整数です");
				}
				else return ErrorZ(thgc, LetterType::OT_Error, ret_i->letter, "配列の深さが足りません");
			}
			(*n)++;
			return me;
		} else if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				Obj* word = val2;
				(*n)++;
				if (word->letter != NULL && word->letter->text != NULL) {
					int tlen = word->letter->text->size;
					char* tdata = word->letter->text->data;
					if (tlen == 6 && memcmp(tdata, "length", 6) == 0) {
						Obj* num = make_number(thgc, NULL);
						return num;
					}
					else if (tlen == 4 && memcmp(tdata, "Sort", 4) == 0) {
						val2 = *(Obj**)get_list(primary->children, *n);
						if (val2->type == LetterType::OT_Bracket) {
							GetterZ(thgc, exeZ(thgc, val2, local), local);
							(*n)++;
							return (Obj*)self;
						}
						else return ErrorZ(thgc, LetterType::OT_Error, val2->letter, "Sortの()がありません");
					}
					else if (tlen == 5 && memcmp(tdata, "Range", 5) == 0) {
						val2 = *(Obj**)get_list(primary->children, *n);
						if (val2->type == LetterType::OT_Bracket) {
							GetterZ(thgc, exeZ(thgc, val2, local), local);
							(*n)++;
							return (Obj*)self;
						}
						else return ErrorZ(thgc, LetterType::OT_Error, val2->letter, "Rangeの()がありません");
					}
					else return ErrorZ(thgc, LetterType::OT_NG, word->letter, "配列には指定名は宣言されていません");
				}
			}
			else return ErrorZ(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が必要です");
		}
		else return ErrorZ(thgc, LetterType::OT_NG, val2->letter, "[]の後が必要です");
	}
	return (Obj*)self;
}

// ============================================================
// Block::SelfZ
// ============================================================
Obj* Block_SelfZ(ThreadGC* thgc, Block* self, Master* local) {
	for (int i = 0; i < self->rets->size; i++) {
		Obj* rv = *(Obj**)get_list(self->rets, i);
		rv = SelfZ(thgc, rv, local);
		char** slot = get_list(self->rets, i);
		*slot = (char*)rv;
	}
	return (Obj*)self;
}

// ============================================================
// Block::GetterZ
// ============================================================
Obj* Block_GetterZ(ThreadGC* thgc, Block* self, Master* local) {
	for (int i = 0; i < self->rets->size; i++) {
		Obj* rv = *(Obj**)get_list(self->rets, i);
		rv = GetterZ(thgc, rv, local);
		char** slot = get_list(self->rets, i);
		*slot = (char*)rv;
	}
	return (Obj*)self;
}

// ============================================================
// CallBlock::exepZ
// ============================================================
Obj* CallBlock_exepZ(ThreadGC* thgc, Block* self, int* n, Master* local, Primary* primary) {
	return (Obj*)self;
}

// ============================================================
// CallBlock::exeZ
// ============================================================
Obj* CallBlock_exeZ(ThreadGC* thgc, Block* self, Master* local) {
	Block* block2 = (Block*)*(Obj**)get_list(self->children, 1);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"object", 6, 1), (char*)local->Object);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"void", 4, 1), (char*)local->Void);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"int", 3, 1), (char*)local->Int);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"short", 5, 1), (char*)local->Short);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"Str", 3, 1), (char*)local->StrT);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"bool", 4, 1), (char*)local->Bool);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"float", 5, 1), (char*)local->FloatT);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"MouseDown", 9, 1), (char*)local->MouseEventT);
	add_mapy(thgc, block2->vmapA, createString(thgc, (char*)"KeyDown", 7, 1), (char*)local->KeyEventT);
	List* blocks = get_blocks(local);
	for (int i = 0; i < local->analblocks->size; i++) {
		Block* block = *(Block**)get_list(local->analblocks, i);
		// blocks = { block }
		blocks->size = 0;
		add_list(thgc, blocks, (char*)block);
		Block_exeZ(thgc, block, local);
		// local->blockslist.RemoveAt(last)
		if (local->blockslist->size > 0) {
			local->blockslist->size--;
		}
	}
	return (Obj*)self;
}

// ============================================================
// Class::exepZ
// ============================================================
Obj* Class_exepZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	Obj* obj = make_classobj(thgc);
	if (val2->type == LetterType::OT_Word) {
		Obj* word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Block) {
			Generic* generic = make_generic(thgc);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			obj = (Obj*)generic;
		}
		declareZ(thgc, local, word->letter->text, obj);
		((ClassObj*)obj)->letter = word->letter;
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				obj->rename = val2->letter->text;
			}
		}
	}
	return self;
}

// ============================================================
// Model::exepZ
// ============================================================
Obj* Model_exepZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	if (val2->type == LetterType::OT_Word) {
		Obj* word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		ModelObj* gj = make_modelobj(thgc, word->letter);
		declareZ(thgc, local, word->letter->text, (Obj*)gj);
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				gj->rename = word->letter->text;
				if (val2->type == LetterType::OT_Block) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
				}
			}
		}
		if (val2->type == LetterType::OT_CallBlock) {
			CallBlock* callblk = (CallBlock*)val2;
			Block* inner = (Block*)*(Obj**)get_list(callblk->children, 1);
			gj->letter2 = inner->letter2;
			gj->draw = callblk;
			inner->obj = (Obj*)make_objblock_z(thgc, (Obj*)gj, 1);
			(*n)++;
			return (Obj*)gj;
		}
	}
	return self;
}

// ============================================================
// Gene::exepZ
// ============================================================
Obj* Gene_exepZ(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	if (val2->type == LetterType::OT_Word) {
		Obj* word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		GeneObj* gj = make_geneobj(thgc, word->letter->text, self->letter);
		populate_geneobj_vmap(thgc, gj);
		if (local->gene != NULL) {
			Generic* gen = (Generic*)local->gene;
			add_mapy(thgc, gen->vmap, word->letter->text, (char*)gj);
		}
		declareZ(thgc, local, word->letter->text, (Obj*)gj);
		if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				gj->letter2 = NULL;
				gj->rename = val2->letter->text;
				if (val2->type == LetterType::OT_Block) {
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
				}
			}
		}
		if (val2->type == LetterType::OT_CallBlock) {
			CallBlock* callblk = (CallBlock*)val2;
			Block* inner = (Block*)*(Obj**)get_list(callblk->children, 1);
			gj->letter2 = inner->letter2;
			gj->call_blk = callblk;
			inner->obj = (Obj*)make_objblock_z(thgc, (Obj*)gj, 1);
			for (int bi = 0; bi < blocks->size; bi++) {
				add_list(thgc, gj->geneblocks, *get_list(blocks, bi));
			}
			(*n)++;
			return (Obj*)gj;
		}
	} else if (val2->type == LetterType::OT_CallBlock) {
		GeneObj* gj = make_geneobj(thgc, NULL, NULL);
		gj->call_blk = (Block*)val2;
		populate_geneobj_vmap(thgc, gj);
		if (local->gene != NULL) {
			Generic* gen = (Generic*)local->gene;
			add_mapy(thgc, gen->vmap, createString(thgc, (char*)"", 0, 1), (char*)gj);
		}
		CallBlock* callblk = (CallBlock*)val2;
		Block* inner = (Block*)*(Obj**)get_list(callblk->children, 1);
		inner->obj = (Obj*)make_objblock_z(thgc, (Obj*)gj, 1);
		for (int bi = 0; bi < blocks->size; bi++) {
			add_list(thgc, gj->geneblocks, *get_list(blocks, bi));
		}
		(*n)++;
		return (Obj*)gj;
	}
	return self;
}
