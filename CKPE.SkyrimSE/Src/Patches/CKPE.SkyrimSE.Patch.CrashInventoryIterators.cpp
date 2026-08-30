// Copyright © 2023-2025 aka CKPE team. All rights reserved.
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
			using TCrashInventoryIterators_sub1 = void(std::int64_t, void*);
			using TCrashInventoryIterators_sub2 = bool(void*, void*);
			using TCrashInventoryIterators_sub3 = std::int64_t(EditorAPI::Forms::TESObjectREFR_Original**, std::int64_t);
			using TCrashInventoryIterators_sub4 = void(void*);
			using TCrashInventoryIterators_sub5 = std::int32_t(std::int64_t, EditorAPI::Forms::TESObjectREFR_Original**);

			static std::function<TCrashInventoryIterators_sub1> CrashInventoryIterators_sub1;
			static std::function<TCrashInventoryIterators_sub1> CrashInventoryIterators_sub2;
			static std::function<TCrashInventoryIterators_sub2> CrashInventoryIterators_sub3;
			static std::function<TCrashInventoryIterators_sub3> CrashInventoryIterators_sub4;
			static std::function<TCrashInventoryIterators_sub4> CrashInventoryIterators_sub5;
			static std::function<TCrashInventoryIterators_sub5> CrashInventoryIterators_sub6;
			static std::function<TCrashInventoryIterators_sub5> CrashInventoryIterators_sub7;

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

				CrashInventoryIterators_sub1(List, &currIter);
				CrashInventoryIterators_sub2(List, &endIter);

				while (CrashInventoryIterators_sub3(&currIter, &endIter))
				{
					// Increase refcount via BSHandleRefObject::IncRefCount
					EditorAPI::Forms::TESObjectREFR_Original* refr;

					CrashInventoryIterators_sub4(&refr, currIter.unk1);
					temporaryCellRefList.push_back(refr);

					// Move to next element
					CrashInventoryIterators_sub5(&currIter);
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

				CrashInventoryIterators_sub1(List, &currIter);
				CrashInventoryIterators_sub2(List, &endIter);

				while (CrashInventoryIterators_sub3(&currIter, &endIter))
				{
					// Increase refcount via BSHandleRefObject::IncRefCount
					EditorAPI::Forms::TESObjectREFR_Original* refr;

					CrashInventoryIterators_sub4(&refr, currIter.unk1);
					temporaryCellRefList.push_back(reinterpret_cast<EditorAPI::Forms::TESObjectREFR_Extremly*>(refr));

					// Move to next element
					CrashInventoryIterators_sub5(&currIter);
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

			bool CrashInventoryIterators::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashInventoryIterators::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashInventoryIterators::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when cell references are added/removed during initialization, 
				// similar to the broken iterator in InventoryChanges
				//
				
				bool ExtremlyMode = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
				if (ExtremlyMode)
				{
					Relocation(ID{ 704102, 1018944 }).WriteJump(&sub1_Extremly);
					Relocation(ID{ 754965, 942541 }).WriteJump(&sub2_Extremly);
				}
				else
				{
					Relocation(ID{ 704102, 1018944 }).WriteJump(&sub1);
					Relocation(ID{ 754965, 942541 }).WriteJump(&sub2);
				}

				CrashInventoryIterators_sub1 = Relocation<TCrashInventoryIterators_sub1>(ID(633835)).Get();
				CrashInventoryIterators_sub2 = Relocation<TCrashInventoryIterators_sub1>(ID(656861)).Get();
				CrashInventoryIterators_sub3 = Relocation<TCrashInventoryIterators_sub2>(ID(656863)).Get();
				CrashInventoryIterators_sub4 = Relocation<TCrashInventoryIterators_sub3>(ID(575421)).Get();
				CrashInventoryIterators_sub5 = Relocation<TCrashInventoryIterators_sub4>(ID(656865)).Get();
				CrashInventoryIterators_sub6 = Relocation<TCrashInventoryIterators_sub5>(ID(550854)).Get();
				CrashInventoryIterators_sub7 = Relocation<TCrashInventoryIterators_sub5>(ID(234583)).Get();
				
				if (VersionLists::GetEditorVersion() < VersionLists::EDITOR_SKYRIM_SE_1_7_99_0)
				{
					auto Rav1 = Relocation(ID(551095), 0x9D9).Address();
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
				}
				else
				{
					auto Rav1 = Relocation(ID(551095), 0x9DD).Address();
					//
					// Fix for crash after erasing an iterator and dereferencing it in "InventoryChanges" code
					//
					class changeInventoryHook : public Xbyak::CodeGenerator
					{
					public:
						changeInventoryHook(uintptr_t addr) : Xbyak::CodeGenerator()
						{
							// iterator = iterator->next
							mov(rax, ptr[rsp + 0xC8]);
							mov(rax, ptr[rax + 0x8]);
							mov(ptr[rsp + 0xC8], rax);

							// Continue with code that destroys the now-previous iterator
							mov(rax, ptr[rsp + 0x50]);
							jmp(ptr[rip]);
							dq(addr);
						}
					} static inventoryHookInstance(Rav1);

					Detours::DetourJump((std::uintptr_t)Rav1 - 5, (std::uintptr_t)inventoryHookInstance.getCode());
				}

				return true;
			}

			std::int32_t CrashInventoryIterators::sub1(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy(a1);
				std::int32_t status = 1;

				// Unknown init function
				for (auto refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = CrashInventoryIterators_sub6(*(std::int64_t*)a2, &refr);
				}

				return status;
			}

			std::int32_t CrashInventoryIterators::sub2(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy(a1);
				std::int32_t status = 1;

				// Now parse the entire list separately - allow InitItem() to modify the cell's hashmap without invalidating any iterators
				for (auto refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = CrashInventoryIterators_sub7(*(std::int64_t*)a2, &refr);
				}

				return status;
			}

			std::int32_t CrashInventoryIterators::sub1_Extremly(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy_Extremly(a1);
				std::int32_t status = 1;

				// Unknown init function
				for (auto refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = CrashInventoryIterators_sub6(*(std::int64_t*)a2, 
						reinterpret_cast<EditorAPI::Forms::TESObjectREFR_Original**>(&refr));
				}

				return status;
			}

			std::int32_t CrashInventoryIterators::sub2_Extremly(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				auto cellRefList = CreateCellPersistentMapCopy_Extremly(a1);
				std::int32_t status = 1;

				// Now parse the entire list separately - allow InitItem() to modify the cell's hashmap without invalidating any iterators
				for (auto refr : cellRefList)
				{
					if (status != 1)
						break;

					// Automatically decrements ref count
					status = CrashInventoryIterators_sub7(*(__int64*)a2,
						reinterpret_cast<EditorAPI::Forms::TESObjectREFR_Original**>(&refr));
				}

				return status;
			}
		}
	}
}