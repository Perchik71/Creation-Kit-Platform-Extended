// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSShaderResourceManager_CK.h

#pragma once

#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiPick.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			class BSShaderResourceManager
			{
			public:
				bool FindIntersectionsTriShapeFastPath(NiPoint3& kOrigin, const NiPoint3& kDir, NiPick& kPick, 
					class BSTriShape* pkTriShape);
				bool FindIntersectionsTriShapeFastPathEx(NiPoint3& kOrigin, const NiPoint3& kDir, NiPick& kPick, 
					class BSTriShape* pkTriShape);
			};
		}
	}
}