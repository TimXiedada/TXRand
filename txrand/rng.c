// rng.c
/*  SPDX-License-Identifier: ISC  */
/*
    Copyright 2021-2023 Xie Youtian

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED ��AS IS�� AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "txrand.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>

/**
 * ������һ����RAND_FUNC_BITS_PROVIDED�����ڼ����׼�⺯��rand()����ֵ���ṩ�Ķ�����λ����
 * ����ͨ����RAND_MAX��1Ȼ��ȡ��2Ϊ�׵Ķ�������ȷ��rand()��������ֵ����������λ����
 * RAND_MAX�Ǳ�׼����rand()�����ܷ��ص����ֵ����1��Ϊ�˴���߽������
 * ���⣬RFBS��һ����������ʾRAND_FUNC_BITS_PROVIDED��ֵ����Ϊԭ���ĺ궨������̫����������^�ᣩ
 */
#define RAND_FUNC_BITS_PROVIDED ((unsigned)log2(((unsigned)RAND_MAX + 1u)))
#define RFBS RAND_FUNC_BITS_PROVIDED


// ����������ƽ̨�Ƿ�֧��RDRANDָ���չ�������Զ������Ƿ�ʹ��RDRANDָ�������������
#if (defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)) && \
    (defined(__GNUC__) || defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER))
    #if defined(__has_include)
        #if __has_include(<immintrin.h>)
            #define TXRAND_HAS_IMMINTRIN 1
            #include <immintrin.h>
        #else
            #define TXRAND_HAS_IMMINTRIN 0
        #endif
    #else
        #define TXRAND_HAS_IMMINTRIN 1
        #include <immintrin.h>
    #endif
#else
        #define TXRAND_HAS_IMMINTRIN 0
#endif

/**
 * StdlibRandFuncValueBitwise����ʹ�ñ�׼���rand()���������������ֵ��
 * �ú���ͨ��λ��������ȡ�������ÿһλ��������һ������ֵ��0��1����
 * ��ʵ��ȷ����ÿ�ε��ö��ܷ���һ���µ��������ֵ���Ҳ����˷����ɵ�������ݡ�
 *
 * @return ����һ���������ֵ��0��1����
 */

static _Bool __fastcall StdlibRandFuncValueBitwise(void) {
    // �˺���������һ������Ĳ���ֵ��
    static _Bool first_run; // ָʾ������������Ƿ��ѳ�ʼ��
    // r ���ڴ洢һ���������i ����ָʾ��һ��Ҫʹ�õ�λ��λ��
    static unsigned r, i;

    if (!first_run) {
        void* p = malloc(1);
        srand((unsigned)time(NULL) ^ (unsigned)clock() ^ (unsigned)p);
        free(p);
        first_run = 1;
    }

    // ����ǰ�����������λ����ʹ��ʱ����������������һ���µ������
    if (!i)
        // RFBS ��һ����������ʾ������� r ��ʹ�õ�λ��
        i = RFBS,
        // ����һ���µ������
        r = rand();

    // �������������һλ�����ݼ������Թ���һ��ʹ��
    return (r >> --i) & 1u;
}
/**
 * ����Ĭ���������������rand��������仺������
 *
 * �˺���ʹ��λ��������׼���е�����������������ϵ��������С�
 * �Ի�������ÿ���ֽڽ�����λ������ȷ���������ֵ�����Ծ��ȷֲ��ڻ������ϣ�Ҳ�����˷����ɵ�������ݡ�
 *
 * @param buffer ָ��Ҫ���������ݵĻ�������ָ�롣
 * @param size �������Ĵ�С�����ֽ�Ϊ��λ����
 * @return ���Ƿ���1����ʾ��������ɣ�����ʵ�ʹ������󣬴���ƿ�����Ҫ���¿��ǣ���
 */
static _Bool __cdecl call_default_rng(void* buffer, size_t size) {
    // ��黺����ָ��ʹ�С�Ƿ���Ч�������Чֱ�ӷ���1���棩����Ϊ���ջ��������ǱȽ�6�ġ�
    if (!buffer || !size) return 1;

    size_t wbyte, wbit;
    // ������������ÿ���ֽ�
    for (wbyte = 0; wbyte < size; wbyte++) {
        // �����ֽڵ�ÿ��λ
        for (wbit = 0; wbit < CHAR_BIT; wbit++) {
            // ʹ��λ����������������������Ľ����ϵ���������
            // ���StdlibRandFuncValueBitwise()�����棬�򻺳�����Ӧλ����Ϊ1��
            // ��������Ϊ0����ȷ���˻�������ÿλ���л��ᱻ������������޸ġ�
            // register _Bool rv = rv;
            ((unsigned char*)buffer)[wbyte] ^= (StdlibRandFuncValueBitwise() ? (1u << wbit) : (0u));
        }
    }
    // ����1���棩����ʾ�����ɹ����
    return (_Bool)1;
}

#if TXRAND_HAS_IMMINTRIN

/**
 * �����������������仺������
 *
 * �˺���ʹ��RDRANDָ���������������������䵽ָ���Ļ������С�
 * ���RDRANDָ����û�ʧ�ܣ�����˵�Ĭ�ϵ������������ʵ�֡�
 * ���buffer��size��Ч����ֱ�ӷ���1���棩��
 * ���size����4�ı�������ᴦ��ʣ��Ĳ���4�ֽڵĲ��֣����ᶪ�������1~3�ֽڣ�û��Ҫ��ô���ţ��Ͼ�RDRANDʹ�õ�CPU���ڲ���rand()�࣬���죩��
 *
 * @param buffer ָ��Ҫ���������ݵĻ�������ָ�롣
 * @param size �������Ĵ�С�����ֽ�Ϊ��λ����
 * @return ���Ƿ���1����ʾ��������ɣ�����ʵ�ʹ������󣬴���ƿ�����Ҫ���¿��ǣ���
 */
_Bool __cdecl call_rng(void* buffer, size_t size) {
    if (!buffer || !size) return 1;
    unsigned char* p = (unsigned char*)buffer;
    size_t filled = 0;
    unsigned val;

    while (filled + sizeof(val) <= size) {
        if (!_rdrand32_step(&val)) {
            // RDRANDʧ�ܣ����˵�Ĭ��ʵ��
            return call_default_rng(p + filled, size - filled);
        }
        memcpy(p + filled, &val, sizeof(val));
        filled += sizeof(val);
    }
    // ����ʣ�಻��4�ֽڵĲ���
    if (filled < size) {
        if (!_rdrand32_step(&val)) {
            return call_default_rng(p + filled, size - filled);
        }
        memcpy(p + filled, &val, size - filled);
    }
    return 1;
}

#else
/**
 * �����������������仺������
 *
 * �˺���ʹ��λ��������׼���е��������������rand�����������ϵ��������У���Ϊ��֪����ƽ̨�Ƿ��������������������ʵ�֡�
 * �μ� call_default_rng() ������ʵ�֡�
 * 
 * @param buffer ָ��Ҫ���������ݵĻ�������ָ�롣
 * @param size �������Ĵ�С�����ֽ�Ϊ��λ����
 * @return ���Ƿ���1����ʾ��������ɣ�����ʵ�ʹ������󣬴���ƿ�����Ҫ���¿��ǣ���
 */

_Bool __cdecl call_rng(void* buffer, size_t size) {
	return call_default_rng(buffer, size);
}

#endif
