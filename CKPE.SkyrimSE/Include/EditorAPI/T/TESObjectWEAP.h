// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/TESEnchantableForm.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/TESAttackDamageForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSBlockBashData.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/TESDescription.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x2B0, vtable RVA 0x3102DF0
				class TESObjectWEAP :
					public TESBoundObject,				// 000
					public TESFullName,					// 050
					public TESModelTextureSwap,			// 068
					public TESIcon,						// 0B0
					public TESEnchantableForm,			// 0D8
					public TESValueForm,				// 0F0
					public TESWeightForm,				// 100
					public TESAttackDamageForm,			// 110
					public BGSDestructibleObjectForm,	// 120
					public BGSEquipType,				// 130
					public BGSPreloadable,				// 140
					public BGSMessageIcon,				// 148
					public BGSPickupPutdownSounds,		// 178
					public BGSBlockBashData,			// 190
					public BGSKeywordForm,				// 1A8
					public TESDescription				// 1C0
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftWeapon;

					virtual ~TESObjectWEAP() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2F8 */ void CollectAdditionalResourceFiles(void* Files) noexcept(true) override;
					/* 318 */ [[nodiscard]] const char* GetObjectTypeName() const noexcept(true) override;

				public:
					std::uint8_t pad1E8[0x50];	// 1E8
					void* unk238;	// 238 - EFSD/MO3T/MOD3
					void* snam;	// 240 - SNAM
					void* xnam;	// 248 - XNAM
					void* nam7;	// 250 - NAM7
					void* tnam;	// 258 - TNAM
					void* unam;	// 260 - UNAM
					void* nam9;	// 268 - NAM9
					void* nam8;	// 270 - NAM8
					void* inam;	// 278 - INAM
					void* wnam;	// 280 - WNAM
					void* cnam;	// 288 - CNAM
					std::uint8_t pad290[0x8];	// 290
					std::uint32_t vnam;	// 298 - VNAM
					std::uint8_t pad29C[0x14];	// 29C
				};
				static_assert(offsetof(TESObjectWEAP, pad1E8) == 0x1E8);
				static_assert(offsetof(TESObjectWEAP, unk238) == 0x238);
				static_assert(offsetof(TESObjectWEAP, snam) == 0x240);
				static_assert(offsetof(TESObjectWEAP, xnam) == 0x248);
				static_assert(offsetof(TESObjectWEAP, nam7) == 0x250);
				static_assert(offsetof(TESObjectWEAP, tnam) == 0x258);
				static_assert(offsetof(TESObjectWEAP, unam) == 0x260);
				static_assert(offsetof(TESObjectWEAP, nam9) == 0x268);
				static_assert(offsetof(TESObjectWEAP, nam8) == 0x270);
				static_assert(offsetof(TESObjectWEAP, inam) == 0x278);
				static_assert(offsetof(TESObjectWEAP, wnam) == 0x280);
				static_assert(offsetof(TESObjectWEAP, cnam) == 0x288);
				static_assert(offsetof(TESObjectWEAP, vnam) == 0x298);
				static_assert(sizeof(TESObjectWEAP) == 0x2B0);
			}
		}
	}
}