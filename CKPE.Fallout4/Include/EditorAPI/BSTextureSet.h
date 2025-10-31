// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/NiAPI/NiObject.h>
#include <EditorAPI/NiAPI/NiPointer.h>
#include <EditorAPI/NiAPI/NiTexture.h>
#include <EditorAPI/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 10
			class BSTextureSet : public NiAPI::NiObject
			{
			public:
				inline BSFixedString GetTextureFilenameFS(std::uint32_t Index) const noexcept(true)
				{ return CKPE::vtbl_call<BSFixedString>(0x150, this, Index); }
				inline const char* GetTextureFilename(std::uint32_t Index) const noexcept(true) 
				{ return CKPE::vtbl_call<const char*>(0x158, this, Index); }

				inline void GetTexture(std::uint32_t Index, NiAPI::NiPointer<NiAPI::NiTexture>& Texture, 
					bool Unk1 = true) const noexcept(true) 
				{ return CKPE::vtbl_call<void>(0x168, this, Index, Texture, Unk1); }
				inline void SetTextureFilename(std::uint32_t Index, const char* FileName) const noexcept(true) 
				{ return CKPE::vtbl_call<void>(0x170, this, Index, FileName); }
			};
			static_assert(sizeof(BSTextureSet) == 0x10);

			// 60
			class BSShaderTextureSet : public BSTextureSet
			{
			public:
				constexpr static std::uint32_t MAX = 10;
				inline const char* At(std::uint32_t Index) const { return Textures[Index]; };
				inline const char* operator[](std::uint32_t Index) const { return At(Index); };
			protected:
				BSFixedString Textures[MAX];	// 10
			};
			static_assert(sizeof(BSShaderTextureSet) == 0x60);
		}
	}
}