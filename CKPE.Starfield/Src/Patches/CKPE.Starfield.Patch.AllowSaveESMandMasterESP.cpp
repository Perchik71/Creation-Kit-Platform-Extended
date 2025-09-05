// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/TESFile.h>
#include <Patches/CKPE.Starfield.Patch.AllowSaveESMandMasterESP.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			extern std::uintptr_t pointer_TESFile_sub1;
			extern std::uintptr_t pointer_TESFile_sub2;
			extern std::uintptr_t pointer_TESFile_sub3;
			extern std::uintptr_t pointer_TESFile_data;
		}

		namespace Patch
		{
			std::uintptr_t pointer_AllowSaveESMandMasterESP_sub1 = 0;

			static BOOL OpenPluginSaveDialog(HWND ParentWindow, BOOL IsESM, LPSTR Buffer,
				std::uint32_t BufferSize)
			{
				auto BasePath = StringUtils::Utf16ToWinCP(PathUtils::GetDataPath());

				const char* filter = "TES Plugin Files (*.esp)\0*.esp\0TES Master Files (*.esm)\0*.esm\0\0";
				const char* title = "Select Target Plugin";
				const char* extension = "esp";

				if (IsESM)
				{
					filter = "TES Master Files (*.esm)\0*.esm\0\0";
					title = "Select Target Master";
					extension = "esm";
				}

				return ((bool(__fastcall*)(HWND, const char*, const char*, const char*, const char*, void*, bool, bool, char*,
					std::uint32_t, const char*, void*, bool))
					pointer_AllowSaveESMandMasterESP_sub1)(ParentWindow, BasePath.c_str(), filter, title, extension,
						nullptr, false, true, Buffer, BufferSize, nullptr, nullptr, false);
			}

			AllowSaveESMandMasterESP::AllowSaveESMandMasterESP() : Common::Patch()
			{
				SetName("Allow Save ESM and Master ESP");
			}

			bool AllowSaveESMandMasterESP::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AllowSaveESMandMasterESP::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AllowSaveESMandMasterESP::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> AllowSaveESMandMasterESP::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool AllowSaveESMandMasterESP::DoQuery() const noexcept(true)
			{
				auto Path = PathUtils::GetApplicationPath();
				bool vc_enabled =
					GetPrivateProfileIntW(L"General", L"bUseVersionControl", 0, (Path + L"CreationKit.ini").c_str()) ||
					GetPrivateProfileIntW(L"General", L"bUseVersionControl", 0, (Path + L"CreationKitCustom.ini").c_str());
				return !vc_enabled && (VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0);
			}

			bool AllowSaveESMandMasterESP::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				EditorAPI::pointer_TESFile_sub1 = __CKPE_OFFSET(0);
				EditorAPI::pointer_TESFile_sub2 = __CKPE_OFFSET(1);
				EditorAPI::pointer_TESFile_data = __CKPE_OFFSET(2);
				pointer_AllowSaveESMandMasterESP_sub1 = __CKPE_OFFSET(11);

				EditorAPI::TESFile::AllowSaveESM = _READ_OPTION_BOOL("CreationKit", "bAllowSaveESM", false);
				EditorAPI::TESFile::AllowMasterESP = _READ_OPTION_BOOL("CreationKit", "bAllowMasterESP", false);

				if (EditorAPI::TESFile::AllowSaveESM || EditorAPI::TESFile::AllowMasterESP)
				{
					*(std::uintptr_t*)&EditorAPI::TESFile::LoadTESInfo =
						Detours::DetourClassJump(__CKPE_OFFSET(3), &EditorAPI::TESFile::hk_LoadTESInfo);
					*(std::uintptr_t*)&EditorAPI::TESFile::WriteTESInfo =
						Detours::DetourClassJump(__CKPE_OFFSET(4), &EditorAPI::TESFile::hk_WriteTESInfo);

					if (EditorAPI::TESFile::AllowSaveESM)
					{
						EditorAPI::pointer_TESFile_sub3 = Detours::DetourClassJump(__CKPE_OFFSET(12),
							&EditorAPI::TESFile::hk_ConversionPlugin);

						// Also allow non-game ESMs to be set as "Active File"
						Detours::DetourClassCall(__CKPE_OFFSET(5), &EditorAPI::TESFile::IsMasterFileToBlacklist);
						SafeWrite::WriteNop(__CKPE_OFFSET(6), 2);

						// Disable: "File '%s' is a master file or is in use.\n\nPlease select another file to save to."
						const char* newFormat = "File '%s' is in use.\n\nPlease select another file to save to.";

						//SafeWrite::Nop(__CKPE_OFFSET(7), 13);	IsMaster() cut Bethesda
						SafeWrite::Write(__CKPE_OFFSET(8), (uint8_t*)newFormat, (uint32_t)(strlen(newFormat) + 1));
						Detours::DetourJump(__CKPE_OFFSET(9), (uintptr_t)&OpenPluginSaveDialog);
					}

					if (EditorAPI::TESFile::AllowMasterESP)
						// Remove the check for IsMaster()
						SafeWrite::WriteNop(__CKPE_OFFSET(10), 9);
				}

				return true;
			}
		}
	}
}