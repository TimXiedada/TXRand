// randint.c
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED ��AS IS�� AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*  
    �����������£�
    randbelow������һ��С�ڸ�����ֵ�����������
    randbelowl��ͬ�ϣ��������ɵ��ǳ����͡�
    randbelowll��ͬ�ϣ��������ɵ��ǳ������͡�
    randbelows��ͬ�ϣ��������ɵ��Ƕ����͡�
    randbelowu��ͬ�ϣ��������ɵ����޷������͡�
    randbelowul��ͬ�ϣ��������ɵ����޷��ų����͡�
    randbelowull��ͬ�ϣ��������ɵ����޷��ų������͡�
    randbelowus��ͬ�ϣ��������ɵ����޷��Ŷ����͡�
    randint������һ��������Χ�ڵ����������
    randintl��ͬ�ϣ��������ɵ��ǳ����͡�
    randintll��ͬ�ϣ��������ɵ��ǳ������͡�
    randints��ͬ�ϣ��������ɵ��Ƕ����͡�
    randintu��ͬ�ϣ��������ɵ����޷������͡�
    randintul��ͬ�ϣ��������ɵ����޷��ų����͡�
    randintull��ͬ�ϣ��������ɵ����޷��ų������͡�
    randintus��ͬ�ϣ��������ɵ����޷��Ŷ����͡�

    ���У���������ɵĺ����� obtain_cached_ull �����������������Ƕ��������һЩ�򵥵ķ�װ��ʹ����ӷ���ʹ�á�
*/

#include "txrandpr.h"
#include "txrand.h"
int TXRANDAPI  randbelow(const int n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num,sizeof(num));
    if (!succ) return -1;
    int ret = n ? num % n : num;
    return ret;
}

long TXRANDAPI  randbelowl(const long n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num, sizeof(num));
    if (!succ) return -1;
    long ret = n ? num % n : num;
    return ret;
}
long long TXRANDAPI  randbelowll(const long long n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num, sizeof(num));
    if (!succ) return -1;
    long long ret = n ? num % n : num;
    return ret;
}
short TXRANDAPI  randbelows(const short n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num, sizeof(num));
    if (!succ) return -1;
    short ret = n ? num % n : num;
    return ret;
}
unsigned TXRANDAPI  randbelowu(const unsigned n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num, sizeof(num));
    if (!succ) return -1;
    unsigned ret = n ? num % n : num;
    return ret;
}
unsigned long TXRANDAPI  randbelowul(const unsigned long n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num, sizeof(num));
    if (!succ) return -1;
    unsigned long ret = n ? num % n : num;
    return ret;
}
unsigned long long TXRANDAPI  randbelowull(const unsigned long long n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num, sizeof(num));
    if (!succ) return -1;
    unsigned long long ret = n ? num % n : num;
    return ret;
}

unsigned short TXRANDAPI  randbelowus(const unsigned short n)
{
    unsigned long long num;
    _Bool succ = obtain_cached_ull(&num, sizeof(num));
    if (!succ) return -1;
    unsigned short ret = n ? num % n : num;
    return ret;
}

/*--------------------��--------------------��--------------------��--------------------��--------------------*/


int TXRANDAPI randint(const int a, const int b)
{
    int delta = b > a ? b - a : a - b;
    if (a == b)return a;
    int k = randbelow(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

long TXRANDAPI randintl(const long a, const long b)
{
    long delta = b > a ? b - a : a - b;
    if (a == b)return a;
    long k = randbelowl(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

long long TXRANDAPI randintll(const long long a, const long long b)
{
    long long delta = b > a ? b - a : a - b;
    if (a == b)return a;
    long long k = randbelowll(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

short TXRANDAPI randints(const short a, const short b)
{
    short delta = b > a ? b - a : a - b;
    if (a == b)return a;
    short k = randbelows(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned TXRANDAPI randintu(const unsigned a, const unsigned b)
{
    unsigned delta = b > a ? b - a : a - b;
    if (a == b)return a;
    unsigned k = randbelowu(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned long TXRANDAPI randintul(const unsigned long a, const unsigned long b)
{
    unsigned long delta = b > a ? b - a : a - b;
    if (a == b)return a;
    unsigned long k = randbelowul(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned long long TXRANDAPI randintull(const unsigned long long a, const unsigned long long b)
{
    unsigned long long delta = b > a ? b - a : a - b;
    if (a == b)return a;
    unsigned long long k = randbelowull(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned short TXRANDAPI randintus(const unsigned short a, const unsigned short b)
{
    unsigned short delta = b > a ? b - a : a - b;
    if (a == b)return a;
    unsigned short k = randbelowus(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}
// end randint.c
