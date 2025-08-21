// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "windows.h"

#include <CKPE.Module.h>
#include <CKPE.MessageBox.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PluginAPI.Plugin.h>

using namespace CKPE;

extern "C"
{
    __declspec(dllexport) PluginAPI::CKPEPluginVersionData CKPEPlugin_Version =
    {
        CKPE::PluginAPI::CKPEPluginVersionData::kVersion,
        1,                                                      // version plug-in
        "Test plugin",
        "Perchik71",
        CKPE::PluginAPI::CKPEPluginVersionData::kGameSkyrimSE,
        0,
        { MAKE_EXE_VERSION_EX(1, 5, 73, 0), 0 },
        0
    };

    __declspec(dllexport) bool CKPEPlugin_HasDependencies() noexcept(true)
    {
        return false;
    }

    __declspec(dllexport) std::uint32_t CKPEPlugin_GetDependCount() noexcept(true)
    {
        return 0;
    }

    __declspec(dllexport) const char* CKPEPlugin_GetDependAt(std::uint32_t id) noexcept(true)
    {
        return nullptr;
    }

    __declspec(dllexport) std::uint32_t CKPEPlugin_Load(const PluginAPI::CKPEPluginInterface* intf) noexcept(true)
    {
        MessageBox::OpenInfo(StringUtils::FormatString("Good %u.%u.%u.%u", 
            GET_EXE_VERSION_EX_MAJOR(intf->RuntimeVersion), GET_EXE_VERSION_EX_MINOR(intf->RuntimeVersion),
            GET_EXE_VERSION_EX_BUILD(intf->RuntimeVersion), GET_EXE_VERSION_EX_REVISION(intf->RuntimeVersion)));

        return true;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

