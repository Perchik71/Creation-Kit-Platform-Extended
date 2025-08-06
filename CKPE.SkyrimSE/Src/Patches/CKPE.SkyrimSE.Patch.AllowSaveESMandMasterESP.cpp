// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/TESFile.h>
#include <Patches/CKPE.SkyrimSE.Patch.AllowSaveESMandMasterESP.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			extern uintptr_t pointer_TESFile_sub1;
			extern uintptr_t pointer_TESFile_sub2;
		}

		namespace Patch
		{
			uintptr_t pointer_AllowSaveESMandMasterESP_sub1 = 0;

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
				auto Path = EditorAPI::BSString::Utils::GetApplicationPath();
				bool vc_enabled =
					GetPrivateProfileIntA("General", "bUseVersionControl", 0, (Path + "CreationKit.ini").c_str()) ||
					GetPrivateProfileIntA("General", "bUseVersionControl", 0, (Path + "CreationKitCustom.ini").c_str());
				return !vc_enabled && (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST);
			}

			bool AllowSaveESMandMasterESP::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				EditorAPI::pointer_TESFile_sub1 = __CKPE_OFFSET(0);
				EditorAPI::pointer_TESFile_sub2 = __CKPE_OFFSET(1);
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
						// Also allow non-game ESMs to be set as "Active File"
						Detours::DetourClassCall(__CKPE_OFFSET(5), &EditorAPI::TESFile::IsActiveFileBlacklist);
						SafeWrite::WriteNop(__CKPE_OFFSET(6), 2);

						// Disable: "File '%s' is a master file or is in use.\n\nPlease select another file to save to."
						const char* newFormat = "File '%s' is in use.\n\nPlease select another file to save to.";

						SafeWrite::WriteNop(__CKPE_OFFSET(7), 12);
						SafeWrite::Write(__CKPE_OFFSET(8), (std::uint8_t*)newFormat, strlen(newFormat) + 1);
						Detours::DetourJump(__CKPE_OFFSET(9), (std::uintptr_t)&OpenPluginSaveDialog);
					}

					if (EditorAPI::TESFile::AllowMasterESP)
						// Remove the check for IsMaster()
						SafeWrite::WriteNop(__CKPE_OFFSET(10), 12);
				}

				return true;
			}

			bool AllowSaveESMandMasterESP::OpenPluginSaveDialog(std::uintptr_t ParentWindow, const char* BasePath, bool IsESM,
				char* Buffer, std::uint32_t BufferSize, const char* Directory)
			{
				if (!BasePath)
					BasePath = "\\Data";

				const char* filter = "TES Plugin Files (*.esp)\0*.esp\0TES Light Master Files (*.esl)\0*.esl\0TES Master Files (*.esm)\0*.esm\0\0";
				const char* title = "Select Target Plugin";
				const char* extension = "esp";

				if (IsESM)
				{
					filter = "TES Master Files (*.esm)\0*.esm\0\0";
					title = "Select Target Master";
					extension = "esm";
				}

				return ((bool(__fastcall*)(std::uintptr_t, const char*, const char*, const char*,
					const char*, void*, bool, bool, char*, std::uint32_t, const char*, void*))
					pointer_AllowSaveESMandMasterESP_sub1)(ParentWindow, BasePath, filter, title, extension, nullptr,
						false, true, Buffer, BufferSize, Directory, nullptr);
			}
		}
	}
}