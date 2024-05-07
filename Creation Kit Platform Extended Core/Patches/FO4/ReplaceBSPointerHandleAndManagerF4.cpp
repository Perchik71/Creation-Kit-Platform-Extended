// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/FO4/BSPointerHandleManager.h"
#include "ReplaceBSPointerHandleAndManagerF4.h"

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
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto Extremly = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					if (Extremly)
					{
						_CONSOLE("[WARNING] An extended set of refs has been included. You use it at your own risk.");

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
				else if (verPatch == 2)
				{
					BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 0;

					{
						ScopeRelocator textSection;

						// Preparation, removal of all embedded pieces of code
						lpRelocator->PatchNop((uintptr_t)lpRelocationDatabaseItem->At(0) + 12, 0x81);
						lpRelocator->PatchMovFromRax((uintptr_t)lpRelocationDatabaseItem->At(0) + 5, lpRelocationDatabaseItem->At(1));

						// Stub out the rest of the functions which shouldn't ever be called now
						//lpRelocator->Patch(lpRelocationDatabaseItem->At(8), { 0xCC });	// BSUntypedPointerHandle::Set			
					}

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0),
						(uintptr_t)&BSPointerHandleManager_Original::InitSDM);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2),
						(uintptr_t)&BSPointerHandleManager_Original::KillSDM);

					//lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3),
					//	(uintptr_t)&BSPointerHandleManagerInterface_Original::GetCurrentHandleOrCreate); 
					//lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4),
					//	(uintptr_t)&BSPointerHandleManagerInterface_Original::Destroy1);
					//lpRelocator->DetourJump(lpRelocationDatabaseItem->At(5),
					//	(uintptr_t)&BSPointerHandleManagerInterface_Original::Destroy2);

					//{
					//	ScopeRelocator textSection;

					//	//
					//	// Deleting the code, restoring the function
					//	//
					//	
					//	auto restoring_destroy1 = [&lpRelocator](uintptr_t rva, uint32_t removal_size)
					//	{
					//		lpRelocator->PatchNop(rva, removal_size);
					//		lpRelocator->Patch(rva, { 0x48, 0x89, 0xC1 });
					//		lpRelocator->DetourCall(rva + 3, (uintptr_t)&BSPointerHandleManagerInterface_Original::Destroy1);
					//	};

					//	restoring_destroy1(lpRelocationDatabaseItem->At(6), 0xF2);
					//	restoring_destroy1(lpRelocationDatabaseItem->At(7), 0xFE);
					//	restoring_destroy1(lpRelocationDatabaseItem->At(8), 0xFE);
					//	restoring_destroy1(lpRelocationDatabaseItem->At(9), 0xFE);
					//	restoring_destroy1(lpRelocationDatabaseItem->At(10), 0xEF);
					//	restoring_destroy1(lpRelocationDatabaseItem->At(11), 0xFE);
					//	restoring_destroy1(lpRelocationDatabaseItem->At(12), 0xEF);

					//	auto restoring_destroy2 = [&lpRelocator](uintptr_t rva, uint8_t off_rsp, uint32_t removal_size)
					//	{
					//		lpRelocator->PatchNop(rva, removal_size);
					//		lpRelocator->Patch(rva, { 0x48, 0x8D, 0x4C, 0x24, off_rsp });
					//		lpRelocator->DetourCall(rva + 5, (uintptr_t)&BSPointerHandleManagerInterface_Original::Destroy2);
					//	};
					//	
					//	restoring_destroy2(lpRelocationDatabaseItem->At(13), 0x30, 0x10B);
					//}
			
					//lpRelocator->DetourJump(lpRelocationDatabaseItem->At(6),
					//	(uintptr_t)&BSPointerHandleManagerInterface_Original::GetSmartPointer1);
					//lpRelocator->DetourJump(lpRelocationDatabaseItem->At(7),
					//	(uintptr_t)&BSPointerHandleManagerInterface_Original::GetSmartPointer2);


					return true;
				}
				
				return false;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
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