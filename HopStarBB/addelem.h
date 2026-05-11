// ============================================================
// addelem.h - id 経由の NewElement add/check/remove (旧 TreeElement)
//
// 設計方針 (= 旧 TreeElement struct + tag tracking 削除後):
//   - thgc->map = 任意 GC オブジェクト用の generic registry (= id → 任意ポインタ)
//   - 「タグ階層の parent/children」「visited / 削除候補マーキング」は持たない
//     (= レンダリング階層 NewElement::childend / next で必要十分)
//   - 削除 (= Minus op) のときだけ map から explicit に取り除く。stale entry は
//     呼び出し側が責任を持って removeMap (= explicit) する設計。
//
// 関数シグネチャは LLVM IR ABI 互換のため変えない (= @CheckTreeElement)。
// `deletes` 引数は互換のため受け取るが、使わない (sweep 廃止)。
// ============================================================

void createLetter(ThreadGC* thgc, NewElement* parent, String* str, FontId font) {
	NewLetter* letter = (NewLetter*)GC_alloc(thgc, _LetterC);
	initLetter(thgc, letter, font, _Letter);
	letter->text = str;
	if (parent != NULL) {
		NewElementAddLast(thgc, NULL, parent, (NewElement*)letter);
	}
}

enum OpCode {
	Plus  = 1,
	Minus = 2,
	Mul   = 4,
	Divi  = 8
};

// ------------------------------------------------------------
// checkTreeElement (= 旧名): id で thgc->map を検索。
//   - 無ければ func() で新規生成 + map 登録 + 親 (parent) 配下に挿入
//   - あって type が一致なら再利用 (= Plus 系で位置/サイズ保持、Mul/Divi で子保持)
//   - あって type が違えば新規生成して旧要素と差し替え + map 上書き
//   - Minus op なら map から remove + rendering からも remove
// 引数 parentt / deletes は ABI 互換のため受け取るが、tag tracking が無くなったので
// 中では未使用。
// ------------------------------------------------------------
NewElement* checkTreeElement(ThreadGC* thgc, NewLocal* local,
	NewElement* parentt, NewElement* parent, List* deletes,
	String* id, enum LetterType type, enum OpCode op,
	NewElement* (*func)(ThreadGC*))
{
	(void)parentt; (void)deletes;   // 互換引数 (= 未使用)
	NewElement* te = (NewElement*)get_mapy(thgc->map, id);
	if (te == NULL) {
		// 新規作成
		te = func(thgc);
		add_mapy(thgc, thgc->map, id, (char*)te);
		if ((op & Minus) == Minus) return NULL;
		if (parent == NULL) return NULL;
		NewElementAddLast(thgc, NULL, parent, te);
		return te;
	}
	if (te->type == type) {
		// 同型 → 再利用
		if ((op & Minus) == Minus) {
			remove_mapy(thgc, thgc->map, id);
			NewRemove(thgc, local, te);
			return NULL;
		}
		if ((op & Plus) != Plus) {
			te->pos.x = 0; te->pos.y = 0;
			te->size.x = 0; te->size.y = 0;
		}
		// Mul / Divi なら子保持、それ以外は rendering 子を全削除
		if (((op & Mul) != Mul) && ((op & Divi) != Divi)) {
			for (NewElement* elem = te->childend; elem->next != te->childend; ) {
				NewRemove(thgc, local, elem->next);
			}
		}
		if (parent == NULL) return te;
		NewRemove(thgc, local, te);
		te->parent = parent;
		NewElementAddLast(thgc, NULL, parent, te);
		return te;
	}
	// 型違い → 差し替え
	NewElement* elem = func(thgc);
	if ((op & Minus) == Minus) {
		remove_mapy(thgc, thgc->map, id);
		NewRemove(thgc, local, te);
		return NULL;
	}
	if ((op & Plus) == Plus) {
		elem->pos = te->pos; elem->size = te->size;
		elem->xtype = te->xtype; elem->ytype = te->ytype;
	}
	if (((op & Mul) == Mul) || ((op & Divi) == Divi)) {
		// 旧の子を新へ移植
		elem->childend = te->childend;
		elem->childend->parent = elem;
		for (NewElement* child = elem->childend->next; child != elem->childend; child = child->next) {
			child->parent = elem;
		}
	} else {
		for (NewElement* elem2 = te->childend; elem2->next != te->childend; ) {
			NewRemove(thgc, local, elem2->next);
		}
	}
	add_mapy(thgc, thgc->map, id, (char*)elem);   // map 上書き
	if (parent == NULL) {
		// 元の位置にスワップ (= rendering 階層の next/before/parent を引き継ぐ)
		elem->next = te->next;
		elem->before = te->before;
		elem->next->before = elem;
		elem->before->next = elem;
		elem->parent = te->parent;
		return elem;
	}
	NewRemove(thgc, local, te);
	NewElementAddLast(thgc, NULL, parent, elem);
	return elem;
}

// ------------------------------------------------------------
// TreeElementLast (= 旧 sweep): tag tracking 廃止に伴い no-op。
// ABI 互換のため signature は残す (= deletes は無視)。
// ------------------------------------------------------------
void TreeElementLast(ThreadGC* thgc, List* deletes) {
	(void)thgc; (void)deletes;
	if (deletes) deletes->size = 0;
}
