/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED ��AS IS�� AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


/*


    ����һ�����ɲ��ظ����������Ĵ���⡣���ṩ�����ֲ�ͬ���㷨��ʵ�����Ŀ�꣬������ȫ�ֱ��� uniq_rand_arr_version_flag ������ʹ���ĸ��㷨��

    ���� uniq_rand_arr_v1 ����򵥵İ汾����ʹ��������ѭ����������������飬��һ��ѭ����������ÿ����������ڶ���ѭ������������ɵ�������Ƿ���֮ǰ��������ظ����������ظ�ʱ�����������������

    uniq_rand_arr_v2 ʹ����һ�ָ���Ч���㷨��������������һ��ָ����Χ�ڵ�����Ȼ��ѭ����������������������е�Ԫ��ɾ���������ɵ������Ϊ 0 ʱ��ɾ������ͷ��Ԫ�ء����������Ϊ 0 ʱ���ҵ������ж�Ӧ��Ԫ�أ�ɾ������

    uniq_rand_arr_v3 Ҳ������һ�ָ�Ч���㷨�������ɳ�Ϊ��Χ�Ľṹ�����飬����ÿ��Ԫ�ذ���һ�����ֺ�һ��Ȩ�ء�Ȩ���ɲ���ϵͳ��������������ɡ�Ȼ������鰴��Ȩ������������ִ�������������С�

    obtain_cached_ull ��һ������������������һ�������л�ȡһ���޷��ų������������������һ����̬���飬��ʼ��ʱʹ�ò���ϵͳ�������������䡣ÿ�ε���ʱ�������Ὣ���������һ��Ԫ�ط��أ����������� 1��

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
            head = current->next; // ��ͷ
            arr[i] = current->n;
            free(current); // �ڴ������
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

/*--------------------��--------------------��--------------------��--------------------��--------------------*/

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
