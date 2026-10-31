// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xA8, vtable RVA 0x30395F8, 124 virtual functions
				class BGSMovementType :
					public TESForm   // 000
				{
				public:
					virtual ~BGSMovementType() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftMovementType;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMovementType[0];
				public:
					BSFixedString unk28; // 28
					std::uint8_t unk30[0x70]; // 30
					float floatHeight; // A0
					std::uint32_t unkA4; // A4
				};
				static_assert(sizeof(BGSMovementType) == 0xA8);
			}
		}
	}
}
