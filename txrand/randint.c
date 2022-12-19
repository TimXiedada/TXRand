#include "txrand.h"

int TXRANDAPI  randbelow(const int n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num,sizeof(num));
    if (!succ) return -1;
    int ret = num % n;
    return ret;
}

long TXRANDAPI  randbelowl(const long n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num, sizeof(num));
    if (!succ) return -1;
    long ret = num % n;
    return ret;
}
long long TXRANDAPI  randbelowll(const long long n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num, sizeof(num));
    if (!succ) return -1;
    long long ret = num % n;
    return ret;
}
short TXRANDAPI  randbelows(const short n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num, sizeof(num));
    if (!succ) return -1;
    short ret = num % n;
    return ret;
}
unsigned TXRANDAPI  randbelowu(const unsigned n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num, sizeof(num));
    if (!succ) return -1;
    unsigned ret = num % n;
    return ret;
}
unsigned long TXRANDAPI  randbelowul(const unsigned long n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num, sizeof(num));
    if (!succ) return -1;
    unsigned long ret = num % n;
    return ret;
}
unsigned long long TXRANDAPI  randbelowull(const unsigned long long n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num, sizeof(num));
    if (!succ) return -1;
    unsigned long long ret = num % n;
    return ret;
}

unsigned short TXRANDAPI  randbelowus(const unsigned short n)
{
    unsigned long long num;
    _Bool succ = TXGetRand(&num, sizeof(num));
    if (!succ) return -1;
    unsigned short ret = num % n;
    return ret;
}

/*--------------------³þ--------------------ºÓ--------------------ºº--------------------½ç--------------------*/


int TXRANDAPI randint(const int a, const int b)
{
    int delta = b > a ? b - a : a - b;
    
    int k = randbelow(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

long TXRANDAPI randintl(const long a, const long b)
{
    long delta = b > a ? b - a : a - b;
    
    long k = randbelowl(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

long long TXRANDAPI randintll(const long long a, const long long b)
{
    long long delta = b > a ? b - a : a - b;
    
    long long k = randbelowll(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

short TXRANDAPI randints(const short a, const short b)
{
    short delta = b > a ? b - a : a - b;
    
    short k = randbelows(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned TXRANDAPI randintu(const unsigned a, const unsigned b)
{
    unsigned delta = b > a ? b - a : a - b;
    
    unsigned k = randbelowu(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned long TXRANDAPI randintul(const unsigned long a, const unsigned long b)
{
    unsigned long delta = b > a ? b - a : a - b;
    
    unsigned long k = randbelowul(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned long long TXRANDAPI randintull(const unsigned long long a, const unsigned long long b)
{
    unsigned long long delta = b > a ? b - a : a - b;
    
    unsigned long long k = randbelowull(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}

unsigned short TXRANDAPI randintus(const unsigned short a, const unsigned short b)
{
    unsigned short delta = b > a ? b - a : a - b;
    
    unsigned short k = randbelowus(delta);
    if (k == -1) return 0;
    return b > a ? a + k : b + k;
}
