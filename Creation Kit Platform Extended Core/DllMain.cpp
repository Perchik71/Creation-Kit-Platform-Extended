#include "Core/Engine.h"

using namespace CreationKitPlatformExtended::Core;

void WINAPI ForceLoadLibraryPerennially(HMODULE hModule)
{
    HMODULE temp;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCSTR)hModule, &temp);
}

void WINAPI GetAppName(PCHAR pBuff, SIZE_T nSize)
{
    CHAR modulePath[MAX_PATH];
    GetModuleFileName(GetModuleHandle(NULL), modulePath, MAX_PATH);

    _strlwr_s(modulePath);
    _splitpath_s(modulePath, NULL, 0, NULL, 0, pBuff, nSize, NULL, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // Процесс подключает эту библиотеку

        // Принудительно загрузить эту библиотеку постоянно
        ForceLoadLibraryPerennially(hModule);

        // Получение имени файла запускающего процесса без .exe и в нижнем регистре
        CHAR appName[MAX_PATH];
        GetAppName(appName, MAX_PATH);

        // Инициализация ядра
        auto Result = Engine::Initialize(hModule, appName);
        if (Failed(Result) && (Result != RC_UNKNOWN_APPLICATION))
            MessageBox(0, GetMessageStringByErrorNo(Result), "Error", MB_OK | MB_ICONERROR);
    }

    return TRUE;
}

