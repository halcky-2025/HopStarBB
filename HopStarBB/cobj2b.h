// ============================================================
// obj2b3.h - B解析パス (vtable dispatch版) - Block 3
// Comment, LinearFunction, AnimationFunction, Comment2,
// Clones, Dolor, HtmObj, TagBlock, ElemType, ElemObj,
// Signal, SignalFunction, ServerClient, ServerFunction,
// Connect, ConnectStock, Address, Mountain, Sum, Question
// ============================================================

// 前方宣言
Obj* Comment_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Comment_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* LinearFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Comment2_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* Comment2_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Dolor_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* HtmObj_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* TagBlock_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* TagBlock_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* ElemType_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* ElemType_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ElemObj_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* Signal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Signal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* SignalFunction_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* SignalFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ServerClient_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* ServerClient_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ServerFunction_exeB(ThreadGC* thgc, Obj* self, Master* local);
Obj* ServerFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Connect_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ConnectStock_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Address_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Mountain_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Sum_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Question_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);

// ============================================================
// Comment::exeB, exepB
// ============================================================
Obj* Comment_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	List* blocks = get_blocks(local);
	Block* block = (Block*)(*(Obj**)get_list(self->children, 0));
	add_list(thgc, blocks, (char*)block);
	vt_exeB(thgc, (Obj*)block, local);
	blocks->size--;
	return self;
}
Obj* Comment_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	Block* block = (Block*)(*(Obj**)get_list(self->children, 0));
	add_list(thgc, blocks, (char*)block);
	vt_exeB(thgc, (Obj*)block, local);
	blocks->size--;
	return self;
}

// ============================================================
// LinearFunction::PrimaryB
// ============================================================
Obj* LinearFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		add_list(thgc, local->calls, (char*)KouhoSet2);
		local->elem_flag = true;
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->elem_flag = false;
		local->calls->size--;
		if (IS_WEG(val)) return val;
		Block* blk = (Block*)val;
		AnimationFunction* anif = AnimationFunction_make(thgc);
		int retsCount = blk->rets->size;
		bool* bools = (bool*)alloca(sizeof(bool) * retsCount);
		for (int i = 0; i < retsCount; i++) bools[i] = false;
		if (blk->labelmap != NULL && blk->labelmap->kvs != NULL) {
			for (int k = 0; k < blk->labelmap->kvs->size; k++) {
				KV* kv = *(KV**)get_list(blk->labelmap->kvs, k);
				int n2 = 0;
				if (str_try_parse_int(kv->key, &n2)) {
					Label* lab = (Label*)kv->value;
					bools[lab->n] = true;
					Obj* retval = *(Obj**)get_list(blk->rets, lab->n);
					if (retval->type == LetterType::OT_Block || retval->type == LetterType::OT_Array) {
						Block* blk1 = (Block*)retval;
						TagBlock* tag = NULL;
						List* blocks = get_blocks(local);
						if (blocks != NULL) {
							for (int i = blocks->size - 1; i >= 0; i--) {
								Block* blk2 = *(Block**)get_list(blocks, i);
								if (blk2->obj != NULL && blk2->obj->type == LetterType::OT_TagBlock) {
									tag = (TagBlock*)blk2->obj;
									break;
								}
							}
						}
						// TODO: anif.SetB(tag.divobj.elem, blk1, local) — Div/Span/SetParamB not yet ported
					}
					else {
						ATSSpan* let = *(ATSSpan**)get_list(blk->letters, lab->n);
						return ErrorA(thgc, LetterType::OT_Error, let, "アニメーションするStyleを指定してください");
					}
				}
			}
		}
		for (int i = 0; i < retsCount; i++) {
			if (bools[i]) continue;
			Obj* val3 = *(Obj**)get_list(blk->rets, i);
			if (val3->type == LetterType::OT_FloatVal) {
				anif->interval = ((FloatVal*)val3)->value;
			}
			else if (val3->type == LetterType::OT_Number) {
				anif->loop = ((Number*)val3)->value;
			}
			else {
				ATSSpan* let = *(ATSSpan**)get_list(blk->letters, 0);
				return ErrorA(thgc, LetterType::OT_Error, let, "インターバルは小数、繰り返しの回数は整数で引数を与えてください");
			}
		}
		return (Obj*)anif;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "linearの()が書かれていません");
}

// ============================================================
// Comment2::exeB, exepB
// ============================================================
Obj* Comment2_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* Comment2_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }

// ============================================================
// Dolor::exepB
// ============================================================
Obj* Dolor_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	if (val2->type == LetterType::OT_Word) {
		if (val2->letter == local->letter && local->kouhos == NULL) {
			local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
			add_kouho(thgc, local->kouhos, createString(thgc, "type", 4, 1), NULL);
			add_kouho(thgc, local->kouhos, createString(thgc, "func", 4, 1), NULL);
		}
		Word* word = (Word*)val2;
		(*n)++;
		if (str_equals(word->name, "type")) {
			Clones* clones = Clones_make(thgc, self->letter);
			return (Obj*)clones;
		}
		else if (str_equals(word->name, "func")) {
			// func: break (fall through to end)
		}
	}
	else if (val2->type == LetterType::OT_Bracket) {
		add_list(thgc, local->calls, (char*)KouhoSet);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		local->calls->size--;
		if (IS_WEG(val)) return val;
		Block* blk = (Block*)val;
		if (blk->rets->size != 1)
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$()は引数を1つしかとれません");
		val2 = *(Obj**)get_list(blk->rets, 0);
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj) {
			if (local->comment_flag) return NULL;
			else {
				ATSSpan* let = *(ATSSpan**)get_list(blk->letters, 0);
				return ErrorA(thgc, LetterType::OT_Error, let, "$()は構造コメントブロックの中でしか使えません");
			}
		}
		else {
			ATSSpan* let = *(ATSSpan**)get_list(blk->letters, 0);
			return ErrorA(thgc, LetterType::OT_Error, let, "$()は整数、文字列を出力します");
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$の後ろには()かtype,funcが来ます");
}

// ============================================================
// HtmObj::exepB
// ============================================================
Obj* HtmObj_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (local->comment_flag) {
		return NULL;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "コメント内で宣言してください");
}

// ============================================================
// TagBlock::exepB, exeB
// ============================================================
Obj* TagBlock_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (local->comment_flag) {
		return NULL;
	}
	return self;
}
Obj* TagBlock_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// ElemType::exepB, PrimaryB
// ============================================================
Obj* ElemType_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Obj* val2 = *(Obj**)get_list(primary->children, *n + 1);
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		Word* word = (Word*)val2;
		Obj* elemobj = make_elemobj(thgc, self, val2->letter);
		declareB(thgc, local, word->name, elemobj);
		return elemobj;
	}
	return self;
}
Obj* ElemType_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		(*n)++;
		Obj* elemobj = make_elemobj(thgc, self, val2->letter);
		declareB(thgc, local, word->name, elemobj);
		return elemobj;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "elem変数の名前が宣言されていません");
}

// ============================================================
// ElemObj::opeB
// ============================================================
Obj* ElemObj_opeB(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if ((key == LetterType::_Plus || key == LetterType::_Not || key == LetterType::_Mul) && val2 == NULL) {
		ElemObj* eo = (ElemObj*)self;
		if (eo->type != NULL) {
			LetterType tt = eo->type->type;
			if (tt == LetterType::OT_Div || tt == LetterType::OT_Sheet || tt == LetterType::OT_Cell) {
				eo->sop = key;
			}
		}
		eo->key = key;
		return self;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "elem値は+,!,*の単項演算子しか作用しません");
}

// ============================================================
// Signal::exepB, PrimaryB
// ============================================================
Obj* Signal_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* Signal_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		Obj* ch = *(Obj**)get_list(primary->children, *n);
		if (ch->type == LetterType::OT_CallBlock) {
			(*n)++;
			return getB(thgc, local, val2->letter->text, val2->letter);
		}
		return ErrorA(thgc, LetterType::OT_Error, ch->letter, "signal関数に{|}が宣言されていない");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数を宣言してください");
}

// ============================================================
// SignalFunction::exeB, PrimaryB
// ============================================================
Obj* SignalFunction_exeB(ThreadGC* thgc, Obj* self, Master* local) {
	Function* sf = (Function*)self;
	if (sf->draw == NULL) return self;
	// clone block1 from draw.children[0]
	Block* block1 = (Block*)vt_Clone(thgc, *(Obj**)get_list(((Obj*)sf->draw)->children, 0));
	// varray = block1.vmapA.Values
	if (block1->vmapA != NULL && block1->vmapA->kvs != NULL) {
		for (int i = 0; i < block1->vmapA->kvs->size && i < 2; i++) {
			KV* kv = *(KV**)get_list(block1->vmapA->kvs, i);
			Variable* var = (Variable*)kv->value;
			if (var != NULL && ((Obj*)var)->type == LetterType::OT_Variable) {
				if (var->cls != NULL && var->cls->type == LetterType::OT_Var) {
					var->cls = make_stocktype(thgc);
				}
			}
		}
	}
	// local.blockslist.Add(blocks)
	add_list(thgc, local->blockslist, (char*)sf->blocks);
	// local.blocks.Add(block1)
	List* blocks = get_blocks(local);
	add_list(thgc, blocks, (char*)block1);
	// block2 = draw.children[1].Clone().exeB().GetterB()
	Obj* cloned = vt_Clone(thgc, *(Obj**)get_list(((Obj*)sf->draw)->children, 1));
	Block* block2 = (Block*)GetterB_vt(thgc, vt_exeB(thgc, cloned, local), local);
	// val2 = block2.rets.Last()
	Obj* val2 = *(Obj**)get_list(block2->rets, block2->rets->size - 1);
	// C#: if (val2.type == ObjType.Return) { val2 = (val2 as Return).value;
	//       if (val2.type == ObjType.Comment) { local.vision.addcomment(comment); local.vision.panel.input = true; local.vision.panel.Invalidate(); } }
	if (val2->type == LetterType::OT_Return) {
		val2 = ((Return*)val2)->value;
		if (val2->type == LetterType::OT_Comment) {
			// C#: local.vision.addcomment(comment) — vision/panel未移植のためcommentsリストに追加
			add_list(thgc, local->comments, (char*)val2);
			// C#: local.vision.panel.input = true — 未移植
			// C#: local.vision.panel.Invalidate() — 未移植
		}
	}
	// C#: else if (val2.type == ObjType.Continue && val2.type == ObjType.Break && val2.type == ObjType.Goto) throw new Exception();
	else if (val2->type == LetterType::OT_Continue || val2->type == LetterType::OT_Break || val2->type == LetterType::OT_Goto) {
		local->blockslist->size--;
		return ErrorA(thgc, LetterType::OT_Error, self->letter, "signal関数内でcontinue/break/gotoは使えません");
	}
	// C#: if (val2.type != ObjType.Comment) throw new Exception();
	if (val2->type != LetterType::OT_Comment) {
		local->blockslist->size--;
		return ErrorA(thgc, LetterType::OT_Error, self->letter, "signal関数の戻り値はコメントである必要があります");
	}
	// C#: local.blockslist.RemoveAt(local.blockslist.Count - 1)
	local->blockslist->size--;
	return val2;
}
Obj* SignalFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "signal関数はユーザーから呼び出せません");
}

// ============================================================
// ServerClient::exepB, PrimaryB
// ============================================================
Obj* ServerClient_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) { return self; }
Obj* ServerClient_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	ServerClient* sc = (ServerClient*)self;
	// C#: if (val2.type == ObjType.Dot)
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// C#: if (val2.type == ObjType.Word)
		if (val2->type == LetterType::OT_Word) {
			// C#: if (val2.letter == local.letter && local.kouhos == null) local.KouhoSetSignal();
			if (val2->letter == local->letter && local->kouhos == NULL) {
				KouhoSetSignal(thgc, local);
			}
			Word* word = (Word*)val2;
			(*n)++;
			// C#: var sf = local.sigmapA[name] as ServerFunction
			Obj* sf = (Obj*)get_mapy(local->sigmapA, sc->name);
			if (sf == NULL) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "sigmapAにserver/clientが見つかりません");
			// C#: var obj = (sf.draw.children[1] as Block).vmapA[word.name]
			Block* blk = (Block*)*(Obj**)get_list(((Obj*)((Function*)sf)->draw)->children, 1);
			Obj* obj = (Obj*)get_mapy(blk->vmapA, word->name);
			// C#: if (obj.type == ObjType.SignalFunction) return obj;
			if (obj != NULL && obj->type == LetterType::OT_SignalFunction) {
				return obj;
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数ではありません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	// C#: else if (val2.type == ObjType.CallBlock) { n++; var sf = local.sigmapA[name]; return sf; }
	else if (val2->type == LetterType::OT_CallBlock) {
		(*n)++;
		Obj* sf = (Obj*)get_mapy(local->sigmapA, sc->name);
		if (sf != NULL) return sf;
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "sigmapAにserver/clientが見つかりません");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "server,clientの後ろには名前か{|}がきます");
}

// ============================================================
// ServerFunction::exeB, PrimaryB
// ============================================================
Obj* ServerFunction_exeB(ThreadGC* thgc, Obj* self, Master* local) { return self; }
Obj* ServerFunction_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, self->letter, "ServerClient関数は呼び出せません");
}

// ============================================================
// Connect::PrimaryB
// ============================================================
Obj* Connect_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	// C#: if (val2.type == ObjType.Dot)
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// C#: if (val2.type == ObjType.Word)
		if (val2->type == LetterType::OT_Word) {
			// C#: if (val2.letter == local.letter && local.kouhos == null) { kouhos = { "new" } }
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				add_kouho(thgc, local->kouhos, createString(thgc, "new", 3, 1), NULL);
			}
			Word* word = (Word*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			// C#: if (word.name == "new")
			if (str_equals(word->name, "new")) {
				// C#: if (val2.type == ObjType.Bracket)
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
					if (IS_WEG(val)) return val;
					Block* blk1 = (Block*)val;
					// C#: if (blk1.rets.Count == 1)
					if (blk1->rets->size == 1) {
						Obj* ret0 = *(Obj**)get_list(blk1->rets, 0);
						// C#: if (rets[0].type == ServerClient) return new ConnectStock(sigmapA[name])
						if (ret0->type == LetterType::OT_ServerClient) {
							Obj* sf = (Obj*)get_mapy(local->sigmapA, ((ServerClient*)ret0)->name);
							return (Obj*)make_connectstock(thgc, sf);
						}
						// C#: else if (rets[0].type == SignalFunction) return new ConnectStock(rets[0])
						else if (ret0->type == LetterType::OT_SignalFunction) {
							return (Obj*)make_connectstock(thgc, ret0);
						}
						else {
							ATSSpan* let = *(ATSSpan**)get_list(blk1->letters, 0);
							return ErrorA(thgc, LetterType::OT_Error, let, "connect.newはアドレスしか引数にとりません");
						}
					}
					else return ErrorA(thgc, LetterType::OT_Error, blk1->letter, "connect.newの引数は0か1です");
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "connect.newの()が宣言されていません");
			}
			// C#: else return Error("connectにXは宣言されていません")
			else return ErrorA(thgc, LetterType::OT_Error, word->letter, "connectに宣言されていません");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".が来ていません");
}

// ============================================================
// ConnectStock::PrimaryB
// ============================================================
Obj* ConnectStock_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	ConnectStock* cstock = (ConnectStock*)self;
	// C#: if (val2.type == ObjType.Dot)
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// C#: if (val2.type == ObjType.Word)
		if (val2->type == LetterType::OT_Word) {
			// C#: kouhos = { "send", "back", "Store" }
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				add_kouho(thgc, local->kouhos, createString(thgc, "send", 4, 1), NULL);
				add_kouho(thgc, local->kouhos, createString(thgc, "back", 4, 1), NULL);
				add_kouho(thgc, local->kouhos, createString(thgc, "Store", 5, 1), NULL);
			}
			Word* word = (Word*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			// C#: if (word.name == "back") return new Value(new AddressType())
			if (str_equals(word->name, "back")) {
				Value* variable = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
				// C#: new AddressType()
				((Val*)variable)->cls = make_addresstype(thgc);
				return (Obj*)variable;
			}
			// C#: else if (word.name == "Store")
			else if (str_equals(word->name, "Store")) {
				// C#: if (val2.type == ObjType.Bracket)
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
					if (IS_WEG(val)) return val;
					Block* block = (Block*)val;
					// C#: for each ret, check ModelValue or GeneValue
					for (int i = 0; i < block->rets->size; i++) {
						Obj* ri = *(Obj**)get_list(block->rets, i);
						if (ri->type != LetterType::OT_ModelVal && ri->type != LetterType::OT_GeneVal) {
							ATSSpan* let = *(ATSSpan**)get_list(block->letters, 0);
							return ErrorA(thgc, LetterType::OT_Error, let, "Storeできるのはmodel変数かgene変数だけです");
						}
					}
					(*n)++;
					return (Obj*)make_voival(thgc);
				}
				else return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Storeの()が宣言されていません");
			}
			// C#: else { insblocks = local.blocks; local.blocks = (value.draw.children[0]).blocks; v = getB(word.name); local.blocks = insblocks; ... }
			else {
				// C#: insblocks = local.blocks; local.blocks = (value.draw.children[0] as Block).blocks; v = getB(word.name); local.blocks = insblocks;
				if (cstock->value != NULL && ((Function*)cstock->value)->draw != NULL) {
					Block* drblk = (Block*)*(Obj**)get_list(((Obj*)((Function*)cstock->value)->draw)->children, 0);
					// C# local.blocks setter = blockslist.Add(value)
					List* insblocks = get_blocks(local);
					// C#: local.blocks = (value.draw.children[0] as Block).blocks
					add_list(thgc, local->blockslist, (char*)drblk->blocks);
					Obj* v = getB(thgc, local, word->name, word->letter);
					// C#: local.blocks = insblocks
					add_list(thgc, local->blockslist, (char*)insblocks);
					// C#: if (v.type == ServerFunction || SignalFunction) return new ConnectStock(v)
					if (v->type == LetterType::OT_ServerFunction || v->type == LetterType::OT_SignalFunction) {
						return (Obj*)make_connectstock(thgc, v);
					}
					else return v;
				}
				return ErrorA(thgc, LetterType::OT_Error, word->letter, "connectの値が不正です");
			}
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	// C#: else if (val2.type == ObjType.Block)
	else if (val2->type == LetterType::OT_Block) {
		Block* blk01 = (Block*)GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		if (blk01->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "connectの引数は1つです");
		Obj* val1 = *(Obj**)get_list(blk01->rets, 0);
		if (val1->type != LetterType::OT_ModelObj && val1->type != LetterType::OT_GeneObj) return ErrorA(thgc, LetterType::OT_Error, val1->letter, "ModelクラスかGeneクラスを指定してください");
		ModelObj* modelobj = (ModelObj*)val1;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);

		// C#: if (val2.type == ObjType.Dot)
		if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			// C#: if (val2.type == ObjType.Word)
			if (val2->type == LetterType::OT_Word) {
				// C#: kouhos = { "Output", "Select", "First" }
				if (val2->letter == local->letter && local->kouhos == NULL) {
					local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
					add_kouho(thgc, local->kouhos, createString(thgc, "Output", 6, 1), NULL);
					add_kouho(thgc, local->kouhos, createString(thgc, "Select", 6, 1), NULL);
					add_kouho(thgc, local->kouhos, createString(thgc, "First", 5, 1), NULL);
				}
				Word* word2 = (Word*)val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				// C#: if (word2.name == "Output")
				if (str_equals(word2->name, "Output")) {
					if (val2->type == LetterType::OT_Bracket) {
						if (val2->children->size == 0) {
							// C#: return new ElemObj(local, local.getA("sheet") as ElemType, null)
							Obj* sheet = (Obj*)get_mapy(((Block*)*(Obj**)get_list(get_blocks(local), 0))->vmapA, createString(thgc, "sheet", 5, 1));
							if (sheet != NULL) return (Obj*)make_elemobj(thgc, sheet, NULL);
							return (Obj*)make_elemobj(thgc, make_cobj(thgc, CType::_CObj, LetterType::OT_Sheet, NULL), NULL);
						}
						else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Outputの引数は0です");
					}
					else return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Outputの()がありません");
				}
				// C#: else if (word2.name == "Select")
				else if (str_equals(word2->name, "Select")) {
					if (val2->type == LetterType::OT_Bracket) {
						Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
						if (IS_WEG(val)) return val;
						Block* blk0 = (Block*)val;
						if (blk0->rets->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Selectの引数は1です");
						Obj* ret0 = *(Obj**)get_list(blk0->rets, 0);
						if (ret0->type != LetterType::OT_Function) return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk0->letters, 0), "Selectの引数は()でなければなりません");
						Function* func = (Function*)ret0;
						Block* blk = (Block*)*(Obj**)get_list(((Obj*)func->draw)->children, 0);
						if (blk->vmapA->kvs->size != 1) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Selectの関数の引数は1です");
						// C#: var ftype = new FuncType(local.Bool); ftype.draws.Add(modelobj); TypeCheck.CheckCVB(ftype, func, Setter, local)
						Obj* ftype = (Obj*)make_functype(thgc, (Obj*)local->Bool);
						add_list(thgc, ((FuncTypeC*)ftype)->draws, (char*)modelobj);
						Obj* ret = CheckCVB_TypeCheck(thgc, ftype, (Obj*)func, CheckType_Setter, local);
						if (IS_WEG(ret)) return ret;
						return (Obj*)blk;
					}
					else return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Selectの()がありません");
				}
				// C#: else if (word2.name == "First")
				else if (str_equals(word2->name, "First")) {
					if (val2->type == LetterType::OT_Bracket) {
						if (val2->children->size == 0) {
							// C#: return Value.New(modelobj, local, word2.letter)
							return New_Value(thgc, (Obj*)modelobj, local, word2->letter);
						}
						else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Firstの引数は0です");
					}
					else return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Firstの()がありません");
				}
				else return ErrorA(thgc, LetterType::OT_Error, word2->letter, "宣言されていません");
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
		}
		else return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が予測されます");
	}
	// C#: else if (value.type == ServerFunction) return this;
	else if (cstock->value->type == LetterType::OT_ServerFunction) {
		return self;
	}
	// C#: else if (value.type == SignalFunction) { if (val2.type == Bracket) ... }
	else if (cstock->value->type == LetterType::OT_SignalFunction) {
		if (val2->type == LetterType::OT_Bracket) {
			Block* blk = (Block*)GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
			if (blk->rets->size == 0) {
				return (Obj*)make_voival(thgc);
			}
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Connectorの引数は0です");
		}
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が予測されます");
}

// ============================================================
// Address::PrimaryB
// ============================================================
Obj* Address_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Address* addr = (Address*)self;
	// C#: if (val2.type == ObjType.Dot)
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// C#: if (val2.type == ObjType.Word)
		if (val2->type == LetterType::OT_Word) {
			// C#: if (val2.letter == local.letter && local.kouhos == null) local.kouhos = new SortedList<string, Obj>();
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
			}
			Word* word = (Word*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			// C#: if (val2.type == ObjType.Dot)
			if (val2->type == LetterType::_Dot) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				// C#: if (val2.type == ObjType.ServerClient)
				if (val2->type == LetterType::OT_ServerClient) {
					(*n)++;
					// C#: var sc = val2 as ServerClient;
					// C#: var p = local.panel.form.LoadProject(address + "." + word.name, sc.name);
					// C#: return (p.children[0] as Block).vmapA[(val2 as ServerClient).name];
					// TODO: panel.form.LoadProject not yet ported
					return NULL;
				}
				else {
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "addressの後ろにはserverかclientがきます");
				}
			}
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "addressの後ろには.か()がきます");
}

// ============================================================
// Mountain::exepB
// ============================================================
Obj* Mountain_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	// C#: var rets = new List<List<int>>();
	List* rets_list = create_list(thgc, sizeof(List*), CType::_List);
head_mountain:
	// C#: if (val2.type == ObjType.Word)
	if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		bool num = true;
		int n2 = word->name->size;
		// C#: List<int> ret = new List<int>();
		List* ret = create_list(thgc, sizeof(int), CType::_Struct);
		// C#: for (var i = word.name.Length - 1; i >= 0; i--)
		for (int i = word->name->size - 1; i >= 0; i--) {
			wchar_t c = GetChar(word->name, i);
			// C#: if (num)
			if (num) {
				// C#: if ('0' <= word.name[i] && word.name[i] <= '9')
				if (L'0' <= c && c <= L'9') {
					// C#: if (!num) { ... } — dead code in original, num is always true here
					if (!num) {
						int n3 = 0;
						for (int j = i + 1; j < n2; j++) {
							wchar_t cj = GetChar(word->name, j);
							n3 *= ('Z' - 'A' + 1);
							n3 += cj - L'A' + 1;
						}
						add_list_v(thgc, ret, &n3);
					}
				}
				// C#: else if ('A' <= word.name[i] && word.name[i] <= 'Z')
				else if (L'A' <= c && c <= L'Z') {
					// C#: if (num) { var text = word.name.Substring(i + 1, n2 - i - 1); ret.Add(Convert.ToInt32(text) - 1); n2 = i + 1; num = !num; }
					if (num) {
						int val_int = 0;
						for (int j = i + 1; j < n2; j++) {
							wchar_t cj = GetChar(word->name, j);
							val_int = val_int * 10 + (cj - L'0');
						}
						val_int -= 1; // C#: Convert.ToInt32(text) - 1
						add_list_v(thgc, ret, &val_int);
						n2 = i + 1;
						num = !num;
					}
				}
				else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "範囲の記述方式が間違っています");
			}
			// C#: when num is false, loop body is skipped (no else branch)
		}
		// C#: after loop
		if (num) {
			// C#: var text = word.name.Substring(0, n2); ret.Add(Convert.ToInt32(text) - 1);
			int val_int = 0;
			for (int j = 0; j < n2; j++) {
				wchar_t cj = GetChar(word->name, j);
				val_int = val_int * 10 + (cj - L'0');
			}
			val_int -= 1;
			add_list_v(thgc, ret, &val_int);
		}
		else {
			// C#: var n3 = 0; for (j=0; j<n2; j++) { n3 *= ('Z'-'A'+2); n3 += word.name[j]-'A'+1; } ret.Add(n3-1);
			int n3 = 0;
			for (int j = 0; j < n2; j++) {
				wchar_t cj = GetChar(word->name, j);
				n3 *= ('Z' - 'A' + 2);
				n3 += cj - L'A' + 1;
			}
			n3 -= 1;
			add_list_v(thgc, ret, &n3);
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		add_list(thgc, rets_list, (char*)ret);
		// C#: if (rets.Count == 1)
		if (rets_list->size == 1) {
			// C#: if (val2.type == ObjType.Mountain) { n++; val2 = primary.children[n]; goto head; }
			if (val2->type == LetterType::OT_Mountain) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head_mountain;
			}
			// C#: var blk = new Block(ObjType.Array); var blk1 = new Block(ObjType.Array);
			Block* blk = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
			Block* blk1 = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
			// C#: blk.rets.Add(blk1); blk.rets.Add(blk1);
			add_list(thgc, blk->rets, (char*)blk1);
			add_list(thgc, blk->rets, (char*)blk1);
			// C#: for (i=0; i<rets[0].Count; i++) blk1.rets.Add(new Number(rets[0][i]) { cls = local.Int });
			List* ret0 = *(List**)get_list(rets_list, 0);
			for (int i = 0; i < ret0->size; i++) {
				Obj* num_obj = make_number(thgc, NULL);
				((Number*)num_obj)->value = *(int*)get_list(ret0, i);
				((Val*)num_obj)->cls = (Obj*)local->Int;
				add_list(thgc, blk1->rets, (char*)num_obj);
			}
			return (Obj*)blk;
		}
		else {
			// C#: n--;
			(*n)--;
			// C#: var blk = new Block(ObjType.Array);
			Block* blk = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
			Block* blk1 = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
			add_list(thgc, blk->rets, (char*)blk1);
			Block* blk2 = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
			add_list(thgc, blk->rets, (char*)blk2);
			// C#: for (i=0; i<rets[0].Count; i++) blk1.rets.Add(new Number(rets[0][i]) { cls = local.Int });
			List* ret0 = *(List**)get_list(rets_list, 0);
			for (int i = 0; i < ret0->size; i++) {
				Obj* num_obj = make_number(thgc, NULL);
				((Number*)num_obj)->value = *(int*)get_list(ret0, i);
				((Val*)num_obj)->cls = (Obj*)local->Int;
				add_list(thgc, blk1->rets, (char*)num_obj);
			}
			// C#: for (i=0; i<rets[1].Count; i++) blk2.rets.Add(new Number(rets[1][i]) { cls = local.Int });
			List* ret1 = *(List**)get_list(rets_list, 1);
			for (int i = 0; i < ret1->size; i++) {
				Obj* num_obj = make_number(thgc, NULL);
				((Number*)num_obj)->value = *(int*)get_list(ret1, i);
				((Val*)num_obj)->cls = (Obj*)local->Int;
				add_list(thgc, blk2->rets, (char*)num_obj);
			}
			return (Obj*)blk;
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "範囲の記述式がきていません");
}

// C#: void SetArrayB(Action<int[]> task, int[] nums, List<List<int>> minimax, int n, Local local)
void SetArrayB(ThreadGC* thgc, Obj* self, int* array, List* minmax0, List* minmax1, int idx, Master* local, int* n2) {
	// C#: if (n == minimax[0].Count) { task(nums); return; }
	if (idx == minmax0->size) {
		// C#: c.count++;
		// C#: var text = sheet.cells[nums[0]][nums[1]].Text3(local).Trim();
		// C#: if (text != "") n2 += Convert.ToInt32(text);
		// C#: cells.Add(sheet.cells[nums[0]][nums[1]]);
		// TODO: sheet.cells not yet ported — task(nums) callback goes here
		return;
	}
	// C#: for (var i = minimax[0][n]; i <= minimax[1][n]; i++) { nums[n] = i; SetArrayB(task, nums, minimax, n + 1, local); }
	int lo = *(int*)get_list(minmax0, idx);
	int hi = *(int*)get_list(minmax1, idx);
	for (int i = lo; i <= hi; i++) {
		array[idx] = i;
		SetArrayB(thgc, self, array, minmax0, minmax1, idx + 1, local, n2);
	}
}

// ============================================================
// Sum::PrimaryB
// ============================================================
Obj* Sum_PrimaryB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	// C#: if (val2.type == ObjType.Bracket)
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		// C#: local.calls.Add(local.KouhoSet2);
		add_list(thgc, local->calls, (char*)KouhoSet2);
		// C#: var val = val2.exeB(local).GetterB(local);
		Obj* val = GetterB_vt(thgc, vt_exeB(thgc, val2, local), local);
		// C#: local.calls.RemoveAt(local.calls.Count - 1);
		local->calls->size--;
		if (IS_WEG(val)) return val;
		Block* blk = (Block*)val;
		// C#: var rets = new List<List<int>>();
		List* rets = create_list(thgc, sizeof(List*), CType::_List);
		// C#: if (blk.rets.Count == 1)
		if (blk->rets->size == 1) {
			// C#: if (blk.rets[0].type == ObjType.Array)
			Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
			if (ret0->type == LetterType::OT_Array) {
				// C#: val = blk.rets[0].exeB(local).GetterB(local);
				val = GetterB_vt(thgc, vt_exeB(thgc, ret0, local), local);
				if (IS_WEG(val)) return val;
				Block* blk2 = (Block*)val;
				// C#: for (i=0; i<blk2.rets.Count; i++)
				for (int i = 0; i < blk2->rets->size; i++) {
					List* int_list = create_list(thgc, sizeof(int), CType::_Struct);
					add_list_v(thgc, rets, &int_list);
					// C#: foreach (var nums in (blk2.rets[i].Clone().GetterB(local) as Block).rets)
					Obj* cloned = vt_Clone(thgc, *(Obj**)get_list(blk2->rets, i));
					Block* nums_blk = (Block*)GetterB_vt(thgc, cloned, local);
					for (int j = 0; j < nums_blk->rets->size; j++) {
						Obj* nums_j = *(Obj**)get_list(nums_blk->rets, j);
						// C#: if (nums.type == ObjType.Number) rets[i].Add((nums as Number).value);
						if (nums_j->type == LetterType::OT_Number) {
							int v = ((Number*)nums_j)->value;
							add_list_v(thgc, int_list, &v);
						}
						else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "範囲の選択はint[]でなければあなりません");
					}
				}
			}
			else return ErrorA(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, 0), "Sumの引数はint[]です。");
		}
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "Sumの引数は1です");
		// C#: rets[0].Count と rets[1].Count を揃える
		List* rets0 = *(List**)get_list(rets, 0);
		List* rets1 = *(List**)get_list(rets, 1);
		if (rets0->size < rets1->size) {
			for (int i = rets0->size; i < rets1->size; i++) {
				int zero = 0;
				add_list_v(thgc, rets0, &zero);
			}
		}
		else if (rets0->size > rets1->size) {
			for (int i = rets1->size; i < rets0->size; i++) {
				int zero = 0;
				add_list_v(thgc, rets1, &zero);
			}
		}
		// C#: minmax計算
		List* minmax0 = create_list(thgc, sizeof(int), CType::_Struct);
		List* minmax1 = create_list(thgc, sizeof(int), CType::_Struct);
		for (int i = 0; i < rets0->size; i++) {
			int r0 = *(int*)get_list(rets0, i);
			int r1 = *(int*)get_list(rets1, i);
			if (r0 <= r1) {
				add_list_v(thgc, minmax0, &r0);
				add_list_v(thgc, minmax1, &r1);
			} else {
				add_list_v(thgc, minmax0, &r1);
				add_list_v(thgc, minmax1, &r0);
			}
		}
		// C#: int[] array = new int[minmax.Count];
		int* array = (int*)alloca(sizeof(int) * minmax0->size);
		// C#: var n2 = 0;
		int n2 = 0;
		// C#: ObjInt c = new ObjInt();
		// C#: List<Cell> cells = new List<Cell>();
		// C#: SetArrayB((nums) => { ... }, array, minmax, 0, local);
		SetArrayB(thgc, self, array, minmax0, minmax1, 0, local, &n2);
		// C#: return new Number(n2) { cls = local.Int };
		Obj* result = make_number(thgc, NULL);
		((Number*)result)->value = n2;
		((Val*)result)->cls = (Obj*)local->Int;
		return result;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Sumの()がありません");
}

// ============================================================
// Question::exepB
// ============================================================
Obj* Question_exepB(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	// C#: var blk = new Block(ObjType.Array); var blk1 = new Block(ObjType.Array);
	Block* blk = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
	Block* blk1 = make_cblock(thgc, CType::_Block, LetterType::OT_Array);
	// C#: blk.rets.Add(blk1); blk.rets.Add(blk1);
	add_list(thgc, blk->rets, (char*)blk1);
	add_list(thgc, blk->rets, (char*)blk1);
	// C#: for (i=0; i<2; i++) blk1.rets.Add(new Number(0) { cls = local.Int });
	for (int i = 0; i < 2; i++) {
		Obj* num = make_number(thgc, NULL);
		((Number*)num)->value = 0;
		((Val*)num)->cls = (Obj*)local->Int;
		add_list(thgc, blk1->rets, (char*)num);
	}
	// C#: return blk1;
	return (Obj*)blk1;
}

// ============================================================
// B3_initVTables - B-pass vtable初期化 (Block 3)
// ============================================================
void B3_initVTables() {
	// --- Comment ---
	vt_Comment.exeB = Comment_exeB;
	vt_Comment.exepB = Comment_exepB;

	// --- Comment2 ---
	vt_Comment2.exeB = Comment2_exeB;
	vt_Comment2.exepB = Comment2_exepB;

	// --- Dolor ---
	vt_Dolor.exepB = Dolor_exepB;

	// --- HtmObj ---
	vt_HtmObj.exepB = HtmObj_exepB;

	// --- TagBlock ---
	vt_TagBlock.exepB = TagBlock_exepB;
	vt_TagBlock.exeB = TagBlock_exeB;

	// --- ElemType ---
	vt_ElemType.exepB = ElemType_exepB;
	vt_ElemType.PrimaryB = ElemType_PrimaryB;

	// --- ElemObj ---
	vt_ElemObj.opeB = ElemObj_opeB;

	// --- Signal ---
	vt_Signal.exepB = Signal_exepB;
	vt_Signal.PrimaryB = Signal_PrimaryB;

	// --- SignalFunction ---
	vt_SignalFunction.exeB = SignalFunction_exeB;
	vt_SignalFunction.PrimaryB = SignalFunction_PrimaryB;

	// --- ServerClient ---
	vt_ServerClient.exepB = ServerClient_exepB;
	vt_ServerClient.PrimaryB = ServerClient_PrimaryB;

	// --- ServerFunction ---
	vt_ServerFunction.exeB = ServerFunction_exeB;
	vt_ServerFunction.PrimaryB = ServerFunction_PrimaryB;

	// --- Connect ---
	vt_Connect.PrimaryB = Connect_PrimaryB;

	// --- ConnectStock ---
	vt_ConnectStock.PrimaryB = ConnectStock_PrimaryB;

	// --- Address ---
	vt_Address.PrimaryB = Address_PrimaryB;

	// --- LinearFunction ---
	vt_LinearFunction.PrimaryB = LinearFunction_PrimaryB;

	// --- Mountain ---
	vt_Mountain.exepB = Mountain_exepB;

	// --- Sum ---
	vt_Sum.PrimaryB = Sum_PrimaryB;

	// --- Question ---
	vt_Question.exepB = Question_exepB;
}
