// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x28, vtable RVA 0x303C1F0. TESForm with list view support; adds no members.
				class TESFormIDListView : public TESForm
				{
				public:
					virtual ~TESFormIDListView() = default;
				};
				static_assert(sizeof(TESFormIDListView) == 0x28);
			}
		}
	}
}