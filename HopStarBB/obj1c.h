// ============================================================
// obj1c.h - C解析パス (LLVM IR code generation) - Obj1C.cs converted
// ============================================================

// ============================================================
// 前方宣言
// ============================================================
Obj* Operator_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Primary_NextC(ThreadGC* thgc, Primary* primary, Master* local, int* i, Obj** val1);
Obj* Primary_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Block_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_exeRangeC(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id);
Obj* Block_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Block_SelfC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Block_EqualC(ThreadGC* thgc, Block* self, LetterType op, Master* local, Obj* val2);
Obj* Block_InC(ThreadGC* thgc, Block* self, LetterType op, Master* local, Obj* val2);
Obj* Block_OpeC(ThreadGC* thgc, Block* self, LetterType op, Master* local, Obj* val2);
Obj* Value_OpeC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Block_SortC(ThreadGC* thgc, Obj* self, Function* func, Master* local);
Obj* Block_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* CallBlock_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* CallBlock_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* declareC(ThreadGC* thgc, Master* local, String* name);
Obj* getC(ThreadGC* thgc, Master* local, String* name);

// ============================================================
// ヘルパー: "%v" + llvm_n++ のLVari生成
// ============================================================
static inline LVari* BitcastCast(ThreadGC* thgc, Master* local, Obj* targetCls, Obj* srcCls, LVari* srcVari);
static inline LVari* make_vn(ThreadGC* thgc, const char* type, const char* prefix = "v") {
	char buf[64];
	snprintf(buf, sizeof(buf), "%%%s%d", prefix, llvm_n++);
	return make_lvari_c(thgc, type, buf);
}

// ============================================================
// BitcastCast: integer width conversion + pointer bitcast
// Ported from C# LLVM.Cast (LLVM.cs Cast(local, t1=target, t2=source, vari)).
// C# logic:
//   t1==t2            -> return vari
//   t1=Int:
//     t2=Short/Bool   -> Zext to i32
//   t1=Short:
//     t2=Int          -> Trunc to i16
//     t2=Bool         -> Zext  to i16
//   t1=Bool:
//     t2=Int/Short    -> Trunc to i1
//   otherwise (pointer/object types): emit bitcast (retain previous C++ behaviour,
//   which is the only way to reinterpret between class pointer types at LLVM IR level).
// ============================================================
static inline LVari* BitcastCast(ThreadGC* thgc, Master* local, Obj* targetCls, Obj* srcCls, LVari* srcVari) {
	if (targetCls == srcCls) return srcVari;
	LFunc* func = llvm_get_func(local->llvm);
	Obj* INT  = (Obj*)local->Int;
	Obj* SHT  = (Obj*)local->Short;
	Obj* BL   = (Obj*)local->Bool;
	// --- Integer-width conversions (mirror C# Cast) ---
	if (targetCls == INT) {
		if (srcCls == SHT || srcCls == BL) {
			LVari* v = make_vn(thgc, "i32", "cv");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Zext, v, srcVari));
			return v;
		}
	} else if (targetCls == SHT) {
		if (srcCls == INT) {
			LVari* v = make_vn(thgc, "i16", "cv");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Trunc, v, srcVari));
			return v;
		}
		if (srcCls == BL) {
			LVari* v = make_vn(thgc, "i16", "cv");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Zext, v, srcVari));
			return v;
		}
	} else if (targetCls == BL) {
		if (srcCls == INT || srcCls == SHT) {
			LVari* v = make_vn(thgc, "i1", "cv");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Trunc, v, srcVari));
			return v;
		}
	}
	// --- Non-integer cases: either identity (incompatible) or pointer bitcast ---
	// If both sides are primitive integer classes but didn't match above, pass through
	// (C# throws here; we keep srcVari to avoid crashing the emitter mid-pass).
	if (targetCls == INT || targetCls == SHT || targetCls == BL ||
		srcCls == INT || srcCls == SHT || srcCls == BL) {
		return srcVari;
	}
	// Class-to-class pointer reinterpretation
	LVari* v = make_vn(thgc, "i8*", "bc");
	add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, srcVari));
	return v;
}

// ============================================================
// SetIdentity: per-class virtual refresh of identity + model/call/drawcall
// C# has model/call/drawcall as dynamic override properties; C++ stores them
// as fields and refreshes via vtable dispatch.
// ============================================================
static inline void Default_SetIdentity(ThreadGC* thgc, Obj* self) {
	if (self->identity != 0) return;
	self->identity = llvm_n++;
}

// ClassObj / GeneObj: "%nameType<id>", "@name<id>", "@nameDraw<id>"
static inline void ClassObj_SetIdentity(ThreadGC* thgc, Obj* self) {
	if (self->identity != 0) return;
	self->identity = llvm_n++;
	const char* lname = self->letter ? str_to_cstr(thgc, self->letter->text) : "c";
	char buf[256]; int len;
	len = snprintf(buf, sizeof(buf), "%%%sType%d", lname, self->identity);
	self->model_str = createString(thgc, buf, len, 1);
	self->model = createString(thgc, buf, len, 1);
	len = snprintf(buf, sizeof(buf), "@%s%d", lname, self->identity);
	self->call_c = createString(thgc, buf, len, 1);
	len = snprintf(buf, sizeof(buf), "@%sDraw%d", lname, self->identity);
	self->drawcall = createString(thgc, buf, len, 1);
}

// Function: "%nameFuncType<id>", "@name<id>", "@nameDraw<id>"
static inline void Function_SetIdentity(ThreadGC* thgc, Obj* self) {
	if (self->identity != 0) return;
	self->identity = llvm_n++;
	const char* lname = self->letter ? str_to_cstr(thgc, self->letter->text) : "f";
	char buf[256]; int len;
	len = snprintf(buf, sizeof(buf), "%%%sFuncType%d", lname, self->identity);
	self->model_str = createString(thgc, buf, len, 1);
	self->model = createString(thgc, buf, len, 1);
	len = snprintf(buf, sizeof(buf), "@%s%d", lname, self->identity);
	self->call_c = createString(thgc, buf, len, 1);
	len = snprintf(buf, sizeof(buf), "@%sDraw%d", lname, self->identity);
	self->drawcall = createString(thgc, buf, len, 1);
}

// ServerClient-style CObj (CObj2C.cs): "%nameType", "@nameCall", "@nameDraw"
static inline void CObj_SetIdentity(ThreadGC* thgc, Obj* self) {
	if (self->identity != 0) return;
	self->identity = llvm_n++;
	const char* lname = self->letter ? str_to_cstr(thgc, self->letter->text) : "c";
	char buf[256]; int len;
	len = snprintf(buf, sizeof(buf), "%%%sType", lname);
	self->model_str = createString(thgc, buf, len, 1);
	self->model = createString(thgc, buf, len, 1);
	len = snprintf(buf, sizeof(buf), "@%sCall", lname);
	self->call_c = createString(thgc, buf, len, 1);
	len = snprintf(buf, sizeof(buf), "@%sDraw", lname);
	self->drawcall = createString(thgc, buf, len, 1);
}

// vtable dispatch
static inline void SetIdentity(ThreadGC* thgc, Obj* self) {
	_vt_lazy_assign(self);
	self->vt->SetIdentity(thgc, self);
}

// ============================================================
// ヘルパー: snprintf+createString
// ============================================================
static inline String* fmt_str(ThreadGC* thgc, const char* fmt, ...) {
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	return createString(thgc, buf, len, 1);
}

static inline LVari* make_vn_s(ThreadGC* thgc, String* type, const char* prefix = "v") {
	String* name = fmt_str(thgc, "%%%s%d", prefix, llvm_n++);
	return make_lvari(thgc, type, name);
}

// ============================================================
// ヘルパー: String*にsuffix付加 (model->dataをC文字列として使わないため)
// ============================================================
static inline String* str_suffix(ThreadGC* thgc, String* s, const char* suffix) {
	return StringAdd(thgc, s, (char*)suffix, (int)strlen(suffix), 1);
}
static inline String* model_type(ThreadGC* thgc, String* model, const char* suffix) {
	if (model) return StringAdd(thgc, model, (char*)suffix, (int)strlen(suffix), 1);
	int slen = (int)strlen(suffix);
	char buf[16]; memcpy(buf, "i8", 2); memcpy(buf + 2, suffix, slen);
	return createString(thgc, buf, 2 + slen, 1);
}

// ============================================================
// ヘルパー: LGeteにインデックスを追加
// ============================================================
static inline LGete* make_lgete_idx2(ThreadGC* thgc, const char* type, LVari* y, LVari* x, LVari* idx0, LVari* idx1) {
	String* ts = createString(thgc, (char*)type, (int)strlen(type), 1);
	LGete* g = make_lgete(thgc, ts, y, x);
	add_list(thgc, ((LComp*)g)->comps, (char*)idx0);
	add_list(thgc, ((LComp*)g)->comps, (char*)idx1);
	return g;
}
static inline LGete* make_lgete_idx2_s(ThreadGC* thgc, String* type, LVari* y, LVari* x, LVari* idx0, LVari* idx1) {
	LGete* g = make_lgete(thgc, type, y, x);
	add_list(thgc, ((LComp*)g)->comps, (char*)idx0);
	add_list(thgc, ((LComp*)g)->comps, (char*)idx1);
	return g;
}

// ============================================================
// ヘルパー: LCallに引数を追加
// ============================================================
static inline void lcall_add(ThreadGC* thgc, LCall* c, LVari* arg) {
	add_list(thgc, ((LComp*)c)->comps, (char*)arg);
}

// ============================================================
// ヘルパー: エラーObj生成
// ============================================================
static inline Obj* ErrorC(ThreadGC* thgc, LetterType type, ATSSpan* letter, const char* msg) {
	Obj* err = make_cobj(thgc, CType::_CObj, type, letter);
	if (type != LetterType::OT_Wait) {
		if (letter) {
			letter->error = createString(thgc, (char*)msg, (int)strlen(msg), 1);
			letter->underlineColor = 0xFF0000FF;
		}
	}
	return err;
}

// ============================================================
// Operator::exeC
// ============================================================
Obj* Operator_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	LetterType op = self->type;
	if (op == LetterType::_Equal || op == LetterType::_Colon || op == LetterType::_In) {
		// Assignment/colon: evaluate RHS first (Getter), then LHS (Self)
		Obj* val2 = GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(self->children, 1), local), local);
		Obj* val1 = SelfC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(self->children, 0), local), local);
		return vt_opeC(thgc, val1, op, local, val2);
	}
	{
		Obj* val1 = GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(self->children, 0), local), local);
		if (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG) return val1;
		Obj* val2 = GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(self->children, 1), local), local);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
		return vt_opeC(thgc, val1, op, local, val2);
	}
}

// ============================================================
// Primary::NextC
// ============================================================
Obj* Primary_NextC(ThreadGC* thgc, Primary* primary, Master* local, int* i, Obj** val1) {
	Obj* child = *(Obj**)get_list(primary->children, *i);
	switch (child->type) {
		case LetterType::OT_Comment2:
			(*i)++;
			break;
		case LetterType::OT_Htm:
		case LetterType::OT_TagBlock:
			vt_exepC(thgc, child, i, local, primary);
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
				// C#: val1 = primary.children[i].exepC(ref i, local, primary);
				*val1 = vt_exepC(thgc, child, i, local, primary);
				(*i)++;
			} else {
				*val1 = GetterC_vt(thgc, *val1, local);
				if ((*val1)->type == LetterType::OT_Wait || (*val1)->type == LetterType::OT_Error || (*val1)->type == LetterType::OT_NG) return *val1;
				// C#: val1 = val1.PrimaryC(ref i, local, primary, primary.children[i].Clone());
				*val1 = PrimaryC_vt(thgc, *val1, i, local, primary, vt_Clone(thgc, child));
			}
			break;
		case LetterType::_Dot:
		case LetterType::_Left:
		case LetterType::_Right:
			*val1 = GetterC_vt(thgc, *val1, local);
			if ((*val1)->type == LetterType::OT_Wait || (*val1)->type == LetterType::OT_Error || (*val1)->type == LetterType::OT_NG) return *val1;
			*val1 = PrimaryC_vt(thgc, *val1, i, local, primary, child);
			break;
		case LetterType::OT_CallBlock:
		case LetterType::OT_Block:
			if (*val1 == NULL) {
				*val1 = vt_exepC(thgc, child, i, local, primary);
				(*i)++;
			} else {
				*val1 = GetterC_vt(thgc, *val1, local);
				if ((*val1)->type == LetterType::OT_Wait || (*val1)->type == LetterType::OT_Error || (*val1)->type == LetterType::OT_NG) return *val1;
				*val1 = PrimaryC_vt(thgc, *val1, i, local, primary, child);
			}
			break;
		default:
			break;
	}
	return *val1;
}

// ============================================================
// Primary::exeC
// ============================================================
Obj* Primary_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	Primary* pself = (Primary*)self;
	// C#: if (local.comments.Count > 0) local.comments.Last().Start();
	// TODO: comments.Last().Start() = instanceslist.Add(new List<Element>()) — Element not ported
	Obj* val1 = NULL;
	for (int i = 0; i < pself->children->size - 1;) {
		Primary_NextC(thgc, pself, local, &i, &val1);
		if (val1 != NULL && (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG)) return val1;
	}
	if (val1 == NULL) val1 = *(Obj**)get_list(pself->children, pself->children->size - 1);

	// C#: for (int i = singleops.Count - 1; i >= 0; i--) { var str = singleops[i].name; val1 = val1.GetterC(local); val1 = val1.opeC(str, local, null); break; }
	for (int i = pself->singleops->size - 1; i >= 0; i--) {
		SingleOp* sop = *(SingleOp**)get_list(pself->singleops, i);
		val1 = GetterC_vt(thgc, val1, local);
		val1 = vt_opeC(thgc, val1, sop->letter->t, local, NULL);
		break;
	}
	return val1;
}

// ============================================================
// Block::exeC
// ============================================================
Obj* Block_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	if (blk->type == LetterType::OT_Array) return (Obj*)blk;
	return Block_exeRangeC(thgc, blk, 0, blk->children->size, local, true);
}

// ============================================================
// Block::exeRangeC
// ============================================================
// C#: for (l = letter.parent; l != null; l = l.parent) { if (l.type == LetterType.Line) break; } return (l as Line).line;
static int getLineFromLetter(ATSSpan* span) {
	if (span == NULL || span->letter == NULL) return 1;
	for (NewElement* l = span->letter->parent; l != NULL; l = l->parent) {
		if (l->type == LetterType::_Line) return ((NewLine*)l)->line;
	}
	return 1;
}

Obj* Block_exeRangeC(ThreadGC* thgc, Block* self, int start, int end, Master* local, bool id) {
	self->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	LFunc* func = llvm_get_func(local->llvm);
	for (int i = start; i < end; i++) {
		Obj* child = *(Obj**)get_list(self->children, i);
		// C#: var li = (children[i].letter.parent -> Line); LocationMarker(li.line, i, ...)
		int ln = getLineFromLetter(child->letter);
		add_list(thgc, func->comps, (char*)make_llocmarker(thgc, ln, i, func->debugId));
		Obj* val = vt_exeC(thgc, child, local);
		add_list(thgc, self->rets, (char*)val);
	}
	if (start == end) {
		// C#: int line = 1; if (this.letter != null) { for (l ...) { if (l.type == Line) line = (l as Line).line; } }
		int ln = getLineFromLetter(((Obj*)self)->letter);
		add_list(thgc, func->comps, (char*)make_llocmarker(thgc, ln, 0, func->debugId));
	}
	return (Obj*)self;
}

// ============================================================
// Block::exepC
// ============================================================
Obj* Block_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	Block* blk = (Block*)self;

	if (blk->type == LetterType::OT_Block) {
		// C#: if (children[m].type == ObjType.Word && (children[m] as Word).name == "T") — Generic/Function declaration path
		int m = 0;
		Obj* child0 = *(Obj**)get_list(blk->children, m);
		if (child0->type == LetterType::OT_Word) {
			Word* w0 = (Word*)child0;
			if (str_equals(w0->name, "T")) {
				m++;
				Obj* val2 = *(Obj**)get_list(blk->children, m);
				// C#: Type type = new VariClass(0);
				Obj* type = (Obj*)make_variclass(thgc, 0);
			head_T:
				// C#: if (val2.type == ObjType.Block)
				if (val2->type == LetterType::OT_Block) {
					Block* tblk = (Block*)val2;
					if (tblk->children->size == 0) {
						// C#: type = new ArrType(type);
						type = (Obj*)make_arrtype(thgc, type);
						m++;
						val2 = *(Obj**)get_list(blk->children, m);
						goto head_T;
					}
					else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型は引数が0です");
				}
				// C#: n = 1; val2 = primary.children[n];
				*n = 1;
				val2 = *(Obj**)get_list(primary->children, *n);
				// C#: if (val2.type == ObjType.Word)
				if (val2->type == LetterType::OT_Word) {
					Word* word = (Word*)val2;
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					// C#: if (val2.type == ObjType.Block) — GenericFunction path
					if (val2->type == LetterType::OT_Block) {
						// C#: var func = local.declareC(word.name, local) as GenericFunction;
						GenericFunction* func = (GenericFunction*)declareC(thgc, local, word->name);
						Block* tblk2 = (Block*)vt_exeC(thgc, val2, local);
						// C#: for (i=0; i<blk.rets.Count; i++) { func.vmap[word2.name] = new VariClass(i); }
						for (int i = 0; i < tblk2->rets->size; i++) {
							Obj* ri = *(Obj**)get_list(tblk2->rets, i);
							if (ri->type == LetterType::OT_Word) {
								Word* word2 = (Word*)ri;
								add_mapy(thgc, func->vmap, word2->name, (char*)make_variclass(thgc, i));
							}
							else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスの引数は名前を指定してください");
						}
						(*n)++;
						val2 = *(Obj**)get_list(primary->children, *n);
						// C#: if (val2.type == ObjType.CallBlock)
						if (val2->type == LetterType::OT_CallBlock) {
							func->draw = (CallBlock*)val2;
							// C#: foreach (var b in local.blocks) func.blocks.Add(b);
							List* blks = get_blocks(local);
							for (int i = 0; i < blks->size; i++) {
								char* b = *(char**)get_list(blks, i);
								add_list(thgc, func->blocks, b);
							}
							return (Obj*)func;
						}
						return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックス関数の{|}が宣言されていません");
					}
					// C#: else if (val2.type == ObjType.CallBlock) — Function path
					else if (val2->type == LetterType::OT_CallBlock) {
						// C#: var func = local.declareC(word.name, local) as Function;
						Function* func = (Function*)declareC(thgc, local, word->name);
						func->draw = (CallBlock*)val2;
						// C#: foreach (var b in local.blocks) func.blocks.Add(b);
						List* blks = get_blocks(local);
						for (int i = 0; i < blks->size; i++) {
							char* b = *(char**)get_list(blks, i);
							add_list(thgc, func->blocks, b);
						}
						return (Obj*)func;
					}
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "ジェネリックス関数の{|}が宣言されていません");
				}
			}
		}
	}
	if (blk->type == LetterType::OT_Block) {
		LFunc* lfunc = llvm_get_func(local->llvm);
		LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
		char buf0[64]; snprintf(buf0, sizeof(buf0), "%%CreateHash%d", llvm_n++);
		LVari* vcl = make_lvari_c(thgc, "%CreateHashType", buf0);
		add_list(thgc, lfunc->comps, (char*)make_lload(thgc, vcl, make_lvari_c(thgc, "%CreateHashType*", "@CreateHash"), false));
		char buf1[64]; snprintf(buf1, sizeof(buf1), "%%ho%d", llvm_n++);
		LVari* voj = make_lvari_c(thgc, "%HashType*", buf1);
		// C#: if (cls.type == ObjType.Var) cls = (cls as Var).cls;
		Type* bcls = blk->cls;
		if (bcls != NULL && bcls->type == LetterType::OT_Var) bcls = (Type*)((Val*)bcls)->cls;
		// C#: objected/size determination
		bool objected = true;
		if (bcls == (Type*)local->Int) {
			objected = false;
		} else if (bcls == (Type*)local->Bool) {
			objected = false;
		}
		LVari* objected_v = make_lvari_c(thgc, "i1", objected ? "true" : "false");
		LCall* call1 = make_lcall(thgc, voj, vcl);
		lcall_add(thgc, call1, thgc4);
		lcall_add(thgc, call1, objected_v);
		add_list(thgc, lfunc->comps, (char*)call1);
		Obj* blk2 = Block_exeRangeC(thgc, blk, 0, blk->children->size, local, false);
		char buf2[64]; snprintf(buf2, sizeof(buf2), "%%AddHash%d", llvm_n++);
		LVari* varih = make_lvari_c(thgc, "%AddHashType", buf2);
		add_list(thgc, lfunc->comps, (char*)make_lload(thgc, varih, make_lvari_c(thgc, "%AddHashType*", "@AddHash"), false));
		for (int i = 0; i < blk->rets->size; i++) {
			Obj* ret_i = *(Obj**)get_list(blk->rets, i);
			// C#: if (blk.labelmapn.ContainsKey(i))
			Label* lab = (Label*)get_mapyn(blk->labelmapn, (char*)(intptr_t)i);
			if (lab != NULL) {
				if (objected) {
					// C#: new Call(null, varih, thgc4, voj, new Vari("%StringType*", blk.labelmapn[i].name), rets[i].vari)
					LCall* addcall = make_lcall(thgc, NULL, varih);
					lcall_add(thgc, addcall, thgc4);
					lcall_add(thgc, addcall, voj);
					lcall_add(thgc, addcall, make_lvari_s(thgc, "%StringType*", lab->name));
					lcall_add(thgc, addcall, ((Value*)ret_i)->vari);
					add_list(thgc, lfunc->comps, (char*)addcall);
				} else {
					// C#: var v0 = new Vari("i8*", ...); IntToPtr(v0, rets[i].vari); Call(null, varih, thgc4, voj, labelmapn[i].name, v0)
					char buf3[64]; snprintf(buf3, sizeof(buf3), "%%ip%d", llvm_n++);
					LVari* v0 = make_lvari_c(thgc, "i8*", buf3);
					add_list(thgc, lfunc->comps, (char*)make_lcast(thgc, LCAST_IntToPtr, v0, ((Value*)ret_i)->vari));
					LCall* addcall = make_lcall(thgc, NULL, varih);
					lcall_add(thgc, addcall, thgc4);
					lcall_add(thgc, addcall, voj);
					lcall_add(thgc, addcall, make_lvari_s(thgc, "%StringType*", lab->name));
					lcall_add(thgc, addcall, v0);
					add_list(thgc, lfunc->comps, (char*)addcall);
				}
			} else {
				if (objected) {
					// C#: new Call(null, varih, voj, new Vari("%StringType*", "null"), rets[i].vari)
					LCall* addcall = make_lcall(thgc, NULL, varih);
					lcall_add(thgc, addcall, voj);
					lcall_add(thgc, addcall, make_lvari_c(thgc, "%StringType*", "null"));
					lcall_add(thgc, addcall, ((Value*)ret_i)->vari);
					add_list(thgc, lfunc->comps, (char*)addcall);
				} else {
					// C#: IntToPtr + Call with null key
					char buf4[64]; snprintf(buf4, sizeof(buf4), "%%ip%d", llvm_n++);
					LVari* v0 = make_lvari_c(thgc, "i8*", buf4);
					add_list(thgc, lfunc->comps, (char*)make_lcast(thgc, LCAST_IntToPtr, v0, ((Value*)ret_i)->vari));
					LCall* addcall = make_lcall(thgc, NULL, varih);
					lcall_add(thgc, addcall, thgc4);
					lcall_add(thgc, addcall, voj);
					lcall_add(thgc, addcall, make_lvari_c(thgc, "%StringType*", "null"));
					lcall_add(thgc, addcall, v0);
					add_list(thgc, lfunc->comps, (char*)addcall);
				}
			}
		}
		// C#: return new Value(new ArrType(cls), voj);
		Value* result = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
		((Val*)result)->cls = (Obj*)make_arrtype(thgc, (Obj*)bcls);
		result->vari = voj;
		return (Obj*)result;
	}
	return Block_exeRangeC(thgc, blk, 0, blk->children->size, local, true);
}

// ============================================================
// Block::PrimaryC
// ============================================================
Obj* Block_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Block* blk = (Block*)self;
	if (blk->type == LetterType::OT_Bracket) {
		if (val2->type == LetterType::OT_Block) {
			if (blk->rets->size == 1) {
				Block* v2blk = (Block*)val2;
				if (v2blk->children->size == 0) {
					Obj* cls = *(Obj**)get_list(blk->rets, 0);
					if (cls != NULL) {
						ArrType* typearray = make_arrtype(thgc, cls);
						return (Obj*)typearray;
					}
					return ErrorC(thgc, LetterType::OT_Error, val2->letter, "array type needs base type");
				}
				return ErrorC(thgc, LetterType::OT_Error, val2->letter, "array type [] must have 0 args");
			}
			return ErrorC(thgc, LetterType::OT_Error, self->letter, "array type needs 1 type");
		}
		else if (val2->type == LetterType::OT_Bracket) {
			Obj* block = vt_exeC(thgc, val2, local);
			Block* blk2 = (Block*)block;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			for (int i = 0; i < blk2->rets->size; i++) {
				Obj* ri = *(Obj**)get_list(blk2->rets, i);
				if (ri->type == LetterType::OT_Word) {
					Obj* cls = *(Obj**)get_list(((Block*)self)->rets, i % ((Block*)self)->rets->size);
					if (isTypeObj(cls)) {
						Obj* variable = declareC(thgc, local, ((Word*)ri)->name);
						*(Obj**)get_list(blk2->rets, i) = variable;
					}
					else return ErrorC(thgc, LetterType::OT_Error, cls->letter, "複数型の変数宣言は型を指定してください");
				}
				else return ErrorC(thgc, LetterType::OT_Error, ri->letter, "複数の変数の宣言には名前を指定してください");
			}
			return block;
		}
		else if (val2->type == LetterType::OT_Word) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			Obj* variable = declareC(thgc, local, val2->letter->text);
			return variable;
		}
		else if (val2->type == LetterType::_Left) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (blk->rets->size == 1) {
				Obj* r0 = *(Obj**)get_list(blk->rets, 0);
				return PrimaryC_vt(thgc, r0, n, local, primary, val2);
			}
			return ErrorC(thgc, LetterType::OT_Error, self->letter, "() can only hold 1 item");
		}
		else if (val2->type == LetterType::_Dot) {
			if (blk->rets->size == 1) {
				Obj* r0 = *(Obj**)get_list(blk->rets, 0);
				return PrimaryC_vt(thgc, r0, n, local, primary, val2);
			}
			return ErrorC(thgc, LetterType::OT_Error, self->letter, "() can only hold 1 item");
		}
		return ErrorC(thgc, LetterType::OT_NG, val2->letter, "need something after ()");
	}
	else if (blk->type == LetterType::OT_Block || blk->type == LetterType::OT_Array) {
		if (val2->type == LetterType::OT_Block) {
			// C#: throw new Exception("");
			return ErrorC(thgc, LetterType::OT_Error, val2->letter, "不正なエラー");
		}
		else if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				// C#: kouhos
				if (val2->letter == local->letter && local->kouhos == NULL) {
					local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
					add_kouho(thgc, local->kouhos, createString(thgc, "length", 6, 1), NULL);
					add_kouho(thgc, local->kouhos, createString(thgc, "Sort", 4, 1), NULL);
					add_kouho(thgc, local->kouhos, createString(thgc, "Range", 5, 1), NULL);
				}
				Word* word = (Word*)val2;
				(*n)++;
				if (str_matchA(word->name, "length", 6)) {
					Obj* num = make_number(thgc, NULL);
					((Number*)num)->cls = local->Int;
					return num;
				}
				else if (str_matchA(word->name, "Sort", 4)) {
					val2 = *(Obj**)get_list(primary->children, *n);
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++;
						Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
						if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
						Block* blk1 = (Block*)val;
						Obj* r0 = *(Obj**)get_list(blk1->rets, 0);
						if (r0->type == LetterType::OT_Function) {
							return Block_SortC(thgc, (Obj*)blk, (Function*)r0, local);
						}
						else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Sortの引数は関数しかだめです");
					}
					else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Sortの()がありません");
				}
				else if (str_matchA(word->name, "Range", 5)) {
					val2 = *(Obj**)get_list(primary->children, *n);
					if (val2->type == LetterType::OT_Bracket) {
						Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
						if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
						Block* rblk = (Block*)val;
						if (rblk->rets->size == 0) { }
						else if (rblk->rets->size == 1) {
							Obj* r0 = *(Obj**)get_list(rblk->rets, 0);
							if (r0->type != LetterType::OT_Number) return ErrorC(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(rblk->letters, 0), "Rangeの引数1は整数です");
						}
						else if (rblk->rets->size == 2) {
							Obj* r0 = *(Obj**)get_list(rblk->rets, 0);
							if (r0->type != LetterType::OT_Number) return ErrorC(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(rblk->letters, 0), "Rangeの引数1は整数です");
							Obj* r1 = *(Obj**)get_list(rblk->rets, 1);
							if (r1->type != LetterType::OT_Number) return ErrorC(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(rblk->letters, 0), "Rangeの引数2は整数です");
						}
						else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Rangeの引数は0,1,2です");
						(*n)++;
						return (Obj*)blk;
					}
					else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Rangeの()がありません");
				}
				else return ErrorC(thgc, LetterType::OT_NG, val2->letter, "配列には宣言されていません");
			}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が必要です");
		}
		else return ErrorC(thgc, LetterType::OT_NG, val2->letter, "[]の後が必要です");
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "invalid error");
}

// ============================================================
// Block::SelfC
// ============================================================
Obj* Block_SelfC(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	for (int i = 0; i < blk->rets->size; i++) {
		Obj* ri = *(Obj**)get_list(blk->rets, i);
		ri = SelfC_vt(thgc, ri, local);
		*(Obj**)get_list(blk->rets, i) = ri;
		if (ri->type == LetterType::OT_Wait || ri->type == LetterType::OT_Error || ri->type == LetterType::OT_NG) return ri;
	}
	return self;
}

// ============================================================
// Block::GetterC
// ============================================================
Obj* Block_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	for (int i = 0; i < blk->rets->size; i++) {
		Obj* ri = *(Obj**)get_list(blk->rets, i);
		ri = GetterC_vt(thgc, ri, local);
		*(Obj**)get_list(blk->rets, i) = ri;
		if (ri->type == LetterType::OT_Wait || ri->type == LetterType::OT_Error || ri->type == LetterType::OT_NG) return ri;
	}
	return self;
}

// ============================================================
// Block::EqualC
// ============================================================
Obj* Block_EqualC(ThreadGC* thgc, Block* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Block || val2->type == LetterType::OT_Array) {
		Block* blk = (Block*)val2;
		int blk_size = blk->rets->size;
		for (int i = 0; i < self->rets->size; i++) {
			Obj* ri = *(Obj**)get_list(self->rets, i);
			Obj* bval = *(Obj**)get_list(blk->rets, i % blk_size);
			((Variable*)ri)->value = bval;
		}
	} else {
		for (int i = 0; i < self->rets->size; i++) {
			Obj* ri = *(Obj**)get_list(self->rets, i);
			vt_opeC(thgc, ri, op, local, val2);
		}
	}
	return (Obj*)self;
}

// ============================================================
// Block::InC
// ============================================================
Obj* Block_InC(ThreadGC* thgc, Block* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Block || val2->type == LetterType::OT_Array) {
		for (int i = 0; i < self->rets->size; i++) {
			Obj* ri = *(Obj**)get_list(self->rets, i);
			Obj* it = make_iterator(thgc);
			((Iterator*)it)->value = (Block*)val2;
			((Variable*)ri)->value = it;
		}
		return (Obj*)self;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "iterator needs array");
}

// ============================================================
// Block::OpeC
// ============================================================
Obj* Block_OpeC(ThreadGC* thgc, Block* self, LetterType op, Master* local, Obj* val2) {
	if (self->type == LetterType::OT_Bracket) {
		if (self->rets->size == 1) {
			Obj* r0 = *(Obj**)get_list(self->rets, 0);
			return vt_opeC(thgc, r0, op, local, val2);
		}
		return ErrorC(thgc, LetterType::OT_Error, self->letter, "() can only hold 1 item");
	}
	return ErrorC(thgc, LetterType::OT_Error, self->letter, "operator only for ()");
}

// ============================================================
// Value::OpeC
// ============================================================
Obj* Value_OpeC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	LFunc* func = llvm_get_func(local->llvm);
	Value* selfval = (Value*)self;
	Obj* cls = selfval->cls;
	LVari* selfvari = selfval->vari;

	if (val2->type == LetterType::OT_Value) {
		Value* value = (Value*)val2;
		Obj* vcls = value->cls;
		if (vcls->type == LetterType::OT_Var) vcls = ((Type*)vcls)->cls;
		value->cls = vcls;

		if (cls == (Obj*)local->Int || cls == (Obj*)local->Short) {
			const char* sizetype = "i32";
			if (cls == (Obj*)local->Short) sizetype = "i16";

			if (vcls == (Obj*)local->StrT) {
				if (op == LetterType::_Plus) {
					if (strcmp(sizetype, "i32") != 0) {
						LVari* v = make_vn(thgc, "i32", "bc");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, selfvari));
						selfvari = v;
					}
					LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
					LVari* nsvari = make_vn(thgc, "%NumberStringType", "ns");
					add_list(thgc, func->comps, (char*)make_lload(thgc, nsvari, make_lvari_c(thgc, "%NumberStringType*", "@NumberString"), false));
					LVari* v0 = make_vn(thgc, "%StringType*", "ns");
					LCall* call0 = make_lcall(thgc, v0, nsvari);
					lcall_add(thgc, call0, thgc4);
					lcall_add(thgc, call0, selfvari);
					add_list(thgc, func->comps, (char*)call0);
					LVari* as2vari = make_vn(thgc, "%AddStringType2*", "as");
					add_list(thgc, func->comps, (char*)make_lload(thgc, as2vari, make_lvari_c(thgc, "%AddStringType2*", "@AddString2"), false));
					LVari* v1 = make_vn(thgc, "%StringType*", "sa");
					LCall* call1 = make_lcall(thgc, v1, as2vari);
					lcall_add(thgc, call1, thgc4);
					lcall_add(thgc, call1, v0);
					lcall_add(thgc, call1, value->vari);
					add_list(thgc, func->comps, (char*)call1);
					Value* ret = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					ret->cls = (Obj*)local->StrT;
					ret->vari = v1;
					return (Obj*)ret;
				}
				else return ErrorC(thgc, LetterType::OT_Error, self->letter, "値の演算はサポートされていません。");
			}
			else if (vcls == (Obj*)local->Int || vcls == (Obj*)local->Short) {
				Obj* ret_cls = cls;
				if (cls != vcls) {
					sizetype = "i32";
					if (vcls == (Obj*)local->Short) {
						LVari* v = make_vn(thgc, "i32", "bc");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
						value->vari = v;
						ret_cls = (Obj*)local->Int;
					}
					else if (cls == (Obj*)local->Short) {
						LVari* v = make_vn(thgc, "i32", "bc");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, selfvari));
						selfvari = v;
						ret_cls = (Obj*)local->Int;
					}
				}
				LVari* vari = make_vn(thgc, sizetype, "op");
				if (op == LetterType::_Plus) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Add, vari, selfvari, value->vari));
				else if (op == LetterType::_Minus) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, vari, selfvari, value->vari));
				else if (op == LetterType::_Mul) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Mul, vari, selfvari, value->vari));
				else if (op == LetterType::_Div) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_SDiv, vari, selfvari, value->vari));
				else if (op == LetterType::_Mod) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_SRem, vari, selfvari, value->vari));
				else if (op == LetterType::_EqualEqual) {
					vari->type = createString(thgc, "i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Eq, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_NotEqual) {
					vari->type = createString(thgc, "i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Ne, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_LessThan) {
					vari->type = createString(thgc, "i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLT, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_MoreThan) {
					vari->type = createString(thgc, "i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SGT, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_LessEqual) {
					vari->type = createString(thgc, "i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLE, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_MoreEqual) {
					vari->type = createString(thgc, "i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SGE, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else return ErrorC(thgc, LetterType::OT_Error, self->letter, "値の演算はサポートされていません。");
				Value* ret = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
				ret->cls = ret_cls; ret->vari = vari;
				return (Obj*)ret;
			}
		}
		else if (cls == (Obj*)local->Bool) {
			// C#: empty
		}
		else if (cls == (Obj*)local->StrT) {
			if (vcls == (Obj*)local->Int) {
				if (vcls == (Obj*)local->Short) {
					LVari* v = make_vn(thgc, "i32", "bc");
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
					value->vari = v;
				}
				if (op == LetterType::_Plus) {
					LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
					LVari* nsvari = make_vn(thgc, "%NumberStringType", "ns");
					add_list(thgc, func->comps, (char*)make_lload(thgc, nsvari, make_lvari_c(thgc, "%NumberStringType*", "@NumberString"), false));
					LVari* v0 = make_vn(thgc, "%StringType*", "ns");
					LCall* call0 = make_lcall(thgc, v0, nsvari);
					lcall_add(thgc, call0, thgc4);
					lcall_add(thgc, call0, value->vari);
					add_list(thgc, func->comps, (char*)call0);
					LVari* as2vari = make_vn(thgc, "%AddStringType2*", "as");
					add_list(thgc, func->comps, (char*)make_lload(thgc, as2vari, make_lvari_c(thgc, "%AddStringType2*", "@AddString2"), false));
					LVari* v1 = make_vn(thgc, "%StringType*", "sa");
					LCall* call1 = make_lcall(thgc, v1, as2vari);
					lcall_add(thgc, call1, thgc4);
					lcall_add(thgc, call1, selfvari);
					lcall_add(thgc, call1, v0);
					add_list(thgc, func->comps, (char*)call1);
					Value* ret = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					ret->cls = (Obj*)local->StrT; ret->vari = v1;
					return (Obj*)ret;
				}
				else return ErrorC(thgc, LetterType::OT_Error, self->letter, "値の演算はサポートされていません。");
			}
			else if (vcls == (Obj*)local->StrT) {
				if (op == LetterType::_Plus) {
					LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
					LVari* as2vari = make_vn(thgc, "%AddStringType2*", "as");
					add_list(thgc, func->comps, (char*)make_lload(thgc, as2vari, make_lvari_c(thgc, "%AddStringType2*", "@AddString2"), false));
					LVari* v1 = make_vn(thgc, "%StringType*", "sa");
					LCall* call1 = make_lcall(thgc, v1, as2vari);
					lcall_add(thgc, call1, thgc4);
					lcall_add(thgc, call1, selfvari);
					lcall_add(thgc, call1, value->vari);
					add_list(thgc, func->comps, (char*)call1);
					Value* ret = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					ret->cls = (Obj*)local->StrT; ret->vari = v1;
					return (Obj*)ret;
				}
				else if (op == LetterType::_EqualEqual) {
					LVari* vari = make_vn(thgc, "i1", "cq");
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Eq, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_NotEqual) {
					LVari* vari = make_vn(thgc, "i1", "cq");
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Ne, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_LessThan) {
					LVari* vari = make_vn(thgc, "i1", "cq");
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLT, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_MoreThan) {
					LVari* vari = make_vn(thgc, "i1", "cq");
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SGT, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_LessEqual) {
					LVari* vari = make_vn(thgc, "i1", "cq");
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLE, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_MoreEqual) {
					LVari* vari = make_vn(thgc, "i1", "cq");
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SGE, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else return ErrorC(thgc, LetterType::OT_Error, self->letter, "値の演算はサポートされていません。");
			}
			else return ErrorC(thgc, LetterType::OT_Error, self->letter, "値の演算はサポートされていません。");
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, self->letter, "値の演算は値同士でなければなりません。");
}

// ============================================================
// Block::SortC
// ============================================================
Obj* Block_SortC(ThreadGC* thgc, Obj* self, Function* func, Master* local) {
	Block* blk = (Block*)self;
	// C#: var ftype = new FuncType(local.Bool); ftype.draws.Add((rets[0] as Val).cls); ftype.draws.Add((rets[0] as Val).cls);
	FuncTypeC* ftype = make_functype(thgc, (Obj*)local->Bool);
	Obj* r0cls = ((Val*)*(Obj**)get_list(blk->rets, 0))->cls;
	add_list(thgc, ftype->draws, (char*)r0cls);
	add_list(thgc, ftype->draws, (char*)r0cls);
	// C#: var ret = TypeCheck.CheckCVB(ftype, func, CheckType.Setter, local);
	Obj* ret = CheckCVB_TypeCheck(thgc, (Obj*)ftype, (Obj*)func, CheckType_Setter, local);
	if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
	return (Obj*)blk;
}

// ============================================================
// Block::opeC (dispatcher)
// ============================================================
Obj* Block_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	Block* blk = (Block*)self;
	if (key == LetterType::_Equal || key == LetterType::_Colon) {
		return Block_EqualC(thgc, blk, key, local, val2);
	}
	if (key == LetterType::_In) {
		return Block_InC(thgc, blk, key, local, val2);
	}
	// +, -, *, /, ==, !=, <, >, <=, >=
	return Block_OpeC(thgc, blk, key, local, val2);
}

// ============================================================
// CallBlock::exepC
// ============================================================
Obj* CallBlock_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// CallBlock::exeC
// ============================================================
Obj* CallBlock_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	Block* cblk = (Block*)self;
	// local.llvm = new LLVM(local)
	local->llvm = make_lllvm(thgc, local);
	// local.migrate = new Migrate(); local.migrate.Start();
	local->migrate = make_migrate(thgc);
	Start_migrate(thgc, local->migrate);

	// func = new Func(local.llvm, "void @main", "i32 %cn")
	LVari* mainY = make_lvari_c(thgc, "void", "@main");
	LFunc* mainFunc = make_lfunc(thgc, local->llvm, mainY);
	LVari* cnParam = make_lvari_c(thgc, "i32", "%cn");
	add_list(thgc, mainFunc->draws, (char*)cnParam);
	add_list(thgc, local->llvm->comps, (char*)mainFunc);
	local->llvm->main_func = mainFunc;

	// main.comps.Add(Store(@cnp, %cn))
	add_list(thgc, mainFunc->comps, (char*)make_lstore(thgc, make_lvari_c(thgc, "i32*", "@cnp"), make_lvari_c(thgc, "i32", "%cn")));

	// typedec = new TypeDec("%mainType")
	LTypeDec* typedec = make_ltypedec(thgc, createString(thgc, (char*)"%mainType", 9, 1), NULL, false);
	add_list(thgc, local->llvm->types, (char*)typedec);

	// funccheck = new Func(local.llvm, "void @mainCheck", thgc2, i8p)
	LVari* thgc3 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* i8p = make_lvari_c(thgc, "i8*", "%self");
	LVari* thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LFunc* funccheck = make_lfunc(thgc, local->llvm, make_lvari_c(thgc, "void", "@mainCheck"));
	add_list(thgc, funccheck->draws, (char*)thgc2);
	add_list(thgc, funccheck->draws, (char*)i8p);
	add_list(thgc, local->llvm->comps, (char*)funccheck);

	// typedec.comps.Add(new TypeVal("i8*", "blk"))
	add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i8*", 3, 1), createString(thgc, (char*)"blk", 3, 1)));

	// GC check function body
	LVari* vari = make_vn(thgc, "i8*", "ge");
	LGete* gete = make_lgete_idx2(thgc, "%mainType", vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0"));
	add_list(thgc, funccheck->comps, (char*)gete);
	LVari* co_val = make_lvari_c(thgc, "%CopyObjectType", "%co");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, co_val, make_lvari_c(thgc, "%CopyObjectType*", "@CopyObject"), false));
	LVari* vari2 = make_vn(thgc, "i8*", "ld");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
	LVari* co_cval = make_vn(thgc, "i8*", "cc");
	LCall* co_call = make_lcall(thgc, co_cval, co_val);
	lcall_add(thgc, co_call, thgc2);
	lcall_add(thgc, co_call, vari);
	add_list(thgc, funccheck->comps, (char*)co_call);
	add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));

	// main body: load thgc, addclass
	add_list(thgc, mainFunc->comps, (char*)make_lload(thgc, thgc3, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));
	LVari* ac_val = make_lvari_c(thgc, "%GC_AddClassType", "%addclass");
	add_list(thgc, mainFunc->comps, (char*)make_lload(thgc, ac_val, make_lvari_c(thgc, "%GC_AddClassType*", "@GC_AddClass"), false));
	LVari* countv = make_lvari_c(thgc, "i32", "");
	int cn_val = local->llvm->cn++;
	cblk->n_c = cn_val; // C#: this.n = local.llvm.cn++

	char cnbuf[32]; snprintf(cnbuf, sizeof(cnbuf), "%d", cn_val);
	LVari* typ = make_lvari_c(thgc, "i32", cnbuf);
	LVari* tnp = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tnp%d", llvm_n++));
	add_list(thgc, mainFunc->comps, (char*)make_lload(thgc, tnp, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%cv%d", llvm_n++));
	add_list(thgc, mainFunc->comps, (char*)make_lbinop(thgc, LBOP_Add, tv, tnp, typ));

	LStrV* strv = make_lstrv(thgc, createString(thgc, (char*)"@m", 2, 1), createString(thgc, (char*)"m", 1, 1), 1);
	add_list(thgc, local->llvm->strs, (char*)strv);
	LCall* ac_call = make_lcall(thgc, NULL, ac_val);
	lcall_add(thgc, ac_call, thgc3);
	lcall_add(thgc, ac_call, tv);
	lcall_add(thgc, ac_call, countv);
	lcall_add(thgc, ac_call, (LVari*)strv);
	lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCCheckFuncType", "@mainCheck"));
	lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCFinalizeFuncType", "null"));
	add_list(thgc, mainFunc->comps, (char*)ac_call);

	LFunc* curFunc = llvm_get_func(local->llvm);
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, curFunc->dbinits, (char*)make_lload(thgc, thgc4, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));

	LVari* rngv = make_lvari_c(thgc, "%RootNodeType**", "%rngv");
	LGete* rngGete = make_lgete_idx2(thgc, "%ThreadGCType", rngv, thgc4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1"));
	add_list(thgc, curFunc->dbinits, (char*)rngGete);
	LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	add_list(thgc, curFunc->dbinits, (char*)make_lload(thgc, rn4, rngv, false));

	// GC_malloc
	LVari* go_val = make_lvari_c(thgc, "%GC_mallocType", "%gcobject");
	add_list(thgc, curFunc->comps, (char*)make_lload(thgc, go_val, make_lvari_c(thgc, "%GC_mallocType*", "@GC_malloc"), false));
	LVari* go_v = make_lvari_c(thgc, "%GCObjectPtr", "%obj");
	LVari* tmp = make_lvari_c(thgc, "i32", cnbuf);
	LVari* tmi = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmi%d", llvm_n++));
	add_list(thgc, curFunc->comps, (char*)make_lload(thgc, tmi, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tmv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmv%d", llvm_n++));
	add_list(thgc, curFunc->comps, (char*)make_lbinop(thgc, LBOP_Add, tmv, tmi, tmp));
	LCall* go_call = make_lcall(thgc, go_v, go_val);
	lcall_add(thgc, go_call, thgc4);
	lcall_add(thgc, go_call, tmv);
	add_list(thgc, curFunc->comps, (char*)go_call);
	LVari* go_c = make_vn(thgc, "%mainType*", "gc");
	add_list(thgc, curFunc->comps, (char*)make_llocmarker(thgc, 1, 0, curFunc->debugId));
	LCast* go_cast = make_lcast(thgc, LCAST_Bitcast, go_c, go_v);

	// Debug variable for mainObj (C# lines 660-674)
	int structTypeId = ldbg_get_or_create_type(thgc, local->llvm->debugInfo, createString(thgc, (char*)"%mainType", 9, 1));
	int ptrTypeId = ldbg_get_pointer_type(thgc, local->llvm->debugInfo, structTypeId);
	int varId = local->llvm->debugInfo->metaId++;
	LDebugVariable* debugVar = (LDebugVariable*)GC_alloc(thgc, _LDebugVariable);
	debugVar->name = createString(thgc, (char*)"mainObj", 7, 1);
	debugVar->typeId = ptrTypeId;
	debugVar->scopeId = curFunc->debugId;
	debugVar->line = 1;
	debugVar->metaId = varId;
	debugVar->argNo = 0;
	add_list(thgc, curFunc->debugVariables, (char*)debugVar);
	go_cast->debugVarId = varId;

	add_list(thgc, curFunc->comps, (char*)go_cast);

	// Alloca + Store
	LVari* objptr = make_lvari_c(thgc, "%mainType**", "%objptr");
	add_list(thgc, curFunc->comps, (char*)make_lalloca(thgc, objptr));
	add_list(thgc, curFunc->comps, (char*)make_lstore(thgc, objptr, go_c));

	// GC_SetRoot
	LVari* srv = make_vn(thgc, "%GC_SetRootType", "sr");
	add_list(thgc, curFunc->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
	LCall* srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, rn4);
	lcall_add(thgc, srcall, objptr);
	add_list(thgc, curFunc->comps, (char*)srcall);

	// Process vmapA of second child (block)
	int order = 1;
	int count = 8;
	Block* blk = (Block*)*(Obj**)get_list(cblk->children, 1);

	// Set model on the block's obj
	if (blk->obj) {
		((Obj*)blk->obj)->model = createString(thgc, (char*)"%mainType", 9, 1);
		((Obj*)blk->obj)->bas = objptr;
	}

	set_blocks(thgc, local, create_list(thgc, sizeof(Block*), CType::_Block));
	List* blocks = get_blocks(local);
	add_list(thgc, blocks, (char*)blk);
	List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);

	// Iterate vmapA
	for (int ki = 0; ki < blk->vmapA->kvs->size; ki++) {
		KV* kv = *(KV**)get_list(blk->vmapA->kvs, ki);
		Obj* v = (Obj*)kv->value;
		if (v->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)v;
			variable->type = LetterType::OT_Variable;
			v->order = order++;
			// Resolve Var -> cls
			Obj* vcls = variable->cls;
			if (vcls && vcls->type == LetterType::OT_Var) vcls = ((Type*)vcls)->cls;

			if (vcls == (Obj*)local->Int) {
				// Int型: typedecに"i32"を登録、GC check不要
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i32", 3, 1), kv->key));
				count += 8;
			}
			else if (vcls == (Obj*)local->Bool) {
				// Bool型: typedecに"i1"を登録、GC check不要
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i1", 2, 1), kv->key));
				count += 8;
			}
			else {
				// オブジェクト型: typedecにmodel+"*"を登録、GC checkあり
				if (vcls) SetIdentity(thgc, (Obj*)vcls);
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, model_type(thgc, vcls ? vcls->model : NULL, "*"), kv->key));
				count += 8;

				// GC check for object types
				vari = make_vn(thgc, "i8*", "ge");
				char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
				gete = make_lgete_idx2(thgc, "%mainType", vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
				add_list(thgc, funccheck->comps, (char*)gete);
				vari2 = make_vn(thgc, "i8*", "ld");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				co_cval = make_vn(thgc, "i8*", "cc");
				co_call = make_lcall(thgc, co_cval, co_val);
				lcall_add(thgc, co_call, thgc2);
				lcall_add(thgc, co_call, vari);
				add_list(thgc, funccheck->comps, (char*)co_call);
				add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			}
		}
		else if (v->type == LetterType::OT_Function) {
			Function* f = (Function*)v;
			SetIdentity(thgc, (Obj*)(f));
			v->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));

			// GC check
			vari = make_vn(thgc, "i8*", "ge");
			char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
			gete = make_lgete_idx2(thgc, "%mainType", vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
			add_list(thgc, funccheck->comps, (char*)gete);
			vari2 = make_vn(thgc, "i8*", "ld");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "cc");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			// Function init call
			LVari* va0 = make_vn(thgc, "%FuncType*", "fa");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", f->letter ? f->letter->text->data : "f", f->identity)));
			lcall_add(thgc, fu_call, rn4);
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->call_c));
			add_list(thgc, curFunc->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, "%mainType*", "ml");
			add_list(thgc, curFunc->comps, (char*)make_lload(thgc, va2, objptr, false));
			LVari* va = make_vn(thgc, "%FuncType*", "fg");
			gete = make_lgete_idx2(thgc, "%mainType", va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
			add_list(thgc, curFunc->comps, (char*)gete);
			add_list(thgc, curFunc->comps, (char*)make_lstore(thgc, va, va0));
		}
		else if (v->type == LetterType::OT_ClassObj) {
			ClassObj* f = (ClassObj*)v;
			SetIdentity(thgc, (Obj*)(f));
			v->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));

			// GC check
			vari = make_vn(thgc, "i8*", "ge");
			char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
			gete = make_lgete_idx2(thgc, "%mainType", vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
			add_list(thgc, funccheck->comps, (char*)gete);
			vari2 = make_vn(thgc, "i8*", "ld");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "cc");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			// ClassObj init call
			LVari* va0 = make_vn(thgc, "%FuncType*", "fa");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", ((Obj*)f)->letter ? ((Obj*)f)->letter->text->data : "c", f->identity)));
			lcall_add(thgc, fu_call, rn4);
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->call_c));
			add_list(thgc, curFunc->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, "%mainType*", "ml");
			add_list(thgc, curFunc->comps, (char*)make_lload(thgc, va2, objptr, false));
			LVari* va = make_vn(thgc, "%FuncType*", "fg");
			gete = make_lgete_idx2(thgc, "%mainType", va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
			add_list(thgc, curFunc->comps, (char*)gete);
			add_list(thgc, curFunc->comps, (char*)make_lstore(thgc, va, va0));
		}
		else if (v->type == LetterType::OT_ModelObj) {
			ModelObj* f = (ModelObj*)v;
			if (f->initial) continue;
			SetIdentity(thgc, (Obj*)(f));
			v->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));

			// GC check
			vari = make_vn(thgc, "i8*", "ge");
			char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
			gete = make_lgete_idx2(thgc, "%mainType", vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
			add_list(thgc, funccheck->comps, (char*)gete);
			vari2 = make_vn(thgc, "i8*", "ld");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "cc");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			// ModelObj init call
			LVari* va0 = make_vn(thgc, "%FuncType*", "fa");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", ((Obj*)f)->letter ? ((Obj*)f)->letter->text->data : "m", f->identity)));
			lcall_add(thgc, fu_call, rn4);
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->call_c));
			add_list(thgc, curFunc->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, "%mainType*", "ml");
			add_list(thgc, curFunc->comps, (char*)make_lload(thgc, va2, objptr, false));
			LVari* va = make_vn(thgc, "%FuncType*", "fg");
			gete = make_lgete_idx2(thgc, "%mainType", va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
			add_list(thgc, curFunc->comps, (char*)gete);
			add_list(thgc, curFunc->comps, (char*)make_lstore(thgc, va, va0));
		}
	}

	// countv.name = count
	char countbuf[32]; snprintf(countbuf, sizeof(countbuf), "%d", count);
	countv->name = createString(thgc, countbuf, (int)strlen(countbuf), 1);
	add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

	// C#: for (i=0; i<decs.Count; i++) decs[i].Dec(local);
	for (int di = 0; di < decs->size; di++) {
		Obj* dec = *(Obj**)get_list(decs, di);
		Dec_vt(thgc, dec, local);
	}

	// C#: blk.obj.obj.bas = objptr;
	if (blk->obj) blk->obj->bas = objptr;

	// C#: local.labs.Add(new Lab("entry"));
	LLab* lab_entry = make_llab_c(thgc, "entry");
	add_list(thgc, local->labs, (char*)lab_entry);

	// C#: var obj = blk.exeC(local) as Block;
	Obj* obj = vt_exeC(thgc, (Obj*)blk, local);

	// C#: local.labs.RemoveAt(local.labs.Count - 1);
	local->labs->size--;

	// Epilogue
	LVari* rnpv = make_lvari_c(thgc, "i32*", "%ptr");
	add_list(thgc, curFunc->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* rnpv12 = make_lvari_c(thgc, "i32", "%v");
	add_list(thgc, curFunc->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
	LVari* rnpv2 = make_lvari_c(thgc, "i32", "%dec");
	add_list(thgc, curFunc->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, curFunc->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

	// C#: if (obj.rets.Count == 0) Ret(i32, "0"); else GetterC→Value check→Bitcast.Cast→Ret
	if (blk->rets == NULL || blk->rets->size == 0) {
		add_list(thgc, curFunc->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "i32", "0")));
	} else {
		Obj* valobj = GetterC_vt(thgc, *(Obj**)get_list(blk->rets, blk->rets->size - 1), local);
		if (valobj->type == LetterType::OT_Value) {
			Val* value = (Val*)valobj;
			if (value->cls == (Obj*)local->Int || value->cls == (Obj*)local->Short || value->cls == (Obj*)local->Bool) {
				LVari* va = BitcastCast(thgc, local, (Obj*)local->Int, value->cls, ((Value*)valobj)->vari);
				add_list(thgc, curFunc->comps, (char*)make_lret(thgc, va));
			} else {
				return ErrorA(thgc, LetterType::OT_Error, valobj->letter, "最後の値は整数型でなければなりません");
			}
		} else {
			return ErrorA(thgc, LetterType::OT_Error, valobj->letter, "最後の値はValue型でなければなりません");
		}
	}

	// C#: local.blocks is property → blockslist.Last()
	// local.blocks.RemoveAt(local.blocks.Count - 1)
	List* lastblocks = *(List**)get_list(local->blockslist, local->blockslist->size - 1);
	lastblocks->size--;
	// local.blockslist.RemoveAt(local.blockslist.Count - 1)
	local->blockslist->size--;
	// C#: func.comps.Add(new Ret(new Vari("void", null)))
	add_list(thgc, mainFunc->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

	// DB init strings
	LVari* csvari = make_vn(thgc, "%CreateStringType", "cs");
	add_list(thgc, curFunc->dbinits, (char*)make_lload(thgc, csvari, make_lvari_c(thgc, "%CreateStringType*", "@CreateString"), false));

	// C#: local.migrate.migrations.Last().fulltext.Replace("\n", "\\0A")
	OneMigrate* lastmig = *(OneMigrate**)get_list(local->migrate->migrations, local->migrate->migrations->size - 1);
	String* ftesc = StringReplace(thgc, lastmig->fulltext, createString(thgc, "\n", 1, 1), createString(thgc, "\\0A", 3, 1));
	int len1 = lastmig->fulltext->size; // original length for LLVM
	LStrV* sd1 = make_lstrv(thgc, createString(thgc, (char*)"@dbtex", 6, 1), ftesc, len1);
	add_list(thgc, local->llvm->strs, (char*)sd1);
	LVari* vd1 = make_vn(thgc, "i8*", "dt");
	char gete1type[32];
	snprintf(gete1type, sizeof(gete1type), "[%d x i8]", len1 + 1);
	add_list(thgc, curFunc->dbinits, (char*)make_lgete_idx2(thgc, gete1type, vd1, (LVari*)sd1, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* vdd1 = make_vn(thgc, "%StringType*", "df");
	LCall* cscall1 = make_lcall(thgc, vdd1, csvari);
	lcall_add(thgc, cscall1, thgc4);
	lcall_add(thgc, cscall1, vd1);
	char len1str[16];
	snprintf(len1str, sizeof(len1str), "%d", len1);
	lcall_add(thgc, cscall1, make_lvari_c(thgc, "i32", len1str));
	lcall_add(thgc, cscall1, make_lvari_c(thgc, "i32", "1"));
	add_list(thgc, curFunc->dbinits, (char*)cscall1);

	int len2 = 2; // "db"
	LStrV* sd2 = make_lstrv(thgc, createString(thgc, (char*)"@db", 3, 1), createString(thgc, (char*)"db", 2, 1), len2);
	add_list(thgc, local->llvm->strs, (char*)sd2);
	LVari* vd2 = make_vn(thgc, "i8*", "db");
	add_list(thgc, curFunc->dbinits, (char*)make_lgete_idx2(thgc, "[3 x i8]", vd2, (LVari*)sd2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* vdd2 = make_vn(thgc, "%StringType*", "ds");
	LCall* cscall = make_lcall(thgc, vdd2, csvari);
	lcall_add(thgc, cscall, thgc4);
	lcall_add(thgc, cscall, vd2);
	lcall_add(thgc, cscall, make_lvari_c(thgc, "i32", "2"));
	lcall_add(thgc, cscall, make_lvari_c(thgc, "i32", "1"));
	add_list(thgc, curFunc->dbinits, (char*)cscall);

	LVari* dbinit = make_vn(thgc, "%DbInitType", "di");
	add_list(thgc, curFunc->dbinits, (char*)make_lload(thgc, dbinit, make_lvari_c(thgc, "%DbInitType*", "@DbInit"), false));
	LVari* sqlp = make_lvari_c(thgc, "ptr", "%sqlp");
	LCall* dbcall = make_lcall(thgc, sqlp, dbinit);
	lcall_add(thgc, dbcall, thgc4);
	lcall_add(thgc, dbcall, vdd2);
	lcall_add(thgc, dbcall, make_lvari_c(thgc, "i32", "0"));
	lcall_add(thgc, dbcall, make_lvari_c(thgc, "i32", "0"));
	lcall_add(thgc, dbcall, make_lvari_c(thgc, "i32", "0"));
	lcall_add(thgc, dbcall, vdd1);
	add_list(thgc, curFunc->dbinits, (char*)dbcall);
	add_list(thgc, curFunc->dbinits, (char*)make_lstore(thgc, make_lvari_c(thgc, "ptr", "@sqlp"), sqlp));

	return self;
}

// ============================================================
// declareC
// ============================================================
Obj* declareC(ThreadGC* thgc, Master* local, String* name) {
	return getC(thgc, local, name);
}

// ============================================================
// getC - variable lookup with LLVM getelementptr chain
// ============================================================
Obj* getC(ThreadGC* thgc, Master* local, String* name) {
	List* blocks = get_blocks(local);
	int count = 0;
	bool outside = false;
	LVari* vari = NULL;
	Block* oldblk = NULL;
	local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
	LFunc* func = llvm_get_func(local->llvm);

	for (int i = 0; i < blocks->size; i++) {
		LComp* loaded = NULL;
		Block* blk = *(Block**)get_list(blocks, blocks->size - i - 1);
		Obj* blkobj = (Obj*)blk->obj;

		if (blkobj->type == LetterType::OT_Comment) {
			if (func->async_flag) {
				LVari* obj = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
				LVari* ogv = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
				add_list(thgc, local->compsC, (char*)make_lgete_idx2(thgc, "%CoroFrameType", ogv, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
				vari = make_vn(thgc, "%GCObjectPtr", "gp");
				add_list(thgc, local->compsC, (char*)make_lload(thgc, vari, ogv, false));
				outside = true;
			} else {
				vari = make_vn(thgc, "i8*", "ge");
				add_list(thgc, local->compsC, (char*)make_lload(thgc, vari, blkobj->bas, false));
				outside = true;
			}
		}
		else if (i == 0 || ((ObjBlock*)blk->obj)->n == 1 || ((ObjBlock*)blk->obj)->n == -1) {
			if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For || blkobj->type == LetterType::OT_Model) {
				if (outside) {
					LVari* newvari = make_vn(thgc, "i8**", "np");
					add_list(thgc, local->compsC, (char*)make_lgete_idx2_s(thgc, ((Obj*)oldblk->obj)->model, newvari, vari, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
					LVari* newvari2 = make_vn(thgc, "i8*", "ld");
					add_list(thgc, local->compsC, (char*)make_lload(thgc, newvari2, newvari, false));
					vari = newvari2;
				} else {
					if (func->async_flag) {
						LVari* obj = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
						LVari* ogv = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
						add_list(thgc, local->compsC, (char*)make_lgete_idx2(thgc, "%CoroFrameType", ogv, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
						vari = make_vn(thgc, "%GCObjectPtr", "gp");
						add_list(thgc, local->compsC, (char*)make_lload(thgc, vari, ogv, false));
						outside = true;
					} else {
						vari = make_vn(thgc, "i8*", "ge");
						loaded = (LComp*)make_lload(thgc, vari, blkobj->bas, false);
					}
				}
			}
			else if (blkobj->type == LetterType::OT_IfBlock) {
				if (outside) {
					LVari* newvari = make_vn(thgc, "i8**", "np");
					add_list(thgc, local->compsC, (char*)make_lgete_idx2_s(thgc, ((Obj*)oldblk->obj)->model, newvari, vari, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
					LVari* newvari2 = make_vn(thgc, "i8*", "ld");
					add_list(thgc, local->compsC, (char*)make_lload(thgc, newvari2, newvari, false));
					vari = newvari2;
				} else {
					if (func->async_flag) {
						LVari* obj = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
						LVari* ogv = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
						add_list(thgc, local->compsC, (char*)make_lgete_idx2(thgc, "%CoroFrameType", ogv, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
						vari = make_vn(thgc, "%GCObjectPtr", "gp");
						add_list(thgc, local->compsC, (char*)make_lload(thgc, vari, ogv, false));
						outside = true;
					} else {
						vari = make_vn(thgc, "i8*", "ge");
						loaded = (LComp*)make_lload(thgc, vari, ((IfBlock*)blkobj)->iflabel->bas, false);
					}
				}
			}
			else if (blkobj->type == LetterType::OT_None || blkobj->type == LetterType::OT_Function
				|| blkobj->type == LetterType::OT_ClassObj || blkobj->type == LetterType::OT_ModelObj
				|| blkobj->type == LetterType::OT_ServerFunction || blkobj->type == LetterType::OT_SignalFunction) {
				if (outside) {
					LVari* newvari = make_vn(thgc, "i8**", "np");
					add_list(thgc, local->compsC, (char*)make_lgete_idx2_s(thgc, ((Obj*)oldblk->obj)->model, newvari, vari, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
					LVari* newvari2 = make_vn(thgc, "i8*", "ld");
					add_list(thgc, local->compsC, (char*)make_lload(thgc, newvari2, newvari, false));
					vari = newvari2;
				} else {
					if (func->async_flag) {
						LVari* obj = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
						LVari* ogv = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
						add_list(thgc, local->compsC, (char*)make_lgete_idx2(thgc, "%CoroFrameType", ogv, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
						vari = make_vn(thgc, "%GCObjectPtr", "gp");
						add_list(thgc, local->compsC, (char*)make_lload(thgc, vari, ogv, false));
						outside = true;
					} else {
						vari = make_vn(thgc, "i8*", "ge");
						add_list(thgc, local->compsC, (char*)make_lload(thgc, vari, blkobj->bas, false));
					}
				}
				outside = true;
			}
		}
		else {
			if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For) {
				if (!outside) {
					vari = make_vn(thgc, "i8*", "ge");
					loaded = (LComp*)make_lload(thgc, vari, blkobj->bas, false);
				}
			}
			else if (blkobj->type == LetterType::OT_IfBlock) {
				if (!outside) {
					vari = make_vn(thgc, "i8*", "ge");
					loaded = (LComp*)make_lload(thgc, vari, ((IfBlock*)blkobj)->iflabel->bas, false);
				}
			}
		}

		// C#: if (blk.vmapA.ContainsKey(name))
		char* found = get_mapy(blk->vmapA, name);
		if (found != NULL) {
			// C#: TagBlock search loop
			if (blkobj->type == LetterType::OT_TagBlock) {
				for (int j = i + 1; j < blocks->size; j++) {
					Block* b = *(Block**)get_list(blocks, blocks->size - j - 1);
					Obj* bobj = (Obj*)b->obj;
					if (bobj->type == LetterType::OT_TagBlock) {
						continue;
					} else if (bobj->type == LetterType::OT_IfBlock) {
						vari = make_vn(thgc, "i8*", "ge");
						loaded = (LComp*)make_lload(thgc, vari, ((IfBlock*)blkobj)->iflabel->bas, false);
						break;
					} else {
						vari = make_vn(thgc, "i8*", "ge");
						loaded = (LComp*)make_lload(thgc, vari, blkobj->bas, false);
						break;
					}
				}
			}
			Obj* obj = (Obj*)found;
			if (loaded != NULL) add_list(thgc, local->compsC, (char*)loaded);
			LVari* newvari = make_vn(thgc, "i8*", "np");
			char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", obj->order);
			add_list(thgc, local->compsC, (char*)make_lgete_idx2_s(thgc, blkobj->model, newvari, vari, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
			func->vari = newvari;

			if (obj->type == LetterType::OT_Variable) {
				return SelfC_vt(thgc, obj, local);
			}
			return obj;
		}
		oldblk = blk;
		count++;
	}

	// C#: if (local.sigmapA.ContainsKey(name)) return local.sigmapA[name]
	char* sig = get_mapy(local->sigmapA, name);
	if (sig != NULL) return (Obj*)sig;

	return ErrorC(thgc, LetterType::OT_Error, NULL, "variable not found");
}
