// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiTexture.h

#pragma once

#include "NiObjectNET.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiTexture : public NiObject
		{
		public:
			NiTexture();
			virtual ~NiTexture();

			// BSFixedString m_kName @ 0x20
			// NiTexture *m_pkPrev; @ 0x30
			// NiTexture *m_pkNext; @ 0x38

			char _pad[0x30];
		};
		static_assert(sizeof(NiTexture) == 0x40);
	}
}