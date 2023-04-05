/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "txrand.h"
#include <limits.h>
/*
    这段代码定义了三个函数，都用于生成随机的布尔值。
    
    第一个函数 randbool 采用一个参数 weight，它用来控制生成布尔值为 true 的概率。
    首先调用 random() 函数生成一个 [0,1) 之间的随机数 i，然后将 i 和 weight 进行比较，
    如果 i 小于 weight，则返回 true，否则返回 false。
    
    第二个函数 randbool_b 采用由操作系统随机数生成器包装出的 obtain_cached_ull 生成一个 64 位的随机数 num，
    然后统计这个随机数中二进制位 0 和 1 的个数，如果 0 的个数少于 1 的个数，则返回 true，否则返回 false。
    注意这里还对随机数的最高位进行了处理，如果最高位为 1，则返回取反的结果，否则返回原始结果。
    
    第三个函数 randbool_q 也是采用由操作系统随机数生成器包装出的 obtain_cached_ull 生成一个 64 位的随机数 num，
    然后从这个随机数的最低位开始，每次取一位二进制数，直到取完 64 位。
    如果这一位为 1，则返回 true，否则返回 false。
    这个函数还使用了静态变量来保存当前已经生成的随机数和还剩余的二进制位数，以便下次调用时继续使用。
*/

_Bool PRIVATEAPI obtain_cached_ull(unsigned long long* const pull, const size_t sizeofull);
_Bool TXRANDAPI  randbool(double weight) {
	const double i = random();
	_Bool returnvalue = (i < weight);
	return returnvalue;
}
_Bool TXRANDAPI  randbool_b(void)
{
    unsigned long long num;
    int x[] = { 0,0 };
    obtain_cached_ull(&num, sizeof(num));
    // num &= ~(1ULL<<63);
    int i;
    for (i = 0; i < CHAR_BIT * (sizeof num) - 1; i++)
    {
        x[(num >> i) & 1]++;
    }
    _Bool retb = (x[0] < x[1]);
    return num >> (CHAR_BIT * (sizeof num) - 1) ? !retb : retb;
}

_Bool TXRANDAPI  randbool_q(void) 
{
    static unsigned long long num = 0;
    static int i = 0;
    if (!i) { obtain_cached_ull(&num, (sizeof num)); i = (sizeof num) * CHAR_BIT; };
    return (_Bool)((num >> (--i)) & 1ULL);
}