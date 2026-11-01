// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSMenuDisplayObject.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class FilterValidationFunction_t;

				class ActorValueInfo;
				class BGSArtObject;
				class BGSDualCastData;
				class BGSExplosion;
				class BGSImpactDataSet;
				class BGSPerk;
				class BGSProjectile;
				class BGSReferenceEffect;
				class SpellItem;
				class TESEffectShader;
				class TESForm;
				class TESImageSpaceModifier;
				class TESObjectLIGH;

				struct EffectSettingData
				{
					std::uint32_t flags; // 00 - REX::TEnumSet<Flag, std::uint32_t>
					float baseCost; // 04
					TESForm* associatedForm; // 08
					ActorValueInfo* associatedSkill; // 10
					ActorValueInfo* resistVariable; // 18
					std::int16_t numCounterEffects; // 20
					TESObjectLIGH* light; // 28
					float taperWeight; // 30
					TESEffectShader* effectShader; // 38
					TESEffectShader* enchantEffect; // 40
					std::int32_t minimumSkill; // 48
					std::int32_t spellmakingArea; // 4C
					float spellmakingChargeTime; // 50
					float taperCurve; // 54
					float taperDuration; // 58
					float secondaryAVWeight; // 5C
					std::int32_t archetype; // 60 - REX::TEnumSet<EffectArchetypes::ArchetypeID, std::int32_t>
					ActorValueInfo* primaryAV; // 68
					BGSProjectile* projectileBase; // 70
					BGSExplosion* explosion; // 78
					std::int32_t castingType; // 80 - REX::TEnumSet<MagicSystem::CastingType, std::int32_t>
					std::int32_t delivery; // 84 - REX::TEnumSet<MagicSystem::Delivery, std::int32_t>
					ActorValueInfo* secondaryAV; // 88
					BGSArtObject* castingArt; // 90
					BGSArtObject* hitEffectArt; // 98
					BGSImpactDataSet* impactDataSet; // A0
					float skillUsageMult; // A8
					BGSDualCastData* dualCastData; // B0
					float dualCastScale; // B8
					BGSArtObject* enchantEffectArt; // C0
					BGSReferenceEffect* hitVisuals; // C8
					BGSReferenceEffect* enchantVisuals; // D0
					SpellItem* equipAbility; // D8
					TESImageSpaceModifier* imageSpaceMod; // E0
					BGSPerk* perk; // E8
					std::int32_t castingSoundLevel; // F0 - REX::TEnumSet<SOUND_LEVEL, std::int32_t>
					float aiScore; // F4
					float aiDelayTime; // F8
				};
				static_assert(sizeof(EffectSettingData) == 0x100);

				// 1.11.240.0: size 0x1E0, vtable RVA 0x30BE680, 124 virtual functions
				class EffectSetting :
					public TESForm,               // 000
					public TESFullName,           // 028
					public BGSMenuDisplayObject,  // 040
					public BGSKeywordForm         // 050
				{
				public:
					virtual ~EffectSetting() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;
					/* 3A0 */ [[nodiscard]] const char* GetObjectTypeName() const noexcept(true) override;

				public:
					FilterValidationFunction_t* filterValidationFunction; // 70
					void* filterValidationItem; // 78
					EffectSettingData data; // 80
					BSSimpleList<EffectSetting*> counterEffects; // 180 - BSSimpleList<EffectSetting*>
					// 16 byte entries (the array stride, found in the indexing code); member order as CommonLibF4's SoundPair
					struct SoundPair
					{
						std::int32_t id; // 00 - MagicSystem::SoundID
						std::uint32_t pad04; // 04
						void* sound; // 08 - BGSSoundDescriptorForm*
					};
					static_assert(sizeof(SoundPair) == 0x10);

					BSTArray<SoundPair> effectSounds; // 190
					BGSLocalizedString magicItemDescription; // 1A8
					std::uint8_t unk1B8[0x18]; // 1B8
					std::uint8_t unk1D0[0x10]; // 1D0
				};
				static_assert(sizeof(EffectSetting) == 0x1E0);
			}
		}
	}
}
