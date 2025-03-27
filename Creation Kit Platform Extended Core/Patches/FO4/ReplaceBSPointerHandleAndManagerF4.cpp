// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/FO4/BSPointerHandleManager.h"
#include "ReplaceBSPointerHandleAndManagerF4.h"

#define CKPE_FO4_ENABLED_REFLIMIT 1

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

			uintptr_t pointer_ReplaceBSPointerHandleAndManager_code1 = 0;
			uintptr_t pointer_ReplaceBSPointerHandleAndManager_code2 = 0;
			uintptr_t pointer_ReplaceBSPointerHandleAndManager_code3 = 0;
			uintptr_t pointer_ReplaceBSPointerHandleAndManager_code4 = 0;
			uint32_t* pointer_ReplaceBSPointerHandleAndManager_data1 = nullptr;
			uint32_t* pointer_ReplaceBSPointerHandleAndManager_data2 = nullptr;

			class HandleManager
			{
			public:
				template<typename HandleType>
				static NiPointer<ObjectType> GetSmartPtr(const HandleType& Handle, bool Exist)
				{
					Exist = false;
					NiPointer<ObjectType> ObjectPtr;
					if (!BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId)
						Exist = BSPointerHandleManagerInterface_Original::GetSmartPointer1(Handle, ObjectPtr);
					else if (BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId == 1)
						Exist = BSPointerHandleManagerInterface_Extended::GetSmartPointer1(Handle, ObjectPtr);
					else
						Exist = BSPointerHandleManagerInterface_Extended_NG::GetSmartPointer1(Handle, ObjectPtr);
				}
			};

			// There are strings in the Cascadia mod that lead to a crash.
			// This will prevent a crash and return false in this case, hopefully the strings will be deleted when saving.
			static bool sub_string_crash(void* unk)
			{
				__try
				{
					return fastCall<bool>(pointer_ReplaceBSPointerHandleAndManager_code1, unk);
				}
				__except (1)
				{
					return false;
				}
			}

			ReplaceBSPointerHandleAndManagerPatch::ReplaceBSPointerHandleAndManagerPatch() : Module(GlobalEnginePtr)
			{}

			bool ReplaceBSPointerHandleAndManagerPatch::HasOption() const
			{
				return false;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ReplaceBSPointerHandleAndManagerPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ReplaceBSPointerHandleAndManagerPatch::GetName() const
			{
				return "Replace BSPointerHandle And Manager";
			}

			bool ReplaceBSPointerHandleAndManagerPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ReplaceBSPointerHandleAndManagerPatch::GetDependencies() const
			{
				return {};
			}

			bool ReplaceBSPointerHandleAndManagerPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST) &&
					(eEditorCurrentVersion != EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_943_1);
			}

			bool ReplaceBSPointerHandleAndManagerPatch::IsVersionValid(const RelocationDatabaseItem* lpRelocationDatabaseItem) const
			{
				auto verPatch = lpRelocationDatabaseItem->Version();
				return (verPatch == 1) || (verPatch == 2);
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Install_163(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem, bool Extremly)
			{
				if (Extremly)
				{
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0),
						(uintptr_t)&BSPointerHandleManager_Extended::InitSDM);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1),
						(uintptr_t)&BSPointerHandleManager_Extended::KillSDM);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2),
						(uintptr_t)&BSPointerHandleManagerInterface_Extended::GetCurrentHandle);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3),
						(uintptr_t)&BSPointerHandleManagerInterface_Extended::CreateHandle);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4),
						(uintptr_t)&BSPointerHandleManagerInterface_Extended::Destroy1);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(5),
						(uintptr_t)&BSPointerHandleManagerInterface_Extended::Destroy2);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(6),
						(uintptr_t)&BSPointerHandleManagerInterface_Extended::GetSmartPointer1);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(7),
						(uintptr_t)&BSPointerHandleManagerInterface_Extended::GetSmartPointer2);

					pointer_ReplaceBSPointerHandleAndManager_code1 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(9));
					pointer_ReplaceBSPointerHandleAndManager_code2 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(10));
					pointer_ReplaceBSPointerHandleAndManager_code3 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(11));
					pointer_ReplaceBSPointerHandleAndManager_code4 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(12));

					ScopeRelocator textSection;

					// Stub out the rest of the functions which shouldn't ever be called now
					lpRelocator->Patch(lpRelocationDatabaseItem->At(8), { 0xCC });	// BSUntypedPointerHandle::Set

					// Conversion BSHandleRefObject::IncRef and BSHandleRefObject::DecRef to 64bit

					IncRefPatch();
					DecRefPatch();

					BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 1;
				}
				else
				{
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0),
						(uintptr_t)&BSPointerHandleManager_Original::InitSDM);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1),
						(uintptr_t)&BSPointerHandleManager_Original::KillSDM);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2),
						(uintptr_t)&BSPointerHandleManagerInterface_Original::GetCurrentHandle);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3),
						(uintptr_t)&BSPointerHandleManagerInterface_Original::CreateHandle);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4),
						(uintptr_t)&BSPointerHandleManagerInterface_Original::Destroy1);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(5),
						(uintptr_t)&BSPointerHandleManagerInterface_Original::Destroy2);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(6),
						(uintptr_t)&BSPointerHandleManagerInterface_Original::GetSmartPointer1);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(7),
						(uintptr_t)&BSPointerHandleManagerInterface_Original::GetSmartPointer2);

					pointer_ReplaceBSPointerHandleAndManager_code1 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(9));
					pointer_ReplaceBSPointerHandleAndManager_code2 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(10));
					pointer_ReplaceBSPointerHandleAndManager_code3 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(11));
					pointer_ReplaceBSPointerHandleAndManager_code4 =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(12));

					ScopeRelocator textSection;

					// Stub out the rest of the functions which shouldn't ever be called now
					lpRelocator->Patch(lpRelocationDatabaseItem->At(8), { 0xCC });	// BSUntypedPointerHandle::Set

					BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 0;
				}

				return true;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Install_980(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem, bool Extremly)
			{
				auto restoring_destroy1 = [&lpRelocator](uintptr_t rva, uint32_t removal_size, uintptr_t func)
				{
					lpRelocator->PatchNop(rva, removal_size);
					lpRelocator->Patch(rva, { 0x48, 0x89, 0xC1 });
					lpRelocator->DetourCall(rva + 3, func);
				};

				auto restoring_destroy2 = [&lpRelocator](uintptr_t rva, uint8_t off_rsp, uint32_t removal_size, uintptr_t func)
				{
					lpRelocator->PatchNop(rva, removal_size);
					lpRelocator->Patch(rva, { 0x48, 0x8D, 0x4C, 0x24, off_rsp });
					lpRelocator->DetourCall(rva + 5, func);
				};

				*(uintptr_t*)&pointer_ReplaceBSPointerHandleAndManager_code1 =
					voltek::detours_function_class_jump(_RELDATA_ADDR(6), (uintptr_t)&sub_string_crash);

				pointer_ReplaceBSPointerHandleAndManager_data1 = (uint32_t*)_RELDATA_ADDR(4);
				pointer_ReplaceBSPointerHandleAndManager_data2 = (uint32_t*)_RELDATA_ADDR(5);

				if (Extremly)
				{
					using namespace std::chrono;
					auto timerStart = high_resolution_clock::now();
					BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 2;

					lpRelocator->DetourCall(_RELDATA_RAV(0),
						(uintptr_t)&BSPointerHandleManager_Extended_NG::InitSDM);
					lpRelocator->DetourCall(_RELDATA_RAV(2),
						(uintptr_t)&BSPointerHandleManager_Extended_NG::KillSDM);

					ScopeRelocator textSection;
					auto textRange = GlobalEnginePtr->GetSection(SECTION_TEXT);

					auto addr = (uintptr_t)_RELDATA_RAV(0);
					// Preparation, removal of all embedded pieces of code
					lpRelocator->PatchNop(addr + 12, 0x7A);
					lpRelocator->PatchMovFromRax(addr + 5, _RELDATA_RAV(1));
					// Specify the size
					memcpy((void*)((uintptr_t)_RELDATA_ADDR(0) + 0x93), &BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);

					// Debug (for check)
					//static std::vector<uintptr_t> storage;

					auto __InstallPatchByPatternMask = [&textRange](
						const char* pattern_mask,	// pattern "? ? 00 E0 03"
						size_t offset,				// offset from find patterns array
						size_t count,				// count need change
						size_t offset_find,			// offset from in once find pattern
						const void* source,			// buffer
						size_t ssize				// buffer size
						) -> size_t {
							auto patterns = voltek::find_patterns(textRange.base, textRange.end - textRange.base, pattern_mask);

							// Debug (for check)
							//storage.append_range(patterns);

							if (patterns.size() > offset)
							{
								size_t max = std::min(patterns.size(), count + offset);
								for (size_t index = offset; index < max; index++)
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
						size_t offset,						// offset from find patterns array
						size_t count,						// count need change
						bool(callback)(uintptr_t addr)		// callback function
						) -> size_t {
							auto patterns = voltek::find_patterns(textRange.base, textRange.end - textRange.base, pattern_mask);

							// Debug (for check)
							//storage.append_range(patterns);

							if (patterns.size() > offset)
							{
								size_t max = std::min(patterns.size(), count + offset);
								size_t total = 0;
								for (size_t index = offset; index < max; index++)
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
						const char* pattern_mask,				// pattern "? ? 00 E0 03"
						size_t offset,							// offset from find patterns array
						size_t count,							// count need change
						size_t offset_find,						// offset from in once find pattern
						const void* source,						// buffer
						size_t ssize,							// buffer size
						std::initializer_list<size_t> excludes	// list exclude indexes
						) -> size_t {
							auto patterns = voltek::find_patterns(textRange.base, textRange.end - textRange.base, pattern_mask);

							// Debug (for check)
							//storage.append_range(patterns);

							if (patterns.size() > offset)
							{
								size_t max = std::min(patterns.size(), count + offset);
								size_t total = 0;
								for (size_t index = offset; index < max; index++)
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

					size_t total = 0;
					size_t total_patches = 0;
					static uint8_t buffer_cmd[0x10];

#if CKPE_FO4_ENABLED_REFLIMIT
					// == REF_COUNT_MASK
					total += __InstallPatchByPatternMaskCustom("? FF 03 00 00 ? FF 03 00 00", 0, -1, [](uintptr_t addr) -> bool
						{
							memcpy((uint8_t*)(addr + 1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							memcpy((uint8_t*)(addr + 6), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					total += __InstallPatchByPatternMaskCustom("F7 ? ? FF 03 00 00", 0, -1, [](uintptr_t addr) -> bool
						{
							if ((*(uint8_t*)(addr + 2) != 0x38) && (*(uint8_t*)(addr + 2) != 0x8)) return false;
							memcpy((uint8_t*)(addr + 3), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					total += __InstallPatchByPatternMaskCustom("81 ? ? FF 03 00 00", 0, -1, [](uintptr_t addr) -> bool
						{
							if ((*(uint8_t*)(addr + 2) != 0x38) && (*(uint8_t*)(addr + 2) != 0x8)) return false;
							memcpy((uint8_t*)(addr + 3), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
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

					total += __InstallPatchByPatternMaskCustom("81 ? FF 03 00 00 81 ? FF 03 00 00", 0, 20, [](uintptr_t addr) -> bool
						{
							memcpy((uint8_t*)(addr + 2), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							memcpy((uint8_t*)(addr + 8), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					total += __InstallPatchByPatternMaskCustom("? FF 03 00 00 ? ? ? ? ? ? ? ? ? FF 03 00 00", 0, 20, [](uintptr_t addr) -> bool
						{
							memcpy((uint8_t*)(addr + 1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							memcpy((uint8_t*)(addr + 14), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
							return true;
						});

					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x538ACF), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x53B0A1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x5B64B6), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x5B64C4), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0xD3A786), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0xD9B4E2), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0xD9B5D1), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);	
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x5AF00D), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x5AF018), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x5AF2B0), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);
					memcpy((uint8_t*)(lpRelocator->GetBase() + 0x5AF2C6), &EditorAPI::BSHandleRefObject_Extremly::REF_COUNT_MASK, 4);

					total_patches += total + 7;
					total = 0;

					// CreateHandle fix
					*(uint8_t*)(lpRelocator->GetBase() + 0x526D5D) = (uint8_t)EditorAPI::BSHandleRefObject_Extremly::ACTIVE_BIT_INDEX;
					*(uint8_t*)(lpRelocator->GetBase() + 0x526DC6) = (uint8_t)EditorAPI::BSHandleRefObject_Extremly::ACTIVE_BIT_INDEX;
					*(uint8_t*)(lpRelocator->GetBase() + 0x526EAC) = (uint8_t)EditorAPI::BSHandleRefObject_Extremly::ACTIVE_BIT_INDEX;
					*(uint8_t*)(lpRelocator->GetBase() + 0x526D64) = (uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;
					*(uint8_t*)(lpRelocator->GetBase() + 0x526DCE) = (uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;
					*(uint8_t*)(lpRelocator->GetBase() + 0x526EA8) = (uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;

					addr = (uintptr_t)_RELDATA_ADDR(3);
					memcpy((void*)(addr + 0x12F), &BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT, 4);
					uint32_t mask = BSUntypedPointerHandle_Extended_NG::MASK_ACTIVE_BIT | BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT;
					memcpy((void*)(addr + 0x134), &mask, 4);

					// Change HANDLE_BIT_INDEX
					uint8_t bit_byte = (uint8_t)EditorAPI::BSHandleRefObject_Extremly::HANDLE_BIT_INDEX;
					total += __InstallPatchByPatternMask("C1 ? 0B 3B", 0, -1, 2, &bit_byte, 1);
					total += __InstallPatchByPatternMask("C1 ? 0B 41 3B", 0, -1, 2, &bit_byte, 1);

					total_patches += total;
					total = 0;

					// Change AGE
					total += __InstallPatchByPatternMask("A9 00 00 E0 03", 0, -1, 1, 
						&BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);
					total += __InstallPatchByPatternMask("F7 ? 00 00 E0 03", 0, -1, 2,
						&BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);
					total += __InstallPatchByPatternMask("25 00 00 E0 03", 0, -1, 1, 
						&BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? 00 00 E0 03", 0, -1, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_AGE_BIT, 4);

					total_patches += total;
					total = 0;

					// Change INDEX
					total += __InstallPatchByPatternMask("41 81 ? FF FF 1F 00", 0, -1, 3, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 E1 FF FF 1F 00", 3, 25, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMaskEx("81 E5 FF FF 1F 00", 0, -1, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4, { 4, 8 });
					total += __InstallPatchByPatternMaskEx("81 E6 FF FF 1F 00", 3, 24, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4, { 7, 8 });
					total += __InstallPatchByPatternMask("81 E7 FF FF 1F 00", 2, 1, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("8B C7 25 FF FF 1F 00 8B D8", 0, -1, 3, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? 48 ? ? 04", 0, -1, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? 48 ? ? 04", 0, -1, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? 44 ? ? 41 ? 00 00 00 00 49 ? ? 04", 0, -1, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 49 ? ? 04", 0, -1, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
					total += __InstallPatchByPatternMask("81 E2 FF FF 1F 00", 0, 2, 2, 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);

					memcpy((void*)(lpRelocator->GetBase() + 0x5279F9), 
						&BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT, 4);
				
					total += 1;
					total_patches += total;
					total = 0;

					uint32_t not_mask = ~BSUntypedPointerHandle_Extended_NG::MASK_INDEX_BIT;

					// Change NOT MASK_INDEX_BIT
					total += __InstallPatchByPatternMask("81 ? 00 00 E0 FF", 0, 56, 2, &not_mask, 4);

					total_patches += total;
					//_MESSAGE("NOT MASK_INDEX_BIT Total %llu", total);

					total = 0;
					not_mask = ~BSUntypedPointerHandle_Extended_NG::MASK_ACTIVE_BIT;

					// Change NOT MASK_ACTIVE_BIT
					total += __InstallPatchByPatternMask("81 ? FF FF FF FB", 0, -1, 2, &not_mask, 4);

					total_patches += total;
					//_MESSAGE("NOT MASK_INDEX_BIT Total %llu", total);

					total = 0;

					// Change UNUSED_BIT_START
					total += __InstallPatchByPatternMask("0F BA E0 1A", 0, 317, 3, 
						&BSUntypedPointerHandle_Extended_NG::UNUSED_BIT_START, 1);
					total += __InstallPatchByPatternMaskEx("0F BA ?? 1A", 0, 72, 3, 
						&BSUntypedPointerHandle_Extended_NG::UNUSED_BIT_START, 1,
						{ 18, 19, 23, 24, 30, 38, 39, 47, 48, 63, 64, 65, 66, 67, 68 });

					total_patches += total;
					total = 0;

					// Change MAX_HANDLE_COUNT
					total += __InstallPatchByPatternMask("81 ? 00 00 20 00", 0, 1, 2, &BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT, 4);
					total += __InstallPatchByPatternMask("BD 00 00 20 00", 0, -1, 1, &BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT, 4);

					total_patches += total;
					//_MESSAGE("MAX_HANDLE_COUNT Total %llu", total);

					total = 0;

					// Change MASK_ACTIVE_BIT
					total += __InstallPatchByPatternMask("F7 ? 00 00 00 04", 0, 3, 2, &BSUntypedPointerHandle_Extended_NG::MASK_ACTIVE_BIT, 4);
#else
					AssertMsg("Refrs extension is disabled by perchik71, it may be in implementation or abandoned.");
#endif /* CKPE_FO4_ENABLED_REFLIMIT */

					////////////////////////////////////////////////////////////////////////////////////////////////////
					//
					// REFUSAL FOR NG, MAINTAINING 64 MILLION LINKS (2^26) IS TOO DIFFICULT AND UNSTABLE, TY FUCKESDA.
					// THE PROBLEM: MULTIPLE INSERTS OF THE FUNCTION AS INLINE, THE INABILITY TO STABLY EDIT THE DECREF
					// CODE, SINCE -1 MAY BE FAR FROM THE CODE SNIPPET.
					// 
					////////////////////////////////////////////////////////////////////////////////////////////////////

					// BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 1;

					//{
					//	ScopeRelocator textSection;

					//	auto addr = (uintptr_t)_RELDATA_RAV(0);
					//	// Preparation, removal of all embedded pieces of code
					//	lpRelocator->PatchNop(addr + 12, 0x7A);
					//	lpRelocator->PatchMovFromRax(addr + 5, _RELDATA_RAV(1));
					//	// Specify the size
					//	memcpy((void*)((uintptr_t)_RELDATA_ADDR(0) + 0x93), &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);

					//	// Modify CreateHandle
					//	addr = (uintptr_t)_RELDATA_ADDR(3);
					//	memcpy((void*)(addr + 0x12F), &BSUntypedPointerHandle_Extended::MAX_HANDLE_COUNT, 4);
					//	uint32_t mask = BSUntypedPointerHandle_Extended::MASK_ACTIVE_BIT | BSUntypedPointerHandle_Extended::MASK_INDEX_BIT;
					//	memcpy((void*)(addr + 0x134), &mask, 4);
					//	/*
					//	*(uint8_t*)(addr + 0x34) = 3;
					//	*(uint8_t*)(addr + 0x2D) = 2;
					//	*(uint8_t*)(addr + 0x21) = 0x39;

					//	*(uint8_t*)(addr + 0x9E) = 3;
					//	*(uint8_t*)(addr + 0x96) = 2;
					//	*(uint8_t*)(addr + 0x8E) = 0x39;

					//	*(uint8_t*)(addr + 0x178) = 3;
					//	*(uint8_t*)(addr + 0x17C) = 2;
					//	*(uint8_t*)(addr + 0x17F) = 0x39;*/
					//}

					//

					// Change DecRef
				//	total += __InstallPatchByPatternMaskCustom("B8 FF FF FF FF F0 0F C1 ? 08 FF C8 A9 FF 03", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xC8\xFF\xF0\x48\x90\x90\x90\x08", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("B8 FF FF FF FF F0 0F C1 ? 38 FF C8", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xC8\xFF\xF0\x48\x90\x90\x90\x38", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("BA FF FF FF FF F0 0F C1 ? 08 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xCA\xFF\xF0\x48\x90\x90\x90\x08", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("BA FF FF FF FF F0 0F C1 ? 38 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xCA\xFF\xF0\x48\x90\x90\x90\x38", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("BE FF FF FF FF F0 0F C1 ? 38 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xCE\xFF\xF0\x48\x90\x90\x90\x38", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("BD FF FF FF FF F0 0F C1 ? 38 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xCD\xFF\xF0\x48\x90\x90\x90\x38", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("BB FF FF FF FF F0 0F C1 ? 08 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xCB\xFF\xF0\x48\x90\x90\x90\x08", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("48 ? ? FF FF FF FF F0 0F C1 ? 38 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 8), 3);
				//			if (*(uint8_t*)(addr + 2) == 0xC0)
				//				memcpy((uint8_t*)addr, "\x48\x83\xC8\xFF\xF0\x48\x90\x90\x90\x38\x90\x90", 12);
				//			else if (*(uint8_t*)(addr + 2) == 0xC7)
				//				memcpy((uint8_t*)addr, "\x48\x83\xCF\xFF\xF0\x48\x90\x90\x90\x38\x90\x90", 12);
				//			else if (*(uint8_t*)(addr + 2) == 0xC6)
				//				memcpy((uint8_t*)addr, "\x48\x83\xCE\xFF\xF0\x48\x90\x90\x90\x38\x90\x90", 12);
				//			else return false;
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("48 ? ? FF FF FF FF F0 0F C1 ? 08 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 8), 3);
				//			if (*(uint8_t*)(addr + 2) == 0xC0)
				//				memcpy((uint8_t*)addr, "\x48\x83\xC8\xFF\xF0\x48\x90\x90\x90\x08\x90\x90", 12);
				//			else if (*(uint8_t*)(addr + 2) == 0xC3)
				//				memcpy((uint8_t*)addr, "\x48\x83\xCB\xFF\xF0\x48\x90\x90\x90\x08\x90\x90", 12);
				//			else if (*(uint8_t*)(addr + 2) == 0xC6)
				//				memcpy((uint8_t*)addr, "\x48\x83\xCE\xFF\xF0\x48\x90\x90\x90\x08\x90\x90", 12);
				//			else return false;
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("? FF FF FF FF F0 ? 0F C1 ? ? ? FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			if ((*(uint8_t*)addr == 0xC7) || (*(uint8_t*)addr == 0xC6))
				//				*(uint8_t*)(addr + 6) += 0x8;
				//			else if ((*(uint8_t*)addr == 0xBF) || (*(uint8_t*)addr == 0xBE))
				//			{
				//				memcpy(buffer_cmd, (uint8_t*)(addr + 7), 4);
				//				if (*(uint8_t*)addr == 0xBF)
				//					memcpy((uint8_t*)addr, "\x49\x83\xCF\xFF\xF0\x4C\x90\x90\x90\x90\x90", 11);
				//				else
				//					memcpy((uint8_t*)addr, "\x49\x83\xCE\xFF\xF0\x4C\x90\x90\x90\x90\x90", 11);
				//				memcpy((uint8_t*)(addr + 6), buffer_cmd, 4);
				//			}
				//			else if (*(uint8_t*)addr == 0xB8)
				//			{
				//				memcpy(buffer_cmd, (uint8_t*)(addr + 7), 5);
				//				memcpy((uint8_t*)addr, "\x48\x83\xC8\xFF\xF0\x49\x90\x90\x90\x90\x90\x90", 12);
				//				memcpy((uint8_t*)(addr + 6), buffer_cmd, 5);
				//			}
				//			else return false;

				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("8B ? F0 ? 0F C1 ? ? FF ?", 0, 36, [](uintptr_t addr) -> bool
				//		{
				//			// mov r??, r??
				//			// lock xadd dword ptr ds:[r?? + 0x??], r??
				//			// FF r??
				//			if (*(uint8_t*)(addr - 1) == 0x41)	// prefix
				//				*(uint8_t*)(addr - 1) = 0x49;
				//			*(uint8_t*)(addr + 3) += 8;
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("8B ? F0 0F C1 ? ? FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			// mov r??, r??
				//			// lock xadd dword ptr ds:[r?? + 0x??], r??
				//			// FF r??
				//			*(uint8_t*)(addr + 2) = 0x48;		// remove lock prefix (idk stable or no)
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("F0 0F C1 ? 38 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)addr = 0x48;				// remove lock prefix (idk stable or no)
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("F0 0F C1 ? 08 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			
				//			
				//			*(uint8_t*)addr = 0x48;				// remove lock prefix (idk stable or no)
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("F0 44 0F C1 ? 38 ? FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 1) = 0x4C;
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("F0 41 0F C1 ? 38 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			if ((*(uint8_t*)(addr - 5) == 0xB8) && (*(uint8_t*)(addr - 1) == 0xFF))
				//				memcpy((uint8_t*)(addr - 5), "\x48\x83\xC8\xFF\x90\x90", 5);
				//			*(uint8_t*)(addr + 1) = 0x49;
				//			return true;
				//		});
				//	
				//	total += __InstallPatchByPatternMaskCustom("F0 41 0F C1 ? 08 FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			if ((*(uint8_t*)(addr - 5) == 0xB8) && (*(uint8_t*)(addr - 1) == 0xFF))
				//				memcpy((uint8_t*)(addr - 5), "\x48\x83\xC8\xFF\x90\x90", 5);
				//			*(uint8_t*)(addr + 1) = 0x49;
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("F0 44 0F C1 ? 08 ? FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 1) = 0x4C;
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("B8 FF FF FF FF F0 0F C1 ? 38 ? ? ? ? ? FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			memcpy(buffer_cmd, (uint8_t*)(addr + 6), 3);
				//			memcpy((uint8_t*)addr, "\x48\x83\xC8\xFF\xF0\x48\x90\x90\x90\x38", 10);
				//			memcpy((uint8_t*)(addr + 6), buffer_cmd, 3);
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("F0 45 0F C1 ? 38 ? FF ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 1) = 0x4D;
				//			return true;
				//		});

				//	lpRelocator->Patch(0x83BC03, { 0x4D });
				//	total++;

				//	total_patches += total;
				//	//_MESSAGE("DecRef Total %llu", total);

				//	total = 0;

				//	// Change AGE
				//	total += __InstallPatchByPatternMask("A9 00 00 E0 03", 0, -1, 1, &BSUntypedPointerHandle_Extended::MASK_AGE_BIT, 4);
				//	total += __InstallPatchByPatternMask("F7 ? 00 00 E0 03", 0, -1, 2, &BSUntypedPointerHandle_Extended::MASK_AGE_BIT, 4);
				//	total += __InstallPatchByPatternMask("25 00 00 E0 03", 0, -1, 1, &BSUntypedPointerHandle_Extended::MASK_AGE_BIT, 4);
				//	total += __InstallPatchByPatternMask("81 ? 00 00 E0 03", 0, -1, 2, &BSUntypedPointerHandle_Extended::MASK_AGE_BIT, 4);

				//	total_patches += total;
				//	//_MESSAGE("AGE Total %llu", total);

				//	total = 0;

				//	// Change INDEX
				//	total += __InstallPatchByPatternMask("41 81 ? FF FF 1F 00", 0, -1, 3, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMask("81 E1 FF FF 1F 00", 3, 25, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMaskEx("81 E5 FF FF 1F 00", 0, -1, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4, { 4, 8 });
				//	total += __InstallPatchByPatternMaskEx("81 E6 FF FF 1F 00", 3, 24, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4, { 7, 8 });
				//	total += __InstallPatchByPatternMask("81 E7 FF FF 1F 00", 2, 1, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMask("8B C7 25 FF FF 1F 00 8B D8", 0, -1, 3, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? 48 ? ? 04", 0, -1, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? 48 ? ? 04", 0, -1, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? 44 ? ? 41 ? 00 00 00 00 49 ? ? 04", 0, -1, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMask("81 ? FF FF 1F 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 49 ? ? 04", 0, -1, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += __InstallPatchByPatternMask("81 E2 FF FF 1F 00", 0, 2, 2, &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	
				//	memcpy((void*)(lpRelocator->GetBase() + 0x5279F9), &BSUntypedPointerHandle_Extended::MASK_INDEX_BIT, 4);
				//	total += 1;

				//	total_patches += total;
				//	//_MESSAGE("INDEX Total %llu", total);

				//	total = 0;
				//	uint32_t not_mask = ~BSUntypedPointerHandle_Extended::MASK_INDEX_BIT;

				//	// Change NOT MASK_INDEX_BIT
				//	total += __InstallPatchByPatternMask("81 ? 00 00 E0 FF", 0, 56, 2, &not_mask, 4);

				//	total_patches += total;
				//	//_MESSAGE("NOT MASK_INDEX_BIT Total %llu", total);
	
				//	total = 0;
				//	not_mask = ~BSUntypedPointerHandle_Extended::MASK_ACTIVE_BIT;

				//	// Change NOT MASK_ACTIVE_BIT
				//	total += __InstallPatchByPatternMask("81 ? FF FF FF FB", 0, -1, 2, &not_mask, 4);

				//	total_patches += total;
				//	//_MESSAGE("NOT MASK_INDEX_BIT Total %llu", total);

				//	total = 0;

				//	// Change UNUSED_BIT_START
				//	total += __InstallPatchByPatternMask("0F BA E0 1A", 0, 317, 3, &BSUntypedPointerHandle_Extended::UNUSED_BIT_START, 1);
				//	total += __InstallPatchByPatternMaskEx("0F BA ?? 1A", 0, 72, 3, &BSUntypedPointerHandle_Extended::UNUSED_BIT_START, 1,
				//		{ 18, 19, 23, 24, 30, 38, 39, 47, 48, 63, 64, 65, 66, 67, 68 });

				//	total_patches += total;
				//	//_MESSAGE("UNUSED_BIT_START Total %llu", total);

				//	total = 0;
				//	// Need to fix it, in order to apply it to the entire register, will have to capture what is not needed...	

				//	/*total += __InstallPatchByPatternMaskCustom("41 ? FF FF FF FF", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			if (*(uint8_t*)(addr - 1) == 0xC7) return false;
				//			auto reg = *(uint8_t*)(addr + 1);
				//			memcpy((uint8_t*)addr, "\x49\x83\x90\xFF\x90\x90", 6);
				//			*(uint8_t*)(addr + 2) = reg + 0x10;
				//			return true;
				//		});*/

				//	lpRelocator->Patch(0xD46793, { 0x48, 0x83, 0xCF, 0xFF });
				//
				//	total_patches += total + 1;
				//	//_MESSAGE("FIXED_FFFFFFFF Total %llu", total);
				//	
				//	total = 0;

				//	// Change REFR test handle index
				///*	total += __InstallPatchByPatternMaskCustom("8B ? 38 C1 ? 0B 41 3B ?", 0, -1, [](uintptr_t addr) -> bool
				//		{ 
				//			*(uint8_t*)(addr + 2) = (uint8_t)0x39;
				//			*(uint8_t*)(addr + 5) = (uint8_t)0x3;
				//			return true;
				//		});
				//	total += __InstallPatchByPatternMaskCustom("8B ? 38 C1 ? 0B 3B ?", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 2) = (uint8_t)0x39;
				//			*(uint8_t*)(addr + 5) = (uint8_t)0x3;
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("? ? 08 C1 ? 0B 3B", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 2) = (uint8_t)0x9;
				//			*(uint8_t*)(addr + 5) = (uint8_t)0x3;
				//			return true;
				//		});

				//	total += __InstallPatchByPatternMaskCustom("? ? 08 C1 ? 0B 41 3B", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 2) = (uint8_t)0x9;
				//			*(uint8_t*)(addr + 5) = (uint8_t)0x3;
				//			return true;
				//		});
				//	
				//	total += __InstallPatchByPatternMaskCustom("8B ? 38 8B ? C1 ? 0B", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 2) = (uint8_t)0x9;
				//			*(uint8_t*)(addr + 7) = (uint8_t)0x3;
				//			return true;
				//		});
				//	
				//	total += __InstallPatchByPatternMaskCustom("? ? 38 C1 ? 0B 41 3B", 0, -1, [](uintptr_t addr) -> bool
				//		{
				//			*(uint8_t*)(addr + 2) = (uint8_t)0x9;
				//			*(uint8_t*)(addr + 5) = (uint8_t)0x3;
				//			return true;
				//		});*/

				//	total_patches += total;
				//	//_MESSAGE("REFR_TEST Total %llu", total);

				//	total = 0;
	
				//	// Change MAX_HANDLE_COUNT
				//	total += __InstallPatchByPatternMask("81 ? 00 00 20 00", 0, 1, 2, &BSUntypedPointerHandle_Extended::MAX_HANDLE_COUNT, 4);
				//	total += __InstallPatchByPatternMask("BD 00 00 20 00", 0, -1, 1, &BSUntypedPointerHandle_Extended::MAX_HANDLE_COUNT, 4);
				//	
				//	total_patches += total;
				//	//_MESSAGE("MAX_HANDLE_COUNT Total %llu", total);

				//	total = 0;

				//	// Change MASK_ACTIVE_BIT
				//	total += __InstallPatchByPatternMask("F7 ? 00 00 00 04", 0, 3, 2, &BSUntypedPointerHandle_Extended::MASK_ACTIVE_BIT, 4);

				//	total_patches += total;
				//	//_MESSAGE("MASK_ACTIVE_BIT Total %llu", total);

				//	/*std::vector<uintptr_t> pp;
				//	for (auto pattern : storage_check)
				//		for (auto find_pattern : storage)
				//			if (((pattern - 40) < find_pattern) && ((pattern + 40) > find_pattern))
				//				pp.push_back(pattern);

				//	for (auto pattern : storage_check)
				//		if (std::find(pp.begin(), pp.end(), pattern) == std::end(pp))
				//			_CONSOLE("%llx", pattern);*/

					auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - timerStart).count();
					_CONSOLE("CreationKitPlatformExtended::Experimental::BSPointerHandle: %llu patches applied in %llums", total_patches, duration);
				}
				else
				{
					BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 0;

					{
						ScopeRelocator textSection;

						auto addr = (uintptr_t)_RELDATA_RAV(0);
						// Preparation, removal of all embedded pieces of code
						lpRelocator->PatchNop(addr + 12, 0x7A);
						lpRelocator->PatchMovFromRax(addr + 5, _RELDATA_RAV(1));	
					}

					lpRelocator->DetourCall(_RELDATA_RAV(0),
						(uintptr_t)&BSPointerHandleManager_Original::InitSDM);
					lpRelocator->DetourCall(_RELDATA_RAV(2),
						(uintptr_t)&BSPointerHandleManager_Original::KillSDM);
				}

				return true;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (!IsVersionValid(lpRelocationDatabaseItem))
					return false;
				
				auto Extremly = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (Extremly)
					_CONSOLE("[WARNING] An extended set of refs has been included. You use it at your own risk.");

				if (verPatch == 1)
					return Install_163(lpRelocator, lpRelocationDatabaseItem, Extremly);
				else if (verPatch == 2)
					return Install_980(lpRelocator, lpRelocationDatabaseItem, Extremly);
				
				return false;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			uint32_t ReplaceBSPointerHandleAndManagerPatch::CheckEx(uintptr_t unused, uintptr_t refr)
			{
				if (!refr)
					return 0;

				return ((TESObjectREFR_Extremly*)refr)->GetHandleEntryIndex();
			}

			uint32_t ReplaceBSPointerHandleAndManagerPatch::Check(uintptr_t unused, uintptr_t refr)
			{
				if (!refr)
					return 0;

				return ((TESObjectREFR_Original*)refr)->GetHandleEntryIndex();
			}

			void ReplaceBSPointerHandleAndManagerPatch::IncRefPatch()
			{
				auto Sec = GlobalEnginePtr->GetSection(SECTION_TEXT);
				auto SignIncRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base,
					"4C 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? BA 3B 00 00 00 E8");
				size_t patched = 0, need_p = 0;
				uint8_t szbuff[64];

				for (auto sign : SignIncRef)
				{
					auto start_r = sign + 24;

					if ((*((uint8_t*)start_r) == 0xF0)) // lock
					{
						if (*((uint8_t*)(start_r + 1)) == 0xFF) // inc
						{
							memcpy_s(szbuff, 3, (uint8_t*)start_r + 1, 3);

							memset((uint8_t*)start_r, 0x90, 0xB);
							((uint8_t*)start_r)[0] = 0xF0;
							((uint8_t*)start_r)[1] = 0x48;
							memcpy_s((uint8_t*)(start_r + 2), 3, szbuff, 3);

							((uint8_t*)start_r)[0xB] = 0xEB;	// jmp	(skip test)

							patched++;
#if FALLOUT4_DEVELOPER_MODE
							memset((uint8_t*)sign, 0x90, 7);
#endif
						}
						else if ((*((uint8_t*)(start_r + 1)) == 0x41) && (*((uint8_t*)(start_r + 2)) == 0xFF)) // inc r8 - r15
						{
							((uint8_t*)start_r)[1] = 0x49;
							((uint8_t*)start_r)[5] = 0x49;

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

			void ReplaceBSPointerHandleAndManagerPatch::DecRefPatch()
			{
				auto IsJump = [](uintptr_t off) -> bool {
					return ((*((uint8_t*)(off)) == 0x75) || (*((uint8_t*)(off)) == 0x74) ||
						((*((uint8_t*)(off)) == 0x0F) && ((*((uint8_t*)(off + 1)) == 0x85) || (*((uint8_t*)(off + 1)) == 0x84))));
				};

				size_t patched = 0;
				uint8_t szbuff[64];
				uint8_t* src = nullptr;
				int8_t jump_to = 0;
				int8_t jump_from = 0;

				auto refmask = (DWORD)EditorAPI::BSHandleRefObject_64_Extremly::REF_COUNT_MASK;
				auto Sec = GlobalEnginePtr->GetSection(SECTION_TEXT);
				auto SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "83 ? FF F0 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x29;

					patched_var1:

						src = (uint8_t*)(sign - jump_from);

						memcpy_s(szbuff, 64, (uint8_t*)sign, 0xF);
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

						src = (uint8_t*)(sign + 3);
						src[0] = 0xEB;
						src[1] = -(jump_from + 5);

						patched++;
					}
					else if (*((uint8_t*)(sign - 0x1E)) == 0x77)
					{
						jump_from = 0x1E;
						jump_to = 0x2D;

						goto patched_var1;
					}
					else if ((*((uint8_t*)(sign - 0x1B)) == 0x77) && (*((uint8_t*)(sign - 1)) == 0x48))
					{
						jump_from = 0x1B;
						jump_to = 0x2A;

						goto patched_var1;
					}
					else if (*((uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x27;

						goto patched_var1;
					}
					else if (*((uint8_t*)(sign - 0x19)) == 0x77)
					{
						jump_from = 0x19;
						jump_to = 0x28;

						goto patched_var1;
					}
				}

				SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "83 ? FF F0 0F ? ? ? FF ? F7 ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

					patched_var2:

						src = (uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (uint8_t*)sign, 0x10);
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

						src = (uint8_t*)(sign + 3);
						src[0] = 0xEB;
						src[1] = -(jump_from + 5);

						patched++;
					}
					else if ((*((uint8_t*)(sign - 0x1B)) == 0x77) && (*((uint8_t*)(sign - 1)) == 0x48))
					{
						jump_from = 0x1B;
						jump_to = 0x2B;

						goto patched_var2;
					}
					else if (*((uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x28;

						goto patched_var2;
					}
				}

				SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "83 ? FF F0 ? 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

						src = (uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (uint8_t*)sign, 0x10);
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

						src = (uint8_t*)(sign + 3);
						src[0] = 0xEB;
						src[1] = -(jump_from + 5);

						patched++;
					}
				}

				SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "8B ? F0 0F ? ? ? FF ? ? FF 03 00 00");

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
					if (*((uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x28;

					patched_var3:

						src = (uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (uint8_t*)sign, 0xE);
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

						src = (uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					if ((*((uint8_t*)(sign - 0x1B)) == 0x77) && (*((uint8_t*)(sign - 1)) == 0x41))
					{
						jump_from = 0x1B;
						jump_to = 0x29;

						goto patched_var3;
					}
					if ((*((uint8_t*)(sign - 0x1D)) == 0x77) && (*((uint8_t*)(sign - 3)) == 0x83))
					{
						jump_from = 0x1D;
						jump_to = 0x2B;

						goto patched_var3;
					}
					else if ((*((uint8_t*)(sign - 0x1F)) == 0x77) && (*((uint8_t*)(sign - 4)) == 0x83))
					{
						jump_from = 0x1F;
						jump_to = 0x2D;

						goto patched_var3;
					}
					else if (*((uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x26;

						goto patched_var3;
					}
					else if (*((uint8_t*)(sign - 0x19)) == 0x77)
					{
						jump_from = 0x19;
						jump_to = 0x27;

						goto patched_var3;
					}
				}

				SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "41 8B ? F0 ? 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

					patched_var5:

						src = (uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (uint8_t*)sign, 0x10);
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

						src = (uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					else if ((*((uint8_t*)(sign - 0x1E)) == 0x77) && (*((uint8_t*)(sign - 3)) == 0x83))
					{
						jump_from = 0x1E;
						jump_to = 0x2E;

						goto patched_var5;
					}
				}

				SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "F0 ? 0F ? ? ? 41 FF ? 41 F7 ? FF 03 00 00");

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
					if (*((uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x2A;

					patched_var6:

						src = (uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (uint8_t*)sign, 0x10);
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

						src = (uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					else if ((*((uint8_t*)(sign - 0x1E)) == 0x77) && (*((uint8_t*)(sign - 3)) == 0x83))
					{
						jump_from = 0x1E;
						jump_to = 0x2E;

						goto patched_var6;
					}
				}

				SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "F0 0F ? ? ? FF ? F7 ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((uint8_t*)(sign - 0x1A)) == 0x77)
					{
						jump_from = 0x1A;
						jump_to = 0x27;

					patched_var7:

						src = (uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (uint8_t*)sign, 0xD);
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

						src = (uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
					else if (*((uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x25;

						goto patched_var7;
					}
				}

				SignDecRef = voltek::find_patterns(Sec.base, Sec.end - Sec.base, "8B ? F0 ? 0F ? ? ? FF ? ? FF 03 00 00");

				for (auto sign : SignDecRef)
				{
					if (*((uint8_t*)(sign - 0x18)) == 0x77)
					{
						jump_from = 0x18;
						jump_to = 0x27;

						src = (uint8_t*)(sign - jump_from);
						memcpy_s(szbuff, 64, (uint8_t*)sign, 0xF);
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

						src = (uint8_t*)(sign + 2);
						src[0] = 0xEB;
						src[1] = -(jump_from + 4);

						patched++;
					}
				}
				
				auto sign = pointer_ReplaceBSPointerHandleAndManager_code1;
				jump_from = 0x1A;
				jump_to = 0x30;

				src = (uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (uint8_t*)sign, 0x16);
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

				src = (uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				patched++;

				sign = pointer_ReplaceBSPointerHandleAndManager_code2;
				jump_from = 0x1A;
				jump_to = 0x28;

				src = (uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (uint8_t*)sign, 0xE);
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

				src = (uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				patched++;

				sign = pointer_ReplaceBSPointerHandleAndManager_code3;
				jump_from = 0x1A;
				jump_to = 0x2B;

				src = (uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (uint8_t*)sign, 0x11);
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

				src = (uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				sign = pointer_ReplaceBSPointerHandleAndManager_code4;
				jump_from = 0x1A;
				jump_to = 0x2B;

				src = (uint8_t*)(sign - jump_from);
				memcpy_s(szbuff, 64, (uint8_t*)sign, 0x11);
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

				src = (uint8_t*)(sign + 2);
				src[0] = 0xEB;
				src[1] = -(jump_from + 4);

				patched += 2;

				_MESSAGE("BSHandleRefObject::DecRef (Patched: %d)", patched);
			}
		}
	}
}