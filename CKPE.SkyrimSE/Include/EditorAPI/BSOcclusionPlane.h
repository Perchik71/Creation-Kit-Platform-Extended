// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAPI\NiObject.h"
#include "NiAPI\NiTransform.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSOcclusionShape : public NiAPI::NiObject
			{
				NiAPI::NiPoint3 _Position;
				NiAPI::NiMatrix3 _Rotate;
				char unk40[8];
				NiAPI::NiPoint2 _Bounds;
				char unk50[0xC8];
			public:
				virtual ~BSOcclusionShape() = default;

				[[nodiscard]] inline const NiAPI::NiPoint3& GetPosition() const noexcept(true) { return _Position; }
				[[nodiscard]] inline const NiAPI::NiMatrix3& GetRotate() const noexcept(true) { return _Rotate; }
				[[nodiscard]] inline const NiAPI::NiPoint2& GetBounds() const noexcept(true) { return _Bounds; }
			};
			static_assert(sizeof(BSOcclusionShape) == 0x118);
		}
	}
}