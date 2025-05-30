// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.GameManager.h>

extern "C"
{
	// for obse64 0.1 alpha and newer
	__declspec(dllexport) CKPEGameLibraryData CKPEGameLibrary_Data =
	{
		CKPEGameLibraryData::kVersion,
		1,
		"Starfield",
		"Perchik71",
	};

	__declspec(dllexport) bool CKPEGameLibrary_Load(const CKPEGameLibraryData* obse)
	{
		return false;
	}
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
       
        break;
    case DLL_PROCESS_DETACH:
        
        break;
    }
    return TRUE;
}