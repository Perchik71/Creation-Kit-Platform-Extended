// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.FileUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.SettingCollection.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/TESFile.h>
#include <Patches/CKPE.SkyrimSE.Patch.BSArchiveManager.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::vector<const EditorAPI::TESFile*> g_SelectedFilesArray;
			std::vector<EditorAPI::BSString*> g_arrayArchivesAvailable;
			std::uintptr_t pointer_BSArchiveManagerModded_sub = 0;
			bool loaded_BSArchiveManagerModded_active = false;

			class BSResourceArchive
			{
			public:
				typedef void* (*eventLoadArchive_t)(void*, const char*, void*, int);
				inline static eventLoadArchive_t OldLooseLoadArchive;
				inline static std::uintptr_t OldLoadArchive;

				static void GetFileSizeStr(unsigned int fileSize, EditorAPI::BSString& fileSizeStr)
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
					CKPE_ASSERT_MSG(file_name, "There is no name of the load archive");

					EditorAPI::BSString filePath = EditorAPI::BSString::Utils::GetDataPath() + file_name, fileSizeStr;
					auto fileSize = FileUtils::GetFileSize(*filePath);
					if (fileSize > 0)
					{
						GetFileSizeStr(fileSize, fileSizeStr);
						_CONSOLE("Load an archive file \"%s\" (%s)...", file_name, *fileSizeStr);
					}

					return OldLooseLoadArchive(Loose, file_name, Unk1, Unk2);
				}

				static void LoadArchive(const char* file_name)
				{
					if (EditorAPI::BSString::Utils::FileExists(EditorAPI::BSString::Utils::GetDataPath() + file_name))
						((void(__fastcall*)(const char*, int, int))OldLoadArchive)(file_name, 0, 0);
				}

				static void Initialize()
				{
					auto pathData = EditorAPI::BSString::Utils::GetDataPath();

					WIN32_FIND_DATA	FileFindData{};
					HANDLE hFindFile = FindFirstFileExA(*(pathData + "*.bsa"), FindExInfoStandard, &FileFindData,
						FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
					if (hFindFile != INVALID_HANDLE_VALUE) 
					{
						do
						{
							g_arrayArchivesAvailable.push_back(new EditorAPI::BSString(FileFindData.cFileName));
						} while (FindNextFile(hFindFile, &FileFindData));
					}

					auto func = [](const std::string& svalue) 
					{
						EditorAPI::BSString strName;

						if (svalue.length() > 0) 
						{
							LPSTR s_c = new CHAR[svalue.length() + 1];
							strcpy(s_c, svalue.c_str());

							LPSTR stoken = strtok(s_c, ",");
							if (stoken)
							{
								do 
								{
									auto index = g_arrayArchivesAvailable.begin();
									auto fname = StringUtils::Trim(stoken);

									for (; index != g_arrayArchivesAvailable.end(); index++)
									{
										if (!_stricmp(fname.c_str(), (*index)->c_str()))
											break;
									}

									if (index != g_arrayArchivesAvailable.end())
									{
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

					Common::INISettingCollection _conf("CreationKit.ini");
					Common::INISettingCollection _User_conf("CreationKitCustom.ini");

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
					auto fname = StringUtils::Trim(ArchiveName);

					for (; index != g_arrayArchivesAvailable.end(); index++)
					{
						if (!_stricmp(fname.c_str(), (*index)->c_str()))
							break;
					}

					return index != g_arrayArchivesAvailable.end();
				}
			};

			BSArchiveManager::BSArchiveManager() : Common::Patch()
			{
				SetName("BSArchiveManager Modded");
			}

			bool BSArchiveManager::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* BSArchiveManager::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bOwnArchiveLoader";
			}

			bool BSArchiveManager::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BSArchiveManager::GetDependencies() const noexcept(true)
			{
				return { "Fix Load Archive Active Plugin" };
			}

			bool BSArchiveManager::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool BSArchiveManager::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				BSResourceArchive::OldLoadArchive = __CKPE_OFFSET(0);
				*(std::uintptr_t*)&BSResourceArchive::OldLooseLoadArchive =
					Detours::DetourClassJump(__CKPE_OFFSET(1), &BSResourceArchive::HKLooseLoadArchive);

				if (verPatch == 1)
				{
					// Data File Loaded skips
					for (std::uint32_t i = 5; i < db->GetCount() - 1; i++)
						SafeWrite::Write(__CKPE_OFFSET(i), { 0x31, 0xC0, 0x90, 0x90, 0x90 });

					SafeWrite::WriteNop(__CKPE_OFFSET(db->GetCount() - 1), 0xC);
				}
				else if (verPatch == 2)
				{
					// Data File Loaded skips
					for (std::uint32_t i = 5; i < db->GetCount(); i++)
						SafeWrite::Write(__CKPE_OFFSET(i), { 0x31, 0xC0, 0x90, 0x90, 0x90 });
				}

				BSResourceArchive::Initialize();

				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&LoadTesFile);
				Detours::DetourJump(__CKPE_OFFSET(3), (std::uintptr_t)&LoadTesFileFinal);

				pointer_BSArchiveManagerModded_sub = __CKPE_OFFSET(4);

				return true;
			}

			void BSArchiveManager::AttachBSAFile(const char* _filename) noexcept(true)
			{
				if (BSResourceArchive::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				BSResourceArchive::LoadArchive(_filename);
			}

			void BSArchiveManager::LoadTesFile(const void* load_file) noexcept(true)
			{
				bool loaded = true;
				auto lf = (EditorAPI::TESFile*)load_file;

				// Sometimes duplicated
				if (std::find(g_SelectedFilesArray.begin(), g_SelectedFilesArray.end(), load_file) ==
					g_SelectedFilesArray.end())
				{
					if (lf->IsActive())
					{
						_CONSOLE("Load active file %s...", lf->GetFileName().c_str());
						if (!loaded_BSArchiveManagerModded_active) loaded = false;
						loaded_BSArchiveManagerModded_active = !loaded_BSArchiveManagerModded_active;
					}
					else if (lf->IsMaster() || lf->IsSmallMaster())
						_CONSOLE("Load master file %s...", lf->GetFileName().c_str());
					else
						_CONSOLE("Load file %s...", lf->GetFileName().c_str());

					g_SelectedFilesArray.push_back(lf);
				}

				if (loaded)
				{
					auto sname = lf->GetFileName();
					sname.Copy(0, sname.FindLastOf('.'));

					AttachBSAFile(*(sname + ".bsa"));
					AttachBSAFile(*(sname + " - Textures.bsa"));
				}

				((void(__fastcall*)(const EditorAPI::TESFile*))pointer_BSArchiveManagerModded_sub)(lf);
			}

			void BSArchiveManager::LoadTesFileFinal() noexcept(true)
			{
				g_SelectedFilesArray.clear();
			}
		}
	}
}