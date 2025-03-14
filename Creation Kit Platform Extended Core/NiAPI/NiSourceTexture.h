// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiSourceTexture.h

#pragma once

#include "NiTexture.h"

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		namespace SkyrimSpectialEdition
		{
			class NiSourceTexture : public NiTexture
			{
			public:
				NiSourceTexture() = default;
				virtual ~NiSourceTexture() = default;
			};
			static_assert(sizeof(NiSourceTexture) == 0x58);
		}

		namespace Fallout4
		{
			class NiSourceTexture : public NiTexture
			{
			public:
				NiSourceTexture() = default;
				virtual ~NiSourceTexture() = default;
			};
			static_assert(sizeof(NiSourceTexture) == 0x48);
		}
	}
}