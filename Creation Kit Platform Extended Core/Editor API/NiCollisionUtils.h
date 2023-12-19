// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiCollisionUtils.h

#pragma once

#include "NiPoint.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiCollisionUtils
		{
		public:
			static bool IntersectTriangle(const NiPoint3& kOrigin, const NiPoint3& kDir, const NiPoint3& kV1, 
				const NiPoint3& kV2, const NiPoint3& kV3, bool bCull, NiPoint3& kIntersect, 
				float& r, float& s, float& t);
		};
	}
}