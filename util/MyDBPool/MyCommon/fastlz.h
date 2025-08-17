/*
  FastLZ - 字节对齐的 LZ77 压缩库
  Copyright (C) 2005-2020 Ariya Hidayat <ariya.hidayat@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef FASTLZ_H
#define FASTLZ_H

/** @def FASTLZ_VERSION
 *  @brief FastLZ 库的版本号，格式为 0x000500
 */
#define FASTLZ_VERSION 0x000500

/** @def FASTLZ_VERSION_MAJOR
 *  @brief FastLZ 库的主版本号
 */
#define FASTLZ_VERSION_MAJOR 0

/** @def FASTLZ_VERSION_MINOR
 *  @brief FastLZ 库的次版本号
 */
#define FASTLZ_VERSION_MINOR 5

/** @def FASTLZ_VERSION_REVISION
 *  @brief FastLZ 库的修订版本号
 */
#define FASTLZ_VERSION_REVISION 0

/** @def FASTLZ_VERSION_STRING
 *  @brief FastLZ 库的版本字符串，格式为 "0.5.0"
 */
#define FASTLZ_VERSION_STRING "0.5.0"

#if defined(__cplusplus)
extern "C" {
#endif

/** @brief 压缩输入缓冲区中的数据块
 *
 *  该函数将输入缓冲区中的数据压缩，并返回压缩后数据块的大小。输入缓冲区的长度由参数 length 指定，
 *  最小输入缓冲区大小为 16 字节。输出缓冲区必须比输入缓冲区大至少 5%，且不得小于 66 字节。
 *  如果输入数据不可压缩，输出数据大小可能比输入缓冲区大小更大。
 *  输入和输出缓冲区不能重叠。
 *
 *  @param level 压缩级别，目前支持级别 1 和 2。级别 1 压缩速度最快，适合短数据；级别 2 压缩速度稍慢，但压缩比更高。
 *  @param input 输入数据缓冲区的指针
 *  @param length 输入数据缓冲区的长度
 *  @param output 输出压缩数据的缓冲区指针
 *  @return 压缩后数据块的大小（字节数）
 *  @note 无论使用何种压缩级别，压缩数据都可以通过 fastlz_decompress 函数解压缩。
*/

int fastlz_compress_level(int level, const void* input, int length, void* output);

/** @brief 解压缩输入缓冲区中的压缩数据块
 *
 *  该函数解压缩输入缓冲区中的压缩数据，并返回解压缩后数据块的大小。如果发生错误（如压缩数据损坏或输出缓冲区不足），
 *  则返回 0。输入和输出缓冲区不能重叠。解压缩过程是内存安全的，保证不会写入超出 maxout 指定大小的输出缓冲区。
 *
 *  @param input 压缩数据缓冲区的指针
 *  @param length 压缩数据缓冲区的长度
 *  @param output 输出解压缩数据的缓冲区指针
 *  @param maxout 输出缓冲区的最大可用大小
 *  @return 解压缩后数据块的大小（字节数），若发生错误则返回 0
 *  @note 解压缩过程与压缩级别无关，总是能够正确解压缩通过 fastlz_compress_level 产生的压缩数据。
 */

int fastlz_decompress(const void* input, int length, void* output, int maxout);

/** @brief 压缩输入缓冲区中的数据块（已废弃）
 *
 *  该函数与 fastlz_compress_level 类似，但会自动选择压缩级别。由于已被废弃，建议使用 fastlz_compress_level。
 *  该函数将在未来版本中完全移除。
 *
 *  @param input 输入数据缓冲区的指针
 *  @param length 输入数据缓冲区的长度
 *  @param output 输出压缩数据的缓冲区指针
 *  @return 压缩后数据块的大小（字节数）
 *  @deprecated 该函数已废弃，请使用 fastlz_compress_level 替代。
*/

int fastlz_compress(const void* input, int length, void* output);

#if defined(__cplusplus)
}
#endif

#endif /* FASTLZ_H */
