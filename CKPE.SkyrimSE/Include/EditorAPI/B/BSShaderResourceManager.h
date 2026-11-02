// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSShaderResourceManager_CK.h

#pragma once

#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiPick.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

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
				constexpr static Common::ID RTTI = IDs::RTTI::BSShaderResourceManager;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSShaderResourceManager[0];

				bool FindIntersectionsTriShapeFastPath(NiPoint3& kOrigin, const NiPoint3& kDir, NiPick& kPick, 
					class BSTriShape* pkTriShape);
				bool FindIntersectionsTriShapeFastPathEx(NiPoint3& kOrigin, const NiPoint3& kDir, NiPick& kPick, 
					class BSTriShape* pkTriShape);
			};
		}
	}
}