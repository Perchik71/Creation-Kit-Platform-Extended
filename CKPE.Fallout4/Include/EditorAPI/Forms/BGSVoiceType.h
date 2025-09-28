// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiFlags.h>
#include "TESForm.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x40
				// func = 124
				class BGSVoiceType : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftVoice;

					enum Flags
					{
						kAllowDefaultDialogue = 1,
						kWoman = 2,
					};
				public:
					virtual ~BGSVoiceType() = default;
				public:
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
				private:
					NiAPI::NiTFlags<std::uint8_t, BGSVoiceType> flag;
					char pad29[0x17];
				};
				static_assert(sizeof(BGSVoiceType) == 0x40);
			}
		}
	}
}
