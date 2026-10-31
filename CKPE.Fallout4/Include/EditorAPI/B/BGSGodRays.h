// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct GodRaysData
				{
					NiAPI::NiColor colorAir; // 00
					NiAPI::NiColor colorBack; // 0C
					NiAPI::NiColor colorFwd; // 18
					float intensity; // 24
					float scatterAir; // 28
					float scatterBack; // 2C
					float scatterFwd; // 30
					float phaseBack; // 34
					float phaseFwd; // 38
				};
				static_assert(sizeof(GodRaysData) == 0x3C);

				// 1.11.240.0: size 0x68, vtable RVA 0x2FC1AB8, 124 virtual functions
				class BGSGodRays :
					public TESForm   // 000
				{
				public:
					virtual ~BGSGodRays() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftGodRays;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSGodRays[0];
				public:
					GodRaysData data; // 28
					std::uint32_t unk64; // 64
				};
				static_assert(sizeof(BGSGodRays) == 0x68);
			}
		}
	}
}
