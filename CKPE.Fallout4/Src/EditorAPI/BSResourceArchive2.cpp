// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <shlwapi.h>

#include <CKPE.Utils.h>
#include <CKPE.Asserts.h>
#include <CKPE.FileUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.SettingCollection.h>
#include <EditorAPI/BSResourceArchive2.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>

#include <vector>
#include <mutex>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				bool NoTextureLoad = false;
				std::uintptr_t pointer_Archive2_sub1 = 0;
				std::uintptr_t pointer_Archive2_sub2 = 0;
				std::uintptr_t pointer_Archive2_sub3 = 0;
				std::vector<BSString*> g_arrayArchivesAvailable;
				bool g_initCKPEPrimary = false;
				std::mutex g_CKPEPrimary;

				void Archive2::Initialize() noexcept(true)
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

					auto func = [](const std::string& svalue) {
						BSString strName;

						if (svalue.length() > 0) {
							char* s_c = new char[svalue.length() + 1];
							strcpy(s_c, svalue.c_str());

							char* stoken = strtok(s_c, ",");
							if (stoken) {
								do {
									auto index = g_arrayArchivesAvailable.begin();
									auto fname = StringUtils::Trim(stoken);

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

				void Archive2::GetFileSizeStr(std::uint64_t fileSize, BSString& fileSizeStr) noexcept(true)
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

				EResultError Archive2::HKLoadStreamArchive(void* arrayDataList, LooseFileStream*& resFile,
					void* Unk1, std::uint32_t Unk2) noexcept(true)
				{
					auto fileName = resFile->FileName->c_str();
					CKPE_ASSERT_MSG(fileName, "There is no name of the load archive");

					BSString filePath, fileSizeStr;
					filePath.Format("%s%s%s", resFile->AppPath->c_str(), resFile->DataPath->c_str(), fileName);
					CKPE_ASSERT_MSG_FMT(BSString::Utils::FileExists(filePath), "Can't found file %s", *filePath);

					auto fileSize = FileUtils::GetFileSize(*filePath);
					auto resultNo = EResultError::kNone;

					GetFileSizeStr(fileSize, fileSizeStr);
					_CONSOLE("Load an archive file \"%s\" (%s)...", fileName, *fileSizeStr);

					resultNo = fast_call<EResultError, void*, LooseFileStream*&, void*, std::uint32_t>
						(pointer_Archive2_sub1, arrayDataList, resFile, Unk1, Unk2);
					CKPE_ASSERT_MSG_FMT(resultNo == EResultError::kNone, "Failed load an archive file %s", fileName);
		
					LoadPrimaryArchive();

					return resultNo;
				}

				EResultError Archive2::HKLoadStreamArchiveEx(void* arrayDataList, InfoEx* infoRes,
					void* Unk1, std::uint32_t Unk2) noexcept(true)
				{
					auto fileName = infoRes->fileName->c_str();
					CKPE_ASSERT_MSG(fileName, "There is no name of the load archive");

					BSString filePath, fileSizeStr;
					filePath.Format("%s%s%s", BSString::Utils::GetApplicationPath().c_str(), "Data\\", fileName);
					CKPE_ASSERT_MSG_FMT(BSString::Utils::FileExists(filePath), "Can't found file %s", *filePath);

					auto fileSize = FileUtils::GetFileSize(*filePath);
					auto resultNo = EResultError::kNone;

					GetFileSizeStr(fileSize, fileSizeStr);
					_CONSOLE("Load an archive file \"%s\" (%s)...", fileName, *fileSizeStr);

					resultNo = fast_call<EResultError, void*, InfoEx*, void*, std::uint32_t>
						(pointer_Archive2_sub1, arrayDataList, infoRes, Unk1, Unk2);
					CKPE_ASSERT_MSG_FMT(resultNo == EResultError::kNone, "Failed load an archive file %s", fileName);

					LoadPrimaryArchive();

					return resultNo;
				}

				void Archive2::HKLoadArchive(const char* fileName, void* Unk1, void* Unk2, std::uint32_t Unk3) noexcept(true)
				{
					if (NoTextureLoad && (StrStrIA(fileName, " - Textures") != nullptr))
						return;

					fast_call<void>(pointer_Archive2_sub3, fileName, Unk1, Unk2, Unk3);
				}

				void Archive2::LoadArchive(const char* fileName) noexcept(true)
				{
					if (BSString::Utils::FileExists(BSString::Utils::GetDataPath() + fileName))
						fast_call<void>(pointer_Archive2_sub2, fileName, 0, 0);
				}

				bool Archive2::IsAvailableForLoad(const char* fileName) noexcept(true)
				{
					for (auto it = g_arrayArchivesAvailable.begin();
						 it != g_arrayArchivesAvailable.end(); it++)
					{
						if (!(*it)->Compare(fileName))
							return true;
					}

					return false;
				}

				void Archive2::LoadPrimaryArchive() noexcept(true)
				{
					{
						std::lock_guard lock(g_CKPEPrimary);

						if (g_initCKPEPrimary)
							return;

						g_initCKPEPrimary = true;
					}

					if (!NoTextureLoad) LoadArchive("CreationKit - Textures.ba2");
				}

				Archive2::OG_NG::ReaderStream::~ReaderStream()
				{
					// ctd okay
					//CKPE::vtbl_call<void>(0x0, (void*)&RTTI);
				}

				EResultError Archive2::OG_NG::ReaderStream::DoOpen() noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x8, (void*)&RTTI);
				}

				void Archive2::OG_NG::ReaderStream::DoClose() noexcept(true)
				{
					CKPE::vtbl_call<void>(0x10, (void*)&RTTI);
				}

				std::uint64_t Archive2::OG_NG::ReaderStream::DoGetKey() const noexcept(true)
				{
					return CKPE::vtbl_call<std::uint64_t >(0x18, (void*)&RTTI);
				}

				EResultError Archive2::OG_NG::ReaderStream::DoGetInfo(void*) noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				void Archive2::OG_NG::ReaderStream::DoClone(void* p) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x28, (void*)&RTTI, p);
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoRead(void* Buffer, std::uint64_t Bytes, std::uint64_t& Read) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x30, (void*)&RTTI, Buffer, Bytes, Read);
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoReadAt(void*, std::uint64_t, std::uint64_t, std::uint64_t&) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoWrite(const void*, std::uint64_t, std::uint64_t&) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoSeek(std::int64_t p, SeekMode m, std::uint64_t& np) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x48, (void*)&RTTI, p, m, &np);
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoSetEndOfStream() noexcept(true)
				{
					return EResultError::kUnsupported;
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoPrefetchAt(uint64_t, uint64_t, uint32_t) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoStartTaggedPrioritizedRead(void* buffer, std::uint64_t bytes,
					std::uint64_t offset, std::uint32_t priority, std::uint32_t* completionTagOut, 
					std::uint32_t& completionTagWaitValue, void* unk) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoWaitTags(std::uint32_t* completionTag,
					std::uint32_t completionTagWaitValue, void* unk) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoPrefetchAll(std::uint32_t a) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x70, (void*)&RTTI, a);
				}
				
				bool Archive2::OG_NG::ReaderStream::GetFileName(BSFixedString& result) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x78, (void*)&RTTI, &result);
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoCreateAsync(void* unk) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x80, (void*)&RTTI, unk);
				}
				
				bool Archive2::OG_NG::ReaderStream::DoQTaggedPrioritizedReadSupported() const noexcept(true)
				{
					return false;
				}
				
				EResultError Archive2::OG_NG::ReaderStream::DoCreateOp(void*, const char*) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}
				
				bool Archive2::OG_NG::ReaderStream::DoGetIsFromArchive() const noexcept(true)
				{
					return true;
				}

				Archive2::AE::ReaderStream::~ReaderStream()
				{
					// ctd okay
					//CKPE::vtbl_call<void>(0x0, (void*)&RTTI);
				}

				EResultError Archive2::AE::ReaderStream::DoOpen() noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x8, (void*)&RTTI);
				}

				void Archive2::AE::ReaderStream::DoClose() noexcept(true)
				{
					CKPE::vtbl_call<void>(0x10, (void*)&RTTI);
				}

				std::uint64_t Archive2::AE::ReaderStream::DoGetKey() const noexcept(true)
				{
					return CKPE::vtbl_call<std::uint64_t >(0x18, (void*)&RTTI);
				}

				EResultError Archive2::AE::ReaderStream::DoGetInfo(void*) noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				void Archive2::AE::ReaderStream::DoClone(void* p) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x28, (void*)&RTTI, p);
				}

				EResultError Archive2::AE::ReaderStream::DoRead(void* Buffer, std::uint64_t Bytes, std::uint64_t& Read) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x30, (void*)&RTTI, Buffer, Bytes, Read);
				}

				EResultError Archive2::AE::ReaderStream::DoReadAt(void*, std::uint64_t, std::uint64_t, std::uint64_t&) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				EResultError Archive2::AE::ReaderStream::DoWrite(const void*, std::uint64_t, std::uint64_t&) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				EResultError Archive2::AE::ReaderStream::DoSeek(std::int64_t p, SeekMode m, std::uint64_t& np) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x48, (void*)&RTTI, p, m, &np);
				}

				EResultError Archive2::AE::ReaderStream::DoSetEndOfStream() noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				EResultError Archive2::AE::ReaderStream::DoPrefetchAt(uint64_t, uint64_t, uint32_t) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				EResultError Archive2::AE::ReaderStream::DoStartTaggedPrioritizedRead(void* buffer, std::uint64_t bytes,
					std::uint64_t offset, std::uint32_t priority, std::uint32_t* completionTagOut,
					std::uint32_t& completionTagWaitValue, void* unk) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				EResultError Archive2::AE::ReaderStream::DoWaitTags(std::uint32_t* completionTag,
					std::uint32_t completionTagWaitValue, void* unk) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				EResultError Archive2::AE::ReaderStream::DoPrefetchAll(std::uint32_t a) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x70, (void*)&RTTI, a);
				}

				bool Archive2::AE::ReaderStream::GetFileName(BSFixedString& result) const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x78, (void*)&RTTI, &result);
				}

				EResultError Archive2::AE::ReaderStream::DoCreateAsync(void* unk) const noexcept(true)
				{
					return CKPE::vtbl_call<EResultError>(0x80, (void*)&RTTI, unk);
				}

				bool Archive2::AE::ReaderStream::DoQTaggedPrioritizedReadSupported() const noexcept(true)
				{
					return false;
				}

				EResultError Archive2::AE::ReaderStream::DoCreateOp(void*, const char*) const noexcept(true)
				{
					return EResultError::kUnsupported;
				}

				bool Archive2::AE::ReaderStream::DoGetIsFromArchive() const noexcept(true)
				{
					return true;
				}
			}
		}
	}
}