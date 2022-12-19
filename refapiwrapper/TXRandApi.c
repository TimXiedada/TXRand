#include "private/TXRandApi.h"
#ifdef _WIN32
#include <windows.h>

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

static _Bool RandSourceClean(void)
{
    if (!phRandHandle)return 0;
    HCRYPTPROV randHandle = *phRandHandle;
    _Bool succ = CryptReleaseContext(randHandle, 0);
    if (succ)
    {
        free(phRandHandle);
        phRandHandle = NULL;
        return succ;
    }
    else return succ;
}

_Bool __cdecl TXGetRand(void* buffer, size_t size)
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