/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED ��AS IS�� AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


#include "txrand.h"
#include <limits.h>
#include <math.h>

double TXRANDAPI random(void)
{
    long long pn;
    double dpn,dr;
    const double dllmax = (double)LLONG_MAX, dllmin = (double)LLONG_MIN;


    do {
        _Bool succ = TXGetRand(&pn, sizeof(pn));
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
        _Bool succ = TXGetRand(&pn, sizeof(pn));
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
        _Bool succ = TXGetRand(&pn, sizeof(pn));
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


/*--------------------��--------------------��--------------------��--------------------��--------------------*/


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

