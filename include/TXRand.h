#ifndef _TXRAND_H_
#define _TXRAND_H_
#ifndef __cplusplus

#endif
#include <stdlib.h>
#define COMING_SOON
#ifndef TXRANDAPI 
#define TXRANDAPI __cdecl

#endif

//typedef void*  pvoid;

_Bool __cdecl TXGetRand                      (void *buffer, size_t size);
/*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
//misc
_Bool TXRANDAPI randbool                       (const double weight); // ok tested
_Bool TXRANDAPI randbool_b                     (void); // ok tested

/*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
//randint.c
int TXRANDAPI randbelow                       (const int n); 
long TXRANDAPI randbelowl                     (const long n);
long long TXRANDAPI randbelowll               (const long long n);
short TXRANDAPI randbelows                    (const short n);
unsigned TXRANDAPI randbelowu                 (const unsigned n);
unsigned long TXRANDAPI randbelowul           (const unsigned long n);
unsigned long long TXRANDAPI randbelowull     (const unsigned long long n);
unsigned short TXRANDAPI randbelowus          (const unsigned short n);

int TXRANDAPI randint                         (const int a, const int b);
long TXRANDAPI randintl                       (const long a, const long b);
long long TXRANDAPI randintll                 (const long long a, const long long b);
short TXRANDAPI randints                      (const short a, const short b);
unsigned TXRANDAPI randintu                   (const unsigned a, const unsigned b);
unsigned long TXRANDAPI randintul             (const unsigned long a, const unsigned long b);
unsigned long long TXRANDAPI randintull       (const unsigned long long a, const unsigned long long b);
unsigned short TXRANDAPI randintus            (const unsigned short a, unsigned const short b);

/*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
//random.c
double TXRANDAPI random                       (void);
float TXRANDAPI randomf                       (void);
long double TXRANDAPI randoml                 (void);

long double TXRANDAPI uniforml                (const long double a, const long double b);
double TXRANDAPI uniform                      (const double a, const double b);
float TXRANDAPI uniformf                      (const float a, const float b);

/*--------------------楚--------------------河--------------------汉--------------------界--------------------*/
//arrop.c
void*  TXRANDAPI  sample                        (const void* pop, size_t size, size_t count, size_t k, void* dest); 
void*  TXRANDAPI  choice                        (const void* seq, size_t size, size_t count); // ok tested
void TXRANDAPI shuffle                        (void *x, size_t size, size_t count); // ok tested
void*  TXRANDAPI  fillbuffer                    (void *buffer, size_t size, unsigned count); // ok tested


#endif
