// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include "Voltek.MemoryManager.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Проинициализируем библиотеку.
        // Бывают случаи инжекта, тогда библиотеку лучше инициализировать,
        // При добавлении процессом, однако, повторный вызов функции в коде,
        // не повлечёт утечки - Безопасно.
        voltek::scalable_memory_manager_initialize();
        break;
    case DLL_THREAD_ATTACH:
        // Исключено.
    case DLL_THREAD_DETACH:
        // Исключено.
    case DLL_PROCESS_DETACH:
        // Отключение библиотеки отдано на откуп программисту.
        // Да и это менеджер памяти, он должен жить весь процесс, глупо вырубать его,
        // во всяком случаи в среде Windows.
        // Процесс иногда вызывает эту часть кода, даже не планируя умирать,
        // и отключение библиотеки отсюда, приводят к CTD.
        break;
    }

    return TRUE;
}

