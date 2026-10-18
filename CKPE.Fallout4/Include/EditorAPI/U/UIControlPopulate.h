// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Functions that fill a UI control (the UIDeffer patch wraps them with WM_SETREDRAW). Named after their call sites; purpose not
			// verified further.
			namespace UIControlPopulate
			{
				// CK 1.11.246.0 0x140536B10: (this, control window, flag).
				inline void Populate1(void* This, void* Control, std::int64_t Flag) noexcept(true)
				{
					using func_t = decltype(&Populate1);
					static Common::Relocation<func_t> func{ Common::ID{ 636530, 1675545 } };
					return func(This, Control, Flag);
				}

				// CK 1.11.246.0 0x140710360: (this, control window, flag).
				inline void Populate2(void* This, void* Control, std::int64_t Flag) noexcept(true)
				{
					using func_t = decltype(&Populate2);
					static Common::Relocation<func_t> func{ Common::ID{ 645724, 1445476 } };
					return func(This, Control, Flag);
				}

				// CK 1.11.246.0 0x140677DA0: (this, control window pointer).
				inline void Populate3(void* This, void** Control) noexcept(true)
				{
					using func_t = decltype(&Populate3);
					static Common::Relocation<func_t> func{ Common::ID{ 643197, 1973863 } };
					return func(This, Control);
				}
			}
		}
	}
}
