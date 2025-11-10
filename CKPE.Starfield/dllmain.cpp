// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Module.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.FileUtils.h>
#include <CKPE.Starfield.VersionLists.h>
#include <CKPE.Starfield.Runner.h>
#include "resource_version2.h"

extern "C"
{
	__declspec(dllexport) CKPEGameLibraryData CKPEGameLibrary_Data =
	{
		CKPEGameLibraryData::kVersion,
		MAKE_EXE_VERSION_EX(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION),
		"Starfield",
		"Perchik71",
	};

	__declspec(dllexport) std::uint32_t CKPEGameLibrary_Query(std::wstring& version)
	{
		CKPE::Starfield::VersionLists::Verify();
		version = CKPE::Starfield::VersionLists::GetEditorVersionByString();
		auto version_num = CKPE::Starfield::VersionLists::GetEditorVersionByNum();
		if (version_num)
		{
			auto ver = CKPE::FileUtils::GetFileVersion(CKPE::PathUtils::GetApplicationFileName());
			if (ver != version_num)
				return CKPE::GameManager::FAKE;
			if (CKPE::Starfield::VersionLists::HasAllowedEditorVersion())
				return CKPE::GameManager::SUPPORTED;
			if (CKPE::Starfield::VersionLists::HasOutdatedEditorVersion())
				return CKPE::GameManager::DEPRECATED;
		}
		return CKPE::GameManager::UNSUPPORTED;
	}

	__declspec(dllexport) bool CKPEGameLibrary_Load(const CKPEGameLibraryInterface* ckpe)
	{
		auto dialog_pakfn = CKPE::StringUtils::FormatString(L"CreationKitPlatformExtended_%s_Dialogs.pak",
			CKPE::Starfield::VersionLists::GetGameName().c_str());
		auto database_pakfn = CKPE::StringUtils::FormatString(L"CreationKitPlatformExtended_%s_Databases.pak",
			CKPE::Starfield::VersionLists::GetGameName().c_str());
		auto interface = CKPE::Common::Interface::GetSingleton();
		interface->Initialize(ckpe, CKPE::Starfield::VersionLists::GetEditorVersion(), CKPEGameLibrary_Data.dataVersion,
			dialog_pakfn, database_pakfn, CKPE::Starfield::VersionLists::GetDatabaseFileName(),
			CKPE::Starfield::VersionLists::GetExternalResourcePackageFileName(), false);
		interface->CmdLineHandler();
		auto runner = CKPE::Starfield::Runner::GetSingleton();
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