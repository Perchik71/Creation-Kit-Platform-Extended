// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/TESFile.h>
#include <EditorAPI/BSResourceArchive2.h>
#include <Patches/CKPE.Fallout4.Patch.BSArchiveManagerModded.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				extern bool NoTextureLoad;
				extern std::uintptr_t pointer_Archive2_sub1;
				extern std::uintptr_t pointer_Archive2_sub2;
				extern std::uintptr_t pointer_Archive2_sub3;
			}
		}

		namespace Patch
		{
			std::vector<const EditorAPI::TESFile*> g_SelectedFilesArray;
			std::uintptr_t pointer_BSArchiveManagerModded_sub = 0;
			bool IsLoaded;
			std::uint8_t supportedBA2Version = 8;

			//static constexpr char szArchiveList[] = "Fallout4 - Voices1.ba2, Fallout4 - Voices2.ba2, Fallout4 - Meshes.ba2, Fallout4 - Animations.ba2, Fallout4 - Interface.ba2, Fallout4 - Misc.ba2, Fallout4 - Sounds.ba2";
			//static constexpr char szArchiveList2[] = "Fallout4 - Voices.ba2, Fallout4 - Interface.ba2, Fallout4 - Meshes.ba2, Fallout4 - MeshesExtra.ba2, Fallout4 - Misc.ba2, Fallout4 - Sounds.ba2, Fallout4 - Materials.ba2";

			static void AttachBA2File(const char* _filename) noexcept(true)
			{
				if (EditorAPI::BSResource::Archive2::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				EditorAPI::BSResource::Archive2::LoadArchive(_filename);
			}

			static void LoadTesFile(const EditorAPI::TESFile* load_file) noexcept(true)
			{
				IsLoaded = false;
				// Sometimes duplicated
				if (std::find(g_SelectedFilesArray.begin(), g_SelectedFilesArray.end(), load_file) ==
					g_SelectedFilesArray.end())
				{
					if (load_file->IsActive())
						_CONSOLE("Load active file %s...", load_file->GetFileName().c_str());
					else if (load_file->IsMaster() || load_file->IsSmallMaster())
						_CONSOLE("Load master file %s...", load_file->GetFileName().c_str());
					else
						_CONSOLE("Load file %s...", load_file->GetFileName().c_str());

					g_SelectedFilesArray.push_back(load_file);
				}

				auto sname = load_file->GetFileName();
				sname.Copy(0, sname.FindLastOf('.'));

				AttachBA2File(*(sname + " - Main.ba2"));
				if (!EditorAPI::BSResource::NoTextureLoad) AttachBA2File(*(sname + " - Textures.ba2"));

				fast_call<void>(pointer_BSArchiveManagerModded_sub, load_file);
			}

			static void LoadTesFileFinal(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam) noexcept(true)
			{
				Common::EditorUI::Hook::HKSendMessageA(hWnd, uMsg, lParam, wParam);
				g_SelectedFilesArray.clear();
				IsLoaded = true;
			}

			static bool HasLoaded() noexcept(true)
			{
				return IsLoaded;
			}

			BSArchiveManagerModded::BSArchiveManagerModded() : Common::Patch()
			{
				SetName("BSArchiveManager Modded");
			}

			bool BSArchiveManagerModded::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BSArchiveManagerModded::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BSArchiveManagerModded::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BSArchiveManagerModded::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BSArchiveManagerModded::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool BSArchiveManagerModded::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				EditorAPI::BSResource::pointer_Archive2_sub1 = __CKPE_OFFSET(6);
				EditorAPI::BSResource::pointer_Archive2_sub2 = __CKPE_OFFSET(0);
				EditorAPI::BSResource::pointer_Archive2_sub3 =
					Detours::DetourClassJump(__CKPE_OFFSET(10), EditorAPI::BSResource::Archive2::HKLoadArchive);
				EditorAPI::BSResource::Archive2::Initialize();

				if (verPatch == 1)
					// Первая версия патча для 1.10.162.0
					Detours::DetourCall(__CKPE_OFFSET(1),
						(std::uintptr_t)&EditorAPI::BSResource::Archive2::HKLoadStreamArchive);
				else
					Detours::DetourCall(__CKPE_OFFSET(1),
						(std::uintptr_t)&EditorAPI::BSResource::Archive2::HKLoadStreamArchiveEx);

				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&LoadTesFile);
				Detours::DetourJump(__CKPE_OFFSET(3), (std::uintptr_t)&LoadTesFileFinal);

				pointer_BSArchiveManagerModded_sub = __CKPE_OFFSET(4);

				// Пропуск загрузки архивов, что не имеют отношения к загружаемому моду, но является предыдущей работой
				SafeWrite::Write(__CKPE_OFFSET(5), { 0xC3 });

				// Исключение из списков архив шейдеров
				//SafeWrite::WriteStringRef(__CKPE_OFFSET(8), szArchiveList);
				//SafeWrite::WriteStringRef(__CKPE_OFFSET(9), szArchiveList2);

				// Так как разница между первой и 8 версией лишь, то что был удалён GNF формат для PlayStation.
				// То очевидно, 8 версии с GNF форматом просто не будет, то вполне безопасно, открывать любые версии архивы.
				if (verPatch == 1)
					// Первая версия патча для 1.10.162.0
					SafeWrite::Write(__CKPE_OFFSET(7), &supportedBA2Version, 1);
	
				if (verPatch == 2)
				{
					// Удаление повторной загрузки архивов
					SafeWrite::Write(__CKPE_OFFSET(11), { 0xEB });
					// Удаление загрузки бреда, который не является активным файлом
					SafeWrite::Write(__CKPE_OFFSET(12), { 0xEB });
				}

				return true;
			}
		}
	}
}