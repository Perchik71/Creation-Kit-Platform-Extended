// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x78, vtable RVA 0x30A6A90, 124 virtual functions
				class BGSShaderParticleGeometryData :
					public TESForm   // 000
				{
				public:
					virtual ~BGSShaderParticleGeometryData() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftShaderParticleGeometryData;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSShaderParticleGeometryData[0];
				public:
					BSTArray<void*> data; // 28 - BSTArray<SETTING_VALUE>
					TESTexture particleTexture; // 40
					BSFixedString materialName; // 68
					std::uint32_t unk70; // 70
					std::uint32_t unk74; // 74
				};
				static_assert(sizeof(BGSShaderParticleGeometryData) == 0x78);
			}
		}
	}
}
