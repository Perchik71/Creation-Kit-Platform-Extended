// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Module.h>
#include <CKPE.StringUtils.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <CKPE.SkyrimSE.Runner.h>
#include "resource_version2.h"

extern "C"
{
	__declspec(dllexport) CKPEGameLibraryData CKPEGameLibrary_Data =
	{
		CKPEGameLibraryData::kVersion,
		MAKE_EXE_VERSION_EX(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION),
		"SkyrimSE",
		"Perchik71",
	};

	__declspec(dllexport) std::uint32_t CKPEGameLibrary_Query(std::wstring& version)
	{
		CKPE::SkyrimSE::VersionLists::Verify();
		version = CKPE::SkyrimSE::VersionLists::GetEditorVersionByString();
		if (CKPE::SkyrimSE::VersionLists::HasAllowedEditorVersion())
			return CKPE::GameManager::SUPPORTED;
		if (CKPE::SkyrimSE::VersionLists::HasOutdatedEditorVersion())
			return CKPE::GameManager::DEPRECATED;
		return CKPE::GameManager::UNSUPPORTED;
	}

	__declspec(dllexport) bool CKPEGameLibrary_Load(const CKPEGameLibraryInterface* ckpe)
	{
		auto dialog_pakfn = CKPE::StringUtils::FormatString(L"CreationKitPlatformExtended_%s_Dialogs.pak",
			CKPE::SkyrimSE::VersionLists::GetGameName().c_str());
		auto database_pakfn = CKPE::StringUtils::FormatString(L"CreationKitPlatformExtended_%s_Databases.pak",
			CKPE::SkyrimSE::VersionLists::GetGameName().c_str());
		auto interface = CKPE::Common::Interface::GetSingleton();
		interface->Initialize(ckpe, CKPE::SkyrimSE::VersionLists::GetEditorVersionByNum(), CKPEGameLibrary_Data.dataVersion,
			dialog_pakfn, database_pakfn, CKPE::SkyrimSE::VersionLists::GetDatabaseFileName(), 
			CKPE::SkyrimSE::VersionLists::GetExternalResourcePackageFileName());
		interface->CmdLineHandler();
		auto runner = CKPE::SkyrimSE::Runner::GetSingleton();
		return runner->Install();
	}
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}