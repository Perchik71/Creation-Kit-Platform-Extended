// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/T/TESImageSpaceModifiableForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSImpactDataSet;
				class BGSSoundDescriptorForm;
				class SpellItem;
				class TESObjectLIGH;

				struct BGSHazardData
				{
					std::uint32_t limit; // 00
					float radius; // 04
					float lifetime; // 08
					float imageSpaceRadius; // 0C
					float targetInterval; // 10
					std::uint32_t flags; // 14 - REX::TEnumSet<BGSHazardFlags, std::uint32_t>
					SpellItem* spell; // 18
					TESObjectLIGH* light; // 20
					BGSImpactDataSet* impactDataSet; // 28
					BGSSoundDescriptorForm* sound; // 30
					float fullEffectRadius; // 38
					float taperWeight; // 3C
					float taperCurve; // 40
				};
				static_assert(sizeof(BGSHazardData) == 0x48);

				// 1.11.240.0: size 0x140, vtable RVA 0x30812C0, 154 virtual functions
				class BGSHazard :
					public TESBoundObject,               // 000
					public TESFullName,                  // 090
					public TESModel,                     // 0A8
					public BGSPreloadable,               // 0E0
					public TESImageSpaceModifiableForm   // 0E8
				{
				public:
					virtual ~BGSHazard() = default;

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

					constexpr static std::uint8_t TYPE_ID = ftHazard;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSHazard[0];
				public:
					BGSHazardData data; // F8
				};
				static_assert(sizeof(BGSHazard) == 0x140);
			}
		}
	}
}
