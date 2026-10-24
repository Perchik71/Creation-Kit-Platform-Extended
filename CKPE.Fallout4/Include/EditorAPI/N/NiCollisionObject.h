// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/N/NiObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiAVObject;

				class NiCollisionObject : public NiObject
				{
				public:
					virtual ~NiCollisionObject() = default;
					/* 2A */ virtual void SetTarget(NiAVObject* Target);
					/* 2B */ virtual void UnkCollisionObject2B();
					/* 2C */ virtual void UnkCollisionObject2C();
					/* 2D */ virtual void UnkCollisionObject2D();
					
					[[nodiscard]] inline const NiObject* GetSceneObject() const noexcept(true) { return _SceneObject; }
					[[nodiscard]] inline NiAVObject* GetTarget() const noexcept(true)
					{
						return reinterpret_cast<NiAVObject*>(_SceneObject);
					}

					/* 0x010 */ NiObject* _SceneObject;
				};
				static_assert(sizeof(NiCollisionObject) == 0x18);
				static_assert(offsetof(NiCollisionObject, _SceneObject) == 0x10);
			}
		}
	}
}
