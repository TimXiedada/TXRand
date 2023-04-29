# TXRand文档——随机整数与随机浮点数生成

本文介绍`randint.c`和`random.c`定义的函数，分别用于生成随机整数和随机浮点数。



## randint.c

### `randbelow`系列函数

```c
int randbelow(const int n);
long randbelowl(const long n);
long long randbelowll(const long long n);
short randbelows(const short n);
unsigned randbelowu(const unsigned n);
unsigned long randbelowul(const unsigned long n);
unsigned long long randbelowull(const unsigned long long n);
unsigned short randbelowus(const unsigned short n);
```

返回 [0, n) 范围内的随机整数。类型详见函数声明。获取随机数失败时将返回0<!--并将全局变量`txrand_fail`设为非零值`-->。

### `randint`系列函数

```c
int randint(const int a, const int b);
long randintl(const long a, const long b);
long long randintll(const long long a, const long long b);
short randints(const short a, const short b);
unsigned randintu(const unsigned a, const unsigned b);
unsigned long randintul(const unsigned long a, const unsigned long b);
unsigned long long randintull(const unsigned long long a, const unsigned long long b);
unsigned short randintus(const unsigned short a, unsigned const short b);
```

返回随机整数 N 满足当 `a <= b` 时 `a <= N < b` ，当 `b < a` 时 `b <= N < a`。类型详见函数声明。获取随机数失败时将返回0<!--并将全局变量`txrand_fail`设为非零值`-->。

## random.c

### `random`系列函数

```c
double random(void);
float randomf(void);
long double randoml(void);
```

返回 [0.0, 1.0) 范围内的一个随机浮点数。类型详见函数声明。如果获取随机数失败了，将返回`nan`或`nanl`或`nanf`表示获取随机数失败<!--并将全局变量`txrand_fail`设为非零值`-->。

### `uniform`系列函数

```c
long double uniforml(const long double a, const long double b);
double uniform(const double a, const double b);
float uniformf(const float a, const float b);
```

返回一个随机浮点数 *N* ，当 `a <= b` 时 `a <= N < b` ，当 `b < a` 时 `b <= N < a`。类型详见函数声明。如果调用`random`或`randoml`或`randomf`函数时失败了，将返回`nan`或`nanl`或`nanf`表示获取随机数失败<!--并将全局变量`txrand_fail`设为非零值`-->。


