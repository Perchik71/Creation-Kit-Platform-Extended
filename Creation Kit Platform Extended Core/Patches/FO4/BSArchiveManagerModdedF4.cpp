// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/BSString.h"
#include "Editor API/EditorUI.h"
#include "Editor API/FO4/BSResourceLooseFilesF4.h"
#include "BSArchiveManagerModdedF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::EditorAPI;

			Array<const TESFile*> g_SelectedFilesArray;
			Array<BSString*> g_arrayArchivesAvailable;
			BSResourceLooseFile* g_lastLoaded = nullptr;
			bool IsLoaded;

			uintptr_t pointer_BSArchiveManagerModded_sub = 0;

			class BSResourceArchive
			{
			public:
				typedef void* (*eventLoadArchive_t)(void*, BSResourceLooseFile*&, void*, int);
				inline static eventLoadArchive_t OldLooseLoadArchive;
				inline static uintptr_t OldLoadArchive;

				static void GetFileSizeStr(unsigned int fileSize, BSString& fileSizeStr)
				{
					if (fileSize >= 0x40000000)
						fileSizeStr.Format("%.3f GByte", ((long double)fileSize) / 0x40000000);
					else if (fileSize >= 0x100000)
						fileSizeStr.Format("%3.3f MByte", ((long double)fileSize) / 0x100000);
					else if (fileSize >= 0x400)
						fileSizeStr.Format("%3.3f KByte", ((long double)fileSize) / 0x400);
					else
						fileSizeStr.Format("%d Byte", fileSize);
				}

				static void* HKLooseLoadArchive(void* Loose, BSResourceLooseFile*& resFile, void* Unk1, int Unk2 = 1)
				{
					/* 
						Кит обращается сюда множество раз, из абсолютно разных мест, как я понимаю,
						он пытается найти необходимые файлы и шейдеры, но во всех архивах.
					*/

					if (g_lastLoaded == resFile)
						return OldLooseLoadArchive(Loose, resFile, Unk1, Unk2);

					/*
					*	Сюда мы попадаем впервые, для архива и лишь раз (гарантировано).
					*/

					g_lastLoaded = resFile;
					auto fileName = resFile->FileName->Get<CHAR>(TRUE);
					AssertMsg(fileName, "There is no name of the load archive");

					BSString filePath, fileSizeStr;
					filePath.Format("%s%s%s", resFile->AppPath->Get<CHAR>(TRUE), resFile->DataPath->Get<CHAR>(TRUE), fileName);

					if (BSString::Utils::FileExists(filePath))
					{
						unsigned int fileSize = 0;
						WIN32_FILE_ATTRIBUTE_DATA fileData;
						if (GetFileAttributesExA(*filePath, GetFileExInfoStandard, &fileData))
							fileSize = (uint64_t)fileData.nFileSizeLow | ((uint64_t)fileData.nFileSizeHigh << 32);

						GetFileSizeStr(fileSize, fileSizeStr);
						_CONSOLE("Load an archive file \"%s\" (%s)...", fileName, *fileSizeStr);
					}

					return OldLooseLoadArchive(Loose, resFile, Unk1, Unk2);
				}

				static void LoadArchive(const char* file_name)
				{
					if (BSString::Utils::FileExists(BSString::Utils::GetDataPath() + file_name))
						((void(__fastcall*)(const char*, int, int))OldLoadArchive)(file_name, 0, 0);
				}

				static void Initialize()
				{
					auto pathData = BSString::Utils::GetDataPath();

					WIN32_FIND_DATA	FileFindData;
					HANDLE hFindFile = FindFirstFileExA(*(pathData + "*.ba2"), FindExInfoStandard, &FileFindData,
						FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
					if (hFindFile != INVALID_HANDLE_VALUE) 
					{
						do 
						{
							g_arrayArchivesAvailable.push_back(new BSString(FileFindData.cFileName));
						} while (FindNextFile(hFindFile, &FileFindData));
					}

					auto func = [](const String& svalue) {
						BSString strName;

						if (svalue.length() > 0) {
							LPSTR s_c = new CHAR[svalue.length() + 1];
							strcpy(s_c, svalue.c_str());

							LPSTR stoken = strtok(s_c, ",");
							if (stoken) {
								do {
									auto index = g_arrayArchivesAvailable.begin();
									auto fname = Utils::Trim(stoken);

									for (; index != g_arrayArchivesAvailable.end(); index++)
									{
										if (!_stricmp(fname.c_str(), (*index)->c_str()))
											break;
									}

									if (index != g_arrayArchivesAvailable.end()) {
										delete* index;
										g_arrayArchivesAvailable.erase(index);
									}

									stoken = strtok(NULL, ",");
								} while (stoken);
							}

							delete[] s_c;
						}
					};

					static const char* SC_NONE = "<NONE>";

					INIConfig _conf("CreationKit.ini");
					INIConfig _User_conf("CreationKitCustom.ini");

					auto s = _User_conf.ReadString("Archive", "sResourceArchiveList", SC_NONE);
					func((s == SC_NONE) ? _conf.ReadString("Archive", "sResourceArchiveList", "") : s);
					s = _User_conf.ReadString("Archive", "sResourceArchiveList2", SC_NONE);
					func((s == SC_NONE) ? _conf.ReadString("Archive", "sResourceArchiveList2", "") : s);
					s = _User_conf.ReadString("Archive", "sResourceArchiveMemoryCacheList", SC_NONE);
					func((s == SC_NONE) ? _conf.ReadString("Archive", "sResourceArchiveMemoryCacheList", "") : s);
					s = _User_conf.ReadString("Archive", "sResourceStartUpArchiveList", SC_NONE);
					func((s == SC_NONE) ? _conf.ReadString("Archive", "sResourceStartUpArchiveList", "") : s);
					s = _User_conf.ReadString("Archive", "sResourceIndexFileList", SC_NONE);
					func((s == SC_NONE) ? _conf.ReadString("Archive", "sResourceIndexFileList", "") : s);
				}

				static bool IsAvailableForLoad(LPCSTR ArchiveName)
				{
					auto index = g_arrayArchivesAvailable.begin();
					auto fname = Utils::Trim(ArchiveName);

					for (; index != g_arrayArchivesAvailable.end(); index++)
					{
						if (!_stricmp(fname.c_str(), (*index)->c_str()))
							break;
					}

					return index != g_arrayArchivesAvailable.end();
				}
			};

			BSArchiveManagerModdedPatch::BSArchiveManagerModdedPatch() : Module(GlobalEnginePtr)
			{}

			bool BSArchiveManagerModdedPatch::HasOption() const
			{
				return true;
			}

			bool BSArchiveManagerModdedPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BSArchiveManagerModdedPatch::GetOptionName() const
			{
				return "CreationKit:bOwnArchiveLoader";
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
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool BSArchiveManagerModdedPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// BSArchiveManager
					//

					BSResourceArchive::OldLoadArchive = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));			
					*(uintptr_t*)&BSResourceArchive::OldLooseLoadArchive =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1)),
							&BSResourceArchive::HKLooseLoadArchive);

					BSResourceArchive::Initialize();

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&LoadTesFile);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3), (uintptr_t)&LoadTesFileFinal);

					pointer_BSArchiveManagerModded_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));

					// Пропуск загрузки архивов, что не имеют отношения к загружаемому моду, но является предыдущей работой
					lpRelocator->Patch(lpRelocationDatabaseItem->At(5), { 0xC3 });

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
				if (BSResourceArchive::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				BSResourceArchive::LoadArchive(_filename);
			}

			void BSArchiveManagerModdedPatch::LoadTesFile(const TESFile* load_file)
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
				AttachBA2File(*(sname + " - Textures.ba2"));

				((void(__fastcall*)(const TESFile*))pointer_BSArchiveManagerModded_sub)(load_file);
			}

			void BSArchiveManagerModdedPatch::LoadTesFileFinal(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam)
			{
				EditorUI::HKSendMessageA(hWnd, uMsg, lParam, wParam);
				g_SelectedFilesArray.clear();
				IsLoaded = true;
			}
		}
	}
}