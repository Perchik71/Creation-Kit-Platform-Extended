// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/N/NiNode.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSNiNode : public NiAPI::NiNode
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSNiNode[0];
			};
			static_assert(sizeof(BSNiNode) == 0x140);
		}
	}
}
