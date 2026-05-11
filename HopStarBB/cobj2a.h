// ============================================================
// obj2a2.h - A解析パス (vtable dispatch版) 第2ブロック
// Comment, TagBlock, Signal, ServerClient等
// ============================================================

// 前方宣言
static void _exeCallBlockPairA(ThreadGC* thgc, Obj* callblk, Obj* owner, Master* local);
Obj* Comment_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Comment_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Comment2_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* Comment2_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Dolor_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* HtmObj_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* TagBlock_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* TagBlock_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* ElemType_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* ElemType_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ElemObj_opeA(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* Signal_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Signal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* SignalFunction_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* SignalFunction_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ServerClient_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* ServerClient_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ServerFunction_exeA(ThreadGC* thgc, Obj* self, Master* local);
Obj* ServerFunction_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Connect_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ConnectStock_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Address_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Mountain_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Sum_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Question_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* LinearFunction_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);

// ============================================================
// Comment::exeA
// ============================================================
Obj* Comment_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	Block* block = (Block*)*(Obj**)get_list(self->children, 0);
	ObjBlock* objblk = make_objblock_z(thgc, self, -1);
	block->obj = (Obj*)objblk;
	block->comment = true;

	// block.vmapA["div"] = new ElemType(ObjType.Div)
	Obj* et_div = make_cobj(thgc, CType::_CObj, LetterType::OT_Div, NULL);
	et_div->vt = &vt_ElemType;
	add_mapy(thgc, block->vmapA, createString(thgc, (char*)"div", 3, 1), (char*)et_div);

	// block.vmapA["br"] = new ElemType(ObjType.Br)
	Obj* et_br = make_cobj(thgc, CType::_CObj, LetterType::OT_Br, NULL);
	et_br->vt = &vt_ElemType;
	add_mapy(thgc, block->vmapA, createString(thgc, (char*)"br", 2, 1), (char*)et_br);

	// block.vmapA["sheet"] = new ElemType(ObjType.Sheet)
	Obj* et_sheet = make_cobj(thgc, CType::_CObj, LetterType::OT_Sheet, NULL);
	et_sheet->vt = &vt_ElemType;
	add_mapy(thgc, block->vmapA, createString(thgc, (char*)"sheet", 5, 1), (char*)et_sheet);

	// block.vmapA["cell"] = new ElemType(ObjType.Cell)
	Obj* et_cell = make_cobj(thgc, CType::_CObj, LetterType::OT_Cell, NULL);
	et_cell->vt = &vt_ElemType;
	add_mapy(thgc, block->vmapA, createString(thgc, (char*)"cell", 4, 1), (char*)et_cell);

	// block.vmapA["span"] = new ElemType(ObjType.Span)
	Obj* et_span = make_cobj(thgc, CType::_CObj, LetterType::OT_Span, NULL);
	et_span->vt = &vt_ElemType;
	add_mapy(thgc, block->vmapA, createString(thgc, (char*)"span", 4, 1), (char*)et_span);

	// block.vmapA["linear"] = new LinearFunction()
	Obj* linear = make_cobj(thgc, CType::_CObj, LetterType::OT_LinearFunction, NULL);
	linear->vt = &vt_LinearFunction;
	add_mapy(thgc, block->vmapA, createString(thgc, (char*)"linear", 6, 1), (char*)linear);

	pushBlock(thgc, local, block);
	// CommentLet handling (simplified)
	List* blocks = get_blocks(local);
	vt_exeA(thgc, (Obj*)block, local);
	blocks->size--;
	local->comments->size--;
	return self;
}

// ============================================================
// Comment::exepA
// ============================================================
Obj* Comment_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return Comment_exeA(thgc, self, local);
}

// ============================================================
// LinearFunction::PrimaryA
// ============================================================
Obj* LinearFunction_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		Block* blk = (Block*)GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		// return new AnimationFunction()
		return (Obj*)AnimationFunction_make(thgc);
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "linearの()が書かれていません");
}

// ============================================================
// Comment2::exeA
// ============================================================
Obj* Comment2_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// Comment2::exepA
// ============================================================
Obj* Comment2_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Dolor::exepA ($type, $func, $(expr))
// ============================================================
Obj* Dolor_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		(*n)++;
		// switch (word.name) { case "type": case "func": }
		if (str_matchA(word->name, "type", 4) || str_matchA(word->name, "func", 4)) {
			Clones* clones = Clones_make(thgc, self->letter);
			return (Obj*)clones;
		}
	} else if (val2->type == LetterType::OT_Bracket) {
		// C#: val2.Clone().exeA(local).GetterA(local)
		Obj* val = GetterA_vt(thgc, vt_exeA(thgc, vt_Clone(thgc, val2), local), local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$()は引数を1つしかとれません");
		val2 = *(Obj**)get_list(blk->rets, 0);
		ATSSpan* blk_letter0 = *(ATSSpan**)get_list(blk->letters, 0);
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
			val2->type == LetterType::OT_FloatVal || val2->type == LetterType::OT_BoolVal) {
			if (local->comments->size > 0) {
				Obj* htm = make_htmobj(thgc, blk_letter0);
				return htm;
			} else {
				return ErrorA(thgc, LetterType::OT_Error, blk_letter0, "$()は構造コメントブロックの中でしか使えません");
			}
		}
		return ErrorA(thgc, LetterType::OT_Error, blk_letter0, "$()は整数、文字列を出力します");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$の後ろには()かtype,funcが来ます");
}

// ============================================================
// HtmObj::exepA
// ============================================================
Obj* HtmObj_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (local->comments->size > 0) {
		return NULL;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "コメント内で宣言してください");
}

// ============================================================
// TagBlock::exepA
// ============================================================
Obj* TagBlock_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (local->comments->size > 0) {
		Block* blk1 = (Block*)GetterA_vt(thgc, vt_exeA(thgc, *(Obj**)get_list(self->children, 0), local), local);
		ObjBlock* objblk1 = make_objblock_z(thgc, self, 0);
		blk1->obj = (Obj*)objblk1;
		pushBlock(thgc, local, blk1);
		vt_exeA(thgc, (Obj*)blk1, local);

		Block* blk2 = (Block*)GetterA_vt(thgc, vt_exeA(thgc, *(Obj**)get_list(self->children, 1), local), local);
		ObjBlock* objblk2 = make_objblock_z(thgc, self, 1);
		blk2->obj = (Obj*)objblk2;
		pushBlock(thgc, local, blk2);
		vt_exeA(thgc, (Obj*)blk2, local);

		List* blocks = get_blocks(local);
		blocks->size--;
		blocks->size--;
		return NULL;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "$()は構造コメントブロックの中でしか使えません");
}

// ============================================================
// TagBlock::exeA
// ============================================================
Obj* TagBlock_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	return TagBlock_exepA(thgc, self, NULL, local, NULL);
}

// ============================================================
// ElemType::exepA
// ============================================================
Obj* ElemType_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Obj* val2 = *(Obj**)get_list(primary->children, *n + 1);
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		Obj* elemobj = make_elemobj(thgc, self, val2->letter);
		declareA(thgc, local, val2->letter->text, elemobj);
		return elemobj;
	}
	return self;
}

// ============================================================
// ElemType::PrimaryA
// ============================================================
Obj* ElemType_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		Obj* elemobj = make_elemobj(thgc, self, val2->letter);
		declareA(thgc, local, val2->letter->text, elemobj);
		return elemobj;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "elem変数の名前が宣言されていません");
}

// ============================================================
// ElemObj::opeA
// ============================================================
Obj* ElemObj_opeA(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if (val2 == NULL) {
		// unary operators +, !, *
		if (key == LetterType::_Plus || key == LetterType::_Not || key == LetterType::_Mul) {
			return self;
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "elem値は+,!,*の単項演算子しか作用しません");
}

// ============================================================
// Signal::exepA
// ============================================================
Obj* Signal_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Signal::PrimaryA
// ============================================================
Obj* Signal_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Word) {
		Obj* word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_CallBlock) {
			if (local->comments->size > 0) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数は構造コメントブロックの中では宣言出来ません");
			(*n)++;
			// SignalFunction作成
			Obj* func = make_function(thgc, LetterType::OT_Signal, word->letter);
			((Function*)func)->draw = (Block*)val2;
			declareA(thgc, local, word->letter->text, func);
			_exeCallBlockPairA(thgc, val2, func, local);
			return func;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数に{|}が宣言されていない");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数を宣言してください");
}

// ============================================================
// SignalFunction::exeA
// ============================================================
Obj* SignalFunction_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// SignalFunction::PrimaryA
// ============================================================
Obj* SignalFunction_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "signal関数はユーザーから呼び出せません");
}

// ============================================================
// ServerClient::exepA
// ============================================================
Obj* ServerClient_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// ServerClient::PrimaryA
// ============================================================
Obj* ServerClient_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			Obj* ret = make_cobj(thgc, CType::_CObj, LetterType::_None, val2->letter);
			return ret;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	} else if (val2->type == LetterType::OT_CallBlock) {
		if (local->comments->size > 0) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "server,client関数は構造コメントブロックの中では宣言出来ません");
		(*n)++;
		// ServerFunction作成
		Obj* sf = make_function(thgc, self->type, self->letter);
		((Function*)sf)->draw = (Block*)val2;
		declareA(thgc, local, self->letter->text, sf);
		add_mapy(thgc, local->sigmapA, self->letter->text, (char*)sf);
		_exeCallBlockPairA(thgc, val2, sf, local);
		return sf;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "server,clientの後ろには名前か{|}がきます");
}

// ============================================================
// ServerFunction::exeA
// ============================================================
Obj* ServerFunction_exeA(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// ServerFunction::PrimaryA
// ============================================================
Obj* ServerFunction_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, self->letter, "ServerClient関数は呼び出せません");
}

// ============================================================
// Connect::PrimaryA
// ============================================================
Obj* Connect_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Obj* word = val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (str_matchA(word->letter->text, "new", 3)) {
				// connect.new(...)
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					Block* blk1 = (Block*)val;
					if (blk1->rets->size == 1) {
						Obj* ret = make_cobj(thgc, CType::_CObj, LetterType::_None, (*(Obj**)get_list(blk1->rets, 0))->letter);
						return ret;
					}
					return ErrorA(thgc, LetterType::OT_Error, ((Block*)val)->letter, "connect.newの引数は0か1です");
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "connect.newの()が宣言されていません");
			}
			return ErrorA(thgc, LetterType::OT_Error, word->letter, "connectの後には.newが必要です");
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".が来ていません");
}

// ============================================================
// ConnectStock::PrimaryA
// ============================================================
Obj* ConnectStock_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Obj* word = val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (str_matchA(word->letter->text, "back", 4)) {
				return make_cobj(thgc, CType::_CObj, LetterType::_None, word->letter);
			} else if (str_matchA(word->letter->text, "Store", 5)) {
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					(*n)++;
					return make_voival(thgc);
				}
				return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Storeの()が宣言されていません");
			}
			return make_cobj(thgc, CType::_CObj, LetterType::_None, word->letter);
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	} else if (val2->type == LetterType::OT_Block) {
		Obj* blk01 = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
		if (blk01->type == LetterType::OT_Wait || blk01->type == LetterType::OT_Error || blk01->type == LetterType::OT_NG) return blk01;
		Block* blk = (Block*)blk01;
		if (blk->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "connectの引数は1つです");
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				Obj* word2 = val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (str_matchA(word2->letter->text, "Output", 6)) {
					if (val2->type == LetterType::OT_Bracket) {
						if (val2->children->size == 0) {
							return make_cobj(thgc, CType::_CObj, LetterType::_None, val2->letter);
						}
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Outputの引数は0です");
					}
					return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Outputに()がありません");
				} else if (str_matchA(word2->letter->text, "Select", 6)) {
					if (val2->type == LetterType::OT_Bracket) {
						Obj* val = GetterA_vt(thgc, vt_exeA(thgc, val2, local), local);
						if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
						return val;
					}
					return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Selectの()がありません");
				} else if (str_matchA(word2->letter->text, "First", 5)) {
					if (val2->type == LetterType::OT_Bracket) {
						if (val2->children->size == 0) {
							return make_cobj(thgc, CType::_CObj, LetterType::_None, word2->letter);
						}
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Firstの引数は0です");
					}
					return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Firstの()がありません");
				}
				return ErrorA(thgc, LetterType::OT_Error, word2->letter, "には宣言されていません");
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
		}
		return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が予測されます");
	}
	return make_cobj(thgc, CType::_CObj, LetterType::_None, val2->letter);
}

// ============================================================
// Address::PrimaryA
// ============================================================
Obj* Address_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			String* addr = ((Address*)self)->address;
			addr = StringAdd(thgc, addr, (char*)"/", 1, 1);
			addr = StringAdd(thgc, addr, val2->letter->text->data, val2->letter->text->size, val2->letter->text->esize);
			((Address*)self)->address = addr;
		} else {
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
		}
	} else if (val2->type == LetterType::OT_Bracket) {
		Obj* cloned = vt_Clone(thgc, val2);
		Obj* block = GetterA_vt(thgc, vt_exeA(thgc, cloned, local), local);
		if (block->type == LetterType::OT_Wait || block->type == LetterType::OT_Error || block->type == LetterType::OT_NG) return block;
		((Address*)self)->draw = (Block*)block;
		(*n)++;
		return self;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "addressの後ろには.か()がきます");
}

// ============================================================
// Mountain::exepA (^ range expression)
// ============================================================
Obj* Mountain_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	if (val2->type == LetterType::OT_Word) {
		Block* arr = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
		return (Obj*)arr;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "範囲の記述式がきていません");
}

// ============================================================
// Sum::PrimaryA
// ============================================================
Obj* Sum_PrimaryA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		return make_number(thgc, NULL);
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Sumの()がありません");
}

// ============================================================
// Question::exepA (?)
// ============================================================
Obj* Question_exepA(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Block* blk = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
	Block* blk1 = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
	add_list(thgc, blk->rets, (char*)blk1);
	add_list(thgc, blk->rets, (char*)blk1);
	for (int i = 0; i < 2; i++) {
		Obj* num = make_number(thgc, NULL);
		add_list(thgc, blk1->rets, (char*)num);
	}
	return (Obj*)blk1;
}

// ============================================================
// A2_initVTables - obj2a2.h用VTable初期化
// ============================================================
void A2_initVTables() {
	// --- Comment ---
	vt_Comment.exeA = Comment_exeA;
	vt_Comment.exepA = Comment_exepA;
	vt_lookup[(int)LetterType::OT_Comment] = &vt_Comment;

	// --- Comment2 ---
	vt_Comment2.exeA = Comment2_exeA;
	vt_Comment2.exepA = Comment2_exepA;
	vt_lookup[(int)LetterType::OT_Comment2] = &vt_Comment2;

	// --- Dolor ---
	vt_Dolor.exepA = Dolor_exepA;
	vt_lookup[(int)LetterType::OT_Dolor] = &vt_Dolor;

	// --- HtmObj ---
	vt_HtmObj.exepA = HtmObj_exepA;
	vt_lookup[(int)LetterType::OT_Htm] = &vt_HtmObj;

	// --- TagBlock (inherits CallBlock Z-pass, has own A/B/C-pass) ---
	vt_TagBlock.exeA = TagBlock_exeA;
	vt_TagBlock.exepA = TagBlock_exepA;
	vt_TagBlock.exeZ = (Obj*(*)(ThreadGC*, Obj*, Master*))CallBlock_exeZ;
	vt_TagBlock.exepZ = (Obj*(*)(ThreadGC*, Obj*, int*, Master*, Primary*))CallBlock_exepZ;
	vt_lookup[(int)LetterType::OT_TagBlock] = &vt_TagBlock;

	// --- Signal ---
	vt_Signal.exepA = Signal_exepA;
	vt_Signal.PrimaryA = Signal_PrimaryA;
	vt_lookup[(int)LetterType::OT_Signal] = &vt_Signal;

	// --- ServerClient ---
	vt_ServerClient.exepA = ServerClient_exepA;
	vt_ServerClient.PrimaryA = ServerClient_PrimaryA;
	vt_lookup[(int)LetterType::OT_ServerClient] = &vt_ServerClient;

	// --- ElemType ---
	vt_ElemType.exepA = ElemType_exepA;
	vt_ElemType.PrimaryA = ElemType_PrimaryA;

	// --- ElemObj ---
	vt_ElemObj.opeA = ElemObj_opeA;

	// --- SignalFunction ---
	vt_SignalFunction.exeA = SignalFunction_exeA;
	vt_SignalFunction.PrimaryA = SignalFunction_PrimaryA;
	vt_lookup[(int)LetterType::OT_SignalFunction] = &vt_SignalFunction;

	// --- ServerFunction ---
	vt_ServerFunction.exeA = ServerFunction_exeA;
	vt_ServerFunction.PrimaryA = ServerFunction_PrimaryA;
	vt_lookup[(int)LetterType::OT_ServerFunction] = &vt_ServerFunction;

	// --- Connect ---
	vt_Connect.PrimaryA = Connect_PrimaryA;

	// --- ConnectStock ---
	vt_ConnectStock.PrimaryA = ConnectStock_PrimaryA;
	vt_lookup[(int)LetterType::OT_ConnectStock] = &vt_ConnectStock;

	// --- Address ---
	vt_Address.PrimaryA = Address_PrimaryA;

	// --- Mountain ---
	vt_Mountain.exepA = Mountain_exepA;
	vt_lookup[(int)LetterType::OT_Mountain] = &vt_Mountain;

	// --- Sum ---
	vt_Sum.PrimaryA = Sum_PrimaryA;

	// --- Question ---
	vt_Question.exepA = Question_exepA;

	// --- LinearFunction ---
	vt_LinearFunction.PrimaryA = LinearFunction_PrimaryA;
	vt_lookup[(int)LetterType::OT_LinearFunction] = &vt_LinearFunction;
}
