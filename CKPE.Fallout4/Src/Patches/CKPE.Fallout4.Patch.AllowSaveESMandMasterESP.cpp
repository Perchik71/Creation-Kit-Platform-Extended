// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/TESFile.h>
#include <Patches/CKPE.Fallout4.Patch.AllowSaveESMandMasterESP.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			extern std::uintptr_t pointer_TESFile_sub1;
			extern std::uintptr_t pointer_TESFile_sub2;
		}

		namespace Patch
		{
			std::uintptr_t pointer_AllowSaveESMandMasterESP_sub1 = 0;

			static BOOL OpenPluginSaveDialog(HWND ParentWindow, LPCSTR BasePath, BOOL IsESM, LPSTR Buffer,
				uint32_t BufferSize, LPCSTR Directory) noexcept(true)
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

				return ((bool(__fastcall*)(HWND, const char*, const char*, const char*, const char*, void*, bool, bool, char*,
					uint32_t, const char*, void*))
					pointer_AllowSaveESMandMasterESP_sub1)(ParentWindow, BasePath, filter, title, extension, nullptr, false, true, Buffer,
						BufferSize, Directory, nullptr);
			}

			static bool IsActiveFileBlacklist(const EditorAPI::TESFile* a_file) noexcept(true)
			{
				return a_file->IsActiveFileBlacklist(true) || (!a_file->IsActive() && a_file->IsActiveFileWarn());
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
				auto Path = EditorAPI::BSString::Utils::GetApplicationPath();
				bool vc_enabled =
					GetPrivateProfileIntA("General", "bUseVersionControl", 0, (Path + "CreationKit.ini").c_str()) ||
					GetPrivateProfileIntA("General", "bUseVersionControl", 0, (Path + "CreationKitCustom.ini").c_str());
				return !vc_enabled && (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST);
			}

			bool AllowSaveESMandMasterESP::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				EditorAPI::pointer_TESFile_sub1 = ID{ 465268, 1942584 }.Address();
				EditorAPI::pointer_TESFile_sub2 = ID{ 445270, 1777850 }.Address();
				pointer_AllowSaveESMandMasterESP_sub1 = ID{ 411272, 1353832 }.Address();

				EditorAPI::TESFile::AllowSaveESM = _READ_OPTION_BOOL("CreationKit", "bAllowSaveESM", false);
				EditorAPI::TESFile::AllowMasterESP = _READ_OPTION_BOOL("CreationKit", "bAllowMasterESP", false);

				if (EditorAPI::TESFile::AllowSaveESM || EditorAPI::TESFile::AllowMasterESP)
				{
					*(std::uintptr_t*)&EditorAPI::TESFile::LoadTESInfo =
						Common::Relocation(Common::ID{ 289312, 1493949 }).WriteJump(&EditorAPI::TESFile::hk_LoadTESInfo);
					*(std::uintptr_t*)&EditorAPI::TESFile::WriteTESInfo =
						Common::Relocation(Common::ID(445271)).WriteJump(&EditorAPI::TESFile::hk_WriteTESInfo);

					if (EditorAPI::TESFile::AllowSaveESM)
					{
						// Also allow non-game ESMs to be set as "Active File"
						Relocation(ID(534861), 0x5F).WriteCall(&IsActiveFileBlacklist);
						Relocation(ID{ 23248, 1716794 }, Offset{ 0x48, 0x4F }).WriteFill(NOP, 2);

						// Disable: "File '%s' is a master file or is in use.\n\nPlease select another file to save to."
						auto newFormat = "File '%s' is in use.\n\nPlease select another file to save to.";

						Relocation(ID{ 148381, 1380402 }, Offset{ 0x489, 0x55A }).WriteFill(NOP, 0xD);
						Relocation(ID(180829)).Write(newFormat, strlen(newFormat) + 1);
						Relocation(ID{ 273721, 1631676 }).WriteJump(&OpenPluginSaveDialog);
					}

					if (EditorAPI::TESFile::AllowMasterESP)
						// Remove the check for IsMaster()
						Relocation(ID{ 445380, 1436783 }, 0x5C).WriteFill(NOP, 9);
				}

				return true;
			}
		}
	}
}