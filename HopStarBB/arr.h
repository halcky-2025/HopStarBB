#pragma once

struct String {
    char* data;
    int size;    // 文字数（文字列の長さ）
    int esize;   // 1文字あたりのバイト数
};
inline void StringCheck(ThreadGC* thgc, char* data) {
    String* s = (String*)data;
    s->data = (char*)copy_object_young(thgc, s->data);
}
wchar_t GetChar(String* str, int n);
String* createString(ThreadGC* thgc, char* tex, int size, int esize) {
	String* str = (String*)GC_alloc(thgc, _String);
	str->data = (char*)GC_alloc_size(thgc, size * esize);
	str->size = size;
    str->esize = esize;
	memcpy(str->data, tex, size * esize);
	return str;
}
String* createString(ThreadGC* thgc, const char* tex, int size, int esize) {
	return createString(thgc, (char*)tex, size, esize);
}
String* createStringant(ThreadGC* thgc, char* tex, int size, int esize) {
    String* str = (String*)GC_alloc_ant(thgc, _String);
    if (str == NULL) return NULL;
    str->data = (char*)GC_alloc_size_ant(thgc, size * esize);
    if (str == NULL) return NULL;
    str->size = size;
    str->esize = esize;
    memcpy(str->data, tex, size * esize);
    return str;
}
String* StringAdd(ThreadGC* thgc, String* str, char* value, int len, int esize) {
    String* ret = (String*)GC_alloc(thgc, _String);
    if (str->esize == esize) {
        ret->esize = esize;
        ret->size = str->size + len;
        ret->data = (char*)GC_alloc_size(thgc, ret->size * ret->esize);
        memcpy(ret->data, str->data, str->size * str->esize);
        memcpy((char*)(ret->data + str->size * str->esize), value, len * esize);
    }
    else {
        ret->esize = 2;
        ret->size = str->size + len;
        ret->data = (char*)GC_alloc_size(thgc, ret->size * 2);
        if (str->esize == 1) {
            for (int i = 0; i < str->size; i++) {
				ret->data[i * 2] = str->data[i];
				ret->data[i * 2 + 1] = 0;
            }
            memcpy((char*)(ret->data + str->size * 2), value, len * 2);
        }
        else {
            memcpy(ret->data, str->data, str->size * 2);
            for (int i = 0; i < len; i++) {
                ret->data[(str->size + i) * 2] = value[i];
                ret->data[(str->size + i) * 2 + 1] = 0;
            }
        }
    }
    return ret;
}
String* StringAdd2(ThreadGC* thgc, String* str, String* value) {
    String* ret = (String*)GC_alloc(thgc, _String);
    if (str->esize == value->esize) {
        ret->esize = str->esize;
        ret->size = str->size + value->size;
        ret->data = (char*)GC_alloc_size(thgc, ret->size * ret->esize);
        memcpy(ret->data, str->data, str->size * str->esize);
        memcpy((char*)(ret->data + str->size * str->esize), value->data, value->size * value->esize);
    }
    else {
        ret->esize = 2;
        ret->size = str->size + value->size;
        ret->data = (char*)GC_alloc_size(thgc, ret->size * 2);
        if (str->esize == 1) {
            for (int i = 0; i < str->size; i++) {
                ret->data[i * 2] = str->data[i];
                ret->data[i * 2 + 1] = 0;
            }
            memcpy((char*)(ret->data + str->size * 2), value->data, value->size * 2);
        }
        else {
            memcpy(ret->data, str->data, str->size * 2);
            for (int i = 0; i < value->size; i++) {
                ret->data[(str->size + i) * 2] = value->data[i];
                ret->data[(str->size + i) * 2 + 1] = 0;
            }
        }
    }
    return ret;
}
// C# String.Replace(oldStr, newStr) — count=0 for all, >0 for max replacements
String* StringReplace(ThreadGC* thgc, String* str, String* old_s, String* new_s, int count = INT_MAX) {
	int es = str->esize;
	if (old_s->esize > es) es = old_s->esize;
	if (new_s->esize > es) es = new_s->esize;
	// 1st pass: collect match positions
	std::vector<int> pos;
	for (int i = 0; i <= str->size - old_s->size; i++) {
		bool match = true;
		for (int j = 0; j < old_s->size; j++) {
			if (GetChar(str, i + j) != GetChar(old_s, j)) { match = false; break; }
		}
		if (match) {
			pos.push_back(i);
			if ((int)pos.size() >= count) break;
			i += old_s->size - 1;
		}
	}
	if (pos.empty()) { return str; }
	// 2nd pass: build result using recorded positions
	int ns = str->size + (int)pos.size() * (new_s->size - old_s->size);
	String* ret = (String*)GC_alloc(thgc, _String);
	ret->esize = es; ret->size = ns;
	ret->data = (char*)GC_alloc_size(thgc, ns * es);
	int wi = 0, prev = 0;
	if (str->esize == es && new_s->esize == es) {
		// fast path: all same esize (1-1-1 or 2-2-2), use memcpy
		for (int pi = 0; pi < (int)pos.size(); pi++) {
			int gap = pos[pi] - prev;
			if (gap > 0) { memcpy(ret->data + wi * es, str->data + prev * es, gap * es); wi += gap; }
			if (new_s->size > 0) { memcpy(ret->data + wi * es, new_s->data, new_s->size * es); wi += new_s->size; }
			prev = pos[pi] + old_s->size;
		}
		int tail = str->size - prev;
		if (tail > 0) { memcpy(ret->data + wi * es, str->data + prev * es, tail * es); wi += tail; }
	} else {
		// slow path: esize mismatch (es == 2), per-char widen via GetChar
		for (int pi = 0; pi < (int)pos.size(); pi++) {
			for (int i = prev; i < pos[pi]; i++) {
				wchar_t c = GetChar(str, i);
				ret->data[wi * 2] = (char)(c & 0xFF); ret->data[wi * 2 + 1] = (char)(c >> 8);
				wi++;
			}
			for (int j = 0; j < new_s->size; j++) {
				wchar_t c = GetChar(new_s, j);
				ret->data[wi * 2] = (char)(c & 0xFF); ret->data[wi * 2 + 1] = (char)(c >> 8);
				wi++;
			}
			prev = pos[pi] + old_s->size;
		}
		for (int i = prev; i < str->size; i++) {
			wchar_t c = GetChar(str, i);
			ret->data[wi * 2] = (char)(c & 0xFF); ret->data[wi * 2 + 1] = (char)(c >> 8);
			wi++;
		}
	}
	return ret;
}
String* StringRemove(ThreadGC* thgc, String* str, int n) {
	String* ret = (String*)GC_alloc(thgc, _String);
    ret->esize = str->esize;
    ret->size = str->size - 1;
	ret->data = (char*)GC_alloc_size(thgc, ret->size * ret->esize);
    memcpy(ret->data, str->data, n * str->esize);
    memcpy((char*)(ret->data + n * str->esize), str->data + (n + 1) * str->esize, (str->size - (n + 1)) * str->esize);
    return ret;
}
String* SubString(ThreadGC* thgc, String* str, int n1, int length) {
	String* ret = (String*)GC_alloc(thgc, _String);
	ret->esize = str->esize;
	ret->size = length;
	ret->data = (char*)GC_alloc_size(thgc, ret->size * ret->esize);
    memcpy(ret->data, str->data + n1 * str->esize, length * str->esize);
	return ret;
}
wchar_t GetChar(String* str, int n) {
    if (str->esize == 1) {
        wchar_t wc = (unsigned char)(*(str->data + n));
        return wc;
    }
    else {
        wchar_t wc = ((unsigned char)*(str->data + n * 2 + 1) << 8) |
            (unsigned char)*(str->data + n * 2);
        return wc;
    }
}
static inline size_t utf8_len_from_codepoint(uint32_t cp) {
    if (cp <= 0x7F)   return 1;
    if (cp <= 0x7FF)  return 2;
    if (cp <= 0xFFFF) return 3;
    return 4;
}

static inline char* utf8_write_codepoint(uint32_t cp, char* w) {
    if (cp <= 0x7F) {
        *w++ = static_cast<char>(cp);
    }
    else if (cp <= 0x7FF) {
        *w++ = static_cast<char>(0xC0 | (cp >> 6));
        *w++ = static_cast<char>(0x80 | (cp & 0x3F));
    }
    else if (cp <= 0xFFFF) {
        *w++ = static_cast<char>(0xE0 | (cp >> 12));
        *w++ = static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        *w++ = static_cast<char>(0x80 | (cp & 0x3F));
    }
    else {
        *w++ = static_cast<char>(0xF0 | (cp >> 18));
        *w++ = static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        *w++ = static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        *w++ = static_cast<char>(0x80 | (cp & 0x3F));
    }
    return w;
}

/* Latin-1 → UTF-8（正確な長さでmalloc） */
char* latin1_to_utf8_malloc_exact(const char* data, int size, int* out_len = nullptr) {
    if (!data || size < 0) throw std::runtime_error("latin1_to_utf8: invalid input");
    const unsigned char* s = reinterpret_cast<const unsigned char*>(data);

    // 1st pass: 必要バイト数
    size_t need = 0;
    for (int i = 0; i < size; ++i)
        need += (s[i] < 0x80) ? 1u : 2u;

    // allocate
    char* buf = static_cast<char*>(std::malloc(need + 1));
    if (!buf) throw std::bad_alloc();

    // 2nd pass: 実書き込み
    char* w = buf;
    for (int i = 0; i < size; ++i) {
        unsigned c = s[i];
        if (c < 0x80) {
            *w++ = static_cast<char>(c);
        }
        else {
            *w++ = static_cast<char>(0xC0 | (c >> 6));
            *w++ = static_cast<char>(0x80 | (c & 0x3F));
        }
    }
    *w = '\0';
    if (out_len) *out_len = static_cast<int>(w - buf);
    return buf;
}

/* UTF-16LE（charCountは文字数）→ UTF-8（正確な長さでmalloc） */
char* utf16le_to_utf8_malloc_exact(const char* data, int charCount, int* out_len = nullptr) {
    int byteSize = charCount * 2;
    if (!data || charCount < 0)
        throw std::runtime_error("utf16le_to_utf8: invalid input size");
    const unsigned char* p = reinterpret_cast<const unsigned char*>(data);
    const size_t len16 = static_cast<size_t>(charCount);

    // 1st pass: 必要バイト数
    size_t need = 0;
    for (size_t i = 0; i < len16; ++i) {
        uint16_t w1 = static_cast<uint16_t>(p[2 * i]) | (static_cast<uint16_t>(p[2 * i + 1]) << 8);
        if (0xD800 <= w1 && w1 <= 0xDBFF) { // high surrogate
            if (i + 1 >= len16) throw std::runtime_error("Unpaired high surrogate");
            uint16_t w2 = static_cast<uint16_t>(p[2 * (i + 1)]) | (static_cast<uint16_t>(p[2 * (i + 1) + 1]) << 8);
            if (!(0xDC00 <= w2 && w2 <= 0xDFFF)) throw std::runtime_error("Invalid low surrogate");
            uint32_t cp = ((uint32_t(w1) - 0xD800) << 10) + (uint32_t(w2) - 0xDC00) + 0x10000;
            need += utf8_len_from_codepoint(cp);
            ++i; // 低位を消費
        }
        else if (0xDC00 <= w1 && w1 <= 0xDFFF) {
            throw std::runtime_error("Unpaired low surrogate");
        }
        else {
            need += utf8_len_from_codepoint(w1);
        }
    }

    // allocate
    char* buf = static_cast<char*>(std::malloc(need + 1));
    if (!buf) throw std::bad_alloc();

    // 2nd pass: 実書き込み
    char* w = buf;
    for (size_t i = 0; i < len16; ++i) {
        uint16_t w1 = static_cast<uint16_t>(p[2 * i]) | (static_cast<uint16_t>(p[2 * i + 1]) << 8);
        uint32_t cp;
        if (0xD800 <= w1 && w1 <= 0xDBFF) {
            uint16_t w2 = static_cast<uint16_t>(p[2 * (i + 1)]) | (static_cast<uint16_t>(p[2 * (i + 1) + 1]) << 8);
            cp = ((uint32_t(w1) - 0xD800) << 10) + (uint32_t(w2) - 0xDC00) + 0x10000;
            ++i;
        }
        else {
            cp = w1;
        }
        w = utf8_write_codepoint(cp, w);
    }
    *w = '\0';
    if (out_len) *out_len = static_cast<int>(w - buf);
    return buf;
}

char* StringUTF8(String* s, int* out_len) {
    if (!s) throw std::runtime_error("StringUTF8: null");
    if (s->esize == 1) return latin1_to_utf8_malloc_exact(s->data, s->size, out_len);
    if (s->esize == 2) return utf16le_to_utf8_malloc_exact(s->data, s->size, out_len);
    throw std::runtime_error("Unsupported esize: " + std::to_string(s->esize));
}

// UTF-8 バイト列 → String*  (StringUTF8 の逆方向)
//   - 全部 ASCII (< 0x80) なら esize=1, size=byteSize でそのまま (1バイト=1文字)
//   - 非 ASCII (日本語など) を含むなら UTF-16LE に変換して esize=2, size=コード単位数
//
//   ※ String 構造体は size = 文字数 (コード単位数), esize = 1 文字あたりのバイト数
//      なので UTF-8 を生バイトで esize=1 として扱うのは誤り
//      (日本語1文字が3バイトだと size が 3 倍になってしまう)
String* StringFromUTF8(ThreadGC* thgc, const char* utf8, int byteSize) {
    if (!utf8 || byteSize <= 0) return createString(thgc, (char*)"", 0, 1);
    const unsigned char* d = reinterpret_cast<const unsigned char*>(utf8);
    size_t n = static_cast<size_t>(byteSize);

    // ASCII チェック (高速パス)
    bool ascii = true;
    for (size_t i = 0; i < n; i++) {
        if (d[i] >= 0x80) { ascii = false; break; }
    }
    if (ascii) return createString(thgc, (char*)d, byteSize, 1);

    // UTF-8 → UTF-16LE (esize=2)
    // 出力上限: 4バイト UTF-8 → サロゲートペア (2 ワイド) なので n ワイド以下
    // 安全のため n*2 ワイド (= n*4 バイト) 取る
    String* str = (String*)GC_alloc(thgc, _String);
    str->esize = 2;
    str->data = (char*)GC_alloc_size(thgc, n * 2 * sizeof(uint16_t));
    uint16_t* out = reinterpret_cast<uint16_t*>(str->data);
    size_t w = 0;
    for (size_t i = 0; i < n; ) {
        uint32_t cp;
        unsigned char c = d[i];
        if (c < 0x80) {
            cp = c; i++;
        } else if ((c & 0xE0) == 0xC0 && i + 1 < n) {
            cp = ((uint32_t)(c & 0x1F) << 6) | (d[i + 1] & 0x3F);
            i += 2;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < n) {
            cp = ((uint32_t)(c & 0x0F) << 12)
               | ((uint32_t)(d[i + 1] & 0x3F) << 6)
               | (d[i + 2] & 0x3F);
            i += 3;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < n) {
            cp = ((uint32_t)(c & 0x07) << 18)
               | ((uint32_t)(d[i + 1] & 0x3F) << 12)
               | ((uint32_t)(d[i + 2] & 0x3F) << 6)
               | (d[i + 3] & 0x3F);
            i += 4;
        } else {
            cp = 0xFFFD;  // 不正シーケンス → REPLACEMENT CHARACTER
            i++;
        }
        if (cp < 0x10000) {
            out[w++] = static_cast<uint16_t>(cp);
        } else {
            // BMP 外 → サロゲートペア
            cp -= 0x10000;
            out[w++] = static_cast<uint16_t>(0xD800 | (cp >> 10));
            out[w++] = static_cast<uint16_t>(0xDC00 | (cp & 0x3FF));
        }
    }
    str->size = static_cast<int>(w);
    return str;
}

// UTF-16LE ワイド文字列 → String*
//   - 全部 ASCII (< 0x80) なら esize=1 に縮退
//   - そうでなければ esize=2 のまま
String* StringFromUTF16(ThreadGC* thgc, const uint16_t* w, size_t wlen) {
    if (!w || wlen == 0) return createString(thgc, (char*)"", 0, 1);
    bool ascii = true;
    for (size_t i = 0; i < wlen; i++) {
        if (w[i] >= 0x80) { ascii = false; break; }
    }
    if (ascii) {
        char* out = (char*)GC_alloc_size(thgc, wlen);
        for (size_t i = 0; i < wlen; i++) out[i] = static_cast<char>(w[i]);
        return createString(thgc, out, static_cast<int>(wlen), 1);
    }
    String* str = (String*)GC_alloc(thgc, _String);
    str->esize = 2;
    str->data = (char*)GC_alloc_size(thgc, wlen * 2);
    memcpy(str->data, w, wlen * 2);
    str->size = static_cast<int>(wlen);
    return str;
}
String* NumberString(ThreadGC* thgc, int n) {
    String* ret = (String*)GC_alloc(thgc, _String);
    ret->esize = 1;
    int count = 0;
    int n2 = n;
    for (;;) {
        count++;
        n2 /= 10;
        if (n2 == 0) break;
    }
    if (n < 0) {
        ret->size = count + 1;
        ret->data = (char*)GC_alloc_size(thgc, ret->size);
        ret->data[0] = '-';
        n2 = -n;
    }
    else {
        ret->size = count;
        ret->data = (char*)GC_alloc_size(thgc, ret->size);
        n2 = n;
    }
    for (int i = 1; ;i++) {
        ret->data[ret->size - i] = '0' + n2 % 10;
        n2 /= 10;
        if (n2 == 0) break;
    }
    return ret;
}
String* FloatString(ThreadGC* thgc, float n, int dlength) {
    String* ret = (String*)GC_alloc(thgc, _String);
    ret->esize = 1;
    int count = 0;
    int n2 = (int)n;
    for (;;) {
        count++;
        n2 /= 10;
    }
    count += dlength;
    if (n < 0) {
        ret->size = count + 2;
        ret->data = (char*)GC_alloc_size(thgc, ret->size);
        ret->data[0] = '-';
        n = -n;
    }
    else {
        ret->size = count + 1;
        ret->data = (char*)GC_alloc_size(thgc, ret->size);
    }
    n2 = (int)n;
    float f2 = n - (float)(int)n2;
    for (int i = 0; i < dlength;) {
        f2 *= 10;
        ret->data[ret->size - dlength + i] = (int)f2 + '0';
        f2 = f2 - (float)(int)f2;
    }
    ret->data[ret->size - dlength - 1] = '.';
    for (int i = dlength + 2; ; i++) {
        ret->data[ret->size - i] = '0' + n2 % 10;
        n2 /= 10;
        if (n2 == 0) break;
    }
    return ret;
}
void PrintString(String* str) {
    if (str->esize == 1) {
        for (int i = 0; i < str->size; i++) {
            putchar(str->data[i]);
        }
    }
    else {
        for (int i = 0; i < str->size; i++) {
            wchar_t result;
            memcpy(&result, str->data + i * 2, sizeof(wchar_t));
            putwchar(result);
        }
    }
}
int StringNumber(String* str) {
	int n = 0;
	if (str->esize == 1) {
		for (int i = 0; i < str->size; i++) {
			n = n * 10 + (str->data[i] - '0');
		}
	}
	else {
		for (int i = 0; i < str->size; i++) {
			wchar_t wc;
			memcpy(&wc, str->data + i * 2, sizeof(wchar_t));
			n = n * 10 + (wc - '0');
		}
	}
	return n;
}
bool StringEqual(char* str1, int len1, int esize1,
    char* str2, int len2, int esize2) {
    if (str1 == str2) return true;
    // NULLポインタチェック
    if (str1 == NULL || str2 == NULL) {
        return false;
    }
    if (len1 != len2) return false;
    // 同じエンコーディングの場合
    if (esize1 == esize2) {
        for (int i = 0; i < len1 * esize1; i++) {
            if (str1[i] != str2[i]) return false;
        }
        return true;
    }
    else {

        char* ascii_str;
        uint16_t* utf16_str;
        size_t ascii_len, utf16_len_words;

        if (esize1 == 1) {
            ascii_str = str1;
            utf16_str = (uint16_t*)str2;
        }
        else {
            ascii_str = str2;
            utf16_str = (uint16_t*)str1;
        }
        for (int i = 0; i < len1; i++) {
            // UTF-16文字列から16ビット文字を取得
            uint16_t utf16_char = (utf16_str)[i];

            // UTF-16文字が ASCII範囲外（0-127）なら一致しない
            if (utf16_char > 0xFF) {
                return false;
            }

            // UTF-16の下位バイトとASCII文字を比較
            if ((char)utf16_char != ascii_str[i]) {
                return false;
            }
        }
        return true;
    }

}
bool StringEqual2(String* str1, String* str2) {
	return StringEqual(str1->data, str1->size, str1->esize,
		str2->data, str2->size, str2->esize);
}
static inline bool str_equals(const char* lit, String* s) {
	if (s == NULL) return false;
	int len = (int)strlen(lit);
	if (s->size != len) return false;
	if (s->esize == 1) return memcmp(s->data, lit, len) == 0;
	for (int i = 0; i < len; i++) {
		if (s->data[i * 2] != lit[i] || s->data[i * 2 + 1] != 0) return false;
	}
	return true;
}
static inline bool str_equals(String* s, const char* lit) {
	return str_equals(lit, s);
}
static inline bool str_equals(String* s1, String* s2) {
	if (s1 == s2) return true;
	if (s1 == NULL || s2 == NULL) return false;
	return StringEqual2(s1, s2);
}
// C# int.TryParse相当 — \0終端でないStringを安全にint変換
static inline bool str_try_parse_int(String* s, int* out) {
	if (s == NULL || s->size == 0) return false;
	int val = 0;
	for (int i = 0; i < s->size; i++) {
		char c;
		if (s->esize == 1) {
			c = s->data[i];
		} else {
			if (s->data[i * 2 + 1] != 0) return false;
			c = s->data[i * 2];
		}
		if (c < '0' || c > '9') return false;
		val = val * 10 + (c - '0');
	}
	*out = val;
	return true;
}
struct List {
    char** data;
    int size;
    int capa;
    int esize;
    CType dtype;
};
void ListCheck(ThreadGC* thgc, char* data) {
    List* list = (List*)data;
    list->data = (char**)copy_object_young(thgc, (char*)list->data);
    if (list->dtype) {
        for (int i = 0; i < list->size; i++) {
            char** value = (char**)((char*)list->data + i * list->esize);
            *value = (char*)copy_object_young(thgc, *value);
        }
    }
}
List* create_list(ThreadGC* thgc, int esize, CType type) {
    List* list = (List*)GC_alloc(thgc, _List);
    list->dtype = type;
    list->data = (char **)GC_alloc_size(thgc, 4 * esize);
    list->size = 0;
    list->capa = 4;
    list->esize = esize;
    return list;
}
// capa 指定版 (= 旧 create_list は DLL 経由の固定 sig で呼ばれるため、別 overload で追加)。
inline List* create_list(ThreadGC* thgc, int esize, CType type, int capa) {
    if (capa < 1) capa = 1;
    List* list = (List*)GC_alloc(thgc, _List);
    list->dtype = type;
    list->data = (char **)GC_alloc_size(thgc, (size_t)capa * (size_t)esize);
    list->size = 0;
    list->capa = capa;
    list->esize = esize;
    return list;
}
// 既存 List の capa を newCapa に変更。size は維持 (newCapa < size の時は size を切り詰める)。
// data buffer を新しく GC_alloc_size して内容を memcpy。旧 buffer は GC が後で回収。
inline void changeCapa(ThreadGC* thgc, List* list, int newCapa) {
    if (newCapa < 1) newCapa = 1;
    if (list->capa == newCapa) return;
    char** data = (char**)GC_alloc_size(thgc, (size_t)newCapa * (size_t)list->esize);
    int copySize = (list->size < newCapa) ? list->size : newCapa;
    if (copySize > 0) memcpy(data, list->data, (size_t)copySize * (size_t)list->esize);
    list->data = data;
    list->capa = newCapa;
    if (list->size > newCapa) list->size = newCapa;
}
List* create_list2(ThreadGC* thgc, List* list, int esize, CType type) {
    list->data = (char**)GC_alloc(thgc, _List);
    list->size = 0;
    list->capa = 4;
    list->esize = esize;
	list->dtype = type;
    return list;
}
List* create_list_ant(ThreadGC* thgc, int esize, CType type) {
    List* list = (List*)GC_alloc_ant(thgc, _List);
	if (list == NULL) return NULL;
    list->dtype = type;
    list->data = (char**)GC_alloc_size_ant(thgc, 4 * esize);
	if (list->data == NULL) return NULL;
    list->size = 0;
    list->capa = 4;
    list->esize = esize;
    return list;
}
List* create_list2_ant(ThreadGC* thgc, List* list, int esize, CType type) {
    list->data = (char**)GC_alloc_ant(thgc, _List);
	if (list->data == NULL) return NULL;
    list->size = 0;
    list->capa = 4;
    list->esize = esize;
    list->dtype = type;
    return list;
}
// ============================================================
// リストへの書き込み: 2種類の API
//   ポインタリスト用 (esize == sizeof(char*)):
//     add_list / fill_list / add_list_ant   -- value はポインタそのものを直接格納
//   値型リスト用 (int, struct, enum など任意の esize):
//     add_list_v / fill_list_v              -- src は source データへの const void*。
//                                              esize バイトを memcpy。src==NULL でゼロ埋め。
// 関数の戻り値 (rvalue) をそのまま add_list に渡せるのはポインタ版だけ。
// 値型は必ず一時変数に入れて &tmp を add_list_v に渡すこと。
// ============================================================

void fill_list(ThreadGC* thgc, List* list, int count, char* value) {
    int need = list->size + count;
    if (need > list->capa) {
        int new_capa = list->capa;
        while (new_capa < need) new_capa *= 2;
        char** data = (char**)GC_alloc_size(thgc, (size_t)new_capa * (size_t)list->esize);
        memcpy(data, list->data, (size_t)list->size * (size_t)list->esize);
        list->data = data;
        list->capa = new_capa;
    }
    for (int i = 0; i < count; i++) {
        char* target = (char*)list->data + (size_t)(list->size + i) * (size_t)list->esize;
        memcpy(target, &value, list->esize);
    }
    list->size += count;
}
void add_list(ThreadGC* thgc, List* list, char* value) {
    if (list->size == list->capa) {
        list->capa *= 2;
        char** data = (char**)GC_alloc_size(thgc, list->capa * list->esize);
        memcpy(data, list->data, list->size * list->esize);
        list->data = data;
    }
    char* target = (char*)list->data + list->size * list->esize;
    memcpy(target, &value, list->esize);
    list->size++;
}
char* add_list_ant(ThreadGC* thgc, List* list, char* value) {
    if (list->size == list->capa) {
        char** data = (char**)GC_alloc_size_ant(thgc, list->capa * list->esize * 2);
        if (data == NULL) return NULL;
        list->capa *= 2;
        memcpy(data, list->data, list->size * list->esize);
        list->data = data;
    }
    char* target = (char*)list->data + list->size * list->esize;
    memcpy(target, &value, list->esize);
    list->size++;
    return (char*)list;
}

// --- 値型リスト用 ----------------------------------------------------------

void fill_list_v(ThreadGC* thgc, List* list, int count, const void* src) {
    int need = list->size + count;
    if (need > list->capa) {
        int new_capa = list->capa;
        while (new_capa < need) new_capa *= 2;
        char** data = (char**)GC_alloc_size(thgc, (size_t)new_capa * (size_t)list->esize);
        memcpy(data, list->data, (size_t)list->size * (size_t)list->esize);
        list->data = data;
        list->capa = new_capa;
    }
    for (int i = 0; i < count; i++) {
        char* target = (char*)list->data + (size_t)(list->size + i) * (size_t)list->esize;
        memcpy(target, src, (size_t)list->esize);
    }
    list->size += count;
}
void add_list_v(ThreadGC* thgc, List* list, const void* src) {
    if (list->size == list->capa) {
        list->capa *= 2;
        char** data = (char**)GC_alloc_size(thgc, list->capa * list->esize);
        memcpy(data, list->data, list->size * list->esize);
        list->data = data;
    }
    char* target = (char*)list->data + (size_t)list->size * (size_t)list->esize;
    memcpy(target, src, (size_t)list->esize);
    list->size++;
}
void remove_list(ThreadGC* thgc, List* list, int index) {
    if (index < 0 || index >= list->size) return;

    // 前詰め（index 以降を1つ左へ）
    char* dst = (char*)list->data + (size_t)index * (size_t)list->esize;
    char* src = (char*)list->data + (size_t)(index + 1) * (size_t)list->esize;
    size_t tail_bytes = (size_t)(list->size - index - 1) * (size_t)list->esize;
    if (tail_bytes > 0) {
        memmove(dst, src, tail_bytes);
    }
    // 要素数を減らす
    list->size--;

    // 小さくしすぎない範囲で容量縮小（capa は最小 4 を維持）
    if (list->capa > 4 && list->size <= list->capa / 4) {
        int new_capa = list->capa / 4;
        if (new_capa < 4) new_capa = 4;
        // esize * new_capa バイト確保してコピー
        char** new_data = (char**)GC_alloc_size(thgc, (size_t)new_capa * (size_t)list->esize);
        // 縮小確保に失敗しても動作継続（容量変更だけ諦める）
        if (new_data) {
            memcpy(new_data, list->data, (size_t)list->size * (size_t)list->esize);
            list->data = new_data;
            list->capa = new_capa;
        }
    }
}
char** get_list(List* list, int index) {
    if (index < 0 || index >= list->size) {
        return NULL;
    }
    return (char**)((char*)list->data + index * list->esize);
}
void remove_list_val(ThreadGC* thgc, List* list, char* val) {
    for (int i = 0; i < list->size; i++) {
        char* gv = *get_list(list, i);
        if (gv == val) {
            remove_list(thgc, list, i);
            return;
        }
    }
}
char** get_last(List* list) {
    return get_list(list, list->size - 1);
}
char** get_list2(List* list, int index) {
    return (char**)((char*)list->data + index * list->esize);
}
void set_list(List* list, int index, char* value) {
    if (index < 0 || index >= list->size) {
        return;
    }
    char** set = (char**)((char*)list->data + index * list->esize);
    *set = value;
}
void set_list2(List* list, int index, char* value) {
    char** set = (char**)((char*)list->data + index * list->esize);
    *set = value;
}
void set_last(List* list, char* value) {
    set_list(list, list->size - 1, value);
}
typedef struct {
    int n;
    String* key;
    char* value;
    CType type;
}KV;
// key は Map context (= Map::keyIsInt) が必要なため MapCheck 側で trace する。
// ここでは value のみ。
void KVCheck(ThreadGC* thgc, char* data) {
	KV* kv = (KV*)data;
	if (kv->type) {
		kv->value = (char*)copy_object_young(thgc, kv->value);
	}
}
typedef struct {
    List** _map;
    int capa;
} MapData;
void MapDataCheck(ThreadGC* thgc, char* data) {
	MapData* map = (MapData*)data;
    map->_map = (List**)copy_object_young(thgc, (char*)map->_map);
	for (int i = 0; i < map->capa; i++) {
        map->_map[i] = (List*)copy_object_young(thgc, (char*)map->_map[i]);
	}
}
struct Map{
    List* kvs;
    MapData* md;
    CType dtype;
    // key の種別: false = String* (= add_mapy 経由)、true = (intptr_t)integer (= add_mapyn 経由)。
    // KV.key スロットを共有しているため、外から見ただけでは判定不可なのでここで明示する。
    // driver が Map 表示 / Map キー検索する際の分岐に使う。
    bool keyIsInt;
};
void MapCheck(ThreadGC* thgc, char* data) {
	Map* map = (Map*)data;
	map->md = (MapData*)copy_object_young(thgc, (char*)map->md);
	// String key の場合のみ、各 KV の key を forward する。
	// 整数キー (= add_mapyn 経由 (intptr_t) cast) は GC ポインタではないので素通し。
	// KVCheck は value のみ扱うため、key は Map context をもつここで処理する。
	//
	// 重要: map->kvs を forward する「前」に from-space buffer 経由で KV を巡回する。
	// 後だと map->kvs->data が to-space に書き換わり、to-space pointer を更に
	// copy_object_young に渡すことになる (= forwarding stub が貼られていないため
	// 二重コピー無限再帰 → stack overflow)。
	if (!map->keyIsInt && map->kvs != NULL) {
		List* kvs_fs = map->kvs;  // from-space List body (= まだ forward されていない)
		for (int i = 0; i < kvs_fs->size; i++) {
			KV** slot = (KV**)((char*)kvs_fs->data + i * kvs_fs->esize);
			KV* old_kv = *slot;
			if (old_kv == NULL) continue;
			KV* new_kv = (KV*)copy_object_young(thgc, (char*)old_kv);
			if (new_kv->key != NULL) {
				new_kv->key = (String*)copy_object_young(thgc, (char*)new_kv->key);
			}
		}
	}
	map->kvs = (List*)copy_object_young(thgc, (char*)map->kvs);
}
MapData* create_map_data(ThreadGC* thgc, int capa, CType type) {
    MapData* md = (MapData*)GC_alloc(thgc, _MapData);
    md->capa = capa;
    md->_map = (List**)GC_alloc_size(thgc, capa * sizeof(List*));
    for (int i = 0; i < md->capa; i++) {
        // create_list は GC を起こしうるが、md は root されているので md->_map のアドレスも更新済 (= md 経由で再評価)
        md->_map[i] = create_list(thgc, sizeof(KV*), type);
    }
    return md;
}
MapData* create_map_data_ant(ThreadGC* thgc, int capa, CType type) {
    MapData* map = (MapData*)GC_alloc_ant(thgc, _MapData);
    if (map == NULL) return NULL;
    map->capa = capa;
    map->_map = (List**)GC_alloc_size_ant(thgc, capa * sizeof(List*));
	if (map->_map == NULL) return NULL;
    for (int i = 0; i < map->capa; i++) {
        if ((map->_map[i] = create_list_ant(thgc, sizeof(KV*), type)) == NULL) return NULL;
    }
    return map;
}
void PrintArray(Map* map, int depth, int type) {
    putchar('[');
	List* list = map->kvs;
    for (int i = 0; i < list->size; i++) {
		if (i != 0) putchar(',');
        KV* kv = (KV*)*get_list2(list, i);
        if (kv->key != NULL) {
            putchar('#');
            PrintString(kv->key);
        }
        putchar(' ');
        if (depth == 0) {
			if (type == 0) {
				printf("%s", kv->value);
			}
			else if (type == 1) {
				printf("%d", kv->value);
			}
			else if (type == 2) {
				printf("%f", kv->value);
			}
		}
        else {
            if (kv->value == NULL) printf("null");
			else PrintArray((Map*)kv->value, depth - 1, type);
        }
    }
    putchar(']');
    putchar('\n');
}
Map* create_mapy(ThreadGC* thgc, CType type) {
    Map* map = (Map*)GC_alloc(thgc, _Map);
	map->dtype = type;
    map->md = create_map_data(thgc, 16, type);
    map->kvs = create_list(thgc, sizeof(KV*), type);
    map->keyIsInt = false;  // add_mapyn 経由で true に切り替わる
    return map;
}
Map* create_mapy_ant(ThreadGC* thgc, CType type) {
    Map* map = (Map*)GC_alloc_ant(thgc, _Map);
    if (map == NULL) return NULL;
    map->dtype = type;
    map->md = create_map_data_ant(thgc, 16, type);
    if (map->md == NULL) return NULL;
    map->kvs = create_list_ant(thgc, sizeof(KV*), type);
	if (map->kvs == NULL) return NULL;
    map->keyIsInt = false;
    return map;
}
int get_hashvalue(String* key, int size) {
    int hash = 10000;
    for (int i = 0; i < key->size; i++) {
        hash ^= (unsigned char)GetChar(key, i);
        hash *= 1619;
        hash %= size;
    }
    return hash;
}
void add_mapy(ThreadGC* thgc, Map* map, String* key, char* value) {
    // value は char* で型不明: GC ポインタかは map->dtype 次第。保守的に root しない (= caller 責任)。
    List* mdl;
head:
    if (key != NULL) {
        mdl = map->md->_map[get_hashvalue(key, map->md->capa)];
        if (mdl->size > 3) {
            map->md = create_map_data(thgc, map->md->capa * 4, map->dtype);
            for (int i = 0; i < map->kvs->size; i++) {
                KV* kv = (KV*)*get_list2(map->kvs, i);
                if (kv->key == NULL) continue;
                List* md2 = map->md->_map[get_hashvalue(kv->key, map->md->capa)];
                add_list(thgc, md2, (char*)kv);
                goto head;
            }
        }
        for (int i = 0; i < mdl->size; i++) {
            KV* kv = (KV*)*get_list2(mdl, i);
            if (kv->key->size != key->size) continue;
            for (int j = 0; ; j++) {
                if (j >= key->size) {
                    kv->value = value;
                    return;
                }
                if (GetChar(kv->key, j) != GetChar(key, j)) break;
            }
        }
        KV* kv2 = (KV*)GC_alloc(thgc, _KV);
        kv2->type = map->dtype;
        kv2->key = key;
        kv2->value = value;
        kv2->n = map->kvs->size;
        add_list(thgc, mdl, (char*)kv2);
        add_list(thgc, map->kvs, (char*)kv2);
    }
    else {
        KV* kv2 = (KV*)GC_alloc(thgc, _KV);
        kv2->type = map->dtype;
        kv2->key = NULL;
        kv2->value = value;
        kv2->n = map->kvs->size;
        add_list(thgc, map->kvs, (char*)kv2);
    }
    return;
}
char* add_mapy_ant(ThreadGC* thgc, Map* map, String* key, char* value) {
    List* mdl;
head:
    if (key != NULL) {
        mdl = map->md->_map[get_hashvalue(key, map->md->capa)];
        if (mdl->size > 3) {
            map->md = create_map_data_ant(thgc, map->md->capa * 4, map->dtype);
            if (map->md == NULL) return NULL;
            for (int i = 0; i < map->kvs->size; i++) {
                KV* kv = (KV*)*get_list2(map->kvs, i);
                if (kv->key == NULL) continue;
                List* md2 = map->md->_map[get_hashvalue(kv->key, map->md->capa)];
                char *o2 = add_list_ant(thgc, md2, (char*)kv);
                if (o2 == NULL) return NULL;
                goto head;
            }
        }
        for (int i = 0; i < mdl->size; i++) {
            KV* kv = (KV*)*get_list2(mdl, i);
            if (kv->key->size != key->size) continue;
            for (int j = 0; ; j++) {
                if (j >= key->size) {
                    kv->value = value;
                    return (char*)map;
                }
                if (GetChar(kv->key, j) != GetChar(key, j)) break;
            }
        }
        KV* kv2 = (KV*)GC_alloc_ant(thgc, _KV);
        if (kv2 == NULL) return NULL;
        kv2->type = map->dtype;
        kv2->key = key;
        kv2->value = value;
        kv2->n = map->kvs->size;
        char * o = add_list_ant(thgc, mdl, (char*)kv2);
		if (o == NULL) return NULL;
        o = add_list_ant(thgc, map->kvs, (char*)kv2);
		if (o == NULL) return NULL;
    }
    else {
        KV* kv2 = (KV*)GC_alloc_ant(thgc, _KV);
		if (kv2 == NULL) return NULL;
        kv2->type = map->dtype;
        kv2->key = NULL;
        kv2->value = value;
        kv2->n = map->kvs->size;
        char *o = add_list_ant(thgc, map->kvs, (char*)kv2);
		if (o == NULL) return NULL;
    }
    return (char*)map;
}
char* get_mapy(Map* map, String* key) {
    List* mdl = map->md->_map[get_hashvalue(key, map->md->capa)];
    for (int i = 0; i < mdl->size; i++) {
        KV* kv = (KV*)*get_list2(mdl, i);
        if (kv->key->size != key->size) continue;
        for (int j = 0; ; j++) {
            if (j >= key->size) {
                return kv->value;
            }
            if (GetChar(kv->key, j) != GetChar(key, j)) break;
        }
    }
    return NULL;
}
int remove_mapy(ThreadGC* thgc, Map* map, String* key) {
    // 通常キー：まずハッシュバケットで該当 KV を探す
    List* mdl = map->md->_map[get_hashvalue(key, map->md->capa)];
    KV* target = NULL;
    int md_index = -1;

    for (int i = 0; i < mdl->size; ++i) {
        KV* kv = (KV*)*get_list2(mdl, i);
        if (!kv || !kv->key) continue;
        if (kv->key->size != key->size) continue;

        // 生バイト比較（既存コード準拠）
        int j = 0;
        for (;; ++j) {
            if (j >= key->size) {
                target = kv;
                md_index = i;
                break;
            }
            if (GetChar(kv->key, j) != GetChar(key, j)) break;
        }
        if (target) break;
        else return 0;
    }

    if (!target) return 0; // 見つからない

    // バケットから削除
    remove_list(thgc, mdl, md_index);

    // kvs からも同じポインタを削除（n を信用してもよいが、安全に線形探索）
    List* kvs = map->kvs;
    remove_list(thgc, kvs, target->n);
    // 後続の n を更新
    for (int j = target->n; j < kvs->size; ++j) {
        KV* kvj = (KV*)*get_list2(kvs, j);
        kvj->n = j;
    }
    return 1;
}
int get_hashvaluen(char* n, int size) {
    uint64_t hash = (uint64_t)10000;
    hash ^= (uint64_t)n;
    hash *= 1619;
    hash %= size;
    return hash;
}
void add_mapyn(ThreadGC* thgc, Map* map, char* key, char* value) {
    // key は (intptr_t) 整数キャスト = 非 GC ポインタ、value も型不明: root しない
    // この関数経由 (= 整数 key)。Map に印を付ける (= driver が key を整数と解釈する)。
    map->keyIsInt = true;
    List* mdl;
head:
    if (key != NULL) {
        mdl = map->md->_map[get_hashvaluen(key, map->md->capa)];
        if (mdl->size > 3) {
            map->md = create_map_data(thgc, map->md->capa * 4, map->dtype);
            for (int i = 0; i < map->kvs->size; i++) {
                KV* kv = (KV*)*get_list2(map->kvs, i);
                if (kv->key == NULL) continue;
                List* md2 = map->md->_map[get_hashvaluen((char*)kv->key, map->md->capa)];
                add_list(thgc, md2, (char*)kv);
            }
            goto head;
        }
        for (int i = 0; i < mdl->size; i++) {
            KV* kv = (KV*)*get_list2(mdl, i);
            if ((char*)kv->key != key) continue;
            else {
                kv->value = value;
                return;
            }
        }
        KV* kv2 = (KV*)GC_alloc(thgc, _KV);
        kv2->type = map->dtype;
        kv2->key = (String*)key;
        kv2->value = value;
        kv2->n = map->kvs->size;
        add_list(thgc, mdl, (char*)kv2);
        add_list(thgc, map->kvs, (char*)kv2);
    }
    else {
        KV* kv2 = (KV*)GC_alloc(thgc, _KV);
        kv2->type = map->dtype;
        kv2->key = NULL;
        kv2->value = value;
        kv2->n = map->kvs->size;
        add_list(thgc, map->kvs, (char*)kv2);
    }
    return;
}
char* add_mapyn_ant(ThreadGC* thgc, Map* map, char* key, char* value) {
    map->keyIsInt = true;
    List* mdl;
head:
    if (key != NULL) {
        mdl = map->md->_map[get_hashvaluen(key, map->md->capa)];
        if (mdl->size > 3) {
            map->md = create_map_data_ant(thgc, map->md->capa * 4, map->dtype);
            if (map->md == NULL) return NULL;
            for (int i = 0; i < map->kvs->size; i++) {
                KV* kv = (KV*)*get_list2(map->kvs, i);
                if (kv->key == NULL) continue;
                List* md2 = map->md->_map[get_hashvaluen((char*)kv->key, map->md->capa)];
                char* o2 = add_list_ant(thgc, md2, (char*)kv);
                if (o2 == NULL) return NULL;
            }
            goto head;
        }
        for (int i = 0; i < mdl->size; i++) {
            KV* kv = (KV*)*get_list2(mdl, i);
            if ((char*)kv->key != key) continue;
            else {
                kv->value = value;
				return (char*)map;
            }
        }
        KV* kv2 = (KV*)GC_alloc_ant(thgc, _KV);
        if (kv2 == NULL) return NULL;
        kv2->type = map->dtype;
        kv2->key = (String*)key;
        kv2->value = value;
        kv2->n = map->kvs->size;
        char* o = add_list_ant(thgc, mdl, (char*)kv2);
        if (o == NULL) return NULL;
        o = add_list_ant(thgc, map->kvs, (char*)kv2);
        if (o == NULL) return NULL;
    }
    else {
        KV* kv2 = (KV*)GC_alloc_ant(thgc, _KV);
        if (kv2 == NULL) return NULL;
        kv2->type = map->dtype;
        kv2->key = NULL;
        kv2->value = value;
        kv2->n = map->kvs->size;
        char* o = add_list_ant(thgc, map->kvs, (char*)kv2);
        if (o == NULL) return NULL;
    }
    return (char*)map;
}
char* get_mapyn(Map* map, char* key) {
    List* mdl = map->md->_map[get_hashvaluen(key, map->md->capa)];
    for (int i = 0; i < mdl->size; i++) {
        KV* kv = (KV*)*get_list2(mdl, i);
        if ((char*)kv->key != key) continue;
        else {
            return kv->value;
		}
    }
    return NULL;
}
int remove_mapyn(ThreadGC* thgc, Map* map, char* key) {
    // 通常キー：まずハッシュバケットで該当 KV を探す
    List* mdl = map->md->_map[get_hashvaluen(key, map->md->capa)];
    KV* target = NULL;
    int md_index = -1;

    for (int i = 0; i < mdl->size; ++i) {
        KV* kv = (KV*)*get_list2(mdl, i);
        if (!kv || !kv->key) continue;
        if ((char*)kv->key != key) continue;
        else {
            target = kv;
            md_index = i;
            break;
        }
    }

    if (!target) return 0; // 見つからない

    // バケットから削除
    remove_list(thgc, mdl, md_index);

    // kvs からも同じポインタを削除（n を信用してもよいが、安全に線形探索）
    List* kvs = map->kvs;
    remove_list(thgc, kvs, target->n);
    // 後続の n を更新
    for (int j = target->n; j < kvs->size; ++j) {
        KV* kvj = (KV*)*get_list2(kvs, j);
        kvj->n = j;
    }
    return 1;
}

// Map の values を List* として返す
static inline List* map_values_list(ThreadGC* thgc, Map* map) {
    if (!map || !map->kvs) return NULL;
    List* result = create_list(thgc, sizeof(char*), map->dtype);
    for (int i = 0; i < map->kvs->size; i++) {
        KV* kv = *(KV**)get_list(map->kvs, i);
        add_list(thgc, result, (char*)kv->value);
    }
    return result;
}

// List の値を直接取得 (ポインタのデリファレンスなし)
static inline char* get_list_val(List* list, int i) {
    return list->data[i];
}

// String* からnull終端C文字列を取得（スタックバッファにコピー不可の場合はmalloc）
static inline char* str_to_cstr(ThreadGC* thgc, String* s) {
    if (!s) return (char*)"";
    char* buf = (char*)GC_alloc_size(thgc, s->size * s->esize + 1);
    memcpy(buf, s->data, s->size * s->esize);
    buf[s->size * s->esize] = '\0';
    return buf;
}

// ============================================================
// 同期プリミティブ (= Atomic / Mutex / CondVar / Event)
//
// クロスプラットフォーム対応:
//   * OS:   Windows / Linux / macOS / iOS / Android
//   * Arch: x64 / ARM (32/64) / RISC-V (32/64)
//
// 設計指針:
//   * 全部「ゼロ初期化で valid」になるよう作る (= GC_alloc / memset 0 / グローバル
//     static どこでも安全に置ける)。これにより独自 GC との相性が良い。
//   * std::atomic / std::mutex / std::condition_variable は使わない
//     - 内部状態の構築が必要で、GC_alloc + cast では UB
//     - GC が move したときに自己参照ポインタが壊れるリスクもある
//     - POSIX pthread_mutex は実装によりゼロ初期化 valid でない (= macOS など)
//   * Atomic はコンパイラ組込み (MSVC: _Interlocked*, GCC/Clang: __atomic_*) で
//     アーキ依存をコンパイラに任せる
//   * Mutex / CondVar / Event は atomic + 「wait_on_address / wake」という
//     futex 風プリミティブで自前実装 (= ゼロ初期化 valid を全 OS で実現)
//
// wait_on_address / wake のプラットフォーム実装:
//   Windows 8+:    WaitOnAddress / WakeByAddressSingle / WakeByAddressAll
//   Linux/Android: futex(2) syscall (= FUTEX_WAIT_PRIVATE / FUTEX_WAKE_PRIVATE)
//   macOS/iOS:     __ulock_wait / __ulock_wake (= XNU 内部 API、libc++ も使用)
// ============================================================

// ------------------------------------------------------------
// プラットフォーム検出
// ------------------------------------------------------------
#if defined(_WIN32)
    #define UARR_OS_WINDOWS 1
    #include <windows.h>
    #include <intrin.h>
    #pragma comment(lib, "Synchronization.lib")
#elif defined(__APPLE__)
    #define UARR_OS_APPLE 1
    #include <stdint.h>
    #include <unistd.h>
    #include <time.h>
    // __ulock_wait / __ulock_wake は private だが libc++/libdispatch も使う stable API
    extern "C" int __ulock_wait(uint32_t op, void* addr, uint64_t value, uint32_t timeout_us);
    extern "C" int __ulock_wake(uint32_t op, void* addr, uint64_t value);
    #ifndef UL_COMPARE_AND_WAIT
        #define UL_COMPARE_AND_WAIT  1
    #endif
    #ifndef ULF_WAKE_ALL
        #define ULF_WAKE_ALL         0x00000100
    #endif
    #ifndef ULF_NO_ERRNO
        #define ULF_NO_ERRNO         0x01000000
    #endif
#elif defined(__linux__) || defined(__ANDROID__)
    #define UARR_OS_LINUX 1
    #include <stdint.h>
    #include <unistd.h>
    #include <time.h>
    #include <linux/futex.h>
    #include <sys/syscall.h>
#else
    #error "Unsupported platform for sync primitives"
#endif

// ------------------------------------------------------------
// AtomicInt (= 符号付き 32bit)
//   ゼロ初期化 = 0。bool 用途は 0=false / 非0=true で兼用。
//   メモリオーダリング: 全部 seq_cst (= 一番強いがアーキ依存の落とし穴を避ける)。
// ------------------------------------------------------------
// すべてのプラットフォームで volatile int (= 32bit signed) で統一。
// Windows の _Interlocked* は volatile long* を取るので呼び出し側でキャスト。
// (Win32 の long は LP64 でも 32bit、size of int == size of long == 4)
struct AtomicInt {
    volatile int v;
};

static inline int atomic_load(const AtomicInt* a) {
#if UARR_OS_WINDOWS
    return a->v;
#else
    return __atomic_load_n(&a->v, __ATOMIC_SEQ_CST);
#endif
}
static inline void atomic_store(AtomicInt* a, int x) {
#if UARR_OS_WINDOWS
    a->v = x;
#else
    __atomic_store_n(&a->v, x, __ATOMIC_SEQ_CST);
#endif
}
static inline int atomic_exchange(AtomicInt* a, int x) {
#if UARR_OS_WINDOWS
    return (int)_InterlockedExchange((volatile long*)&a->v, (long)x);
#else
    return __atomic_exchange_n(&a->v, x, __ATOMIC_SEQ_CST);
#endif
}
// expected と一致したら desired に差し替えて true。失敗時は expected に現在値を書き戻す。
static inline bool atomic_cas(AtomicInt* a, int* expected, int desired) {
#if UARR_OS_WINDOWS
    long prev = _InterlockedCompareExchange((volatile long*)&a->v, (long)desired, (long)*expected);
    bool ok = (prev == (long)*expected);
    *expected = (int)prev;
    return ok;
#else
    return __atomic_compare_exchange_n(&a->v, expected, desired,
                                       false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
}
// 加算前の値を返す (= post-increment 風)。減算は負を渡す。
static inline int atomic_fetch_add(AtomicInt* a, int delta) {
#if UARR_OS_WINDOWS
    return (int)_InterlockedExchangeAdd((volatile long*)&a->v, (long)delta);
#else
    return __atomic_fetch_add(&a->v, delta, __ATOMIC_SEQ_CST);
#endif
}
static inline int atomic_inc(AtomicInt* a) { return atomic_fetch_add(a,  1) + 1; }
static inline int atomic_dec(AtomicInt* a) { return atomic_fetch_add(a, -1) - 1; }

// ------------------------------------------------------------
// AtomicInt64 (= 符号付き 64bit)
//   x64 / ARM64 / RISC-V64 では aligned 8byte もハード atomic。
//   ARM32 / RISC-V32 はコンパイラが LDREXD/STREXD ペアで実装。
// ------------------------------------------------------------
struct AtomicInt64 {
    volatile long long v;
};

static inline long long atomic_load(const AtomicInt64* a) {
#if UARR_OS_WINDOWS
    return (long long)a->v;
#else
    return __atomic_load_n(&a->v, __ATOMIC_SEQ_CST);
#endif
}
static inline void atomic_store(AtomicInt64* a, long long x) {
#if UARR_OS_WINDOWS
    a->v = x;
#else
    __atomic_store_n(&a->v, x, __ATOMIC_SEQ_CST);
#endif
}
static inline long long atomic_exchange(AtomicInt64* a, long long x) {
#if UARR_OS_WINDOWS
    return (long long)_InterlockedExchange64(&a->v, x);
#else
    return __atomic_exchange_n(&a->v, x, __ATOMIC_SEQ_CST);
#endif
}
static inline bool atomic_cas(AtomicInt64* a, long long* expected, long long desired) {
#if UARR_OS_WINDOWS
    long long prev = _InterlockedCompareExchange64(&a->v, desired, *expected);
    bool ok = (prev == *expected);
    *expected = prev;
    return ok;
#else
    return __atomic_compare_exchange_n(&a->v, expected, desired,
                                       false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
}
static inline long long atomic_fetch_add(AtomicInt64* a, long long delta) {
#if UARR_OS_WINDOWS
    return (long long)_InterlockedExchangeAdd64(&a->v, delta);
#else
    return __atomic_fetch_add(&a->v, delta, __ATOMIC_SEQ_CST);
#endif
}

// ============================================================
// AtomicPtr / AtomicPtrT<T> (= ポインタの atomic load/store/exchange/cas)
//
// ポインタ用の atomic は **2 通りの使い方** を提供する:
//
//   (1) AtomicPtr           = void* 専用のラッパ struct (= 旧来からあるシンプル版)
//   (2) AtomicPtrT<T>       = 任意の型 T* を保持する template ラッパ
//   (3) atomic_load_p / store_p / exchange_p / cas_p
//                           = 構造体ラップなしで「生の T* 変数」を直接 atomic 操作する
//                             template 関数
//
// なぜ 3 種類あるのか:
//   * (1) は最小限。void* を入れて取り出すだけ、キャストで使う。
//   * (2) は型安全。AtomicPtrT<Foo> p; なら atomic_load(&p) は Foo* を返す。
//     新規コードはこちらを推奨。
//   * (3) はもっとも自由。既存の構造体メンバ (例: struct Node { Node* next; }) を
//     書き換えずに、その next フィールドを atomic に操作できる:
//         Node* old = atomic_load_p(&node->next);    // ラップ不要、ポインタ直接
//     lock-free linked list 等で必須。
//
// const correctness について:
//   * load 系は読むだけなので、ポインタ自体の constness は許容したい。
//     よって `T* const volatile*` (= 「const ポインタ to volatile T*」) を受け付ける。
//   * store/exchange/cas 系は書き換えるので `T* volatile*` (= mutable) のみ。
//
// 用語の解説 (= C/C++ ポインタ修飾子の読み方):
//   T*                  = T へのポインタ
//   T* volatile         = T へのポインタ (= 値が他スレッドから変更され得る volatile)
//   T* volatile*        = 「T へのポインタ (volatile)」へのポインタ
//                          → 関数引数として渡すとき、そのアドレスにある「T*」を
//                            atomic に load/store する想定
//   T* const volatile*  = 上と同じだが、ポインタ自体は const (= load 専用)
// ============================================================

// ------------------------------------------------------------
// (1) AtomicPtr: void* 専用のラッパ
//
//   メリット:  template でないので #include コスト最小、互換性が高い
//   デメリット: void* で型情報を失うのでキャストが必要
//
//   例:
//     AtomicPtr ap = {};
//     atomic_store(&ap, (void*)myObj);
//     MyObj* obj = (MyObj*)atomic_load(&ap);
// ------------------------------------------------------------
struct AtomicPtr {
    void* volatile v;
};
static inline void* atomic_load(const AtomicPtr* a) {
#if UARR_OS_WINDOWS
    // x64: aligned 8byte ポインタの load は MOV 1 命令で atomic
    return a->v;
#else
    return __atomic_load_n(&a->v, __ATOMIC_SEQ_CST);
#endif
}
static inline void atomic_store(AtomicPtr* a, void* p) {
#if UARR_OS_WINDOWS
    a->v = p;
#else
    __atomic_store_n(&a->v, p, __ATOMIC_SEQ_CST);
#endif
}
static inline void* atomic_exchange(AtomicPtr* a, void* p) {
#if UARR_OS_WINDOWS
    // _InterlockedExchangePointer = XCHG 命令 (= seq_cst RMW)
    return _InterlockedExchangePointer(&a->v, p);
#else
    return __atomic_exchange_n(&a->v, p, __ATOMIC_SEQ_CST);
#endif
}
static inline bool atomic_cas(AtomicPtr* a, void** expected, void* desired) {
#if UARR_OS_WINDOWS
    // CMPXCHG 命令: a->v が *expected と等しければ desired に置き換える
    // 戻り値は元の値 (= prev)。等しかったら成功 (true)、違ったら *expected に prev を返して false
    void* prev = _InterlockedCompareExchangePointer(&a->v, desired, *expected);
    bool ok = (prev == *expected);
    *expected = prev;
    return ok;
#else
    return __atomic_compare_exchange_n(&a->v, expected, desired,
                                       false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
}

// ------------------------------------------------------------
// (2) AtomicPtrT<T>: 型付きポインタの template ラッパ
//
//   メリット:  型安全 (= キャスト不要、return 型も T* で正しい)
//   デメリット: template のため、C で書くプロジェクトとの相互運用は困難
//
//   例:
//     struct Foo { int x; };
//     AtomicPtrT<Foo> ap = {};        // ゼロ初期化 (= NULL)
//     atomic_store(&ap, new Foo());   // 型一致しているのでキャスト不要
//     Foo* f = atomic_load(&ap);      // Foo* で返る、キャスト不要
//     Foo* exp = f;
//     atomic_cas(&ap, &exp, nullptr); // 型一致した CAS
//
//   void 特殊化 (= AtomicPtrT<void>) は AtomicPtr と等価。互換のため両方残す。
// ------------------------------------------------------------
template<typename T>
struct AtomicPtrT {
    // T* volatile = 「T へのポインタ」が volatile (= 他スレッドが書き換える可能性あり)
    // ゼロ初期化で NULL になる (= GC_alloc / memset 0 直後すぐ使える)
    T* volatile v;
};

// load: 現在の値を読んで返す。ポインタは const でも変わらないので
//       *a を読み取るだけ (= 引数を const AtomicPtrT<T>* で受ける)
template<typename T>
static inline T* atomic_load(const AtomicPtrT<T>* a) {
#if UARR_OS_WINDOWS
    return a->v;
#else
    return __atomic_load_n(&a->v, __ATOMIC_SEQ_CST);
#endif
}

// store: 新しいポインタ p で a の値を上書き
template<typename T>
static inline void atomic_store(AtomicPtrT<T>* a, T* p) {
#if UARR_OS_WINDOWS
    a->v = p;
#else
    __atomic_store_n(&a->v, p, __ATOMIC_SEQ_CST);
#endif
}

// exchange: a の値を p に置き換えて、置き換える前の値を返す (= seq_cst RMW)
template<typename T>
static inline T* atomic_exchange(AtomicPtrT<T>* a, T* p) {
#if UARR_OS_WINDOWS
    // void** にキャストしてから渡す。x64 は 8byte ポインタなので Pointer 版で OK
    return (T*)_InterlockedExchangePointer((void* volatile*)&a->v, (void*)p);
#else
    return __atomic_exchange_n(&a->v, p, __ATOMIC_SEQ_CST);
#endif
}

// cas: 「a の値が *expected と等しければ desired に書き換える」を atomic に行う
//      失敗時は *expected に現在値を返し、戻り値 false。成功時は true。
template<typename T>
static inline bool atomic_cas(AtomicPtrT<T>* a, T** expected, T* desired) {
#if UARR_OS_WINDOWS
    void* prev = _InterlockedCompareExchangePointer(
        (void* volatile*)&a->v, (void*)desired, (void*)*expected);
    bool ok = (prev == (void*)*expected);
    *expected = (T*)prev;
    return ok;
#else
    return __atomic_compare_exchange_n(&a->v, expected, desired,
                                       false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
}

// ------------------------------------------------------------
// (3) atomic_load_p / store_p / exchange_p / cas_p
//     = ラッパ struct を介さずに、生の T* 変数 (= 既存の構造体メンバ等) を
//       直接 atomic 操作する template 関数。
//
// 用途:
//   既に struct が定義されていて、そのメンバを atomic に読み書きしたいが、
//   AtomicPtr で wrap するために構造体定義を変えたくない場合。代表例:
//
//     struct Node {
//         Node* volatile next;   // volatile を付けるだけで構造体 layout を維持
//         int value;
//     };
//     // lock-free push (= compare-and-swap で head を差し替え)
//     void push(Node** head, Node* newNode) {
//         Node* old = atomic_load_p(head);
//         do { newNode->next = old; }
//         while (!atomic_cas_p(head, &old, newNode));
//     }
//
// 関数名に _p (= pointer) を付けて、ラッパ版 (= atomic_load(AtomicPtr*)) と
// オーバーロード解決の曖昧さを避ける。
// ------------------------------------------------------------

// 「T* に対する volatile アドレス」を読む。ポインタ自体は const (= 引数読まない)。
//   T* const volatile*
//     ^^   ^^^^^^^^ ^
//     |    |        | この pointer は const (= 関数内で書き換えない、& で受け取るだけ)
//     |    | ターゲットの T* が volatile (= 他スレッドが書き換える)
//     | T へのポインタ
template<typename T>
static inline T* atomic_load_p(T* const volatile* p) {
#if UARR_OS_WINDOWS
    // x64 で aligned 8byte の load は MOV 1 命令で atomic
    return *p;
#else
    // GCC/Clang は const volatile* を __atomic_load_n に渡せないので const_cast
    return __atomic_load_n(const_cast<T* volatile*>(p), __ATOMIC_SEQ_CST);
#endif
}

// 「T* の volatile アドレス」に新値 v を書く
template<typename T>
static inline void atomic_store_p(T* volatile* p, T* v) {
#if UARR_OS_WINDOWS
    *p = v;
#else
    __atomic_store_n(p, v, __ATOMIC_SEQ_CST);
#endif
}

// exchange: *p の値を v に置き換えて、置き換える前の値を返す
template<typename T>
static inline T* atomic_exchange_p(T* volatile* p, T* v) {
#if UARR_OS_WINDOWS
    return (T*)_InterlockedExchangePointer((void* volatile*)p, (void*)v);
#else
    return __atomic_exchange_n(p, v, __ATOMIC_SEQ_CST);
#endif
}

// cas: *p == *expected なら *p = desired に書き換え、true を返す。
//      失敗時は *expected に現在の *p を返し、false を返す。
template<typename T>
static inline bool atomic_cas_p(T* volatile* p, T** expected, T* desired) {
#if UARR_OS_WINDOWS
    void* prev = _InterlockedCompareExchangePointer(
        (void* volatile*)p, (void*)desired, (void*)*expected);
    bool ok = (prev == (void*)*expected);
    *expected = (T*)prev;
    return ok;
#else
    return __atomic_compare_exchange_n(p, expected, desired,
                                       false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
}

// ============================================================
// メモリバリア (= 特殊ケース用、通常の atomic_* は seq_cst なので不要)
// ============================================================
static inline void memory_barrier() {
#if UARR_OS_WINDOWS
    MemoryBarrier();
#else
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
#endif
}
static inline void compiler_barrier() {
#if UARR_OS_WINDOWS
    _ReadWriteBarrier();
#else
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
#endif
}

// ============================================================
// プラットフォーム抽象 wait_on_address / wake (= futex 風)
//
// 用途:
//   待機側 (= wait): (volatile int*) addr の現在値が cmpVal なら寝る。
//                   別スレッドが wake で起こすか timeout で起きる。
//   起こす側 (= wake): addr で寝てるスレッドを 1 つ / 全部起こす。
//
//   timeout_ms == UINT32_MAX で無限待ち。
//   spurious wakeup あり得るので、呼び出し側はループで再 check すること。
// ============================================================
static inline void platform_wait_on_address(volatile int* addr, int cmpVal, uint32_t timeout_ms) {
#if UARR_OS_WINDOWS
    long cmp = (long)cmpVal;
    DWORD to = (timeout_ms == 0xFFFFFFFFu) ? INFINITE : timeout_ms;
    WaitOnAddress((volatile VOID*)addr, &cmp, sizeof(long), to);
#elif UARR_OS_LINUX
    if (timeout_ms == 0xFFFFFFFFu) {
        syscall(SYS_futex, (int*)addr, FUTEX_WAIT_PRIVATE, cmpVal, NULL, NULL, 0);
    } else {
        struct timespec ts;
        ts.tv_sec  = (time_t)(timeout_ms / 1000u);
        ts.tv_nsec = (long)((timeout_ms % 1000u) * 1000000ul);
        syscall(SYS_futex, (int*)addr, FUTEX_WAIT_PRIVATE, cmpVal, &ts, NULL, 0);
    }
#elif UARR_OS_APPLE
    uint32_t to_us = (timeout_ms == 0xFFFFFFFFu) ? 0 : (timeout_ms * 1000u);
    __ulock_wait(UL_COMPARE_AND_WAIT | ULF_NO_ERRNO,
                 (void*)addr, (uint64_t)(uint32_t)cmpVal, to_us);
#endif
}

static inline void platform_wake_one(volatile int* addr) {
#if UARR_OS_WINDOWS
    WakeByAddressSingle((PVOID)addr);
#elif UARR_OS_LINUX
    syscall(SYS_futex, (int*)addr, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
#elif UARR_OS_APPLE
    __ulock_wake(UL_COMPARE_AND_WAIT | ULF_NO_ERRNO, (void*)addr, 0);
#endif
}

static inline void platform_wake_all(volatile int* addr) {
#if UARR_OS_WINDOWS
    WakeByAddressAll((PVOID)addr);
#elif UARR_OS_LINUX
    syscall(SYS_futex, (int*)addr, FUTEX_WAKE_PRIVATE, 0x7fffffff, NULL, NULL, 0);
#elif UARR_OS_APPLE
    __ulock_wake(UL_COMPARE_AND_WAIT | ULF_WAKE_ALL | ULF_NO_ERRNO, (void*)addr, 0);
#endif
}

// ============================================================
// Mutex (= state-based futex mutex)
//   state: 0 = free, 1 = locked (no waiters), 2 = locked (with waiters)
//   ゼロ初期化 = free。再帰非対応。
// ============================================================
struct Mutex {
    AtomicInt state;
};

static inline void mutex_lock(Mutex* m) {
    int expected = 0;
    if (atomic_cas(&m->state, &expected, 1)) return;   // fast path
    // slow path: state を 2 (= waiters あり) にして寝る
    while (atomic_exchange(&m->state, 2) != 0) {
        platform_wait_on_address(&m->state.v, 2, 0xFFFFFFFFu);
    }
}

static inline bool mutex_trylock(Mutex* m) {
    int expected = 0;
    return atomic_cas(&m->state, &expected, 1);
}

static inline void mutex_unlock(Mutex* m) {
    if (atomic_exchange(&m->state, 0) == 2) {
        platform_wake_one(&m->state.v);
    }
}

// RAII (= scope 終了で自動 unlock)。{ MutexGuard g(&m); /* critical */ } で使う。
struct MutexGuard {
    Mutex* m_;
    MutexGuard(Mutex* m) : m_(m) { mutex_lock(m); }
    ~MutexGuard() { mutex_unlock(m_); }
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
};

// ============================================================
// CondVar (= シーケンス番号ベースの条件変数)
//   notify のたびに seq を inc + wake。
//   wait は m を unlock → seq の現在値で寝る → 起きたら m を再 lock。
//   ゼロ初期化 = seq 0。
// ============================================================
struct CondVar {
    AtomicInt seq;
};

static inline void condvar_wait(CondVar* c, Mutex* m) {
    int s = atomic_load(&c->seq);
    mutex_unlock(m);
    platform_wait_on_address(&c->seq.v, s, 0xFFFFFFFFu);
    mutex_lock(m);
}

// timeout_ms 待つ。seq 変化を検知できれば true (= notify 受信)、timeout なら false。
// spurious wakeup 対応のためループで再 check すること。
static inline bool condvar_wait_timeout(CondVar* c, Mutex* m, uint32_t timeout_ms) {
    int s = atomic_load(&c->seq);
    mutex_unlock(m);
    platform_wait_on_address(&c->seq.v, s, timeout_ms);
    mutex_lock(m);
    return atomic_load(&c->seq) != s;
}

static inline void condvar_notify_one(CondVar* c) {
    atomic_inc(&c->seq);
    platform_wake_one(&c->seq.v);
}
static inline void condvar_notify_all(CondVar* c) {
    atomic_inc(&c->seq);
    platform_wake_all(&c->seq.v);
}

// ============================================================
// Event (= 1bit シグナル、auto-reset)
//   v: 0 = 未シグナル, 1 = シグナル。
//   wait は CAS で 1→0 を奪い取る (= 同時複数 wait なら 1 人だけ起きる)。
//   ゼロ初期化 = 未シグナル。
// ============================================================
struct Event {
    AtomicInt v;
};

static inline void event_signal(Event* e) {
    int expected = 0;
    if (atomic_cas(&e->v, &expected, 1)) {
        platform_wake_one(&e->v.v);
    }
}

static inline void event_wait(Event* e) {
    while (true) {
        int expected = 1;
        if (atomic_cas(&e->v, &expected, 0)) return;
        platform_wait_on_address(&e->v.v, 0, 0xFFFFFFFFu);
    }
}

// timeout_ms ms 待つ。取れたら true、timeout なら false。
static inline bool event_wait_timeout(Event* e, uint32_t timeout_ms) {
#if UARR_OS_WINDOWS
    DWORD start = GetTickCount();
#else
    struct timespec ts0; clock_gettime(CLOCK_MONOTONIC, &ts0);
    uint64_t start_ms = (uint64_t)ts0.tv_sec * 1000ULL + ts0.tv_nsec / 1000000ULL;
#endif
    while (true) {
        int expected = 1;
        if (atomic_cas(&e->v, &expected, 0)) return true;

        uint32_t elapsed;
#if UARR_OS_WINDOWS
        elapsed = (uint32_t)(GetTickCount() - start);
#else
        struct timespec ts1; clock_gettime(CLOCK_MONOTONIC, &ts1);
        uint64_t now_ms = (uint64_t)ts1.tv_sec * 1000ULL + ts1.tv_nsec / 1000000ULL;
        elapsed = (uint32_t)(now_ms - start_ms);
#endif
        if (elapsed >= timeout_ms) return false;
        platform_wait_on_address(&e->v.v, 0, timeout_ms - elapsed);
    }
}

