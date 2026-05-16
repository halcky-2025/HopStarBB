#pragma once
// ============================================================
// state.h - UI 状態 (= 開いたファイル / BP) を SQLite に永続化
//
// 既存の db2.h::SqlPool / Generator / execf を再利用 (= 自前で worker thread を
// 作らない)。SqlPool 内の worker thread が SQL 実行 → 完了で thgc->queue に
// handle を push → UI thread の resume_all() が body を再開する流れ。
//
// 使い方:
//   state::init(thgc, dbPath);                        // 起動時 1 回
//   state::record_open(thgc, path);                   // OpenFile の中
//   state::record_bp  (thgc, file, line, enabled);    // breakpoint() の中
//   state::replay     (thgc, openCb, bpCb);           // extorch 起動時
// ============================================================

#include "db2.h"
#include <SDL3/SDL_log.h>
#include "sqlite3.h"
#include <string>
#include <vector>
#include <tuple>
#include <functional>

namespace state {

// プロセス内の唯一の SqlPool (= state.db を 1 worker で開く)。
inline SqlPool* g_pool = nullptr;
// state.db のパスを覚えておいて replay() で再利用する。
inline std::string g_dbPath;

// ----------------------------------------------------------------
// 内部: 任意の Generator を thgc->queue に投入して非同期実行を始める
//   completed まで放置 (= UI thread の resume_all が逐次進める)。
// ----------------------------------------------------------------
inline void _spawn(ThreadGC* thgc, Generator g) {
	auto h = g.coro;
	g.coro = {};                              // ~Generator() で destroy しない
	h.promise().queue = thgc->queue;
	thgc->queue->push(h);
}

// ----------------------------------------------------------------
// 初期化: SqlPool 作成 → schema を非同期で投入
// ----------------------------------------------------------------
// schema creation: SqlPool より前に sqlite3_exec で直接行う。
// coroutine 経由で CREATE TABLE を流す方式は Android で resume が
// 戻ってこない症状を踏んだので回避。schema は冪等 (IF NOT EXISTS) で
// あれば毎回回しても無害。
inline void _createSchema(const char* dbPath) {
	sqlite3* db = nullptr;
	if (sqlite3_open_v2(dbPath, &db,
		SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK)
	{
		SDL_Log("[state] _createSchema: open failed: %s",
			db ? sqlite3_errmsg(db) : "(null)");
		if (db) sqlite3_close_v2(db);
		return;
	}
	const char* ddl =
		"CREATE TABLE IF NOT EXISTS opened_files("
		" path TEXT PRIMARY KEY,"
		" opened_at INTEGER"
		");"
		"CREATE TABLE IF NOT EXISTS breakpoints("
		" file TEXT NOT NULL,"
		" line INTEGER NOT NULL,"
		" enabled INTEGER NOT NULL,"
		" PRIMARY KEY(file, line)"
		");";
	char* errmsg = nullptr;
	if (sqlite3_exec(db, ddl, nullptr, nullptr, &errmsg) != SQLITE_OK) {
		SDL_Log("[state] _createSchema: exec failed: %s", errmsg ? errmsg : "(null)");
		if (errmsg) sqlite3_free(errmsg);
	} else {
		SDL_Log("[state] _createSchema: done");
	}
	sqlite3_close_v2(db);
}

inline void init(ThreadGC* thgc, const char* dbPath) {
	if (g_pool) return;
	// schema 作成は同期で済ませる (= worker thread / coroutine を介さない)。
	// 起動 1 回限り & IF NOT EXISTS なので冪等。SqlPool 立ち上げ前に DB の
	// 形を整えておく方が、後段の async access が単純になる。
	_createSchema(dbPath);
	g_dbPath = dbPath;
	g_pool = new SqlPool(std::string(dbPath), 1);   // worker 1 つで十分
	SDL_Log("[state] init: %s", dbPath);
}

// ----------------------------------------------------------------
// record_open: 開いたファイルパスを保存
// ----------------------------------------------------------------
inline Generator _recordOpen(ThreadGC* thgc, SqlPool* pool, CoroutineQueue* queue,
	std::string path)
{
	auto tx = (Transaction*)co_await begin_tx(thgc, pool, queue);
	SqlParam p{};
	p.kind = SQL_TEXT;
	p.data.text.ptr = (char*)path.c_str();
	p.data.text.len = (int)path.size();
	co_await execf(tx,
		"INSERT OR REPLACE INTO opened_files(path, opened_at) "
		"VALUES(?, strftime('%s','now'))",
		{ p }, NULL, NULL);
	co_await tx->commit();
	co_return (char*)0;
}

inline void record_open(ThreadGC* thgc, const std::string& path) {
	if (!g_pool) { SDL_Log("[state] record_open: g_pool=null, skip path=%s", path.c_str()); return; }
	SDL_Log("[state] record_open: path=%s", path.c_str());
	_spawn(thgc, _recordOpen(thgc, g_pool, thgc->queue, path));
}

// ----------------------------------------------------------------
// remove_open: 閉じたファイルを保存リストから削除
// ----------------------------------------------------------------
inline Generator _removeOpen(ThreadGC* thgc, SqlPool* pool, CoroutineQueue* queue,
	std::string path)
{
	auto tx = (Transaction*)co_await begin_tx(thgc, pool, queue);
	SqlParam p{};
	p.kind = SQL_TEXT;
	p.data.text.ptr = (char*)path.c_str();
	p.data.text.len = (int)path.size();
	co_await execf(tx,
		"DELETE FROM opened_files WHERE path = ?",
		{ p }, NULL, NULL);
	co_await tx->commit();
	co_return (char*)0;
}

inline void remove_open(ThreadGC* thgc, const std::string& path) {
	if (!g_pool) return;
	_spawn(thgc, _removeOpen(thgc, g_pool, thgc->queue, path));
}

// ----------------------------------------------------------------
// record_bp: BP の追加 (enabled=true) / 削除 (enabled=false) を保存
// ----------------------------------------------------------------
inline Generator _recordBp(ThreadGC* thgc, SqlPool* pool, CoroutineQueue* queue,
	std::string file, uint32_t line, bool enabled)
{
	auto tx = (Transaction*)co_await begin_tx(thgc, pool, queue);
	SqlParam pf{}, pl{};
	pf.kind = SQL_TEXT;
	pf.data.text.ptr = (char*)file.c_str();
	pf.data.text.len = (int)file.size();
	pl.kind = SQL_INT;
	pl.data.ival = (int)line;
	if (enabled) {
		co_await execf(tx,
			"INSERT OR REPLACE INTO breakpoints(file, line, enabled) "
			"VALUES(?, ?, 1)",
			{ pf, pl }, NULL, NULL);
	} else {
		co_await execf(tx,
			"DELETE FROM breakpoints WHERE file = ? AND line = ?",
			{ pf, pl }, NULL, NULL);
	}
	co_await tx->commit();
	co_return (char*)0;
}

inline void record_bp(ThreadGC* thgc, const std::string& file, uint32_t line, bool enabled) {
	if (!g_pool) return;
	_spawn(thgc, _recordBp(thgc, g_pool, thgc->queue, file, line, enabled));
}

// ----------------------------------------------------------------
// clear_breakpoints: SQLite の breakpoints テーブルを全件削除
//   いつのまにか入っていた stale BP を一掃する用。LLDBClient のメモリ上の
//   BP リストは別途 lldb_clearBreakpoints で消すこと。
// ----------------------------------------------------------------
inline Generator _clearBps(ThreadGC* thgc, SqlPool* pool, CoroutineQueue* queue) {
	auto tx = (Transaction*)co_await begin_tx(thgc, pool, queue);
	co_await execf(tx, "DELETE FROM breakpoints", {}, NULL, NULL);
	co_await tx->commit();
	co_return (char*)0;
}
inline void clear_breakpoints(ThreadGC* thgc) {
	if (!g_pool) return;
	_spawn(thgc, _clearBps(thgc, g_pool, thgc->queue));
}

// ----------------------------------------------------------------
// replay: opened_files / breakpoints を全件読み出して callback で再生
//
//   row callback (= execf の func) は worker thread 上で呼ばれる (thgc->mutex 保持中)。
//   ここでは「結果を heap struct に積むだけ」にして、co_await から帰ってきた
//   UI thread 側でユーザコールバックを呼ぶ。
// ----------------------------------------------------------------
struct _ReplayBuf {
	std::vector<std::string>                                 files;
	std::vector<std::tuple<std::string, uint32_t, bool>>     bps;
};

inline char* _setOpenedRow(ThreadGC* /*thgc*/, CoroutineQueue* /*queue*/,
	std::unique_lock<std::mutex>* /*lock*/, sqlite3_stmt* st, char* obj)
{
	auto* buf = (_ReplayBuf*)obj;
	const unsigned char* p = sqlite3_column_text(st, 0);
	if (p) buf->files.emplace_back((const char*)p);
	return obj;   // execf が rs->rows に追加するのでダミーで obj を返す
}

inline char* _setBpRow(ThreadGC* /*thgc*/, CoroutineQueue* /*queue*/,
	std::unique_lock<std::mutex>* /*lock*/, sqlite3_stmt* st, char* obj)
{
	auto* buf = (_ReplayBuf*)obj;
	const unsigned char* f = sqlite3_column_text(st, 0);
	int line = sqlite3_column_int(st, 1);
	int en   = sqlite3_column_int(st, 2);
	if (f) buf->bps.emplace_back((const char*)f, (uint32_t)line, en != 0);
	return obj;
}

inline Generator _replay(ThreadGC* thgc, SqlPool* pool, CoroutineQueue* queue,
	std::function<void(ThreadGC*, const char*)> openCb,
	std::function<void(ThreadGC*, const char*, uint32_t, bool)> bpCb)
{
	auto* buf = new _ReplayBuf();
	auto tx = (Transaction*)co_await begin_tx(thgc, pool, queue);
	co_await execf(tx,
		"SELECT path FROM opened_files ORDER BY opened_at",
		{}, _setOpenedRow, (char*)buf);
	co_await execf(tx,
		"SELECT file, line, enabled FROM breakpoints",
		{}, _setBpRow, (char*)buf);
	co_await tx->commit();
	SDL_Log("[state] replay: files=%zu bps=%zu", buf->files.size(), buf->bps.size());

	// === BP 復元 (= 軽い、LLDBClient.breakpoints に追加するだけ) ===
	if (bpCb) {
		for (auto& tup : buf->bps) {
			bpCb(thgc, std::get<0>(tup).c_str(), std::get<1>(tup), std::get<2>(tup));
		}
	}

	// === ファイル open (= 重い、各 viewer は offscreen + tile 群を生成する) ===
	// 1 ファイル / 1 フレームのペースで開く。すべて同フレームで開くと bgfx の
	// queueOffscreenResize / tile 作成が立て込んで描画が乱れる。
	if (openCb) {
		for (auto& f : buf->files) {
			SDL_Log("[state] replay open: %s", f.c_str());
			openCb(thgc, f.c_str());
			co_yield nullptr;   // ← UI フレーム送り。次の resume_all まで待機。
		}
	}
	delete buf;
	co_return (char*)0;
}

inline void replay(ThreadGC* thgc,
	std::function<void(ThreadGC*, const char*)> openCb,
	std::function<void(ThreadGC*, const char*, uint32_t, bool)> bpCb)
{
	if (!g_pool) return;
	_spawn(thgc, _replay(thgc, g_pool, thgc->queue,
		std::move(openCb), std::move(bpCb)));
}

} // namespace state
