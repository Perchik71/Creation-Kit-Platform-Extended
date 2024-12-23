// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "BSResourceArchive2SF.h"
#include "NiAPI\NiMemoryManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
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
					ZeroMemory(&FileFindData, sizeof(WIN32_FIND_DATA));
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

				uint32_t Archive2::HKLoadArchive(const char* fileName, __int64 unknown01,
					__int64 unknown02, __int64 unknown03, __int64 unknown04, __int64 unknown05,
					__int64 unknown06, __int64 unknown07)
				{
					if (!fileName)
						return 0;

					AssertMsg(fileName, "There is no name of the load archive");

					BSString filePath = BSString::Utils::GetDataPath() + fileName, fileSizeStr;
					if (BSString::Utils::FileExists(filePath))
					{
						unsigned int fileSize = 0;
						WIN32_FILE_ATTRIBUTE_DATA fileData;
						if (GetFileAttributesExA(*filePath, GetFileExInfoStandard, &fileData))
							fileSize = (uint64_t)fileData.nFileSizeLow | ((uint64_t)fileData.nFileSizeHigh << 32);

						GetFileSizeStr(fileSize, fileSizeStr);
						_CONSOLE("Load an archive file \"%s\" (%s)...", fileName, *fileSizeStr);
					}

					return fastCall<uint32_t>(pointer_Archive2_sub1, fileName, unknown01, 
						unknown02, unknown03, unknown04, unknown05, unknown06, unknown07);
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
			}
		}
	}
}