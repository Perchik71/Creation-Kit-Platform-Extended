// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/B/BGSPropertySheet.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct CLASS_DATA
				{
					std::int32_t serviceFlags; // 00
					float bleedoutDefault; // 04
				};
				static_assert(sizeof(CLASS_DATA) == 0x8);

				// 1.11.240.0: size 0xA8, vtable RVA 0x3041B38, 124 virtual functions
				class TESClass :
					public TESForm,           // 000
					public TESFullName,       // 028
					public TESDescription,    // 040
					public TESTexture,        // 068
					public BGSPropertySheet   // 090
				{
				public:
					virtual ~TESClass() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftClass;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESClass[0];
				public:
					CLASS_DATA data; // A0
				};
				static_assert(sizeof(TESClass) == 0xA8);
			}
		}
	}
}
