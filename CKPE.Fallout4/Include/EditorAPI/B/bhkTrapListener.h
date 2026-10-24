// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE::Fallout4::EditorAPI
{
	// Only the abstract interface is known; no complete-object size proof.
	class bhkTrapListener
	{
	protected:
		/* 00 */ virtual void TrapListener_Unknown00() = 0;
		/* 01 */ virtual void TrapListener_Unknown01() = 0;
		/* 02 */ virtual void TrapListener_Unknown02() = 0;
		/* 03 */ virtual void TrapListener_Unknown03() = 0;
		/* 04 */ virtual void TrapListener_Unknown04() = 0;
	};
	static_assert(sizeof(bhkTrapListener) == sizeof(void*));
}
