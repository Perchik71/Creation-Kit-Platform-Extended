// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiObject.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiCollisionObject : public NiObject
				{
					NiObject* _SceneObject;
				public:
					virtual ~NiCollisionObject() = default;
					
					[[nodiscard]] inline const NiObject* GetSceneObject() const noexcept(true) { return _SceneObject; }
				};
				static_assert(sizeof(NiCollisionObject) == 0x18);
			}
		}
	}
}
