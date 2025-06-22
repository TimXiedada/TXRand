// arrop.c
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED ��AS IS�� AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


#include "txrand.h"
#include "txrandpr.h"
#include <string.h>

/*


    ��δ���ʵ����һЩ��������ɺ����������صĺ���������������£�

    uniq_rand_arr �������ú�������һ������ len ��Ԫ�ص����飬ÿ��Ԫ�ص�ֵ�� 0 �� range-1 ֮�䣬��ÿ��Ԫ�ص�ֵ������ͬ���ú���ʹ���� calloc ���������ڴ���䣬�����ط��������ָ�롣

    mboffset �������ú���ʵ����һ��ָ��ƫ�ƺ�������ָ�� pstart ����ƫ�� offsetbbs ���ֽڣ�������ƫ�ƺ��ָ�롣�ú���ʹ���� size_t ���͵ı�������ָ�����㣬������ת��Ϊ void ���͵�ָ�뷵�ء�

    choice �������ú����� seq ���������ѡ��һ��Ԫ�أ������ظ�Ԫ�ص�ָ�롣�������ȵ����� call_os_rng ��������һ������� b��Ȼ����� count ȡģ���õ�һ���� 0 �� count-1 ֮�������������ʹ�� mboffset ������������Ԫ�ص�ָ�벢���ء�

    sample �������ú����� pop ���������ѡ�� k ��Ԫ�أ������Ǹ��Ƶ� dest �����У������� dest �����ָ�롣Ϊ�˱����ظ�ѡ��Ԫ�أ������ȵ��� uniq_rand_arr ��������һ������ k ����ͬԪ�ص����� simarr��Ȼ�����ν� pop �����ж�Ӧ�±��Ԫ�ظ��Ƶ� dest �����С�����ʹ���� calloc ���������ڴ���䣬���ں���ִ����Ϻ��ͷ� simarr �� _simarr ������ڴ档

    memswp �������ú���ʵ����һ��ָ�뽻����������ָ�� a ��ָ�� b ָ��Ĵ�СΪ size ���ڴ����򽻻����������ȼ��ָ�� a ��ָ�� b �Ƿ���ͬ����������ָ����ڴ������Ƿ����ص����������ֱ�ӷ��ء����򣬺���ʹ����������������ָ�� a ��ָ�� b ָ����ڴ����򣬲����ؽ����Ĵ�����

    shuffle �������ú���ʵ����һ������ϴ�ƺ����������� seq �е�Ԫ���������˳�򡣺������ȵ��� uniq_rand_arr ��������һ������ count ����ͬԪ�ص����� sa��Ȼ������ʹ�� memswp ������������ seq �е�Ԫ�غ����� sa �ж�Ӧ�±��Ԫ�ء�����ʹ���� calloc ���������ڴ���䣬���ں���ִ����Ϻ��ͷ� sa ������ڴ档

    fillbuffer �������ú���ʹ�� call_os_rng �������� count ������ֽڣ�����������䵽 buffer ָ����ڴ������С��������� buffer ָ�룬������������ʧ���򷵻� NULL��


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
    if (srcIndex < dstIndex) { //ǰ�ƺ�
        deltaIndex = dstIndex - srcIndex;
        memmove(mboffset(array, srcIndex, bs), mboffset(array, srcIndex + 1, bs), deltaIndex * bs);
    }
    else { //����ǰ
        deltaIndex = srcIndex - dstIndex;
        memmove(mboffset(array, dstIndex, bs), mboffset(array, dstIndex + 1, bs), deltaIndex * bs);
    }
    memcpy(buffer, mboffset(array, dstIndex, bs), bs);
    free(buffer);
    return 1;
}

// end arrop.c
