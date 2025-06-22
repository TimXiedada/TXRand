// txrand.h
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _TXRAND_H_
#define _TXRAND_H_
#ifdef __cplusplus
extern "C" {
    typedef bool _Bool;
#endif
#include <stdlib.h>
#ifndef COMING_SOON
#define COMING_SOON
#endif // COMING_SOON
#ifndef TXRANDAPI 
#define TXRANDAPI __cdecl
#endif // TXRANDAPI
    //typedef void*  pvoid;
    // Wrapper to OS random number generator.
    _Bool __cdecl call_rng(void* buffer, size_t size);
    /*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
    //randbool.c
    _Bool TXRANDAPI randbool_w(const double weight);
    _Bool TXRANDAPI randbool_b(void); 
    _Bool TXRANDAPI randbool_q(void);
    #ifdef RANDBOOL_BINOM
        #define randbool randbool_b
    #else
        #define randbool randbool_q
    #endif // RANDBOOL_BINOM

    /*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
    //randint.c
    int TXRANDAPI randbelow(const int n);
    long TXRANDAPI randbelowl(const long n);
    long long TXRANDAPI randbelowll(const long long n);
    short TXRANDAPI randbelows(const short n);
    unsigned TXRANDAPI randbelowu(const unsigned n);
    unsigned long TXRANDAPI randbelowul(const unsigned long n);
    unsigned long long TXRANDAPI randbelowull(const unsigned long long n);
    unsigned short TXRANDAPI randbelowus(const unsigned short n);

    int TXRANDAPI randint(const int a, const int b);
    long TXRANDAPI randintl(const long a, const long b);
    long long TXRANDAPI randintll(const long long a, const long long b);
    short TXRANDAPI randints(const short a, const short b);
    unsigned TXRANDAPI randintu(const unsigned a, const unsigned b);
    unsigned long TXRANDAPI randintul(const unsigned long a, const unsigned long b);
    unsigned long long TXRANDAPI randintull(const unsigned long long a, const unsigned long long b);
    unsigned short TXRANDAPI randintus(const unsigned short a, unsigned const short b);
    #define dice() (randint(1,7))
    /*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
    //random.c
    double TXRANDAPI random(void);
    float TXRANDAPI randomf(void);
    long double TXRANDAPI randoml(void);

    long double TXRANDAPI uniforml(const long double a, const long double b);
    double TXRANDAPI uniform(const double a, const double b);
    float TXRANDAPI uniformf(const float a, const float b);

    /*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
    //arrop.c
    void* TXRANDAPI  sample(const void* pop, size_t size, size_t count, size_t k, void* dest);
    void* TXRANDAPI  choice(const void* seq, size_t size, size_t count); // ok tested
    void TXRANDAPI shuffle(void* x, size_t size, size_t count); // ok tested
    void* TXRANDAPI  fillbuffer(void* buffer, size_t size, size_t count); // ok tested

#ifdef __cplusplus
}
#endif
#endif
// end txrand.h
