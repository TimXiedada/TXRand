// txrandpr.h
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED ¡°AS IS¡± AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
// Private functions used in TXRand library.
#ifndef _TXRANDP_H_
#define _TXRANDP_H_
#include <stdlib.h>
#ifndef PRIVATEAPI
#define PRIVATEAPI __fastcall
#endif // PRIVATEAPI
size_t* PRIVATEAPI uniq_rand_arr(const size_t len, const size_t range, size_t* const arr);

_Bool PRIVATEAPI obtain_cached_ull(unsigned long long* const pull, const size_t sizeofull);

#endif
// end txrandpr.h
