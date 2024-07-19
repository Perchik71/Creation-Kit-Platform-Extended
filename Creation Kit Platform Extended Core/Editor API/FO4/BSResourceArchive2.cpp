// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "BSResourceArchive2.h"
#include "NiAPI\NiMemoryManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::Core;

			namespace BSResource
			{
				uintptr_t pointer_Archive2_sub1 = 0;
				uintptr_t pointer_Archive2_sub2 = 0;
				Array<BSString*> g_arrayArchivesAvailable;
				bool g_initCKPEPrimary = false;
				std::mutex g_CKPEPrimary;

				void Archive2::Initialize()
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

				void Archive2::GetFileSizeStr(uint64_t fileSize, BSString& fileSizeStr)
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

				Archive2::EResultError Archive2::HKLoadArchive(void* arrayDataList, LooseFileStream*& resFile,
					void* Unk1, uint32_t Unk2)
				{
					auto fileName = resFile->FileName->Get<CHAR>(true);
					AssertMsg(fileName, "There is no name of the load archive");

					BSString filePath, fileSizeStr;
					filePath.Format("%s%s%s", resFile->AppPath->Get<CHAR>(true), resFile->DataPath->Get<CHAR>(true), fileName);
					AssertMsgVa(BSString::Utils::FileExists(filePath), "Can't found file %s", *filePath);

					uint64_t fileSize = 0;
					WIN32_FILE_ATTRIBUTE_DATA fileData;
					if (GetFileAttributesExA(*filePath, GetFileExInfoStandard, &fileData))
						fileSize = (uint64_t)fileData.nFileSizeLow | ((uint64_t)fileData.nFileSizeHigh << 32);

					auto resultNo = EC_NONE;

					if (_stricmp(fileName, "Fallout4 - Shaders.ba2"))  // skip load Fallout4 - Shaders.ba2
					{
						GetFileSizeStr(fileSize, fileSizeStr);
						_CONSOLE("Load an archive file \"%s\" (%s)...", fileName, *fileSizeStr);

						resultNo = fastCall<EResultError, void*, LooseFileStream*&, void*, uint32_t>
							(pointer_Archive2_sub1, arrayDataList, resFile, Unk1, Unk2);
						AssertMsgVa(resultNo == EC_NONE, "Failed load an archive file %s", fileName);
					}

					LoadPrimaryArchive();

					return resultNo;
				}

				Archive2::EResultError Archive2::HKLoadArchiveEx(void* arrayDataList, InfoEx* infoRes,
					void* Unk1, uint32_t Unk2)
				{
					auto fileName2 = infoRes->fileName->Get<CHAR>(true);
					AssertMsg(fileName2, "There is no name of the load archive");

					BSString filePath, fileSizeStr;
					filePath.Format("%s%s%s", BSString::Utils::GetApplicationPath().c_str(), "Data\\", fileName2);
					AssertMsgVa(BSString::Utils::FileExists(filePath), "Can't found file %s", *filePath);

					uint64_t fileSize = 0;
					WIN32_FILE_ATTRIBUTE_DATA fileData;
					if (GetFileAttributesExA(*filePath, GetFileExInfoStandard, &fileData))
						fileSize = (uint64_t)fileData.nFileSizeLow | ((uint64_t)fileData.nFileSizeHigh << 32);

					auto resultNo = EC_NONE;

					if (_stricmp(fileName2, "Fallout4 - Shaders.ba2"))  // skip load Fallout4 - Shaders.ba2
					{
						GetFileSizeStr(fileSize, fileSizeStr);
						_CONSOLE("Load an archive file \"%s\" (%s)...", fileName2, *fileSizeStr);

						resultNo = fastCall<EResultError, void*, InfoEx*, void*, uint32_t>
							(pointer_Archive2_sub1, arrayDataList, infoRes, Unk1, Unk2);
						AssertMsgVa(resultNo == EC_NONE, "Failed load an archive file %s", fileName2);
					}

					LoadPrimaryArchive();

					return resultNo;
				}

				void Archive2::LoadArchive(const char* fileName)
				{
					if (BSString::Utils::FileExists(BSString::Utils::GetDataPath() + fileName))
						fastCall<void>(pointer_Archive2_sub2, fileName, 0, 0);
				}

				bool Archive2::IsAvailableForLoad(const char* fileName)
				{
					for (auto it = g_arrayArchivesAvailable.begin();
						 it != g_arrayArchivesAvailable.end(); it++)
					{
						if (!(*it)->Compare(fileName))
							return true;
					}

					return false;
				}

				void Archive2::LoadPrimaryArchive()
				{
					{
						std::lock_guard lock(g_CKPEPrimary);

						if (g_initCKPEPrimary)
							return;

						g_initCKPEPrimary = true;
					}

					LoadArchive("CreationKit - Shaders.ba2");
					LoadArchive("CreationKit - Textures.ba2");
				}
			}
		}
	}
}