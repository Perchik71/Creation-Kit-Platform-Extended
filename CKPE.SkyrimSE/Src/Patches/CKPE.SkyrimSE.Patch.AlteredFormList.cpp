// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <unordered_set>
#include <unordered_map>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.AlteredFormList.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			inline static void* (*OldAlteredFormList_Create)(EditorAPI::Forms::TESForm::Array*, std::uint32_t);
			inline static void (*OldAlteredFormList_RemoveAllEntries)(EditorAPI::Forms::TESForm::Array*, bool);
			inline static void (*OldAlteredFormList_Insert)(EditorAPI::Forms::TESForm::Array*, EditorAPI::Forms::TESForm*&);
			inline static void (*OldAlteredFormList_RemoveEntry)(EditorAPI::Forms::TESForm::Array*, std::uint32_t, std::uint32_t);
			inline static const char* (*GetFormTypeStr)(EditorAPI::Forms::TESForm::FormType);
			inline static EditorAPI::Forms::TESForm::Array* (*OldFormReferenceMap_Find)(std::uint64_t Key);
			inline static EditorAPI::Forms::TESForm::Array* (*OldFormReferenceMap_Create)(EditorAPI::Forms::TESForm::Array*);
			inline static void (*OldFormReferenceMap_RemoveEntry)(EditorAPI::Forms::TESForm::Array*, int);

			std::unordered_set<EditorAPI::Forms::TESForm*, std::hash<EditorAPI::Forms::TESForm*>,
				std::equal_to<EditorAPI::Forms::TESForm*>> AlteredFormListShadow;
			std::unordered_map<std::uint64_t, EditorAPI::Forms::TESForm::Array*, std::hash<std::uint64_t>,
				std::equal_to<std::uint64_t>> FormReferenceMap;

			static void* AlteredFormList_Create(EditorAPI::Forms::TESForm::Array* Array, 
				std::uint32_t Unknown) noexcept(true)
			{
				AlteredFormListShadow.clear();
				return OldAlteredFormList_Create(Array, Unknown);
			}

			static void AlteredFormList_RemoveAllEntries(EditorAPI::Forms::TESForm::Array* Array,
				bool Unknown) noexcept(true)
			{
				AlteredFormListShadow.clear();
				OldAlteredFormList_RemoveAllEntries(Array, Unknown);
			}

			static void AlteredFormList_Insert(EditorAPI::Forms::TESForm::Array* Array, 
				EditorAPI::Forms::TESForm*& Entry) noexcept(true)
			{
				AlteredFormListShadow.insert(Entry);
				OldAlteredFormList_Insert(Array, Entry);
			}

			static void AlteredFormList_RemoveEntry(EditorAPI::Forms::TESForm::Array* Array, 
				std::uint32_t Index, std::uint32_t Unknown) noexcept(true)
			{
				AlteredFormListShadow.erase(Array->at(Index));
				OldAlteredFormList_RemoveEntry(Array, Index, Unknown);
			}

			static bool AlteredFormList_ElementExists(EditorAPI::Forms::TESForm::Array* Array, 
				EditorAPI::Forms::TESForm*& Entry) noexcept(true)
			{
				return AlteredFormListShadow.count(Entry) > 0;
			}

			static void FormReferenceMap_RemoveAllEntries() noexcept(true)
			{
				for (auto [k, v] : FormReferenceMap)
				{
					if (v)
						OldFormReferenceMap_RemoveEntry(v, 1);
				}

				FormReferenceMap.clear();
			}

			static EditorAPI::Forms::TESForm::Array* FormReferenceMap_FindOrCreate(std::uint64_t Key,
				bool Create) noexcept(true)
			{
				auto itr = FormReferenceMap.find(Key);
				if (itr != FormReferenceMap.end() && itr->second)
					return itr->second;

				if (Create)
				{
					auto* ptr = OldFormReferenceMap_Find(24);
					if (ptr)
						ptr = OldFormReferenceMap_Create(ptr);

					FormReferenceMap.insert_or_assign(Key, ptr);
					return ptr;
				}

				return nullptr;
			}

			static void FormReferenceMap_RemoveEntry(std::uint64_t Key) noexcept(true)
			{
				auto itr = FormReferenceMap.find(Key);
				if (itr != FormReferenceMap.end())
				{
					EditorAPI::Forms::TESForm::Array* ptr = itr->second;
					FormReferenceMap.erase(itr);

					if (ptr)
						OldFormReferenceMap_RemoveEntry(ptr, 1);
				}
			}

			static bool FormReferenceMap_Get(std::uint64_t Unused, std::uint64_t Key,
				EditorAPI::Forms::TESForm::Array** Value) noexcept(true)
			{
				// Function doesn't care if entry is nullptr, only if it exists
				auto itr = FormReferenceMap.find(Key);
				if (itr != FormReferenceMap.end())
				{
					*Value = itr->second;
					return true;
				}

				return false;
			}

			AlteredFormList::AlteredFormList() : Common::Patch()
			{
				SetName("Altered Form List And Reference Map");
			}

			bool AlteredFormList::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AlteredFormList::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AlteredFormList::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> AlteredFormList::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AlteredFormList::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool AlteredFormList::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				EditorAPI::Forms::TESForm::FindFormByFormID = 
					decltype(EditorAPI::Forms::TESForm::FindFormByFormID)(__CKPE_OFFSET(0));

				OldAlteredFormList_Create = decltype(&AlteredFormList_Create)(__CKPE_OFFSET(1));
				OldAlteredFormList_RemoveAllEntries = decltype(&AlteredFormList_RemoveAllEntries)(__CKPE_OFFSET(2));
				OldAlteredFormList_Insert = decltype(&AlteredFormList_Insert)(__CKPE_OFFSET(3));
				OldAlteredFormList_RemoveEntry = decltype(&AlteredFormList_RemoveEntry)(__CKPE_OFFSET(4));

				OldFormReferenceMap_RemoveEntry = (void (*)(EditorAPI::Forms::TESForm::Array*, int))(__CKPE_OFFSET(5));
				OldFormReferenceMap_Find = (EditorAPI::Forms::TESForm::Array* (*)(std::uint64_t Key))(__CKPE_OFFSET(6));
				OldFormReferenceMap_Create = 
					(EditorAPI::Forms::TESForm::Array* (*)(EditorAPI::Forms::TESForm::Array*))(__CKPE_OFFSET(7));

				Detours::DetourJump(__CKPE_OFFSET(8), (uintptr_t)&FormReferenceMap_RemoveAllEntries);
				Detours::DetourJump(__CKPE_OFFSET(9), (uintptr_t)&FormReferenceMap_FindOrCreate);
				Detours::DetourJump(__CKPE_OFFSET(10), (uintptr_t)&FormReferenceMap_RemoveEntry);
				Detours::DetourJump(__CKPE_OFFSET(11), (uintptr_t)&FormReferenceMap_Get);

				SafeWrite::Write(__CKPE_OFFSET(12), { 0xCC });
				Detours::DetourCall(__CKPE_OFFSET(13), (uintptr_t)&AlteredFormList_Create);
				Detours::DetourCall(__CKPE_OFFSET(14), (uintptr_t)&AlteredFormList_RemoveAllEntries);
				Detours::DetourCall(__CKPE_OFFSET(15), (uintptr_t)&AlteredFormList_Insert);
				Detours::DetourCall(__CKPE_OFFSET(16), (uintptr_t)&AlteredFormList_RemoveEntry);
				Detours::DetourCall(__CKPE_OFFSET(17), (uintptr_t)&AlteredFormList_RemoveEntry);
				Detours::DetourCall(__CKPE_OFFSET(18), (uintptr_t)&AlteredFormList_ElementExists);

				return true;
			}
		}
	}
}