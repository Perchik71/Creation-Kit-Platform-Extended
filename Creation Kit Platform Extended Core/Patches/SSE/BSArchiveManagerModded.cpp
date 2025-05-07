// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/BSString.h"
#include "BSArchiveManagerModded.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::EditorAPI;

			Array<const TESFile*> g_SelectedFilesArray;
			Array<BSString*> g_arrayArchivesAvailable;

			uintptr_t pointer_BSArchiveManagerModded_sub = 0;

			class BSResourceArchive
			{
			public:
				typedef void* (*eventLoadArchive_t)(void*, const char*, void*, int);
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

				static void* HKLooseLoadArchive(void* Loose, const char* file_name, void* Unk1, int Unk2 = 1)
				{
					AssertMsg(file_name, "There is no name of the load archive");

					BSString filePath = BSString::Utils::GetDataPath() + file_name, fileSizeStr;
					if (BSString::Utils::FileExists(filePath))
					{
						unsigned int fileSize = 0;
						WIN32_FILE_ATTRIBUTE_DATA fileData;
						if (GetFileAttributesExA(*filePath, GetFileExInfoStandard, &fileData))
							fileSize = (uint64_t)fileData.nFileSizeLow | ((uint64_t)fileData.nFileSizeHigh << 32);

						GetFileSizeStr(fileSize, fileSizeStr);
						_CONSOLE("Load an archive file \"%s\" (%s)...", file_name, *fileSizeStr);
					}

					return OldLooseLoadArchive(Loose, file_name, Unk1, Unk2);
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
					HANDLE hFindFile = FindFirstFileExA(*(pathData + "*.bsa"), FindExInfoStandard, &FileFindData,
						FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
					if (hFindFile != INVALID_HANDLE_VALUE) {
						do {
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

					INISettingCollection _conf("CreationKit.ini");
					INISettingCollection _User_conf("CreationKitCustom.ini");

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
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool BSArchiveManagerModdedPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// BSArchiveManager
					//

					BSResourceArchive::OldLoadArchive = _RELDATA_ADDR(0);
					*(uintptr_t*)&BSResourceArchive::OldLooseLoadArchive =
						voltek::detours_function_class_jump(_RELDATA_ADDR(1), &BSResourceArchive::HKLooseLoadArchive);

					BSResourceArchive::Initialize();

					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&LoadTesFile);
					lpRelocator->DetourJump(_RELDATA_RAV(3), (uintptr_t)&LoadTesFileFinal);

					pointer_BSArchiveManagerModded_sub = _RELDATA_ADDR(4);

					return true;
				}

				return false;
			}

			bool BSArchiveManagerModdedPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void BSArchiveManagerModdedPatch::AttachBSAFile(LPCSTR _filename)
			{
				if (BSResourceArchive::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				BSResourceArchive::LoadArchive(_filename);
			}

			void BSArchiveManagerModdedPatch::LoadTesFile(const TESFile* load_file)
			{
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

				AttachBSAFile(*(sname + ".bsa"));
				AttachBSAFile(*(sname + " - Textures.bsa"));

				((void(__fastcall*)(const TESFile*))pointer_BSArchiveManagerModded_sub)(load_file);
			}

			void BSArchiveManagerModdedPatch::LoadTesFileFinal()
			{
				g_SelectedFilesArray.clear();
			}
		}
	}
}