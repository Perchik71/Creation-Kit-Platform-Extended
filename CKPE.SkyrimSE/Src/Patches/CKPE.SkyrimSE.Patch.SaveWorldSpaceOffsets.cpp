// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <string>
#include <cstring>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/T/TESFile.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESDataHandler.h>
#include <EditorAPI/T/TESVersionControl.h>
#include <Patches/CKPE.SkyrimSE.Patch.SaveWorldSpaceOffsets.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			// Save(this = TESDataHandler, file name) -> bool
			using Save_t = bool(*)(void*, const char*);
			static Save_t pSave = nullptr;

			static bool HasSourceFile(const EditorAPI::Forms::TESForm* form, const EditorAPI::TESFile* file) noexcept(true)
			{
				auto info = form->GetModInfo();
				if (!info || !info->entries)
					return false;

				for (std::uint32_t i = 0; i < info->size; i++)
					if (info->entries[i] == file)
						return true;

				return false;
			}

			// With bAllowSaveESM the CK keeps the active ESM flagged as a plain plugin (the flag is stripped on load and
			// again while the header is written), so the in-memory flag cannot tell whether the saved file is a master.
			static bool IsMasterExtension(const char* name) noexcept(true)
			{
				auto extension = strrchr(name, '.');
				return extension && !_stricmp(extension, ".esm");
			}

			// OpenFile only accepts a master; flag the file as one while it is mapped, then restore the flag.
			class MasterFlagGuard
			{
				EditorAPI::TESFile* _file;
				bool _changed;
			public:
				MasterFlagGuard(EditorAPI::TESFile* file, bool set) noexcept(true) : _file(file), _changed(set)
				{
					if (_changed)
						_file->SetMaster(true);
				}

				~MasterFlagGuard() noexcept(true)
				{
					if (_changed)
						_file->SetMaster(false);
				}
			};

			static bool GetFileStamp(const std::string& path, WIN32_FILE_ATTRIBUTE_DATA& data) noexcept(true)
			{
				ZeroMemory(&data, sizeof(data));
				return GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data) != FALSE;
			}

			static bool SameStamp(const WIN32_FILE_ATTRIBUTE_DATA& a, const WIN32_FILE_ATTRIBUTE_DATA& b) noexcept(true)
			{
				return (a.ftLastWriteTime.dwLowDateTime == b.ftLastWriteTime.dwLowDateTime) &&
					(a.ftLastWriteTime.dwHighDateTime == b.ftLastWriteTime.dwHighDateTime) &&
					(a.nFileSizeLow == b.nFileSizeLow) && (a.nFileSizeHigh == b.nFileSizeHigh);
			}

			SaveWorldSpaceOffsets::SaveWorldSpaceOffsets() : Common::Patch()
			{
				SetName("Save WorldSpace Offsets");
			}

			bool SaveWorldSpaceOffsets::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* SaveWorldSpaceOffsets::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bSaveWorldSpaceOffsets";
			}

			bool SaveWorldSpaceOffsets::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> SaveWorldSpaceOffsets::GetDependencies() const noexcept(true)
			{
				return { "Console", "TESDataHandler" };
			}

			bool SaveWorldSpaceOffsets::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool SaveWorldSpaceOffsets::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				*(std::uintptr_t*)&pSave = Relocation(ID(551174)).WriteJump(&hk_Save);

				return true;
			}

			void SaveWorldSpaceOffsets::UpdateOffsets() noexcept(true)
			{
				auto handler = EditorAPI::TESDataHandler::Singleton.Singleton;
				if (!handler)
					return;

				// OFST is only read by the game for master files (ESM)
				auto file = handler->ActiveMod;
				if (!file)
					return;

				const bool was_master = file->IsMaster();
				if (!was_master && !IsMasterExtension(file->GetFileName().c_str()))
				{
					_CONSOLE("Save WorldSpace Offsets: \"%s\" is not a master file, skipped", file->GetFileName().c_str());
					return;
				}

				MasterFlagGuard master_flag(file, !was_master);

				auto& gate = EditorAPI::TESVersionControl::OpenFileGate();
				auto& batch = EditorAPI::TESVersionControl::BatchMode();
				auto& target = EditorAPI::TESVersionControl::TargetFile();

				const auto prev_gate = gate;
				const auto prev_batch = batch;
				const auto prev_target = target;

				std::uint32_t updated = 0, failed = 0;

				// OpenFile is normally only available with version control; the CK's batch tools open master files
				// without it by raising this gate for the duration of the operation.
				gate = 1;
				{
					EditorAPI::TESVersionControl vc;
					if (vc.OpenFile(file))
					{
						batch = 1;
						target = file;

						auto& worlds = handler->arrWRLD;
						for (std::uint32_t i = 0; i < worlds.size(); i++)
						{
							auto world = worlds[i];
							if (!world || world->IsDeleted() || !HasSourceFile(world, file))
								continue;

							if (vc.UpdateWorldSpaceOffsets(world))
								updated++;
							else
								failed++;
						}

						batch = prev_batch;
						target = prev_target;
					}
					else
						_CONSOLE("Save WorldSpace Offsets: the CK could not open \"%s\" for the offset update", file->GetFileName().c_str());

					vc.Close();
				}
				gate = prev_gate;

				_CONSOLE("Save WorldSpace Offsets: %u worldspace(s) updated, %u failed in \"%s\"", updated, failed,
					file->GetFileName().c_str());
			}

			bool SaveWorldSpaceOffsets::hk_Save(void* handler, const char* file_name) noexcept(true)
			{
				// The save may be refused or skipped (it still reports success when it only launches another tool), so
				// the offsets are only rebuilt when the file on disk really changed during the call.
				WIN32_FILE_ATTRIBUTE_DATA before{}, after{};
				std::string path;

				auto data_handler = EditorAPI::TESDataHandler::Singleton.Singleton;
				auto active = data_handler ? data_handler->ActiveMod : nullptr;
				if (active)
				{
					path = (std::string)active->GetFilePath().c_str() + active->GetFileName().c_str();
					GetFileStamp(path, before);
				}

				auto result = pSave(handler, file_name);

				if (result && active && !path.empty())
				{
					if (GetFileStamp(path, after) && !SameStamp(before, after))
						UpdateOffsets();
					else
						_CONSOLE("Save WorldSpace Offsets: \"%s\" was not changed by the save, skipped", active->GetFileName().c_str());
				}

				return result;
			}
		}
	}
}
