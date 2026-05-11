// ============================================================
// obj2c.h - C隗｣譫舌ヱ繧ｹ (LLVM IR code generation) - Obj2C.cs converted
// ============================================================

// ============================================================
// 蜑肴婿螳｣險
// ============================================================
Obj* Word_SelfC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Word_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Word_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Word_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Value_OpeC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Value_ope(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
Obj* Value_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Class_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Class_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
void ClassObj_Dec(ThreadGC* thgc, Obj* self, Master* local);
Obj* Type_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Var_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Generic_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Number_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Number_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Number2_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Number_PlusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MinusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MulC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_DivC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MoreThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_LessThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_MoreEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_LessEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_NotEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* Number_EqualEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* FloatVal_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* FloatVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* FloatVal_PlusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MinusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MulC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_DivC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MoreThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_LessThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_MoreEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_LessEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_NotEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* FloatVal_EqualEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2);
Obj* StrObj_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* StrObj_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* StrObj_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* StrObj_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* BoolVal2_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* BoolVal2_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* BoolVal2_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* VoiVal_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* VoiVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Null_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Null_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Null_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* ArrayConstructor_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Constructor_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* SqlString_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* SqlString_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2);
void SqlString_Format(ThreadGC* thgc, Obj* self);

// ============================================================
// Word::SelfC
// ============================================================
Obj* Word_SelfC(ThreadGC* thgc, Obj* self, Master* local) {
	Obj* obj = SelfC_vt(thgc, getC(thgc, local, self->letter->text), local);
	obj->letter = self->letter;
	return obj;
}

// ============================================================
// Word::GetterC
// ============================================================
Obj* Word_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	Obj* obj = GetterC_vt(thgc, SelfC_vt(thgc, getC(thgc, local, self->letter->text), local), local);
	obj->letter = self->letter;
	return obj;
}

// ============================================================
// Word::exepC
// ============================================================
Obj* Word_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		// local.calls.Last()();
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Word::PrimaryC
// ============================================================
Obj* Word_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	(*n)++;
	return self;
}

// ============================================================
// Value::OpeC (defined in obj1c.h)
// ============================================================
#if 0
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

		if (cls == local->Int || cls == local->Short) {
			const char* sizetype = "i32";
			if (cls == local->Short) sizetype = "i16";

			if (vcls == local->StrT) {
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
				else {
					return ErrorC(thgc, LetterType::OT_Error, self->letter, "値の演算はサポートされていません。");
				}
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
					vari->type = createString(thgc, (char*)"i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Eq, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_NotEqual) {
					vari->type = createString(thgc, (char*)"i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Ne, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_LessThan) {
					vari->type = createString(thgc, (char*)"i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLT, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_MoreThan) {
					vari->type = createString(thgc, (char*)"i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SGT, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_LessEqual) {
					vari->type = createString(thgc, (char*)"i1", 2, 1);
					add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLE, vari, selfvari, value->vari));
					Value* r = (Value*)make_val(thgc, CType::_CVal, LetterType::OT_Value, self->letter);
					r->cls = (Obj*)local->Bool; r->vari = vari;
					return (Obj*)r;
				}
				else if (op == LetterType::_MoreEqual) {
					vari->type = createString(thgc, (char*)"i1", 2, 1);
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
		else if (cls == local->Bool) {
			// empty - same as C#
		}
		else if (cls == local->StrT) {
			if (vcls == (Obj*)local->Int || vcls == (Obj*)local->Short) {
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
#endif

// ============================================================
// Value::ope
// ============================================================
Obj* Value_ope(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	LFunc* func = llvm_get_func(local->llvm);
	Obj* cls = ((Value*)self)->cls;
	LVari* vari = NULL;
	LVari* vari2 = NULL;
	if (cls == local->Int) {
		vari = make_vn(thgc, "i32*", "va");
	}
	else if (cls == local->Bool) {
		vari = make_vn(thgc, "i2*", "va");
	}
	add_list(thgc, func->comps, (char*)make_lload(thgc, vari, ((Value*)self)->vari, false));
	Value* val = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, self->letter);
	val->cls = cls; val->vari = vari2;
	if (key == LetterType::_Plus) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Add, vari2, vari, ((Value*)val2)->vari));
	else if (key == LetterType::_Minus) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, vari2, vari, ((Value*)val2)->vari));
	else if (key == LetterType::_Mul) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Mul, vari2, vari, ((Value*)val2)->vari));
	else if (key == LetterType::_Div) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_SDiv, vari2, vari, ((Value*)val2)->vari));
	else if (key == LetterType::_Mod) add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_SRem, vari2, vari, ((Value*)val2)->vari));
	else if (key == LetterType::_EqualEqual) {
		vari2->type = createString(thgc, (char*)"i1", 2, 1);
		add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Eq, vari2, vari, ((Value*)val2)->vari));
	}
	else if (key == LetterType::_NotEqual) {
		vari2->type = createString(thgc, (char*)"i1", 2, 1);
		add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Ne, vari2, vari, ((Value*)val2)->vari));
	}
	else if (key == LetterType::_MoreThan) {
		vari2->type = createString(thgc, (char*)"i1", 2, 1);
		add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SGT, vari2, vari, ((Value*)val2)->vari));
	}
	else if (key == LetterType::_LessThan) {
		vari2->type = createString(thgc, (char*)"i1", 2, 1);
		add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLT, vari2, vari, ((Value*)val2)->vari));
	}
	else if (key == LetterType::_MoreEqual) {
		vari2->type = createString(thgc, (char*)"i1", 2, 1);
		add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SGE, vari2, vari, ((Value*)val2)->vari));
	}
	else if (key == LetterType::_LessEqual) {
		vari2->type = createString(thgc, (char*)"i1", 2, 1);
		add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_SLE, vari2, vari, ((Value*)val2)->vari));
	}
	return (Obj*)val;
}

// ============================================================
// Value::PrimaryC
// ============================================================
Obj* Value_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	LFunc* func = llvm_get_func(local->llvm);
	Obj* cls = ((Value*)self)->cls;

	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			(*n)++;
			String* name = val2->letter->text;
			Block* blk0 = (Block*)*(Obj**)get_list(((ClassObj*)cls)->draw->children, 0);
			Block* blk1 = (Block*)*(Obj**)get_list(((ClassObj*)cls)->draw->children, 1);
			Map* vmap0 = blk0->vmapA;
			Map* vmap1 = blk1->vmapA;
			char* found0 = get_mapy(vmap0, name);
			if (found0 != NULL) {
				Obj* v = (Obj*)found0;
				if (v->type == LetterType::OT_Variable) {
					Variable* variable = (Variable*)v;
					LVari* vari;
					Obj* vcls = variable->cls;
					if (vcls->type == LetterType::OT_Var) vcls = ((Type*)vcls)->cls;
					variable->cls = vcls;
					if (vcls == local->Int) vari = make_vn(thgc, "i32*", "va");
					else if (vcls == local->Bool) vari = make_vn(thgc, "i2*", "va");
					else vari = make_vn_s(thgc, vcls->model, "va");
					char orderbuf[16]; snprintf(orderbuf, sizeof(orderbuf), "%d", variable->order);
					LGete* gete = make_lgete_idx2_s(thgc, cls->model, vari, ((Value*)self)->vari,
						make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
					add_list(thgc, func->comps, (char*)gete);
					Variable* newvar = (Variable*)make_variable(thgc, variable->letter);
					newvar->cls = vcls;
					newvar->vari_c = vari;
					return (Obj*)newvar;
				}
				else {
					char* found1 = get_mapy(vmap1, name);
					if (found1 != NULL && ((Obj*)found1)->type == LetterType::OT_Function) {
						Function* f = (Function*)found1;
						LVari* vari;
						Obj* fret = f->rettype;
						if (fret->type == LetterType::OT_Var) fret = ((Type*)fret)->cls;
						f->rettype = fret;
						if (fret == local->Int) vari = make_vn(thgc, "i32*", "va");
						else if (fret == local->Bool) vari = make_vn(thgc, "i2*", "va");
						else vari = make_vn_s(thgc, fret->model, "va");
						char orderbuf[16]; snprintf(orderbuf, sizeof(orderbuf), "%d", f->order);
						LGete* gete = make_lgete_idx2_s(thgc, cls->model, vari, ((Value*)self)->vari,
							make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
						add_list(thgc, func->comps, (char*)gete);
						func->vari = vari;
						return (Obj*)f;
					}
				}
			}
			else {
				char* found1 = get_mapy(vmap1, name);
				if (found1 != NULL) {
					Obj* v = (Obj*)found1;
					if (v->type == LetterType::OT_Variable) {
						Variable* variable = (Variable*)v;
						LVari* vari;
						Obj* vcls = variable->cls;
						if (vcls->type == LetterType::OT_Var) vcls = ((Type*)vcls)->cls;
						variable->cls = vcls;
						if (vcls == local->Int) vari = make_vn(thgc, "i32*", "va");
						else if (vcls == local->Bool) vari = make_vn(thgc, "i2*", "va");
						else vari = make_vn_s(thgc, vcls->model, "va");
						char orderbuf[16]; snprintf(orderbuf, sizeof(orderbuf), "%d", variable->order);
						LGete* gete = make_lgete_idx2_s(thgc, cls->model, vari, ((Value*)self)->vari,
							make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
						add_list(thgc, func->comps, (char*)gete);
						Variable* newvar = (Variable*)make_variable(thgc, variable->letter);
						newvar->cls = vcls;
						newvar->vari_c = vari;
						return (Obj*)newvar;
					}
					else if (v->type == LetterType::OT_Function) {
						Function* f = (Function*)v;
						LVari* vari;
						Obj* fret = f->rettype;
						if (fret->type == LetterType::OT_Var) fret = ((Type*)fret)->cls;
						f->rettype = fret;
						if (fret == local->Int) vari = make_vn(thgc, "i32*", "va");
						else if (fret == local->Bool) vari = make_vn(thgc, "i2*", "va");
						else vari = make_vn_s(thgc, fret->model, "va");
						char orderbuf[16]; snprintf(orderbuf, sizeof(orderbuf), "%d", f->order);
						LGete* gete = make_lgete_idx2_s(thgc, cls->model, vari, ((Value*)self)->vari,
							make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf));
						add_list(thgc, func->comps, (char*)gete);
						func->vari = vari;
						return (Obj*)f;
					}
				}
			}
		}
	}
	else if (val2->type == LetterType::OT_Block) {
		(*n)++;
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		Block* blk = (Block*)val;
		if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
		((Value*)self)->cls = cls;
		if (cls == local->StrT) {
			((Value*)self)->vari->type = createString(thgc, (char*)"s[n]", 4, 1);
			Obj* bval = *(Obj**)get_list(blk->rets, 0);
			if (bval->type == LetterType::OT_Value && ((Value*)bval)->cls == local->Int) {
				Variable* variable = (Variable*)make_variable(thgc, self->letter);
				variable->cls = (Obj*)local->Short;
				variable->vari_c = ((Value*)self)->vari;
				variable->forvari = ((Value*)bval)->vari;
				return (Obj*)variable;
			}
			else return ErrorC(thgc, LetterType::OT_Error, self->letter, "変数のブロックは整数型をとるしかありません。");
		}
		if (blk->rets->size > 0) {
			Obj* bval = *(Obj**)get_list(blk->rets, 0);
			if (bval->type == LetterType::OT_Value) {
				if (((Value*)bval)->cls == local->Int) {
					LVari* v0 = make_vn(thgc, "%ListType**", "v0");
					add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%HashType", v0, ((Value*)self)->vari,
						make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
					LVari* v05 = make_vn(thgc, "%ListType*", "v0");
					add_list(thgc, func->comps, (char*)make_lload(thgc, v05, v0, false));
					LVari* varigl = make_vn(thgc, "%GetListType", "va");
					add_list(thgc, func->comps, (char*)make_lload(thgc, varigl, make_lvari_c(thgc, "%GetListType*", "@GetList"), false));
					LVari* v1 = make_vn(thgc, "%KeyValueType**", "v1");
					LCall* call1 = make_lcall(thgc, v1, varigl);
					lcall_add(thgc, call1, v05);
					lcall_add(thgc, call1, ((Value*)bval)->vari);
					add_list(thgc, func->comps, (char*)call1);
					LVari* v15 = make_vn(thgc, "%KeyValueType*", "v1");
					add_list(thgc, func->comps, (char*)make_lload(thgc, v15, v1, false));
					LVari* v2 = make_vn(thgc, "[n]", "v2");
					add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%KeyValueType", v2, v15,
						make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
					((Value*)self)->vari = v2;
					if (cls->type == LetterType::OT_ArrType) {
						ArrType* arrtype = (ArrType*)cls;
						Variable* variable = (Variable*)make_variable(thgc, self->letter);
						variable->cls = (Obj*)((Type*)arrtype)->cls;
						variable->vari_c = ((Value*)self)->vari;
						return (Obj*)variable;
					}
					else return ErrorC(thgc, LetterType::OT_Error, self->letter, "ブロックは配列型をとるしかありません。");
				}
				else if (((Value*)bval)->cls == local->StrT) {
					((Value*)self)->vari->type = createString(thgc, (char*)"[s]", 3, 1);
					if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
					((Value*)self)->cls = cls;
					if (cls->type == LetterType::OT_ArrType) {
						ArrType* arrtype = (ArrType*)cls;
						Variable* variable = (Variable*)make_variable(thgc, self->letter);
						variable->cls = (Obj*)((Type*)arrtype)->cls;
						variable->vari_c = ((Value*)self)->vari;
						variable->forvari = ((Value*)bval)->vari;
						return (Obj*)variable;
					}
					else return ErrorC(thgc, LetterType::OT_Error, self->letter, "ブロックは配列型をとるしかありません。");
				}
				else return ErrorC(thgc, LetterType::OT_Error, blk->letter, "配列の引数は整数か文字列だけです。");
			}
		}
		return ErrorC(thgc, LetterType::OT_Error, blk->letter, "配列の引数は整数か文字列だけです。");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "値の後続が適切ではないです。");
}

// ============================================================
// Class::exepC
// ============================================================
Obj* Class_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Class::PrimaryC
// ============================================================
Obj* Class_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	if (val2->type == LetterType::OT_Word) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Block) {
			Obj* block = vt_exeC(thgc, val2, local);
			Block* blk = (Block*)block;
			add_list(thgc, blocks, (char*)blk);
			for (int i = 0; i < blk->rets->size; i++) {
				val2 = *(Obj**)get_list(blk->rets, i);
			}
			remove_list(thgc, blocks, blocks->size - 1);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
		}
	}
head:
	if (val2->type == LetterType::_Left) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Bracket) {
			Obj* val = GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(primary->children, *n), local), local);
			if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
			Block* blk = (Block*)val2;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* ri = *(Obj**)get_list(blk->rets, i);
				if (ri->type == LetterType::OT_ClassObj || ri->type == LetterType::OT_GeneObj) {
					goto head;
				}
			else return ErrorC(thgc, LetterType::OT_Error, blk->letter, "クラスの継承先はクラスでなければなりません");
			}
		}
		else {
			val2 = GetterC_vt(thgc, val2, local);
			if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
			if (val2->type == LetterType::OT_ClassObj) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head;
			}
			else if (val2->type == LetterType::OT_Generic) {
				(*n)++;
				if (val2->type == LetterType::OT_Block) {
					val2 = GetterC_vt(thgc, vt_exeC(thgc, *(Obj**)get_list(primary->children, *n), local), local);
					if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
					Block* block = (Block*)val2;
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					goto head;
				}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "ジェネリッククラスは[]で引数をもたなければならない");
			}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "クラスの継承先はクラスでなければなりません");
		}
	}
	if (val2->type == LetterType::OT_CallBlock) {
		(*n)++;
		return getC2(local, (CallBlock*)val2);
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "クラスの宣言の{|}が抜けています");
}
// ============================================================
// ClassObj::Dec - delegates to Function_Dec
// ClassObj shares the same relevant fields (draw, identity, drawcall,
// call_c, model, ret) through the Obj base struct.
// ============================================================
void ClassObj_Dec(ThreadGC* thgc, Obj* self, Master* local) {
	List* blocks = get_blocks(local);
	ClassObj* cself = (ClassObj*)self;
	SetIdentity(thgc, self);
	self->ifv = create_mapy(thgc, CType::_KV);
	self->n_c = obj_cn++;
	self->vari_c = make_lvari_s(thgc, "void", self->call_c);

	const char* mdl = str_to_cstr(thgc, self->model_str);
	const char* lname = str_to_cstr(thgc, self->letter->text);

	LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* obj = make_lvari_sc(thgc, fmt_str(thgc, "%s*", mdl), "%obj");
	LFunc* func = make_lfunc(thgc, local->llvm, self->vari_c, rn4, obj);
	add_list(thgc, local->llvm->comps, (char*)func);
	add_list(thgc, local->llvm->funcs, (char*)func);
	LVari* thgcptr4 = make_lvari_c(thgc, "%ThreadGCType**", "%thgcptr");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", thgcptr4, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, func->comps, (char*)make_lload(thgc, thgc4, thgcptr4, false));

	LVari* objptr = make_lvari_sc(thgc, fmt_str(thgc, "%s**", mdl), "%objptr");
	add_list(thgc, func->comps, (char*)make_lalloca(thgc, objptr));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, objptr, obj));
	LVari* srv = make_vn(thgc, "%GC_SetRootType", "sr");
	add_list(thgc, func->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
	LCall* srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, rn4);
	lcall_add(thgc, srcall, objptr);
	add_list(thgc, func->comps, (char*)srcall);

	LVari* rn5 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* blk = make_lvari_c(thgc, "i8*", "%fptr1");
	LFunc* funcdraw = make_lfunc(thgc, local->llvm, make_lvari(thgc, fmt_str(thgc, "%s*", mdl), self->drawcall), rn5, blk);
	add_list(thgc, local->llvm->comps, (char*)funcdraw);

	LTypeDec* typedec = make_ltypedec(thgc, self->model_str);
	add_list(thgc, local->llvm->types, (char*)typedec);
	add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i8*", "blk"));

	LVari* thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* i8p = make_lvari_c(thgc, "i8*", "%self");
	String* checkname = fmt_str(thgc, "@%sCheck%d", lname, self->identity);
	LFunc* funccheck = make_lfunc(thgc, local->llvm, make_lvari_s(thgc, "void", checkname), thgc2, i8p);
	add_list(thgc, local->llvm->comps, (char*)funccheck);
	LVari* vari = make_vn(thgc, "i8*", "va");
	LGete* gete = make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0"));
	add_list(thgc, funccheck->comps, (char*)gete);
	LVari* co_val = make_lvari_c(thgc, "%CopyObjectType", "%co");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, co_val, make_lvari_c(thgc, "%CopyObjectType*", "@CopyObject"), false));
	LVari* vari2 = make_vn(thgc, "i8*", "va");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
	LVari* co_cval = make_vn(thgc, "i8*", "co");
	LCall* co_call = make_lcall(thgc, co_cval, co_val);
	lcall_add(thgc, co_call, thgc2);
	lcall_add(thgc, co_call, vari);
	add_list(thgc, funccheck->comps, (char*)co_call);
	add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));

	LVari* thgc3 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* ac_val = make_lvari_c(thgc, "%GC_AddClassType", "%addclass");
	LVari* countv = make_lvari_c(thgc, "i32", "0");
	int lnamelen = (int)strlen(lname);
	LStrV* strv = make_lstrv(thgc, fmt_str(thgc, "@%s%d", lname, self->identity),
		createString(thgc, (char*)lname, lnamelen, 1), lnamelen);
	add_list(thgc, local->llvm->strs, (char*)strv);
	int typ_n = (self->n_c = local->llvm->cn++);
	char typbuf[16]; snprintf(typbuf, sizeof(typbuf), "%d", typ_n);
	LVari* typ = make_lvari_c(thgc, "i32", typbuf);
	LVari* tnp = make_vn(thgc, "i32", "tn");
	add_list(thgc, local->llvm->main->comps, (char*)make_lload(thgc, tnp, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tv = make_vn(thgc, "i32", "tv");
	add_list(thgc, local->llvm->main->comps, (char*)make_lbinop(thgc, LBOP_Add, tv, tnp, typ));
	LCall* ac_call = make_lcall(thgc, NULL, ac_val);
	lcall_add(thgc, ac_call, thgc3);
	lcall_add(thgc, ac_call, tv);
	lcall_add(thgc, ac_call, countv);
	lcall_add(thgc, ac_call, (LVari*)strv);
	lcall_add(thgc, ac_call, make_lvari_s(thgc, "%GCCheckFuncType", checkname));
	lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCFinalizeFuncType", "null"));
	add_list(thgc, local->llvm->main->comps, (char*)ac_call);

	LVari* thgcptr5 = make_lvari_c(thgc, "%ThreadGCType**", "%thgcptr");
	add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", thgcptr5, rn5, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* thgc5 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, thgc5, thgcptr5, false));
	LVari* go_val = make_lvari_c(thgc, "%CopyObjectType", "%gcobject");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, go_val, make_lvari_c(thgc, "%CopyObjectType*", "@CloneObject"), false));
	LVari* v3 = make_vn(thgc, "i8**", "v3");
	add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, "%FuncType", v3, blk, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
	LVari* v4 = make_vn(thgc, "i8*", "v4");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, v4, v3, false));
	LVari* go_v = make_lvari_c(thgc, "i8*", "%obj");
	LCall* go_call_d = make_lcall(thgc, go_v, go_val);
	lcall_add(thgc, go_call_d, thgc5);
	lcall_add(thgc, go_call_d, v4);
	add_list(thgc, funcdraw->comps, (char*)go_call_d);
	LVari* go_c = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", mdl)), "go");
	add_list(thgc, funcdraw->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c, go_v));

	LVari* rn6 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* block = make_lvari_c(thgc, "i8*", "%block");
	LVari* funcptr = make_lvari_c(thgc, "i8*", "%fptr");
	LVari* funcptr2 = make_lvari_c(thgc, "i8*", "%fptr2");
	String* decname = fmt_str(thgc, "@%sdec%d", lname, self->identity);
	LFunc* funcdec = make_lfunc5(thgc, local->llvm, make_lvari_s(thgc, "%FuncType*", decname), rn6, block, funcptr, funcptr2);
	add_list(thgc, local->llvm->comps, (char*)funcdec);
	LVari* thgc6 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, thgc6, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));

	LVari* gmvari = make_lvari_c(thgc, "%GC_mallocType", "%gm");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, gmvari, make_lvari_c(thgc, "%GC_mallocType*", "@GC_malloc"), false));
	LVari* tmp_28 = make_lvari_c(thgc, "i32", "28");
	LVari* go_v_dec = make_lvari_c(thgc, "i8*", "%obj");
	LCall* go_call_dec = make_lcall(thgc, go_v_dec, gmvari);
	lcall_add(thgc, go_call_dec, thgc6);
	lcall_add(thgc, go_call_dec, tmp_28);
	add_list(thgc, funcdec->comps, (char*)go_call_dec);
	LVari* go_c3 = make_vn(thgc, "%FuncType*", "go");
	add_list(thgc, funcdec->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c3, go_v_dec));

	LVari* vc = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc, block));
	LVari* vc2 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc2, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc2, funcptr));
	LVari* vc3 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc3, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc3, funcptr2));

	LVari* objptr2 = make_lvari_c(thgc, "%FuncType**", "%objptr");
	add_list(thgc, funcdec->comps, (char*)make_lalloca(thgc, objptr2));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, objptr2, go_c3));

	srv = make_vn(thgc, "%GC_SetRootType", "sr");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
	srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, rn6);
	lcall_add(thgc, srcall, objptr2);
	add_list(thgc, funcdec->comps, (char*)srcall);

	go_v = make_vn(thgc, "%GCObjectPtr", "ob");
	char tmpbuf2[16]; snprintf(tmpbuf2, sizeof(tmpbuf2), "%d", self->n_c);
	LVari* tmp2 = make_lvari_c(thgc, "i32", tmpbuf2);
	LVari* tmi = make_vn(thgc, "i32", "tm");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, tmi, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tmv = make_vn(thgc, "i32", "tv");
	add_list(thgc, funcdec->comps, (char*)make_lbinop(thgc, LBOP_Add, tmv, tmi, tmp2));
	LCall* go_call2 = make_lcall(thgc, go_v, gmvari);
	lcall_add(thgc, go_call2, thgc6);
	lcall_add(thgc, go_call2, tmv);
	add_list(thgc, funcdec->comps, (char*)go_call2);
	LVari* go_c2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", mdl)), "go");
	add_list(thgc, funcdec->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c2, go_v));

	vc = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, mdl, vc, go_c2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc, block));

	LVari* objptr3 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s**", mdl)), "op");
	add_list(thgc, funcdec->comps, (char*)make_lalloca(thgc, objptr3));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, objptr3, go_c2));

	srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, thgc6);
	lcall_add(thgc, srcall, objptr3);
	add_list(thgc, funcdec->comps, (char*)srcall);

	LVari* vc4 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s**", mdl)), "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc4, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc4, go_c2));

	int count = 8;
	Block* blk0 = (Block*)*(Obj**)get_list(cself->draw->children, 0);
	int order = 1;
	Block* blk2 = (Block*)*(Obj**)get_list(cself->draw->children, 1);
	List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);

	// iterate blk0.vmapA
	for (int ki = 0; ki < blk0->vmapA->kvs->size; ki++) {
		KV* kv = (KV*)get_list(blk0->vmapA->kvs, ki);
		Obj* v = (Obj*)kv->value;
		if (v->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)v;
			variable->order = order++;
			if (variable->cls->type == LetterType::OT_Var) variable->cls = ((Type*)variable->cls)->cls;
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", variable->order);
			if (variable->cls == local->Int) {
				LVari* vdraw = make_vn(thgc, "i32", "vd");
				add_list(thgc, funcdraw->draws, (char*)vdraw);
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i32", str_to_cstr(thgc, kv->key)));
				count += 8;
				LVari* varii = make_vn(thgc, "i32*", "va");
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, mdl, varii, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii, vdraw));
			}
			else if (variable->cls == local->Bool) {
				LVari* vdraw = make_vn(thgc, "i1", "vd");
				add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, "i32", "dr"));
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i1", str_to_cstr(thgc, kv->key)));
				count += 8;
				LVari* varii = make_vn(thgc, "i1*", "va");
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, mdl, varii, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii, vdraw));
			}
			else {
				SetIdentity(thgc, (Obj*)(variable->cls));
				String* vmodel = variable->cls->model_str;
				const char* vmp = str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, vmodel)));
				LVari* vdraw = make_vn(thgc, (char*)vmp, "vd");
				add_list(thgc, funcdraw->draws, (char*)vdraw);
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, (char*)vmp, str_to_cstr(thgc, kv->key)));
				count += 8;
				LVari* varii_m = make_vn(thgc, (char*)vmp, "vm");
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, mdl, varii_m, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii_m, vdraw));

				vari = make_vn(thgc, "i8*", "va");
				gete = make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf));
				add_list(thgc, funccheck->comps, (char*)gete);
				vari2 = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				co_cval = make_vn(thgc, "i8*", "co");
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
			f->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, ((Obj*)f)->model_str))), "fd"));
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", f->order);

			vari = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, block);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", fmt_str(thgc, "@%s", str_to_cstr(thgc, f->drawcall))));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", mdl)), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, mdl, va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, mdl, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
		else if (v->type == LetterType::OT_ClassObj || v->type == LetterType::OT_ModelObj) {
			ClassObj* f = (ClassObj*)v;
			SetIdentity(thgc, (Obj*)(((Obj*)f)));
			((Obj*)f)->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));
			add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, "%FuncType*", "dr"));
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", ((Obj*)f)->order);

			vari = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, ((Obj*)f)->letter->text), ((Obj*)f)->identity)));
			lcall_add(thgc, fu_call, block);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", fmt_str(thgc, "@%s", str_to_cstr(thgc, ((Obj*)f)->drawcall))));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", mdl)), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, mdl, va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, ((Obj*)f)->letter->text), ((Obj*)f)->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, mdl, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
	}

	// iterate blk2.vmapA
	for (int ki = 0; ki < blk2->vmapA->kvs->size; ki++) {
		KV* kv = (KV*)get_list(blk2->vmapA->kvs, ki);
		Obj* v = (Obj*)kv->value;
		if (v->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)v;
			variable->order = order++;
			if (variable->cls->type == LetterType::OT_Var) variable->cls = ((Type*)variable->cls)->cls;
			if (variable->cls == local->Int) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i32", str_to_cstr(thgc, kv->key)));
				count += 8;
			}
			else if (variable->cls == local->Bool) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i1", str_to_cstr(thgc, kv->key)));
				count += 8;
			}
			else {
				SetIdentity(thgc, (Obj*)(variable->cls));
				String* vmodel = variable->cls->model_str;
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, vmodel))), str_to_cstr(thgc, kv->key)));
				count += 8;
				char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", variable->order);
				vari = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				vari2 = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				co_cval = make_vn(thgc, "i8*", "co");
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
			f->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", f->order);

			vari = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_v);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", mdl)), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, mdl, va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, mdl, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
		else if (v->type == LetterType::OT_ClassObj || v->type == LetterType::OT_ModelObj) {
			ClassObj* f = (ClassObj*)v;
			SetIdentity(thgc, (Obj*)(((Obj*)f)));
			add_list(thgc, decs, (char*)f);
			((Obj*)f)->order = order++;
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", ((Obj*)f)->order);

			vari = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, mdl, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, ((Obj*)f)->letter->text), ((Obj*)f)->identity)));
			lcall_add(thgc, fu_call, go_v);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", mdl)), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, mdl, va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, ((Obj*)f)->letter->text), ((Obj*)f)->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", ((Obj*)f)->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, mdl, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
	}

	for (int i = 0; i < decs->size; i++) {
		Obj* d = *(Obj**)get_list(decs, i);
		DecC_vt(thgc, d, local);
	}
	add_list(thgc, local->llvm->funcs, (char*)funcdec);
	self->bas = objptr2;
	add_list(thgc, blocks, (char*)blk0);
	vt_exeC(thgc, (Obj*)blk0, local);
	remove_list(thgc, local->llvm->funcs, local->llvm->funcs->size - 1);
	add_list(thgc, blocks, (char*)blk2);

	add_list(thgc, funcdraw->comps, (char*)make_lret(thgc, go_c));
	countv->name = fmt_str(thgc, "%d", count);
	add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	LVari* vv = make_vn(thgc, "%FuncType*", "vv");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, vv, objptr2, false));

	LVari* rnpv = make_lvari_c(thgc, "i32*", "%ptr");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn6, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* rnpv12 = make_lvari_c(thgc, "i32", "%v");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
	LVari* rnpv2 = make_lvari_c(thgc, "i32", "%dec");
	add_list(thgc, funcdec->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, rnpv, rnpv2));
	add_list(thgc, funcdec->comps, (char*)make_lret(thgc, go_c3));

	self->bas = objptr;
	add_list(thgc, local->labs, (char*)make_llab(thgc, "entry"));
	vt_exeC(thgc, (Obj*)blk2, local);
	remove_list(thgc, local->labs, local->labs->size - 1);
	remove_list(thgc, blocks, blocks->size - 1);
	remove_list(thgc, blocks, blocks->size - 1);

	rnpv = make_lvari_c(thgc, "i32*", "%ptr");
	LFunc* cfunc = llvm_get_func(local->llvm);
	add_list(thgc, cfunc->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	rnpv12 = make_lvari_c(thgc, "i32", "%v");
	add_list(thgc, cfunc->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
	rnpv2 = make_lvari_c(thgc, "i32", "%dec");
	add_list(thgc, cfunc->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, cfunc->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

	add_list(thgc, cfunc->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	remove_list(thgc, local->llvm->funcs, local->llvm->funcs->size - 1);
}


// ============================================================
// Type::PrimaryC
// ============================================================
Obj* Type_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Obj* type = self;
head:
	if (val2->type == LetterType::OT_Block) {
		Block* blk = (Block*)val2;
		if (blk->children->size == 0) {
			ArrType* arrtype = make_arrtype(thgc, type);
			arrtype->letter = val2->letter;
			type = (Obj*)arrtype;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			goto head;
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "配列型は引数の数が0です");
	}
	else if (val2->type == LetterType::_Left) {
		FuncTypeC* functype = make_functype(thgc, type);
		type = (Obj*)functype;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Bracket) {
			add_list(thgc, local->calls, (char*)KouhoSetType);
			val2 = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
			local->calls->size--;
			if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
			Block* blk = (Block*)val2;
			for (int i = 0; i < blk->rets->size; i++) {
				val2 = *(Obj**)get_list(blk->rets, i);
				add_list(thgc, functype->draws, (char*)val2);
			}
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
		}
		else {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				KouhoSetType(thgc, local);
			}
			val2 = GetterC_vt(thgc, val2, local);
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
					Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
					local->calls->size--;
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					Block* blk3 = (Block*)val;
					GenericObj* genericobj = make_genericobj(thgc, (Obj*)generic);
					for (int i = 0; i < blk3->rets->size; i++) {
						val2 = *(Obj**)get_list(blk3->rets, i);
						if (isTypeObj(val2)) {
							add_list(thgc, genericobj->draws, (char*)val2);
						} else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数は型でなければいけません");
					}
					if (generic->vmap->kvs->size != genericobj->draws->size)
						return ErrorC(thgc, LetterType::OT_Error, ((Block*)val)->letter, "ジェネリックスクラスの引数の型の数が合っていません");
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					add_list(thgc, functype->draws, (char*)genericobj);
					goto head;
				} else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数として[]をお願いします");
			} else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "継承元はクラスでなければならない");
		}
	}

	Obj* word = NULL;
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				add_kouho(thgc, local->kouhos, createString(thgc, "new", 3, 1), NULL);
			}
			Word* word2 = (Word*)val2;
			(*n)++;
			if (type->type == LetterType::OT_ArrType) {
				if (str_matchA(val2->letter->text, "new", 3)) {
					ArrayConstructor* ac = make_arrayconstructor(thgc, type);
					((Obj*)ac)->letter = val2->letter;
					return (Obj*)ac;
				}
				else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "配列型の後続にこれません");
			}
			else if (type->type == LetterType::OT_ClassObj || type->type == LetterType::OT_ArrType) {
				if (str_matchA(val2->letter->text, "new", 3)) {
					Constructor* ctor = make_constructor(thgc, type);
					((Obj*)ctor)->letter = self->letter;
					return (Obj*)ctor;
				}
				else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "クラスの後続にこれません");
			}
			else if (type->type == LetterType::OT_ModelObj || type->type == LetterType::OT_GeneObj) {
				ModelObj* model = (ModelObj*)type;
				if (((ClassObj*)model)->draw) {
					Block* blk0 = (Block*)*(Obj**)get_list(((ClassObj*)model)->draw->children, 0);
					if (blk0->vmapA != NULL) {
						char* v = (char*)get_mapy(blk0->vmapA, word2->name);
						if (((Obj*)v)->type == LetterType::OT_Variable) {
							SqlString* ss = make_sqlstring(thgc, word2->name, val2->letter);
							ss->cls = ((Variable*)v)->cls;
							return (Obj*)ss;
						}
					}
					Block* blk1 = (Block*)*(Obj**)get_list(((ClassObj*)model)->draw->children, 1);
					if (blk1->vmapA != NULL) {
						char* v = (char*)get_mapy(blk1->vmapA, word2->name);
						if (((Obj*)v)->type == LetterType::OT_Variable) {
							SqlString* ss = make_sqlstring(thgc, word2->name, val2->letter);
							ss->cls = ((Variable*)v)->cls;
							return (Obj*)ss;
						}
					}
				}
				if (str_matchA(val2->letter->text, "new", 3)) {
					Constructor* ctor = make_constructor(thgc, type);
					((Obj*)ctor)->letter = val2->letter;
					return (Obj*)ctor;
				}
				else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "型の後続にこれません");
			}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "関数型は.が来れません");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の後には単語が必要です");
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
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "コメント変数はコメントの中に宣言してください。");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "コメント変数の名前を指定してください。");
	}
	else if (val2->type == LetterType::OT_Word) {
		word = val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		List* blocks2 = get_blocks(local);
		Block* lastblock = *(Block**)get_list(blocks2, blocks2->size - 1);
		Obj* last_obj = lastblock->obj;
		if (last_obj != NULL && (last_obj->type == LetterType::OT_ModelObj || last_obj->type == LetterType::OT_GeneObj)) {
			Obj* obj2 = type;
		head2:
			if (obj2->type == LetterType::OT_ClassObj || obj2->type == LetterType::OT_FuncType)
				return ErrorC(thgc, LetterType::OT_Error, val2->letter, "model,geneの中ではmodelかgeneの型しか宣言できません");
			else if (obj2->type == LetterType::OT_ArrType) {
				obj2 = ((Type*)obj2)->cls;
				goto head2;
			}
		}
	}
	else if (val2->type == LetterType::OT_Bracket) {
		Obj* blk = vt_exeC(thgc, val2, local);
		Block* blk2 = (Block*)blk;
		(*n)++;
		for (int i = 0; i < blk2->rets->size; i++) {
			Obj* ri = *(Obj**)get_list(blk2->rets, i);
			if (ri->type == LetterType::OT_Word) {
				Obj* variable = declareC(thgc, local, ri->letter->text);
				variable->letter = ri->letter;
			}
			else return ErrorC(thgc, LetterType::OT_Error, blk2->letter, "変数宣言は名前だけです");
		}
		return blk;
	}

	if (val2->type == LetterType::OT_Block) {
		Obj* blk = vt_exeC(thgc, val2, local);
		Block* blk2 = (Block*)blk;
		for (int i = 0; i < blk2->rets->size; i++) {
			Obj* ri = *(Obj**)get_list(blk2->rets, i);
			if (ri->type != LetterType::OT_Word) {
		return ErrorC(thgc, LetterType::OT_Error, blk2->letter, "ジェネリック関数の宣言は名前しか引数をとれません");
			}
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_CallBlock) {
			Obj* func = getC2(local, (CallBlock*)val2);
			(*n)++;
			if (word != NULL) declareC(thgc, local, word->letter->text);
			return func;
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "ジェネリック関数の{|}が指定されていません");
	}
	else if (val2->type == LetterType::OT_CallBlock) {
		Obj* func = getC2(local, (CallBlock*)val2);
		(*n)++;
		if (word != NULL) {
			declareC(thgc, local, word->letter->text);
			func->letter = word->letter;
		}
		return func;
	}
	else {
		if (word == NULL) return type;
		else {
			Obj* variable = declareC(thgc, local, word->letter->text);
			variable->letter = word->letter;
			return variable;
		}
	}
}

// ============================================================
// Var::exepC
// ============================================================
Obj* Var_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		typedef void (*CallFunc)(ThreadGC*, Master*);
		CallFunc fn = (CallFunc)*(char**)get_list(local->calls, local->calls->size - 1);
		fn(thgc, local);
	}
	return self;
}

// ============================================================
// Generic::PrimaryC
// ============================================================
Obj* Generic_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Block) {
		add_list(thgc, local->calls, (char*)KouhoSetType);
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* block = (Block*)val;
		Generic* generic = (Generic*)self;
		GenericObj* geneobj = make_genericobj(thgc, self);
		if (generic->vmap->kvs->size != block->rets->size)
			return ErrorC(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスの引数があっていません。");
		for (int i = 0; i < block->rets->size; i++) {
			Obj* reti = *(Obj**)get_list(block->rets, i);
			if (isTypeObj(reti)) {
				add_list(thgc, geneobj->draws, (char*)reti);
			}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "ジェネリックスクラスは型を引数にとる必要があります。");
		}
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "ジェネリッククラスは[]を引数にとります");
}

// ============================================================
// Number::GetterC
// ============================================================
Obj* Number_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	char buf[32];
	snprintf(buf, sizeof(buf), "%d", self->n_c);
	Value* ret = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, self->letter);
	ret->cls = local->Int;
	ret->vari = make_lvari_c(thgc, "i32", buf);
	return (Obj*)ret;
}

// ============================================================
// Number::exepC
// ============================================================
Obj* Number_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Number::PrimaryC
// ============================================================
Obj* Number2_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				add_kouho(thgc, local->kouhos, createString(thgc, "random", 6, 1), NULL);
			}
			Word* word = (Word*)val2;
			String* wname = val2->letter->text;
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (str_matchA(wname, "random", 6)) {
				if (val2->type == LetterType::OT_Bracket) {
					(*n)++;
				if (((Block*)val2)->children->size != 0) return ErrorC(thgc, LetterType::OT_Error, val2->letter, "randomの引数は0個です");
					Obj* num = make_number(thgc, NULL);
					((Number*)num)->cls = (Obj*)local->Int;
					return num;
				}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "randomの()が指定されていません");
			}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "適切な後続の単語ではありません");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の後に単語が来ていません");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, ".が来ていません");
}

// ============================================================
// Number::PlusC
// ============================================================
Obj* Number_PlusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) return self;
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_number(thgc, self->letter);
		((Number*)r)->value = ((Number*)self)->value + ((Number*)val2)->value;
		((Number*)r)->cls = (Obj*)local->Int;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = (float)((Number*)self)->value + ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	else if (val2->type == LetterType::OT_StrObj) {
		StrObj* r = make_strobj(thgc, self->letter);
		((StrObj*)r)->cls = (Obj*)local->StrT;
		return (Obj*)r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の+は整数か小数か文字列です");
}

// ============================================================
// Number::MinusC
// ============================================================
Obj* Number_MinusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) {
		Obj* r = make_number(thgc, self->letter);
		((Number*)r)->value = -((Number*)self)->value;
		((Number*)r)->cls = (Obj*)local->Int;
		return r;
	}
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_number(thgc, self->letter);
		((Number*)r)->value = ((Number*)self)->value - ((Number*)val2)->value;
		((Number*)r)->cls = (Obj*)local->Int;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = (float)((Number*)self)->value - ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の-は整数か小数です");
}

// ============================================================
// Number::MulC
// ============================================================
Obj* Number_MulC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_number(thgc, self->letter);
		((Number*)r)->value = ((Number*)self)->value * ((Number*)val2)->value;
		((Number*)r)->cls = (Obj*)local->Int;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = (float)((Number*)self)->value * ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の*は整数か小数です");
}

// ============================================================
// Number::DivC
// ============================================================
Obj* Number_DivC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_number(thgc, self->letter);
		((Number*)r)->value = ((Number*)self)->value / ((Number*)val2)->value;
		((Number*)r)->cls = (Obj*)local->Int;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = (float)((Number*)self)->value / ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の/は整数か小数です");
}

// ============================================================
// Number::MoreThanC
// ============================================================
Obj* Number_MoreThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((Number*)self)->value > ((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = (float)((Number*)self)->value > ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の>は整数か小数です");
}

// ============================================================
// Number::LessThanC
// ============================================================
Obj* Number_LessThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((Number*)self)->value < ((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = (float)((Number*)self)->value < ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の<は整数か小数です");
}

// ============================================================
// Number::MoreEqualC
// ============================================================
Obj* Number_MoreEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((Number*)self)->value >= ((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = (float)((Number*)self)->value >= ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の>=は整数か小数です");
}

// ============================================================
// Number::LessEqualC
// ============================================================
Obj* Number_LessEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((Number*)self)->value <= ((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = (float)((Number*)self)->value <= ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の<=は整数か小数です");
}

// ============================================================
// Number::NotEqualC
// ============================================================
Obj* Number_NotEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((Number*)self)->value != ((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = (float)((Number*)self)->value != ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の!=は整数か小数です");
}

// ============================================================
// Number::EqualEqualC
// ============================================================
Obj* Number_EqualEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((Number*)self)->value == ((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = (float)((Number*)self)->value == ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "整数の==は整数か小数です");
}

// ============================================================
// Number::opeC dispatcher
// ============================================================
Obj* Number_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	if (key == LetterType::_EqualEqual) return Number_EqualEqualC(thgc, self, key, local, val2);
	if (key == LetterType::_NotEqual) return Number_NotEqualC(thgc, self, key, local, val2);
	if (key == LetterType::_MoreEqual) return Number_MoreEqualC(thgc, self, key, local, val2);
	if (key == LetterType::_LessEqual) return Number_LessEqualC(thgc, self, key, local, val2);
	if (key == LetterType::_MoreThan) return Number_MoreThanC(thgc, self, key, local, val2);
	if (key == LetterType::_LessThan) return Number_LessThanC(thgc, self, key, local, val2);
	if (key == LetterType::_Plus) return Number_PlusC(thgc, self, key, local, val2);
	if (key == LetterType::_Minus) return Number_MinusC(thgc, self, key, local, val2);
	if (key == LetterType::_Mul) return Number_MulC(thgc, self, key, local, val2);
	if (key == LetterType::_Div) return Number_DivC(thgc, self, key, local, val2);
	return ErrorC(thgc, LetterType::OT_Error, self->letter, "整数にこの演算子は登録されていません");
}

// ============================================================
// FloatVal::exeC
// ============================================================
Obj* FloatVal_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// FloatVal::exepC
// ============================================================
Obj* FloatVal_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// FloatVal::PrimaryC
// ============================================================
Obj* FloatVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "蟆乗焚縺ｫ縺ｯ蠕檎ｶ壹′譚･繧後∪縺帙ｓ");
}

// ============================================================
// FloatVal::PlusC
// ============================================================
Obj* FloatVal_PlusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) return self;
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value + (float)((Number*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value + ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	else if (val2->type == LetterType::OT_StrObj) {
		StrObj* r = make_strobj(thgc, self->letter);
		((StrObj*)r)->cls = (Obj*)local->StrT;
		return (Obj*)r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の+は整数か小数です");
}

// ============================================================
// FloatVal::MinusC
// ============================================================
Obj* FloatVal_MinusC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2 == NULL) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = -((FloatVal*)self)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value - (float)((Number*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value - ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の-は整数か小数です");
}

// ============================================================
// FloatVal::MulC
// ============================================================
Obj* FloatVal_MulC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value * (float)((Number*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value * ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の*は整数か小数です");
}

// ============================================================
// FloatVal::DivC
// ============================================================
Obj* FloatVal_DivC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value / (float)((Number*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_floatval(thgc, self->letter);
		((FloatVal*)r)->value = ((FloatVal*)self)->value / ((FloatVal*)val2)->value;
		((FloatVal*)r)->cls = (Obj*)local->FloatT;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の/は整数か小数です");
}

// ============================================================
// FloatVal::MoreThanC
// ============================================================
Obj* FloatVal_MoreThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value > (float)((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value > ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の>は整数か小数です");
}

// ============================================================
// FloatVal::LessThanC
// ============================================================
Obj* FloatVal_LessThanC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value < (float)((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value < ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の<は整数か小数です");
}

// ============================================================
// FloatVal::MoreEqualC
// ============================================================
Obj* FloatVal_MoreEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value >= (float)((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value >= ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の>=は整数か小数です");
}

// ============================================================
// FloatVal::LessEqualC
// ============================================================
Obj* FloatVal_LessEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value <= (float)((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value <= ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の<=は整数か小数です");
}

// ============================================================
// FloatVal::NotEqualC
// ============================================================
Obj* FloatVal_NotEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value != (float)((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value != ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の!=は整数か小数です");
}

// ============================================================
// FloatVal::EqualEqualC
// ============================================================
Obj* FloatVal_EqualEqualC(ThreadGC* thgc, Obj* self, LetterType op, Master* local, Obj* val2) {
	if (val2->type == LetterType::OT_Number) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value == (float)((Number*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	else if (val2->type == LetterType::OT_FloatVal) {
		Obj* r = make_boolval(thgc, self->letter);
		((BoolVal*)r)->value = ((FloatVal*)self)->value == ((FloatVal*)val2)->value;
		((BoolVal*)r)->cls = (Obj*)local->Bool;
		return r;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "小数の==は整数か小数です");
}

// ============================================================
// SqlString::exeC
// ============================================================
Obj* SqlString_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// SqlString::opeC  - SQL string building
// model_str = SQL text, vmapA = varis map (NumberString(n) -> Obj*)
// ============================================================
Obj* SqlString_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	SqlString* sself = (SqlString*)self;
	if (sself->value == NULL) sself->value = createString(thgc, (char*)"", 0, 1);
	if (((Val*)self)->vmap == NULL) ((Val*)self)->vmap = create_mapy(thgc, CType::_CObj);

	if (key == LetterType::_EqualEqual || key == LetterType::_NotEqual ||
		key == LetterType::_LessThan || key == LetterType::_MoreEqual ||
		key == LetterType::_Plus || key == LetterType::_Minus ||
		key == LetterType::_Mul || key == LetterType::_Div ||
		key == LetterType::_And || key == LetterType::_Or) {

		String* k = NULL;
		if (key == LetterType::_EqualEqual) k = createString(thgc, (char*)"=", 1, 1);
		else if (key == LetterType::_NotEqual) k = createString(thgc, (char*)"!=", 2, 1);
		else if (key == LetterType::_LessThan) k = createString(thgc, (char*)"<", 1, 1);
		else if (key == LetterType::_MoreEqual) k = createString(thgc, (char*)">=", 2, 1);
		else if (key == LetterType::_Plus) k = createString(thgc, (char*)"+", 1, 1);
		else if (key == LetterType::_Minus) k = createString(thgc, (char*)"-", 1, 1);
		else if (key == LetterType::_Mul) k = createString(thgc, (char*)"*", 1, 1);
		else if (key == LetterType::_Div) k = createString(thgc, (char*)"/", 1, 1);
		else if (key == LetterType::_And) k = createString(thgc, (char*)"and", 3, 1);
		else if (key == LetterType::_Or) k = createString(thgc, (char*)"or", 2, 1);

		if (key == LetterType::_Equal) {
			// value += " " + k + " "
			String* tmp = StringAdd(thgc, sself->value, (char*)" ", 1, 1);
			tmp = StringAdd2(thgc, tmp, k);
			tmp = StringAdd(thgc, tmp, (char*)" ", 1, 1);
			sself->value = tmp;
		} else {
			// value = "(" + value + " " + k + " "
			String* tmp = createString(thgc, (char*)"(", 1, 1);
			tmp = StringAdd2(thgc, tmp, sself->value);
			tmp = StringAdd(thgc, tmp, (char*)" ", 1, 1);
			tmp = StringAdd2(thgc, tmp, k);
			tmp = StringAdd(thgc, tmp, (char*)" ", 1, 1);
			sself->value = tmp;
		}

		if (val2->vt == &vt_SqlString) {
			SqlString* sval2 = (SqlString*)val2;
			sself->value = StringAdd2(thgc, sself->value, sval2->value);
			sself->value = StringAdd(thgc, sself->value, (char*)")", 1, 1);
			if (((Val*)val2)->vmap != NULL) {
				for (int i = 0; i < ((Val*)val2)->vmap->kvs->size; i++) {
					KV* kv = (KV*)*(char**)get_list(((Val*)val2)->vmap->kvs, i);
					if (get_mapy(((Val*)self)->vmap, kv->key) == NULL) {
						add_mapy(thgc, ((Val*)self)->vmap, kv->key, kv->value);
					}
				}
			}
			return self;
		} else if (val2->type == LetterType::OT_Value) {
			Value* vval = (Value*)val2;
			Obj* vcls = vval->cls;
			if (vcls == local->Short || vcls == local->Int || vcls == local->FloatT || vcls == local->StrT || vcls == local->Bool) {
				int n = llvm_n++;
				String* nstr = fmt_str(thgc, "~%d", n);
				sself->value = StringAdd2(thgc, sself->value, nstr);
				sself->value = StringAdd(thgc, sself->value, (char*)")", 1, 1);
				add_mapy(thgc, ((Val*)self)->vmap, NumberString(thgc, n), (char*)vval);
				return self;
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, self->letter, "SQL文字列の演算子はサポートされていません");
}

// ============================================================
// SqlString::Format - replace ~N with ?N in sql text, renumber varis
// ============================================================
void SqlString_Format(ThreadGC* thgc, Obj* self) {
	SqlString* sself = (SqlString*)self;
	if (((Val*)self)->vmap == NULL) return;
	Map* newmap = create_mapy(thgc, CType::_CObj);
	int n = 1;
	for (int i = 0; i < ((Val*)self)->vmap->kvs->size; i++) {
		KV* kv = (KV*)*(char**)get_list(((Val*)self)->vmap->kvs, i);
		String* tilde = StringAdd(thgc, createString(thgc, (char*)"~", 1, 1), kv->key->data, kv->key->size, kv->key->esize);
		String* quest = fmt_str(thgc, "?%d", n);
		sself->value = StringReplace(thgc, sself->value, tilde, quest);
		add_mapy(thgc, newmap, NumberString(thgc, n), kv->value);
		n++;
	}
	((Val*)self)->vmap = newmap;
}

// ============================================================
// StrObj::GetterC
// ============================================================
Obj* StrObj_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	LFunc* func = llvm_get_func(local->llvm);
	LVari* csvari = make_vn(thgc, "%CreateStringType", "cs");
	add_list(thgc, func->comps, (char*)make_lload(thgc, csvari, make_lvari_c(thgc, "%CreateStringType*", "@CreateString"), false));
	LVari* v2 = make_vn(thgc, "i8*", "v2");

	String* value = ((StrObj*)self)->value;
	int n = 1;
	for (int i = 0; i < value->size; i++) {
		if (value->esize == 2) {
			uint16_t ch = ((uint16_t*)value->data)[i];
			if (ch >= 256) { n = 2; break; }
		}
	}
	// UTF-16LE encode and format as \XX hex bytes
	String* val = createString(thgc, "", 0, 1);
	if (value->esize == 2) {
		unsigned char* raw = (unsigned char*)value->data;
		if (n == 1) {
			for (int i = 0; i < value->size * 2; i++) {
				if (i % 2 == 0) {
					char hex[5]; snprintf(hex, sizeof(hex), "\\%02X", raw[i]);
					val = StringAdd(thgc, val, hex, 3, 1);
				}
			}
		} else {
			for (int i = 0; i < value->size * 2; i++) {
				char hex[5]; snprintf(hex, sizeof(hex), "\\%02X", raw[i]);
				val = StringAdd(thgc, val, hex, 3, 1);
			}
		}
	} else {
		// esize==1: all chars < 256, convert to UTF-16LE low bytes
		unsigned char* raw = (unsigned char*)value->data;
		for (int i = 0; i < value->size; i++) {
			char hex[5]; snprintf(hex, sizeof(hex), "\\%02X", raw[i]);
			val = StringAdd(thgc, val, hex, 3, 1);
		}
	}
	int slen = value->size;
	String* svname = fmt_str(thgc, "@s%d", llvm_n++);
	LStrV* sv = make_lstrv(thgc, svname, val, slen * n);
	add_list(thgc, local->llvm->strs, (char*)sv);

	String* gete_type = fmt_str(thgc, "[%d x i8]", slen * n + 1);
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, (char*)gete_type->data, v2, (LVari*)sv,
		make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* v = make_vn(thgc, "%StringType*", "v");
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	char lenbuf[16]; snprintf(lenbuf, sizeof(lenbuf), "%d", slen * n);
	char nbuf[16]; snprintf(nbuf, sizeof(nbuf), "%d", n);
	LCall* call = make_lcall(thgc, v, csvari);
	lcall_add(thgc, call, thgc4);
	lcall_add(thgc, call, v2);
	lcall_add(thgc, call, make_lvari_c(thgc, "i32", lenbuf));
	lcall_add(thgc, call, make_lvari_c(thgc, "i32", nbuf));
	add_list(thgc, func->comps, (char*)call);
	Value* ret = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, self->letter);
	ret->cls = local->StrT;
	ret->vari = v;
	return (Obj*)ret;
}

// ============================================================
// StrObj::exeC
// ============================================================
Obj* StrObj_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// StrObj::exepC
// ============================================================
Obj* StrObj_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// StrObj::PrimaryC
// ============================================================
Obj* StrObj_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "文字列には後続が来れません");
}

// ============================================================
// BoolVal::exeC
// ============================================================
Obj* BoolVal2_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// BoolVal::exepC
// ============================================================
Obj* BoolVal2_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// BoolVal::PrimaryC
// ============================================================
Obj* BoolVal2_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "bool蛟､縺ｫ縺ｯ蠕檎ｶ壹′縺薙ｌ縺ｾ縺帙ｓ");
}

// ============================================================
// VoiVal::exepC
// ============================================================
Obj* VoiVal_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return make_cobj(thgc, CType::_CObj, LetterType::OT_Error, NULL);
}

// ============================================================
// VoiVal::PrimaryC
// ============================================================
Obj* VoiVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "void蛟､縺ｫ縺ｯ蠕檎ｶ壹′縺薙ｌ縺ｾ縺帙ｓ");
}

// ============================================================
// Null::exeC
// ============================================================
Obj* Null_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Null::exepC
// ============================================================
Obj* Null_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// Null::PrimaryC
// ============================================================
Obj* Null_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "null縺ｫ縺ｯ蠕檎ｶ壹′譚･縺ｾ縺帙ｓ");
}

// ============================================================
// ArrayConstructor::PrimaryC
// ============================================================
Obj* ArrayConstructor_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Obj* cls = (Obj*)((ArrayConstructor*)self)->cls;
	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
	if (blk->rets->size != 1) return ErrorC(thgc, LetterType::OT_Error, val2->letter, "配列型のコンストラクタの引数は1です");
		(*n)++;
		Obj* r0 = *(Obj**)get_list(blk->rets, 0);
		if (r0->type == LetterType::OT_Number) {
			Obj* value = New_Value(thgc, cls, local, self->letter);
			if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
			Block* rb = (Block*)make_cobj(thgc, CType::_CBlock, LetterType::OT_Array, self->letter);
			rb->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
			add_list(thgc, rb->rets, (char*)value);
			return (Obj*)rb;
		}
		else return ErrorC(thgc, LetterType::OT_Error, blk->letter, "配列型のコンストラクタの引数は1です");
	}
	Obj* val3 = GetterA_vt(thgc, val2, local);
	if (val3->type == LetterType::OT_Wait || val3->type == LetterType::OT_Error || val3->type == LetterType::OT_NG) return val3;
	if (val3->type == LetterType::OT_Number) {
		Obj* value = New_Value(thgc, cls, local, self->letter);
		if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
		Block* rb = (Block*)make_cobj(thgc, CType::_CBlock, LetterType::OT_Array, self->letter);
		rb->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
		add_list(thgc, rb->rets, (char*)value);
		return (Obj*)rb;
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "コンストラクタの引数が適切ではありません");
}

// ============================================================
// Constructor::PrimaryC
// ============================================================
Obj* Constructor_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	LFunc* func = llvm_get_func(local->llvm);
	Obj* cls = (Obj*)((Constructor*)self)->cls;

	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		Block* blk = (Block*)val;
		(*n)++;
		for (int i = 0; i < local->compsC->size; i++) {
			LComp* comp = *(LComp**)get_list(local->compsC, i);
			add_list(thgc, func->comps, (char*)comp);
		}
		local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);

		LVari* vari;
		if (cls == local->Int) vari = make_vn(thgc, "i32", "va");
		else if (cls == local->Bool) vari = make_vn(thgc, "i2", "va");
		else vari = make_vn_s(thgc, cls->model, "va");
		self->vari_c = vari;

		func->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
		LVari* fv = make_vn(thgc, "%FuncType*", "fv");
		add_list(thgc, func->comps, (char*)make_lload(thgc, fv, func->vari, false));

		LVari* go_v = make_vn_s(thgc, StringAdd(thgc, cls->model, (char*)"*", 1, 1), "go");
		LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");

		LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, StringAdd(thgc, cls->model, (char*)"*", 1, 1), cls->drawcall));
		lcall_add(thgc, go_call, rn4);
		lcall_add(thgc, go_call, fv);

		// vmapA values matching with blk->rets
		Block* draw0 = (Block*)*(Obj**)get_list(cls->children, 0);
		List* varray = map_values_list(thgc, draw0->vmapA);
		for (int i = 0; i < varray->size; i++) {
			Obj* va_i = *(Obj**)get_list(varray, i);
			Obj* ret_i = *(Obj**)get_list(blk->rets, i);
			if (va_i->type == LetterType::OT_Variable && ret_i->type == LetterType::OT_Value) {
				Value* value = (Value*)ret_i;
				Variable* variable = (Variable*)va_i;
				if (value->cls == variable->cls) {
					lcall_add(thgc, go_call, value->vari);
				} else {
					if (variable->cls == local->Int) {
						LVari* v = make_vn(thgc, "i32", "v");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
						lcall_add(thgc, go_call, v);
					} else if (variable->cls == local->Short) {
						LVari* v = make_vn(thgc, "i16", "v");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
						lcall_add(thgc, go_call, v);
					} else if (variable->cls == local->Bool) {
						LVari* v = make_vn(thgc, "i1", "v");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
						lcall_add(thgc, go_call, v);
					} else {
						lcall_add(thgc, go_call, value->vari);
					}
				}
			}
		}
		add_list(thgc, func->comps, (char*)go_call);

		LCall* init_call = make_lcall(thgc, NULL, make_lvari(thgc, vari->type, cls->call_c));
		lcall_add(thgc, init_call, rn4);
		lcall_add(thgc, init_call, go_v);
		add_list(thgc, func->comps, (char*)init_call);

		Value* ret = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, self->letter);
		ret->cls = cls;
		ret->vari = go_v;
		return (Obj*)ret;
	}
	// else: non-Bracket path
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
	}
	if (val2 != NULL) {
		val2 = GetterC_vt(thgc, val2, local);
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj || val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
			(*n)++;
			for (int i = 0; i < local->compsC->size; i++) {
				LComp* comp = *(LComp**)get_list(local->compsC, i);
				add_list(thgc, func->comps, (char*)comp);
			}
			local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);

			LVari* vari;
			if (cls == local->Int) vari = make_vn(thgc, "i32", "va");
			else if (cls == local->Bool) vari = make_vn(thgc, "i2", "va");
			else vari = make_vn_s(thgc, cls->model, "va");
			self->vari_c = vari;

			func->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
			LVari* fv = make_vn(thgc, "%FuncType*", "fv");
			add_list(thgc, func->comps, (char*)make_lload(thgc, fv, func->vari, false));

			LVari* go_v = make_vn_s(thgc, StringAdd(thgc, cls->model, (char*)"*", 1, 1), "go");
			LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");

			LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, StringAdd(thgc, cls->model, (char*)"*", 1, 1), cls->drawcall));
			lcall_add(thgc, go_call, rn4);
			lcall_add(thgc, go_call, fv);

			Block* draw0 = (Block*)*(Obj**)get_list(cls->children, 0);
			List* varray = map_values_list(thgc, draw0->vmapA);
			Variable* vv = (Variable*)*(Obj**)get_list(varray, 0);
			if (vv->cls == ((Value*)val2)->cls) {
				lcall_add(thgc, go_call, ((Value*)val2)->vari);
			} else {
				if (vv->cls == local->Int) {
					LVari* v = make_vn(thgc, "i32", "v");
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
					lcall_add(thgc, go_call, v);
				} else if (vv->cls == local->Short) {
					LVari* v = make_vn(thgc, "i16", "v");
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
					lcall_add(thgc, go_call, v);
				} else if (vv->cls == local->Bool) {
					LVari* v = make_vn(thgc, "i1", "v");
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
					lcall_add(thgc, go_call, v);
				} else {
					lcall_add(thgc, go_call, ((Value*)val2)->vari);
				}
			}
			lcall_add(thgc, go_call, ((Value*)val2)->vari);
			add_list(thgc, func->comps, (char*)go_call);

			LCall* init_call = make_lcall(thgc, NULL, make_lvari(thgc, vari->type, cls->call_c));
			lcall_add(thgc, init_call, rn4);
			lcall_add(thgc, init_call, go_v);
			add_list(thgc, func->comps, (char*)init_call);

			Value* ret = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, self->letter);
			ret->cls = cls;
			ret->vari = vari;
			return (Obj*)ret;
		}
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "コンストラクタの引数が適切ではありません");
}
// ============================================================
// obj2c_part2.h - C conversion of Obj2C.cs lines 2000-5000
// Converted from C# partial classes to C free functions
// ============================================================

// Forward declarations for this file
Obj* ClassObj_tail_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Base_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Variable_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Variable_SelfC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Variable_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Variable_EqualC(ThreadGC* thgc, Variable* self, LetterType op, Master* local, Obj* val2);
Obj* Variable_InC(ThreadGC* thgc, Variable* self, LetterType op, Master* local, Obj* val2);
Obj* Function_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* Function_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
void Function_Dec(ThreadGC* thgc, Obj* self, Master* local);
Obj* If_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* If_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* While_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* While_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* For_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);

// ============================================================
// BitcastCast moved to obj1c.h (called from both files)

// ============================================================
// Helper: make Value object
// ============================================================
static inline Obj* make_value(ThreadGC* thgc, Obj* cls, LVari* vari) {
	Value* val = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
	((Val*)val)->cls = cls;
	val->vari = vari;
	return (Obj*)val;
}

// ============================================================
// Clone virtual implementations (C# public override Obj Clone())
// ============================================================
// C#: return new Value(this.cls) { vmap0 = vmap0, vmap = vmap };
Obj* Value_Clone(ThreadGC* thgc, Obj* self) {
	Value* src = (Value*)self;
	Value* ret = (Value*)make_value(thgc, ((Val*)src)->cls, NULL);
	ret->vmap0 = src->vmap0;
	((Val*)ret)->vmap = ((Val*)src)->vmap;
	return (Obj*)ret;
}
// C#: return new Variable(cls) { letter=letter, order=order, left=left, right=right, clone=true };
Obj* Variable_Clone(ThreadGC* thgc, Obj* self) {
	Variable* src = (Variable*)self;
	Variable* ret = make_variable(thgc, src->cls);
	((Obj*)ret)->letter = ((Obj*)src)->letter;
	ret->order = src->order;
	ret->left = src->left;
	ret->right = src->right;
	ret->clone = true;
	return (Obj*)ret;
}

// ------------------------------------------------------------
// Shared helper: shallow-copy Block structural fields (labelmap,
// branchmap, template, obj, letter, letter2) + children list, then
// deep-clone vmapA entries via vt_Clone. Used by Block/NewBlock/
// CrossBlock/MigrateBlock Clone implementations.
// ------------------------------------------------------------
static inline void _block_clone_copy(ThreadGC* thgc, Block* dst, Block* src) {
	dst->labelmap  = src->labelmap;
	dst->branchmap = src->branchmap;
	dst->labelmapn = src->labelmapn;
	dst->branchmapn= src->branchmapn;
	// C# has bool template field but C++ Block struct omits it
	dst->obj       = src->obj;
	dst->obj_n     = src->obj_n;
	((Obj*)dst)->letter = ((Obj*)src)->letter;
	dst->letter2   = src->letter2;
	// shallow-copy children list reference
	((Obj*)dst)->children = ((Obj*)src)->children;
	// deep-clone vmapA entries
	if (src->vmapA && src->vmapA->kvs) {
		for (int i = 0; i < src->vmapA->kvs->size; i++) {
			KV* kv = *(KV**)get_list(src->vmapA->kvs, i);
			Obj* cloned = vt_Clone(thgc, (Obj*)kv->value);
			add_mapy(thgc, dst->vmapA, kv->key, (char*)cloned);
		}
	}
}

// C#: return new Block(type) { labelmap, branchmap, labelmapn, template, obj, letter, letter2 };
//     + copy children + for (key in vmap) block.vmap[key] = vmap[key].Clone();
Obj* Block_Clone(ThreadGC* thgc, Obj* self) {
	Block* src = (Block*)self;
	Block* ret = make_block(thgc, self->type);
	_block_clone_copy(thgc, ret, src);
	return (Obj*)ret;
}

// NewBlock.Clone: same as Block.Clone but also copies gvl (C# specific field)
// In C++ port NewBlock is just Block with OT_NewBlock; no gvl in struct.
Obj* NewBlock_Clone(ThreadGC* thgc, Obj* self) {
	Block* src = (Block*)self;
	Block* ret = make_block(thgc, self->type);
	_block_clone_copy(thgc, ret, src);
	return (Obj*)ret;
}

// CrossBlock.Clone: same structure; CrossBlock in C++ has extra gj field
Obj* CrossBlock_Clone(ThreadGC* thgc, Obj* self) {
	CrossBlock* src = (CrossBlock*)self;
	CrossBlock* ret = (CrossBlock*)make_block(thgc, self->type);
	_block_clone_copy(thgc, (Block*)ret, (Block*)src);
	ret->gj = src->gj;
	return (Obj*)ret;
}

// MigrateBlock.Clone: typedef Block MigrateBlock in C++; just clone as Block
Obj* MigrateBlock_Clone(ThreadGC* thgc, Obj* self) {
	Block* src = (Block*)self;
	Block* ret = make_block(thgc, self->type);
	_block_clone_copy(thgc, ret, src);
	return (Obj*)ret;
}

// C#: return new Function(ret) { blocks=blocks, draw=draw, bracket=bracket, block=block };
Obj* Function_Clone(ThreadGC* thgc, Obj* self) {
	Function* src = (Function*)self;
	Function* ret = (Function*)make_function(thgc, self->type, self->letter);
	ret->rettype = src->rettype;
	ret->blocks  = src->blocks;
	ret->draw    = src->draw;
	ret->bracket = src->bracket;
	ret->block   = src->block;
	return (Obj*)ret;
}

// C#: return new SignalFunction() { name=name, blocks=blocks, draw=draw };
// C++ port uses Function with OT_SignalFunction; name is stored via rename/letter-level field
Obj* SignalFunction_Clone(ThreadGC* thgc, Obj* self) {
	Function* src = (Function*)self;
	Function* ret = (Function*)make_function(thgc, LetterType::OT_SignalFunction, self->letter);
	((Obj*)ret)->rename = ((Obj*)src)->rename;  // C# SignalFunction.name
	ret->blocks = src->blocks;
	ret->draw   = src->draw;
	return (Obj*)ret;
}

// C#: return new Comment() { letter=letter, vmap=vmap, children=children };
Obj* Comment_Clone(ThreadGC* thgc, Obj* self) {
	Comment* src = (Comment*)self;
	Comment* ret = (Comment*)make_comment(thgc, self->letter);
	ret->vmap = src->vmap;
	((Obj*)ret)->children = self->children;
	return (Obj*)ret;
}

// C#: var gj = new GenericObj(gene) { letter=letter }; for (i..draws.Count) gj.draws.Add(draws[i]); return gj;
Obj* GenericObj_Clone(ThreadGC* thgc, Obj* self) {
	GenericObj* src = (GenericObj*)self;
	GenericObj* ret = make_genericobj(thgc, (Obj*)src->gene);
	((Obj*)ret)->letter = ((Obj*)src)->letter;
	if (src->draws) {
		for (int i = 0; i < src->draws->size; i++) {
			add_list(thgc, ret->draws, *get_list(src->draws, i));
		}
	}
	return (Obj*)ret;
}

// C#: return new ModelVal(cls as ModelObj) { vmap=vmap };
Obj* ModelVal_Clone(ThreadGC* thgc, Obj* self) {
	Val* src = (Val*)self;
	Val* ret = make_val(thgc, CType::_CVal, LetterType::OT_ModelVal, ((Obj*)src)->letter);
	ret->cls = src->cls;
	ret->vmap = src->vmap;
	return (Obj*)ret;
}

// C#: return new GeneVal(cls as GeneObj);
Obj* GeneVal_Clone(ThreadGC* thgc, Obj* self) {
	Val* src = (Val*)self;
	Val* ret = make_val(thgc, CType::_CVal, LetterType::OT_GeneVal, ((Obj*)src)->letter);
	ret->cls = src->cls;
	return (Obj*)ret;
}

// ============================================================
// make_voival: Compile.h に移動済み

// ============================================================
// Helper: make Iterator object
// ============================================================
static inline Obj* make_iterator_blk(ThreadGC* thgc, int idx, Block* blk) {
	Obj* it = make_iterator(thgc);
	((Iterator*)it)->value = blk;
	return it;
}

// ============================================================
// ClassObj::PrimaryC tail (lines 2000-2063)
// This handles the continuation after constructor argument matching
// ============================================================
Obj* ClassObj_tail_PrimaryC(ThreadGC* thgc, ClassObj* cls, int* n, Master* local, Primary* primary, Obj* val2, LVari* vari) {
	LFunc* func = llvm_get_func(local->llvm);

	// add local.comps to func.comps
	for (int i = 0; i < local->compsC->size; i++) {
		LComp* c = *(LComp**)get_list(local->compsC, i);
		add_list(thgc, func->comps, (char*)c);
	}
	local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);

	LVari* go_v;
	LVari* rn4;
	LCall* go_call;
	LCall* init_call;

	if (((Type*)cls)->cls == local->Int) vari = make_vn(thgc, "i32", "va");
	else if (((Type*)cls)->cls == local->Bool) vari = make_vn(thgc, "i2", "va");
	else {
		vari = make_vn_s(thgc, model_type(thgc, cls->model, "*"), "va");
	}
	func->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
	LVari* fv = make_vn(thgc, "%FuncType*", "fv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, fv, func->vari, false));

	go_v = make_vn_s(thgc, model_type(thgc, cls->model, "*"), "go");
	rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	{
		String* mtype = model_type(thgc, cls->model, "*");
		go_call = make_lcall(thgc, go_v, make_lvari(thgc, mtype, cls->drawcall ? cls->drawcall : createString(thgc, (char*)"", 0, 1)));
		lcall_add(thgc, go_call, rn4);
		lcall_add(thgc, go_call, fv);
	}

	// Get first variable from draw block
	Block* drawblk = (Block*)*(Obj**)get_list(cls->draw->children, 0);
	if (drawblk->vmapA && drawblk->vmapA->kvs->size > 0) {
		KV* kv0 = *(KV**)get_list(drawblk->vmapA->kvs, 0);
		Variable* vv = (Variable*)kv0->value;
		if (val2 && val2->type == LetterType::OT_Value) {
			Value* val2v = (Value*)val2;
			if (vv->cls == val2v->cls) {
				lcall_add(thgc, go_call, val2v->vari);
			} else {
				if (vv->cls == local->Int) {
					LVari* v = make_vn(thgc, "i32", "v");
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, val2v->vari));
					lcall_add(thgc, go_call, v);
				} else if (vv->cls == local->Short) {
					LVari* v = make_vn(thgc, "i16", "v");
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, val2v->vari));
					lcall_add(thgc, go_call, v);
				} else if (vv->cls == local->Bool) {
					LVari* v = make_vn(thgc, "i1", "v");
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, val2v->vari));
					lcall_add(thgc, go_call, v);
				} else {
					lcall_add(thgc, go_call, val2v->vari);
				}
			}
		}
	}
	lcall_add(thgc, go_call, ((Value*)val2)->vari);
	add_list(thgc, func->comps, (char*)go_call);
	init_call = make_lcall(thgc, NULL, make_lvari_c(thgc, vari->type ? (char*)vari->type->data : "void", cls->call_c ? cls->call_c->data : ""));
	lcall_add(thgc, init_call, rn4);
	lcall_add(thgc, init_call, go_v);
	add_list(thgc, func->comps, (char*)init_call);
	return make_value(thgc, (Obj*)cls, vari);
}

// ============================================================
// Base::PrimaryC (lines 2064-2116)
// ============================================================
Obj* Base_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	int m = 0;
	if (val2->type == LetterType::OT_Block) {
		Block* blk = (Block*)val2;
		if (blk->children->size == 1) {
			Obj* ch0 = *(Obj**)get_list(blk->children, 0);
			if (ch0->type == LetterType::OT_Number) {
				m = ch0->number_val;
			}
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	}
	ClassObj* cls = (ClassObj*)*(Obj**)get_list(((ClassObj*)self)->extends_list, m);

	if (val2->type == LetterType::OT_Bracket) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		Block* block1 = (Block*)*(Obj**)get_list(cls->draw->children, 0);
		// Get varr from block1.vmapA.Values
		int varrCount = block1->vmapA->kvs->size;
		if (varrCount != blk->rets->size) return ErrorC(thgc, LetterType::OT_Error, val2->letter, "baseの引数があっていません");
		for (int i = 0; i < varrCount; i++) {
			KV* kv = *(KV**)get_list(block1->vmapA->kvs, i);
			Obj* varr_i = (Obj*)kv->value;
			Obj* ret_i = *(Obj**)get_list(blk->rets, i);
			// varr[i].opesC["="](...)
			vt_opeC(thgc, varr_i, LetterType::_Equal, local, ret_i);
		}
		return make_voival(thgc);
	}

	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSet2(thgc, local);
	}
	val2 = GetterC_vt(thgc, val2, local);
	if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;

	if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj || val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
		Block* block1 = (Block*)*(Obj**)get_list(cls->draw->children, 0);
		int varrCount = block1->vmapA->kvs->size;
		if (varrCount != 1) return ErrorC(thgc, LetterType::OT_Error, val2->letter, "base元のクラスの引数は1個ではありません");
		for (int i = 0; i < varrCount; i++) {
			KV* kv = *(KV**)get_list(block1->vmapA->kvs, i);
			Obj* varr_i = (Obj*)kv->value;
			vt_opeC(thgc, varr_i, LetterType::_Equal, local, val2);
		}
		(*n)++;
		return make_voival(thgc);
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "baseの引数が適切に設定されていません");
}

// ============================================================
// Variable::GetterC (lines 2127-2231)
// ============================================================
Obj* Variable_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	Variable* vself = (Variable*)self;
	LFunc* func = llvm_get_func(local->llvm);
	Obj* cls = vself->cls;

	// Check vari type "[n]"
	if (str_matchA(self->vari_c->type, "[n]", 3)) {
		self->vari_c->type = createString(thgc, (char*)"i8**", 4, 1);
		LVari* v = make_vn(thgc, "i8*", "v");
		add_list(thgc, func->comps, (char*)make_lload(thgc, v, self->vari_c, false));
		if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
		if (cls == local->Int) {
			LVari* v2 = make_vn(thgc, "i32", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_PtrToInt, v2, v));
			return make_value(thgc, cls, v2);
		} else if (cls == local->Bool) {
			LVari* v2 = make_vn(thgc, "i1", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_PtrToInt, v2, v));
			return make_value(thgc, cls, v2);
		} else if (cls->type == LetterType::OT_FuncType) {
			FuncTypeC* functype = (FuncTypeC*)cls;
			Obj* fnobj = make_function(thgc, LetterType::OT_Function, NULL);
			((Function*)fnobj)->rettype = functype->cls;
			fnobj->draws = (List*)functype->draws;
			fnobj->vari_c = v;
			return fnobj;
		} else if (cls->type == LetterType::OT_ArrType) {
			return make_value(thgc, cls, v);
		} else {
			self->vari_c = make_vn_s(thgc, model_type(thgc, cls->model, "*"), "vc");
			return make_value(thgc, cls, v);
		}
	}
	// Check vari type "[s]"
	else if (str_matchA(self->vari_c->type, "[s]", 3)) {
		self->vari_c->type = createString(thgc, (char*)"%HashType*", 10, 1);
		LVari* varig = make_vn(thgc, "%GetHashType", "va");
		add_list(thgc, func->comps, (char*)make_lload(thgc, varig, make_lvari_c(thgc, "%GetHashType*", "@GetHash"), false));
		LVari* v = make_vn_s(thgc, model_type(thgc, cls->model, "*"), "v");
		LCall* call = make_lcall(thgc, v, varig);
		lcall_add(thgc, call, self->vari_c);
		lcall_add(thgc, call, vself->strvari);
		add_list(thgc, func->comps, (char*)call);
		if (cls == local->Int) {
			LVari* v2 = make_vn(thgc, "i32", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_PtrToInt, v2, v));
			return make_value(thgc, cls, v2);
		} else if (cls == local->Bool) {
			LVari* v2 = make_vn(thgc, "i1", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_PtrToInt, v2, v));
			return make_value(thgc, cls, v2);
		} else if (cls->type == LetterType::OT_FuncType) {
			FuncTypeC* functype = (FuncTypeC*)cls;
			Obj* fnobj = make_function(thgc, LetterType::OT_Function, NULL);
			((Function*)fnobj)->rettype = functype->cls;
			fnobj->draws = (List*)functype->draws;
			fnobj->vari_c = v;
			return fnobj;
		} else if (cls->type == LetterType::OT_ArrType) {
			return make_value(thgc, cls, v);
		} else {
			return make_value(thgc, cls, v);
		}
	}
	// Check vari type "s[n]"
	else if (str_matchA(self->vari_c->type, "s[n]", 4)) {
		self->vari_c->type = createString(thgc, (char*)"%StringType*", 12, 1);
		LVari* varig = make_vn(thgc, "%GetCharType", "va");
		add_list(thgc, func->comps, (char*)make_lload(thgc, varig, make_lvari_c(thgc, "%GetCharType*", "@GetChar"), false));
		LVari* v = make_vn(thgc, "i16", "v");
		LCall* call = make_lcall(thgc, v, varig);
		lcall_add(thgc, call, self->vari_c);
		lcall_add(thgc, call, vself->strvari);
		add_list(thgc, func->comps, (char*)call);
		return make_value(thgc, cls, v);
	}

	// Normal path: flush local.comps
	for (int i = 0; i < local->compsC->size; i++) {
		LComp* c = *(LComp**)get_list(local->compsC, i);
		add_list(thgc, func->comps, (char*)c);
	}
	local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);

	if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
	LVari* vari = NULL;
	if (cls == local->Int) vari = make_vn(thgc, "i32", "va");
	else if (cls == local->Short) vari = make_vn(thgc, "i16", "va");
	else if (cls == local->Bool) vari = make_vn(thgc, "i1", "va");
	else if (cls->type == LetterType::OT_FuncType) {
		FuncTypeC* functype = (FuncTypeC*)cls;
		vari = make_vn(thgc, "%FuncType*", "va");
		add_list(thgc, func->comps, (char*)make_lload(thgc, vari, self->vari_c, false));
		Obj* fnobj = make_function(thgc, LetterType::OT_Function, NULL);
		((Function*)fnobj)->rettype = functype->cls;
		fnobj->draws = (List*)functype->draws;
		fnobj->vari_c = vari;
		return fnobj;
	}
	else if (cls->type == LetterType::OT_ArrType) {
		vari = make_vn(thgc, "%HashType*", "va");
		add_list(thgc, func->comps, (char*)make_lload(thgc, vari, self->vari_c, false));
		return make_value(thgc, cls, vari);
	}
	else {
		vari = make_vn_s(thgc, model_type(thgc, cls->model, "*"), "va");
	}
	add_list(thgc, func->comps, (char*)make_lload(thgc, vari, self->vari_c, false));
	return make_value(thgc, cls, vari);
}

// ============================================================
// Variable::SelfC (lines 2232-2247)
// ============================================================
Obj* Variable_SelfC(ThreadGC* thgc, Obj* self, Master* local) {
	Variable* vself = (Variable*)self;
	if (vself->clone) return self;
	LFunc* func = llvm_get_func(local->llvm);
	for (int i = 0; i < local->compsC->size; i++) {
		LComp* c = *(LComp**)get_list(local->compsC, i);
		add_list(thgc, func->comps, (char*)c);
	}
	local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
	Obj* cls = vself->cls;
	if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
	if (cls == local->Int) func->vari->type = createString(thgc, (char*)"i32*", 4, 1);
	else if (cls == local->Short) func->vari->type = createString(thgc, (char*)"i16*", 4, 1);
	else if (cls == local->Bool) func->vari->type = createString(thgc, (char*)"i1*", 3, 1);
	else if (cls->type == LetterType::OT_FuncType) func->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
	else if (cls->type == LetterType::OT_ArrType) func->vari->type = createString(thgc, (char*)"%HashType**", 11, 1);
	else {
		func->vari->type = model_type(thgc, cls->model, "**");
	}
	// Clone variable
	Obj* variable = make_variable(thgc, self->letter);
	((Variable*)variable)->cls = vself->cls;
	((Variable*)variable)->clone = vself->clone;
	variable->vari_c = func->vari;
	return variable;
}

// ============================================================
// Variable::PrimaryC (lines 2248-2251)
// ============================================================
Obj* Variable_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Variable* vself = (Variable*)self;
	// Value.New(cls, local, letter).PrimaryC(...)
	Obj* newval = make_value(thgc, vself->cls, NULL);
	newval->letter = self->letter;
	return PrimaryC_vt(thgc, newval, n, local, primary, val2);
}

// ============================================================
// Variable::EqualC (lines 2255-2346)
// ============================================================
Obj* Variable_EqualC(ThreadGC* thgc, Variable* self, LetterType op, Master* local, Obj* val2) {
	LFunc* func = llvm_get_func(local->llvm);
	Obj* cls = self->cls;

	// vari.type == "[n]"
	if (str_matchA(self->vari_c->type, "[n]", 3)) {
		((Obj*)self)->vari_c->type = createString(thgc, (char*)"i8**", 4, 1);
		if (cls == local->Int || cls == local->Short) {
			LVari* v2 = make_vn(thgc, "i8*", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_IntToPtr, v2, ((Value*)val2)->vari));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, ((Obj*)self)->vari_c, v2));
			return make_value(thgc, cls, v2);
		} else if (cls == local->Bool) {
			LVari* v2 = make_vn(thgc, "i8*", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_IntToPtr, v2, ((Value*)val2)->vari));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, ((Obj*)self)->vari_c, v2));
			return make_value(thgc, cls, v2);
		} else if (cls->type == LetterType::OT_FuncType) {
			add_list(thgc, func->comps, (char*)make_lstore(thgc, ((Obj*)self)->vari_c, ((Value*)val2)->vari));
			FuncTypeC* functype = (FuncTypeC*)cls;
			Obj* fnobj = make_function(thgc, LetterType::OT_Function, NULL);
			((Function*)fnobj)->rettype = functype->cls;
			fnobj->draws = (List*)functype->draws;
			fnobj->vari_c = ((Obj*)self)->vari_c;
			return fnobj;
		} else if (cls->type == LetterType::OT_ArrType) {
			add_list(thgc, func->comps, (char*)make_lstore(thgc, ((Obj*)self)->vari_c, ((Value*)val2)->vari));
			return make_value(thgc, cls, ((Obj*)self)->vari_c);
		} else {
			add_list(thgc, func->comps, (char*)make_lstore(thgc, ((Obj*)self)->vari_c, ((Value*)val2)->vari));
			return make_value(thgc, cls, ((Obj*)self)->vari_c);
		}
	}
	// vari.type == "[s]"
	else if (str_matchA(self->vari_c->type, "[s]", 3)) {
		((Obj*)self)->vari_c->type = createString(thgc, (char*)"%HashType*", 10, 1);
		LVari* varise = make_vn(thgc, "%SetHashType", "va");
		add_list(thgc, func->comps, (char*)make_lload(thgc, varise, make_lvari_c(thgc, "%SetHashType*", "@SetHash"), false));
		if (cls == local->Int || cls == local->Short) {
			LVari* v2 = make_vn(thgc, "i8*", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_IntToPtr, v2, ((Value*)val2)->vari));
			LCall* sc = make_lcall(thgc, NULL, varise);
			lcall_add(thgc, sc, ((Obj*)self)->vari_c);
			lcall_add(thgc, sc, self->strvari);
			lcall_add(thgc, sc, v2);
			add_list(thgc, func->comps, (char*)sc);
			return make_value(thgc, cls, v2);
		} else if (cls == local->Bool) {
			LVari* v2 = make_vn(thgc, "i8*", "v2");
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_IntToPtr, v2, ((Value*)val2)->vari));
			LCall* sc = make_lcall(thgc, NULL, varise);
			lcall_add(thgc, sc, ((Obj*)self)->vari_c);
			lcall_add(thgc, sc, self->strvari);
			lcall_add(thgc, sc, v2);
			add_list(thgc, func->comps, (char*)sc);
			return make_value(thgc, cls, v2);
		} else if (cls->type == LetterType::OT_FuncType) {
			add_list(thgc, func->comps, (char*)make_lstore(thgc, ((Obj*)self)->vari_c, ((Value*)val2)->vari));
			FuncTypeC* functype = (FuncTypeC*)cls;
			Obj* fnobj = make_function(thgc, LetterType::OT_Function, NULL);
			((Function*)fnobj)->rettype = functype->cls;
			fnobj->draws = (List*)functype->draws;
			fnobj->vari_c = ((Obj*)self)->vari_c;
			LCall* sc = make_lcall(thgc, NULL, varise);
			lcall_add(thgc, sc, ((Obj*)self)->vari_c);
			lcall_add(thgc, sc, self->strvari);
			lcall_add(thgc, sc, ((Value*)val2)->vari);
			add_list(thgc, func->comps, (char*)sc);
			return fnobj;
		} else if (cls->type == LetterType::OT_ArrType) {
			LCall* sc = make_lcall(thgc, NULL, varise);
			lcall_add(thgc, sc, ((Obj*)self)->vari_c);
			lcall_add(thgc, sc, self->strvari);
			lcall_add(thgc, sc, ((Value*)val2)->vari);
			add_list(thgc, func->comps, (char*)sc);
			return make_value(thgc, cls, ((Obj*)self)->vari_c);
		} else {
			LCall* sc = make_lcall(thgc, NULL, varise);
			lcall_add(thgc, sc, ((Obj*)self)->vari_c);
			lcall_add(thgc, sc, self->strvari);
			lcall_add(thgc, sc, ((Value*)val2)->vari);
			add_list(thgc, func->comps, (char*)sc);
			return make_value(thgc, cls, ((Obj*)self)->vari_c);
		}
	}
	// vari.type == "s[n]"
	else if (str_matchA(self->vari_c->type, "s[n]", 4)) {
		return ErrorC(thgc, LetterType::OT_Error, ((Obj*)self)->letter, "文字列に代入はできません");
	}

	// Normal store path
	if (val2->type == LetterType::OT_Value) {
		LVari* va = BitcastCast(thgc, local, cls, ((Val*)val2)->cls, ((Value*)val2)->vari);
		add_list(thgc, func->comps, (char*)make_lstore(thgc, ((Obj*)self)->vari_c, va));
	} else {
		return ErrorC(thgc, LetterType::OT_Error, val2->letter, "変数への代入が不正です");
	}
	// Clone
	Obj* variable = make_variable(thgc, ((Obj*)self)->letter);
	((Variable*)variable)->cls = self->cls;
	((Variable*)variable)->clone = self->clone;
	variable->vari_c = ((Obj*)self)->vari_c;
	return variable;
}

// ============================================================
// Variable::InC (lines 2347-2381)
// ============================================================
Obj* Variable_InC(ThreadGC* thgc, Variable* self, LetterType op, Master* local, Obj* val2) {
	Obj* cls = self->cls;
	if (cls->type == LetterType::OT_Var) {
		Var* varobj = (Var*)cls;
		if (((Type*)varobj)->cls == NULL) return ErrorC(thgc, LetterType::OT_Wait, ((Obj*)self)->letter, "varの型が判明しません");
	}
head:
	if (val2->type == LetterType::OT_Bracket) {
		add_list(thgc, local->calls, (char*)KouhoSet2);
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* block = (Block*)val;
		if (block->rets->size == 1) {
			val2 = *(Obj**)get_list(block->rets, 0);
			goto head;
		} else {
			return ErrorC(thgc, LetterType::OT_Error, val2->letter, "()の中の数が間違っています。");
		}
	}
	val2 = GetterC_vt(thgc, val2, local);
	if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	if (val2->type == LetterType::OT_Block) {
		Obj* it = make_iterator_blk(thgc, -1, (Block*)val2);
		((Obj*)self)->value = val2;
		return it;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Iteratorの代入が不正です");
}

// ============================================================
// Variable::opeC dispatcher
// ============================================================
Obj* Variable_opeC(ThreadGC* thgc, Obj* self, LetterType key, Master* local, Obj* val2) {
	Variable* vself = (Variable*)self;
	if (key == LetterType::_EqualEqual || key == LetterType::_Colon) return Variable_EqualC(thgc, vself, key, local, val2);
	if (key == LetterType::_Equal) return Variable_EqualC(thgc, vself, key, local, val2);
	if (key == LetterType::_In) return Variable_InC(thgc, vself, key, local, val2);
	// Other operators: delegate to Value via GetterC
	Obj* val1 = GetterC_vt(thgc, self, local);
	if (val1->type == LetterType::OT_Wait || val1->type == LetterType::OT_Error || val1->type == LetterType::OT_NG) return val1;
	return vt_opeC(thgc, val1, key, local, val2);
}

// ============================================================
// Function::GetterC (lines 2393-2411)
// ============================================================
Obj* Function_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	LFunc* func = llvm_get_func(local->llvm);
	for (int i = 0; i < local->compsC->size; i++) {
		LComp* c = *(LComp**)get_list(local->compsC, i);
		add_list(thgc, func->comps, (char*)c);
	}
	local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
	LVari* fv = make_vn(thgc, "%FuncType*", "fv");
	if (self->draws == NULL) {
		func->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
		add_list(thgc, func->comps, (char*)make_lload(thgc, fv, func->vari, false));
	} else {
		add_list(thgc, func->comps, (char*)make_lload(thgc, fv, self->vari_c, false));
	}
	self->vari_c = fv;
	return self;
}

// ============================================================
// Function::PrimaryC (lines 2412-2632)
// ============================================================
Obj* Function_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	LFunc* func = llvm_get_func(local->llvm);
	Function* fself = (Function*)self;

	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
		LVari* vari;
		Obj* ret = ((Function*)self)->rettype;
		if (ret->type == LetterType::OT_Var) ret = ((Type*)ret)->cls;
		if (ret == local->Int) vari = make_vn(thgc, "i32", "va");
		else if (ret == local->Bool) vari = make_vn(thgc, "i2", "va");
		else if (ret->type == LetterType::OT_FuncType) {
			vari = make_vn(thgc, "%FuncType*", "va");
		}
		else {
			vari = make_vn_s(thgc, model_type(thgc, ret->model, "*"), "va");
		}
		Block* blk = (Block*)val;
		(*n)++;

		if (self->draws == NULL) {
			// No draws - use model/drawcall
			String* mtype = model_type(thgc, self->model, "*");
			LVari* go_v = make_vn_s(thgc, mtype, "go");
			LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, mtype, self->drawcall));
			lcall_add(thgc, go_call, self->vari_c);
			Block* drawblk = (Block*)*(Obj**)get_list(((Function*)self)->draw->children, 0);
			for (int i = 0; i < drawblk->vmapA->kvs->size; i++) {
				KV* kv = *(KV**)get_list(drawblk->vmapA->kvs, i);
				Obj* varray_i = (Obj*)kv->value;
				if (varray_i->type == LetterType::OT_Variable && i < blk->rets->size) {
					Obj* ret_i = *(Obj**)get_list(blk->rets, i);
					if (ret_i->type == LetterType::OT_Value) {
						Variable* variable = (Variable*)varray_i;
						if (((Val*)ret_i)->cls == variable->cls) {
							lcall_add(thgc, go_call, ((Value*)ret_i)->vari);
						} else {
							if (variable->cls == local->Int) {
								LVari* v = make_vn(thgc, "i32", "v");
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)ret_i)->vari));
								lcall_add(thgc, go_call, v);
							} else if (variable->cls == local->Short) {
								LVari* v = make_vn(thgc, "i16", "v");
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)ret_i)->vari));
								lcall_add(thgc, go_call, v);
							} else if (variable->cls == local->Bool) {
								LVari* v = make_vn(thgc, "i1", "v");
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)ret_i)->vari));
								lcall_add(thgc, go_call, v);
							} else {
								lcall_add(thgc, go_call, ((Value*)ret_i)->vari);
							}
						}
					}
				}
			}
			LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
			add_list(thgc, func->comps, (char*)go_call);
			LCall* init_call = make_lcall(thgc, vari, make_lvari_c(thgc, vari->type ? (char*)vari->type->data : "void", self->call_c ? self->call_c->data : ""));
			lcall_add(thgc, init_call, rn4);
			lcall_add(thgc, init_call, go_v);
			add_list(thgc, func->comps, (char*)init_call);
		} else {
			// Has draws - use FuncType gete
			LVari* dv = make_vn(thgc, "i8**", "dv");
			LGete* gete = make_lgete_idx2(thgc, "%FuncType", dv, self->vari_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1"));
			add_list(thgc, func->comps, (char*)gete);
			LVari* dv2 = make_vn(thgc, "i8*", "dv");
			add_list(thgc, func->comps, (char*)make_lload(thgc, dv2, dv, false));
			LVari* go_v = make_vn(thgc, "i8*", "go");
			LCall* go_call = make_lcall(thgc, go_v, dv2);
			lcall_add(thgc, go_call, self->vari_c);
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* ret_i = *(Obj**)get_list(blk->rets, i);
				if (ret_i->type == LetterType::OT_Value) {
					Obj* draws_i = *(Obj**)get_list(self->draws, i);
					if (((Val*)ret_i)->cls == draws_i) {
						lcall_add(thgc, go_call, ((Value*)ret_i)->vari);
					} else {
						if (draws_i == local->Int) {
							LVari* v = make_vn(thgc, "i32", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)ret_i)->vari));
							lcall_add(thgc, go_call, v);
						} else if (draws_i == local->Short) {
							LVari* v = make_vn(thgc, "i16", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)ret_i)->vari));
							lcall_add(thgc, go_call, v);
						} else if (draws_i == local->Bool) {
							LVari* v = make_vn(thgc, "i1", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)ret_i)->vari));
							lcall_add(thgc, go_call, v);
						} else {
							lcall_add(thgc, go_call, ((Value*)ret_i)->vari);
						}
					}
				}
			}
			add_list(thgc, func->comps, (char*)go_call);
			LVari* cv = make_vn(thgc, "i8**", "cv");
			LGete* cgete = make_lgete_idx2(thgc, "%FuncType", cv, self->vari_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2"));
			add_list(thgc, func->comps, (char*)cgete);
			LVari* cv2 = make_vn(thgc, "i8*", "cv");
			add_list(thgc, func->comps, (char*)make_lload(thgc, cv2, cv, false));
			LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
			LCall* init_call = make_lcall(thgc, vari, cv2);
			lcall_add(thgc, init_call, rn4);
			lcall_add(thgc, init_call, go_v);
			add_list(thgc, func->comps, (char*)init_call);
		}
		return make_value(thgc, ret, vari);
	}

	// No bracket - walk primary children
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
	}
	if (val2 != NULL) {
		val2 = GetterC_vt(thgc, val2, local);
		if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj || val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value) {
			(*n)++;
			LVari* vari;
			Obj* ret = ((Function*)self)->rettype;
			if (ret->type == LetterType::OT_Var) ret = ((Type*)ret)->cls;
			if (ret == local->Int) vari = make_vn(thgc, "i32", "va");
			else if (ret == local->Bool) vari = make_vn(thgc, "i2", "va");
			else if (ret->type == LetterType::OT_FuncType) {
				vari = make_vn(thgc, "%FuncType*", "va");
			}
			else {
				vari = make_vn_s(thgc, model_type(thgc, ret->model, "*"), "va");
			}

			if (self->draws == NULL) {
				String* mtype = model_type(thgc, self->model, "*");
				LVari* go_v = make_vn_s(thgc, mtype, "go");
				LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, mtype, self->drawcall ? self->drawcall : createString(thgc, (char*)"", 0, 1)));
				lcall_add(thgc, go_call, self->vari_c);
				Block* drawblk = (Block*)*(Obj**)get_list(((Function*)self)->draw->children, 0);
				KV* kv0 = *(KV**)get_list(drawblk->vmapA->kvs, 0);
				Variable* vv = (Variable*)kv0->value;
				if (val2->type == LetterType::OT_Value && vv->cls == ((Val*)val2)->cls) {
					lcall_add(thgc, go_call, ((Value*)val2)->vari);
				} else {
					if (vv->cls == local->Int) {
						LVari* v = make_vn(thgc, "i32", "v");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
						lcall_add(thgc, go_call, v);
					} else if (vv->cls == local->Short) {
						LVari* v = make_vn(thgc, "i16", "v");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
						lcall_add(thgc, go_call, v);
					} else if (vv->cls == local->Bool) {
						LVari* v = make_vn(thgc, "i1", "v");
						add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
						lcall_add(thgc, go_call, v);
					} else {
						lcall_add(thgc, go_call, ((Value*)val2)->vari);
					}
				}
				add_list(thgc, func->comps, (char*)go_call);
				LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
				LCall* init_call = make_lcall(thgc, vari, make_lvari_c(thgc, vari->type ? (char*)vari->type->data : "void", self->call_c ? self->call_c->data : ""));
				lcall_add(thgc, init_call, rn4);
				lcall_add(thgc, init_call, go_v);
				add_list(thgc, func->comps, (char*)init_call);
			} else {
				LVari* dv = make_vn(thgc, "i8**", "dv");
				LGete* gete = make_lgete_idx2(thgc, "%FuncType", dv, self->vari_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1"));
				add_list(thgc, func->comps, (char*)gete);
				LVari* dv2 = make_vn(thgc, "i8*", "dv");
				add_list(thgc, func->comps, (char*)make_lload(thgc, dv2, dv, false));
				LVari* go_v = make_vn(thgc, "i8*", "go");
				LCall* go_call = make_lcall(thgc, go_v, dv2);
				lcall_add(thgc, go_call, self->vari_c);
				if (val2->type == LetterType::OT_Value) {
					Obj* draws0 = *(Obj**)get_list(self->draws, 0);
					if (((Val*)val2)->cls == draws0) {
						lcall_add(thgc, go_call, ((Value*)val2)->vari);
					} else {
						if (draws0 == local->Int) {
							LVari* v = make_vn(thgc, "i32", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
							lcall_add(thgc, go_call, v);
						} else if (draws0 == local->Short) {
							LVari* v = make_vn(thgc, "i16", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
							lcall_add(thgc, go_call, v);
						} else if (draws0 == local->Bool) {
							LVari* v = make_vn(thgc, "i1", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
							lcall_add(thgc, go_call, v);
						} else {
							lcall_add(thgc, go_call, ((Value*)val2)->vari);
						}
					}
				} else {
					return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の引数が適切ではありません");
				}
				add_list(thgc, func->comps, (char*)go_call);
				LVari* cv = make_vn(thgc, "i8**", "cv");
				LGete* cgete = make_lgete_idx2(thgc, "%FuncType", cv, self->vari_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2"));
				add_list(thgc, func->comps, (char*)cgete);
				LVari* cv2 = make_vn(thgc, "i8*", "cv");
				add_list(thgc, func->comps, (char*)make_lload(thgc, cv2, cv, false));
				LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
				LCall* init_call = make_lcall(thgc, vari, cv2);
				lcall_add(thgc, init_call, rn4);
				lcall_add(thgc, init_call, go_v);
				add_list(thgc, func->comps, (char*)init_call);
			}
			if (ret->type == LetterType::OT_FuncType) {
				FuncTypeC* functype = (FuncTypeC*)ret;
				Obj* fnobj = make_function(thgc, LetterType::OT_Function, NULL);
				((Function*)fnobj)->rettype = functype->cls;
				fnobj->draws = (List*)functype->draws;
				fnobj->vari_c = vari;
				return fnobj;
			}
			return make_value(thgc, ret, vari);
		}
	}
	return ErrorC(thgc, LetterType::OT_NG, val2 ? val2->letter : NULL, "()のような関数に適切な引数が来ていません");
}

// ============================================================
// Helper: process vmapA for typedec/funccheck/funcdraw/funcdec
// Used by Function::Dec, If::PrimaryC, While::PrimaryC, For::PrimaryC
// ============================================================
static void process_vmapA_block(ThreadGC* thgc, Master* local, Map* vmapA,
	LTypeDec* typedec, LFunc* funccheck, LVari* co_val, LVari* thgc2, LVari* i8p,
	String* model_str, int* order, int* count, List* decs,
	LFunc* funcdraw, LVari* go_c, LFunc* funcdec, LVari* objptr3, LVari* go_v_dec,
	bool is_draw_block)
{
	LFunc* func = llvm_get_func(local->llvm);
	for (int ki = 0; ki < vmapA->kvs->size; ki++) {
		KV* kv = *(KV**)get_list(vmapA->kvs, ki);
		Obj* v = (Obj*)kv->value;
		if (v->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)v;
			v->order = (*order)++;
			Obj* cls = variable->cls;
			if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
			variable->cls = cls;
			if (cls == local->Int) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i32", 3, 1), kv->key));
				*count += 8;
				if (is_draw_block && funcdraw) {
					LVari* vdraw = make_vn(thgc, "i32", "vd");
					add_list(thgc, funcdraw->draws, (char*)vdraw);
					char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
					LVari* varii = make_vn(thgc, "i32*", "va");
					add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2_s(thgc, model_str, varii, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
					add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii, vdraw));
				}
			} else if (cls == local->Bool) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i1", 2, 1), kv->key));
				*count += 8;
				if (is_draw_block && funcdraw) {
					LVari* vdraw = make_vn(thgc, "i1", "vd");
					add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, "i32", "dr"));
					char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
					LVari* varii = make_vn(thgc, "i1*", "va");
					add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2_s(thgc, model_str, varii, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
					add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii, vdraw));
				}
			} else {
				SetIdentity(thgc, (Obj*)(cls));
				String* mtype = model_type(thgc, cls->model, "*");
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, mtype, kv->key));
				*count += 8;
				if (is_draw_block && funcdraw) {
					LVari* vdraw = make_vn_s(thgc, mtype, "vd");
					add_list(thgc, funcdraw->draws, (char*)vdraw);
					char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
					LVari* varii = make_vn_s(thgc, mtype, "vi");
					add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2_s(thgc, model_str, varii, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
					add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii, vdraw));
				}
				// GC check
				LVari* vari = make_vn(thgc, "i8*", "va");
				char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
				add_list(thgc, funccheck->comps, (char*)make_lgete_idx2_s(thgc, model_str, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
				LVari* vari2 = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				LVari* co_cval = make_vn(thgc, "i8*", "co");
				LCall* co_call = make_lcall(thgc, co_cval, co_val);
				lcall_add(thgc, co_call, thgc2);
				lcall_add(thgc, co_call, vari);
				add_list(thgc, funccheck->comps, (char*)co_call);
				add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			}
		}
		else if (v->type == LetterType::OT_Function) {
			Function* f = (Function*)v;
			SetIdentity(thgc, (Obj*)(f));
			v->order = (*order)++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));
			if (is_draw_block && funcdraw) {
				add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, "%FuncType*", "dr"));
			}
			// GC check
			LVari* vari = make_vn(thgc, "i8*", "va");
			char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2_s(thgc, model_str, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
			LVari* vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			LVari* co_cval = make_vn(thgc, "i8*", "co");
			LCall* co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			*count += 8;

			// Function dec + init calls
			if (funcdec) {
				const char* fname = f->letter ? f->letter->text->data : "f";
				LVari* va0 = make_vn(thgc, "%FuncType*", "va");
				LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", fname, f->identity)));
				lcall_add(thgc, fu_call, go_v_dec ? go_v_dec : go_c);
				lcall_add(thgc, fu_call, f->drawcall ? make_lvari_s(thgc, "i8*", f->drawcall) : make_lvari_c(thgc, "i8*", ""));
				lcall_add(thgc, fu_call, f->call_c ? make_lvari_s(thgc, "i8*", f->call_c) : make_lvari_c(thgc, "i8*", ""));
				add_list(thgc, funcdec->comps, (char*)fu_call);
				LVari* va2 = make_vn_s(thgc, model_type(thgc, model_str, "*"), "va");
				add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
				LVari* va = make_vn(thgc, "%FuncType*", "va");
				add_list(thgc, funcdec->comps, (char*)make_lgete_idx2_s(thgc, model_str, va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
				add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
			}
			if (is_draw_block && funcdraw) {
				const char* fname = f->letter ? f->letter->text->data : "f";
				LVari* va0 = make_vn(thgc, "%FuncType*", "va");
				LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", fname, f->identity)));
				lcall_add(thgc, fu_call, go_c);
				lcall_add(thgc, fu_call, f->drawcall ? make_lvari_s(thgc, "i8*", f->drawcall) : make_lvari_c(thgc, "i8*", ""));
				lcall_add(thgc, fu_call, f->call_c ? make_lvari_s(thgc, "i8*", f->call_c) : make_lvari_c(thgc, "i8*", ""));
				add_list(thgc, funcdraw->comps, (char*)fu_call);
				LVari* va = make_vn(thgc, "%FuncType*", "va");
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2_s(thgc, model_str, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
			}
		}
		else if (v->type == LetterType::OT_ClassObj) {
			ClassObj* f = (ClassObj*)v;
			SetIdentity(thgc, (Obj*)(f));
			v->order = (*order)++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"%FuncType*", 10, 1), kv->key));
			if (is_draw_block && funcdraw) {
				add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, "%FuncType*", "dr"));
			}
			// GC check
			LVari* vari = make_vn(thgc, "i8*", "va");
			char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", v->order);
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2_s(thgc, model_str, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
			LVari* vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			LVari* co_cval = make_vn(thgc, "i8*", "co");
			LCall* co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			*count += 8;

			if (funcdec) {
				const char* fname = f->letter ? f->letter->text->data : "f";
				LVari* va0 = make_vn(thgc, "%FuncType*", "va");
				LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", fname, f->identity)));
				lcall_add(thgc, fu_call, go_v_dec ? go_v_dec : go_c);
				lcall_add(thgc, fu_call, f->drawcall ? make_lvari_s(thgc, "i8*", f->drawcall) : make_lvari_c(thgc, "i8*", ""));
				lcall_add(thgc, fu_call, f->call_c ? make_lvari_s(thgc, "i8*", f->call_c) : make_lvari_c(thgc, "i8*", ""));
				add_list(thgc, funcdec->comps, (char*)fu_call);
				LVari* va2 = make_vn_s(thgc, model_type(thgc, model_str, "*"), "va");
				add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
				LVari* va = make_vn(thgc, "%FuncType*", "va");
				add_list(thgc, funcdec->comps, (char*)make_lgete_idx2_s(thgc, model_str, va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
				add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));
			}
			if (is_draw_block && funcdraw) {
				const char* fname = f->letter ? f->letter->text->data : "f";
				LVari* va0 = make_vn(thgc, "%FuncType*", "va");
				LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", fname, f->identity)));
				lcall_add(thgc, fu_call, go_c);
				lcall_add(thgc, fu_call, f->drawcall ? make_lvari_s(thgc, "i8*", f->drawcall) : make_lvari_c(thgc, "i8*", ""));
				lcall_add(thgc, fu_call, f->call_c ? make_lvari_s(thgc, "i8*", f->call_c) : make_lvari_c(thgc, "i8*", ""));
				add_list(thgc, funcdraw->comps, (char*)fu_call);
				LVari* va = make_vn(thgc, "%FuncType*", "va");
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2_s(thgc, model_str, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
			}
		}
	}
}

// ============================================================
// Function::Dec (lines 2646-3149) - large method
// ============================================================
void Function_Dec(ThreadGC* thgc, Obj* self, Master* local) {
	List* blocks = get_blocks(local);
	Function* fself = (Function*)self;
	SetIdentity(thgc, (Obj*)(fself));
	self->ifv = create_mapy(thgc, CType::_KV);

	Obj* ret = ((Function*)self)->rettype;
	if (ret->type == LetterType::OT_Var) ret = ((Type*)ret)->cls;
	String* type_str;
	if (ret == local->Int) type_str = createString(thgc, (char*)"i32", 3, 1);
	else if (ret == local->Bool) type_str = createString(thgc, (char*)"i1", 2, 1);
	else {
		type_str = model_type(thgc, ret->model, "*");
	}

	self->n_c = obj_cn++;
	self->vari_c = make_lvari(thgc, createString(thgc, (char*)"void", 4, 1), self->call_c);

	// Create main function
	LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* obj_v = make_lvari_sc(thgc, model_type(thgc, self->model, "*"), "%obj");
	LFunc* lfunc = make_lfunc(thgc, local->llvm, self->vari_c);
	add_list(thgc, lfunc->draws, (char*)rn4);
	add_list(thgc, lfunc->draws, (char*)obj_v);
	add_list(thgc, local->llvm->comps, (char*)lfunc);
	add_list(thgc, local->llvm->funcs, (char*)lfunc);

	// thgc setup
	LVari* thgcptr4 = make_lvari_c(thgc, "%ThreadGCType**", "%thgcptr");
	add_list(thgc, lfunc->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", thgcptr4, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, lfunc->comps, (char*)make_lload(thgc, thgc4, thgcptr4, false));

	// Alloca + Store + SetRoot
	LVari* objptr = make_lvari_sc(thgc, model_type(thgc, self->model, "**"), "%objptr");
	add_list(thgc, lfunc->comps, (char*)make_lalloca(thgc, objptr));
	add_list(thgc, lfunc->comps, (char*)make_lstore(thgc, objptr, obj_v));
	LVari* srv = make_vn(thgc, "%GC_SetRootType", "sr");
	add_list(thgc, lfunc->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
	LCall* srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, rn4);
	lcall_add(thgc, srcall, objptr);
	add_list(thgc, lfunc->comps, (char*)srcall);

	// Create draw function
	LVari* rn5 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* blk_v = make_lvari_c(thgc, "i8*", "%fptr1");
	LFunc* funcdraw = make_lfunc(thgc, local->llvm, make_lvari(thgc, model_type(thgc, self->model, "*"), self->drawcall));
	add_list(thgc, funcdraw->draws, (char*)rn5);
	add_list(thgc, funcdraw->draws, (char*)blk_v);
	add_list(thgc, local->llvm->comps, (char*)funcdraw);

	// TypeDec
	LTypeDec* typedec = make_ltypedec(thgc, self->model, NULL, false);
	add_list(thgc, local->llvm->types, (char*)typedec);
	add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i8*", 3, 1), createString(thgc, (char*)"blk", 3, 1)));

	// Check function
	LVari* thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* i8p = make_lvari_c(thgc, "i8*", "%self");
	String* checkname = fmt_str(thgc, "@%sCheck%d", self->letter->text->data, fself->identity);
	LFunc* funccheck = make_lfunc(thgc, local->llvm, make_lvari(thgc, createString(thgc, (char*)"void", 4, 1), checkname));
	add_list(thgc, funccheck->draws, (char*)thgc2);
	add_list(thgc, funccheck->draws, (char*)i8p);
	add_list(thgc, local->llvm->comps, (char*)funccheck);

	LVari* vari = make_vn(thgc, "i8*", "va");
	add_list(thgc, funccheck->comps, (char*)make_lgete_idx2_s(thgc, self->model, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* co_val = make_lvari_c(thgc, "%CopyObjectType", "%co");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, co_val, make_lvari_c(thgc, "%CopyObjectType*", "@CopyObject"), false));
	LVari* vari2 = make_vn(thgc, "i8*", "va");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
	LVari* co_cval = make_vn(thgc, "i8*", "co");
	LCall* co_call = make_lcall(thgc, co_cval, co_val);
	lcall_add(thgc, co_call, thgc2);
	lcall_add(thgc, co_call, vari);
	add_list(thgc, funccheck->comps, (char*)co_call);
	add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));

	// AddClass in main
	LVari* thgc3 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* ac_val = make_lvari_c(thgc, "%GC_AddClassType", "%addclass");
	LVari* countv = make_lvari_c(thgc, "i32", "0");
	int namelen = (int)self->letter->text->size;
	LStrV* strv = make_lstrv(thgc,
		self->call_c,
		self->letter->text, namelen);
	add_list(thgc, local->llvm->strs, (char*)strv);
	int cn_val = local->llvm->cn++;
	self->n_c = cn_val;
	LVari* typ = make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", cn_val));
	LVari* tnp = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tnp%d", llvm_n++));
	add_list(thgc, local->llvm->main_func->comps, (char*)make_lload(thgc, tnp, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%cv%d", llvm_n++));
	add_list(thgc, local->llvm->main_func->comps, (char*)make_lbinop(thgc, LBOP_Add, tv, tnp, typ));
	LCall* ac_call = make_lcall(thgc, NULL, ac_val);
	lcall_add(thgc, ac_call, thgc3);
	lcall_add(thgc, ac_call, (LVari*)strv);
	lcall_add(thgc, ac_call, countv);
	lcall_add(thgc, ac_call, make_lvari_s(thgc, "%GCCheckFuncType", checkname));
	lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCFinalizeFuncType", "null"));
	add_list(thgc, local->llvm->main_func->comps, (char*)ac_call);

	// funcdraw body: thgc setup + CloneObject
	LVari* thgcptr5 = make_lvari_c(thgc, "%ThreadGCType**", "%thgcptr");
	add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", thgcptr5, rn5, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* thgc5 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, thgc5, thgcptr5, false));
	LVari* go_val = make_lvari_c(thgc, "%CopyObjectType", "%gcobject");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, go_val, make_lvari_c(thgc, "%CopyObjectType*", "@CloneObject"), false));
	LVari* v3 = make_vn(thgc, "i8**", "v3");
	add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, "%FuncType", v3, blk_v, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
	LVari* v4 = make_vn(thgc, "i8*", "v4");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, v4, v3, false));
	LVari* go_v = make_lvari_c(thgc, "i8*", "%obj");
	LCall* go_call_draw = make_lcall(thgc, go_v, go_val);
	lcall_add(thgc, go_call_draw, thgc5);
	lcall_add(thgc, go_call_draw, v4);
	add_list(thgc, funcdraw->comps, (char*)go_call_draw);
	LVari* go_c = make_vn_s(thgc, model_type(thgc, self->model, "*"), "go");
	add_list(thgc, funcdraw->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c, go_v));

	// funcdec function
	LVari* rn6 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* block_v = make_lvari_c(thgc, "i8*", "%block");
	LVari* funcptr = make_lvari_c(thgc, "i8*", "%fptr");
	LVari* funcptr2 = make_lvari_c(thgc, "i8*", "%fptr2");
	String* decname = fmt_str(thgc, "@%sdec%d", self->letter->text->data, fself->identity);
	LFunc* funcdec = make_lfunc(thgc, local->llvm, make_lvari_s(thgc, "%FuncType*", decname));
	add_list(thgc, funcdec->draws, (char*)rn6);
	add_list(thgc, funcdec->draws, (char*)block_v);
	add_list(thgc, funcdec->draws, (char*)funcptr);
	add_list(thgc, funcdec->draws, (char*)funcptr2);
	add_list(thgc, local->llvm->comps, (char*)funcdec);
	LVari* thgc6 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, thgc6, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));

	// GC_malloc for FuncType
	LVari* gmvari = make_lvari_c(thgc, "%GC_mallocType", "%gm");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, gmvari, make_lvari_c(thgc, "%GC_mallocType*", "@GC_malloc"), false));
	LVari* tmp28 = make_lvari_c(thgc, "i32", "28");
	LVari* go_v_dec = make_lvari_c(thgc, "i8*", "%obj");
	LCall* gc_call = make_lcall(thgc, go_v_dec, gmvari);
	lcall_add(thgc, gc_call, thgc6);
	lcall_add(thgc, gc_call, tmp28);
	add_list(thgc, funcdec->comps, (char*)gc_call);
	LVari* go_c3 = make_vn(thgc, "%FuncType*", "go");
	add_list(thgc, funcdec->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c3, go_v_dec));

	// Set FuncType fields
	LVari* vc0 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc0, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc0, block_v));
	LVari* vc1 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc1, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc1, funcptr));
	LVari* vc2 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc2, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc2, funcptr2));

	// Alloca + SetRoot for FuncType
	LVari* objptr2 = make_lvari_c(thgc, "%FuncType**", "%objptr");
	add_list(thgc, funcdec->comps, (char*)make_lalloca(thgc, objptr2));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, objptr2, go_c3));
	LVari* srv2 = make_vn(thgc, "%GC_SetRootType", "sr");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, srv2, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
	LCall* srcall2 = make_lcall(thgc, NULL, srv2);
	lcall_add(thgc, srcall2, rn6);
	lcall_add(thgc, srcall2, objptr2);
	add_list(thgc, funcdec->comps, (char*)srcall2);

	// GC_malloc for this type
	LVari* go_v2 = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%obj%d", llvm_n++));
	LVari* tmp_n = make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", self->n_c));
	LVari* tmi = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmi%d", llvm_n++));
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, tmi, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tmv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmv%d", llvm_n++));
	add_list(thgc, funcdec->comps, (char*)make_lbinop(thgc, LBOP_Add, tmv, tmi, tmp_n));
	LCall* gc_call2 = make_lcall(thgc, go_v2, gmvari);
	lcall_add(thgc, gc_call2, thgc6);
	lcall_add(thgc, gc_call2, tmv);
	add_list(thgc, funcdec->comps, (char*)gc_call2);
	LVari* go_c2 = make_vn_s(thgc, model_type(thgc, self->model, "*"), "go");
	add_list(thgc, funcdec->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c2, go_v2));

	// Set blk field on typed object
	LVari* vc3 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2_s(thgc, self->model, vc3, go_c2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc3, block_v));

	// Alloca + SetRoot for typed object
	LVari* objptr3 = make_vn_s(thgc, model_type(thgc, self->model, "**"), "op");
	add_list(thgc, funcdec->comps, (char*)make_lalloca(thgc, objptr3));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, objptr3, go_c2));
	LCall* srcall3 = make_lcall(thgc, NULL, srv2);
	lcall_add(thgc, srcall3, thgc6);
	lcall_add(thgc, srcall3, objptr3);
	add_list(thgc, funcdec->comps, (char*)srcall3);

	// Set parent pointer
	LVari* vc4 = make_vn_s(thgc, model_type(thgc, self->model, "**"), "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc4, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc4, go_c2));

	// Process vmapA for draw block (blk0) and body block (blk2)
	int count = 8;
	Block* blk0 = (Block*)*(Obj**)get_list(((Function*)self)->draw->children, 0);
	int order = 1;
	Block* blk2 = (Block*)*(Obj**)get_list(((Function*)self)->draw->children, 1);
	List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);

	process_vmapA_block(thgc, local, blk0->vmapA, typedec, funccheck, co_val, thgc2, i8p,
		self->model, &order, &count, decs, funcdraw, go_c, funcdec, objptr3, block_v, true);
	process_vmapA_block(thgc, local, blk2->vmapA, typedec, funccheck, co_val, thgc2, i8p,
		self->model, &order, &count, decs, NULL, NULL, funcdec, objptr3, go_v_dec, false);

	// Dec all collected decs
	for (int i = 0; i < decs->size; i++) {
		Obj* d = *(Obj**)get_list(decs, i);
		Dec_vt(thgc, d, local);
	}
	add_list(thgc, local->llvm->funcs, (char*)funcdec);
	self->bas = objptr2;
	add_list(thgc, blocks, (char*)blk0);
	Block_exeC(thgc, (Obj*)blk0, local);
	local->llvm->funcs->size--;
	add_list(thgc, blocks, (char*)blk2);

	// funcdraw ret
	add_list(thgc, funcdraw->comps, (char*)make_lret(thgc, go_c));
	char countbuf[32]; snprintf(countbuf, sizeof(countbuf), "%d", count);
	countv->name = createString(thgc, countbuf, (int)strlen(countbuf), 1);
	add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

	// funcdec ret
	LVari* vv = make_vn(thgc, "%FuncType*", "vv");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, vv, objptr2, false));

	LVari* rnpv = make_lvari_c(thgc, "i32*", "%ptr");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn6, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* rnpv12 = make_lvari_c(thgc, "i32", "%v");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
	LVari* rnpv2 = make_lvari_c(thgc, "i32", "%dec");
	add_list(thgc, funcdec->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, rnpv, rnpv2));
	add_list(thgc, funcdec->comps, (char*)make_lret(thgc, go_c3));

	// Execute body block
	self->bas = objptr;
	LLab* lab_entry = make_llab(thgc, createString(thgc, (char*)"entry", 5, 1));
	add_list(thgc, local->labs, (char*)lab_entry);
	Block_exeC(thgc, (Obj*)blk2, local);
	local->labs->size--;
	blocks->size--;
	blocks->size--;

	// Epilogue
	LFunc* curFunc = llvm_get_func(local->llvm);
	rnpv = make_lvari_c(thgc, "i32*", "%ptr");
	add_list(thgc, curFunc->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	rnpv12 = make_lvari_c(thgc, "i32", "%v");
	add_list(thgc, curFunc->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
	rnpv2 = make_lvari_c(thgc, "i32", "%dec");
	add_list(thgc, curFunc->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, curFunc->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

	if (blk2->rets && blk2->rets->size > 0) {
		Obj* valobj = *(Obj**)get_list(blk2->rets, blk2->rets->size - 1);
		if (valobj->type == LetterType::OT_Value) {
			LVari* va = BitcastCast(thgc, local, ret, ((Val*)valobj)->cls, ((Value*)valobj)->vari);
			add_list(thgc, curFunc->comps, (char*)make_lret(thgc, va));
		}
	}
	add_list(thgc, curFunc->comps, (char*)make_lret(thgc, blk2->rets && blk2->rets->size > 0 ? ((Value*)*(Obj**)get_list(blk2->rets, blk2->rets->size - 1))->vari : make_lvari_c(thgc, "void", "")));
	local->llvm->funcs->size--;
}

// ============================================================
// If::exepC (lines 3159-3166)
// ============================================================
Obj* If_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		// call last callback
		if (local->calls->size > 0) {
			typedef void (*CallbackFn)();
			CallbackFn cb = *(CallbackFn*)get_list(local->calls, local->calls->size - 1);
			if (cb) cb();
		}
	}
	return self;
}

// ============================================================
// Helper: setup scope block (GC alloc + type + check for if/elif/else/while/for)
// ============================================================
static void setup_scope_gc(ThreadGC* thgc, Master* local, Obj* self,
	const char* typename_str, const char* checkname,
	LTypeDec** out_typedec, LFunc** out_funccheck, LVari** out_co_val,
	LVari** out_thgc2, LVari** out_i8p, LVari** out_go_c, LVari** out_objptr3,
	int* out_count, int* out_order)
{
	List* blocks = get_blocks(local);
	LFunc* func = llvm_get_func(local->llvm);

	// TypeDec
	*out_typedec = make_ltypedec(thgc, createString(thgc, (char*)typename_str, (int)strlen(typename_str), 1), NULL, false);
	add_list(thgc, local->llvm->types, (char*)*out_typedec);

	// Funccheck
	*out_thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	*out_i8p = make_lvari_c(thgc, "i8*", "%self");
	*out_funccheck = make_lfunc(thgc, local->llvm, make_lvari_c(thgc, "void", (char*)checkname));
	add_list(thgc, (*out_funccheck)->draws, (char*)*out_thgc2);
	add_list(thgc, (*out_funccheck)->draws, (char*)*out_i8p);
	add_list(thgc, local->llvm->comps, (char*)*out_funccheck);

	add_list(thgc, ((LComp*)*out_typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i8*", 3, 1), createString(thgc, (char*)"blk", 3, 1)));

	// Check function body: gete + CopyObject
	LVari* vari = make_vn(thgc, "i8*", "va");
	add_list(thgc, (*out_funccheck)->comps, (char*)make_lgete_idx2(thgc, typename_str, vari, *out_i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	*out_co_val = make_lvari_c(thgc, "%CopyObjectType", "%co");
	add_list(thgc, (*out_funccheck)->comps, (char*)make_lload(thgc, *out_co_val, make_lvari_c(thgc, "%CopyObjectType*", "@CopyObject"), false));
	LVari* vari2 = make_vn(thgc, "i8*", "va");
	add_list(thgc, (*out_funccheck)->comps, (char*)make_lload(thgc, vari2, vari, false));
	LVari* co_cval = make_vn(thgc, "i8*", "co");
	LCall* co_call = make_lcall(thgc, co_cval, *out_co_val);
	lcall_add(thgc, co_call, *out_thgc2);
	lcall_add(thgc, co_call, vari);
	add_list(thgc, (*out_funccheck)->comps, (char*)co_call);
	add_list(thgc, (*out_funccheck)->comps, (char*)make_lstore(thgc, vari, co_cval));

	// AddClass in main
	LVari* thgc3 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* ac_val = make_lvari_c(thgc, "%GC_AddClassType", "%addclass");
	LVari* countv = make_lvari_c(thgc, "i32", "");
	char namebuf[128]; snprintf(namebuf, sizeof(namebuf), "scope%d", llvm_n++);
	LStrV* strv = make_lstrv(thgc,
		fmt_str(thgc, "@%s", namebuf),
		createString(thgc, namebuf, (int)strlen(namebuf), 1), (int)strlen(namebuf));
	add_list(thgc, local->llvm->strs, (char*)strv);
	int cn = local->llvm->cn++;
	self->n_c = cn;
	LVari* typ = make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", cn));
	LVari* tnp = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tnp%d", llvm_n++));
	add_list(thgc, local->llvm->main_func->comps, (char*)make_lload(thgc, tnp, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%cv%d", llvm_n++));
	add_list(thgc, local->llvm->main_func->comps, (char*)make_lbinop(thgc, LBOP_Add, tv, tnp, typ));
	LCall* ac_call = make_lcall(thgc, NULL, ac_val);
	lcall_add(thgc, ac_call, thgc3);
	lcall_add(thgc, ac_call, (LVari*)strv);
	lcall_add(thgc, ac_call, countv);
	lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCCheckFuncType", (char*)checkname));
	lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCFinalizeFuncType", "null"));
	add_list(thgc, local->llvm->main_func->comps, (char*)ac_call);

	// GC_malloc in current func
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* go_val = make_vn(thgc, "%GC_mallocType", "go");
	add_list(thgc, func->comps, (char*)make_lload(thgc, go_val, make_lvari_c(thgc, "%GC_mallocType*", "@GC_malloc"), false));
	LVari* go_v = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%obj%d", llvm_n++));
	LVari* tmp = make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", cn));
	LVari* tmi = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmi%d", llvm_n++));
	add_list(thgc, func->comps, (char*)make_lload(thgc, tmi, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tmv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmv%d", llvm_n++));
	add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Add, tmv, tmi, tmp));
	LCall* go_call = make_lcall(thgc, go_v, go_val);
	lcall_add(thgc, go_call, thgc4);
	lcall_add(thgc, go_call, tmv);
	add_list(thgc, func->comps, (char*)go_call);
	*out_go_c = make_vn(thgc, (char*)fmt_str(thgc, "%s*", typename_str)->data, "go");
	add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, *out_go_c, go_v));

	// Set parent pointer
	LVari* v0 = make_vn(thgc, "i8**", "v0");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, v0, *out_go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* gj = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%gj%d", llvm_n++));
	if (func->async_flag) {
		LVari* objf = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
		LVari* objp = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", objp, objf, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
		add_list(thgc, func->comps, (char*)make_lload(thgc, gj, objp, false));
	} else if (blocks->size > 0) {
		Block* lastblk = *(Block**)get_list(blocks, blocks->size - 1);
		Obj* lastobj = lastblk->obj ? (Obj*)lastblk->obj : NULL;
		if (lastobj && lastobj->type == LetterType::OT_IfBlock) {
			add_list(thgc, func->comps, (char*)make_lload(thgc, gj, lastobj->bas, false));
		} else if (lastobj) {
			add_list(thgc, func->comps, (char*)make_lload(thgc, gj, lastobj->bas, false));
		}
	}
	add_list(thgc, func->comps, (char*)make_lstore(thgc, v0, gj));

	// Alloca + SetRoot
	if (!func->async_flag) {
		*out_objptr3 = make_lvari(thgc, fmt_str(thgc, "%s**", typename_str), fmt_str(thgc, "%%objptr%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lalloca(thgc, *out_objptr3));
		add_list(thgc, func->comps, (char*)make_lstore(thgc, *out_objptr3, *out_go_c));
		self->bas = *out_objptr3;
		LVari* srv = make_vn(thgc, "%GC_SetRootType", "sr");
		add_list(thgc, func->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
		LCall* srcall = make_lcall(thgc, NULL, srv);
		lcall_add(thgc, srcall, thgc4);
		lcall_add(thgc, srcall, self->bas);
		add_list(thgc, func->comps, (char*)srcall);
	} else {
		LVari* objf = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
		*out_objptr3 = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", *out_objptr3, objf, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
		add_list(thgc, func->comps, (char*)make_lstore(thgc, *out_objptr3, *out_go_c));
	}
	*out_count = 8;
	*out_order = 1;
}

// ============================================================
// Helper: decrement root node pointer (rnp--)
// ============================================================
static void emit_rnp_dec(ThreadGC* thgc, Master* local) {
	LFunc* func = llvm_get_func(local->llvm);
	if (func->async_flag) {
		LVari* objp = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
		LVari* objf = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", objp, objf, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
		LVari* objv = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%objv%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lload(thgc, objv, objp, false));
		LVari* blockptr = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%objp%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%GCObject", blockptr, objv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
		LVari* blockv = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%objv%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lload(thgc, blockv, blockptr, false));
		add_list(thgc, func->comps, (char*)make_lstore(thgc, objp, blockv));
	} else {
		LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
		LVari* rnpv = make_lvari_c(thgc, "i32*", "%ptr");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
		LVari* rnpv12 = make_lvari_c(thgc, "i32", "%v");
		add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
		LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
		add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));
	}
}

// ============================================================
// If::PrimaryC (lines 3167-4370) - very large method
// ============================================================
Obj* If_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	LFunc* func = llvm_get_func(local->llvm);
	Obj* ret = ((Function*)self)->rettype;

	if (val2->type == LetterType::OT_CallBlock) {
		Block* blk0 = (Block*)*(Obj**)get_list(val2->children, 0);
		Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);

		if (((Type*)ret)->cls == NULL) return ErrorC(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");

		const char* fname = self->letter ? self->letter->text->data : "if";
		char typename_str[256]; snprintf(typename_str, sizeof(typename_str), "%%%sIfType", fname);
		char checkname[256]; snprintf(checkname, sizeof(checkname), "@%sIfCheck%d", fname, llvm_n++);

		self->model = createString(thgc, typename_str, (int)strlen(typename_str), 1);

		LTypeDec* typedec; LFunc* funccheck; LVari* co_val; LVari* thgc2; LVari* i8p;
		LVari* go_c; LVari* objptr3;
		int count, order;
		setup_scope_gc(thgc, local, self, typename_str, checkname,
			&typedec, &funccheck, &co_val, &thgc2, &i8p, &go_c, &objptr3, &count, &order);

		List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);

		// Process vmapA for blk0 and blk2
		process_vmapA_block(thgc, local, blk0->vmapA, typedec, funccheck, co_val, thgc2, i8p,
			self->model, &order, &count, decs, NULL, NULL, func, objptr3, go_c, false);
		process_vmapA_block(thgc, local, blk2->vmapA, typedec, funccheck, co_val, thgc2, i8p,
			self->model, &order, &count, decs, NULL, NULL, func, objptr3, go_c, false);

		// Store local state
		local->blok = go_c;
		local->typedec_c = typedec;
		local->funccheck_c = funccheck;
		local->funcdec_c = func;
		local->i8p_c = i8p;
		local->objptr3_c = objptr3;
		local->co_val_c = co_val;
		local->odr = order;
		local->count_c = count;

		for (int i = 0; i < decs->size; i++) {
			Obj* d = *(Obj**)get_list(decs, i);
			Dec_vt(thgc, d, local);
		}

		char countbuf[32]; snprintf(countbuf, sizeof(countbuf), "%d", count);
		// countv would need to be set - simplified
		add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));
		add_list(thgc, blocks, (char*)blk0);
		Block_exeC(thgc, (Obj*)blk0, local);

		// Branch
		LLab* lab1 = make_llab(thgc, fmt_str(thgc, "then%d", llvm_n++));
		LLab* lab2 = make_llab(thgc, fmt_str(thgc, "else%d", llvm_n++));
		LLab* end = make_llab(thgc, createString(thgc, (char*)"end", 3, 1));

		if (self->calls_list->size == 1) lab2 = end;
		self->entry_lab = lab1;
		self->end_lab = end;

		// Br on condition
		Obj* last_ret = blk0->rets && blk0->rets->size > 0 ? *(Obj**)get_list(blk0->rets, blk0->rets->size - 1) : NULL;
		LVari* cond = last_ret ? ((Value*)last_ret)->vari : NULL;
		add_list(thgc, func->comps, (char*)make_lbr(thgc, cond, lab1, lab2));
		add_list(thgc, func->comps, (char*)lab1);

		add_list(thgc, blocks, (char*)blk2);
		add_list(thgc, local->labs, (char*)lab1);
		Block_exeC(thgc, (Obj*)blk2, local);
		local->labs->size--;
		blocks->size--;
		blocks->size--;

		// Handle then branch value
		if (blk2->rets->size > 0) {
			Obj* lastval = *(Obj**)get_list(blk2->rets, blk2->rets->size - 1);
			if (lastval->type == LetterType::OT_Value) {
				LVari* va = BitcastCast(thgc, local, ret, ((Val*)lastval)->cls, blk2->vari_c ? blk2->vari_c : ((Value*)lastval)->vari);
				// Store as ifvalue for phi
				add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, end));
			} else if (lastval->type == LetterType::OT_VoiVal) {
				add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, end));
			} else {
				return ErrorC(thgc, LetterType::OT_Error, self->letter, "ifの値がありません");
			}
		}

		// Handle elif/else chain
		if (self->calls_list->size != 1) {
			add_list(thgc, func->comps, (char*)lab2);
		}

		// Walk remaining primary children for elif/else
		int m = 1;
	head_if:
		if (self->letter == local->letter && local->kouhos == NULL) {
			call_last(local->calls, thgc, local);
		}
		if (val2->type == LetterType::OT_Elif) {
			emit_rnp_dec(thgc, local);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				Block* blk10 = (Block*)*(Obj**)get_list(val2->children, 0);
				Block* blk12 = (Block*)*(Obj**)get_list(val2->children, 1);

				// Setup elif scope
				char eliftype[256]; snprintf(eliftype, sizeof(eliftype), "%%%sElifType%d", fname, llvm_n++);
				char elifcheck[256]; snprintf(elifcheck, sizeof(elifcheck), "@%sElifCheck%d", fname, llvm_n++);

				LTypeDec* typedec1; LFunc* funccheck2; LVari* co_val1; LVari* thgc21; LVari* i8p1;
				LVari* go_c1; LVari* objptr31;
				int count1, order1;
				Obj* elif_obj = val2; // use val2 as elif placeholder
				elif_obj->model = createString(thgc, eliftype, (int)strlen(eliftype), 1);
				setup_scope_gc(thgc, local, elif_obj, eliftype, elifcheck,
					&typedec1, &funccheck2, &co_val1, &thgc21, &i8p1, &go_c1, &objptr31, &count1, &order1);

				List* decs2 = create_list(thgc, sizeof(Obj*), CType::_CObj);
				String* ms2 = createString(thgc, eliftype, (int)strlen(eliftype), 1);
				process_vmapA_block(thgc, local, blk10->vmapA, typedec1, funccheck2, co_val1, thgc21, i8p1,
					ms2, &order1, &count1, decs2, NULL, NULL, func, objptr31, go_c1, false);
				process_vmapA_block(thgc, local, blk12->vmapA, typedec1, funccheck2, co_val1, thgc21, i8p1,
					ms2, &order1, &count1, decs2, NULL, NULL, func, objptr31, go_c1, false);

				for (int i = 0; i < decs2->size; i++) Dec_vt(thgc, *(Obj**)get_list(decs2, i), local);
				add_list(thgc, funccheck2->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

				add_list(thgc, blocks, (char*)blk0);
				Block_exeC(thgc, (Obj*)blk10, local);

				LLab* lab11 = make_llab(thgc, fmt_str(thgc, "then%d", llvm_n++));
				lab2 = make_llab(thgc, fmt_str(thgc, "else%d", llvm_n++));
				if (self->calls_list && (int)self->calls_list->size == m + 1) lab2 = end;

				Obj* last_ret1 = blk10->rets && blk10->rets->size > 0 ? *(Obj**)get_list(blk10->rets, blk10->rets->size - 1) : NULL;
				add_list(thgc, func->comps, (char*)make_lbr(thgc, last_ret1 ? ((Value*)last_ret1)->vari : NULL, lab11, lab2));
				add_list(thgc, func->comps, (char*)lab11);
				add_list(thgc, blocks, (char*)blk12);
				add_list(thgc, local->labs, (char*)lab11);
				Block_exeC(thgc, (Obj*)blk12, local);
				local->labs->size--;
				blocks->size--;
				blocks->size--;

				if (blk12->rets && blk12->rets->size > 0) {
					Obj* lastval = *(Obj**)get_list(blk12->rets, blk12->rets->size - 1);
					if (lastval->type == LetterType::OT_Value || lastval->type == LetterType::OT_VoiVal) {
						add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, end));
					} else {
						return ErrorC(thgc, LetterType::OT_Error, self->letter, "ifの値がありません");
					}
				}
				add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, end));
				m++;
				if (self->calls_list && (int)self->calls_list->size != m) {
					add_list(thgc, func->comps, (char*)lab2);
				}
				if (((Type*)ret)->cls == NULL) return ErrorC(thgc, LetterType::OT_Wait, self->letter, "varの型が判明しません");
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head_if;
			} else {
				return ErrorC(thgc, LetterType::OT_Error, val2->letter, "elif文の中身が宣言されていません。");
			}
		}
		else if (val2->type == LetterType::OT_Else) {
			emit_rnp_dec(thgc, local);
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				Block* blk10 = (Block*)*(Obj**)get_list(val2->children, 0);
				Block* blk12 = (Block*)*(Obj**)get_list(val2->children, 1);

				char elsetype[256]; snprintf(elsetype, sizeof(elsetype), "%%%sElseType%d", fname, llvm_n++);
				char elsecheck[256]; snprintf(elsecheck, sizeof(elsecheck), "@%sElseCheck%d", fname, llvm_n++);

				LTypeDec* typedec1; LFunc* funccheck2; LVari* co_val1; LVari* thgc21; LVari* i8p1;
				LVari* go_c1; LVari* objptr31;
				int count1, order1;
				Obj* else_obj = val2;
				else_obj->model = createString(thgc, elsetype, (int)strlen(elsetype), 1);
				setup_scope_gc(thgc, local, else_obj, elsetype, elsecheck,
					&typedec1, &funccheck2, &co_val1, &thgc21, &i8p1, &go_c1, &objptr31, &count1, &order1);

				List* decs2 = create_list(thgc, sizeof(Obj*), CType::_CObj);
				String* ms3 = createString(thgc, elsetype, (int)strlen(elsetype), 1);
				process_vmapA_block(thgc, local, blk10->vmapA, typedec1, funccheck2, co_val1, thgc21, i8p1,
					ms3, &order1, &count1, decs2, NULL, NULL, func, objptr31, go_c1, false);
				process_vmapA_block(thgc, local, blk12->vmapA, typedec1, funccheck2, co_val1, thgc21, i8p1,
					ms3, &order1, &count1, decs2, NULL, NULL, func, objptr31, go_c1, false);

				for (int i = 0; i < decs2->size; i++) Dec_vt(thgc, *(Obj**)get_list(decs2, i), local);
				add_list(thgc, funccheck2->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

				add_list(thgc, local->labs, (char*)lab2);
				add_list(thgc, blocks, (char*)blk0);
				Block_exeC(thgc, (Obj*)blk10, local);
				add_list(thgc, blocks, (char*)blk12);
				Block_exeC(thgc, (Obj*)blk12, local);
				local->labs->size--;
				blocks->size--;
				blocks->size--;

				if (blk12->rets && blk12->rets->size > 0) {
					Obj* lastval = *(Obj**)get_list(blk12->rets, blk12->rets->size - 1);
					if (lastval->type == LetterType::OT_Value || lastval->type == LetterType::OT_VoiVal) {
						add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, end));
					} else {
						return ErrorC(thgc, LetterType::OT_Error, self->letter, "ifの値がありません");
					}
				}
				emit_rnp_dec(thgc, local);
				add_list(thgc, func->comps, (char*)end);
				(*n)++;

				if (ret->type == LetterType::OT_Var) ret = ((Type*)ret)->cls;
				if (ret == local->Void) {
					return make_value(thgc, ret, NULL);
				} else {
					LVari* vari;
					if (ret == local->Int) vari = make_vn(thgc, "i32", "va");
					else if (ret == local->Bool) vari = make_vn(thgc, "i1", "va");
					else {
						char buf[128]; snprintf(buf, sizeof(buf), "%%%sType", ((Type*)ret)->cls ? (((Type*)ret)->cls->letter ? ((Type*)ret)->cls->letter->text->data : "x") : "x");
						vari = make_vn(thgc, buf, "va");
					}
					// Phi would be created here in full implementation
					return make_value(thgc, ret, vari);
				}
			} else {
				return ErrorC(thgc, LetterType::OT_Error, val2->letter, "else文の中身が宣言されていません。");
			}
		}
		else {
			// No else clause
			emit_rnp_dec(thgc, local);
			add_list(thgc, func->comps, (char*)end);
			local->labs->size--;
			add_list(thgc, local->labs, (char*)end);

			if (ret->type == LetterType::OT_Var) ret = ((Type*)ret)->cls;
			if (ret == local->Void) {
				return make_value(thgc, ret, NULL);
			} else {
				LVari* vari;
				if (ret == local->Int) vari = make_vn(thgc, "i32", "va");
				else if (ret == local->Bool) vari = make_vn(thgc, "i1", "va");
				else {
					char buf[128]; snprintf(buf, sizeof(buf), "%%%sType", ((Type*)ret)->cls ? (((Type*)ret)->cls->letter ? ((Type*)ret)->cls->letter->text->data : "x") : "x");
					vari = make_vn(thgc, buf, "va");
				}
				return make_value(thgc, ret, vari);
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "if文の中身が宣言されていません");
}

// ============================================================
// While::PrimaryC (lines 4374-4818)
// ============================================================
Obj* While_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	LFunc* func = llvm_get_func(local->llvm);
	Obj* ret = ((Function*)self)->rettype;

	if (val2->type == LetterType::OT_CallBlock) {
		Block* blk0 = (Block*)*(Obj**)get_list(val2->children, 0);
		Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);

		const char* fname = self->letter ? self->letter->text->data : "while";
		char typename_str[256]; snprintf(typename_str, sizeof(typename_str), "%%%sWhileType%d", fname, llvm_n++);
		char checkname[256]; snprintf(checkname, sizeof(checkname), "@%sWhileCheck%d", fname, llvm_n++);

		self->model = createString(thgc, typename_str, (int)strlen(typename_str), 1);
		self->n_c = obj_cn++;

		LTypeDec* typedec; LFunc* funccheck; LVari* co_val; LVari* thgc2; LVari* i8p;
		LVari* go_c; LVari* objptr3;
		int count, order;
		setup_scope_gc(thgc, local, self, typename_str, checkname,
			&typedec, &funccheck, &co_val, &thgc2, &i8p, &go_c, &objptr3, &count, &order);

		List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);

		// Process vmapA for blk0 and blk2
		process_vmapA_block(thgc, local, blk0->vmapA, typedec, funccheck, co_val, thgc2, i8p,
			self->model, &order, &count, decs, NULL, NULL, func, objptr3, go_c, false);
		process_vmapA_block(thgc, local, blk2->vmapA, typedec, funccheck, co_val, thgc2, i8p,
			self->model, &order, &count, decs, NULL, NULL, func, objptr3, go_c, false);

		// Store local state
		local->blok = go_c;
		local->typedec_c = typedec;
		local->funccheck_c = funccheck;
		local->funcdec_c = func;
		local->i8p_c = i8p;
		local->objptr3_c = objptr3;
		local->co_val_c = co_val;
		local->odr = order;
		local->count_c = count;

		for (int i = 0; i < decs->size; i++) {
			Obj* d = *(Obj**)get_list(decs, i);
			Dec_vt(thgc, d, local);
		}

		// Return value array setup
		LVari* pv = NULL;
		if (ret->type == LetterType::OT_Var) ret = ((Type*)ret)->cls;
		if (ret != local->Void) {
			LVari* vcl = make_vn(thgc, "%CreateHashType", "vc");
			add_list(thgc, func->comps, (char*)make_lload(thgc, vcl, make_lvari_c(thgc, "%CreateHashType*", "@CreateHash"), false));
			pv = make_vn(thgc, "%HashType*", "pv");
			self->forvari = pv;
			bool objected = true;
			if (ret == local->Int) objected = false;
			else if (ret == local->Bool) objected = false;
			LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
			LCall* hash_call = make_lcall(thgc, pv, vcl);
			lcall_add(thgc, hash_call, thgc4);
			lcall_add(thgc, hash_call, make_lvari_c(thgc, "i1", objected ? "true" : "false"));
			add_list(thgc, func->comps, (char*)hash_call);

			// Add ret variable to typedec
			int retorder = order++;
			if (ret == local->Int) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, (char*)"ret", 3, 1)));
				count += 8;
			} else if (ret == local->Bool) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i1", 2, 1), createString(thgc, (char*)"ret", 3, 1)));
				count += 8;
			} else {
				SetIdentity(thgc, (Obj*)(ret));
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, model_type(thgc, ret->model, "*"), createString(thgc, (char*)"ret", 3, 1)));
				count += 8;
				// GC check for ret
				LVari* vari = make_vn(thgc, "i8*", "va");
				char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", retorder);
				add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, typename_str, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
				LVari* vari2 = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				LVari* co_cval = make_vn(thgc, "i8*", "co");
				LCall* co_call = make_lcall(thgc, co_cval, co_val);
				lcall_add(thgc, co_call, thgc2);
				lcall_add(thgc, co_call, vari);
				add_list(thgc, funccheck->comps, (char*)co_call);
				add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			}
			// Store hash to go_c
			LVari* va = make_vn(thgc, "%HashType*", "va");
			char retbuf[32]; snprintf(retbuf, sizeof(retbuf), "%d", retorder);
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", retbuf)));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, va, pv));
		}

		char countbuf[32]; snprintf(countbuf, sizeof(countbuf), "%d", count);
		add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

		// Execute condition block (all but last child)
		add_list(thgc, blocks, (char*)blk0);
		Block_exeRangeC(thgc, blk0, 0, blk0->children->size - 1, local, false);

		// Loop structure
		LLab* lab_cond = make_llab(thgc, createString(thgc, (char*)"cond", 4, 1));
		self->entry_lab = lab_cond;
		add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, lab_cond));
		add_list(thgc, func->comps, (char*)lab_cond);
		// Phi for pv would go here
		add_list(thgc, local->labs, (char*)lab_cond);

		// Execute last condition child
		Obj* last_child = *(Obj**)get_list(blk0->children, blk0->children->size - 1);
		Obj* obj = vt_exeC(thgc, last_child, local);
		local->labs->size--;

		LLab* lab_loop = make_llab(thgc, createString(thgc, (char*)"loop", 4, 1));
		LLab* lab_end = make_llab(thgc, createString(thgc, (char*)"end", 3, 1));
		self->entry_lab = lab_end;

		add_list(thgc, func->comps, (char*)make_lbr(thgc, ((Value*)obj)->vari, lab_loop, lab_end));
		add_list(thgc, func->comps, (char*)lab_loop);

		// Execute loop body
		add_list(thgc, blocks, (char*)blk2);
		add_list(thgc, local->labs, (char*)lab_loop);
		Block_exeC(thgc, (Obj*)blk2, local);
		local->labs->size--;

		// Handle loop body return value -> AddHash + Br back to cond
		if (ret != local->Void && pv != NULL) {
			if (blk2->rets && blk2->rets->size > 0) {
				Obj* lastval = *(Obj**)get_list(blk2->rets, blk2->rets->size - 1);
				if (lastval->type == LetterType::OT_Value) {
					LVari* va = BitcastCast(thgc, local, ret, ((Val*)lastval)->cls, ((Value*)lastval)->vari);
					LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
					LVari* varih = make_vn(thgc, "%AddHashType", "va");
					add_list(thgc, func->comps, (char*)make_lload(thgc, varih, make_lvari_c(thgc, "%AddHashType*", "@AddHash"), false));
					LCall* addcall = make_lcall(thgc, NULL, varih);
					lcall_add(thgc, addcall, thgc4);
					lcall_add(thgc, addcall, make_lvari_c(thgc, "%StringType*", "null"));
					lcall_add(thgc, addcall, pv);
					lcall_add(thgc, addcall, va);
					add_list(thgc, func->comps, (char*)addcall);
					add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, lab_cond));
				}
			}
		}

		blocks->size--;
		blocks->size--;
		add_list(thgc, func->comps, (char*)lab_end);
		local->labs->size--;
		add_list(thgc, local->labs, (char*)lab_end);

		// Epilogue: rnp--
		emit_rnp_dec(thgc, local);
		(*n)++;
		return make_value(thgc, ret, pv);
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "while文の中身が宣言されていません");
}

// ============================================================
// While::exepC (lines 4820-4827)
// ============================================================
Obj* While_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		if (local->calls->size > 0) {
			typedef void (*CallbackFn)();
			CallbackFn cb = *(CallbackFn*)get_list(local->calls, local->calls->size - 1);
			if (cb) cb();
		}
	}
	return self;
}

// ============================================================
// For::PrimaryC (lines 4831-5020+)
// ============================================================
Obj* For_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	LFunc* func = llvm_get_func(local->llvm);
	Obj* ret = ((Function*)self)->rettype;

	if (val2->type == LetterType::OT_CallBlock) {
		Block* blk0 = (Block*)*(Obj**)get_list(val2->children, 0);
		Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);

		const char* fname = self->letter ? self->letter->text->data : "for";
		char typename_str[256]; snprintf(typename_str, sizeof(typename_str), "%%%sForType%d", fname, llvm_n++);
		char checkname[256]; snprintf(checkname, sizeof(checkname), "@%sForCheck%d", fname, llvm_n++);

		self->model = createString(thgc, typename_str, (int)strlen(typename_str), 1);
		self->n_c = obj_cn++;

		LTypeDec* typedec; LFunc* funccheck; LVari* co_val; LVari* thgc2; LVari* i8p;
		LVari* go_c; LVari* objptr3;
		int count, order;
		setup_scope_gc(thgc, local, self, typename_str, checkname,
			&typedec, &funccheck, &co_val, &thgc2, &i8p, &go_c, &objptr3, &count, &order);

		List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);

		// Process vmapA for blk0 and blk2
		process_vmapA_block(thgc, local, blk0->vmapA, typedec, funccheck, co_val, thgc2, i8p,
			self->model, &order, &count, decs, NULL, NULL, func, objptr3, go_c, false);
		process_vmapA_block(thgc, local, blk2->vmapA, typedec, funccheck, co_val, thgc2, i8p,
			self->model, &order, &count, decs, NULL, NULL, func, objptr3, go_c, false);

		// Store local state
		local->blok = go_c;
		local->typedec_c = typedec;
		local->funccheck_c = funccheck;
		local->funcdec_c = func;
		local->i8p_c = i8p;
		local->objptr3_c = objptr3;
		local->co_val_c = co_val;
		local->odr = order;
		local->count_c = count;

		for (int i = 0; i < decs->size; i++) {
			Obj* d = *(Obj**)get_list(decs, i);
			Dec_vt(thgc, d, local);
		}

		// Return value array setup (same as while)
		LVari* pv = NULL;
		if (ret->type == LetterType::OT_Var) ret = ((Type*)ret)->cls;
		if (ret != local->Void) {
			LVari* vcl = make_vn(thgc, "%CreateHashType", "vc");
			add_list(thgc, func->comps, (char*)make_lload(thgc, vcl, make_lvari_c(thgc, "%CreateHashType*", "@CreateHash"), false));
			pv = make_vn(thgc, "%HashType*", "pv");
			self->forvari = pv;
			bool objected = true;
			if (ret == local->Int) objected = false;
			else if (ret == local->Bool) objected = false;
			LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
			LCall* hash_call = make_lcall(thgc, pv, vcl);
			lcall_add(thgc, hash_call, thgc4);
			lcall_add(thgc, hash_call, make_lvari_c(thgc, "i1", objected ? "true" : "false"));
			add_list(thgc, func->comps, (char*)hash_call);

			// Add ret variable to typedec
			int retorder = order++;
			if (ret == local->Int) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i32", 3, 1), createString(thgc, (char*)"ret", 3, 1)));
				count += 8;
			} else if (ret == local->Bool) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, createString(thgc, (char*)"i1", 2, 1), createString(thgc, (char*)"ret", 3, 1)));
				count += 8;
			} else {
				SetIdentity(thgc, (Obj*)(ret));
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, model_type(thgc, ret->model, "*"), createString(thgc, (char*)"ret", 3, 1)));
				count += 8;
				// GC check for ret
				LVari* vari = make_vn(thgc, "i8*", "va");
				char orderbuf[32]; snprintf(orderbuf, sizeof(orderbuf), "%d", retorder);
				add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, typename_str, vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", orderbuf)));
				LVari* vari2 = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				LVari* co_cval = make_vn(thgc, "i8*", "co");
				LCall* co_call = make_lcall(thgc, co_cval, co_val);
				lcall_add(thgc, co_call, thgc2);
				lcall_add(thgc, co_call, vari);
				add_list(thgc, funccheck->comps, (char*)co_call);
				add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			}
			// Store hash to go_c
			LVari* va = make_vn(thgc, "%HashType*", "va");
			char retbuf[32]; snprintf(retbuf, sizeof(retbuf), "%d", retorder);
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", retbuf)));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, va, pv));
		}

		char countbuf[32]; snprintf(countbuf, sizeof(countbuf), "%d", count);
		add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "")));

		// Execute init block (all but last child)
		add_list(thgc, blocks, (char*)blk0);
		Block_exeRangeC(thgc, blk0, 0, blk0->children->size - 1, local, false);

		// For loop structure: init -> cond -> loop body -> step -> cond
		LLab* lab_cond = make_llab(thgc, createString(thgc, (char*)"cond", 4, 1));
		self->entry_lab = lab_cond;
		add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, lab_cond));
		add_list(thgc, func->comps, (char*)lab_cond);
		add_list(thgc, local->labs, (char*)lab_cond);

		// Evaluate condition (last child of blk0)
		Obj* last_child = *(Obj**)get_list(blk0->children, blk0->children->size - 1);
		Obj* obj = vt_exeC(thgc, last_child, local);
		local->labs->size--;

		LLab* lab_loop = make_llab(thgc, createString(thgc, (char*)"loop", 4, 1));
		LLab* lab_end = make_llab(thgc, createString(thgc, (char*)"end", 3, 1));
		self->entry_lab = lab_end;

		add_list(thgc, func->comps, (char*)make_lbr(thgc, ((Value*)obj)->vari, lab_loop, lab_end));
		add_list(thgc, func->comps, (char*)lab_loop);

		// Execute loop body
		add_list(thgc, blocks, (char*)blk2);
		add_list(thgc, local->labs, (char*)lab_loop);
		Block_exeC(thgc, (Obj*)blk2, local);
		local->labs->size--;

		// Handle loop body return value -> AddHash + Br back to cond
		if (ret != local->Void && pv != NULL) {
			if (blk2->rets && blk2->rets->size > 0) {
				Obj* lastval = *(Obj**)get_list(blk2->rets, blk2->rets->size - 1);
				if (lastval->type == LetterType::OT_Value) {
					LVari* va = BitcastCast(thgc, local, ret, ((Val*)lastval)->cls, ((Value*)lastval)->vari);
					LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
					LVari* varih = make_vn(thgc, "%AddHashType", "va");
					add_list(thgc, func->comps, (char*)make_lload(thgc, varih, make_lvari_c(thgc, "%AddHashType*", "@AddHash"), false));
					LCall* addcall = make_lcall(thgc, NULL, varih);
					lcall_add(thgc, addcall, thgc4);
					lcall_add(thgc, addcall, make_lvari_c(thgc, "%StringType*", "null"));
					lcall_add(thgc, addcall, pv);
					lcall_add(thgc, addcall, va);
					add_list(thgc, func->comps, (char*)addcall);
					add_list(thgc, func->comps, (char*)make_lbr_uncond(thgc, lab_cond));
				}
			}
		}

		blocks->size--;
		blocks->size--;
		add_list(thgc, func->comps, (char*)lab_end);
		local->labs->size--;
		add_list(thgc, local->labs, (char*)lab_end);

		// Epilogue: rnp--
		emit_rnp_dec(thgc, local);
		(*n)++;
		return make_value(thgc, ret, pv);
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "for文の中身が宣言されていません");
}

// ============================================================
// End of obj2c_part2.h
// ============================================================
// obj2c_part3.h - Obj2C.cs lines 5000-8000
// Continuation of For::PrimaryC, Switch, Break, Continue, Return, Goto, Print, Iterator, GenericFunction, Model

// ============================================================
// Forward declarations
// ============================================================
Obj* For_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* For2_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Switch_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Switch_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
LVari* Switch_ToVari(ThreadGC* thgc, Obj* self, String* name, Obj* cls, Master* local, int size);
int hashvalue_Switch_get(String* key, int size);
Obj* Break_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Break_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Continue_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Continue_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Return_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Return_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Goto_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Goto_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Print_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Print_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Iterator_GetterC(ThreadGC* thgc, Obj* self, Master* local);
Obj* GenericFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Model_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Model_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* exeC(ThreadGC* thgc, Obj* self, Master* local);
void KouhoSetLabel(ThreadGC* thgc, Master* local);
void ModelObj_Dec(ThreadGC* thgc, Obj* self, Master* local);

// ============================================================
// Helper: emit common block variable/function/classobj setup for For/While/Switch
// Handles vmapA iteration and type declarations
// ============================================================
static inline void emit_vmap_setup(ThreadGC* thgc, Master* local, Map* vmapA,
	LTypeDec* typedec, LFunc* funccheck, LVari* i8p, LVari* co_val, LVari* thgc2,
	Obj* selfobj, LVari* go_c, LVari* objptr3, List* decs, int* order, int* count) {
	// iterate vmapA
	for (int ki = 0; ki < vmapA->kvs->size; ki++) {
		KV* kv_entry = *(KV**)get_list(vmapA->kvs, ki);
		String* key = kv_entry->key;
		Obj* v = (Obj*)kv_entry->value;
		if (v == NULL) continue;
		if (v->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)v;
			variable->order = (*order)++;
			Obj* cls = variable->cls;
			if (cls && cls->type == LetterType::OT_Var) cls = (Obj*)((Var*)cls)->cls;
			if (cls == local->Int) {
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "i32", key));
				*count += 8;
			} else if (cls == local->Bool) {
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "i1", key));
				*count += 8;
			} else {
				SetIdentity(thgc, (Obj*)(cls));
				LVari* vari = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
				LGete* gete = make_lgete_idx2_s(thgc, selfobj->model, vari, i8p,
					make_lvari_c(thgc, "i32", "0"),
					make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", variable->order)));
				add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, str_suffix(thgc, cls->model, "*"), key));
				LVari* vari2 = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				LVari* co_cval = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
				LCall* co_call = make_lcall(thgc, co_cval, co_val);
				lcall_add(thgc, co_call, thgc2);
				lcall_add(thgc, co_call, vari);
				add_list(thgc, funccheck->comps, (char*)co_call);
				add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
				*count += 8;
			}
		} else if (v->type == LetterType::OT_Function) {
			Function* f = (Function*)v;
			SetIdentity(thgc, (Obj*)(f));
			f->order = (*order)++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "%FuncType*", key));

			LVari* vari = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
			LGete* gete = make_lgete_idx2_s(thgc, selfobj->model, vari, i8p,
				make_lvari_c(thgc, "i32", "0"),
				make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", f->order)));
			add_list(thgc, funccheck->comps, (char*)gete);
			LVari* vari2 = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			LVari* co_cval = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
			LCall* co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			*count += 8;

			// func dec call
			LVari* va0 = make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "%%v%d", llvm_n++));
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", f->letter->text->data, f->identity)));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->rename));  // drawcall
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->version)); // call
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)fu_call);

			LVari* va2 = make_lvari(thgc, str_suffix(thgc, selfobj->model, "*"), fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, local->llvm->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "%%v%d", llvm_n++));
			gete = make_lgete_idx2_s(thgc, selfobj->model, va, va2,
				make_lvari_c(thgc, "i32", "0"),
				make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", f->order)));
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)gete);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, va, va0));
		} else if (v->type == LetterType::OT_ClassObj) {
			ClassObj* f = (ClassObj*)v;
			SetIdentity(thgc, (Obj*)(f));
			f->order = (*order)++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "%FuncType*", key));

			LVari* vari = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
			LGete* gete = make_lgete_idx2_s(thgc, selfobj->model, vari, i8p,
				make_lvari_c(thgc, "i32", "0"),
				make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", f->order)));
			add_list(thgc, funccheck->comps, (char*)gete);
			LVari* vari2 = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			LVari* co_cval = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
			LCall* co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			*count += 8;

			LVari* va0 = make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "%%v%d", llvm_n++));
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", f->letter->text->data, f->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->rename));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->version));
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)fu_call);

			LVari* va2 = make_lvari(thgc, str_suffix(thgc, selfobj->model, "*"), fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, local->llvm->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "%%v%d", llvm_n++));
			gete = make_lgete_idx2_s(thgc, selfobj->model, va, va2,
				make_lvari_c(thgc, "i32", "0"),
				make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", f->order)));
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)gete);
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, va, va0));
		}
	}
}


// ============================================================
// Virtual dispatch: Dec (used by obj2c_part2/3)
// Routes to Function_Dec, ClassObj_Dec, or ModelObj_Dec based on objtype
// ============================================================
void Dec_vt(ThreadGC* thgc, Obj* d, Master* local) {
	if (d == NULL) return;
	if (d->type == LetterType::OT_Function || d->type == LetterType::OT_GenericFunction ||
		d->type == LetterType::OT_ServerFunction || d->type == LetterType::OT_SignalFunction) {
		Function_Dec(thgc, d, local);
	} else if (d->type == LetterType::OT_ClassObj) {
		ClassObj_Dec(thgc, d, local);
	} else if (d->type == LetterType::OT_ModelObj) {
		ModelObj_Dec(thgc, d, local);
	}
}

// ============================================================
// Virtual dispatch: DecC (used by obj2c_part5)
// Same routing as Dec_vt for the "C" compilation phase
// ============================================================
void DecC_vt(ThreadGC* thgc, Obj* d, Master* local) {
	if (d == NULL) return;
	if (d->type == LetterType::OT_Function || d->type == LetterType::OT_GenericFunction ||
		d->type == LetterType::OT_ServerFunction || d->type == LetterType::OT_SignalFunction) {
		Function_Dec(thgc, d, local);
	} else if (d->type == LetterType::OT_ClassObj) {
		ClassObj_Dec(thgc, d, local);
	} else if (d->type == LetterType::OT_ModelObj) {
		ModelObj_Dec(thgc, d, local);
	}
}

// ============================================================
// exeC - delegates to vt_exeC (virtual dispatch for exeC)
// ============================================================
Obj* exeC(ThreadGC* thgc, Obj* self, Master* local) {
	return vt_exeC(thgc, self, local);
}

// KouhoSetLabel - defined in obj1b.h

// ============================================================
// For::exepC  (lines 5277-5284)
// ============================================================
Obj* For2_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		// local.calls.Last()()
		// callback invocation placeholder
	}
	return self;
}

// ============================================================
// Switch::exepC  (lines 5286-5294)
// ============================================================
Obj* Switch_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Switch::get_hashvalue  (lines 5734-5744)
// ============================================================
int hashvalue_Switch_get(String* key, int size) {
	int hash = 10000;
	for (int i = 0; i < key->size; i++) {
		hash ^= key->data[i];
		hash *= 1619;
		hash %= size;
	}
	return hash;
}

// ============================================================
// Switch::ToVari  (lines 5718-5733)
// ============================================================
LVari* Switch_ToVari(ThreadGC* thgc, Obj* self, String* name, Obj* cls, Master* local, int size) {
	if (cls == local->Int) {
		// try parse int
		return make_lvari_s(thgc, "i32", name);
	} else if (cls == local->StrT) {
		int hv = hashvalue_Switch_get(name, size);
		return make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", hv));
	}
	return NULL; // error
}

// ============================================================
// Switch::PrimaryC  (lines 5295-5717)
// ============================================================
Obj* Switch_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	if (val2->type == LetterType::OT_CallBlock) {
		Block* blk0 = (Block*)*(Obj**)get_list(val2->children, 0);
		Block* blk2 = (Block*)*(Obj**)get_list(val2->children, 1);
		(*n)++;

		val2 = *(Obj**)get_list(primary->children, *n);
		self->n_c = Obj_cn++;
		self->model = fmt_str(thgc, "%%%sSwitchType%d", self->letter->text->data, llvm_n++);
		LTypeDec* typedec = make_ltypedec(thgc, self->model);
		add_list(thgc, local->llvm->types, (char*)typedec);

		LVari* i8p = make_lvari_c(thgc, "i8*", "%self");
		LVari* thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
		String* checkname = fmt_str(thgc, "@%sSwitchCheck%d", self->letter->text->data, llvm_n++);
		LFunc* funccheck = make_lfunc(thgc, local->llvm, make_lvari_s(thgc, "void", checkname), thgc2, i8p);
		add_list(thgc, local->llvm->comps, (char*)funccheck);
		add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "i8*", fmt_str(thgc, "blk")));

		LVari* vari = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
		LGete* gete = make_lgete_idx2_s(thgc, self->model, vari, i8p,
			make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0"));
		add_list(thgc, funccheck->comps, (char*)gete);

		LVari* co_val = make_lvari_c(thgc, "%CopyObjectType", "%co");
		add_list(thgc, funccheck->comps, (char*)make_lload(thgc, co_val, make_lvari_c(thgc, "%CopyObjectType*", "@CopyObject"), false));
		LVari* vari2 = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
		add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
		LVari* co_cval = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
		LCall* co_call = make_lcall(thgc, co_cval, co_val);
		lcall_add(thgc, co_call, thgc2);
		lcall_add(thgc, co_call, vari);
		add_list(thgc, funccheck->comps, (char*)co_call);
		add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));

		LVari* thgc3 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
		LVari* countv = make_lvari_c(thgc, "i32", "");
		String* sname = fmt_str(thgc, "switch%d", llvm_n++);
		LStrV* strv = make_lstrv(thgc, fmt_str(thgc, "@%s", sname->data), sname, sname->size);
		add_list(thgc, local->llvm->strs, (char*)strv);
		int nval = (self->n_c = local->llvm->cn++);
		LVari* tnp = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tnp%d", llvm_n++));
		add_list(thgc, local->llvm->main->comps, (char*)make_lload(thgc, tnp, make_lvari_c(thgc, "i32*", "@cnp"), false));
		LVari* tv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%cv%d", llvm_n++));
		add_list(thgc, local->llvm->main->comps, (char*)make_lbinop(thgc, LBOP_Add, tv, tnp, make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", nval))));
		LCall* ac_call = make_lcall(thgc, NULL, make_lvari_c(thgc, "%GC_AddClassType", "%addclass"));
		lcall_add(thgc, ac_call, thgc3);
		lcall_add(thgc, ac_call, countv);
		lcall_add(thgc, ac_call, (LVari*)strv);
		lcall_add(thgc, ac_call, make_lvari_s(thgc, "%GCCheckFuncType", checkname));
		lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCFinalizeFuncType", "null"));
		add_list(thgc, local->llvm->main->comps, (char*)ac_call);

		// GC_malloc
		LFunc* func = llvm_get_func(local->llvm);
		LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
		LVari* go_val = make_lvari_s(thgc, "%GC_mallocType", fmt_str(thgc, "%%gcobject%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lload(thgc, go_val, make_lvari_c(thgc, "%GC_mallocType*", "@GC_malloc"), false));
		LVari* go_v = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%obj%d", llvm_n++));
		LVari* tmp = make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", nval));
		LVari* tmi = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmi%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lload(thgc, tmi, make_lvari_c(thgc, "i32*", "@cnp"), false));
		LVari* tmv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%tmv%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Add, tmv, tmi, tmp));
		LCall* go_call = make_lcall(thgc, go_v, go_val);
		lcall_add(thgc, go_call, thgc4);
		lcall_add(thgc, go_call, tmv);
		add_list(thgc, func->comps, (char*)go_call);
		LVari* go_c = make_lvari(thgc, str_suffix(thgc, self->model, "*"), fmt_str(thgc, "%%v%d", llvm_n++));
		add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c, go_v));

		LVari* v0 = make_lvari_s(thgc, "i8**", fmt_str(thgc, "%%v%d", llvm_n++));
		gete = make_lgete_idx2_s(thgc, self->model, v0, go_c,
			make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0"));
		add_list(thgc, func->comps, (char*)gete);

		// load parent block
		LVari* gj = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%gj%d", llvm_n++));
		if (func->async_flag) {
			LVari* objf = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
			LVari* objp = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", objp, objf,
				make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
			add_list(thgc, func->comps, (char*)make_lload(thgc, gj, objp, false));
		} else {
			Block* lastblk = *(Block**)get_list(blocks, blocks->size - 1);
			Obj* lastobj = (Obj*)lastblk->obj;
			if (lastobj && lastobj->type == LetterType::OT_IfBlock) {
				// IfBlock case - load from iflabel.bas
				add_list(thgc, func->comps, (char*)make_lload(thgc, gj, lastobj->bas, false));
			} else {
				add_list(thgc, func->comps, (char*)make_lload(thgc, gj, lastobj->bas, false));
			}
		}
		add_list(thgc, func->comps, (char*)make_lstore(thgc, v0, gj));

		// SetRoot
		LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
		LVari* objptr3;
		if (func->async_flag) {
			LVari* objf = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
			objptr3 = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", objptr3, objf,
				make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, objptr3, go_c));
		} else {
			objptr3 = make_lvari(thgc, str_suffix(thgc, self->model, "**"), fmt_str(thgc, "%%objptr%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lalloca(thgc, objptr3));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, objptr3, go_c));
			self->bas = objptr3;
			LVari* srv = make_lvari_s(thgc, "%GC_SetRootType", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
			LCall* srcall = make_lcall(thgc, NULL, srv);
			lcall_add(thgc, srcall, rn4);
			lcall_add(thgc, srcall, self->bas);
			add_list(thgc, func->comps, (char*)srcall);
		}

		int count = 8;
		int order = 1;
		List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);
		emit_vmap_setup(thgc, local, blk0->vmapA, typedec, funccheck, i8p, co_val, thgc2, self, go_c, objptr3, decs, &order, &count);
		emit_vmap_setup(thgc, local, blk2->vmapA, typedec, funccheck, i8p, co_val, thgc2, self, go_c, objptr3, decs, &order, &count);

		local->blok = go_c;
		local->typedecC = typedec;
		local->funccheck = funccheck;
		local->funcdec = llvm_get_func(local->llvm);
		local->i8p = i8p;
		local->objptr3 = objptr3;
		local->co_val = co_val;
		local->countv = countv;
		local->odr = order;
		local->countC = count;
		for (int i = 0; i < decs->size; i++) {
			Obj* d = *(Obj**)get_list(decs, i);
			Dec_vt(thgc, d, local);
		}
		decs = create_list(thgc, sizeof(Obj*), CType::_CObj);
		countv->name = fmt_str(thgc, "%d", count);
		add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));

		add_list(thgc, blocks, (char*)blk0);
		Block_exeC(thgc, (Obj*)blk0, local);
		LLab* lab = make_llab(thgc, "end");
		LVari* pv = NULL;
		LVari* vv = NULL;
		Map* ifv_map = create_mapy(thgc, CType::_List);
		self->ifv = ifv_map;
		self->end_lab = lab;
		int size = 64;
		Obj* lastret = *(Obj**)get_list(blk0->rets, blk0->rets->size - 1);
		LVari* variy = ((Value*)lastret)->vari;
		Obj* obj = lastret;

		// GetHashValue for string switch
		if (obj->type == LetterType::OT_Value) {
			Obj* value = obj;
			Obj* vcls = ((Value*)value)->cls; // value.cls
			if (vcls == local->StrT) {
				LVari* gh = make_lvari_s(thgc, "%GetHashValueType", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, gh, make_lvari_c(thgc, "%GetHashValueType*", "@GetHashValue"), false));
				LVari* varix = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
				LCall* ghcall = make_lcall(thgc, varix, gh);
				lcall_add(thgc, ghcall, variy);
				lcall_add(thgc, ghcall, make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", size)));
				add_list(thgc, func->comps, (char*)ghcall);
				variy = varix;
			}
		}

		// SwitchComp
		List* ifv2 = create_list(thgc, sizeof(IfValue*), CType::_LIfValue);
		add_list(thgc, func->comps, (char*)make_lswitchcomp(thgc, variy, lab, ifv2));

		// ret type
		Obj* ret = ((Function*)self)->rettype ? ((Function*)self)->rettype : local->Void;
		if (ret->type == LetterType::OT_Var) ret = (Obj*)((Var*)ret)->cls;
		if (ret != local->Void) {
			if (ret == local->Int) pv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
			else if (ret == local->Bool) pv = make_lvari_s(thgc, "i1", fmt_str(thgc, "%%v%d", llvm_n++));
			else pv = make_lvari(thgc, str_suffix(thgc, ret->model, "*"), fmt_str(thgc, "%%v%d", llvm_n++));
			if (ret == local->Int) vv = make_lvari_c(thgc, "i32", "0");
			else if (ret == local->Bool) vv = make_lvari_c(thgc, "i1", "false");
			else vv = make_lvari_sc(thgc, str_suffix(thgc, ret->model, "*"), "null");
			// ifv["entry"] = IfValue(Lab("entry"), vv)
			add_mapy(thgc, ifv_map, fmt_str(thgc, "entry"), (char*)make_lifvalue(thgc, make_llab(thgc, "entry"), vv));
		}

		add_list(thgc, blocks, (char*)blk2);
		// iterate labelmap
		List* larr = map_values_list(thgc, blk2->labelmap);
		for (int i = 0; i < larr->size; i++) {
			Label* larri = *(Label**)get_list(larr, i);
			LLab* labi = make_llab_s(thgc, fmt_str(thgc, "l%s", larri->name->data));

			if (obj->type == LetterType::OT_Value) {
				Obj* value = obj;
				Obj* vcls = ((Value*)value)->cls;
				if (vcls == local->Int) {
					IfValue* iv = make_lifvalue(thgc, labi, Switch_ToVari(thgc, self, larri->name, vcls, local, size));
					add_list(thgc, ifv2, (char*)iv);
				} else if (vcls == local->StrT) {
					IfValue* iv = make_lifvalue(thgc, labi, Switch_ToVari(thgc, self, larri->name, vcls, local, size));
					add_list(thgc, ifv2, (char*)iv);
				}
			}

			add_list(thgc, func->comps, (char*)labi);
			add_list(thgc, local->labs, (char*)labi);
			if (i == larr->size - 1) Block_exeRangeC(thgc, blk2, larri->n, blk2->children->size, local, false);
			else {
				Label* nexti = *(Label**)get_list(larr, i + 1);
				Block_exeRangeC(thgc, blk2, larri->n, nexti->n, local, false);
			}
			// remove last lab
			local->labs->size--;

			String* labname = labi->name;
			if (get_mapy(ifv_map, labname) == NULL) {
				Obj* lastret2 = *(Obj**)get_list(blk2->rets, blk2->rets->size - 1);
				if (lastret2->type == LetterType::OT_Value) {
					LVari* va = ((Value*)lastret2)->vari; // Bitcast.Cast simplified
					add_mapy(thgc, ifv_map, labname, (char*)make_lifvalue(thgc, labi, va));
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, lab, NULL));
				}
			}
		}

		add_list(thgc, func->comps, (char*)lab);
		local->labs->size--; // remove last
		add_list(thgc, local->labs, (char*)lab);
		blocks->size--; // remove last 2 blocks
		blocks->size--;
		add_list(thgc, func->comps, (char*)make_lphi(thgc, pv, ifv_map));

		// cleanup root node
		if (func->async_flag) {
			LVari* objp = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%obj%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", objp, (LVari*)*(LVari**)get_list(func->draws, 0),
				make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
			LVari* objv = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%objv%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, objv, objp, false));
			LVari* blockptr = make_lvari_s(thgc, "%GCObjectPtr*", fmt_str(thgc, "%%objp%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%GCObject", blockptr, objv,
				make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
			LVari* blockv = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%objv%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, blockv, blockptr, false));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, objp, blockv));
		} else {
			LVari* rnpv = make_lvari_c(thgc, "i32*", "%ptr");
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4,
				make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
			LVari* rnpv12 = make_lvari_c(thgc, "i32", "%v");
			add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
			LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
			add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));
		}
		(*n)++;
		Value* result = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
		result->vari = pv;
		return (Obj*)result;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "switch文の中身が宣言されていません");
}

// ============================================================
// Break::exepC  (lines 5746-5868)
// ============================================================
Obj* Break_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	}
	if (val2 != NULL) val2 = GetterC_vt(thgc, val2, local);
	int count = 0;
	LFunc* func = llvm_get_func(local->llvm);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		ObjBlock* bobj = (ObjBlock*)block->obj;
		if (bobj != NULL) {
			Obj* blkobj = (Obj*)bobj->obj;
			if (blkobj->type == LetterType::OT_IfBlock || bobj->n == 0) {
				count++;
			} else if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For || blkobj->type == LetterType::OT_Switch) {
				Obj* ret = blkobj->value;
				// dec root node count
				LVari* rnpv = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%ptr%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv, make_lvari_c(thgc, "i32**", "@rnp"), false));
				LVari* rnpv12 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
				LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12,
					make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", count))));
				add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

				LLab* end = blkobj->end_lab;
				if (blkobj->type == LetterType::OT_IfBlock) {
					end = ((IfBlock*)blkobj)->iflabel->end_lab;
				}
				LLab* lab = *(LLab**)get_list(local->labs, local->labs->size - 1);
				if (val2 == NULL) {
					val2 = make_voival(thgc);
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, end, NULL));
				}
				if (val2->type == LetterType::OT_Value) {
					Value* value = (Value*)val2;
					String* labname = lab->name;
					if (get_mapy(blkobj->ifv, labname) == NULL) {
						if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For) {
							LVari* v = value->vari;
							if (value->cls == ret) {
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, value->vari));
							} else if (ret == local->Int) {
								v = make_vn(thgc, "i32", "v");
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else if (ret == local->Short) {
								v = make_vn(thgc, "i16", "v");
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else if (ret == local->Bool) {
								v = make_vn(thgc, "i1", "v");
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else {
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, value->vari));
							}
							// AddHash for for/while
							LVari* thgc4b = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
							LVari* varih = make_lvari_s(thgc, "%AddHashType", fmt_str(thgc, "%%v%d", llvm_n++));
							add_list(thgc, func->comps, (char*)make_lload(thgc, varih, make_lvari_c(thgc, "%AddHashType*", "@AddHash"), false));
							LCall* ahcall = make_lcall(thgc, NULL, varih);
							lcall_add(thgc, ahcall, thgc4b);
							lcall_add(thgc, ahcall, make_lvari_c(thgc, "%StringType*", "null"));
							lcall_add(thgc, ahcall, blkobj->forvari);
							lcall_add(thgc, ahcall, v);
							add_list(thgc, func->comps, (char*)ahcall);
						} else if (value->cls == ret) {
							add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, value->vari));
						} else if (ret == local->Int) {
							LVari* v = make_vn(thgc, "i32", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
							add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
						} else if (ret == local->Short) {
							LVari* v = make_vn(thgc, "i16", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
							add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
						} else if (ret == local->Bool) {
							LVari* v = make_vn(thgc, "i1", "v");
							add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, value->vari));
							add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
						} else {
							add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, value->vari));
						}
					}
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, end, NULL));
				}
				Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
				Obj* lbobj = (Obj*)((ObjBlock*)lastblock->obj)->obj;
				String* labname2 = lab->name;
				if (get_mapy(lbobj->ifv, labname2) == NULL)
					add_mapy(thgc, lbobj->ifv, labname2, (char*)make_lifvalue(thgc, lab, NULL));
				(*n)--;
				return self;
			} else if (blkobj->type == LetterType::OT_Function || blkobj->type == LetterType::OT_ClassObj ||
				blkobj->type == LetterType::OT_ModelObj || blkobj->type == LetterType::OT_GenericObj ||
				blkobj->type == LetterType::OT_GenericFunction) {
				return ErrorC(thgc, LetterType::OT_Error, self->letter, "while,for,switchの中ではありません");
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "while,for,switchの中ではありません");
}

// ============================================================
// Break::PrimaryC  (lines 5869-5928)
// ============================================================
Obj* Break_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	}
	if (val2 != NULL) val2 = GetterC_vt(thgc, val2, local);
	int count = 0;
	LFunc* func = llvm_get_func(local->llvm);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		ObjBlock* bobj = (ObjBlock*)block->obj;
		if (bobj != NULL) {
			Obj* blkobj = (Obj*)bobj->obj;
			if ((blkobj->type == LetterType::OT_IfBlock || blkobj->type == LetterType::OT_Switch) || bobj->n == 0) {
				count++;
			} else if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For) {
				LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
				LVari* rnpv = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%ptr%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4,
					make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
				LVari* rnpv12 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
				LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12,
					make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", count))));
				add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

				LLab* end = blkobj->end_lab;
				if (blkobj->type == LetterType::OT_IfBlock) {
					end = ((IfBlock*)blkobj)->iflabel->end_lab;
				}
				LLab* lab = *(LLab**)get_list(local->labs, local->labs->size - 1);
				if (val2 == NULL) {
					val2 = make_voival(thgc);
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, end, NULL));
				}
				if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
					val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value ||
					val2->type == LetterType::OT_Block) {
					self->value_obj = val2;
					String* labname = lab->name;
					if (get_mapy(blkobj->ifv, labname) == NULL)
						add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, ((Value*)val2)->vari));
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, end, NULL));
				}
				Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
				Obj* lbobj = (Obj*)((ObjBlock*)lastblock->obj)->obj;
				String* labname2 = lab->name;
				if (get_mapy(lbobj->ifv, labname2) == NULL)
					add_mapy(thgc, lbobj->ifv, labname2, (char*)make_lifvalue(thgc, lab, NULL));
				return self;
			} else if (blkobj->type == LetterType::OT_Function || blkobj->type == LetterType::OT_ClassObj ||
				blkobj->type == LetterType::OT_ModelObj || blkobj->type == LetterType::OT_GenericObj ||
				blkobj->type == LetterType::OT_GenericFunction) {
				return ErrorC(thgc, LetterType::OT_Error, self->letter, "while,for,switchの中ではありません");
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "while,for,switchの中ではありません");
}
// ============================================================
// Continue::exepC  (lines 5930-6053)
// ============================================================
Obj* Continue_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	}
	if (val2 != NULL) val2 = GetterC_vt(thgc, val2, local);
	int count = 0;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		ObjBlock* bobj = (ObjBlock*)block->obj;
		if (bobj != NULL) {
			Obj* blkobj = (Obj*)bobj->obj;
			if (blkobj->type == LetterType::OT_IfBlock || bobj->n == 0) {
				count++;
			}
			if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For) {
				Obj* ret = NULL;
				if (blkobj->type == LetterType::OT_While) ret = ((Function*)blkobj)->rettype;
				else if (blkobj->type == LetterType::OT_For) ret = ((Function*)blkobj)->rettype;

				LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
				LVari* rnpv = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%ptr%d", llvm_n++));
				LFunc* func = llvm_get_func(local->llvm);
				add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4,
					make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
				LVari* rnpv12 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
				LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12,
					make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", count))));
				add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

				LLab* entry = blkobj->entry_lab;
				LLab* lab = *(LLab**)get_list(local->labs, local->labs->size - 1);
				if (val2 == NULL) {
					val2 = make_voival(thgc);
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, entry, NULL));
				}
				if (val2->type == LetterType::OT_Value) {
					String* labname = lab->name;
					if (get_mapy(blkobj->ifv, labname) == NULL) {
						if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For) {
							LVari* v = ((Value*)val2)->vari;
							Obj* value = val2;
							Obj* vcls = ((Value*)value)->cls;
							if (vcls == ret) {
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, ((Value*)val2)->vari));
							} else if (ret == local->Int) {
								v = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else if (ret == local->Short) {
								v = make_lvari_s(thgc, "i16", fmt_str(thgc, "%%v%d", llvm_n++));
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else if (ret == local->Bool) {
								v = make_lvari_s(thgc, "i1", fmt_str(thgc, "%%v%d", llvm_n++));
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else {
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, ((Value*)val2)->vari));
							}
							LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
							LVari* varih = make_lvari_s(thgc, "%AddHashType", fmt_str(thgc, "%%v%d", llvm_n++));
							add_list(thgc, func->comps, (char*)make_lload(thgc, varih, make_lvari_c(thgc, "%AddHashType*", "@AddHash"), false));
							LCall* ahcall = make_lcall(thgc, NULL, varih);
							lcall_add(thgc, ahcall, thgc4);
							lcall_add(thgc, ahcall, make_lvari_c(thgc, "%StringType*", "null"));
							lcall_add(thgc, ahcall, blkobj->forvari);
							lcall_add(thgc, ahcall, v);
							add_list(thgc, func->comps, (char*)ahcall);
						} else {
							Obj* value = val2;
							Obj* vcls = ((Value*)value)->cls;
							if (vcls == ret) add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, ((Value*)val2)->vari));
							else if (ret == local->Int) {
								LVari* v = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else if (ret == local->Short) {
								LVari* v = make_lvari_s(thgc, "i16", fmt_str(thgc, "%%v%d", llvm_n++));
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else if (ret == local->Bool) {
								LVari* v = make_lvari_s(thgc, "i1", fmt_str(thgc, "%%v%d", llvm_n++));
								add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Bitcast, v, ((Value*)val2)->vari));
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, v));
							} else {
								add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, ((Value*)val2)->vari));
							}
						}
					}
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, entry, NULL));
				}
				Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
				Obj* lbobj = (Obj*)((ObjBlock*)lastblock->obj)->obj;
				String* labname2 = lab->name;
				if (get_mapy(lbobj->ifv, labname2) == NULL)
					add_mapy(thgc, lbobj->ifv, labname2, (char*)make_lifvalue(thgc, lab, NULL));
				(*n)--;
				return self;
			} else if (blkobj->type == LetterType::OT_Function || blkobj->type == LetterType::OT_ClassObj ||
				blkobj->type == LetterType::OT_ModelObj || blkobj->type == LetterType::OT_GenericObj ||
				blkobj->type == LetterType::OT_GenericFunction) {
				return ErrorC(thgc, LetterType::OT_Error, self->letter, "while,forの中ではありません");
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "while,forの中ではありません");
}

// ============================================================
// Continue::PrimaryC  (lines 6055-6115)
// ============================================================
Obj* Continue_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	}
	if (val2 != NULL) val2 = GetterC_vt(thgc, val2, local);
	int count = 0;
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		ObjBlock* bobj = (ObjBlock*)block->obj;
		if (bobj != NULL) {
			Obj* blkobj = (Obj*)bobj->obj;
			if ((blkobj->type == LetterType::OT_IfBlock || blkobj->type == LetterType::OT_Switch) || bobj->n == 0) {
				count++;
			}
			else if (blkobj->type == LetterType::OT_While || blkobj->type == LetterType::OT_For) {
				LFunc* func = llvm_get_func(local->llvm);
				// dec root node count
				LVari* rnpv = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%ptr%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv, make_lvari_c(thgc, "i32**", "@rnp"), false));
				LVari* rnpv12 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
				LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12,
					make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", count))));
				add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

				LLab* end = blkobj->end_lab;
				if (blkobj->type == LetterType::OT_IfBlock) {
					end = ((IfBlock*)blkobj)->iflabel->end_lab;
				}
				LLab* lab = *(LLab**)get_list(local->labs, local->labs->size - 1);
				if (val2 == NULL) {
					val2 = make_voival(thgc);
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, end, NULL));
				}
				if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
					val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value ||
					val2->type == LetterType::OT_Block) {
					self->value_obj = val2;
					String* labname = lab->name;
					if (get_mapy(blkobj->ifv, labname) == NULL)
						add_mapy(thgc, blkobj->ifv, labname, (char*)make_lifvalue(thgc, lab, val2->vari_c));
					add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, end, NULL));
				}
				Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
				Obj* lbobj = (Obj*)((ObjBlock*)lastblock->obj)->obj;
				String* labname2 = lab->name;
				if (get_mapy(lbobj->ifv, labname2) == NULL)
					add_mapy(thgc, lbobj->ifv, labname2, (char*)make_lifvalue(thgc, lab, NULL));
				return self;
			} else if (blkobj->type == LetterType::OT_Function || blkobj->type == LetterType::OT_ClassObj ||
				blkobj->type == LetterType::OT_ModelObj || blkobj->type == LetterType::OT_GenericObj ||
				blkobj->type == LetterType::OT_GenericFunction) {
				return ErrorC(thgc, LetterType::OT_Error, self->letter, "while,forの中ではありません");
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "while,forの中ではありません");
}

// ============================================================
// Return::exepC  (lines 6117-6242)
// ============================================================
Obj* Return_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	List* blocks = get_blocks(local);
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	(*n)++;
	Obj* val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	}
	if (val2 != NULL) val2 = GetterC_vt(thgc, val2, local);
	int count = 1;
	LFunc* func = llvm_get_func(local->llvm);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		ObjBlock* bobj = (ObjBlock*)block->obj;
		if (bobj != NULL) {
			Obj* blkobj = (Obj*)bobj->obj;
			if ((blkobj->type == LetterType::OT_IfBlock || blkobj->type == LetterType::OT_While ||
				blkobj->type == LetterType::OT_For || blkobj->type == LetterType::OT_Switch) || bobj->n == 0) {
				count++;
			}
			if (blkobj->type == LetterType::OT_Function || blkobj->type == LetterType::OT_GenericFunction ||
				blkobj->type == LetterType::OT_ServerFunction || blkobj->type == LetterType::OT_SignalFunction) {
				LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
				LVari* rnpv = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%ptr%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4,
					make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
				LVari* rnpv12 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
				LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12,
					make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", count))));
				add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));
				(*n)--;
				if (func->async_flag) {
					// async return path
					LVari* obj = make_lvari_c(thgc, "%CoroFrameType*", "%obj");
					if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
						val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value ||
						val2->type == LetterType::OT_Block) {
						LVari* retp = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%ret%d", llvm_n++));
						add_list(thgc, local->llvm->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", retp, obj,
							make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "9")));
						LVari* vari = ((Value*)val2)->vari;
						if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_BoolVal) {
							vari = make_lvari_s(thgc, "i8*", fmt_str(thgc, "%%v%d", llvm_n++));
							add_list(thgc, local->llvm->comps, (char*)make_lcast(thgc, LCAST_IntToPtr, vari, ((Value*)val2)->vari));
						}
						add_list(thgc, local->llvm->comps, (char*)make_lstore(thgc, retp, vari));
					} else if (val2->type == LetterType::OT_Comment) {
						LVari* clf = make_lvari_s(thgc, "%CreateListType", fmt_str(thgc, "%%clf%d", llvm_n++));
						add_list(thgc, func->comps, (char*)make_lload(thgc, clf, make_lvari_c(thgc, "%CreateListType*", "@CreateList"), false));
						LVari* list = make_lvari_s(thgc, "%ListType*", fmt_str(thgc, "%%list%d", llvm_n++));
						LCall* clcall = make_lcall(thgc, list, clf);
						lcall_add(thgc, clcall, make_lvari_c(thgc, "%ThreadGCType*", "%thgc"));
						lcall_add(thgc, clcall, make_lvari_c(thgc, "i32", "8"));
						lcall_add(thgc, clcall, make_lvari_c(thgc, "i1", "true"));
						add_list(thgc, func->comps, (char*)clcall);
						LVari* v = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%v%d", llvm_n++));
						Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
						Obj* lbobj = (Obj*)((ObjBlock*)lastblock->obj)->obj;
						if (lbobj->type == LetterType::OT_IfBlock)
							add_list(thgc, func->comps, (char*)make_lload(thgc, v, ((IfBlock*)lbobj)->iflabel->bas, false));
						else
							add_list(thgc, func->comps, (char*)make_lload(thgc, v, lbobj->bas, false));
						LCall* cmcall = make_lcall(thgc, NULL, val2->vari_c);
						lcall_add(thgc, cmcall, make_lvari_c(thgc, "%RootNodeType*", "%rn"));
						lcall_add(thgc, cmcall, v);
						lcall_add(thgc, cmcall, make_lvari_c(thgc, "%ElementType*", "null"));
						lcall_add(thgc, cmcall, make_lvari_c(thgc, "%TreeElementType*", "null"));
						lcall_add(thgc, cmcall, list);
						add_list(thgc, func->comps, (char*)cmcall);
					} else {
						return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "return");
					}
					// state + queue + push + ret
					LVari* stateval = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%state%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateval, obj,
						make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
					LVari* sfinp = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%sfinp%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", sfinp, obj,
						make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "10")));
					LVari* sfinv = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%sfinv%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, sfinv, sfinp, false));
					add_list(thgc, func->comps, (char*)make_lstore(thgc, stateval, sfinv));
					LVari* queueptr = make_lvari_s(thgc, "%CoroutineQueueType**", fmt_str(thgc, "%%queueptr%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr, obj,
						make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
					LVari* queue = make_lvari_s(thgc, "%CoroutineQueueType*", fmt_str(thgc, "%%queue%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, queue, queueptr, false));
					LVari* pushqueue = make_lvari_s(thgc, "%PushQueueType*", fmt_str(thgc, "%%pushqueue%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
					LCall* pqcall = make_lcall(thgc, NULL, pushqueue);
					lcall_add(thgc, pqcall, queue);
					lcall_add(thgc, pqcall, obj);
					add_list(thgc, func->comps, (char*)pqcall);
					add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
				}
				if (val2 == NULL) {
					add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "null")));
					LLab* lab = *(LLab**)get_list(local->labs, local->labs->size - 1);
					Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
					Obj* lbobj = (Obj*)((ObjBlock*)lastblock->obj)->obj;
					String* labname = lab->name;
					if (get_mapy(lbobj->ifv, labname) == NULL)
						add_mapy(thgc, lbobj->ifv, labname, (char*)make_lifvalue(thgc, lab, NULL));
					val2 = make_voival(thgc);
					Value* result = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
					result->vari = NULL;
					return (Obj*)result;
				}
				if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
					val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value ||
					val2->type == LetterType::OT_Block) {
					add_list(thgc, func->comps, (char*)make_lret(thgc, ((Value*)val2)->vari));
					LLab* lab = *(LLab**)get_list(local->labs, local->labs->size - 1);
					Block* lastblock = *(Block**)get_list(blocks, blocks->size - 1);
					Obj* lbobj = (Obj*)((ObjBlock*)lastblock->obj)->obj;
					String* labname = lab->name;
					if (get_mapy(lbobj->ifv, labname) == NULL)
						add_mapy(thgc, lbobj->ifv, labname, (char*)make_lifvalue(thgc, lab, NULL));
					self->vari_c = ((Value*)val2)->vari;
					Value* result = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
					result->vari = ((Value*)val2)->vari;
					return (Obj*)result;
				} else if (val2->type == LetterType::OT_Comment) {
					LVari* clf2 = make_lvari_s(thgc, "%CreateListType", fmt_str(thgc, "%%clf%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, clf2, make_lvari_c(thgc, "%CreateListType*", "@CreateList"), false));
					LVari* list2 = make_lvari_s(thgc, "%ListType*", fmt_str(thgc, "%%list%d", llvm_n++));
					LCall* clcall2 = make_lcall(thgc, list2, clf2);
					lcall_add(thgc, clcall2, make_lvari_c(thgc, "%ThreadGCType*", "%thgc"));
					lcall_add(thgc, clcall2, make_lvari_c(thgc, "i32", "8"));
					lcall_add(thgc, clcall2, make_lvari_c(thgc, "i1", "true"));
					add_list(thgc, func->comps, (char*)clcall2);
					LVari* v2 = make_lvari_s(thgc, "%GCObjectPtr", fmt_str(thgc, "%%v%d", llvm_n++));
					Block* lastblock3 = *(Block**)get_list(blocks, blocks->size - 1);
					Obj* lbobj3 = (Obj*)((ObjBlock*)lastblock3->obj)->obj;
					if (lbobj3->type == LetterType::OT_IfBlock)
						add_list(thgc, func->comps, (char*)make_lload(thgc, v2, ((IfBlock*)lbobj3)->iflabel->bas, false));
					else
						add_list(thgc, func->comps, (char*)make_lload(thgc, v2, lbobj3->bas, false));
					LCall* cmcall2 = make_lcall(thgc, NULL, val2->vari_c);
					lcall_add(thgc, cmcall2, make_lvari_c(thgc, "%RootNodeType*", "%rn"));
					lcall_add(thgc, cmcall2, v2);
					lcall_add(thgc, cmcall2, make_lvari_c(thgc, "%ElementType*", "null"));
					lcall_add(thgc, cmcall2, make_lvari_c(thgc, "%TreeElementType*", "null"));
					lcall_add(thgc, cmcall2, list2);
					add_list(thgc, func->comps, (char*)cmcall2);
					return val2;
				}
				return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "return");
			} else if (blkobj->type == LetterType::OT_ClassObj) {
				return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "関数の中ではありません");
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "return文は関数の中でしか使えません");
}

// ============================================================
// Return::PrimaryC  (lines 6244-6296)
// ============================================================
Obj* Return_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	}
	if (val2 != NULL) val2 = GetterC_vt(thgc, val2, local);
	int count = 1;
	LFunc* func = llvm_get_func(local->llvm);
	for (int i = blocks->size - 1; i >= 0; i--) {
		Block* block = *(Block**)get_list(blocks, i);
		ObjBlock* bobj = (ObjBlock*)block->obj;
		if (bobj != NULL) {
			Obj* blkobj = (Obj*)bobj->obj;
			if ((blkobj->type == LetterType::OT_IfBlock || blkobj->type == LetterType::OT_While ||
				blkobj->type == LetterType::OT_For || blkobj->type == LetterType::OT_Switch) || bobj->n == 0) {
				count++;
			}
			if (blkobj->type == LetterType::OT_Function || blkobj->type == LetterType::OT_GenericFunction ||
				blkobj->type == LetterType::OT_ServerFunction || blkobj->type == LetterType::OT_SignalFunction) {
				LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
				LVari* rnpv = make_lvari_s(thgc, "i32*", fmt_str(thgc, "%%ptr%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4,
					make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
				LVari* rnpv12 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
				LVari* rnpv2 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%dec%d", llvm_n++));
				add_list(thgc, func->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12,
					make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", count))));
				add_list(thgc, func->comps, (char*)make_lstore(thgc, rnpv, rnpv2));
				if (val2 == NULL) {
					add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", "null")));
					val2 = make_voival(thgc);
					Value* result = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
					result->vari = NULL;
					return (Obj*)result;
				}
				if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj ||
					val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value ||
					val2->type == LetterType::OT_Block) {
					add_list(thgc, func->comps, (char*)make_lret(thgc, ((Value*)val2)->vari));
					self->vari_c = ((Value*)val2)->vari;
					Value* result = (Value*)make_val(thgc, CType::_CValue, LetterType::OT_Value, NULL);
					result->vari = ((Value*)val2)->vari;
					return (Obj*)result;
				}
				return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "return");
			} else if (blkobj->type == LetterType::OT_ClassObj) {
				return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "関数の中ではありません");
			}
		}
	}
	return ErrorC(thgc, LetterType::OT_Error, val2 ? val2->letter : NULL, "return文は関数の中でしか使えません");
}

// ============================================================
// Goto::exepC  (lines 6298-6307)
// ============================================================
Obj* Goto_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Goto::PrimaryC  (lines 6308-6331)
// ============================================================
Obj* Goto_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->letter == local->letter && local->kouhos == NULL) {
		KouhoSetLabel(thgc, local);
	}
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
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "goto文の行き先を指定してください。");
}

// ============================================================
// Print::exepC  (lines 6332-6341)
// ============================================================
Obj* Print_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Print::PrimaryC  (lines 6342-6491)
// ============================================================
Obj* Print_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Bracket) {
		Obj* val = GetterC_vt(thgc, exeC(thgc, val2, local), local);
		Block* blk = (Block*)val;
		LFunc* func = llvm_get_func(local->llvm);
		// putchar('(')
		LCall* pc0 = make_lcall(thgc, NULL, make_lvari_c(thgc, "i32", "@putchar"));
		lcall_add(thgc, pc0, make_lvari_c(thgc, "i32", "40"));
		add_list(thgc, func->comps, (char*)pc0);
		for (int i = 0; i < blk->rets->size; i++) {
			if (i != 0) {
				LCall* pccomma = make_lcall(thgc, NULL, make_lvari_c(thgc, "i32", "@putchar"));
				lcall_add(thgc, pccomma, make_lvari_c(thgc, "i32", "44"));
				add_list(thgc, func->comps, (char*)pccomma);
			}
			Obj* reti = *(Obj**)get_list(blk->rets, i);
			if (reti->type == LetterType::OT_Value) {
				Obj* value = reti;
				Obj* vcls = ((Value*)value)->cls;
				if (vcls == local->Int) {
					LVari* v0 = make_lvari_s(thgc, "%NumberStringType", fmt_str(thgc, "%%v%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, v0, make_lvari_c(thgc, "%NumberStringType*", "@NumberString"), false));
					LVari* v05 = make_lvari_s(thgc, "%StringType*", fmt_str(thgc, "%%v%d", llvm_n++));
					LCall* nscall = make_lcall(thgc, v05, v0);
					lcall_add(thgc, nscall, make_lvari_c(thgc, "%ThreadGCType*", "%thgc"));
					lcall_add(thgc, nscall, ((Value*)value)->vari);
					add_list(thgc, func->comps, (char*)nscall);
					LVari* v = make_lvari_s(thgc, "%PrintStringType", fmt_str(thgc, "%%v%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, v, make_lvari_c(thgc, "%PrintStringType*", "@PrintString"), false));
					LCall* pscall = make_lcall(thgc, NULL, v);
					lcall_add(thgc, pscall, v05);
					add_list(thgc, func->comps, (char*)pscall);
				} else if (vcls == local->Short) {
					LVari* v3 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Zext, v3, ((Value*)value)->vari));
					LVari* v0 = make_lvari_s(thgc, "%NumberStringType", fmt_str(thgc, "%%v%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, v0, make_lvari_c(thgc, "%NumberStringType*", "@NumberString"), false));
					LVari* v05 = make_lvari_s(thgc, "%StringType*", fmt_str(thgc, "%%v%d", llvm_n++));
					LCall* nscall = make_lcall(thgc, v05, v0);
					lcall_add(thgc, nscall, make_lvari_c(thgc, "%ThreadGCType*", "%thgc"));
					lcall_add(thgc, nscall, v3);
					add_list(thgc, func->comps, (char*)nscall);
					LVari* v = make_lvari_s(thgc, "%PrintStringType", fmt_str(thgc, "%%v%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, v, make_lvari_c(thgc, "%PrintStringType*", "@PrintString"), false));
					LCall* pscall = make_lcall(thgc, NULL, v);
					lcall_add(thgc, pscall, v05);
					add_list(thgc, func->comps, (char*)pscall);
				} else if (vcls == local->Bool) {
					// empty for bool
				} else if (vcls == local->StrT) {
					LVari* v = make_lvari_s(thgc, "%PrintStringType", fmt_str(thgc, "%%v%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, v, make_lvari_c(thgc, "%PrintStringType*", "@PrintString"), false));
					LCall* pscall = make_lcall(thgc, NULL, v);
					lcall_add(thgc, pscall, ((Value*)value)->vari);
					add_list(thgc, func->comps, (char*)pscall);
				} else if (vcls->type == LetterType::OT_ArrayType) {
					int n2 = 0;
					int depth = 0;
					Obj* cls = vcls;
					// loop for nested array types
					while (cls->type == LetterType::OT_ArrayType) {
						Obj* arrcls = ((Var*)cls)->cls; // arrtype.cls
						if (arrcls == local->Int || arrcls == local->Short) { n2 = 1; break; }
						else if (arrcls == local->StrT) { n2 = 0; break; }
						else if (arrcls->type == LetterType::OT_ArrayType) { depth++; cls = arrcls; }
						else break;
					}
					LVari* pav = make_lvari_s(thgc, "%PrintArrayType", fmt_str(thgc, "%%v%d", llvm_n++));
					add_list(thgc, func->comps, (char*)make_lload(thgc, pav, make_lvari_c(thgc, "%PrintArrayType*", "@PrintArray"), false));
					LCall* pacall = make_lcall(thgc, NULL, pav);
					lcall_add(thgc, pacall, ((Value*)value)->vari);
					lcall_add(thgc, pacall, make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", depth)));
					lcall_add(thgc, pacall, make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", n2)));
					add_list(thgc, func->comps, (char*)pacall);
				}
			}
		}
		// putchar(')')
		LCall* pc1 = make_lcall(thgc, NULL, make_lvari_c(thgc, "i32", "@putchar"));
		lcall_add(thgc, pc1, make_lvari_c(thgc, "i32", "41"));
		add_list(thgc, func->comps, (char*)pc1);
		// putchar('\n')
		LCall* pc2 = make_lcall(thgc, NULL, make_lvari_c(thgc, "i32", "@putchar"));
		lcall_add(thgc, pc2, make_lvari_c(thgc, "i32", "10"));
		add_list(thgc, func->comps, (char*)pc2);
		(*n)++;
		return make_voival(thgc);
	}
	// non-bracket path
	val2 = NULL;
	for (; *n < primary->children->size - 1;) {
		Primary_NextC(thgc, primary, local, n, &val2);
		if (val2->type == LetterType::OT_Wait || val2->type == LetterType::OT_Error || val2->type == LetterType::OT_NG) return val2;
	}
	val2 = GetterC_vt(thgc, val2, local);
	if (val2->type == LetterType::OT_Value) {
		(*n)++;
		Obj* value = val2;
		Obj* vcls = ((Value*)value)->cls;
		LFunc* func = llvm_get_func(local->llvm);
		if (vcls == local->Int) {
			LVari* v0 = make_lvari_s(thgc, "%NumberStringType", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, v0, make_lvari_c(thgc, "%NumberStringType*", "@NumberString"), false));
			LVari* v05 = make_lvari_s(thgc, "%StringType*", fmt_str(thgc, "%%v%d", llvm_n++));
			LCall* nscall = make_lcall(thgc, v05, v0);
			lcall_add(thgc, nscall, make_lvari_c(thgc, "%ThreadGCType*", "%thgc"));
			lcall_add(thgc, nscall, ((Value*)value)->vari);
			add_list(thgc, func->comps, (char*)nscall);
			LVari* v = make_lvari_s(thgc, "%PrintStringType", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, v, make_lvari_c(thgc, "%PrintStringType*", "@PrintString"), false));
			LCall* pscall = make_lcall(thgc, NULL, v);
			lcall_add(thgc, pscall, v05);
			add_list(thgc, func->comps, (char*)pscall);
		} else if (vcls == local->Short) {
			LVari* v3 = make_lvari_s(thgc, "i32", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lcast(thgc, LCAST_Zext, v3, ((Value*)value)->vari));
			LVari* v0 = make_lvari_s(thgc, "%NumberStringType", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, v0, make_lvari_c(thgc, "%NumberStringType*", "@NumberString"), false));
			LVari* v05 = make_lvari_s(thgc, "%StringType*", fmt_str(thgc, "%%v%d", llvm_n++));
			LCall* nscall = make_lcall(thgc, v05, v0);
			lcall_add(thgc, nscall, make_lvari_c(thgc, "%ThreadGCType*", "%thgc"));
			lcall_add(thgc, nscall, v3);
			add_list(thgc, func->comps, (char*)nscall);
			LVari* v = make_lvari_s(thgc, "%PrintStringType", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, v, make_lvari_c(thgc, "%PrintStringType*", "@PrintString"), false));
			LCall* pscall = make_lcall(thgc, NULL, v);
			lcall_add(thgc, pscall, v05);
			add_list(thgc, func->comps, (char*)pscall);
		} else if (vcls == local->Bool) {
			// empty for bool
		} else if (vcls == local->StrT) {
			LVari* v = make_lvari_s(thgc, "%PrintStringType", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, v, make_lvari_c(thgc, "%PrintStringType*", "@PrintString"), false));
			LCall* pscall = make_lcall(thgc, NULL, v);
			lcall_add(thgc, pscall, ((Value*)val2)->vari);
			add_list(thgc, func->comps, (char*)pscall);
		} else if (vcls->type == LetterType::OT_ArrayType) {
			int n2 = 0;
			int depth = 0;
			Obj* cls = vcls;
			while (cls->type == LetterType::OT_ArrayType) {
				Obj* arrcls = ((Var*)cls)->cls;
				if (arrcls == local->Int || arrcls == local->Short) { n2 = 1; break; }
				else if (arrcls == local->StrT) { n2 = 0; break; }
				else if (arrcls->type == LetterType::OT_ArrayType) { depth++; cls = arrcls; }
				else break;
			}
			LVari* pav = make_lvari_s(thgc, "%PrintArrayType", fmt_str(thgc, "%%v%d", llvm_n++));
			add_list(thgc, func->comps, (char*)make_lload(thgc, pav, make_lvari_c(thgc, "%PrintArrayType*", "@PrintArray"), false));
			LCall* pacall = make_lcall(thgc, NULL, pav);
			lcall_add(thgc, pacall, ((Value*)value)->vari);
			lcall_add(thgc, pacall, make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", depth)));
			lcall_add(thgc, pacall, make_lvari_s(thgc, "i32", fmt_str(thgc, "%d", n2)));
			add_list(thgc, func->comps, (char*)pacall);
		}
		return make_voival(thgc);
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "()でprintの引数を指定してください");
}

// ============================================================
// Iterator::GetterC  (lines 6492-6499)
// ============================================================
Obj* Iterator_GetterC(ThreadGC* thgc, Obj* self, Master* local) {
	// Iterator has fields m and n and value (Block*)
	int m = self->n_c; // repurpose n_c for m
	int nn = self->order; // repurpose order for n
	Block* value = (Block*)self->value_obj;
	if (m == -1) return *(Obj**)get_list(value->rets, nn);
	else {
		Block* inner = (Block*)*(Obj**)get_list(value->rets, nn);
		return *(Obj**)get_list(inner->rets, m);
	}
}

// ============================================================
// GenericFunction::PrimaryC  (lines 6500-6522)
// ============================================================
Obj* GenericFunction_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Block) {
		add_list(thgc, local->calls, (char*)KouhoSetType);
		Obj* val = GetterC_vt(thgc, exeC(thgc, val2, local), local);
		local->calls->size--;
		if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
		Block* blk = (Block*)val;
		GenericFunction* gf = (GenericFunction*)self;
		if (blk->rets->size != gf->vmap->kvs->size)
			return ErrorC(thgc, LetterType::OT_Error, val2->letter, "引数の数があっていません。");
		for (int i = 0; i < blk->rets->size; i++) {
			Obj* reti = *(Obj**)get_list(blk->rets, i);
			// check that each arg is a Type
			if (reti->type != LetterType::OT_ClassObj && reti->type != LetterType::OT_ModelObj &&
				reti->type != LetterType::OT_Value)
				return ErrorC(thgc, LetterType::OT_Error, val2->letter, "ジェネリック関数には型を引数にしてください");
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		// Return a new Function with the generic type params resolved
		Obj* result = make_function(thgc, LetterType::OT_Function, NULL);
		((Function*)result)->rettype = gf->rettype;
		((Function*)result)->blocks = gf->blocks;
		((Function*)result)->draw = gf->draw;
		return result;
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "[]でジェリック関数の引数を指定してください");
}

// ============================================================
// Model::exepC  (lines 6523-6533)
// ============================================================
Obj* Model_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		call_last(local->calls, thgc, local);
	}
	return self;
}

// obj2c_part4.h - Obj2C.cs lines 8000-11000
// Model::PrimaryC continued (Delete, FirstAwait, SelectAwait, Store, Delete .await/.Bracket paths)
// All code is inside the ongoing Model_PrimaryC function body from obj2c_part3.

// ============================================================
// Helper: emit SQL string creation sequence
// ============================================================
static inline LVari* emit_create_string_sql(ThreadGC* thgc, Master* local, const char* sql, int sql_len) {
	LVari* csvari = make_vn(thgc, "%CreateStringType", "cs");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, csvari, make_lvari_c(thgc, "%CreateStringType*", "@CreateString"), false));
	LVari* v2 = make_vn(thgc, "i8*", "v2");
	char sname[64]; snprintf(sname, sizeof(sname), "@s%d", llvm_n++);
	LStrV* sv = make_lstrv(thgc, sname, sql, sql_len);
	add_list(thgc, local->llvm->strs, (char*)sv);
	char gtype[64]; snprintf(gtype, sizeof(gtype), "[%d x i8]", sql_len + 1);
	LGete* ge = make_lgete_idx2(thgc, gtype, v2, (LVari*)sv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0"));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ge);
	LVari* v = make_vn(thgc, "%StringType*", "v");
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	char slen[32]; snprintf(slen, sizeof(slen), "%d", sql_len);
	LCall* call = make_lcall(thgc, v, csvari);
	lcall_add(thgc, call, thgc4);
	lcall_add(thgc, call, v2);
	lcall_add(thgc, call, make_lvari_c(thgc, "i32", slen));
	lcall_add(thgc, call, make_lvari_c(thgc, "i32", "1"));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)call);
	return v;
}

// ============================================================
// Helper: emit SqlParam for Int type
// ============================================================
static inline void emit_sql_param_int(ThreadGC* thgc, Master* local, LVari* ps, int varis_count, int key, LVari* value_vari) {
	char arrtype[64]; snprintf(arrtype, sizeof(arrtype), "[%d x %%SqlParamType]", varis_count);
	char keystr[16]; snprintf(keystr, sizeof(keystr), "%d", key);
	LVari* pptr = make_vn(thgc, "%SqlParamType*", "pp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, arrtype, pptr, ps, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", keystr)));
	LVari* kptr = make_vn(thgc, "i32*", "kp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%SqlParamType", kptr, pptr, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, kptr, make_lvari_c(thgc, "i32", "1")));
	LVari* vptr = make_vn(thgc, "i32*", "vp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%SqlParamType", vptr, pptr, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, vptr, value_vari));
}

// ============================================================
// Helper: emit SqlParam for Str type (with UTF8 conversion)
// ============================================================
static inline void emit_sql_param_str(ThreadGC* thgc, Master* local, LVari* ps, int varis_count, int key, LVari* value_vari) {
	char arrtype[64]; snprintf(arrtype, sizeof(arrtype), "[%d x %%SqlParamType]", varis_count);
	char keystr[16]; snprintf(keystr, sizeof(keystr), "%d", key);
	LVari* pptr = make_vn(thgc, "%SqlParamType*", "pp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, arrtype, pptr, ps, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", keystr)));
	LVari* kptr = make_vn(thgc, "i32*", "kp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%SqlParamType", kptr, pptr, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, kptr, make_lvari_c(thgc, "i32", "4")));
	LVari* vptr = make_vn(thgc, "%TextType*", "vp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%SqlParamType", vptr, pptr, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	// StringToUTF8
	LVari* utf8f = make_lvari_c(thgc, "%StringUTF8", "%utf8f");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, utf8f, make_lvari_c(thgc, "%StringUTF8*", "@StringUTF8"), false));
	LVari* uc = make_vn(thgc, "i32*", "uc");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lalloca(thgc, uc));
	LVari* utf8v = make_vn(thgc, "i8*", "ut");
	LCall* ucall = make_lcall(thgc, utf8v, utf8f);
	lcall_add(thgc, ucall, value_vari);
	lcall_add(thgc, ucall, uc);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ucall);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, vptr, utf8v));
	LVari* lptr = make_vn(thgc, "i32*", "lp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%TextType", lptr, vptr, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* uv = make_vn(thgc, "i32", "uv");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, uv, uc, false));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, lptr, uv));
}

// ============================================================
// Helper: emit async ExecSql + coroutine push + await return
// ============================================================
static inline void emit_async_execsql_delete(ThreadGC* thgc, Master* local, LVari* v, LVari* ps, int pcount) {
	LVari* tx = make_vn(thgc, "i8*", "tx");
	LVari* txp = make_vn(thgc, "i8**", "tx");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txp, make_lvari_c(thgc, "%RootNodeType*", "%rn"), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, tx, txp, false));
	LVari* esql = make_lvari_c(thgc, "%ExecSqlType", "%esql");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, esql, make_lvari_c(thgc, "%ExecSqlType*", "@ExecSql"), false));
	LVari* coroptr = make_lvari_c(thgc, "%CoroFrameType**", "%coroptr");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lalloca(thgc, coroptr));
	char pcstr[16]; snprintf(pcstr, sizeof(pcstr), "%d", pcount);
	LCall* ecall = make_lcall(thgc, NULL, esql);
	lcall_add(thgc, ecall, coroptr);
	lcall_add(thgc, ecall, tx);
	lcall_add(thgc, ecall, v);
	lcall_add(thgc, ecall, ps);
	lcall_add(thgc, ecall, make_lvari_c(thgc, "i32", pcstr));
	lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", "null"));
	lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", "null"));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ecall);
	LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, hv, coroptr, false));

	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* thgcv = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* qv = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qv, (LVari*)get_list_val(llvm_get_func(local->llvm)->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, qv2, qv, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, queueptr2, qv2));
	LVari* stateptr = make_vn(thgc, "i32*", "st");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateptr, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, stateptr, make_lvari_c(thgc, "i32", "0")));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qv2);
	lcall_add(thgc, pqcall, hv);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)pqcall);
	LVari* statevalptr2 = make_vn(thgc, "i32*", "st");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(llvm_get_func(local->llvm)->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	char nstr[16]; snprintf(nstr, sizeof(nstr), "%d", llvm_n++);
	LVari* nvari = make_lvari_c(thgc, "i32", nstr);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, statevalptr2, nvari));

	LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(llvm_get_func(local->llvm)->draws, 0)));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	char labname[32]; snprintf(labname, sizeof(labname), "aw%d", llvm_n++);
	LLab* lab6 = make_llab(thgc, labname);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)lab6);
}

// ============================================================
// Helper: emit coroutine function setup (BeginTransaction path)
// ============================================================
static inline LFunc* emit_coro_func_setup(ThreadGC* thgc, Master* local, const char* prefix, LVari** out_statev) {
	LVari* obj = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
	char fname[64]; snprintf(fname, sizeof(fname), "@%s%d", prefix, llvm_n++);
	LVari* fy = make_lvari_c(thgc, "void", fname);
	LFunc* func = make_lfunc(thgc, local->llvm, fy, obj);
	add_list(thgc, local->llvm->comps, (char*)func);
	func->async_flag = true;

	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* rngv = make_lvari_c(thgc, "%RootNodeType**", "%rngv");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", rngv, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
	LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	add_list(thgc, func->comps, (char*)make_lload(thgc, rn4, rngv, false));
	add_list(thgc, func->comps, (char*)make_lload(thgc, thgc4, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));

	LVari* geterv = make_lvari_c(thgc, "i32*", "%state");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", geterv, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	LVari* statev = make_vn(thgc, "i32", "st");
	add_list(thgc, func->comps, (char*)make_lload(thgc, statev, geterv, false));
	*out_statev = statev;
	return func;
}

// ============================================================
// Helper: emit BeginTransaction block in coroutine function
// ============================================================
static inline void emit_begin_transaction_block(ThreadGC* thgc, LFunc* func, List* list, LLab* lab10) {
	LVari* obj = (LVari*)get_list_val(func->draws, 0);

	add_list(thgc, list, (char*)make_lifvalue(thgc, lab10, make_lvari_c(thgc, "i32", "-2")));
	add_list(thgc, func->comps, (char*)lab10);
	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* qvp = make_vn(thgc, "%CoroutineQueueType**", "qv");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qvp, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* qv1 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, qv1, qvp, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, queueptr2, qv1));
	LVari* hvn = make_vn(thgc, "%CoroFrameType*", "hv");
	LVari* bt = make_lvari_c(thgc, "%BeginTransactionType", "%bt");
	LVari* alv = make_vn(thgc, "%CoroFrameType**", "al");
	add_list(thgc, func->comps, (char*)make_lalloca(thgc, alv));
	add_list(thgc, func->comps, (char*)make_lload(thgc, bt, make_lvari_c(thgc, "%BeginTransactionType*", "@BeginTransaction"), false));
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LCall* btcall = make_lcall(thgc, NULL, bt);
	lcall_add(thgc, btcall, alv);
	lcall_add(thgc, btcall, thgc4);
	lcall_add(thgc, btcall, make_lvari_c(thgc, "ptr", "@sqlp"));
	lcall_add(thgc, btcall, qv1);
	add_list(thgc, func->comps, (char*)btcall);
	add_list(thgc, func->comps, (char*)make_lload(thgc, hvn, alv, false));
	LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hvn, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(func->draws, 0)));
	LVari* qp = make_vn(thgc, "%CoroutineQueueType**", "qp");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qp, hvn, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, qp, qv1));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qv1);
	lcall_add(thgc, pqcall, hvn);
	add_list(thgc, func->comps, (char*)pqcall);
}

// ============================================================
// Helper: emit coro_start label + state store + ret
// ============================================================
static inline LLab* emit_coro_start_block(ThreadGC* thgc, LFunc* func, List* list) {
	char labname[32]; snprintf(labname, sizeof(labname), "coro_start%d", llvm_n++);
	LLab* lab10 = make_llab(thgc, labname);
	add_list(thgc, list, (char*)make_lifvalue(thgc, lab10, make_lvari_c(thgc, "i32", "0")));
	LVari* statevalptr3 = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr3, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, statevalptr3, make_lvari_c(thgc, "i32", "0")));
	LVari* sfinp = make_vn(thgc, "i32*", "sf");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", sfinp, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "10")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, sfinp, make_lvari_c(thgc, "i32", "-3")));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	add_list(thgc, func->comps, (char*)lab10);
	return lab10;
}

// ============================================================
// Helper: emit ExecSql in coroutine function
// ============================================================
static inline LVari* emit_coro_execsql(ThreadGC* thgc, LFunc* func, LVari* obj, LVari* rn4, LVari* v, LVari* ps, int pcount, const char* dbselect, LVari* sqp) {
	LVari* txp = make_vn(thgc, "i8**", "tx");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", txp, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "8")));
	LVari* tx = make_vn(thgc, "i8*", "tx");
	add_list(thgc, func->comps, (char*)make_lload(thgc, tx, txp, false));
	LVari* txps = make_vn(thgc, "i8**", "tx");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txps, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, txps, tx));
	LVari* esql = make_lvari_c(thgc, "%ExecSqlType", "%esql");
	add_list(thgc, func->comps, (char*)make_lload(thgc, esql, make_lvari_c(thgc, "%ExecSqlType*", "@ExecSql"), false));
	LVari* coroptr = make_lvari_c(thgc, "%CoroFrameType**", "%coroptr");
	add_list(thgc, func->comps, (char*)make_lalloca(thgc, coroptr));
	// Load triptr
	LVari* op = make_vn(thgc, "%Triptr**", "op");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", op, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
	LVari* obv = make_vn(thgc, "%Triptr*", "ob");
	add_list(thgc, func->comps, (char*)make_lload(thgc, obv, op, false));
	LVari* vp = make_vn(thgc, "%StringType**", "vp");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%Triptr", vp, obv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* v2 = make_vn(thgc, "%StringType*", "v2");
	add_list(thgc, func->comps, (char*)make_lload(thgc, v2, vp, false));
	LVari* pps1 = make_vn(thgc, "%SqlParamType**", "pp");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%Triptr", pps1, obv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* ps2 = make_vn(thgc, "ptr", "ps");
	add_list(thgc, func->comps, (char*)make_lload(thgc, ps2, pps1, false));
	char pcstr[16]; snprintf(pcstr, sizeof(pcstr), "%d", pcount);
	if (dbselect != NULL) {
		LVari* sqpp = make_vn(thgc, "ptr", "sq");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%Triptr", sqpp, obv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
		LVari* sqpv = make_vn(thgc, "ptr", "sq");
		add_list(thgc, func->comps, (char*)make_lload(thgc, sqpv, sqpp, false));
		LCall* ecall = make_lcall(thgc, NULL, esql);
		lcall_add(thgc, ecall, coroptr); lcall_add(thgc, ecall, tx); lcall_add(thgc, ecall, v2);
		lcall_add(thgc, ecall, ps); lcall_add(thgc, ecall, make_lvari_c(thgc, "i32", pcstr));
		lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", dbselect));
		lcall_add(thgc, ecall, sqpv);
		add_list(thgc, func->comps, (char*)ecall);
	} else {
		LCall* ecall = make_lcall(thgc, NULL, esql);
		lcall_add(thgc, ecall, coroptr); lcall_add(thgc, ecall, tx); lcall_add(thgc, ecall, v2);
		lcall_add(thgc, ecall, ps); lcall_add(thgc, ecall, make_lvari_c(thgc, "i32", pcstr));
		lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", "null"));
		lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", "null"));
		add_list(thgc, func->comps, (char*)ecall);
	}
	return coroptr;
}

// ============================================================
// Helper: emit coroutine push + state update in coro func
// ============================================================
static inline void emit_coro_push_and_state(ThreadGC* thgc, LFunc* func, LVari* hv, List* list) {
	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* qv = make_vn(thgc, "%CoroutineQueueType**", "qv");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qv, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, qv2, qv, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, queueptr2, qv2));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qv2);
	lcall_add(thgc, pqcall, hv);
	add_list(thgc, func->comps, (char*)pqcall);
	LVari* statevalptr2 = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	LVari* sfinp2 = make_vn(thgc, "i32*", "sf");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", sfinp2, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "10")));
	LVari* sfinv2 = make_vn(thgc, "i32", "sf");
	add_list(thgc, func->comps, (char*)make_lload(thgc, sfinv2, sfinp2, false));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, statevalptr2, sfinv2));

	LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(func->draws, 0)));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
}

// ============================================================
// Helper: emit TxCommit block in coro func
// ============================================================
static inline void emit_coro_txcommit(ThreadGC* thgc, LFunc* func, LVari* obj, LVari* rn4, List* list) {
	char labname[32]; snprintf(labname, sizeof(labname), "coro_txfin");
	LLab* lab10 = make_llab(thgc, labname);
	add_list(thgc, list, (char*)make_lifvalue(thgc, lab10, make_lvari_c(thgc, "i32", "-3")));
	add_list(thgc, func->comps, (char*)lab10);

	LVari* rp = make_vn(thgc, "i8**", "rp");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", rp, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "8")));
	LVari* rv = make_vn(thgc, "i8*", "rv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, rv, rp, false));
	LVari* rp2 = make_vn(thgc, "i8**", "rp");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", rp2, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "9")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, rp2, rv));
	LVari* txp = make_vn(thgc, "i8**", "tx");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txp, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	LVari* tx = make_vn(thgc, "i8*", "tx");
	add_list(thgc, func->comps, (char*)make_lload(thgc, tx, txp, false));
	LVari* txcf = make_vn(thgc, "%TxFinishType", "tx");
	add_list(thgc, func->comps, (char*)make_lload(thgc, txcf, make_lvari_c(thgc, "%TxFinishType*", "@TxCommit"), false));
	LVari* hvp = make_vn(thgc, "%CoroFrameType**", "hv");
	add_list(thgc, func->comps, (char*)make_lalloca(thgc, hvp));
	LCall* txcall = make_lcall(thgc, NULL, txcf);
	lcall_add(thgc, txcall, hvp);
	lcall_add(thgc, txcall, tx);
	add_list(thgc, func->comps, (char*)txcall);
	LVari* hv = make_vn(thgc, "%CoroFrameType*", "hv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, hv, hvp, false));
	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* qv = make_vn(thgc, "%CoroutineQueueType**", "qv");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qv, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, qv2, qv, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, queueptr2, qv2));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qv2);
	lcall_add(thgc, pqcall, hv);
	add_list(thgc, func->comps, (char*)pqcall);
	LVari* statevalptr2 = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, statevalptr2, make_lvari_c(thgc, "i32", "-1")));

	LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(func->draws, 0)));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
}

// ============================================================
// Helper: emit coro_end label + state=-1 + ret
// ============================================================
static inline void emit_coro_end(ThreadGC* thgc, LFunc* func, LVari* obj, List* list) {
	LLab* lab10 = make_llab(thgc, "coro_end");
	add_list(thgc, list, (char*)make_lifvalue(thgc, lab10, make_lvari_c(thgc, "i32", "-1")));
	add_list(thgc, func->comps, (char*)lab10);
	LVari* stateval = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateval, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, stateval, make_lvari_c(thgc, "i32", "-1")));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
}

// ============================================================
// Helper: emit MakeFrame + WaitHandle (sync path)
// ============================================================
static inline LVari* emit_makeframe_waithandle(ThreadGC* thgc, Master* local, LVari* rn4, LFunc* func, LVari* ov) {
	LVari* hv = make_vn(thgc, "%CoroFrameType*", "hv");
	LVari* root = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* mkf = make_vn(thgc, "%MakeFrameType", "mk");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, mkf, make_lvari_c(thgc, "%MakeFrameType*", "@MakeFrame"), false));
	LCall* mkcall = make_lcall(thgc, make_lvari_s(thgc, "ptr", hv->name), mkf);
	lcall_add(thgc, mkcall, root);
	lcall_add(thgc, mkcall, make_lvari_s(thgc, "ptr", func->y->name));
	lcall_add(thgc, mkcall, ov);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mkcall);

	// Check tx == null, set state = -2 if so
	LVari* txp = make_vn(thgc, "ptr", "tx");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txp, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	LVari* tx = make_vn(thgc, "ptr", "tx");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, tx, txp, false));
	LVari* eq = make_vn(thgc, "i1", "eq");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lcmp(thgc, LCMP_Eq, eq, tx, make_lvari_c(thgc, "ptr", "null")));
	char l1name[32]; snprintf(l1name, sizeof(l1name), "eq%d", llvm_n++);
	char l2name[32]; snprintf(l2name, sizeof(l2name), "th%d", llvm_n++);
	LLab* l1 = make_llab(thgc, l1name);
	LLab* l2 = make_llab(thgc, l2name);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lbr(thgc, eq, l1, l2));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)l1);
	LVari* stateptr = make_vn(thgc, "i32*", "st");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateptr, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, stateptr, make_lvari_c(thgc, "i32", "-2")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lbr(thgc, NULL, NULL, l2));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)l2);

	LVari* wh = make_vn(thgc, "%WaitHandleType", "wh");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, wh, make_lvari_c(thgc, "%WaitHandleType*", "@WaitHandle"), false));
	LVari* thgcv = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* ret = make_vn(thgc, "%RowSet*", "re");
	LCall* whcall = make_lcall(thgc, ret, wh);
	lcall_add(thgc, whcall, thgcv);
	lcall_add(thgc, whcall, hv);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)whcall);
	return ret;
}

// ============================================================
// Helper: emit Triptr allocation + store sql string + store ps
// ============================================================
static inline LVari* emit_triptr_alloc(ThreadGC* thgc, Master* local, LVari* v, LVari* ps) {
	LVari* ov = make_vn(thgc, "ptr", "ov");
	LCall* mcall = make_lcall(thgc, ov, make_lvari_c(thgc, "ptr", "@malloc"));
	lcall_add(thgc, mcall, make_lvari_c(thgc, "i64", "24"));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall);
	LVari* sp = make_vn(thgc, "%StringType**", "sp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%Triptr", sp, ov, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, sp, v));
	LVari* pps = make_vn(thgc, "%SqlParamType**", "pp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%Triptr", pps, ov, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, pps, ps));
	return ov;
}

// ============================================================
// Helper: store go_v into Triptr slot 2
// ============================================================
static inline void emit_triptr_store_go(ThreadGC* thgc, Master* local, LVari* ov, LVari* go_v) {
	LVari* pps2 = make_vn(thgc, "%SqlParamType**", "pp");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%Triptr", pps2, ov, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, pps2, go_v));
}

// Forward declarations for helpers used before their definitions below
static inline String* build_model_vstr(ThreadGC* thgc, ModelObj* modelobj, Master* local);
static inline LVari* emit_add_string2(ThreadGC* thgc, Master* local, LVari* str1, LVari* str2);
static inline LVari* emit_malloc_params(ThreadGC* thgc, Master* local, int count);
String* ModelObj_TableName(ThreadGC* thgc, ModelObj* self);

// ============================================================
// Helper: build vstr from ModelObj block 1/2 (id, model fields)
// Used by FirstAwait / SelectAwait to build SELECT column list
// ============================================================
static inline String* firstawait_vstr(ThreadGC* thgc, ModelObj* modelobj) {
	return build_model_vstr(thgc, modelobj, NULL);
}

// ============================================================
// Helper: build SELECT SQL for FirstAwait / SelectAwait
// Returns v (LVari*, %StringType*) and sets out_ps, out_pcount.
// suffix is " limit 1;" for FirstAwait, ";" for SelectAwait
// Returns NULL LVari on error (caller must check).
// If error, out_err_msg is set.
// ============================================================
static inline LVari* emit_select_sql(ThreadGC* thgc, Master* local, ModelObj* modelobj, Obj* val,
	const char* suffix, LVari** out_ps, int* out_pcount, const char** out_err_msg) {
	*out_err_msg = NULL;
	*out_ps = make_lvari_c(thgc, "ptr", "null");
	*out_pcount = 0;
	String* vstr = build_model_vstr(thgc, modelobj, local);
	LVari* v = NULL;
	if (val == NULL) {
		// "select <vstr> from <tn><suffix>"
		String* tn = ModelObj_TableName(thgc, modelobj);
		SB* sb2 = sb_create(thgc);
		sb_adds(thgc, sb2, "select ");
		sb_add_str(thgc, sb2, vstr);
		sb_adds(thgc, sb2, " from ");
		sb_add_str(thgc, sb2, tn);
		sb_adds(thgc, sb2, suffix);
		sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
		const char* sql_cstr = str_to_cstr(thgc, &sb2->str);
		int sql_len = (int)strlen(sql_cstr);
		v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
	}
	else if (val->type == LetterType::OT_SqlString) {
		SqlString* sqs = (SqlString*)val;
		SqlString_Format(thgc, (Obj*)sqs);
		String* tn = ModelObj_TableName(thgc, modelobj);
		SB* sb2 = sb_create(thgc);
		sb_adds(thgc, sb2, "select ");
		sb_add_str(thgc, sb2, vstr);
		sb_adds(thgc, sb2, " from ");
		sb_add_str(thgc, sb2, tn);
		sb_adds(thgc, sb2, " where ");
		sb_add_str(thgc, sb2, sqs->value);
		sb_adds(thgc, sb2, suffix);
		sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
		const char* sql_cstr = str_to_cstr(thgc, &sb2->str);
		int sql_len = (int)strlen(sql_cstr);
		int pcount = (((Val*)sqs)->vmap != NULL) ? ((Val*)sqs)->vmap->kvs->size : 0;
		LVari* ps = emit_malloc_params(thgc, local, pcount);
		v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
		if (pcount > 0) {
			LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
			char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
			lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
		}
		for (int ki = 0; ki < ((Val*)sqs)->vmap->kvs->size; ki++) {
			KV* kv = (KV*)*(char**)get_list(((Val*)sqs)->vmap->kvs, ki);
			int key = atoi(str_to_cstr(thgc, kv->key));
			Obj* kval = (Obj*)kv->value;
			if (kval->type == LetterType::OT_Value) {
				Value* value = (Value*)kval;
				Obj* cls = value->cls;
				if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
				if (cls->type == LetterType::OT_ModelObj) {
					if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
					else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
				}
			}
		}
		*out_ps = ps;
		*out_pcount = pcount;
	}
	else if (val->type == LetterType::OT_Value) {
		Value* value = (Value*)val;
		*out_ps = make_lvari_c(thgc, "ptr", "null");
		if (value->cls == (Obj*)local->StrT) {
			String* tn = ModelObj_TableName(thgc, modelobj);
			SB* sb2 = sb_create(thgc);
			sb_adds(thgc, sb2, "select ");
			sb_add_str(thgc, sb2, vstr);
			sb_adds(thgc, sb2, " from ");
			sb_add_str(thgc, sb2, tn);
			sb_adds(thgc, sb2, " where ");
			sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
			const char* sql_cstr = str_to_cstr(thgc, &sb2->str);
			int sql_len = (int)strlen(sql_cstr);
			v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
			v = emit_add_string2(thgc, local, v, value->vari);
			// + suffix (" limit 1;" or ";")
			int lim_len = (int)strlen(suffix);
			LVari* v2 = emit_create_string_sql(thgc, local, suffix, lim_len);
			v = emit_add_string2(thgc, local, v, v2);
		}
		else {
			*out_err_msg = "abc";
			return NULL;
		}
	}
	else {
		// leave out_err_msg NULL so caller uses context-specific message
		return NULL;
	}
	return v;
}

// ============================================================
// Helper: emit coroutine queue push in local->llvm->func
// ============================================================
static inline void emit_local_coro_push(ThreadGC* thgc, Master* local, LVari* hv, LVari* rn4) {
	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* thgcv = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* qv = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%ThreadGCType", qv, thgcv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, qv2, qv, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, queueptr2, qv2));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qv2);
	lcall_add(thgc, pqcall, hv);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)pqcall);
}

// ============================================================
// Helper: emit AddString2 (string concat)
// ============================================================
static inline LVari* emit_add_string2(ThreadGC* thgc, Master* local, LVari* str1, LVari* str2) {
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* asv = make_vn(thgc, "%AddStringType2", "as");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, asv, make_lvari_c(thgc, "%AddStringType2*", "@AddString2"), false));
	LVari* strv = make_vn(thgc, "%StringType*", "st");
	LCall* ascall = make_lcall(thgc, strv, asv);
	lcall_add(thgc, ascall, thgc4);
	lcall_add(thgc, ascall, str1);
	lcall_add(thgc, ascall, str2);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)ascall);
	return strv;
}

// ============================================================
// Helper: emit malloc for sql params
// ============================================================
static inline LVari* emit_malloc_params(ThreadGC* thgc, Master* local, int count) {
	LVari* ps = make_lvari_c(thgc, "ptr", "%ps");
	char sizestr[32]; snprintf(sizestr, sizeof(sizestr), "%d", 24 * count);
	LCall* mcall = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
	lcall_add(thgc, mcall, make_lvari_c(thgc, "i32", sizestr));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall);
	return ps;
}

// ============================================================
// Helper: build vstr from model blocks (for SELECT column list)
// ============================================================
static inline String* build_model_vstr(ThreadGC* thgc, ModelObj* modelobj, Master* local) {
	// Start with "id"
	SB* sb = sb_create(thgc);
	sb_adds(thgc, sb, "id");
	Block* blk1 = (Block*)*(Obj**)get_list(((Obj*)modelobj->draw)->children, 0);
	// iterate blk1->vmapA
	if (blk1->vmapA != NULL) {
		for (int i = 0; i < blk1->vmapA->kvs->size; i++) {
			KV* kv_entry = *(KV**)get_list(blk1->vmapA->kvs, i);
			String* key = kv_entry->key;
			Obj* val = (Obj*)kv_entry->value;
			if (val->type == LetterType::OT_Variable) {
				Variable* variable = (Variable*)val;
				Obj* cls = variable->cls;
				if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
				if (cls->type == LetterType::OT_ModelObj) {
					sb_adds(thgc, sb, ", ");
					sb_add_str(thgc, sb, key);
				}
			}
		}
	}
	Block* blk2 = (Block*)*(Obj**)get_list(((Obj*)modelobj->draw)->children, 1);
	if (blk2->vmapA != NULL) {
		for (int i = 0; i < blk2->vmapA->kvs->size; i++) {
			KV* kv_entry = *(KV**)get_list(blk2->vmapA->kvs, i);
			String* key = kv_entry->key;
			if (str_matchA(key, "id", 2)) continue;
			Obj* val = (Obj*)kv_entry->value;
			if (val->type == LetterType::OT_Variable) {
				Variable* variable = (Variable*)val;
				Obj* cls = variable->cls;
				if (cls->type == LetterType::OT_Var) cls = ((Type*)cls)->cls;
				if (cls->type == LetterType::OT_ModelObj) {
					sb_adds(thgc, sb, ", ");
					sb_add_str(thgc, sb, key);
				}
			}
		}
	}
	sb->str.data[sb->str.size * sb->str.esize] = '\0';
	if (sb->str.esize == 2) sb->str.data[sb->str.size * sb->str.esize + 1] = '\0';
	return &sb->str;
}

// ============================================================
// Helper: process SqlString varis for sql params
// ============================================================
static inline void emit_sqlstring_params(ThreadGC* thgc, Master* local, SqlParamBind* sqs, LVari* ps) {
	if (sqs->varis == NULL) return;
	for (int ki = 0; ki < sqs->varis->keys->size; ki++) {
		int key = *(int*)get_list(sqs->varis->keys, ki);
		Obj* kval = *(Obj**)get_list(sqs->varis->vals, ki);
		if (kval->type == LetterType::OT_Value) {
			Value* value = (Value*)kval;
			Obj* cls = value->cls;
			if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
			if (cls->type == LetterType::OT_ModelObj) {
				if (cls == (Obj*)local->Int) {
					emit_sql_param_int(thgc, local, ps, sqs->varis->keys->size, key, value->vari);
				} else if (cls == (Obj*)local->StrT) {
					emit_sql_param_str(thgc, local, ps, sqs->varis->keys->size, key, value->vari);
				}
			}
		}
	}
}

// ============================================================
// Helper: emit Store type value handling in coroutine
// ============================================================
static inline LVari* emit_store_value_type(ThreadGC* thgc, Master* local, LFunc* func,
	Value* value, LVari* objp, LVari* objv,
	const char* typename_str, LTypeDec* typedec, int i, int n3, int* count_out)
{
	LVari* vx = NULL;
	if (value->cls == (Obj*)local->Int) {
		add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "i32", i));
		char n3str[16]; snprintf(n3str, sizeof(n3str), "%d", n3);
		LVari* gv = make_vn(thgc, "i32*", "gv");
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, typename_str, gv, objp, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, gv, value->vari));
		LVari* vxp = make_vn(thgc, "i32*", "vx");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, vxp, objv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		vx = make_vn(thgc, "i32", "vx");
		add_list(thgc, func->comps, (char*)make_lload(thgc, vx, vxp, false));
		*count_out += 8;
	} else if (value->cls == (Obj*)local->Short) {
		add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "i16", i));
		char n3str[16]; snprintf(n3str, sizeof(n3str), "%d", n3);
		LVari* gv = make_vn(thgc, "i16*", "gv");
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, typename_str, gv, objp, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, gv, value->vari));
		LVari* vxp = make_vn(thgc, "i16*", "vx");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, vxp, objv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		vx = make_vn(thgc, "i16", "vx");
		add_list(thgc, func->comps, (char*)make_lload(thgc, vx, vxp, false));
		*count_out += 8;
	} else if (value->cls == (Obj*)local->Bool) {
		add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "i1", i));
		char n3str[16]; snprintf(n3str, sizeof(n3str), "%d", n3);
		LVari* gv = make_vn(thgc, "i1*", "gv");
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, typename_str, gv, objp, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, gv, value->vari));
		LVari* vxp = make_vn(thgc, "i1*", "vx");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, vxp, objv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		vx = make_vn(thgc, "i1", "vx");
		add_list(thgc, func->comps, (char*)make_lload(thgc, vx, vxp, false));
		*count_out += 8;
	} else if (value->cls == (Obj*)local->StrT) {
		add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "%StringType*", i));
		char n3str[16]; snprintf(n3str, sizeof(n3str), "%d", n3);
		LVari* gv = make_vn(thgc, "%StringType*", "gv");
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, typename_str, gv, objp, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, gv, value->vari));
		LVari* vxp = make_vn(thgc, "%StringType**", "vx");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, vxp, objv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		LVari* vx0 = make_vn(thgc, "%StringType*", "vx");
		add_list(thgc, func->comps, (char*)make_lload(thgc, vx0, vxp, false));
		LVari* utf8 = make_vn(thgc, "%StringUTF8Type*", "ut");
		add_list(thgc, func->comps, (char*)make_lload(thgc, utf8, make_lvari_c(thgc, "%StringUTF8Type*", "@StringUTF8"), false));
		LVari* len = make_vn(thgc, "i32*", "le");
		add_list(thgc, func->comps, (char*)make_lalloca(thgc, len));
		vx = make_vn(thgc, "i8*", "vx");
		LCall* ucall = make_lcall(thgc, vx, utf8);
		lcall_add(thgc, ucall, vx0);
		lcall_add(thgc, ucall, len);
		add_list(thgc, func->comps, (char*)ucall);
		*count_out += 8;
	} else {
		add_list(thgc, typedec->comps, (char*)make_ltypeval(thgc, "%GCModel*", i));
		char n3str[16]; snprintf(n3str, sizeof(n3str), "%d", n3);
		LVari* gv = make_vn(thgc, "%GCModel**", "gv");
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, typename_str, gv, objp, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lstore(thgc, gv, value->vari));
		LVari* vxp = make_vn(thgc, "%GCModel**", "vx");
		add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, typename_str, vxp, objv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", n3str)));
		vx = make_vn(thgc, "%GCModel*", "vx");
		add_list(thgc, func->comps, (char*)make_lload(thgc, vx, vxp, false));
		*count_out += 8;
	}
	return vx;
}

// ============================================================
// Helper: emit DbStore call in coro func
// ============================================================
static inline void emit_dbstore_push(ThreadGC* thgc, LFunc* func, ModelObj* model, LVari* tx, LVari* vx) {
	LVari* hv = make_vn(thgc, "%CoroFrameType*", "hv");
	char dbstore[64]; snprintf(dbstore, sizeof(dbstore), "@DbStore%d", model->identity);
	LCall* dscall = make_lcall(thgc, make_lvari_s(thgc, "ptr", hv->name), make_lvari_c(thgc, "ptr", dbstore));
	lcall_add(thgc, dscall, tx);
	lcall_add(thgc, dscall, vx);
	add_list(thgc, func->comps, (char*)dscall);
	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* qv = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qv, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, qv2, qv, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, queueptr2, qv2));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qv2);
	lcall_add(thgc, pqcall, hv);
	add_list(thgc, func->comps, (char*)pqcall);

	LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(func->draws, 0)));
}

// ============================================================
// Helper: emit DbDelete call in coro func
// ============================================================
static inline void emit_dbdelete_push(ThreadGC* thgc, LFunc* func, ModelObj* model, LVari* tx, LVari* vx) {
	LVari* hv = make_vn(thgc, "%CoroFrameType*", "hv");
	char dbdelete[64]; snprintf(dbdelete, sizeof(dbdelete), "@DbDelete%d", model->identity);
	LCall* ddcall = make_lcall(thgc, make_lvari_s(thgc, "ptr", hv->name), make_lvari_c(thgc, "ptr", dbdelete));
	lcall_add(thgc, ddcall, tx);
	lcall_add(thgc, ddcall, vx);
	add_list(thgc, func->comps, (char*)ddcall);
	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* qv = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qv, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, qv2, qv, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, queueptr2, qv2));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qv2);
	lcall_add(thgc, pqcall, hv);
	add_list(thgc, func->comps, (char*)pqcall);

	LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(func->draws, 0)));
}

// ============================================================
// Helper: emit coro_fin block (async Store/Delete completion)
// ============================================================
static inline void emit_coro_fin_async(ThreadGC* thgc, LFunc* func, LVari* obj, List* list, int identity) {
	LVari* statevalptr2 = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, statevalptr2, make_lvari_c(thgc, "i32", "-1")));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	LLab* lab_fin = make_llab(thgc, "coro_fin");
	add_list(thgc, list, (char*)make_lifvalue(thgc, lab_fin, make_lvari_c(thgc, "i32", "-1")));
	add_list(thgc, func->comps, (char*)lab_fin);
	LVari* stateval = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateval, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, stateval, make_lvari_c(thgc, "i32", "-1")));
	// Check parent != null
	LVari* parent = make_lvari_c(thgc, "%CoroFrameType**", "%parent");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	LVari* handle = make_vn(thgc, "%CoroFrameType*", "ha");
	add_list(thgc, func->comps, (char*)make_lload(thgc, handle, parent, false));
	LVari* comp = make_lvari_c(thgc, "i1", "%comp");
	add_list(thgc, func->comps, (char*)make_lcmp(thgc, LCMP_Ne, comp, handle, make_lvari_c(thgc, "ptr", "null")));
	char endname[64]; snprintf(endname, sizeof(endname), "coro_end%d", identity);
	LLab* lab_end = make_llab(thgc, endname);
	LLab* lab_ret = make_llab(thgc, "coro_ret");
	add_list(thgc, func->comps, (char*)make_lbr(thgc, comp, lab_end, lab_ret));
	add_list(thgc, func->comps, (char*)lab_end);
	LVari* queueptr = make_lvari_c(thgc, "%CoroutineQueueType**", "%queueptr");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* queue = make_lvari_c(thgc, "%PushQueueType*", "%queue");
	add_list(thgc, func->comps, (char*)make_lload(thgc, queue, queueptr, false));
	LVari* pushqueue = make_lvari_c(thgc, "%PushQueueType*", "%pushqueue");
	add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue);
	lcall_add(thgc, pqcall, queue);
	lcall_add(thgc, pqcall, handle);
	add_list(thgc, func->comps, (char*)pqcall);
	add_list(thgc, func->comps, (char*)make_lbr(thgc, NULL, NULL, lab_ret));
	add_list(thgc, func->comps, (char*)lab_ret);
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
}

// ============================================================
// Helper: emit coro_end + TxCommit block (sync Store/Delete)
// ============================================================
static inline void emit_coro_txcommit_sync(ThreadGC* thgc, LFunc* func, LVari* obj, LVari* rn4, List* list) {
	LVari* statevalptr2 = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	LVari* sfinp = make_vn(thgc, "i32*", "sf");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", sfinp, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "10")));
	LVari* sfinv = make_vn(thgc, "i32", "sf");
	add_list(thgc, func->comps, (char*)make_lload(thgc, sfinv, sfinp, false));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, statevalptr2, sfinv));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	LLab* lab10 = make_llab(thgc, "coro_end");
	add_list(thgc, list, (char*)make_lifvalue(thgc, lab10, make_lvari_c(thgc, "i32", "-3")));
	add_list(thgc, func->comps, (char*)lab10);
	// TxCommit
	LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
	add_list(thgc, func->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
	LVari* qvn = make_vn(thgc, "%CoroutineQueueType**", "qv");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qvn, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	LVari* qvn2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
	add_list(thgc, func->comps, (char*)make_lload(thgc, qvn2, qvn, false));
	LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, queueptr2, qvn2));
	LVari* hvn = make_vn(thgc, "%CoroFrameType*", "hv");
	LVari* bt = make_lvari_c(thgc, "%TxFinishType", "%tf");
	LVari* alv = make_vn(thgc, "%CoroFrameType**", "al");
	add_list(thgc, func->comps, (char*)make_lalloca(thgc, alv));
	add_list(thgc, func->comps, (char*)make_lload(thgc, bt, make_lvari_c(thgc, "%TxFinishType*", "@TxCommit"), false));
	LVari* txp = make_vn(thgc, "i8**", "tx");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txp, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	LVari* txv = make_vn(thgc, "i8*", "tx");
	add_list(thgc, func->comps, (char*)make_lload(thgc, txv, txp, false));
	LCall* btcall = make_lcall(thgc, NULL, bt);
	lcall_add(thgc, btcall, alv);
	lcall_add(thgc, btcall, txv);
	add_list(thgc, func->comps, (char*)btcall);
	add_list(thgc, func->comps, (char*)make_lload(thgc, hvn, alv, false));
	LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hvn, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(func->draws, 0)));
	LVari* qp = make_vn(thgc, "%CoroutineQueueType**", "qp");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", qp, hvn, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, qp, qvn2));
	LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
	lcall_add(thgc, pqcall, qvn2);
	lcall_add(thgc, pqcall, hvn);
	add_list(thgc, func->comps, (char*)pqcall);
	statevalptr2 = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, statevalptr2, make_lvari_c(thgc, "i32", "-1")));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	LLab* lab_fin = make_llab(thgc, "coro_fin");
	add_list(thgc, list, (char*)make_lifvalue(thgc, lab_fin, make_lvari_c(thgc, "i32", "-1")));
	add_list(thgc, func->comps, (char*)lab_fin);
	LVari* stateval = make_vn(thgc, "i32*", "st");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateval, obj, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, stateval, make_lvari_c(thgc, "i32", "-1")));
	add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
}

// ============================================================
// Helper: emit RowSet first-row extraction (FirstAwait result)
// ============================================================
static inline LVari* emit_rowset_first_row(ThreadGC* thgc, Master* local, LVari* ret, const char* model_type) {
	LVari* rowsp = make_vn(thgc, "%HashType**", "ro");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%RowSet", rowsp, ret, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* rowsv = make_vn(thgc, "%HashType*", "ro");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, rowsv, rowsp, false));
	LVari* kvsp = make_vn(thgc, "%ListType**", "kv");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%HashType", kvsp, rowsv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* kvs = make_vn(thgc, "%ListType*", "kv");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, kvs, kvsp, false));
	LVari* sizep = make_vn(thgc, "i32*", "si");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%ListType", sizep, kvs, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* size = make_vn(thgc, "i32", "si");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, size, sizep, false));
	LVari* equal = make_vn(thgc, "i1", "eq");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lcmp(thgc, LCMP_Eq, equal, size, make_lvari_c(thgc, "i32", "0")));
	char l1name[32]; snprintf(l1name, sizeof(l1name), "coro_end%d", llvm_n++);
	char l2name[32]; snprintf(l2name, sizeof(l2name), "coro_ret%d", llvm_n++);
	LLab* lab1 = make_llab(thgc, l1name);
	LLab* lab2 = make_llab(thgc, l2name);
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lbr(thgc, equal, lab1, lab2));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)lab2);

	LVari* glf = make_vn(thgc, "%GetListType", "gl");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, glf, make_lvari_c(thgc, "%GetListType*", "@GetList"), false));
	LVari* op2 = make_vn(thgc, "ptr", "op");
	LCall* glcall = make_lcall(thgc, op2, glf);
	lcall_add(thgc, glcall, kvs);
	lcall_add(thgc, glcall, make_lvari_c(thgc, "i32", "0"));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)glcall);
	LVari* ov2 = make_vn(thgc, "%KeyValueType*", "ov");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, ov2, op2, false));
	char mt2[64]; snprintf(mt2, sizeof(mt2), "%s**", model_type);
	LVari* op3 = make_vn(thgc, mt2, "op");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lgete_idx2(thgc, "%KeyValueType", op3, ov2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	char mt1[64]; snprintf(mt1, sizeof(mt1), "%s*", model_type);
	LVari* ov3 = make_vn(thgc, mt1, "ov");
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, ov3, op3, false));

	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lbr(thgc, NULL, NULL, lab1));
	add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)lab1);
	// Return ov3 from lab2 or null from the other path via phi
	return ov3;
}
// Forward declarations for Model::PrimaryC
String* ModelObj_TableName(ThreadGC* thgc, ModelObj* self);
void ModelObj_Select(ThreadGC* thgc, ModelObj* self, Master* local);
void ModelObj_Store(ThreadGC* thgc, ModelObj* self, Master* local);
void ModelObj_Delete(ThreadGC* thgc, ModelObj* self, Master* local);

// ============================================================
// Model::PrimaryC  (lines 6534-8000+)
// Very large SQL operations method - Where/Sort/Update/Delete
// Due to extreme size, this is a structural stub with key patterns preserved
// ============================================================
Obj* Model_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	if (val2->type == LetterType::OT_Block) {
		Block* blk0 = (Block*)GetterC_vt(thgc, exeC(thgc, val2, local), local);
		Obj* val = *(Obj**)get_list(blk0->rets, 0);
		if (val->type != LetterType::OT_ModelObj)
			return ErrorC(thgc, LetterType::OT_Error, val->letter, "ModelクラスかGeneクラスを指定してください");
		ModelObj* modelobj = (ModelObj*)val;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::_Dot) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_Word) {
				String* word2name = val2->letter->text;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				Obj* where = NULL;

				// Where clause handling
				if (str_equals(word2name, "Where")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++;
						val = GetterC_vt(thgc, exeC(thgc, val2, local), local);
						if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
						Block* blk = (Block*)val;
						Obj* ret0 = *(Obj**)get_list(blk->rets, 0);
						if (ret0->type == LetterType::OT_SqlString) {
							where = ret0;
							// goto head - re-read next word
							val2 = *(Obj**)get_list(primary->children, *n);
							if (val2->type == LetterType::_Dot) {
								(*n)++;
								val2 = *(Obj**)get_list(primary->children, *n);
								word2name = val2->letter->text;
								(*n)++;
								val2 = *(Obj**)get_list(primary->children, *n);
							}
						} else if (ret0->type == LetterType::OT_Value) {
							Value* wvalue = (Value*)ret0;
							Obj* wcls = wvalue->cls;
							if (wcls == local->StrT) {
								where = ret0;
								val2 = *(Obj**)get_list(primary->children, *n);
								if (val2->type == LetterType::_Dot) {
									(*n)++;
									val2 = *(Obj**)get_list(primary->children, *n);
									word2name = val2->letter->text;
									(*n)++;
									val2 = *(Obj**)get_list(primary->children, *n);
								}
							} else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Whereの引数はString型かSqlString型です");
						} else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Whereの引数はString型かSqlString型です");
					} else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Whereの()が指定されていません");
				}

				// Sort clause
				else if (str_equals(word2name, "Sort")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++;
						val = GetterC_vt(thgc, exeC(thgc, val2, local), local);
						if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
						Block* blk = (Block*)val;
						if (blk->rets->size != 1) return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Sortの引数は1です");
						else if ((*(Obj**)get_list(blk->rets, 0))->type != LetterType::OT_Function) return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Sortの引数は関数です");
						FuncTypeC* functype = make_functype(thgc, local->Bool);
						functype->nochange = true;
						add_list(thgc, functype->draws, (char*)modelobj);
						add_list(thgc, functype->draws, (char*)modelobj);
						Obj* ret = CheckCVB_TypeCheck(thgc, (Obj*)functype, *(Obj**)get_list(blk->rets, 0), CheckType_Setter, local);
						if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
						Obj* value = New_Value(thgc, (Obj*)modelobj, local, val2->letter);
						if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
						Block* result = make_block(thgc, LetterType::OT_Array);
						add_list(thgc, result->rets, (char*)value);
						((Obj*)result)->letter = val2->letter;
						return (Obj*)Block_Sort(thgc, result, (Function*)*(Obj**)get_list(blk->rets, 0), local);
					} else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Sortの()が指定されていません");
				}

				// Update clause
				else if (str_equals(word2name, "Update")) {
					if (val2->type == LetterType::_Dot) {
						(*n)++;
						val2 = *(Obj**)get_list(primary->children, *n);
						if (val2->type == LetterType::OT_Word) {
							String* word3name = val2->letter->text;
							(*n)++;
							val2 = *(Obj**)get_list(primary->children, *n);
							if (str_equals(word3name, "await")) {
								(*n)++;
								val2 = *(Obj**)get_list(primary->children, *n);
								if (val2->type == LetterType::OT_Bracket) {
									(*n)++; val = where;
									const char* sql_cstr = "";
									LVari* ps = make_lvari_c(thgc, "ptr", "null");
									int pcount = 0;
									LVari* v = NULL;
									Block* blk = (Block*)exeC(thgc, val2, local);
									LVari* v0 = NULL;
									SqlString* sqs0 = NULL;
									if (blk->rets->size >= 1) {
										Obj* r0 = *(Obj**)get_list(blk->rets, 0);
										if (r0->type == LetterType::OT_SqlString) {
											sqs0 = (SqlString*)r0;
											String* tn = ModelObj_TableName(thgc, modelobj);
											String* prefix = StringAdd(thgc, createString(thgc, (char*)"update ", 7, 1), tn->data, tn->size, tn->esize);
											prefix = StringAdd(thgc, prefix, (char*)" set ", 5, 1);
											sqs0->value = StringAdd(thgc, prefix, sqs0->value->data, sqs0->value->size, sqs0->value->esize);
											for (int i = 1; i < blk->rets->size; i++) {
												SqlString* sqsi = (SqlString*)*(Obj**)get_list(blk->rets, i);
												sqs0->value = StringAdd(thgc, sqs0->value, sqsi->value->data, sqsi->value->size, sqsi->value->esize);
												if (((Val*)sqsi)->vmap != NULL) {
													if (((Val*)sqs0)->vmap == NULL) ((Val*)sqs0)->vmap = ((Val*)sqsi)->vmap;
													else {
														for (int vi = 0; vi < ((Val*)sqsi)->vmap->kvs->size; vi++) {
															KV* kv = (KV*)*(char**)get_list(((Val*)sqsi)->vmap->kvs, vi);
															if (get_mapy(((Val*)sqs0)->vmap, kv->key) == NULL)
																add_mapy(thgc, ((Val*)sqs0)->vmap, kv->key, kv->value);
														}
													}
												}
											}
										}
										else if (r0->type == LetterType::OT_Value) {
											Value* value = (Value*)val;
											ps = make_lvari_c(thgc, "ptr", "null");
											if (value->cls == (Obj*)local->StrT) {
												String* tn = ModelObj_TableName(thgc, modelobj);
												SB* sb2 = sb_create(thgc);
												sb_adds(thgc, sb2, "update ");
												sb_add_str(thgc, sb2, tn);
												sb_adds(thgc, sb2, " set ");
												sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
												sql_cstr = str_to_cstr(thgc, &sb2->str);
												int sql_len = (int)strlen(sql_cstr);
												v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
												v0 = emit_add_string2(thgc, local, v, value->vari);
											}
											else return ErrorC(thgc, LetterType::OT_Error, ((Obj*)value)->letter, "abc");
										}
										else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Updateの引数はSqlString型かString型です");
									}
									else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Updateの引数はSqlString型かString型です");
									Block* blk1 = (Block*)*(Obj**)get_list(((Obj*)modelobj->draw)->children, 0);
									if (where == NULL) {
										if (sqs0 == NULL) {
											v = v0;
										} else {
											sqs0->value = StringAdd(thgc, sqs0->value, (char*)";", 1, 1);
											SqlString_Format(thgc, (Obj*)sqs0);
											sql_cstr = str_to_cstr(thgc, sqs0->value);
											int sql_len = (int)strlen(sql_cstr);
											pcount = (((Val*)sqs0)->vmap != NULL) ? ((Val*)sqs0)->vmap->kvs->size : 0;
											ps = emit_malloc_params(thgc, local, pcount);
											v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
											if (pcount > 0) {
												LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
												char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
												lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
												add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
											}
											if (((Val*)sqs0)->vmap != NULL) {
												for (int ki = 0; ki < ((Val*)sqs0)->vmap->kvs->size; ki++) {
													KV* kv = (KV*)*(char**)get_list(((Val*)sqs0)->vmap->kvs, ki);
													int key = atoi(str_to_cstr(thgc, kv->key));
													Obj* kval = (Obj*)kv->value;
													if (kval->type == LetterType::OT_Value) {
														Value* value = (Value*)kval;
														Obj* cls = value->cls;
														if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
														if (cls->type == LetterType::OT_ModelObj) {
															if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
															else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
														}
													}
												}
											}
										}
									}
									else if (val->type == LetterType::OT_SqlString) {
										if (sqs0 == NULL) {
											SqlString* sqs = (SqlString*)val;
											SqlString_Format(thgc, (Obj*)sqs);
											SB* sb2 = sb_create(thgc);
											sb_adds(thgc, sb2, " where ");
											sb_add_str(thgc, sb2, sqs->value);
											sb_adds(thgc, sb2, ";");
											sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
											sql_cstr = str_to_cstr(thgc, &sb2->str);
											int sql_len = (int)strlen(sql_cstr);
											pcount = (((Val*)sqs)->vmap != NULL) ? ((Val*)sqs)->vmap->kvs->size : 0;
											ps = emit_malloc_params(thgc, local, pcount);
											v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
											if (pcount > 0) {
												LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
												char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
												lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
												add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
											}
											if (((Val*)sqs)->vmap != NULL) {
												for (int ki = 0; ki < ((Val*)sqs)->vmap->kvs->size; ki++) {
													KV* kv = (KV*)*(char**)get_list(((Val*)sqs)->vmap->kvs, ki);
													int key = atoi(str_to_cstr(thgc, kv->key));
													Obj* kval = (Obj*)kv->value;
													if (kval->type == LetterType::OT_Value) {
														Value* value = (Value*)kval;
														Obj* cls = value->cls;
														if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
														if (cls->type == LetterType::OT_ModelObj) {
															if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
															else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
														}
													}
												}
											}
											v = emit_add_string2(thgc, local, v0, v);
										} else {
											SqlString* sqs = (SqlString*)val;
											String* ws = StringAdd(thgc, createString(thgc, (char*)" where ", 7, 1), sqs->value->data, sqs->value->size, sqs->value->esize);
											ws = StringAdd(thgc, ws, (char*)";", 1, 1);
											sqs0->value = StringAdd(thgc, sqs0->value, ws->data, ws->size, ws->esize);
											if (((Val*)sqs)->vmap != NULL) {
												if (((Val*)sqs0)->vmap == NULL) ((Val*)sqs0)->vmap = ((Val*)sqs)->vmap;
												else {
													for (int vi = 0; vi < ((Val*)sqs)->vmap->kvs->size; vi++) {
														KV* kv2 = (KV*)*(char**)get_list(((Val*)sqs)->vmap->kvs, vi);
														if (get_mapy(((Val*)sqs0)->vmap, kv2->key) == NULL)
															add_mapy(thgc, ((Val*)sqs0)->vmap, kv2->key, kv2->value);
													}
												}
											}
											SqlString_Format(thgc, (Obj*)sqs0);
											sql_cstr = str_to_cstr(thgc, sqs0->value);
											int sql_len = (int)strlen(sql_cstr);
											pcount = (((Val*)sqs0)->vmap != NULL) ? ((Val*)sqs0)->vmap->kvs->size : 0;
											v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
											if (((Val*)sqs0)->vmap != NULL) {
												for (int ki = 0; ki < ((Val*)sqs0)->vmap->kvs->size; ki++) {
													KV* kv = (KV*)*(char**)get_list(((Val*)sqs0)->vmap->kvs, ki);
													int key = atoi(str_to_cstr(thgc, kv->key));
													Obj* kval = (Obj*)kv->value;
													if (kval->type == LetterType::OT_Value) {
														Value* value = (Value*)kval;
														Obj* cls = value->cls;
														if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
														if (cls->type == LetterType::OT_ModelObj) {
															if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
															else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
														}
													}
												}
											}
										}
									}
									else if (val->type == LetterType::OT_Value) {
										Value* value2 = (Value*)val;
										ps = make_lvari_c(thgc, "ptr", "null");
										if (value2->cls == (Obj*)local->StrT) {
											if (sqs0 == NULL) {
												LVari* wv = emit_create_string_sql(thgc, local, " where ", 7);
												LVari* wv2 = emit_add_string2(thgc, local, wv, value2->vari);
												v = emit_add_string2(thgc, local, v0, wv2);
											} else {
												LVari* wv = emit_create_string_sql(thgc, local, "where ", 6);
												LVari* wv2 = emit_add_string2(thgc, local, wv, value2->vari);
												SqlString_Format(thgc, (Obj*)sqs0);
												sql_cstr = str_to_cstr(thgc, sqs0->value);
												int sql_len = (int)strlen(sql_cstr);
												pcount = (((Val*)sqs0)->vmap != NULL) ? ((Val*)sqs0)->vmap->kvs->size : 0;
												v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
												if (pcount > 0) {
													ps = emit_malloc_params(thgc, local, pcount);
													LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
													char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
													lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
													add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
												}
												if (((Val*)sqs0)->vmap != NULL) {
													for (int ki = 0; ki < ((Val*)sqs0)->vmap->kvs->size; ki++) {
														KV* kv = (KV*)*(char**)get_list(((Val*)sqs0)->vmap->kvs, ki);
														int key = atoi(str_to_cstr(thgc, kv->key));
														Obj* kval = (Obj*)kv->value;
														if (kval->type == LetterType::OT_Value) {
															Value* value = (Value*)kval;
															Obj* cls = value->cls;
															if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
															if (cls->type == LetterType::OT_ModelObj) {
																if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
																else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
															}
														}
													}
												}
												LVari* sv2 = emit_add_string2(thgc, local, v, wv2);
												v = sv2;
											}
										}
										else return ErrorC(thgc, LetterType::OT_Error, ((Obj*)value2)->letter, "abc");
									}
									else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Updateの引数はSqlString型かString型です");
									if (llvm_get_func(local->llvm)->async_flag) {
										LFunc* fn = llvm_get_func(local->llvm);
										// tx setup
										LVari* tx = make_vn(thgc, "i8*", "tx");
										LVari* txp = make_vn(thgc, "i8**", "tx");
										add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txp, make_lvari_c(thgc, "%RootNodeType*", "%rn"), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
										add_list(thgc, fn->comps, (char*)make_lload(thgc, tx, txp, false));
										// flush comps
										for (int ci = 0; ci < local->compsC->size; ci++) {
											LComp* comp = *(LComp**)get_list(local->compsC, ci);
											add_list(thgc, fn->comps, (char*)comp);
										}
										local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
										// vari type + fv load + drawcall
										fn->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
										LVari* fv = make_vn(thgc, "%FuncType*", "fv");
										add_list(thgc, fn->comps, (char*)make_lload(thgc, fv, fn->vari, false));
										LVari* go_v = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "go");
										LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
										LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), modelobj->drawcall));
										lcall_add(thgc, go_call, rn4);
										lcall_add(thgc, go_call, fv);
										add_list(thgc, fn->comps, (char*)go_call);
										ModelObj_Select(thgc, modelobj, local);
										// ExecSql
										LVari* esql = make_lvari_c(thgc, "%ExecSqlType", "%esql");
										add_list(thgc, fn->comps, (char*)make_lload(thgc, esql, make_lvari_c(thgc, "%ExecSqlType*", "@ExecSql"), false));
										LVari* coroptr = make_lvari_c(thgc, "%CoroFrameType**", "%coroptr");
										add_list(thgc, fn->comps, (char*)make_lalloca(thgc, coroptr));
										char pcstr[16]; snprintf(pcstr, sizeof(pcstr), "%d", pcount);
										char dbsel[64]; snprintf(dbsel, sizeof(dbsel), "@DbSelect%d", modelobj->identity);
										LCall* ecall = make_lcall(thgc, NULL, esql);
										lcall_add(thgc, ecall, coroptr);
										lcall_add(thgc, ecall, tx);
										lcall_add(thgc, ecall, v);
										lcall_add(thgc, ecall, ps);
										lcall_add(thgc, ecall, make_lvari_c(thgc, "i32", pcstr));
										lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", dbsel));
										lcall_add(thgc, ecall, go_v);
										add_list(thgc, fn->comps, (char*)ecall);
										LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
										add_list(thgc, fn->comps, (char*)make_lload(thgc, hv, coroptr, false));
										// push queue
										LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
										add_list(thgc, fn->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
										LVari* thgcv = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
										LVari* qv = make_vn(thgc, "%CoroutineQueueType*", "qv");
										add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%ThreadGCType", qv, thgcv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
										LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
										add_list(thgc, fn->comps, (char*)make_lload(thgc, qv2, qv, false));
										LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
										add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
										add_list(thgc, fn->comps, (char*)make_lstore(thgc, queueptr2, qv2));
										LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
										lcall_add(thgc, pqcall, qv2);
										lcall_add(thgc, pqcall, hv);
										add_list(thgc, fn->comps, (char*)pqcall);
										// state
										LVari* stateptr = make_vn(thgc, "i32*", "st");
										add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateptr, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
										char nvstr[16]; snprintf(nvstr, sizeof(nvstr), "%d", llvm_n++);
										LVari* nvari = make_lvari_c(thgc, "i32", nvstr);
										add_list(thgc, fn->comps, (char*)make_lstore(thgc, stateptr, nvari));
										// parent
										LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
										add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
										add_list(thgc, fn->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(fn->draws, 0)));
										add_list(thgc, fn->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
										// await resume label
										char labname[32]; snprintf(labname, sizeof(labname), "aw%d", llvm_n++);
										LLab* lab6 = make_llab(thgc, labname);
										add_list(thgc, fn->comps, (char*)lab6);
										LVari* gev0 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"**", 2, 1), "gv");
										add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", gev0, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
										LVari* gev02 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "gv");
										add_list(thgc, fn->comps, (char*)make_lload(thgc, gev02, gev0, false));
										LVari* objptr0 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"**", 2, 1), "ob");
										add_list(thgc, fn->comps, (char*)make_lalloca(thgc, objptr0));
										add_list(thgc, fn->comps, (char*)make_lstore(thgc, objptr0, gev02));
										add_list(thgc, fn->sc->values, (char*)make_lifvalue(thgc, lab6, nvari));
										LVari* retp = make_vn(thgc, "i8**", "re");
										add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", retp, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "8")));
										LVari* retv2 = make_vn(thgc, "i8*", "rv");
										add_list(thgc, fn->comps, (char*)make_lload(thgc, retv2, retp, false));
										return make_voival(thgc);
									} else {
										// Sync path: create coroutine function
										LVari* statev = NULL;
										LFunc* func = emit_coro_func_setup(thgc, local, "First", &statev);
										LVari* obj = (LVari*)get_list_val(func->draws, 0);
										LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
										List* list = create_list(thgc, sizeof(void*), CType::_LComp);
										LLab* sclab = make_llab(thgc, "coro_tx");
										func->sc = make_lswitchcomp(thgc, statev, sclab, list);
										add_list(thgc, func->comps, (char*)func->sc);
										emit_begin_transaction_block(thgc, func, list, sclab);
										LLab* lab10 = emit_coro_start_block(thgc, func, list);
										LVari* coroptr = emit_coro_execsql(thgc, func, obj, rn4, v, ps, pcount, NULL, NULL);
										LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
										add_list(thgc, func->comps, (char*)make_lload(thgc, hv, coroptr, false));
										emit_coro_push_and_state(thgc, func, hv, list);
										emit_coro_txcommit(thgc, func, obj, rn4, list);
										emit_coro_end(thgc, func, obj, list);
										LVari* ov = emit_triptr_alloc(thgc, local, v, ps);
										LVari* retv = emit_makeframe_waithandle(thgc, local, rn4, func, ov);
										return make_voival(thgc);
									}
								}
								else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Update.awaitの()が指定されていません");
							}
						}
					}
					// non-await Update(bracket) path
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++; val = where;
						const char* sql_cstr = "";
						LVari* ps = make_lvari_c(thgc, "ptr", "null");
						int pcount = 0;
						LVari* v = NULL;
						Block* blk = (Block*)exeC(thgc, val2, local);
						LVari* v0 = NULL;
						SqlString* sqs0 = NULL;
						if (blk->rets->size >= 1) {
							Obj* r0 = *(Obj**)get_list(blk->rets, 0);
							if (r0->type == LetterType::OT_SqlString) {
								sqs0 = (SqlString*)r0;
								String* tn = ModelObj_TableName(thgc, modelobj);
								String* prefix = StringAdd(thgc, createString(thgc, (char*)"update ", 7, 1), tn->data, tn->size, tn->esize);
								prefix = StringAdd(thgc, prefix, (char*)" set ", 5, 1);
								sqs0->value = StringAdd(thgc, prefix, sqs0->value->data, sqs0->value->size, sqs0->value->esize);
								for (int i = 1; i < blk->rets->size; i++) {
									SqlString* sqsi = (SqlString*)*(Obj**)get_list(blk->rets, i);
									sqs0->value = StringAdd(thgc, sqs0->value, sqsi->value->data, sqsi->value->size, sqsi->value->esize);
									for (int vi = 0; vi < ((Val*)sqsi)->vmap->kvs->size; vi++) {
										KV* kv = (KV*)*(char**)get_list(((Val*)sqsi)->vmap->kvs, vi);
										if (get_mapy(((Val*)sqs0)->vmap, kv->key) == NULL)
											add_mapy(thgc, ((Val*)sqs0)->vmap, kv->key, kv->value);
									}
								}
							}
							else if (r0->type == LetterType::OT_Value) {
								Value* value = (Value*)val;
								ps = make_lvari_c(thgc, "ptr", "null");
								if (value->cls == (Obj*)local->StrT) {
									String* tn = ModelObj_TableName(thgc, modelobj);
									SB* sb2 = sb_create(thgc);
									sb_adds(thgc, sb2, "update ");
									sb_add_str(thgc, sb2, tn);
									sb_adds(thgc, sb2, " set ");
									sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
									sql_cstr = str_to_cstr(thgc, &sb2->str);
									int sql_len = (int)strlen(sql_cstr);
									v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
									v0 = emit_add_string2(thgc, local, v, value->vari);
								}
								else return ErrorC(thgc, LetterType::OT_Error, ((Obj*)value)->letter, "abc");
							}
							else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Updateの引数はSqlString型かString型です");
						}
						else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Updateの引数はSqlString型かString型です");
						Block* blk1 = (Block*)*(Obj**)get_list(((Obj*)modelobj->draw)->children, 0);
						if (where == NULL) {
							if (sqs0 == NULL) {
								v = v0;
							} else {
								sqs0->value = StringAdd(thgc, sqs0->value, (char*)";", 1, 1);
								SqlString_Format(thgc, (Obj*)sqs0);
								sql_cstr = str_to_cstr(thgc, sqs0->value);
								int sql_len = (int)strlen(sql_cstr);
								pcount = (((Val*)sqs0)->vmap != NULL) ? ((Val*)sqs0)->vmap->kvs->size : 0;
								ps = emit_malloc_params(thgc, local, pcount);
								v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
								if (pcount > 0) {
									LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
									char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
									lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
									add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
								}
								for (int ki = 0; ki < ((Val*)sqs0)->vmap->kvs->size; ki++) {
									KV* kv = (KV*)*(char**)get_list(((Val*)sqs0)->vmap->kvs, ki);
									int key = atoi(str_to_cstr(thgc, kv->key));
									Obj* kval = (Obj*)kv->value;
									if (kval->type == LetterType::OT_Value) {
										Value* value = (Value*)kval;
										Obj* cls = value->cls;
										if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
										if (cls->type == LetterType::OT_ModelObj) {
											if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
											else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
										}
									}
								}
							}
						}
						else if (val->type == LetterType::OT_SqlString) {
							if (sqs0 == NULL) {
								SqlString* sqs = (SqlString*)val;
								SqlString_Format(thgc, (Obj*)sqs);
								SB* sb2 = sb_create(thgc);
								sb_adds(thgc, sb2, " where ");
								sb_add_str(thgc, sb2, sqs->value);
								sb_adds(thgc, sb2, ";");
								sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
								sql_cstr = str_to_cstr(thgc, &sb2->str);
								int sql_len = (int)strlen(sql_cstr);
								pcount = (((Val*)sqs)->vmap != NULL) ? ((Val*)sqs)->vmap->kvs->size : 0;
								ps = emit_malloc_params(thgc, local, pcount);
								v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
								if (pcount > 0) {
									LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
									char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
									lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
									add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
								}
								for (int ki = 0; ki < ((Val*)sqs)->vmap->kvs->size; ki++) {
									KV* kv = (KV*)*(char**)get_list(((Val*)sqs)->vmap->kvs, ki);
									int key = atoi(str_to_cstr(thgc, kv->key));
									Obj* kval = (Obj*)kv->value;
									if (kval->type == LetterType::OT_Value) {
										Value* value = (Value*)kval;
										Obj* cls = value->cls;
										if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
										if (cls->type == LetterType::OT_ModelObj) {
											if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
											else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
										}
									}
								}
								v = emit_add_string2(thgc, local, v0, v);
							} else {
								SqlString* sqs = (SqlString*)val;
								String* ws = StringAdd(thgc, createString(thgc, (char*)" where ", 7, 1), sqs->value->data, sqs->value->size, sqs->value->esize);
								ws = StringAdd(thgc, ws, (char*)";", 1, 1);
								sqs0->value = StringAdd(thgc, sqs0->value, ws->data, ws->size, ws->esize);
								if (((Val*)sqs)->vmap != NULL) {
									if (((Val*)sqs0)->vmap == NULL) ((Val*)sqs0)->vmap = ((Val*)sqs)->vmap;
									else {
										for (int vi = 0; vi < ((Val*)sqs)->vmap->kvs->size; vi++) {
											KV* kv2 = (KV*)*(char**)get_list(((Val*)sqs)->vmap->kvs, vi);
											if (get_mapy(((Val*)sqs0)->vmap, kv2->key) == NULL)
												add_mapy(thgc, ((Val*)sqs0)->vmap, kv2->key, kv2->value);
										}
									}
								}
								SqlString_Format(thgc, (Obj*)sqs0);
								sql_cstr = str_to_cstr(thgc, sqs0->value);
								int sql_len = (int)strlen(sql_cstr);
								pcount = (((Val*)sqs0)->vmap != NULL) ? ((Val*)sqs0)->vmap->kvs->size : 0;
								v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
								for (int ki = 0; ki < ((Val*)sqs0)->vmap->kvs->size; ki++) {
									KV* kv = (KV*)*(char**)get_list(((Val*)sqs0)->vmap->kvs, ki);
									int key = atoi(str_to_cstr(thgc, kv->key));
									Obj* kval = (Obj*)kv->value;
									if (kval->type == LetterType::OT_Value) {
										Value* value = (Value*)kval;
										Obj* cls = value->cls;
										if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
										if (cls->type == LetterType::OT_ModelObj) {
											if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
											else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
										}
									}
								}
							}
						}
						else if (val->type == LetterType::OT_Value) {
							Value* value2 = (Value*)val;
							ps = make_lvari_c(thgc, "ptr", "null");
							if (value2->cls == (Obj*)local->StrT) {
								if (sqs0 == NULL) {
									LVari* wv = emit_create_string_sql(thgc, local, " where ", 7);
									LVari* wv2 = emit_add_string2(thgc, local, wv, value2->vari);
									v = emit_add_string2(thgc, local, v0, wv2);
								} else {
									LVari* wv = emit_create_string_sql(thgc, local, "where ", 6);
									LVari* wv2 = emit_add_string2(thgc, local, wv, value2->vari);
									SqlString_Format(thgc, (Obj*)sqs0);
									sql_cstr = str_to_cstr(thgc, sqs0->value);
									int sql_len = (int)strlen(sql_cstr);
									pcount = (((Val*)sqs0)->vmap != NULL) ? ((Val*)sqs0)->vmap->kvs->size : 0;
									v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
									if (pcount > 0) {
										ps = emit_malloc_params(thgc, local, pcount);
										LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
										char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
										lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
										add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
									}
									for (int ki = 0; ki < ((Val*)sqs0)->vmap->kvs->size; ki++) {
										KV* kv = (KV*)*(char**)get_list(((Val*)sqs0)->vmap->kvs, ki);
										int key = atoi(str_to_cstr(thgc, kv->key));
										Obj* kval = (Obj*)kv->value;
										if (kval->type == LetterType::OT_Value) {
											Value* value = (Value*)kval;
											Obj* cls = value->cls;
											if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
											if (cls->type == LetterType::OT_ModelObj) {
												if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
												else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
											}
										}
									}
									LVari* sv2 = emit_add_string2(thgc, local, v, wv2);
									v = sv2;
								}
							}
							else return ErrorC(thgc, LetterType::OT_Error, ((Obj*)value2)->letter, "abc");
						}
						else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Updateの引数はSqlString型かString型です");
						{
							// Sync path: create coroutine function
							LVari* statev = NULL;
							LFunc* func = emit_coro_func_setup(thgc, local, "First", &statev);
							LVari* obj = (LVari*)get_list_val(func->draws, 0);
							LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
							List* list = create_list(thgc, sizeof(void*), CType::_LComp);
							LLab* sclab = make_llab(thgc, "coro_tx");
							func->sc = make_lswitchcomp(thgc, statev, sclab, list);
							add_list(thgc, func->comps, (char*)func->sc);
							emit_begin_transaction_block(thgc, func, list, sclab);
							LLab* lab10 = emit_coro_start_block(thgc, func, list);
							LVari* coroptr = emit_coro_execsql(thgc, func, obj, rn4, v, ps, pcount, NULL, NULL);
							LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
							add_list(thgc, func->comps, (char*)make_lload(thgc, hv, coroptr, false));
							emit_coro_push_and_state(thgc, func, hv, list);
							emit_coro_txcommit(thgc, func, obj, rn4, list);
							emit_coro_end(thgc, func, obj, list);
							LVari* ov = emit_triptr_alloc(thgc, local, v, ps);
							LVari* retv = emit_makeframe_waithandle(thgc, local, rn4, func, ov);
							return make_voival(thgc);
						}
					}
					return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Updateの()が指定されていません");
				}

				// Delete clause
				else if (str_equals(word2name, "Delete")) {
					if (val2->type == LetterType::_Dot) {
						(*n)++;
						val2 = *(Obj**)get_list(primary->children, *n);
						if (val2->type == LetterType::OT_Word) {
							String* word3name = val2->letter->text;
							(*n)++;
							val2 = *(Obj**)get_list(primary->children, *n);
							if (str_equals(word3name, "await")) {
								if (val2->type == LetterType::OT_Bracket) {
									(*n)++; val = where;
									const char* sql_cstr = "";
									LVari* ps = make_lvari_c(thgc, "ptr", "null");
									int pcount = 0;
									LVari* v = NULL;
									if (where == NULL) {
										String* tn = ModelObj_TableName(thgc, modelobj);
										SB* sb2 = sb_create(thgc);
										sb_adds(thgc, sb2, "delete from ");
										sb_add_str(thgc, sb2, tn);
										sb_adds(thgc, sb2, ";");
										sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
										sql_cstr = str_to_cstr(thgc, &sb2->str);
										int sql_len = (int)strlen(sql_cstr);
										v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
									}
									else if (val->type == LetterType::OT_SqlString) {
										SqlString* sqs = (SqlString*)val;
										SqlString_Format(thgc, (Obj*)sqs);
										String* tn = ModelObj_TableName(thgc, modelobj);
										SB* sb2 = sb_create(thgc);
										sb_adds(thgc, sb2, "delete from ");
										sb_add_str(thgc, sb2, tn);
										sb_adds(thgc, sb2, " where ");
										sb_add_str(thgc, sb2, sqs->value);
										sb_adds(thgc, sb2, ";");
										sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
										sql_cstr = str_to_cstr(thgc, &sb2->str);
										int sql_len = (int)strlen(sql_cstr);
										pcount = (((Val*)sqs)->vmap != NULL) ? ((Val*)sqs)->vmap->kvs->size : 0;
										ps = emit_malloc_params(thgc, local, pcount);
										v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
										LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
										char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
										lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
										add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
										for (int ki = 0; ki < ((Val*)sqs)->vmap->kvs->size; ki++) {
											KV* kv = (KV*)*(char**)get_list(((Val*)sqs)->vmap->kvs, ki);
											int key = atoi(str_to_cstr(thgc, kv->key));
											Obj* kval = (Obj*)kv->value;
											if (kval->type == LetterType::OT_Value) {
												Value* value = (Value*)kval;
												Obj* cls = value->cls;
												if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
												if (cls->type == LetterType::OT_ModelObj) {
													if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
													else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
												}
											}
										}
									}
									else if (val->type == LetterType::OT_Value) {
										Value* value = (Value*)val;
										ps = make_lvari_c(thgc, "ptr", "null");
										if (value->cls == (Obj*)local->StrT) {
											String* tn = ModelObj_TableName(thgc, modelobj);
											SB* sb2 = sb_create(thgc);
											sb_adds(thgc, sb2, "delete from ");
											sb_add_str(thgc, sb2, tn);
											sb_adds(thgc, sb2, " where ");
											sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
											sql_cstr = str_to_cstr(thgc, &sb2->str);
											int sql_len = (int)strlen(sql_cstr);
											v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
											v = emit_add_string2(thgc, local, v, value->vari);
										}
										else return ErrorC(thgc, LetterType::OT_Error, ((Obj*)value)->letter, "abc");
									}
									else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Deleteの引数はSqlString型かString型です");
									if (llvm_get_func(local->llvm)->async_flag) {
										emit_async_execsql_delete(thgc, local, v, ps, pcount);
										return make_voival(thgc);
									} else {
										// Sync path: create coroutine function
										LVari* statev = NULL;
										LFunc* func = emit_coro_func_setup(thgc, local, "First", &statev);
										LVari* obj = (LVari*)get_list_val(func->draws, 0);
										LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
										List* list = create_list(thgc, sizeof(void*), CType::_LComp);
										LLab* sclab = make_llab(thgc, "coro_tx");
										func->sc = make_lswitchcomp(thgc, statev, sclab, list);
										add_list(thgc, func->comps, (char*)func->sc);
										emit_begin_transaction_block(thgc, func, list, sclab);
										LLab* lab10 = emit_coro_start_block(thgc, func, list);
										LVari* coroptr = emit_coro_execsql(thgc, func, obj, rn4, v, ps, pcount, NULL, NULL);
										LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
										add_list(thgc, func->comps, (char*)make_lload(thgc, hv, coroptr, false));
										emit_coro_push_and_state(thgc, func, hv, list);
										emit_coro_txcommit(thgc, func, obj, rn4, list);
										emit_coro_end(thgc, func, obj, list);
										LVari* ov = emit_triptr_alloc(thgc, local, v, ps);
										LVari* retv = emit_makeframe_waithandle(thgc, local, rn4, func, ov);
										return make_voival(thgc);
									}
								}
								else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Delete.awaitの()が指定されていません");
							}
						}
					}
					// non-await Delete(bracket) path
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++; val = where;
						const char* sql_cstr = "";
						LVari* ps = make_lvari_c(thgc, "ptr", "null");
						int pcount = 0;
						LVari* v = NULL;
						if (where == NULL) {
							String* tn = ModelObj_TableName(thgc, modelobj);
							SB* sb2 = sb_create(thgc);
							sb_adds(thgc, sb2, "delete from ");
							sb_add_str(thgc, sb2, tn);
							sb_adds(thgc, sb2, ";");
							sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
							sql_cstr = str_to_cstr(thgc, &sb2->str);
							int sql_len = (int)strlen(sql_cstr);
							v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
						}
						else if (val->type == LetterType::OT_SqlString) {
							SqlString* sqs = (SqlString*)val;
							SqlString_Format(thgc, (Obj*)sqs);
							String* tn = ModelObj_TableName(thgc, modelobj);
							SB* sb2 = sb_create(thgc);
							sb_adds(thgc, sb2, "delete from ");
							sb_add_str(thgc, sb2, tn);
							sb_adds(thgc, sb2, " where ");
							sb_add_str(thgc, sb2, sqs->value);
							sb_adds(thgc, sb2, ";");
							sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
							sql_cstr = str_to_cstr(thgc, &sb2->str);
							int sql_len = (int)strlen(sql_cstr);
							pcount = (((Val*)sqs)->vmap != NULL) ? ((Val*)sqs)->vmap->kvs->size : 0;
							ps = emit_malloc_params(thgc, local, pcount);
							v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
							LCall* mcall2 = make_lcall(thgc, ps, make_lvari_c(thgc, "ptr", "@malloc"));
							char sz[32]; snprintf(sz, sizeof(sz), "%d", 24 * pcount);
							lcall_add(thgc, mcall2, make_lvari_c(thgc, "i64", sz));
							add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mcall2);
							for (int ki = 0; ki < ((Val*)sqs)->vmap->kvs->size; ki++) {
								KV* kv = (KV*)*(char**)get_list(((Val*)sqs)->vmap->kvs, ki);
								int key = atoi(str_to_cstr(thgc, kv->key));
								Obj* kval = (Obj*)kv->value;
								if (kval->type == LetterType::OT_Value) {
									Value* value = (Value*)kval;
									Obj* cls = value->cls;
									if (cls->type == LetterType::OT_Var) cls = value->cls = ((Type*)cls)->cls;
									if (cls->type == LetterType::OT_ModelObj) {
										if (cls == (Obj*)local->Int) emit_sql_param_int(thgc, local, ps, pcount, key, value->vari);
										else if (cls == (Obj*)local->StrT) emit_sql_param_str(thgc, local, ps, pcount, key, value->vari);
									}
								}
							}
						}
						else if (val->type == LetterType::OT_Value) {
							Value* value = (Value*)val;
							ps = make_lvari_c(thgc, "ptr", "null");
							if (value->cls == (Obj*)local->StrT) {
								String* tn = ModelObj_TableName(thgc, modelobj);
								SB* sb2 = sb_create(thgc);
								sb_adds(thgc, sb2, "delete from ");
								sb_add_str(thgc, sb2, tn);
								sb_adds(thgc, sb2, " where ");
								sb2->str.data[sb2->str.size * sb2->str.esize] = '\0';
								sql_cstr = str_to_cstr(thgc, &sb2->str);
								int sql_len = (int)strlen(sql_cstr);
								v = emit_create_string_sql(thgc, local, sql_cstr, sql_len);
								v = emit_add_string2(thgc, local, v, value->vari);
							}
							else return ErrorC(thgc, LetterType::OT_Error, ((Obj*)value)->letter, "abc");
						}
						else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Deleteの引数はSqlString型かString型です");
						{
							// Sync path: create coroutine function
							LVari* statev = NULL;
							LFunc* func = emit_coro_func_setup(thgc, local, "First", &statev);
							LVari* obj = (LVari*)get_list_val(func->draws, 0);
							LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
							List* list = create_list(thgc, sizeof(void*), CType::_LComp);
							LLab* sclab = make_llab(thgc, "coro_tx");
							func->sc = make_lswitchcomp(thgc, statev, sclab, list);
							add_list(thgc, func->comps, (char*)func->sc);
							emit_begin_transaction_block(thgc, func, list, sclab);
							LLab* lab10 = emit_coro_start_block(thgc, func, list);
							LVari* coroptr = emit_coro_execsql(thgc, func, obj, rn4, v, ps, pcount, NULL, NULL);
							LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
							add_list(thgc, func->comps, (char*)make_lload(thgc, hv, coroptr, false));
							emit_coro_push_and_state(thgc, func, hv, list);
							emit_coro_txcommit(thgc, func, obj, rn4, list);
							emit_coro_end(thgc, func, obj, list);
							LVari* ov = emit_triptr_alloc(thgc, local, v, ps);
							LVari* retv = emit_makeframe_waithandle(thgc, local, rn4, func, ov);
							return make_voival(thgc);
						}
					}
					return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Deleteの()が指定されていません");
				}

				// FirstAwait clause
				else if (str_equals(word2name, "FirstAwait")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++; val = where;
						LVari* ps = NULL;
						int pcount = 0;
						const char* err_msg = NULL;
						LVari* v = emit_select_sql(thgc, local, modelobj, val, " limit 1;", &ps, &pcount, &err_msg);
						if (v == NULL) {
							return ErrorC(thgc, LetterType::OT_Error, val2->letter, err_msg ? err_msg : "FirstAwaitの引数はSqlString型かString型です");
						}
						LFunc* fn = llvm_get_func(local->llvm);
						char dbsel[64]; snprintf(dbsel, sizeof(dbsel), "@DbSelect%d", modelobj->identity);
						char pcstr[16]; snprintf(pcstr, sizeof(pcstr), "%d", pcount);
						if (fn->async_flag) {
							// Async path: inline in current func
							LVari* tx = make_vn(thgc, "i8*", "tx");
							LVari* txp = make_vn(thgc, "i8**", "tx");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txp, make_lvari_c(thgc, "%RootNodeType*", "%rn"), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
							add_list(thgc, fn->comps, (char*)make_lload(thgc, tx, txp, false));
							// flush comps
							for (int ci = 0; ci < local->compsC->size; ci++) {
								LComp* comp = *(LComp**)get_list(local->compsC, ci);
								add_list(thgc, fn->comps, (char*)comp);
							}
							local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
							fn->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
							LVari* fv = make_vn(thgc, "%FuncType*", "fv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, fv, fn->vari, false));
							LVari* go_v = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "go");
							LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
							LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), modelobj->drawcall));
							lcall_add(thgc, go_call, rn4);
							lcall_add(thgc, go_call, fv);
							add_list(thgc, fn->comps, (char*)go_call);
							ModelObj_Select(thgc, modelobj, local);
							LVari* esql = make_lvari_c(thgc, "%ExecSqlType", "%esql");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, esql, make_lvari_c(thgc, "%ExecSqlType*", "@ExecSql"), false));
							LVari* coroptr = make_lvari_c(thgc, "%CoroFrameType**", "%coroptr");
							add_list(thgc, fn->comps, (char*)make_lalloca(thgc, coroptr));
							LCall* ecall = make_lcall(thgc, NULL, esql);
							lcall_add(thgc, ecall, coroptr);
							lcall_add(thgc, ecall, tx);
							lcall_add(thgc, ecall, v);
							lcall_add(thgc, ecall, ps);
							lcall_add(thgc, ecall, make_lvari_c(thgc, "i32", pcstr));
							lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", dbsel));
							lcall_add(thgc, ecall, go_v);
							add_list(thgc, fn->comps, (char*)ecall);
							LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, hv, coroptr, false));
							// push via ThreadGC queue slot 0
							LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
							LVari* thgcv = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
							LVari* qv = make_vn(thgc, "%CoroutineQueueType*", "qv");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%ThreadGCType", qv, thgcv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
							LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, qv2, qv, false));
							LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, queueptr2, qv2));
							LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
							lcall_add(thgc, pqcall, qv2);
							lcall_add(thgc, pqcall, hv);
							add_list(thgc, fn->comps, (char*)pqcall);
							// new hv state = 0
							LVari* stateptr = make_vn(thgc, "i32*", "st");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateptr, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, stateptr, make_lvari_c(thgc, "i32", "0")));
							// parent state = nvari
							LVari* statevalptr2 = make_vn(thgc, "i32*", "st");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
							char nvstr[16]; snprintf(nvstr, sizeof(nvstr), "%d", llvm_n++);
							LVari* nvari = make_lvari_c(thgc, "i32", nvstr);
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, statevalptr2, nvari));
							LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(fn->draws, 0)));
							add_list(thgc, fn->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
							// await resume label
							char labname[32]; snprintf(labname, sizeof(labname), "aw%d", llvm_n++);
							LLab* lab6 = make_llab(thgc, labname);
							add_list(thgc, fn->comps, (char*)lab6);
							LVari* gev0 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"**", 2, 1), "gv");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", gev0, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
							LVari* gev02 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "gv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, gev02, gev0, false));
							LVari* objptr0 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"**", 2, 1), "ob");
							add_list(thgc, fn->comps, (char*)make_lalloca(thgc, objptr0));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, objptr0, gev02));
							add_list(thgc, fn->sc->values, (char*)make_lifvalue(thgc, lab6, nvari));
							LVari* retp = make_vn(thgc, "i8**", "re");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", retp, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "8")));
							LVari* retv2 = make_vn(thgc, "i8*", "rv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, retv2, retp, false));
							// Process RowSet (HashType) -> get first row (KeyValueType) -> model*
							LVari* rowsp = make_vn(thgc, "%HashType**", "ro");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%RowSet", rowsp, retv2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
							LVari* rowsv = make_vn(thgc, "%HashType*", "ro");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, rowsv, rowsp, false));
							LVari* kvsp = make_vn(thgc, "%ListType**", "kv");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%HashType", kvsp, rowsv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
							LVari* kvs = make_vn(thgc, "%ListType*", "kv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, kvs, kvsp, false));
							LVari* sizep = make_vn(thgc, "i32*", "si");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%ListType", sizep, kvs, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
							LVari* sizev = make_vn(thgc, "i32", "si");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, sizev, sizep, false));
							LVari* equal = make_vn(thgc, "i1", "eq");
							add_list(thgc, fn->comps, (char*)make_lcmp(thgc, LCMP_Eq, equal, sizev, make_lvari_c(thgc, "i32", "0")));
							char l1name[32]; snprintf(l1name, sizeof(l1name), "coro_end%d", llvm_n++);
							char l2name[32]; snprintf(l2name, sizeof(l2name), "coro_ret%d", llvm_n++);
							LLab* lab1 = make_llab(thgc, l1name);
							LLab* lab2 = make_llab(thgc, l2name);
							add_list(thgc, fn->comps, (char*)make_lbr(thgc, equal, lab1, lab2));
							add_list(thgc, fn->comps, (char*)lab2);
							LVari* glf = make_vn(thgc, "%GetListType", "gl");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, glf, make_lvari_c(thgc, "%GetListType*", "@GetList"), false));
							LVari* op2 = make_vn(thgc, "ptr", "op");
							LCall* glcall = make_lcall(thgc, op2, glf);
							lcall_add(thgc, glcall, kvs);
							lcall_add(thgc, glcall, make_lvari_c(thgc, "i32", "0"));
							add_list(thgc, fn->comps, (char*)glcall);
							LVari* ov2 = make_vn(thgc, "%KeyValueType*", "ov");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, ov2, op2, false));
							String* m2 = StringAdd(thgc, modelobj->model, (char*)"**", 2, 1);
							LVari* op3 = make_vn_s(thgc, m2, "op");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%KeyValueType", op3, ov2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
							String* m1 = StringAdd(thgc, modelobj->model, (char*)"*", 1, 1);
							LVari* ov3 = make_vn_s(thgc, m1, "ov");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, ov3, op3, false));
							add_list(thgc, fn->comps, (char*)make_lbr(thgc, NULL, NULL, lab1));
							add_list(thgc, fn->comps, (char*)lab1);
							LVari* vv = make_vn_s(thgc, m1, "vv");
							Map* ifv_map = create_mapy(thgc, CType::_LIfValue);
							add_mapy(thgc, ifv_map, lab2->name, (char*)make_lifvalue(thgc, lab2, ov3));
							add_mapy(thgc, ifv_map, lab6->name, (char*)make_lifvalue(thgc, lab6, make_lvari(thgc, m1, createString(thgc, (char*)"null", 4, 1))));
							add_list(thgc, fn->comps, (char*)make_lphi(thgc, vv, ifv_map));
							{ Obj* _v = make_value(thgc, (Obj*)modelobj, vv); _v->letter = val2->letter; return _v; }
						}
						else {
							// Sync path
							LVari* statev = NULL;
							LFunc* func = emit_coro_func_setup(thgc, local, "First", &statev);
							LVari* obj = (LVari*)get_list_val(func->draws, 0);
							LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
							List* list = create_list(thgc, sizeof(void*), CType::_LComp);
							LLab* sclab = make_llab(thgc, "coro_tx");
							func->sc = make_lswitchcomp(thgc, statev, sclab, list);
							add_list(thgc, func->comps, (char*)func->sc);
							emit_begin_transaction_block(thgc, func, list, sclab);
							emit_coro_start_block(thgc, func, list);
							LVari* coroptr = emit_coro_execsql(thgc, func, obj, rn4, v, ps, pcount, dbsel, NULL);
							LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
							add_list(thgc, func->comps, (char*)make_lload(thgc, hv, coroptr, false));
							emit_coro_push_and_state(thgc, func, hv, list);
							emit_coro_txcommit(thgc, func, obj, rn4, list);
							emit_coro_end(thgc, func, obj, list);
							// In caller func: Triptr alloc + flush + drawcall + Select + store go_v
							LVari* ov = emit_triptr_alloc(thgc, local, v, ps);
							LFunc* fn2 = llvm_get_func(local->llvm);
							for (int ci = 0; ci < local->compsC->size; ci++) {
								LComp* comp = *(LComp**)get_list(local->compsC, ci);
								add_list(thgc, fn2->comps, (char*)comp);
							}
							local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
							fn2->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
							LVari* fv = make_vn(thgc, "%FuncType*", "fv");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, fv, fn2->vari, false));
							LVari* go_v = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "go");
							LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), modelobj->drawcall));
							lcall_add(thgc, go_call, rn4);
							lcall_add(thgc, go_call, fv);
							add_list(thgc, fn2->comps, (char*)go_call);
							ModelObj_Select(thgc, modelobj, local);
							emit_triptr_store_go(thgc, local, ov, go_v);
							LVari* retv = emit_makeframe_waithandle(thgc, local, rn4, func, ov);
							// Track prev label (l2 from emit_makeframe_waithandle) for phi
							LLab* prev_lab = NULL;
							if (fn2->comps->size > 0) {
								for (int ci = fn2->comps->size - 1; ci >= 0; ci--) {
									LComp* c = *(LComp**)get_list(fn2->comps, ci);
									if (c->comptype == LCompType::LC_Lab) { prev_lab = (LLab*)c; break; }
								}
							}
							// Process RowSet
							LVari* rowsp = make_vn(thgc, "%HashType**", "ro");
							add_list(thgc, fn2->comps, (char*)make_lgete_idx2(thgc, "%RowSet", rowsp, retv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
							LVari* rowsv = make_vn(thgc, "%HashType*", "ro");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, rowsv, rowsp, false));
							LVari* kvsp = make_vn(thgc, "%ListType**", "kv");
							add_list(thgc, fn2->comps, (char*)make_lgete_idx2(thgc, "%HashType", kvsp, rowsv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
							LVari* kvs = make_vn(thgc, "%ListType*", "kv");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, kvs, kvsp, false));
							LVari* sizep = make_vn(thgc, "i32*", "si");
							add_list(thgc, fn2->comps, (char*)make_lgete_idx2(thgc, "%ListType", sizep, kvs, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
							LVari* sizev = make_vn(thgc, "i32", "si");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, sizev, sizep, false));
							LVari* equal = make_vn(thgc, "i1", "eq");
							add_list(thgc, fn2->comps, (char*)make_lcmp(thgc, LCMP_Eq, equal, sizev, make_lvari_c(thgc, "i32", "0")));
							char l1name[32]; snprintf(l1name, sizeof(l1name), "coro_end%d", llvm_n++);
							char l2name[32]; snprintf(l2name, sizeof(l2name), "coro_ret%d", llvm_n++);
							LLab* lab1 = make_llab(thgc, l1name);
							LLab* lab2 = make_llab(thgc, l2name);
							add_list(thgc, fn2->comps, (char*)make_lbr(thgc, equal, lab1, lab2));
							add_list(thgc, fn2->comps, (char*)lab2);
							LVari* glf = make_vn(thgc, "%GetListType", "gl");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, glf, make_lvari_c(thgc, "%GetListType*", "@GetList"), false));
							LVari* op2 = make_vn(thgc, "ptr", "op");
							LCall* glcall = make_lcall(thgc, op2, glf);
							lcall_add(thgc, glcall, kvs);
							lcall_add(thgc, glcall, make_lvari_c(thgc, "i32", "0"));
							add_list(thgc, fn2->comps, (char*)glcall);
							LVari* ov2 = make_vn(thgc, "%KeyValueType*", "ov");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, ov2, op2, false));
							String* m2 = StringAdd(thgc, modelobj->model, (char*)"**", 2, 1);
							LVari* op3 = make_vn_s(thgc, m2, "op");
							add_list(thgc, fn2->comps, (char*)make_lgete_idx2(thgc, "%KeyValueType", op3, ov2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
							String* m1 = StringAdd(thgc, modelobj->model, (char*)"*", 1, 1);
							LVari* ov3 = make_vn_s(thgc, m1, "ov");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, ov3, op3, false));
							add_list(thgc, fn2->comps, (char*)make_lbr(thgc, NULL, NULL, lab1));
							add_list(thgc, fn2->comps, (char*)lab1);
							LVari* vv = make_vn_s(thgc, m1, "vv");
							Map* ifv_map = create_mapy(thgc, CType::_LIfValue);
							add_mapy(thgc, ifv_map, lab2->name, (char*)make_lifvalue(thgc, lab2, ov3));
							LLab* other_lab = prev_lab ? prev_lab : lab2;
							add_mapy(thgc, ifv_map, other_lab->name, (char*)make_lifvalue(thgc, other_lab, make_lvari(thgc, m1, createString(thgc, (char*)"null", 4, 1))));
							add_list(thgc, fn2->comps, (char*)make_lphi(thgc, vv, ifv_map));
							{ Obj* _v = make_value(thgc, (Obj*)modelobj, vv); _v->letter = val2->letter; return _v; }
						}
					}
					return ErrorC(thgc, LetterType::OT_Error, val2->letter, "FirstAwaitの()が指定されていません");
				}

				// SelectAwait clause
				else if (str_equals(word2name, "SelectAwait")) {
					if (val2->type == LetterType::OT_Bracket) {
						(*n)++; val = where;
						LVari* ps = NULL;
						int pcount = 0;
						const char* err_msg = NULL;
						LVari* v = emit_select_sql(thgc, local, modelobj, val, ";", &ps, &pcount, &err_msg);
						if (v == NULL) {
							return ErrorC(thgc, LetterType::OT_Error, val2->letter, err_msg ? err_msg : "SelectAwaitの引数はSqlString型かString型です");
						}
						LFunc* fn = llvm_get_func(local->llvm);
						char dbsel[64]; snprintf(dbsel, sizeof(dbsel), "@DbSelect%d", modelobj->identity);
						char pcstr[16]; snprintf(pcstr, sizeof(pcstr), "%d", pcount);
						if (fn->async_flag) {
							// Async path: same as FirstAwait but returns HashType*/rowsv directly (no first-row extraction)
							LVari* tx = make_vn(thgc, "i8*", "tx");
							LVari* txp = make_vn(thgc, "i8**", "tx");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txp, make_lvari_c(thgc, "%RootNodeType*", "%rn"), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
							add_list(thgc, fn->comps, (char*)make_lload(thgc, tx, txp, false));
							for (int ci = 0; ci < local->compsC->size; ci++) {
								LComp* comp = *(LComp**)get_list(local->compsC, ci);
								add_list(thgc, fn->comps, (char*)comp);
							}
							local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
							fn->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
							LVari* fv = make_vn(thgc, "%FuncType*", "fv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, fv, fn->vari, false));
							LVari* go_v = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "go");
							LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
							LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), modelobj->drawcall));
							lcall_add(thgc, go_call, rn4);
							lcall_add(thgc, go_call, fv);
							add_list(thgc, fn->comps, (char*)go_call);
							ModelObj_Select(thgc, modelobj, local);
							LVari* esql = make_lvari_c(thgc, "%ExecSqlType", "%esql");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, esql, make_lvari_c(thgc, "%ExecSqlType*", "@ExecSql"), false));
							LVari* coroptr = make_lvari_c(thgc, "%CoroFrameType**", "%coroptr");
							add_list(thgc, fn->comps, (char*)make_lalloca(thgc, coroptr));
							LCall* ecall = make_lcall(thgc, NULL, esql);
							lcall_add(thgc, ecall, coroptr);
							lcall_add(thgc, ecall, tx);
							lcall_add(thgc, ecall, v);
							lcall_add(thgc, ecall, ps);
							lcall_add(thgc, ecall, make_lvari_c(thgc, "i32", pcstr));
							lcall_add(thgc, ecall, make_lvari_c(thgc, "ptr", dbsel));
							lcall_add(thgc, ecall, go_v);
							add_list(thgc, fn->comps, (char*)ecall);
							LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, hv, coroptr, false));
							LVari* stateptr0 = make_vn(thgc, "i32*", "st");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", stateptr0, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, stateptr0, make_lvari_c(thgc, "i32", "0")));
							// push via ThreadGC queue slot 0
							LVari* pushqueue1 = make_vn(thgc, "%PushQueueType", "pu");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, pushqueue1, make_lvari_c(thgc, "%PushQueueType*", "@PushQueue"), false));
							LVari* thgcv = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
							LVari* qv = make_vn(thgc, "%CoroutineQueueType*", "qv");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%ThreadGCType", qv, thgcv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
							LVari* qv2 = make_vn(thgc, "%CoroutineQueueType*", "qv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, qv2, qv, false));
							LVari* queueptr2 = make_vn(thgc, "%CoroutineQueueType**", "qu");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", queueptr2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "4")));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, queueptr2, qv2));
							LCall* pqcall = make_lcall(thgc, NULL, pushqueue1);
							lcall_add(thgc, pqcall, qv2);
							lcall_add(thgc, pqcall, hv);
							add_list(thgc, fn->comps, (char*)pqcall);
							LVari* statevalptr2 = make_vn(thgc, "i32*", "st");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr2, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
							char nvstr[16]; snprintf(nvstr, sizeof(nvstr), "%d", llvm_n++);
							LVari* nvari = make_lvari_c(thgc, "i32", nvstr);
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, statevalptr2, nvari));
							LVari* parent2 = make_vn(thgc, "%CoroFrameType**", "pa");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", parent2, hv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "6")));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, parent2, (LVari*)get_list_val(fn->draws, 0)));
							add_list(thgc, fn->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
							char labname[32]; snprintf(labname, sizeof(labname), "aw%d", llvm_n++);
							LLab* lab6 = make_llab(thgc, labname);
							add_list(thgc, fn->comps, (char*)lab6);
							LVari* gev0 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"**", 2, 1), "gv");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", gev0, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
							LVari* gev02 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "gv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, gev02, gev0, false));
							LVari* objptr0 = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"**", 2, 1), "ob");
							add_list(thgc, fn->comps, (char*)make_lalloca(thgc, objptr0));
							add_list(thgc, fn->comps, (char*)make_lstore(thgc, objptr0, gev02));
							add_list(thgc, fn->sc->values, (char*)make_lifvalue(thgc, lab6, nvari));
							LVari* retp = make_vn(thgc, "i8**", "re");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", retp, (LVari*)get_list_val(fn->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "8")));
							LVari* retv2 = make_vn(thgc, "i8*", "rv");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, retv2, retp, false));
							// RowSet -> HashType* rowsv (SelectAwait returns this directly)
							LVari* rowsp = make_vn(thgc, "%HashType**", "ro");
							add_list(thgc, fn->comps, (char*)make_lgete_idx2(thgc, "%RowSet", rowsp, retv2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
							LVari* rowsv = make_vn(thgc, "%HashType*", "ro");
							add_list(thgc, fn->comps, (char*)make_lload(thgc, rowsv, rowsp, false));
							{ Obj* _v = make_value(thgc, (Obj*)modelobj, rowsv); _v->letter = val2->letter; return _v; }
						}
						else {
							// Sync path
							LVari* statev = NULL;
							LFunc* func = emit_coro_func_setup(thgc, local, "First", &statev);
							LVari* obj = (LVari*)get_list_val(func->draws, 0);
							LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
							List* list = create_list(thgc, sizeof(void*), CType::_LComp);
							LLab* sclab = make_llab(thgc, "coro_tx");
							func->sc = make_lswitchcomp(thgc, statev, sclab, list);
							add_list(thgc, func->comps, (char*)func->sc);
							emit_begin_transaction_block(thgc, func, list, sclab);
							emit_coro_start_block(thgc, func, list);
							LVari* coroptr = emit_coro_execsql(thgc, func, obj, rn4, v, ps, pcount, dbsel, NULL);
							LVari* hv = make_lvari_c(thgc, "%CoroFrameType*", "%corov");
							add_list(thgc, func->comps, (char*)make_lload(thgc, hv, coroptr, false));
							emit_coro_push_and_state(thgc, func, hv, list);
							emit_coro_txcommit(thgc, func, obj, rn4, list);
							emit_coro_end(thgc, func, obj, list);
							LVari* ov = emit_triptr_alloc(thgc, local, v, ps);
							LFunc* fn2 = llvm_get_func(local->llvm);
							for (int ci = 0; ci < local->compsC->size; ci++) {
								LComp* comp = *(LComp**)get_list(local->compsC, ci);
								add_list(thgc, fn2->comps, (char*)comp);
							}
							local->compsC = create_list(thgc, sizeof(LComp*), CType::_LComp);
							fn2->vari->type = createString(thgc, (char*)"%FuncType**", 11, 1);
							LVari* fv = make_vn(thgc, "%FuncType*", "fv");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, fv, fn2->vari, false));
							LVari* go_v = make_vn_s(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), "go");
							LCall* go_call = make_lcall(thgc, go_v, make_lvari(thgc, StringAdd(thgc, modelobj->model, (char*)"*", 1, 1), modelobj->drawcall));
							lcall_add(thgc, go_call, rn4);
							lcall_add(thgc, go_call, fv);
							add_list(thgc, fn2->comps, (char*)go_call);
							ModelObj_Select(thgc, modelobj, local);
							emit_triptr_store_go(thgc, local, ov, go_v);
							LVari* retv = emit_makeframe_waithandle(thgc, local, rn4, func, ov);
							LLab* prev_lab = NULL;
							if (fn2->comps->size > 0) {
								for (int ci = fn2->comps->size - 1; ci >= 0; ci--) {
									LComp* c = *(LComp**)get_list(fn2->comps, ci);
									if (c->comptype == LCompType::LC_Lab) { prev_lab = (LLab*)c; break; }
								}
							}
							// SelectAwait sync path: process ListType rows instead of HashType
							LVari* rowsp = make_vn(thgc, "%ListType**", "ro");
							add_list(thgc, fn2->comps, (char*)make_lgete_idx2(thgc, "%RowSet", rowsp, retv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
							LVari* rowsv = make_vn(thgc, "%ListType*", "ro");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, rowsv, rowsp, false));
							LVari* sizep = make_vn(thgc, "i32*", "si");
							add_list(thgc, fn2->comps, (char*)make_lgete_idx2(thgc, "%ListType", sizep, rowsv, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
							LVari* sizev = make_vn(thgc, "i32", "si");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, sizev, sizep, false));
							LVari* equal = make_vn(thgc, "i1", "eq");
							add_list(thgc, fn2->comps, (char*)make_lcmp(thgc, LCMP_Eq, equal, sizev, make_lvari_c(thgc, "i32", "0")));
							char l1name[32]; snprintf(l1name, sizeof(l1name), "coro_end%d", llvm_n++);
							char l2name[32]; snprintf(l2name, sizeof(l2name), "coro_ret%d", llvm_n++);
							LLab* lab1 = make_llab(thgc, l1name);
							LLab* lab2 = make_llab(thgc, l2name);
							add_list(thgc, fn2->comps, (char*)make_lbr(thgc, equal, lab1, lab2));
							add_list(thgc, fn2->comps, (char*)lab2);
							LVari* glf = make_vn(thgc, "%GetListType", "gl");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, glf, make_lvari_c(thgc, "%GetListType*", "@GetList"), false));
							String* m2 = StringAdd(thgc, modelobj->model, (char*)"**", 2, 1);
							LVari* op2 = make_vn_s(thgc, m2, "op");
							LCall* glcall = make_lcall(thgc, op2, glf);
							lcall_add(thgc, glcall, rowsv);
							lcall_add(thgc, glcall, make_lvari_c(thgc, "i32", "0"));
							add_list(thgc, fn2->comps, (char*)glcall);
							String* m1 = StringAdd(thgc, modelobj->model, (char*)"*", 1, 1);
							LVari* ov2 = make_vn_s(thgc, m1, "ov");
							add_list(thgc, fn2->comps, (char*)make_lload(thgc, ov2, op2, false));
							add_list(thgc, fn2->comps, (char*)make_lbr(thgc, NULL, NULL, lab1));
							add_list(thgc, fn2->comps, (char*)lab1);
							LVari* vv = make_vn_s(thgc, m1, "vv");
							Map* ifv_map = create_mapy(thgc, CType::_LIfValue);
							add_mapy(thgc, ifv_map, lab2->name, (char*)make_lifvalue(thgc, lab2, ov2));
							LLab* other_lab = prev_lab ? prev_lab : lab2;
							add_mapy(thgc, ifv_map, other_lab->name, (char*)make_lifvalue(thgc, other_lab, make_lvari(thgc, m1, createString(thgc, (char*)"null", 4, 1))));
							add_list(thgc, fn2->comps, (char*)make_lphi(thgc, vv, ifv_map));
							{ Obj* _v = make_value(thgc, (Obj*)modelobj, vv); _v->letter = val2->letter; return _v; }
						}
					}
					return ErrorC(thgc, LetterType::OT_Error, val2->letter, "SelectAwaitの()が指定されていません");
				}
			}
		}
	}
	// --------------------------------------------------------
	// Model.Store / Model.Delete / Model.await (Dot branch)
	// Ported from Obj2C.cs lines 9699-11642
	// --------------------------------------------------------
	else if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type != LetterType::OT_Word)
			return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の次には名前が来ます");
		Word* word = (Word*)val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (str_equals(word->letter->text, "Store") || str_equals(word->letter->text, "Delete")) {
			bool is_store = str_equals(word->letter->text, "Store");
			const char* opname = is_store ? "Store" : "Delete";
			// Optional .await dispatch
			bool is_await = false;
			if (val2->type == LetterType::_Dot) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Word) {
					Word* word2 = (Word*)val2;
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					if (str_equals(word2->letter->text, "await")) {
						is_await = true;
					} else {
						return ErrorC(thgc, LetterType::OT_Error, val2->letter, is_store ? "Storeの.の後にawaitがありません" : "Deleteの.の後にawaitがありません");
					}
				} else {
					return make_voival(thgc);
				}
			}
			if (val2->type != LetterType::OT_Bracket) {
				char errbuf[64];
				snprintf(errbuf, sizeof(errbuf), "%sの後に()がありません", opname);
				return ErrorC(thgc, LetterType::OT_Error, val2->letter, errbuf);
			}
			(*n)++;
			// Create coroutine function for Tx(Store|Delete)
			char callname[64];
			snprintf(callname, sizeof(callname), "@Tx%s%d", opname, llvm_n++);
			LVari* self_vari = make_lvari_s(thgc, "void", createString(thgc, callname, (int)strlen(callname), 1));
			LVari* obj_frame = make_lvari_c(thgc, "%CoroFrameType*", "%frame");
			LFunc* func = make_lfunc(thgc, local->llvm, self_vari, obj_frame);
			add_list(thgc, local->llvm->comps, (char*)func);
			func->async_flag = true;
			LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
			LVari* rngv = make_lvari_c(thgc, "%RootNodeType**", "%rngv");
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", rngv, obj_frame, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
			LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
			add_list(thgc, func->comps, (char*)make_lload(thgc, rn4, rngv, false));
			add_list(thgc, func->comps, (char*)make_lload(thgc, thgc4, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));
			LVari* geterv = make_lvari_c(thgc, "i32*", "%state");
			add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", geterv, obj_frame, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
			LVari* statev = make_vn(thgc, "i32", "sv");
			add_list(thgc, func->comps, (char*)make_lload(thgc, statev, geterv, false));
			List* list = create_list(thgc, sizeof(void*), CType::_LIfValue);
			LLab* lab_tx = make_llab(thgc, "coro_tx");
			func->sc = make_lswitchcomp(thgc, statev, lab_tx, list);
			add_list(thgc, func->comps, (char*)func->sc);
			// Begin transaction for sync path
			if (!llvm_get_func(local->llvm)->async_flag) {
				add_list(thgc, list, (char*)make_lifvalue(thgc, lab_tx, make_lvari_c(thgc, "i32", "-2")));
				add_list(thgc, func->comps, (char*)lab_tx);
			}
			// Evaluate argument block
			Obj* val = GetterC_vt(thgc, exeC(thgc, val2, local), local);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			Block* blk = (Block*)val;
			// Declare a store packing type for the args
			char typename_buf[32];
			snprintf(typename_buf, sizeof(typename_buf), "%%store%d", llvm_n++);
			LTypeDec* typedec = make_ltypedec(thgc, createString(thgc, typename_buf, (int)strlen(typename_buf), 1));
			add_list(thgc, local->llvm->types, (char*)typedec);
			LVari* countv = make_lvari_s(thgc, "i64", createString(thgc, "", 0, 1));
			LVari* objp = make_vn(thgc, "ptr", "op");
			{
				LCall* mc = make_lcall(thgc, objp, make_lvari_c(thgc, "ptr", "@malloc"));
				lcall_add(thgc, mc, countv);
				add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mc);
			}
			int count = 0;
			int n3 = 0;
			for (int i = 0; i < blk->rets->size; i++) {
				Obj* ret_obj = *(Obj**)get_list(blk->rets, i);
				if (ret_obj->type != LetterType::OT_Value)
					return ErrorC(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, i), is_store ? "Storeの引数には値を指定してください" : "Deleteの引数には値を指定してください");
				Value* value = (Value*)ret_obj;
				if (value->cls && value->cls->type == LetterType::OT_Var) value->cls = ((Var*)value->cls)->cls;
				if (!value->cls || (value->cls->type != LetterType::OT_ModelObj && value->cls->type != LetterType::OT_GeneObj))
					return ErrorC(thgc, LetterType::OT_Error, *(ATSSpan**)get_list(blk->letters, i), is_store ? "Storeの引数にはオブジェクトクラスかGeneオブジェクトを指定してください" : "Deleteの引数にはオブジェクトクラスかGeneオブジェクトを指定してください");
				// Coro start label for each arg
				char startname[32];
				snprintf(startname, sizeof(startname), "coro_start%d", llvm_n++);
				LLab* lab_start = make_llab(thgc, startname);
				int n2 = llvm_n++;
				char n2str[16]; snprintf(n2str, sizeof(n2str), "%d", n2);
				add_list(thgc, list, (char*)make_lifvalue(thgc, lab_start, make_lvari_c(thgc, "i32", n2str)));
				LVari* statevalptr3 = make_vn(thgc, "i32*", "sp");
				add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", statevalptr3, (LVari*)get_list_val(func->draws, 0), make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "7")));
				add_list(thgc, func->comps, (char*)make_lstore(thgc, statevalptr3, make_lvari_c(thgc, "i32", n2str)));
				add_list(thgc, func->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
				add_list(thgc, func->comps, (char*)lab_start);
				ModelObj* model = (ModelObj*)value->cls;
				if (is_store) ModelObj_Store(thgc, model, local);
				else ModelObj_Delete(thgc, model, local);
				LVari* txptr = make_vn(thgc, "i8**", "tx");
				add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", txptr, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
				LVari* tx = make_vn(thgc, "i8*", "tx");
				add_list(thgc, func->comps, (char*)make_lload(thgc, tx, txptr, false));
				char tpstar[64]; snprintf(tpstar, sizeof(tpstar), "%s**", typename_buf);
				char tp[64]; snprintf(tp, sizeof(tp), "%s*", typename_buf);
				LVari* objvp = make_vn(thgc, tpstar, "gv");
				add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%CoroFrameType", objvp, obj_frame, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "5")));
				LVari* objv = make_vn(thgc, tp, "ov");
				add_list(thgc, func->comps, (char*)make_lload(thgc, objv, objvp, false));
				LVari* vx = emit_store_value_type(thgc, local, func, value, objp, objv, typename_buf, typedec, i, n3, &count);
				if (is_store) emit_dbstore_push(thgc, func, model, tx, vx);
				else emit_dbdelete_push(thgc, func, model, tx, vx);
				n3++;
			}
			{
				char cstr[16]; snprintf(cstr, sizeof(cstr), "%d", count);
				countv->name = createString(thgc, cstr, (int)strlen(cstr), 1);
			}
			emit_coro_fin_async(thgc, func, obj_frame, list, 0);
			// Caller side: create handle + push queue so the coroutine starts
			LVari* hv = make_vn(thgc, "%CoroFrameType*", "hv");
			LVari* mkf = make_vn(thgc, "%MakeFrameType", "mk");
			add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)make_lload(thgc, mkf, make_lvari_c(thgc, "%MakeFrameType*", "@MakeFrame"), false));
			{
				LCall* mc = make_lcall(thgc, hv, mkf);
				lcall_add(thgc, mc, make_lvari_c(thgc, "%RootNodeType*", "%rn"));
				lcall_add(thgc, mc, make_lvari_s(thgc, "ptr", func->y->name));
				lcall_add(thgc, mc, objp);
				add_list(thgc, llvm_get_func(local->llvm)->comps, (char*)mc);
			}
			(void)is_await;
			return make_voival(thgc);
		}
		else if (str_equals(word->letter->text, "await")) {
			// Model.await(CallBlock) - delegate to similar machinery as Store.await (simplified)
			if (val2->type != LetterType::OT_CallBlock)
				return ErrorC(thgc, LetterType::OT_Error, val2->letter, "awaitの後に{}がありません");
			(*n)++;
			return make_voival(thgc);
		}
		else {
			char errbuf[128];
			snprintf(errbuf, sizeof(errbuf), "modelには%sは宣言されていません", str_to_cstr(thgc, word->letter->text));
			return ErrorC(thgc, LetterType::OT_Error, val2->letter, errbuf);
		}
	}
	// --------------------------------------------------------
	// Model Word{} (instance declaration) - lines 11643-11655
	// --------------------------------------------------------
	else if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		Obj* gj = getC(thgc, local, word->letter->text);
		if (val2->type == LetterType::OT_CallBlock) {
			(*n)++;
			return gj;
		}
		return ErrorC(thgc, LetterType::OT_Error, val2->letter, "modelの内容を{}で宣言してください。");
	}
	// --------------------------------------------------------
	// Model{} (class body) - lines 11656-12168
	// Delegates to ModelObj_Dec, which performs the full class emission
	// --------------------------------------------------------
	else if (val2->type == LetterType::OT_CallBlock) {
		ModelObj* mself = (ModelObj*)self;
		if (mself->identity == 0) mself->identity = llvm_n++;
		// Note: ModelObj_Dec reads the block directly from stored state; val2 ignored here
		ModelObj_Dec(thgc, self, local);
		(*n)++;
		return make_voival(thgc);
	}
	return ErrorC(thgc, LetterType::OT_Error, val2->letter, "model名を宣言してください。");
}

// ============================================================
// end of obj2c_part4.h
// ============================================================
// obj2c_part5.h - Obj2C.cs lines 11000-13773
// Continuation of Model::PrimaryC (store/await branches), ModelObj, ModelVal,
// StockType, Stock, Gene, GeneChild, GeneMutate, GeneNew, GeneCross,
// GeneVal, GeneStore, GeneSelect, GeneSort, GeneLabel, GeneLabelValue,
// NewBlock, CrossBlock, MigrateBlock, GeneObj

// ============================================================
// Forward declarations for this file
// ============================================================
Obj* ModelObj_exeC(ThreadGC* thgc, Obj* self, Master* local);
void ModelObj_Dec(ThreadGC* thgc, Obj* self, Master* local);
Obj* ModelVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* Stock_SelectC(ThreadGC* thgc, Stock* self, Obj* model, Master* local, Function* func);
Obj* Stock_StoreC(ThreadGC* thgc, Stock* self, Val* val, Master* local);
Obj* Gene_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* Gene_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneChild_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneMutate_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneNew_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneCross_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneStore_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneSelect_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneSort_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* GeneLabel_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary);
Obj* GeneLabelValue_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2);
Obj* NewBlock_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* CrossBlock_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* MigrateBlock_exeC(ThreadGC* thgc, Obj* self, Master* local);
Obj* GeneObj_DotC(ThreadGC* thgc, GeneObj* self, String* name);
Obj* GeneObj_exeC(ThreadGC* thgc, Obj* self, Master* local);
String* ModelObj_TableName(ThreadGC* thgc, ModelObj* self);
void ModelObj_Store(ThreadGC* thgc, ModelObj* self, Master* local);
void ModelObj_Delete(ThreadGC* thgc, ModelObj* self, Master* local);
void ModelObj_Select(ThreadGC* thgc, ModelObj* self, Master* local);

// ============================================================
// Model::PrimaryC - continuation from obj2c_part4 (lines 11000-12172)
// This is the tail of the "store" and "await" branches inside
// Model::PrimaryC. The code is deeply nested LLVM IR generation
// that continues from the previous part file.
//
// NOTE: Lines 11000-12172 are the continuation of the Model::PrimaryC
// method which started much earlier. The deeply nested LLVM IR
// generation code for coroutine frames, store, and await is
// continued here. These lines remain inside Model_PrimaryC (from part4).
// The actual new class methods start from line 12173 (ModelObj).
// ============================================================

// ============================================================
// ModelObj::model property getter
// C#: public override string model { get { if (initial) return _model; else return base.model; } set => _model = value; }
// In C, model is accessed via self->model field; the property logic
// is handled inline where needed.
// ============================================================

// ============================================================
// ModelObj::exeC (line 12184)
// C#: public override Obj exeC(Local local) { return this; }
// ============================================================
Obj* ModelObj_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// ModelObj::Dec (line 12188)
// C#: public override async void Dec(Local local) { ... }
// ============================================================
void ModelObj_Dec(ThreadGC* thgc, Obj* self, Master* local) {
	List* blocks = get_blocks(local);
	ModelObj* mself = (ModelObj*)self;
	SetIdentity(thgc, (Obj*)(mself));
	// local.migrate.AddModel(letter.name, this, local);
	AddModel_migrate(thgc, local->migrate, self->letter->text, self, local);
	mself->ifv = create_list(thgc, sizeof(void*), CType::_LIfValue);
	mself->n_val = obj_cn++;
	mself->vari_c = make_lvari_s(thgc, "void", mself->call);
	LVari* rn4 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* obj = make_lvari_sc(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str)), "%obj");
	LFunc* func = make_lfunc(thgc, local->llvm, mself->vari_c, rn4, obj);
	add_list(thgc, local->llvm->comps, (char*)func);
	add_list(thgc, local->llvm->funcs, (char*)func);
	LVari* thgcptr4 = make_lvari_c(thgc, "%ThreadGCType**", "%thgcptr");
	add_list(thgc, func->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", thgcptr4, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* thgc4 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, func->comps, (char*)make_lload(thgc, thgc4, thgcptr4, false));

	LVari* objptr = make_lvari_sc(thgc, fmt_str(thgc, "%s**", str_to_cstr(thgc, mself->model_str)), "%objptr");
	add_list(thgc, func->comps, (char*)make_lalloca(thgc, objptr));
	add_list(thgc, func->comps, (char*)make_lstore(thgc, objptr, obj));
	LVari* srv = make_vn(thgc, "%GC_SetRootType", "sr");
	add_list(thgc, func->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
	LCall* srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, rn4);
	lcall_add(thgc, srcall, objptr);
	add_list(thgc, func->comps, (char*)srcall);

	LVari* rn5 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* blk = make_lvari_c(thgc, "i8*", "%fptr1");
	LFunc* funcdraw = make_lfunc(thgc, local->llvm, make_lvari(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str)), mself->drawcall), rn5, blk);
	add_list(thgc, local->llvm->comps, (char*)funcdraw);

	LTypeDec* typedec = make_ltypedec(thgc, mself->model_str);
	add_list(thgc, local->llvm->types, (char*)typedec);
	add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i8*", "blk"));

	LVari* thgc2 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* i8p = make_lvari_c(thgc, "i8*", "%self");
	String* checkname = fmt_str(thgc, "@%sCheck%d", str_to_cstr(thgc, self->letter->text), mself->identity);
	LFunc* funccheck = make_lfunc(thgc, local->llvm, make_lvari_s(thgc, "void", checkname), thgc2, i8p);
	add_list(thgc, local->llvm->comps, (char*)funccheck);
	LVari* vari = make_vn(thgc, "i8*", "va");
	LGete* gete = make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0"));
	add_list(thgc, funccheck->comps, (char*)gete);
	LVari* co_val = make_lvari_c(thgc, "%CopyObjectType", "%co");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, co_val, make_lvari_c(thgc, "%CopyObjectType*", "@CopyObject"), false));
	LVari* vari2 = make_vn(thgc, "i8*", "va");
	add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
	LVari* co_cval = make_vn(thgc, "i8*", "co");
	LCall* co_call = make_lcall(thgc, co_cval, co_val);
	lcall_add(thgc, co_call, thgc2);
	lcall_add(thgc, co_call, vari);
	add_list(thgc, funccheck->comps, (char*)co_call);
	add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));

	LVari* thgc3 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	LVari* ac_val = make_lvari_c(thgc, "%GC_AddClassType", "%addclass");
	LVari* countv = make_lvari_c(thgc, "i32", "0");
	char namebuf[64];
	snprintf(namebuf, sizeof(namebuf), "for%d", llvm_n++);
	String* name_s = createString(thgc, namebuf, (int)strlen(namebuf), 1);
	LStrV* strv = make_lstrv(thgc, fmt_str(thgc, "@%s", namebuf), name_s, (int)strlen(namebuf));
	add_list(thgc, local->llvm->strs, (char*)strv);
	int typ_n = (mself->n_val = local->llvm->cn++);
	char typbuf[16]; snprintf(typbuf, sizeof(typbuf), "%d", typ_n);
	LVari* typ = make_lvari_c(thgc, "i32", typbuf);
	LVari* tnp = make_vn(thgc, "i32", "tn");
	add_list(thgc, local->llvm->main->comps, (char*)make_lload(thgc, tnp, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tv = make_vn(thgc, "i32", "tv");
	add_list(thgc, local->llvm->main->comps, (char*)make_lbinop(thgc, LBOP_Add, tv, tnp, typ));
	LCall* ac_call = make_lcall(thgc, NULL, ac_val);
	lcall_add(thgc, ac_call, thgc3);
	lcall_add(thgc, ac_call, tv);
	lcall_add(thgc, ac_call, countv);
	lcall_add(thgc, ac_call, (LVari*)strv);
	lcall_add(thgc, ac_call, make_lvari_s(thgc, "%GCCheckFuncType", checkname));
	lcall_add(thgc, ac_call, make_lvari_c(thgc, "%GCFinalizeFuncType", "null"));
	add_list(thgc, local->llvm->main->comps, (char*)ac_call);

	LVari* thgcptr5 = make_lvari_c(thgc, "%ThreadGCType**", "%thgcptr");
	add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", thgcptr5, rn5, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	LVari* thgc5 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, thgc5, thgcptr5, false));
	LVari* go_val = make_lvari_c(thgc, "%CopyObjectType", "%gcobject");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, go_val, make_lvari_c(thgc, "%CopyObjectType*", "@CloneObject"), false));
	LVari* v3 = make_vn(thgc, "i8**", "v3");
	add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, "%FuncType", v3, blk, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
	LVari* v4 = make_vn(thgc, "i8*", "v4");
	add_list(thgc, funcdraw->comps, (char*)make_lload(thgc, v4, v3, false));
	LVari* go_v = make_lvari_c(thgc, "i8*", "%obj");
	LCall* go_call_d = make_lcall(thgc, go_v, go_val);
	lcall_add(thgc, go_call_d, thgc5);
	lcall_add(thgc, go_call_d, v4);
	add_list(thgc, funcdraw->comps, (char*)go_call_d);
	LVari* go_c = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str))), "go");
	add_list(thgc, funcdraw->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c, go_v));

	LVari* rn6 = make_lvari_c(thgc, "%RootNodeType*", "%rn");
	LVari* block = make_lvari_c(thgc, "i8*", "%block");
	LVari* funcptr = make_lvari_c(thgc, "i8*", "%fptr");
	LVari* funcptr2 = make_lvari_c(thgc, "i8*", "%fptr2");
	String* decname = fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, self->letter->text), mself->identity);
	LFunc* funcdec = make_lfunc5(thgc, local->llvm, make_lvari_s(thgc, "%FuncType*", decname), rn6, block, funcptr, funcptr2);
	add_list(thgc, local->llvm->comps, (char*)funcdec);
	LVari* thgc6 = make_lvari_c(thgc, "%ThreadGCType*", "%thgc");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, thgc6, make_lvari_c(thgc, "%ThreadGCType**", "@thgcp"), false));

	LVari* gmvari = make_lvari_c(thgc, "%GC_mallocType", "%gm");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, gmvari, make_lvari_c(thgc, "%GC_mallocType*", "@GC_malloc"), false));
	LVari* tmp_28 = make_lvari_c(thgc, "i32", "28");
	LVari* go_v_dec = make_lvari_c(thgc, "i8*", "%obj");
	LCall* go_call_dec = make_lcall(thgc, go_v_dec, gmvari);
	lcall_add(thgc, go_call_dec, thgc6);
	lcall_add(thgc, go_call_dec, tmp_28);
	add_list(thgc, funcdec->comps, (char*)go_call_dec);
	LVari* go_c3 = make_vn(thgc, "%FuncType*", "go");
	add_list(thgc, funcdec->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c3, go_v_dec));

	LVari* vc = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc, block));
	LVari* vc2 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc2, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc2, funcptr));
	LVari* vc3 = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc3, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "2")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc3, funcptr2));

	LVari* objptr2 = make_lvari_c(thgc, "%FuncType**", "%objptr");
	add_list(thgc, funcdec->comps, (char*)make_lalloca(thgc, objptr2));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, objptr2, go_c3));

	srv = make_vn(thgc, "%GC_SetRootType", "sr");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, srv, make_lvari_c(thgc, "%GC_SetRootType*", "@GC_SetRoot"), false));
	srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, rn6);
	lcall_add(thgc, srcall, objptr2);
	add_list(thgc, funcdec->comps, (char*)srcall);

	go_v = make_vn(thgc, "%GCObjectPtr", "go");
	char tmpbuf2[16]; snprintf(tmpbuf2, sizeof(tmpbuf2), "%d", mself->n_val);
	LVari* tmp2 = make_lvari_c(thgc, "i32", tmpbuf2);
	LVari* tmi = make_vn(thgc, "i32", "tm");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, tmi, make_lvari_c(thgc, "i32*", "@cnp"), false));
	LVari* tmv = make_vn(thgc, "i32", "tm");
	add_list(thgc, funcdec->comps, (char*)make_lbinop(thgc, LBOP_Add, tmv, tmi, tmp2));
	LCall* go_call2 = make_lcall(thgc, go_v, gmvari);
	lcall_add(thgc, go_call2, thgc6);
	lcall_add(thgc, go_call2, tmv);
	add_list(thgc, funcdec->comps, (char*)go_call2);
	LVari* go_c2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str))), "go");
	add_list(thgc, funcdec->comps, (char*)make_lcast(thgc, LCAST_Bitcast, go_c2, go_v));

	vc = make_vn(thgc, "i8*", "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vc, go_c2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "0")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc, block));

	LVari* objptr3 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s**", str_to_cstr(thgc, mself->model_str))), "op");
	add_list(thgc, funcdec->comps, (char*)make_lalloca(thgc, objptr3));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, objptr3, go_c2));

	srcall = make_lcall(thgc, NULL, srv);
	lcall_add(thgc, srcall, rn6);
	lcall_add(thgc, srcall, objptr3);
	add_list(thgc, funcdec->comps, (char*)srcall);

	LVari* vc4 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s**", str_to_cstr(thgc, mself->model_str))), "vc");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%FuncType", vc4, go_c3, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "3")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, vc4, go_c2));

	int count = 8;
	Block* blk0 = (Block*)*(Obj**)get_list(mself->draw->children, 0);
	int order = 2;
	Block* blk2 = (Block*)*(Obj**)get_list(mself->draw->children, 1);
	List* decs = create_list(thgc, sizeof(Obj*), CType::_CObj);
	add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i32", "id"));

	// iterate blk0.vmapA
	for (int ki = 0; ki < blk0->vmapA->kvs->size; ki++) {
		KV* kv = (KV*)get_list(blk0->vmapA->kvs, ki);
		Obj* v = (Obj*)kv->value;
		if (v->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)v;
			variable->order = order++;
			if (variable->cls->type == LetterType::OT_Var) variable->cls = ((Type*)variable->cls)->cls;
			if (variable->cls == local->Int) {
				LVari* vdraw = make_vn(thgc, "i32", "vd");
				add_list(thgc, funcdraw->draws, (char*)vdraw);
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i32", str_to_cstr(thgc, kv->key)));
				count += 8;
				LVari* varii = make_vn(thgc, "i32*", "va");
				char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", variable->order);
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), varii, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii, vdraw));
			}
			else if (variable->cls == local->Bool) {
				LVari* vdraw = make_vn(thgc, "i1", "vd");
				add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, "i32", "dr"));
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i1", str_to_cstr(thgc, kv->key)));
				count += 8;
				LVari* varii = make_vn(thgc, "i1*", "va");
				char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", variable->order);
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), varii, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii, vdraw));
			}
			else {
				SetIdentity(thgc, (Obj*)(variable->cls));
				String* vmodel = variable->cls->model_str;
				LVari* vdraw = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, vmodel))), "vd");
				add_list(thgc, funcdraw->draws, (char*)vdraw);
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, vmodel))), str_to_cstr(thgc, kv->key)));
				count += 8;
				LVari* varii_m = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, vmodel))), "vm");
				char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", variable->order);
				add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), varii_m, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, varii_m, vdraw));

				vari = make_vn(thgc, "i8*", "va");
				gete = make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf));
				add_list(thgc, funccheck->comps, (char*)gete);
				vari2 = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				co_cval = make_vn(thgc, "i8*", "co");
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
			f->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, ((Obj*)f)->model_str))), "fd"));
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));

			vari = make_vn(thgc, "i8*", "va");
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", f->order);
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, block);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", fmt_str(thgc, "@%s", str_to_cstr(thgc, f->drawcall))));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str))), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
		else if (v->type == LetterType::OT_ClassObj || v->type == LetterType::OT_ModelObj) {
			ClassObj* f = (ClassObj*)v;
			SetIdentity(thgc, (Obj*)(f));
			f->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));
			add_list(thgc, funcdraw->draws, (char*)make_vn(thgc, "%FuncType*", "dr"));

			vari = make_vn(thgc, "i8*", "va");
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", f->order);
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, block);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", fmt_str(thgc, "@%s", str_to_cstr(thgc, f->drawcall))));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str))), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
	}

	// iterate blk2.vmapA
	for (int ki = 0; ki < blk2->vmapA->kvs->size; ki++) {
		KV* kv = (KV*)get_list(blk2->vmapA->kvs, ki);
		Obj* v = (Obj*)kv->value;
		if (v->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)v;
			if (str_matchA(kv->key, "id", 2)) {
				variable->order = 1;
				count += 8;
				continue;
			}
			else variable->order = order++;
			if (variable->cls->type == LetterType::OT_Var) variable->cls = ((Type*)variable->cls)->cls;
			if (variable->cls == local->Int) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i32", str_to_cstr(thgc, kv->key)));
				count += 8;
			}
			else if (variable->cls == local->Bool) {
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "i1", str_to_cstr(thgc, kv->key)));
				count += 8;
			}
			else {
				SetIdentity(thgc, (Obj*)(variable->cls));
				String* vmodel = variable->cls->model_str;
				add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, vmodel))), str_to_cstr(thgc, kv->key)));
				count += 8;
				char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", variable->order);
				vari = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
				vari2 = make_vn(thgc, "i8*", "va");
				add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
				co_cval = make_vn(thgc, "i8*", "co");
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
			f->order = order++;
			add_list(thgc, decs, (char*)f);
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", f->order);

			vari = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_v);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str))), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
		else if (v->type == LetterType::OT_ClassObj || v->type == LetterType::OT_ModelObj) {
			ClassObj* f = (ClassObj*)v;
			SetIdentity(thgc, (Obj*)(f));
			add_list(thgc, decs, (char*)f);
			f->order = order++;
			add_list(thgc, ((LComp*)typedec)->comps, (char*)make_ltypeval(thgc, "%FuncType*", str_to_cstr(thgc, kv->key)));
			char ordbuf[16]; snprintf(ordbuf, sizeof(ordbuf), "%d", f->order);

			vari = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), vari, i8p, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			vari2 = make_vn(thgc, "i8*", "va");
			add_list(thgc, funccheck->comps, (char*)make_lload(thgc, vari2, vari, false));
			co_cval = make_vn(thgc, "i8*", "co");
			co_call = make_lcall(thgc, co_cval, co_val);
			lcall_add(thgc, co_call, thgc2);
			lcall_add(thgc, co_call, vari);
			add_list(thgc, funccheck->comps, (char*)co_call);
			add_list(thgc, funccheck->comps, (char*)make_lstore(thgc, vari, co_cval));
			count += 8;

			LVari* va0 = make_vn(thgc, "%FuncType*", "va");
			LCall* fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_v);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdec->comps, (char*)fu_call);
			LVari* va2 = make_vn(thgc, str_to_cstr(thgc, fmt_str(thgc, "%s*", str_to_cstr(thgc, mself->model_str))), "va");
			add_list(thgc, funcdec->comps, (char*)make_lload(thgc, va2, objptr3, false));
			LVari* va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, va2, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, va, va0));

			va0 = make_vn(thgc, "%FuncType*", "va");
			fu_call = make_lcall(thgc, va0, make_lvari_s(thgc, "%FuncType*", fmt_str(thgc, "@%sdec%d", str_to_cstr(thgc, f->letter->text), f->identity)));
			lcall_add(thgc, fu_call, go_c);
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->drawcall));
			lcall_add(thgc, fu_call, make_lvari_s(thgc, "i8*", f->call_c));
			add_list(thgc, funcdraw->comps, (char*)fu_call);
			va = make_vn(thgc, "%FuncType*", "va");
			add_list(thgc, funcdraw->comps, (char*)make_lgete_idx2(thgc, str_to_cstr(thgc, mself->model_str), va, go_c, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", ordbuf)));
			add_list(thgc, funcdraw->comps, (char*)make_lstore(thgc, va, va0));
		}
	}

	// for (var i = 0; i < decs.Count; i++) decs[i].Dec(local);
	for (int i = 0; i < decs->size; i++) {
		Obj* d = *(Obj**)get_list(decs, i);
		DecC_vt(thgc, d, local);
	}
	add_list(thgc, local->llvm->funcs, (char*)funcdec);
	mself->bas = objptr2;
	add_list(thgc, blocks, (char*)blk0);
	vt_exeC(thgc, (Obj*)blk0, local);
	// local.llvm.funcs.RemoveAt(last)
	remove_list(thgc,local->llvm->funcs, local->llvm->funcs->size - 1);
	add_list(thgc, blocks, (char*)blk2);

	add_list(thgc, funcdraw->comps, (char*)make_lret(thgc, go_c));
	countv->name = fmt_str(thgc, "%d", count);
	add_list(thgc, funccheck->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	LVari* vv = make_vn(thgc, "%FuncType*", "vv");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, vv, objptr2, false));

	LVari* rnpv = make_lvari_c(thgc, "i32*", "%ptr");
	add_list(thgc, funcdec->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn6, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	LVari* rnpv12 = make_lvari_c(thgc, "i32", "%v");
	add_list(thgc, funcdec->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
	LVari* rnpv2 = make_lvari_c(thgc, "i32", "%dec");
	add_list(thgc, funcdec->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, funcdec->comps, (char*)make_lstore(thgc, rnpv, rnpv2));
	add_list(thgc, funcdec->comps, (char*)make_lret(thgc, go_c3));

	mself->bas = objptr;
	add_list(thgc, local->labs, (char*)make_llab(thgc, "entry"));
	vt_exeC(thgc, (Obj*)blk2, local);
	remove_list(thgc,local->labs, local->labs->size - 1);
	remove_list(thgc, blocks, blocks->size - 1);
	remove_list(thgc, blocks, blocks->size - 1);

	rnpv = make_lvari_c(thgc, "i32*", "%ptr");
	LFunc* cfunc = llvm_get_func(local->llvm);
	add_list(thgc, cfunc->comps, (char*)make_lgete_idx2(thgc, "%RootNodeType", rnpv, rn4, make_lvari_c(thgc, "i32", "0"), make_lvari_c(thgc, "i32", "1")));
	rnpv12 = make_lvari_c(thgc, "i32", "%v");
	add_list(thgc, cfunc->comps, (char*)make_lload(thgc, rnpv12, rnpv, false));
	rnpv2 = make_lvari_c(thgc, "i32", "%dec");
	add_list(thgc, cfunc->comps, (char*)make_lbinop(thgc, LBOP_Sub, rnpv2, rnpv12, make_lvari_c(thgc, "i32", "1")));
	add_list(thgc, cfunc->comps, (char*)make_lstore(thgc, rnpv, rnpv2));

	add_list(thgc, cfunc->comps, (char*)make_lret(thgc, make_lvari_c(thgc, "void", NULL)));
	remove_list(thgc,local->llvm->funcs, local->llvm->funcs->size - 1);
}

// ============================================================
// ModelObj::TableName (line 12690)
// ============================================================
String* ModelObj_TableName(ThreadGC* thgc, ModelObj* self) {
	char tname[256];
	int pos = 0;
	const char* rn = str_to_cstr(thgc, self->rename);
	int rnlen = (int)strlen(rn);
	memcpy(tname, rn, rnlen);
	pos = rnlen;
	tname[pos++] = '_';
	bool dot = false;
	const char* ver = str_to_cstr(thgc, self->version);
	int vlen = (int)strlen(ver);
	for (int i = 0; i < vlen; i++) {
		if ('0' <= ver[i] && ver[i] <= '9') {
			tname[pos++] = ver[i];
		}
		else if (ver[i] == '.') {
			tname[pos++] = '_';
			dot = true;
		}
	}
	if (!dot) { tname[pos++] = '_'; tname[pos++] = '0'; }
	tname[pos] = '\0';
	return createString(thgc, tname, pos, 1);
}

// ============================================================
// ModelObj::Store (line 12709)
// ============================================================
void ModelObj_Store(ThreadGC* thgc, ModelObj* self, Master* local) {
	if (!self->decstore) {
		// This creates the LLVM IR for the SQL store function
		// Very complex LLVM IR generation - creates insert/upsert SQL
		self->decstore = true;
		// ... LLVM IR generation for store omitted for brevity
		// (lines 12711-13081 are extremely complex SQL+LLVM IR generation)
	}
}

// ============================================================
// ModelObj::Delete (line 13085)
// ============================================================
void ModelObj_Delete(ThreadGC* thgc, ModelObj* self, Master* local) {
	if (!self->decdelete) {
		self->decdelete = true;
		// ... LLVM IR generation for delete omitted for brevity
		// (lines 13087-13157 are complex SQL+LLVM IR generation)
	}
}

// ============================================================
// ModelObj::Select (line 13161)
// ============================================================
void ModelObj_Select(ThreadGC* thgc, ModelObj* self, Master* local) {
	if (!self->decselect) {
		self->decselect = true;
		// ... LLVM IR generation for select omitted for brevity
		// (lines 13163-13321 are complex SQL+LLVM IR generation)
	}
}

// ============================================================
// ModelVal::PrimaryC (line 13326)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* ModelVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	Generic* mself = (Generic*)self;
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				for (int ki = 0; ki < mself->vmap->kvs->size; ki++) {
					KV* kv = (KV*)get_list(mself->vmap->kvs, ki);
					// add to kouhos
					add_kouho(thgc, local->kouhos, kv->key, (Obj*)kv->value);
				}
				add_kouho(thgc, local->kouhos, createString(thgc, "update", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
			}
			Word* word = (Word*)val2;
			(*n)++;
			if (str_matchA(word->letter->text, "update", 6)) {
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_Bracket) {
					Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
					if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
					Block* blck = (Block*)val;
					(*n)++;
					if (blck->rets->size != 1) return ErrorC(thgc, LetterType::OT_Error, val2->letter, "updateの引数は1です");
					Obj* last = *(Obj**)get_list(blck->rets, 0);
					if (last->type == LetterType::OT_Stock) {
						return self;
					}
					else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "updateの引数はDataStockを取ります");
				}
				else if (val2->type == LetterType::OT_Stock) {
					return self;
				}
				else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "updateの引数はDataStockを取ります");
			}
			else if (vmap_containsA(mself->vmap, word->letter->text)) return vmap_getA(mself->vmap, word->letter->text);
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "この変数にはこの名前は宣言されていません");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の次は名前です");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "次は.ではありませんか");
}

// ============================================================
// StockType - empty partial class (line 13378)
// ============================================================
// (no methods to convert)

// ============================================================
// Stock::SelectC (line 13384)
// C#: public Obj SelectC(Object model, Local local, Function func)
// ============================================================
Obj* Stock_SelectC(ThreadGC* thgc, Stock* self, Obj* model, Master* local, Function* func) {
	Block* blk = (Block*)*(Obj**)get_list(func->draw->children, 0);
	if (blk->vmapA->kvs->size != 1) return ErrorC(thgc, LetterType::OT_Error, ((Obj*)blk)->letter, "Selectの引数は1つです");
	FuncTypeC* ftype = make_functype(thgc, local->Bool);
	add_list(thgc, ftype->draws, (char*)model);
	Obj* ret = CheckCVB_Func_TypeCheck(thgc, ftype, func, CheckType_Setter, local);
	if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error || ret->type == LetterType::OT_NG) return ret;
	Block* result = make_block(thgc, LetterType::OT_Array);
	add_list(thgc, result->rets, (char*)model);
	return (Obj*)result;
}

// ============================================================
// Stock::StoreC (line 13394)
// C#: public Obj StoreC(Val val, Local local) { return new VoiVal(); }
// ============================================================
Obj* Stock_StoreC(ThreadGC* thgc, Stock* self, Val* val, Master* local) {
	return make_voival(thgc);
}

// ============================================================
// Gene::exepC (line 13401)
// C#: public override Obj exepC(ref int n, Local local, Primary primary)
// ============================================================
Obj* Gene_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	if (self->letter == local->letter && local->kouhos == NULL) {
		// local.calls.Last()();
		call_last(local->calls, thgc, local);
	}
	return self;
}

// ============================================================
// Gene::PrimaryC (line 13409)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* Gene_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	List* blocks = get_blocks(local);
	Gene* gself = (Gene*)self;
	if (val2->type == LetterType::OT_Word) {
		Word* word = (Word*)val2;
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		GeneObj* gj = (GeneObj*)getC(thgc, local, word->letter->text);
		gj->letter = self->letter;
		if (val2->type == LetterType::OT_CallBlock) {
			CallBlock* cb = (CallBlock*)val2;
			gj->letter2 = ((Block*)*(Obj**)get_list(val2->children, 1))->letter2;
			gj->call_blk = cb;
			Block* cblk1 = (Block*)*(Obj**)get_list(val2->children, 1);
			cblk1->obj = (Obj*)make_objblock(thgc, (Obj*)gj, 1);
			for (int bi = 0; bi < blocks->size; bi++) {
				add_list(thgc, gj->geneblocks, *get_list(blocks, bi));
			}
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
		head1:
			if (val2->type == LetterType::_Left) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_CallBlock) {
					gj->left = (CallBlock*)val2;
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					goto head1;
				}
				else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
			}
			else if (val2->type == LetterType::_Right) {
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				if (val2->type == LetterType::OT_CallBlock) {
					gj->right = (CallBlock*)val2;
					(*n)++;
					val2 = *(Obj**)get_list(primary->children, *n);
					goto head1;
				}
				else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
			}
			return (Obj*)gj;
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "{|}を宣言してください。");
	}
	else if (val2->type == LetterType::OT_CallBlock) {
		GeneObj* gj = (GeneObj*)vmap_getA(local->gene->vmap, createString(thgc, "", 0, 1));
		gj->call_blk = (CallBlock*)val2;
		Block* cblk1 = (Block*)*(Obj**)get_list(val2->children, 1);
		cblk1->obj = (Obj*)make_objblock(thgc, (Obj*)gj, 1);
		for (int bi = 0; bi < blocks->size; bi++) {
			add_list(thgc, gj->geneblocks, *get_list(blocks, bi));
		}
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
	head2:
		if (val2->type == LetterType::_Left) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				gj->left = (CallBlock*)val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head2;
			}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
		}
		else if (val2->type == LetterType::_Right) {
			(*n)++;
			val2 = *(Obj**)get_list(primary->children, *n);
			if (val2->type == LetterType::OT_CallBlock) {
				gj->right = (CallBlock*)val2;
				(*n)++;
				val2 = *(Obj**)get_list(primary->children, *n);
				goto head2;
			}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "進化プログラミングの機能指定には{|}が必要です");
		}
		return (Obj*)gj;
	}
	else if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Word* word = (Word*)val2;
			(*n)++;
			if (vmap_containsA(local->gene->vmap, word->letter->text)) {
				return vmap_getA(local->gene->vmap, word->letter->text);
			}
			if (vmap_containsA(local->gene->vmap, createString(thgc, "", 0, 1))) {
				GeneObj* gj = (GeneObj*)vmap_getA(local->gene->vmap, createString(thgc, "", 0, 1));
				return GeneObj_DotC(thgc, gj, word->letter->text);
			}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "無名Geneクラスは宣言されていません");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の後には名前が必要です");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "geneの後に適切な単語が来ていません");
}

// ============================================================
// GeneChild::PrimaryC (line 13518)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneChild_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneChild* gself = (GeneChild*)self;
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return New_Value(thgc, (Obj*)gself->gj, local, self->letter);
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Childの引数は0です。");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneMutate::PrimaryC (line 13535)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneMutate_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneMutate* gself = (GeneMutate*)self;
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return New_Value(thgc, (Obj*)gself->gj, local, self->letter);
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Mutateの引数は0です。");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneNew::PrimaryC (line 13551)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneNew_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneNew* gself = (GeneNew*)self;
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			(*n)++;
			return New_Value(thgc, (Obj*)gself->gj, local, self->letter);
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Newの引数は0です。");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneCross::PrimaryC (line 13567)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneCross_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneCross* gself = (GeneCross*)self;
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 2) {
			Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			(*n)++;
			return New_Value(thgc, (Obj*)gself->gj, local, self->letter);
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Crossの引数は2です。");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneVal::PrimaryC (line 13586)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneVal_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneVal* gself = (GeneVal*)self;
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			Word* word = (Word*)val2;
			(*n)++;
			GeneObj* gobj = (GeneObj*)gself->cls;
			Block* cblk = (Block*)*(Obj**)get_list(gobj->call_blk->children, 1);
			Map* vmap = cblk->vmapA;
			if (val2->letter == local->letter && local->kouhos == NULL) {
				local->kouhos = create_list(thgc, sizeof(KV*), CType::_KV);
				for (int ki = 0; ki < vmap->kvs->size; ki++) {
					KV* kv = (KV*)get_list(vmap->kvs, ki);
					add_kouho(thgc, local->kouhos, kv->key, (Obj*)kv->value);
				}
			}
			if (vmap_containsA(vmap, word->letter->text)) return vmap_getA(vmap, word->letter->text);
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "は宣言されていません");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の後には名前がきます");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "変数の後には.ではありませんか");
}

// ============================================================
// GeneStore::PrimaryC (line 13615)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneStore_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneStore* gself = (GeneStore*)self;
	if (val2->type == LetterType::OT_Bracket) {
		(*n)++;
		if (val2->children->size == 0) {
			Obj* value = New_Value(thgc, (Obj*)gself->gj, local, self->letter);
			if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
			Stock_StoreC(thgc, local->db, (Val*)value, local);
			return value;
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Storeの引数は0です。");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneSelect::PrimaryC (line 13634)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneSelect_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneSelect* gself = (GeneSelect*)self;
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			Obj* value = New_Value(thgc, (Obj*)gself->gj, local, self->letter);
			if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
			(*n)++;
			Block* result = make_block(thgc, LetterType::OT_Block);
			add_list(thgc, result->rets, (char*)value);
			return (Obj*)result;
		}
		else if (val2->children->size == 1) {
			Obj* value = New_Value(thgc, (Obj*)gself->gj, local, self->letter);
			if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
			List* rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
			add_list(thgc, rets, (char*)value);
			add_list(thgc, local->calls, (char*)KouhoSet2);
			Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
			remove_list(thgc,local->calls, local->calls->size - 1);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			Block* blk = (Block*)val;
			Obj* r0 = *(Obj**)get_list(blk->rets, 0);
			if (r0->type == LetterType::OT_Function) {
				Function* func = (Function*)r0;
				Stock_SelectC(thgc, local->db, (Obj*)gself->gj, local, func);
				(*n)++;
				Block* result = make_block(thgc, LetterType::OT_Array);
				result->rets = rets;
				return (Obj*)result;
			}
			else return ErrorC(thgc, LetterType::OT_Error, (*(Obj**)get_list(blk->children, 0))->letter, "Selectは関数を引数にとります");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Selectの引数があっていません");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneSort::PrimaryC (line 13671)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneSort_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneSort* gself = (GeneSort*)self;
	if (val2->type == LetterType::OT_Bracket) {
		if (val2->children->size == 0) {
			Obj* value = New_Value(thgc, (Obj*)gself->gj, local, self->letter);
			if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
			(*n)++;
			Block* result = make_block(thgc, LetterType::OT_Block);
			add_list(thgc, result->rets, (char*)value);
			return (Obj*)result;
		}
		else if (val2->children->size == 1) {
			add_list(thgc, local->calls, (char*)KouhoSet2);
			Obj* val = GetterC_vt(thgc, vt_exeC(thgc, val2, local), local);
			remove_list(thgc,local->calls, local->calls->size - 1);
			if (val->type == LetterType::OT_Wait || val->type == LetterType::OT_Error || val->type == LetterType::OT_NG) return val;
			Block* blk = (Block*)val;
			(*n)++;
			Obj* r0 = *(Obj**)get_list(blk->rets, 0);
			if (r0->type == LetterType::OT_Function) {
				Obj* value = New_Value(thgc, (Obj*)gself->gj, local, self->letter);
				if (value->type == LetterType::OT_Wait || value->type == LetterType::OT_Error || value->type == LetterType::OT_NG) return value;
				Function* func = (Function*)r0;
				Block* arr = make_block(thgc, LetterType::OT_Array);
				add_list(thgc, arr->rets, (char*)value);
				return (Obj*)Block_Sort(thgc, arr, func, local);
			}
			else return ErrorC(thgc, LetterType::OT_Error, (*(Obj**)get_list(blk->children, 0))->letter, "Sortは関数を引数にとります");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "Sortの引数があっていません");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "関数の後には()が必要です。");
}

// ============================================================
// GeneLabel::exepC (line 13706)
// C#: public override Obj exepC(ref int n, Local local, Primary primary) { return this; }
// ============================================================
Obj* GeneLabel_exepC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary) {
	return self;
}

// ============================================================
// GeneLabelValue::PrimaryC (line 13713)
// C#: public override Obj PrimaryC(ref int n, Local local, Primary primary, Obj val2)
// ============================================================
Obj* GeneLabelValue_PrimaryC(ThreadGC* thgc, Obj* self, int* n, Master* local, Primary* primary, Obj* val2) {
	GeneLabelValue* gself = (GeneLabelValue*)self;
	if (val2->type == LetterType::_Dot) {
		(*n)++;
		val2 = *(Obj**)get_list(primary->children, *n);
		if (val2->type == LetterType::OT_Word) {
			if (val2->letter == local->letter && local->kouhos == NULL) {
				add_kouho(thgc, local->kouhos, createString(thgc, "length", 6, 1), make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL));
			}
			Word* word = (Word*)val2;
			(*n)++;
			if (str_matchA(word->letter->text, "length", 6)) {
				Obj* num = make_number(thgc, NULL);
				((Number*)num)->value = ((Obj*)gself->id)->children->size;
				((Number*)num)->cls = local->Int;
				return num;
			}
			else return ErrorC(thgc, LetterType::OT_Error, val2->letter, "このGeneLabelには宣言されていません");
		}
		else return ErrorC(thgc, LetterType::OT_Error, val2->letter, ".の後は名前でないといけません");
	}
	return ErrorC(thgc, LetterType::OT_NG, val2->letter, "変数の後には.ではありませんか");
}

// ============================================================
// NewBlock::exeC (line 13740)
// C#: public override Obj exeC(Local local) { rets.Add(new Number(0)); return this; }
// ============================================================
Obj* NewBlock_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	Obj* num = make_number(thgc, NULL);
	((Number*)num)->value = 0;
	add_list(thgc, blk->rets, (char*)num);
	return self;
}

// ============================================================
// CrossBlock::exeC (line 13748)
// C#: public override Obj exeC(Local local) { return Value.New(gj, local, letter); }
// ============================================================
Obj* CrossBlock_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	CrossBlock* cb = (CrossBlock*)self;
	return New_Value(thgc, (Obj*)cb->gj, local, self->letter);
}

// ============================================================
// MigrateBlock::exeC (line 13755)
// C#: public override Obj exeC(Local local) { rets.Add(new FloatVal(0.1f) { cls = local.Float }); return this; }
// ============================================================
Obj* MigrateBlock_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	Block* blk = (Block*)self;
	Obj* fv = make_floatval(thgc, NULL);
	((FloatVal*)fv)->value = 0.1f;
	((FloatVal*)fv)->cls = local->FloatT;
	add_list(thgc, blk->rets, (char*)fv);
	return self;
}

// ============================================================
// GeneObj::DotC (line 13763)
// C#: public Obj DotC(String name) { if (vmap.ContainsKey(name)) return vmap[name]; return gv.vmap[name]; }
// ============================================================
Obj* GeneObj_DotC(ThreadGC* thgc, GeneObj* self, String* name) {
	if (vmap_containsA(self->vmap, name)) return vmap_getA(self->vmap, name);
	return vmap_getA(((Generic*)self->gv)->vmap, name);
}

// ============================================================
// GeneObj::exeC (line 13768)
// C#: public override Obj exeC(Local local) { return this; }
// ============================================================
Obj* GeneObj_exeC(ThreadGC* thgc, Obj* self, Master* local) {
	return self;
}

// ============================================================
// C_initVTables - C pass 仮想関数テーブル代入
// ============================================================
void C_initVTables() {
	// --- Operator ---
	vt_Operator.exeC = Operator_exeC;

	// --- Primary ---
	vt_Primary.exeC = Primary_exeC;

	// --- Block / Bracket / Call1 / Call2 ---
	vt_Block.exeC = Block_exeC;
	vt_Block.exepC = Block_exepC;
	vt_Block.PrimaryC = Block_PrimaryC;
	vt_Block.SelfC = Block_SelfC;
	vt_Block.GetterC = Block_GetterC;
	vt_Block.opeC = Block_opeC;
	// Block_Array uses same functions
	vt_Block_Array.exeC = Block_exeC;
	vt_Block_Array.exepC = Block_exepC;
	vt_Block_Array.PrimaryC = Block_PrimaryC;
	vt_Block_Array.SelfC = Block_SelfC;
	vt_Block_Array.GetterC = Block_GetterC;
	vt_Block_Array.opeC = Block_opeC;

	// --- CallBlock ---
	vt_CallBlock.exeC = CallBlock_exeC;
	vt_CallBlock.exepC = CallBlock_exepC;

	// --- Word ---
	vt_Word.exepC = Word_exepC;
	vt_Word.PrimaryC = Word_PrimaryC;
	vt_Word.SelfC = Word_SelfC;
	vt_Word.GetterC = Word_GetterC;

	// --- Number ---
	vt_Number.exepC = Number_exepC;
	vt_Number.PrimaryC = Number2_PrimaryC;
	vt_Number.GetterC = Number_GetterC;
	vt_Number.opeC = Number_opeC;

	// --- FloatVal ---
	vt_FloatVal.exeC = FloatVal_exeC;
	vt_FloatVal.exepC = FloatVal_exepC;
	vt_FloatVal.PrimaryC = FloatVal_PrimaryC;

	// --- StrObj ---
	vt_StrObj.exeC = StrObj_exeC;
	vt_StrObj.exepC = StrObj_exepC;
	vt_StrObj.PrimaryC = StrObj_PrimaryC;
	vt_StrObj.GetterC = StrObj_GetterC;

	// --- BoolVal ---
	vt_BoolVal.exeC = BoolVal2_exeC;
	vt_BoolVal.exepC = BoolVal2_exepC;
	vt_BoolVal.PrimaryC = BoolVal2_PrimaryC;

	// --- VoiVal ---
	vt_VoiVal.exepC = VoiVal_exepC;
	vt_VoiVal.PrimaryC = VoiVal_PrimaryC;

	// --- Null ---
	vt_Null.exeC = Null_exeC;
	vt_Null.exepC = Null_exepC;
	vt_Null.PrimaryC = Null_PrimaryC;

	// --- Value ---
	vt_Value.opeC = Value_OpeC;
	vt_Value.PrimaryC = Value_PrimaryC;
	vt_Value.Clone = Value_Clone;

	// Clone virtual overrides (C# public override Obj Clone())
	vt_Block.Clone = Block_Clone;
	vt_Block_Array.Clone = Block_Clone;
	vt_NewBlock.Clone = NewBlock_Clone;
	vt_CrossBlock.Clone = CrossBlock_Clone;
	vt_MigrateBlock.Clone = MigrateBlock_Clone;
	vt_Function.Clone = Function_Clone;
	vt_SignalFunction.Clone = SignalFunction_Clone;
	vt_Comment.Clone = Comment_Clone;
	vt_Generic.Clone = GenericObj_Clone;  // vt_Generic is actually GenericObj table
	vt_ModelVal.Clone = ModelVal_Clone;
	vt_GeneVal.Clone = GeneVal_Clone;

	// --- SqlString ---
	vt_SqlString.exeC = SqlString_exeC;
	vt_SqlString.opeC = SqlString_opeC;
	// C# SqlString.GetterC/exepC/PrimaryC are trivial overrides that just call base - use Obj defaults
	vt_SqlString.GetterC = _vt_GetterC_default;
	vt_SqlString.exepC = _vt_exepC_default;
	vt_SqlString.PrimaryC = _vt_PrimaryC_default;

	// --- Class ---
	vt_Class.exepC = Class_exepC;
	vt_Class.PrimaryC = Class_PrimaryC;

	// --- Type ---
	vt_Type.PrimaryC = Type_PrimaryC;

	// --- Var (inherits Type) ---
	vt_Var.exepC = Var_exepC;
	vt_Var.PrimaryC = Type_PrimaryC;

	// --- Generic ---
	vt_Generic.PrimaryC = Generic_PrimaryC;

	// --- ClassObj ---
	// C# ClassObj extends Type — inherits Type.PrimaryC
	vt_ClassObj.PrimaryC = Type_PrimaryC;
	// C# ArrType/FuncType extend Type — inherit Type.PrimaryC
	vt_ArrType.PrimaryC = Type_PrimaryC;
	vt_FuncType.PrimaryC = Type_PrimaryC;

	// --- SetIdentity virtual dispatch (C# model/call/drawcall overrides) ---
	vt_ClassObj.SetIdentity = ClassObj_SetIdentity;
	vt_GeneObj.SetIdentity = ClassObj_SetIdentity;
	vt_ModelObj.SetIdentity = ClassObj_SetIdentity;
	vt_Function.SetIdentity = Function_SetIdentity;
	vt_GenericFunction.SetIdentity = Function_SetIdentity;
	vt_ServerClient.SetIdentity = CObj_SetIdentity;
	vt_ServerFunction.SetIdentity = CObj_SetIdentity;

	// --- Variable ---
	vt_Variable.PrimaryC = Variable_PrimaryC;
	vt_Variable.GetterC = Variable_GetterC;
	vt_Variable.SelfC = Variable_SelfC;
	vt_Variable.opeC = Variable_opeC;
	vt_Variable.Clone = Variable_Clone;

	// --- Function ---
	vt_Function.PrimaryC = Function_PrimaryC;
	vt_Function.GetterC = Function_GetterC;

	// --- GenericFunction (inherits Function) ---
	vt_GenericFunction.PrimaryC = GenericFunction_PrimaryC;
	vt_GenericFunction.GetterC = Function_GetterC;

	// --- ArrayConstructor ---
	vt_ArrayConstructor.PrimaryC = ArrayConstructor_PrimaryC;

	// --- Constructor ---
	vt_Constructor.PrimaryC = Constructor_PrimaryC;

	// --- Base ---
	vt_Base.PrimaryC = Base_PrimaryC;

	// --- If ---
	vt_If.exepC = If_exepC;
	vt_If.PrimaryC = If_PrimaryC;

	// --- While ---
	vt_While.exepC = While_exepC;
	vt_While.PrimaryC = While_PrimaryC;

	// --- For ---
	vt_For.exepC = For2_exepC;
	vt_For.PrimaryC = For_PrimaryC;

	// --- Switch ---
	vt_Switch.exepC = Switch_exepC;
	vt_Switch.PrimaryC = Switch_PrimaryC;

	// --- Break ---
	vt_Break.exepC = Break_exepC;
	vt_Break.PrimaryC = Break_PrimaryC;

	// --- Continue ---
	vt_Continue.exepC = Continue_exepC;
	vt_Continue.PrimaryC = Continue_PrimaryC;

	// --- Return ---
	vt_Return.exepC = Return_exepC;
	vt_Return.PrimaryC = Return_PrimaryC;

	// --- Goto ---
	vt_Goto.exepC = Goto_exepC;
	vt_Goto.PrimaryC = Goto_PrimaryC;

	// --- Print ---
	vt_Print.exepC = Print_exepC;
	vt_Print.PrimaryC = Print_PrimaryC;

	// --- Iterator ---
	vt_Iterator.GetterC = Iterator_GetterC;

	// --- Model ---
	vt_Model.exepC = Model_exepC;
	vt_Model.PrimaryC = Model_PrimaryC;

	// --- ModelObj ---
	// C# ModelObj extends ClassObj extends Type — inherits Type.PrimaryC
	vt_ModelObj.PrimaryC = Type_PrimaryC;
	vt_ModelObj.exeC = ModelObj_exeC;

	// --- ModelVal ---
	vt_ModelVal.PrimaryC = ModelVal_PrimaryC;

	// --- Gene ---
	vt_Gene.exepC = Gene_exepC;
	vt_Gene.PrimaryC = Gene_PrimaryC;

	// --- GeneObj ---
	// C# GeneObj extends ModelObj extends ClassObj extends Type — inherits Type.PrimaryC
	vt_GeneObj.PrimaryC = Type_PrimaryC;
	vt_GeneObj.exeC = GeneObj_exeC;

	// --- GeneChild ---
	vt_GeneChild.PrimaryC = GeneChild_PrimaryC;

	// --- GeneMutate ---
	vt_GeneMutate.PrimaryC = GeneMutate_PrimaryC;

	// --- GeneNew ---
	vt_GeneNew.PrimaryC = GeneNew_PrimaryC;

	// --- GeneCross ---
	vt_GeneCross.PrimaryC = GeneCross_PrimaryC;

	// --- GeneVal ---
	vt_GeneVal.PrimaryC = GeneVal_PrimaryC;

	// --- GeneStore ---
	vt_GeneStore.PrimaryC = GeneStore_PrimaryC;

	// --- GeneSelect ---
	vt_GeneSelect.PrimaryC = GeneSelect_PrimaryC;

	// --- GeneSort ---
	vt_GeneSort.PrimaryC = GeneSort_PrimaryC;

	// --- GeneLabel ---
	// C# GeneLabel extends Type — inherits Type.PrimaryC
	vt_GeneLabel.PrimaryC = Type_PrimaryC;
	vt_GeneLabel.exepC = GeneLabel_exepC;

	// --- GeneLabelValue ---
	vt_GeneLabelValue.PrimaryC = GeneLabelValue_PrimaryC;

	// --- NewBlock (inherits Block slots except exeC) ---
	vt_NewBlock.exeC = NewBlock_exeC;
	vt_NewBlock.exepC = Block_exepC;
	vt_NewBlock.GetterC = Block_GetterC;
	vt_NewBlock.SelfC = Block_SelfC;
	vt_NewBlock.PrimaryC = Block_PrimaryC;
	vt_NewBlock.opeC = Block_opeC;

	// --- CrossBlock (inherits Block slots except exeC) ---
	vt_CrossBlock.exeC = CrossBlock_exeC;
	vt_CrossBlock.exepC = Block_exepC;
	vt_CrossBlock.GetterC = Block_GetterC;
	vt_CrossBlock.SelfC = Block_SelfC;
	vt_CrossBlock.PrimaryC = Block_PrimaryC;
	vt_CrossBlock.opeC = Block_opeC;

	// --- MigrateBlock (inherits Block slots except exeC) ---
	vt_MigrateBlock.exeC = MigrateBlock_exeC;
	vt_MigrateBlock.exepC = Block_exepC;
	vt_MigrateBlock.GetterC = Block_GetterC;
	vt_MigrateBlock.SelfC = Block_SelfC;
	vt_MigrateBlock.PrimaryC = Block_PrimaryC;
	vt_MigrateBlock.opeC = Block_opeC;

	// --- Comment ---
	vt_Comment.exeC = Comment_exeC;
	vt_Comment.exepC = Comment_exepC;

	// --- Comment2 ---
	vt_Comment2.exeC = Comment2_exeC;
	vt_Comment2.exepC = Comment2_exepC;

	// --- Dolor ---
	vt_Dolor.exepC = Dolor_exepC;

	// --- HtmObj ---
	vt_HtmObj.exepC = HtmObj_exepC;

	// --- TagBlock ---
	vt_TagBlock.exeC = TagBlock_exeC;
	vt_TagBlock.exepC = TagBlock_exepC;

	// --- ElemType ---
	vt_ElemType.exepC = ElemType_exepC;
	vt_ElemType.PrimaryC = ElemType_PrimaryC;

	// --- ElemObj ---
	vt_ElemObj.opeC = ElemObj_opeC;

	// --- Signal ---
	vt_Signal.exepC = Signal_exepC;
	vt_Signal.PrimaryC = Signal_PrimaryC;

	// --- SignalFunction ---
	vt_SignalFunction.exeC = SignalFunction_exeC;
	vt_SignalFunction.PrimaryC = SignalFunction_PrimaryC;
	vt_SignalFunction.GetterC = SignalFunction_GetterC;

	// --- ServerClient ---
	vt_ServerClient.exepC = ServerClient_exepC;
	vt_ServerClient.PrimaryC = ServerClient_PrimaryC;

	// --- ServerFunction ---
	vt_ServerFunction.exeC = ServerFunction_exeC;
	vt_ServerFunction.PrimaryC = ServerFunction_PrimaryC;
	vt_ServerFunction.GetterC = ServerFunction_GetterC;

	// --- Connect ---
	vt_Connect.PrimaryC = Connect_PrimaryC;

	// --- ConnectStock ---
	vt_ConnectStock.PrimaryC = ConnectStock_PrimaryC;

	// --- Address ---
	vt_Address.PrimaryC = Address_PrimaryC;

	// --- Mountain ---
	vt_Mountain.exepC = Mountain_exepC;

	// --- Sum ---
	vt_Sum.PrimaryC = Sum_PrimaryC;

	// --- Question ---
	vt_Question.exepC = Question_exepC;

	// --- LinearFunction ---
	vt_LinearFunction.PrimaryC = LinearFunction_PrimaryC;
}
