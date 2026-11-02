// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiSourceTexture.h

#pragma once

#include <EditorAPI/N/NiTexture.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiSourceTexture : public NiTexture
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::NiSourceTexture;
					constexpr static Common::ID VTABLE = IDs::VTABLE::NiSourceTexture[0];

					NiSourceTexture() = default;
					virtual ~NiSourceTexture() = default;
				};
				static_assert(sizeof(NiSourceTexture) == 0x58);
			}
		}
	}
}