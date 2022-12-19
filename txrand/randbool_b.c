#include "txrand.h"


_Bool TXRANDAPI  randbool_b(void)
{
    unsigned long long num;
    int x[] = { 0,0 };
    TXGetRand(&num, sizeof(num));
    // num &= ~(1ULL<<63);
    int i;
    for (i = 0; i < 8*(sizeof num)-1; i++)
    {
        x[(num >> i) & 1]++;
    }
    _Bool retb = (x[0] < x[1]);
    return num >> (8 * (sizeof num)-1) ? !retb : retb;
}

