// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/SF/TESFileSF.h"
#include "Editor API/SF/BSResourceArchive2SF.h"
#include "BSArchiveManagerModdedSF.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			namespace BSResource
			{
				extern uintptr_t pointer_Archive2_sub1;
				extern uintptr_t pointer_Archive2_sub2;
			}
		}
	}

	namespace Patches
	{
		namespace Starfield
		{
			bool IsLoaded;
			Array<const TESFile*> g_SelectedFilesArray;
			uintptr_t pointer_BSArchiveManagerModded_sub = 0;

			BSArchiveManagerModdedPatch::BSArchiveManagerModdedPatch() : Module(GlobalEnginePtr)
			{}

			bool BSArchiveManagerModdedPatch::HasOption() const
			{
				return false;
			}

			bool BSArchiveManagerModdedPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BSArchiveManagerModdedPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* BSArchiveManagerModdedPatch::GetName() const
			{
				return "BSArchiveManager Modded";
			}

			bool BSArchiveManagerModdedPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> BSArchiveManagerModdedPatch::GetDependencies() const
			{
				return {};
			}

			bool BSArchiveManagerModdedPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_74_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool BSArchiveManagerModdedPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					EditorAPI::Starfield::BSResource::pointer_Archive2_sub2 = _RELDATA_ADDR(0);
					EditorAPI::Starfield::BSResource::Archive2::Initialize();

					// Удаление пролога (ловушка занимает 5 байт и повредит прыжок)
					lpRelocator->PatchNop(_RELDATA_RAV(1), 9);
					// Ловушка на загрузку архивов
					EditorAPI::Starfield::BSResource::pointer_Archive2_sub1 = 
						lpRelocator->DetourFunctionClass(_RELDATA_RAV(1),
						(uintptr_t)&EditorAPI::Starfield::BSResource::Archive2::HKLoadArchive);
					
					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&LoadTesFile);
					lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&LoadTesFileFinal);
					pointer_BSArchiveManagerModded_sub = _RELDATA_ADDR(4);

					// Пропуск загрузки архивов, что не имеют отношения к загружаемому моду, но является предыдущей работой
					// Так же используется для загрузки архивов самим СК, так что душим это.
					lpRelocator->Patch(_RELDATA_RAV(5), { 0xC3 });
					// Вырезать EditorDataFilesLoaded.txt
					lpRelocator->Patch(_RELDATA_RAV(6), { 0xEB });
	
					return true;
				}

				return false;
			}

			bool BSArchiveManagerModdedPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool BSArchiveManagerModdedPatch::HasLoaded()
			{
				return IsLoaded;
			}

			void BSArchiveManagerModdedPatch::AttachBA2File(LPCSTR _filename)
			{
				if (EditorAPI::Starfield::BSResource::Archive2::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				EditorAPI::Starfield::BSResource::Archive2::LoadArchive(_filename);
			}

			void BSArchiveManagerModdedPatch::LoadTesFile(const TESFile* load_file, __int8 unknown)
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

				// Find archives this plugin
				WIN32_FIND_DATA FindFileData;
				ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATA));
				auto hFind = FindFirstFileExA(*(EditorAPI::BSString::Utils::GetDataPath() + "*.ba2"),
					FindExInfoStandard, &FindFileData, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					auto CheckLen = sname.Length();
					auto CheckStr = *sname;

					do
					{
						if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
							continue;

						if (!_strnicmp(FindFileData.cFileName, CheckStr, CheckLen))
						{
							if (!_strnicmp(FindFileData.cFileName + CheckLen, " - Main", 7))
								AttachBA2File(FindFileData.cFileName);
							else if (!_strnicmp(FindFileData.cFileName + CheckLen, " - Textures", 11))
								AttachBA2File(FindFileData.cFileName);
						}
					} while(FindNextFileA(hFind, &FindFileData));
				}

				fastCall<void>(pointer_BSArchiveManagerModded_sub, load_file, unknown);
			}

			void BSArchiveManagerModdedPatch::LoadTesFileFinal()
			{
				g_SelectedFilesArray.clear();
				IsLoaded = true;
			}
		}
	}
}