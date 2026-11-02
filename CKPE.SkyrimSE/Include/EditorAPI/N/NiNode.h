// Copyright © 2023-2024 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/N/NiAVObject.h>
#include <EditorAPI/N/NiTObjectArray.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiNode : public NiAVObject
				{
					NiTObjectArray<NiAVObject*> _array_objs;
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::NiNode;
					constexpr static Common::ID VTABLE = IDs::VTABLE::NiNode[0];

					virtual ~NiNode() = default;
				};
				static_assert(sizeof(NiNode) == 0x128);
			}
		}
	}
}
