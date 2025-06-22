// arrop.c
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


#include "txrand.h"
#include "txrandpr.h"
#include <string.h>

/*


    这段代码实现了一些随机数生成和数组操作相关的函数。具体分析如下：

    uniq_rand_arr 函数：该函数生成一个包含 len 个元素的数组，每个元素的值在 0 到 range-1 之间，且每个元素的值都不相同。该函数使用了 calloc 函数进行内存分配，并返回分配的数组指针。

    mboffset 函数：该函数实现了一个指针偏移函数，将指针 pstart 往后偏移 offsetbbs 个字节，并返回偏移后的指针。该函数使用了 size_t 类型的变量进行指针运算，并将其转换为 void 类型的指针返回。

    choice 函数：该函数从 seq 数组中随机选择一个元素，并返回该元素的指针。函数首先调用了 call_os_rng 函数生成一个随机数 b，然后将其对 count 取模，得到一个在 0 到 count-1 之间的随机数。最后使用 mboffset 函数计算出随机元素的指针并返回。

    sample 函数：该函数从 pop 数组中随机选择 k 个元素，将它们复制到 dest 数组中，并返回 dest 数组的指针。为了避免重复选择元素，函数先调用 uniq_rand_arr 函数生成一个包含 k 个不同元素的数组 simarr，然后依次将 pop 数组中对应下标的元素复制到 dest 数组中。函数使用了 calloc 函数进行内存分配，并在函数执行完毕后释放 simarr 和 _simarr 数组的内存。

    memswp 函数：该函数实现了一个指针交换函数，将指针 a 和指针 b 指向的大小为 size 的内存区域交换。函数首先检查指针 a 和指针 b 是否相同，或者它们指向的内存区域是否有重叠，如果有则直接返回。否则，函数使用异或运算逐个交换指针 a 和指针 b 指向的内存区域，并返回交换的次数。

    shuffle 函数：该函数实现了一个数组洗牌函数，将数组 seq 中的元素随机打乱顺序。函数首先调用 uniq_rand_arr 函数生成一个包含 count 个不同元素的数组 sa，然后依次使用 memswp 函数交换数组 seq 中的元素和数组 sa 中对应下标的元素。函数使用了 calloc 函数进行内存分配，并在函数执行完毕后释放 sa 数组的内存。

    fillbuffer 函数：该函数使用 call_os_rng 函数生成 count 个随机字节，并将它们填充到 buffer 指向的内存区域中。函数返回 buffer 指针，如果生成随机数失败则返回 NULL。


*/


static inline void* PRIVATEAPI mboffset(const void* pstart, size_t offsetb, size_t bs) { 
    size_t _pdest;
    void* pdest;
    _pdest = (size_t)pstart;
    _pdest += bs * offsetb;
    pdest = (void*)_pdest;
    return pdest;
}

void*  TXRANDAPI choice(const void* seq, size_t size, size_t count)
{
    size_t b;
    
    if (!call_rng(&b, sizeof(b))) return NULL;
    b %= count;
    
    return mboffset(seq,b,size);
}

void*  TXRANDAPI sample(const void* pop, size_t size, size_t count, size_t k, void* dest) {
    size_t *_simarr,*simarr, i;
    _simarr = calloc(k,size);
    if (!_simarr) return NULL;
    simarr = uniq_rand_arr(k,count,_simarr);
    if (!simarr) { free(_simarr); return NULL; };
    for (i = 0; i < k; i++) memcpy(mboffset(dest, i, size),mboffset(pop,simarr[i],size), size);   
    return dest;
}


inline static size_t PRIVATEAPI memswp(void* a, void* b, size_t size) {
    {
        size_t cmpa, cmpb;
        cmpa = (size_t)a, cmpb = (size_t)b;
        if ((a == b) || ((cmpa < cmpb) && (cmpa + size >= cmpb)) || ((cmpb < cmpa) && (cmpb + size >= cmpa)))return 0;
    }
    {
        unsigned char *swpa = a, *swpb = b;
        size_t i;
        for (i = 0; i < size; i++) { 
            swpa[i] ^= swpb[i];
            swpb[i] ^= swpa[i];
            swpa[i] ^= swpb[i]; 
        }
        return i+1;
    }
}

void shuffle(void* seq, size_t size, size_t count) {
    size_t* sa, i;
    sa = calloc(count,sizeof(size_t));
    if (!sa) return;
    if (!uniq_rand_arr(count, count, sa)) {
        free(sa);
        return;
    };
    for (i = 0; i < count; i++) {
        memswp(mboffset(seq, i, size), mboffset(seq, sa[i], size), size);
    }
    /*
    if (!uniq_rand_arr(count, count, sa)) {
        free(sa);
        return;
    };
    for (i = 0; i < count; i++) {
        memswp(mboffset(seq, sa[i], size), mboffset(seq, i, size), size);
    }
    */
    free(sa);
    return;
}


void* TXRANDAPI fillbuffer(void* buffer, size_t size, size_t count)
{
    _Bool succ = call_rng(buffer, size * count);
    if (!succ) return NULL;
    else return buffer;
}

static _Bool insert_in_array(size_t srcIndex, size_t dstIndex, size_t bs, void* array) {
    if (srcIndex == dstIndex)return 1;
    void* buffer;
    size_t deltaIndex;
    buffer = malloc(bs);
    if (!buffer)return 0;
    memcpy(buffer,mboffset(array,srcIndex,bs),bs);
    if (srcIndex < dstIndex) { //前移后
        deltaIndex = dstIndex - srcIndex;
        memmove(mboffset(array, srcIndex, bs), mboffset(array, srcIndex + 1, bs), deltaIndex * bs);
    }
    else { //后移前
        deltaIndex = srcIndex - dstIndex;
        memmove(mboffset(array, dstIndex, bs), mboffset(array, dstIndex + 1, bs), deltaIndex * bs);
    }
    memcpy(buffer, mboffset(array, dstIndex, bs), bs);
    free(buffer);
    return 1;
}

// end arrop.c
