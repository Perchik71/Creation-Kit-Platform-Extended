// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <unordered_set>
#include <unordered_map>
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

			static bool AlteredFormList_ElementExists([[maybe_unused]] EditorAPI::Forms::TESForm::Array* Array, 
				EditorAPI::Forms::TESForm*& Entry) noexcept(true)
			{
				return AlteredFormListShadow.contains(Entry);
			}

			static void FormReferenceMap_RemoveAllEntries() noexcept(true)
			{
				for (auto& [k, v] : FormReferenceMap)
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

			static bool FormReferenceMap_Get([[maybe_unused]] std::uint64_t Unused, std::uint64_t Key,
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

			bool AlteredFormList::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool AlteredFormList::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool AlteredFormList::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				EditorAPI::Forms::TESForm::FindFormByFormID = 
					decltype(EditorAPI::Forms::TESForm::FindFormByFormID)(ID(76804).Address());
				
				OldAlteredFormList_Create = decltype(&AlteredFormList_Create)(ID(569171).Address());
				OldAlteredFormList_RemoveAllEntries = decltype(&AlteredFormList_RemoveAllEntries)(ID(600108).Address());
				OldAlteredFormList_Insert = decltype(&AlteredFormList_Insert)(ID(261367).Address());
				OldAlteredFormList_RemoveEntry = decltype(&AlteredFormList_RemoveEntry)(ID(297636).Address());
				OldFormReferenceMap_RemoveEntry = decltype(OldFormReferenceMap_RemoveEntry)(ID(607945).Address());
				OldFormReferenceMap_Find = decltype(OldFormReferenceMap_Find)(ID(654236).Address());
				OldFormReferenceMap_Create = decltype(OldFormReferenceMap_Create)(ID(374023).Address());
				
				Relocation(ID(561071)).WriteJump(FormReferenceMap_RemoveAllEntries);
				Relocation(ID(561075)).WriteJump(FormReferenceMap_FindOrCreate);
				Relocation(ID(147864)).WriteJump(FormReferenceMap_RemoveEntry);
				Relocation(ID(344361)).WriteJump(FormReferenceMap_Get);

				Relocation(ID(30046), 0x10E).WriteFill(INT3, 5);
				Relocation(ID(459525), 0x10F).WriteCall(AlteredFormList_Create);
				Relocation(ID(551175), 0xE8).WriteCall(AlteredFormList_RemoveAllEntries);
				Relocation(ID(555428), 0x122).WriteCall(AlteredFormList_Insert);
				Relocation(ID(555951), 0x166).WriteCall(AlteredFormList_RemoveEntry);
				Relocation(ID(555428), 0x1D3).WriteCall(AlteredFormList_RemoveEntry);
				Relocation(ID(555428), 0xFD).WriteCall(AlteredFormList_ElementExists);

				return true;
			}
		}
	}
}