// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <xbyak.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashInventoryIterators.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_CrashInventoryIterators_sub1 = 0;
			std::uintptr_t pointer_CrashInventoryIterators_sub2 = 0;
			std::uintptr_t pointer_CrashInventoryIterators_sub3 = 0;
			std::uintptr_t pointer_CrashInventoryIterators_sub4 = 0;
			std::uintptr_t pointer_CrashInventoryIterators_sub5 = 0;
			std::uintptr_t pointer_CrashInventoryIterators_sub6 = 0;
			std::uintptr_t pointer_CrashInventoryIterators_sub7 = 0;

			static std::vector<EditorAPI::Forms::TESObjectREFR_Original*> 
				CreateCellPersistentMapCopy(std::int64_t List) noexcept(true)
			{
				// Create a copy of the cell's persistent ref hashmap and increase the ref count for all elements
				std::vector<EditorAPI::Forms::TESObjectREFR_Original*> temporaryCellRefList;

				struct
				{
					std::uintptr_t unk1;
					std::uintptr_t unk2;
					std::uint32_t unk3;
				} currIter, endIter;

				((void(__fastcall*)(std::int64_t, void*))pointer_CrashInventoryIterators_sub1)(List, &currIter);
				((void(__fastcall*)(std::int64_t, void*))pointer_CrashInventoryIterators_sub2)(List, &endIter);

				while (((bool(__fastcall*)(void*, void*))pointer_CrashInventoryIterators_sub3)(&currIter, &endIter))
				{
					// Increase refcount via BSHandleRefObject::IncRefCount
					EditorAPI::Forms::TESObjectREFR_Original* refr;

					((std::int64_t(__fastcall*)(EditorAPI::Forms::TESObjectREFR_Original**, std::int64_t))
						pointer_CrashInventoryIterators_sub4)(&refr, currIter.unk1);
					temporaryCellRefList.push_back(refr);

					// Move to next element
					((void(__fastcall*)(void*))pointer_CrashInventoryIterators_sub5)(&currIter);
				}

				return temporaryCellRefList;
			}

			static std::vector<EditorAPI::Forms::TESObjectREFR_Extremly*> 
				CreateCellPersistentMapCopy_Extremly(std::int64_t List) noexcept(true)
			{
				// Create a copy of the cell's persistent ref hashmap and increase the ref count for all elements
				std::vector<EditorAPI::Forms::TESObjectREFR_Extremly*> temporaryCellRefList;

				struct
				{
					std::uintptr_t unk1;
					std::uintptr_t unk2;
					std::uint32_t unk3;
				} currIter, endIter;

				((void(__fastcall*)(std::int64_t, void*))pointer_CrashInventoryIterators_sub1)(List, &currIter);
				((void(__fastcall*)(std::int64_t, void*))pointer_CrashInventoryIterators_sub2)(List, &endIter);

				while (((bool(__fastcall*)(void*, void*))pointer_CrashInventoryIterators_sub3)(&currIter, &endIter))
				{
					// Increase refcount via BSHandleRefObject::IncRefCount
					EditorAPI::Forms::TESObjectREFR_Extremly* refr;

					((std::int64_t(__fastcall*)(EditorAPI::Forms::TESObjectREFR_Extremly**, std::int64_t))
						pointer_CrashInventoryIterators_sub4)(&refr, currIter.unk1);
					temporaryCellRefList.push_back(refr);

					// Move to next element
					((void(__fastcall*)(void*))pointer_CrashInventoryIterators_sub5)(&currIter);
				}

				return temporaryCellRefList;
			}

			CrashInventoryIterators::CrashInventoryIterators() : Common::Patch()
			{
				SetName("Crash Inventory Iterators");
			}

			bool CrashInventoryIterators::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashInventoryIterators::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashInventoryIterators::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> CrashInventoryIterators::GetDependencies() const noexcept(true)
			{
				return { "Replace BSPointerHandle And Manager" };
			}

			bool CrashInventoryIterators::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashInventoryIterators::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when cell references are added/removed during initialization, 
				// similar to the broken iterator in InventoryChanges
				//

				bool ExtremlyMode = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
				if (ExtremlyMode)
				{
					Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub1_Extremly);
					Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&sub2_Extremly);
				}
				else
				{
					Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub1);
					Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&sub2);
				}

				pointer_CrashInventoryIterators_sub1 = __CKPE_OFFSET(2);
				pointer_CrashInventoryIterators_sub2 = __CKPE_OFFSET(3);
				pointer_CrashInventoryIterators_sub3 = __CKPE_OFFSET(4);
				pointer_CrashInventoryIterators_sub4 = __CKPE_OFFSET(5);
				pointer_CrashInventoryIterators_sub5 = __CKPE_OFFSET(6);
				pointer_CrashInventoryIterators_sub6 = __CKPE_OFFSET(7);
				pointer_CrashInventoryIterators_sub7 = __CKPE_OFFSET(8);

				auto Rav1 = __CKPE_OFFSET(9);
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
				} static inventoryHookInstance(Rav1);

				Detours::DetourJump((std::uintptr_t)Rav1 - 5, (std::uintptr_t)inventoryHookInstance.getCode());

				return true;
			}

			std::int32_t CrashInventoryIterators::sub1(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy(a1);
				std::int32_t status = 1;

				// Unknown init function
				for (EditorAPI::Forms::TESObjectREFR_Original* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((std::int32_t(__fastcall*)(std::int64_t, EditorAPI::Forms::TESObjectREFR_Original**))
						pointer_CrashInventoryIterators_sub6)(*(std::int64_t*)a2, &refr);
				}

				return status;
			}

			std::int32_t CrashInventoryIterators::sub2(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy(a1);
				std::int32_t status = 1;

				// Now parse the entire list separately - allow InitItem() to modify the cell's hashmap without invalidating any iterators
				for (EditorAPI::Forms::TESObjectREFR_Original* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((std::int32_t(__fastcall*)(std::int64_t, EditorAPI::Forms::TESObjectREFR_Original**))
						pointer_CrashInventoryIterators_sub7)(*(std::int64_t*)a2, &refr);
				}

				return status;
			}

			std::int32_t CrashInventoryIterators::sub1_Extremly(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy_Extremly(a1);
				std::int32_t status = 1;

				// Unknown init function
				for (EditorAPI::Forms::TESObjectREFR_Extremly* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((std::int32_t(__fastcall*)(std::int64_t, EditorAPI::Forms::TESObjectREFR_Extremly**))
						pointer_CrashInventoryIterators_sub6)(*(std::int64_t*)a2, &refr);
				}

				return status;
			}

			std::int32_t CrashInventoryIterators::sub2_Extremly(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy_Extremly(a1);
				std::int32_t status = 1;

				// Now parse the entire list separately - allow InitItem() to modify the cell's hashmap without invalidating any iterators
				for (EditorAPI::Forms::TESObjectREFR_Extremly* refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = ((std::int32_t(__fastcall*)(std::int64_t, EditorAPI::Forms::TESObjectREFR_Extremly**))
						pointer_CrashInventoryIterators_sub7)(*(__int64*)a2, &refr);
				}

				return status;
			}
		}
	}
}