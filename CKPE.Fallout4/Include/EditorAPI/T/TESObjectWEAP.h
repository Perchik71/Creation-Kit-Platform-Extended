// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESEnchantableForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSBlockBashData.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/TBO_InstanceData.h>
#include <EditorAPI/B/BGSTypedFormValuePair.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSInstanceNamingRulesForm.h>
#include <EditorAPI/B/BGSAttachParentArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class ActorValueInfo;
				class BGSAimModel;
				class BGSEquipSlot;
				class BGSImpactDataSet;
				class BGSSoundDescriptorForm;
				class BGSZoomData;
				class SpellItem;
				class TESAmmo;
				class TESLevItem;

				class BGSProjectile;
				class EnchantmentItem;
				class BGSMaterialSwap;

				// 1.11.246.0: size 0x30, the ranged weapon data (CommonLibF4 TESObjectWEAP::RangedData)
				struct WeaponRangedData
				{
					BGSProjectile* overrideProjectile; // 00
					float fireSeconds; // 08
					float firingRumbleLeftMotorStrength; // 0C
					float firingRumbleRightMotorStrength; // 10
					float firingRumbleDuration; // 14
					float reloadSeconds; // 18
					float boltChargeSeconds; // 1C
					float sightedTransitionSeconds; // 20
					std::uint32_t rumblePeriod; // 24
					std::int32_t rumblePattern; // 28 - REX::TEnumSet<WEAPON_RUMBLE_PATTERN, std::int32_t>
					std::int8_t numProjectiles; // 2C
					std::uint8_t pad2D[3]; // 2D
				};
				static_assert(sizeof(WeaponRangedData) == 0x30);

				// Weapon instance data, created by TESObjectWEAP::CreateInstanceData (0x138 bytes)
				struct TESObjectWEAPData :
					public TBO_InstanceData   // 00
				{
					BGSSoundDescriptorForm* attackSound; // 10
					BGSSoundDescriptorForm* attackSound2D; // 18
					BGSSoundDescriptorForm* attackLoop; // 20
					BGSSoundDescriptorForm* attackFailSound; // 28
					BGSSoundDescriptorForm* idleSound; // 30
					BGSSoundDescriptorForm* equipSound; // 38
					BGSSoundDescriptorForm* unEquipSound; // 40
					BGSSoundDescriptorForm* fastEquipSound; // 48
					BGSBlockBashData* blockBashData; // 50
					BGSImpactDataSet* impactDataSet; // 58
					TESLevItem* npcAddAmmoList; // 60
					TESAmmo* ammo; // 68
					BGSEquipSlot* equipSlot; // 70
					SpellItem* effect; // 78
					BGSKeywordForm* keywords; // 80
					BGSAimModel* aimModel; // 88
					BGSZoomData* zoomData; // 90
					WeaponRangedData* rangedData; // 98
					BSTArray<EnchantmentItem*>* enchantments; // A0
					BSTArray<BGSMaterialSwap*>* materialSwaps; // A8
					BGSTypedFormValuePair::Array* damageTypes; // B0
					BGSTypedFormValuePair::Array* actorValues; // B8
					float attackSeconds; // C0
					float reloadSpeed; // C4
					float speed; // C8
					float reach; // CC
					float minRange; // D0
					float maxRange; // D4
					float attackDelaySec; // D8
					float damageToWeaponMult; // DC
					float outOfRangeDamageMult; // E0
					float secondaryDamage; // E4
					float criticalChargeBonus; // E8
					float weight; // EC
					float soundLevelMult; // F0
					float attackActionPointCost; // F4
					float fullPowerSeconds; // F8
					float minPowerPerShot; // FC
					float colorRemappingIndex; // 100
					float criticalDamageMult; // 104
					std::int32_t staggerValue; // 108 - REX::TEnumSet<STAGGER_MAGNITUDE, std::int32_t>
					std::uint32_t value; // 10C
					std::uint32_t flags; // 110 - REX::TEnumSet<WEAPON_FLAGS, std::uint32_t>
					std::int32_t soundLevel; // 114 - REX::TEnumSet<SOUND_LEVEL, std::int32_t>
					std::int32_t hitBehavior; // 118 - REX::TEnumSet<WEAPONHITBEHAVIOR, std::int32_t>
					ActorValueInfo* skill; // 120
					ActorValueInfo* resistance; // 128
					std::uint16_t ammoCapacity; // 130
					std::uint16_t attackDamage; // 132
					std::uint16_t rank; // 134
					std::int8_t accuracyBonus; // 136
					std::uint8_t type; // 137 - REX::TEnum<WEAPON_TYPE, std::uint8_t>
				};
				static_assert(sizeof(TESObjectWEAPData) == 0x138);

				// 1.11.240.0: size 0x3B0, vtable RVA 0x309E6E0, 154 virtual functions
				class TESObjectWEAP :
					public TESBoundObject,              // 000
					public TESFullName,                 // 090
					public BGSModelMaterialSwap,        // 0A8
					public TESIcon,                     // 0F8
					public TESEnchantableForm,          // 120
					public BGSDestructibleObjectForm,   // 138
					public BGSEquipType,                // 148
					public BGSPreloadable,              // 158
					public BGSMessageIcon,              // 160
					public BGSPickupPutdownSounds,      // 190
					public BGSBlockBashData,            // 1A8
					public BGSKeywordForm,              // 1C0
					public TESDescription,              // 1E0
					public BGSInstanceNamingRulesForm   // 208
				{
				public:
					virtual ~TESObjectWEAP() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 118 */ [[nodiscard]] TESFile* GetDescriptionOwnerFile() const noexcept(true) override;
					/* 140 */ [[nodiscard]] bool HasPlayable() const noexcept(true) override;
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
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;
					/* 3A0 */ [[nodiscard]] const char* GetObjectTypeName() const noexcept(true) override;
					/* 3D0 */ [[nodiscard]] std::uint32_t GetFilledSlotsImpl() const noexcept(true) override;
					/* 3D8 */ [[nodiscard]] float GetDesirability() const noexcept(true) override;
					/* 440 */ TBO_InstanceData* CreateInstanceData(const TBO_InstanceData* pData) const noexcept(true) override;
					/* 448 */ const TBO_InstanceData* GetBaseInstanceData() const noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftWeapon;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectWEAP[0];
				public:
					TESObjectWEAPData weaponData; // 218 - TESObjectWEAP::Data (0x138, starts with a vptr)
					void* firstPersonModel; // 350 - BGSModelMaterialSwap*
					TESObjectWEAP* weaponTemplate; // 358
					void* embeddedWeaponMod; // 360 - BGSMod::Attachment::Mod*
					BGSAttachParentArray attachParents; // 368
					std::int32_t unk388; // 388 - 2 on InitializeData/ClearData
					std::uint32_t unk38C; // 38C
					std::uint64_t unk390; // 390 - only cleared by the constructor
					std::uint32_t unk398; // 398 - only cleared by the constructor
					std::uint32_t unk39C; // 39C
					std::uint8_t unk3A0; // 3A0 - 0 on construction (16-byte member initialised by a small constructor: bytes 0,1,1, then a null pointer at 3A8)
					std::uint8_t unk3A1; // 3A1 - 1 on construction
					std::uint8_t unk3A2; // 3A2 - 1 on construction
					std::uint8_t pad3A3[0x5]; // 3A3
					void* unk3A8; // 3A8 - null on construction
				};
				static_assert(sizeof(TESObjectWEAP) == 0x3B0);
			}
		}
	}
}
