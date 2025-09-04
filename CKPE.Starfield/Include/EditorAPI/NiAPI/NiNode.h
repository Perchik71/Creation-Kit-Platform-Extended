// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiAVObject.h>
#include <EditorAPI/NiAPI/NiTObjectArray.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiNode : public NiAVObject
				{
					//NiTObjectArray<NiAVObject*> _array_objs;
				public:
					virtual ~NiNode() = default;
				};
				static_assert(sizeof(NiNode) == 0x140);
			}
		}
	}
}
