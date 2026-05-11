// (使用していない: clang 21 の f16cintrin.h が _cvtsh_ss / _cvtss_sh を
//  __F16C__ 定義時に自前で提供するため、追加対策は不要だった。
//  当初 libtorch で _cvtsh_ss 未宣言エラーが出ていたのは、resource-dir 指定が
//  なくて clang のヘッダにフォールバックできていなかったためで、
//  -resource-dir を入れた時点で自動解決していた。)
