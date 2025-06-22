// rng.c
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "txrand.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>

/**
 * 定义了一个宏RAND_FUNC_BITS_PROVIDED，用于计算标准库函数rand()返回值中提供的二进制位数。
 * 这里通过将RAND_MAX加1然后取以2为底的对数，来确定rand()函数返回值的最大二进制位数。
 * RAND_MAX是标准库中rand()函数能返回的最大值，加1是为了处理边界情况。
 * 另外，RFBS是一个别名，表示RAND_FUNC_BITS_PROVIDED的值。因为原来的宏定义名称太长啦！（︶^︶）
 */
#define RAND_FUNC_BITS_PROVIDED ((unsigned)log2(((unsigned)RAND_MAX + 1u)))
#define RFBS RAND_FUNC_BITS_PROVIDED


// 检查编译器和平台是否支持RDRAND指令集扩展，用于自动决定是否使用RDRAND指令生成随机数。
#if (defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)) && \
    (defined(__GNUC__) || defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER))
    #if defined(__has_include)
        #if __has_include(<immintrin.h>)
            #define TXRAND_HAS_IMMINTRIN 1
            #include <immintrin.h>
        #else
            #define TXRAND_HAS_IMMINTRIN 0
        #endif
    #else
        #define TXRAND_HAS_IMMINTRIN 1
        #include <immintrin.h>
    #endif
#else
        #define TXRAND_HAS_IMMINTRIN 0
#endif

/**
 * StdlibRandFuncValueBitwise函数使用标准库的rand()函数生成随机布尔值。
 * 该函数通过位操作来获取随机数的每一位，并返回一个布尔值（0或1）。
 * 该实现确保了每次调用都能返回一个新的随机布尔值，且不会浪费生成的随机数据。
 *
 * @return 返回一个随机布尔值（0或1）。
 */

static _Bool __fastcall StdlibRandFuncValueBitwise(void) {
    // 此函数将返回一个随机的布尔值。
    static _Bool first_run; // 指示随机数生成器是否已初始化
    // r 用于存储一组随机数，i 用于指示下一个要使用的位的位置
    static unsigned r, i;

    if (!first_run) {
        void* p = malloc(1);
        srand((unsigned)time(NULL) ^ (unsigned)clock() ^ (unsigned)p);
        free(p);
        first_run = 1;
    }

    // 当当前随机数的所有位都已使用时，更新索引并生成一个新的随机数
    if (!i)
        // RFBS 是一个常量，表示从随机数 r 中使用的位数
        i = RFBS,
        // 生成一个新的随机数
        r = rand();

    // 返回随机数的下一位，并递减索引以供下一次使用
    return (r >> --i) & 1u;
}
/**
 * 调用默认随机数生成器（rand函数）填充缓冲区。
 *
 * 此函数使用位操作将标准库中的随机数生成器输出混合到缓冲区中。
 * 对缓冲区的每个字节进行逐位操作，确保生成数字的随机性均匀分布在缓冲区上，也不会浪费生成的随机数据。
 *
 * @param buffer 指向要填充随机数据的缓冲区的指针。
 * @param size 缓冲区的大小（以字节为单位）。
 * @return 总是返回1，表示操作已完成（根据实际功能需求，此设计可能需要重新考虑）。
 */
static _Bool __cdecl call_default_rng(void* buffer, size_t size) {
    // 检查缓冲区指针和大小是否有效，如果无效直接返回1（真），因为填充空缓冲区还是比较6的。
    if (!buffer || !size) return 1;

    size_t wbyte, wbit;
    // 遍历缓冲区的每个字节
    for (wbyte = 0; wbyte < size; wbyte++) {
        // 遍历字节的每个位
        for (wbit = 0; wbit < CHAR_BIT; wbit++) {
            // 使用位异或运算符将随机数生成器的结果混合到缓冲区中
            // 如果StdlibRandFuncValueBitwise()返回真，则缓冲区相应位设置为1；
            // 否则，设置为0。这确保了缓冲区的每位都有机会被随机数生成器修改。
            // register _Bool rv = rv;
            ((unsigned char*)buffer)[wbyte] ^= (StdlibRandFuncValueBitwise() ? (1u << wbit) : (0u));
        }
    }
    // 返回1（真），表示操作成功完成
    return (_Bool)1;
}

#if TXRAND_HAS_IMMINTRIN

/**
 * 调用随机数生成器填充缓冲区。
 *
 * 此函数使用RDRAND指令生成随机数，并将其填充到指定的缓冲区中。
 * 如果RDRAND指令不可用或失败，则回退到默认的随机数生成器实现。
 * 如果buffer或size无效，则直接返回1（真）。
 * 如果size不是4的倍数，则会处理剩余的不足4字节的部分，但会丢弃多余的1~3字节（没必要这么抠门，毕竟RDRAND使用的CPU周期不比rand()多，更快）。
 *
 * @param buffer 指向要填充随机数据的缓冲区的指针。
 * @param size 缓冲区的大小（以字节为单位）。
 * @return 总是返回1，表示操作已完成（根据实际功能需求，此设计可能需要重新考虑）。
 */
_Bool __cdecl call_rng(void* buffer, size_t size) {
    if (!buffer || !size) return 1;
    unsigned char* p = (unsigned char*)buffer;
    size_t filled = 0;
    unsigned val;

    while (filled + sizeof(val) <= size) {
        if (!_rdrand32_step(&val)) {
            // RDRAND失败，回退到默认实现
            return call_default_rng(p + filled, size - filled);
        }
        memcpy(p + filled, &val, sizeof(val));
        filled += sizeof(val);
    }
    // 处理剩余不足4字节的部分
    if (filled < size) {
        if (!_rdrand32_step(&val)) {
            return call_default_rng(p + filled, size - filled);
        }
        memcpy(p + filled, &val, size - filled);
    }
    return 1;
}

#else
/**
 * 调用随机数生成器填充缓冲区。
 *
 * 此函数使用位操作将标准库中的随机数生成器（rand函数）输出混合到缓冲区中，因为不知道该平台是否有其它的随机数生成器实现。
 * 参见 call_default_rng() 函数的实现。
 * 
 * @param buffer 指向要填充随机数据的缓冲区的指针。
 * @param size 缓冲区的大小（以字节为单位）。
 * @return 总是返回1，表示操作已完成（根据实际功能需求，此设计可能需要重新考虑）。
 */

_Bool __cdecl call_rng(void* buffer, size_t size) {
	return call_default_rng(buffer, size);
}

#endif
