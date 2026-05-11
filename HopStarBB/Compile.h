
// C# Letter always has name/text set. C++ operator ATSSpans had text=NULL,
// causing crashes in Operator_exeC etc. Auto-generate text from LetterType.
static String* _span_text_from_type(ThreadGC* thgc, LetterType t) {
	switch (t) {
	case LetterType::_Equal:      return createString(thgc, (char*)"=", 1, 1);
	case LetterType::_EqualEqual: return createString(thgc, (char*)"==", 2, 1);
	case LetterType::_NotEqual:   return createString(thgc, (char*)"!=", 2, 1);
	case LetterType::_Not:        return createString(thgc, (char*)"!", 1, 1);
	case LetterType::_Plus:       return createString(thgc, (char*)"+", 1, 1);
	case LetterType::_Minus:      return createString(thgc, (char*)"-", 1, 1);
	case LetterType::_Mul:        return createString(thgc, (char*)"*", 1, 1);
	case LetterType::_Div:        return createString(thgc, (char*)"/", 1, 1);
	case LetterType::_LessThan:   return createString(thgc, (char*)"<", 1, 1);
	case LetterType::_MoreThan:   return createString(thgc, (char*)">", 1, 1);
	case LetterType::_LessEqual:  return createString(thgc, (char*)"<=", 2, 1);
	case LetterType::_MoreEqual:  return createString(thgc, (char*)">=", 2, 1);
	case LetterType::_Colon:      return createString(thgc, (char*)":", 1, 1);
	case LetterType::_Percent:    return createString(thgc, (char*)"%", 1, 1);
	case LetterType::_Dot:        return createString(thgc, (char*)".", 1, 1);
	case LetterType::_Comma:      return createString(thgc, (char*)",", 1, 1);
	case LetterType::_Semicolon:  return createString(thgc, (char*)";", 1, 1);
	case LetterType::_Left:       return createString(thgc, (char*)"<-", 2, 1);
	case LetterType::_Right:      return createString(thgc, (char*)"->", 2, 1);
	case LetterType::_Mountain:   return createString(thgc, (char*)"^", 1, 1);
	case LetterType::_Question:   return createString(thgc, (char*)"?", 1, 1);
	case LetterType::_Sharp:      return createString(thgc, (char*)"#", 1, 1);
	case LetterType::_Bou:        return createString(thgc, (char*)"|", 1, 1);
	case LetterType::_Dolor:      return createString(thgc, (char*)"$", 1, 1);
	default: return NULL;
	}
}
ATSSpan* make_span(ThreadGC* thgc, FontId font, LetterType t, int start, int end, uint32_t color, String* text, NewElement* letter = nullptr) {
	ATSSpan* s = (ATSSpan*)GC_alloc(thgc, CType::_ATSSpan);
	s->color = color;
	s->font = font;
	s->t = t;
	s->start = start;
	s->end = end;
	s->text = text ? text : _span_text_from_type(thgc, t);
	s->error = NULL;
	s->underlineColor = 0;
	s->letter = letter;
	return s;
}

List* Compile(ThreadGC* thgc, String* str, FontId font, NewElement* letter = nullptr) {
	List* list = create_list(thgc, sizeof(List*), CType::_List);
	List* list0 = create_list(thgc, sizeof(ATSSpan*), CType::_ATSSpan);
	add_list(thgc, list, (char*)list0);
	int len = str->size;
	// 旧仕様: スパンを「行頭からの相対座標 (i - n)」で持っていた。
	// 新仕様: 全スパンを絶対座標 (i, j) で保持。SetText 側で行頭基点を引いて letter-相対化する。
	// → SubString(text, m, len) の m を別追跡しなくて済み、\n マーカーとの座標系不一致も無くなる。
	for (int i = 0; i < len; i++) {
		wchar_t c = GetChar(str, i);
		if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_') {
			int j = i + 1;
			for (; j < len; j++) {
				c = GetChar(str, j);
				if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_') continue;
				break;
			}
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Name, i, j, 0x000000FF, SubString(thgc, str, i, j - i), letter));
			i = j - 1;
		}
		else if ('0' <= c && c <= '9') {
			int j = i + 1;
			bool flv = false;
			for (; j < len; j++) {
				c = GetChar(str, j);
				if ('0' <= c && c <= '9') continue;
				else if (c == '.') {
					if (flv) break;
					j++;
					if (j < len && '0' <= GetChar(str, j) && GetChar(str, j) <= '9') {
						flv = true;
						continue;
					}
					else {
						j--;
						break;
					}
				}
				break;
			}
			if (flv) add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Decimal, i, j, 0x000000FF, SubString(thgc, str, i, j - i), letter));
			else add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Number, i, j, 0x000000FF, SubString(thgc, str, i, j - i), letter));
			i = j - 1;
		}
		else if (c == '"') {
			int j = i + 1;
			for (; ; j++) {
				if (j >= len) {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Str, i, j, 0x8B4513FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					break;
				}
				c = GetChar(str, j);
				if (c == '"') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Str, i, j + 1, 0x8B4513FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					j++;
					break;
				}
				else if (c == '\n' || c == '\0') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Str, i, j, 0x8B4513FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					break;
				}
			}
			i = j - 1;
		}
		else if (c == '&') {
			int j = i + 1;
			for (; j < len; j++) {
				c = GetChar(str, j);
				if (c == '\n' || c == '\0') break;
			}
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_And, i, j, 0x8B4513FF, SubString(thgc, str, i, j - i), letter));
			i = j - 1;
		}
		else if (c == '^') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Mountain, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '`') {
			int j = i + 1;
			for (; ; j++) {
				if (j >= len) {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_HLetter, i, j, 0x000000FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					break;
				}
				c = GetChar(str, j);
				if (c == '`') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_HLetter, i, j + 1, 0x000000FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					j++;
					break;
				}
				else if (c == '<' || c == '>' || c == '~' || c == '$') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_HLetter, i, j, 0x000000FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					break;
				}
				else if (c == '\n' || c == '\0') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_HLetter, i, j, 0x000000FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					break;
				}
			}
			i = j - 1;
		}
		else if (c == ' ') {
			int j = i + 1;
			for (; j < len; j++) {
				if (GetChar(str, j) != ' ') break;
			}
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Space, i, j, 0x000000FF, SubString(thgc, str, i, j - i), letter));
			i = j - 1;
		}
		else if (c == '\t') {
			int j = i + 1;
			for (; j < len; j++) {
				if (GetChar(str, j) != '\t') break;
			}
			// LetterType::_Tab で span 化。text には元の \t 文字をそのまま保持 (= 保存時は \t に戻る)。
			// 描画側で _Tab を見て「タブ 1 個 = スペース 4 個分の幅」で advance させる。
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Tab, i, j, 0x000000FF, SubString(thgc, str, i, j - i), letter));
			i = j - 1;
		}
		else if (c == ':') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Colon, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == ';') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Semicolon, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '$') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Dolor, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == ',') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Comma, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '|') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Bou, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '#') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Sharp, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '%') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Percent, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '@') {
			if (i + 1 < len) {
				wchar_t c2 = GetChar(str, i + 1);
				if (('a' <= c2 && c2 <= 'z') || ('A' <= c2 && c2 <= 'Z')) {
					int j = i + 2;
					for (; j < len; j++) {
						c2 = GetChar(str, j);
						if (('a' <= c2 && c2 <= 'z') || ('A' <= c2 && c2 <= 'Z')) continue;
						break;
					}
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_AtLetter, i, j, 0x000000FF, SubString(thgc, str, i, j - i), letter));
					i = j - 1;
				}
			}
		}
		else if (c == '\n') {
			list0 = create_list(thgc, sizeof(ATSSpan*), CType::_ATSSpan);
			add_list(thgc, list, (char*)list0);
			// (旧 n = i + 1; は廃止。スパンは全部絶対座標になったので不要)
		}
		else if (c == '=') {
			if (i + 1 < len && GetChar(str, i + 1) == '=') {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_EqualEqual, i, i + 2, 0x000000FF, NULL, letter));
				i++;
			}
			else {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Equal, i, i + 1, 0x000000FF, NULL, letter));
			}
		}
		else if (c == '!') {
			if (i + 1 < len && GetChar(str, i + 1) == '=') {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_NotEqual, i, i + 2, 0x000000FF, NULL, letter));
				i++;
			}
			else {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Not, i, i + 1, 0x000000FF, NULL, letter));
			}
		}
		else if (c == '+') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Plus, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '-') {
			if (i + 1 < len) {
				wchar_t c2 = GetChar(str, i + 1);
				if (c2 == '>') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Right, i, i + 2, 0x000000FF, NULL, letter));
					i++;
				}
				else if (c2 == '-') {
					i++;
					int j = i + 1;
					for (; j < len; j++) {
						c2 = GetChar(str, j);
						if (c2 == '\n' || c2 == '\0') break;
					}
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_CommentSingle, i - 1, j, 0x008000FF, SubString(thgc, str, i + 1, j - i - 1), letter));
					i = j - 1;
				}
				else {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Minus, i, i + 1, 0x000000FF, NULL, letter));
				}
			}
			else {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Minus, i, i + 1, 0x000000FF, NULL, letter));
			}
		}
		else if (c == '<') {
			if (i + 1 < len) {
				wchar_t c2 = GetChar(str, i + 1);
				if (c2 == '-') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Left, i, i + 2, 0x000000FF, NULL, letter));
					i++;
				}
				else if (c2 == '=') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_LessEqual, i, i + 2, 0x000000FF, NULL, letter));
					i++;
				}
				else if (c2 == '&') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_StringTag, i, i + 2, 0x000000FF, NULL, letter));
					i++;
				}
				else {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_LessThan, i, i + 1, 0x000000FF, NULL, letter));
				}
			}
			else {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_LessThan, i, i + 1, 0x000000FF, NULL, letter));
			}
		}
		else if (c == '>') {
			if (i + 1 < len) {
				wchar_t c2 = GetChar(str, i + 1);
				if (c2 == '=') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_MoreEqual, i, i + 2, 0x000000FF, NULL, letter));
					i++;
				}
				else if (c2 == '>') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_RightRight, i, i + 2, 0x000000FF, NULL, letter));
					i++;
				}
				else {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_MoreThan, i, i + 1, 0x000000FF, NULL, letter));
				}
			}
			else {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_MoreThan, i, i + 1, 0x000000FF, NULL, letter));
			}
		}
		else if (c == '*') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Mul, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '/') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Div, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '~') {
			if (i + 1 < len && GetChar(str, i + 1) == '~') {
				i++;
				if (i + 1 < len && GetChar(str, i + 1) == '~') {
					i++;
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_NyoroNyoroNyoro, i - 2, i + 1, 0x000000FF, NULL, letter));
				}
				else {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_NyoroNyoro, i - 1, i + 1, 0x000000FF, NULL, letter));
				}
			}
			else {
				add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Nyoro, i, i + 1, 0x000000FF, NULL, letter));
			}
		}
		else if (c == '.') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Dot, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '(') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_BracketL, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == ')') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_BracketR, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '[') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_BlockL, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == ']') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_BlockR, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '{') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_BraceL, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '}') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_BraceR, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c == '\0') {
			break;
		}
		else if (c == '?') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Question, i, i + 1, 0x000000FF, NULL, letter));
		}
		else if (c >= 256) {
			int j = i + 1;
			for (; j < len; j++) {
				if (GetChar(str, j) < 256) break;
			}
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_CommentSingle, i, j, 0x000000FF, SubString(thgc, str, i, j - i), letter));
			i = j - 1;
		}
	}
	return list;
}

// ============================================================
// パーサー構造体定義
// ============================================================

// 前方宣言
struct Obj;
struct Block;
struct Primary;
struct Master;

// LLVM型の前方宣言 (llvm.hで定義)
struct LVari;
struct LLab;
struct LFunc;
struct LLLVM;
struct LTypeDec;
struct LStrV;
struct LComp;
struct LDebugVariable;
struct LDebugInfo;

// 仮想関数テーブル (C#のvirtual/overrideに対応)
struct ObjVT {
	Obj* (*exeZ)(ThreadGC*, Obj*, Master*);
	Obj* (*exepZ)(ThreadGC*, Obj*, int*, Master*, Primary*);
	Obj* (*GetterZ)(ThreadGC*, Obj*, Master*);
	Obj* (*SelfZ)(ThreadGC*, Obj*, Master*);
	Obj* (*opeZ)(ThreadGC*, Obj*, LetterType, Master*, Obj*);
	Obj* (*PrimaryZ)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*);
	Obj* (*exeA)(ThreadGC*, Obj*, Master*);
	Obj* (*exepA)(ThreadGC*, Obj*, int*, Master*, Primary*);
	Obj* (*GetterA)(ThreadGC*, Obj*, Master*);
	Obj* (*SelfA)(ThreadGC*, Obj*, Master*);
	Obj* (*opeA)(ThreadGC*, Obj*, LetterType, Master*, Obj*);
	Obj* (*PrimaryA)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*);
	Obj* (*exeB)(ThreadGC*, Obj*, Master*);
	Obj* (*exepB)(ThreadGC*, Obj*, int*, Master*, Primary*);
	Obj* (*GetterB)(ThreadGC*, Obj*, Master*);
	Obj* (*SelfB)(ThreadGC*, Obj*, Master*);
	Obj* (*opeB)(ThreadGC*, Obj*, LetterType, Master*, Obj*);
	Obj* (*PrimaryB)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*);
	Obj* (*exeC)(ThreadGC*, Obj*, Master*);
	Obj* (*exepC)(ThreadGC*, Obj*, int*, Master*, Primary*);
	Obj* (*GetterC)(ThreadGC*, Obj*, Master*);
	Obj* (*SelfC)(ThreadGC*, Obj*, Master*);
	Obj* (*opeC)(ThreadGC*, Obj*, LetterType, Master*, Obj*);
	Obj* (*PrimaryC)(ThreadGC*, Obj*, int*, Master*, Primary*, Obj*);
	void (*SetIdentity)(ThreadGC*, Obj*);  // C# virtual model/call/drawcall refresh
	Obj* (*Clone)(ThreadGC*, Obj*);        // C# virtual Obj Clone()
};

// Primary用VTableの前方宣言 (objvt.hで定義)
extern ObjVT vt_Primary;

struct Obj {
	LetterType type;     // C# type
	ATSSpan* letter;        // C# letter
	List* children;         // C# children
	ObjVT* vt;              // C++ vtable dispatch
	String* rename;         // C# Obj1Z.cs
	String* version;        // C# Obj1Z.cs
	// C pass fields (C# Obj1C.cs)
	String* model;          // C# _model
	LVari* forvari;         // C# forvari
	// ret for While/For/Switch is stored in 'value' field (set in PrimaryB)
	int n_c;                // C# n
	int order;              // C# order
	LVari* vari_c;          // C# vari
	LVari* bas;             // C# bas
	LLab* entry;            // C# entry
	LLab* end_lab;          // C# end
	Map* ifv;               // C# ifv
	// C# virtual properties (Obj1C.cs) — kept on Obj for flat dispatch
	String* drawcall;       // C# virtual drawcall
	String* call_c;         // C# virtual call
	String* model_str;      // C# virtual model (for codegen)
	// C-pass codegen fields (used across types via Obj*)
	int identity;
	Obj* vartype;
	Obj* value;             // used in C-pass for Variable/Number value via Obj*
	int number_val;         // declareA2 counter
	List* draws;            // C-pass: Function, FuncTypeC, GeneObj
	List* calls_list;       // C-pass
	LLab* entry_lab;        // C-pass
	Obj* value_obj;         // C-pass
	List* singleops;        // Primary.singleops via Obj*
	List* rets;             // C-pass block rets via Obj*
};
// Obj 共通 trace。派生は traceObj を呼んだ後に独自フィールドを追加 trace。
inline void traceObj(ThreadGC* thgc, Obj* o) {
	o->letter     = (ATSSpan*)copy_object_young(thgc, (char*)o->letter);
	o->children   = (List*)   copy_object_young(thgc, (char*)o->children);
	o->rename     = (String*) copy_object_young(thgc, (char*)o->rename);
	o->version    = (String*) copy_object_young(thgc, (char*)o->version);
	o->model      = (String*) copy_object_young(thgc, (char*)o->model);
	o->forvari    = (LVari*)  copy_object_young(thgc, (char*)o->forvari);
	o->vari_c     = (LVari*)  copy_object_young(thgc, (char*)o->vari_c);
	o->bas        = (LVari*)  copy_object_young(thgc, (char*)o->bas);
	o->entry      = (LLab*)   copy_object_young(thgc, (char*)o->entry);
	o->end_lab    = (LLab*)   copy_object_young(thgc, (char*)o->end_lab);
	o->ifv        = (Map*)    copy_object_young(thgc, (char*)o->ifv);
	o->drawcall   = (String*) copy_object_young(thgc, (char*)o->drawcall);
	o->call_c     = (String*) copy_object_young(thgc, (char*)o->call_c);
	o->model_str  = (String*) copy_object_young(thgc, (char*)o->model_str);
	o->vartype    = (Obj*)    copy_object_young(thgc, (char*)o->vartype);
	o->value      = (Obj*)    copy_object_young(thgc, (char*)o->value);
	o->draws      = (List*)   copy_object_young(thgc, (char*)o->draws);
	o->calls_list = (List*)   copy_object_young(thgc, (char*)o->calls_list);
	o->entry_lab  = (LLab*)   copy_object_young(thgc, (char*)o->entry_lab);
	o->value_obj  = (Obj*)    copy_object_young(thgc, (char*)o->value_obj);
	o->singleops  = (List*)   copy_object_young(thgc, (char*)o->singleops);
	o->rets       = (List*)   copy_object_young(thgc, (char*)o->rets);
}
inline void ObjCheck(ThreadGC* thgc, char* data) {
	traceObj(thgc, (Obj*)data);
}

// Val : Obj — C# Val : Obj { cls, vmap }
struct Val : Obj {
	Obj* cls;
	Map* vmap;
};
inline void traceVal(ThreadGC* thgc, Val* v) {
	traceObj(thgc, (Obj*)v);
	v->cls  = (Obj*)copy_object_young(thgc, (char*)v->cls);
	v->vmap = (Map*)copy_object_young(thgc, (char*)v->vmap);
}
inline void ValCheck(ThreadGC* thgc, char* data) {
	traceVal(thgc, (Val*)data);
}

// Type : Obj — C# Type : Obj { cls, letter2, initial }
struct Type : Obj {
	Obj* cls;
	ATSSpan* letter2;
	bool initial;
};
inline void traceType(ThreadGC* thgc, Type* t) {
	traceObj(thgc, (Obj*)t);
	t->cls     = (Obj*)    copy_object_young(thgc, (char*)t->cls);
	t->letter2 = (ATSSpan*)copy_object_young(thgc, (char*)t->letter2);
}
inline void TypeCheck(ThreadGC* thgc, char* data) {
	traceType(thgc, (Type*)data);
}

// FinishStatus: analyse結果のステータス
enum FinishStatus { FS_Yet = 0, FS_Finish = 1, FS_Error = 2 };

// CodeStatus: analyse時の各children要素のステータス
struct CodeStatus {
	FinishStatus status;
	ATSSpan* letter;
	String* error;
};

struct Block : Obj {
	Map* branchmap;
	Map* labelmap;
	Map* labelmapn;
	Map* branchmapn;  // int key → Label* (C# SortedList<int,Label>)
	ATSSpan* letter2;
	Map* vmapA;
	Map* vmapR;     // C# vmapR - rename duplicate detection (separate from vmapA)
	Type* cls;      // C# Block.cls
	Obj* obj;       // C# ObjBlock.obj
	int obj_n;      // C# ObjBlock.n
	List* letters;
	// analyse用フィールド
	List* statuses;
	bool finished;
	bool comment;
	List* blocks;
	List* returns_list;
	List* childblocks;  // List of Block* - 子ブロック (C# childblocks)
};
inline void BlockCheck(ThreadGC* thgc, char* data) {
	Block* b = (Block*)data;
	traceObj(thgc, (Obj*)b);
	b->branchmap    = (Map*)copy_object_young(thgc, (char*)b->branchmap);
	b->labelmap     = (Map*)copy_object_young(thgc, (char*)b->labelmap);
	b->labelmapn    = (Map*)copy_object_young(thgc, (char*)b->labelmapn);
	b->branchmapn   = (Map*)copy_object_young(thgc, (char*)b->branchmapn);
	b->letter2      = (ATSSpan*)copy_object_young(thgc, (char*)b->letter2);
	b->vmapA        = (Map*)copy_object_young(thgc, (char*)b->vmapA);
	b->vmapR        = (Map*)copy_object_young(thgc, (char*)b->vmapR);
	b->cls          = (Type*)copy_object_young(thgc, (char*)b->cls);
	b->obj          = (Obj*) copy_object_young(thgc, (char*)b->obj);
	b->letters      = (List*)copy_object_young(thgc, (char*)b->letters);
	b->statuses     = (List*)copy_object_young(thgc, (char*)b->statuses);
	b->blocks       = (List*)copy_object_young(thgc, (char*)b->blocks);
	b->returns_list = (List*)copy_object_young(thgc, (char*)b->returns_list);
	b->childblocks  = (List*)copy_object_young(thgc, (char*)b->childblocks);
}

struct TagBlock : Obj {
	Map* branchmap;
	Map* labelmap;
	Map* labelmapn;
	ATSSpan* letter2;
	ATSSpan* tagletter;
	Map* vmapA;
	Obj* obj;
};
inline void TagBlockCheck(ThreadGC* thgc, char* data) {
	TagBlock* b = (TagBlock*)data;
	traceObj(thgc, (Obj*)b);
	b->branchmap = (Map*)copy_object_young(thgc, (char*)b->branchmap);
	b->labelmap  = (Map*)copy_object_young(thgc, (char*)b->labelmap);
	b->labelmapn = (Map*)copy_object_young(thgc, (char*)b->labelmapn);
	b->letter2   = (ATSSpan*)copy_object_young(thgc, (char*)b->letter2);
	b->tagletter = (ATSSpan*)copy_object_young(thgc, (char*)b->tagletter);
	b->vmapA     = (Map*)copy_object_young(thgc, (char*)b->vmapA);
	b->obj       = (Obj*)copy_object_young(thgc, (char*)b->obj);
}

struct Primary : Obj {
};
inline void PrimaryCheck(ThreadGC* thgc, char* data) {
	traceObj(thgc, (Obj*)data);
}

struct Label {
	ATSSpan* letter;
	String* name;
	int n;
	Map* labelmap;
};
inline void LabelCheck(ThreadGC* thgc, char* data) {
	Label* l = (Label*)data;
	l->letter   = (ATSSpan*)copy_object_young(thgc, (char*)l->letter);
	l->name     = (String*) copy_object_young(thgc, (char*)l->name);
	l->labelmap = (Map*)    copy_object_young(thgc, (char*)l->labelmap);
}

struct PrimOp {
	LetterType objtype;
	ATSSpan* letter;
};

inline void PrimOpCheck(ThreadGC* thgc, char* data) {
	PrimOp* p = (PrimOp*)data;
	p->letter = (ATSSpan*)copy_object_young(thgc, (char*)p->letter);
}

struct SingleOp {
	ATSSpan* letter;
};

inline void SingleOpCheck(ThreadGC* thgc, char* data) {
	SingleOp* s = (SingleOp*)data;
	s->letter = (ATSSpan*)copy_object_young(thgc, (char*)s->letter);
}

struct Generic;
struct Stock;
struct ModelObj;

// C# OneMigrate: fulltext, models, tables, AddModel
struct OneMigrate {
	String* fulltext;
	Map* models;   // String* -> ModelObj*
	Map* tables;   // String* -> ModelObj*
};
inline void OneMigrateCheck(ThreadGC* thgc, char* data) {
	OneMigrate* o = (OneMigrate*)data;
	o->fulltext = (String*)copy_object_young(thgc, (char*)o->fulltext);
	o->models   = (Map*)   copy_object_young(thgc, (char*)o->models);
	o->tables   = (Map*)   copy_object_young(thgc, (char*)o->tables);
}

// C# Migrate: migrations list, Start(), AddModel()
struct MigrateObj {
	List* migrations; // List of OneMigrate*
};
inline void MigrateObjCheck(ThreadGC* thgc, char* data) {
	MigrateObj* m = (MigrateObj*)data;
	m->migrations = (List*)copy_object_young(thgc, (char*)m->migrations);
}

struct Master {
	NewElement* top;
	NewElement* elem;
	LetterType lasttype;
	ATSSpan* last;
	ATSSpan* before;
	int n;
	void init(ThreadGC* thgc, NewElement* _top) {
		elem = top = _top; n = 0;
		operators = create_list(thgc, sizeof(List*), CType::_List);
		// = :
		List* op0 = create_list(thgc, sizeof(LetterType), CType::_Struct);
		add_list(thgc, op0, (char*)LetterType::_Equal);
		add_list(thgc, op0, (char*)LetterType::_Colon);
		add_list(thgc, operators, (char*)op0);
		// < <= > >= == !=
		List* op1 = create_list(thgc, sizeof(LetterType), CType::_Struct);
		add_list(thgc, op1, (char*)LetterType::_LessThan);
		add_list(thgc, op1, (char*)LetterType::_LessEqual);
		add_list(thgc, op1, (char*)LetterType::_MoreThan);
		add_list(thgc, op1, (char*)LetterType::_MoreEqual);
		add_list(thgc, op1, (char*)LetterType::_EqualEqual);
		add_list(thgc, op1, (char*)LetterType::_NotEqual);
		add_list(thgc, operators, (char*)op1);
		// + -
		List* op2 = (List*)create_list(thgc, sizeof(LetterType), CType::_Struct);
		add_list(thgc, op2, (char*)LetterType::_Plus);
		add_list(thgc, op2, (char*)LetterType::_Minus);
		add_list(thgc, operators, (char*)op2);
		// * / %
		List* op3 = create_list(thgc, sizeof(LetterType), CType::_Struct);
		add_list(thgc, op3, (char*)LetterType::_Mul);
		add_list(thgc, op3, (char*)LetterType::_Div);
		add_list(thgc, op3, (char*)LetterType::_Percent);
		add_list(thgc, operators, (char*)op3);
		labelmap = create_mapy(thgc, CType::_List);
		// Initialize Master fields that C# initializes as field initializers
		calls = create_list(thgc, sizeof(void*), CType::_Struct);
		labs = create_list(thgc, sizeof(void*), CType::_Struct);
		sigmapA = create_mapy(thgc, CType::_List);
		vmapA2 = create_mapy(thgc, CType::_CObj);
		elem_flag = false;
		// ModelObj singletons - C# ModelObj : ClassObj has children, extends, entry, end
		auto _initModel = [&](ModelObj** out) {
			*out = (ModelObj*)GC_alloc(thgc, CType::_ModelObj);
			Obj* o = (Obj*)*out;
			o->type = LetterType::OT_ModelObj;
			o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
			// extends_list is on ClassObj which isn't fully defined here yet;
			// it will be initialized in init2() after ClassObj is defined
		};
		_initModel(&Void);
		_initModel(&Object);
		_initModel(&Int);
		_initModel(&StrT);
		((Obj*)StrT)->model_str = createString(thgc, (char*)"%StringType", 11, 1);
		_initModel(&Short);
		_initModel(&Bool);
		_initModel(&FloatT);
		_initModel(&MouseEventT);
		_initModel(&KeyEventT);
	}
	// init2: called after Generic/Stock structs are fully defined
	void init2(ThreadGC* thgc);
	void Next() {
	head:
		if (elem->type == LetterType::_Letter) {
			NewLetter* letter = (NewLetter*)elem;
			for (; n < letter->atsspans->size; n++) {
				auto as = *(ATSSpan**)get_list(letter->atsspans, n);
				if (as->t == LetterType::_Space) continue;
				else {
					last = as;
					as->letter = (NewElement*)letter;
					before = as;
					lasttype = as->t;
					n++;
					return;
				}
			}
			n = 0;
		}
		if (elem->childend != NULL) {
			elem = elem->childend->next;
			goto head;
		}
		if (elem->type == LetterType::_ElemEnd) {
			elem = elem->parent->next;
			if (elem->before == top) {
				lasttype = LetterType::_End;
				last = NULL;
				return;
			}
			else if (lasttype == LetterType::_Kaigyou) goto head;
			else {
				lasttype = LetterType::_Kaigyou;
				last = NULL;
				return;
			}
		}
		elem = elem->next;
		goto head;
	}
	List* operators;
	List* analblocks;
	Map* labelmap;
	int IntCls;
	int FloatCls;
	int StrCls;
	List* blockslist;
	Obj* callblock;
	List* comments;
	Generic* gene;
	ModelObj* Object;
	ModelObj* Void;
	ModelObj* Int;
	ModelObj* Short;
	ModelObj* StrT;
	ModelObj* Bool;
	ModelObj* FloatT;
	ModelObj* MouseEventT;
	ModelObj* KeyEventT;
	// C pass (LLVM IR code generation) fields
	LLLVM* llvm;
	List* compsC;
	LVari* go_c;
	LVari* i8p;
	LVari* countv;
	LVari* objptr3;
	LVari* co_val;
	LVari* blok;
	LTypeDec* typedecC;
	int countC;
	int odr;
	LFunc* funcdec;
	LFunc* funccheck;
	Map* sigmapA;
	// Additional members for code generation pass
	List* calls;
	List* kouhos;
	List* labs;
	ATSSpan* letter;
	LTypeDec* typedec_c;
	LFunc* funccheck_c;
	LFunc* funcdec_c;
	LVari* i8p_c;
	LVari* objptr3_c;
	LVari* co_val_c;
	int count_c;
	MigrateObj* migrate;
	Stock* db;
	// analyse用フィールド
	int step;
	bool secondB;
	bool comment_flag;
	bool elem_flag;  // C# local.elem (bool) — LinearFunction PrimaryB用
	Obj* obj;
	Map* vmapA2;  // C# Dictionary<int, Obj> vmapA2 — CallBlock.N indexed lookup
};
inline void MasterCheck(ThreadGC* thgc, char* data) {
	Master* m = (Master*)data;
	m->top         = (NewElement*)copy_object_young(thgc, (char*)m->top);
	m->elem        = (NewElement*)copy_object_young(thgc, (char*)m->elem);
	m->last        = (ATSSpan*)   copy_object_young(thgc, (char*)m->last);
	m->before      = (ATSSpan*)   copy_object_young(thgc, (char*)m->before);
	m->operators   = (List*) copy_object_young(thgc, (char*)m->operators);
	m->analblocks  = (List*) copy_object_young(thgc, (char*)m->analblocks);
	m->labelmap    = (Map*)  copy_object_young(thgc, (char*)m->labelmap);
	m->blockslist  = (List*) copy_object_young(thgc, (char*)m->blockslist);
	m->callblock   = (Obj*)  copy_object_young(thgc, (char*)m->callblock);
	m->comments    = (List*) copy_object_young(thgc, (char*)m->comments);
	m->gene        = (Generic*) copy_object_young(thgc, (char*)m->gene);
	m->Object      = (ModelObj*)copy_object_young(thgc, (char*)m->Object);
	m->Void        = (ModelObj*)copy_object_young(thgc, (char*)m->Void);
	m->Int         = (ModelObj*)copy_object_young(thgc, (char*)m->Int);
	m->Short       = (ModelObj*)copy_object_young(thgc, (char*)m->Short);
	m->StrT        = (ModelObj*)copy_object_young(thgc, (char*)m->StrT);
	m->Bool        = (ModelObj*)copy_object_young(thgc, (char*)m->Bool);
	m->FloatT      = (ModelObj*)copy_object_young(thgc, (char*)m->FloatT);
	m->MouseEventT = (ModelObj*)copy_object_young(thgc, (char*)m->MouseEventT);
	m->KeyEventT   = (ModelObj*)copy_object_young(thgc, (char*)m->KeyEventT);
	m->llvm        = (LLLVM*)   copy_object_young(thgc, (char*)m->llvm);
	m->compsC      = (List*)    copy_object_young(thgc, (char*)m->compsC);
	m->go_c        = (LVari*)   copy_object_young(thgc, (char*)m->go_c);
	m->i8p         = (LVari*)   copy_object_young(thgc, (char*)m->i8p);
	m->countv      = (LVari*)   copy_object_young(thgc, (char*)m->countv);
	m->objptr3     = (LVari*)   copy_object_young(thgc, (char*)m->objptr3);
	m->co_val      = (LVari*)   copy_object_young(thgc, (char*)m->co_val);
	m->blok        = (LVari*)   copy_object_young(thgc, (char*)m->blok);
	m->typedecC    = (LTypeDec*)copy_object_young(thgc, (char*)m->typedecC);
	m->funcdec     = (LFunc*)   copy_object_young(thgc, (char*)m->funcdec);
	m->funccheck   = (LFunc*)   copy_object_young(thgc, (char*)m->funccheck);
	m->sigmapA     = (Map*)     copy_object_young(thgc, (char*)m->sigmapA);
	m->calls       = (List*)    copy_object_young(thgc, (char*)m->calls);
	m->kouhos      = (List*)    copy_object_young(thgc, (char*)m->kouhos);
	m->labs        = (List*)    copy_object_young(thgc, (char*)m->labs);
	m->letter      = (ATSSpan*) copy_object_young(thgc, (char*)m->letter);
	m->typedec_c   = (LTypeDec*)copy_object_young(thgc, (char*)m->typedec_c);
	m->funccheck_c = (LFunc*)   copy_object_young(thgc, (char*)m->funccheck_c);
	m->funcdec_c   = (LFunc*)   copy_object_young(thgc, (char*)m->funcdec_c);
	m->i8p_c       = (LVari*)   copy_object_young(thgc, (char*)m->i8p_c);
	m->objptr3_c   = (LVari*)   copy_object_young(thgc, (char*)m->objptr3_c);
	m->co_val_c    = (LVari*)   copy_object_young(thgc, (char*)m->co_val_c);
	m->migrate     = (MigrateObj*)copy_object_young(thgc, (char*)m->migrate);
	m->db          = (Stock*)   copy_object_young(thgc, (char*)m->db);
	m->obj         = (Obj*)     copy_object_young(thgc, (char*)m->obj);
	m->vmapA2      = (Map*)     copy_object_young(thgc, (char*)m->vmapA2);
}

// ============================================================
// Z解析用の型構造体
// ============================================================

// Var: a type variable (OT_Var), used for generic type resolution
// C#: Var : Type { List<Var> connects }
struct Var : Type {
	List* connects;  // List of Var*
};

inline void VarCheck(ThreadGC* thgc, char* data) {
	Var* v = (Var*)data;
	traceType(thgc, (Type*)v);
	v->connects = (List*)copy_object_young(thgc, (char*)v->connects);
}

// VariClass : Type — C# VariClass : Type { int n }
struct VariClass : Type {
	int n;
};

inline void VariClassCheck(ThreadGC* thgc, char* data) {
	traceType(thgc, (Type*)data);
}

// ArrType : Type — C# ArrType : Type { int array, List<int> list }
struct ArrType : Type {
	int array;
	List* list;
};

inline void ArrTypeCheck(ThreadGC* thgc, char* data) {
	ArrType* a = (ArrType*)data;
	traceType(thgc, (Type*)a);
	a->list = (List*)copy_object_young(thgc, (char*)a->list);
}

// FuncType : Type — C# FuncType : Type { bool nochange, List<Type> draws, Type cls }
struct FuncTypeC : Type {
	bool nochange;
	List* draws;
};

inline void FuncTypeCCheck(ThreadGC* thgc, char* data) {
	FuncTypeC* f = (FuncTypeC*)data;
	traceType(thgc, (Type*)f);
	f->draws = (List*)copy_object_young(thgc, (char*)f->draws);
}

// Variable : Obj — C# Variable : Obj { value, cls, left, right, comment, clone, order }
struct Variable : Obj {
	Obj* value;
	Obj* cls;
	Obj* left;
	Obj* right;
	ATSSpan* comment;
	bool clone;
	int order;
	LVari* strvari;  // C++ specific (C pass)
};

inline void VariableCheck(ThreadGC* thgc, char* data) {
	Variable* v = (Variable*)data;
	traceObj(thgc, (Obj*)v);
	v->value   = (Obj*)    copy_object_young(thgc, (char*)v->value);
	v->cls     = (Obj*)    copy_object_young(thgc, (char*)v->cls);
	v->left    = (Obj*)    copy_object_young(thgc, (char*)v->left);
	v->right   = (Obj*)    copy_object_young(thgc, (char*)v->right);
	v->comment = (ATSSpan*)copy_object_young(thgc, (char*)v->comment);
	v->strvari = (LVari*)  copy_object_young(thgc, (char*)v->strvari);
}

// FloatVal : Val — C# FloatVal : Val { float value }
struct FloatVal : Val {
	float value;
};

inline void FloatValCheck(ThreadGC* thgc, char* data) {
	traceVal(thgc, (Val*)data);
}

// Function : Obj — C# Function : Obj { blocks, draw, ret, bracket, block, entry, end }
struct Function : Obj {
	List* blocks;
	Block* draw;  // C# CallBlock draw (CallBlock = typedef Block)
	Obj* rettype;  // C# ret
	Block* bracket;
	Block* block;
	LLab* entry;
	LLab* end;
};

inline void traceFunction(ThreadGC* thgc, Function* f) {
	traceObj(thgc, (Obj*)f);
	f->blocks  = (List*) copy_object_young(thgc, (char*)f->blocks);
	f->draw    = (Block*)copy_object_young(thgc, (char*)f->draw);
	f->rettype = (Obj*)  copy_object_young(thgc, (char*)f->rettype);
	f->bracket = (Block*)copy_object_young(thgc, (char*)f->bracket);
	f->block   = (Block*)copy_object_young(thgc, (char*)f->block);
	f->entry   = (LLab*) copy_object_young(thgc, (char*)f->entry);
	f->end     = (LLab*) copy_object_young(thgc, (char*)f->end);
}
inline void FunctionCheck(ThreadGC* thgc, char* data) {
	traceFunction(thgc, (Function*)data);
}

// GenericFunction : Function — C# GenericFunction : Function { vmap }
struct GenericFunction : Function {
	Map* vmap;
};

inline void GenericFunctionCheck(ThreadGC* thgc, char* data) {
	GenericFunction* g = (GenericFunction*)data;
	traceFunction(thgc, (Function*)g);
	g->vmap = (Map*)copy_object_young(thgc, (char*)g->vmap);
}

// Address : Obj — C# Address : Obj { address, draw }
struct Address : Obj {
	String* address;
	Block* draw;
};

inline void AddressCheck(ThreadGC* thgc, char* data) {
	Address* a = (Address*)data;
	traceObj(thgc, (Obj*)a);
	a->address = (String*)copy_object_young(thgc, (char*)a->address);
	a->draw    = (Block*) copy_object_young(thgc, (char*)a->draw);
}

// (Type defined after Obj)

// ClassObj : Type — C# ClassObj : Type { blocks, draw, ret, extends }
struct ClassObj : Type {
	List* blocks;
	Block* draw;  // C# CallBlock draw
	Obj* ret;
	List* extends_list;
};

inline void traceClassObj(ThreadGC* thgc, ClassObj* c) {
	traceType(thgc, (Type*)c);
	c->blocks       = (List*) copy_object_young(thgc, (char*)c->blocks);
	c->draw         = (Block*)copy_object_young(thgc, (char*)c->draw);
	c->ret          = (Obj*)  copy_object_young(thgc, (char*)c->ret);
	c->extends_list = (List*) copy_object_young(thgc, (char*)c->extends_list);
}
inline void ClassObjCheck(ThreadGC* thgc, char* data) {
	traceClassObj(thgc, (ClassObj*)data);
}

// SqlParamMap: map with separate keys (int list) and vals (Obj* list) for SQL parameter binding
struct SqlParamMap {
	List* keys;
	List* vals;
};

inline void SqlParamMapCheck(ThreadGC* thgc, char* data) {
	SqlParamMap* m = (SqlParamMap*)data;
	m->keys = (List*)copy_object_young(thgc, (char*)m->keys);
	m->vals = (List*)copy_object_young(thgc, (char*)m->vals);
}

// SqlParamBind: SQL string with parameter bindings (internal)
struct SqlParamBind {
	String* sql;
	SqlParamMap* varis;
};

inline void SqlParamBindCheck(ThreadGC* thgc, char* data) {
	SqlParamBind* b = (SqlParamBind*)data;
	b->sql   = (String*)     copy_object_young(thgc, (char*)b->sql);
	b->varis = (SqlParamMap*)copy_object_young(thgc, (char*)b->varis);
}

// Constructor: C# Constructor : Obj
// Constructor : Obj — C# Constructor : Obj { ClassObj cls }
struct Constructor : Obj {
	ClassObj* cls;
};

inline void ConstructorCheck(ThreadGC* thgc, char* data) {
	Constructor* c = (Constructor*)data;
	traceObj(thgc, (Obj*)c);
	c->cls = (ClassObj*)copy_object_young(thgc, (char*)c->cls);
}

// ArrayConstructor : Obj — C# ArrayConstructor : Obj { ArrType cls }
struct ArrayConstructor : Obj {
	ArrType* cls;
};

inline void ArrayConstructorCheck(ThreadGC* thgc, char* data) {
	ArrayConstructor* a = (ArrayConstructor*)data;
	traceObj(thgc, (Obj*)a);
	a->cls = (ArrType*)copy_object_young(thgc, (char*)a->cls);
}

// SqlString : Val — C# SqlString : Val { String value }
struct SqlString : Val {
	String* value;
};

inline void SqlStringCheck(ThreadGC* thgc, char* data) {
	SqlString* s = (SqlString*)data;
	traceVal(thgc, (Val*)s);
	s->value = (String*)copy_object_young(thgc, (char*)s->value);
}

// BoolVal : Val — C# BoolVal : Val { bool value }
struct BoolVal : Val {
	bool value;
};

inline void BoolValCheck(ThreadGC* thgc, char* data) {
	traceVal(thgc, (Val*)data);
}

// Null : Val — C# Null : Val (no additional fields)
// Uses make_cobj with OT_Null

// Value : Val — C# Value : Val (resolved value)
struct Value : Val {
	Map* vmap0;     // C# Value.vmap0 - ClassObj draw[0] vmapA
	LVari* vari;
};

inline void ValueCheck(ThreadGC* thgc, char* data) {
	Value* v = (Value*)data;
	traceVal(thgc, (Val*)v);
	v->vmap0 = (Map*)  copy_object_young(thgc, (char*)v->vmap0);
	v->vari  = (LVari*)copy_object_young(thgc, (char*)v->vari);
}

// ModelVal : Val — C# ModelVal : Val
// Uses make_cobj with OT_ModelVal

// ModelObj : ClassObj — C# ModelObj : ClassObj (no additional fields in C#)
// C++ keeps C-pass specific fields
struct ModelObj : ClassObj {
	int n_val;
	String* call;
	String* drawcall;
	List* ifv;
	bool decstore;
	bool decdelete;
	bool decselect;
	LVari* bas;
};

inline void traceModelObj(ThreadGC* thgc, ModelObj* m) {
	traceClassObj(thgc, (ClassObj*)m);
	m->call     = (String*)copy_object_young(thgc, (char*)m->call);
	m->drawcall = (String*)copy_object_young(thgc, (char*)m->drawcall);
	m->ifv      = (List*)  copy_object_young(thgc, (char*)m->ifv);
	m->bas      = (LVari*) copy_object_young(thgc, (char*)m->bas);
}
inline void ModelObjCheck(ThreadGC* thgc, char* data) {
	traceModelObj(thgc, (ModelObj*)data);
}

typedef Block CallBlock;

// GeneObj : ModelObj — C# GeneObj : ModelObj { name, blocks, gvs, gv, call, left, right, vmap }
struct GeneObj : ModelObj {
	String* name;
	CallBlock* call_blk;  // C# call (CallBlock) - named differently to avoid conflict with ModelObj.call
	List* geneblocks;  // C# blocks (separate from ClassObj.blocks)
	Map* vmap;
	List* gvs;
	Obj* gv;
	CallBlock* left;
	CallBlock* right;
};

inline void GeneObjCheck(ThreadGC* thgc, char* data) {
	GeneObj* g = (GeneObj*)data;
	traceModelObj(thgc, (ModelObj*)g);
	g->name       = (String*)   copy_object_young(thgc, (char*)g->name);
	g->call_blk   = (CallBlock*)copy_object_young(thgc, (char*)g->call_blk);
	g->geneblocks = (List*)     copy_object_young(thgc, (char*)g->geneblocks);
	g->vmap       = (Map*)      copy_object_young(thgc, (char*)g->vmap);
	g->gvs        = (List*)     copy_object_young(thgc, (char*)g->gvs);
	g->gv         = (Obj*)      copy_object_young(thgc, (char*)g->gv);
	g->left       = (CallBlock*)copy_object_young(thgc, (char*)g->left);
	g->right      = (CallBlock*)copy_object_young(thgc, (char*)g->right);
}

// Stock: data store type
struct Stock : Obj {
	Map* vmap;
};

inline void StockCheck(ThreadGC* thgc, char* data) {
	Stock* s = (Stock*)data;
	traceObj(thgc, (Obj*)s);
	s->vmap = (Map*)copy_object_young(thgc, (char*)s->vmap);
}

// Generic : Obj — C# Generic : Obj { letter2, draw, block, blocks, vmap, extends }
struct Generic : Obj {
	ATSSpan* letter2;
	Block* draw;  // C# CallBlock draw
	Block* block;
	List* blocks;
	Map* vmap;
	List* extends_list;
};

inline void GenericCheck(ThreadGC* thgc, char* data) {
	Generic* g = (Generic*)data;
	traceObj(thgc, (Obj*)g);
	g->letter2      = (ATSSpan*)copy_object_young(thgc, (char*)g->letter2);
	g->draw         = (Block*)  copy_object_young(thgc, (char*)g->draw);
	g->block        = (Block*)  copy_object_young(thgc, (char*)g->block);
	g->blocks       = (List*)   copy_object_young(thgc, (char*)g->blocks);
	g->vmap         = (Map*)    copy_object_young(thgc, (char*)g->vmap);
	g->extends_list = (List*)   copy_object_young(thgc, (char*)g->extends_list);
}

// GenericObj : Type — C# GenericObj : Type { extends, gene, draws }
struct GenericObj : Type {
	List* extends_list;
	Generic* gene;
	List* draws;
};

inline void GenericObjCheck(ThreadGC* thgc, char* data) {
	GenericObj* g = (GenericObj*)data;
	traceType(thgc, (Type*)g);
	g->extends_list = (List*)   copy_object_young(thgc, (char*)g->extends_list);
	g->gene         = (Generic*)copy_object_young(thgc, (char*)g->gene);
	g->draws        = (List*)   copy_object_young(thgc, (char*)g->draws);
}

struct ObjBlock : Obj {
	Obj* obj;
	int n;
};

inline void ObjBlockCheck(ThreadGC* thgc, char* data) {
	ObjBlock* o = (ObjBlock*)data;
	traceObj(thgc, (Obj*)o);
	o->obj = (Obj*)copy_object_young(thgc, (char*)o->obj);
}

struct Base : Obj {
	Obj* value;
	List* extends_list;
};

inline void BaseCheck(ThreadGC* thgc, char* data) {
	Base* b = (Base*)data;
	traceObj(thgc, (Obj*)b);
	b->value        = (Obj*) copy_object_young(thgc, (char*)b->value);
	b->extends_list = (List*)copy_object_young(thgc, (char*)b->extends_list);
}

// C# IfBlock: call, iflabel, ifobj
struct IfBlock : Obj {
	CallBlock* call;
	Obj* iflabel;
	Obj* ifobj;
};

inline void IfBlockCheck(ThreadGC* thgc, char* data) {
	IfBlock* i = (IfBlock*)data;
	traceObj(thgc, (Obj*)i);
	i->call    = (CallBlock*)copy_object_young(thgc, (char*)i->call);
	i->iflabel = (Obj*)      copy_object_young(thgc, (char*)i->iflabel);
	i->ifobj   = (Obj*)      copy_object_young(thgc, (char*)i->ifobj);
}

inline IfBlock* make_ifblock(ThreadGC* thgc, CallBlock* call, Obj* iflabel, Obj* ifobj) {
	IfBlock* ifb = (IfBlock*)GC_alloc(thgc, CType::_CIfBlock);
	((Obj*)ifb)->type = LetterType::OT_IfBlock;
	((Obj*)ifb)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	ifb->call = call;
	ifb->iflabel = iflabel;
	ifb->ifobj = ifobj;
	return ifb;
}

// ServerClient: client/server type with name
struct ServerClient : Obj {
	String* name;
};

inline void ServerClientCheck(ThreadGC* thgc, char* data) {
	ServerClient* s = (ServerClient*)data;
	traceObj(thgc, (Obj*)s);
	s->name = (String*)copy_object_young(thgc, (char*)s->name);
}

extern ObjVT vt_Obj_default;
inline ObjBlock* make_objblock(ThreadGC* thgc, Obj* obj, int n) {
	ObjBlock* ob = (ObjBlock*)GC_alloc(thgc, CType::_ObjBlock);
	ob->type = LetterType::OT_None;
	ob->vt = &vt_Obj_default;
	ob->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	ob->obj = obj;
	ob->n = n;
	return ob;
}

// Number : Val — C# Number : Val { int value }
struct Number : Val {
	int value;
};

inline void NumberCheck(ThreadGC* thgc, char* data) {
	traceVal(thgc, (Val*)data);
}

// StrObj : Val — C# StrObj : Val { String value }
struct StrObj : Val {
	String* value;
};

inline void StrObjCheck(ThreadGC* thgc, char* data) {
	StrObj* s = (StrObj*)data;
	traceVal(thgc, (Val*)s);
	s->value = (String*)copy_object_young(thgc, (char*)s->value);
}

// Word: identifier name
struct Word : Obj {
	String* name;
};

inline void WordCheck(ThreadGC* thgc, char* data) {
	Word* w = (Word*)data;
	traceObj(thgc, (Obj*)w);
	w->name = (String*)copy_object_young(thgc, (char*)w->name);
}

// (Val defined after Obj)

// Gene subtypes - all have a gj (parent GeneObj*) reference
struct GeneChild : Obj { GeneObj* gj; };
inline void GeneChildCheck(ThreadGC* thgc, char* data) {
	GeneChild* g = (GeneChild*)data;
	traceObj(thgc, (Obj*)g);
	g->gj = (GeneObj*)copy_object_young(thgc, (char*)g->gj);
}
struct GeneMutate : Obj { GeneObj* gj; };
inline void GeneMutateCheck(ThreadGC* thgc, char* data) {
	GeneMutate* g = (GeneMutate*)data;
	traceObj(thgc, (Obj*)g);
	g->gj = (GeneObj*)copy_object_young(thgc, (char*)g->gj);
}
struct GeneNew : Obj { GeneObj* gj; };
inline void GeneNewCheck(ThreadGC* thgc, char* data) {
	GeneNew* g = (GeneNew*)data;
	traceObj(thgc, (Obj*)g);
	g->gj = (GeneObj*)copy_object_young(thgc, (char*)g->gj);
}
struct GeneCross : Obj { GeneObj* gj; };
inline void GeneCrossCheck(ThreadGC* thgc, char* data) {
	GeneCross* g = (GeneCross*)data;
	traceObj(thgc, (Obj*)g);
	g->gj = (GeneObj*)copy_object_young(thgc, (char*)g->gj);
}
struct GeneVal : Val {};  // C# GeneVal : Val
inline void GeneValCheck(ThreadGC* thgc, char* data) {
	traceVal(thgc, (Val*)data);
}
struct GeneStore : Obj { GeneObj* gj; };
inline void GeneStoreCheck(ThreadGC* thgc, char* data) {
	GeneStore* g = (GeneStore*)data;
	traceObj(thgc, (Obj*)g);
	g->gj = (GeneObj*)copy_object_young(thgc, (char*)g->gj);
}
struct GeneSelect : Obj { GeneObj* gj; };
inline void GeneSelectCheck(ThreadGC* thgc, char* data) {
	GeneSelect* g = (GeneSelect*)data;
	traceObj(thgc, (Obj*)g);
	g->gj = (GeneObj*)copy_object_young(thgc, (char*)g->gj);
}
struct GeneSort : Obj { GeneObj* gj; };
inline void GeneSortCheck(ThreadGC* thgc, char* data) {
	GeneSort* g = (GeneSort*)data;
	traceObj(thgc, (Obj*)g);
	g->gj = (GeneObj*)copy_object_young(thgc, (char*)g->gj);
}
struct GeneLabel : Type {};  // C# GeneLabel : Type
inline void GeneLabelCheck(ThreadGC* thgc, char* data) {
	traceType(thgc, (Type*)data);
}
struct GeneLabelValue : Val { GeneLabel* id; };  // C# GeneLabelValue : Val { Id id }
inline void GeneLabelValueCheck(ThreadGC* thgc, char* data) {
	GeneLabelValue* g = (GeneLabelValue*)data;
	traceVal(thgc, (Val*)g);
	g->id = (GeneLabel*)copy_object_young(thgc, (char*)g->id);
}
struct CommentType : Type {};  // C# CommentType : Type (no additional fields)
inline void CommentTypeCheck(ThreadGC* thgc, char* data) {
	traceType(thgc, (Type*)data);
}
struct Comment : Obj {
	Map* vmap;  // C# Dictionary<String, Letter> vmap
};
inline void CommentCheck(ThreadGC* thgc, char* data) {
	Comment* c = (Comment*)data;
	traceObj(thgc, (Obj*)c);
	c->vmap = (Map*)copy_object_young(thgc, (char*)c->vmap);
}

// C# CommentLet : Letter — UI element for structured comments
struct CommentLet {
	Comment* comment;
	List* instanceslist;  // List of List* (each is a list of Element pointers)
	List* elems;           // List of Element pointers
	List* nums;            // List of int
};

inline void CommentLetCheck(ThreadGC* thgc, char* data) {
	CommentLet* c = (CommentLet*)data;
	c->comment       = (Comment*)copy_object_young(thgc, (char*)c->comment);
	c->instanceslist = (List*)   copy_object_young(thgc, (char*)c->instanceslist);
	c->elems         = (List*)   copy_object_young(thgc, (char*)c->elems);
	c->nums          = (List*)   copy_object_young(thgc, (char*)c->nums);
}

inline CommentLet* create_commentlet(ThreadGC* thgc) {
	CommentLet* cl = (CommentLet*)GC_alloc(thgc, CType::_CommentLet);
	cl->comment = NULL;
	cl->instanceslist = create_list(thgc, sizeof(List*), CType::_List);
	cl->elems = create_list(thgc, sizeof(char*), CType::_CObj);
	cl->nums = create_list(thgc, sizeof(int), CType::_CObj);
	return cl;
}

inline void Start_CommentLet(ThreadGC* thgc, CommentLet* cl) {
	List* newlist = create_list(thgc, sizeof(char*), CType::_CObj);
	add_list(thgc, cl->instanceslist, (char*)newlist);
}

inline void Renew_CommentLet(ThreadGC* thgc, CommentLet* cl) {
	cl->instanceslist = create_list(thgc, sizeof(List*), CType::_List);
	cl->elems = create_list(thgc, sizeof(char*), CType::_CObj);
	cl->nums = create_list(thgc, sizeof(int), CType::_CObj);
}

// C# CommentLet.instances property: get => instanceslist.Last()
inline List* CommentLet_instances(CommentLet* cl) {
	if (cl->instanceslist == NULL || cl->instanceslist->size == 0) return NULL;
	return *(List**)get_list(cl->instanceslist, cl->instanceslist->size - 1);
}

// C# CommentLet.SingleB(op, local) — simplified for C++ (UI handling)
inline void SingleB_CommentLet(ThreadGC* thgc, CommentLet* cl, SingleOp* op, Master* local) {
	List* instances = CommentLet_instances(cl);
	if (op == NULL) {
		// foreach (var elem in instances) elems[elems.Count - 1].add(elem);
		// UI element manipulation — simplified
	} else if (instances != NULL && instances->size > 0) {
		if (op->letter != NULL) {
			LetterType lt = op->letter->t;
			if (lt == LetterType::_Dot) {
				// nums[nums.Count - 1] = 0;
				if (cl->nums->size > 0) {
					int zero = 0;
					set_list(cl->nums, cl->nums->size - 1, (char*)(intptr_t)zero);
				}
			} else if (lt == LetterType::_Mul) {
				// nums[nums.Count - 1]++;
				if (cl->nums->size > 0) {
					int val = (int)(intptr_t)*(char**)get_list(cl->nums, cl->nums->size - 1);
					val++;
					set_list(cl->nums, cl->nums->size - 1, (char*)(intptr_t)val);
				}
			} else if (lt == LetterType::_RightRight) {
				// >> case: UI async operation — simplified
				if (cl->instanceslist->size > 0) cl->instanceslist->size--;
				if (cl->nums->size > 0) {
					int zero = 0;
					set_list(cl->nums, cl->nums->size - 1, (char*)(intptr_t)zero);
				}
				return;
			} else {
				// default: nums[nums.Count - 1] = 0;
				if (cl->nums->size > 0) {
					int zero = 0;
					set_list(cl->nums, cl->nums->size - 1, (char*)(intptr_t)zero);
				}
			}
		}
	}
	// instanceslist.RemoveAt(instanceslist.Count - 1)
	if (cl->instanceslist->size > 0) cl->instanceslist->size--;
}

// C# CommentLet.ValueAdd(name): create letter, register in comment.vmap
inline ATSSpan* comment_value_add(ThreadGC* thgc, Comment* comment, String* name) {
	ATSSpan* cletter = (ATSSpan*)GC_alloc(thgc, CType::_ATSSpan);
	cletter->t = LetterType::OT_Htm;
	cletter->text = createString(thgc, (char*)"", 0, 1);
	if (!comment->vmap) comment->vmap = create_mapy(thgc, CType::_Map);
	add_mapy(thgc, comment->vmap, name, (char*)cletter);
	return cletter;
}
// CrossBlock/MigrateBlock: Block subtypes used in Gene
struct CrossBlock : Block { GeneObj* gj; };
typedef Block MigrateBlock;

// CheckType: type checking mode enum
enum CheckType { CheckType_Setter = 0, CheckType_Getter = 1, CheckType_Finish = 2, CheckType_Return = 3 };

extern ObjVT* vt_lookup[];
#ifndef VT_LOOKUP_SIZE
#define VT_LOOKUP_SIZE 256
#endif

// ファクトリ関数 - オブジェクト初期化の共通化
inline Obj* make_classobj(ThreadGC* thgc) {
	Obj* obj = (Obj*)GC_alloc(thgc, CType::_ClassObj);
	obj->type = LetterType::OT_ClassObj;
	obj->vt = vt_lookup[(int)LetterType::OT_ClassObj];
	obj->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((ClassObj*)obj)->extends_list = create_list(thgc, sizeof(Obj*), CType::_CObj);
	obj->rename = NULL;
	obj->version = NULL;
	return obj;
}

inline Generic* make_generic(ThreadGC* thgc) {
	Generic* g = (Generic*)GC_alloc(thgc, CType::_GenericType);
	g->type = LetterType::OT_Generic;
	g->vt = vt_lookup[(int)LetterType::OT_Generic];
	g->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	g->extends_list = create_list(thgc, sizeof(Obj*), CType::_CObj);
	g->vmap = create_mapy(thgc, CType::_List);
	g->block = NULL;
	g->letter2 = NULL;
	g->draw = NULL;
	return g;
}

inline ModelObj* make_modelobj(ThreadGC* thgc, ATSSpan* letter) {
	ModelObj* m = (ModelObj*)GC_alloc(thgc, CType::_ModelObj);
	m->type = LetterType::OT_Model;
	m->vt = vt_lookup[(int)LetterType::OT_Model];
	m->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	m->letter = letter;
	m->draw = NULL;
	m->rename = NULL;
	m->letter2 = NULL;
	((ClassObj*)m)->extends_list = create_list(thgc, sizeof(Obj*), CType::_CObj);
	return m;
}

inline GeneObj* make_geneobj(ThreadGC* thgc, String* name, ATSSpan* letter) {
	GeneObj* g = (GeneObj*)GC_alloc(thgc, CType::_GeneObj);
	((Obj*)g)->type = LetterType::OT_Gene;
	((Obj*)g)->vt = vt_lookup[(int)LetterType::OT_Gene];
	((Obj*)g)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)g)->letter = letter;
	((Type*)g)->letter2 = NULL;
	((Type*)g)->cls = NULL;
	((Type*)g)->initial = false;
	((ClassObj*)g)->blocks = NULL;
	((ClassObj*)g)->draw = NULL;
	((ClassObj*)g)->ret = NULL;
	((ClassObj*)g)->extends_list = NULL;
	g->name = name;
	g->call_blk = NULL;
	g->geneblocks = create_list(thgc, sizeof(Block*), CType::_Block);
	g->vmap = create_mapy(thgc, CType::_List);
	g->gvs = NULL;
	g->gv = NULL;
	g->left = NULL;
	g->right = NULL;
	return g;
}

inline Base* make_base(ThreadGC* thgc, Obj* value, List* extends) {
	Base* b = (Base*)GC_alloc(thgc, CType::_Base);
	b->type = LetterType::OT_Base;
	b->vt = vt_lookup[(int)LetterType::OT_Base];
	b->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	b->value = value;
	b->extends_list = extends;
	return b;
}

inline ObjBlock* make_objblock_z(ThreadGC* thgc, Obj* obj, int n) {
	ObjBlock* ob = (ObjBlock*)GC_alloc(thgc, CType::_ObjBlock);
	ob->type = LetterType::OT_ObjBlock;
	ob->vt = &vt_Obj_default;
	ob->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	ob->obj = obj;
	ob->n = n;
	return ob;
}

inline void Master::init2(ThreadGC* thgc) {
	// Initialize extends_list on ModelObj singletons (deferred from init() because ClassObj wasn't fully defined)
	auto _initExtends = [&](ModelObj* m) {
		((ClassObj*)m)->extends_list = create_list(thgc, sizeof(Obj*), CType::_CObj);
	};
	_initExtends(Void); _initExtends(Object); _initExtends(Int); _initExtends(StrT);
	_initExtends(Short); _initExtends(Bool); _initExtends(FloatT);
	_initExtends(MouseEventT); _initExtends(KeyEventT);
	// C# Short.extends.Add(Int), Int.extends.Add(Float)
	add_list(thgc, ((ClassObj*)Short)->extends_list, (char*)Int);
	add_list(thgc, ((ClassObj*)Int)->extends_list, (char*)FloatT);
	// Set initial flag on built-in ModelObj singletons (C# field initializer: initial = true)
	Object->initial = true;
	Void->initial = true;
	Int->initial = true;
	Short->initial = true;
	StrT->initial = true;
	Bool->initial = true;
	FloatT->initial = true;
	MouseEventT->initial = true;
	KeyEventT->initial = true;
	// C# gene = new Gene() (Gene extends Generic with vmap)
	gene = make_generic(thgc);
	// C# db = new Stock()
	db = (Stock*)GC_alloc(thgc, CType::_Stock);
	((Obj*)db)->type = LetterType::OT_Stock;
	((Obj*)db)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	db->vmap = create_mapy(thgc, CType::_List);
}

// ============================================================
// ヘルパー関数
// ============================================================
extern ObjVT vt_Obj_default;
extern ObjVT vt_ElemObj;
extern ObjVT* vt_lookup[];
#ifndef VT_LOOKUP_SIZE
#define VT_LOOKUP_SIZE 256
#endif

// populate_geneobj_vmap: C# GeneObj constructor populates vmap with 7 subtypes
inline void populate_geneobj_vmap(ThreadGC* thgc, GeneObj* gj) {
	auto _mk = [&](int ctype) -> Obj* {
		Obj* o = (Obj*)GC_alloc(thgc, ctype);
		o->type = LetterType::OT_GeneObj;
		o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
		o->vt = &vt_Obj_default; // A-pass initVTables will set proper vt
		return o;
	};
	GeneNew* gn = (GeneNew*)_mk(CType::_GeneNew); gn->gj = gj;
	add_mapy(thgc, gj->vmap, createString(thgc, (char*)"New", 3, 1), (char*)gn);
	GeneStore* gs = (GeneStore*)_mk(CType::_GeneStore); gs->gj = gj;
	add_mapy(thgc, gj->vmap, createString(thgc, (char*)"Store", 5, 1), (char*)gs);
	GeneSort* gso = (GeneSort*)_mk(CType::_GeneSort); gso->gj = gj;
	add_mapy(thgc, gj->vmap, createString(thgc, (char*)"Sort", 4, 1), (char*)gso);
	GeneMutate* gm = (GeneMutate*)_mk(CType::_GeneMutate); gm->gj = gj;
	add_mapy(thgc, gj->vmap, createString(thgc, (char*)"Mutate", 6, 1), (char*)gm);
	GeneCross* gx = (GeneCross*)_mk(CType::_GeneCross); gx->gj = gj;
	add_mapy(thgc, gj->vmap, createString(thgc, (char*)"Cross", 5, 1), (char*)gx);
	GeneChild* gc2 = (GeneChild*)_mk(CType::_GeneChild); gc2->gj = gj;
	add_mapy(thgc, gj->vmap, createString(thgc, (char*)"Child", 5, 1), (char*)gc2);
	GeneSelect* gse = (GeneSelect*)_mk(CType::_GeneSelect); gse->gj = gj;
	add_mapy(thgc, gj->vmap, createString(thgc, (char*)"Select", 6, 1), (char*)gse);
}

inline void clear_letter(ATSSpan* letter) {
	if (letter != NULL) { letter->error = NULL; letter->underlineColor = 0; }
}

Obj* make_cobj(ThreadGC* thgc, int ctype, LetterType ot, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, ctype);
	o->type = ot;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)ot >= 0 && (int)ot < VT_LOOKUP_SIZE) ? vt_lookup[(int)ot] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	return o;
}

Obj* make_comment(ThreadGC* thgc, ATSSpan* letter) {
	Comment* o = (Comment*)GC_alloc(thgc, CType::_Comment);
	((Obj*)o)->type = LetterType::OT_Comment;
	((Obj*)o)->letter = letter;
	((Obj*)o)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)o)->vt = ((int)LetterType::OT_Comment >= 0 && (int)LetterType::OT_Comment < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Comment] : &vt_Obj_default;
	((Obj*)o)->rename = NULL;
	((Obj*)o)->version = NULL;
	o->vmap = NULL;
	return (Obj*)o;
}

Obj* make_commenttype(ThreadGC* thgc) {
	CommentType* o = (CommentType*)GC_alloc(thgc, CType::_CommentType);
	((Obj*)o)->type = LetterType::OT_CommentType;
	((Obj*)o)->letter = NULL;
	((Obj*)o)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)o)->vt = &vt_Obj_default;
	((Obj*)o)->rename = NULL;
	((Obj*)o)->version = NULL;
	((Type*)o)->cls = NULL;
	((Type*)o)->letter2 = NULL;
	((Type*)o)->initial = false;
	return (Obj*)o;
}

Obj* make_comment2(ThreadGC* thgc, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_Comment2);
	o->type = LetterType::OT_Comment2;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::OT_Comment2 >= 0 && (int)LetterType::OT_Comment2 < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Comment2] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	return o;
}

Obj* make_function(ThreadGC* thgc, LetterType ot, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_Function);
	o->type = ot;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)ot >= 0 && (int)ot < VT_LOOKUP_SIZE) ? vt_lookup[(int)ot] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	((Function*)o)->blocks = NULL;
	((Function*)o)->draw = NULL;
	((Function*)o)->rettype = NULL;
	((Function*)o)->bracket = NULL;
	((Function*)o)->block = NULL;
	((Function*)o)->entry = NULL;
	((Function*)o)->end = NULL;
	return o;
}

Obj* make_htmobj(ThreadGC* thgc, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_HtmObj);
	o->type = LetterType::OT_Htm;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::OT_Htm >= 0 && (int)LetterType::OT_Htm < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Htm] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	return o;
}

Obj* make_number(ThreadGC* thgc, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_CNumber);
	o->type = LetterType::OT_Number;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::OT_Number >= 0 && (int)LetterType::OT_Number < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Number] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	((Number*)o)->value = 0;
	return o;
}

Obj* make_floatval(ThreadGC* thgc, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_FloatVal);
	o->type = LetterType::OT_FloatVal;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::OT_FloatVal >= 0 && (int)LetterType::OT_FloatVal < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_FloatVal] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	((FloatVal*)o)->value = 0.0f;
	return o;
}

Obj* make_boolval(ThreadGC* thgc, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_CBoolVal);
	o->type = LetterType::OT_BoolVal;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::OT_BoolVal >= 0 && (int)LetterType::OT_BoolVal < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_BoolVal] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	return o;
}

Obj* make_variable(ThreadGC* thgc, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_CVariable);
	o->type = LetterType::OT_Variable;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::OT_Variable >= 0 && (int)LetterType::OT_Variable < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Variable] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	((Variable*)o)->value = NULL;
	((Variable*)o)->cls = NULL;
	((Variable*)o)->left = NULL;
	((Variable*)o)->right = NULL;
	((Variable*)o)->comment = NULL;
	((Variable*)o)->clone = false;
	((Variable*)o)->order = 0;
	((Variable*)o)->strvari = NULL;
	return o;
}

// make_stocktype: C# new StockType()
inline Obj* make_stocktype(ThreadGC* thgc) {
	Type* t = (Type*)GC_alloc(thgc, CType::_CVar);
	((Obj*)t)->type = LetterType::OT_StockType;
	((Obj*)t)->letter = NULL;
	((Obj*)t)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)t)->vt = &vt_Obj_default;
	((Obj*)t)->rename = NULL;
	((Obj*)t)->version = NULL;
	t->cls = NULL;
	t->letter2 = NULL;
	t->initial = false;
	return (Obj*)t;
}

// make_addresstype: C# new AddressType()
inline Obj* make_addresstype(ThreadGC* thgc) {
	Type* t = (Type*)GC_alloc(thgc, CType::_CVar);
	((Obj*)t)->type = LetterType::OT_AddressType;
	((Obj*)t)->letter = NULL;
	((Obj*)t)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)t)->vt = &vt_Obj_default;
	((Obj*)t)->rename = NULL;
	((Obj*)t)->version = NULL;
	t->cls = NULL;
	t->letter2 = NULL;
	t->initial = false;
	return (Obj*)t;
}

Obj* make_var(ThreadGC* thgc, ATSSpan* letter) {
	Var* v = (Var*)GC_alloc(thgc, CType::_CVar);
	Obj* o = (Obj*)v;
	o->type = LetterType::OT_Var;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::OT_Var >= 0 && (int)LetterType::OT_Var < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Var] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	v->cls = NULL;
	v->letter2 = NULL;
	v->initial = false;
	v->connects = create_list(thgc, sizeof(Var*), CType::_CVar);
	return o;
}

Obj* make_address(ThreadGC* thgc, ATSSpan* letter) {
	Obj* o = (Obj*)GC_alloc(thgc, CType::_Address);
	o->type = LetterType::_AtLetter;
	o->letter = letter;
	o->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	o->vt = ((int)LetterType::_AtLetter >= 0 && (int)LetterType::_AtLetter < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::_AtLetter] : &vt_Obj_default;
	o->rename = NULL;
	o->version = NULL;
	((Address*)o)->address = letter ? letter->text : NULL;
	((Address*)o)->draw = NULL;
	return o;
}

// ElemObj : Obj — C# ElemObj : Obj { String key, ElemType type, Element elem, String id }
struct ElemObj : Obj {
	LetterType key;    // C# String key — "+", "!", "*" → LetterType
	Obj* type;         // C# ElemType type
	void* elem;        // C# Element elem (Div/Span/etc)
	String* id;        // C# String id
	LetterType sop;    // C# (elem as Div).sop — Div未移植のためElemObjに保持
};

inline void ElemObjCheck(ThreadGC* thgc, char* data) {
	ElemObj* e = (ElemObj*)data;
	traceObj(thgc, (Obj*)e);
	e->type = (Obj*)   copy_object_young(thgc, (char*)e->type);
	e->id   = (String*)copy_object_young(thgc, (char*)e->id);
	// elem は Element/Div で型が確定しないため、ここではトレースしない
}

ElemObj* make_elemobj(ThreadGC* thgc, Obj* self, ATSSpan* letter) {
	ElemObj* o = (ElemObj*)GC_alloc(thgc, CType::_CElemObj);
	((Obj*)o)->type = LetterType::OT_ElemObj;
	((Obj*)o)->letter = letter;
	((Obj*)o)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)o)->vt = &vt_ElemObj;
	((Obj*)o)->rename = NULL;
	((Obj*)o)->version = NULL;
	o->key = (LetterType)0;
	o->type = self;
	o->elem = NULL;
	o->id = (letter != NULL) ? letter->text : NULL;
	o->sop = (LetterType)0;
	return o;
}

// make_val: allocate a Val-sized object (has cls, vmap fields)
Val* make_val(ThreadGC* thgc, int ctype, LetterType ot, ATSSpan* letter) {
	Val* v = (Val*)GC_alloc(thgc, ctype);
	((Obj*)v)->type = ot;
	((Obj*)v)->letter = letter;
	((Obj*)v)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)v)->vt = ((int)ot >= 0 && (int)ot < VT_LOOKUP_SIZE) ? vt_lookup[(int)ot] : &vt_Obj_default;
	((Obj*)v)->rename = NULL;
	((Obj*)v)->version = NULL;
	v->cls = NULL;
	v->vmap = NULL;
	return v;
}

inline Obj* make_voival(ThreadGC* thgc) {
	Val* v = make_val(thgc, CType::_CVal, LetterType::OT_VoiVal, NULL);
	return (Obj*)v;
}

// ConnectStock : Stock — C# ConnectStock : Stock { SignalFunction value, Address address }
struct ConnectStock : Stock {
	Obj* value;  // C# SignalFunction value
};

inline void ConnectStockCheck(ThreadGC* thgc, char* data) {
	ConnectStock* c = (ConnectStock*)data;
	traceObj(thgc, (Obj*)c);
	((Stock*)c)->vmap = (Map*)copy_object_young(thgc, (char*)((Stock*)c)->vmap);
	c->value = (Obj*)copy_object_young(thgc, (char*)c->value);
}

// make_connectstock: C# new ConnectStock(sf)
inline ConnectStock* make_connectstock(ThreadGC* thgc, Obj* sf) {
	ConnectStock* cs = (ConnectStock*)GC_alloc(thgc, CType::_CConnectStock);
	((Obj*)cs)->type = LetterType::OT_ConnectStock;
	((Obj*)cs)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)cs)->vt = vt_lookup[(int)LetterType::OT_ConnectStock];
	((Obj*)cs)->rename = NULL;
	((Obj*)cs)->version = NULL;
	((Stock*)cs)->vmap = create_mapy(thgc, CType::_List);
	cs->value = sf;
	return cs;
}

// make_stock: create a Stock object
inline Stock* make_stock(ThreadGC* thgc) {
	Stock* s = (Stock*)GC_alloc(thgc, CType::_Stock);
	((Obj*)s)->type = LetterType::OT_Stock;
	((Obj*)s)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)s)->vt = &vt_Obj_default;
	((Obj*)s)->rename = NULL;
	((Obj*)s)->version = NULL;
	s->vmap = create_mapy(thgc, CType::_List);
	return s;
}

// Break : Obj — C# Break : Obj { Obj value }
struct Break : Obj {
	Obj* value;
};

inline void BreakCheck(ThreadGC* thgc, char* data) {
	struct Break* br = (struct Break*)data;
	traceObj(thgc, (Obj*)br);
	br->value = (Obj*)copy_object_young(thgc, (char*)br->value);
}

// Continue : Obj — C# Continue : Obj { Obj value }
struct Continue : Obj {
	Obj* value;
};

inline void ContinueCheck(ThreadGC* thgc, char* data) {
	Continue* c = (Continue*)data;
	traceObj(thgc, (Obj*)c);
	c->value = (Obj*)copy_object_young(thgc, (char*)c->value);
}

// Return : Obj — C# Return : Obj { Obj value }
struct Return : Obj {
	Obj* value;
};

inline void ReturnCheck(ThreadGC* thgc, char* data) {
	Return* r = (Return*)data;
	traceObj(thgc, (Obj*)r);
	r->value = (Obj*)copy_object_young(thgc, (char*)r->value);
}

// Clones : Obj — C# Clones : Obj { List<Letter[]> objs }
struct Clones : Obj {
	List* objs; // List of ATSSpan*[]
};

inline void ClonesCheck(ThreadGC* thgc, char* data) {
	Clones* c = (Clones*)data;
	traceObj(thgc, (Obj*)c);
	c->objs = (List*)copy_object_young(thgc, (char*)c->objs);
}

// Clones_make: C# new Clones() { letter = ... }
Clones* Clones_make(ThreadGC* thgc, ATSSpan* letter) {
	Clones* c = (Clones*)GC_alloc(thgc, CType::_CClones);
	((Obj*)c)->type = LetterType::OT_Clones;
	((Obj*)c)->letter = letter;
	((Obj*)c)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)c)->vt = vt_lookup[(int)LetterType::OT_Clones];
	((Obj*)c)->rename = NULL;
	((Obj*)c)->version = NULL;
	c->objs = create_list(thgc, sizeof(ATSSpan*), CType::_CObj);
	return c;
}

// AnimationFunction : Obj — C# AnimationFunction : Obj { float interval, int loop, SortedList<int,Block> imap, Div div, int start }
struct AnimationFunction : Obj {
	float interval;
	int loop;
	Map* imap;    // SortedList<int, Block>
	void* div;    // Div*
	int start;
};

inline void AnimationFunctionCheck(ThreadGC* thgc, char* data) {
	AnimationFunction* a = (AnimationFunction*)data;
	traceObj(thgc, (Obj*)a);
	a->imap = (Map*)copy_object_young(thgc, (char*)a->imap);
	// div は Div* — 未移植のためここではトレースしない
}

// AnimationFunction_make: C# new AnimationFunction()
AnimationFunction* AnimationFunction_make(ThreadGC* thgc) {
	AnimationFunction* a = (AnimationFunction*)GC_alloc(thgc, CType::_CAnimationFunction);
	((Obj*)a)->type = LetterType::OT_AnimationFunction;
	((Obj*)a)->letter = NULL;
	((Obj*)a)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)a)->vt = vt_lookup[(int)LetterType::OT_AnimationFunction];
	((Obj*)a)->rename = NULL;
	((Obj*)a)->version = NULL;
	a->interval = 0;
	a->loop = 1;
	a->imap = NULL;
	a->div = NULL;
	a->start = 0;
	return a;
}

// Goto : Obj — C# Goto : Obj { String value }
struct Goto : Obj {
	String* value;
};

inline void GotoCheck(ThreadGC* thgc, char* data) {
	Goto* g = (Goto*)data;
	traceObj(thgc, (Obj*)g);
	g->value = (String*)copy_object_young(thgc, (char*)g->value);
}

// Iterator : Obj — C# Iterator : Obj { Block value, int m, int n }
struct Iterator : Obj {
	Block* value;
	int m;
	int n;
};

inline void IteratorCheck(ThreadGC* thgc, char* data) {
	Iterator* i = (Iterator*)data;
	traceObj(thgc, (Obj*)i);
	i->value = (Block*)copy_object_young(thgc, (char*)i->value);
}

// make_iterator: create an Iterator object
inline Obj* make_iterator(ThreadGC* thgc, ATSSpan* letter = NULL) {
	Iterator* it = (Iterator*)GC_alloc(thgc, CType::_CIterator);
	((Obj*)it)->type = LetterType::OT_Iterator;
	((Obj*)it)->letter = letter;
	((Obj*)it)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)it)->vt = ((int)LetterType::OT_Iterator >= 0 && (int)LetterType::OT_Iterator < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Iterator] : &vt_Obj_default;
	((Obj*)it)->rename = NULL;
	((Obj*)it)->version = NULL;
	it->value = NULL;
	it->m = 0;
	it->n = 0;
	return (Obj*)it;
}

// Model : Obj — C# Model : Obj { Dictionary<String, Obj> vmap }
struct Model : Obj {
	Map* vmap;
};

// Gene : Obj — C# Gene : Obj { Dictionary<String, Obj> vmap, GeneObj gj }
struct Gene : Obj {
	Map* vmap;
	GeneObj* gj;
};

// Operator : Obj — C# Operator : Obj { string name }
struct Operator : Obj {
	String* name;
};
inline void OperatorCheck(ThreadGC* thgc, char* data) {
	Operator* o = (Operator*)data;
	traceObj(thgc, (Obj*)o);
	o->name = (String*)copy_object_young(thgc, (char*)o->name);
}
inline Goto* make_goto(ThreadGC* thgc, ATSSpan* letter) {
	Goto* g = (Goto*)GC_alloc(thgc, CType::_CGoto);
	((Obj*)g)->type = LetterType::OT_Goto;
	((Obj*)g)->letter = letter;
	((Obj*)g)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)g)->vt = ((int)LetterType::OT_Goto >= 0 && (int)LetterType::OT_Goto < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Goto] : &vt_Obj_default;
	((Obj*)g)->rename = NULL;
	((Obj*)g)->version = NULL;
	g->value = NULL;
	return g;
}

inline StrObj* make_strobj(ThreadGC* thgc, ATSSpan* letter) {
	StrObj* s = (StrObj*)GC_alloc(thgc, CType::_StrObj);
	((Obj*)s)->type = LetterType::OT_StrObj;
	((Obj*)s)->letter = letter;
	((Obj*)s)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)s)->vt = ((int)LetterType::OT_StrObj >= 0 && (int)LetterType::OT_StrObj < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_StrObj] : &vt_Obj_default;
	((Obj*)s)->rename = NULL;
	((Obj*)s)->version = NULL;
	((Val*)s)->cls = NULL;
	s->value = letter ? letter->text : NULL;
	return s;
}

inline Word* make_word(ThreadGC* thgc, ATSSpan* letter) {
	Word* w = (Word*)GC_alloc(thgc, CType::_Word);
	((Obj*)w)->type = LetterType::OT_Word;
	((Obj*)w)->letter = letter;
	((Obj*)w)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)w)->vt = ((int)LetterType::OT_Word >= 0 && (int)LetterType::OT_Word < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_Word] : &vt_Obj_default;
	((Obj*)w)->rename = NULL;
	((Obj*)w)->version = NULL;
	w->name = letter ? letter->text : NULL;
	return w;
}

// C# blocks property: get => blockslist.Last(), set => blockslist.Add(value)
inline List* get_blocks(Master* local) {
	if (local->blockslist == NULL || local->blockslist->size == 0) return NULL;
	return *(List**)get_list(local->blockslist, local->blockslist->size - 1);
}
inline void set_blocks(ThreadGC* thgc, Master* local, List* newblocks) {
	if (local->blockslist == NULL) {
		local->blockslist = create_list(thgc, sizeof(List*), CType::_List);
	}
	add_list(thgc, local->blockslist, (char*)newblocks);
}

Block* make_cblock(ThreadGC* thgc, int ctype, LetterType ot) {
	Block* b = (Block*)GC_alloc(thgc, ctype);
	b->type = ot;
	b->letter = NULL;
	b->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	b->vt = ((int)ot >= 0 && (int)ot < VT_LOOKUP_SIZE) ? vt_lookup[(int)ot] : &vt_Obj_default;
	b->branchmap = create_mapy(thgc, CType::_List);
	b->labelmap = create_mapy(thgc, CType::_List);
	b->labelmapn = create_mapy(thgc, CType::_List);
	b->branchmapn = create_mapy(thgc, CType::_List);
	b->letter2 = NULL;
	b->vmapA = create_mapy(thgc, CType::_List);
	b->vmapR = create_mapy(thgc, CType::_List);
	b->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	b->obj = make_cobj(thgc, CType::_CObj, LetterType::OT_None, NULL);
	b->obj_n = -1;
	b->letters = NULL;
	b->statuses = NULL;
	b->finished = false;
	b->comment = false;
	b->blocks = create_list(thgc, sizeof(Block*), CType::_Block);
	b->returns_list = create_list(thgc, sizeof(Obj*), CType::_CObj);
	b->childblocks = create_list(thgc, sizeof(Block*), CType::_Block);
	return b;
}

Obj* make_word_change(ThreadGC* thgc, ATSSpan* letter, Master* local) {
	String* name = letter->text;
	if (!name) return (Obj*)make_word(thgc, letter);
	int len = name->size;
	int es = name->esize;
	// 1文字目を取得（ASCII比較用）
	char c0 = (es == 1) ? name->data[0] : name->data[0]; // UTF-16 LEでもASCIIなら下位バイト同じ
	switch (len) {
	case 2: // if
		if (c0 == 'i' && str_equals("if", name))
			return make_cobj(thgc, CType::_CIf, LetterType::OT_If, letter);
		break;
	case 3: // var, for
		if (c0 == 'v' && str_equals("var", name))
			return make_var(thgc, letter);
		if (c0 == 'f' && str_equals("for", name))
			return make_cobj(thgc, CType::_CFor, LetterType::OT_For, letter);
		break;
	case 4: // elif, else, goto, true, gene
		if (c0 == 'e') {
			if (str_equals("elif", name)) return make_cobj(thgc, CType::_CElif, LetterType::OT_Elif, letter);
			if (str_equals("else", name)) return make_cobj(thgc, CType::_CElse, LetterType::OT_Else, letter);
		}
		if (c0 == 'g') {
			if (str_equals("goto", name)) return (Obj*)make_goto(thgc, letter);
			if (str_equals("gene", name)) return make_cobj(thgc, CType::_CGene, LetterType::OT_Gene, letter);
		}
		if (c0 == 't' && str_equals("true", name)) {
			Obj* o = make_boolval(thgc, letter);
			o->number_val = 1;
			((Val*)o)->cls = (Obj*)local->Bool;
			return o;
		}
		break;
	case 5: // class, while, label, print, false, model
		if (c0 == 'c' && str_equals("class", name))
			return make_cobj(thgc, CType::_CClass, LetterType::OT_Class, letter);
		if (c0 == 'w' && str_equals("while", name))
			return make_cobj(thgc, CType::_CWhile, LetterType::OT_While, letter);
		if (c0 == 'l' && str_equals("label", name)) {
			GeneLabel* gl = (GeneLabel*)GC_alloc(thgc, CType::_CGeneLabel);
			((Obj*)gl)->type = LetterType::OT_GeneLabel;
			((Obj*)gl)->letter = letter;
			((Obj*)gl)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
			((Obj*)gl)->vt = ((int)LetterType::OT_GeneLabel >= 0 && (int)LetterType::OT_GeneLabel < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_GeneLabel] : &vt_Obj_default;
			((Type*)gl)->cls = NULL;
			((Type*)gl)->letter2 = NULL;
			((Type*)gl)->initial = false;
			return (Obj*)gl;
		}
		if (c0 == 'p' && str_equals("print", name))
			return make_cobj(thgc, CType::_CPrint, LetterType::OT_Print, letter);
		if (c0 == 'f' && str_equals("false", name)) {
			Obj* o = make_boolval(thgc, letter);
			o->number_val = 0;
			((Val*)o)->cls = (Obj*)local->Bool;
			return o;
		}
		if (c0 == 'm' && str_equals("model", name))
			return make_cobj(thgc, CType::_CModel, LetterType::OT_Model, letter);
		break;
	case 6: // switch, return, client, server, signal
		if (c0 == 's') {
			if (str_equals("switch", name)) return make_cobj(thgc, CType::_CSwitch, LetterType::OT_Switch, letter);
			if (str_equals("signal", name)) return make_cobj(thgc, CType::_CSignal, LetterType::OT_Signal, letter);
			if (str_equals("server", name)) {
				ServerClient* sc = (ServerClient*)GC_alloc(thgc, CType::_CServerClient);
				sc->type = LetterType::OT_ServerClient;
				sc->letter = letter;
				sc->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
				sc->vt = ((int)LetterType::OT_ServerClient >= 0 && (int)LetterType::OT_ServerClient < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_ServerClient] : &vt_Obj_default;
				sc->name = createString(thgc, "server", 6, 1);
				return (Obj*)sc;
			}
		}
		if (c0 == 'r' && str_equals("return", name))
			return make_cobj(thgc, CType::_CReturn, LetterType::OT_Return, letter);
		if (c0 == 'c' && str_equals("client", name)) {
			ServerClient* sc = (ServerClient*)GC_alloc(thgc, CType::_CServerClient);
			sc->type = LetterType::OT_ServerClient;
			sc->letter = letter;
			sc->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
			sc->vt = ((int)LetterType::OT_ServerClient >= 0 && (int)LetterType::OT_ServerClient < VT_LOOKUP_SIZE) ? vt_lookup[(int)LetterType::OT_ServerClient] : &vt_Obj_default;
			sc->name = createString(thgc, "client", 6, 1);
			return (Obj*)sc;
		}
		break;
	case 8: // continue
		if (c0 == 'c' && str_equals("continue", name))
			return make_cobj(thgc, CType::_CContinue, LetterType::OT_Continue, letter);
		break;
	}
	// default: return as Word
	return (Obj*)make_word(thgc, letter);
}

// make_block: create a Block with the given LetterType
inline Block* make_block(ThreadGC* thgc, LetterType ot) {
	return make_cblock(thgc, CType::_Block, ot);
}

// vmap helper functions
inline bool vmap_containsA(Map* vmap, String* key) {
	return get_mapy(vmap, key) != NULL;
}

inline bool vmap_containsA(List* vmap, String* key) {
	for (int i = 0; i < vmap->size; i++) {
		KV* kv = (KV*)get_list(vmap, i);
		if (str_equals(kv->key, key)) return true;
	}
	return false;
}

inline Obj* vmap_getA(Map* vmap, String* key) {
	return (Obj*)get_mapy(vmap, key);
}

inline Obj* vmap_getA(List* vmap, String* key) {
	for (int i = 0; i < vmap->size; i++) {
		KV* kv = (KV*)get_list(vmap, i);
		if (str_equals(kv->key, key)) return (Obj*)kv->value;
	}
	return NULL;
}

// add_kouho: add a completion candidate to the kouho list
inline void add_kouho(ThreadGC* thgc, List* kouhos, String* key, Obj* value) {
	KV* kv = (KV*)GC_alloc(thgc, CType::_KV);
	kv->key = key;
	kv->value = (char*)value;
	add_list(thgc, kouhos, (char*)kv);
}

// Kouho functions (forward declarations)
void KouhoSet(ThreadGC* thgc, Master* local);
void KouhoSet2(ThreadGC* thgc, Master* local);
void KouhoSetClaGen(ThreadGC* thgc, Master* local);
void KouhoSetType(ThreadGC* thgc, Master* local);
void KouhoSetFunction(ThreadGC* thgc, Master* local);
void KouhoSetLabel(ThreadGC* thgc, Master* local);
void KouhoSetModelGene(ThreadGC* thgc, Master* local);
void KouhoSetSignal(ThreadGC* thgc, Master* local);
void KouhoSetElem(ThreadGC* thgc, Master* local);

// getB2: C# local.getB2(CallBlock call) => vmapA2[call.n]
inline Obj* getB2(Master* local, CallBlock* call) {
	int idx = ((Obj*)call)->number_val;
	return (Obj*)get_mapyn(local->vmapA2, (char*)(intptr_t)idx);
}

// getC2: C# local.getC2(CallBlock call) => vmapA2[call.n]
inline Obj* getC2(Master* local, CallBlock* call) {
	int idx = ((Obj*)call)->number_val;
	return (Obj*)get_mapyn(local->vmapA2, (char*)(intptr_t)idx);
}

// call_last: invoke the last function pointer in the calls list
inline void call_last(List* calls, ThreadGC* thgc, Master* local) {
	if (calls->size > 0) {
		void (*fn)(ThreadGC*, Master*) = *(void (**)(ThreadGC*, Master*))get_list(calls, calls->size - 1);
		if (fn) fn(thgc, local);
	}
}

// OneMigrate / Migrate helpers
inline OneMigrate* make_onemigrate(ThreadGC* thgc) {
	OneMigrate* om = (OneMigrate*)GC_alloc(thgc, CType::_OneMigrate);
	om->fulltext = createString(thgc, (char*)"", 0, 1);
	om->models = create_mapy(thgc, CType::_KV);
	om->tables = create_mapy(thgc, CType::_KV);
	return om;
}
inline MigrateObj* make_migrate(ThreadGC* thgc) {
	MigrateObj* m = (MigrateObj*)GC_alloc(thgc, CType::_Migrate);
	m->migrations = create_list(thgc, sizeof(OneMigrate*), CType::_OneMigrate);
	return m;
}
inline void Start_migrate(ThreadGC* thgc, MigrateObj* mig) {
	mig->migrations = create_list(thgc, sizeof(OneMigrate*), CType::_OneMigrate);
	OneMigrate* om = make_onemigrate(thgc);
	add_list(thgc, mig->migrations, (char*)om);
}

// C# OneMigrate.AddModel — builds fulltext from model fields
static void AddField_onemigrate(ThreadGC* thgc, String** ft, Variable* vari, Master* local) {
	if (vari->cls == (Obj*)local->Int)        *ft = StringAdd2(thgc,*ft, createString(thgc, (char*)" @int ", 6, 1));
	else if (vari->cls == (Obj*)local->Short)  *ft = StringAdd2(thgc,*ft, createString(thgc, (char*)" @short ", 8, 1));
	else if (vari->cls == (Obj*)local->FloatT)  *ft = StringAdd2(thgc,*ft, createString(thgc, (char*)" @float ", 8, 1));
	else if (vari->cls == (Obj*)local->Bool)   *ft = StringAdd2(thgc,*ft, createString(thgc, (char*)" @bool ", 7, 1));
	else if (vari->cls == (Obj*)local->StrT)    *ft = StringAdd2(thgc,*ft, createString(thgc, (char*)" @string ", 9, 1));
	else if (vari->cls->type == LetterType::OT_ArrayType) { return; }
	else if (vari->cls->type == LetterType::OT_ModelObj) {
		*ft = StringAdd2(thgc,*ft, createString(thgc, (char*)" @", 2, 1));
		*ft = StringAdd2(thgc,*ft, vari->cls->letter->text);
		*ft = StringAdd2(thgc,*ft, createString(thgc, (char*)" ", 1, 1));
	} else return;
	*ft = StringAdd2(thgc,*ft, ((Obj*)vari)->letter->text);
	*ft = StringAdd2(thgc,*ft, createString(thgc, (char*)">", 1, 1));
	*ft = StringAdd2(thgc,*ft, ((Obj*)vari)->rename);
	*ft = StringAdd2(thgc,*ft, createString(thgc, (char*)"\n", 1, 1));
}

inline void AddModel_migrate(ThreadGC* thgc, MigrateObj* mig, String* name, Obj* model, Master* local) {
	// C#: if (migrations.Count == 1) migrations.Last().AddModel(name, model, null, local);
	//     else migrations.Last().AddModel(name, model, migrations[Count - 2], local);
	OneMigrate* om = *(OneMigrate**)get_list(mig->migrations, mig->migrations->size - 1);
	OneMigrate* before = NULL;
	if (mig->migrations->size > 1) {
		before = *(OneMigrate**)get_list(mig->migrations, mig->migrations->size - 2);
	}
	ModelObj* mo = (ModelObj*)model;
	add_mapy(thgc, om->models, name, (char*)mo);
	// fulltext += "@" + model.letter.name + ">" + model.rename + "^" + model.version + "\n"
	om->fulltext = StringAdd2(thgc,om->fulltext, createString(thgc, (char*)"@", 1, 1));
	om->fulltext = StringAdd2(thgc,om->fulltext, model->letter->text);
	om->fulltext = StringAdd2(thgc,om->fulltext, createString(thgc, (char*)">", 1, 1));
	om->fulltext = StringAdd2(thgc,om->fulltext, model->rename);
	om->fulltext = StringAdd2(thgc,om->fulltext, createString(thgc, (char*)"^", 1, 1));
	om->fulltext = StringAdd2(thgc,om->fulltext, model->version);
	om->fulltext = StringAdd2(thgc,om->fulltext, createString(thgc, (char*)"\n", 1, 1));
	// blk1 = model.draw.children[0]
	Block* blk1 = (Block*)*(Obj**)get_list(((ClassObj*)mo)->draw->children, 0);
	for (int i = 0; i < blk1->vmapA->kvs->size; i++) {
		KV* kv = *(KV**)get_list(blk1->vmapA->kvs, i);
		Obj* v = (Obj*)kv->value;
		if (v != NULL && v->type == LetterType::OT_Variable) {
			AddField_onemigrate(thgc, &om->fulltext, (Variable*)v, local);
		}
	}
	// blk2 = model.draw.children[1]
	Block* blk2 = (Block*)*(Obj**)get_list(((ClassObj*)mo)->draw->children, 1);
	for (int i = 0; i < blk2->vmapA->kvs->size; i++) {
		KV* kv = *(KV**)get_list(blk2->vmapA->kvs, i);
		// C#: if (kv.Key == "id") continue;
		if (kv->key && str_equals(kv->key, "id")) continue;
		Obj* v = (Obj*)kv->value;
		if (v != NULL && v->type == LetterType::OT_Variable) {
			AddField_onemigrate(thgc, &om->fulltext, (Variable*)v, local);
		}
	}
}

// make_functype: create a FuncTypeC with given return type (cls)
inline FuncTypeC* make_functype(ThreadGC* thgc, Obj* rettype) {
	FuncTypeC* ft = (FuncTypeC*)GC_alloc(thgc, CType::_FuncTypeC);
	((Obj*)ft)->type = LetterType::OT_FuncType;
	((Obj*)ft)->vt = vt_lookup[(int)LetterType::OT_FuncType];
	((Obj*)ft)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Type*)ft)->cls = rettype;
	ft->nochange = false;
	ft->draws = create_list(thgc, sizeof(Obj*), CType::_CObj);
	return ft;
}

// make_arrtype: create an ArrType with given base type (Type.cls)
inline ArrType* make_arrtype(ThreadGC* thgc, Obj* base) {
	ArrType* at = (ArrType*)GC_alloc(thgc, CType::_ArrType);
	((Obj*)at)->type = LetterType::OT_ArrType;
	((Obj*)at)->vt = vt_lookup[(int)LetterType::OT_ArrType];
	((Obj*)at)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Type*)at)->cls = base;
	at->array = 0;
	at->list = NULL;
	return at;
}

// make_variable: create a Variable with given type
inline Variable* make_variable(ThreadGC* thgc, Obj* cls) {
	Variable* v = (Variable*)GC_alloc(thgc, CType::_Variable);
	((Obj*)v)->type = LetterType::OT_Variable;
	((Obj*)v)->vt = vt_lookup[(int)LetterType::OT_Variable];
	((Obj*)v)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	v->cls = cls;
	v->value = NULL;
	v->left = NULL;
	v->right = NULL;
	v->comment = NULL;
	v->clone = false;
	v->order = 0;
	v->strvari = NULL;
	return v;
}

// make_genericobj: create a GenericObj with given generic
inline GenericObj* make_genericobj(ThreadGC* thgc, Obj* generic) {
	GenericObj* go = (GenericObj*)GC_alloc(thgc, CType::_GenericObj);
	((Obj*)go)->type = LetterType::OT_GenericObj;
	((Obj*)go)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	go->gene = (Generic*)generic;
	go->draws = create_list(thgc, sizeof(Obj*), CType::_CObj);
	go->extends_list = NULL;
	return go;
}

// make_genericfunction: create a GenericFunction with given return type
inline GenericFunction* make_genericfunction(ThreadGC* thgc, Obj* rettype) {
	GenericFunction* gf = (GenericFunction*)GC_alloc(thgc, CType::_GenericFunction);
	gf->type = LetterType::OT_GenericFunction;
	gf->vt = vt_lookup[(int)LetterType::OT_GenericFunction];
	gf->rettype = rettype;
	gf->vmap = create_mapy(thgc, CType::_List);
	gf->blocks = create_list(thgc, sizeof(Block*), CType::_Block);
	gf->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	return gf;
}

// make_function: create a Function with given return type
inline Function* make_function(ThreadGC* thgc, Obj* rettype) {
	Function* f = (Function*)GC_alloc(thgc, CType::_Function);
	f->type = LetterType::OT_Function;
	f->vt = vt_lookup[(int)LetterType::OT_Function];
	f->rettype = rettype;
	f->blocks = create_list(thgc, sizeof(Block*), CType::_Block);
	f->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	f->bracket = NULL;
	return f;
}

// make_variclass: create a VariClass with given index
inline VariClass* make_variclass(ThreadGC* thgc, int index) {
	VariClass* vc = (VariClass*)GC_alloc(thgc, CType::_VariClass);
	vc->type = LetterType::OT_VariClass;
	vc->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	vc->n = index;
	return vc;
}

// make_constructor: create a Constructor with given ClassObj
inline Constructor* make_constructor(ThreadGC* thgc, Obj* cls) {
	Constructor* c = (Constructor*)GC_alloc(thgc, CType::_Constructor);
	((Obj*)c)->type = LetterType::OT_Constructor;
	((Obj*)c)->vt = vt_lookup[(int)LetterType::OT_Constructor];
	((Obj*)c)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	c->cls = (ClassObj*)cls;
	return c;
}

// make_arrayconstructor: create an ArrayConstructor with given ArrType
inline ArrayConstructor* make_arrayconstructor(ThreadGC* thgc, Obj* cls) {
	ArrayConstructor* ac = (ArrayConstructor*)GC_alloc(thgc, CType::_ArrayConstructor);
	((Obj*)ac)->type = LetterType::OT_ArrayConstructor;
	((Obj*)ac)->vt = vt_lookup[(int)LetterType::OT_ArrayConstructor];
	((Obj*)ac)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	ac->cls = (ArrType*)cls;
	return ac;
}

// make_sqlstring: create a SqlString (C# SqlString : Val)
inline SqlString* make_sqlstring(ThreadGC* thgc, String* value, ATSSpan* letter) {
	SqlString* s = (SqlString*)GC_alloc(thgc, CType::_SqlString);
	((Obj*)s)->type = LetterType::OT_SqlString;
	((Obj*)s)->vt = vt_lookup[(int)LetterType::OT_SqlString];
	((Obj*)s)->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	((Obj*)s)->letter = letter;
	((Obj*)s)->rename = NULL;
	((Obj*)s)->version = NULL;
	((Val*)s)->cls = NULL;
	((Val*)s)->vmap = NULL;
	s->value = value;
	return s;
}

// C# "val is Type" check — Type and all its subclasses
inline bool isType(Obj* obj) {
	LetterType t = obj->type;
	return t == LetterType::OT_ClassObj || t == LetterType::OT_ModelObj
		|| t == LetterType::OT_GeneObj || t == LetterType::OT_ArrType || t == LetterType::OT_FuncType
		|| t == LetterType::OT_VariClass || t == LetterType::OT_GenericObj || t == LetterType::OT_Var
		|| t == LetterType::OT_GeneLabel;
}

#define IS_WEG_B(obj) ((obj)->type == LetterType::OT_Wait || (obj)->type == LetterType::OT_Error || (obj)->type == LetterType::OT_NG)

// Forward declarations for type checking helpers
inline Obj* CheckCVB_TypeCheck(ThreadGC* thgc, Obj* cls, Obj* val2, CheckType ctype, Master* local);
inline Obj* EqualClassB(ThreadGC* thgc, Obj* cls, Obj* cls2, Master* local, ATSSpan* let);
// Forward declaration for ErrorA (defined in obj1a.h, included later)
Obj* ErrorA(ThreadGC* thgc, LetterType type, ATSSpan* letter, const char* msg);

// VarConnectB: C# TypeCheck.VarConnectB(Var, Type) - propagate type to connected vars
inline void VarConnectB(Var* vartype, Obj* cls) {
	vartype->cls = cls;
	if (vartype->connects == NULL) return;
	for (int i = 0; i < vartype->connects->size; i++) {
		Var* conn = *(Var**)get_list(vartype->connects, i);
		if (conn->cls == NULL) VarConnectB(conn, cls);
	}
}

// ExtendsAddB: C# TypeCheck.ExtendsAddB(Type, List<Type>) - collect all extends recursively
inline void ExtendsAddB(ThreadGC* thgc, Obj* cls, List* list) {
	if (cls == NULL) return;
	if (cls->type == LetterType::OT_ClassObj) {
		ClassObj* cj = (ClassObj*)cls;
		if (cj->extends_list == NULL) return;
		for (int i = 0; i < cj->extends_list->size; i++) {
			add_list(thgc, list, *(char**)get_list(cj->extends_list, i));
		}
		for (int i = 0; i < cj->extends_list->size; i++) {
			ExtendsAddB(thgc, *(Obj**)get_list(cj->extends_list, i), list);
		}
	} else if (cls->type == LetterType::OT_GenericObj) {
		GenericObj* gj = (GenericObj*)cls;
		if (gj->extends_list == NULL) return;
		for (int i = 0; i < gj->extends_list->size; i++) {
			add_list(thgc, list, *(char**)get_list(gj->extends_list, i));
		}
		for (int i = 0; i < gj->extends_list->size; i++) {
			ExtendsAddB(thgc, *(Obj**)get_list(gj->extends_list, i), list);
		}
	} else if (cls->type == LetterType::OT_ModelObj || cls->type == LetterType::OT_GeneObj) {
		ModelObj* gj = (ModelObj*)cls;
		if (gj->extends_list == NULL) return;
		for (int i = 0; i < gj->extends_list->size; i++) {
			add_list(thgc, list, *(char**)get_list(gj->extends_list, i));
		}
		for (int i = 0; i < gj->extends_list->size; i++) {
			ExtendsAddB(thgc, *(Obj**)get_list(gj->extends_list, i), list);
		}
	}
}

// ExtendCompareB: C# TypeCheck.ExtendCompareB(Type, Type, Local) - find common base type
inline Obj* ExtendCompareB(ThreadGC* thgc, Obj* cls1, Obj* cls2, Master* local) {
	List* list1 = create_list(thgc, sizeof(Obj*), CType::_CObj);
	List* list2 = create_list(thgc, sizeof(Obj*), CType::_CObj);
	Var* vartype = NULL;
	if (cls1->type == LetterType::OT_Var) {
		vartype = (Var*)cls1;
		cls1 = vartype->cls;
	}
	if (cls1 == NULL || cls2 == NULL) {
		if (vartype != NULL) vartype->cls = (Obj*)local->Object;
		return (Obj*)local->Object;
	}
	add_list(thgc, list1, (char*)cls1);
	ExtendsAddB(thgc, cls1, list1);
	add_list(thgc, list2, (char*)cls2);
	ExtendsAddB(thgc, cls2, list2);
	for (int i = 0; i < list1->size; i++) {
		for (int j = 0; j < list2->size; j++) {
			Obj* l1 = *(Obj**)get_list(list1, i);
			Obj* l2 = *(Obj**)get_list(list2, j);
			if (l1->type == LetterType::OT_ClassObj && l2->type == LetterType::OT_ClassObj) {
				if (l1 == l2) { if (vartype != NULL) vartype->cls = l2; return l1; }
			} else if (l1->type == LetterType::OT_ModelObj && l2->type == LetterType::OT_ModelObj) {
				if (l1 == l2) { if (vartype != NULL) vartype->cls = l2; return l1; }
			} else if (l1->type == LetterType::OT_GeneObj && l2->type == LetterType::OT_GeneObj) {
				if (l1 == l2) { if (vartype != NULL) vartype->cls = l2; return l1; }
			} else if (l1->type == LetterType::OT_GenericObj && l2->type == LetterType::OT_GenericObj) {
				GenericObj* gj1 = (GenericObj*)l1;
				GenericObj* gj2 = (GenericObj*)l2;
				if (gj1->gene == gj2->gene && gj1->draws->size == gj2->draws->size) {
					bool result = true;
					for (int k = 0; k < gj1->draws->size; k++) {
						Obj* d1 = *(Obj**)get_list(gj1->draws, k);
						Obj* d2 = *(Obj**)get_list(gj2->draws, k);
						if (d1 != d2) result = false;
					}
					if (result) { if (vartype != NULL) vartype->cls = (Obj*)gj1; return (Obj*)gj1; }
				}
			}
		}
	}
	if (vartype != NULL) vartype->cls = (Obj*)local->Object;
	return (Obj*)local->Object;
}

// EqualClassB: C# TypeCheck.EqualClassB(Type, Type, Local, Letter) - check type equality
inline Obj* EqualClassB(ThreadGC* thgc, Obj* cls, Obj* cls2, Master* local, ATSSpan* let) {
	Var* vart = NULL;
	Var* vart2 = NULL;
	if (cls->type == LetterType::OT_Var) { vart = (Var*)cls; if (vart->cls != NULL) cls = vart->cls; }
	if (cls2->type == LetterType::OT_Var) { vart2 = (Var*)cls2; if (vart2->cls != NULL) cls2 = vart2->cls; }
	if (cls->type == LetterType::OT_Var) {
		Var* vartype = (Var*)cls;
		if (cls2->type == LetterType::OT_Var) {
			Var* vartype2 = (Var*)cls2;
			add_list(thgc, vartype2->connects, (char*)vartype);
			add_list(thgc, vartype->connects, (char*)vartype2);
			return make_cobj(thgc, CType::_CObj, LetterType::OT_Wait, NULL);
		} else {
			if (vart2 != NULL) cls2 = vart2->cls;
			VarConnectB(vartype, cls2);
			return cls;
		}
	} else if (cls2->type == LetterType::OT_Var) {
		if (vart != NULL) cls = vart->cls;
		Var* vartype2 = (Var*)cls2;
		VarConnectB(vartype2, cls);
		return cls;
	} else if (cls->type == LetterType::OT_ArrType) {
		if (cls2->type == LetterType::OT_ArrType) return EqualClassB(thgc, ((ArrType*)cls)->cls, ((ArrType*)cls2)->cls, local, let);
		else return ErrorA(thgc, LetterType::OT_Error, let, "型が合ってない");
	} else if (cls->type == LetterType::OT_FuncType) {
		FuncTypeC* functype = (FuncTypeC*)cls;
		if (cls2->type == LetterType::OT_FuncType) {
			FuncTypeC* functype2 = (FuncTypeC*)cls2;
			bool wait = false;
			Obj* ret = EqualClassB(thgc, functype->cls, functype2->cls, local, let);
			if (ret->type == LetterType::OT_Wait) wait = true;
			else if (ret->type == LetterType::OT_Error) return ret;
			if (functype->draws->size != functype2->draws->size) return ErrorA(thgc, LetterType::OT_Error, let, "関数型の引数の数が合わない");
			for (int i = 0; i < functype->draws->size; i++) {
				ret = EqualClassB(thgc, *(Obj**)get_list(functype->draws, i), *(Obj**)get_list(functype2->draws, i), local, let);
				if (ret->type == LetterType::OT_Wait) wait = true;
				else if (ret->type == LetterType::OT_Error) return ret;
			}
			if (wait) return make_cobj(thgc, CType::_CObj, LetterType::OT_Wait, NULL);
			return cls;
		} else return ErrorA(thgc, LetterType::OT_Error, let, "関数型が適切です");
	} else if (cls->type == LetterType::OT_CommentType) {
		if (cls2->type == LetterType::OT_CommentType) return cls;
		else return ErrorA(thgc, LetterType::OT_Error, let, "型が構造コメントタイプである必要があります");
	} else if (cls->type == LetterType::OT_GenericObj) {
		GenericObj* gj = (GenericObj*)cls;
		if (cls2->type == LetterType::OT_GenericObj) {
			GenericObj* gj2 = (GenericObj*)cls2;
			if (gj->gene != gj2->gene) return ErrorA(thgc, LetterType::OT_Error, let, "ジェネリックスが違います");
			if (gj->draws->size != gj2->draws->size) return ErrorA(thgc, LetterType::OT_Error, let, "ジェネリックスの引数の数が違います");
			for (int i = 0; i < gj->draws->size; i++) {
				Obj* ret = EqualClassB(thgc, *(Obj**)get_list(gj->draws, i), *(Obj**)get_list(gj2->draws, i), local, let);
				if (ret->type == LetterType::OT_Wait || ret->type == LetterType::OT_Error) return ret;
			}
			return cls;
		} else return ErrorA(thgc, LetterType::OT_Error, let, "型が違います");
	} else if (cls->type == LetterType::OT_ClassObj) {
		if (cls2->type == LetterType::OT_ClassObj) { if (cls != cls2) return ErrorA(thgc, LetterType::OT_Error, let, "型が違います"); return cls; }
		else return ErrorA(thgc, LetterType::OT_Error, let, "型が違います");
	} else if (cls->type == LetterType::OT_ModelObj) {
		if (cls2->type == LetterType::OT_ModelObj) { if (cls != cls2) return ErrorA(thgc, LetterType::OT_Error, let, "型が違います"); return cls; }
		else return ErrorA(thgc, LetterType::OT_Error, let, "型が違います");
	} else if (cls->type == LetterType::OT_GeneObj) {
		if (cls2->type == LetterType::OT_GeneObj) { if (cls != cls2) return ErrorA(thgc, LetterType::OT_Error, let, "型が違います"); return cls; }
		else return ErrorA(thgc, LetterType::OT_Error, let, "型が違います");
	} else return ErrorA(thgc, LetterType::OT_Error, let, "不正なエラーです");
}

// CheckCVB_Func_TypeCheck: type checking for functions (FuncType variant)
// C# CheckCVB FuncType branch: validates Function against FuncTypeC
inline Obj* CheckCVB_Func_TypeCheck(ThreadGC* thgc, FuncTypeC* ftype, Function* func, CheckType ctype, Master* local) {
	bool wait = false;
	Obj* ret = EqualClassB(thgc, ftype->cls, func->rettype, local, ((Obj*)func)->letter);
	if (ret->type == LetterType::OT_Wait) wait = true;
	else if (ret->type == LetterType::OT_Error) return ret;
	Block* blk = *(Block**)get_list(func->draw->children, 0);
	List* varray = map_values_list(thgc, blk->vmapA);
	if (ftype->draws->size != varray->size) return ErrorA(thgc, LetterType::OT_Error, ((Obj*)func)->letter, "関数の引数があっていません");
	for (int i = 0; i < varray->size; i++) {
		Obj* vi = *(Obj**)get_list(varray, i);
		if (vi->type == LetterType::OT_Variable) {
			Variable* variable = (Variable*)vi;
			ret = EqualClassB(thgc, *(Obj**)get_list(ftype->draws, i), variable->cls, local, vi->letter);
			if (ret->type == LetterType::OT_Wait) wait = true;
			else if (ret->type == LetterType::OT_Error) return ret;
		} else return ErrorA(thgc, LetterType::OT_Error, vi->letter, "関数の引数は変数でなければなりません");
	}
	if (wait) return make_cobj(thgc, CType::_CObj, LetterType::OT_Wait, NULL);
	return (Obj*)ftype;
}

// New_Value: C# Value.New(Type cls, Local local, Letter letter) - forward declaration
inline Obj* New_Value(ThreadGC* thgc, Obj* cls, Master* local, ATSSpan* letter);

// CheckCVB_TypeCheck: type checking (Type cls, Obj val2) - C# TypeCheck.CheckCVB(Type, Obj, CheckType, Local)
inline Obj* CheckCVB_TypeCheck(ThreadGC* thgc, Obj* cls, Obj* val2, CheckType ctype, Master* local) {
	Var* vartype2 = NULL;
	// Resolve Var if it has a cls
	if (cls->type == LetterType::OT_Var) {
		Var* vartype = (Var*)cls;
		if (vartype->cls != NULL) {
			vartype2 = vartype;
			cls = vartype->cls;
		}
	}
	if (cls->type == LetterType::OT_ArrType) {
		ArrType* arrtype = (ArrType*)cls;
		if (val2->type == LetterType::OT_Block || val2->type == LetterType::OT_Array) {
			Obj* arrcls = arrtype->cls;
			Block* block = (Block*)val2;
			if (val2->children->size == 0) return arrcls;
			else if (block->cls == NULL) {
				if (arrcls->type == LetterType::OT_Var) {
					Var* vartype = (Var*)arrcls;
					if (vartype->cls != NULL) arrcls = vartype->cls;
					else {
						block->cls = (Type*)vartype;
						for (int i = 0; i < block->rets->size; i++) {
							Obj* ret2 = CheckCVB_TypeCheck(thgc, (Obj*)block->cls, *(Obj**)get_list(block->rets, i), ctype, local);
							if (ret2->type == LetterType::OT_Wait || ret2->type == LetterType::OT_Error) return ret2;
						}
						return (Obj*)arrtype;
					}
				}
				else {
					// arrcls is not Var (or was resolved)
					block->cls = (Type*)arrcls;
					for (int i = 0; i < block->rets->size - 1; i++) {
						Obj* ret2 = CheckCVB_TypeCheck(thgc, (Obj*)block->cls, *(Obj**)get_list(block->rets, i), ctype, local);
						if (ret2->type == LetterType::OT_Wait || ret2->type == LetterType::OT_Error) return ret2;
					}
				}
			} else {
				if (arrcls->type == LetterType::OT_Var) {
					Var* vartype = (Var*)arrcls;
					if (vartype->cls == NULL) {
						vartype->cls = (Obj*)block->cls;
						return (Obj*)arrtype;
					}
				}
				if (arrcls->type == LetterType::OT_ArrType || arrcls->type == LetterType::OT_FuncType) {
					Obj* ret2 = EqualClassB(thgc, arrcls, (Obj*)block->cls, local, arrcls->letter);
					if (ret2->type == LetterType::OT_Wait || ret2->type == LetterType::OT_Error) return ret2;
				} else if (ExtendCompareB(thgc, arrcls, (Obj*)block->cls, local) == NULL) {
					return ErrorA(thgc, LetterType::OT_Error, val2->letter, "共通の継承元がありません");
				}
			}
			return (Obj*)arrtype;
		} else if (val2->type == LetterType::OT_Value) {
			Value* value = (Value*)val2;
			Obj* ret2 = EqualClassB(thgc, (Obj*)arrtype, ((Val*)value)->cls, local, val2->letter);
			return ret2;
		} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "配列型は配列を引数にとります");
	} else if (cls->type == LetterType::OT_FuncType) {
		FuncTypeC* functype = (FuncTypeC*)cls;
		if (val2->type == LetterType::OT_Function) {
			return CheckCVB_Func_TypeCheck(thgc, functype, (Function*)val2, ctype, local);
		} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "戻り値は関数になっています");
	} else if (cls->type == LetterType::OT_GenericObj || cls->type == LetterType::OT_ClassObj
		|| cls->type == LetterType::OT_ModelObj || cls->type == LetterType::OT_GeneObj) {
		if (val2->type == LetterType::OT_VoiVal) {
			if (cls == (Obj*)local->Void) return cls;
			else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "voidが適切です。");
		} else if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj
			|| val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value
			|| val2->type == LetterType::OT_ModelVal || val2->type == LetterType::OT_GeneVal) {
			Obj* cls2 = ((Val*)val2)->cls;
			if (vartype2 == NULL) {
				return ExtendCompareB(thgc, cls, cls2, local);
			} else {
				return ExtendCompareB(thgc, (Obj*)vartype2, cls2, local);
			}
		} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "値が来ていません");
	} else if (cls->type == LetterType::OT_CommentType) {
		// CommentType branch
		if (val2->type == LetterType::OT_Comment) return cls;
		else if (val2->type == LetterType::OT_SignalFunction) return cls;
		else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "戻り値が構造コメントである必要があります");
	} else if (cls->type == LetterType::OT_Var) {
		// cls is still Var (cls was not resolved above because vartype->cls was NULL)
		Var* vartype = (Var*)cls;
		if (val2->type == LetterType::OT_Block || val2->type == LetterType::OT_Array) {
			Obj* newvar = make_var(thgc, NULL);
			ArrType* arrtype = make_arrtype(thgc, newvar);
			vartype->cls = (Obj*)arrtype;
			Block* block = (Block*)val2;
			if (block->rets->size == 0) {
				return (Obj*)vartype;
			} else if (block->cls == NULL) {
				Var* innervar = (Var*)arrtype->cls;  // the Var inside arrtype
				block->cls = (Type*)innervar;
				for (int i = 0; i < block->rets->size; i++) {
					Obj* ret2 = CheckCVB_TypeCheck(thgc, (Obj*)block->cls, *(Obj**)get_list(block->rets, i), ctype, local);
					if (ret2 == NULL) return ErrorA(thgc, LetterType::OT_Error, val2->letter, "型チェックエラー");
				}
				block->cls = (Type*)innervar->cls;  // resolved inner var's cls
			}
			((Var*)arrtype->cls)->cls = (Obj*)block->cls;
			return (Obj*)vartype;
		} else if (val2->type == LetterType::OT_Function) {
			Function* func = (Function*)val2;
			Obj* retvar = make_var(thgc, NULL);
			((Var*)retvar)->cls = func->rettype;
			FuncTypeC* functype = make_functype(thgc, retvar);
			Block* block1 = *(Block**)get_list(func->draw->children, 0);
			List* vvals = map_values_list(thgc, block1->vmapA);
			for (int i = 0; i < vvals->size; i++) {
				Obj* v = *(Obj**)get_list(vvals, i);
				if (v->type == LetterType::OT_Variable) {
					Variable* variable = (Variable*)v;
					if (variable->cls->type == LetterType::OT_Var) {
						add_list(thgc, functype->draws, (char*)variable->cls);
					} else {
						Obj* wrapvar = make_var(thgc, NULL);
						((Var*)wrapvar)->cls = variable->cls;
						add_list(thgc, functype->draws, (char*)wrapvar);
					}
				}
			}
			vartype->cls = (Obj*)functype;
			return (Obj*)vartype;
		} else if (val2->type == LetterType::OT_Comment) {
			// CommentType
			Obj* ct = make_commenttype(thgc);
			vartype->cls = ct;
			return (Obj*)vartype;
		} else if (val2->type == LetterType::OT_Number || val2->type == LetterType::OT_StrObj
			|| val2->type == LetterType::OT_BoolVal || val2->type == LetterType::OT_Value
			|| val2->type == LetterType::OT_ModelVal || val2->type == LetterType::OT_GeneVal) {
			Obj* cls2 = ((Val*)val2)->cls;
			vartype->cls = cls2;
			return (Obj*)vartype;
		} else if (val2->type == LetterType::OT_VoiVal) {
			vartype->cls = (Obj*)local->Void;
			return (Obj*)vartype;
		} else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "値ではありません");
	} else if (cls->type == LetterType::OT_GeneLabel && val2->type == LetterType::OT_GeneVal) return cls;
	else return ErrorA(thgc, LetterType::OT_Error, val2->letter, "不正なエラー");
}

// CheckB_TypeCheck: type checking for B-pass - C# TypeCheck.CheckB(Obj, Obj, CheckType, Local)
inline Obj* CheckB_TypeCheck(ThreadGC* thgc, Obj* val, Obj* val2, CheckType ctype, Master* local) {
	Obj* cls = NULL;
	if (val->type == LetterType::OT_If) {
		cls = val->value;  // C# If.ret stored in Obj.value
	} else if (val->type == LetterType::OT_While) {
		cls = val->value;  // C# While.ret stored in Obj.value
	} else if (val->type == LetterType::OT_For) {
		cls = val->value;  // C# For.ret stored in Obj.value
	} else if (val->type == LetterType::OT_Switch) {
		cls = val->value;  // C# Switch.ret stored in Obj.value
	} else if (val->type == LetterType::OT_Variable) {
		cls = ((Variable*)val)->cls;
	} else if (val->type == LetterType::OT_Function) {
		cls = ((Function*)val)->rettype;
	} else if (val->type == LetterType::OT_SignalFunction) {
		cls = ((Function*)val)->rettype;
	} else if (val->type == LetterType::OT_ServerFunction) {
		cls = ((Function*)val)->rettype;
	} else {
		return val2; // fallback
	}
	if (val2->type == LetterType::OT_Variable) {
		val2 = New_Value(thgc, ((Variable*)val2)->cls, local, val2->letter);
		if (IS_WEG_B(val2)) return val2;
	}
	return CheckCVB_TypeCheck(thgc, cls, val2, ctype, local);
}

// New_Value: C# Value.New(Type cls, Local local, Letter letter)
// Creates the appropriate value object based on the type
inline Obj* New_Value(ThreadGC* thgc, Obj* cls, Master* local, ATSSpan* letter) {
	// Var → resolve to underlying type
	if (cls->type == LetterType::OT_Var) {
		Obj* varc = ((Var*)cls)->cls;
		if (varc == NULL) {
			Obj* err = make_cobj(thgc, CType::_CObj, LetterType::OT_Wait, letter);
			return err;
		}
		cls = varc;
	}
	// Int → Number
	if (cls == (Obj*)local->Int) {
		Obj* num = (Obj*)GC_alloc(thgc, CType::_CNumber);
		num->type = LetterType::OT_Number;
		num->vt = vt_lookup[(int)LetterType::OT_Number];
		((Val*)num)->cls = cls;
		num->letter = letter;
		return num;
	}
	// Short → Number
	if (cls == (Obj*)local->Short) {
		Obj* num = (Obj*)GC_alloc(thgc, CType::_CNumber);
		num->type = LetterType::OT_Number;
		num->vt = vt_lookup[(int)LetterType::OT_Number];
		((Val*)num)->cls = (Obj*)local->Short;
		num->letter = letter;
		return num;
	}
	// Float → FloatVal
	if (cls == (Obj*)local->FloatT) {
		Obj* fv = (Obj*)GC_alloc(thgc, CType::_CObj);
		fv->type = LetterType::OT_FloatVal;
		fv->vt = vt_lookup[(int)LetterType::OT_FloatVal];
		((Val*)fv)->cls = (Obj*)local->FloatT;
		fv->letter = letter;
		return fv;
	}
	// Create Value
	Obj* value = (Obj*)make_val(thgc, CType::_CValue, LetterType::OT_Value, letter);
	((Val*)value)->cls = cls;
	// Str → Value with empty vmapA
	if (cls == (Obj*)local->StrT) {
		((Val*)value)->vmap = create_mapy(thgc, CType::_List);
		return value;
	}
	// ClassObj → Value with vmapA from draw
	if (cls->type == LetterType::OT_ClassObj) {
		if (((ClassObj*)cls)->draw) {
			Block* blk0 = (Block*)*(Obj**)get_list(((ClassObj*)cls)->draw->children, 0);
			((Value*)value)->vmap0 = blk0->vmapA;
			Block* blk1 = (Block*)*(Obj**)get_list(((ClassObj*)cls)->draw->children, 1);
			((Val*)value)->vmap = blk1->vmapA;
		}
		return value;
	}
	// ModelObj → VoiVal for Void, else ModelVal
	if (cls->type == LetterType::OT_ModelObj) {
		if (cls == (Obj*)local->Void) {
			return make_voival(thgc);
		}
		ModelObj* model = (ModelObj*)cls;
		Obj* mv = (Obj*)make_val(thgc, CType::_CVal, LetterType::OT_ModelVal, letter);
		((Val*)mv)->cls = cls;
		if (model->draw) {
			Block* blk1 = (Block*)*(Obj**)get_list(model->draw->children, 1);
			((Val*)mv)->vmap = blk1->vmapA;
		}
		return mv;
	}
	// GeneObj → GeneVal
	if (cls->type == LetterType::OT_GeneObj) {
		GeneObj* gobj = (GeneObj*)cls;
		Obj* gv = (Obj*)make_val(thgc, CType::_CVal, LetterType::OT_GeneVal, letter);
		((Val*)gv)->cls = cls;
		if (gobj->call_blk && gobj->call_blk->children && gobj->call_blk->children->size > 1) {
			Block* blk1 = (Block*)*(Obj**)get_list(gobj->call_blk->children, 1);
			((Val*)gv)->vmap = blk1->vmapA;
		}
		return gv;
	}
	// ArrayType → Value with empty vmapA
	if (cls->type == LetterType::OT_ArrType) {
		((Val*)value)->vmap = create_mapy(thgc, CType::_List);
		return value;
	}
	// FuncType → Value with empty vmapA
	if (cls->type == LetterType::OT_FuncType) {
		((Val*)value)->vmap = create_mapy(thgc, CType::_List);
		return value;
	}
	// GenericObj → Value with vmapA from generic.draw
	if (cls->type == LetterType::OT_GenericObj) {
		GenericObj* gobj = (GenericObj*)cls;
		if (gobj->gene && gobj->gene->draw) {
			Block* blk1 = (Block*)*(Obj**)get_list(gobj->gene->draw->children, 1);
			((Val*)value)->vmap = blk1->vmapA;
		}
		return value;
	}
	return make_cobj(thgc, CType::_CObj, LetterType::OT_Error, letter);
}

// Block_Sort: sort a block array using a comparison function
// Block_Sort: defined in obj2b.h (needs vt_exeA/GetterA_vt/vt_opeA from objvt.h)
Block* Block_Sort(ThreadGC* thgc, Block* arr, Function* func, Master* local);

// ============================================================
// 前方宣言
// ============================================================
Obj* ParseBlock(ThreadGC* thgc, Master* local, LetterType end, int comments, bool* error);
Block* ParseBlock2(ThreadGC* thgc, Master* local, LetterType finish, int comments, bool* error);
Block* ParseLines(ThreadGC* thgc, Master* local, LetterType sub, LetterType sub2, LetterType sub3, LetterType finish, LetterType type, int comments, bool* error, ATSSpan** outLetter);
Obj* ParseOpe1(ThreadGC* thgc, Master* local, int n, int comments, bool tag, LetterType type, bool* error);
Obj* ParseOperator(ThreadGC* thgc, Master* local, int n, int comments, bool tag, LetterType type, bool* error);
Obj* ParsePrimary(ThreadGC* thgc, Master* local, int comments, LetterType type, bool* error);

// ============================================================
// ParseStart
// ============================================================
Obj* ParseStart(ThreadGC* thgc, Master* local, bool* error) {
	local->analblocks = create_list(thgc, sizeof(Block*), CType::_Block);
	local->Next();
	Block* item = make_cblock(thgc, CType::_CallBlock, LetterType::OT_CallBlock);
	add_list(thgc, item->children, (char*)make_cobj(thgc, CType::_CObj, LetterType::OT_Call1, NULL));
	ATSSpan* outLetter = NULL;
	Block* item2 = ParseLines(thgc, local, LetterType::_Kaigyou, LetterType::_Semicolon, LetterType::_Comma, LetterType::_End, LetterType::OT_Call2, 0, error, &outLetter);
	add_list(thgc, item->children, (char*)item2);
	add_list(thgc, local->analblocks, (char*)item2);
	return (Obj*)item;
}

// ============================================================
// ParseBlock
// ============================================================
Obj* ParseBlock(ThreadGC* thgc, Master* local, LetterType end, int comments, bool* error) {
	Block* item;
	if (end == LetterType::_MoreThan) {
		item = (Block*)GC_alloc(thgc, CType::_TagBlock);
		item->type = LetterType::OT_TagBlock;
		item->letter = NULL;
		item->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
		item->vt = vt_lookup[(int)LetterType::OT_TagBlock];
		item->branchmap = create_mapy(thgc, CType::_List);
		item->labelmap = create_mapy(thgc, CType::_List);
		item->labelmapn = create_mapy(thgc, CType::_List);
		item->letter2 = NULL;
		item->vmapA = create_mapy(thgc, CType::_List);
		item->rets = create_list(thgc, sizeof(Obj*), CType::_CObj);
	} else {
		item = make_cblock(thgc, CType::_CallBlock, LetterType::OT_CallBlock);
	}
head:
	if (local->lasttype == LetterType::_Kaigyou) local->Next();
	ATSSpan* outLetter = NULL;
	Block* item2 = ParseLines(thgc, local, LetterType::_Kaigyou, LetterType::_Comma, LetterType::_Semicolon, LetterType::_Bou, LetterType::OT_Call1, comments, error, &outLetter);
	add_list(thgc, local->analblocks, (char*)item2);
	add_list(thgc, item->children, (char*)item2);
	Block* item3 = ParseBlock2(thgc, local, end, comments, error);
	add_list(thgc, item->children, (char*)item3);
	add_list(thgc, local->analblocks, (char*)item3);
	return (Obj*)item;
}

// ============================================================
// ParseBlock2
// ============================================================
Block* ParseBlock2(ThreadGC* thgc, Master* local, LetterType finish, int comments, bool* error) {
	ATSSpan* outLetter = NULL;
	Block* item = ParseLines(thgc, local, LetterType::_Kaigyou, LetterType::_Semicolon, LetterType::_Comma, finish, LetterType::OT_Call2, comments, error, &outLetter);
	return item;
}

// ============================================================
// ParseLines
// ============================================================
Block* ParseLines(ThreadGC* thgc, Master* local, LetterType sub, LetterType sub2, LetterType sub3, LetterType finish, LetterType type, int comments, bool* error, ATSSpan** outLetter) {
	Block* item = make_cblock(thgc, CType::_Block, type);
	bool tag = false;
	if (finish == LetterType::_MoreThan) tag = true;
head:
	if (local->lasttype == LetterType::_Sharp) {
		clear_letter(local->last);
		local->Next();
		if (local->lasttype == LetterType::_Sharp) {
			clear_letter(local->last);
			local->Next();
			if (local->lasttype == LetterType::_Name || local->lasttype == LetterType::_Number || local->lasttype == LetterType::_Str) {
				String* name = local->last->text;
				ATSSpan* let = local->last;
				clear_letter(local->last);
				local->Next();
				Label* label0 = (Label*)get_mapy(local->labelmap, name);
				if (label0 == NULL) {
					label0 = (Label*)GC_alloc(thgc, CType::_Label);
					label0->letter = let; label0->name = name; label0->n = 0;
					label0->labelmap = create_mapy(thgc, CType::_List);
					add_mapy(thgc, local->labelmap, name, (char*)label0);
				}
				Label* label = (Label*)get_mapy(item->branchmap, name);
				if (label == NULL) {
					label = (Label*)GC_alloc(thgc, CType::_Label);
					label->letter = let; label->name = name; label->n = item->children->size;
					label->labelmap = create_mapy(thgc, CType::_List);
					add_mapy(thgc, item->branchmap, name, (char*)label);
				}
				if (local->lasttype == LetterType::_Dot) {
					clear_letter(local->last);
					local->Next();
					if (local->lasttype == LetterType::_Name || local->lasttype == LetterType::_Number || local->lasttype == LetterType::_Str) {
						String* name2 = local->last->text;
						Label* label00 = (Label*)get_mapy(label0->labelmap, name2);
						if (label00 == NULL) {
							label00 = (Label*)GC_alloc(thgc, CType::_Label);
							label00->letter = local->last; label00->name = name2; label00->n = 0;
							label00->labelmap = create_mapy(thgc, CType::_List);
							add_mapy(thgc, label0->labelmap, name2, (char*)label00);
						}
						Label* label2 = (Label*)get_mapy(label->labelmap, name2);
						if (label2 == NULL) {
							label2 = (Label*)GC_alloc(thgc, CType::_Label);
							label2->letter = local->last; label2->name = name2; label2->n = item->children->size;
							label2->labelmap = create_mapy(thgc, CType::_List);
							add_mapy(thgc, label->labelmap, name2, (char*)label2);
						}
						clear_letter(local->last);
						local->Next();
						if (local->lasttype == LetterType::_Dot) {
							local->Next();
							if (local->lasttype == LetterType::_Name || local->lasttype == LetterType::_Number || local->lasttype == LetterType::_Str) {
								String* name3 = local->last->text;
								Label* label000 = (Label*)get_mapy(label00->labelmap, name);
								if (label000 == NULL) {
									label000 = (Label*)GC_alloc(thgc, CType::_Label);
									label000->letter = local->last; label000->name = name3; label000->n = 0;
									label000->labelmap = create_mapy(thgc, CType::_List);
									add_mapy(thgc, label00->labelmap, name, (char*)label000);
								}
								Label* label3 = (Label*)get_mapy(label2->labelmap, name3);
								if (label3 == NULL) {
									label3 = (Label*)GC_alloc(thgc, CType::_Label);
									label3->letter = local->last; label3->name = name3; label3->n = item->children->size;
									label3->labelmap = create_mapy(thgc, CType::_List);
									add_mapy(thgc, label2->labelmap, name3, (char*)label3);
								}
								clear_letter(local->last);
								local->Next();
							} else {
								local->last->error = createString(thgc, (char*)"Label syntax error", 18, 1);
								local->last->underlineColor = 0xFF0000FF;
								*error = true;
							}
						}
					} else {
						local->last->error = createString(thgc, (char*)"Label syntax error", 18, 1);
						local->last->underlineColor = 0xFF0000FF;
						*error = true;
					}
				}
			}
		}
		else if (local->lasttype == LetterType::_Name || local->lasttype == LetterType::_Number || local->lasttype == LetterType::_Str) {
			String* name = local->last->text;
			ATSSpan* let = local->last;
			clear_letter(let);
			local->Next();
			if (local->lasttype == LetterType::_Dot) {
				clear_letter(local->last);
				local->Next();
				if (local->lasttype == LetterType::_Name || local->lasttype == LetterType::_Number || local->lasttype == LetterType::_Str) {
					Label* label = (Label*)get_mapy(item->labelmap, name);
					if (label == NULL) {
						label = (Label*)GC_alloc(thgc, CType::_Label);
						label->letter = local->last; label->name = local->last->text; label->n = item->children->size;
						label->labelmap = create_mapy(thgc, CType::_List);
						clear_letter(local->last);
						add_mapy(thgc, item->labelmap, name, (char*)label);
						add_mapyn(thgc, item->labelmapn, (char*)(intptr_t)item->children->size, (char*)label);
					}
					if (get_mapy(label->labelmap, local->last->text) != NULL) {
						local->last->error = createString(thgc, (char*)"Label syntax error", 18, 1);
						local->last->underlineColor = 0xFF0000FF;
						*error = true;
					} else {
						Label* label2 = (Label*)GC_alloc(thgc, CType::_Label);
						label2->letter = local->last; label2->name = local->last->text; label2->n = item->children->size;
						label2->labelmap = create_mapy(thgc, CType::_List);
						clear_letter(local->last);
						add_mapy(thgc, label->labelmap, local->last->text, (char*)label2);
						add_mapyn(thgc, item->labelmapn, (char*)(intptr_t)item->children->size, (char*)label2);
					}
					local->Next();
				} else {
					*error = true;
				}
			} else {
				if (get_mapy(item->labelmap, name) != NULL) {
					local->last->error = createString(thgc, (char*)"Label syntax error", 18, 1);
					local->last->underlineColor = 0xFF0000FF;
					*error = true;
				} else {
					Label* label = (Label*)GC_alloc(thgc, CType::_Label);
					label->letter = let; label->name = name; label->n = item->children->size;
					label->labelmap = create_mapy(thgc, CType::_List);
					clear_letter(let);
					add_mapy(thgc, item->labelmap, name, (char*)label);
					add_mapyn(thgc, item->labelmapn, (char*)(intptr_t)item->children->size, (char*)label);
				}
			}
		}
		else if (local->lasttype == LetterType::_Str) {
			Label* label = (Label*)GC_alloc(thgc, CType::_Label);
			label->letter = local->last; label->name = local->last->text; label->n = item->children->size;
			label->labelmap = create_mapy(thgc, CType::_List);
			add_mapy(thgc, item->labelmap, local->last->text, (char*)label);
			add_mapyn(thgc, item->labelmapn, (char*)(intptr_t)item->children->size, (char*)label);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Number) {
			Label* label = (Label*)GC_alloc(thgc, CType::_Label);
			label->letter = local->last; label->name = local->last->text; label->n = item->children->size;
			label->labelmap = create_mapy(thgc, CType::_List);
			add_mapy(thgc, item->labelmap, local->last->text, (char*)label);
			add_mapyn(thgc, item->labelmapn, (char*)(intptr_t)item->children->size, (char*)label);
			clear_letter(local->last);
			local->Next();
		}
		else {
			local->last->error = createString(thgc, (char*)"Label syntax error", 18, 1);
			local->last->underlineColor = 0xFF0000FF;
			*error = true;
		}
	}
	if (local->lasttype == sub) {
		local->Next();
		goto head;
	}
	else if (local->lasttype == sub2) {
		local->Next();
		goto head;
	}
	else if (local->lasttype == sub3) {
		local->Next();
		goto head;
	}
	else if (local->lasttype == finish) {
		ATSSpan* letter = local->last;
		clear_letter(letter);
		item->letter2 = letter;
		local->Next();
		if (outLetter) *outLetter = letter;
		return item;
	}
	add_list(thgc, item->children, (char*)ParseOperator(thgc, local, 0, comments, tag, type, error));
	if (local->lasttype == sub) {
		local->Next();
		goto head;
	}
	else if (local->lasttype == sub2) {
		local->Next();
		goto head;
	}
	else if (local->lasttype == sub3) {
		local->Next();
		goto head;
	}
	else if (local->lasttype == finish) {
		ATSSpan* letter = local->last;
		clear_letter(letter);
		item->letter2 = letter;
		local->Next();
		if (outLetter) *outLetter = letter;
		return item;
	}
	else if (local->lasttype == LetterType::_End) {
		if (local->last) {
			local->before->error = createString(thgc, (char*)"Parse error", 11, 1);
			local->before->underlineColor = 0xFF0000FF;
		}
		if (outLetter) *outLetter = local->last;
		return item;
	}
	else {
		*error = true;
		if (local->last) {
			local->last->error = createString(thgc, (char*)"Parse error", 11, 1);
			local->last->underlineColor = 0xFF0000FF;
		}
		local->Next();
		goto head;
	}
}

// ============================================================
// ParseOpe1
// ============================================================
Obj* ParseOpe1(ThreadGC* thgc, Master* local, int n, int comments, bool tag, LetterType type, bool* error) {
	if (n < local->operators->size) return ParseOperator(thgc, local, n, comments, tag, type, error);
	else return ParsePrimary(thgc, local, comments, type, error);
}

// ============================================================
// ParseOperator
// ============================================================
Obj* ParseOperator(ThreadGC* thgc, Master* local, int n, int comments, bool tag, LetterType type, bool* error) {
	Obj* item = ParseOpe1(thgc, local, n + 1, comments, tag, type, error);
	List* opdef = *(List**)get_list(local->operators, n);
	for (int i = 0; i < opdef->size; i++) {
		LetterType op = *(LetterType*)get_list(opdef, i);
		if (local->lasttype == op) {
			if (local->lasttype == LetterType::_MoreThan && tag) {
				return item;
			}
			Obj* item2 = make_cobj(thgc, CType::_COperator, op, local->last);
			clear_letter(local->last);
			local->Next();
			add_list(thgc, item2->children, (char*)item);
			add_list(thgc, item2->children, (char*)ParseOpe1(thgc, local, n + 1, comments, tag, type, error));
			return item2;
		}
	}
	return item;
}

// ============================================================
// ParseTagString
// ============================================================
Obj* ParseTagString(ThreadGC* thgc, Master* local, LetterType type, bool* error) {
	for (;;) {
		if (local->lasttype == LetterType::_StringTag) {
			ATSSpan* letter = local->last;
			local->Next();
			Obj* tagblock = ParseBlock(thgc, local, LetterType::_MoreThan, 1, error);
			tagblock->letter = letter;
			clear_letter(letter);
		}
		else if (local->lasttype == LetterType::_End || local->lasttype == LetterType::_Kaigyou) {
			return NULL;
		}
		else {
			local->Next();
		}
	}
	return NULL;
}

// ============================================================
// ParsePrimary
// ============================================================
Obj* ParsePrimary(ThreadGC* thgc, Master* local, int comments, LetterType type, bool* error) {
	Primary* item = (Primary*)GC_alloc(thgc, CType::_Primary);
	item->type = LetterType::OT_None;
	item->letter = local->last;
	item->children = create_list(thgc, sizeof(Obj*), CType::_CObj);
	item->vt = &vt_Primary;
	item->singleops = create_list(thgc, sizeof(SingleOp*), CType::_SingleOp);

	if (comments > 0) {
		if (local->lasttype == LetterType::_Dot || local->lasttype == LetterType::_Mul || local->lasttype == LetterType::_RightRight) {
			SingleOp* sop = (SingleOp*)GC_alloc(thgc, CType::_SingleOp);
			sop->letter = local->last;
			add_list(thgc, item->singleops, (char*)sop);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Plus || local->lasttype == LetterType::_Minus || local->lasttype == LetterType::_Mul || local->lasttype == LetterType::_Div) {
			SingleOp* sop = (SingleOp*)GC_alloc(thgc, CType::_SingleOp);
			sop->letter = local->last;
			add_list(thgc, item->singleops, (char*)sop);
			clear_letter(local->last);
			local->Next();
		}
	}
	else if (local->lasttype == LetterType::_Plus || local->lasttype == LetterType::_Minus || local->lasttype == LetterType::_Not) {
		SingleOp* sop = (SingleOp*)GC_alloc(thgc, CType::_SingleOp);
		sop->letter = local->last;
		add_list(thgc, item->singleops, (char*)sop);
		clear_letter(local->last);
		local->Next();
	}

	bool first = true;
	for (;;) {
		if (comments > 0 && type != LetterType::OT_Call1 && (local->lasttype == LetterType::_LessThan || local->lasttype == LetterType::_StringTag)) {
			ATSSpan* letter = local->last;
			clear_letter(letter);
			local->Next();
			Obj* tagblock = ParseBlock(thgc, local, LetterType::_MoreThan, comments, error);
			tagblock->letter = letter;
			add_list(thgc, item->children, (char*)tagblock);
		}
		else if (comments > 0 && local->lasttype == LetterType::_Dolor) {
			Obj* dolor = make_cobj(thgc, CType::_CDolor, LetterType::OT_Dolor, local->last);
			add_list(thgc, item->children, (char*)dolor);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Nyoro) {
			Obj* comment = make_comment(thgc, local->last);
			clear_letter(local->last);
			add_list(thgc, item->children, (char*)comment);
			local->Next();
			ATSSpan* outL = NULL;
			Block* ret = ParseLines(thgc, local, LetterType::_Kaigyou, LetterType::_Semicolon, LetterType::_Comma, LetterType::_NyoroNyoro, LetterType::OT_Comment, comments + 1, error, &outL);
			add_list(thgc, comment->children, (char*)ret);
		}
		else if (local->lasttype == LetterType::_NyoroNyoroNyoro) {
			Obj* comment2 = make_comment2(thgc, local->last);
			clear_letter(local->last);
			add_list(thgc, item->children, (char*)comment2);
			local->Next();
			ATSSpan* outL = NULL;
			Block* ret = ParseLines(thgc, local, LetterType::_Kaigyou, LetterType::_Semicolon, LetterType::_Comma, LetterType::_NyoroNyoro, LetterType::OT_Comment, comments + 1, error, &outL);
			add_list(thgc, comment2->children, (char*)ret);
		}
		else if (comments > 0 && local->lasttype == LetterType::_HLetter) {
			Obj* htm = make_htmobj(thgc, local->last);
			add_list(thgc, item->children, (char*)htm);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Mountain) {
			Obj* mtn = make_cobj(thgc, CType::_CMountain, LetterType::OT_Mountain, local->last);
			add_list(thgc, item->children, (char*)mtn);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Question) {
			Obj* q = make_cobj(thgc, CType::_CQuestion, LetterType::_Question, local->last);
			add_list(thgc, item->children, (char*)q);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Name) {
			Obj* word = make_word_change(thgc, local->last, local);
			add_list(thgc, item->children, (char*)word);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_AtLetter) {
			Obj* addr = make_address(thgc, local->last);
			add_list(thgc, item->children, (char*)addr);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Number) {
			Obj* num = make_number(thgc, local->last);
			add_list(thgc, item->children, (char*)num);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Decimal) {
			Obj* fv = make_floatval(thgc, local->last);
			add_list(thgc, item->children, (char*)fv);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_Str) {
			Obj* so = (Obj*)make_strobj(thgc, local->last);
			add_list(thgc, item->children, (char*)so);
			clear_letter(local->last);
			local->Next();
		}
		else if (local->lasttype == LetterType::_BracketL) {
			ATSSpan* letter = local->last;
			clear_letter(letter);
			local->Next();
			ATSSpan* outL = NULL;
			Block* block = ParseLines(thgc, local, LetterType::_Semicolon, LetterType::_Comma, LetterType::_Kaigyou, LetterType::_BracketR, LetterType::OT_Bracket, comments, error, &outL);
			block->letter = letter;
			add_list(thgc, item->children, (char*)block);
		}
		else if (local->lasttype == LetterType::_BlockL) {
			ATSSpan* letter = local->last;
			clear_letter(letter);
			local->Next();
			ATSSpan* outL = NULL;
			Block* block = ParseLines(thgc, local, LetterType::_Semicolon, LetterType::_Comma, LetterType::_Kaigyou, LetterType::_BlockR, LetterType::OT_Block, comments, error, &outL);
			block->letter = letter;
			add_list(thgc, item->children, (char*)block);
		}
		else if (local->lasttype == LetterType::_BraceL) {
			ATSSpan* letter = local->last;
			clear_letter(letter);
			local->Next();
			Obj* callblock = ParseBlock(thgc, local, LetterType::_BraceR, comments, error);
			callblock->letter = letter;
			add_list(thgc, item->children, (char*)callblock);
		}
		else if (!first) {
			if (local->lasttype == LetterType::_Dot || local->lasttype == LetterType::_Left || local->lasttype == LetterType::_Right) {
				PrimOp* pop = (PrimOp*)GC_alloc(thgc, CType::_PrimOp);
				pop->letter = local->last;
				// 字句型 (_Dot / _Left / _Right) を objtype にそのまま流用。
				// 演算子 (_Plus / _Mul / _Div / _RightRight 等) と同じ扱い。
				pop->objtype = local->lasttype;
				add_list(thgc, item->children, (char*)pop);
				clear_letter(local->last);
				local->Next();
				first = true;
				continue;
			}
			else {
				add_list(thgc, item->children, (char*)make_cobj(thgc, CType::_CObj, LetterType::OT_None, local->last));
				return (Obj*)item;
			}
		}
		else {
			if (local->lasttype == _Kaigyou) {
				local->Next();
				continue;
			}
			if (local->lasttype == _End) {
				local->before->error = createString(thgc, (char*)"No Value", 8, 1);
				local->before->underlineColor = 0xFF0000FF;
			}
			else {
				local->last->error = createString(thgc, (char*)"No Value", 8, 1);
				local->last->underlineColor = 0xFF0000FF;
			}
			add_list(thgc, item->children, (char*)make_cobj(thgc, CType::_CObj, LetterType::OT_None, local->last));
			return (Obj*)item;
		}
		first = false;
	}
}

// ============================================================
// CompileJapanese - split text into words
// ============================================================
// char types: kanji, katakana, hiragana, ascii
// particles after kanji/katakana are included in the word
// hiragana splits at particle boundaries

static inline int jpCharType(wchar_t c) {
	// 0=other, 1=hiragana, 2=katakana, 3=kanji, 4=ascii/digit
	if (c >= 0x3040 && c <= 0x309F) return 1; // hiragana
	if (c >= 0x30A0 && c <= 0x30FF) return 2; // katakana
	if (c >= 0x4E00 && c <= 0x9FFF) return 3; // CJK unified
	if (c >= 0x3400 && c <= 0x4DBF) return 3; // CJK ext-A
	if (c >= 0xF900 && c <= 0xFAFF) return 3; // CJK compat
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
	    (c >= '0' && c <= '9') || c == '_') return 4; // ascii/digit
	return 0;
}

static inline bool isJoshi(wchar_t c) {
	// Japanese particles (ha,ga,wo,ni,de,no,to,mo,he,ka,ya,yo,ne,na,wa,zo,ze,ba)
	if (c == 0x306F || c == 0x304C || c == 0x3092 || c == 0x306B ||
	    c == 0x3067 || c == 0x306E || c == 0x3068 || c == 0x3082 ||
	    c == 0x3078 || c == 0x304B || c == 0x3084 || c == 0x3088 ||
	    c == 0x306D || c == 0x306A || c == 0x308F || c == 0x305E ||
	    c == 0x305C || c == 0x3070) return true;
	// Punctuation (half-width)
	if (c == ' ' || c == '.' || c == ',' || c == '!' || c == '?' ||
	    c == ';' || c == ':' || c == '\t') return true;
	// Punctuation (full-width)
	if (c == 0x3002 || c == 0x3001 || c == 0xFF0E || c == 0xFF0C ||
	    c == 0xFF01 || c == 0xFF1F || c == 0xFF1B || c == 0xFF1A ||
	    c == 0x30FB || c == 0x3000) return true;
	return false;
}

// 2-char particles: kara,made,yori,nado,dake,shite,tte,node,noni,kedo,demo,daga,deha,nimo,toha,heha,kamo
static inline bool isJoshi2(wchar_t c1, wchar_t c2) {
	return (c1 == 0x304B && c2 == 0x3089) ||  // kara
	       (c1 == 0x307E && c2 == 0x3067) ||  // made
	       (c1 == 0x3088 && c2 == 0x308A) ||  // yori
	       (c1 == 0x306A && c2 == 0x3069) ||  // nado
	       (c1 == 0x3060 && c2 == 0x3051) ||  // dake
	       (c1 == 0x3057 && c2 == 0x3066) ||  // shite
	       (c1 == 0x3063 && c2 == 0x3066) ||  // tte
	       (c1 == 0x306E && c2 == 0x3067) ||  // node
	       (c1 == 0x306E && c2 == 0x306B) ||  // noni
	       (c1 == 0x3051 && c2 == 0x3069) ||  // kedo
	       (c1 == 0x3067 && c2 == 0x3082) ||  // demo
	       (c1 == 0x3060 && c2 == 0x304C) ||  // daga
	       (c1 == 0x3067 && c2 == 0x306F) ||  // deha
	       (c1 == 0x306B && c2 == 0x3082) ||  // nimo
	       (c1 == 0x3068 && c2 == 0x306F) ||  // toha
	       (c1 == 0x3078 && c2 == 0x306F) ||  // heha
	       (c1 == 0x304B && c2 == 0x3082);    // kamo
}

List* CompileJapanese(ThreadGC* thgc, String* str, FontId font) {
	List* list0 = create_list(thgc, sizeof(ATSSpan*), CType::_ATSSpan);
	int len = str->size;
	int i = 0;
	while (i < len) {
		wchar_t c = GetChar(str, i);

		// newline
		if (c == '\n') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Kaigyou, i, i + 1, 0x000000FF, NULL));
			int j = i + 1;
			for (; j < len; j++) {
				wchar_t c2 = GetChar(str, j);
				if (c2 == ' ') {
					int si = j;
					j++;
					for (; j < len; j++) {
						if (GetChar(str, j) != ' ') break;
					}
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Space, si, j, 0x000000FF, SubString(thgc, str, si, j - si)));
					j--;
				}
				else if (c2 == '\n') {
					add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_Kaigyou, j, j + 1, 0x000000FF, NULL));
				}
				else break;
			}
			i = j;
			continue;
		}
		// null terminator
		if (c == '\0') {
			add_list(thgc, list0, (char*)make_span(thgc, font, LetterType::_End, i, i + 1, 0x000000FF, NULL));
			break;
		}

		int ct = jpCharType(c);

		if (ct == 0) {
			int ep = i + 1;
			if (isJoshi(c)) {
				while (ep < len && isJoshi(GetChar(str, ep))) ep++;
			}
			add_list(thgc, list0, (char*)make_span(thgc, font,
				LetterType::_CommentSingle, i, ep, 0x000000FF,
				SubString(thgc, str, i, ep - i)));
			i = ep;
		}
		else if (ct == 4) {
			int j = i + 1;
			while (j < len && jpCharType(GetChar(str, j)) == 4) j++;
			int ep = j;
			while (ep < len && isJoshi(GetChar(str, ep))) ep++;
			add_list(thgc, list0, (char*)make_span(thgc, font,
				LetterType::_CommentSingle, i, ep, 0x000000FF,
				SubString(thgc, str, i, ep - i)));
			i = ep;
		}
		else if (ct == 2 || ct == 3) {
			int j = i + 1;
			while (j < len && jpCharType(GetChar(str, j)) == ct) j++;
			int ep = j;
			if (j < len) {
				wchar_t cj = GetChar(str, j);
				int cjt = jpCharType(cj);
				if (cjt == 1 || cjt == 0) {
					if (j + 1 < len && isJoshi2(cj, GetChar(str, j + 1))) {
						ep = j + 2;
					}
					else if (isJoshi(cj)) {
						ep = j + 1;
					}
				}
			}
			add_list(thgc, list0, (char*)make_span(thgc, font,
				LetterType::_CommentSingle, i, ep, 0x000000FF,
				SubString(thgc, str, i, ep - i)));
			i = ep;
		}
		else {
			// hiragana: split at particle boundary (include particle)
			int j = i + 1;
			while (j < len) {
				wchar_t cj = GetChar(str, j);
				if (jpCharType(cj) != 1) break;
				// 2-char particle check
				if (j + 1 < len && isJoshi2(cj, GetChar(str, j + 1))) {
					j += 2;
					break;
				}
				if (isJoshi(cj)) {
					j++;
					break;
				}
				j++;
			}
			add_list(thgc, list0, (char*)make_span(thgc, font,
				LetterType::_CommentSingle, i, j, 0x000000FF,
				SubString(thgc, str, i, j - i)));
			i = j;
		}
	}
	return list0;
}

#include "objz.h"
#include "objvt.h"
#include "obj1a.h"
#include "cobj2a.h"
#include "obj2a.h"
#include "obj1b.h"
#include "obj2b.h"
#include "cobj2b.h"
#include "llvm.h"
#include "obj1c.h"
#include "cobj2c.h"
#include "obj2c.h"
