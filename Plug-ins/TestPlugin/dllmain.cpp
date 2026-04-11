// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "windows.h"

#include <CKPE.Module.h>
#include <CKPE.MessageBox.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.PluginAPI.PluginAPI.h>

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
        auto path = std::make_unique<wchar_t[]>(MAX_PATH);
        if (!path) return false;

        auto sfname = PathUtils::GetCKPELogsPluginPath() + L"TestPlugin.log";
        PathUtils::CreateFolder(PathUtils::ExtractFilePath(PathUtils::Normalize(sfname)));

        if (!PluginAPI::UserPluginLogger.Open(sfname))
        {
            CKPE::_ERROR(L"Couldn't create \"%s\" file", sfname.c_str());
            return false;
        }

        CKPE::_ERROR(L"create \"%s\" file", sfname.c_str());
        PluginAPI::_MESSAGE("TEST");
        _CONSOLE("Hello world");

        auto DialogManager = (PluginAPI::CKPEDialogManagerInterface*)intf->QueryInterface(PluginAPI::kInterface_DialogManager);
        if (DialogManager && DialogManager->InterfaceVersion == PluginAPI::CKPEDialogManagerInterface::kInterfaceVersion)
        {
            PluginAPI::_MESSAGE("Dialog 1000 %s", DialogManager->HasDialog(1000) ? "exist" : "no exist");
            PluginAPI::_MESSAGE("Dialog 122 %s", DialogManager->HasDialog(122) ? "exist" : "no exist");
        }
        else 
            CKPE::_ERROR("An error occurred when receiving the CKPEDialogManagerInterface or version mismatches");

    #if 0
        MessageBox::OpenInfo(StringUtils::FormatString("Good %u.%u.%u.%u", 
            GET_EXE_VERSION_EX_MAJOR(intf->RuntimeVersion), GET_EXE_VERSION_EX_MINOR(intf->RuntimeVersion),
            GET_EXE_VERSION_EX_BUILD(intf->RuntimeVersion), GET_EXE_VERSION_EX_REVISION(intf->RuntimeVersion)));
    #endif

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

