// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/TESImageSpaceModifiableForm.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x98, vtable RVA 0x30BEB10
				class BGSCameraShot :
					public TESForm,	// 00
					public TESModel,	// 28
					public TESImageSpaceModifiableForm	// 58
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftCameraShot;

					virtual ~BGSCameraShot() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk68[0x30];	// 68 - data, layout not resolved
				};
				static_assert(offsetof(BGSCameraShot, unk68) == 0x68);
				static_assert(sizeof(BGSCameraShot) == 0x98);
			}
		}
	}
}