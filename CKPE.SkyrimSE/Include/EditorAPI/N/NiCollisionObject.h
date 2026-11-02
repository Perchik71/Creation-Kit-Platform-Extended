// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiObject.h>
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
				class NiAVObject;
				class NiCollisionObject : public NiObject
				{
					// members
					NiAVObject* sceneObject;
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::NiCollisionObject;
					constexpr static Common::ID VTABLE = IDs::VTABLE::NiCollisionObject[0];

					virtual ~NiCollisionObject() = 0;

					// add
					virtual void Unk_25(void);
					virtual void Unk_26(void) = 0;
					virtual void Unk_27(void) = 0;
					virtual void Unk_28(void);
					virtual void Unk_29(void);

					inline NiAVObject* GetSceneObject() const noexcept(true) { return sceneObject; }
				};
				static_assert(sizeof(NiCollisionObject) == 0x18);
			}
		}
	}
}