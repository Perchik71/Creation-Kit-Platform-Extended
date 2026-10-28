// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <CKPE.Utils.h>
#include <EditorAPI/N/NiObject.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiTexture.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 10
			class BSTextureSet : public NiAPI::NiObject
			{
			protected:
				/* 2A */ virtual const BSFixedString& GetTextureFilenameRef(std::uint32_t Index) const noexcept(true);
				/* 2B */ virtual const char* GetTextureFilenameImpl(std::uint32_t Index) const noexcept(true);
				/* 2C */ virtual void sub_160();
				/* 2D */ virtual void GetTextureImpl(std::uint32_t Index,
					NiAPI::NiPointer<NiAPI::NiTexture>& Texture, bool Unk1) const noexcept(true);
				/* 2E */ virtual void SetTextureFilenameImpl(std::uint32_t Index,
					const char* FileName) const noexcept(true);
			public:
				inline BSFixedString GetTextureFilenameFS(std::uint32_t Index) const noexcept(true)
				{ return GetTextureFilenameRef(Index); }
				inline const char* GetTextureFilename(std::uint32_t Index) const noexcept(true) 
				{ return GetTextureFilenameImpl(Index); }

				inline void GetTexture(std::uint32_t Index, NiAPI::NiPointer<NiAPI::NiTexture>& Texture, 
					bool Unk1 = true) const noexcept(true) 
				{ GetTextureImpl(Index, Texture, Unk1); }
				inline void SetTextureFilename(std::uint32_t Index, const char* FileName) const noexcept(true) 
				{ SetTextureFilenameImpl(Index, FileName); }
			};
			static_assert(sizeof(BSTextureSet) == 0x10);

			// 60
			class BSShaderTextureSet : public BSTextureSet
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSShaderTextureSet[0];

				constexpr static std::uint32_t MAX = 10;
				inline const char* At(std::uint32_t Index) const { return Textures[Index]; };
				inline const char* operator[](std::uint32_t Index) const { return At(Index); };
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{ return offsetof(BSShaderTextureSet, Textures) == 0x10; }
			protected:
				BSFixedString Textures[MAX];	// 10
			};
			static_assert(sizeof(BSShaderTextureSet) == 0x60);
			static_assert(BSShaderTextureSet::VerifyLayout());
		}
	}
}