// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashInventoryIterators.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashInventoryIterators_sub1 = 0;
			uintptr_t pointer_CrashInventoryIterators_sub2 = 0;
			uintptr_t pointer_CrashInventoryIterators_sub3 = 0;
			uintptr_t pointer_CrashInventoryIterators_sub4 = 0;
			uintptr_t pointer_CrashInventoryIterators_sub5 = 0;
			uintptr_t pointer_CrashInventoryIterators_sub6 = 0;
			uintptr_t pointer_CrashInventoryIterators_sub7 = 0;

			CrashInventoryIteratorsPatch::CrashInventoryIteratorsPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashInventoryIteratorsPatch::HasOption() const
			{
				return false;
			}

			bool CrashInventoryIteratorsPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashInventoryIteratorsPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashInventoryIteratorsPatch::GetName() const
			{
				return "Crash Inventory Iterators";
			}

			bool CrashInventoryIteratorsPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> CrashInventoryIteratorsPatch::GetDependencies() const
			{
				return { "Replace BSPointerHandle And Manager" };
			}

			bool CrashInventoryIteratorsPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool CrashInventoryIteratorsPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when cell references are added/removed during initialization, 
					// similar to the broken iterator in InventoryChanges
					//

					bool ExtremlyMode = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
					if (ExtremlyMode)
					{
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub1_Extremly);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub2_Extremly);
					}
					else
					{
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub1);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub2);
					}

					pointer_CrashInventoryIterators_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_CrashInventoryIterators_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					pointer_CrashInventoryIterators_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));
					pointer_CrashInventoryIterators_sub4 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));
					pointer_CrashInventoryIterators_sub5 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6));
					pointer_CrashInventoryIterators_sub6 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(7));
					pointer_CrashInventoryIterators_sub7 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(8));


					auto Rav1 = lpRelocationDatabaseItem->At(9);
					//
					// Fix for crash after erasing an iterator and dereferencing it in "InventoryChanges" code
					//
					class changeInventoryHook : public Xbyak::CodeGenerator
					{
					public:
						changeInventoryHook(uintptr_t addr) : Xbyak::CodeGenerator()
						{
							// iterator = iterator->next
							mov(rax, ptr[rsp + 0xD0]);
							mov(rax, ptr[rax + 0x8]);
							mov(ptr[rsp + 0xD0], rax);

							// Continue with code that destroys the now-previous iterator
							mov(rax, ptr[rsp + 0x50]);
							jmp(ptr[rip]);
							dq(addr);
						}
					} static inventoryHookInstance(lpRelocator->Rav2Off(Rav1));

					lpRelocator->DetourJump((uintptr_t)Rav1 - 5, (uintptr_t)inventoryHookInstance.getCode());

					return true;
				}

				return false;
			}

			bool CrashInventoryIteratorsPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			int CrashInventoryIteratorsPatch::sub1(__int64 a1, __int64 a2)
			{
				auto cellRefList = CreateCellPersistentMapCopy(a1);
				int status = 1;

				// Unknown init function
				for (TESObjectREFR_Original* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((int(__fastcall*)(__int64, TESObjectREFR_Original**))pointer_CrashInventoryIterators_sub6)(*(__int64*)a2, &refr);
				}

				return status;
			}

			int CrashInventoryIteratorsPatch::sub2(__int64 a1, __int64 a2)
			{
				auto cellRefList = CreateCellPersistentMapCopy(a1);
				int status = 1;

				// Now parse the entire list separately - allow InitItem() to modify the cell's hashmap without invalidating any iterators
				for (TESObjectREFR_Original* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((int(__fastcall*)(__int64, TESObjectREFR_Original**))pointer_CrashInventoryIterators_sub7)(*(__int64*)a2, &refr);
				}

				return status;
			}

			int CrashInventoryIteratorsPatch::sub1_Extremly(__int64 a1, __int64 a2)
			{
				auto cellRefList = CreateCellPersistentMapCopy_Extremly(a1);
				int status = 1;

				// Unknown init function
				for (TESObjectREFR_Extremly* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((int(__fastcall*)(__int64, TESObjectREFR_Extremly**))pointer_CrashInventoryIterators_sub6)(*(__int64*)a2, &refr);
				}

				return status;
			}

			int CrashInventoryIteratorsPatch::sub2_Extremly(__int64 a1, __int64 a2)
			{
				auto cellRefList = CreateCellPersistentMapCopy_Extremly(a1);
				int status = 1;

				// Now parse the entire list separately - allow InitItem() to modify the cell's hashmap without invalidating any iterators
				for (TESObjectREFR_Extremly* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((int(__fastcall*)(__int64, TESObjectREFR_Extremly**))pointer_CrashInventoryIterators_sub7)(*(__int64*)a2, &refr);
				}

				return status;
			}

			Array<TESObjectREFR_Original*> CrashInventoryIteratorsPatch::CreateCellPersistentMapCopy(__int64 List)
			{
				// Create a copy of the cell's persistent ref hashmap and increase the ref count for all elements
				Array<TESObjectREFR_Original*> temporaryCellRefList;

				struct
				{
					uintptr_t unk1;
					uintptr_t unk2;
					uint32_t unk3;
				} currIter, endIter;

				((void(__fastcall*)(__int64, void*))pointer_CrashInventoryIterators_sub1)(List, &currIter);
				((void(__fastcall*)(__int64, void*))pointer_CrashInventoryIterators_sub2)(List, &endIter);

				while (((bool(__fastcall*)(void*, void*))pointer_CrashInventoryIterators_sub3)(&currIter, &endIter))
				{
					// Increase refcount via BSHandleRefObject::IncRefCount
					TESObjectREFR_Original* refr;

					((__int64(__fastcall*)(TESObjectREFR_Original**, __int64))pointer_CrashInventoryIterators_sub4)(&refr, currIter.unk1);
					temporaryCellRefList.push_back(refr);

					// Move to next element
					((void(__fastcall*)(void*))pointer_CrashInventoryIterators_sub5)(&currIter);
				}

				return temporaryCellRefList;
			}

			Array<TESObjectREFR_Extremly*> CrashInventoryIteratorsPatch::CreateCellPersistentMapCopy_Extremly(__int64 List)
			{
				// Create a copy of the cell's persistent ref hashmap and increase the ref count for all elements
				Array<TESObjectREFR_Extremly*> temporaryCellRefList;

				struct
				{
					uintptr_t unk1;
					uintptr_t unk2;
					uint32_t unk3;
				} currIter, endIter;

				((void(__fastcall*)(__int64, void*))pointer_CrashInventoryIterators_sub1)(List, &currIter);
				((void(__fastcall*)(__int64, void*))pointer_CrashInventoryIterators_sub2)(List, &endIter);

				while (((bool(__fastcall*)(void*, void*))pointer_CrashInventoryIterators_sub3)(&currIter, &endIter))
				{
					// Increase refcount via BSHandleRefObject::IncRefCount
					TESObjectREFR_Extremly* refr;

					((__int64(__fastcall*)(TESObjectREFR_Extremly**, __int64))pointer_CrashInventoryIterators_sub4)(&refr, currIter.unk1);
					temporaryCellRefList.push_back(refr);

					// Move to next element
					((void(__fastcall*)(void*))pointer_CrashInventoryIterators_sub5)(&currIter);
				}

				return temporaryCellRefList;
			}
		}
	}
}