// ============================================================
// cobj2c.h - C解析パス (LLVM IR code generation) - CObj2C.cs converted
// ============================================================

// ヘルパーマクロ
#define IS_WEG_C(obj) ((obj)->type == LetterType::OT_Wait || (obj)->type == LetterType::OT_Error || (obj)->type == LetterType::OT_NG)

// 前方宣言
Obj* Comment_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Comment_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* LinearFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Comment2_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Comment2_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Dolor_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* HtmObj_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* TagBlock_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* TagBlock_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* ElemType_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* ElemType_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ElemObj_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* Signal_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Signal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* SignalFunction_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* SignalFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* SignalFunction_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* ServerClient_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* ServerClient_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ServerFunction_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* ServerFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ServerFunction_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Connect_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ConnectStock_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Address_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Mountain_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Sum_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Question_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);

// ============================================================
// Comment::exeC
// ============================================================
Obj* Comment_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	List* blocks = get_blocks(local);
	Block* block = *(Block**)get_list(self->children, 0);

	// block.obj = new ObjBlock() { obj = this, n = -1 }
	ObjBlock* objblk = make_objblock_z(thgc, self, -1);
	block->obj = (Obj*)objblk;

	// block.comment = true
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

	// block.vmap["linear"] = new LinearFunction()
	Obj* linear = make_cobj(thgc, CType::_CObj, LetterType::OT_LinearFunction, NULL);
	linear->vt = &vt_LinearFunction;
	add_mapy(thgc, block->vmapA, createString(thgc, (char*)"linear", 6, 1), (char*)linear);

	add_list(thgc, blocks, (char*)block);

	// CommentLet handling (simplified)
	add_list(thgc, local->comments, (char*)self);

	vt_exeC(thgc, (Obj*)block, local);

	blocks->size--;
	local->comments->size--;
	return self;
}

// ============================================================
// Comment::exepC
// ============================================================
Obj* Comment_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	Block* blk = *(Block**)get_list(self->children, 0);
	add_list(thgc, blocks, (char*)blk);

	LVari* obj = make_lvari_c(thgc, "%GCObjectPtr", "%obj");
	LVari* rn = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* parent = make_lvari_c(thgc, "%ElementType*", "%parent");
	LVari* parentte = make_lvari_c(thgc, "%TreeElementType*", "%parentte");
	LVari* deletes = make_lvari_c(thgc, "%ListType*", "%deletes");

	int n2 = llvm_n++;
	char modelbuf[64];
	snprintf(modelbuf, sizeof(modelbuf), "%%com%d", n2);
	self->model = createString(thgc, modelbuf, (int)strlen(modelbuf), 1);

	char elbuf[64];
	snprintf(elbuf, sizeof(elbuf), "@el%d", n2);
	LVari* func_y = make_lvari(thgc, createString(thgc, (char*)"void", 4, 1), createString(thgc, elbuf, (int)strlen(elbuf), 1));
	LFunc* func = make_lfunc(thgc, local->llvm, func_y);
	add_list(thgc, func->draws, (char*)rn);
	add_list(thgc, func->draws, (char*)obj);
	add_list(thgc, func->draws, (char*)parent);
	add_list(thgc, func->draws, (char*)parentte);
	add_list(thgc, func->draws, (char*)deletes);
	add_list(thgc, local->llvm->comps, (char*)func);
	add_list(thgc, local->llvm->funcs, (char*)func);

	LVari* l_thgc = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, l_thgc, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));
	add_list(thgc, local->llvm->tagparents, (char*)parent);
	add_list(thgc, local->llvm->tagparenttes, (char*)parentte);

	// comelet handling (simplified)
	add_list(thgc, local->comments, (char*)self);

	LTypeDec* typedec = make_ltypedec(thgc, self->model, NULL, false);
	add_list(thgc, local->llvm->types, (char*)typedec);
	add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i8*", 3, 1), createString(thgc, (char*)"blk", 3, 1)));

	char checkbuf[64];
	snprintf(checkbuf, sizeof(checkbuf), "@elCheck%d", n2);
	LVari* thgc3 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* ac_val = make_lvari_c(thgc, "%GC_AddClassType", "%addclass");
	LVari* countv = make_lvari_c(thgc, "i32", "0");

	char namebuf[64];
	snprintf(namebuf, sizeof(namebuf), "comment%d", n2);
	int namelen = (int)strlen(namebuf);
	char atnamebuf[66];
	snprintf(atnamebuf, sizeof(atnamebuf), "@%s", namebuf);
	LStrV* strv = make_lstrv(thgc, createString(thgc, atnamebuf, (int)strlen(atnamebuf), 1), createString(thgc, namebuf, namelen, 1), namelen * 1);
	add_list(thgc, local->llvm->strs, (char*)strv);

	self->n_c = local->llvm->cn++;
	char typbuf[16];
	snprintf(typbuf, sizeof(typbuf), "%d", self->n_c);
	LVari* typ = make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, typbuf, (int)strlen(typbuf), 1));

	char tnpbuf[32];
	snprintf(tnpbuf, sizeof(tnpbuf), "%%tnp%d", llvm_n++);
	LVari* tnp = make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, tnpbuf, (int)strlen(tnpbuf), 1));
	add_list(thgc, local->llvm->main_func->comps, (char*)make_lload(thgc, tnp, make_lvari_c(thgc, "i32*", "@cnp"), false));

	char tvbuf[32];
	snprintf(tvbuf, sizeof(tvbuf), "%%cv%d", llvm_n++);
	LVari* tv = make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, tvbuf, (int)strlen(tvbuf), 1));
	add_list(thgc, local->llvm->main_func->comps, (char*)make_lbinop(thgc, LBOP_Add, tv, tnp, typ));

	LCall* ac_call = make_lcall(thgc, NULL, ac_val);
	add_list(thgc, ac_call->comps, (char*)thgc3);
	add_list(thgc, ac_call->comps, (char*)tv);
	add_list(thgc, ac_call->comps, (char*)countv);
	add_list(thgc, ac_call->comps, (char*)strv);
	add_list(thgc, ac_call->comps, (char*)make_lvari_c(thgc, "%GCCheckFuncType", checkbuf));
	add_list(thgc, ac_call->comps, (char*)make_lvari_c(thgc, "%GCFinalizeFuncType", "null"));
	add_list(thgc, local->llvm->main_func->comps, (char*)ac_call);

	LVari* thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* i8p = make_lvari_c(thgc, "i8*", "%self");
	LVari* funccheck_y = make_lvari(thgc, createString(thgc, (char*)"void", 4, 1), createString(thgc, checkbuf, (int)strlen(checkbuf), 1));
	LFunc* funccheck = make_lfunc(thgc, local->llvm, funccheck_y);
	add_list(thgc, funccheck->draws, (char*)thgc2);
	add_list(thgc, funccheck->draws, (char*)i8p);
	add_list(thgc, local->llvm->comps, (char*)funccheck);

	// funccheck: gete, load CopyObject, load, call, store for index 0
	{
		char vbuf[32];
		snprintf(vbuf, sizeof(vbuf), "%%v%d", llvm_n++);
		LVari* vari = make_lvari_c(thgc, "i8*", vbuf);
		LGete* gete = make_lgete(thgc, self->model, vari, i8p);
		add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
		add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
		add_list(thgc, funccheck->comps, (char*)gete);

		LVari* co_val2 = make_lvari_c(thgc, "%CopyObjectType", "%co");
		add_list(thgc, funccheck->comps, (char*)make_lload(thgc, co_val2, make_lvari_c(thgc, "%CopyObjectType*", "@CopyObject"), false));

		snprintf(vbuf, sizeof(vbuf), "%%v%d", llvm_n++);
		LVari* vari2 = make_lvari_c(thgc, "i8*", vbuf);
		add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));

		snprintf(vbuf, sizeof(vbuf), "%%v%d", llvm_n++);
		LVari* co_cval = make_lvari_c(thgc, "i8*", vbuf);
		LCall* co_call = make_lcall(thgc, co_cval, co_val2);
		add_list(thgc, co_call->comps, (char*)thgc2);
		add_list(thgc, co_call->comps, (char*)vari);
		add_list(thgc, funccheck->comps, (char*)co_call);
		add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
	}

	// funcdec
	char decbuf[64];
	snprintf(decbuf, sizeof(decbuf), "@eldec%d", n2);
	LVari* rn6 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* block_v = make_lvari_c(thgc, "i8*", "%block");
	char modstar[72];
	snprintf(modstar, sizeof(modstar), "%s*", modelbuf);
	LVari* funcdec_y = make_lvari(thgc, createString(thgc, modstar, (int)strlen(modstar), 1), createString(thgc, decbuf, (int)strlen(decbuf), 1));
	LFunc* funcdec = make_lfunc(thgc, local->llvm, funcdec_y);
	add_list(thgc, funcdec->draws, (char*)rn6);
	add_list(thgc, funcdec->draws, (char*)block_v);
	add_list(thgc, local->llvm->comps, (char*)funcdec);

	LVari* thgc6 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, thgc6, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));

	LVari* gmvari = make_lvari_c(thgc, "%GC_mallocType", "%gm");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, gmvari, make_lvari_c(thgc, "%GC_mallocType*", "@GC_malloc"), false));

	{
		char vbuf[32];
		snprintf(vbuf, sizeof(vbuf), "%%obj%d", llvm_n++);
		LVari* go_v = make_lvari_c(thgc, "%GCObjectPtr", vbuf);

		char tmpbuf[16];
		snprintf(tmpbuf, sizeof(tmpbuf), "%d", self->n_c);
		LVari* tmp = make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, tmpbuf, (int)strlen(tmpbuf), 1));

		char tmibuf[32];
		snprintf(tmibuf, sizeof(tmibuf), "%%tmi%d", llvm_n++);
		LVari* tmi = make_lvari_c(thgc, "i32", tmibuf);
		add_list(thgc, funcdec->comps, (char*)make_lload(thgc, tmi, make_lvari_c(thgc, "i32*", "@cnp"), false));

		char tmvbuf[32];
		snprintf(tmvbuf, sizeof(tmvbuf), "%%tmv%d", llvm_n++);
		LVari* tmv = make_lvari_c(thgc, "i32", tmvbuf);
		add_list(thgc, funcdec->comps, (char*)make_lbinop(thgc, LBOP_Add, tmv, tmi, tmp));

		LCall* go_call = make_lcall(thgc, go_v, gmvari);
		add_list(thgc, go_call->comps, (char*)thgc6);
		add_list(thgc, go_call->comps, (char*)tmv);
		add_list(thgc, funcdec->comps, (char*)go_call);
		add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, make_lvari_c(thgc, "ptr", "@client"), go_v));

		snprintf(vbuf, sizeof(vbuf), "%%v%d", llvm_n++);
		LVari* go_c2 = make_lvari(thgc, createString(thgc, modstar, (int)strlen(modstar), 1), createString(thgc, vbuf, (int)strlen(vbuf), 1));
		add_list(thgc, funcdec->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c2, go_v));

		snprintf(vbuf, sizeof(vbuf), "%%v%d", llvm_n++);
		LVari* vc = make_lvari_c(thgc, "i8*", vbuf);
		LGete* gete = make_lgete(thgc, self->model, vc, go_c2);
		add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
		add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
		add_list(thgc, funcdec->comps, (char*)gete);
		add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc, block_v));

		char opbuf[32];
		snprintf(opbuf, sizeof(opbuf), "%%v%d", llvm_n++);
		char opsbuf[72];
		snprintf(opsbuf, sizeof(opsbuf), "%s**", modelbuf);
		LVari* objptr3 = make_lvari(thgc, createString(thgc, opsbuf, (int)strlen(opsbuf), 1), createString(thgc, opbuf, (int)strlen(opbuf), 1));
		add_list(thgc, funcdec->comps, (char*)make_lalloca(thgc, objptr3));
		add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, objptr3, go_c2));

		int count = 8;
		int order = 1;

		List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);

		// iterate blk.vmapA
		for (int _i = 0; _i < blk->vmapA->kvs->size; _i++) {
			KV* kv = *(KV**)get_list(blk->vmapA->kvs, _i);
			Obj* v = (Obj*)kv->value;
			if (v->type == LetterType::OT_Variable) {
				v->order = order++;
				// variable.cls handling simplified - set typedec based on type
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i8*", 3, 1), kv->key));
				count += 8;
				// funccheck GC copy for non-primitive types
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2_ = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2_, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval_ = make_lvari_c(thgc, "i8*", vb);
					LVari* co_val3 = make_lvari_c(thgc, "%CopyObjectType", "%co");
					LCall* co_call_ = make_lcall(thgc, co_cval_, co_val3);
					add_list(thgc, co_call_->comps, (char*)thgc2);
					add_list(thgc, co_call_->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call_);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval_));
				}
			} else if (v->type == LetterType::OT_Function) {
				SetIdentity(thgc, (Obj*)(v));
				v->order = order++;
				add_list(thgc, decs, (char*)v);
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));

				// funccheck copy
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);
					// duplicate gete for Function (matches C# code)
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2_ = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2_, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval_ = make_lvari_c(thgc, "i8*", vb);
					LVari* co_val3 = make_lvari_c(thgc, "%CopyObjectType", "%co");
					LCall* co_call_ = make_lcall(thgc, co_cval_, co_val3);
					add_list(thgc, co_call_->comps, (char*)thgc2);
					add_list(thgc, co_call_->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call_);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval_));
				}
				count += 8;

				// funcdec: call dec, load, gete, store
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va0 = make_lvari_c(thgc, "%FuncType*", vb);
					// build call name "@{name}dec{identity}"
					// (simplified - uses letter name + dec + identity)
					char fcbuf[128];
					// TODO: proper letter name extraction
					LCall* fu_call = make_lcall(thgc, va0, make_lvari_c(thgc, "%FuncType*", fcbuf));
					add_list(thgc, fu_call->comps, (char*)block_v);
					add_list(thgc, funcdec->comps, (char*)fu_call);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va2 = make_lvari(thgc, createString(thgc, modstar, (int)strlen(modstar), 1), createString(thgc, vb, (int)strlen(vb), 1));
					add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va = make_lvari_c(thgc, "%FuncType*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, va, va2);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funcdec->comps, (char*)ge);
					add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
				}
			} else if (v->type == LetterType::OT_ClassObj) {
				SetIdentity(thgc, (Obj*)(v));
				v->order = order++;
				add_list(thgc, decs, (char*)v);
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));

				// funccheck copy (same pattern as Function)
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2_ = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2_, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval_ = make_lvari_c(thgc, "i8*", vb);
					LVari* co_val3 = make_lvari_c(thgc, "%CopyObjectType", "%co");
					LCall* co_call_ = make_lcall(thgc, co_cval_, co_val3);
					add_list(thgc, co_call_->comps, (char*)thgc2);
					add_list(thgc, co_call_->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call_);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval_));
				}
				count += 8;

				// funcdec: same pattern
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va0 = make_lvari_c(thgc, "%FuncType*", vb);
					LCall* fu_call = make_lcall(thgc, va0, make_lvari_c(thgc, "%FuncType*", "@classdec"));
					add_list(thgc, fu_call->comps, (char*)block_v);
					add_list(thgc, funcdec->comps, (char*)fu_call);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va2 = make_lvari(thgc, createString(thgc, modstar, (int)strlen(modstar), 1), createString(thgc, vb, (int)strlen(vb), 1));
					add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va = make_lvari_c(thgc, "%FuncType*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, va, va2);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funcdec->comps, (char*)ge);
					add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
				}
			}
		}

		// Store into local
		local->blok = block_v;
		local->typedecC = typedec;
		local->funccheck = funccheck;
		local->funcdec = funcdec;
		local->i8p = i8p;
		local->objptr3 = objptr3;
		local->co_val = make_lvari_c(thgc, "%CopyObjectType", "%co");
		local->countv = countv;
		local->odr = order;
		local->countC = count;

		// decs[i].Dec(local)
		for (int _d = 0; _d < decs->size; _d++) {
			Obj* dec = *(Obj**)get_list(decs, _d);
			// Dec is a virtual dispatch - would need Dec_vt if available
			// For now, this matches the C# pattern
		}

		add_list(thgc, funcdec->comps, (char*)make_lret(thgc, go_c2));
		add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

		// func body
		char vvbuf[32];
		snprintf(vvbuf, sizeof(vvbuf), "%%vv%d", llvm_n++);
		LVari* vv = make_lvari(thgc, createString(thgc, modstar, (int)strlen(modstar), 1), createString(thgc, vvbuf, (int)strlen(vvbuf), 1));
		LCall* vv_call = make_lcall(thgc, vv, funcdec_y);
		add_list(thgc, vv_call->comps, (char*)rn);
		add_list(thgc, vv_call->comps, (char*)obj);
		add_list(thgc, func->comps, (char*)vv_call);

		char opbuf2[32];
		snprintf(opbuf2, sizeof(opbuf2), "%%objptr%d", llvm_n++);
		LVari* objptr = make_lvari(thgc, createString(thgc, opsbuf, (int)strlen(opsbuf), 1), createString(thgc, opbuf2, (int)strlen(opbuf2), 1));
		add_list(thgc, func->comps, (char*)make_lalloca(thgc, objptr));
		add_list(thgc, func->comps, (char*)make_lstore(thgc, objptr, vv));

		((ObjBlock*)((Block*)blk)->obj)->obj->bas = objptr;
		vt_exeC(thgc, (Obj*)blk, local);

		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));
		local->llvm->funcs->size--;
		local->llvm->tagparents->size--;
		local->llvm->tagparenttes->size--;
		blocks->size--;
		local->comments->size--;
		self->vari_c = func_y;
	}
	return self;
}

// ============================================================
// Comment2::exeC
// ============================================================
Obj* Comment2_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// Comment2::exepC
// ============================================================
Obj* Comment2_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Clones::RightRightC (not a vtable method, helper)
// ============================================================
Obj* Clones_RightRightC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	return self;
}

// ============================================================
// Dolor::exepC
// ============================================================
Obj* Dolor_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		String* wordname = val2->letter->text;
		if (str_matchA(wordname, "type", 4)) {
			Clones* clones = Clones_make(thgc, self->letter);
			return (Obj*)clones;
		} else if (str_matchA(wordname, "func", 4)) {
			// break (no-op)
		}
	} else if (val2->type == LetterType::OT_Bracket) {
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		local->compsC->size--;
		if (IS_WEG_C(val)) return val;
		Block* blk = (Block*)val;
		if (blk->rets->size != 1) {
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$()は引数を1つしかとれません");
		}
		Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
		if (ret0->type == LetterType::OT_Number || ret0->type == LetterType::OT_StrObj) {
			if (local->comments->size > 0) {
				LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
				// tagvars.Last() length check
				String* lasttagvar = *(String**)get_list(local->llvm->tagvars, local->llvm->tagvars->size - 1);
				if (lasttagvar != NULL && lasttagvar->size > 0) {
					char strbuf[32];
					snprintf(strbuf, sizeof(strbuf), "%%str%d", llvm_n++);
					LVari* str = make_lvari_c(thgc, "%StringType*", strbuf);
					int n2val = 1;
					char vbuf[32];
					snprintf(vbuf, sizeof(vbuf), "%%v%d", llvm_n++);
					LVari* v2 = make_lvari_c(thgc, "i8*", vbuf);
					char svbuf[32];
					snprintf(svbuf, sizeof(svbuf), "@s%d", llvm_n++);
					LStrV* sv = make_lstrv(thgc, createString(thgc, svbuf, (int)strlen(svbuf), 1), lasttagvar, lasttagvar->size * n2val);
					add_list(thgc, local->llvm->strs, (char*)sv);
					char getebuf[64];
					snprintf(getebuf, sizeof(getebuf), "[%d x i8]", lasttagvar->size * n2val + 1);
					LGete* gete = make_lgete(thgc, createString(thgc, getebuf, (int)strlen(getebuf), 1), v2, (LVari*)sv);
					add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)gete);

					char fvbuf[32];
					snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
					LVari* fv = make_lvari_c(thgc, "%CreateStringType*", fvbuf);
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv, make_lvari_c(thgc, "%CreateStringType*", "@CreateString"), false));
					char lenbuf[16];
					snprintf(lenbuf, sizeof(lenbuf), "%d", lasttagvar->size);
					char n2buf[16];
					snprintf(n2buf, sizeof(n2buf), "%d", n2val);
					LCall* cscall = make_lcall(thgc, str, fv);
					add_list(thgc, cscall->comps, (char*)thgc4);
					add_list(thgc, cscall->comps, (char*)v2);
					add_list(thgc, cscall->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, lenbuf, (int)strlen(lenbuf), 1)));
					add_list(thgc, cscall->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, n2buf, (int)strlen(n2buf), 1)));
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)cscall);

					LVari* lasttagvarvar = *(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1);
					if (lasttagvarvar == NULL) {
						// tagvarvars[last] = str
						*(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1) = str;
						return NULL;
					} else {
						char fv2buf[32];
						snprintf(fv2buf, sizeof(fv2buf), "%%fv%d", llvm_n++);
						LVari* fv2 = make_lvari_c(thgc, "%AddStringType2", fv2buf);
						add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv2, make_lvari_c(thgc, "%AddStringType2*", "@AddString2"), false));
						char str2buf[32];
						snprintf(str2buf, sizeof(str2buf), "%%str%d", llvm_n++);
						LVari* str2 = make_lvari_c(thgc, "%StringType*", str2buf);
						LCall* as_call = make_lcall(thgc, str2, fv2);
						add_list(thgc, as_call->comps, (char*)thgc4);
						add_list(thgc, as_call->comps, (char*)lasttagvarvar);
						add_list(thgc, as_call->comps, (char*)str);
						add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)as_call);
						*(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1) = str2;
					}
					// tagvars[last] = ""
					*(String**)get_list(local->llvm->tagvars, local->llvm->tagvars->size - 1) = createString(thgc, (char*)"", 0, 1);
				}
				{
					char fvbuf[32];
					snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
					LVari* fv = make_lvari_c(thgc, "%CreateLetterType", fvbuf);
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv, make_lvari_c(thgc, "%CreateLetterType*", "@CreateLetterType"), false));
					char strbuf[32];
					snprintf(strbuf, sizeof(strbuf), "%%str%d", llvm_n++);
					LVari* str = make_lvari_c(thgc, "%StringType*", strbuf);
					if (ret0->type == LetterType::OT_Number) {
						char fv2buf[32];
						snprintf(fv2buf, sizeof(fv2buf), "%%fv%d", llvm_n++);
						LVari* fv2 = make_lvari_c(thgc, "%NumberStringType", fv2buf);
						add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv2, make_lvari_c(thgc, "%NumberStringType*", "@NumberString"), false));
						LCall* ns_call = make_lcall(thgc, str, fv2);
						add_list(thgc, ns_call->comps, (char*)thgc4);
						add_list(thgc, ns_call->comps, (char*)ret0->vari_c);
						add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ns_call);
					} else {
						str = ret0->vari_c;
					}
					LVari* lasttagvarvar = *(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1);
					if (lasttagvarvar == NULL) {
						*(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1) = str;
						return NULL;
					} else {
						char fv2buf[32];
						snprintf(fv2buf, sizeof(fv2buf), "%%fv%d", llvm_n++);
						LVari* fv2 = make_lvari_c(thgc, "%AddStringType2", fv2buf);
						add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv2, make_lvari_c(thgc, "%AddStringType2*", "@AddString2"), false));
						char str2buf[32];
						snprintf(str2buf, sizeof(str2buf), "%%str%d", llvm_n++);
						LVari* str2 = make_lvari_c(thgc, "%StringType*", str2buf);
						LCall* as_call = make_lcall(thgc, str2, fv2);
						add_list(thgc, as_call->comps, (char*)thgc4);
						add_list(thgc, as_call->comps, (char*)lasttagvarvar);
						add_list(thgc, as_call->comps, (char*)str);
						add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)as_call);
						*(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1) = str2;
					}
				}
			} else {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$()は構造コメントブロックの中でしか使えません");
			}
		} else {
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$()は整数、文字列を出力します");
		}
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "$の後ろには()かtype,funcが来ます");
}

// ============================================================
// HtmObj::exepC
// ============================================================
Obj* HtmObj_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (local->comments->size > 0) {
		*(String**)get_list(local->llvm->tagvars, local->llvm->tagvars->size - 1) = self->letter->text;
		return NULL;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "コメント内で宣言してください");
}

// ============================================================
// TagBlock::exeC
// ============================================================
Obj* TagBlock_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	List* blocks = get_blocks(local);
	if (local->comments->size > 0) {
		Block* blk1 = (Block*)GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(self->children, 0), local), local);
		blk1->comment = true;
		ObjBlock* objblk1 = make_objblock_z(thgc, self, 0);
		blk1->obj = (Obj*)objblk1;
		pushBlock(thgc, local, blk1);
		vt_exeC(thgc, (Obj*)blk1, local);

		Block* blk2 = (Block*)GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(self->children, 1), local), local);
		blk2->comment = true;
		ObjBlock* objblk2 = make_objblock_z(thgc, self, 1);
		blk2->obj = (Obj*)objblk2;
		pushBlock(thgc, local, blk2);
		vt_exeC(thgc, (Obj*)blk2, local);

		if (blocks->size > 0) blocks->size--;
		if (blocks->size > 0) blocks->size--;
	}
	return NULL;
}

// ============================================================
// TagBlock::exepC
// ============================================================
Obj* TagBlock_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	if (local->comments->size > 0) {
		Block* blk1 = *(Block**)get_list(self->children, 0);
		blk1->comment = true;
		ObjBlock* objblk1 = make_objblock_z(thgc, self, 0);
		blk1->obj = (Obj*)objblk1;
		int order = local->odr;
		add_list(thgc, blocks, (char*)blk1);
		Block* blk2 = *(Block**)get_list(self->children, 1);
		blk2->comment = true;
		ObjBlock* objblk2 = make_objblock_z(thgc, self, 1);
		blk2->obj = (Obj*)objblk2;
		List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);
		LVari* thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
		LVari* block = local->blok;
		LTypeDec* typedec = local->typedecC;
		int count = local->countC;
		LFunc* funccheck = local->funccheck;
		LFunc* funcdec = local->funcdec;
		LVari* i8p = local->i8p;
		LVari* objptr3 = local->objptr3;
		LVari* co_val = local->co_val;

		// iterate blk1.vmapA
		for (int _i = 0; _i < blk1->vmapA->kvs->size; _i++) {
			KV* kv = *(KV**)get_list(blk1->vmapA->kvs, _i);
			Obj* v = (Obj*)kv->value;
			if (v->type == LetterType::OT_Variable) {
				v->order = order++;
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i8*", 3, 1), kv->key));
				count += 8;
				// funccheck copy
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2 = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval = make_lvari_c(thgc, "i8*", vb);
					LCall* co_call = make_lcall(thgc, co_cval, co_val);
					add_list(thgc, co_call->comps, (char*)thgc2);
					add_list(thgc, co_call->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
				}
			} else if (v->type == LetterType::OT_Function) {
				SetIdentity(thgc, (Obj*)(v));
				v->order = order++;
				add_list(thgc, decs, (char*)v);
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));
				// funccheck
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2 = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval = make_lvari_c(thgc, "i8*", vb);
					LCall* co_call = make_lcall(thgc, co_cval, co_val);
					add_list(thgc, co_call->comps, (char*)thgc2);
					add_list(thgc, co_call->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
				}
				count += 8;
				// funcdec pattern
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va0 = make_lvari_c(thgc, "%FuncType*", vb);
					LCall* fu_call = make_lcall(thgc, va0, make_lvari_c(thgc, "%FuncType*", "@funcdec"));
					add_list(thgc, fu_call->comps, (char*)block);
					add_list(thgc, funcdec->comps, (char*)fu_call);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					String* modstar = self->model;
					char msbuf[72];
					snprintf(msbuf, sizeof(msbuf), "%.*s*", modstar->size, modstar->data);
					LVari* va2 = make_lvari(thgc, createString(thgc, msbuf, (int)strlen(msbuf), 1), createString(thgc, vb, (int)strlen(vb), 1));
					add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va = make_lvari_c(thgc, "%FuncType*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, va, va2);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funcdec->comps, (char*)ge);
					add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
				}
			} else if (v->type == LetterType::OT_ClassObj) {
				SetIdentity(thgc, (Obj*)(v));
				v->order = order++;
				add_list(thgc, decs, (char*)v);
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));
				// funccheck + funcdec (same pattern)
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2 = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval = make_lvari_c(thgc, "i8*", vb);
					LCall* co_call = make_lcall(thgc, co_cval, co_val);
					add_list(thgc, co_call->comps, (char*)thgc2);
					add_list(thgc, co_call->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
				}
				count += 8;
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va0 = make_lvari_c(thgc, "%FuncType*", vb);
					LCall* fu_call = make_lcall(thgc, va0, make_lvari_c(thgc, "%FuncType*", "@classdec"));
					add_list(thgc, fu_call->comps, (char*)block);
					add_list(thgc, funcdec->comps, (char*)fu_call);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					String* modstar = self->model;
					char msbuf[72];
					snprintf(msbuf, sizeof(msbuf), "%.*s*", modstar->size, modstar->data);
					LVari* va2 = make_lvari(thgc, createString(thgc, msbuf, (int)strlen(msbuf), 1), createString(thgc, vb, (int)strlen(vb), 1));
					add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va = make_lvari_c(thgc, "%FuncType*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, va, va2);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funcdec->comps, (char*)ge);
					add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
				}
			}
		}

		// iterate blk2.vmapA (same patterns for second block)
		for (int _i = 0; _i < blk2->vmapA->kvs->size; _i++) {
			KV* kv = *(KV**)get_list(blk2->vmapA->kvs, _i);
			Obj* v = (Obj*)kv->value;
			if (v->type == LetterType::OT_Variable) {
				v->order = order++;
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i8*", 3, 1), kv->key));
				count += 8;
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2 = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval = make_lvari_c(thgc, "i8*", vb);
					LCall* co_call = make_lcall(thgc, co_cval, co_val);
					add_list(thgc, co_call->comps, (char*)thgc2);
					add_list(thgc, co_call->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
				}
			} else if (v->type == LetterType::OT_Function) {
				SetIdentity(thgc, (Obj*)(v));
				v->order = order++;
				add_list(thgc, decs, (char*)v);
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2 = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval = make_lvari_c(thgc, "i8*", vb);
					LCall* co_call = make_lcall(thgc, co_cval, co_val);
					add_list(thgc, co_call->comps, (char*)thgc2);
					add_list(thgc, co_call->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
				}
				count += 8;
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va0 = make_lvari_c(thgc, "%FuncType*", vb);
					LCall* fu_call = make_lcall(thgc, va0, make_lvari_c(thgc, "%FuncType*", "@funcdec"));
					add_list(thgc, fu_call->comps, (char*)block);
					add_list(thgc, funcdec->comps, (char*)fu_call);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					String* modstr = self->model;
					char msbuf[72];
					snprintf(msbuf, sizeof(msbuf), "%.*s*", modstr->size, modstr->data);
					LVari* va2 = make_lvari(thgc, createString(thgc, msbuf, (int)strlen(msbuf), 1), createString(thgc, vb, (int)strlen(vb), 1));
					add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va = make_lvari_c(thgc, "%FuncType*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, va, va2);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funcdec->comps, (char*)ge);
					add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
				}
			} else if (v->type == LetterType::OT_ClassObj) {
				SetIdentity(thgc, (Obj*)(v));
				v->order = order++;
				add_list(thgc, decs, (char*)v);
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari = make_lvari_c(thgc, "i8*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, vari, i8p);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funccheck->comps, (char*)ge);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* vari2 = make_lvari_c(thgc, "i8*", vb);
					add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* co_cval = make_lvari_c(thgc, "i8*", vb);
					LCall* co_call = make_lcall(thgc, co_cval, co_val);
					add_list(thgc, co_call->comps, (char*)thgc2);
					add_list(thgc, co_call->comps, (char*)vari);
					add_list(thgc, funccheck->comps, (char*)co_call);
					add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
				}
				count += 8;
				{
					char vb[32];
					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va0 = make_lvari_c(thgc, "%FuncType*", vb);
					LCall* fu_call = make_lcall(thgc, va0, make_lvari_c(thgc, "%FuncType*", "@classdec"));
					add_list(thgc, fu_call->comps, (char*)block);
					add_list(thgc, funcdec->comps, (char*)fu_call);

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					String* modstr = self->model;
					char msbuf[72];
					snprintf(msbuf, sizeof(msbuf), "%.*s*", modstr->size, modstr->data);
					LVari* va2 = make_lvari(thgc, createString(thgc, msbuf, (int)strlen(msbuf), 1), createString(thgc, vb, (int)strlen(vb), 1));
					add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));

					snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
					LVari* va = make_lvari_c(thgc, "%FuncType*", vb);
					char ordbuf[16];
					snprintf(ordbuf, sizeof(ordbuf), "%d", v->order);
					LGete* ge = make_lgete(thgc, self->model, va, va2);
					add_list(thgc, ge->comps, (char*)make_lvari_c(thgc, "i32", "0"));
					add_list(thgc, ge->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, ordbuf, (int)strlen(ordbuf), 1)));
					add_list(thgc, funcdec->comps, (char*)ge);
					add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
				}
			}
		}

		local->odr = order;
		local->countC = count;
		// countv.name = count (update the LVari name)
		{
			char cbuf[16];
			snprintf(cbuf, sizeof(cbuf), "%d", count);
			local->countv->name = createString(thgc, cbuf, (int)strlen(cbuf), 1);
		}

		// decs[i].Dec(local) - virtual dispatch
		for (int _d = 0; _d < decs->size; _d++) {
			// Dec dispatch placeholder
		}

		Obj* obj_result = GetterC_vt(thgc, vt_exeC(thgc, (Obj*)blk1, local), local);
		Block* obj_blk = (Block*)obj_result;

		LVari* l_thgc = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
		if (local->llvm->tagvars->size > 0) {
			char fvbuf[32];
			snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
			LVari* fv = make_lvari_c(thgc, "%CreateLetterTpe*", fvbuf);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv, make_lvari_c(thgc, "%CreateLetterTpe*", "@CreateLetter"), false));
			LVari* lastparent = *(LVari**)get_list(local->llvm->tagparents, local->llvm->tagparents->size - 1);
			LVari* lasttagvarvar = *(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1);
			LCall* cl_call = make_lcall(thgc, NULL, fv);
			add_list(thgc, cl_call->comps, (char*)lastparent);
			add_list(thgc, cl_call->comps, (char*)lasttagvarvar);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)cl_call);
		}

		bool id = false;
		LVari* ev = NULL;
		Obj* ret0 = *(Obj**)get_list(obj_blk->rets, 0);

		// Element creation based on type
		switch (ret0->type) {
		case LetterType::OT_Div: {
			char fvbuf[32];
			snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
			LVari* fv = make_lvari_c(thgc, "%CreateDivType*", fvbuf);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv, make_lvari_c(thgc, "%CreateDivType*", "@CreateDiv"), false));
			char evbuf[32];
			snprintf(evbuf, sizeof(evbuf), "%%ev%d", llvm_n++);
			ev = make_lvari_c(thgc, "%ElementType*", evbuf);
			LCall* ev_call = make_lcall(thgc, ev, fv);
			add_list(thgc, ev_call->comps, (char*)l_thgc);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ev_call);
			snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
			LVari* fv4 = make_lvari_c(thgc, "%ElementAddType*", fvbuf);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv4, make_lvari_c(thgc, "%ElementAddType*", "@ElementAdd"), false));
			LCall* ea_call = make_lcall(thgc, NULL, fv4);
			add_list(thgc, ea_call->comps, (char*)l_thgc);
			add_list(thgc, ea_call->comps, (char*)(local->llvm->tagparents->size == 0 ? NULL : *(LVari**)get_list(local->llvm->tagparents, local->llvm->tagparents->size - 1)));
			add_list(thgc, ea_call->comps, (char*)ev);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ea_call);
			add_list(thgc, local->llvm->tagparents, (char*)ev);
		} break;
		case LetterType::OT_Sheet: {
			char fvbuf[32];
			snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
			LVari* fv = make_lvari_c(thgc, "%CreateDivType*", fvbuf);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv, make_lvari_c(thgc, "%CreateDivType*", "@CreateSheet"), false));
			char evbuf[32];
			snprintf(evbuf, sizeof(evbuf), "%%ev%d", llvm_n++);
			ev = make_lvari_c(thgc, "%ElementType*", evbuf);
			LCall* ev_call = make_lcall(thgc, ev, fv);
			add_list(thgc, ev_call->comps, (char*)l_thgc);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ev_call);
			snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
			LVari* fv4 = make_lvari_c(thgc, "%ElementAddType*", fvbuf);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv4, make_lvari_c(thgc, "%ElementAddType*", "@ElementAdd"), false));
			LCall* ea_call = make_lcall(thgc, NULL, fv4);
			add_list(thgc, ea_call->comps, (char*)l_thgc);
			add_list(thgc, ea_call->comps, (char*)(local->llvm->tagparents->size == 0 ? NULL : *(LVari**)get_list(local->llvm->tagparents, local->llvm->tagparents->size - 1)));
			add_list(thgc, ea_call->comps, (char*)ev);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ea_call);
			add_list(thgc, local->llvm->tagparents, (char*)ev);
		} break;
		case LetterType::OT_Br: {
			char fvbuf[32];
			snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
			LVari* fv2 = make_lvari_c(thgc, "%CreateKaigyouType*", fvbuf);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv2, make_lvari_c(thgc, "%CreateKaigyouType*", "@CreateKaigyou"), false));
			char evbuf[32];
			snprintf(evbuf, sizeof(evbuf), "%%ev%d", llvm_n++);
			LVari* ev2 = make_lvari_c(thgc, "%ElementType*", evbuf);
			LCall* br_call = make_lcall(thgc, ev2, fv2);
			add_list(thgc, br_call->comps, (char*)(local->llvm->tagparents->size == 0 ? make_lvari_c(thgc, "%ElementType*", "null") : *(LVari**)get_list(local->llvm->tagparents, local->llvm->tagparents->size - 1)));
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)br_call);
			return self;
		}
		default:
			break;
		}

		// Element properties (left, top, gomousedown, etc.)
		for (int i = 1; i < (int)blk1->rets->size; i++) {
			Obj* val = *(Obj**)get_list(blk1->rets, i);
			// labelmapn check
			char ibuf[16];
			snprintf(ibuf, sizeof(ibuf), "%d", i);
			// Property setting via label names - simplified
			// (Full label handling would check labelmapn for each property)
		}

		// tagvars push
		add_list(thgc, local->llvm->tagvars, (char*)createString(thgc, (char*)"", 0, 1));
		add_list(thgc, local->llvm->tagvarvars, (char*)NULL);
		add_list(thgc, blocks, (char*)blk2);
		vt_exeC(thgc, *(Obj**)get_list(self->children, 1), local);

		LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
		if (ev != NULL) {
			// Check if tagvars.Last() has content
			String* lasttagvar = *(String**)get_list(local->llvm->tagvars, local->llvm->tagvars->size - 1);
			if (lasttagvar != NULL && lasttagvar->size > 0) {
				char strbuf[32];
				snprintf(strbuf, sizeof(strbuf), "%%str%d", llvm_n++);
				LVari* str = make_lvari_c(thgc, "%StringType*", strbuf);
				int n2val = 1;
				char vb[32];
				snprintf(vb, sizeof(vb), "%%v%d", llvm_n++);
				LVari* v2 = make_lvari_c(thgc, "i8*", vb);
				char svbuf[32];
				snprintf(svbuf, sizeof(svbuf), "@s%d", llvm_n++);
				LStrV* sv = make_lstrv(thgc, createString(thgc, svbuf, (int)strlen(svbuf), 1), lasttagvar, lasttagvar->size * n2val);
				add_list(thgc, local->llvm->strs, (char*)sv);
				char getebuf[64];
				snprintf(getebuf, sizeof(getebuf), "[%d x i8]", lasttagvar->size * n2val + 1);
				LGete* gete = make_lgete(thgc, createString(thgc, getebuf, (int)strlen(getebuf), 1), v2, (LVari*)sv);
				add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
				add_list(thgc, gete->comps, (char*)make_lvari_c(thgc, "i32", "0"));
				add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)gete);

				char fvbuf[32];
				snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
				LVari* fv = make_lvari_c(thgc, "%CreateStringType*", fvbuf);
				add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv, make_lvari_c(thgc, "%CreateStringType*", "@CreateString"), false));
				char lenbuf[16];
				snprintf(lenbuf, sizeof(lenbuf), "%d", lasttagvar->size);
				char n2buf[16];
				snprintf(n2buf, sizeof(n2buf), "%d", n2val);
				LCall* cscall = make_lcall(thgc, str, fv);
				add_list(thgc, cscall->comps, (char*)thgc4);
				add_list(thgc, cscall->comps, (char*)v2);
				add_list(thgc, cscall->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, lenbuf, (int)strlen(lenbuf), 1)));
				add_list(thgc, cscall->comps, (char*)make_lvari(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, n2buf, (int)strlen(n2buf), 1)));
				add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)cscall);

				LVari* lasttagvarvar = *(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1);
				if (lasttagvarvar == NULL) {
					*(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1) = str;
				} else {
					char fv2buf[32];
					snprintf(fv2buf, sizeof(fv2buf), "%%fv%d", llvm_n++);
					LVari* fv2 = make_lvari_c(thgc, "%AddStringType2", fv2buf);
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv2, make_lvari_c(thgc, "%AddStringType2*", "@AddString2"), false));
					char str2buf[32];
					snprintf(str2buf, sizeof(str2buf), "%%str%d", llvm_n++);
					LVari* str2 = make_lvari_c(thgc, "%StringType*", str2buf);
					LCall* as_call = make_lcall(thgc, str2, fv2);
					add_list(thgc, as_call->comps, (char*)thgc4);
					add_list(thgc, as_call->comps, (char*)lasttagvarvar);
					add_list(thgc, as_call->comps, (char*)str);
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)as_call);
					*(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1) = str2;
				}
				*(String**)get_list(local->llvm->tagvars, local->llvm->tagvars->size - 1) = createString(thgc, (char*)"", 0, 1);
			}
			LVari* lasttagvarvar2 = *(LVari**)get_list(local->llvm->tagvarvars, local->llvm->tagvarvars->size - 1);
			if (lasttagvarvar2 != NULL) {
				char fvbuf[32];
				snprintf(fvbuf, sizeof(fvbuf), "%%fv%d", llvm_n++);
				LVari* fv4 = make_lvari_c(thgc, "%CreateLetterType", fvbuf);
				add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv4, make_lvari_c(thgc, "%CreateLetterType*", "@CreateLetter"), false));
				LCall* cl_call = make_lcall(thgc, NULL, fv4);
				add_list(thgc, cl_call->comps, (char*)thgc4);
				add_list(thgc, cl_call->comps, (char*)*(LVari**)get_list(local->llvm->tagparents, local->llvm->tagparents->size - 1));
				add_list(thgc, cl_call->comps, (char*)lasttagvarvar2);
				add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)cl_call);
			}
			// CreateEnd
			char fv3buf[32];
			snprintf(fv3buf, sizeof(fv3buf), "%%fv%d", llvm_n++);
			LVari* fv3 = make_lvari_c(thgc, "%CreateEndType", fv3buf);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv3, make_lvari_c(thgc, "%CreateEndType*", "@CreateEnd"), false));
			char ev3buf[32];
			snprintf(ev3buf, sizeof(ev3buf), "%%ev%d", llvm_n++);
			LVari* ev3 = make_lvari_c(thgc, "%ElementType*", ev3buf);
			LCall* end_call = make_lcall(thgc, ev3, fv3);
			add_list(thgc, end_call->comps, (char*)thgc4);
			add_list(thgc, end_call->comps, (char*)*(LVari**)get_list(local->llvm->tagparents, local->llvm->tagparents->size - 1));
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)end_call);
		}

		blocks->size--;
		blocks->size--;
		local->llvm->tagvars->size--;
		local->llvm->tagvarvars->size--;
		local->llvm->tagparents->size--;
		if (id) local->llvm->tagparenttes->size--;
	}
	return self;
}

// ============================================================
// ElemType::exepC
// ============================================================
Obj* ElemType_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Obj* val2 = *(Obj**)get_list(primary->children, *n + 1);
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		Obj* elemobj = make_elemobj(thgc, self, val2->letter);
		return elemobj;
	}
	return self;
}

// ============================================================
// ElemType::PrimaryC
// ============================================================
Obj* ElemType_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		Obj* elemobj = make_elemobj(thgc, self, val2->letter);
		// declareB
		declareB(thgc, local, val2->letter->text, elemobj);
		return elemobj;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "elem変数の名前が宣言されていません");
}

// ============================================================
// ElemObj::opeC
// ============================================================
Obj* ElemObj_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	bool is_plus = (key == LetterType::_Plus);
	bool is_minus = (key == LetterType::_Minus);
	bool is_mul = (key == LetterType::_Mul);
	bool is_div = (key == LetterType::_Div);
	if ((is_plus || is_minus || is_mul || is_div) && val2 == NULL) {
		// op flags and key setting (simplified)
		return self;
	}
	return ErrorA(thgc, LetterType::OT_Error, self->letter, "elem値は+,!,*の単項演算子しか作用しません");
}

// ============================================================
// Signal::exepC
// ============================================================
Obj* Signal_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Signal::PrimaryC
// ============================================================
Obj* Signal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	if (val2->type == LetterType::OT_Word) {
		String* word_name = val2->letter->text;
		ATSSpan* word_letter = val2->letter;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_CallBlock) {
			if (local->comments->size > 0) {
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数は構造コメントブロックの中では宣言出来ません");
			}
			(*n)++;

			// Create SignalFunction
			Obj* func = make_function(thgc, LetterType::OT_SignalFunction, word_letter);
			func->vt = &vt_SignalFunction;
			((Function*)func)->draw = (Block*)val2;

			// Declare in local
			declareA(thgc, local, word_name, func);

			// Process blk1 (children[0] of CallBlock)
			Block* blk1 = *(Block**)get_list(val2->children, 0);
			ObjBlock* objblk1 = make_objblock_z(thgc, func, 0);
			blk1->obj = (Obj*)objblk1;
			pushBlock(thgc, local, blk1);
			vt_exeC(thgc, (Obj*)blk1, local);

			// Process blk2 (children[1] of CallBlock)
			Block* blk2 = *(Block**)get_list(val2->children, 1);
			ObjBlock* objblk2 = make_objblock_z(thgc, func, 1);
			blk2->obj = (Obj*)objblk2;
			pushBlock(thgc, local, blk2);
			vt_exeC(thgc, (Obj*)blk2, local);

			// Pop both blocks
			if (blocks->size > 0) blocks->size--;
			if (blocks->size > 0) blocks->size--;

			// Check if inside ServerFunction block
			for (int i = blocks->size - 1; i > 0; i--) {
				Block* b = *(Block**)get_list(blocks, i);
				if (b->obj != NULL && ((ObjBlock*)b->obj)->obj->type == LetterType::OT_ServerFunction) {
					return func;
				}
			}
			return ErrorA(thgc, LetterType::OT_Error, self->letter, "signal関数はserver{|},client{|}の中でしか宣言出来ません");
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数に{|}が宣言されていない");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "signal関数を宣言してください");
}

// ============================================================
// SignalFunction::exeC
// ============================================================
Obj* SignalFunction_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	List* blocks = get_blocks(local);
	if (((Function*)self)->draw == NULL) return self;

	// Clone block1 from draw.children[0], set up req/res as Stock variables
	Block* block1 = *(Block**)get_list(((Obj*)((Function*)self)->draw)->children, 0);

	// Set up req and res as Stock objects in block1.vmapA
	Stock* stock1 = make_stock(thgc);
	Stock* stock2 = make_stock(thgc);
	add_mapy(thgc, block1->vmapA, createString(thgc, (char*)"req", 3, 1), (char*)stock1);
	add_mapy(thgc, block1->vmapA, createString(thgc, (char*)"res", 3, 1), (char*)stock2);

	// Add block1 to local.blocks and execute
	add_list(thgc, blocks, (char*)block1);

	// Execute block2 (children[1] of draw)
	Block* block2 = (Block*)GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(((Obj*)((Function*)self)->draw)->children, 1), local), local);

	// Check last ret for Return containing Comment
	if (block2 != NULL && block2->rets != NULL && block2->rets->size > 0) {
		Obj* val2 = *(Obj**)get_list(block2->rets, block2->rets->size - 1);
		if (val2->type == LetterType::OT_Return && val2->value != NULL) {
			val2 = val2->value;
			if (val2->type == LetterType::OT_Comment) {
				// Remove block1 from blocks
				if (blocks->size > 0) blocks->size--;
				return val2;
			}
		}
	}

	// Remove block1 from blocks
	if (blocks->size > 0) blocks->size--;
	return self;
}

// ============================================================
// SignalFunction::PrimaryC
// ============================================================
Obj* SignalFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "signal関数はユーザーから呼び出せません");
}

// ============================================================
// SignalFunction::GetterC
// ============================================================
Obj* SignalFunction_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	// add comps to func.comps
	for (int i = 0; i < local->compsC->size; i++) {
		add_list(thgc, llvm_get_func(local->llvm)->comps, *(char**)get_list(local->compsC, i));
	}
	local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);

	char vbuf[32];
	snprintf(vbuf, sizeof(vbuf), "%%v%d", llvm_n++);
	LVari* fv = make_lvari_c(thgc, "%MemFuncType*", vbuf);
	// func.vari.type = "%MemFuncType**"
	llvm_get_func(local->llvm)->vari->type = createString(thgc, (char*)"%MemFuncType**", 14, 1);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, fv, llvm_get_func(local->llvm)->vari, false));
	// Return clone-like result
	Obj* sf = make_cobj(thgc, CType::_CObj, self->type, self->letter);
	sf->vari_c = fv;
	return sf;
}

// ============================================================
// ServerClient::exepC
// ============================================================
Obj* ServerClient_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// ServerClient::PrimaryC
// ============================================================
Obj* ServerClient_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			// lookup sigmapA[name + ":" + word.name]
			// simplified - would need string concatenation
			return NULL;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	} else if (val2->type == LetterType::OT_Word) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_CallBlock) {
			(*n)++;
			// lookup sigmapA and return ServerFunction
			return NULL;
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, "server,clientの{|}が宣言されていません");
	} else if (val2->type == LetterType::OT_CallBlock) {
		(*n)++;
		// draw = val2 as CallBlock, Dec(local)
		return NULL;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "server,clientの後ろには名前か{|}がきます");
}

// ============================================================
// ServerFunction::exeC
// ============================================================
Obj* ServerFunction_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// ServerFunction::PrimaryC
// ============================================================
Obj* ServerFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorA(thgc, LetterType::OT_NG, self->letter, "ServerClient関数は呼び出せません");
}

// ============================================================
// ServerFunction::GetterC
// ============================================================
Obj* ServerFunction_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// Connect::PrimaryC
// ============================================================
Obj* Connect_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			// word.name == "new"
			if (val2->type == LetterType::OT_Bracket) {
				Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
				if (IS_WEG_C(val)) return val;
				Block* blk1 = (Block*)val;
				if (blk1->rets->size == 1) {
					// create MemTable
					char mtbuf[32];
					snprintf(mtbuf, sizeof(mtbuf), "%%mt%d", llvm_n++);
					LVari* mt = make_lvari_c(thgc, "%CreateMemTableType", mtbuf);
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, mt, make_lvari_c(thgc, "%CreateMemTableType*", "@CreateMemTable"), false));
					char mvbuf[32];
					snprintf(mvbuf, sizeof(mvbuf), "%%mv%d", llvm_n++);
					LVari* mv = make_lvari_c(thgc, "%MemTableType*", mvbuf);
					LCall* mv_call = make_lcall(thgc, mv, mt);
					add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mv_call);
					self->vari_c = mv;
					// Return ConnectStock-like object
					Obj* cs = make_cobj(thgc, CType::_CObj, LetterType::OT_ConnectStock, self->letter);
					return cs;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "connect.newの引数は0か1です");
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, "connect.newの()が宣言されていません");
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".が来ていません");
}

// ============================================================
// ConnectStock::PrimaryC
// ============================================================
Obj* ConnectStock_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			String* wordname = (*(Obj**)get_list(primary->children, *n - 1))->letter->text;
			if (str_matchA(wordname, "back", 4)) {
				// memtable.back = coroutine
				return make_variable(thgc, self->letter);
			} else if (str_matchA(wordname, "Store", 5)) {
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
					if (IS_WEG_C(val)) return val;
					(*n)++;
					return make_voival(thgc);
				}
				return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Storeの()が宣言されていません");
			} else {
				// lookup in value's draw blocks
				return make_variable(thgc, self->letter);
			}
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	} else if (val2->type == LetterType::OT_Block) {
		Obj* blk01 = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		Block* blk_result = (Block*)blk01;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				String* word2name = (*(Obj**)get_list(primary->children, *n - 1))->letter->text;
				if (str_matchA(word2name, "Output", 6)) {
					if (val2->type == LetterType::OT_Bracket) {
						return make_cobj(thgc, CType::_CObj, LetterType::OT_ElemObj, self->letter);
					}
					return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Outputの()がありません");
				} else if (str_matchA(word2name, "Select", 6)) {
					if (val2->type == LetterType::OT_Bracket) {
						Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
						if (IS_WEG_C(val)) return val;
						return val;
					}
					return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Selectの()がありません");
				} else if (str_matchA(word2name, "First", 5)) {
					if (val2->type == LetterType::OT_Bracket) {
						return make_cobj(thgc, CType::_CObj, LetterType::OT_ModelVal, self->letter);
					}
					return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Firstの()がありません");
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "宣言されていません");
			}
			return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
		}
		return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が予測されます");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, ".が予測されます");
}

// ============================================================
// Address::PrimaryC
// ============================================================
Obj* Address_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			if (val2->type == LetterType::_Dot) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_ServerClient) {
					(*n)++;
					// LoadProjectC and return vmapA lookup
					return NULL;
				}
				return ErrorA(thgc, LetterType::OT_Error, val2->letter, "addressの後ろにはserverかclientがきます");
			}
		}
		return ErrorA(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "addressの後ろには.か()がきます");
}

// ============================================================
// Mountain::exepC
// ============================================================
Obj* Mountain_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	(*n)++;
	Obj* val2 = *(Obj**)get_list(primary->children, *n);

	// First range word
	if (val2->type == LetterType::OT_Word) {
		// Parse range notation (e.g., "A1", "B2")
		// Build array blocks for range
		List* rets_list = create_list(thgc, sizeof(List*), CType::_List);

		// Parse first word into indices
		List* ret = create_list(thgc, sizeof(int), CType::_Struct);
		// (simplified range parsing)
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		add_list(thgc, rets_list, (char*)ret);

		if (rets_list->size == 1) {
			// Check for Mountain separator
			if (val2->type == LetterType::OT_Mountain) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				// Parse second range word
				List* ret2 = create_list(thgc, sizeof(int), CType::_Struct);
				add_list(thgc, rets_list, (char*)ret2);
				(*n)--;
			}
			// Build result block
			Block* blk = make_cblock(thgc, CType::_CBlock, LetterType::OT_Array);
			Block* blk1 = make_cblock(thgc, CType::_CBlock, LetterType::OT_Array);
			blk->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
			blk1->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
			add_list(thgc, blk->rets, (char*)blk1);
			add_list(thgc, blk->rets, (char*)blk1);
			return (Obj*)blk;
		}
		// Two ranges
		Block* blk = make_cblock(thgc, CType::_CBlock, LetterType::OT_Array);
		Block* blk1 = make_cblock(thgc, CType::_CBlock, LetterType::OT_Array);
		Block* blk2 = make_cblock(thgc, CType::_CBlock, LetterType::OT_Array);
		blk->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
		blk1->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
		blk2->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
		add_list(thgc, blk->rets, (char*)blk1);
		add_list(thgc, blk->rets, (char*)blk2);
		return (Obj*)blk;
	}
	return ErrorA(thgc, LetterType::OT_Error, val2->letter, "範囲の記述式がきていません");
}

// ============================================================
// Sum::PrimaryC
// ============================================================
Obj* Sum_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		// Sum computation - simplified for C pass
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		local->compsC->size--;
		if (IS_WEG_C(val)) return val;
		// Return Number result
		Obj* num = make_number(thgc, self->letter);
		((Val*)num)->cls = local->Int;
		return num;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "Sumの()がありません");
}

// ============================================================
// Question::exepC
// ============================================================
Obj* Question_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Block* blk = make_cblock(thgc, CType::_CBlock, LetterType::OT_Array);
	Block* blk1 = make_cblock(thgc, CType::_CBlock, LetterType::OT_Array);
	blk->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	blk1->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	add_list(thgc, blk->rets, (char*)blk1);
	add_list(thgc, blk->rets, (char*)blk1);
	for (int i = 0; i < 2; i++) {
		Obj* num = make_number(thgc, NULL);
		((Val*)num)->cls = (Obj*)local->Int;
		add_list(thgc, blk1->rets, (char*)num);
	}
	return (Obj*)blk1;
}

// ============================================================
// LinearFunction::PrimaryC
// ============================================================
Obj* LinearFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		// Animation function creation (simplified for C pass)
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		local->compsC->size--;
		if (IS_WEG_C(val)) return val;
		// Return AnimationFunction-like result
		AnimationFunction* anif = AnimationFunction_make(thgc);
		return anif;
	}
	return ErrorA(thgc, LetterType::OT_NG, val2->letter, "linearの()が書かれていません");
}
