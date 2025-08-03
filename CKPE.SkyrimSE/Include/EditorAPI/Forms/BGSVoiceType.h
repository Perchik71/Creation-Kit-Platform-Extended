// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x30
				// func = 124
				class BGSVoiceType : public TESForm
				{
				public:
					enum Gender
					{
						Male = 0,
						Woman = 2,
					};
				public:
					virtual ~BGSVoiceType() = default;
				public:
					inline bool HasMale() const { return !HasWoman(); }
					inline bool HasWoman() const { return (flag & Woman) == Woman; }
					inline bool HasAllowDefaultDialogue() const { return (flag & 1) == 1; }

					inline void SetGender(Gender NewGender)
					{
						flag = (HasAllowDefaultDialogue() ? 1 : 0) +
							((NewGender == Woman) ? (uint8_t)Woman : (uint8_t)Male);
					}

					inline void SetAllowDefaultDialogue(bool v) { flag = (flag & 1) + (v ? 1 : 0); }
				private:
					uint8_t flag;
					char pad29[0x7];
				};
				static_assert(sizeof(BGSVoiceType) == 0x30);
			}
		}
	}
}
