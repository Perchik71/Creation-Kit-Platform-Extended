// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiFlags.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x30, vtable RVA 0x3040CE8, 124 virtual functions
				class BGSVoiceType :
					public TESForm   // 000
				{
				public:
					virtual ~BGSVoiceType() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftVoiceType;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSVoiceType[0];

					enum Flags
					{
						kAllowDefaultDialogue = 1,
						kWoman = 2,
					};

					[[nodiscard]] inline bool HasMale() const noexcept(true) { return !HasWoman(); }
					[[nodiscard]] inline bool HasWoman() const noexcept(true) { return flag.Has(kWoman); }
					[[nodiscard]] inline bool HasAllowDefaultDialogue() const noexcept(true) { return flag.Has(kAllowDefaultDialogue); }

					inline void SetGender(bool Woman)
					{
						if (Woman)
							flag.Set(kWoman);
						else
							flag.Unset(kWoman);
					}

					inline void SetAllowDefaultDialogue(bool v)
					{
						if (v)
							flag.Set(kAllowDefaultDialogue);
						else
							flag.Unset(kAllowDefaultDialogue);
					}

				public:
					NiAPI::NiTFlags<std::uint8_t, BGSVoiceType> flag; // 28
					std::uint8_t pad29[7]; // 29
				};
				static_assert(sizeof(BGSVoiceType) == 0x30);
			}
		}
	}
}
