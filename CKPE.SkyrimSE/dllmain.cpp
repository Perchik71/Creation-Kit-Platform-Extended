// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.GameManager.h>
#include <CKPE.Module.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include "resource_version2.h"

extern "C"
{
	__declspec(dllexport) CKPEGameLibraryData CKPEGameLibrary_Data =
	{
		CKPEGameLibraryData::kVersion,
		MAKE_EXE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD),
		"SkyrimSE",
		"Perchik71",
	};

	__declspec(dllexport) std::uint32_t CKPEGameLibrary_Query()
	{
		CKPE::SkyrimSE::VersionLists::Verify();
		if (CKPE::SkyrimSE::VersionLists::HasAllowedEditorVersion())
			return CKPE::GameManager::CK_SUPPORTED;
		if (CKPE::SkyrimSE::VersionLists::HasOutdatedEditorVersion())
			return CKPE::GameManager::CK_DEPRECATED;
		return CKPE::GameManager::CK_UNKNOWN;
	}

	__declspec(dllexport) bool CKPEGameLibrary_Load(const CKPEGameLibraryData* ckpe)
	{
		return true;
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