// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x108, vtable RVA 0x307E3E8, 154 virtual functions
				class BGSArtObject :
					public TESBoundObject,        // 000
					public BGSKeywordForm,        // 090
					public BGSModelMaterialSwap   // 0B0
				{
				public:
					virtual ~BGSArtObject() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftArt;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSArtObject[0];
				public:
					std::uint32_t unk100; // 100
					std::uint32_t unk104; // 104
				};
				static_assert(sizeof(BGSArtObject) == 0x108);
			}
		}
	}
}
