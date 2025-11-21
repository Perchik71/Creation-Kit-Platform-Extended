// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Asserts.h>
#include <CKPE.Patterns.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <EditorAPI/NiAPI/NiPointer.h>
#include <EditorAPI/BSPointerHandleManager.h>
#include <Patches/CKPE.Fallout4.Patch.ReplaceBSPointerHandleAndManager.h>

#include <chrono>
#include <algorithm>

#define CKPE_FO4_ENABLED_REFLIMIT 1

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_ReplaceBSPointerHandleAndManager_code1 = 0;
			std::uintptr_t pointer_ReplaceBSPointerHandleAndManager_code2 = 0;
			std::uintptr_t pointer_ReplaceBSPointerHandleAndManager_code3 = 0;
			std::uintptr_t pointer_ReplaceBSPointerHandleAndManager_code4 = 0;
			std::uint32_t* pointer_ReplaceBSPointerHandleAndManager_data1 = nullptr;
			std::uint32_t* pointer_ReplaceBSPointerHandleAndManager_data2 = nullptr;

			class HandleManager
			{
			public:
				template<typename HandleType, typename ObjectType>
				static EditorAPI::NiAPI::NiPointer<ObjectType> GetSmartPtr(const HandleType& Handle, bool Exist)
				{
					Exist = false;
					EditorAPI::NiAPI::NiPointer<ObjectType> ObjectPtr;
					if (!EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId)
						Exist = EditorAPI::BSPointerHandleManagerInterface_Original::GetSmartPointer1(Handle, ObjectPtr);
					else if (EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId == 1)
						Exist = EditorAPI::BSPointerHandleManagerInterface_Extended::GetSmartPointer1(Handle, ObjectPtr);
					else
						Exist = EditorAPI::BSPointerHandleManagerInterface_Extended_NG::GetSmartPointer1(Handle, ObjectPtr);
				}
			};

			// There are strings in the Cascadia mod that lead to a crash.
			// This will prevent a crash and return false in this case, hopefully the strings will be deleted when saving.
			/*static bool sub_string_crash(void* unk)
			{
				__try
				{
					return fast_call<bool>(pointer_ReplaceBSPointerHandleAndManager_code1, unk);
				}
				__except (1)
				{
					return false;
				}
			}*/

			ReplaceBSPointerHandleAndManager::ReplaceBSPointerHandleAndManager() : Common::Patch()
			{
				SetName("Replace BSPointerHandle And Manager");
			}

			bool ReplaceBSPointerHandleAndManager::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ReplaceBSPointerHandleAndManager::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ReplaceBSPointerHandleAndManager::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ReplaceBSPointerHandleAndManager::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ReplaceBSPointerHandleAndManager::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1;
			}

			bool ReplaceBSPointerHandleAndManager::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto Extremly = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
				if (Extremly)
					_CONSOLE("[WARNING] An extended set of refs has been included. You use it at your own risk.");

				if (verPatch == 1)
					return Install_163(db, Extremly);
				else if (verPatch == 2)
					return Install_980(db, Extremly);

				return false;
			}

			bool ReplaceBSPointerHandleAndManager::Install_163(Common::RelocatorDB::PatchDB* db, bool Extremly) noexcept(true)
			{
				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				if (Extremly)
				{
					Detours::DetourJump(__CKPE_OFFSET(0),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Extended::InitSDM);
					Detours::DetourCall(__CKPE_OFFSET(1),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Extended::KillSDM);
					Detours::DetourJump(__CKPE_OFFSET(2),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::GetCurrentHandle);
					Detours::DetourJump(__CKPE_OFFSET(3),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::CreateHandle);
					Detours::DetourJump(__CKPE_OFFSET(4),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::Destroy1);
					Detours::DetourJump(__CKPE_OFFSET(5),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::Destroy2);
					Detours::DetourJump(__CKPE_OFFSET(6),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::GetSmartPointer1);
					Detours::DetourJump(__CKPE_OFFSET(7),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::GetSmartPointer2);

					pointer_ReplaceBSPointerHandleAndManager_code1 = __CKPE_OFFSET(9);
					pointer_ReplaceBSPointerHandleAndManager_code2 = __CKPE_OFFSET(10);
					pointer_ReplaceBSPointerHandleAndManager_code3 = __CKPE_OFFSET(11);
					pointer_ReplaceBSPointerHandleAndManager_code4 = __CKPE_OFFSET(12);

					// Cutting a lot is faster this way
					auto stext = _interface->GetApplication()->GetSegment(Segment::text);
					ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

					// Stub out the rest of the functions which shouldn't ever be called now
					SafeWrite::Write(__CKPE_OFFSET(8), { 0xCC });	// BSUntypedPointerHandle::Set

					// Conversion BSHandleRefObject::IncRef and BSHandleRefObject::DecRef to 64bit

					IncRefPatch(stext);
					DecRefPatch(stext);

					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 1;
				}
				else
				{
					Detours::DetourJump(__CKPE_OFFSET(0),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Original::InitSDM);
					Detours::DetourCall(__CKPE_OFFSET(1),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Original::KillSDM);
					Detours::DetourJump(__CKPE_OFFSET(2),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Original::GetCurrentHandle);
					Detours::DetourJump(__CKPE_OFFSET(3),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Original::CreateHandle);
					Detours::DetourJump(__CKPE_OFFSET(4),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Original::Destroy1);
					Detours::DetourJump(__CKPE_OFFSET(5),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Original::Destroy2);
					Detours::DetourJump(__CKPE_OFFSET(6),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Original::GetSmartPointer1);
					Detours::DetourJump(__CKPE_OFFSET(7),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Original::GetSmartPointer2);

					// Stub out the rest of the functions which shouldn't ever be called now
					SafeWrite::Write(__CKPE_OFFSET(8), { 0xCC });	// BSUntypedPointerHandle::Set

					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 0;
				}

				return true;
			}

			bool ReplaceBSPointerHandleAndManager::Install_980(Common::RelocatorDB::PatchDB* db, bool Extremly) noexcept(true)
			{
				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				auto restoring_destroy1 = [](std::uintptr_t rva, std::uint32_t removal_size, std::uintptr_t func)
					{
						SafeWrite::WriteNop(rva, removal_size);
						SafeWrite::Write(rva, { 0x48, 0x89, 0xC1 });
						Detours::DetourCall(rva + 3, func);
					};

				auto restoring_destroy2 = [](std::uintptr_t rva, std::uint8_t off_rsp, std::uint32_t removal_size, std::uintptr_t func)
					{
						SafeWrite::WriteNop(rva, removal_size);
						SafeWrite::Write(rva, { 0x48, 0x8D, 0x4C, 0x24, off_rsp });
						Detours::DetourCall(rva + 5, func);
					};

				/**(std::uintptr_t*)&pointer_ReplaceBSPointerHandleAndManager_code1 =
					Detours::DetourClassJump(__CKPE_OFFSET(6), (std::uintptr_t)&sub_string_crash);*/

				pointer_ReplaceBSPointerHandleAndManager_data1 = (std::uint32_t*)__CKPE_OFFSET(4);
				pointer_ReplaceBSPointerHandleAndManager_data2 = (std::uint32_t*)__CKPE_OFFSET(5);

				if (Extremly)
				{
					using namespace std::chrono;
					auto timerStart = high_resolution_clock::now();
					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 2;

					Detours::DetourCall(__CKPE_OFFSET(0),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Extended_NG::InitSDM);
					Detours::DetourCall(__CKPE_OFFSET(2),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Extended_NG::KillSDM);

					// Cutting a lot is faster this way
					auto textRange = _interface->GetApplication()->GetSegment(Segment::text);
					ScopeSafeWrite text(textRange.GetAddress(), textRange.GetSize());

					auto addr = __CKPE_OFFSET(0);

					if (VersionLists::GetEditorVersion() >= VersionLists::EDITOR_FALLOUT_C4_1_11_137_0)
					{
						// Preparation, removal of all embedded pieces of code
						SafeWrite::WriteNop(addr + 12, 0x7D);
						SafeWrite::WriteMovFromRax(addr + 5, __CKPE_OFFSET(1));
						// Specify the size
						memcpy((void*)(__CKPE_OFFSET(0) + 0x96), &EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					}
					else
					{
						// Preparation, removal of all embedded pieces of code
						SafeWrite::WriteNop(addr + 12, 0x7A);
						SafeWrite::WriteMovFromRax(addr + 5, __CKPE_OFFSET(1));
						// Specify the size
						memcpy((void*)(addr + 0x93), &EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					}

					// Debug (for check)
					//static std::vector<std::uintptr_t> storage;

					auto __InstallPatchByPatternMask = [&textRange](
						const char* pattern_mask,		// pattern "? ? 00 E0 03"
						std::size_t offset,				// offset from find patterns array
						std::size_t count,				// count need change
						std::size_t offset_find,		// offset from in once find pattern
						const void* source,				// buffer
						std::size_t ssize				// buffer size
						) -> std::size_t {
							auto patterns = Patterns::FindsByMask(textRange.GetAddress(), textRange.GetSize(), pattern_mask);

							// Debug (for check)
							//storage.append_range(patterns);

							if (patterns.size() > offset)
							{
								std::size_t max = std::min(patterns.size(), count + offset);
								for (std::size_t index = offset; index < max; index++)
									memcpy((void*)(patterns[index] + offset_find), source, ssize);

								// Debug
								//_CONSOLE("__InstallPatchByPatternMask() function was executed successfully. PatternMask: \"%s\", Count: %llu",
								//	pattern_mask, max - offset);

								return max - offset;
							}
							//else
							//{
							//	// Debug
							//	_CONSOLE("__InstallPatchByPatternMask() function return failed. PatternMask: \"%s\", Count: %llu",
							//		pattern_mask, 0);
							//}

							return 0;
						};

					auto __InstallPatchByPatternMaskCustom = [&textRange](
						const char* pattern_mask,			// pattern "? ? 00 E0 03"
						std::size_t offset,						// offset from find patterns array
						std::size_t count,						// count need change
						bool(callback)(std::uintptr_t addr)		// callback function
						) -> std::size_t {
							auto patterns = Patterns::FindsByMask(textRange.GetAddress(), textRange.GetSize(), pattern_mask);

							// Debug (for check)
							//storage.append_range(patterns);

							if (patterns.size() > offset)
							{
								std::size_t max = std::min(patterns.size(), count + offset);
								std::size_t total = 0;
								for (std::size_t index = offset; index < max; index++)
								{
									if (callback(patterns[index]))
										total++;
								}

								// Debug
								//_CONSOLE("__InstallPatchByPatternMask() function was executed successfully. PatternMask: \"%s\", Count: %llu",
								//	pattern_mask, max - offset);

								return total;
							}
							//else
							//{
							//	// Debug
							//	_CONSOLE("__InstallPatchByPatternMask() function return failed. PatternMask: \"%s\", Count: %llu",
							//		pattern_mask, 0);
							//}

							return 0;
						};

					auto __InstallPatchByPatternMaskEx = [&textRange](
						const char* pattern_mask,					// pattern "? ? 00 E0 03"
						std::size_t offset,							// offset from find patterns array
						std::size_t count,							// count need change
						std::size_t offset_find,					// offset from in once find pattern
						const void* source,							// buffer
						std::size_t ssize,							// buffer size
						std::initializer_list<std::size_t> excludes	// list exclude indexes
						) -> std::size_t {
							auto patterns = Patterns::FindsByMask(textRange.GetAddress(), textRange.GetSize(), pattern_mask);

							// Debug (for check)
							//storage.append_range(patterns);

							if (patterns.size() > offset)
							{
								std::size_t max = std::min(patterns.size(), count + offset);
								std::size_t total = 0;
								for (std::size_t index = offset; index < max; index++)
									if (std::find(excludes.begin(), excludes.end(), index) == std::end(excludes))
									{
										memcpy((void*)(patterns[index] + offset_find), source, ssize);
										total++;
									}

								// Debug
								//_CONSOLE("__InstallPatchByPatternMaskEx() function was executed successfully. PatternMask: \"%s\", Count: %llu",
								//	pattern_mask, total);

								return total;
							}
							//else
							//{
							//	// Debug
							//	_CONSOLE("__InstallPatchByPatternMaskEx() function return failed. PatternMask: \"%s\", Count: %llu",
							//		pattern_mask, 0);
							//}

							return 0;
						};

					std::size_t total = 0;
					std::size_t total_patches = 0;
					static std::uint8_t buffer_cmd[0x10];

#if CKPE_FO4_ENABLED_REFLIMIT
					// == REF_COUNT_MASK
					total += __InstallPatchByPatternMaskCustom("? FF 03 00 00 ? FF 03 00 00", 0, -1, [](std::uintptr_t addr) -> bool
						{
							memcpy((std::uint8_t*)(addr + 1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							memcpy((std::uint8_t*)(addr + 6), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					total += __InstallPatchByPatternMaskCustom("F7 ? ? FF 03 00 00", 0, -1, [](std::uintptr_t addr) -> bool
						{
							if ((*(std::uint8_t*)(addr + 2) != 0x38) && (*(std::uint8_t*)(addr + 2) != 0x8)) return false;
							memcpy((std::uint8_t*)(addr + 3), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					total += __InstallPatchByPatternMaskCustom("81 ? ? FF 03 00 00", 0, -1, [](std::uintptr_t addr) -> bool
						{
							if ((*(std::uint8_t*)(addr + 2) != 0x38) && (*(std::uint8_t*)(addr + 2) != 0x8)) return false;
							memcpy((std::uint8_t*)(addr + 3), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					total += __InstallPatchByPatternMask("FF C8 A9 FF 03 00 00", 0, -1, 3,
						&EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					total += __InstallPatchByPatternMask("FF ? F7 ? FF 03 00 00", 0, -1, 4,
						&EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					total += __InstallPatchByPatternMask("41 F7 ? 24 38 FF 03 00 00", 0, -1, 5,
						&EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					total += __InstallPatchByPatternMask("41 FF ? 41 F7 ? FF 03 00 00", 0, -1, 6,
						&EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);

					total += __InstallPatchByPatternMaskCustom("81 ? FF 03 00 00 81 ? FF 03 00 00", 0, 20, [](std::uintptr_t addr) -> bool
						{
							memcpy((std::uint8_t*)(addr + 2), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							memcpy((std::uint8_t*)(addr + 8), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					total += __InstallPatchByPatternMaskCustom("? FF 03 00 00 ? ? ? ? ? ? ? ? ? FF 03 00 00", 0, 20, [](std::uintptr_t addr) -> bool
						{
							if (*(std::uint8_t*)(addr + 5) != 0x8D)
							{
								memcpy((std::uint8_t*)(addr + 1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
								memcpy((std::uint8_t*)(addr + 14), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							}
							return true;
						});

					memcpy((std::uint8_t*)(base + 0x538ACF), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0x53B0A1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0x5B64B6), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0x5B64C4), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0xD3A786), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0xD9B4E2), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0xD9B5D1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0x5AF00D), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0x5AF018), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0x5AF2B0), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((std::uint8_t*)(base + 0x5AF2C6), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);

					total_patches += total + 7;
					total = 0;

					// CreateHandle fix
					*(std::uint8_t*)(base + 0x526D5D) = (std::uint8_t)EditorAPI::BSHandleRefObject_Extremly::ACTIVE_BIT_INDEX;
					*(std::uint8_t*)(base + 0x526DC6) = (std::uint8_t)EditorAPI::BSHandleRefObject_Extremly::ACTIVE_BIT_INDEX;
					*(std::uint8_t*)(base + 0x526EAC) = (std::uint8_t)EditorAPI::BSHandleRefObject_Extremly::ACTIVE_BIT_INDEX;
					*(std::uint8_t*)(base + 0x526D64) = (std::uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;
					*(std::uint8_t*)(base + 0x526DCE) = (std::uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;
					*(std::uint8_t*)(base + 0x526EA8) = (std::uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;

					addr = (std::uintptr_t)__CKPE_OFFSET(3);
					memcpy((void*)(addr + 0x12F), &EditorAPI::BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT, 4);
					std::uint32_t mask = EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_ACTIVE_BIT |
						EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT;
					memcpy((void*)(addr + 0x134), &mask, 4);

					// Change HANDLE_BIT_INDEX
					std::uint8_t bit_byte = (std::uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;
					total += __InstallPatchByPatternMask("C1 ? 0B 3B", 0, -1, 2, &bit_byte, 1);
					total += __InstallPatchByPatternMask("C1 ? 0B 41 3B", 0, -1, 2, &bit_byte, 1);

					total_patches += total;
					total = 0;

					// Change AGE
					total += __InstallPatchByPatternMask("A9 00 00 E0 03", 0, -1, 1,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);
					total += __InstallPatchByPatternMask("F7 ? 00 00 E0 03", 0, -1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);
					total += __InstallPatchByPatternMask("25 00 00 E0 03", 0, -1, 1,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? 00 00 E0 03", 0, -1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);

					total_patches += total;
					total = 0;

					// Change INDEX
					total += __InstallPatchByPatternMask("41 81 ? FF FF 1F 00", 0, -1, 3,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 E1 FF FF 1F 00", 3, 25, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMaskEx("81 E5 FF FF 1F 00", 0, -1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4, { 4, 8 });
					total += __InstallPatchByPatternMaskEx("81 E6 FF FF 1F 00", 3, 24, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4, { 7, 8 });
					total += __InstallPatchByPatternMask("81 E7 FF FF 1F 00", 2, 1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("8B C7 25 FF FF 1F 00 8B D8", 0, -1, 3,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? 48 ? ? 04", 0, -1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? 48 ? ? 04", 0, -1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? 44 ? ? 41 ? 00 00 00 00 49 ? ? 04", 0, -1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 49 ? ? 04", 0, -1, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 E2 FF FF 1F 00", 0, 2, 2,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);

					memcpy((void*)(base + 0x5279F9),
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);

					total += 1;
					total_patches += total;
					total = 0;

					std::uint32_t not_mask = ~EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT;

					// Change NOT MASK_INDEX_BIT
					total += __InstallPatchByPatternMask("81 ? 00 00 E0 FF", 0, 56, 2, &not_mask, 4);

					total_patches += total;
					//_MESSAGE("NOT MASK_INDEX_BIT Total %llu", total);

					total = 0;
					not_mask = ~EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_ACTIVE_BIT;

					// Change NOT MASK_ACTIVE_BIT
					total += __InstallPatchByPatternMask("81 ? FF FF FF FB", 0, -1, 2, &not_mask, 4);

					total_patches += total;
					//_MESSAGE("NOT MASK_INDEX_BIT Total %llu", total);

					total = 0;

					// Change UNUSED_BIT_START
					total += __InstallPatchByPatternMask("0F BA E0 1A", 0, 317, 3,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::UNUSED_BIT_START, 1);
					total += __InstallPatchByPatternMaskEx("0F BA ?? 1A", 0, 72, 3,
						&EditorAPI::BSUntypedPointerHandle_Extended_NG::UNUSED_BIT_START, 1,
						{ 18, 19, 23, 24, 30, 38, 39, 47, 48, 63, 64, 65, 66, 67, 68 });

					total_patches += total;
					total = 0;

					// Change MAX_HANDLE_COUNT
					total += __InstallPatchByPatternMask("81 ? 00 00 20 00", 0, 1, 2, &EditorAPI::BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT, 4);
					total += __InstallPatchByPatternMask("BD 00 00 20 00", 0, -1, 1, &EditorAPI::BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT, 4);

					total_patches += total;
					//_MESSAGE("MAX_HANDLE_COUNT Total %llu", total);

					total = 0;

					// Change MASK_ACTIVE_BIT
					total += __InstallPatchByPatternMask("F7 ? 00 00 00 04", 0, 3, 2, &EditorAPI::BSUntypedPointerHandle_Extended_NG::MASK_ACTIVE_BIT, 4);
#else
					CKPE_ASSERT_MSG("Refrs extension is disabled by perchik71, it may be in implementation or abandoned.");
#endif /* CKPE_FO4_ENABLED_REFLIMIT */

					auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - timerStart).count();
					_CONSOLE("CreationKitPlatformExtended::Experimental::BSPointerHandle: %llu patches applied in %llums", total_patches, duration);
				}
				else
				{
					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 0;

					{
						auto addr = (std::uintptr_t)__CKPE_OFFSET(0);
						// Preparation, removal of all embedded pieces of code
						SafeWrite::WriteNop(addr + 12, 0x7A);
						SafeWrite::WriteMovFromRax(addr + 5, __CKPE_OFFSET(1));
					}

					Detours::DetourCall(__CKPE_OFFSET(0),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Original::InitSDM);
					Detours::DetourCall(__CKPE_OFFSET(2),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Original::KillSDM);
				}

				return true;
			}

			std::uint32_t ReplaceBSPointerHandleAndManager::CheckEx(std::uintptr_t unused, std::uintptr_t refr) noexcept(true)
			{
				if (!refr)
					return 0;

				return ((EditorAPI::Forms::TESObjectREFR_Extremly*)refr)->GetHandleEntryIndex();
			}

			std::uint32_t ReplaceBSPointerHandleAndManager::Check(std::uintptr_t unused, std::uintptr_t refr) noexcept(true)
			{
				if (!refr)
					return 0;

				return ((EditorAPI::Forms::TESObjectREFR_Original*)refr)->GetHandleEntryIndex();
			}

			void ReplaceBSPointerHandleAndManager::IncRefPatch(const Segment& t) noexcept(true)
			{
				auto SignIncRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(),
					"4C 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? BA 3B 00 00 00 E8");
				std::size_t patched = 0, need_p = 0;
				std::uint8_t szbuff[64];

				for (auto sign : SignIncRef)
				{
					auto start_r = sign + 24;

					if ((*((std::uint8_t*)start_r) == 0xF0)) // lock
					{
						if (*((std::uint8_t*)(start_r + 1)) == 0xFF) // inc
						{
							memcpy_s(szbuff, 3, (std::uint8_t*)start_r + 1, 3);

							memset((std::uint8_t*)start_r, 0x90, 0xB);
							((std::uint8_t*)start_r)[0] = 0xF0;
							((std::uint8_t*)start_r)[1] = 0x48;
							memcpy_s((std::uint8_t*)(start_r + 2), 3, szbuff, 3);

							((std::uint8_t*)start_r)[0xB] = 0xEB;	// jmp	(skip test)

							patched++;
#if FALLOUT4_DEVELOPER_MODE
							memset((uint8_t*)sign, 0x90, 7);
#endif
						}
						else if ((*((std::uint8_t*)(start_r + 1)) == 0x41) && (*((std::uint8_t*)(start_r + 2)) == 0xFF)) // inc r8 - r15
						{
							((std::uint8_t*)start_r)[1] = 0x49;
							((std::uint8_t*)start_r)[5] = 0x49;

							patched++;
#if FALLOUT4_DEVELOPER_MODE
							memset((uint8_t*)sign, 0x90, 7);
#endif
						}
#if FALLOUT4_DEVELOPER_MODE
						need_p++;
#endif
					}
				}

				_MESSAGE("BSHandleRefObject::IncRef (Patched: %d)", patched);
			}

			void ReplaceBSPointerHandleAndManager::DecRefPatch(const Segment& t) noexcept(true)
			{
				auto IsJump = [](std::uintptr_t off) -> bool {
					return ((*((std::uint8_t*)(off)) == 0x75) || (*((std::uint8_t*)(off)) == 0x74) ||
						((*((std::uint8_t*)(off)) == 0x0F) && ((*((std::uint8_t*)(off + 1)) == 0x85) || (*((std::uint8_t*)(off + 1)) == 0x84))));
					};

				std::size_t patched = 0;
				std::uint8_t szbuff[64];
				std::uint8_t* src = nullptr;
				std::int8_t jump_to = 0;
				std::int8_t jump_from = 0;

				auto refmask = (DWORD)EditorAPI::BSHandleRefObject_64_Extremly::REF_COUNT_MASK;
				auto SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "83 ? FF F0 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x29;

					patched_var1:

						src = (std::uint8_t*)(sign - jump_from);

						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0xF);
						memset(src, 0x90, jump_to);

						src[0] = 0x48;
						memcpy_s(src + 1, 0xF, szbuff, 3);
						src[4] = 0xF0;
						src[5] = 0x48;
						memcpy_s(src + 6, 0xF, szbuff + 4, 4);
						src[10] = 0x48;
						memcpy_s(src + 11, 0xF, szbuff + 8, 2);
						src[13] = 0x48;
						src[14] = szbuff[10];
						memcpy_s(src + 15, 0xF, &refmask, sizeof(refmask));
						src[19] = 0xEB;
						src[20] = jump_to - 21;

						src = (std::uint8_t*)(sign + 3);
						src[0] = 0xEB;
						src[1] = -(jump_from + 5);

						patched++;
					}
					else if (*((std::uint8_t*)(sign - 0x1E)) == 0x77)
					{
						jump_from = 0x1E;
						jump_to = 0x2D;

						goto patched_var1;
					}
					else if ((*((std::uint8_t*)(sign - 0x1B)) == 0x77) && (*((std::uint8_t*)(sign - 1)) == 0x48))
					{
						jump_from = 0x1B;
						jump_to = 0x2A;

						goto patched_var1;
					}
					else if (*((std::uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x27;

						goto patched_var1;
					}
					else if (*((std::uint8_t*)(sign - 0x19)) == 0x77)
					{
						jump_from = 0x19;
						jump_to = 0x28;

						goto patched_var1;
					}
				}

				SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "83 ? FF F0 0F ? ? ? FF ? F7 ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

					patched_var2:

						src = (std::uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0x10);
						memset(src, 0x90, jump_to);

						src[0] = 0x48;
						memcpy_s(src + 1, 0x10, szbuff, 3);
						src[4] = 0xF0;
						src[5] = 0x48;
						memcpy_s(src + 6, 0x10, szbuff + 4, 4);
						src[10] = 0x48;
						memcpy_s(src + 11, 0x10, szbuff + 8, 2);
						src[13] = 0x48;
						memcpy_s(src + 14, 0x10, szbuff + 10, 2);
						memcpy_s(src + 16, 0x10, &refmask, sizeof(refmask));
						src[20] = 0xEB;
						src[21] = jump_to - 22;

						src = (std::uint8_t*)(sign + 3);
						src[0] = 0xEB;
						src[1] = -(jump_from + 5);

						patched++;
					}
					else if ((*((std::uint8_t*)(sign - 0x1B)) == 0x77) && (*((std::uint8_t*)(sign - 1)) == 0x48))
					{
						jump_from = 0x1B;
						jump_to = 0x2B;

						goto patched_var2;
					}
					else if (*((std::uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x28;

						goto patched_var2;
					}
				}

				SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "83 ? FF F0 ? 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

						src = (std::uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0x10);
						memset(src, 0x90, jump_to);

						src[0] = 0x48;
						memcpy_s(src + 1, 0x10, szbuff, 3);
						src[4] = 0xF0;
						src[5] = szbuff[4] == 0x41 ? 0x49 : 0x4C;
						memcpy_s(src + 6, 0x10, szbuff + 5, 4);
						src[10] = 0x48;
						memcpy_s(src + 11, 0x10, szbuff + 9, 2);
						src[13] = 0x48;
						src[14] = szbuff[11];
						memcpy_s(src + 15, 0x10, &refmask, sizeof(refmask));
						src[19] = 0xEB;
						src[20] = jump_to - 21;

						src = (std::uint8_t*)(sign + 3);
						src[0] = 0xEB;
						src[1] = -(jump_from + 5);

						patched++;
					}
				}

				SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "8B ? F0 0F ? ? ? FF ? ? FF 03 00 00");

				// or r??, FF
				// lock xadd qword ptr ss:[r??+38], r??
				// dec r??
				// test r??, 3FF
				// 
				// ^ ^ converting the bottom to the top ^ ^
				// 
				// mov e??, e??
				// lock xadd dword ptr ss:[r??+38], e??
				// dec e??
				// test e??, 3FF

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x28;

					patched_var3:

						src = (std::uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0xE);
						memset(src, 0x90, jump_to);

						src[0] = 0x48;
						src[1] = 0x83;
						src[2] = szbuff[8];
						src[3] = 0xFF;

						src[4] = 0xF0;
						src[5] = 0x48;
						memcpy_s(src + 6, 0xE, szbuff + 3, 4);
						src[10] = 0x48;
						memcpy_s(src + 11, 0xE, szbuff + 7, 2);
						src[13] = 0x48;
						src[14] = szbuff[9];
						memcpy_s(src + 15, 0xE, &refmask, sizeof(refmask));
						src[19] = 0xEB;
						src[20] = jump_to - 21;

						src = (std::uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					if ((*((std::uint8_t*)(sign - 0x1B)) == 0x77) && (*((std::uint8_t*)(sign - 1)) == 0x41))
					{
						jump_from = 0x1B;
						jump_to = 0x29;

						goto patched_var3;
					}
					if ((*((std::uint8_t*)(sign - 0x1D)) == 0x77) && (*((std::uint8_t*)(sign - 3)) == 0x83))
					{
						jump_from = 0x1D;
						jump_to = 0x2B;

						goto patched_var3;
					}
					else if ((*((std::uint8_t*)(sign - 0x1F)) == 0x77) && (*((uint8_t*)(sign - 4)) == 0x83))
					{
						jump_from = 0x1F;
						jump_to = 0x2D;

						goto patched_var3;
					}
					else if (*((std::uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x26;

						goto patched_var3;
					}
					else if (*((std::uint8_t*)(sign - 0x19)) == 0x77)
					{
						jump_from = 0x19;
						jump_to = 0x27;

						goto patched_var3;
					}
				}

				SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "41 8B ? F0 ? 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

					patched_var5:

						src = (std::uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0x10);
						memset(src, 0x90, jump_to);

						src[0] = 0x48;
						src[1] = 0x83;
						src[2] = szbuff[10];
						src[3] = 0xFF;

						src[4] = 0xF0;
						src[5] = (szbuff[4] == 0x44) ? 0x4C : 0x49;
						memcpy_s(src + 6, 0x10, szbuff + 5, 4);
						src[10] = 0x48;
						memcpy_s(src + 11, 0xE, szbuff + 9, 2);
						src[13] = 0x48;
						src[14] = szbuff[11];
						memcpy_s(src + 15, 0x10, &refmask, sizeof(refmask));
						src[19] = 0xEB;
						src[20] = jump_to - 21;

						src = (std::uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					else if ((*((std::uint8_t*)(sign - 0x1E)) == 0x77) && (*((std::uint8_t*)(sign - 3)) == 0x83))
					{
						jump_from = 0x1E;
						jump_to = 0x2E;

						goto patched_var5;
					}
				}

				SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "F0 ? 0F ? ? ? 41 FF ? 41 F7 ? FF 03 00 00");

				// or r??, FF
				// lock xadd qword ptr ss:[r??+38], r??
				// dec r??
				// test r??, 3FF
				// 
				// ^ ^ converting the bottom to the top ^ ^
				// 
				// lock xadd dword ptr ss:[r??+38], e??
				// dec e??
				// test e??, 3FF

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

					patched_var6:

						src = (std::uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0x10);
						memset(src, 0x90, jump_to);

						src[0] = 0x49;
						src[1] = 0x83;
						src[2] = szbuff[8];
						src[3] = 0xFF;

						src[4] = 0xF0;
						src[5] = szbuff[1] == 0x44 ? 0x4C : 0x49;
						memcpy_s(src + 6, 0x10, szbuff + 2, 4);
						src[10] = 0x49;
						memcpy_s(src + 11, 0x10, szbuff + 7, 2);
						src[13] = 0x49;
						memcpy_s(src + 14, 0x10, szbuff + 10, 2);
						memcpy_s(src + 16, 0x10, &refmask, sizeof(refmask));
						src[20] = 0xEB;
						src[21] = jump_to - 22;

						src = (std::uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					else if ((*((std::uint8_t*)(sign - 0x1E)) == 0x77) && (*((std::uint8_t*)(sign - 3)) == 0x83))
					{
						jump_from = 0x1E;
						jump_to = 0x2E;

						goto patched_var6;
					}
				}

				SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "F0 0F ? ? ? FF ? F7 ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x27;

					patched_var7:

						src = (std::uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0xD);
						memset(src, 0x90, jump_to);

						src[0] = 0x48;
						src[1] = 0x83;
						src[2] = szbuff[6];
						src[3] = 0xFF;

						src[4] = 0xF0;
						src[5] = 0x48;
						memcpy_s(src + 6, 0x10, szbuff + 1, 4);
						src[10] = 0x48;
						memcpy_s(src + 11, 0x10, szbuff + 5, 2);
						src[13] = 0x48;
						memcpy_s(src + 14, 0x10, szbuff + 7, 2);
						memcpy_s(src + 16, 0x10, &refmask, sizeof(refmask));
						src[20] = 0xEB;
						src[21] = jump_to - 22;

						src = (std::uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					else if (*((std::uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x25;

						goto patched_var7;
					}
				}

				SignDecRef = Patterns::FindsByMask(t.GetAddress(), t.GetSize(), "8B ? F0 ? 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((std::uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x27;

						src = (std::uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0xF);
						memset(src, 0x90, jump_to);

						src[0] = 0x48;
						src[1] = 0x83;
						src[2] = szbuff[9];
						src[3] = 0xFF;

						src[4] = 0xF0;
						src[5] = szbuff[3] == 0x41 ? 0x49 : 0x4C;
						memcpy_s(src + 6, 0xF, szbuff + 4, 4);
						src[10] = 0x48;
						memcpy_s(src + 11, 0xF, szbuff + 8, 2);
						src[13] = 0x48;
						src[14] = szbuff[10];
						memcpy_s(src + 15, 0xF, &refmask, sizeof(refmask));
						src[19] = 0xEB;
						src[20] = jump_to - 21;

						src = (std::uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
				}

				auto sign = pointer_ReplaceBSPointerHandleAndManager_code1;
				jump_from = 0x1A;
				jump_to = 0x30;

				src = (std::uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0x16);
				memset(src, 0x90, jump_to);

				src[0] = 0x48;
				memcpy_s(src + 1, 0x16, szbuff, 3);
				src[4] = 0xF0;
				src[5] = 0x48;
				memcpy_s(src + 6, 0x16, szbuff + 4, 4);
				memcpy_s(src + 10, 0x16, szbuff + 8, 7);
				src[17] = 0x48;
				memcpy_s(src + 18, 0x16, szbuff + 15, 2);
				src[20] = 0x48;
				src[21] = szbuff[17];
				memcpy_s(src + 22, 0x16, &refmask, sizeof(refmask));
				src[26] = 0xEB;
				src[27] = jump_to - 28;

				src = (std::uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				patched++;

				sign = pointer_ReplaceBSPointerHandleAndManager_code2;
				jump_from = 0x1A;
				jump_to = 0x28;

				src = (std::uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0xE);
				memset(src, 0x90, jump_to);

				src[0] = 0x48;
				src[1] = 0x83;
				src[2] = szbuff[7];
				src[3] = 0xFF;

				src[4] = 0xF0;
				src[5] = 0x49;
				memcpy_s(src + 6, 0xE, szbuff + 2, 4);
				src[10] = 0x48;
				memcpy_s(src + 11, 0xE, szbuff + 6, 2);
				src[13] = 0x48;
				memcpy_s(src + 14, 0xE, szbuff + 8, 2);
				memcpy_s(src + 16, 0xE, &refmask, sizeof(refmask));
				src[20] = 0xEB;
				src[21] = jump_to - 22;

				src = (std::uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				patched++;

				sign = pointer_ReplaceBSPointerHandleAndManager_code3;
				jump_from = 0x1A;
				jump_to = 0x2B;

				src = (std::uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0x11);
				memset(src, 0x90, jump_to);

				src[0] = 0x48;
				memcpy_s(src + 1, 0x11, szbuff, 3);
				src[4] = 0xF0;
				src[5] = 0x49;
				memcpy_s(src + 6, 0x11, szbuff + 5, 5);
				src[11] = 0x48;
				memcpy_s(src + 12, 0x11, szbuff + 10, 2);
				src[14] = 0x48;
				src[15] = szbuff[12];
				memcpy_s(src + 16, 0x11, &refmask, sizeof(refmask));
				src[20] = 0xEB;
				src[21] = jump_to - 22;

				src = (std::uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				sign = pointer_ReplaceBSPointerHandleAndManager_code4;
				jump_from = 0x1A;
				jump_to = 0x2B;

				src = (std::uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (std::uint8_t*)sign, 0x11);
				memset(src, 0x90, jump_to);

				src[0] = 0x48;
				memcpy_s(src + 1, 0x11, szbuff, 3);
				src[4] = 0xF0;
				src[5] = 0x49;
				memcpy_s(src + 6, 0x11, szbuff + 5, 4);
				src[10] = 0x48;
				memcpy_s(src + 11, 0x11, szbuff + 9, 2);
				src[13] = 0x48;
				memcpy_s(src + 14, 0x11, szbuff + 11, 2);
				memcpy_s(src + 16, 0x11, &refmask, sizeof(refmask));
				src[20] = 0xEB;
				src[21] = jump_to - 22;

				src = (std::uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				patched += 2;

				_MESSAGE("BSHandleRefObject::DecRef (Patched: %d)", patched);
			}
		}
	}
}