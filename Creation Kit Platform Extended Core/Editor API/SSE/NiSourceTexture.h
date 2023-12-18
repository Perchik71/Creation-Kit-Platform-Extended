// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiSourceTexture.h

#pragma once

#include "NiTexture.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			namespace BSGraphics
			{
				struct Texture;
			}

			class NiSourceTexture : public NiTexture
			{
			public:
				NiSourceTexture() = default;
				virtual ~NiSourceTexture() = default;

				char _pad0[0x8];
				BSGraphics::Texture* pRendererTexture;
				char _pad1[0x8];

				BSGraphics::Texture* QRendererTexture() const
				{
					return pRendererTexture;
				}
			};
			static_assert(sizeof(NiSourceTexture) == 0x58);
		}
	}
}