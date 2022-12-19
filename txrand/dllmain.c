// dllmain.c : 定义 DLL 应用程序的入口点。
#include <windows.h>
#include "txrand_withapi.h"
BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        InitRandSource();
    case DLL_THREAD_ATTACH:
        InitRandSource();
    case DLL_THREAD_DETACH:
 //       RandSourceClean();
    case DLL_PROCESS_DETACH:
 //       RandSourceClean();

        break;
    }
    
    
    return TRUE;
}

