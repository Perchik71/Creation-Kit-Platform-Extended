// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.FileUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/TESFile.h>
#include <Patches/CKPE.Starfield.Patch.BSArchiveManager.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			std::vector<const EditorAPI::TESFile*> g_SelectedFilesArray;
			std::vector<EditorAPI::BSString*> g_arrayArchivesAvailable;
			std::uintptr_t pointer_BSArchiveManagerModded_sub = 0;
			bool loaded_BSArchiveManagerModded_active = false;

			class BSResourceArchive2
			{
			public:
				inline static std::uintptr_t OldLoadArchive;

				typedef std::uint32_t (*eventLoadArchive_t)(const char*, std::uint64_t, std::uint64_t, std::uint64_t,
					std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t);
				inline static eventLoadArchive_t OldLooseLoadArchive;

				static void GetFileSizeStr(std::uint64_t fileSize, EditorAPI::BSString& fileSizeStr) noexcept(true)
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

				static std::uint32_t HKLoadArchive(const char* fileName, std::uint64_t unknown01,
					std::uint64_t unknown02, std::uint64_t unknown03, std::uint64_t unknown04, std::uint64_t unknown05,
					std::uint64_t unknown06, std::uint64_t unknown07) noexcept(true)
				{
					if (!fileName)
						return 0;

					CKPE_ASSERT_MSG(fileName, "There is no name of the load archive");

					EditorAPI::BSString filePath = EditorAPI::BSString::Utils::GetDataPath() + fileName, fileSizeStr;
					auto fileSize = FileUtils::GetFileSize(*filePath);
					if (fileSize > 0)
					{
						GetFileSizeStr(fileSize, fileSizeStr);
						_CONSOLE("Load an archive file \"%s\" (%s)...", fileName, *fileSizeStr);
					}

					return OldLooseLoadArchive(fileName, unknown01, unknown02, unknown03, unknown04, unknown05, 
						unknown06, unknown07);
				}

				static void LoadArchive(const char* file_name) noexcept(true)
				{
					if (EditorAPI::BSString::Utils::FileExists(EditorAPI::BSString::Utils::GetDataPath() + file_name))
						((void(__fastcall*)(const char*, std::int32_t, std::int32_t))OldLoadArchive)(file_name, 0, 0);
				}

				static void Initialize() noexcept(true)
				{
					auto pathData = EditorAPI::BSString::Utils::GetDataPath();

					WIN32_FIND_DATA	FileFindData{};
					HANDLE hFindFile = FindFirstFileExA(*(pathData + "*.ba2"), FindExInfoStandard, &FileFindData,
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

					//ctd ??? (random times)
#if 0
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
#else
					auto buff = std::make_unique<char[]>(2048);
					auto buff_def = std::make_unique<char[]>(2048);

					ZeroMemory(buff.get(), 2048);
					ZeroMemory(buff_def.get(), 2048);

					GetPrivateProfileStringA("Archive", "sResourceArchiveList", SC_NONE,
						buff.get(), 2048, "CreationKitCustom.ini");
					GetPrivateProfileStringA("Archive", "sResourceArchiveList", "",
						buff_def.get(), 2048, "CreationKit.ini");

					func((!strcmp(buff.get(), SC_NONE)) ? buff_def.get() : buff.get());

					ZeroMemory(buff.get(), 2048);
					ZeroMemory(buff_def.get(), 2048);

					GetPrivateProfileStringA("Archive", "sResourceArchiveList2", SC_NONE,
						buff.get(), 2048, "CreationKitCustom.ini");
					GetPrivateProfileStringA("Archive", "sResourceArchiveList2", "",
						buff_def.get(), 2048, "CreationKit.ini");

					func((!strcmp(buff.get(), SC_NONE)) ? buff_def.get() : buff.get());

					ZeroMemory(buff.get(), 2048);
					ZeroMemory(buff_def.get(), 2048);

					GetPrivateProfileStringA("Archive", "sResourceArchiveMemoryCacheList", SC_NONE,
						buff.get(), 2048, "CreationKitCustom.ini");
					GetPrivateProfileStringA("Archive", "sResourceArchiveMemoryCacheList", "",
						buff_def.get(), 2048, "CreationKit.ini");

					func((!strcmp(buff.get(), SC_NONE)) ? buff_def.get() : buff.get());

					ZeroMemory(buff.get(), 2048);
					ZeroMemory(buff_def.get(), 2048);

					GetPrivateProfileStringA("Archive", "sResourceStartUpArchiveList", SC_NONE,
						buff.get(), 2048, "CreationKitCustom.ini");
					GetPrivateProfileStringA("Archive", "sResourceStartUpArchiveList", "",
						buff_def.get(), 2048, "CreationKit.ini");

					func((!strcmp(buff.get(), SC_NONE)) ? buff_def.get() : buff.get());

					ZeroMemory(buff.get(), 2048);
					ZeroMemory(buff_def.get(), 2048);

					GetPrivateProfileStringA("Archive", "sResourceIndexFileList", SC_NONE,
						buff.get(), 2048, "CreationKitCustom.ini");
					GetPrivateProfileStringA("Archive", "sResourceIndexFileList", "",
						buff_def.get(), 2048, "CreationKit.ini");

					func((!strcmp(buff.get(), SC_NONE)) ? buff_def.get() : buff.get());
#endif
				}

				static bool IsAvailableForLoad(const char* ArchiveName) noexcept(true)
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

			static bool HasLoaded() noexcept(true)
			{
				return loaded_BSArchiveManagerModded_active;
			}

			static void AttachBA2File(const char* _filename) noexcept(true)
			{
				if (BSResourceArchive2::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				BSResourceArchive2::LoadArchive(_filename);
			}

			static void LoadTesFile(const EditorAPI::TESFile* load_file, std::uint8_t unknown) noexcept(true)
			{
				loaded_BSArchiveManagerModded_active = false;
				// Sometimes duplicated
				if (std::find(g_SelectedFilesArray.begin(), g_SelectedFilesArray.end(), load_file) ==
					g_SelectedFilesArray.end())
				{
					if (load_file->IsActive())
						_CONSOLE("Load active file %s...", load_file->FileName);
					else if (load_file->IsMaster() || load_file->IsSmallMaster())
						_CONSOLE("Load master file %s...", load_file->FileName);
					else
						_CONSOLE("Load file %s...", load_file->FileName);

					g_SelectedFilesArray.push_back(load_file);
				}

				EditorAPI::BSString sname = load_file->FileName;
				sname.Copy(0, sname.FindLastOf('.'));

				AttachBA2File(*(sname + " - Main.ba2"));
				AttachBA2File(*(sname + " - Textures.ba2"));

				fast_call<void>(pointer_BSArchiveManagerModded_sub, load_file, unknown);
			}

			static void LoadTesFileFinal() noexcept(true)
			{
				g_SelectedFilesArray.clear();
				loaded_BSArchiveManagerModded_active = true;
			}


			BSArchiveManager::BSArchiveManager() : Common::Patch()
			{
				SetName("BSArchiveManager Modded");
			}

			bool BSArchiveManager::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BSArchiveManager::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BSArchiveManager::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> BSArchiveManager::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool BSArchiveManager::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_74_0;
			}

			bool BSArchiveManager::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				BSResourceArchive2::OldLoadArchive = __CKPE_OFFSET(0);
				BSResourceArchive2::Initialize();

				// Удаление пролога (ловушка занимает 5 байт и повредит прыжок)
				SafeWrite::WriteNop(__CKPE_OFFSET(1), 9);
				// Ловушка на загрузку архивов
				*(std::uintptr_t*)&BSResourceArchive2::OldLooseLoadArchive =
					Detours::DetourClassJump(__CKPE_OFFSET(1), &BSResourceArchive2::HKLoadArchive);

				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&LoadTesFile);
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&LoadTesFileFinal);

				pointer_BSArchiveManagerModded_sub = __CKPE_OFFSET(4);

				// Пропуск загрузки архивов, что не имеют отношения к загружаемому моду, но является предыдущей работой
				// Так же используется для загрузки архивов самим СК, так что душим это.
				SafeWrite::Write(__CKPE_OFFSET(5), { 0xC3 });
				// Вырезать EditorDataFilesLoaded.txt
				if (verPatch == 1)
					SafeWrite::Write(__CKPE_OFFSET(6), { 0xEB });
				else if (verPatch == 2)
					SafeWrite::Write(__CKPE_OFFSET(6), { 0x90, 0xE9 });

				return true;
			}
		}
	}
}