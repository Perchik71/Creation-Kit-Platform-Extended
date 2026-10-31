// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/T/TESEnchantableForm.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/T/TESImageSpaceModifiableForm.h>
#include <EditorAPI/N/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSImpactDataSet;
				class BGSProjectile;
				class BGSSoundDescriptorForm;
				class TESBoundObject;
				class TESObjectLIGH;

				struct BGSExplosionData
				{
					TESObjectLIGH* light; // 00
					BGSSoundDescriptorForm* sound1; // 08
					BGSSoundDescriptorForm* sound2; // 10
					BGSImpactDataSet* impactDataSet; // 18
					TESBoundObject* impactPlacedObject; // 20
					BGSProjectile* spawnProjectile; // 28
					NiAPI::NiPoint3 projectileVector; // 30
					float projectileSpread; // 3C
					std::uint32_t projectileCount; // 40
					float force; // 44
					float damage; // 48
					float innerRadius; // 4C
					float outerRadius; // 50
					float imageSpaceRadius; // 54
					float verticalOffsetMult; // 58
					std::uint32_t flags; // 5C
					std::int32_t soundLevel; // 60 - REX::TEnumSet<SOUND_LEVEL, std::int32_t>
					float placedObjectFadeDelay; // 64
					std::int32_t staggerMagnitude; // 68 - REX::TEnumSet<STAGGER_MAGNITUDE, std::int32_t>
				};
				static_assert(sizeof(BGSExplosionData) == 0x70);

				// 1.11.246.0: size 0x180, vtable RVA 0x3081520, 154 virtual functions
				class BGSExplosion :
					public TESBoundObject,               // 000
					public TESFullName,                  // 090
					public TESModel,                     // 0A8
					public TESEnchantableForm,           // 0E0
					public BGSPreloadable,               // 0F8
					public TESImageSpaceModifiableForm   // 100
				{
				public:
					virtual ~BGSExplosion() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
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
					/* 400 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D, bool bBackground) noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftExplosion;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSExplosion[0];
				public:
					BGSExplosionData data; // 110
				};
				static_assert(sizeof(BGSExplosion) == 0x180);
			}
		}
	}
}
