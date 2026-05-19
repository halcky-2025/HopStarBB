#pragma once
#include "encode.h"
#include "lldb.h"
// ugui.h で定義される HopStar::set_target のフリー関数版を othelem.h からも呼ぶため
// に前方宣言しておく (ugui.h は newelem.h の後に include される想定)。
void HopStar_set_target(HopStar* hp, ThreadGC* thgc);

struct NewImage : NewElement {

};
inline void NewImageCheck(ThreadGC* thgc, char* data) {
	traceNewElement(thgc, (NewElement*)data);
}
RetMeasureType ImageMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	return RetMeasure_None;
}
void ImageDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	bgfx::TextureHandle* tex1 = &nulltex;
	bgfx::TextureHandle* tex2 = &nulltex;
	bgfx::TextureHandle* tex3 = &nulltex;
	bgfx::TextureHandle* tex4 = &nulltex;
	if (isValidImageId(elem->background->tex1)) {
		ResolvedTexture rt = myResolveForDraw(thgc, elem->background->tex1);
		if (rt.isValid()) {
			tex1 = rt.texture();
			elem->background->angle = rt.u0;
			elem->background->curl = rt.v0;
			elem->background->scrollX = rt.u1;
			elem->background->scrollY = rt.v1;
		}
	}
	if (isValidImageId(elem->background->tex2)) {
		ResolvedTexture rt2 = myResolveForDraw(thgc, elem->background->tex2);
		if (rt2.isValid()) tex2 = rt2.texture();
	}
	if (isValidImageId(elem->background->tex3)) {
		ResolvedTexture rt3 = myResolveForDraw(thgc, elem->background->tex3);
		if (rt3.isValid()) tex3 = rt3.texture();
	}
	if (isValidImageId(elem->background->tex4)) {
		ResolvedTexture rt4 = myResolveForDraw(thgc, elem->background->tex4);
		if (rt4.isValid()) tex4 = rt4.texture();
	}
	g->layer->pushBackground(elem->background, g->pos.x + elem->margins[3], g->pos.y + elem->margins[0], elem->size.x + elem->paddings[1] + elem->paddings[3], elem->size.y + elem->paddings[0] + elem->paddings[2], g->zIndex + elem->zIndex + 0.1,
		tex1, tex2, tex3, tex4, g->fb, g->fbsize, g->viewId, 0.0f, elem->boWidthes, &elem->background->bs);
}
void ImageDrawSelect(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q) {
	if (m == n) return;
	g->layer->pushFill(pos.x + self->pos.x + self->pos2.x, self->pos.y + self->pos2.y, pos.y + self->size.x + 1, self->size.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x4477ff66, 0, 0.0f, 0.0f, 1.0f, 0, 12000.0f, g->fb, g->fbsize, g->viewId);
}
int ImageMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	NewImage* letter = (NewImage*)self;
	if (self->pos2.x + self->pos.x + self->margins[3] <= e->x - pos.x && e->x - pos.x < self->pos2.x + self->pos.x + self->size.x - self->margins[1]) {
		if (self->pos2.y + self->pos.y + self->margins[0] <= e->y - pos.y && e->y - pos.y < self->pos2.y + self->pos.y + self->size.y - self->margins[2]) {
			if (self->GoMouseDown != NULL)
				FireEvent(thgc, self, self->GoMouseDown, _MouseEvent, e, sizeof(MouseEvent));
			if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				local->select.from = local->select.to = self;
				local->select.fromid = local->select.toid = self->id;
				local->select.m = local->select.n = 0;
				Offscreen* seloff = FindOffscreen(self);
				local->select.window = seloff ? seloff->window : nullptr;
			}
			else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
				Offscreen* tooff = FindOffscreen(self);
				NativeWindow* toWin = tooff ? tooff->window : nullptr;
				if (toWin == local->select.window) {
					local->select.to = self;
					local->select.toid = self->id;
					local->select.n = 0;
				}
			}

			if (self->BackMouseDown != NULL)
				FireEvent(thgc, self, self->BackMouseDown, _MouseEvent, e, sizeof(MouseEvent));
			return 0;
		}
	}
	return -1;
}
void initImage(ThreadGC* thgc, NewImage* im1) {
	im1->gc = thgc;
	im1->type = LetterType::_Image;
	im1->next = im1;
	im1->before = im1;
	im1->parent = NULL;
	im1->childend = NULL;
	im1->Measure = ImageMeasure;
	im1->Draw = ImageDraw;
	im1->Mouse = ImageMouse;
	im1->Key = ElementKey;
	im1->DrawSelection = ImageDrawSelect;
	im1->len = len1;
	im1->animate = -1;
}
struct PopupWindow : NewElement {
	bool visible;
	int cornerRadius;
};
inline void PopupWindowCheck(ThreadGC* thgc, char* data) {
	traceNewElement(thgc, (NewElement*)data);
}
struct NewDown;
struct NewDrop : NewElement {
	PopupWindow* pw;
	NewDown* select;
};
inline void NewDropCheck(ThreadGC* thgc, char* data) {
	NewDrop* d = (NewDrop*)data;
	traceNewElement(thgc, (NewElement*)d);
	d->pw     = (PopupWindow*)copy_object_young(thgc, (char*)d->pw);
	d->select = (NewDown*)    copy_object_young(thgc, (char*)d->select);
}
struct NewDown : NewLine {
	NewDrop* drop;
};
inline void NewDownCheck(ThreadGC* thgc, char* data) {
	NewDown* d = (NewDown*)data;
	traceNewElement(thgc, (NewElement*)d);
	d->drop = (NewDrop*)copy_object_young(thgc, (char*)d->drop);
}
int DropMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	NewDrop* drop = (NewDrop*)self;
	if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		drop->pw->visible = true;
		myShowWindow(thgc, drop->pw->offscreen->window);
	}
	return 0;
}
int DropKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local) {
	return 0;
}
RetMeasureType NewDropMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	measure->pos.x += elem->size.x;
	return RetMeasure_None;
}
// シェブロン（V字・45度）を描画
void drawChevron(LayerInfo* layer, float cx, float cy, float size, float thickness,
	float zIndex, uint32_t color, bgfx::FrameBufferHandle* fb, PointI* fbsize, uint64_t viewId) {
	int steps = (int)(size * 0.5f);
	if (steps < 3) steps = 3;
	float half = size * 0.5f;  // 45度: 幅と高さが同じ
	Background dot{};
	dot.type = DrawCommandType::Fill;
	dot.fillcolor = color;
	dot.bs.boRadiuses[0] = dot.bs.boRadiuses[1] = dot.bs.boRadiuses[2] = dot.bs.boRadiuses[3] = thickness * 0.5f;
	float dotBW[4] = {0, 0, 0, 0};
	dot.aa = 1.0f;
	for (int i = 0; i <= steps; i++) {
		float t = (float)i / (float)steps;
		// 左腕: 左上 → 中央下（cyを中心にする）
		float lx = cx - half + t * half;
		float ly = (cy - half * 0.5f) + t * half;
		layer->pushBackground(&dot, lx - thickness * 0.5f, ly - thickness * 0.5f,
			thickness, thickness, zIndex, &nulltex, &nulltex, &nulltex, &nulltex, fb, fbsize, viewId, 0.0f, dotBW, &dot.bs);
		// 右腕: 右上 → 中央下（cyを中心にする）
		float rx = cx + half - t * half;
		float ry = (cy - half * 0.5f) + t * half;
		layer->pushBackground(&dot, rx - thickness * 0.5f, ry - thickness * 0.5f,
			thickness, thickness, zIndex, &nulltex, &nulltex, &nulltex, &nulltex, fb, fbsize, viewId, 0.0f, dotBW, &dot.bs);
	}
}
void DropDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	NewDrop* drop = (NewDrop*)elem;
	bgfx::TextureHandle* tex1 = &nulltex;
	bgfx::TextureHandle* tex2 = &nulltex;
	if (isValidImageId(elem->background->tex1)) {
		ResolvedTexture rt = myResolveForDraw(thgc, elem->background->tex1);
		if (rt.isValid()) {
			tex1 = rt.texture();
			elem->background->angle = rt.u0;
			elem->background->curl = rt.v0;
			elem->background->scrollX = rt.u1;
			elem->background->scrollY = rt.v1;
		}
	}
	if (isValidImageId(elem->background->tex2)) {
		ResolvedTexture rt2 = myResolveForDraw(thgc, elem->background->tex2);
		if (rt2.isValid()) tex2 = rt2.texture();
	}
	g->layer->pushBackground(elem->background, g->pos.x + elem->pos.x + elem->margins[3], g->pos.y + elem->pos.y + elem->margins[0], elem->size.x + elem->paddings[1] + elem->paddings[3], elem->size.y + elem->paddings[0] + elem->paddings[2], g->zIndex + elem->zIndex + 0.1,
		tex1, tex2, &nulltex, &nulltex, g->fb, g->fbsize, g->viewId, 0.0f, elem->boWidthes, &elem->background->bs);
	if (drop->select != NULL) drop->select->Draw(thgc, drop->select, g, local, q);
	// 右端にシェブロン描画
	float dx = g->pos.x + elem->pos.x;
	float dy = g->pos.y + elem->pos.y;
	float chevronSize = elem->size.y * 0.4f;
	float chevronX = dx + elem->size.x - elem->size.y * 0.5f;
	float chevronY = dy + elem->size.y * 0.5f;
	drawChevron((LayerInfo*)g->layer, chevronX, chevronY, chevronSize, 1.5f,
		g->zIndex + elem->zIndex + 0.9f, 0x808080FF, g->fb, g->fbsize, g->viewId);
	g->pos.x += elem->size.x;
}
void DropValueChange0(NewElement* elem, int n) {

}
void initDrop(ThreadGC* thgc, NewDrop* drop, PopupWindow* pw) {
	drop->gc = thgc;
	drop->type = LetterType::_Drop;
	drop->next = drop->before = drop;
	drop->childend = NULL;
	drop->offscreened = true;
	drop->Measure = NewDropMeasure;
	drop->Draw = DropDraw;
	drop->Mouse = DropMouse;
	drop->Key = ElementKey;
	drop->DrawSelection = ImageDrawSelect;
	drop->len = len1;
	// SIValues: bit-packed values (Bcount 個)
	//   values[Bcount-1] = ChangeCallback (データ更新時に発火させる固定 slot)
	drop->values = makeSIValues(thgc, Bcount);
	{
		SIVal cb;
		cb.bits = (uint64_t)SIType::ChangeCallback
		        | ((uint64_t)DropValueChange0 & SIV_VAL_MASK);
		drop->values->values[Bcount - 1] = cb;
	}
	drop->background = (Background*)GC_alloc(thgc, CType::_Background);
	drop->background->fillcolor = 0xFFFFFFFF;
	drop->background->type = DrawCommandType::Fill;
	drop->background->bs.boRadiuses[0] = drop->background->bs.boRadiuses[1] = drop->background->bs.boRadiuses[2] = drop->background->bs.boRadiuses[3] = 4;
	drop->boWidthes[0] = drop->boWidthes[1] = drop->boWidthes[2] = drop->boWidthes[3] = 1;
	drop->background->bs.borderColor = 0x8080a0ff;
	drop->pw = pw;
	drop->animate = -1;
}
int DownMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	NewDown* down = (NewDown*)self;
	if (self->offscreened) e->group = self->offscreen->group;
	if (self->GoMouseDown != NULL)
		FireEvent(thgc, self, self->GoMouseDown, _MouseEvent, e, sizeof(MouseEvent));
	if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN && down->parent->type == LetterType::_Popup) {
		PopupWindow* popup = (PopupWindow*)down->parent;
		NewDrop* drop = (NewDrop*)popup->offscreen->window->anchorElement;
		drop->select = down;
		markLayoutOf(drop, local);
		myHideWindow(thgc, popup->offscreen->window);
	}
	pos.x += self->pos.x + self->margins[3] + self->boWidthes[3] + self->paddings[3] - self->scroll.x;
	pos.y += self->pos.y + self->margins[0] + self->boWidthes[0] + self->paddings[0] - self->scroll.y;
	for (NewElement* elem = self->childend->next; elem != self->childend; elem = elem->next) {
		if (self->orient) {
			if (elem->pos.x + elem->pos2.x + elem->margins[3] <= e->x - pos.x && e->x - pos.x < elem->pos.x + elem->pos2.y + elem->size.x + elem->margins[3] + elem->paddings[1] + elem->paddings[3] + elem->boWidthes[1] + elem->boWidthes[3]) {
				if (elem->hoverColor != 0) add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
				elem->Mouse(elem->gc, elem, e, { pos.x + elem->pos2.x, pos.y}, elem->gc->local);
			}
		}
		else {
			if (elem->pos.y + elem->pos2.y + elem->margins[0] <= e->y - pos.y && e->y - pos.y < elem->pos.y + elem->pos2.y + elem->size.y + elem->margins[0] + elem->paddings[0] + elem->paddings[2] + elem->boWidthes[0] + elem->boWidthes[2]) {
				if (elem->hoverColor != 0) add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
				elem->Mouse(elem->gc, elem, e, { pos.x, pos.y + elem->pos2.y}, elem->gc->local);
			}

		}
	}

	if (self->BackMouseDown != NULL)
		FireEvent(thgc, self, self->BackMouseDown, _MouseEvent, e, sizeof(MouseEvent));
	return -1;
}
int DownKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local) {
	return 0;
}
void initDown(ThreadGC* thgc, NewDown* down) {
	down->gc = thgc;
	down->type = LetterType::_Down;
	down->next = down->before = down;
	down->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, down->childend, down);
	down->orient = true;
	down->Measure = ElementMeasure;
	down->Draw = ElementDraw;
	down->Mouse = DownMouse;
	down->Key = ElementKey;
	down->DrawSelection = ElementDrawSelect;
	down->len = len1;
	down->animate = -1;
}
void reinitDown(ThreadGC* thgc, NewDown* down, String* text) {
	if (down->parent->type == LetterType::_Popup) {
		PopupWindow* popup = (PopupWindow*)down->parent;
		NewDrop* drop = (NewDrop*)popup->offscreen->window->anchorElement;
		drop->select = down;
	}
}
int PopupMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, NewLocal* local) {
	return 0;
}
int PopupKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local) {
	return 0;
}
RetMeasureType PopupMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	ElementMeasure(thgc, elem, measure, local, n);
	myResizePopupWindow(thgc, elem->offscreen->window, elem->size.x, elem->size.y);
	return RetMeasure_None;
}
void initPopup(ThreadGC* thgc, NewLocal* local, PopupWindow* popup, PopupAnchor pa, NewElement* elem, int cornerRaidus, int x = 0, int y = 0) {
	popup->gc = thgc;
	popup->type = LetterType::_Popup;
	popup->next = popup->before = popup;
	popup->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, popup->childend, popup);
	popup->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
	popup->offscreen->type = OffscreenType::Off;
	popup->offscreen->group = &createGroup(thgc);
	popup->offscreen->elem = popup;
	popup->offscreen->markLayout(local, popup);
	popup->offscreen->curlProgress = -1.0f;
	popup->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
	popup->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
	popup->offscreen->next = popup->offscreen->before = popup->offscreen;
	popup->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
	popup->offscreen->child->next = popup->offscreen->child->before = popup->offscreen->child;
	popup->offscreen->child->parent = popup->offscreen;
	popup->offscreened = true;
	popup->background = (Background*)GC_alloc(thgc, CType::_Background);
	popup->background->fillcolor = 0xffffffff;
#if defined(__ANDROID__)
	// Android では別 OS window が無く、popup を main backbuffer にオーバーレイ描画する。
	// → SDL_CreatePopupWindow が描いてくれていた境界が無いので、UI 側でボーダー / シャドウを
	//   描画する。これで popup らしい見た目になり、背景の UI との視覚的区切りが付く。
	popup->background->bs.borderColor = 0xC0C0C0FF;  // light gray border
	popup->boWidthes[0] = popup->boWidthes[1] = popup->boWidthes[2] = popup->boWidthes[3] = 1.0f;
	popup->background->bs.shadowBlur = 8.0f;
	popup->background->bs.shadowColor = 0x00000040;  // semi-transparent shadow
	popup->background->bs.shadowX = 0.0f;
	popup->background->bs.shadowY = 2.0f;
#else
	popup->background->bs.borderColor = 0x00000000;
	popup->boWidthes[0] = popup->boWidthes[1] = popup->boWidthes[2] = popup->boWidthes[3] = 0.0f;
	popup->background->bs.shadowBlur = 1.0f;
	popup->background->bs.shadowColor = 0x00000000;
#endif
	popup->Measure = PopupMeasure;
	popup->Draw = ElementDraw;
	popup->Mouse = ElementMouse;
	popup->Key = PopupKey;
	popup->DrawSelection = ElementDrawSelect;
	popup->len = len1;
	NativeWindow* p = myCreatePopupWindow(thgc, NativeWindowType::WindowType_Popup, pa, x, y, 200, 400, cornerRaidus, elem, false);
	if (p) {
		p->local = popup;
		popup->offscreen->window = p;
#if defined(__ANDROID__) || (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_SIMULATOR))
		// Android / iOS: 別 OS window が無いので、popup の絶対位置を **parent chain 経由で**
		// 解決させる。子要素リスト (childend) には挿入せず popup->parent のみセット:
		//   - getAbsolutePosition(popup) は parent ポインタを辿って anchor の絶対位置を
		//     取得し、popup->pos を加算 → anchor 真下に表示される
		//   - anchor element の measure / draw / hit test の子ループには popup は入らない
		//     → toolline / drop 等のレイアウトや click 動線を壊さない
		//   - popup の描画は screen ループ (drawScreen) が独立に行う
		if (pa == PopupAnchor::Anchor_Element && elem) {
			popup->parent = elem;
			popup->pos.x = 0;
			popup->pos.y = (float)elem->size.y;  // anchor elem の真下
		} else {
			popup->pos.x = (float)x;
			popup->pos.y = (float)y;
		}
#endif
	}
	popup->animate = -1;
}
// ポップアップをツリーに単騎挿入し、Offscreen の NativeWindow を設定する
// 子要素はこの後に追加すること（子は popup->nw を継承する）
void addPopupToTree(ThreadGC* thgc, NewLocal* local, NewElement* parent, PopupWindow* popup) {
	NewElementAddLast(thgc, local, parent, popup);
	if (popup->offscreen && popup->offscreen->window) {
		popup->offscreen->window = popup->offscreen->window;
	}
}
struct NewTabPage;
struct NewTabTitle;
struct LinkedElement : NewElement {
	NewElement* page;
};
inline void LinkedElementCheck(ThreadGC* thgc, char* data) {
	LinkedElement* l = (LinkedElement*)data;
	traceNewElement(thgc, (NewElement*)l);
	// l->page は cross-heap (= 別 thgc の Old goLocal) もしくは同 heap (= 通常の page)。
	// copy_object_young の Old early return + cross-heap guard で安全に no-op になる。
	l->page = (NewElement*)copy_object_young(thgc, (char*)l->page);
}
struct NewTabBar : NewElement{
	PointF tabsize;
	NewElement* select;
	LinkedElement* linked;
	int direction;
};
inline void NewTabBarCheck(ThreadGC* thgc, char* data) {
	NewTabBar* t = (NewTabBar*)data;
	traceNewElement(thgc, (NewElement*)t);
	t->select = (NewElement*)  copy_object_young(thgc, (char*)t->select);
	t->linked = (LinkedElement*)copy_object_young(thgc, (char*)t->linked);
}
struct NewTabTitle : NewElement {
	NewTabBar* control;
	NewElement* page;
};
inline void NewTabTitleCheck(ThreadGC* thgc, char* data) {
	NewTabTitle* t = (NewTabTitle*)data;
	traceNewElement(thgc, (NewElement*)t);
	t->control = (NewTabBar*) copy_object_young(thgc, (char*)t->control);
	// t->page は cross-heap (= 別 thgc の Old goLocal)。Old early return で no-op。
	t->page    = (NewElement*)copy_object_young(thgc, (char*)t->page);
}
struct NewTabPage : NewElement {
	NewTabTitle* title;
};
inline void NewTabPageCheck(ThreadGC* thgc, char* data) {
	NewTabPage* p = (NewTabPage*)data;
	traceNewElement(thgc, (NewElement*)p);
	p->title = (NewTabTitle*)copy_object_young(thgc, (char*)p->title);
}
RetMeasureType TabMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	NewTabBar* tab = (NewTabBar*)elem;
	// selectに対応するページをLinkedElementに同期
	if (tab->linked && tab->select && tab->select->type == LetterType::_TabTitle) {
		NewElement* page = ((NewTabTitle*)tab->select)->page;
		tab->linked->page = page;
	}
	return ElementMeasure(thgc, elem, measure, local, n);
}
void TabDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	NewTabBar* tab = (NewTabBar*)elem;
	// 親要素のタイル状態を退避 (戻り際に nullptr で潰さないため)
	TiledTextureInfo*        _saveTiledTarget        = g->layer->tiledTarget;
	ImageId                  _saveTiledTargetImageId = g->layer->tiledTargetImageId;
	bgfx::FrameBufferHandle* _saveTiledPlaceholderFBO = g->layer->tiledPlaceholderFBO;
	uint64_t                 _saveTiledBaseViewId    = g->layer->tiledBaseViewId;
	bool offscreened = false;
	float sbarx = 0, sbary = 0;
	//x Scroll xchanged
	if (baseType(elem->xtype) == SizeType::Scroll) {
		sbary += 5; offscreened = true;
	}
	else if (baseType(elem->ytype) == SizeType::Page) {
		offscreened = true;
		sbarx += 10;
	}
	if (baseType(elem->ytype) == SizeType::Scroll) {
		sbarx += 5;
		offscreened = true;
	}
	else if (baseType(elem->ytype) == SizeType::Page) {
		offscreened = true;
		sbary += 10;
	}
	if (elem->type == LetterType::_Main) {
		offscreened = true;
	}
	NewGraphic g2;
	float sizex = elem->size.x + elem->paddings[1] + elem->paddings[3]; float sizey = elem->size.y + elem->paddings[0] + elem->paddings[2];
	float viewSizex = sizex, viewSizey = sizey;
	float tabExtL = 0, tabExtR = 0, tabExtT = 0, tabExtB = 0;

	// スクロールバー描画。winFb に zIndex 10001 で書く。オフスクリーン処理 (blit /
	// write / early-return) に左右されない位置で一回走らせて、PaintOffscreen で
	// early-return しても毎フレーム描かれるようにする (=スクロール値が変化したら
	// 即追従)。ElementDraw 側と同じパターン。
	if (g->winFb) {
		PointF absPos2 = getAbsolutePosition(elem);
		if (baseType(elem->ytype) == SizeType::Scroll) {
			drawRightScrollBar(g->layer,
				absPos2.x + viewSizex - sbarx * 3 / 2, absPos2.y, sbarx, viewSizey - sbary, sbarx * 3 / 2,
				elem->scroll.y, viewSizey, elem->size2.y,
				10001.0f,
				g->winFb, g->winFbsize, g->winViewId);
		}
		if (baseType(elem->xtype) == SizeType::Scroll) {
			drawUnderScrollBar(g->layer,
				absPos2.x, absPos2.y + viewSizey - sbary * 3 / 2, viewSizex - sbarx, sbary, sbary * 3 / 2,
				elem->scroll.x, viewSizex, elem->size2.x,
				10001.0f,
				g->winFb, g->winFbsize, g->winViewId);
		}
	}

	if (elem->offscreen != NULL) {
		if (elem->background) {
			float tabR = elem->background->backUVMinX;
			int tabPat = (int)elem->background->cornerPattern;
			float tabExt = tabR * 1.5f;
			if (tabR > 0.0f && tabPat >= 9 && tabPat <= 12) {
				if (tabPat == 9)       tabExtT = tabExt;
				else if (tabPat == 10) tabExtR = tabExt;
				else if (tabPat == 11) tabExtB = tabExt;
				else                   tabExtL = tabExt;
			}
		}
		// PaintOffscreenならこのoffscreenの描画は不要（子offscreenはugui.hのループで個別に処理される）
		if (g->paint == Offscreen::PaintOffscreen) return;
		elem->offscreen->paint = Offscreen::PaintOffscreen;
		float z = 0;
		PointF absPos = getAbsolutePosition(elem, &z);
		if (elem->size2.x >= sizex) sizex = elem->size2.x;
		if (elem->size2.y >= sizey) sizey = elem->size2.y;
		sizex += tabExtL + tabExtR;
		sizey += tabExtT + tabExtB;
		// 現サイズは実テクスチャから読む (Tiled なら totalSize、Standalone なら size)。
		// queueOffscreenResize が同期更新するので「自前 fbsize キャッシュ」と等価。
		PointI curFb = { 0, 0 };
		{
			TiledTextureInfo* curTti = mygetTiledTextureInfo(thgc, elem->offscreen->imPing);
			if (curTti) curFb = curTti->totalSize;
			else {
				StandaloneTextureInfo* curSti = mygetStandaloneTextureInfo(thgc, elem->offscreen->imPing);
				if (curSti) curFb = curSti->size;
			}
		}
		float size3x = curFb.x, size3y = curFb.y;
		bool resized = false;
		if (curFb.x < sizex) {
			size3x = sizex * 1.3;
			resized = true;
		}
		else if (sizex < curFb.x / 3) {
			size3x = sizex * 1.3;
			resized = true;
		}
		if (curFb.y < sizey) {
			size3y = sizey * 1.3;
			resized = true;
		}
		else if (sizey < curFb.y / 3) {
			size3y = sizey * 1.3;
			resized = true;
		}
		if (resized) {
			int pageW = (baseType(elem->xtype) == SizeType::Page) ? (int)viewSizex : 0;
			int pageH = (baseType(elem->ytype) == SizeType::Page) ? (int)viewSizey : 0;
			// タイプ遷移 (Standalone↔Tiled) では新 id が返るので必ず受け取る
			elem->offscreen->imPing = queueOffscreenResize(thgc, elem->offscreen->imPing, size3x, size3y, pageW, pageH);
			elem->offscreen->imPong = queueOffscreenResize(thgc, elem->offscreen->imPong, size3x, size3y, pageW, pageH);
		}
		ImageId readId = elem->offscreen->imPing;
		ImageId writeId = elem->offscreen->imPing;

		// タイルFBOか通常FBOかで分岐
		TiledTextureInfo* tiledRead = mygetTiledTextureInfo(thgc, readId);

		if (tiledRead) {
			// 読み取り: 最大2×2タイルを1回のpushImageで描画 (X/Y で別辺長)
			int tsx = (int)tiledRead->tileSize;
			int tsy = (int)tiledRead->tileSizeY;
			if (tsx > 0 && tsy > 0) {
				int srcX = (int)(elem->scroll.x);
				int srcY = (int)(elem->scroll.y);
				int srcW = (int)std::ceil(viewSizex + tabExtL + tabExtR);
				int srcH = (int)std::ceil(viewSizey + tabExtT + tabExtB);
				int col0 = std::clamp(srcX / tsx,                0, tiledRead->tilesX - 1);
				int row0 = std::clamp(srcY / tsy,                0, tiledRead->tilesY - 1);
				int col1 = std::clamp((srcX + srcW - 1) / tsx,   0, tiledRead->tilesX - 1);
				int row1 = std::clamp((srcY + srcH - 1) / tsy,   0, tiledRead->tilesY - 1);
				if (col1 > col0 + 1) col1 = col0 + 1;
				if (row1 > row0 + 1) row1 = row0 + 1;
				// ★ in-place 更新で tiles アドレス安定。無条件に &tile.handle を渡す。
				bgfx::TextureHandle* texSlots[4] = { &nulltex, &nulltex, &nulltex, &nulltex };
				auto& tl = tiledRead->tiles[row0 * tiledRead->tilesX + col0];
				texSlots[0] = &tl.handle;
				if (col1 > col0) {
					auto& tr = tiledRead->tiles[row0 * tiledRead->tilesX + col1];
					texSlots[1] = &tr.handle;
				}
				if (row1 > row0) {
					auto& bl = tiledRead->tiles[row1 * tiledRead->tilesX + col0];
					texSlots[2] = &bl.handle;
				}
				if (col1 > col0 && row1 > row0) {
					auto& br = tiledRead->tiles[row1 * tiledRead->tilesX + col1];
					texSlots[3] = &br.handle;
				}
				float tileEndX = (col1 < tiledRead->tilesX) ?
					tiledRead->tiles[row0 * tiledRead->tilesX + col1].x + tiledRead->tiles[row0 * tiledRead->tilesX + col1].w :
					(col1 + 1) * tsx;
				float tileEndY = (row1 < tiledRead->tilesY) ?
					tiledRead->tiles[row1 * tiledRead->tilesX + col0].y + tiledRead->tiles[row1 * tiledRead->tilesX + col0].h :
					(row1 + 1) * tsy;
				// UV 計算: グリッド原点 (col0*tsx, row0*tsy) からのオフセットを
				//          実テクスチャ寸法 tl.w / tl.h で正規化 (詳細は newelem.h 側コメント参照)
				int tw = tl.w > 0 ? tl.w : tsx;
				int th = tl.h > 0 ? tl.h : tsy;
				float uvX = (elem->scroll.x - col0 * tsx) / (float)tw;
				float uvY = (elem->scroll.y - row0 * tsy) / (float)th;
				float uvW = (viewSizex + tabExtL + tabExtR) / (float)tw;
				float uvH = (viewSizey + tabExtT + tabExtB) / (float)th;
				auto* bg = elem->background;
				g->layer->pushImage(
					absPos.x + elem->pos.x + elem->margins[3], absPos.y + elem->pos.y + elem->margins[0],
					viewSizex, viewSizey,
					uvX, uvY, uvW, uvH,
					bg ? bg->bs.boRadiuses[0] : 0, bg ? bg->bs.boRadiuses[1] : 0,
					bg ? bg->bs.boRadiuses[2] : 0, bg ? bg->bs.boRadiuses[3] : 0,
					bg ? bg->aa : 0,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0, 0xFFFFFFFF,
					10000 + std::floor(z), texSlots[0], g->fb, g->fbsize, g->viewId,
					0.0f,
					bg ? bg->cornerPattern : 0.0f,
					bg ? bg->backUVMinX : 0.0f,
					texSlots[1], texSlots[2], texSlots[3]);
			}
			if (g->paint == Offscreen::PaintOffscreen) return;
			elem->offscreen->paint = Offscreen::PaintOffscreen;
			// 書き込み: LayerInfoにタイル状態を設定し、push関数が自動的にタイル展開する
			TiledTextureInfo* tiledWrite = mygetTiledTextureInfo(thgc, writeId);
			if (tiledWrite && !tiledWrite->tiles.empty()) {
				auto& firstTile = tiledWrite->tiles[0];
				// タイル数分の論理viewIdをグローバルカウンタから確保
				uint64_t tileBaseId = ::viewId;
				::viewId += (uint64_t)tiledWrite->tiles.size();
				// LayerInfo 所有の安定 sentinel を使う (詳細は newelem.h 側コメント参照)
				g->layer->tiledTarget = tiledWrite;
				g->layer->tiledTargetImageId = writeId;
				g->layer->tiledPlaceholderFBO = &g->layer->_tiledSentinel;
				g->layer->tiledBaseViewId = tileBaseId;
				g2 = NewGraphic{ g->layer, elem, elem, {0, 0}, {elem->size2.x, elem->size2.y}, {0,0}, {0,0},
					writeId, g->layer->tiledPlaceholderFBO, &tiledWrite->totalSize,
					elem->offscreen->viewId = tileBaseId, 0, elem->offscreen->group };
			}
		}
		else {
			auto readInfo = mygetStandaloneTextureInfo(thgc, readId);
			// ページめくりアニメーション中
			if (elem->offscreen->curlProgress >= 0.0f && elem->offscreen->curlProgress <= 1.0f) {
				float fromPage = elem->offscreen->curlFromPage;
				float toPage = elem->offscreen->curlToPage;
				float progress = elem->offscreen->curlProgress;
				int axis = elem->offscreen->curlAxis;
				// 現在ページと次ページのUV
				float fromUvX, fromUvY, toUvX, toUvY;
				float uvW = viewSizex / size3x, uvH = viewSizey / size3y;
				if (axis == 0) {
					fromUvX = 0; fromUvY = fromPage * viewSizey / size3y;
					toUvX = 0;   toUvY = toPage * viewSizey / size3y;
				}
				else {
					fromUvX = fromPage * viewSizex / size3x; fromUvY = 0;
					toUvX = toPage * viewSizex / size3x;     toUvY = 0;
				}
				bool goingBack = toPage < fromPage;
				float curlAngle;
				if (axis == 0) {
					curlAngle = goingBack ? -1.5708f : 1.5708f;
				}
				else {
					curlAngle = goingBack ? 3.14159f : 0.0f;
				}
				// 戻る場合: progressを反転（両開き: 0.5→0, 片面: 1.0→0）
				float endProgress = elem->offscreen->spread ? 0.5f : 1.0f;
				float drawProgress = goingBack ? (endProgress - progress) : progress;
				float drawFromUvX = goingBack ? toUvX : fromUvX;
				float drawFromUvY = goingBack ? toUvY : fromUvY;
				float drawToUvX = goingBack ? fromUvX : toUvX;
				float drawToUvY = goingBack ? fromUvY : toUvY;
				auto* bg = elem->background;
				g->layer->pushPageCurl(absPos.x + elem->pos.x + elem->margins[3], absPos.y + elem->pos.y + elem->margins[0], viewSizex, viewSizey,
					drawProgress, -30.0f, curlAngle,
					0xFFFFFFFF,
					drawFromUvX, drawFromUvY, uvW, uvH,
					drawToUvX, drawToUvY,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0,
					&readInfo->handle, &readInfo->handle, 10000 + std::floor(z),
					g->fb, g->fbsize, g->viewId, 0.0f, axis == 0, elem->offscreen->spread);
				// アニメーション進行
				elem->offscreen->curlProgress += g->deltaTime * 2.0f;
				if (elem->offscreen->curlProgress > endProgress) {
					elem->offscreen->curlProgress = -1.0f;
					// アニメーション完了、scroll を toPage に確定。
					// fromPage / toPage が N 離れていても 1 回のカールで遷移完了
					// (= shader が fromUv/toUv のテクスチャを直接ブレンドする)。
					if (axis == 0) elem->scroll.y = toPage * viewSizey;
					else elem->scroll.x = toPage * viewSizex;
				}
				elem->offscreen->markPaint(local);
			}
			else {
				auto* bg = elem->background;
				float imgX = absPos.x + elem->pos.x + elem->margins[3];
				float imgY = absPos.y + elem->pos.y + elem->margins[0];
				float imgW = viewSizex;
				float imgH = viewSizey;
				g->layer->pushImage(imgX, imgY, imgW, imgH,
					(elem->scroll.x) / size3x, (elem->scroll.y) / size3y, (imgW + tabExtL + tabExtR) / size3x, (imgH + tabExtT + tabExtB) / size3y,
					bg ? bg->bs.boRadiuses[0] : 0, bg ? bg->bs.boRadiuses[1] : 0, bg ? bg->bs.boRadiuses[2] : 0, bg ? bg->bs.boRadiuses[3] : 0,
					bg ? bg->aa : 0,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0, 0xFFFFFFFF,
					10000 + std::floor(z), &readInfo->handle, g->fb, g->fbsize, g->viewId,
					0.0f,
					bg ? bg->cornerPattern : 0.0f,
					bg ? bg->backUVMinX : 0.0f);
			}
			if (g->paint == Offscreen::PaintOffscreen) return;
			elem->offscreen->paint = Offscreen::PaintOffscreen;
			auto writeInfo = mygetStandaloneTextureInfo(thgc, writeId);
			uint64_t offscreenViewId = ::viewId++;
			g2 = NewGraphic{ g->layer, elem, elem, {tabExtL, tabExtT}, {elem->size2.x + tabExtL + tabExtR, elem->size2.y + tabExtT + tabExtB}, {0,0}, {0,0},
				writeId, &writeInfo->fbo, &writeInfo->size, elem->offscreen->viewId = offscreenViewId, 0, elem->offscreen->group };
		}
		elem->offscreen->ping = !elem->offscreen->ping;
		std::vector<float>* widths = new std::vector<float>{ 10.0f, 10.0f, 10.0f };
		int size = widths->size();
		g2.zIndex = z;
	}
	else {
		g2 = NewGraphic{ g->layer, elem, elem, {g->pos.x, g->pos.y}, {elem->size2.x, elem->size2.y}, {0,0}, {0,0},
			g->im,  g->fb,  g->fbsize, g->viewId, 0, g->group };
		g2.pos.x += elem->margins[3];
		g2.pos.y += elem->margins[0];
		g2.zIndex = g->zIndex + elem->zIndex;
	}
	g2.start = g->pos;

	if (elem->background != NULL) {
		bgfx::TextureHandle* tex1, * tex2, * tex3, * tex4;
		// 全 placement 共通: myResolveSlot で stable な &slot.handle を取る。
		if (isValidImageId(elem->background->tex1)) {
			auto* slot = myResolveSlot(thgc, elem->background->tex1);
			tex1 = slot ? &slot->handle : &nulltex;
		}
		else tex1 = &nulltex;
		if (isValidImageId(elem->background->tex2)) {
			auto* slot = myResolveSlot(thgc, elem->background->tex2);
			tex2 = slot ? &slot->handle : &nulltex;
		}
		else tex2 = &nulltex;
		if (isValidImageId(elem->background->tex3)) {
			auto* slot = myResolveSlot(thgc, elem->background->tex3);
			tex3 = slot ? &slot->handle : &nulltex;
		}
		else tex3 = &nulltex;
		if (isValidImageId(elem->background->tex4)) {
			auto* slot = myResolveSlot(thgc, elem->background->tex4);
			tex4 = slot ? &slot->handle : &nulltex;
		}
		else tex4 = &nulltex;
		bool hasOffscreen = elem->offscreen != NULL;
		float bgW = std::max(elem->size2.x, elem->size.x + elem->paddings[1] + elem->paddings[3]) + tabExtL + tabExtR;
		float bgH = std::max(elem->size2.y, elem->size.y + elem->paddings[0] + elem->paddings[2]) + tabExtT + tabExtB;
		float noBo[4] = {0, 0, 0, 0};
		g2.layer->pushBackground(elem->background, g2.pos.x - tabExtL, g2.pos.y - tabExtT, bgW, bgH,
				std::floor(g2.zIndex) + 0.1,
				tex1, tex2, tex3, tex4, g2.fb, g2.fbsize, g2.viewId, 0.0f,
				hasOffscreen ? noBo : elem->boWidthes,
				hasOffscreen ? &noBorderShadow : &elem->background->bs);
	}
	g2.pos.x += elem->boWidthes[3] + elem->paddings[3];
	g2.pos.y += elem->boWidthes[0] + elem->paddings[0];
	float baseX = g2.pos.x, baseY = g2.pos.y;
	NewElement* start = elem->childend->next;
	//background.draw
	float szx = 0, szy = 0;
	for (NewElement* child = elem->childend->next; child != elem->childend; ) {
		if (child->offscreen != NULL) { child = child->next; continue; }
		g2.pos.x = baseX + child->pos2.x + child->pos.x;
		g2.pos.y = baseY + child->pos2.y + child->pos.y;
		if (child == tab->select) {
			if (tab->direction == 1) g->layer->pushFill(g2.pos.x, g2.pos.y, child->size.x + child->paddings[1] + child->paddings[3], child->size.y + child->paddings[0] + child->paddings[2], 4.0f, 4.0f, 6.0f, 6.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.3f, 0xCCCCFFFF, 0x000000FF, 1.0f, 1.0f, 1.0f, 0, std::floor(g2.zIndex) + 0.2, g2.fb, g2.fbsize, g2.viewId, 0, tab->direction);
			else if (tab->direction == 2) g->layer->pushFill(g2.pos.x, g2.pos.y, child->size.x + child->paddings[1] + child->paddings[3], child->size.y + child->paddings[0] + child->paddings[2], 6.0f, 6.0f, 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.3f, 0xCCCCFFFF, 0x000000FF, 1.0f, 1.0f, 1.0f, 0, std::floor(g2.zIndex) + 0.2, g2.fb, g2.fbsize, g2.viewId, 0, tab->direction);
			else if (tab->direction == 3) g->layer->pushFill(g2.pos.x, g2.pos.y, child->size.x + child->paddings[1] + child->paddings[3], child->size.y + child->paddings[0] + child->paddings[2], 4.0f, 6.0f, 4.0f, 6.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.3f, 0xCCCCFFFF, 0x000000FF, 1.0f, 1.0f, 1.0f, 0, std::floor(g2.zIndex) + 0.2, g2.fb, g2.fbsize, g2.viewId, 0, tab->direction);
			else if (tab->direction == 4) g->layer->pushFill(g2.pos.x, g2.pos.y, child->size.x + child->paddings[1] + child->paddings[3], child->size.y + child->paddings[0] + child->paddings[2], 6.0f, 4.0f, 6.0f, 4.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.3f, 0xCCCCFFFF, 0x000000FF, 1.0f, 1.0f, 1.0f, 0, std::floor(g2.zIndex) + 0.2, g2.fb, g2.fbsize, g2.viewId, 0, tab->direction);
		}
		child->Draw(child->gc, child, &g2, child->gc->local, q);
		child = child->next;
	}
	// スクロールバー描画は ↑ オフスクリーン処理の前に winFb 上で済ませた。
	// 旧コード (offscreen FBO への描画) は PaintOffscreen で early-return すると
	// scroll 値変化に追従しない (FBO cache に古い位置が残る) ため削除。
	// タイル描画状態を親の値で復元 (nullptr で潰すと外側のタイル展開が壊れる)
	g->layer->tiledTarget         = _saveTiledTarget;
	g->layer->tiledTargetImageId  = _saveTiledTargetImageId;
	g->layer->tiledPlaceholderFBO = _saveTiledPlaceholderFBO;
	g->layer->tiledBaseViewId     = _saveTiledBaseViewId;
}
int TabMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	NewTabBar* tab = (NewTabBar*)self;
	if (self->offscreened) e->group = self->offscreen->group;
	if (self->GoMouseDown != NULL)
		FireEvent(thgc, self, self->GoMouseDown, _MouseEvent, e, sizeof(MouseEvent));

	// スクロールバー / ページングバーのクリック判定 (MouseDown時のみ開始)
	if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float sbarx = 0, sbary = 0;
		if (baseType(self->xtype) == SizeType::Scroll) sbary += 5 * 2;
		else if (baseType(self->xtype) == SizeType::Page) sbary += 15;
		if (baseType(self->ytype) == SizeType::Scroll) sbarx += 5 * 2;
		else if (baseType(self->ytype) == SizeType::Page) sbarx += 15;
		float sizex = self->size.x + self->paddings[1] + self->paddings[3];
		float sizey = self->size.y + self->paddings[0] + self->paddings[2];
		float localX = e->x - pos.x - self->pos.x - self->margins[3] - self->boWidthes[3];
		float localY = e->y - pos.y - self->pos.y - self->margins[0] - self->boWidthes[0];

		// 右スクロールバー (ytype == Scroll)
		if (baseType(self->ytype) == SizeType::Scroll && self->size2.y > sizey) {
			float barX = sizex - sbarx;
			float barH = sizey - sbary;
			if (localX >= barX && localX < sizex && localY >= 0 && localY < barH) {
				float maxScroll = self->size2.y - sizey;
				// つまみの位置とサイズを計算
				float thumbRatio = sizey / self->size2.y;
				float thumbH = std::max(barH * thumbRatio, 16.f);
				float scrollRatio = (maxScroll > 0) ? self->scroll.y / maxScroll : 0.f;
				float thumbY = (barH - thumbH) * scrollRatio;
				// つまみ上をクリックしたか
				if (localY >= thumbY && localY < thumbY + thumbH) {
					local->scrollDrag = { self, 0, localY - thumbY };
				}
				else if (localY < thumbY) {
					// つまみより上: つまみ上端がクリック位置に来る
					float newRatio = std::clamp(localY / (barH - thumbH), 0.f, 1.f);
					self->scroll.y = newRatio * maxScroll;
					local->scrollDrag = { self, 0, 0.f };
				}
				else {
					// つまみより下: つまみ下端がクリック位置に来る
					float newThumbY = localY - thumbH;
					float newRatio = std::clamp(newThumbY / (barH - thumbH), 0.f, 1.f);
					self->scroll.y = newRatio * maxScroll;
					local->scrollDrag = { self, 0, thumbH };
				}
				if (self->offscreen) self->offscreen->markPaint(local);
				return -1;
			}
		}
		// 下スクロールバー (xtype == Scroll)
		if (baseType(self->xtype) == SizeType::Scroll && self->size2.x > sizex) {
			float barY = sizey - sbary;
			float barW = sizex - sbarx;
			if (localY >= barY && localY < sizey && localX >= 0 && localX < barW) {
				float maxScroll = self->size2.x - sizex;
				// つまみの位置とサイズを計算
				float thumbRatio = sizex / self->size2.x;
				float thumbW = std::max(barW * thumbRatio, 16.f);
				float scrollRatio = (maxScroll > 0) ? self->scroll.x / maxScroll : 0.f;
				float thumbX = (barW - thumbW) * scrollRatio;
				// つまみ上をクリックしたか
				if (localX >= thumbX && localX < thumbX + thumbW) {
					local->scrollDrag = { self, 1, localX - thumbX };
				}
				else if (localX < thumbX) {
					// つまみより左: つまみ左端がクリック位置に来る
					float newRatio = std::clamp(localX / (barW - thumbW), 0.f, 1.f);
					self->scroll.x = newRatio * maxScroll;
					local->scrollDrag = { self, 1, 0.f };
				}
				else {
					// つまみより右: つまみ右端がクリック位置に来る
					float newThumbX = localX - thumbW;
					float newRatio = std::clamp(newThumbX / (barW - thumbW), 0.f, 1.f);
					self->scroll.x = newRatio * maxScroll;
					local->scrollDrag = { self, 1, thumbW };
				}
				if (self->offscreen) self->offscreen->markPaint(local);
				return -1;
			}
		}
		// 右ページングバー (ytype == Page)
		if (baseType(self->ytype) == SizeType::Page && self->size2.y > sizey) {
			float barX = sizex - sbarx;
			if (localX >= barX && localX < sizex && localY >= 0 && localY < sizey) {
				float totalPages = std::ceil(self->size2.y / sizey);
				float currentPage = std::floor(self->scroll.y / sizey);
				float toPage = currentPage;
				if (localY < sizey * 0.5f) {
					if (currentPage > 0) toPage = currentPage - 1;
				}
				else {
					if (currentPage < totalPages - 1) toPage = currentPage + 1;
				}
				if (toPage != currentPage && self->offscreen) {
					self->offscreen->curlFromPage = currentPage;
					self->offscreen->curlToPage = toPage;
					self->offscreen->curlProgress = 0.0f;
					self->offscreen->curlAxis = 0;
					self->offscreen->markPaint(local);
				}
				return -1;
			}
		}
		// 下ページングバー (xtype == Page)
		if (baseType(self->xtype) == SizeType::Page && self->size2.x > sizex) {
			float barY = sizey - sbary;
			if (localY >= barY && localY < sizey && localX >= 0 && localX < sizex) {
				float totalPages = std::ceil(self->size2.x / sizex);
				float currentPage = std::floor(self->scroll.x / sizex);
				float toPage = currentPage;
				if (localX < sizex * 0.5f) {
					if (currentPage > 0) toPage = currentPage - 1;
				}
				else {
					if (currentPage < totalPages - 1) toPage = currentPage + 1;
				}
				if (toPage != currentPage && self->offscreen) {
					self->offscreen->curlFromPage = currentPage;
					self->offscreen->curlToPage = toPage;
					self->offscreen->curlProgress = 0.0f;
					self->offscreen->curlAxis = 1;
					self->offscreen->markPaint(local);
				}
				return -1;
			}
		}
	}

	pos.x += self->pos.x + self->margins[3] + self->boWidthes[3] + self->paddings[3] - self->scroll.x;
	pos.y += self->pos.y + self->margins[0] + self->boWidthes[0] + self->paddings[0] - self->scroll.y;
	if (self->childend != NULL) {
		NewElement* elem = self->childend->next;
		if (self->orient) {
			if (e->x - pos.x < elem->pos.x + elem->pos2.x + elem->margins[3]) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->next);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = 0;
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = 0;
						}
					}
				}
			}
		}
		else {
			if (e->y - pos.y < elem->pos.y + elem->pos2.y + elem->margins[0]) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->next);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = 0;
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = 0;
						}
					}
				}
			}
		}
		for (; elem != self->childend; elem = elem->next) {
			float hitL = 0, hitR = 0, hitT = 0, hitB = 0;
			if (elem->type == _Sidelet && elem->background) {
				Sidelet* sidelet = (Sidelet*)elem;
				switch (sidelet->direction) {
				case 0: hitT = Sctf(18, 2.0f); break;
				case 1: hitR = Sctf(18, 2.0f); break;
				case 2: hitB = Sctf(18, 2.0f); break;
				case 3: hitL = Sctf(18, 2.0f); break;
				}
			}
			if (self->orient) {
				if (elem->pos.x + elem->pos2.x + elem->margins[3] - hitL <= e->x - pos.x && e->x - pos.x < elem->pos.x + elem->pos2.x + elem->size.x + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3] + hitR) {
					if (elem->hoverColor != 0) {
						add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
					}
					elem->Mouse(elem->gc, elem, e, { pos.x + elem->pos2.x, pos.y }, elem->gc->local);
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						tab->select = elem;
						if (elem->type == LetterType::_TabTitle) {
							NewTabTitle* title = (NewTabTitle*)elem;
							tab->linked->page = title->page;
							markLayoutOf(tab->linked, local);
							FindOffscreen(tab)->markPaint(local, Offscreen::PaintCommand);
							// ページが別スレッド所有の NewLocal なら hoppy->target も切替
							if (title->page && title->page->type == LetterType::_Main) {
								NewLocal* pageLocal = (NewLocal*)title->page;
								if (pageLocal->gc && pageLocal->gc->hoppy) {
									HopStar_set_target(pageLocal->gc->hoppy, pageLocal->gc);
								}
							}
						}
					}
				}
			}
			else {
				if (elem->pos.y + elem->pos2.y + elem->margins[0] - hitT <= e->y - pos.y && e->y - pos.y < elem->pos2.y + elem->size.y + elem->margins[0] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2] + hitB) {
					if (elem->hoverColor != 0) {
						add_list(elem->gc, elem->gc->hoveredList, (char*)elem);
					}
					elem->Mouse(elem->gc, elem, e, { pos.x, pos.y + elem->pos2.y }, elem->gc->local);
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						tab->select = elem;
						if (elem->type == LetterType::_TabTitle) {
							NewTabTitle* title = (NewTabTitle*)elem;
							tab->linked->page = title->page;
							markLayoutOf(tab->linked, local);
							FindOffscreen(tab)->markPaint(local, Offscreen::PaintCommand);
							// ページが別スレッド所有の NewLocal なら hoppy->target も切替
							if (title->page && title->page->type == LetterType::_Main) {
								NewLocal* pageLocal = (NewLocal*)title->page;
								if (pageLocal->gc && pageLocal->gc->hoppy) {
									HopStar_set_target(pageLocal->gc->hoppy, pageLocal->gc);
								}
							}
						}
					}
				}
			}
		}
		if (self->orient) {
			elem = elem->before;
			if (elem->pos.x + elem->pos2.x + elem->size.x + elem->margins[3] + elem->boWidthes[1] + elem->boWidthes[3] + elem->paddings[1] + elem->paddings[3] <= e->x - pos.x) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->before);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = el->len(el);
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = el->len(el);
						}
					}
				}
			}
		}
		else {
			elem = elem->before;
			if (elem->pos.y + elem->pos2.y + elem->size.y + elem->margins[0] + elem->boWidthes[0] + elem->boWidthes[2] + elem->paddings[0] + elem->paddings[2] <= e->y - pos.y) {
				NewElement* el = elem;
				for (; el->childend != NULL; el = el->childend->before);
				if (el->parent->parent && el->parent->parent->editable != EditType::NoSelect) {
					if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
						local->select.from = local->select.to = el;
						local->select.fromid = local->select.toid = el->id;
						local->select.m = local->select.n = el->len(el);
						Offscreen* seloff = FindOffscreen(el);
						local->select.window = seloff ? seloff->window : nullptr;
					}
					else if (e->action == SDL_EVENT_MOUSE_BUTTON_UP || e->click) {
						Offscreen* tooff = FindOffscreen(el);
						NativeWindow* toWin = tooff ? tooff->window : nullptr;
						if (toWin == local->select.window) {
							local->select.to = el;
							local->select.toid = el->id;
							local->select.n = el->len(el);
						}
					}
				}
			}
		}
	}

	if (self->BackMouseDown != NULL)
		FireEvent(thgc, self, self->BackMouseDown, _MouseEvent, e, sizeof(MouseEvent));
	return -1;
}
void initTabTitle(ThreadGC* thgc, NewTabTitle* title) {
	title->gc = thgc;
	title->type = LetterType::_TabTitle;
	title->next = title->before = title;
	title->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, title->childend, title);
	title->Measure = ElementMeasure;
	title->Draw = ElementDraw;
	title->Mouse = ElementMouse;
	title->Key = ElementKey;
	title->DrawSelection = ImageDrawSelect;
	title->len = len1;
	title->page = NULL;
	title->paddings[0] = title->paddings[2] = 1;
	title->paddings[1] = title->paddings[3] = 6;
	title->animate = -1;
}
void initTab(ThreadGC* thgc, NewTabBar* tab) {
	tab->gc = thgc;
	tab->type = LetterType::_TabControl;
	tab->next = tab->before = tab;
	tab->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, tab->childend, tab);
	tab->Measure = TabMeasure;
	tab->Draw = TabDraw;
	tab->Mouse = TabMouse;
	tab->Key = ElementKey;
	tab->DrawSelection = ImageDrawSelect;
	tab->len = len1;
	tab->orient = true;
	//tab->strs = create_list(thgc, sizeof(String*), CType::_String);
	tab->size = { 800, 400 };
	tab->direction = 1;
	tab->linked = NULL;
	tab->background = (Background*)GC_alloc(thgc, CType::_Background);
	tab->background->fillcolor = 0x8080aaff;
	tab->background->type = DrawCommandType::Fill;
	tab->background->bs.borderColor = 0x404066ff;
	tab->boWidthes[2] = 1;
	tab->boWidthes[0] = 1;
	tab->animate = -1;
}
// ============================================================================
// LinkedElement: childendに指定した要素を自分の位置に表示するポータル
// childendを差し替えるだけでページ切替ができる
// ============================================================================
RetMeasureType LinkedMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	NewElement* target = ((LinkedElement*)elem)->page;
	if (target == NULL) return RetMeasure_None;

	NewMeasure newmeasure;
	newmeasure.pos = { 0, 0 };
	newmeasure.size = elem->size;
	newmeasure.start = { 0, 0 };
	newmeasure.group = measure->group;
	if (elem->xtype != SizeType::Auto && !isPercent(elem->xtype))
		newmeasure.floatRightX = elem->size.x;
	if (elem->ytype != SizeType::Auto && !isPercent(elem->ytype))
		newmeasure.floatRightY = elem->size.y;
	target->size = elem->size;
	return RetMeasure_None;
}
void LinkedDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	LinkedElement* linked = (LinkedElement*)elem;
	NewElement* target = linked->page;

	if (target == NULL || target->type == LetterType::_Main) return;
}
int LinkedMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	NewElement* target = ((LinkedElement*)self)->page;
	if (target == NULL) return -1;

	return target->Mouse(target->gc, target, e, pos, target->gc->local);
}
void initLinkedElement(ThreadGC* thgc, LinkedElement* linked) {
	linked->gc = thgc;
	linked->type = LetterType::_Linked;
	linked->next = linked->before = linked;
	linked->parent = NULL;
	linked->childend = NULL;
	linked->page = NULL;
	linked->Measure = LinkedMeasure;
	linked->Draw = LinkedDraw;
	linked->Mouse = LinkedMouse;
	linked->Key = ElementKey;
	linked->DrawSelection = ImageDrawSelect;
	linked->len = len1;
	linked->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
	linked->offscreen->type = OffscreenType::LinkedOff;
	linked->offscreen->group = &createGroup(thgc);
	linked->offscreen->elem = linked;
	linked->offscreen->curlProgress = -1.0f;
	linked->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
	linked->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
	linked->offscreen->next = linked->offscreen->before = linked->offscreen;
	linked->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
	linked->offscreen->child->next = linked->offscreen->child->before = linked->offscreen->child;
	linked->offscreen->child->parent = linked->offscreen;
	linked->animate = -1;
}
RetMeasureType TbMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* n) {
	ElementMeasure(thgc, elem, measure, local, n);
	TextBox* tb = (TextBox*)elem;
	if (tb->recompile) {
		tb->master->elem = elem;
		tb->master->lasttype = LetterType::_Kaigyou;
		bool error = false;
		tb->master->callblock = ParseStart(thgc, tb->master, &error);
		tb->master->comments = create_list(thgc, sizeof(Comment*), CType::_Comment);
		tb->master->blockslist = create_list(thgc, sizeof(List*), CType::_List);
		add_list(thgc, tb->master->blockslist, (char*)create_list(thgc, sizeof(List*), CType::_List));
		tb->master->callblock->vt->exeZ(thgc, tb->master->callblock, tb->master);
		bool error2 = false;
		tb->master->callblock->vt->exeA(thgc, tb->master->callblock, tb->master);
		Block_Show(thgc, *(Block**)get_list(tb->master->callblock->children, 1), "", &error2, tb->master);
		bool error3 = false;
		tb->master->callblock->vt->exeB(thgc, tb->master->callblock, tb->master);
		Block_Show(thgc, *(Block**)get_list(tb->master->callblock->children, 1), "@", &error3, tb->master);
	}
	return RetMeasure_None;
}
void SetText(ThreadGC* thgc, NewLocal* local,TextBox* tb, String* text) {
	tb->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, (NewEndElement*)tb->childend, tb);
	List* list = Compile(thgc, text, tb->font, NULL);
	// Compile はスパンを **絶対座標** で返すように変更したので、
	// 行頭の絶対位置 = 最初のスパンの start。SubString も絶対 start から取り出すだけ。
	for (int i = 0; i < list->size; i++) {
		NewLine* line = (NewLine*)GC_alloc(thgc, CType::_LineC);
		initLine(thgc, line);
		NewDirectAddLast(thgc, local, tb, line);
		List* list2 = *(List**)get_list(list, i);
		if (list2->size > 0) {
			NewLetter* let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
			initLetter(thgc, let, tb->font, LetterType::_Letter);
			let->recompile = false;
			ATSSpan* first = *(ATSSpan**)get_list(list2, 0);
			ATSSpan* last  = *(ATSSpan**)get_list(list2, list2->size - 1);
			int lineStart = first->start;            // 絶対 = この行の先頭
			int lineLen   = last->end - lineStart;   // 絶対終端 - 行頭
			let->text = SubString(thgc, text, lineStart, lineLen);
			let->atsspans = list2;
			// スパンを letter-相対化 (一括で行頭オフセットを引く)
			for (int j = 0; j < list2->size; j++) {
				ATSSpan* a = *(ATSSpan**)get_list(list2, j);
				a->start -= lineStart;
				a->end   -= lineStart;
				a->letter = let;
			}
			NewNext(thgc, local, line->childend, let);
		}
		// 空行 (list2.size == 0) は letter を作らない。NewLine だけで段組される。
	}
	tb->recompile = true;
}
void initTb(ThreadGC* thgc, TextBox* tb) {
	tb->gc = thgc;
	tb->type = LetterType::_TextBox;
	tb->next = tb;
	tb->before = tb;
	tb->parent = NULL;
	tb->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, (NewEndElement*)tb->childend, tb);
	tb->id = 0;
	tb->Measure = TbMeasure;
	tb->Draw = ElementDraw;
	tb->Mouse = ElementMouse;
	tb->Key = ElementKey;
	tb->DrawSelection = ElementDrawSelect;
	tb->len = len1;
	tb->master = (Master*)GC_alloc(thgc, CType::_Master);
	tb->master->init(thgc, tb);
	tb->master->init2(thgc);
	tb->editable = EditType::Editable;
	tb->animate = -1;
}
struct Button : NewElement {

};
inline void ButtonCheck(ThreadGC* thgc, char* data) {
	traceNewElement(thgc, (NewElement*)data);
}
// 前方宣言: extorch.h で定義されるが、othelem.h が先にインクルードされるため。
#if !TARGET_OS_IOS && !TARGET_OS_SIMULATOR && !defined(__ANDROID__) && !defined(__linux__)
void runCompiledDllAsync(HopStar* hoppy, const char* dll_name);
#endif


inline NewElement* MakeImageView(ThreadGC* thgc, Bitmap* bm) {
	NewImage* im = (NewImage*)GC_alloc(thgc, CType::_ImageC);
	initImage(thgc, im);
	im->background = (Background*)GC_alloc(thgc, CType::_Background);
	if (bm) im->background->tex1 = bm->tex;
	im->background->type = DrawCommandType::RawImage;
	im->background->fillcolor = 0xFFFFFFFF;
	setPercentX(im, 1.0f, 0, SizeType::Range);
	setPercentY(im, 1.0f, 0, SizeType::Range);
	return (NewElement*)im;
}

inline NewElement* MakeAudioPlayer(ThreadGC* thgc, Sound* snd) {
	NewElement* e = (NewElement*)GC_alloc(thgc, CType::_ElementC);
	initElement(thgc, e);
	setPercentX(e, 1.0f, 0, SizeType::Range);
	setPercentY(e, 1.0f, 0, SizeType::Range);
	if (snd) Sound_Play(snd);
	return e;
}

inline NewElement* MakeVideoPlayer(ThreadGC* thgc, VideoSound* vs) {
	NewImage* im = (NewImage*)GC_alloc(thgc, CType::_ImageC);
	initImage(thgc, im);
	im->background = (Background*)GC_alloc(thgc, CType::_Background);
	if (vs) im->background->tex1 = vs->v.tex;
	im->background->type = DrawCommandType::RawImage;
	im->background->fillcolor = 0xFFFFFFFF;
	setPercentX(im, 1.0f, 0, SizeType::Range);
	setPercentY(im, 1.0f, 0, SizeType::Range);
	return (NewElement*)im;
}
// ============================================================
// View 化ヘルパ群 (encode.h の To* で得たデータを NewElement に貼る)
// ============================================================
inline NewElement* MakeTextBox(ThreadGC* thgc, String* text) {
	TextBox* tb = (TextBox*)GC_alloc(thgc, CType::_TextBoxC);
	initTb(thgc, tb);
	tb->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
	tb->offscreen->group = &createGroup(thgc);
	tb->offscreen->elem = tb;
	tb->offscreen->type = OffscreenType::Off;
	tb->offscreen->markLayout(thgc->local, tb);
	tb->offscreen->curlProgress = -1.0f;
	tb->offscreen->window = thgc->windows.empty() ? nullptr : thgc->windows[0];
	tb->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
	tb->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
	tb->offscreen->next = tb->offscreen->before = tb->offscreen;
	tb->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
	tb->offscreen->child->next = tb->offscreen->child->before = tb->offscreen->child;
	tb->offscreen->child->parent = NULL;
	tb->offscreened = true;
	tb->background = (Background*)GC_alloc(thgc, CType::_Background);
	tb->background->tex1 = myloadTexture2D(thgc, "123.png", ImageUsage::Background);
	tb->background->fillcolor = 0xffffffff;
	tb->background->bs.borderColor = 0x000000FF;
	tb->boWidthes[0] = tb->boWidthes[1] = tb->boWidthes[2] = tb->boWidthes[3] = 0.0f;
	tb->background->bs.shadowBlur = 1.0f;
	tb->background->bs.shadowColor = 0x00000000;
	tb->background->type = DrawCommandType::Fill;
	tb->zIndex = 0.0;
	tb->font = getFont("sans", 16);
	setPercentX(tb, 1.0f, 0, SizeType::Break);
	setPercentY(tb, 1.0f, 0, SizeType::Page);
	if (text) SetText(thgc, thgc->local, tb, text);
	return (NewElement*)tb;
}
// 既存タブバーに viewer をぶら下げる
inline void AddViewerToTab(ThreadGC* thgc, NewElement* viewer, const char* tabName) {
	SDL_Log("[AddViewerToTab] enter thgc=%p viewer=%p tabName=%s thgc->local=%p thgc->map=%p",
	        (void*)thgc, (void*)viewer, tabName, (void*)(thgc?thgc->local:nullptr), (void*)(thgc?thgc->map:nullptr));
	if (!viewer) { SDL_Log("[AddViewerToTab] viewer is null, return"); return; }
	String* tabKey = createString(thgc, (char*)"tab", 3, 1);
	NewElement* te = (NewElement*)get_mapy(thgc->map, tabKey);
	SDL_Log("[AddViewerToTab] get_mapy(tab) -> te=%p", (void*)te);
	if (!te) { SDL_Log("[AddViewerToTab] te==nullptr, EARLY RETURN (tab not registered in thgc->map)"); return; }
	NewTabBar* tab = (NewTabBar*)te;
	SDL_Log("[AddViewerToTab] tab=%p tab->linked=%p", (void*)tab, (void*)tab->linked);

	NewTabTitle* title = (NewTabTitle*)GC_alloc(thgc, CType::_TabTitleC);
	initTabTitle(thgc, title);
	NewDirectAddLast(thgc, thgc->local, tab, title);

	NewLetter* let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
	initLetter(thgc, let, thgc->local->font, LetterType::_Letter);
	let->text = createString(thgc, (char*)tabName, (int)strlen(tabName), 1);
	NewElementAddLast(thgc, thgc->local, title, let);

	tab->linked->page = viewer;
	tab->select       = title;
	title->page       = viewer;
	viewer->parent    = (NewElement*)tab->linked;   // event bubble / parent walk 用

	tab->linked->offscreen->markLayout(thgc->local, (NewElement*)tab->linked);
	markLayoutOf(tab, thgc->local);
	SDL_Log("[AddViewerToTab] done. tab->linked->page=%p tab->select=%p",
	        (void*)tab->linked->page, (void*)tab->select);
}


void ButtonClick(Frame* frame) {
	MemObj* mo = (MemObj*)frame->promise.blockobj;
	MouseEvent* act = (MouseEvent*)mo->action;
	ThreadGC* thgc = act->elem->gc;
	if (act->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		/*
		String* str = createString(act->elem->gc, (char*)"text", 4, 1);
		TreeElement* te = (TreeElement*)get_mapy(act->elem->gc->map, str);
		TextBox* tb = (TextBox*)te->elem;
		tb->master->elem = tb;
		tb->master->lasttype = LetterType::_Kaigyou;
		bool error = false;
		tb->master->callblock = ParseStart(tb->gc, tb->master, &error);
		tb->master->comments = create_list(tb->gc, sizeof(Comment*), CType::_Comment);
		tb->master->blockslist = create_list(tb->gc, sizeof(List*), CType::_List);
		add_list(tb->gc, tb->master->blockslist, (char*)create_list(tb->gc, sizeof(List*), CType::_List));
		tb->master->callblock->vt->exeZ(tb->gc, tb->master->callblock, tb->master);
		bool error2 = false;
		tb->master->callblock->vt->exeA(tb->gc, tb->master->callblock, tb->master);
		Block_Show(tb->gc, *(Block**)get_list(tb->master->callblock->children, 1), "", &error2, tb->master);
		bool error3 = false;
		tb->master->callblock->vt->exeB(tb->gc, tb->master->callblock, tb->master);
		Block_Show(tb->gc, *(Block**)get_list(tb->master->callblock->children, 1), "@", &error3, tb->master);
		if (!error && !error2 && !error3) {
			tb->master->callblock->vt->exeC(tb->gc, tb->master->callblock, tb->master);
			// LLVM IR を書き出して clang++/lld で clang3/tes.dll をビルド。
			// ビルドが成功したら別スレッドで DLL をロードし main エントリを実行する。
			// UI スレッド (このボタンクリックコンテキスト) はブロックされない。
			if (llvm_compile(tb->gc, tb->master->llvm, "tes")) {
#if !TARGET_OS_IOS && !TARGET_OS_SIMULATOR && !defined(__ANDROID__) && !defined(__linux__)
				// hoppy 経由で新規タブを生成し、そこで DLL を実行する。
				// tb->gc->hoppy は既に HopStar* を保持している。
				runCompiledDllAsync(tb->gc->hoppy, "tes");
#endif
			}
		}*/
		// LLDB が **HopStarBB.exe 自身** をもう一度起動して inferior として
		// デバッグする。clang3 ビルドの main.exe は使わない (= bgfx D3D11 の
		// clang/MSVC ABI 不一致を回避)。
		// ただし子プロセス側 (= 既に inferior として動いてる HopStarBB.exe) でこの
		// 行に到達したら、孫プロセスを生まない。env var "HOPSTARBB_LLDB_CHILD" を
		// startDebug が子に注入し、子側はここで早期 return する。
		// 環境変数チェック: getenv は POSIX 標準で Win/Linux/Mac/Android 全部で動く。
		if (getenv("HOPSTARBB_LLDB_CHILD") != NULL) {
			fprintf(stderr, "[LLDB] inferior 側のクリック: 孫 spawn は抑制\n");
		}
		else {
			// getLLDBClient が型チェック + 必要なら新規作成して "debug" キーに登録。
			//   * 実行中  → 停止 (= driver subprocess kill)
			//   * 未実行  → 開始 (= compile + debug)
			LLDBClient* cli = getLLDBClient(thgc);
			if (lldb_isRunning(cli)) {
				fprintf(stderr, "[LLDB] 実行中 → 停止要求\n");
				lldb_stop(cli);
			}
			else {
				fprintf(stderr, "[LLDB] 未実行 → 開始\n");
				lldb_compileAndDebug(thgc, cli, NULL);
			}
		}
	}
}
void initButton(ThreadGC* thgc, Button* button) {
	button->gc = thgc;
	button->type = LetterType::_Button;
	button->next = button;
	button->before = button;
	button->parent = NULL;
	button->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	for(int i = 0; i < 4; i++) button->margins[i] = Sc(5);
	// 内側余白 (text と border の間)。論理 dp 6 横、4 縦 のあたりが押しやすく見た目も自然。
	button->paddings[0] = button->paddings[2] = Sc(4);
	button->paddings[1] = button->paddings[3] = Sc(6);
	initNewEndElement(thgc, (NewEndElement*)button->childend, button);
	button->id = 0;
	button->Measure = ElementMeasure;
	button->Draw = ElementDraw;
	button->Mouse = ElementMouse;
	button->Key = ElementKey;
	button->DrawSelection = ElementDrawSelect;
	button->len = len1;
	button->background = (Background*)GC_alloc(thgc, CType::_Background);
	button->orient = true;
	std::vector<float> colors = std::vector<float>{
		0.865f, 0.865f, 0.500f, 1.0f,  // 上: #9090CC
		0.439f, 0.639f, 0.667f, 1.0f   // 下: #7070AA};
	};
	std::vector<float> widths = std::vector<float>{ 1.2f, 0.2f};
	int n = addPattern(thgc, colors, widths);
	button->background->offset = n;
	int size = widths.size();
	button->background->count = size;
	button->background->type = DrawCommandType::Gradient;
	for(int i = 0; i < 4; i++) button->background->bs.boRadiuses[i] = Sc(12);
	for (int i = 0; i < 4; i++) button->boWidthes[i] = Sc(1);
	button->background->bs.borderColor = 0x000000FF;
	button->background->bs.shadowColor = 0x80808000;
	button->background->bs.shadowBlur = Scf(1.5f);
	button->background->bs.shadowX = Scf(3.0f);
	button->background->bs.shadowY = Scf(3.0f);
	button->background->cornerPattern = 0;
	button->background->backUVMinX = 10.0;
	button->background->fillcolor = 0xffffffff;
	button->xalign = AlignType::CenterAlign;
	button->animate = -1;
}
Generator animateSlide(ThreadGC* thgc, Sidelet* s, float duration, bool direction) {
	float elapsed = 0;
	while (elapsed < duration) {
		float t = elapsed / duration;
		if (direction) s->animate = 1.0f - (1.0f - t) * (1.0f - t);  // ease-out quadratic
		else s->animate = (1.0 - t) * (1.0f - t);
		co_yield nullptr;              // 次フレームのresume_allで再開
		elapsed += thgc->deltaTime;
	}
	if (direction) s->animate = -1;
	else s->animate = -2;
	printf("anim: done\n");
	co_return nullptr;
}
void SideletDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	Sidelet* sidelet = (Sidelet*)elem;
	if (sidelet->hide) {
		if (elem->animate == -2) {
			if (sidelet->direction == 0) elem->pos.y = elem->size.y + elem->margins[0] + elem->margins[2] + elem->paddings[0] + elem->paddings[2] + elem->boWidthes[0] + elem->boWidthes[2];
			else if (sidelet->direction == 1) elem->pos.x = -(elem->size.x + elem->margins[1] + elem->margins[3] + elem->paddings[1] + elem->paddings[3] + elem->boWidthes[1] + elem->boWidthes[3]);
			else if (sidelet->direction == 2) elem->pos.y = -(elem->size.y + elem->margins[0] + elem->margins[2] + elem->paddings[0] + elem->paddings[2] + elem->boWidthes[0] + elem->boWidthes[2]);
			else if (sidelet->direction == 3) elem->pos.x = elem->size.x + elem->margins[1] + elem->margins[3] + elem->paddings[1] + elem->paddings[3] + elem->boWidthes[1] + elem->boWidthes[3];
		}
		else if (elem->animate == -1) {
			if (sidelet->direction == 0 || sidelet->direction == 2) elem->pos.y = 0;
			else if (sidelet->direction == 1 || sidelet->direction == 3) elem->pos.x = 0;
		}
		else {
			if (sidelet->direction == 0) elem->pos.y = (elem->size.y + elem->margins[0] + elem->margins[2] + elem->paddings[0] + elem->paddings[2] + elem->boWidthes[0] + elem->boWidthes[2]) * (1 - elem->animate);
			else if (sidelet->direction == 1) elem->pos.x = (elem->size.x + elem->margins[1] + elem->margins[3] + elem->paddings[1] + elem->paddings[3] + elem->boWidthes[1] + elem->boWidthes[3]) * (elem->animate - 1);
			else if (sidelet->direction == 2) elem->pos.y = (elem->size.y + elem->margins[0] + elem->margins[2] + elem->paddings[0] + elem->paddings[2] + elem->boWidthes[0] + elem->boWidthes[2]) * (elem->animate - 1);
			else if (sidelet->direction == 3) elem->pos.x = (elem->size.x + elem->margins[1] + elem->margins[3] + elem->paddings[1] + elem->paddings[3] + elem->boWidthes[1] + elem->boWidthes[3]) * (1 - elem->animate);
		}
	}
	ElementDraw(thgc, elem, g, local, q);
}
int SideletMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	Sidelet* sidelet = (Sidelet*)self;
	ElementMouse(thgc, self, e, pos, local);
	if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		SDL_Log("[Sidelet] BUTTON_DOWN e=(%.0f,%.0f) pos=(%.0f,%.0f) "
		        "self.pos=(%.0f,%.0f) self.size=(%.0f,%.0f) dir=%d percent=%.2f animate=%.2f hide=%d",
		        (float)e->x, (float)e->y, pos.x, pos.y,
		        self->pos.x, self->pos.y, self->size.x, self->size.y,
		        sidelet->direction, sidelet->percent, self->animate, (int)sidelet->hide);
	}
	float bx, ax, by, ay;
	if (sidelet->direction == 0) {
		bx = self->pos.x + (self->size.x + self->paddings[1] + self->paddings[3]) * sidelet->percent + self->margins[3] - Sctf(12, 2.0f);
		ax = bx + Sctf(24, 2.0f);
		ay = self->pos.y + self->margins[0];
		by = ay - Sctf(18, 2.0f);
	}
	else if (sidelet->direction == 1) {
		bx = self->pos.x + self->size.x + self->margins[3] + self->boWidthes[3] + self->paddings[3] + self->paddings[1];
		ax = bx + Sctf(18, 2.0f);
		by = self->pos.y + (self->size.y + self->paddings[0] + self->paddings[2]) * sidelet->percent + self->margins[0] - Sctf(12, 2.0f);
		ay = by + Sctf(24, 2.0f);
	}
	else if (sidelet->direction == 2) {
		bx = self->pos.x + (self->size.x + self->paddings[1] + self->paddings[3]) * sidelet->percent + self->margins[3] - Sctf(12, 2.0f);
		ax = bx + Sctf(24, 2.0f);
		by = self->pos.y + self->size.y + self->margins[0] + self->boWidthes[0] + self->paddings[3] + self->paddings[1];
		ax = by + Sctf(18, 2.0f);
	}
	else if (sidelet->direction == 3) {
		ax = self->pos.x + self->margins[3];
		bx = ax - Sctf(18, 2.0f);
		by = self->pos.y + (self->size.y + self->paddings[0] + self->paddings[2]) * sidelet->percent + self->margins[0] - Sctf(12, 2.0f);
		ay = by + Sctf(24, 2.0f);
	}
	if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float lx = (float)e->x - pos.x;
		float ly = (float)e->y - pos.y;
		bool xOk = (bx < lx && lx <= ax);
		bool yOk = (by < ly && ly <= ay);
		SDL_Log("[Sidelet] knob bx=%.0f ax=%.0f by=%.0f ay=%.0f localxy=(%.0f,%.0f) xOk=%d yOk=%d %s",
		        bx, ax, by, ay, lx, ly, (int)xOk, (int)yOk,
		        (xOk && yOk) ? "HIT" : "MISS");
	}
	if (bx < e->x - pos.x && e->x - pos.x <= ax) {
		if (by < e->y - pos.y && e->y - pos.y <= ay) {
			if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				// 現在の "表示状態" を判定。
				//   -1                 ⇒ 完全表示中
				//   -2                 ⇒ 完全非表示
				//   0 〜 1 のいずれか ⇒ アニメ進行中 / 進行待ち
				//     hide animation:  animate 1.0 → 0  (始点 = 表示中)
				//     show animation:  animate 0   → 1.0 (始点 = 非表示)
				//   旧コードは -1 / -2 にしか反応せず、進行中タップを完全に
				//   無視して "全く反応しない" 状態に陥っていた。
				//   進行中なら現在値から「より近い settled state」を推定して
				//   逆方向アニメを起動する (タップごとにトグル)。
				bool isCurrentlyVisible;
				if (self->animate == -1)        isCurrentlyVisible = true;
				else if (self->animate == -2)   isCurrentlyVisible = false;
				else                            isCurrentlyVisible = (self->animate >= 0.5f);
				SDL_Log("[Sidelet] HIT toggle: animate=%.2f -> isVisible=%d -> %s",
				        self->animate, (int)isCurrentlyVisible,
				        isCurrentlyVisible ? "HIDE" : "SHOW");
				if (!isCurrentlyVisible) {
					// 非表示 → 表示開始
					self->animate = 0;
					auto anim = animateSlide(thgc, sidelet, 1.0, true);
					auto coro = anim.coro;
					anim.coro = {};
					thgc->animQueue->push(coro);
				}
				else {
					// 表示中 → 非表示開始
					self->animate = 1.0;
					auto anim = animateSlide(thgc, sidelet, 1.0, false);
					auto coro = anim.coro;
					anim.coro = {};
					thgc->animQueue->push(coro);
				}
			}
		}
	}
	return 0;
}
void initSidelet(ThreadGC* thgc, Sidelet* sidelet) {
	sidelet->gc = thgc;
	sidelet->type = LetterType::_Sidelet;
	sidelet->next = sidelet;
	sidelet->before = sidelet;
	sidelet->parent = NULL;
	sidelet->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, (NewEndElement*)sidelet->childend, sidelet);
	sidelet->id = 0;
	sidelet->Measure = ElementMeasure;
	sidelet->Draw = SideletDraw;
	sidelet->Mouse = SideletMouse;
	sidelet->Key = ElementKey;
	sidelet->DrawSelection = ElementDrawSelect;
	sidelet->len = len1;
	sidelet->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
	sidelet->offscreen->group = &createGroup(thgc);
	sidelet->offscreen->elem = sidelet;
	sidelet->offscreen->type = OffscreenType::Off;
	sidelet->offscreen->markLayout(thgc->local, sidelet);
	sidelet->offscreen->curlProgress = -1.0f;
	sidelet->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
	sidelet->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
	sidelet->offscreen->next = sidelet->offscreen->before = sidelet->offscreen;
	sidelet->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
	sidelet->offscreen->child->next = sidelet->offscreen->child->before = sidelet->offscreen->child;
	sidelet->offscreen->child->parent = sidelet->offscreen;
	sidelet->offscreen->markLayout(thgc->local, sidelet);
	sidelet->offscreened = true;
	sidelet->background = (Background*)GC_alloc(thgc, CType::_Background);
	sidelet->background->type = DrawCommandType::Fill;
	for (int i = 0; i < 4; i++) sidelet->boWidthes[i] = Sc(1);
	sidelet->background->bs.borderColor = 0x000000FF;
	sidelet->background->bs.shadowColor = 0x80808000;
	sidelet->background->bs.shadowBlur = Scf(1.5f);
	sidelet->background->bs.shadowX = Scf(3.0f);
	sidelet->background->bs.shadowY = Scf(3.0f);
	sidelet->background->cornerPattern = 9 + sidelet->direction + sidelet->percent;
	// つまみは指で掴むので touch scale を効かせる。Windows では 12, Android では
	// 12 * 2.625 * 2.5 ≈ 79 px になり、指でしっかり掴める大きさ。
	sidelet->background->backUVMinX = Sctf(12.0);
	sidelet->background->fillcolor = 0xffffffff;
	sidelet->animate = -2;
}

// ============================================================
// NewTable: [i,j] でセルアクセスできる行列レイアウト要素。
//
// 設計 (Excel スタイル):
//   1. セルはぶっこみまくる (順序は問わない、(rowId, colId) hash でアクセス)
//   2. (rowId, colId) を char* に packed して Map (add_mapyn / get_mapyn) で検索
//   3. Column 情報・Row 情報はそれぞれ別 List で持つ (width / height + 安定 id)
//   4. freeze: 上 freezeRowCount 行 + 左 freezeColCount 列だけ固定 (End 固定なし、Excel と同じ)
//   5. オフスクリーンは「テーブル全体」を抱える 1 枚だけ
//      - Phase 1: 全セルを絶対座標で 1 回描画
//      - Phase 2: offscreen から 4 ブリット (TL / Top / Left / Center) で UV を切り出して overlay に貼る
// ============================================================

// 行/列メタ。安定 ID + 固定サイズ。auto は AutoFit API で明示的に行う。
//   GC オブジェクトとして alloc し、List / Map にはアドレスをそのまま格納する。
//   index = 現在 t->columns / t->rows の何番目か。挿入/削除で自動更新される想定。
//   autofit = true なら NewTableMeasure 内で全 cell サイズを見て width/height を自動調整。
struct NewTable;
struct LineInfo;
typedef LineInfo ColumnInfo;
typedef LineInfo RowInfo;
// 行・列クリック (= マウスイベント) ハンドラ統一シグネチャ。
//   NewTableMouse で hit した (i, j) について、その行 / 列 / cell (= NewElement*) も付随して渡す。
//   local も渡すので、選択状態 / mark layout / mark paint 等を直接更新できる。NULL は no-op。
typedef void (*TableCellHandler)(ThreadGC* thgc, NewLocal* local, NewTable* t,
	int i, int j, RowInfo* row, ColumnInfo* column, NewElement* cell,
	MouseEvent* e);
// 行の展開ハンドラ (= NewTable に 1 個)。childs を遅延構築する用途。
//   ユーザがクリックして RowInfo->expanded を true に切り替えた瞬間に呼ばれる。
//   childs が NULL なら caller (= ハンドラ) が driver 等から子要素を取得して埋める。
//   非同期で良い (= 後フレームの drain で childs を populate)。
//   ※ row 単位のコールバックは 8 byte/row 無駄になるので、テーブル単位で 1 つ持つ。
struct LineInfo;
struct NewTable;
typedef void (*RowExpandHandler)(ThreadGC* thgc, NewLocal* local, NewTable* t, LineInfo* row);
// 列 / 行のサイズモード。bool autofit + bool showLineNumbers/Letters を 1 enum に統合。
//   サイズ系 (下位 2bit, 排他):
//     Manual  = 0  : width/height を絶対 px として使う (default)
//     Auto   = 1  : 旧 autofit。属する cell の measure 後 max size を width/height にセット
//     Percent = 2  : newelem.h:1356 の式で解決
//                     pct = fmodf(width, 1.0);  mult = (pct == 0) ? 1.0 : pct;
//                     size = floor(remain * mult) + pct
//                     → 0.0 = 100%, 0.3 = 30%, 1.3 = 30% (整数部は cache)
//                     remain = elem->size.x — (Manual + Auto + ShowLines の合計)
//   フラグ:
//     ShowLines = 4 : 行番号 (列) / 列ラベル (行) を描画するヘッダ。サイズ系と OR 可能。
//                     例: Auto | ShowLines = 内容に合わせて autofit する行番号列
//                         Percent | ShowLines = 表幅の指定 % を取るヘッダ列
// サイズモードのみ (ShowLines / Collapsed はそれぞれ bool フィールドへ分離)。
// uint8_t にすることで RowInfo / ColumnInfo の bool フィールドと合わせて 4 byte に packing する。
enum class TableLineMode : uint8_t {
	Manual    = 0,
	Auto      = 1,
	Percent   = 2,
};
inline bool tlmIsAuto   (TableLineMode m) { return m == TableLineMode::Auto;    }
inline bool tlmIsPercent(TableLineMode m) { return m == TableLineMode::Percent; }

// 行/列共通の情報。中身が完全対称なので 1 構造体にまとめる。
// union で size / height / width を相互エイリアスにし、行→height、列→width で
// 既存コードがそのまま読み書きできる。
struct LineInfo {
	int   id;                         // [0..3]   4
	union {                           // [4..7]   4
		float size;
		float height;                 //   行として読むとき
		float width;                  //   列として読むとき
	};
	TableLineMode mode;               // [8]      1
	bool  expanded;                   // [9]      1   折り畳み状態
	bool  showLines;                  // [10]     1   ヘッダ (行番号 / 列ラベル)
	bool  visible;                    // [11]     1   描画対象か
	int   depth;                      // [12..15] 4   インデント用
	// list 上の位置 (= index) は LineInfo に持たない (= 挿入/削除で全件同期するコストを避ける)。
	// 必要なら walkVisibleLines の callback `i` を使う、もしくは List* を線形走査する。
	LineInfo* parent;                 // [16..23] 8   NULL = root level
	List*     childs;                 // [24..31] 8   List<LineInfo*>、NULL = 子なし
	TableCellHandler onClick;         // [32..39] 8
};
// LineInfo 自体は GC_alloc で _RowInfo / _ColumnInfo 型として確保される。
// parent (= LineInfo*) と childs (= List*) を trace。
inline void RowInfoCheck(ThreadGC* thgc, char* data) {
	LineInfo* l = (LineInfo*)data;
	l->parent = (LineInfo*)copy_object_young(thgc, (char*)l->parent);
	l->childs = (List*)    copy_object_young(thgc, (char*)l->childs);
}
// ColumnInfo == LineInfo (同じ tracer)
// エイリアス (= 既存コードはそのまま動く)
typedef LineInfo RowInfo;
typedef LineInfo ColumnInfo;

struct NewTable : NewElement {
	List*    columns;        // List<ColumnInfo>
	List*    rows;           // List<RowInfo>
	Map*     cells;          // (rowId, colId) packed → NewElement* (cell の中身そのもの)
	Map*     rowIdMap;       // rowId → (rowIndex+1)
	Map*     colIdMap;       // colId → (colIndex+1)
	int      nextRowId;
	int      nextColId;

	int      freezeRowCount;
	int      freezeColCount;

	// 描画先は elem->offscreen->imPing を直接使う (framework が確保 / 自動 resize)

	// 2D セル選択モード:
	//   true  → Mouse Down/Move でセル単位の矩形選択を行う (= Excel ライク)
	//           DrawSelection が選択範囲をハイライトする
	//   false → 選択処理をスキップ (= cell の Mouse 転送だけ。セル内部編集を優先したい時用)
	bool cellselect;

	// 2D 選択 (= path-ベース anchor + extent のみ、cache 持たない)。
	//   List* (= 呼び出し側が GC alloc した path) を直接指す (= 中身コピーしない)。NULL = 未設定。
	//   選択範囲は anchor↔extent の visible-DFS 順 [min..max] (= 描画時に都度算出)。
	List* selAnchorRow;     // List<int>: path (root→anchor row)、NULL = 未設定
	List* selAnchorCol;
	List* selExtentRow;
	List* selExtentCol;

	// 編集中 cell (= DoubleClick で入った cell)。grid を超えて全 content を overlay 表示する。
	//   path-ベース (= List<int>): expand/collapse でも安定 ID。size==0 なら編集中なし。
	//   Draw 時に通常 cell ループの後で、この cell を高 zIndex で再描画 (= 上にかぶせる)。
	List* editRowPath;      // List<int> path (root→edit row)
	List* editColPath;      // List<int> path
	// editCell overlay 用の clip rect。winFb 直 push 時のはみ出し防止。
	//   Draw 開始時に (screenX, screenY, cellW, cellH) を入れて
	//   layer->currentScissor に &editScissor を渡す。renderAllCommands まで生存する必要があるため
	//   stack ではなく構造体メンバとして保持。
	ScissorRect editScissor;

	// グリッド線の表示
	//   showGridX : 縦線 (= 列境界) を描画するか
	//   showGridY : 横線 (= 行境界) を描画するか
	//   gridColor : グリッド線の色 (RGBA)
	bool     showGridX;
	bool     showGridY;
	uint32_t gridColor;

	// === ツリービュー (= デバッガ風変数インスペクタ) ===
	//   treeView      : true なら最初の非 header 列に [+]/[-] 展開アイコン + インデント描画
	//   treeIndent    : 階層あたりのインデント幅 (px)
	//   treeIconSize  : [+]/[-] アイコンの一辺サイズ (px)
	//   treeLineColor : アイコン枠 + 接続線の色
	bool     treeView;
	float    treeIndent;
	float    treeIconSize;
	uint32_t treeLineColor;

	// setTableCell(t, i, j, ...) で i, j が現在の rows/columns 範囲外だった場合の自動追加に使うサイズ。
	float    defaultColWidth;
	float    defaultRowHeight;

	// ヘッダ (= 行番号 / 列ラベル) のテキスト / 背景色。
	// 「どの列 / どの行 がヘッダか」自体は ColumnInfo::showLineNumbers / RowInfo::showColumnLetters で指定。
	uint32_t headerTextColor;
	uint32_t headerBgColor;

	// ヘッダ用 String* キャッシュ。NumberString / columnLetterString は index に対して決定的なので、
	// 毎フレーム再生成せず List に詰めて使い回す。
	//   lineNumberCache : 各 List 要素 = NumberString(idx + 1)、index 0 → "1"
	//   columnLetterCache : 各 List 要素 = columnLetterString(idx)、index 0 → "A"
	// 行/列の挿入・削除でインデックスが移動してもキャッシュエントリの内容は変わらない (= 不変)。
	List* lineNumberCache;
	List* columnLetterCache;

	// 表全体の cell クリックハンドラ (= どの cell でも発火)。
	// ColumnInfo::onClick / RowInfo::onClick (= per 列・行) と並行して発火する。
	TableCellHandler onCellClick;
	// 行展開ハンドラ (= treeView のクリック展開時に呼ばれる)。
	// row 単位ではなく table 単位 (= 8 byte/row 節約)。NULL なら何もしない。
	// 呼ばれるタイミング: ユーザが [+]/[-] アイコンをクリックして expanded が
	// false → true に切り替わった瞬間 (= データ未取得状態の展開要求)。
	// 既に childs が埋まっている場合は呼ばない (= キャッシュ済とみなす)。
	RowExpandHandler onExpand;
	NewElement* childend2;

	// 次回 NewTableMeasure で再 measure する cell の list (= cell-level dirty 追跡)。
	// setTableCell や markCellChange 経由で cell が登録され、measure 後に clear。
	// expand/collapse は登録しない (= cell measure をスキップしつつ resolve/relayout だけ走る)。
	List* dirtyCells;
};
inline void NewTableCheck(ThreadGC* thgc, char* data) {
	NewTable* t = (NewTable*)data;
	traceNewElement(thgc, (NewElement*)t);
	t->columns           = (List*)copy_object_young(thgc, (char*)t->columns);
	t->rows              = (List*)copy_object_young(thgc, (char*)t->rows);
	t->cells             = (Map*) copy_object_young(thgc, (char*)t->cells);
	t->rowIdMap          = (Map*) copy_object_young(thgc, (char*)t->rowIdMap);
	t->colIdMap          = (Map*) copy_object_young(thgc, (char*)t->colIdMap);
	t->selAnchorRow      = (List*)copy_object_young(thgc, (char*)t->selAnchorRow);
	t->selAnchorCol      = (List*)copy_object_young(thgc, (char*)t->selAnchorCol);
	t->selExtentRow      = (List*)copy_object_young(thgc, (char*)t->selExtentRow);
	t->selExtentCol      = (List*)copy_object_young(thgc, (char*)t->selExtentCol);
	t->editRowPath       = (List*)copy_object_young(thgc, (char*)t->editRowPath);
	t->editColPath       = (List*)copy_object_young(thgc, (char*)t->editColPath);
	t->lineNumberCache   = (List*)copy_object_young(thgc, (char*)t->lineNumberCache);
	t->columnLetterCache = (List*)copy_object_young(thgc, (char*)t->columnLetterCache);
	t->childend2         = (NewElement*)copy_object_young(thgc, (char*)t->childend2);
	t->dirtyCells        = (List*)copy_object_young(thgc, (char*)t->dirtyCells);
}

// 編集中 cell ヘルパの forward 宣言 (NewTableDraw 内で参照される)
inline bool isTableEditCellLine(NewTable* t, LineInfo* row, LineInfo* col);
inline LineInfo* tableEditRow(NewTable* t);
inline LineInfo* tableEditCol(NewTable* t);
inline NewElement* tableEditCell(NewTable* t);
// 行/列追加の forward 宣言 (setTableCell の自動拡張で使う; 実体はもっと下)
inline int addTableRow   (ThreadGC* thgc, NewTable* t, float height);
inline int addTableColumn(ThreadGC* thgc, NewTable* t, float width);

// ヘッダ String* キャッシュ取得。指定 index までキャッシュが足りなければ末尾に append で延ばす。
inline String* getCachedLineNumber(ThreadGC* thgc, NewTable* t, int rowIdx);
inline String* getCachedColumnLetter(ThreadGC* thgc, NewTable* t, int colIdx);

// 列インデックス (0-base) → Excel 風列ラベル ("A", "B", ..., "Z", "AA", "AB", ..., "AZ", "BA", ...)。
//   esize=1 ASCII 文字列を返す。drawString が直接描画できる形。
inline String* columnLetterString(ThreadGC* thgc, int colIndex) {
	if (colIndex < 0) return createString(thgc, (char*)"", 0, 1);
	char buf[8];
	int len = 0;
	int n = colIndex;
	do {
		buf[len++] = (char)('A' + (n % 26));
		n = n / 26 - 1;
	} while (n >= 0 && len < (int)sizeof(buf));
	// 逆順
	for (int i = 0, j = len - 1; i < j; i++, j--) {
		char tmp = buf[i]; buf[i] = buf[j]; buf[j] = tmp;
	}
	return createString(thgc, buf, len, 1);
}

inline String* getCachedLineNumber(ThreadGC* thgc, NewTable* t, int rowIdx) {
	if (rowIdx < 0) return createString(thgc, (char*)"", 0, 1);
	while (t->lineNumberCache->size <= rowIdx) {
		int idx = t->lineNumberCache->size;
		String* s = NumberString(thgc, idx + 1);
		add_list(thgc, t->lineNumberCache, (char*)s);
	}
	return (String*)*get_list2(t->lineNumberCache, rowIdx);
}
inline String* getCachedColumnLetter(ThreadGC* thgc, NewTable* t, int colIdx) {
	if (colIdx < 0) return createString(thgc, (char*)"", 0, 1);
	while (t->columnLetterCache->size <= colIdx) {
		int idx = t->columnLetterCache->size;
		String* s = columnLetterString(thgc, idx);
		add_list(thgc, t->columnLetterCache, (char*)s);
	}
	return (String*)*get_list2(t->columnLetterCache, colIdx);
}

// (rowId, colId) → Map のキー。
//   素直に (rowId<<32)|colId と並べると、Map の hash 関数 (= 内部で `% 2^n`) が低位ビット
//   しか使わないため rowId 部分が完全に消え、同 colId のセル同士が全部同じバケットに
//   落ちて chain 線形成長 → 性能崩壊する。
//   そこで rowId に大きな奇数 (2654435761 = ⌊2^32 / φ⌋, Knuth の乗法ハッシュ定数) を
//   掛けて colId を足す = 単純な hash combine。奇数倍なので 2^n 法で rowId は全単射に
//   分散し、低位ビットに rowId 由来の差が必ず効く。
//   rowId, colId が共に < 2^31 の範囲なら (rowId, colId) → key は単射。
inline char* tableKey(int rowId, int colId) {
	uint64_t pack = (uint64_t)(uint32_t)rowId * 2654435761ULL + (uint32_t)colId;
	return (char*)(uintptr_t)pack;
}

// 汎用 (List* に i を直接適用)。expand などで「可視 line のみのリスト」を渡したい
// ケースに対応するため、index アクセス系は全部 List* を受ける。
inline LineInfo* tableLine(List* lines, int i) {
	if ((unsigned)i >= (unsigned)lines->size) return NULL;
	return (LineInfo*)*get_list2(lines, i);
}
// columns / rows はポインタリスト。各 LineInfo* は GC_alloc されたもの。
inline ColumnInfo* tableCol(NewTable* t, int j) { return tableLine(t->columns, j); }
inline RowInfo*    tableRow(NewTable* t, int i) { return tableLine(t->rows,    i); }

// 汎用 (List* / Map* 経由) — 行/列対称
inline int tableLineIndexToId(List* lines, int i) {
	if ((unsigned)i >= (unsigned)lines->size) return -1;
	LineInfo* l = (LineInfo*)*get_list2(lines, i);
	return l ? l->id : -1;
}
inline LineInfo* tableLineById(Map* idMap, int id) {
	return (LineInfo*)get_mapyn(idMap, (char*)(intptr_t)id);
}
// id → list 上の位置。LineInfo は index を持たないので List* を線形走査する。
//   見つからなければ -1。Map で l を引いてからアドレス比較。
inline int tableLineIdToIndex(List* lines, Map* idMap, int id) {
	LineInfo* target = tableLineById(idMap, id);
	if (!target) return -1;
	int n = lines->size;
	for (int i = 0; i < n; i++) {
		if ((LineInfo*)*get_list2(lines, i) == target) return i;
	}
	return -1;
}

// LineInfo の **親兄弟リスト上での位置** を返す (= parent->childs か t->rows/columns)。
//   tableLineIdToIndex は idMap から target を引いた後 t->rows / t->columns を線形走査
//   するため、tree-構造で深い位置にあると O(全行数) になる。
//   こちらは sibling list だけ走査するので O(兄弟数)。
// 見つからなければ -1。
//
// hintIdx: 前回呼び出しの結果 + 1 等の「ここから探すと当たりそう」な位置。
//   < 0 か範囲外なら無視して 0 から走査。
//   walkVisibleLines は NULL や visible=false の要素をスキップする可能性があるため、
//   「前回 +1」をそのまま答えとして使うのは危険。検証 (= 該当位置の値が line か?) を
//   noop か、外れていれば線形フォールバック。
inline int lineIndexInParentHint(LineInfo* line, List* siblings, int hintIdx) {
	if (!line || !siblings) return -1;
	int n = siblings->size;
	// (1) ヒント位置を直接確認 (= 連続兄弟なら O(1))
	if (hintIdx >= 0 && hintIdx < n) {
		if ((LineInfo*)*get_list2(siblings, hintIdx) == line) return hintIdx;
	}
	// (2) ヒント位置以降を線形走査 (= NULL/visible=false で数個スキップした場合)
	for (int i = (hintIdx >= 0 ? hintIdx + 1 : 0); i < n; i++) {
		if ((LineInfo*)*get_list2(siblings, i) == line) return i;
	}
	// (3) ヒント位置より前 (= 兄弟順が逆転するレアケース)
	int upTo = (hintIdx >= 0 && hintIdx < n) ? hintIdx : 0;
	for (int i = 0; i < upTo; i++) {
		if ((LineInfo*)*get_list2(siblings, i) == line) return i;
	}
	return -1;
}

// 旧 API (= ヒント無し版)
inline int lineIndexInParent(LineInfo* line, List* rootList) {
	if (!line) return -1;
	List* siblings = line->parent ? line->parent->childs : rootList;
	return lineIndexInParentHint(line, siblings, -1);
}
inline int rowIndexInParent(NewTable* t, LineInfo* row) {
	return lineIndexInParent(row, t->rows);
}
inline int colIndexInParent(NewTable* t, LineInfo* col) {
	return lineIndexInParent(col, t->columns);
}
// ヒント版 wrapper
inline int rowIndexInParentHint(NewTable* t, LineInfo* row, int hintIdx) {
	List* siblings = row->parent ? row->parent->childs : t->rows;
	return lineIndexInParentHint(row, siblings, hintIdx);
}
inline int colIndexInParentHint(NewTable* t, LineInfo* col, int hintIdx) {
	List* siblings = col->parent ? col->parent->childs : t->columns;
	return lineIndexInParentHint(col, siblings, hintIdx);
}

// 行/列向け 1 行 wrapper
inline int       rowIndexToId(NewTable* t, int i)        { return tableLineIndexToId(t->rows,    i); }
inline int       colIndexToId(NewTable* t, int j)        { return tableLineIndexToId(t->columns, j); }
inline RowInfo*  tableRowById(NewTable* t, int rowId)    { return tableLineById(t->rowIdMap, rowId); }
inline ColumnInfo* tableColById(NewTable* t, int colId)  { return tableLineById(t->colIdMap, colId); }
inline int       rowIdToIndex(NewTable* t, int rowId)    { return tableLineIdToIndex(t->rows,    t->rowIdMap, rowId); }
inline int       colIdToIndex(NewTable* t, int colId)    { return tableLineIdToIndex(t->columns, t->colIdMap, colId); }

// セル取得 (汎用)。rowList と colList を渡すので、expand 用の可視リスト等にも対応。
inline NewElement* getTableCellFromLists(NewTable* t, List* rowList, List* colList, int i, int j) {
	int rid = tableLineIndexToId(rowList, i);
	int cid = tableLineIndexToId(colList, j);
	if (rid < 0 || cid < 0) return NULL;
	return (NewElement*)get_mapyn(t->cells, tableKey(rid, cid));
}
inline NewElement* getTableCell(NewTable* t, int i, int j) {
	return getTableCellFromLists(t, t->rows, t->columns, i, j);
}

// セル設定 (汎用)。auto-extend 無し、対象 list の i,j が範囲外なら no-op。
inline void setTableCellFromLists(ThreadGC* thgc, NewTable* t,
                                  List* rowList, List* colList,
                                  int i, int j, NewElement* elem) {
	int rid = tableLineIndexToId(rowList, i);
	int cid = tableLineIndexToId(colList, j);
	if (rid < 0 || cid < 0) return;
	ensureSIValues(thgc, elem);
	elem->values->values[TABLE_POS_SLOT].bits =
		(uint64_t)SIType::UIntInt
		| ((((uint64_t)(uint32_t)rid << 32) | (uint32_t)cid) & SIV_VAL_MASK);
	elem->parent = (NewElement*)t;
	add_mapyn(thgc, t->cells, tableKey(rid, cid), (char*)elem);
	// offscreen 持つ table のみ dirty-cells に登録 (= 次回 measure で差分処理)。
	// offscreen 持たない table は小規模前提で全件 measure するので登録不要。
	if (t->offscreen) add_list(thgc, t->dirtyCells, (char*)elem);
}

// cell 内部更新時 (= text 編集等) は `markLayoutOf(cell, local)` を呼ぶ。
//   内部で `FindOffscreen(cell)->markLayout(local, cell)` 相当を NULL 安全に実行し、
//   Offscreen::markLayout が「親が offscreen 持つ Table なら dirtyCells に登録」を自動 dispatch する。
// addTableLine は後方で定義 (= forward 宣言)。
inline int addTableLine(ThreadGC* thgc, List* lines, Map* idMap, int* nextId,
                        CType type, float size, LineInfo* parent);

// セル設定 (auto-extend 付き)。rowList / colList の任意 List* に対して i, j 位置に cell を置く。
//   t->rows / t->columns を渡せば標準動作、parent->childs を渡せば子配下に挿入。
//   size 不足なら addTableLine で末尾追加。
inline void setTableCell(ThreadGC* thgc, NewTable* t,
                         List* rowList, List* colList,
                         int i, int j, NewElement* elem) {
	if (i < 0 || j < 0 || !rowList || !colList) return;
	while (rowList->size <= i) {
		addTableLine(thgc, rowList, t->rowIdMap, &t->nextRowId,
		             CType::_RowInfo, t->defaultRowHeight, NULL);
	}
	while (colList->size <= j) {
		addTableLine(thgc, colList, t->colIdMap, &t->nextColId,
		             CType::_ColumnInfo, t->defaultColWidth, NULL);
	}
	setTableCellFromLists(thgc, t, rowList, colList, i, j, elem);
}

// 標準版 (= t->rows / t->columns 固定の wrapper)。i, j は List* 上の位置。
inline void setTableCell(ThreadGC* thgc, NewTable* t, int i, int j, NewElement* elem) {
	setTableCell(thgc, t, t->rows, t->columns, i, j, elem);
}

// セル削除 (汎用)。Map から消すだけ、elem の SIValues / parent は放置
inline void removeTableCellFromLists(ThreadGC* thgc, NewTable* t, List* rowList, List* colList, int i, int j) {
	int rid = tableLineIndexToId(rowList, i);
	int cid = tableLineIndexToId(colList, j);
	if (rid < 0 || cid < 0) return;
	remove_mapyn(thgc, t->cells, tableKey(rid, cid));
}
inline void removeTableCell(ThreadGC* thgc, NewTable* t, int i, int j) {
	removeTableCellFromLists(thgc, t, t->rows, t->columns, i, j);
}
// id 直指定版
inline NewElement* getTableCellById(NewTable* t, int rowId, int colId) {
	return (NewElement*)get_mapyn(t->cells, tableKey(rowId, colId));
}
inline void setTableCellById(ThreadGC* thgc, NewTable* t, int rowId, int colId, NewElement* elem) {
	ensureSIValues(thgc, elem);
	elem->values->values[TABLE_POS_SLOT].bits =
		(uint64_t)SIType::UIntInt
		| ((((uint64_t)(uint32_t)rowId << 32) | (uint32_t)colId) & SIV_VAL_MASK);
	elem->parent = (NewElement*)t;
	add_mapyn(thgc, t->cells, tableKey(rowId, colId), (char*)elem);
}

// cell から自分の (rowId, colId) を取り出す。
//   呼び出し側が「この cell は Table 登録済み」と分かっている前提。
//   未登録の cell に対して呼ぶと未定義 (= 開発側エラー、TABLE_POS_SLOT は None のまま)。
inline void cellTablePos(NewElement* cell, int* rowId, int* colId) {
	SIVal v = getSIValue(cell->values, TABLE_POS_SLOT, SIType::UIntInt);
	*rowId = (int)v.hi;
	*colId = (int)v.lo;
}

// === DFS walker (テンプレート化、std::function オーバーヘッド無し) ===
// expanded == true な subtree のみ recurse、collapsed なら subtree 全部スキップ。
// visit(LineInfo* l, int i) が呼ばれるのは visible (= 全祖先 expanded) な line に限られる。
//   i は「現在 walk 中の List* (= lines もしくは parent->childs) における for-loop 上の位置」。
//   LineInfo に index を持たない代わりに、走査中の caller がこの i を使う。
// 全 visible 走査 (早期終了なし)。List* を for で iterate しつつ、l->childs (expanded なら)
// に自身を再帰呼出し。invariant: lines は root list (= 子孫は parent->childs のみ)。
template<typename Visit>
inline void walkVisibleLines(List* lines, Visit visit) {
	int n = lines->size;
	for (int i = 0; i < n; i++) {
		LineInfo* l = (LineInfo*)*get_list2(lines, i);
		if (!l) continue;
		visit(l, i);
		if (l->expanded && l->childs) {
			walkVisibleLines(l->childs, visit);
		}
	}
}

// 早期終了対応版。visit は bool を返す: true=継続 / false=即座に走査打ち切り。
//   List* を for で iterate しつつ、各 l->childs (expanded なら) に自身を再帰呼出し。
//   戻り値 bool で終了を伝搬。top-level caller は戻り値無視で OK。
template<typename Visit>
inline bool walkVisibleLinesUntil(List* lines, Visit visit) {
	int n = lines->size;
	for (int i = 0; i < n; i++) {
		LineInfo* l = (LineInfo*)*get_list2(lines, i);
		if (!l) continue;
		if (!visit(l, i)) return false;
		if (l->expanded && l->childs) {
			if (!walkVisibleLinesUntil(l->childs, visit)) return false;
		}
	}
	return true;
}

// rect 範囲の summary。
//   minLine == NULL なら範囲が空。
//   maxIdx は maxLine の List* 上の for-loop 位置 (header 用)。
//   startPx は最初にヒットした line の絶対 pixel 位置 (= 描画起点 / caller の startX,startY)。
//   totalSize は範囲内 visible line の size 合計 (= 描画スパン / caller の totalW,totalH)。
//   visible line リスト本体は NewTable::visibleRowsScratch / visibleColsScratch (= scratch List)。
struct TableLineRange {
	LineInfo* minLine   = nullptr;
	LineInfo* maxLine   = nullptr;
	int       maxIdx    = -1;
	float     startPx   = 0.0f;
	float     totalSize = 0.0f;
};
struct TableRect2D {
	TableLineRange cols;   // X 軸範囲 (= t->columns 上)
	TableLineRange rows;   // Y 軸範囲 (= t->rows 上)
};

// 全 visible cell を DFS visible 順 (= row DFS × col DFS) で iterate。
//   callback: void(int rowId, int colId, NewElement* cell)
//   tree 折り畳みは walkVisibleLines が自動で skip するので、collapsed subtree の cell は
//   そもそも訪問されない (= 子孫ごとに visible 判定する必要なし)。
//   row/col のどちらにも紐付かない (= 削除済 cell が cells map に残っている等) は cell 取得失敗で skip。
template<typename F>
inline void forEachTableCell(NewTable* t, F&& callback) {
	walkVisibleLines(t->rows, [&](LineInfo* row, int /*ri*/) {
		walkVisibleLines(t->columns, [&](LineInfo* col, int /*ci*/) {
			NewElement* cell = getTableCellById(t, row->id, col->id);
			if (cell) callback(row->id, col->id, cell);
		});
	});
}

// 内部: 1 walk で rect 交差判定 + summary 蓄積 + scratch List<LineInfo*> 収集。
// 早期終了 (rEnd 超過) で全リスト走査回避。jdc は walkVisibleLinesUntil の上に書ける。
inline void fillTableLineRange(ThreadGC* thgc, List* lines, float rPos, float rSize,
                               TableLineRange& r, List* scratch) {
	scratch->size = 0;     // 配列の中身は GC が管理、capa は維持 (= realloc 起きない)
	float pos = 0;
	float rEnd = rPos + rSize;
	walkVisibleLinesUntil(lines, [&](LineInfo* l, int i) -> bool {
		if (pos >= rEnd) return false;       // 範囲を抜けた → 走査打ち切り
		float bottom = pos + l->size;
		if (bottom > rPos) {                 // rect と交差
			if (!r.minLine) { r.minLine = l; r.startPx = pos; }
			r.maxLine    = l;
			r.maxIdx     = i;
			r.totalSize += l->size;
			add_list(thgc, scratch, (char*)l);
		}
		pos += l->size;
		return true;
	});
}

// scratch 行/列 List<LineInfo*> は caller (= Paint 時に create_list したもの) が渡す。
// Paint 内で複数回 writeCellsInRect が走るとき、scratch buffer を共有して再利用する。
inline TableRect2D tableRangeFromRect(ThreadGC* thgc, NewTable* t,
                                      float rx, float ry, float rw, float rh,
                                      List* rowsScratch, List* colsScratch) {
	TableRect2D out{};
	fillTableLineRange(thgc, t->columns, rx, rw, out.cols, colsScratch);
	fillTableLineRange(thgc, t->rows,    ry, rh, out.rows, rowsScratch);
	return out;
}

// 列/行の追加 (汎用)。返り値は安定 ID。
//   LineInfo を GC_alloc し、List (ポインタ格納) と Map (id→info) の双方に同じアドレスを入れる。
//   parent != NULL なら child の parent / depth を設定 (= tree 子要素として追加)。
//   CType は List / Map と整合させるため caller が指定 (= 行用は _RowInfo、列用は _ColumnInfo)。
inline int addTableLine(ThreadGC* thgc, List* lines, Map* idMap, int* nextId,
                        CType type, float size, LineInfo* parent = NULL) {
	LineInfo* l = (LineInfo*)GC_alloc(thgc, type);
	l->id        = ++(*nextId);
	l->size      = size;
	l->mode      = TableLineMode::Manual;
	l->expanded  = true;
	l->showLines = false;
	l->visible   = true;
	l->depth     = parent ? (parent->depth + 1) : 0;
	l->parent    = parent;
	l->childs    = NULL;
	l->onClick   = NULL;
	add_list(thgc, lines, (char*)l);
	add_mapyn(thgc, idMap, (char*)(intptr_t)l->id, (char*)l);
	int id = l->id;
	return id;
}
inline int addTableColumn(ThreadGC* thgc, NewTable* t, float width) {
	return addTableLine(thgc, t->columns, t->colIdMap, &t->nextColId,
	                    CType::_ColumnInfo, width);
}
inline int addTableRow(ThreadGC* thgc, NewTable* t, float height) {
	return addTableLine(thgc, t->rows, t->rowIdMap, &t->nextRowId,
	                    CType::_RowInfo, height);
}

// 任意位置への Row 挿入。返り値は新規 row の rowId。
//   index は新しい行が挿入される位置 (= 既存行はそれ以降が下にシフト)。
//   index >= rows->size なら末尾追加 (= addTableRow 同等)。
// 任意位置への Line 挿入 (汎用)。返り値は新規 Line の id。
//   index >= lines->size なら末尾追加 (= addTableLine 同等)。
inline int insertTableLineAt(ThreadGC* thgc, List* lines, Map* idMap, int* nextId,
                             CType type, int index, float size) {
	if (index < 0) index = 0;
	if (index >= lines->size) return addTableLine(thgc, lines, idMap, nextId, type, size);
	LineInfo* l = (LineInfo*)GC_alloc(thgc, type);
	l->id        = ++(*nextId);
	l->size      = size;
	l->mode      = TableLineMode::Manual;
	l->expanded  = true;
	l->showLines = false;
	l->visible   = true;
	l->depth     = 0;
	l->parent    = NULL;
	l->childs    = NULL;
	l->onClick   = NULL;
	// list を 1 拡張して、index 以降を 1 個ずつ右にずらす
	add_list(thgc, lines, (char*)l);
	int n = lines->size;
	for (int k = n - 1; k > index; k--) {
		char* prev = *get_list2(lines, k - 1);
		set_list(lines, k, prev);
	}
	set_list(lines, index, (char*)l);
	// 旧 index 同期ループは削除 (LineInfo は index を持たない)。
	add_mapyn(thgc, idMap, (char*)(intptr_t)l->id, (char*)l);
	int id = l->id;
	return id;
}
inline int insertTableRowAt(ThreadGC* thgc, NewTable* t, int index, float height) {
	return insertTableLineAt(thgc, t->rows, t->rowIdMap, &t->nextRowId,
	                         CType::_RowInfo, index, height);
}
inline int insertTableColumnAt(ThreadGC* thgc, NewTable* t, int index, float width) {
	return insertTableLineAt(thgc, t->columns, t->colIdMap, &t->nextColId,
	                         CType::_ColumnInfo, index, width);
}

// 汎用: list と idMap から index 番目を消す。
// cell map の操作は呼び出し側で行う (= 軸ごとに key 順が違うため)。
//   旧仕様の index 同期ループは廃止 (LineInfo は index を持たない)。
inline void removeTableLineFromListAndMap(ThreadGC* thgc, List* lines, Map* idMap, int index) {
	if ((unsigned)index >= (unsigned)lines->size) return;
	LineInfo* l = (LineInfo*)*get_list2(lines, index);
	remove_mapyn(thgc, idMap, (char*)(intptr_t)l->id);
	remove_list(thgc, lines, index);
}

// 任意位置の Row 削除。該当行の全 cell も cells map から消す。
inline void removeTableRowAt(ThreadGC* thgc, NewTable* t, int index) {
	if ((unsigned)index >= (unsigned)t->rows->size) return;
	RowInfo* r = (RowInfo*)*get_list2(t->rows, index);
	int rowId = r->id;
	int cn = t->columns->size;
	for (int j = 0; j < cn; j++) {
		ColumnInfo* col = tableCol(t, j);
		if (col) remove_mapyn(thgc, t->cells, tableKey(rowId, col->id));
	}
	removeTableLineFromListAndMap(thgc, t->rows, t->rowIdMap, index);
}
inline void removeTableColumnAt(ThreadGC* thgc, NewTable* t, int index) {
	if ((unsigned)index >= (unsigned)t->columns->size) return;
	ColumnInfo* c = (ColumnInfo*)*get_list2(t->columns, index);
	int colId = c->id;
	int rn = t->rows->size;
	for (int i = 0; i < rn; i++) {
		RowInfo* row = tableRow(t, i);
		if (row) remove_mapyn(thgc, t->cells, tableKey(row->id, colId));
	}
	removeTableLineFromListAndMap(thgc, t->columns, t->colIdMap, index);
}

// ============================================================
// Tree (折り畳み) サポート — 行/列対称
// ============================================================
// LineInfo (= RowInfo / ColumnInfo) に親子関係を持たせて expand/collapse できる。
//
//   `setLineParent(thgc, child, parent)`  — 親を設定し parent.childs に追加。
//   `rebuildLineVisibility(lines)`        — DFS で全祖先 expanded なら visible=true。
//   `expandLine(t, local, lines, line, bool)` — expanded 切替 → 再計算 → markPaint。
//
// 行用は lines = t->rows、列用は lines = t->columns を渡す。
//
// 走査側 (tableAbsX/Y, tableRangeFromRect, forEachTableCell) は visible=false の行/列を
// 0 サイズとして扱うので、不可視部分は描画されない。
// ============================================================

// 親子関係の設定。child の親を変更し、親の childs に追加。depth は親 +1 で再計算。
//   行/列どちらにも使える (LineInfo* で受ける)。
inline void setLineParent(ThreadGC* thgc, LineInfo* child, LineInfo* parent) {
	if (!child) return;
	// 旧親の childs から外す
	if (child->parent && child->parent->childs) {
		remove_list_val(thgc, child->parent->childs, (char*)child);
	}
	child->parent = parent;
	if (parent) {
		if (!parent->childs) {
			parent->childs = create_list(thgc, (int)sizeof(char*), CType::_RowInfo);
		}
		add_list(thgc, parent->childs, (char*)child);
		child->depth = parent->depth + 1;
	} else {
		child->depth = 0;
	}
}

// visibility 再計算: ルート (parent==NULL) から DFS で「全祖先が expanded」なら visible=true。
//   line.visible = (parent==null) ? true : (parent.visible && parent.expanded)
//   行/列どちらにも使える (List* lines として t->rows / t->columns を渡す)。
inline void rebuildLineVisibility(List* lines) {
	int n = lines->size;
	std::function<void(LineInfo*, bool)> walk = [&](LineInfo* l, bool ancestorExpanded) {
		l->visible = ancestorExpanded;
		bool childVisible = ancestorExpanded && l->expanded;
		if (l->childs) {
			for (int k = 0; k < l->childs->size; k++) {
				LineInfo* c = (LineInfo*)*get_list2(l->childs, k);
				if (c) walk(c, childVisible);
			}
		}
	};
	for (int i = 0; i < n; i++) {
		LineInfo* l = (LineInfo*)*get_list2(lines, i);
		if (l && l->parent == NULL) walk(l, true);
	}
}

// 折り畳み切替: line の expanded を b に → visibility 再計算 → markLayout。
//   dirtyCells に何も追加しないので NewTableMeasure 内で cell->Measure はスキップされ、
//   resolveTableLineSizes + relayoutTablePositions のみ走る (= 軽量パス)。
//   markLayout 経由なので親要素への size 伝搬も標準どおり機能する。
inline void expandLine(NewTable* t, NewLocal* local, List* lines, LineInfo* line, bool b) {
	if (!line || line->expanded == b) return;
	line->expanded = b;
	rebuildLineVisibility(lines);
	markLayoutOf(t, local);
}

// newelem.h で forward 宣言した行操作の実装。
//   refLine / lineToRemove の親が _TableElem (= NewTable) でなければ何もしない。
//   parent が Table で、refLine が cells map に登録済みのとき:
//     tableInsertRowForLine: refLine の行位置 (= rowIdToIndex(rowId)) に新 Row を挿入し、
//       newLine を refLine と同じ列に setTableCellById で配置。
//     tableRemoveRowForLine: lineToRemove の行を removeTableRowAt で削除 (= 該当行の cells も消える)。
inline void tableInsertRowForLine(ThreadGC* thgc, NewElement* refLine, NewElement* newLine) {
	if (!refLine || !refLine->parent || !newLine) return;
	if (refLine->parent->type != LetterType::_TableElem) return;
	NewTable* t = (NewTable*)refLine->parent;
	int rowId, colId;
	cellTablePos(refLine, &rowId, &colId);
	int rowIdx = rowIdToIndex(t, rowId);
	if (rowIdx < 0) return;
	int newRowId = insertTableRowAt(thgc, t, rowIdx, t->defaultRowHeight);
	setTableCellById(thgc, t, newRowId, colId, newLine);
}
inline void tableRemoveRowForLine(ThreadGC* thgc, NewElement* lineToRemove) {
	if (!lineToRemove || !lineToRemove->parent) return;
	if (lineToRemove->parent->type != LetterType::_TableElem) return;
	NewTable* t = (NewTable*)lineToRemove->parent;
	int rowId, colId;
	cellTablePos(lineToRemove, &rowId, &colId);
	(void)colId;
	int rowIdx = rowIdToIndex(t, rowId);
	if (rowIdx < 0) return;
	removeTableRowAt(thgc, t, rowIdx);
}

// AutoFit: 列 j の全 cell を unconstrained で measure → max width を col->width に設定。
//   列幅変更後は offscreen->layout = true で全体 reflow を促す。
inline void autoFitTableColumn(ThreadGC* thgc, NewTable* t, int j, NewLocal* local) {
	ColumnInfo* ci = tableCol(t, j);
	if (!ci) return;
	int cid = ci->id;
	float w = 0;
	int rn = t->rows->size;
	for (int i = 0; i < rn; i++) {
		int rid = rowIndexToId(t, i);
		if (rid < 0) continue;
		NewElement* cell = (NewElement*)get_mapyn(t->cells, tableKey(rid, cid));
		if (!cell) continue;
		if (cell->Measure) {
			NewMeasure cm = {}; cm.base = cell;
			int d = 0;
			cell->Measure(thgc, cell, &cm, local, &d);
		}
		if (cell->size.x > w) w = cell->size.x;
	}
	ci->width = w;
	if (t->offscreen) t->offscreen->layout = true;
}
inline void autoFitTableRow(ThreadGC* thgc, NewTable* t, int i, NewLocal* local) {
	RowInfo* ri = tableRow(t, i);
	if (!ri) return;
	int rid = ri->id;
	float h = 0;
	int cn = t->columns->size;
	for (int j = 0; j < cn; j++) {
		int cid = colIndexToId(t, j);
		if (cid < 0) continue;
		NewElement* cell = (NewElement*)get_mapyn(t->cells, tableKey(rid, cid));
		if (!cell) continue;
		if (cell->Measure) {
			NewMeasure cm = {}; cm.base = cell;
			int d = 0;
			cell->Measure(thgc, cell, &cm, local, &d);
		}
		if (cell->size.y > h) h = cell->size.y;
	}
	ri->height = h;
	if (t->offscreen) t->offscreen->layout = true;
}
inline void autoFitTableAll(ThreadGC* thgc, NewTable* t, NewLocal* local) {
	int cn = t->columns->size, rn = t->rows->size;
	for (int j = 0; j < cn; j++) autoFitTableColumn(thgc, t, j, local);
	for (int i = 0; i < rn; i++) autoFitTableRow(thgc, t, i, local);
}

// freeze 設定 (上 rowCount 行 + 左 colCount 列を固定)
inline void setTableFreeze(NewTable* t, int rowCount, int colCount) {
	t->freezeRowCount = rowCount;
	t->freezeColCount = colCount;
}

// target line までの DFS visible 累計サイズ (= target の左端 / 上端の絶対座標)。
//   target == NULL なら visible 全合計を返す (= total size)。
//   tree-aware: 折り畳まれた subtree は walkVisibleLines が skip するので size に含まれない。
inline float tableAbs(List* lines, LineInfo* target) {
	float pos = 0;
	walkVisibleLinesUntil(lines, [&](LineInfo* l, int /*i*/) -> bool {
		if (target && l == target) return false;   // 見つかったら即終了
		pos += l->size;
		return true;
	});
	return pos;
}
inline float tableAbsX(NewTable* t, LineInfo* col) { return tableAbs(t->columns, col); }
inline float tableAbsY(NewTable* t, LineInfo* row) { return tableAbs(t->rows,    row); }

inline float tableTotalWidth(NewTable* t)   { return tableAbs(t->columns, NULL); }
inline float tableTotalHeight(NewTable* t)  { return tableAbs(t->rows,    NULL); }
// freeze は root-level 行/列の最初の N 個を凍結する (= 子の expand 状況に関係なく root-level 単位)。
//   freezeCount-th root-level line までの visible 累積を返す。
inline float tableFreezeSize(List* lines, int freezeCount) {
	if (freezeCount <= 0) return 0;
	int n = lines->size;
	if (freezeCount > n) freezeCount = n;
	LineInfo* boundary = (LineInfo*)*get_list2(lines, freezeCount - 1);
	// boundary 自身も freeze に含めるので、boundary の次の line までの累積。
	// → tableAbs(lines, nextRootLine) で next root の位置を取る、なければ全 visible 合計。
	if (freezeCount < n) {
		LineInfo* next = (LineInfo*)*get_list2(lines, freezeCount);
		return tableAbs(lines, next);
	}
	(void)boundary;
	return tableAbs(lines, NULL);
}
inline float tableFreezeWidth(NewTable* t)  { return tableFreezeSize(t->columns, t->freezeColCount); }
inline float tableFreezeHeight(NewTable* t) { return tableFreezeSize(t->rows,    t->freezeRowCount); }

// freeze 判定: line が freezeCount より前の root subtree 内なら true (= 凍結中)。
inline bool isInFreezeRange(List* lines, LineInfo* line, int freezeCount) {
	if (!line || freezeCount <= 0) return false;
	LineInfo* root = line;
	while (root->parent) root = root->parent;
	int n = lines->size;
	if (freezeCount > n) freezeCount = n;
	for (int k = 0; k < freezeCount; k++) {
		if ((LineInfo*)*get_list2(lines, k) == root) return true;
	}
	return false;
}

// freeze 内: 絶対座標 / freeze 外: 絶対座標 - scroll  (行/列対称、tree-aware)
inline float tableScreen(List* lines, LineInfo* line, int freezeCount, float scroll) {
	float a = tableAbs(lines, line);
	return isInFreezeRange(lines, line, freezeCount) ? a : (a - scroll);
}
inline float tableScreenX(NewTable* t, LineInfo* col, float scrollX) {
	return tableScreen(t->columns, col, t->freezeColCount, scrollX);
}
inline float tableScreenY(NewTable* t, LineInfo* row, float scrollY) {
	return tableScreen(t->rows,    row, t->freezeRowCount, scrollY);
}

// Measure (markLayout 後に framework が呼ぶ): ElementMeasure 流。
//   - offscreen->layout が false なら早期 return
//   - offscreen 持ち: dirtyCells の cell のみ measure (差分)
//   - offscreen 無し: 全 cell measure (小規模前提)
//   - 列/行 Auto/Percent サイズ解決 → 全 cell の pos2 + table の size2 更新
RetMeasureType NewTableMeasure(ThreadGC* thgc, NewElement* elem, NewMeasure* measure, NewLocal* local, int* nptr) {
	NewTable* t = (NewTable*)elem;
	if (elem->offscreen) {
		if (!elem->offscreen->layout) return RetMeasure_None;
		elem->offscreen->layout = false;
		// offscreen 持ち: dirtyCells のみ measure (= cell-level 差分追跡)。
		// expand/collapse のみのトリガでは dirtyCells が空 → cell measure 全スキップ。
		int dn = t->dirtyCells->size;
		for (int k = 0; k < dn; k++) {
			NewElement* cell = (NewElement*)*get_list2(t->dirtyCells, k);
			if (cell && cell->Measure) {
				NewMeasure cm = {}; cm.base = cell;
				int d = 0;
				cell->Measure(thgc, cell, &cm, local, &d);
			}
		}
		t->dirtyCells->size = 0;   // 全件 measure 完了 → list クリア
	} else {
		// offscreen 無しの table は小規模前提 → 全 cell measure (= 旧挙動)
		forEachTableCell(t, [&](int rid, int cid, NewElement* cell) {
			(void)rid; (void)cid;
			if (cell->Measure) {
				NewMeasure cm = {};
				cm.base = cell;
				int d = 0;
				cell->Measure(thgc, cell, &cm, local, &d);
			}
		});
	}

	// === 列 / 行の最終サイズ解決 (DFS visible のみ) ===
	//   Auto    : visible row の cell から max(size) を width に
	//   Percent : pct=fmodf(size,1); mult=(pct==0)?1:pct; size=floor(base*mult)+pct
	auto resolvePercent = [](float currentSize, float baseSize) -> float {
		float pct  = fmodf(currentSize, 1.0f);
		float mult = (pct == 0.0f) ? 1.0f : pct;
		return floorf(baseSize * mult) + pct;
	};
	walkVisibleLines(t->columns, [&](LineInfo* col, int /*ci*/) {
		if (tlmIsAuto(col->mode)) {
			float maxW = 0;
			walkVisibleLines(t->rows, [&](LineInfo* row, int /*ri*/) {
				NewElement* cell = getTableCellById(t, row->id, col->id);
				if (cell && cell->size.x > maxW) maxW = cell->size.x;
			});
			col->width = maxW;
		} else if (tlmIsPercent(col->mode)) {
			col->width = resolvePercent(col->width, elem->size.x);
		}
	});
	walkVisibleLines(t->rows, [&](LineInfo* row, int /*ri*/) {
		if (tlmIsAuto(row->mode)) {
			float maxH = 0;
			walkVisibleLines(t->columns, [&](LineInfo* col, int /*ci*/) {
				NewElement* cell = getTableCellById(t, row->id, col->id);
				if (cell && cell->size.y > maxH) maxH = cell->size.y;
			});
			row->height = maxH;
		} else if (tlmIsPercent(row->mode)) {
			row->height = resolvePercent(row->height, elem->size.y);
		}
	});

	// === 全 cell の pos2 + table 自身の size2 を更新 ===
	// walk 中に curX / curY を累積すれば 1 周で済む (= O(R × C))。
	// tableAbsX/Y を per-cell に呼ぶと内部で毎回 walk して O(R × C × (R + C)) になる。
	float curY = 0;
	float maxX = 0;
	walkVisibleLines(t->rows, [&](LineInfo* row, int /*ri*/) {
		float curX = 0;
		walkVisibleLines(t->columns, [&](LineInfo* col, int /*ci*/) {
			NewElement* cell = getTableCellById(t, row->id, col->id);
			if (cell) {
				cell->pos2.x = curX;
				cell->pos2.y = curY;
			}
			curX += col->size;
		});
		if (curX > maxX) maxX = curX;
		curY += row->size;
	});
	float totalW = maxX;
	float totalH = curY;
	if (elem->xtype == baseType(SizeType::Auto)) elem->size.x = totalW;
	if (elem->ytype == baseType(SizeType::Auto)) elem->size.y = totalH;
	elem->size2.x = totalW;
	elem->size2.y = totalH;
	measure->pos.x += elem->size.x;
	return RetMeasure_None;
}

// Draw:
//   Phase 1: 全セルを offscreen にテーブル絶対座標 (tableAbsX/Y) で 1 回だけ描画
//   Phase 2: 同じ offscreen から 4 ブリット (TL / Top / Left / Center)
//            UV を pixel→normalized に変換して切り出す。scroll は中央/Top の x、中央/Left の y にだけ加算。
// NewTableDraw: ElementDraw 構造を踏襲。
//   ElementDraw からの差分:
//     - childend 反復 → cell map 反復 (= visible 領域内の cell を blanket re-measure + draw)
//     - 単一 pushImage 表示 → 4-blit (TL / Top / Left / Center) で freeze pane を実現
//   それ以外 (tile state 退避/復帰、ping/pong、resize ヒステリシス、bg、scrollbar) は ElementDraw 通り。
void NewTableDraw(ThreadGC* thgc, NewElement* elem, NewGraphic* g, NewLocal* local, RenderCommandQueue* q) {
	NewTable* t = (NewTable*)elem;

	// === 1. 親のタイル状態を退避 ===
	TiledTextureInfo*        _saveTiledTarget         = g->layer->tiledTarget;
	ImageId                  _saveTiledTargetImageId  = g->layer->tiledTargetImageId;
	bgfx::FrameBufferHandle* _saveTiledPlaceholderFBO = g->layer->tiledPlaceholderFBO;
	uint64_t                 _saveTiledBaseViewId     = g->layer->tiledBaseViewId;

	// === 2. サイズ計算 ===
	float sbarx = 0, sbary = 0;
	if (baseType(elem->xtype) == SizeType::Scroll) sbary += 5;
	else if (baseType(elem->xtype) == SizeType::Page) sbary += 10;
	if (baseType(elem->ytype) == SizeType::Scroll) sbarx += 5;
	else if (baseType(elem->ytype) == SizeType::Page) sbarx += 10;

	NewGraphic g2{};
	float sizex = elem->size.x + elem->paddings[1] + elem->paddings[3];
	float sizey = elem->size.y + elem->paddings[0] + elem->paddings[2];
	float viewSizex = sizex, viewSizey = sizey;
	float tabExtL = 0, tabExtR = 0, tabExtT = 0, tabExtB = 0;

	// cell の scissor pointer 用 pool は LayerInfo::scissorPool (per-frame, 全 NewTableDraw
	// で共有) を使う。pointer は cmd 寿命 (= LayerInfo::clear() 呼出まで) 安定。
	// 旧実装の thread_local 版は「table A の Draw → table B の Draw」と続いた時に
	// B の clear() で A の pointer を壊して、A が描画されない事故を起こしていた。
	std::vector<ScissorRect>& cellScissors = g->layer->scissorPool;
	if (cellScissors.capacity() == 0) cellScissors.reserve(16384);

	// === scratch List<LineInfo*> (= Paint 内で使い捨て、複数 writeCellsInRect 間で共有) ===
	// capa 512 で確保 → 通常の表示範囲 (~300 行) なら add_list 中の realloc は 0 回。
	// 各 writeCellsInRect 呼出の冒頭で size=0 にリセット (= GC が後で本体を回収)。
	List* visibleRowsScratch = create_list(thgc, (int)sizeof(char*), CType::_RowInfo,    512);
	List* visibleColsScratch = create_list(thgc, (int)sizeof(char*), CType::_ColumnInfo, 512);

	// Table は常に offscreened
	if (elem->offscreen != NULL) {
		elem->offscreen->paint = Offscreen::PaintOffscreen;
		// === 2a. tabExt (cornerPattern からタブ拡張) ===
		if (elem->background) {
			float tabR = elem->background->backUVMinX;
			int tabPat = (int)elem->background->cornerPattern;
			float tabExt = tabR * 1.5f;
			if (tabR > 0.0f && tabPat >= 9 && tabPat <= 12) {
				if (tabPat == 9)       tabExtT = tabExt;
				else if (tabPat == 10) tabExtR = tabExt;
				else if (tabPat == 11) tabExtB = tabExt;
				else                   tabExtL = tabExt;
			}
		}
		float z = 0.0f;
		PointF absPos = getAbsolutePosition(elem, &z);

		// === 2b. content size (size2) を加味 ===
		if (elem->size2.x >= sizex) sizex = elem->size2.x;
		if (elem->size2.y >= sizey) sizey = elem->size2.y;
		sizex += tabExtL + tabExtR;
		sizey += tabExtT + tabExtB;

		// === 3. resize (ヒステリシス: grow=1.3x、shrink=1/3x) ===
		// 現サイズは実テクスチャ (Tiled なら totalSize、Standalone なら size) から読む。
		// queueOffscreenResize が同期更新するので「自前 fbsize キャッシュ」と等価で、
		// 自前キャッシュを持つと初回の Standalone↔Tiled 遷移などで実状態と乖離するのを避ける。
		PointI curFb = { 0, 0 };
		{
			TiledTextureInfo* curTti = mygetTiledTextureInfo(thgc, elem->offscreen->imPing);
			if (curTti) curFb = curTti->totalSize;
			else {
				StandaloneTextureInfo* curSti = mygetStandaloneTextureInfo(thgc, elem->offscreen->imPing);
				if (curSti) curFb = curSti->size;
			}
		}
		float size3x = curFb.x, size3y = curFb.y;
		bool resized = false;
		if (curFb.x < sizex) { size3x = sizex * 1.3f; resized = true; }
		else if (sizex < curFb.x / 3) { size3x = sizex * 1.3f; resized = true; }
		if (curFb.y < sizey) { size3y = sizey * 1.3f; resized = true; }
		else if (sizey < curFb.y / 3) { size3y = sizey * 1.3f; resized = true; }
		if (resized) {
			int pageW = (baseType(elem->xtype) == SizeType::Page) ? (int)viewSizex : 0;
			int pageH = (baseType(elem->ytype) == SizeType::Page) ? (int)viewSizey : 0;
			elem->offscreen->imPing = queueOffscreenResize(thgc, elem->offscreen->imPing, size3x, size3y, pageW, pageH);
			elem->offscreen->imPong = queueOffscreenResize(thgc, elem->offscreen->imPong, size3x, size3y, pageW, pageH);
		}

		// Table は ping/pong しない (= 同フレーム内で write → display を完結させる)。
		// imPing を単一バッファとして使う。
		ImageId readId  = elem->offscreen->imPing;
		ImageId writeId = readId;

		// === 4. write 用 g2 を先に構築 (cell 描画は display より前) ===
		TiledTextureInfo* tiledWrite = mygetTiledTextureInfo(thgc, writeId);
		if (tiledWrite && !tiledWrite->tiles.empty()) {
			uint64_t tileBaseId = ::viewId;
			::viewId += (uint64_t)tiledWrite->tiles.size();
			g->layer->tiledTarget         = tiledWrite;
			g->layer->tiledTargetImageId  = writeId;
			g->layer->tiledPlaceholderFBO = &g->layer->_tiledSentinel;
			g->layer->tiledBaseViewId     = tileBaseId;
			g2 = NewGraphic{ g->layer, elem, elem, {0, 0}, {elem->size2.x, elem->size2.y}, {0,0}, {0,0},
				writeId, g->layer->tiledPlaceholderFBO, &tiledWrite->totalSize,
				elem->offscreen->viewId = tileBaseId, 0, elem->offscreen->group };
		}
		else {
			auto writeInfo = mygetStandaloneTextureInfo(thgc, writeId);
			uint64_t offscreenViewId = ::viewId++;
			g2 = NewGraphic{ g->layer, elem, elem, {tabExtL, tabExtT},
				{elem->size2.x + tabExtL + tabExtR, elem->size2.y + tabExtT + tabExtB},
				{0,0}, {0,0},
				writeId, &writeInfo->fbo, &writeInfo->size,
				elem->offscreen->viewId = offscreenViewId, 0, elem->offscreen->group };
		}
		g2.winFb = g->winFb; g2.winFbsize = g->winFbsize; g2.winViewId = g->winViewId;
		g2.zIndex = z;
		g2.start = g->pos;

		// 背景情報を事前解決 (writeCellsInRect で push 用に使う)。
		//   ここでは push しない (= heldGen が current の tile / sti に対しては
		//   そもそも writeCellsInRect が呼ばれないので無駄が出ない)。
		auto resolveBgTex = [&](ImageId id) -> bgfx::TextureHandle* {
			if (!isValidImageId(id)) return &nulltex;
			auto info = mygetStandaloneTextureInfo(thgc, id);
			return info ? &info->handle : &nulltex;
		};
		bgfx::TextureHandle* bgTex1 = elem->background ? resolveBgTex(elem->background->tex1) : &nulltex;
		bgfx::TextureHandle* bgTex2 = elem->background ? resolveBgTex(elem->background->tex2) : &nulltex;
		bgfx::TextureHandle* bgTex3 = elem->background ? resolveBgTex(elem->background->tex3) : &nulltex;
		bgfx::TextureHandle* bgTex4 = elem->background ? resolveBgTex(elem->background->tex4) : &nulltex;
		float bgW = std::max(elem->size2.x, elem->size.x + elem->paddings[1] + elem->paddings[3]) + tabExtL + tabExtR;
		float bgH = std::max(elem->size2.y, elem->size.y + elem->paddings[0] + elem->paddings[2]) + tabExtT + tabExtB;
		float bgX = g2.pos.x - tabExtL;
		float bgY = g2.pos.y - tabExtT;
		float bgZ = std::floor(g2.zIndex);
		g2.pos.x += elem->boWidthes[3] + elem->paddings[3];
		g2.pos.y += elem->boWidthes[0] + elem->paddings[0];

		// === 6. 4 領域 (TL/Top/Left/Center) ごとに「measure+write → display」 ===
		float fzW   = tableFreezeWidth(t);
		float fzH   = tableFreezeHeight(t);
		float visW  = viewSizex - fzW; if (visW < 0) visW = 0;
		float visH  = viewSizey - fzH; if (visH < 0) visH = 0;
		float scrX  = elem->scroll.x;
		float scrY  = elem->scroll.y;
		float dispBaseX = absPos.x + elem->pos.x + elem->margins[3];
		float dispBaseY = absPos.y + elem->pos.y + elem->margins[0];
		auto* bg = elem->background;
		float dispZ = 10000 + std::floor(z);
		float baseInOff_x = g2.pos.x;
		float baseInOff_y = g2.pos.y;

		// (ax, ay, aw, ah) (= offscreen 上の範囲) に入る cell を draw + グリッド線。
		// 背景もここで push する (= cells と同じ heldGen ゲートで gate される)。
		auto writeCellsInRect = [&](int ax, int ay, int aw, int ah,
		                            bgfx::FrameBufferHandle* fb, PointI* fbsz) {
			// 背景 (= cell より先に push = 下のレイヤー)。
			//   tile mode は layer->tiledTargetPageIndex でこの tile FBO に絞られる、
			//   standalone は fb / fbsz そのまま。FBO 境界外は自然 clip。
			if (elem->background) {
				float noBo[4] = {0, 0, 0, 0};
				g2.layer->pushBackground(elem->background, bgX, bgY, bgW, bgH, bgZ,
					bgTex1, bgTex2, bgTex3, bgTex4, fb, fbsz, g2.viewId, 1.0f,
					noBo, &noBorderShadow);
			}
			// 範囲走査: tableRangeFromRect で summary 取得 + scratch List に visible 列挙。
			// per-line iter は scratch List を線形走査 (再帰なし、walk なし)。
			TableRect2D range = tableRangeFromRect(thgc, t, (float)ax, (float)ay, (float)aw, (float)ah,
			                                       visibleRowsScratch, visibleColsScratch);
			if (!range.rows.minLine || !range.cols.minLine) return;
			float startX = range.cols.startPx;
			float startY = range.rows.startPx;
			float totalW = range.cols.totalSize;
			float totalH = range.rows.totalSize;
			List* visRows = visibleRowsScratch;
			List* visCols = visibleColsScratch;
			int rN = visRows->size;
			int cN = visCols->size;

			// === Header 背景の bulk 描画 ===
			{
				Background headBg{};
				headBg.type      = DrawCommandType::Fill;
				headBg.fillcolor = t->headerBgColor;
				float zero[4] = {0,0,0,0};
				float headZ = g2.zIndex + 0.3f;
				// 列ヘッダ: showLines 列に縦長 totalH 1 枚
				float curX = startX;
				for (int j = 0; j < cN; j++) {
					LineInfo* col = (LineInfo*)*get_list2(visCols, j);
					if (col->showLines) {
						g2.layer->pushBackground(&headBg,
							baseInOff_x + curX, baseInOff_y + startY,
							col->width, totalH, headZ,
							&nulltex, &nulltex, &nulltex, &nulltex,
							fb, fbsz, g2.viewId, 0.0f, zero, &headBg.bs);
					}
					curX += col->width;
				}
				// 行ヘッダ: showLines 行に横長 totalW 1 枚
				float curY = startY;
				for (int i = 0; i < rN; i++) {
					LineInfo* row = (LineInfo*)*get_list2(visRows, i);
					if (row->showLines) {
						g2.layer->pushBackground(&headBg,
							baseInOff_x + startX, baseInOff_y + curY,
							totalW, row->height, headZ,
							&nulltex, &nulltex, &nulltex, &nulltex,
							fb, fbsz, g2.viewId, 0.0f, zero, &headBg.bs);
					}
					curY += row->height;
				}
			}

			// === ツリービュー: tree col (= 最初の非 header col) を探す ===
			int treeColIdx = -1;
			float treeColX = 0;
			if (t->treeView) {
				float curX = startX;
				for (int j = 0; j < cN; j++) {
					LineInfo* col = (LineInfo*)*get_list2(visCols, j);
					if (!col->showLines) { treeColIdx = j; treeColX = curX; break; }
					curX += col->width;
				}
			}

			// === Cell 描画 (nested loop + colspan は pending pattern) ===
			// 行番号取得を高速化するためのヒント:
			//   前回の (depth, idxInParent) を覚えておき、同じ親なら hintIdx = prev+1
			//   でヒント検索 (= 当たれば O(1)、外れても近場で線形)。NULL や
			//   visible=false で List 位置が飛ぶ可能性があるため、「+1」を確定値として
			//   は使わず、必ず lineIndexInParentHint で検証する。
			int prevRowDepth = -1;
			int prevRowIdxInParent = -1;
			LineInfo* prevRowParent = NULL;
			float ay2 = startY;
			for (int i = 0; i < rN; i++) {
				LineInfo* row = (LineInfo*)*get_list2(visRows, i);
				float h = row->height;
				bool isColumnLetterRow = row->showLines;
				// hint: 同じ親 + 同じ depth なら prev+1 から探す。違えば 0 から (= -1 で 0 起点)。
				int hintIdx = (row->parent == prevRowParent && row->depth == prevRowDepth)
					? prevRowIdxInParent + 1 : -1;
				int rowIdxInParent = rowIndexInParentHint(t, row, hintIdx);
				prevRowParent      = row->parent;
				prevRowDepth       = row->depth;
				prevRowIdxInParent = rowIdxInParent;
				// colspan pending state
				NewElement* pendingCell = nullptr;
				float pendingX = 0, pendingW = 0;
				auto flushPending = [&]() {
					if (!pendingCell) return;
					float cellX = baseInOff_x + pendingX;
					float cellY = baseInOff_y + ay2;
					// cell の table-local pos を pos2 に書き戻す (= expand/collapse 後の最新位置)。
					pendingCell->pos2.x = pendingX;
					pendingCell->pos2.y = ay2;
					bool needScissor = (pendingCell->size.x > pendingW) || (pendingCell->size.y > h)
						|| (pendingCell->size2.x > pendingW) || (pendingCell->size2.y > h);
					const ScissorRect* savedSci = g2.layer->currentScissor;
					if (needScissor && cellScissors.size() < cellScissors.capacity()) {
						cellScissors.push_back(ScissorRect{ cellX, cellY, pendingW, h });
						g2.layer->currentScissor = &cellScissors.back();
					}
					NewGraphic cg = g2;
					cg.pos.x  = cellX;
					cg.pos.y  = cellY;
					cg.fb     = fb;
					cg.fbsize = fbsz;
					cg.zIndex = g2.zIndex + 0.5f;
					pendingCell->Draw(thgc, pendingCell, &cg, local, q);
					if (needScissor) g2.layer->currentScissor = savedSci;
					pendingCell = nullptr;
				};
				int       prevColDepth       = -1;
				int       prevColIdxInParent = -1;
				LineInfo* prevColParent      = NULL;
				float ax2 = startX;
				for (int j = 0; j < cN; j++) {
					LineInfo* col = (LineInfo*)*get_list2(visCols, j);
					float w = col->width;
					float cellX = baseInOff_x + ax2;
					float cellY = baseInOff_y + ay2;
					bool isLineNumberCol = col->showLines;
					// 列レター用: 前回の (parent, depth) と一致なら prev+1 を hint に
					int hintColIdx = (col->parent == prevColParent && col->depth == prevColDepth)
						? prevColIdxInParent + 1 : -1;
					int colIdxInParent = colIndexInParentHint(t, col, hintColIdx);
					prevColParent      = col->parent;
					prevColDepth       = col->depth;
					prevColIdxInParent = colIdxInParent;
					if (isLineNumberCol || isColumnLetterRow) {
						flushPending();
						NewElement* hcell = getTableCellById(t, row->id, col->id);
						if (hcell) { hcell->pos2.x = ax2; hcell->pos2.y = ay2; }
						if (hcell && hcell->background) {
							Background overlayBg{};
							overlayBg.type           = DrawCommandType::Fill;
							overlayBg.fillcolor      = hcell->background->fillcolor;
							overlayBg.bs.borderColor = hcell->background->bs.borderColor;
							float zero[4] = {0,0,0,0};
							g2.layer->pushBackground(&overlayBg, cellX, cellY, w, h, g2.zIndex + 0.305f,
								&nulltex, &nulltex, &nulltex, &nulltex,
								fb, fbsz, g2.viewId, 0.0f, zero, &overlayBg.bs);
						}
						auto drawHeaderShrunk = [&](String* str) {
							const float padX = 4.0f, padY = 2.0f;
							float availW = w - padX * 2;
							if (availW <= 0 || str->size <= 0) return;
							float mw = 0, mh = 0;
							size_t mn = 0;
							MeasureString(*getAtlas(thgc), t->font, str, str->size, 10000.0f,
								&mw, &mh, &mn, g2.group);
							float scaleX = (mw > availW && mw > 0) ? (availW / mw) : 1.0f;
							drawStringScaledRange(g2.layer, *getAtlas(thgc), t->font,
								str, 0, str->size,
								cellX + padX, cellY + padY, scaleX,
								g2.zIndex + 0.31f,
								t->headerTextColor, g2.group, fb, fbsz, g2.viewId);
						};
						if (isLineNumberCol && !isColumnLetterRow) {
							// rowIdxInParent はループ冒頭で計算済 (= 前回ヒントで O(1) or O(兄弟数))
							drawHeaderShrunk(getCachedLineNumber(thgc, t,
								rowIdxInParent >= 0 ? rowIdxInParent : i));
						} else if (!isLineNumberCol && isColumnLetterRow) {
							drawHeaderShrunk(getCachedColumnLetter(thgc, t,
								colIdxInParent >= 0 ? colIdxInParent : j));
						}
					} else {
						NewElement* cell = getTableCellById(t, row->id, col->id);
						if (cell && cell->Draw) {
							flushPending();
							pendingCell = cell;
							// tree col は indent + icon 分だけ右にずらす (= cell content と icon が重ならない)。
							// children の有無に関係なく icon 領域を常に予約 (= 同 depth の row が
							// 子持ち / 子なしで content x が揃う)。
							float treeOff = 0;
							if (t->treeView && j == treeColIdx) {
								treeOff = row->depth * t->treeIndent + t->treeIconSize + 4;
							}
							pendingX    = ax2 + treeOff;
							pendingW    = w   - treeOff;
						} else if (pendingCell) {
							// NULL cell: 直前 cell の width を伸ばす (colspan 拡張)
							pendingW += w;
						}
					}
					ax2 += w;
				}
				flushPending();
				ay2 += h;
			}

			// === ツリービュー decoration: 接続線 + [+]/[-] アイコン ===
			if (t->treeView && treeColIdx >= 0) {
				Background line{};
				line.type      = DrawCommandType::Fill;
				line.fillcolor = t->treeLineColor;
				float zero[4] = {0,0,0,0};
				float decoZ   = g2.zIndex + 0.6f;   // cell content (0.5) より上
				float halfIc  = t->treeIconSize * 0.5f;
				float curY = startY;
				for (int i = 0; i < rN; i++) {
					LineInfo* row = (LineInfo*)*get_list2(visRows, i);
					float h = row->height;
					if (!row->showLines) {
						// 展開可能 = 既に子がロード済 OR 「展開状態だがデータ未取得 (= 遅延ロード待ち)」
						// 後者は driver から expand=1 で送られてきた行に対し drainer が
						// curRow->expanded = true にしておく (= MightHaveChildren=true 行が該当)。
						bool hasChildren = (row->childs && row->childs->size > 0) ||
						                   (row->expanded && !row->childs);
						float iconCx = treeColX + row->depth * t->treeIndent + halfIc + 2;
						float iconCy = curY + h * 0.5f;
						// 縦線: 親世代分の indent column に上から下へ (= 階層 connector)
						for (int d = 0; d < row->depth; d++) {
							float lx = treeColX + d * t->treeIndent + halfIc + 2;
							g2.layer->pushBackground(&line,
								baseInOff_x + lx - 0.5f, baseInOff_y + curY,
								1.0f, h, decoZ,
								&nulltex, &nulltex, &nulltex, &nulltex,
								fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
						}
						// 横線: 親 indent col → cell content 開始位置 (icon center まで)
						if (row->depth > 0) {
							float lx0 = treeColX + (row->depth - 1) * t->treeIndent + halfIc + 2;
							g2.layer->pushBackground(&line,
								baseInOff_x + lx0, baseInOff_y + iconCy - 0.5f,
								(iconCx - lx0), 1.0f, decoZ,
								&nulltex, &nulltex, &nulltex, &nulltex,
								fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
						}
						// [+]/[-] アイコン (= 子があるときのみ): 白塗り + 濃い枠 + 角丸 + 中央線
						if (hasChildren) {
							float ix = iconCx - halfIc;
							float iy = iconCy - halfIc;
							float sz = t->treeIconSize;
							Background icBg{};
							icBg.type             = DrawCommandType::Fill;
							icBg.fillcolor        = 0xFFFFFFFF;
							icBg.bs.borderColor   = 0x404040FF;          // 濃いグレー枠
							float r = sz * 0.25f;                         // icon サイズの 25% (= 比率で角丸)
							icBg.bs.boRadiuses[0] = r;
							icBg.bs.boRadiuses[1] = r;
							icBg.bs.boRadiuses[2] = r;
							icBg.bs.boRadiuses[3] = r;
							float bw[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
							g2.layer->pushBackground(&icBg,
								baseInOff_x + ix, baseInOff_y + iy, sz, sz,
								decoZ, &nulltex, &nulltex, &nulltex, &nulltex,
								fb, fbsz, g2.viewId, 0.0f, bw, &icBg.bs);
							// 横線 (= "−" / "+" 共通)
							g2.layer->pushBackground(&line,
								baseInOff_x + ix + 2, baseInOff_y + iconCy - 0.5f,
								sz - 4, 1.0f, decoZ + 0.01f,
								&nulltex, &nulltex, &nulltex, &nulltex,
								fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
							// 縦線 (= "+" のとき = data 非表示)。
							//   collapsed (expanded=false): "+"
							//   expanded だが childs 未ロード (= 遅延展開待ち): "+"
							//   expanded で childs あり: "−" (= 縦線描画しない)
							bool drawPlus = (!row->expanded) ||
							                (!row->childs || row->childs->size == 0);
							if (drawPlus) {
								g2.layer->pushBackground(&line,
									baseInOff_x + iconCx - 0.5f, baseInOff_y + iy + 2,
									1.0f, sz - 4, decoZ + 0.01f,
									&nulltex, &nulltex, &nulltex, &nulltex,
									fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
							}
						}
					}
					curY += h;
				}
			}

			// === グリッド線 ===
			if (t->showGridX || t->showGridY) {
				Background line{};
				line.type      = DrawCommandType::Fill;
				line.fillcolor = t->gridColor;
				float zero[4] = {0,0,0,0};
				float gridZ   = g2.zIndex + 0.4f;
				if (t->showGridX) {
					float curX = startX;
					for (int j = 0; j < cN; j++) {
						LineInfo* col = (LineInfo*)*get_list2(visCols, j);
						g2.layer->pushBackground(&line,
							baseInOff_x + curX - 0.5f, baseInOff_y + startY,
							1.0f, totalH, gridZ,
							&nulltex, &nulltex, &nulltex, &nulltex,
							fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
						curX += col->width;
					}
					g2.layer->pushBackground(&line,
						baseInOff_x + startX + totalW - 0.5f, baseInOff_y + startY,
						1.0f, totalH, gridZ,
						&nulltex, &nulltex, &nulltex, &nulltex,
						fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
				}
				if (t->showGridY) {
					float curY = startY;
					for (int i = 0; i < rN; i++) {
						LineInfo* row = (LineInfo*)*get_list2(visRows, i);
						g2.layer->pushBackground(&line,
							baseInOff_x + startX, baseInOff_y + curY - 0.5f,
							totalW, 1.0f, gridZ,
							&nulltex, &nulltex, &nulltex, &nulltex,
							fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
						curY += row->height;
					}
					g2.layer->pushBackground(&line,
						baseInOff_x + startX, baseInOff_y + startY + totalH - 0.5f,
						totalW, 1.0f, gridZ,
						&nulltex, &nulltex, &nulltex, &nulltex,
						fb, fbsz, g2.viewId, 0.0f, zero, &line.bs);
				}
			}
		};

		// 表示 (read): src 矩形 (sx, sy, w, h) を dest (dx, dy, w, h) に貼る。
		auto pushDisplay = [&](float sx, float sy, float dx, float dy, float w, float h) {
			if (w <= 0 || h <= 0) return;
			TiledTextureInfo*      tti = mygetTiledTextureInfo(thgc, readId);
			StandaloneTextureInfo* sti = mygetStandaloneTextureInfo(thgc, readId);
			if (tti) {
				int tsx = (int)tti->tileSize, tsy = (int)tti->tileSizeY;
				if (tsx <= 0 || tsy <= 0) return;
				int srcX = (int)sx, srcY = (int)sy, srcW = (int)std::ceil(w), srcH = (int)std::ceil(h);
				int col0 = std::clamp(srcX / tsx,                0, tti->tilesX - 1);
				int row0 = std::clamp(srcY / tsy,                0, tti->tilesY - 1);
				int col1 = std::clamp((srcX + srcW - 1) / tsx,   0, tti->tilesX - 1);
				int row1 = std::clamp((srcY + srcH - 1) / tsy,   0, tti->tilesY - 1);
				if (col1 > col0 + 1) col1 = col0 + 1;
				if (row1 > row0 + 1) row1 = row0 + 1;
				// ★ producer 時点で tile.handle がまだ placeholder (INVALID) でも、
				//   tiles ベクタのアドレスは createTiledOffscreenInternal の in-place 更新で
				//   安定しており、submit 時には valid な handle が deref される。
				//   ここで isValid チェックして &nulltex に倒すと cmd が永久に nulltex 固定に
				//   なるので、無条件に &tl.handle を渡す。
				bgfx::TextureHandle* texSlots[4] = { &nulltex, &nulltex, &nulltex, &nulltex };
				auto& tl = tti->tiles[row0 * tti->tilesX + col0];
				texSlots[0] = &tl.handle;
				if (col1 > col0) {
					auto& tr = tti->tiles[row0 * tti->tilesX + col1];
					texSlots[1] = &tr.handle;
				}
				if (row1 > row0) {
					auto& bl = tti->tiles[row1 * tti->tilesX + col0];
					texSlots[2] = &bl.handle;
				}
				if (col1 > col0 && row1 > row0) {
					auto& br = tti->tiles[row1 * tti->tilesX + col1];
					texSlots[3] = &br.handle;
				}
				int tw = tl.w > 0 ? tl.w : tsx;
				int th = tl.h > 0 ? tl.h : tsy;
				float uvX = (sx - col0 * tsx) / (float)tw;
				float uvY = (sy - row0 * tsy) / (float)th;
				float uvW = w / (float)tw;
				float uvH = h / (float)th;
				g->layer->pushImage(dx, dy, w, h,
					uvX, uvY, uvW, uvH,
					bg ? bg->bs.boRadiuses[0] : 0, bg ? bg->bs.boRadiuses[1] : 0,
					bg ? bg->bs.boRadiuses[2] : 0, bg ? bg->bs.boRadiuses[3] : 0,
					bg ? bg->aa : 0,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0, 0xFFFFFFFF,
					dispZ, texSlots[0], g->fb, g->fbsize, g->viewId,
					0.0f,
					bg ? bg->cornerPattern : 0.0f,
					bg ? bg->backUVMinX : 0.0f,
					texSlots[1], texSlots[2], texSlots[3]);
			}
			else if (sti) {
				float u0 = sx / size3x;
				float v0 = sy / size3y;
				float uW = w  / size3x;
				float uH = h  / size3y;
				g->layer->pushImage(dx, dy, w, h,
					u0, v0, uW, uH,
					bg ? bg->bs.boRadiuses[0] : 0, bg ? bg->bs.boRadiuses[1] : 0,
					bg ? bg->bs.boRadiuses[2] : 0, bg ? bg->bs.boRadiuses[3] : 0,
					bg ? bg->aa : 0,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0, 0xFFFFFFFF,
					dispZ, &sti->handle, g->fb, g->fbsize, g->viewId,
					0.0f,
					bg ? bg->cornerPattern : 0.0f,
					bg ? bg->backUVMinX : 0.0f);
			}
		};

		// === Phase A: cell の measure + write ===
		//   fbo を持つ各 unit (Tile / StandaloneTextureInfo) の heldGen を Offscreen::updateGen と比較。
		//   tile mode: tiledTarget は維持したまま、可視 4 region 内の各 tile を per-tile に処理する。
		//              push() 直前に layer->tiledTargetPageIndex = tileIdx を立てて、auto-expand 経路を
		//              「単一 tile only push」に切り替える。これで:
		//                - heldGen != curGen の tile だけ書くという per-tile 最適化が維持される
		//                - cmd は tiledImageId + tileIndex で保存される (submit 時 resolve) ので、
		//                  &tile.fbo の dangling 問題が起きない
		//                - 同一 cell が複数 region iteration で重複 push されるのを防ぐ (done[] で dedup)
		//   standalone: sti->heldGen 不一致なら 4 領域全部 write。
		{
			TiledTextureInfo*      tti = mygetTiledTextureInfo(thgc, writeId);
			StandaloneTextureInfo* sti = mygetStandaloneTextureInfo(thgc, writeId);
			uint32_t curGen = elem->offscreen->updateGen;

			if (tti) {
				std::vector<bool> done(tti->tiles.size(), false);
				auto processTilesInRect = [&](float sx, float sy, float w, float h) {
					if (w <= 0 || h <= 0) return;
					tti->forEachTileInRect((int)sx, (int)sy, (int)w, (int)h,
						[&](TiledTextureInfo::Tile& tile, int, int, int, int) {
							int ts = (int)tti->tileSize, tsy = (int)tti->tileSizeY;
							if (ts <= 0 || tsy <= 0) return;
							int row = (int)tile.y / tsy;
							int col = (int)tile.x / ts;
							int idx = row * tti->tilesX + col;
							if ((unsigned)idx >= done.size() || done[idx]) return;
							done[idx] = true;
							// heldGen 一致 = この tile は前フレームで書き終わって変更が無い → 再書き込み skip。
							// (= 開いた直後だけ初回 write、以降は scroll 等で別 tile が visible になったときだけ
							//   その新 tile を 1 回 write する流れになり、毎フレーム 数万 cmd 投げる事故を防ぐ)
							if (tile.heldGen.load(std::memory_order_relaxed) == curGen) return;
							int savedPage = g->layer->tiledTargetPageIndex;
							g->layer->tiledTargetPageIndex = idx;
							writeCellsInRect((int)tile.x, (int)tile.y, (int)tile.w, (int)tile.h,
							                 g->layer->tiledPlaceholderFBO, &tti->totalSize);
							g->layer->tiledTargetPageIndex = savedPage;
							// publish() の block 機構で render 確定なので即時更新 OK。
							tile.heldGen.store(curGen, std::memory_order_relaxed);
						});
				};
				processTilesInRect(0,        0,        fzW,  fzH);
				processTilesInRect(fzW+scrX, 0,        visW, fzH);
				processTilesInRect(0,        fzH+scrY, fzW,  visH);
				processTilesInRect(fzW+scrX, fzH+scrY, visW, visH);
			}
			else if (sti) {
				if (sti->heldGen.load(std::memory_order_relaxed) != curGen) {
					// 1 枚テクスチャ: テクスチャサイズ全体で 1 回呼び出し。
					writeCellsInRect(0, 0, sti->size.x, sti->size.y, &sti->fbo, &sti->size);
					// publish() の block 機構により、producer の次フレーム実行時には
					// consumer が cmds 受領済 (= render 確定) なので即時更新で OK。
					sti->heldGen.store(curGen, std::memory_order_relaxed);
				}
			}

			// === 編集中 cell の overlay (= メインウィンドウ winFb に直接描画) ===
			//   offscreen / scroll / freeze と独立に常に最前面に表示する。
			//   1) 白背景 + 黒ボーダーを cell サイズで winFb に push
			//   2) cell content を winFb に push
			NewElement* editCell = tableEditCell(t);
			if (editCell && editCell->Draw) {
				LineInfo* eRow = tableEditRow(t);
				LineInfo* eCol = tableEditCol(t);
				if (eRow && eCol) {
					// 編集 cell のスクリーン絶対位置 (= table 開始位置 + cell の table 内位置、freeze/scroll 考慮)
					float screenX = absPos.x + elem->margins[3] + elem->boWidthes[3] + elem->paddings[3]
					              + tableScreenX(t, eCol, elem->scroll.x);
					float screenY = absPos.y + elem->margins[0] + elem->boWidthes[0] + elem->paddings[0]
					              + tableScreenY(t, eRow, elem->scroll.y);
					float cellW = editCell->size.x;
					float cellH = editCell->size.y;
					// table の表示 dispZ (= 10000 + floor(g->zIndex+elem->zIndex)) よりさらに上に乗せる
					float zEdit = 10002 + std::floor(z);

					// === scissor (clip rect) ===
					// winFb に直接 push するので何もしないと cell content (overflow 含む) が
					// メインウィンドウ全域に貼り付く。
					//
					// scissor は cell 自身ではなく Table の表示範囲全体
					//   (dispBaseX, dispBaseY, viewSizex, viewSizey)  = Phase B の 4 blit が覆う領域
					// にする。cell サイズで clip すると overflow content (= 編集中に見せたい全文)
					// まで切れて overlay の意味がなくなるため。
					// renderAllCommands まで生きる必要があるので構造体メンバ (t->editScissor) に書く。
					t->editScissor = ScissorRect{ dispBaseX, dispBaseY, viewSizex, viewSizey };
					const ScissorRect* savedScissor = g->layer->currentScissor;
					g->layer->currentScissor = &t->editScissor;

					// 1) 白背景 + ボーダー
					Background editBg{};
					editBg.type           = DrawCommandType::Fill;
					editBg.fillcolor      = 0xFFFFFFFF;
					editBg.bs.borderColor = 0x000000FF;
					float boWidth[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
					g->layer->pushBackground(&editBg,
						screenX, screenY, cellW, cellH,
						zEdit,
						&nulltex, &nulltex, &nulltex, &nulltex,
						g->winFb, g->winFbsize, g->winViewId, 0.0f, boWidth, &editBg.bs);
					// 2) cell content を winFb に
					NewGraphic cg = g2;
					cg.pos.x  = screenX;
					cg.pos.y  = screenY;
					cg.fb     = g->winFb;
					cg.fbsize = g->winFbsize;
					cg.viewId = g->winViewId;
					cg.zIndex = zEdit + 0.01f;
					editCell->Draw(thgc, editCell, &cg, local, q);

					g->layer->currentScissor = savedScissor;
				}
			}
		}

		// === Phase B ===
		// curlProgress active なら、ページめくり方向に応じて該当 region のみを
		// pushPageCurl で描画する (= 残り region は固定 = 通常 pushDisplay):
		//   Y-page (axis=0): Left + Center にカール (X 固定なので TL / Top はそのまま)
		//   X-page (axis=1): Top  + Center にカール (Y 固定なので TL / Left はそのまま)
		// tile mode: framework が tile size をページサイズの倍数に丸めているので、
		//            1 ページは必ず 1 タイル内に収まる (= from-tile / to-tile を tileAt で
		//            引けば、pushPageCurl の tex1/tex2 にそのまま渡せる)。
		bool curlActive = (elem->offscreen->curlProgress >= 0.0f && elem->offscreen->curlProgress <= 1.0f);
		StandaloneTextureInfo* curlSti = mygetStandaloneTextureInfo(thgc, readId);
		TiledTextureInfo*      curlTti = mygetTiledTextureInfo(thgc, readId);
		if (curlActive && (curlSti || curlTti)) {
			float fromPage = elem->offscreen->curlFromPage;
			float toPage   = elem->offscreen->curlToPage;
			float progress = elem->offscreen->curlProgress;
			int   axis     = elem->offscreen->curlAxis;
			bool  goingBack    = toPage < fromPage;
			float curlAngle    = (axis == 0) ? (goingBack ? -1.5708f : 1.5708f)
			                                 : (goingBack ?  3.14159f : 0.0f);
			float endProgress  = elem->offscreen->spread ? 0.5f : 1.0f;
			float drawProgress = goingBack ? (endProgress - progress) : progress;

			// region 単位の curl: sx,sy = offscreen 座標の region 開始位置, w,h = region サイズ。
			//   from page Y (or X) は freeze 領域分オフセットしてから page 番号 × ページ幅で進む。
			auto pushRegionCurl = [&](float sx, float sy, float dx, float dy, float w, float h) {
				if (w <= 0 || h <= 0) return;
				// 1 ページの offscreen 上の絶対位置 (= sx + page * w / sy + page * h)
				float fromBaseX = sx, fromBaseY = sy;
				float toBaseX   = sx, toBaseY   = sy;
				if (axis == 0) {
					fromBaseY = sy + fromPage * h;
					toBaseY   = sy + toPage   * h;
				} else {
					fromBaseX = sx + fromPage * w;
					toBaseX   = sx + toPage   * w;
				}
				bgfx::TextureHandle* fromTex = nullptr;
				bgfx::TextureHandle* toTex   = nullptr;
				float fromUvX, fromUvY, toUvX, toUvY, uvW, uvH;
				if (curlTti) {
					// tile mode: from / to をそれぞれ覆うタイルを引いて、UV はタイルローカル。
					//   framework が tile size をページサイズの倍数に丸めているので、
					//   1 ページは 1 タイル内に必ず収まる (= 単一 tile.handle で済む)。
					// lazy 確保: tileAt が返す前に index 経由で ensureTile
					int fromIdx = curlTti->tileIndexAt((int)fromBaseX, (int)fromBaseY);
					int toIdx   = curlTti->tileIndexAt((int)toBaseX,   (int)toBaseY);
					if (fromIdx >= 0) mEnsureTile(thgc, readId, fromIdx);
					if (toIdx   >= 0) mEnsureTile(thgc, readId, toIdx);
					auto* fromTile = curlTti->tileAt((int)fromBaseX, (int)fromBaseY);
					auto* toTile   = curlTti->tileAt((int)toBaseX,   (int)toBaseY);
					if (!fromTile || !toTile) return;
					fromTex = &fromTile->handle;
					toTex   = &toTile->handle;
					float fromTw = (float)fromTile->w, fromTh = (float)fromTile->h;
					float toTw   = (float)toTile->w,   toTh   = (float)toTile->h;
					fromUvX = (fromBaseX - (float)fromTile->x) / fromTw;
					fromUvY = (fromBaseY - (float)fromTile->y) / fromTh;
					toUvX   = (toBaseX   - (float)toTile->x)   / toTw;
					toUvY   = (toBaseY   - (float)toTile->y)   / toTh;
					// uvW / uvH はタイルローカル比 (= w / tile.w 等)。from/to 同じタイルサイズ前提。
					uvW = w / fromTw;
					uvH = h / fromTh;
				} else {
					// standalone: 1 枚テクスチャ全体に対する UV。
					fromTex = &curlSti->handle;
					toTex   = &curlSti->handle;
					fromUvX = fromBaseX / size3x;
					fromUvY = fromBaseY / size3y;
					toUvX   = toBaseX   / size3x;
					toUvY   = toBaseY   / size3y;
					uvW     = w / size3x;
					uvH     = h / size3y;
				}
				float drawFromUvX = goingBack ? toUvX   : fromUvX;
				float drawFromUvY = goingBack ? toUvY   : fromUvY;
				float drawToUvX   = goingBack ? fromUvX : toUvX;
				float drawToUvY   = goingBack ? fromUvY : toUvY;
				bgfx::TextureHandle* drawFromTex = goingBack ? toTex   : fromTex;
				bgfx::TextureHandle* drawToTex   = goingBack ? fromTex : toTex;
				g->layer->pushPageCurl(dx, dy, w, h,
					drawProgress, -30.0f, curlAngle,
					0xFFFFFFFF,
					drawFromUvX, drawFromUvY, uvW, uvH,
					drawToUvX, drawToUvY,
					elem->boWidthes[0], elem->boWidthes[1], elem->boWidthes[2], elem->boWidthes[3],
					bg ? bg->bs.borderColor : 0,
					bg ? bg->bs.shadowX : 0, bg ? bg->bs.shadowY : 0, bg ? bg->bs.shadowBlur : 0,
					bg ? bg->bs.shadowColor : 0,
					drawFromTex, drawToTex, dispZ,
					g->fb, g->fbsize, g->viewId, 0.0f, axis == 0, elem->offscreen->spread);
			};

			// TL: 必ず固定 (freeze の対角)
			pushDisplay(0, 0, dispBaseX, dispBaseY, fzW, fzH);
			if (axis == 0) {
				// Y-page: X 軸は不変 → Top はそのまま、Left + Center は curl
				pushDisplay(fzW+scrX, 0, dispBaseX+fzW, dispBaseY, visW, fzH);            // Top (no curl)
				pushRegionCurl(0,        fzH, dispBaseX,     dispBaseY+fzH, fzW,  visH);  // Left (curl)
				pushRegionCurl(fzW+scrX, fzH, dispBaseX+fzW, dispBaseY+fzH, visW, visH);  // Center (curl)
			} else {
				// X-page: Y 軸は不変 → Left はそのまま、Top + Center は curl
				pushDisplay(0, fzH+scrY, dispBaseX, dispBaseY+fzH, fzW, visH);            // Left (no curl)
				pushRegionCurl(fzW, 0,   dispBaseX+fzW, dispBaseY,     visW, fzH);        // Top (curl)
				pushRegionCurl(fzW, fzH, dispBaseX+fzW, dispBaseY+fzH, visW, visH);       // Center (curl)
			}

			// アニメーション進行
			elem->offscreen->curlProgress += g->deltaTime * 2.0f;
			if (elem->offscreen->curlProgress > endProgress) {
				elem->offscreen->curlProgress = -1.0f;
				if (axis == 0) elem->scroll.y = toPage * visH;
				else           elem->scroll.x = toPage * visW;
			}
			elem->offscreen->markPaint(local);
		}
		else {
			pushDisplay(0,        0,        dispBaseX,        dispBaseY,        fzW,  fzH);   // TL
			pushDisplay(fzW+scrX, 0,        dispBaseX+fzW,    dispBaseY,        visW, fzH);   // Top
			pushDisplay(0,        fzH+scrY, dispBaseX,        dispBaseY+fzH,    fzW,  visH); // Left
			pushDisplay(fzW+scrX, fzH+scrY, dispBaseX+fzW,    dispBaseY+fzH,    visW, visH); // Center
		}

		// === 7. スクロールバー / ページングバー overlay ===
		// offscreen で描画した内容を window fb に display した上に、スクロール量を
		// 視覚化する overlay を重ねる。
		// 重要: PaintOffscreen で early-return すると scroll 値の変化に追従しない
		// (cache にスクロール値が乗らない設計) ため、early-return より前で書く。
		// z は dispZ + 1 (= table image より必ず上) にする。固定 10001 だと親 sidelet 等の
		// zIndex によって image が 10001 を超えて隠す事故が起きる。
		{
			PointF absPos2 = getAbsolutePosition(elem);
			float overlayZ = dispZ + 1.0f;
			if (baseType(elem->ytype) == SizeType::Scroll) {
				drawRightScrollBar(g2.layer,
					absPos2.x + viewSizex - sbarx * 3 / 2, absPos2.y, sbarx, viewSizey - sbary, sbarx * 3 / 2,
					elem->scroll.y, viewSizey, elem->size2.y,
					overlayZ, g2.winFb, g2.winFbsize, g2.winViewId);
			}
			if (baseType(elem->xtype) == SizeType::Scroll) {
				drawUnderScrollBar(g2.layer,
					absPos2.x, absPos2.y + viewSizey - sbary * 3 / 2, viewSizex - sbarx, sbary, sbary * 3 / 2,
					elem->scroll.x, viewSizex, elem->size2.x,
					overlayZ, g2.winFb, g2.winFbsize, g2.winViewId);
			}
			if (baseType(elem->ytype) == SizeType::Page) {
				float totalPages = std::ceil(elem->size2.y / viewSizey);
				float currentPage = std::floor(elem->scroll.y / viewSizey);
				drawRightPagingBar(g2.layer, *getAtlas(thgc), getFont("sans", 16),
					absPos2.x + viewSizex - sbarx, absPos2.y, sbarx, viewSizey - sbary,
					currentPage, totalPages,
					overlayZ, g2.group, g2.winFb, g2.winFbsize, g2.winViewId);
			}
			if (baseType(elem->xtype) == SizeType::Page) {
				float totalPages = std::ceil(elem->size2.x / viewSizex);
				float currentPage = std::floor(elem->scroll.x / viewSizex);
				drawUnderPagingBar(g2.layer, *getAtlas(thgc), getFont("sans", 16),
					absPos2.x, absPos2.y + viewSizey - sbary, viewSizex - sbarx, sbary,
					currentPage, totalPages,
					overlayZ, g2.group, g2.winFb, g2.winFbsize, g2.winViewId);
			}
		}

		if (g->paint == Offscreen::PaintOffscreen) {
			g->layer->tiledTarget         = _saveTiledTarget;
			g->layer->tiledTargetImageId  = _saveTiledTargetImageId;
			g->layer->tiledPlaceholderFBO = _saveTiledPlaceholderFBO;
			g->layer->tiledBaseViewId     = _saveTiledBaseViewId;
			return;
		}
	}
	else {
		// === 非 offscreen の単純全描画パス ===
		// 座標系: g->pos = 親の描画先 fb 内での自分の左上 (parent-local)。
		// **絶対座標 (getAbsolutePosition) は使わない**。親が offscreen の場合
		// g->pos は offscreen-local、親が window 直なら window 座標、いずれも
		// "親の fb で意味のある座標" になっている。ElementDraw の else 分岐と
		// 同じ作法。
		NewGraphic g2 = *g;
		g2.start   = g->pos;
		g2.pos.x   = g->pos.x + elem->margins[3]
					+ elem->boWidthes[3] + elem->paddings[3];
		g2.pos.y   = g->pos.y + elem->margins[0]
					+ elem->boWidthes[0] + elem->paddings[0];
		// **絶対 z** にする (= 親 z + elem 自分 z)。bg / 各 cell / grid 線が
		// 同じ z 空間で重ね順を保つため。これをローカル相対のままにすると
		// 親が深くネスト (sidelet 等で z>0) してる場合に bg と cell の
		// 順序が逆転して cell が隠れる。
		g2.zIndex  = g->zIndex + elem->zIndex;
		float bgZ  = std::floor(g2.zIndex);
		BorderShadow noBs{};

		// 背景
		if (elem->background) {
			float noBo[4] = {0,0,0,0};
			g2.layer->pushBackground(elem->background,
				g2.pos.x, g2.pos.y, elem->size.x, elem->size.y, bgZ,
				&nulltex, &nulltex, &nulltex, &nulltex,
				g->fb, g->fbsize, g->viewId, 1.0f, noBo, &noBs);
		}

		// totalW / totalH は NewTableMeasure で size2 として計算済み。
		float totalW = elem->size2.x;
		float totalH = elem->size2.y;

		// === scratch に visible row/col を 1 walk で収集 (= 以降の cell/grid iter は線形) ===
		visibleRowsScratch->size = 0;
		visibleColsScratch->size = 0;
		walkVisibleLines(t->rows, [&](LineInfo* l, int /*i*/) {
			if (l->visible) add_list(thgc, visibleRowsScratch, (char*)l);
		});
		walkVisibleLines(t->columns, [&](LineInfo* l, int /*i*/) {
			if (l->visible) add_list(thgc, visibleColsScratch, (char*)l);
		});
		int rN = visibleRowsScratch->size;
		int cN = visibleColsScratch->size;

		// === Cell 描画 (nested loop + colspan は pending pattern) ===
		// 行番号取得を高速化するためのヒント (= 前回 (parent, depth) と一致なら +1 から検索)
		int       prevRowDepth       = -1;
		int       prevRowIdxInParent = -1;
		LineInfo* prevRowParent      = NULL;
		float ay2 = 0;
		for (int i = 0; i < rN; i++) {
			LineInfo* row = (LineInfo*)*get_list2(visibleRowsScratch, i);
			float h = row->height;
			bool isHeaderRow = row->showLines;
			int hintIdx = (row->parent == prevRowParent && row->depth == prevRowDepth)
				? prevRowIdxInParent + 1 : -1;
			int rowIdxInParent = rowIndexInParentHint(t, row, hintIdx);
			prevRowParent      = row->parent;
			prevRowDepth       = row->depth;
			prevRowIdxInParent = rowIdxInParent;

			NewElement* pendingCell = nullptr;
			float pendingX = 0, pendingW = 0;
			auto flushPending = [&]() {
				if (!pendingCell) return;
				float cellX = g2.pos.x + pendingX;
				float cellY = g2.pos.y + ay2;
				// cell の table-local pos を pos2 に書き戻す (= expand/collapse 後の最新位置)。
				pendingCell->pos2.x = pendingX;
				pendingCell->pos2.y = ay2;
				bool needScissor = (pendingCell->size.x > pendingW) || (pendingCell->size.y > h)
					|| (pendingCell->size2.x > pendingW) || (pendingCell->size2.y > h);
				const ScissorRect* savedSci = g2.layer->currentScissor;
				if (needScissor && cellScissors.size() < cellScissors.capacity()) {
					cellScissors.push_back(ScissorRect{ cellX, cellY, pendingW, h });
					g2.layer->currentScissor = &cellScissors.back();
				}
				NewGraphic cg = g2;
				cg.pos.x  = cellX;
				cg.pos.y  = cellY;
				cg.fb     = g->fb;
				cg.fbsize = g->fbsize;
				cg.viewId = g->viewId;
				pendingCell->Draw(thgc, pendingCell, &cg, local, q);
				if (needScissor) g2.layer->currentScissor = savedSci;
				pendingCell = nullptr;
			};

			int       prevColDepth       = -1;
			int       prevColIdxInParent = -1;
			LineInfo* prevColParent      = NULL;
			float ax2 = 0;
			for (int j = 0; j < cN; j++) {
				LineInfo* col = (LineInfo*)*get_list2(visibleColsScratch, j);
				float w = col->width;
				float cellX = g2.pos.x + ax2;
				float cellY = g2.pos.y + ay2;
				bool isHeaderCol = col->showLines;
				int hintColIdx = (col->parent == prevColParent && col->depth == prevColDepth)
					? prevColIdxInParent + 1 : -1;
				int colIdxInParent = colIndexInParentHint(t, col, hintColIdx);
				prevColParent      = col->parent;
				prevColDepth       = col->depth;
				prevColIdxInParent = colIdxInParent;

				if (isHeaderCol || isHeaderRow) {
					flushPending();
					Background headBg{};
					headBg.type      = DrawCommandType::Fill;
					headBg.fillcolor = t->headerBgColor;
					float zero[4] = {0,0,0,0};
					g2.layer->pushBackground(&headBg, cellX, cellY, w, h,
						g2.zIndex + 0.3f, &nulltex, &nulltex, &nulltex, &nulltex,
						g->fb, g->fbsize, g->viewId, 0.0f, zero, &headBg.bs);

					NewElement* hcell = getTableCellById(t, row->id, col->id);
					if (hcell) { hcell->pos2.x = ax2; hcell->pos2.y = ay2; }
					if (hcell && hcell->background) {
						Background overlayBg{};
						overlayBg.type           = DrawCommandType::Fill;
						overlayBg.fillcolor      = hcell->background->fillcolor;
						overlayBg.bs.borderColor = hcell->background->bs.borderColor;
						g2.layer->pushBackground(&overlayBg, cellX, cellY, w, h,
							g2.zIndex + 0.305f, &nulltex, &nulltex, &nulltex, &nulltex,
							g->fb, g->fbsize, g->viewId, 0.0f, zero, &overlayBg.bs);
					}

					String* str = nullptr;
					if (isHeaderCol && !isHeaderRow) {
						// rowIdxInParent / colIdxInParent はループ冒頭で計算済 (= 前回ヒント)
						str = getCachedLineNumber(thgc, t,
							rowIdxInParent >= 0 ? rowIdxInParent : i);
					}
					else if (!isHeaderCol && isHeaderRow) {
						str = getCachedColumnLetter(thgc, t,
							colIdxInParent >= 0 ? colIdxInParent : j);
					}
					if (str && str->size > 0) {
						const float padX = 4.0f, padY = 2.0f;
						float availW = w - padX * 2;
						if (availW > 0) {
							float mw = 0, mh = 0; size_t mn = 0;
							MeasureString(*getAtlas(thgc), t->font, str, str->size,
								10000.0f, &mw, &mh, &mn, g2.group);
							float scaleX = (mw > availW && mw > 0) ? (availW / mw) : 1.0f;
							drawStringScaledRange(g2.layer, *getAtlas(thgc), t->font,
								str, 0, str->size, cellX + padX, cellY + padY, scaleX,
								g2.zIndex + 0.31f, t->headerTextColor, g2.group,
								g->fb, g->fbsize, g->viewId);
						}
					}
				} else {
					NewElement* cell = getTableCellById(t, row->id, col->id);
					if (cell && cell->Draw) {
						flushPending();
						pendingCell = cell;
						pendingX    = ax2;
						pendingW    = w;
					} else if (pendingCell) {
						pendingW += w;
					}
				}
				ax2 += w;
			}
			flushPending();
			ay2 += h;
		}

		// === グリッド線 ===
		if (t->showGridX || t->showGridY) {
			Background line{};
			line.type      = DrawCommandType::Fill;
			line.fillcolor = t->gridColor;
			float zero[4] = {0,0,0,0};
			float gridZ   = g2.zIndex + 0.4f;
			if (t->showGridX) {
				float curX = 0;
				for (int j = 0; j < cN; j++) {
					LineInfo* col = (LineInfo*)*get_list2(visibleColsScratch, j);
					g2.layer->pushBackground(&line,
						g2.pos.x + curX - 0.5f, g2.pos.y, 1.0f, totalH, gridZ,
						&nulltex, &nulltex, &nulltex, &nulltex,
						g->fb, g->fbsize, g->viewId, 0.0f, zero, &line.bs);
					curX += col->width;
				}
				g2.layer->pushBackground(&line,
					g2.pos.x + totalW - 0.5f, g2.pos.y, 1.0f, totalH, gridZ,
					&nulltex, &nulltex, &nulltex, &nulltex,
					g->fb, g->fbsize, g->viewId, 0.0f, zero, &line.bs);
			}
			if (t->showGridY) {
				float curYg = 0;
				for (int i = 0; i < rN; i++) {
					LineInfo* row = (LineInfo*)*get_list2(visibleRowsScratch, i);
					g2.layer->pushBackground(&line,
						g2.pos.x, g2.pos.y + curYg - 0.5f, totalW, 1.0f, gridZ,
						&nulltex, &nulltex, &nulltex, &nulltex,
						g->fb, g->fbsize, g->viewId, 0.0f, zero, &line.bs);
					curYg += row->height;
				}
				g2.layer->pushBackground(&line,
					g2.pos.x, g2.pos.y + totalH - 0.5f, totalW, 1.0f, gridZ,
					&nulltex, &nulltex, &nulltex, &nulltex,
					g->fb, g->fbsize, g->viewId, 0.0f, zero, &line.bs);
			}
		}

		// === 編集中 cell の overlay (= 全要素より前面) ===
		//   z は 10000+ ベース (= offscreen 経路の zEdit と同水準)。
		//   path から LineInfo* を引いて tableAbs で位置を出す (= 1 walk / 軸)。
		NewElement* editCell = tableEditCell(t);
		LineInfo* eRow = tableEditRow(t);
		LineInfo* eCol = tableEditCol(t);
		if (editCell && editCell->Draw && eRow && eCol) {
			float editX = tableAbsX(t, eCol);
			float editY = tableAbsY(t, eRow);
			bool foundCol = true, foundRow = true;
			if (foundCol && foundRow) {
				float cellW = editCell->size.x;
				float cellH = editCell->size.y;
				float zEdit = 10002 + std::floor(g2.zIndex);
				Background editBg{};
				editBg.type           = DrawCommandType::Fill;
				editBg.fillcolor      = 0xFFFFFFFF;
				editBg.bs.borderColor = 0x000000FF;
				float boWidth[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				g2.layer->pushBackground(&editBg,
					g2.pos.x + editX, g2.pos.y + editY, cellW, cellH,
					zEdit, &nulltex, &nulltex, &nulltex, &nulltex,
					g->fb, g->fbsize, g->viewId, 0.0f, boWidth, &editBg.bs);
				NewGraphic cg = g2;
				cg.pos.x  = g2.pos.x + editX;
				cg.pos.y  = g2.pos.y + editY;
				cg.fb     = g->fb;
				cg.fbsize = g->fbsize;
				cg.viewId = g->viewId;
				cg.zIndex = zEdit + 0.01f;
				editCell->Draw(thgc, editCell, &cg, local, q);
			}
		}
	}

	// === 9. 親のタイル状態を復元 ===
	g->layer->tiledTarget         = _saveTiledTarget;
	g->layer->tiledTargetImageId  = _saveTiledTargetImageId;
	g->layer->tiledPlaceholderFBO = _saveTiledPlaceholderFBO;
	g->layer->tiledBaseViewId     = _saveTiledBaseViewId;
}

// === 選択 (path-ベース anchor/extent + seq キャッシュ) ===
//   selAnchorRow/Col: 元クリック (= shift-extend の起点)。path で安定保持。
//   selExtentRow/Col: 最後の click/drag 先。path で安定保持。
//   selRows/Cols    : anchor↔extent を visible-DFS で展開した seq cache (描画用)。
//                     expand/collapse 後は再構築 (= path から再 walk)。


// LineInfo* から path (= rootList から line までの i 蓄積) を逆引き。
//   parent ポインタを上に辿り、各レベルで container を線形 scan して idx を取る。
//   tree depth は浅いので scan コストは無視できる。
inline void pathOfLine(ThreadGC* thgc, List* rootList, LineInfo* line, List* outPath) {
	if (!line) { outPath->size = 0; return; }
	int stack[32];
	int sd = 0;
	LineInfo* cur = line;
	while (cur && sd < 32) {
		LineInfo* parent = cur->parent;
		List* container = parent ? parent->childs : rootList;
		int idx = -1;
		int n = container->size;
		for (int k = 0; k < n; k++) {
			if ((LineInfo*)*get_list2(container, k) == cur) { idx = k; break; }
		}
		if (idx < 0) { outPath->size = 0; return; }
		stack[sd++] = idx;
		cur = parent;
	}
	// stack は leaf→root 順に積まれてるので outPath には逆順で書く。
	if (outPath->capa < sd) changeCapa(thgc, outPath, sd);
	int* dst = (int*)outPath->data;
	for (int k = 0; k < sd; k++) dst[k] = stack[sd - 1 - k];
	outPath->size = sd;
}

// path (= List<int>: rootList から target までの i 蓄積) で line を引く。
//   path 不正なら NULL。
inline LineInfo* lineByPath(List* rootList, List* path) {
	if (!path || path->size == 0) return nullptr;
	int* p = (int*)path->data;
	List* container = rootList;
	LineInfo* cur = nullptr;
	for (int d = 0; d < path->size; d++) {
		int idx = p[d];
		if (idx < 0 || idx >= container->size) return nullptr;
		cur = (LineInfo*)*get_list2(container, idx);
		if (d + 1 < path->size) {
			if (!cur || !cur->childs) return nullptr;
			container = cur->childs;
		}
	}
	return cur;
}

// 1 つの target line の visible-DFS seq を walk で求める。target が non-visible / 不在なら -1。
inline int seqOfLine(List* lines, LineInfo* target) {
	int seq = 0;
	int found = -1;
	walkVisibleLinesUntil(lines, [&](LineInfo* l, int /*i*/) -> bool {
		if (l == target) { found = seq; return false; }
		seq++;
		return true;
	});
	return found;
}

// 2 つの target line の visible-DFS seq を 1 walk で同時に求める。
// 両方ヒットしたら早期終了。target が non-visible / 不在なら出力 -1。
inline void seqOf2Lines(List* lines, LineInfo* a, LineInfo* b, int* outA, int* outB) {
	*outA = -1;
	*outB = -1;
	int seq = 0;
	walkVisibleLinesUntil(lines, [&](LineInfo* l, int /*i*/) -> bool {
		if (l == a) *outA = seq;
		if (l == b) *outB = seq;
		if (*outA >= 0 && *outB >= 0) return false;
		seq++;
		return true;
	});
}


inline LineInfo* tableEditRow(NewTable* t) { return lineByPath(t->rows,    t->editRowPath); }
inline LineInfo* tableEditCol(NewTable* t) { return lineByPath(t->columns, t->editColPath); }
inline NewElement* tableEditCell(NewTable* t) {
	LineInfo* eRow = tableEditRow(t);
	LineInfo* eCol = tableEditCol(t);
	if (!eRow || !eCol) return NULL;
	return (NewElement*)get_mapyn(t->cells, tableKey(eRow->id, eCol->id));
}
inline bool isTableEditCellLine(NewTable* t, LineInfo* row, LineInfo* col) {
	LineInfo* eRow = tableEditRow(t);
	LineInfo* eCol = tableEditCol(t);
	return row == eRow && col == eCol;
}

// (lx, ly) スクリーンローカル座標 → (i, j) 逆引き。範囲外は -1。
//   freeze 内 (lx < freezeWidth) は scroll を加えず、それ以外は scrollX を加えた絶対 X で列を探す。
// 内部: tree DFS visible で hit 探索しつつ path (祖先 i 蓄積) を build。
//   見つかったら outPath に path をコピー、outSeq に visible-DFS 通し番号をセット。
inline bool tableHitLineHelper(ThreadGC* thgc, LineInfo* l, int i, int sd,
                                int* stackIdxs,
                                float* posPtr, float target, int* seqRef,
                                LineInfo** outHit, List* outPath, int* outSeq) {
	if (!l) return true;
	float pos = *posPtr;
	if (target < pos + l->size) {
		*outHit = l;
		*outSeq = *seqRef;
		outPath->size = 0;
		for (int k = 0; k < sd; k++) add_list_v(thgc, outPath, &stackIdxs[k]);
		add_list_v(thgc, outPath, &i);
		return false;
	}
	*posPtr = pos + l->size;
	(*seqRef)++;
	if (l->expanded && l->childs) {
		stackIdxs[sd] = i;
		int n = l->childs->size;
		for (int k = 0; k < n; k++) {
			LineInfo* ch = (LineInfo*)*get_list2(l->childs, k);
			if (!tableHitLineHelper(thgc, ch, k, sd + 1, stackIdxs,
			                         posPtr, target, seqRef, outHit, outPath, outSeq))
				return false;
		}
	}
	return true;
}

// pos (lines 上の絶対座標) で hit する visible line を返す。
//   outPath: rootList から hit までの i 蓄積 (= 安定 ID)。caller が空 List<int> を渡す。
//   outSeq:  visible-DFS 通し番号 (= 描画ループでの選択判定用)。
inline LineInfo* tableHitLine(ThreadGC* thgc, List* lines, float target,
                              List* outPath, int* outSeq) {
	LineInfo* hit = nullptr;
	int hitSeq = -1;
	int seq = 0;
	int stackIdxs[32];
	float pos = 0;
	int n = lines->size;
	for (int i = 0; i < n; i++) {
		LineInfo* l = (LineInfo*)*get_list2(lines, i);
		if (!tableHitLineHelper(thgc, l, i, 0, stackIdxs,
		                         &pos, target, &seq, &hit, outPath, &hitSeq))
			break;
	}
	if (outSeq) *outSeq = hitSeq;
	return hit;
}

// table-local 座標 (lx, ly) で hit する visible row/col を返す。
//   freeze 内なら座標そのまま、freeze 外なら scroll を加算してから判定。
//   outRow/outCol = LineInfo*、outRowPath/outColPath = path (= 安定 ID)、
//   outRowSeq/outColSeq = visible-DFS 通し番号。
inline void tableHitTest(ThreadGC* thgc, NewTable* t, float lx, float ly,
                         LineInfo** outRow, LineInfo** outCol,
                         List* outRowPath, int* outRowSeq,
                         List* outColPath, int* outColSeq) {
	float fzW = tableFreezeWidth(t);
	float fzH = tableFreezeHeight(t);
	float scrollX = ((NewElement*)t)->scroll.x;
	float scrollY = ((NewElement*)t)->scroll.y;
	float absX = (lx < fzW) ? lx : (lx + scrollX);
	float absY = (ly < fzH) ? ly : (ly + scrollY);
	*outCol = tableHitLine(thgc, t->columns, absX, outColPath, outColSeq);
	*outRow = tableHitLine(thgc, t->rows,    absY, outRowPath, outRowSeq);
}

// DrawSelection: 2D 矩形選択範囲のセルを overlay にハイライト。
//   cellselect == false なら何もしない。
void TableDrawSelection(ThreadGC* thgc, NewLocal* local, NewElement* self, int m, int n, NewGraphic* g, PointF pos, RenderCommandQueue* q) {
	(void)m; (void)n; (void)pos; (void)q;
	NewTable* t = (NewTable*)self;
	if (!t->cellselect) return;
	if (local->select.from != self || local->select.to != self) return;
	if (!t->selAnchorRow || !t->selAnchorCol) return;

	// anchor / extent path → LineInfo* → cell。anchor / extent cell は click 時に確定済なので
	// 必ず存在する。それぞれの cell->pos2 (= Draw 時に書き戻された table-local pos) を使えば
	// 走査なしで矩形範囲を算出できる。
	LineInfo* aRow = lineByPath(t->rows,    t->selAnchorRow);
	LineInfo* eRow = lineByPath(t->rows,    t->selExtentRow);
	LineInfo* aCol = lineByPath(t->columns, t->selAnchorCol);
	LineInfo* eCol = lineByPath(t->columns, t->selExtentCol);
	if (!aRow || !eRow || !aCol || !eCol) return;
	NewElement* aCell = getTableCellById(t, aRow->id, aCol->id);
	NewElement* eCell = getTableCellById(t, eRow->id, eCol->id);
	if (!aCell || !eCell) return;

	float baseX   = g->pos.x + self->pos.x + self->margins[3];
	float baseY   = g->pos.y + self->pos.y + self->margins[0];
	float scrollX = self->scroll.x;
	float scrollY = self->scroll.y;

	// 矩形範囲: anchor / extent の pos2 と {a/e}{Row/Col} の size から min/max。
	float aL = aCell->pos2.x, aT = aCell->pos2.y;
	float aR = aL + aCol->width, aB = aT + aRow->height;
	float eL = eCell->pos2.x, eT = eCell->pos2.y;
	float eR = eL + eCol->width, eB = eT + eRow->height;
	float left   = aL < eL ? aL : eL;
	float top    = aT < eT ? aT : eT;
	float right  = aR > eR ? aR : eR;
	float bottom = aB > eB ? aB : eB;
	LineInfo* topRow  = (aT < eT) ? aRow : eRow;
	LineInfo* leftCol = (aL < eL) ? aCol : eCol;

	// freeze 判定: top-left の行/列が freeze 範囲外なら scroll を引く。
	float adjX = isInFreezeRange(t->columns, leftCol, t->freezeColCount) ? 0 : scrollX;
	float adjY = isInFreezeRange(t->rows,    topRow,  t->freezeRowCount) ? 0 : scrollY;

	// 1) 選択矩形 1 push で塗る
	{
		Background sel{};
		sel.type      = DrawCommandType::Fill;
		sel.fillcolor = 0x4080E060;
		float zero[4] = {0,0,0,0};
		g->layer->pushBackground(&sel,
			baseX + left - adjX, baseY + top - adjY,
			right - left, bottom - top,
			g->zIndex + self->zIndex + 0.5f,
			&nulltex, &nulltex, &nulltex, &nulltex,
			g->winFb, g->winFbsize, g->winViewId, 0.0f, zero, &sel.bs);
	}

	// 2) anchor cell を上書き (= Excel 風アクティブセル、白塗り + 緑枠)
	{
		float aAdjX = isInFreezeRange(t->columns, aCol, t->freezeColCount) ? 0 : scrollX;
		float aAdjY = isInFreezeRange(t->rows,    aRow, t->freezeRowCount) ? 0 : scrollY;
		Background ah{};
		ah.type      = DrawCommandType::Fill;
		ah.fillcolor = 0xFFFFFFFF;
		ah.bs.borderColor = 0x008000FF;
		float bw[4] = { 2.0f, 2.0f, 2.0f, 2.0f };
		g->layer->pushBackground(&ah,
			baseX + aL - aAdjX, baseY + aT - aAdjY,
			aCol->width, aRow->height,
			g->zIndex + self->zIndex + 0.51f,
			&nulltex, &nulltex, &nulltex, &nulltex,
			g->winFb, g->winFbsize, g->winViewId, 0.0f, bw, &ah.bs);
	}
}

// Key: Esc で edit cell を解除。それ以外は ElementKey にフォールバック。
//   KeyEvent は main.cpp が SDL_EVENT_KEY_DOWN のときだけ生成するので action 判定は不要。
int NewTableKey(ThreadGC* thgc, NewElement* self, int m, int n, KeyEvent* e, NewLocal* local) {
	NewTable* t = (NewTable*)self;
	if (e->key == SDLK_ESCAPE) {
		if (t->editRowPath && t->editColPath) {
			t->editRowPath = NULL;
			t->editColPath = NULL;
			if (self->offscreen) self->offscreen->markPaint(local);
			return -1;
		}
	}
	return ElementKey(thgc, self, m, n, e, local);
}

// Mouse:
//   - MouseDown (button 1): anchor + extent を hit セルに設定 (= 単セル選択)。Shift 押下時は anchor を保持して extent だけ更新 (= 範囲選択)。
//   - MouseMove with button 1 pressed: extent を現在位置に更新 (drag 中)。
//   - その後、hit したセルに event を転送 (cell 内テキスト編集等のため)。
int NewTableMouse(ThreadGC* thgc, NewElement* self, MouseEvent* e, PointF pos, NewLocal* local) {
	NewTable* t = (NewTable*)self;

	// === スクロール / ページングバーのクリック判定 (ElementMouse と同形) ===
	if (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float sbarx = 0, sbary = 0;
		if (baseType(self->xtype) == SizeType::Scroll)      sbary += 5 * 2;
		else if (baseType(self->xtype) == SizeType::Page)   sbary += 15;
		if (baseType(self->ytype) == SizeType::Scroll)      sbarx += 5 * 2;
		else if (baseType(self->ytype) == SizeType::Page)   sbarx += 15;
		float sizex = self->size.x + self->paddings[1] + self->paddings[3];
		float sizey = self->size.y + self->paddings[0] + self->paddings[2];
		float localX = e->x - pos.x - self->pos.x - self->margins[3] - self->boWidthes[3];
		float localY = e->y - pos.y - self->pos.y - self->margins[0] - self->boWidthes[0];

		// 右スクロールバー (ytype == Scroll)
		if (baseType(self->ytype) == SizeType::Scroll && self->size2.y > sizey) {
			float barX = sizex - sbarx;
			float barH = sizey - sbary;
			if (localX >= barX && localX < sizex && localY >= 0 && localY < barH) {
				float maxScroll = self->size2.y - sizey;
				float thumbRatio = sizey / self->size2.y;
				float thumbH = std::max(barH * thumbRatio, 16.f);
				float scrollRatio = (maxScroll > 0) ? self->scroll.y / maxScroll : 0.f;
				float thumbY = (barH - thumbH) * scrollRatio;
				if (localY >= thumbY && localY < thumbY + thumbH) {
					local->scrollDrag = { self, 0, localY - thumbY };
				} else if (localY < thumbY) {
					float newRatio = std::clamp(localY / (barH - thumbH), 0.f, 1.f);
					self->scroll.y = newRatio * maxScroll;
					local->scrollDrag = { self, 0, 0.f };
				} else {
					float newThumbY = localY - thumbH;
					float newRatio = std::clamp(newThumbY / (barH - thumbH), 0.f, 1.f);
					self->scroll.y = newRatio * maxScroll;
					local->scrollDrag = { self, 0, thumbH };
				}
				if (self->offscreen) self->offscreen->markPaint(local);
				return -1;
			}
		}
		// 下スクロールバー (xtype == Scroll)
		if (baseType(self->xtype) == SizeType::Scroll && self->size2.x > sizex) {
			float barY = sizey - sbary;
			float barW = sizex - sbarx;
			if (localY >= barY && localY < sizey && localX >= 0 && localX < barW) {
				float maxScroll = self->size2.x - sizex;
				float thumbRatio = sizex / self->size2.x;
				float thumbW = std::max(barW * thumbRatio, 16.f);
				float scrollRatio = (maxScroll > 0) ? self->scroll.x / maxScroll : 0.f;
				float thumbX = (barW - thumbW) * scrollRatio;
				if (localX >= thumbX && localX < thumbX + thumbW) {
					local->scrollDrag = { self, 1, localX - thumbX };
				} else if (localX < thumbX) {
					float newRatio = std::clamp(localX / (barW - thumbW), 0.f, 1.f);
					self->scroll.x = newRatio * maxScroll;
					local->scrollDrag = { self, 1, 0.f };
				} else {
					float newThumbX = localX - thumbW;
					float newRatio = std::clamp(newThumbX / (barW - thumbW), 0.f, 1.f);
					self->scroll.x = newRatio * maxScroll;
					local->scrollDrag = { self, 1, thumbW };
				}
				if (self->offscreen) self->offscreen->markPaint(local);
				return -1;
			}
		}
		// 右ページングバー (ytype == Page)
		if (baseType(self->ytype) == SizeType::Page && self->size2.y > sizey) {
			float barX = sizex - sbarx;
			if (localX >= barX && localX < sizex && localY >= 0 && localY < sizey) {
				float totalPages = std::ceil(self->size2.y / sizey);
				float currentPage = std::floor(self->scroll.y / sizey);
				float toPage = currentPage;
				if (localY < sizey * 0.5f) {
					if (currentPage > 0) toPage = currentPage - 1;
				} else {
					if (currentPage < totalPages - 1) toPage = currentPage + 1;
				}
				if (toPage != currentPage && self->offscreen) {
					self->offscreen->curlFromPage = currentPage;
					self->offscreen->curlToPage   = toPage;
					self->offscreen->curlProgress = 0.0f;
					self->offscreen->curlAxis     = 0;
					self->offscreen->markPaint(local);
				}
				return -1;
			}
		}
		// 下ページングバー (xtype == Page)
		if (baseType(self->xtype) == SizeType::Page && self->size2.x > sizex) {
			float barY = sizey - sbary;
			if (localY >= barY && localY < sizey && localX >= 0 && localX < sizex) {
				float totalPages = std::ceil(self->size2.x / sizex);
				float currentPage = std::floor(self->scroll.x / sizex);
				float toPage = currentPage;
				if (localX < sizex * 0.5f) {
					if (currentPage > 0) toPage = currentPage - 1;
				} else {
					if (currentPage < totalPages - 1) toPage = currentPage + 1;
				}
				if (toPage != currentPage && self->offscreen) {
					self->offscreen->curlFromPage = currentPage;
					self->offscreen->curlToPage   = toPage;
					self->offscreen->curlProgress = 0.0f;
					self->offscreen->curlAxis     = 1;
					self->offscreen->markPaint(local);
				}
				return -1;
			}
		}
	}

	// pos = 親が累積してきたオフセット (ElementMouse の convention)。
	// content-origin 基準 (= margin + border + padding 後) に揃える。
	// tableAbsX/Y / tableScreenX/Y / cell->pos2 は全部 content-origin 基準なので、
	// lx/ly もそれに合わせる (= 旧 margin 後 だけだと border/padding 分だけずれる)。
	float lx = e->x - pos.x - self->pos.x - self->margins[3] - self->boWidthes[3] - self->paddings[3];
	float ly = e->y - pos.y - self->pos.y - self->margins[0] - self->boWidthes[0] - self->paddings[0];
	if (lx < 0 || ly < 0) return 0;

	int rowSeq = -1, colSeq = -1;
	LineInfo* rowLI = nullptr;
	LineInfo* colLI = nullptr;
	// hit 結果と一緒に row/col の path も拾う (= 選択 anchor/extent 用、安定 ID)。
	List* hitRowPath = create_list(thgc, (int)sizeof(int), CType::_Struct);
	List* hitColPath = create_list(thgc, (int)sizeof(int), CType::_Struct);
	tableHitTest(thgc, t, lx, ly, &rowLI, &colLI,
	             hitRowPath, &rowSeq, hitColPath, &colSeq);

	// 編集中 cell の overflow 領域も hit 対象にする (= grid cell 幅を越えた content 部分)。
	// 上書き時は LineInfo* / seq / path 全部 editCell 行/列のものに差し替える。
	if (t->editRowPath && t->editColPath) {
		NewElement* editCell = tableEditCell(t);
		LineInfo* eRow = tableEditRow(t);
		LineInfo* eCol = tableEditCol(t);
		if (editCell && eRow && eCol) {
			float scrollX = self->scroll.x;
			float scrollY = self->scroll.y;
			float cellX = tableScreenX(t, eCol, scrollX);
			float cellY = tableScreenY(t, eRow, scrollY);
			float cellW = editCell->size.x;     // 実 content 幅 (overflow 含む)
			float cellH = editCell->size.y;
			if (lx >= cellX && lx < cellX + cellW &&
			    ly >= cellY && ly < cellY + cellH) {
				rowLI  = eRow;
				colLI  = eCol;
				rowSeq = seqOfLine(t->rows,    eRow);
				colSeq = seqOfLine(t->columns, eCol);
				pathOfLine(thgc, t->rows,    eRow, hitRowPath);
				pathOfLine(thgc, t->columns, eCol, hitColPath);
			}
		}
	}

	NewElement* cell = (rowLI && colLI) ? getTableCellById(t, rowLI->id, colLI->id) : NULL;

	// === ツリービュー: [+]/[-] アイコンクリックで expanded toggle ===
	// hit 領域 = アイコン列幅全体 × 行全高 (= 中心矩形より広めに、ミスクリック減らす)。
	// 行全高にしてあるので、行のどこを縦に押しても OK。横は indent 開始 ~ indent + iconSize + 4 まで。
	if (t->treeView && rowLI && colLI && cell && (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN)) {
		// 展開可能 = 子ロード済 OR 「展開状態だが未ロード」(= 遅延展開行)
		bool hasChildren = (rowLI->childs && rowLI->childs->size > 0) ||
		                   (rowLI->expanded && !rowLI->childs);
		if (hasChildren && !colLI->showLines) {
			// cell->pos2 = cell の table-content 座標 (= Draw 時に書き戻し済)。
			// tree col の cell pos2.x は colLeftX + depth*indent + iconSize + 4 (= content 開始)。
			// indent column 左端 = pos2.x - iconSize - 4、幅 = treeIndent (1 indent column 分)。
			float indentLeftAbs  = cell->pos2.x - t->treeIconSize - 4;
			float indentRightAbs = indentLeftAbs + t->treeIndent;
			// freeze 範囲外なら scroll 引き、freeze 内なら絶対そのまま。
			bool inFreezeX = isInFreezeRange(t->columns, colLI, t->freezeColCount);
			bool inFreezeY = isInFreezeRange(t->rows,    rowLI, t->freezeRowCount);
			float indentLeft  = inFreezeX ? indentLeftAbs  : (indentLeftAbs  - self->scroll.x);
			float indentRight = inFreezeX ? indentRightAbs : (indentRightAbs - self->scroll.x);
			float rowTop      = inFreezeY ? cell->pos2.y   : (cell->pos2.y   - self->scroll.y);
			float rowBot      = rowTop + rowLI->height;
			if (lx >= indentLeft && lx < indentRight &&
			    ly >= rowTop && ly < rowBot) {
				// 「展開状態だが未ロード」(= expanded=true && childs=null) の行をクリック
				//   → t->onExpand を発火してデータ取得要求 (= expanded は true のまま維持)。
				// それ以外は単純トグル (= 折り畳み / 再展開)。
				if (rowLI->expanded && !rowLI->childs) {
					if (t->onExpand) t->onExpand(thgc, local, t, rowLI);
				} else {
					rowLI->expanded = !rowLI->expanded;
				}
				rebuildLineVisibility(t->rows);
				if (self->offscreen) {
					self->offscreen->markLayout(local, self);
					self->offscreen->markPaint(local);
				}
				return -1;
			}
		}
	}

	// e->call はフレームワークが現状セットしないので使わない。e->action の SDL イベント値で判定。
	// DoubleClick: SDL の clicks 値が MouseEvent に伝播していないため、現状単独イベントとして
	// 検出する手段がない。当面はシングルクリック扱い (= isDoubleClick = false)。本来は
	// MouseEvent に clicks フィールドを足して main.cpp で event.button.clicks をコピーする等の
	// 拡張が必要。
	bool isMouseDown = (e->action == SDL_EVENT_MOUSE_BUTTON_DOWN);
	bool isMouseMove = (e->action == SDL_EVENT_MOUSE_MOTION);
	bool isDoubleClick = false;

	bool clickingEditCell = (rowLI && colLI) &&
	                        rowLI == tableEditRow(t) && colLI == tableEditCol(t);

	// === editCell の解除 (= MouseDown 系で edit cell 以外をクリックした場合) ===
	if (isMouseDown && !clickingEditCell && !isDoubleClick) {
		t->editRowPath = NULL;
		t->editColPath = NULL;
	}

	// === マウスハンドラ発火 ===
	// hit した cell について、3 種のハンドラを並行して発火 (NULL は no-op):
	//   - NewTable::onCellClick     : table 全体共通 (= どの cell でも発火)
	//   - ColumnInfo::onClick       : この列の cell でのみ
	//   - RowInfo::onClick          : この行の cell でのみ
	// 統一シグネチャ TableCellHandler(thgc, t, i, j, row, column, cell, e)。
	// MouseDown / MouseMove / MouseUp 等の区別は `e->action` を見て分岐する。
	if (rowLI && colLI) {
		// rowi/col は visible-DFS seq を渡す (= 互換用、tree-aware)。
		if (t->onCellClick)        t->onCellClick(thgc, local, t, rowSeq, colSeq, rowLI, colLI, cell, e);
		if (colLI->onClick)        colLI->onClick(thgc, local, t, rowSeq, colSeq, rowLI, colLI, cell, e);
		if (rowLI->onClick)        rowLI->onClick(thgc, local, t, rowSeq, colSeq, rowLI, colLI, cell, e);
	}

	if (t->cellselect && !isDoubleClick && !clickingEditCell) {
		// === Excel ライク: 2D 矩形セル選択 (path-ベース anchor、seq cache) ===
		if (rowLI && colLI) {
			if (isMouseDown) {
				bool shift = (SDL_GetModState() & SDL_KMOD_SHIFT) != 0;
				if (shift && t->selAnchorRow && t->selAnchorCol) {
					// extend: anchor 維持、extent のみ List* 差し替え
					t->selExtentRow = hitRowPath;
					t->selExtentCol = hitColPath;
				} else {
					// set: anchor = extent = hit (= List* assign)
					t->selAnchorRow = hitRowPath;
					t->selAnchorCol = hitColPath;
					t->selExtentRow = hitRowPath;
					t->selExtentCol = hitColPath;
				}
				local->select.from = local->select.to = self;
				local->select.fromid = local->select.toid = self->id;
				local->select.m = local->select.n = 0;
				Offscreen* seloff = FindOffscreen(self);
				local->select.window = seloff ? seloff->window : nullptr;
			}
			else if (isMouseMove) {
				Uint32 mb = SDL_GetMouseState(NULL, NULL);
				if (mb & SDL_BUTTON_LMASK) {
					if (!t->selAnchorRow) {
						t->selAnchorRow = hitRowPath;
						t->selAnchorCol = hitColPath;
					}
					t->selExtentRow = hitRowPath;
					t->selExtentCol = hitColPath;
					local->select.to   = self;
					local->select.toid = self->id;
				}
			}
		}
		return 0;
	}

	// DoubleClick で edit cell を更新 (編集モード突入)
	if (isDoubleClick && rowLI && colLI) {
		t->editRowPath = hitRowPath;
		t->editColPath = hitColPath;
	}

	// === cellselect = false または DoubleClick: cell に event 転送 ===
	//   pos に「cell まで」の位置を累積して渡す:
	//     = 親累積 (pos)
	//     + self (Table) の pos + margin + border + padding
	//     + tableAbsX/Y (= i, j に起因する cell の table 内位置)
	//     - scroll (freeze 外の cell のみ)
	if (cell && cell->Mouse) {
		// cell->pos2 は描画時に書き戻された table-local pos (= tableAbsX/Y の walk 不要)。
		PointF cellPos;
		cellPos.x = pos.x + self->pos.x + self->margins[3] + self->boWidthes[3] + self->paddings[3]
		          + cell->pos2.x;
		cellPos.y = pos.y + self->pos.y + self->margins[0] + self->boWidthes[0] + self->paddings[0]
		          + cell->pos2.y;
		// freeze 判定は path-ベース (= rowLI/colLI が freeze range 内か)
		if (!isInFreezeRange(t->columns, colLI, t->freezeColCount)) cellPos.x -= self->scroll.x;
		if (!isInFreezeRange(t->rows,    rowLI, t->freezeRowCount)) cellPos.y -= self->scroll.y;
		return cell->Mouse(thgc, cell, e, cellPos, local);
	}
	return 0;
}

// 空テーブルを作って初期化。
//   columns / rows は addTableColumn / addTableRow で動的に追加していく。
//   セルは setTableCell でぶっこむ。freeze は setTableFreeze(t, rows, cols) で設定。
inline void initTable(ThreadGC* thgc, NewTable* t) {
	t->gc            = thgc;
	t->type          = LetterType::_TableElem;
	t->next          = t->before = t;
	t->childend      = NULL;
	t->Measure       = NewTableMeasure;
	t->Draw          = NewTableDraw;
	t->Mouse         = NewTableMouse;
	t->Key           = NewTableKey;
	t->DrawSelection = TableDrawSelection;
	t->len = len1;
	t->offscreened   = true;   // 標準 Offscreen 機構に乗る (= elem->offscreen->imPing/imPong を framework が確保)
	t->freezeRowCount = 0;
	t->freezeColCount = 0;
	t->cellselect = false;      // デフォルト: セル選択モード ON (Excel ライク)
	// 選択状態の初期値: anchor/extent は NULL (= caller の List* を後で assign)。
	t->selAnchorRow = NULL;
	t->selAnchorCol = NULL;
	t->selExtentRow = NULL;
	t->selExtentCol = NULL;
	// 編集中 cell: なし (= 空 path)
	t->editRowPath = create_list(thgc, (int)sizeof(int), CType::_Struct);
	t->editColPath = create_list(thgc, (int)sizeof(int), CType::_Struct);
	// グリッド線: デフォルトで両軸 ON、薄いグレー
	t->showGridX  = true;
	t->showGridY  = true;
	t->gridColor  = 0xC0C0C0FF;
	// ツリービュー: デフォルト OFF。treeView=true で最初の非 header 列に展開 UI 表示。
	t->treeView      = false;
	t->treeIndent    = Scf(16.0f);
	t->treeIconSize  = Scf(9.0f);
	t->treeLineColor = 0x3070C0FF;  // 青系 (+/- 線 / 接続線 の色)

	// columns / rows はポインタリスト。要素は GC_alloc された ColumnInfo* / RowInfo*。
	//   list->type を _ColumnInfo / _RowInfo にすることで GC が per-entry に GC_clone する。
	t->columns = create_list(thgc, (int)sizeof(char*), CType::_ColumnInfo);
	t->rows    = create_list(thgc, (int)sizeof(char*), CType::_RowInfo);
	// (rowId, colId) packed → NewElement* の Map。value 側は NewElement なので type=_ElementC。
	t->cells    = create_mapy(thgc, CType::_ElementC);
	// id → ColumnInfo* / RowInfo* の Map。value は GC オブジェクトなので per-entry walk のため type を指定。
	t->rowIdMap = create_mapy(thgc, CType::_RowInfo);
	t->colIdMap = create_mapy(thgc, CType::_ColumnInfo);
	t->nextRowId = 0;
	t->nextColId = 0;
	// 自動追加時のデフォルトサイズ。後から `t->defaultColWidth = ...;` で上書き可。
	t->defaultColWidth  = Scf(80.0f);
	t->defaultRowHeight = Scf(24.0f);
	// ヘッダ用テキスト / 背景色 (= ColumnInfo::showLineNumbers / RowInfo::showColumnLetters の列・行で使われる)
	t->headerTextColor = 0x808080FF;
	t->headerBgColor   = 0xF0F0F0FF;
	// ヘッダ用 String* キャッシュ (ポインタリスト、要素は GC 管理 String*)。
	t->lineNumberCache   = create_list(thgc, (int)sizeof(char*), CType::_String);
	t->columnLetterCache = create_list(thgc, (int)sizeof(char*), CType::_String);
	// 次回 measure 対象の cell list (offscreen 持たない cell のみ登録)
	t->dirtyCells        = create_list(thgc, (int)sizeof(char*), CType::_ElementC);
	// table 全体の cell クリックハンドラ (= 未設定)。
	t->onCellClick = NULL;
	t->onExpand    = NULL;
	t->childend = (NewElement*)GC_alloc(thgc, CType::_EndC);
	initNewEndElement(thgc, (NewEndElement*)t->childend, t);
	t->offscreen = (Offscreen*)GC_alloc(thgc, CType::_Offscreen);
	t->offscreen->group = &createGroup(thgc);
	t->offscreen->elem = t;
	t->offscreen->type = OffscreenType::Off;
	t->offscreen->curlProgress = -1.0f;
	t->offscreen->imPing = queueOffscreenNew(thgc, 1, 1);
	t->offscreen->imPong = queueOffscreenNew(thgc, 1, 1);
	t->offscreen->next = t->offscreen->before = t->offscreen;
	t->offscreen->child = (Offscreen*)GC_alloc(thgc, CType::_OffscreenEnd);
	t->offscreen->child->next = t->offscreen->child->before = t->offscreen->child;
	t->offscreen->child->parent = t->offscreen;
	t->offscreened = true;
	t->background = (Background*)GC_alloc(thgc, CType::_Background);
	t->background->fillcolor = 0xffffffff;
	t->background->bs.borderColor = 0x000000FF;
	t->boWidthes[0] = t->boWidthes[1] = t->boWidthes[2] = t->boWidthes[3] = 0.0f;
	t->background->bs.shadowBlur = 1.0f;
	t->background->bs.shadowColor = 0x00000000;
	t->background->type = DrawCommandType::Fill;
}

// ============================================================
// ScrollTo: 任意の NewElement に対して scroll を target 位置にセットする汎用関数。
// Page モードなら自動でページ境界にスナップ (= curl/page アニメーションと整合)。
// Scroll モードなら任意 px。設定後 offscreen の markLayout/markPaint も発火。
//
// 使い方:
//   ScrollTo(local, elem, 0, 1234.5f);    // y=1234.5 px (Scroll) / 近いページ (Page)
//   ScrollToLine(local, tb, 207);         // 行 207 を中央付近に
// ============================================================
inline void ScrollTo(NewLocal* local, NewElement* elem, float x, float y) {
	if (!elem) return;
	float viewW = elem->size.x;
	float viewH = elem->size.y;

	// === X 軸 ===
	bool xIsPage = (baseType(elem->xtype) == SizeType::Page) && (viewW > 0.0f);
	int  xTargetPage = -1;
	if (xIsPage) {
		int page = (int)std::round(x / viewW);
		if (page < 0) page = 0;
		xTargetPage = page;
	} else {
		if (x < 0.0f) x = 0.0f;
		elem->scroll.x = x;
	}

	// === Y 軸 ===
	bool yIsPage = (baseType(elem->ytype) == SizeType::Page) && (viewH > 0.0f);
	int  yTargetPage = -1;
	if (yIsPage) {
		int page = (int)std::round(y / viewH);
		if (page < 0) page = 0;
		yTargetPage = page;
	} else {
		if (y < 0.0f) y = 0.0f;
		elem->scroll.y = y;
	}

	// === Page モードの場合、curl アニメ 1 回起動 (= fromPage→toPage が N 離れて
	// いても shader が両端の UV だけサンプルするため、1 回で遷移完了) ===
	// curl は 1 軸のみ対応 (= X / Y 同時 Page は無い前提)。Y 優先。
	if (elem->offscreen) {
		int axis = -1;          // 0 = Y, 1 = X
		int currentPage = 0;
		int targetPage  = 0;
		float pageSize  = 0.0f;
		if (yIsPage) {
			axis = 0;
			pageSize = viewH;
			currentPage = (int)std::round(elem->scroll.y / viewH);
			targetPage  = yTargetPage;
		} else if (xIsPage) {
			axis = 1;
			pageSize = viewW;
			currentPage = (int)std::round(elem->scroll.x / viewW);
			targetPage  = xTargetPage;
		}
		if (axis >= 0 && targetPage != currentPage) {
			// 1 回の curl で from→to を遷移 (= 中間ページは描画されない)
			elem->offscreen->curlFromPage = (float)currentPage;
			elem->offscreen->curlToPage   = (float)targetPage;
			elem->offscreen->curlProgress = 0.0f;
			elem->offscreen->curlAxis     = axis;
		} else if (axis >= 0) {
			// 既に target ページにいるので scroll をスナップだけ
			if (axis == 0) elem->scroll.y = targetPage * pageSize;
			else           elem->scroll.x = targetPage * pageSize;
		}
	}

	// 再描画要求
	Offscreen* off = FindOffscreen(elem);
	if (off) {
		off->markLayout(local, elem);
		off->markPaint(local);
	}
}

// テーブルの「行 rowIdx (0-indexed) が viewport 中央付近」になるよう scroll 設定。
// rowIdx は t->rows 上の絶対位置 (= 行番号 - 1)。
inline void ScrollToLine(NewLocal* local, NewTable* tb, int rowIdx) {
	if (!tb || !tb->rows) return;
	if (rowIdx < 0 || rowIdx >= tb->rows->size) return;
	// 累積で目的行の y 位置を計算
	float targetY = 0.0f;
	for (int i = 0; i < rowIdx; i++) {
		LineInfo* r = (LineInfo*)*get_list2(tb->rows, i);
		if (r) targetY += r->size;
	}
	// viewport 中央付近に来るよう調整
	float scrollY = targetY - tb->size.y * 0.5f;
	if (scrollY < 0.0f) scrollY = 0.0f;
	ScrollTo(local, (NewElement*)tb, tb->scroll.x, scrollY);
}

// ============================================================
// scrollSourceToFrame: thgc->map から path → viewer を引いて、指定行を
// viewport 中央付近にくるよう scroll.y を設定。
//
// 入力 file は basename だけのケースもある (= driver の SBLineEntry::GetFileSpec()
// が basename を返す) ので、map で完全一致しない場合は basename suffix で
// 一致する path を線形検索する。line は 1-indexed。
// ============================================================
inline NewElement* findViewerByPathOrBasename(ThreadGC* thgc, const char* file) {
	if (!thgc || !thgc->map || !file || !*file) return NULL;
	int flen = (int)strlen(file);
	// (1) 完全一致 (= path 全体が一致)
	{
		String* key = createString(thgc, (char*)file, flen, 1);
		NewElement* v = (NewElement*)get_mapy(thgc->map, key);
		if (v && GC_typeOf(v) == _NewTable) return v;
	}
	// (2) basename suffix 一致 (= map を線形走査して末尾比較)
	//     thgc->map の全エントリを iterate するヘルパが無いので、kvs を直接走査。
	if (!thgc->map->kvs) return NULL;
	for (int i = 0; i < thgc->map->kvs->size; i++) {
		KV* kv = *(KV**)get_list(thgc->map->kvs, i);
		if (!kv || !kv->value) continue;
		NewElement* v = (NewElement*)kv->value;
		if (GC_typeOf(v) != _NewTable) continue;
		// kv->key は String*。末尾が file と一致するか確認
		String* k = (String*)kv->key;
		if (!k || k->size < flen) continue;
		const char* ks = k->data;
		// path 末尾 flen バイト = file
		if (memcmp(ks + (k->size - flen), file, flen) == 0) {
			// 直前が path 区切りか先頭であることを確認 (= "main.cpp" が "submain.cpp" と
			// 誤マッチしないように)
			if (k->size == flen ||
				ks[k->size - flen - 1] == '/' || ks[k->size - flen - 1] == '\\') {
				return v;
			}
		}
	}
	return NULL;
}

inline void scrollSourceToFrame(ThreadGC* thgc, NewLocal* local,
	const char* file, uint32_t line)
{
	NewElement* viewer = findViewerByPathOrBasename(thgc, file);
	if (!viewer) {
		fprintf(stderr, "[scroll] viewer not found for file=%s\n", file); fflush(stderr);
		return;
	}
	NewTable* tb = (NewTable*)viewer;
	if (!tb->rows || tb->rows->size == 0 || line == 0) return;
	int rowIdx = (int)line - 1;
	if (rowIdx < 0 || rowIdx >= tb->rows->size) return;
	fprintf(stderr, "[scroll] file=%s line=%u rowIdx=%d size.y=%.1f\n",
		file, line, rowIdx, tb->size.y); fflush(stderr);
	ScrollToLine(local, tb, rowIdx);
}

// ============================================================
// paintDebugStopLine: デバッグ停止行を黄色でハイライト
//
// LLDBClient.highlightCell に「現在 yellow にしている cell」を保存し、
// 新しい停止位置に切り替わるたびに前回 cell の背景色を元に戻す + 新 cell を yellow に。
// 黄色は半透明寄り (= 0xFFEE88C0) で、上の文字が透けて見える程度に。
// line=0 なら前回ハイライト解除のみ実施 (= debugger continue/exit 時)。
// ============================================================
inline void paintDebugStopLine(ThreadGC* thgc, NewLocal* local,
	NewElement* viewer, uint32_t line)
{
	LLDBClient* cli = getLLDBClient(thgc);

	// === 前回の yellow cell を元に戻す ===
	if (cli->highlightCell) {
		NewElement* prev = cli->highlightCell;
		if (prev->background) {
			if (cli->highlightHadBackground) {
				prev->background->fillcolor = cli->highlightOldFill;
			} else {
				// 元々 background が無かった → background ポインタごと削除
				prev->background = NULL;
			}
		}
		// 旧 viewer も再描画フラグを立てる (= prev のオフスクリーン)
		Offscreen* prevOff = FindOffscreen(prev);
		if (prevOff) prevOff->markPaint(local);
	}
	cli->highlightCell = NULL;

	// === line == 0 (= clear-only) なら新規ハイライトはしない ===
	if (line == 0 || !viewer) { return; }
	if (GC_typeOf(viewer) != _NewTable) { return; }
	NewTable* tb = (NewTable*)viewer;
	if (!tb->rows) { return; }
	int rowIdx = (int)line - 1;
	if (rowIdx < 0 || rowIdx >= tb->rows->size) { return; }

	// === col 1 (= content) cell を取得して黄色背景を被せる ===
	NewElement* cell = getTableCell(tb, rowIdx, 1);
	if (!cell) { return; }
	if (!cell->background) {
		cell->background = (Background*)GC_alloc(thgc, CType::_Background);
		cell->background->type = DrawCommandType::Fill;
		cli->highlightHadBackground = 0;
		cli->highlightOldFill       = 0x00000000;
	} else {
		cli->highlightHadBackground = 1;
		cli->highlightOldFill       = cell->background->fillcolor;
	}
	cell->background->fillcolor = 0xFFEE88C0;   // 黄色 (半透明寄り)
	cli->highlightCell = cell;

	if (FindOffscreen(tb)) FindOffscreen(tb)->markPaint(local);
}

// ============================================================
// 復元: viewer (= NewTable / source code) の path に対する BP を
// LLDBClient.breakpoints から拾って、ガター列 (= column 0) に赤丸セルを生成。
//
// 呼び出しタイミング:
//   * OpenFile 完了後 (= 新規開いたファイル、まだ赤丸無し)
//   * state::replay の openCb 完了後 (= 前回終了時の状態を復元)
//
// 既存セルがあれば fillcolor だけ赤に上書き。無ければ新規 cell を作る。
// ============================================================
inline void paintBreakpointsForFile(ThreadGC* thgc, NewLocal* local,
	NewElement* viewer, const char* path)
{
	if (!viewer || GC_typeOf(viewer) != _NewTable) return;
	NewTable* tb = (NewTable*)viewer;
	if (!tb->rows || tb->rows->size == 0) { return; }
	LLDBClient* cli = getLLDBClient(thgc);
	if (!cli || !cli->breakpoints) return;
	int pathLen = (int)strlen(path);
	for (int k = 0; k < cli->breakpoints->size; k++) {
		BpEntry* bp = *(BpEntry**)get_list(cli->breakpoints, k);
		if (!bp || !bp->file || !bp->enabled) continue;
		if (bp->file->size != pathLen) continue;
		if (memcmp(bp->file->data, path, pathLen) != 0) continue;
		// bp->line は 1-indexed (= line number)、行 index は line - 1
		int rowIdx = (int)bp->line - 1;
		if (rowIdx < 0 || rowIdx >= tb->rows->size) continue;
		// 既存ガターセルがあれば色だけ更新、無ければ新規作成
		NewElement* cell = getTableCell(tb, rowIdx, 0);
		if (cell && cell->background) {
			cell->background->fillcolor = 0xFF0000FF;
		} else {
			NewElement* elem = (NewElement*)GC_alloc(thgc, CType::_ElementC);
			initElement(thgc, elem);
			NewNext(thgc, local, tb->childend, elem);
			setTableCell(thgc, tb, rowIdx, 0, elem);
			elem->background = (Background*)GC_alloc(thgc, CType::_Background);
			elem->background->fillcolor = 0xFF0000FF;
			elem->background->bs.borderColor = 0x000000FF;
			elem->boWidthes[0] = elem->boWidthes[1] =
			elem->boWidthes[2] = elem->boWidthes[3] = 0.0f;
			elem->background->bs.shadowBlur = 1.0f;
			elem->background->bs.shadowColor = 0x00000000;
			elem->background->type = DrawCommandType::Fill;
		}
	}
	if (FindOffscreen(tb)) FindOffscreen(tb)->markPaint(local);
}

void breakpoint(ThreadGC* thgc, NewLocal* local, NewTable* tb,
	int i, int j, RowInfo* row, ColumnInfo* column, NewElement* cell,
	MouseEvent* e)
{
	if (e->action != SDL_EVENT_MOUSE_BUTTON_DOWN) return;
	// トグル後に「BP が有効か」を表す。下のセル操作で更新する。
	bool enabled = false;
	if (cell == NULL) {
		// 新規セル作成 = 赤丸表示 = BP 有効化
		NewElement* elem = (NewElement*)GC_alloc(thgc, CType::_ElementC);
		initElement(thgc, elem);
		NewNext(thgc, local, tb->childend, elem);
		setTableCell(thgc, tb, i, j, elem);
		elem->background = (Background*)GC_alloc(thgc, CType::_Background);
		elem->background->fillcolor = 0xFF0000FF;
		elem->background->bs.borderColor = 0x000000FF;
		elem->boWidthes[0] = elem->boWidthes[1] = elem->boWidthes[2] = elem->boWidthes[3] = 0.0f;
		elem->background->bs.shadowBlur = 1.0f;
		elem->background->bs.shadowColor = 0x00000000;
		elem->background->type = DrawCommandType::Fill;
		enabled = true;
	}
	else {
		// 既存セルの色トグル: 透明 → 赤 (= 有効化) / 赤 → 透明 (= 無効化)
		if (cell->background->fillcolor == 0x00000000) {
			cell->background->fillcolor = 0xFF0000FF;
			enabled = true;
		}
		else {
			cell->background->fillcolor = 0x00000000;
			enabled = false;
		}
	}
	FindOffscreen(tb)->markPaint(local);

	// LLDBClient は SetRichText 時点で thgc->map["debug"] に登録済み (= 型チェックも
	// getLLDBClient 内で実施)。ファイルパスは tb->values[FILE_PATH_SLOT] から取得。
	if (tb->values && tb->values->n > FILE_PATH_SLOT &&
		SIVal_type(tb->values->values[FILE_PATH_SLOT]) == SIType::String)
	{
		SIVal pv = getSIValue(tb->values, FILE_PATH_SLOT, SIType::String);
		String* path = pv.str;
		if (path && path->size > 0) {
			int flen = path->size;
			char fileBuf[1024];
			if (flen >= (int)sizeof(fileBuf)) flen = (int)sizeof(fileBuf) - 1;
			memcpy(fileBuf, path->data, flen);
			fileBuf[flen] = '\0';
			uint32_t lineNo = (uint32_t)(i + 1);
			fprintf(stderr, "[LLDB] BP %s: %s:%u\n",
				enabled ? "ADD" : "REMOVE", fileBuf, lineNo);
			lldb_setBreakpoint(thgc, getLLDBClient(thgc), fileBuf, lineNo, enabled);
			// 永続化: 次回起動時に同じ BP を自動復元するため SQLite に記録
			state::record_bp(thgc, std::string(fileBuf, flen), lineNo, enabled);
		}
	}
}
void SetRichText(ThreadGC* thgc, NewLocal* local, NewTable* tb, String* text) {
	// ソースコード viewer 化のタイミングで LLDBClient を thgc->map["debug"] に
	// 先行登録しておく。これで breakpoint() ハンドラ (= ガター click) が常に
	// 有効な client に BP を反映できる (= driver 未起動でもリストに蓄積)。
	getLLDBClient(thgc);

	tb->childend->id = UINT64_MAX;
	tb->childend2 = (NewElement*)GC_alloc(thgc, CType::_ElementC);
	initEndBodyElement(thgc, tb->childend2);
	NewDirectAddLast(thgc, local, tb, tb->childend2);
	addTableColumn(thgc, tb, 48);
	tableCol(tb, 0)->showLines = true;
	List* list = Compile(thgc, text, tb->font, NULL);
	// Compile はスパンを **絶対座標** で返すように変更したので、
	// 行頭の絶対位置 = 最初のスパンの start。SubString も絶対 start から取り出すだけ。
	for (int i = 0; i < list->size; i++) {
		NewLine* line = (NewLine*)GC_alloc(thgc, CType::_LineC);
		initLine(thgc, line);
		setTableCell(thgc, tb, i, 1, line);
		NewDirectAddLast(thgc, local, tb, line);
		List* list2 = *(List**)get_list(list, i);
		if (list2->size > 0) {
			NewLetter* let = (NewLetter*)GC_alloc(thgc, CType::_LetterC);
			initLetter(thgc, let, tb->font, LetterType::_Letter);
			let->recompile = false;
			ATSSpan* first = *(ATSSpan**)get_list(list2, 0);
			ATSSpan* last = *(ATSSpan**)get_list(list2, list2->size - 1);
			int lineStart = first->start;            // 絶対 = この行の先頭
			int lineLen = last->end - lineStart;   // 絶対終端 - 行頭
			let->text = SubString(thgc, text, lineStart, lineLen);
			let->atsspans = list2;
			// スパンを letter-相対化 (一括で行頭オフセットを引く)
			for (int j = 0; j < list2->size; j++) {
				ATSSpan* a = *(ATSSpan**)get_list(list2, j);
				a->start -= lineStart;
				a->end -= lineStart;
				a->letter = let;
			}
			NewNext(thgc, local, line->childend, let);
		}
		// 空行 (list2.size == 0) は letter を作らない。NewLine だけで段組される。
	}
	tableCol(tb, 1)->mode = TableLineMode::Auto;
	auto c = tableCol(tb, 0);
	c->mode = TableLineMode::Manual;
	c->showLines = true;
	c->width = 48;
	c->onClick = breakpoint;
}
// ============================================================
// View 化ヘルパ群 (encode.h の To* で得たデータを NewElement に貼る)
// ============================================================
inline NewElement* MakeRichTextBox(ThreadGC* thgc, String* text) {
	NewTable* tb = (NewTable*)GC_alloc(thgc, CType::_NewTable);
	initTable(thgc, tb);
	tb->offscreen->markLayout(thgc->local, tb);
	tb->offscreen->curlProgress = -1.0f;
	NativeWindow* mainWin = (!thgc->windows.empty()) ? thgc->windows[0] : nullptr;
	NativeWindow* linkWin = new NativeWindow();
	linkWin->sdlWindow = mainWin->sdlWindow;
	linkWin->size = mainWin->size;
	linkWin->viewId = mainWin->viewId;
	linkWin->type = WindowType_Main;
	linkWin->nwh = mainWin->nwh;
	linkWin->local = tb;
	tb->offscreen->window = linkWin;
	thgc->linkwins.push_back(linkWin);
	// linkWin の GC ポインタ field を root 登録
	linkWin->rootNode = GC_add_root_node(thgc);
	GC_add_root(linkWin->rootNode, (char**)&linkWin->local);
	GC_add_root(linkWin->rootNode, (char**)&linkWin->anchorElement);
	/*std::vector<float> colors = std::vector<float>{
		0.865f, 0.865f, 0.500f, 1.0f,  // 上: #9090CC
		0.439f, 0.639f, 0.667f, 1.0f   // 下: #7070AA};
	};
	std::vector<float> widths = std::vector<float>{ 200, 200 };
	int n = addPattern(thgc, colors, widths);
	tb->background->offset = n;
	int size = widths.size();
	tb->background->count = size;
	tb->background->type = DrawCommandType::Gradient;
	for (int i = 0; i < 4; i++) tb->background->bs.boRadiuses[i] = 12;
	for (int i = 0; i < 4; i++) tb->boWidthes[i] = 1;
	tb->background->bs.borderColor = 0x000000FF;
	tb->background->bs.shadowColor = 0x80808000;
	tb->background->bs.shadowBlur = 1.5f;
	tb->background->bs.shadowX = 3.0f;
	tb->background->bs.shadowY = 3.0f;
	tb->background->cornerPattern = 0;
	tb->background->backUVMinX = 10.0;
	tb->background->fillcolor = 0xffffffff;*/
	tb->zIndex = 0.0;
	tb->font = getFont("sans", 16);
	// X / Y とも Scroll モード。Page モードはページめくりアニメが要るとき復活させる方針。
	setPercentX(tb, 1.0f, 0, SizeType::Scroll);
	setPercentY(tb, 1.0f, 0, SizeType::Page);
	SDL_Log("[MakeRichTextBox] before SetRichText tb=%p text=%p text->size=%d tb->offscreen=%p tb->offscreen->window=%p linkWin=%p",
	        (void*)tb, (void*)text, text?text->size:-1,
	        (void*)tb->offscreen, (void*)(tb->offscreen?tb->offscreen->window:nullptr), (void*)linkWin);
	if (text) SetRichText(thgc, thgc->local, tb, text);
	// 行数カウント (childend からたどる)
	int nChildren = 0;
	if (tb->childend) {
		NewElement* it = tb->childend->next;
		while (it && it != tb->childend && nChildren < 100000) { nChildren++; it = it->next; }
	}
	SDL_Log("[MakeRichTextBox] after SetRichText tb=%p children=%d childend=%p tb->offscreened=%d",
	        (void*)tb, nChildren, (void*)tb->childend, (int)tb->offscreened);
	return (NewElement*)tb;
}
// ============================================================
// OpenFile: ボタンクリック → ダイアログ → 非同期読込 → kind 判定 → viewer
//
// 流れ (3 段階の非同期):
//   (1) UI thread: SDL_ShowOpenFileDialog を呼んで即 return
//   (2) OS dialog thread: 選択結果 → SDL_PushEvent(SDL_EVENT_DIALOG_RESULT)
//   (3) UI thread: DispatchDialogEvent → ここの第1 lambda
//   (4) UI thread: LoadFileAsync(path, ...) で FileEngine worker にジョブ投入、即 return
//   (5) FileEngine worker: read 実行 → Stream 組立 → SDL_PushEvent(SDL_EVENT_FILE_LOADED)
//   (6) UI thread: DispatchFileLoadEvent → ここの第2 lambda
//   (7) UI thread: kind 判定 + viewer 作成 + AddViewerToTab
//
// → ダイアログ表示中もファイル読み込み中も UI は固まらない。
// ============================================================
void OpenFile(Frame* frame) {
	MemObj* mo = (MemObj*)frame->promise.blockobj;
	MouseEvent* act = (MouseEvent*)mo->action;
	if (act->action != SDL_EVENT_MOUSE_BUTTON_DOWN) return;
	ThreadGC* thgc = act->elem->gc;   // lambda 起動前に取得
	SDL_Log("[OpenFile] click thgc=%p tid=%llu", (void*)thgc,
	        (unsigned long long)std::hash<std::thread::id>{}(std::this_thread::get_id()));

	// (1)-(3): ダイアログ → main thread に戻ってきた時点でこの lambda が呼ばれる
	ShowOpenFileDialog(thgc, [](ThreadGC* thgc, const char* path) {
		SDL_Log("[OpenFile.cb1] dialog result thgc=%p path=%s tid=%llu", (void*)thgc, path,
		        (unsigned long long)std::hash<std::thread::id>{}(std::this_thread::get_id()));
		// path は SDL_strdup された一時バッファ。lambda の外まで生かしたいので std::string へ。
		std::string pathStr(path);

		// (4)-(6): FileEngine worker で読み込み、main thread に戻ってきたらこの lambda
		LoadFileAsync(thgc, pathStr, [pathStr](ThreadGC* thgc, Stream* s) {
			SDL_Log("[OpenFile.cb2] load done thgc=%p s=%p tid=%llu", (void*)thgc, (void*)s,
			        (unsigned long long)std::hash<std::thread::id>{}(std::this_thread::get_id()));
			// (7): main thread, viewer 作成
			if (!s) return;

			// ★ デバッグ: 開いたファイルの最初の数バイト + 検出 kind をログ。
			//   content:// URI 経由で別ファイルが返ってきてないか、kind 検出が
			//   おかしくなってないかを確認するため。
			{
				const uint8_t* d = s->data;
				size_t n = s->size;
				char hex[64] = {0};
				size_t hn = n < 16 ? n : 16;
				for (size_t i = 0; i < hn; i++) {
					snprintf(hex + i*3, 4, "%02x ", (unsigned)d[i]);
				}
				FileKind kBytes = detectFileKindFromBytes(d, n);
				FileKind kExt = detectFileKindFromExt(pathStr.c_str());
				SDL_Log("[OpenFile] path=%s size=%zu first16=%s kExt=%d kBytes=%d",
				        pathStr.c_str(), n, hex, (int)kExt, (int)kBytes);
			}
			NewElement* viewer = nullptr;
			switch (detectFileKind(pathStr.c_str(), s)) {
			case FileKind::Text:
			case FileKind::SourceCode: {
				viewer = MakeRichTextBox(thgc, ToText(thgc, s, Encoding::Auto));
				break;
			}
			case FileKind::Image: {
				viewer = MakeImageView(thgc, ToImage(thgc, s));
				break;
			}
			case FileKind::Audio: {
				viewer = MakeAudioPlayer(thgc, ToSound(thgc, s));
				break;
			}
			case FileKind::Video: {
				viewer = MakeVideoPlayer(thgc, ToVideoSound(thgc, s));
				break;
			}
			default: {
				viewer = MakeTextBox(thgc, ToHex(thgc, s));
				break;
			}
			}
			if (!viewer) { return; }

			// ファイルパスを viewer->values[FILE_PATH_SLOT] に格納 (SIType::String)。
			// breakpoint() ハンドラ等が getSIValue で取り出して BP の file path に使う。
			ensureSIValues(thgc, viewer);
			SIVal pv;
			String* pathStr2 = createString(thgc, (char*)pathStr.c_str(),
				(int)pathStr.size(), 1);
			pv.str = pathStr2;
			// setSIValue は bit 操作のみ → pathStr2 は viewer->values 経由で reachable (viewer rooted)。
			// add_mapy は内部で key を root するので move にも追従。caller 側 GC_ROOT 不要。
			setSIValue(viewer->values, FILE_PATH_SLOT, SIType::String, pv);
			// thgc->map にも path → viewer の reverse 登録 (= スタックフレームで
			// 「このファイルの viewer」を引くため)。値は viewer ポインタ。
			add_mapy(thgc, thgc->map, pathStr2, (char*)viewer);

			// 永続化: 次回起動時に同じファイルを自動で開き直すため SQLite に記録
			state::record_open(thgc, pathStr);

			// 既存 BP リストから、このファイルに該当する BP のガター赤丸を復元
			paintBreakpointsForFile(thgc, thgc->local, viewer, pathStr.c_str());

			// タブ名 = ファイル表示名
			std::string displayName;
#ifdef __ANDROID__
			// SAF (content://) URI のときは ContentResolver から DISPLAY_NAME を取り出す。
			// 取れたらそれをタブ名に使う。失敗時はパス末尾フォールバック。
			if (pathStr.rfind("content://", 0) == 0) {
				displayName = androidDisplayNameForUri(pathStr);
			}
#endif
			if (displayName.empty()) {
				const char* path = pathStr.c_str();
				const char* basename = strrchr(path, '/');
				if (!basename) basename = strrchr(path, '\\');
				basename = basename ? basename + 1 : path;
				displayName = basename;
			}
			AddViewerToTab(thgc, viewer, displayName.c_str());
			});
		});
}


void ContinueClick(Frame* frame) {
	MemObj* mo = (MemObj*)frame->promise.blockobj;
	MouseEvent* act = (MouseEvent*)mo->action;
	if (act->action != SDL_EVENT_MOUSE_BUTTON_DOWN) return;
	ThreadGC* thgc = act->elem->gc;
	LLDBClient* cli = getLLDBClient(thgc);
	if (lldb_isPaused(cli)) {
		// 黄色ハイライト解除 (= デバッグ再開で「現在停止行」が無くなる)
		paintDebugStopLine(thgc, thgc->local, NULL, 0);
		lldb_resume(cli);
	}
}
void StepInClick (Frame* frame) {
	MemObj* mo = (MemObj*)frame->promise.blockobj;
	MouseEvent* act = (MouseEvent*)mo->action;
	if (act->action != SDL_EVENT_MOUSE_BUTTON_DOWN) return;
	ThreadGC* thgc = act->elem->gc;
	LLDBClient* cli = getLLDBClient(thgc);
	if (lldb_isPaused(cli)) {
		paintDebugStopLine(thgc, thgc->local, NULL, 0);
		lldb_stepIn(cli);
	}
}
void StepOverClick(Frame* frame) {
	MemObj* mo = (MemObj*)frame->promise.blockobj;
	MouseEvent* act = (MouseEvent*)mo->action;
	if (act->action != SDL_EVENT_MOUSE_BUTTON_DOWN) return;
	ThreadGC* thgc = act->elem->gc;
	LLDBClient* cli = getLLDBClient(thgc);
	if (lldb_isPaused(cli)) {
		paintDebugStopLine(thgc, thgc->local, NULL, 0);
		lldb_stepOver(cli);
	}
}
void StepOutClick(Frame* frame) {
	MemObj* mo = (MemObj*)frame->promise.blockobj;
	MouseEvent* act = (MouseEvent*)mo->action;
	if (act->action != SDL_EVENT_MOUSE_BUTTON_DOWN) return;
	ThreadGC* thgc = act->elem->gc;
	LLDBClient* cli = getLLDBClient(thgc);
	if (lldb_isPaused(cli)) {
		paintDebugStopLine(thgc, thgc->local, NULL, 0);
		lldb_stepOut(cli);
	}
}