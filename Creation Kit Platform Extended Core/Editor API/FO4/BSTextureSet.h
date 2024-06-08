// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiObject.h"
#include "NiAPI/NiPointer.h"
#include "NiAPI/NiTexture.h"
#include "BSFixedString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace NiAPI::Fallout4;

			// 10
			class BSTextureSet : public NiObject
			{
			public:
				inline BSFixedString GetTextureFilenameFS(uint32_t Index) const { return thisVirtualCall<BSFixedString>(0x150, this, Index); }
				inline const char* GetTextureFilename(uint32_t Index) const { return thisVirtualCall<const char*>(0x158, this, Index); }

				inline void GetTexture(uint32_t Index, NiAPI::NiPointer<NiAPI::NiTexture>& Texture, bool Unk1 = true) const { return thisVirtualCall<void>(0x168, this, Index, Texture, Unk1); }
				inline void SetTextureFilename(uint32_t Index, const char* FileName) const { return thisVirtualCall<void>(0x170, this, Index, FileName); }
			};
			static_assert(sizeof(BSTextureSet) == 0x10);

			// 60
			class BSShaderTextureSet : public BSTextureSet
			{
			public:
				constexpr static uint32_t MAX = 10;
				inline const char* At(uint32_t Index) const { return Textures[Index].c_str_ref(); };
				inline const char* operator[](uint32_t Index) const { return At(Index); };
			protected:
				BSFixedString Textures[MAX];	// 10
			};
			static_assert(sizeof(BSShaderTextureSet) == 0x60);
		}
	}
}