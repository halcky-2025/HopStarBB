#pragma once
// ============================================================
// encode.h
//
// 設計方針: 「デコード層」と「UI 化層」を分離する。
//
//   ToText(stream, encoding)   → String*       (デコード済み UTF-8 テキスト)
//   ToHex(stream)              → String*       (HEX ダンプ文字列)
//   ToImage(stream)            → Image*        (RGBA pixels + bgfx texture)
//   ToSound(stream)            → Sound*        (miniaudio handle、再生可)
//   ToVideo(stream)            → Video*        (libav handle、フレーム取得可)
//   ToVideoSound(stream)       → VideoSound*   (Video + audio device)
//
// + クロスプラットフォーム ファイルダイアログ:
//   ShowOpenFileDialog   (callback で開いたパスが返る)
//   ShowSaveFileDialog
//   ShowOpenFolderDialog
//
// UI に貼りたい時はこれを更にラップする (TextBox/NewImage/AudioPlayer/...)。
// → 同じデコード結果を流用して保存・編集・別 viewer 切替もできる。
// ============================================================

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <new>
#include <vector>
#include <string>
#include <SDL3/SDL_dialog.h>
#include <portaudio.h>

// FileEngine 本体 (worker thread + readAsync を使う):
//   - getFileEngine()           インスタンス取得
//   - readFileToMemory(path)    ローカル / Internal / Resource
//   - readFileFromAddress(addr) URL / 任意アドレス (cloud/local 自動判定)
#include "file_engine.h"
HopStarIO::FileEngine* getFileEngine();
std::vector<uint8_t>   readFileToMemory(const std::string& path);
std::vector<uint8_t>   readFileFromAddress(const std::string& address);

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

// ============================================================
// Stream: 生バイト列の抽象 (file or memory)
// ============================================================
struct Stream {
	uint8_t* data;
	size_t   size;
	size_t   pos;
	bool     owned;
};

inline Stream* StreamFromMemory(ThreadGC* thgc, uint8_t* data, size_t size) {
	Stream* s = (Stream*)GC_alloc_size(thgc, sizeof(Stream));
	s->data  = data;  s->size = size;  s->pos = 0;  s->owned = false;
	return s;
}

inline Stream* StreamFromFile(ThreadGC* thgc, const char* path) {
	FILE* f = fopen(path, "rb");
	if (!f) return nullptr;
	fseek(f, 0, SEEK_END);
	long sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (sz < 0) { fclose(f); return nullptr; }
	Stream* s = (Stream*)GC_alloc_size(thgc, sizeof(Stream));
	s->data  = (uint8_t*)GC_alloc_size(thgc, (size_t)sz);
	s->size  = (size_t)sz;  s->pos = 0;  s->owned = true;
	if (sz > 0) fread(s->data, 1, (size_t)sz, f);
	fclose(f);
	return s;
}

// FileEngine 経由でファイルを読む (推奨):
//   - iOS Resource bundle / Android assets / Cloud / P2P を透過的に扱う
//   - ローカルパスもそのまま OK (内部で External 扱い)
//   - URL もそのまま渡せる (HTTP/Cloud は cloudApp 経由)
inline Stream* StreamFromFileEngine(ThreadGC* thgc, const char* addressOrPath) {
	if (!addressOrPath) return nullptr;
	std::vector<uint8_t> bytes = readFileFromAddress(std::string(addressOrPath));
	if (bytes.empty()) return nullptr;
	Stream* s = (Stream*)GC_alloc_size(thgc, sizeof(Stream));
	s->data  = (uint8_t*)GC_alloc_size(thgc, bytes.size());
	memcpy(s->data, bytes.data(), bytes.size());
	s->size  = bytes.size();
	s->pos   = 0;
	s->owned = true;
	return s;
}

inline size_t  StreamRead(Stream* s, void* dst, size_t n) {
	size_t avail = s->size - s->pos, r = (n < avail) ? n : avail;
	memcpy(dst, s->data + s->pos, r); s->pos += r; return r;
}
inline bool    StreamSeek(Stream* s, int64_t off, int origin) {
	int64_t np;
	switch (origin) {
		case SEEK_SET: np = off; break;
		case SEEK_CUR: np = (int64_t)s->pos + off; break;
		case SEEK_END: np = (int64_t)s->size + off; break;
		default: return false;
	}
	if (np < 0 || np > (int64_t)s->size) return false;
	s->pos = (size_t)np; return true;
}
inline int64_t StreamTell(Stream* s) { return (int64_t)s->pos; }
inline int64_t StreamSize(Stream* s) { return (int64_t)s->size; }
inline void    StreamRewind(Stream* s) { s->pos = 0; }

// ============================================================
// 文字エンコーディング
// ============================================================
enum class Encoding {
	Auto,
	UTF8, UTF8_BOM,
	UTF16_LE, UTF16_BE,
	ShiftJIS, EUC_JP,
	Windows1252,
};

// 先頭 n byte だけを覗いて UTF-8 として valid か判定する。
// 注意: 検出用 (= ファイル先頭の sniffing) で使うので、最後の multi-byte 文字が
// バッファ境界で切れていても「ここまで valid」を返す。
//   例: n=1024 で 1023 byte 目が 0xE3 (3 byte 文字の先頭) のとき、
//   続きの 2 byte は範囲外。これを invalid とすると BOM 付き UTF-8 が誤判定される。
inline bool isValidUTF8(const uint8_t* d, size_t n) {
	size_t i = 0;
	while (i < n) {
		uint8_t b = d[i]; size_t need;
		if      ((b & 0x80) == 0)    need = 0;
		else if ((b & 0xE0) == 0xC0) need = 1;
		else if ((b & 0xF0) == 0xE0) need = 2;
		else if ((b & 0xF8) == 0xF0) need = 3;
		else return false;
		if (i + need >= n) return true;   // 末尾の文字が cut された → ここまで valid
		for (size_t j = 1; j <= need; j++)
			if ((d[i + j] & 0xC0) != 0x80) return false;
		i += need + 1;
	}
	return true;
}

inline Encoding detectEncoding(const uint8_t* d, size_t n) {
	if (n >= 3 && d[0] == 0xEF && d[1] == 0xBB && d[2] == 0xBF) return Encoding::UTF8_BOM;
	if (n >= 2 && d[0] == 0xFF && d[1] == 0xFE) return Encoding::UTF16_LE;
	if (n >= 2 && d[0] == 0xFE && d[1] == 0xFF) return Encoding::UTF16_BE;
	if (isValidUTF8(d, n)) return Encoding::UTF8;
	int sjis = 0, eucjp = 0;
	for (size_t i = 0; i + 1 < n; i++) {
		uint8_t b = d[i], c = d[i + 1];
		if (((b >= 0x81 && b <= 0x9F) || (b >= 0xE0 && b <= 0xFC)) &&
			((c >= 0x40 && c <= 0x7E) || (c >= 0x80 && c <= 0xFC))) sjis++;
		if (b >= 0xA1 && b <= 0xFE && c >= 0xA1 && c <= 0xFE) eucjp++;
	}
	if (sjis > eucjp + 5) return Encoding::ShiftJIS;
	if (eucjp > sjis + 5) return Encoding::EUC_JP;
	return Encoding::Windows1252;
}

inline int utf8_emit(uint8_t* dst, uint32_t cp) {
	if (cp <= 0x7F)   { dst[0] = (uint8_t)cp; return 1; }
	if (cp <= 0x7FF)  { dst[0] = (uint8_t)(0xC0 | (cp >> 6));
	                    dst[1] = (uint8_t)(0x80 | (cp & 0x3F)); return 2; }
	if (cp <= 0xFFFF) { dst[0] = (uint8_t)(0xE0 | (cp >> 12));
	                    dst[1] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
	                    dst[2] = (uint8_t)(0x80 | (cp & 0x3F)); return 3; }
	dst[0] = (uint8_t)(0xF0 | (cp >> 18));
	dst[1] = (uint8_t)(0x80 | ((cp >> 12) & 0x3F));
	dst[2] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
	dst[3] = (uint8_t)(0x80 | (cp & 0x3F));
	return 4;
}

// UTF-16 (LE/BE) → String*
//   入力をワイド配列にしてから StringFromUTF16 (ASCII チェック + esize 自動判定) へ
inline String* DecodeUTF16(ThreadGC* thgc, const uint8_t* d, size_t n, bool be) {
	size_t wlen = n / 2;
	if (wlen == 0) return createString(thgc, (char*)"", 0, 1);
	uint16_t* tmp = (uint16_t*)GC_alloc_size(thgc, wlen * sizeof(uint16_t));
	if (be) {
		for (size_t i = 0; i < wlen; i++) {
			tmp[i] = (uint16_t)(((uint16_t)d[i * 2] << 8) | d[i * 2 + 1]);
		}
	} else {
		for (size_t i = 0; i < wlen; i++) {
			tmp[i] = (uint16_t)(d[i * 2] | ((uint16_t)d[i * 2 + 1] << 8));
		}
	}
	return StringFromUTF16(thgc, tmp, wlen);
}

#ifdef _WIN32
#include <windows.h>
// Shift-JIS → String* (Windows: cp932 経由で UTF-16 ワイドに変換 → StringFromUTF16)
inline String* DecodeShiftJIS(ThreadGC* thgc, const uint8_t* d, size_t n) {
	if (n == 0) return createString(thgc, (char*)"", 0, 1);
	int wlen = MultiByteToWideChar(932, 0, (LPCCH)d, (int)n, nullptr, 0);
	if (wlen == 0) return StringFromUTF8(thgc, (const char*)d, (int)n);
	wchar_t* wbuf = (wchar_t*)GC_alloc_size(thgc, sizeof(wchar_t) * (size_t)wlen);
	MultiByteToWideChar(932, 0, (LPCCH)d, (int)n, wbuf, wlen);
	return StringFromUTF16(thgc, (const uint16_t*)wbuf, (size_t)wlen);
}
// EUC-JP → String* (Windows: cp20932 経由)
inline String* DecodeEUCJP(ThreadGC* thgc, const uint8_t* d, size_t n) {
	if (n == 0) return createString(thgc, (char*)"", 0, 1);
	int wlen = MultiByteToWideChar(20932, 0, (LPCCH)d, (int)n, nullptr, 0);
	if (wlen == 0) return StringFromUTF8(thgc, (const char*)d, (int)n);
	wchar_t* wbuf = (wchar_t*)GC_alloc_size(thgc, sizeof(wchar_t) * (size_t)wlen);
	MultiByteToWideChar(20932, 0, (LPCCH)d, (int)n, wbuf, wlen);
	return StringFromUTF16(thgc, (const uint16_t*)wbuf, (size_t)wlen);
}
#else
// 非 Windows: 簡易フォールバック (UTF-8 として扱う)
inline String* DecodeShiftJIS(ThreadGC* thgc, const uint8_t* d, size_t n) {
	return StringFromUTF8(thgc, (const char*)d, (int)n);
}
inline String* DecodeEUCJP(ThreadGC* thgc, const uint8_t* d, size_t n) {
	return StringFromUTF8(thgc, (const char*)d, (int)n);
}
#endif

// ============================================================
// ToText: Stream → String*  (UTF-8 にデコード済みのテキスト)
// ============================================================
inline String* ToText(ThreadGC* thgc, Stream* s, Encoding enc = Encoding::Auto) {
	if (!s) return nullptr;
	if (enc == Encoding::Auto) enc = detectEncoding(s->data, s->size);
	switch (enc) {
		case Encoding::UTF8:
			// 純 ASCII なら esize=1, 日本語等を含むなら esize=2 (UTF-16LE) で返す
			return StringFromUTF8(thgc, (const char*)s->data, (int)s->size);
		case Encoding::UTF8_BOM: {
			size_t skip = (s->size >= 3) ? 3 : 0;
			return StringFromUTF8(thgc, (const char*)(s->data + skip), (int)(s->size - skip));
		}
		case Encoding::UTF16_LE: return DecodeUTF16(thgc, s->data + 2, s->size >= 2 ? s->size - 2 : 0, false);
		case Encoding::UTF16_BE: return DecodeUTF16(thgc, s->data + 2, s->size >= 2 ? s->size - 2 : 0, true);
		case Encoding::ShiftJIS: return DecodeShiftJIS(thgc, s->data, s->size);
		case Encoding::EUC_JP:   return DecodeEUCJP(thgc, s->data, s->size);
		default:                 return StringFromUTF8(thgc, (const char*)s->data, (int)s->size);
	}
}

// ============================================================
// ToHex: Stream → String*  (HEX ダンプ文字列)
//   "00000000  48 65 6c 6c 6f 20 57 6f  72 6c 64 0a              |Hello World.|"
// ============================================================
inline String* ToHex(ThreadGC* thgc, Stream* s) {
	if (!s) return nullptr;
	const size_t bpl = 16;
	size_t lines = (s->size + bpl - 1) / bpl + 1;
	size_t cap = lines * 80 + 64;
	char* buf = (char*)GC_alloc_size(thgc, cap);
	size_t w = 0;
	for (size_t off = 0; off < s->size; off += bpl) {
		w += (size_t)snprintf(buf + w, cap - w, "%08zx  ", off);
		for (size_t i = 0; i < bpl; i++) {
			if (off + i < s->size) w += (size_t)snprintf(buf + w, cap - w, "%02x ", s->data[off + i]);
			else                   w += (size_t)snprintf(buf + w, cap - w, "   ");
			if (i == 7) buf[w++] = ' ';
		}
		buf[w++] = ' '; buf[w++] = '|';
		for (size_t i = 0; i < bpl && off + i < s->size; i++) {
			uint8_t b = s->data[off + i];
			buf[w++] = (b >= 0x20 && b < 0x7F) ? (char)b : '.';
		}
		buf[w++] = '|';
		buf[w++] = '\n';
	}
	return createString(thgc, buf, (int)w, 1);
}

// ============================================================
// Bitmap: デコード結果 (RGBA pixels + 任意で bgfx texture)
//   ※ 'Image' は DrawCommandType の列挙子と衝突するため Bitmap に改名
// ============================================================
struct Bitmap {
	uint8_t*     pixels;    // RGBA8 画素列 (w*h*4 byte) — GC 管理外
	int          w, h;
	ImageId      tex;       // bgfx に upload 済みなら有効
};

// bgfx texture を ImageId として登録するヘルパ (既存 ImageMaster と整合する API があれば差し替え)
// 当面: bgfx::createTexture2D で直接作って ImageId を発行する。
inline ImageId RegisterBgfxTextureRGBA(ThreadGC* thgc, const uint8_t* rgba, int w, int h) {
	const bgfx::Memory* mem = bgfx::copy(rgba, (uint32_t)(w * h * 4));
	bgfx::TextureHandle bh = bgfx::createTexture2D(
		(uint16_t)w, (uint16_t)h, false, 1,
		bgfx::TextureFormat::RGBA8,
		BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
		mem);
	// ImageId は uint64; bgfx ハンドル + サイズを 1 つの値に詰める簡易フォーマット。
	// 既存 ImageMaster に統合する場合は registerExternalTexture(thgc, bh, w, h) 等を呼ぶ。
	(void)thgc;
	uint64_t id = ((uint64_t)bh.idx << 32) | ((uint64_t)w & 0xFFFF) | (((uint64_t)h & 0xFFFF) << 16);
	return (ImageId)id;
}

// ToImage: Stream → Bitmap*
inline Bitmap* ToImage(ThreadGC* thgc, Stream* s) {
	if (!s) return nullptr;
	int w = 0, h = 0, ch = 0;
	uint8_t* pixels = stbi_load_from_memory(s->data, (int)s->size, &w, &h, &ch, 4);
	if (!pixels) return nullptr;
	Bitmap* img = (Bitmap*)GC_alloc_size(thgc, sizeof(Bitmap));
	img->pixels = pixels;       // stb 側のヒープ。Bitmap を破棄する時 stbi_image_free
	img->w = w; img->h = h;
	img->tex = RegisterBgfxTextureRGBA(thgc, pixels, w, h);
	return img;
}

// 不要になったら呼ぶ (GC では追跡しないバイト列を片付ける)
inline void BitmapFree(Bitmap* img) {
	if (img && img->pixels) { stbi_image_free(img->pixels); img->pixels = nullptr; }
}

// ============================================================
// Sound: ffmpeg で全 PCM をデコードしてメモリに展開、PortAudio で再生
//   既存 audio.h (nle::AudioOutput) と同じ PortAudio パターンを採用。
//   iOS は platform/apple/portaudio_ios.mm の AudioUnit shim が処理する。
// ============================================================
struct Sound {
	// デコード済み PCM (interleaved float32)
	float*   pcm;
	size_t   pcmFrames;       // フレーム数 (サンプル数 / channels)
	int      channels;
	int      sampleRate;

	// PortAudio
	PaStream* paStream;
	bool      paOpen;

	// 再生位置 (callback と main で共有 — 厳密 atomic 不要、
	// 1 フレーム ~5ms 単位の不整合は人間に知覚されない)
	volatile size_t playPos;
	volatile bool   playing;
};

// 内部: ffmpeg avformat の AVIO 用 (Stream を memory として開く)
struct _AudDecCtx { Stream* s; size_t pos; };
inline int _AudDec_Read(void* opaque, uint8_t* buf, int n) {
	auto* d = (_AudDecCtx*)opaque;
	size_t avail = d->s->size - d->pos;
	int r = (int)((size_t)n < avail ? (size_t)n : avail);
	if (r <= 0) return AVERROR_EOF;
	memcpy(buf, d->s->data + d->pos, (size_t)r); d->pos += (size_t)r; return r;
}
inline int64_t _AudDec_Seek(void* opaque, int64_t off, int whence) {
	auto* d = (_AudDecCtx*)opaque;
	if (whence == AVSEEK_SIZE) return (int64_t)d->s->size;
	int64_t np;
	switch (whence) {
		case SEEK_SET: np = off; break;
		case SEEK_CUR: np = (int64_t)d->pos + off; break;
		case SEEK_END: np = (int64_t)d->s->size + off; break;
		default: return -1;
	}
	if (np < 0 || np > (int64_t)d->s->size) return -1;
	d->pos = (size_t)np; return np;
}

// ffmpeg で memory から音声を decode し、interleaved f32 PCM として返す
inline bool DecodeAudioToPCMFloat(Stream* s, float** outPcm, size_t* outFrames,
                                  int* outChannels, int* outSampleRate)
{
	*outPcm = nullptr; *outFrames = 0; *outChannels = 0; *outSampleRate = 0;
	_AudDecCtx dc{ s, 0 };
	const int avBufSize = 4096;
	uint8_t* avBuf = (uint8_t*)av_malloc(avBufSize);
	AVIOContext* avio = avio_alloc_context(avBuf, avBufSize, 0, &dc, _AudDec_Read, nullptr, _AudDec_Seek);
	AVFormatContext* fmt = avformat_alloc_context();
	fmt->pb = avio;

	auto cleanup = [&](float* p) {
		if (fmt) avformat_close_input(&fmt);
		if (avio) { if (avio->buffer) av_free(avio->buffer); avio_context_free(&avio); }
		if (p) free(p);
	};

	if (avformat_open_input(&fmt, nullptr, nullptr, nullptr) < 0) { cleanup(nullptr); return false; }
	if (avformat_find_stream_info(fmt, nullptr) < 0)            { cleanup(nullptr); return false; }

	int aStream = -1;
	for (unsigned i = 0; i < fmt->nb_streams; i++)
		if (fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) { aStream = (int)i; break; }
	if (aStream < 0) { cleanup(nullptr); return false; }

	AVCodecParameters* par = fmt->streams[aStream]->codecpar;
	const AVCodec* codec = avcodec_find_decoder(par->codec_id);
	AVCodecContext* ac = avcodec_alloc_context3(codec);
	avcodec_parameters_to_context(ac, par);
	if (avcodec_open2(ac, codec, nullptr) < 0) { avcodec_free_context(&ac); cleanup(nullptr); return false; }

	int channels = ac->ch_layout.nb_channels;
	int sampleRate = ac->sample_rate;

	SwrContext* swr = swr_alloc();
	av_opt_set_chlayout(swr, "in_chlayout",   &ac->ch_layout, 0);
	av_opt_set_int     (swr, "in_sample_rate", sampleRate, 0);
	av_opt_set_sample_fmt(swr, "in_sample_fmt", ac->sample_fmt, 0);
	av_opt_set_chlayout(swr, "out_chlayout",   &ac->ch_layout, 0);
	av_opt_set_int     (swr, "out_sample_rate", sampleRate, 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_FLT, 0);
	if (swr_init(swr) < 0) { swr_free(&swr); avcodec_free_context(&ac); cleanup(nullptr); return false; }

	AVPacket* pkt = av_packet_alloc();
	AVFrame*  frame = av_frame_alloc();
	float*    pcm = nullptr;
	size_t    pcmCap = 0, pcmSize = 0;     // フレーム単位

	while (av_read_frame(fmt, pkt) >= 0) {
		if (pkt->stream_index == aStream && avcodec_send_packet(ac, pkt) >= 0) {
			while (avcodec_receive_frame(ac, frame) >= 0) {
				int outSamples = (int)swr_get_out_samples(swr, frame->nb_samples);
				if (outSamples < 0) outSamples = frame->nb_samples;
				if (pcmSize + (size_t)outSamples > pcmCap) {
					size_t newCap = pcmCap ? pcmCap * 2 : (size_t)(sampleRate * 2);
					while (newCap < pcmSize + (size_t)outSamples) newCap *= 2;
					pcm = (float*)realloc(pcm, newCap * (size_t)channels * sizeof(float));
					pcmCap = newCap;
				}
				uint8_t* outBuf[1] = { (uint8_t*)(pcm + pcmSize * (size_t)channels) };
				int wrote = swr_convert(swr, outBuf, outSamples,
				                        (const uint8_t**)frame->data, frame->nb_samples);
				if (wrote > 0) pcmSize += (size_t)wrote;
			}
		}
		av_packet_unref(pkt);
	}
	// flush
	avcodec_send_packet(ac, nullptr);
	while (avcodec_receive_frame(ac, frame) >= 0) {
		int outSamples = (int)swr_get_out_samples(swr, frame->nb_samples);
		if (outSamples < 0) outSamples = frame->nb_samples;
		if (pcmSize + (size_t)outSamples > pcmCap) {
			size_t newCap = pcmCap ? pcmCap * 2 : 4096;
			while (newCap < pcmSize + (size_t)outSamples) newCap *= 2;
			pcm = (float*)realloc(pcm, newCap * (size_t)channels * sizeof(float));
			pcmCap = newCap;
		}
		uint8_t* outBuf[1] = { (uint8_t*)(pcm + pcmSize * (size_t)channels) };
		int wrote = swr_convert(swr, outBuf, outSamples,
		                        (const uint8_t**)frame->data, frame->nb_samples);
		if (wrote > 0) pcmSize += (size_t)wrote;
	}

	av_packet_free(&pkt);
	av_frame_free(&frame);
	swr_free(&swr);
	avcodec_free_context(&ac);
	avformat_close_input(&fmt);
	if (avio) { if (avio->buffer) av_free(avio->buffer); avio_context_free(&avio); }

	*outPcm        = pcm;
	*outFrames     = pcmSize;
	*outChannels   = channels;
	*outSampleRate = sampleRate;
	return pcm != nullptr && pcmSize > 0;
}

inline int Sound_PaCallback(const void* /*in*/, void* out, unsigned long frames,
                            const PaStreamCallbackTimeInfo* /*ti*/,
                            PaStreamCallbackFlags /*flags*/, void* userData)
{
	Sound* sd = (Sound*)userData;
	float* dst = (float*)out;
	if (!sd->playing) {
		memset(dst, 0, frames * (size_t)sd->channels * sizeof(float));
		return paContinue;
	}
	size_t pos = sd->playPos;
	size_t avail = (sd->pcmFrames > pos) ? (sd->pcmFrames - pos) : 0;
	size_t take = (frames < avail) ? frames : avail;
	if (take > 0) memcpy(dst, sd->pcm + pos * (size_t)sd->channels, take * (size_t)sd->channels * sizeof(float));
	if (take < frames) memset(dst + take * (size_t)sd->channels, 0,
	                          (frames - take) * (size_t)sd->channels * sizeof(float));
	sd->playPos = pos + take;
	return paContinue;
}

inline Sound* ToSound(ThreadGC* thgc, Stream* s) {
	if (!s || s->size == 0) return nullptr;
	Sound* sd = (Sound*)GC_alloc_size(thgc, sizeof(Sound));
	memset(sd, 0, sizeof(Sound));

	if (!DecodeAudioToPCMFloat(s, &sd->pcm, &sd->pcmFrames, &sd->channels, &sd->sampleRate)) {
		return sd;   // デコード失敗でも空 Sound 返す (使用側は paOpen で判断)
	}

	Pa_Initialize();   // 多重呼び出しは内部で参照カウント
	PaError err = Pa_OpenDefaultStream(
		&sd->paStream,
		0,                 // 入力チャンネル
		sd->channels,      // 出力チャンネル
		paFloat32,
		(double)sd->sampleRate,
		256,               // frames per buffer (audio.h と同じ)
		Sound_PaCallback,
		sd);
	if (err == paNoError) sd->paOpen = true;
	return sd;
}

inline void Sound_Play(Sound* sd) {
	if (!sd || !sd->paOpen) return;
	sd->playing = true;
	Pa_StartStream(sd->paStream);
}
inline void Sound_Pause(Sound* sd) {
	if (!sd || !sd->paOpen) return;
	sd->playing = false;
	Pa_StopStream(sd->paStream);
}
inline void Sound_Seek(Sound* sd, double timeSec) {
	if (!sd) return;
	size_t pos = (size_t)(timeSec * sd->sampleRate);
	if (pos > sd->pcmFrames) pos = sd->pcmFrames;
	sd->playPos = pos;
}
inline void Sound_Free(Sound* sd) {
	if (!sd) return;
	if (sd->paOpen) {
		Pa_StopStream(sd->paStream);
		Pa_CloseStream(sd->paStream);
		sd->paOpen = false;
	}
	if (sd->pcm) { free(sd->pcm); sd->pcm = nullptr; }
}

// ============================================================
// Video: ffmpeg state + AVIO カスタム入力
// ============================================================
struct Video {
	// AVIO 用 source (memory)
	uint8_t*         srcBuffer;
	size_t           srcSize;
	size_t           srcPos;

	// libav state
	AVIOContext*     avio;
	AVFormatContext* fmt;
	AVCodecContext*  vCodec;
	int              vStream;
	SwsContext*      sws;
	AVFrame*         frame;
	AVFrame*         rgbaFrame;
	uint8_t*         rgbaBuffer;

	// メタ
	int              width, height;
	double           timebase;
	double           duration;     // 秒
	double           currentTime;  // 秒

	// 現在表示しているフレーム (毎フレーム再生中は更新)
	ImageId          tex;

	bool             valid;
};

inline int Video_Read(void* opaque, uint8_t* buf, int n) {
	Video* vp = (Video*)opaque;
	size_t avail = vp->srcSize - vp->srcPos;
	int r = (int)((size_t)n < avail ? (size_t)n : avail);
	if (r <= 0) return AVERROR_EOF;
	memcpy(buf, vp->srcBuffer + vp->srcPos, (size_t)r);
	vp->srcPos += (size_t)r;
	return r;
}
inline int64_t Video_Seek(void* opaque, int64_t off, int whence) {
	Video* vp = (Video*)opaque;
	if (whence == AVSEEK_SIZE) return (int64_t)vp->srcSize;
	int64_t np;
	switch (whence) {
		case SEEK_SET: np = off; break;
		case SEEK_CUR: np = (int64_t)vp->srcPos + off; break;
		case SEEK_END: np = (int64_t)vp->srcSize + off; break;
		default: return -1;
	}
	if (np < 0 || np > (int64_t)vp->srcSize) return -1;
	vp->srcPos = (size_t)np; return np;
}

// 1 フレーム取得して内部 RGBA バッファ + tex を更新する
inline bool Video_DecodeNextFrame(ThreadGC* thgc, Video* vp) {
	if (!vp || !vp->valid) return false;
	AVPacket* pkt = av_packet_alloc();
	bool got = false;
	while (av_read_frame(vp->fmt, pkt) >= 0) {
		if (pkt->stream_index == vp->vStream) {
			if (avcodec_send_packet(vp->vCodec, pkt) >= 0
				&& avcodec_receive_frame(vp->vCodec, vp->frame) >= 0) {
				sws_scale(vp->sws,
					vp->frame->data, vp->frame->linesize, 0, vp->height,
					vp->rgbaFrame->data, vp->rgbaFrame->linesize);
				vp->tex = RegisterBgfxTextureRGBA(thgc, vp->rgbaBuffer, vp->width, vp->height);
				vp->currentTime = vp->frame->pts * vp->timebase;
				got = true;
			}
		}
		av_packet_unref(pkt);
		if (got) break;
	}
	av_packet_free(&pkt);
	return got;
}

// ToVideo: Stream → Video*  (映像のみ; 音声無視)
inline Video* ToVideo(ThreadGC* thgc, Stream* s) {
	if (!s || s->size == 0) return nullptr;
	Video* vp = (Video*)GC_alloc_size(thgc, sizeof(Video));
	memset(vp, 0, sizeof(Video));
	vp->vStream = -1;

	vp->srcBuffer = (uint8_t*)malloc(s->size);
	memcpy(vp->srcBuffer, s->data, s->size);
	vp->srcSize = s->size;

	const int avBufSize = 4096;
	uint8_t* avBuf = (uint8_t*)av_malloc(avBufSize);
	vp->avio = avio_alloc_context(avBuf, avBufSize, 0, vp, Video_Read, nullptr, Video_Seek);
	vp->fmt = avformat_alloc_context();
	vp->fmt->pb = vp->avio;

	if (avformat_open_input(&vp->fmt, nullptr, nullptr, nullptr) < 0) return vp;
	if (avformat_find_stream_info(vp->fmt, nullptr) < 0)            return vp;

	for (unsigned i = 0; i < vp->fmt->nb_streams; i++) {
		if (vp->fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			vp->vStream = (int)i; break;
		}
	}
	if (vp->vStream < 0) return vp;

	AVCodecParameters* par = vp->fmt->streams[vp->vStream]->codecpar;
	const AVCodec* codec = avcodec_find_decoder(par->codec_id);
	vp->vCodec = avcodec_alloc_context3(codec);
	avcodec_parameters_to_context(vp->vCodec, par);
	if (avcodec_open2(vp->vCodec, codec, nullptr) < 0) return vp;

	vp->width    = vp->vCodec->width;
	vp->height   = vp->vCodec->height;
	vp->timebase = av_q2d(vp->fmt->streams[vp->vStream]->time_base);
	vp->duration = (vp->fmt->duration > 0) ? (double)vp->fmt->duration / AV_TIME_BASE : 0.0;
	vp->sws      = sws_getContext(vp->width, vp->height, vp->vCodec->pix_fmt,
	                              vp->width, vp->height, AV_PIX_FMT_RGBA,
	                              SWS_BILINEAR, nullptr, nullptr, nullptr);
	vp->frame     = av_frame_alloc();
	vp->rgbaFrame = av_frame_alloc();
	int rgbSz = av_image_get_buffer_size(AV_PIX_FMT_RGBA, vp->width, vp->height, 1);
	vp->rgbaBuffer = (uint8_t*)av_malloc((size_t)rgbSz);
	av_image_fill_arrays(vp->rgbaFrame->data, vp->rgbaFrame->linesize,
	                     vp->rgbaBuffer, AV_PIX_FMT_RGBA, vp->width, vp->height, 1);
	vp->valid = true;

	// 先頭フレームを 1 枚デコードして tex に焼く (サムネ表示用)
	Video_DecodeNextFrame(thgc, vp);
	return vp;
}

inline void Video_Free(Video* vp) {
	if (!vp) return;
	if (vp->frame)      av_frame_free(&vp->frame);
	if (vp->rgbaFrame)  av_frame_free(&vp->rgbaFrame);
	if (vp->rgbaBuffer) av_free(vp->rgbaBuffer);
	if (vp->sws)        sws_freeContext(vp->sws);
	if (vp->vCodec)     avcodec_free_context(&vp->vCodec);
	if (vp->fmt)        avformat_close_input(&vp->fmt);
	if (vp->avio) {
		if (vp->avio->buffer) av_free(vp->avio->buffer);
		avio_context_free(&vp->avio);
	}
	if (vp->srcBuffer)  free(vp->srcBuffer);
	memset(vp, 0, sizeof(Video));
}

// ============================================================
// VideoSound: Video + 音声 (PortAudio + ffmpeg)
//
// 簡易実装: 音声トラックを起動時に全 PCM を memory にデコード、
//          PortAudio で再生する。currentTime と playPos の同期は
//          呼び側が `Video_DecodeNextFrame` を呼ぶタイミングで行う。
//
// 長尺動画 (1 時間超) で全 PCM を持つとメモリ消費が大きいので、
// 将来 ring buffer ベースの streaming に切替えても良い。
// ============================================================
struct VideoSound {
	Video v;          // 映像部分

	// 音声 PCM (interleaved float32、起動時に一括デコード)
	float*   apcm;
	size_t   apcmFrames;
	int      aChannels;
	int      aSampleRate;

	// PortAudio
	PaStream* paStream;
	bool      paOpen;

	volatile size_t playPos;
	volatile bool   playing;
};

inline int VideoSound_PaCallback(const void* /*in*/, void* out, unsigned long frames,
                                 const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* ud)
{
	VideoSound* vs = (VideoSound*)ud;
	float* dst = (float*)out;
	if (!vs->playing || !vs->apcm) {
		memset(dst, 0, frames * (size_t)vs->aChannels * sizeof(float));
		return paContinue;
	}
	size_t pos = vs->playPos;
	size_t avail = (vs->apcmFrames > pos) ? (vs->apcmFrames - pos) : 0;
	size_t take = (frames < avail) ? frames : avail;
	if (take > 0) memcpy(dst, vs->apcm + pos * (size_t)vs->aChannels, take * (size_t)vs->aChannels * sizeof(float));
	if (take < frames) memset(dst + take * (size_t)vs->aChannels, 0,
	                          (frames - take) * (size_t)vs->aChannels * sizeof(float));
	vs->playPos = pos + take;
	return paContinue;
}

inline VideoSound* ToVideoSound(ThreadGC* thgc, Stream* s) {
	if (!s) return nullptr;
	// まず映像を Video* として初期化 (内部で 1 frame デコード済み)
	Video* base = ToVideo(thgc, s);
	if (!base) return nullptr;

	VideoSound* vs = (VideoSound*)GC_alloc_size(thgc, sizeof(VideoSound));
	memset(vs, 0, sizeof(VideoSound));
	vs->v = *base;       // libav handle はコピー安全 (ポインタのみ)

	// 音声を別途 memory から全部デコード (Video の avformat とは別 instance)
	if (DecodeAudioToPCMFloat(s, &vs->apcm, &vs->apcmFrames, &vs->aChannels, &vs->aSampleRate)) {
		Pa_Initialize();
		if (Pa_OpenDefaultStream(
				&vs->paStream,
				0, vs->aChannels, paFloat32, (double)vs->aSampleRate,
				256, VideoSound_PaCallback, vs) == paNoError) {
			vs->paOpen = true;
		}
	}
	return vs;
}

inline void VideoSound_Play(VideoSound* vs) {
	if (!vs || !vs->paOpen) return;
	vs->playing = true;
	Pa_StartStream(vs->paStream);
}
inline void VideoSound_Pause(VideoSound* vs) {
	if (!vs || !vs->paOpen) return;
	vs->playing = false;
	Pa_StopStream(vs->paStream);
}
inline void VideoSound_Seek(VideoSound* vs, double timeSec) {
	if (!vs) return;
	size_t pos = (size_t)(timeSec * vs->aSampleRate);
	if (pos > vs->apcmFrames) pos = vs->apcmFrames;
	vs->playPos = pos;
	// Video 側のシークも別途必要 (av_seek_frame)
}
inline void VideoSound_Free(VideoSound* vs) {
	if (!vs) return;
	if (vs->paOpen) {
		Pa_StopStream(vs->paStream);
		Pa_CloseStream(vs->paStream);
		vs->paOpen = false;
	}
	if (vs->apcm) { free(vs->apcm); vs->apcm = nullptr; }
	Video_Free(&vs->v);
	memset(vs, 0, sizeof(VideoSound));
}

// ============================================================
// 自動判定 (ファイル種別)
// ============================================================
enum class FileKind { Unknown, Text, SourceCode, Image, Video, Audio, Binary };

inline FileKind detectFileKindFromBytes(const uint8_t* d, size_t n) {
	if (n < 4) return FileKind::Unknown;
	if (d[0] == 0x89 && d[1] == 'P' && d[2] == 'N' && d[3] == 'G') return FileKind::Image;
	if (d[0] == 0xFF && d[1] == 0xD8 && d[2] == 0xFF)              return FileKind::Image;
	if (d[0] == 'G' && d[1] == 'I' && d[2] == 'F' && d[3] == '8')  return FileKind::Image;
	if (d[0] == 'B' && d[1] == 'M')                                 return FileKind::Image;
	if (n >= 12 && d[4] == 'f' && d[5] == 't' && d[6] == 'y' && d[7] == 'p') return FileKind::Video;
	if (d[0] == 0x1A && d[1] == 0x45 && d[2] == 0xDF && d[3] == 0xA3)         return FileKind::Video;
	if (d[0] == 'R' && d[1] == 'I' && d[2] == 'F' && d[3] == 'F') {
		if (n >= 12 && d[8] == 'A' && d[9] == 'V' && d[10] == 'I') return FileKind::Video;
		if (n >= 12 && d[8] == 'W' && d[9] == 'A' && d[10] == 'V') return FileKind::Audio;
		if (n >= 12 && d[8] == 'W' && d[9] == 'E' && d[10] == 'B' && d[11] == 'P') return FileKind::Image;
	}
	if (d[0] == 'I' && d[1] == 'D' && d[2] == '3')                 return FileKind::Audio;
	if (d[0] == 0xFF && (d[1] & 0xE0) == 0xE0)                     return FileKind::Audio;
	if (d[0] == 'f' && d[1] == 'L' && d[2] == 'a' && d[3] == 'C')  return FileKind::Audio;
	if (d[0] == 'O' && d[1] == 'g' && d[2] == 'g' && d[3] == 'S')  return FileKind::Audio;
	if (isValidUTF8(d, n < 1024 ? n : 1024)) return FileKind::Text;
	return FileKind::Binary;
}

inline FileKind detectFileKindFromExt(const char* path) {
	const char* dot = strrchr(path, '.');
	if (!dot || dot == path) return FileKind::Unknown;
	const char* ext = dot + 1;
	char lo[16] = {0};
	for (int i = 0; i < 15 && ext[i]; i++)
		lo[i] = (char)((ext[i] >= 'A' && ext[i] <= 'Z') ? ext[i] + 32 : ext[i]);
	struct E { const char* e; FileKind k; };
	static const E table[] = {
		{"cy", FileKind::SourceCode},
		{"txt", FileKind::Text}, {"md", FileKind::Text}, {"json", FileKind::Text},
		{"xml", FileKind::Text}, {"yaml", FileKind::Text}, {"yml", FileKind::Text},
		{"py", FileKind::Text},  {"js", FileKind::Text},  {"ts", FileKind::Text},
		{"c", FileKind::Text},   {"cpp", FileKind::Text}, {"h", FileKind::Text}, {"hpp", FileKind::Text},
		{"cs", FileKind::Text},  {"go", FileKind::Text},  {"rs", FileKind::Text}, {"java", FileKind::Text},
		{"png", FileKind::Image}, {"jpg", FileKind::Image}, {"jpeg", FileKind::Image},
		{"gif", FileKind::Image}, {"bmp", FileKind::Image}, {"webp", FileKind::Image}, {"tga", FileKind::Image},
		{"mp4", FileKind::Video}, {"mov", FileKind::Video}, {"avi", FileKind::Video},
		{"mkv", FileKind::Video}, {"webm", FileKind::Video}, {"m4v", FileKind::Video},
		{"mp3", FileKind::Audio}, {"wav", FileKind::Audio}, {"flac", FileKind::Audio},
		{"ogg", FileKind::Audio}, {"m4a", FileKind::Audio}, {"aac", FileKind::Audio}, {"opus", FileKind::Audio},
	};
	for (auto& e : table) if (strcmp(lo, e.e) == 0) return e.k;
	return FileKind::Unknown;
}

inline FileKind detectFileKind(const char* path, Stream* s) {
	FileKind k = detectFileKindFromExt(path);
	if (k != FileKind::Unknown && k != FileKind::Text) return k;
	FileKind kb = detectFileKindFromBytes(s->data, s->size);
	if (kb != FileKind::Unknown) return kb;
	return k != FileKind::Unknown ? k : FileKind::Binary;
}

// ============================================================
// Cross-platform File Dialogs (SDL3)
//
// 内部で SDL_ShowOpenFileDialog 等を呼ぶ。OS ネイティブの dialog が出る:
//   Windows : IFileOpenDialog (COM)
//   macOS   : NSOpenPanel
//   Linux   : xdg-portal (or zenity fallback)
//   iOS     : UIDocumentPickerViewController
//   Android : Storage Access Framework
//   Web     : <input type="file">
//
// 完了は非同期 (callback 経由)。callback 内で UI を直接触る場合は、
// SDL3 が main スレッドに dispatch して呼ぶので一般には安全だが、
// OS によっては別スレッドで来るので必要なら自前で main スレッド回送する。
// ============================================================

// ============================================================
// SDL ダイアログのコールバックは別スレッドから来る可能性がある
// (iOS は main、Linux xdg-portal は dbus thread、Windows は呼び出し thread 等)。
// よって UI を直接触らず、いったん SDL_PushEvent でメインスレッドに
// マーシャルし、main.cpp の event loop で消費する。
//
// 使い方:
//   1. プログラム起動時に  RegisterDialogEvents()  を 1 回呼ぶ
//   2. main.cpp の SDL_PollEvent ループで
//        if (DispatchDialogEvent(&event)) continue;
//      を入れる (戻り値 true なら処理済み)
// ============================================================

// 結果保持 (ダイアログ → main thread の橋渡し用、SDL_Event::user.data1 に積む)
struct _DialogResult {
	ThreadGC* thgc;
	bool      isMulti;
	// 単一: path 1 本 + std::function
	char*     path;
	std::function<void(ThreadGC*, const char*)>* singleCb;
	// 複数: NULL 終端の char** + std::function
	char**    paths;
	int       pathCount;
	std::function<void(ThreadGC*, const char* const*)>* multiCb;
};

inline Uint32 SDL_EVENT_DIALOG_RESULT = 0;

inline void RegisterDialogEvents() {
	if (SDL_EVENT_DIALOG_RESULT == 0) {
		SDL_EVENT_DIALOG_RESULT = SDL_RegisterEvents(1);
	}
}

// _DialogCtx: SDL コールバックに渡すコンテキスト
struct _DialogCtx {
	ThreadGC* thgc;
	std::function<void(ThreadGC*, const char* /*path*/)>            single;
	std::function<void(ThreadGC*, const char* const* /*paths*/)>    multi;
	bool isMulti;
};

// SDL コールバック: 別スレッドの可能性 → SDL_PushEvent で main thread に dispatch
inline void SDLCALL _onDialogResult(void* userdata, const char* const* filelist, int /*filter*/) {
	auto* ctx = static_cast<_DialogCtx*>(userdata);
	if (!filelist || !filelist[0]) {   // キャンセル / エラー
		delete ctx;
		return;
	}
	if (SDL_EVENT_DIALOG_RESULT == 0) RegisterDialogEvents();

	auto* res = new _DialogResult{};
	res->thgc    = ctx->thgc;
	res->isMulti = ctx->isMulti;
	if (ctx->isMulti) {
		int n = 0;
		while (filelist[n]) n++;
		res->paths = new char*[n + 1];
		for (int i = 0; i < n; i++) res->paths[i] = SDL_strdup(filelist[i]);
		res->paths[n] = nullptr;
		res->pathCount = n;
		res->multiCb   = new std::function<void(ThreadGC*, const char* const*)>(ctx->multi);
	} else {
		res->path     = SDL_strdup(filelist[0]);
		res->singleCb = new std::function<void(ThreadGC*, const char*)>(ctx->single);
	}
	SDL_Event ev = {};
	ev.type = SDL_EVENT_DIALOG_RESULT;
	ev.user.data1 = res;
	SDL_PushEvent(&ev);    // ← thread-safe、main loop で拾う

	delete ctx;
}

// main.cpp の SDL_PollEvent ループで呼ぶ。
// 戻り値: true なら本イベントを処理した → 上位はループ継続
inline bool DispatchDialogEvent(SDL_Event* event) {
	if (!event || event->type != SDL_EVENT_DIALOG_RESULT) return false;
	auto* res = (_DialogResult*)event->user.data1;
	if (!res) return true;
	if (res->isMulti) {
		if (res->multiCb) (*res->multiCb)(res->thgc, (const char* const*)res->paths);
		delete res->multiCb;
		if (res->paths) {
			for (int i = 0; i < res->pathCount; i++) SDL_free(res->paths[i]);
			delete[] res->paths;
		}
	} else {
		if (res->singleCb) (*res->singleCb)(res->thgc, res->path);
		delete res->singleCb;
		if (res->path) SDL_free(res->path);
	}
	delete res;
	return true;
}

// ============================================================
// 非同期ファイル読み込み (FileEngine worker → thgc->queue → resume_all)
//
// 設計 (db2.h::Worker と同じパターン):
//   1. LoadFileAsync(thgc, path, cb)
//        - 完了処理を行う Generator コルーチンを生成 (initial_suspend で停止状態)
//        - coroutine handle を抜き取る (Generator が destroy しないように coro={})
//        - FileEngine::readAsync() に handle と結果バッファを渡す
//        - 即 return (UI スレッド ブロックなし)
//   2. FileEngine worker thread:
//        - read 実行 (UI には触らない)
//        - 結果バッファに生バイトを書き込む
//        - thgc->queue->push(handle)  ← UI thread に「再開してくれ」と通知
//   3. UI thread の GoThread ループの thgc->queue->resume_all() が拾い、
//      handle.resume() でコルーチン body を **UI thread 上で** 実行:
//        - GC_alloc_size + Stream 組み立て
//        - ユーザーコールバック (cb) 起動
//        - co_return → final_suspend で handle.destroy() される
//
// → SDL カスタムイベントは使わない。db2 の SQL と同じ機構に揃える。
// → 全ての UI/GC 作業は thgc を所有する UI thread 上で完結する。
// ============================================================

// worker → coroutine 間の配送箱
struct _AsyncFileLoad {
	ThreadGC*            thgc;
	std::vector<uint8_t> data;       // worker が読み込んだ生バイト
	bool                 success = false;
	std::function<void(ThreadGC*, Stream*)>* callback = nullptr;
};

// ── UI thread 上で resume される完了コルーチン ──
// initial_suspend = suspend_always なので、生成時には body は走らない。
// thgc->queue->resume_all() から handle.resume() で初めてここが走る。
inline Generator _RunFileLoadCompletion(_AsyncFileLoad* res) {
	// ここから UI thread
	Stream* s = nullptr;
	if (res->success && !res->data.empty()) {
		s = (Stream*)GC_alloc_size(res->thgc, sizeof(Stream));
		s->data  = (uint8_t*)GC_alloc_size(res->thgc, res->data.size());
		memcpy(s->data, res->data.data(), res->data.size());
		s->size  = res->data.size();
		s->pos   = 0;
		s->owned = true;
	}
	SDL_Log("[LoadFile] complete: success=%d size=%zu s=%p",
		(int)res->success, res->data.size(), (void*)s);
	if (res->callback) (*res->callback)(res->thgc, s);
	delete res->callback;
	delete res;
	co_return (char*)0;
}

#ifdef __ANDROID__
#include <jni.h>
#include <SDL3/SDL_system.h>

// content:// / file:// URI から DISPLAY_NAME (= ファイル名) を取り出す。
// 取得失敗時は空文字を返す。HopStarBBActivity.getDisplayNameForUri を JNI で呼ぶ。
inline std::string androidDisplayNameForUri(const std::string& uri) {
    if (uri.empty()) { SDL_Log("[Title] empty uri"); return {}; }
    JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();
    if (!env) { SDL_Log("[Title] SDL_GetAndroidJNIEnv returned NULL"); return {}; }
    // ワーカースレッドの JNI では FindClass がアプリ class を見つけられないので、
    // SDL の Activity 経由で GetObjectClass する (= activity の class loader を使う).
    jobject activity = (jobject)SDL_GetAndroidActivity();
    if (!activity) { SDL_Log("[Title] SDL_GetAndroidActivity returned NULL"); return {}; }
    jclass cls = env->GetObjectClass(activity);
    env->DeleteLocalRef(activity);
    if (!cls) {
        SDL_Log("[Title] GetObjectClass on activity failed");
        env->ExceptionDescribe(); env->ExceptionClear();
        return {};
    }
    jmethodID mid = env->GetStaticMethodID(cls, "getDisplayNameForUri",
                                            "(Ljava/lang/String;)Ljava/lang/String;");
    if (!mid) {
        SDL_Log("[Title] GetStaticMethodID getDisplayNameForUri failed");
        env->ExceptionDescribe(); env->ExceptionClear();
        env->DeleteLocalRef(cls); return {};
    }
    jstring jUri = env->NewStringUTF(uri.c_str());
    jstring jRes = (jstring)env->CallStaticObjectMethod(cls, mid, jUri);
    env->DeleteLocalRef(jUri);
    env->DeleteLocalRef(cls);
    if (env->ExceptionCheck()) {
        SDL_Log("[Title] CallStaticObjectMethod exception");
        env->ExceptionDescribe(); env->ExceptionClear();
        return {};
    }
    if (!jRes) { SDL_Log("[Title] Java returned null"); return {}; }
    const char* c = env->GetStringUTFChars(jRes, nullptr);
    std::string out = c ? std::string(c) : std::string();
    SDL_Log("[Title] resolved=%s", out.c_str());
    if (c) env->ReleaseStringUTFChars(jRes, c);
    env->DeleteLocalRef(jRes);
    return out;
}
#endif

inline void LoadFileAsync(ThreadGC* thgc,
                          const std::string& path,
                          std::function<void(ThreadGC*, Stream*)> callback)
{
	auto* cb  = new std::function<void(ThreadGC*, Stream*)>(std::move(callback));
	auto* res = new _AsyncFileLoad{ thgc, {}, false, cb };

	// 完了処理コルーチンを準備 (initial_suspend で止まっている)
	Generator g = _RunFileLoadCompletion(res);
	auto handle = g.coro;
	g.coro = {};                                  // ~Generator() で destroy されないように
	handle.promise().queue = thgc->queue;         // どのキューで再開するか覚えさせる

	HopStarIO::FileEngine* eng = getFileEngine();
	if (!eng) {
		// FileEngine 無し → 即座に UI thread のキューへ (失敗として resume させる)
		thgc->queue->push(handle);
		return;
	}

	// path → FileDescriptor (External / URL を自動判定)
	HopStarIO::FileDescriptor desc;
	const std::string& p = path;
	if (p.rfind("http://", 0) == 0 || p.rfind("https://", 0) == 0) {
		desc.path     = p;
		desc.location = HopStarIO::Location::Server;
		desc.access   = HopStarIO::Access::Read;
		desc.address  = p;
	} else {
		desc = eng->fromExternalPath(p, HopStarIO::Access::Read);
	}

	// FileEngine の worker thread にジョブ投入。
	// この lambda は **worker thread 上で実行** されるので UI には触らない。
	// 結果バッファに値を書き込んで、thgc->queue に handle を push するだけ。
	eng->readAsync(desc, [thgc, res, handle](HopStarIO::ReadResult result) {
		// ── worker thread ── (UI には触らない)
		res->data    = std::move(result.data);
		res->success = result.success;
		thgc->queue->push(handle);   // ← UI thread の resume_all() で完了コルーチンが走る
	});
}

// 既定のフィルタ: 全種別カバー
inline const SDL_DialogFileFilter* DefaultOpenFilters(int* outCount) {
	static const SDL_DialogFileFilter filters[] = {
		{ "All Supported",
		  "txt;cy;py;c;cpp;h;hpp;cs;go;rs;java;js;ts;md;json;xml;yaml;yml;"
		  "png;jpg;jpeg;gif;bmp;webp;tga;"
		  "mp4;mov;avi;mkv;webm;m4v;"
		  "mp3;wav;flac;ogg;m4a;aac;opus" },
		{ "Source / Text",
		  "cy;txt;md;json;xml;yaml;yml;py;js;ts;c;cpp;h;hpp;cs;go;rs;java" },
		{ "Images",
		  "png;jpg;jpeg;gif;bmp;webp;tga;tif;tiff" },
		{ "Videos",
		  "mp4;mov;avi;mkv;webm;m4v;flv;wmv" },
		{ "Audios",
		  "mp3;wav;flac;ogg;m4a;aac;opus;wma" },
		{ "All Files", "*" },
	};
	*outCount = (int)(sizeof(filters) / sizeof(filters[0]));
	return filters;
}

// 単一ファイル選択
inline void ShowOpenFileDialog(ThreadGC* thgc,
                               std::function<void(ThreadGC*, const char* path)> onSelected,
                               const char* defaultPath = nullptr,
                               const SDL_DialogFileFilter* filters = nullptr,
                               int filterCount = 0)
{
	auto* ctx = new _DialogCtx{ thgc, std::move(onSelected), {}, /*isMulti=*/false };
	if (!filters) filters = DefaultOpenFilters(&filterCount);
	SDL_Window* win = (thgc && !thgc->windows.empty()) ? thgc->windows[0]->sdlWindow : nullptr;
	SDL_ShowOpenFileDialog(_onDialogResult, ctx, win, filters, filterCount, defaultPath, /*allow_many=*/false);
}

// 複数ファイル選択 (NULL 終端の char** が来る)
inline void ShowOpenFilesDialog(ThreadGC* thgc,
                                std::function<void(ThreadGC*, const char* const* paths)> onSelected,
                                const char* defaultPath = nullptr,
                                const SDL_DialogFileFilter* filters = nullptr,
                                int filterCount = 0)
{
	auto* ctx = new _DialogCtx{ thgc, {}, std::move(onSelected), /*isMulti=*/true };
	if (!filters) filters = DefaultOpenFilters(&filterCount);
	SDL_Window* win = (thgc && !thgc->windows.empty()) ? thgc->windows[0]->sdlWindow : nullptr;
	SDL_ShowOpenFileDialog(_onDialogResult, ctx, win, filters, filterCount, defaultPath, /*allow_many=*/true);
}

// 保存ダイアログ
inline void ShowSaveFileDialog(ThreadGC* thgc,
                               std::function<void(ThreadGC*, const char* path)> onSelected,
                               const char* defaultPath = nullptr,
                               const SDL_DialogFileFilter* filters = nullptr,
                               int filterCount = 0)
{
	auto* ctx = new _DialogCtx{ thgc, std::move(onSelected), {}, /*isMulti=*/false };
	if (!filters) filters = DefaultOpenFilters(&filterCount);
	SDL_Window* win = (thgc && !thgc->windows.empty()) ? thgc->windows[0]->sdlWindow : nullptr;
	SDL_ShowSaveFileDialog(_onDialogResult, ctx, win, filters, filterCount, defaultPath);
}

// フォルダ選択
inline void ShowOpenFolderDialog(ThreadGC* thgc,
                                 std::function<void(ThreadGC*, const char* path)> onSelected,
                                 const char* defaultPath = nullptr)
{
	auto* ctx = new _DialogCtx{ thgc, std::move(onSelected), {}, /*isMulti=*/false };
	SDL_Window* win = (thgc && !thgc->windows.empty()) ? thgc->windows[0]->sdlWindow : nullptr;
	SDL_ShowOpenFolderDialog(_onDialogResult, ctx, win, defaultPath, /*allow_many=*/false);
}
