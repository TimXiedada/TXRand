// random.c
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
    
    
    这段代码实现了三个函数：random、uniform和uniforml，分别用于生成不同类型的随机数。
    其中，random函数生成double类型的随机数，randomf函数生成float类型的随机数，randoml函数生成long double类型的随机数。
    这些函数的实现方式都是调用obtain_cached_ull函数获取随机数种子，然后通过一定的转换方式将随机数种子转换为相应的随机数。
    
    uniform和uniforml函数用于生成一定范围内的随机数，
    在实现上先计算出范围内的差值delta，
    然后通过调用random或randoml函数生成[0,1)范围内的随机数nd，
    最后将delta与nd相乘得到最终的随机数。
    需要注意的是，如果调用random或randoml函数时失败了，将返回nan或nanl表示获取随机数失败。
    
    
*/

#include "txrand.h"
#include "txrandpr.h"

#include <limits.h>
#include <math.h>


double TXRANDAPI random(void)
{
    long long pn;
    double dpn,dr;
    const double dllmax = (double)LLONG_MAX, dllmin = (double)LLONG_MIN;


    do {
        _Bool succ = obtain_cached_ull(&pn, sizeof(pn));
        if (!succ) return nan("Failed to obtain random value!");
        dpn = (double)pn;
        if (dpn < 0.0L) {
            dr = dpn / dllmin;
        }
        else if (dpn > 0.0L) {
            dr = dpn / dllmax;
        }
        else { 
            dr = 0.0;
        }
    } while (dr == 1.0);
    return dr;
}

float TXRANDAPI randomf(void)
{
    long long pn;
    float fpn, fr;
    const float fllmax = (float)LLONG_MAX, fllmin = (float)LLONG_MIN;


    do {
        _Bool succ = obtain_cached_ull(&pn, sizeof(pn));
        if (!succ) return nanf("Failed to obtain random value!");
        fpn = (float)pn;
        if (fpn < 0.0F) {
            fr = fpn / fllmin;
        }
        else if (fpn > 0.0F) {
            fr = fpn / fllmax;
        }
        else {
            fr = 0.0F;
        }
    } while (fr == 1.0F);
    return fr;
}

long double TXRANDAPI randoml(void)
{
    long long pn;
    long double ldpn, ldr;
    const long double ldllmax = (long double)LLONG_MAX, ldllmin = (long double)LLONG_MIN;


    do {
        _Bool succ = obtain_cached_ull(&pn, sizeof(pn));
        if (!succ) return nanl("Failed to obtain random value!");
        ldpn = (long double)pn;
        if (ldpn < 0.0L) {
            ldr = ldpn / ldllmin;
        }
        else if (ldpn > 0.0L) {
            ldr = ldpn / ldllmax;
        }
        else {
            ldr = 0.0L;
        }
    } while (ldr == 1.0L);
    return ldr;
}


/*--------------------楚--------------------河--------------------汉--------------------界--------------------*/


long double TXRANDAPI uniforml(const long double a, const long double b) {
    if (a == b) return b;
    long double delta;
    delta = b > a ? b - a : a - b;
    long double nd = randoml();
    if (isnan(nd))return nanl("Failed to obtain random value!");
    delta *= nd;
    return a>b?delta+b:delta+a;
} 

double TXRANDAPI uniform(const double a, const double b) {
    if (a == b) return b;
    double delta;
    delta = b > a ? b - a : a - b;
    double nd = randoml();
    if (isnan(nd))return nan("Failed to obtain random value!");
    delta *= nd;
    return a > b ? delta + b : delta + a;
}

float TXRANDAPI uniformf(const float a, const float b) {
    if (a == b) return b;
    float delta;
    delta = b > a ? b - a : a - b;
    float nd = randomf();
    if (isnan(nd))return nanf("Failed to obtain random value!");
    delta *= nd;
    return a > b ? delta + b : delta + a;
}

// end random.c
