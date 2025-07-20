// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiCollisionUtils.h

#pragma once

#include "NiPoint.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiCollisionUtils
				{
				public:
					static bool IntersectTriangle(const NiPoint3& kOrigin, const NiPoint3& kDir, const NiPoint3& kV1,
						const NiPoint3& kV2, const NiPoint3& kV3, bool bCull, NiPoint3& kIntersect,
						float& r, float& s, float& t) noexcept(true);
				};
			}
		}
	}
}