// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include "CKPE.WinHttp.Native.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CKPE::WinHttp::Init();
        break;
    case DLL_PROCESS_DETACH:
        CKPE::WinHttp::Shutdown();
        break;
    }
    return TRUE;
}