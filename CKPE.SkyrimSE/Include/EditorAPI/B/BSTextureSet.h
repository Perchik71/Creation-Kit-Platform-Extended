// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiSourceTexture.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSTextureSet :
				public NiAPI::NiObject
			{
			public:
				enum class Texture : std::uint32_t
				{
					kDiffuse = 0,
					kNormal,
					kGloss = kNormal,
					kEnvironmentMask,
					kSubsurfaceTint = kEnvironmentMask,
					kGlowMap,
					kDetailMap = kGlowMap,
					kHeight,
					kEnvironment,
					kMultilayer,
					kBacklightMask,
					kSpecular = kBacklightMask,
					kTotal
				};

				virtual ~BSTextureSet() = default;

				// add
				virtual const char* GetTexturePath(Texture a_texture) = 0;
				virtual void SetTexture(Texture a_texture, NiAPI::NiSourceTexture* a_srcTexture) = 0;
				virtual void SetTexturePath(Texture a_texture, const char* a_path) = 0;
			};
			static_assert(sizeof(BSTextureSet) == 0x10);
		}
	}
}