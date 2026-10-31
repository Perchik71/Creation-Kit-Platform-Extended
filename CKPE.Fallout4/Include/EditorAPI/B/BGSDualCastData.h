// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSArtObject;
				class BGSExplosion;
				class BGSImpactDataSet;
				class BGSProjectile;
				class TESEffectShader;

				struct BGSDualCastDataDEF
				{
					BGSProjectile* projectile; // 00
					BGSExplosion* explosion; // 08
					TESEffectShader* effectShader; // 10
					BGSArtObject* hitEffectArt; // 18
					BGSImpactDataSet* impactDataSet; // 20
					std::uint32_t flags; // 28
				};
				static_assert(sizeof(BGSDualCastDataDEF) == 0x30);

				// 1.11.240.0: size 0xC0, vtable RVA 0x30BD660, 154 virtual functions
				class BGSDualCastData :
					public TESBoundObject   // 000
				{
				public:
					virtual ~BGSDualCastData() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

				public:
					BGSDualCastDataDEF data; // 90
				};
				static_assert(sizeof(BGSDualCastData) == 0xC0);
			}
		}
	}
}
