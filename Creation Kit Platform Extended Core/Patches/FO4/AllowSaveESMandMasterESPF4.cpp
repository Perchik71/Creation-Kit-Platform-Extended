// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "AllowSaveESMandMasterESPF4.h"
#include "Patches/INICacheData.h"
#include "Editor API/FO4/TESFileF4.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		extern CreationKitPlatformExtended::Patches::INICacheDataPatch* INICacheData;
	}

	namespace EditorAPI
	{
		namespace Fallout4
		{
			extern uintptr_t pointer_TESFile_sub1;
			extern uintptr_t pointer_TESFile_sub2;
			extern uintptr_t pointer_TESFile_data;
		}
	}

	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

			uintptr_t pointer_AllowSaveESMandMasterESP_sub1 = 0;

			AllowSaveESMandMasterESPPatch::AllowSaveESMandMasterESPPatch() : Module(GlobalEnginePtr)
			{}

			bool AllowSaveESMandMasterESPPatch::HasOption() const
			{
				return false;
			}

			bool AllowSaveESMandMasterESPPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* AllowSaveESMandMasterESPPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* AllowSaveESMandMasterESPPatch::GetName() const
			{
				return "Allow Save ESM and Master ESP";
			}

			bool AllowSaveESMandMasterESPPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> AllowSaveESMandMasterESPPatch::GetDependencies() const
			{
				return { "Console" };
			}

			bool AllowSaveESMandMasterESPPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool AllowSaveESMandMasterESPPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					pointer_TESFile_sub1 = _RELDATA_ADDR(0);
					pointer_TESFile_sub2 = _RELDATA_ADDR(1);
					pointer_TESFile_data = _RELDATA_ADDR(2);
					pointer_AllowSaveESMandMasterESP_sub1 = _RELDATA_ADDR(11);

					TESFile::AllowSaveESM = _READ_OPTION_BOOL("CreationKit", "bAllowSaveESM", false);
					TESFile::AllowMasterESP = _READ_OPTION_BOOL("CreationKit", "bAllowMasterESP", false);

					if (TESFile::AllowSaveESM || TESFile::AllowMasterESP)
					{
						*(uintptr_t*)&TESFile::LoadTESInfo =
							voltek::detours_function_class_jump(_RELDATA_ADDR(3), &TESFile::hk_LoadTESInfo);
						*(uintptr_t*)&TESFile::WriteTESInfo = 
							voltek::detours_function_class_jump(_RELDATA_ADDR(4), &TESFile::hk_WriteTESInfo);

						if (TESFile::AllowSaveESM)
						{
							// Also allow non-game ESMs to be set as "Active File"
							lpRelocator->DetourCall(_RELDATA_RAV(5), &TESFile::IsMasterFileToBlacklist);
							lpRelocator->PatchNop(_RELDATA_RAV(6), 2);

							// Disable: "File '%s' is a master file or is in use.\n\nPlease select another file to save to."
							const char* newFormat = "File '%s' is in use.\n\nPlease select another file to save to.";

							lpRelocator->PatchNop(_RELDATA_RAV(7), 13);
							lpRelocator->Patch(_RELDATA_RAV(8), (uint8_t*)newFormat, (uint32_t)(strlen(newFormat) + 1));
							lpRelocator->DetourJump(_RELDATA_RAV(9), (uintptr_t)&OpenPluginSaveDialog);
						}

						if (TESFile::AllowMasterESP)
							// Remove the check for IsMaster()
							lpRelocator->PatchNop(_RELDATA_RAV(10), 9);
					}


					return true;
				}

				return false;
			}

			bool AllowSaveESMandMasterESPPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			BOOL AllowSaveESMandMasterESPPatch::OpenPluginSaveDialog(HWND ParentWindow, LPCSTR BasePath, BOOL IsESM, LPSTR Buffer,
				uint32_t BufferSize, LPCSTR Directory)
			{
				if (!BasePath)
					BasePath = "\\Data";

				const char* filter = "TES Plugin Files (*.esp)\0*.esp\0TES Light Master Files (*.esl)\0*.esp\0TES Master Files (*.esm)\0*.esm\0\0";
				const char* title = "Select Target Plugin";
				const char* extension = "esp";

				if (IsESM)
				{
					filter = "TES Master Files (*.esm)\0*.esm\0\0";
					title = "Select Target Master";
					extension = "esm";
				}

				auto result = ((BOOL(__fastcall*)(HWND, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPVOID, BOOL, BOOL, LPSTR, uint32_t, LPCSTR, LPVOID))
					pointer_AllowSaveESMandMasterESP_sub1)(ParentWindow, BasePath, filter, title, extension, NULL, FALSE, TRUE, Buffer,
						BufferSize, Directory, NULL);

				bool bUseVersionControl = false;

				if (INICacheData->HasActive())
					bUseVersionControl = (bool)INICacheData->HKGetPrivateProfileIntA("General", "bUseVersionControl", 0,
						(EditorAPI::BSString::Utils::GetApplicationPath() + "CreationKitCustom.ini").c_str());
				else
					bUseVersionControl = (bool)GetPrivateProfileIntA("General", "bUseVersionControl", 0,
						(EditorAPI::BSString::Utils::GetApplicationPath() + "CreationKitCustom.ini").c_str());

				if (result && bUseVersionControl)
				{
					std::string sbuf = Buffer;
					auto ibegin = sbuf.find_last_of('\\');
					if (ibegin == sbuf.npos) {
						ibegin = sbuf.find_last_of('/');
						if (ibegin == sbuf.npos)
							goto end_func;
						else
							sbuf = sbuf.substr(ibegin + 1);
					}
					else
						sbuf = sbuf.substr(ibegin + 1);

					strcpy_s(Buffer, BufferSize, sbuf.c_str());
				}
			end_func:
				return result;
			}
		}
	}
}