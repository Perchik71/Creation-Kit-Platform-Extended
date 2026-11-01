// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xD8, vtable RVA 0x305CC18, 124 virtual functions
				class BGSMaterialObject :
					public TESForm,           // 000
					public TESModel,          // 028
					public BSMaterialObject   // 060
				{
				public:
					virtual ~BGSMaterialObject() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftMaterialObject;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMaterialObject[0];
				public:
					// DNAM file data entry (16 bytes, the array stride)
					struct FILE_DATA
					{
						char* buffer; // 00
						std::uint32_t bufferSize; // 08
						std::uint32_t pad0C; // 0C
					};
					static_assert(sizeof(FILE_DATA) == 0x10);

					BSTArray<FILE_DATA> fileData; // B0
					std::uint64_t unkC8; // C8
					std::uint64_t unkD0; // D0
				};
				static_assert(sizeof(BGSMaterialObject) == 0xD8);
			}
		}
	}
}
