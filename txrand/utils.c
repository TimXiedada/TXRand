/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


/*


    这是一个生成不重复随机数数组的代码库。它提供了三种不同的算法来实现这个目标，并根据全局变量 uniq_rand_arr_version_flag 来决定使用哪个算法。

    其中 uniq_rand_arr_v1 是最简单的版本，它使用了两个循环来生成随机数数组，第一个循环依次生成每个随机数，第二个循环用来检查生成的随机数是否与之前的随机数重复。当发现重复时，重新生成随机数。

    uniq_rand_arr_v2 使用了一种更高效的算法。它首先生成了一个指定范围内的链表，然后循环生成随机数，并将链表中的元素删除。当生成的随机数为 0 时，删除链表头部元素。当随机数不为 0 时，找到链表中对应的元素，删除它。

    uniq_rand_arr_v3 也采用了一种高效的算法。它生成长为范围的结构体数组，其中每个元素包含一个数字和一个权重。权重由操作系统随机数生成器生成。然后对数组按照权重排序，最后将数字存入随机数数组中。

    obtain_cached_ull 是一个辅助函数，用来从一个缓存中获取一个无符号长整型数。这个缓存是一个静态数组，初始化时使用操作系统随机数生成器填充。每次调用时，函数会将缓存中最后一个元素返回，并将索引减 1。

*/

#include "txrand.h"
#include <string.h>
int uniq_rand_arr_version_flag = 0;
const int default_uniq_rand_arr_version_flag = 3;


static size_t* PRIVATEAPI uniq_rand_arr_v2(const size_t len, const size_t range, size_t* const arr) {
    if (!arr||!range||!len) return NULL;
    typedef struct _node {
        size_t n;
        struct _node* next;
    }node, * pnode;
    size_t i, j, k;  
    size_t* ka = malloc(sizeof(k)*len);
    if (!ka)return NULL;
    pnode head, current, temp;
    head = (pnode)malloc(sizeof(struct _node));
    if (!head)return NULL;
    current = head;
    current->n = 0;    
    call_os_rng(ka, sizeof(k)* len);
    for (i = 1; i < range; i++) {
        current->next = malloc(sizeof(struct _node));
        if (!(current->next))return NULL;
        current = current->next;
        current->n = i;
    }
    current->next = NULL;
    for (i = 0; i < len; i++) {
        current = head;
        k = ka[i]%(range - i);
        if (!k) {
            head = current->next; // 掐头
            arr[i] = current->n;
            free(current); // 内存免费了
            continue;
        }
        for (j = 0; j < k - 1; j++)if (current)current = current->next; else return NULL;
        arr[i] = current->next->n;
        temp = current->next->next;
        free(current->next);
        current->next = temp;
    }
    current = head;
    if (!current)goto End;
    while (current->next) {
        temp = current->next;
        free(current);
        current = temp;
    }
    End:return arr;
}

struct pair { size_t n, w; };
int paircmp(const void* a, const void* b) {
    return ((struct pair*)a)->w < ((struct pair*)b)->w ? 1 : ((struct pair*)a)->w >((struct pair*)b)->w ? -1 : 0;
}
static size_t* PRIVATEAPI uniq_rand_arr_v3(const size_t len, const size_t range, size_t* const arr) {
    if (!range || !len || !arr)return NULL;;
    struct pair* pa = (struct pair*)malloc(sizeof(struct pair) * range);
    if (!pa)return NULL;
    size_t i;
    if (!call_os_rng(pa, sizeof(struct pair) * range)) { free(pa); return NULL; };
    for (i = 0; i < range; i++)pa[i].n = i;
    qsort(pa, range, sizeof(struct pair), paircmp);
    for (i = 0; i < range; i++)arr[i] = pa[i].n;
    free(pa);
    return arr;
}

static size_t* PRIVATEAPI uniq_rand_arr_v1(const size_t len, const size_t range, size_t* const arr) {
    if (!arr) return NULL;
    size_t i, b, i2;
    for (i = 0; i < len; i++) {
    GetRandomUnsigned: {
        if (!call_os_rng(&b, sizeof(b))) return NULL;
        b %= range;
    }
    for (i2 = 0; i2 < i; i2++) {
        if (arr[i2] == b) goto GetRandomUnsigned;
    }
    arr[i] = b;
    }
    return arr;
}

size_t* PRIVATEAPI uniq_rand_arr(const size_t len, const size_t range, size_t* const arr) {
    start:switch (uniq_rand_arr_version_flag) {
    case 1:
        return uniq_rand_arr_v1(len, range, arr);
    case 2:
        return uniq_rand_arr_v2(len, range, arr);
    case 3:
        return uniq_rand_arr_v3(len, range, arr);
    default:
        uniq_rand_arr_version_flag = default_uniq_rand_arr_version_flag;
        goto start;
    }
}

/*--------------------楚--------------------河--------------------汉--------------------界--------------------*/

#ifndef ULL_BUF_SIZE
#define ULL_BUF_SIZE 8192
#endif

_Bool PRIVATEAPI obtain_cached_ull(unsigned long long* const pull,const size_t sizeofull) {
    static unsigned long long ullcache[ULL_BUF_SIZE];
    static int i;
    if ((!pull)||(sizeofull!=sizeof(unsigned long long)))return 0;
    if ((!i)){
        if(!call_os_rng(ullcache, sizeof(ullcache)))return 0;
        else i = ULL_BUF_SIZE;
    }

    *pull = ullcache[--i];
    return 1;
} 
