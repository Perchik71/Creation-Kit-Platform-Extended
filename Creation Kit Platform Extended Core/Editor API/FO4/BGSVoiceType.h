// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESFormF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x40
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
				char pad29[0x17];
			};
			static_assert(sizeof(BGSVoiceType) == 0x40);
		}
	}
}
