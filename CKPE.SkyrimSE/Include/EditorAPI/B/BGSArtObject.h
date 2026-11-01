// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESModelTextureSwap.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0xA0, vtable RVA 0x30E91B8
				class BGSArtObject :
					public TESBoundObject,	// 00
					public TESModelTextureSwap	// 50
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftArt;

					virtual ~BGSArtObject() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t dnam[0x8];	// 98 - DNAM
				};
				static_assert(offsetof(BGSArtObject, dnam) == 0x98);
				static_assert(sizeof(BGSArtObject) == 0xA0);
			}
		}
	}
}