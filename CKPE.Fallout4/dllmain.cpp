// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Module.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <CKPE.Fallout4.Runner.h>
#include "resource_version2.h"

extern "C"
{
	__declspec(dllexport) CKPEGameLibraryData CKPEGameLibrary_Data =
	{
		CKPEGameLibraryData::kVersion,
		MAKE_EXE_VERSION_EX(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION),
		"Fallout4",
		"Perchik71",
	};

	__declspec(dllexport) std::uint32_t CKPEGameLibrary_Query()
	{
		CKPE::Fallout4::VersionLists::Verify();
		if (CKPE::Fallout4::VersionLists::HasAllowedEditorVersion())
			return CKPE::GameManager::SUPPORTED;
		if (CKPE::Fallout4::VersionLists::HasOutdatedEditorVersion())
			return CKPE::GameManager::DEPRECATED;
		return CKPE::GameManager::UNSUPPORTED;
	}

	__declspec(dllexport) bool CKPEGameLibrary_Load(const CKPEGameLibraryInterface* ckpe)
	{
		auto interface = CKPE::Common::Interface::GetSingleton();
		interface->Initialize(ckpe, CKPEGameLibrary_Data.dataVersion);
		interface->CmdLineHandler();
		auto runner = CKPE::Fallout4::Runner::GetSingleton();
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