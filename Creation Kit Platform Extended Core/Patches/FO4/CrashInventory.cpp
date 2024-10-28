// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashInventory.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t ptr_CrashInventoryPatch_sub0 = 0;

			CrashInventoryPatch::CrashInventoryPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashInventoryPatch::HasOption() const
			{
				return false;
			}

			bool CrashInventoryPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashInventoryPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashInventoryPatch::GetName() const
			{
				return "Crash Inventory";
			}

			bool CrashInventoryPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashInventoryPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashInventoryPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool CrashInventoryPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// movzx edx, byte ptr ds:[rdi+0x12] 
					// test rbp, rbp
					// je creationkit_f4_1_10_162.1407414DA
					// lea rcx, qword ptr ss:[rbp+0x18]
					// call creationkit_f4_1_10_162.14200D0C0
					// lea rcx, qword ptr ss:[rbp+0x8]
					// call creationkit_f4_1_10_162.14007B300
					// mov rsi, rax
					// nop
					// nop
					// lea rcx, qword ptr ss:[rbp+0x18]

					//lpRelocator->Patch(_RELDATA_RAV(0), { 0x0F, 0xB6, 0x57, 0x12, 0x48, 0x85, 0xED, 0x0F, 0x84,
					//	0x7F, 0x00, 0x00, 0x00, 0x48, 0x8D, 0x4D, 0x18, 0xE8, 0x5C, 0xBC, 0x8C, 0x01, 0x48, 0x8D,
					//	0x4D, 0x08, 0xE8, 0x93, 0x9E, 0x93, 0xFF, 0x48, 0x89, 0xC6, 0x90, 0x90, 0x48, 0x8D, 0x4D, 
					//	0x18 });

					*(uintptr_t*)&ptr_CrashInventoryPatch_sub0 =
						voltek::detours_function_class_jump(_RELDATA_ADDR(0) - 0x7E, (uintptr_t)&sub1);

					auto rva = (uintptr_t)_RELDATA_RAV(1);
					lpRelocator->PatchNop(rva, 0x7B);
					lpRelocator->PatchNop(rva + 0xE0, 0x7B);

					return true;
				}
				else if (lpRelocationDatabaseItem->Version() == 2)
				{
					// movzx edx, byte ptr ds:[rdi+0x12]
					// test r14, r14
					// je <some>
					// lea rcx, qword ptr ds:[r14+0x18]
					// call creationkit_f4_1_10_982_3.141FD7770
					// lea rcx, qword ptr ds:[r14+0x8]
					// call creationkit_f4_1_10_982_3.1407A8980
					// mov rsi, rax
					// nop
					// nop
					// lea rcx, qword ptr ds:[r14+0x18]

					/*lpRelocator->Patch(_RELDATA_RAV(0), { 0x0F, 0xB6, 0x57, 0x12, 0x4D, 0x85, 0xF6, 0x0F,
						0x84, 0x7F, 0x00, 0x00, 0x00, 0x49, 0x8D, 0x4E, 0x18, 0xE8, 0x6D, 0xA4, 0x82, 0x01,
						0x49, 0x8D, 0x4E, 0x08, 0xE8, 0x74, 0xB6, 0xFF, 0xFF, 0x48, 0x89, 0xC6, 0x90, 0x90,
						0x49, 0x8D, 0x4E, 0x18 });*/

					*(uintptr_t*)&ptr_CrashInventoryPatch_sub0 =
						voltek::detours_function_class_jump(_RELDATA_ADDR(0) - 0xBD, (uintptr_t)&sub1);

					auto rva = (uintptr_t)_RELDATA_RAV(1);
					lpRelocator->PatchNop(rva, 0x7F);
					lpRelocator->PatchNop(rva + 0xF0, 0x7F);

					return true;
				}

				return false;
			}

			bool CrashInventoryPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void* CrashInventoryPatch::sub1(void* unk)
			{
				__try
				{
					if (!unk || 
						!(*(uint8_t**)((uintptr_t)unk + 0x8)) || 
						!(*(uint8_t**)((uintptr_t)unk + 0x18)))
						return nullptr;
					
					return fastCall<void*>(ptr_CrashInventoryPatch_sub0, unk);
				}
				__except (1)
				{
					return nullptr;
				}
			}
		}
	}
}