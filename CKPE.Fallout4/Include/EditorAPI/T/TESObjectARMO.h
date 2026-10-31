// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSTypedFormValuePair.h>
#include <EditorAPI/B/TBO_InstanceData.h>
#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESRaceForm.h>
#include <EditorAPI/T/TESEnchantableForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESBipedModelForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSBipedObjectForm.h>
#include <EditorAPI/B/BGSBlockBashData.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSInstanceNamingRulesForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BGSAttachParentArray.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectARMA;
				class EnchantmentItem;
				class BGSMaterialSwap;

				// 1.11.246.0: size 0x58, created by TESObjectARMO::CreateInstanceData (0x140C6CCF0); the constructor defaults match CommonLibF4's
				// TESObjectARMO::InstanceData (colorRemappingIndex is FLT_MAX until set).
				class ArmorInstanceData :
					public TBO_InstanceData   // 00
				{
				public:
					BSTArray<EnchantmentItem*>* enchantments; // 10
					BSTArray<BGSMaterialSwap*>* materialSwaps; // 18
					BGSBlockBashData* blockBashData; // 20
					BGSKeywordForm* keywords; // 28
					BGSTypedFormValuePair::Array* damageTypes; // 30
					BGSTypedFormValuePair::Array* actorValues; // 38
					float weight; // 40
					float colorRemappingIndex; // 44
					std::uint32_t value; // 48
					std::uint32_t health; // 4C
					std::int32_t staggerRating; // 50 - REX::TEnumSet<STAGGER_MAGNITUDE, std::int32_t>
					std::uint16_t rating; // 54
					std::uint16_t index; // 56
				};
				static_assert(sizeof(ArmorInstanceData) == 0x58);

				class ArmorAddon
				{
				public:
					std::uint16_t index; // 00
					std::uint8_t pad02[6]; // 02
					TESObjectARMA* armorAddon; // 08
				};
				static_assert(sizeof(ArmorAddon) == 0x10);

				// 1.11.240.0: size 0x3C0, vtable RVA 0x3094C30, 154 virtual functions
				class TESObjectARMO :
					public TESBoundObject,              // 000
					public TESFullName,                 // 090
					public TESRaceForm,                 // 0A8
					public TESEnchantableForm,          // 0B8
					public BGSDestructibleObjectForm,   // 0D0
					public BGSPickupPutdownSounds,      // 0E0
					public TESBipedModelForm,           // 0F8
					public BGSEquipType,                // 288
					public BGSBipedObjectForm,          // 298
					public BGSBlockBashData,            // 2A8
					public BGSKeywordForm,              // 2C0
					public TESDescription,              // 2E0
					public BGSInstanceNamingRulesForm   // 308
				{
				public:
					virtual ~TESObjectARMO() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 118 */ [[nodiscard]] TESFile* GetDescriptionOwnerFile() const noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;
					/* 3D8 */ [[nodiscard]] float GetDesirability() const noexcept(true) override;
					/* 440 */ TBO_InstanceData* CreateInstanceData(const TBO_InstanceData* pData) const noexcept(true) override;
					/* 448 */ const TBO_InstanceData* GetBaseInstanceData() const noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftArmor;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectARMO[0];
				public:
					ArmorInstanceData armorData; // 318
					BSTArray<ArmorAddon> modelArray; // 370
					TESObjectARMO* armorTemplate; // 388
					BGSAttachParentArray attachParents; // 390
					std::uint8_t unk3B0; // 3B0 - 0 on construction (same 16-byte member as TESObjectWEAP 3A0: bytes 0,1,1, then a null pointer at 3B8)
					std::uint8_t unk3B1; // 3B1 - 1 on construction
					std::uint8_t unk3B2; // 3B2 - 1 on construction
					std::uint8_t pad3B3[0x5]; // 3B3
					void* unk3B8; // 3B8 - null on construction
				};
				static_assert(sizeof(TESObjectARMO) == 0x3C0);
			}
		}
	}
}
