// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

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
					constexpr static Common::ID RTTI = IDs::RTTI::TESFormIDListView;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESFormIDListView[0];

					virtual ~TESFormIDListView() = default;
				};
				static_assert(sizeof(TESFormIDListView) == 0x28);
			}
		}
	}
}