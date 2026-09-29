// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;
			}

			class TESModelTextureSwap : public TESModel
			{
			public:
				struct AlternateTexture  // MODS
				{
					Forms::BGSTextureSet*	textureSet;  // 00
					std::uint32_t			index3D;     // 08
					BSFixedString			name3D;      // 10
				};
				static_assert(sizeof(AlternateTexture) == 0x18);
			private:
				// members
				AlternateTexture* alternateTextures;     // 28 - MODS
				std::uint32_t     numAlternateTextures;  // 30
				std::uint32_t     pad34;                 // 34
			public:
				virtual ~TESModelTextureSwap() = default;

				TESModelTextureSwap* GetAsModelTextureSwap() noexcept(true) override { return this; }

				[[nodiscard]] inline AlternateTexture* GetAlternateTextureList() const noexcept(true) { return alternateTextures; }
				[[nodiscard]] inline std::uint32_t GetAlternateTextureCount() const noexcept(true) { return numAlternateTextures; }
			};
			static_assert(sizeof(TESModelTextureSwap) == 0x48);
		}
	}
}