// Copyright © 2023-2025 aka CKPE team. All rights reserved.
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
			using TOpenPluginSaveDialog = bool(std::uintptr_t, const char*, const char*, const char*,
				const char*, void*, bool, bool, char*, std::uint32_t, const char*, void*);
			static std::function<TOpenPluginSaveDialog> AllowSaveESMandMasterESP_sub;

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

			bool AllowSaveESMandMasterESP::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
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
				using namespace Common;

				EditorAPI::pointer_TESFile_sub1 = ID(569310).Address();
				EditorAPI::pointer_TESFile_sub2 = ID(539487).Address();

				AllowSaveESMandMasterESP_sub = Relocation<TOpenPluginSaveDialog>(ID(555288)).Get();

				EditorAPI::TESFile::AllowSaveESM = _READ_OPTION_BOOL("CreationKit", "bAllowSaveESM", false);
				EditorAPI::TESFile::AllowMasterESP = _READ_OPTION_BOOL("CreationKit", "bAllowMasterESP", false);
				if (EditorAPI::TESFile::AllowSaveESM || EditorAPI::TESFile::AllowMasterESP)
				{
					*(std::uintptr_t*)&EditorAPI::TESFile::LoadTESInfo = Relocation(ID(363414)).WriteJump(&EditorAPI::TESFile::hk_LoadTESInfo);
					*(std::uintptr_t*)&EditorAPI::TESFile::WriteTESInfo = Relocation(ID(549393)).WriteJump(&EditorAPI::TESFile::hk_WriteTESInfo);
					
					if (EditorAPI::TESFile::AllowSaveESM)
					{
						// Also allow non-game ESMs to be set as "Active File"
						Relocation(ID(600608), Offset{ 0x47, 0x54 }).WriteCall(&IsActiveFileBlacklist);
						Relocation(ID(341913), 0x10E).WriteFill(NOP, 2);

						// Disable: "File '%s' is a master file or is in use.\n\nPlease select another file to save to."
						const char* newFormat = "File '%s' is in use.\n\nPlease select another file to save to.";

						Relocation(ID(551174), 0x96A).WriteFill(NOP, 12);
						Relocation(ID(106374)).Write(newFormat, strlen(newFormat) + 1);
						Relocation(ID(205873)).WriteJump(&OpenPluginSaveDialog);
					}

					if (EditorAPI::TESFile::AllowMasterESP)
						// Remove the check for IsMaster()
						Relocation(ID(1578), 0x29).WriteFill(NOP, 12);
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

				return AllowSaveESMandMasterESP_sub(ParentWindow, BasePath, filter, title, extension, nullptr,
					false, true, Buffer, BufferSize, Directory, nullptr);
			}
		}
	}
}