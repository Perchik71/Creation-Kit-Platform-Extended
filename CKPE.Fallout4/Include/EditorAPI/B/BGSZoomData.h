// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESImageSpaceModifier;

				// 1.11.240.0: size 0x48, vtable RVA 0x2FC0920, 124 virtual functions
				class BGSZoomData :
					public TESForm   // 000
				{
				public:
					virtual ~BGSZoomData() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftZoomData;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSZoomData[0];
				public:
					std::uint32_t unk28; // 28
					std::uint32_t unk2C; // 2C
					std::uint32_t unk30; // 30
					std::uint32_t unk34; // 34
					std::uint32_t unk38; // 38
					std::uint32_t unk3C; // 3C
					TESImageSpaceModifier* isMod; // 40
				};
				static_assert(sizeof(BGSZoomData) == 0x48);
			}
		}
	}
}
