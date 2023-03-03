#include "txrand.h"
#include <string.h>



static inline void* PRIVATEAPI mboffset(const void* pstart, size_t offsetb, size_t bs) {
    size_t _pdest;
    void* pdest;
    _pdest = (size_t)pstart;
    _pdest += bs * offsetb;
    pdest = (void*)_pdest;
    return pdest;
}

static size_t* PRIVATEAPI uniq_rand_arr(size_t len,size_t range,size_t* arr) {
    if (!arr) return NULL;
    typedef struct _node {
        size_t n;
        struct _node* next;
    }node,*pnode;
    pnode head, current, temp;
    head = (pnode)malloc(sizeof (struct _node));
    if (!head)return NULL;
    current = head;
    current->n = 0;
    size_t i, j, k;
    for (i = 1; i < range; i++) {
        current->next = malloc(sizeof(struct _node));
        if (!(current->next))return NULL;
        current = current->next;
        current->n = i;
    }
    current->next = NULL;
    for (i = 0; i < len; i++) {
        current = head;
        k = randbelow(range-i);
        if (!k) {
            head = current->next; // 掐头
            arr[i] = current->n;
            free(current); // 内存免费了
            continue;
        }
        for (j = 0; j < k-1; j++)if (current)current = current->next; else return NULL;
        arr[i]=current->next->n;
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
/*
struct pair { size_t n, w; };
int paircmp(void * a,void * b) {
    return ((struct pair*)a)->w < ((struct pair*)b)->w ? 1 : ((struct pair*)a)->w >((struct pair*)b)->w ? -1 : 0;
}
static size_t* PRIVATEAPI uniq_rand_arr_v3(size_t len, size_t range, size_t* arr) {
    
}
*/
static size_t* PRIVATEAPI uniq_rand_arr_v1(size_t len, size_t range, size_t* arr) {
    if (!arr) return NULL;
    size_t i, b, i2;
    for (i = 0; i < len; i++) {
        GetRandomUnsigned: {
            if (!TXGetRand(&b, sizeof(b))) return NULL;
            b %= range;
        }
        for (i2 = 0; i2 < i; i2++) {
            if (arr[i2] == b) goto GetRandomUnsigned;
        }
        arr[i] = b;
    }
    return arr;
}
 void*  TXRANDAPI choice(const void* seq, size_t size, size_t count)
{
    size_t b;
    {
    if (!TXGetRand(&b, sizeof(b))) return NULL;
    b %= count;
    }
    return mboffset(seq,b,size);
}

 void*  TXRANDAPI sample(const void* pop, size_t size, size_t count, size_t k, void* dest) {
    size_t *_simarr,*simarr, i;
    _simarr = calloc(k,size);
    if (!_simarr) return NULL;
    simarr = uniq_rand_arr(k,count,_simarr);
    if (!simarr) { free(_simarr); return NULL; };
    for (i = 0; i < k; i++) {
        memcpy(mboffset(dest, i, size),mboffset(pop,simarr[i],size), size);
    }
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
    if (!uniq_rand_arr(count, count, sa)) {
        free(sa);
        return;
    };
    for (i = 0; i < count; i++) {
        memswp(mboffset(seq, sa[i], size), mboffset(seq, i, size), size);
    }
    free(sa);
    return;
}

/*
static _Bool InsertInArray(size_t srcIndex, size_t dstIndex,
                          size_t bs, void* array) {
    if (srcIndex == dstIndex)return 1;
    void* buffer;
    size_t deltaIndex;
    buffer = malloc(bs);
    if (!buffer)return 0;
    memcpy(buffer,mboffset(array,srcIndex,bs),bs);
    if (srcIndex < dstIndex) { //前移后
        deltaIndex = dstIndex - srcIndex;
        memmove(mboffset(array, srcIndex, bs), mboffset(array, srcIndex + 1, bs), deltaIndex * bs);
    }
    else { //后移前
        deltaIndex = srcIndex - dstIndex;
        memmove(mboffset(array, dstIndex, bs), mboffset(array, dstIndex + 1, bs), deltaIndex * bs);
    }
    memcpy(buffer, mboffset(array, dstIndex, bs), bs);
    free(buffer);
    return 1;
}
*/
