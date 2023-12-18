// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiTexture.h

#pragma once

#include "NiObjectNET.h"

#pragma pack(push, 8)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class NiTexture : public NiObject
			{
			public:
				NiTexture() = default;
				virtual ~NiTexture() = default;

				char _pad[0x30];
			};
			static_assert(sizeof(NiTexture) == 0x40);
		}
	}
}

#pragma pack(pop)