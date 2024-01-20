// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "AllowSaveESMandMasterESPF4.h"
#include "Editor API/FO4/TESFileF4.h"

namespace CreationKitPlatformExtended
{
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
					pointer_TESFile_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
					pointer_TESFile_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_TESFile_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_AllowSaveESMandMasterESP_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(11));

					TESFile::AllowSaveESM = _READ_OPTION_BOOL("CreationKit", "bAllowSaveESM", false);
					TESFile::AllowMasterESP = _READ_OPTION_BOOL("CreationKit", "bAllowMasterESP", false);

					if (TESFile::AllowSaveESM || TESFile::AllowMasterESP)
					{
						*(uintptr_t*)&TESFile::LoadTESInfo =
							Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3)),
								&TESFile::hk_LoadTESInfo);
						*(uintptr_t*)&TESFile::WriteTESInfo = 
							Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4)),
								&TESFile::hk_WriteTESInfo);

						if (TESFile::AllowSaveESM)
						{
							// Also allow non-game ESMs to be set as "Active File"
							lpRelocator->DetourCall(lpRelocationDatabaseItem->At(5),
								&TESFile::IsMasterFileToBlacklist);
							lpRelocator->PatchNop(lpRelocationDatabaseItem->At(6), 2);

							// Disable: "File '%s' is a master file or is in use.\n\nPlease select another file to save to."
							const char* newFormat = "File '%s' is in use.\n\nPlease select another file to save to.";

							lpRelocator->PatchNop(lpRelocationDatabaseItem->At(7), 13);
							lpRelocator->Patch(lpRelocationDatabaseItem->At(8), (uint8_t*)newFormat, 
								(uint32_t)(strlen(newFormat) + 1));

							lpRelocator->DetourJump(lpRelocationDatabaseItem->At(9),
								(uintptr_t)&OpenPluginSaveDialog);
						}

						if (TESFile::AllowMasterESP)
							// Remove the check for IsMaster()
							lpRelocator->PatchNop(lpRelocationDatabaseItem->At(10), 9);
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

			bool AllowSaveESMandMasterESPPatch::OpenPluginSaveDialog(HWND ParentWindow, const char* BasePath, bool IsESM,
				char* Buffer, uint32_t BufferSize, const char* Directory)
			{
				if (!BasePath)
					BasePath = "\\Data";

				const char* filter = "TES Plugin Files (*.esp)\0*.esp\0TES Master Files (*.esm)\0*.esm\0\0";
				const char* title = "Select Target Plugin";
				const char* extension = "esp";

				if (IsESM)
				{
					filter = "TES Master Files (*.esm)\0*.esm\0\0";
					title = "Select Target Master";
					extension = "esm";
				}

				auto result = ((bool(__fastcall*)(HWND, const char*, const char*, const char*, 
					const char*, void*, bool, bool, char*, uint32_t, const char*, void*))
					pointer_AllowSaveESMandMasterESP_sub1)(ParentWindow, BasePath, filter, title, extension, nullptr,
						false, true, Buffer, BufferSize, Directory, nullptr);

				INIConfig CK_CfgCustom = INIConfig("CreationKitCustom.ini");
				if (result && CK_CfgCustom.ReadBool("General", "bUseVersionControl", false)) {
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