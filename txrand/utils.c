// utils.c
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


/*


*/

#include "txrand.h"
#include "txrandpr.h"

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
    call_rng(ka, sizeof(k)* len);
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
static int paircmp(const void* a, const void* b) {
    return ((struct pair*)a)->w < ((struct pair*)b)->w ? 1 : ((struct pair*)a)->w >((struct pair*)b)->w ? -1 : 0;
}
static size_t* PRIVATEAPI uniq_rand_arr_v3(const size_t len, const size_t range, size_t* const arr) {
    if (!range || !len || !arr)return NULL;;
    struct pair* pa = (struct pair*)malloc(sizeof(struct pair) * range);
    if (!pa)return NULL;
    size_t i;
    if (!call_rng(pa, sizeof(struct pair) * range)) { free(pa); return NULL; };
    for (i = 0; i < range; i++)pa[i].n = i;
    qsort(pa, range, sizeof(struct pair), paircmp);
    for (i = 0; i < range; i++)arr[i] = pa[i].n;
    free(pa);
    return arr;
}

/*
更高效的方式是“Fisher–Yates 洗牌算法”（也称为Knuth Shuffle），它能在线性时间O(n)内生成唯一随机数组，且只需O(range)空间。
核心思想：
1. 先初始化一个顺序数组[0, 1, ..., range-1]。
2. 从0到len-1，依次将当前位置与后面任意一个位置交换。
3. 取前len个元素即为结果。

伪代码：
- 分配一个长度为range的临时数组tmp，填充0~range-1。
- for i from 0 to len-1:
    - 生成随机数j in [i, range-1]
    - 交换tmp[i]和tmp[j]
    - arr[i] = tmp[i]
- 释放tmp

我TM怎么没想到呢？诶不对，我想到过，但是觉得不行（
*/
static size_t* PRIVATEAPI uniq_rand_arr_v4(const size_t len, const size_t range, size_t* const arr) {
    if (!arr || !range || !len || len > range) return NULL;
    size_t* tmp = (size_t*)malloc(sizeof(size_t) * range);
    if (!tmp) return NULL;
    for (size_t i = 0; i < range; ++i) tmp[i] = i;
    for (size_t i = 0; i < len; ++i) {
        size_t j, r;
        if (!call_rng(&r, sizeof(r))) { free(tmp); return NULL; }
        j = i + (r % (range - i));
        size_t t = tmp[i];
        tmp[i] = tmp[j];
        tmp[j] = t;
        arr[i] = tmp[i];
    }
    free(tmp);
    return arr;
}


static size_t* PRIVATEAPI uniq_rand_arr_v1(const size_t len, const size_t range, size_t* const arr) {
    if (!arr || !range || !len || len > range) return NULL;
    size_t i, b, i2;
    for (i = 0; i < len; i++) {
    GetRandomUnsigned: {
        if (!call_rng(&b, sizeof(b))) return NULL;
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
    case 4:
        return uniq_rand_arr_v4(len, range, arr);
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
        if(!call_rng(ullcache, sizeof(ullcache)))return 0;
        else i = ULL_BUF_SIZE;
    }

    *pull = ullcache[--i];
    return 1;
} 
// end utils.c
