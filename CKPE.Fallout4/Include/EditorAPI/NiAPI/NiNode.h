// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiAVObject.h>
#include <EditorAPI/NiAPI/NiTObjectArray.h>
#include <EditorAPI/BSAtomicValue.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiNode : public NiAVObject
				{
					NiTObjectArray<NiPointer<NiAVObject>> _array_objs;
					BSTAtomicValue<std::uint32_t> _dirtyState;
					float _meshLODFadeAmount;
				public:
					virtual ~NiNode() = default;
				};
				static_assert(sizeof(NiNode) == 0x140);
			}
		}
	}
}
