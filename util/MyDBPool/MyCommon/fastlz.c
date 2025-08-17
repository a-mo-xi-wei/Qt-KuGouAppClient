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

#include "fastlz.h"

#include <stdint.h>

/** @def FASTLZ_SAFE
 *  @brief 启用解压缩时的边界检查
 *  @note 通常建议保持此宏定义以确保解压缩安全。
 */
#define FASTLZ_SAFE
#if defined(FASTLZ_USE_SAFE_DECOMPRESSOR) && (FASTLZ_USE_SAFE_DECOMPRESSOR == 0)
#undef FASTLZ_SAFE
#endif

/** @def FASTLZ_LIKELY
 *  @brief 提供分支预测优化提示，适用于支持的编译器
 */
#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 2))
#define FASTLZ_LIKELY(c) (__builtin_expect(!!(c), 1))
#define FASTLZ_UNLIKELY(c) (__builtin_expect(!!(c), 0))
#else
#define FASTLZ_LIKELY(c) (c)
#define FASTLZ_UNLIKELY(c) (c)
#endif

/** @def FLZ_ARCH64
 *  @brief 启用针对 64 位架构的优化实现
 */
#if defined(__x86_64__) || defined(_M_X64)
#define FLZ_ARCH64
#endif

/** @def FASTLZ_BOUND_CHECK
 *  @brief 边界检查宏，用于在解压缩时检查输入输出缓冲区是否越界
 */
#if defined(FASTLZ_SAFE)
#define FASTLZ_BOUND_CHECK(cond) \
  if (FASTLZ_UNLIKELY(!(cond))) return 0;
#else
#define FASTLZ_BOUND_CHECK(cond) \
  do {                           \
  } while (0)
#endif

/** @brief 自定义内存移动函数
 *
 *  将数据从源地址复制到目标地址，复制字节数由 count 指定。
 *  如果未定义 FASTLZ_USE_MEMMOVE 或其值为 0，则使用逐字节复制。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 *  @param count 要复制的字节数
 */
#if defined(FASTLZ_USE_MEMMOVE) && (FASTLZ_USE_MEMMOVE == 0)
static void fastlz_memmove(uint8_t* dest, const uint8_t* src, uint32_t count) {
  do {
    *dest++ = *src++;
  } while (--count);
}

/** @brief 自定义内存复制函数
 *
 *  调用 fastlz_memmove 实现内存复制。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 *  @param count 要复制的字节数
 */
static void fastlz_memcpy(uint8_t* dest, const uint8_t* src, uint32_t count) {
  return fastlz_memmove(dest, src, count);
}
#else

#include <string.h>

/** @brief 内存移动函数
 *
 *  根据条件选择使用标准库 memmove 或逐字节复制。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 *  @param count 要复制的字节数
 */
static void fastlz_memmove(uint8_t* dest, const uint8_t* src, uint32_t count) {
  if ((count > 4) && (dest >= src + count)) {
    memmove(dest, src, count);
  } else {
    switch (count) {
      default:
        do {
          *dest++ = *src++;
        } while (--count);
        break;
      case 3:
        *dest++ = *src++;
      case 2:
        *dest++ = *src++;
      case 1:
        *dest++ = *src++;
      case 0:
        break;
    }
  }
}

/** @brief 内存复制函数
 *
 *  使用标准库 memcpy 实现内存复制。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 *  @param count 要复制的字节数
 */
static void fastlz_memcpy(uint8_t* dest, const uint8_t* src, uint32_t count) { memcpy(dest, src, count); }
#endif

#if defined(FLZ_ARCH64)

/** @brief 读取 32 位无符号整数
 *
 *  针对 64 位架构优化，直接从指针读取 32 位整数。
 *
 *  @param ptr 数据指针
 *  @return 读取的 32 位无符号整数
 */
static uint32_t flz_readu32(const void* ptr) { return *(const uint32_t*)ptr; }

/** @brief 读取 64 位无符号整数
 *
 *  针对 64 位架构优化，直接从指针读取 64 位整数。
 *
 *  @param ptr 数据指针
 *  @return 读取的 64 位无符号整数
 */
static uint64_t flz_readu64(const void* ptr) { return *(const uint64_t*)ptr; }

/** @brief 比较两个数据块
 *
 *  针对 64 位架构优化，比较两个数据块的相同字节数。
 *
 *  @param p 第一个数据块指针
 *  @param q 第二个数据块指针
 *  @param r 第二个数据块的结束边界
 *  @return 相同的字节数
 */
static uint32_t flz_cmp(const uint8_t* p, const uint8_t* q, const uint8_t* r) {
  const uint8_t* start = p;

  if (flz_readu64(p) == flz_readu64(q)) {
    p += 8;
    q += 8;
  }
  if (flz_readu32(p) == flz_readu32(q)) {
    p += 4;
    q += 4;
  }
  while (q < r)
    if (*p++ != *q++) break;
  return p - start;
}

/** @brief 复制 64 位数据块
 *
 *  针对 64 位架构优化，快速复制数据块。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 *  @param count 要复制的字节数
 */
static void flz_copy64(uint8_t* dest, const uint8_t* src, uint32_t count) {
  const uint64_t* p = (const uint64_t*)src;
  uint64_t* q = (uint64_t*)dest;
  if (count < 16) {
    if (count >= 8) {
      *q++ = *p++;
    }
    *q++ = *p++;
  } else {
    *q++ = *p++;
    *q++ = *p++;
    *q++ = *p++;
    *q++ = *p++;
  }
}

/** @brief 复制 256 位数据块
 *
 *  针对 64 位架构优化，快速复制 256 位数据。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 */
static void flz_copy256(void* dest, const void* src) {
  const uint64_t* p = (const uint64_t*)src;
  uint64_t* q = (uint64_t*)dest;
  *q++ = *p++;
  *q++ = *p++;
  *q++ = *p++;
  *q++ = *p++;
}

#endif /* FLZ_ARCH64 */

#if !defined(FLZ_ARCH64)

/** @brief 读取 32 位无符号整数
 *
 *  针对非 64 位架构，逐字节读取并组合成 32 位整数。
 *
 *  @param ptr 数据指针
 *  @return 读取的 32 位无符号整数
 */
static uint32_t flz_readu32(const void* ptr) {
  const uint8_t* p = (const uint8_t*)ptr;
  return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

/** @brief 比较两个数据块
 *
 *  针对非 64 位架构，逐字节比较两个数据块的相同字节数。
 *
 *  @param p 第一个数据块指针
 *  @param q 第二个数据块指针
 *  @param r 第二个数据块的结束边界
 *  @return 相同的字节数
 */
static uint32_t flz_cmp(const uint8_t* p, const uint8_t* q, const uint8_t* r) {
  const uint8_t* start = p;
  while (q < r)
    if (*p++ != *q++) break;
  return p - start;
}

/** @brief 复制 64 位数据块
 *
 *  针对非 64 位架构，逐字节复制数据块。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 *  @param count 要复制的字节数
 */
static void flz_copy64(uint8_t* dest, const uint8_t* src, uint32_t count) {
  const uint8_t* p = (const uint8_t*)src;
  uint8_t* q = (uint8_t*)dest;
  int c;
  for (c = 0; c < count * 8; ++c) {
    *q++ = *p++;
  }
}

/** @brief 复制 256 位数据块
 *
 *  针对非 64 位架构，逐字节复制 256 位数据。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 */
static void flz_copy256(void* dest, const void* src) {
  const uint8_t* p = (const uint8_t*)src;
  uint8_t* q = (uint8_t*)dest;
  int c;
  for (c = 0; c < 32; ++c) {
    *q++ = *p++;
  }
}

#endif /* !FLZ_ARCH64 */

/** @def MAX_COPY
 *  @brief 最大单次复制字节数
 */
#define MAX_COPY 32

/** @def MAX_LEN
 *  @brief 最大匹配长度（256 + 8）
 */
#define MAX_LEN 264

/** @def MAX_L1_DISTANCE
 *  @brief 级别 1 压缩的最大匹配距离
 */
#define MAX_L1_DISTANCE 8192

/** @def MAX_L2_DISTANCE
 *  @brief 级别 2 压缩的最大匹配距离
 */
#define MAX_L2_DISTANCE 8191

/** @def MAX_FARDISTANCE
 *  @brief 最大远距离匹配
 */
#define MAX_FARDISTANCE (65535 + MAX_L2_DISTANCE - 1)

/** @def HASH_LOG
 *  @brief 哈希表对数大小
 */
#define HASH_LOG 14

/** @def HASH_SIZE
 *  @brief 哈希表大小（2^HASH_LOG）
 */
#define HASH_SIZE (1 << HASH_LOG)

/** @def HASH_MASK
 *  @brief 哈希表掩码
 */
#define HASH_MASK (HASH_SIZE - 1)

/** @brief 计算哈希值
 *
 *  根据输入的 32 位值计算哈希值，用于快速查找匹配。
 *
 *  @param v 输入的 32 位值
 *  @return 计算得到的哈希值
 */
static uint16_t flz_hash(uint32_t v) {
  uint32_t h = (v * 2654435769LL) >> (32 - HASH_LOG);
  return h & HASH_MASK;
}

/** @brief 处理字面量数据
 *
 *  将指定长度的字面量数据从源地址复制到目标地址。
 *
 *  @param runs 要复制的字面量字节数
 *  @param src 源数据指针
 *  @param dest 目标数据指针
 *  @return 更新后的目标数据指针
 */
static uint8_t* flz_literals(uint32_t runs, const uint8_t* src, uint8_t* dest) {
  while (runs >= MAX_COPY) {
    *dest++ = MAX_COPY - 1;
    flz_copy256(dest, src);
    src += MAX_COPY;
    dest += MAX_COPY;
    runs -= MAX_COPY;
  }
  if (runs > 0) {
    *dest++ = runs - 1;
    flz_copy64(dest, src, runs);
    dest += runs;
  }
  return dest;
}

/** @brief 小块数据复制
 *
 *  复制最多 32 字节的数据，针对小数据块优化。
 *
 *  @param dest 目标地址指针
 *  @param src 源地址指针
 *  @param count 要复制的字节数
 */
static void flz_smallcopy(uint8_t* dest, const uint8_t* src, uint32_t count) {
#if defined(FLZ_ARCH64)
  if (count >= 8) {
    const uint64_t* p = (const uint64_t*)src;
    uint64_t* q = (uint64_t*)dest;
    while (count > 8) {
      *q++ = *p++;
      count -= 8;
      dest += 8;
      src += 8;
    }
  }
#endif
  fastlz_memcpy(dest, src, count);
}

/** @brief 最终字面量处理
 *
 *  处理剩余的字面量数据并复制到目标地址。
 *
 *  @param runs 要复制的字面量字节数
 *  @param src 源数据指针
 *  @param dest 目标数据指针
 *  @return 更新后的目标数据指针
 */
static uint8_t* flz_finalize(uint32_t runs, const uint8_t* src, uint8_t* dest) {
  while (runs >= MAX_COPY) {
    *dest++ = MAX_COPY - 1;
    flz_smallcopy(dest, src, MAX_COPY);
    src += MAX_COPY;
    dest += MAX_COPY;
    runs -= MAX_COPY;
  }
  if (runs > 0) {
    *dest++ = runs - 1;
    flz_smallcopy(dest, src, runs);
    dest += runs;
  }
  return dest;
}

/** @brief 级别 1 压缩的匹配编码
 *
 *  将匹配的长度和距离编码到输出缓冲区。
 *
 *  @param len 匹配长度
 *  @param distance 匹配距离
 *  @param op 输出缓冲区指针
 *  @return 更新后的输出缓冲区指针
 */
static uint8_t* flz1_match(uint32_t len, uint32_t distance, uint8_t* op) {
  --distance;
  if (FASTLZ_UNLIKELY(len > MAX_LEN - 2))
    while (len > MAX_LEN - 2) {
      *op++ = (7 << 5) + (distance >> 8);
      *op++ = MAX_LEN - 2 - 7 - 2;
      *op++ = (distance & 255);
      len -= MAX_LEN - 2;
    }
  if (len < 7) {
    *op++ = (len << 5) + (distance >> 8);
    *op++ = (distance & 255);
  } else {
    *op++ = (7 << 5) + (distance >> 8);
    *op++ = len - 7;
    *op++ = (distance & 255);
  }
  return op;
}

/** @brief 级别 1 压缩实现
 *
 *  使用级别 1 压缩算法压缩输入数据。
 *
 *  @param input 输入数据缓冲区指针
 *  @param length 输入数据长度
 *  @param output 输出压缩数据缓冲区指针
 *  @return 压缩后数据的大小
 */
int fastlz1_compress(const void* input, int length, void* output) {
  const uint8_t* ip = (const uint8_t*)input;
  const uint8_t* ip_start = ip;
  const uint8_t* ip_bound = ip + length - 4; /* because readU32 */
  const uint8_t* ip_limit = ip + length - 12 - 1;
  uint8_t* op = (uint8_t*)output;

  uint32_t htab[HASH_SIZE];
  uint32_t seq, hash;

  /* 初始化哈希表 */
  for (hash = 0; hash < HASH_SIZE; ++hash) htab[hash] = 0;

  /* 从字面量复制开始 */
  const uint8_t* anchor = ip;
  ip += 2;

  /* 主循环 */
  while (FASTLZ_LIKELY(ip < ip_limit)) {
    const uint8_t* ref;
    uint32_t distance, cmp;

    /* 寻找潜在匹配 */
    do {
      seq = flz_readu32(ip) & 0xffffff;
      hash = flz_hash(seq);
      ref = ip_start + htab[hash];
      htab[hash] = ip - ip_start;
      distance = ip - ref;
      cmp = FASTLZ_LIKELY(distance < MAX_L1_DISTANCE) ? flz_readu32(ref) & 0xffffff : 0x1000000;
      if (FASTLZ_UNLIKELY(ip >= ip_limit)) break;
      ++ip;
    } while (seq != cmp);

    if (FASTLZ_UNLIKELY(ip >= ip_limit)) break;
    --ip;

    if (FASTLZ_LIKELY(ip > anchor)) {
      op = flz_literals(ip - anchor, anchor, op);
    }

    uint32_t len = flz_cmp(ref + 3, ip + 3, ip_bound);
    op = flz1_match(len, distance, op);

    /* 更新匹配边界的哈希表 */
    ip += len;
    seq = flz_readu32(ip);
    hash = flz_hash(seq & 0xffffff);
    htab[hash] = ip++ - ip_start;
    seq >>= 8;
    hash = flz_hash(seq);
    htab[hash] = ip++ - ip_start;

    anchor = ip;
  }

  uint32_t copy = (uint8_t*)input + length - anchor;
  op = flz_finalize(copy, anchor, op);

  return op - (uint8_t*)output;
}

/** @brief 级别 1 解压缩实现
 *
 *  使用级别 1 解压缩算法解压缩输入数据。
 *
 *  @param input 压缩数据缓冲区指针
 *  @param length 压缩数据长度
 *  @param output 输出解压缩数据缓冲区指针
 *  @param maxout 输出缓冲区最大大小
 *  @return 解压缩后数据的大小，若失败则返回 0
 */
int fastlz1_decompress(const void* input, int length, void* output, int maxout) {
  const uint8_t* ip = (const uint8_t*)input;
  const uint8_t* ip_limit = ip + length;
  const uint8_t* ip_bound = ip_limit - 2;
  uint8_t* op = (uint8_t*)output;
  uint8_t* op_limit = op + maxout;
  uint32_t ctrl = (*ip++) & 31;

  while (1) {
    if (ctrl >= 32) {
      uint32_t len = (ctrl >> 5) - 1;
      uint32_t ofs = (ctrl & 31) << 8;
      const uint8_t* ref = op - ofs - 1;
      if (len == 7 - 1) {
        FASTLZ_BOUND_CHECK(ip <= ip_bound);
        len += *ip++;
      }
      ref -= *ip++;
      len += 3;
      FASTLZ_BOUND_CHECK(op + len <= op_limit);
      FASTLZ_BOUND_CHECK(ref >= (uint8_t*)output);
      fastlz_memmove(op, ref, len);
      op += len;
    } else {
      ctrl++;
      FASTLZ_BOUND_CHECK(op + ctrl <= op_limit);
      FASTLZ_BOUND_CHECK(ip + ctrl <= ip_limit);
      fastlz_memcpy(op, ip, ctrl);
      ip += ctrl;
      op += ctrl;
    }

    if (FASTLZ_UNLIKELY(ip > ip_bound)) break;
    ctrl = *ip++;
  }

  return op - (uint8_t*)output;
}

/** @brief 级别 2 压缩的匹配编码
 *
 *  将匹配的长度和距离编码到输出缓冲区，支持远距离匹配。
 *
 *  @param len 匹配长度
 *  @param distance 匹配距离
 *  @param op 输出缓冲区指针
 *  @return 更新后的输出缓冲区指针
 */
static uint8_t* flz2_match(uint32_t len, uint32_t distance, uint8_t* op) {
  --distance;
  if (distance < MAX_L2_DISTANCE) {
    if (len < 7) {
      *op++ = (len << 5) + (distance >> 8);
      *op++ = (distance & 255);
    } else {
      *op++ = (7 << 5) + (distance >> 8);
      for (len -= 7; len >= 255; len -= 255) *op++ = 255;
      *op++ = len;
      *op++ = (distance & 255);
    }
  } else {
    /* 远距离匹配 */
    if (len < 7) {
      distance -= MAX_L2_DISTANCE;
      *op++ = (len << 5) + 31;
      *op++ = 255;
      *op++ = distance >> 8;
      *op++ = distance & 255;
    } else {
      distance -= MAX_L2_DISTANCE;
      *op++ = (7 << 5) + 31;
      for (len -= 7; len >= 255; len -= 255) *op++ = 255;
      *op++ = len;
      *op++ = 255;
      *op++ = distance >> 8;
      *op++ = distance & 255;
    }
  }
  return op;
}

/** @brief 级别 2 压缩实现
 *
 *  使用级别 2 压缩算法压缩输入数据，支持更远的匹配距离。
 *
 *  @param input 输入数据缓冲区指针
 *  @param length 输入数据长度
 *  @param output 输出压缩数据缓冲区指针
 *  @return 压缩后数据的大小
 */
int fastlz2_compress(const void* input, int length, void* output) {
  const uint8_t* ip = (const uint8_t*)input;
  const uint8_t* ip_start = ip;
  const uint8_t* ip_bound = ip + length - 4; /* because readU32 */
  const uint8_t* ip_limit = ip + length - 12 - 1;
  uint8_t* op = (uint8_t*)output;

  uint32_t htab[HASH_SIZE];
  uint32_t seq, hash;

  /* 初始化哈希表 */
  for (hash = 0; hash < HASH_SIZE; ++hash) htab[hash] = 0;

  /* 从字面量复制开始 */
  const uint8_t* anchor = ip;
  ip += 2;

  /* 主循环 */
  while (FASTLZ_LIKELY(ip < ip_limit)) {
    const uint8_t* ref;
    uint32_t distance, cmp;

    /* 寻找潜在匹配 */
    do {
      seq = flz_readu32(ip) & 0xffffff;
      hash = flz_hash(seq);
      ref = ip_start + htab[hash];
      htab[hash] = ip - ip_start;
      distance = ip - ref;
      cmp = FASTLZ_LIKELY(distance < MAX_FARDISTANCE) ? flz_readu32(ref) & 0xffffff : 0x1000000;
      if (FASTLZ_UNLIKELY(ip >= ip_limit)) break;
      ++ip;
    } while (seq != cmp);

    if (FASTLZ_UNLIKELY(ip >= ip_limit)) break;

    --ip;

    /* 远距离匹配需要至少 5 字节 */
    if (distance >= MAX_L2_DISTANCE) {
      if (ref[3] != ip[3] || ref[4] != ip[4]) {
        ++ip;
        continue;
      }
    }

    if (FASTLZ_LIKELY(ip > anchor)) {
      op = flz_literals(ip - anchor, anchor, op);
    }

    uint32_t len = flz_cmp(ref + 3, ip + 3, ip_bound);
    op = flz2_match(len, distance, op);

    /* 更新匹配边界的哈希表 */
    ip += len;
    seq = flz_readu32(ip);
    hash = flz_hash(seq & 0xffffff);
    htab[hash] = ip++ - ip_start;
    seq >>= 8;
    hash = flz_hash(seq);
    htab[hash] = ip++ - ip_start;

    anchor = ip;
  }

  uint32_t copy = (uint8_t*)input + length - anchor;
  op = flz_finalize(copy, anchor, op);

  /* 标记为级别 2 压缩 */
  *(uint8_t*)output |= (1 << 5);

  return op - (uint8_t*)output;
}

/** @brief 级别 2 解压缩实现
 *
 *  使用级别 2 解压缩算法解压缩输入数据，支持远距离匹配。
 *
 *  @param input 压缩数据缓冲区指针
 *  @param length 压缩数据长度
 *  @param output 输出解压缩数据缓冲区指针
 *  @param maxout 输出缓冲区最大大小
 *  @return 解压缩后数据的大小，若失败则返回 0
 */
int fastlz2_decompress(const void* input, int length, void* output, int maxout) {
  const uint8_t* ip = (const uint8_t*)input;
  const uint8_t* ip_limit = ip + length;
  const uint8_t* ip_bound = ip_limit - 2;
  uint8_t* op = (uint8_t*)output;
  uint8_t* op_limit = op + maxout;
  uint32_t ctrl = (*ip++) & 31;

  while (1) {
    if (ctrl >= 32) {
      uint32_t len = (ctrl >> 5) - 1;
      uint32_t ofs = (ctrl & 31) << 8;
      const uint8_t* ref = op - ofs - 1;

      uint8_t code;
      if (len == 7 - 1) do {
          FASTLZ_BOUND_CHECK(ip <= ip_bound);
          code = *ip++;
          len += code;
        } while (code == 255);
      code = *ip++;
      ref -= code;
      len += 3;

      /* 支持 16 位距离的匹配 */
      if (FASTLZ_UNLIKELY(code == 255))
        if (FASTLZ_LIKELY(ofs == (31 << 8))) {
          FASTLZ_BOUND_CHECK(ip < ip_bound);
          ofs = (*ip++) << 8;
          ofs += *ip++;
          ref = op - ofs - MAX_L2_DISTANCE - 1;
        }

      FASTLZ_BOUND_CHECK(op + len <= op_limit);
      FASTLZ_BOUND_CHECK(ref >= (uint8_t*)output);
      fastlz_memmove(op, ref, len);
      op += len;
    } else {
      ctrl++;
      FASTLZ_BOUND_CHECK(op + ctrl <= op_limit);
      FASTLZ_BOUND_CHECK(ip + ctrl <= ip_limit);
      fastlz_memcpy(op, ip, ctrl);
      ip += ctrl;
      op += ctrl;
    }

    if (FASTLZ_UNLIKELY(ip >= ip_limit)) break;
    ctrl = *ip++;
  }

  return op - (uint8_t*)output;
}

/** @brief 通用压缩函数
 *
 *  根据输入数据长度选择级别 1 或级别 2 压缩算法。短数据（小于 65536 字节）使用级别 1，否则使用级别 2。
 *
 *  @param input 输入数据缓冲区指针
 *  @param length 输入数据长度
 *  @param output 输出压缩数据缓冲区指针
 *  @return 压缩后数据的大小
 *  @deprecated 该函数已废弃，建议使用 fastlz_compress_level。
 */
int fastlz_compress(const void* input, int length, void* output) {
  /* 短数据块使用级别 1 */
  if (length < 65536) return fastlz1_compress(input, length, output);

  /* 否则使用级别 2 */
  return fastlz2_compress(input, length, output);
}

/** @brief 通用解压缩函数
 *
 *  根据压缩数据的级别标识自动选择级别 1 或级别 2 解压缩算法。
 *
 *  @param input 压缩数据缓冲区指针
 *  @param length 压缩数据长度
 *  @param output 输出解压缩数据缓冲区指针
 *  @param maxout 输出缓冲区最大大小
 *  @return 解压缩后数据的大小，若失败则返回 0
 */
int fastlz_decompress(const void* input, int length, void* output, int maxout) {
  /* 根据压缩级别标识选择解压缩算法 */
  int level = ((*(const uint8_t*)input) >> 5) + 1;

  if (level == 1) return fastlz1_decompress(input, length, output, maxout);
  if (level == 2) return fastlz2_decompress(input, length, output, maxout);

  /* 未知级别，返回错误 */
  return 0;
}

/** @brief 指定压缩级别的压缩函数
 *
 *  根据指定的压缩级别（1 或 2）压缩输入数据。
 *
 *  @param level 压缩级别（1 或 2）
 *  @param input 输入数据缓冲区指针
 *  @param length 输入数据长度
 *  @param output 输出压缩数据缓冲区指针
 *  @return 压缩后数据的大小，若级别无效则返回 0
 */
int fastlz_compress_level(int level, const void* input, int length, void* output) {
  if (level == 1) return fastlz1_compress(input, length, output);
  if (level == 2) return fastlz2_compress(input, length, output);

  return 0;
}