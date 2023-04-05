/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED ��AS IS�� AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "txrand.h"
#include <limits.h>
/*
    ��δ��붨����������������������������Ĳ���ֵ��
    
    ��һ������ randbool ����һ������ weight���������������ɲ���ֵΪ true �ĸ��ʡ�
    ���ȵ��� random() ��������һ�� [0,1) ֮�������� i��Ȼ�� i �� weight ���бȽϣ�
    ��� i С�� weight���򷵻� true�����򷵻� false��
    
    �ڶ������� randbool_b �����ɲ���ϵͳ�������������װ���� obtain_cached_ull ����һ�� 64 λ������� num��
    Ȼ��ͳ�����������ж�����λ 0 �� 1 �ĸ�������� 0 �ĸ������� 1 �ĸ������򷵻� true�����򷵻� false��
    ע�����ﻹ������������λ�����˴���������λΪ 1���򷵻�ȡ���Ľ�������򷵻�ԭʼ�����
    
    ���������� randbool_q Ҳ�ǲ����ɲ���ϵͳ�������������װ���� obtain_cached_ull ����һ�� 64 λ������� num��
    Ȼ����������������λ��ʼ��ÿ��ȡһλ����������ֱ��ȡ�� 64 λ��
    �����һλΪ 1���򷵻� true�����򷵻� false��
    ���������ʹ���˾�̬���������浱ǰ�Ѿ����ɵ�������ͻ�ʣ��Ķ�����λ�����Ա��´ε���ʱ����ʹ�á�
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