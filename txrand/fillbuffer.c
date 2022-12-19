#include "txrand.h"


void* TXRANDAPI fillbuffer(void *buffer, size_t size, unsigned count)
{
    _Bool succ = TXGetRand(buffer,size*count);
    if (!succ) return NULL;
    else return buffer;
}

