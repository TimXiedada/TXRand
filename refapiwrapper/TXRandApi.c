/*  SPDX-License-Identifier: CC0-1.0  */
/*
    A wrapper of Windows API to TXRand random source.
    Add this into your project if you would like to use TXRand on Windows XP or later version.
    Made by the author of TXRand. Licensed with CC0.
*/
/*

    这段代码实现了一个调用操作系统随机数生成器的函数call_os_rng，并使用了Microsoft CryptoAPI。具体分析如下：

    1.定义了一个全局变量phRandHandle，类型为HCRYPTPROV*，用于保存CryptoAPI生成的随机数生成器句柄。

    2.定义了一个静态函数InitRandSource，用于初始化随机数生成器。首先判断phRandHandle是否为NULL，如果不为NULL则直接返回。如果为NULL，则调用calloc函数分配一个HCRYPTPROV大小的内存，并将指针赋值给phRandHandle。接着调用CryptAcquireContext函数获取随机数生成器句柄，如果失败且错误码为0x80090016，则说明指定的密钥容器不存在，需要创建新的密钥容器，于是再次调用CryptAcquireContext函数并指定CRYPT_NEWKEYSET标志位。如果仍然失败且错误码为0x8009000F，则说明当前用户没有访问密钥容器的权限，于是再次调用CryptAcquireContext函数并不指定任何标志位。最后将获取到的随机数生成器句柄赋值给phRandHandle指向的地址，并返回获取成功的状态。

    3.定义了一个静态函数RandSourceClean，用于释放随机数生成器句柄。首先判断phRandHandle是否为NULL，如果是则直接返回。否则将phRandHandle指向的地址解引用得到随机数生成器句柄，调用CryptReleaseContext函数释放该句柄，并根据返回值判断是否释放成功。如果成功，则释放phRandHandle指向的内存并将其置为NULL，否则直接返回。

    4.实现了一个导出函数call_os_rng，用于调用操作系统随机数生成器并生成指定大小的随机数。首先判断phRandHandle是否为NULL，如果是则调用InitRandSource函数初始化随机数生成器并将RandSourceClean函数注册为进程退出清理函数。如果初始化失败则返回0。接着从phRandHandle指向的地址解引用得到随机数生成器句柄，调用CryptGenRandom函数生成指定大小的随机数，并将结果存储在buffer指向的地址中。最后返回生成成功的状态。

*/

#if (defined _WIN32) || (defined _WIN64) || (defined MINGW32) || (defined MINGW64)
#include <windows.h>
#ifdef __cplusplus
extern "C" bool __cdecl call_os_rng(void* buffer, size_t size);
#else
_Bool __cdecl call_os_rng(void* buffer, size_t size);
#endif

static HCRYPTPROV* phRandHandle = NULL;

static _Bool InitRandSource(void)
{
    if (phRandHandle) return 0;
    phRandHandle = calloc(1,sizeof(HCRYPTPROV));
    if (!phRandHandle) return 0;
    LPCTSTR UserName = TEXT("TimXiedadaRandomLibraryKeyContainer");
    HCRYPTPROV hRnd;
    _Bool succ = CryptAcquireContext(&hRnd, UserName, NULL, PROV_RSA_FULL, 0);
    if ((!succ) && GetLastError() == 0x80090016) succ = CryptAcquireContext(&hRnd, UserName, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
    if ((!succ) && GetLastError() == 0x8009000F) succ = CryptAcquireContext(&hRnd, UserName, NULL, PROV_RSA_FULL,0 );
    if(phRandHandle)*phRandHandle = hRnd;
    return succ;
}

static void RandSourceClean(void)
{
    if (!phRandHandle)return ;
    HCRYPTPROV randHandle = *phRandHandle;
    _Bool succ = CryptReleaseContext(randHandle, 0);
    if (succ)
    {
        free(phRandHandle);
        phRandHandle = NULL;
        return;
    }
    else return;
}

_Bool __cdecl call_os_rng(void* buffer, size_t size)
{
    if (!phRandHandle) {
        _Bool succ = InitRandSource();
        if (!succ) return 0;
        atexit(RandSourceClean);
    }
    HCRYPTPROV randHandle = *phRandHandle;
    return (_Bool)CryptGenRandom(randHandle, (DWORD)size, (BYTE*)buffer);
}
#elif defined __unix

#endif
