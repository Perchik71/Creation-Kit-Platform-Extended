// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSHandleRefObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// TESObjectREFR is a typedef, so it can't be forward-declared as a class
				template<class _Ty> class TESObjectREFR_base;
				typedef TESObjectREFR_base<BSHandleRefObject_Original> TESObjectREFR;
			}
		}
	}
}
