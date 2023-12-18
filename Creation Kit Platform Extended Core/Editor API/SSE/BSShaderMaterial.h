// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSShader/BSShaderMaterial.h

#pragma once

#include "../NiPoint.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class BSIntrusiveRefCounted
			{
			public:
				unsigned int uiRefCount;
			};

			class BSShaderMaterial : public BSIntrusiveRefCounted
			{
			public:
				virtual ~BSShaderMaterial();
				virtual BSShaderMaterial* CreateNew();
				virtual void CopyMembers(BSShaderMaterial* Other);
				virtual bool DoIsCopy(const BSShaderMaterial* Other);
				virtual uint32_t ComputeCRC32(uint32_t Unknown);
				virtual BSShaderMaterial* GetDefault();
				virtual uint32_t GetUnknown1() { return 0; }
				virtual uint32_t GetUnknown2() { return 0; }
				// virtual void Print(); -- Creation kit only

				NiPoint2 kTexCoordOffset[2];
				NiPoint2 kTexCoordScale[2];
				char _pad[0xC];
			};
			static_assert(sizeof(BSShaderMaterial) == 0x38);
		}
	}
}