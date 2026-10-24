// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShader.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/I/ImageSpaceEffect.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSImagespaceShader : public BSShader
	{
	public:
		struct RuntimeData
		{
			/* 0x000 */ ImageSpaceEffect effect;
			/* 0x0B0 */ const char* className;
			/* 0x0B8 */ const char* sourcePrefix;
			/* 0x0C0 */ std::uint32_t unk0C0;
			/* 0x0C4 */ std::uint8_t unk0C4;
			/* 0x0C5 */ std::byte _pad0C5[3];
			/* 0x0C8 */ BSTArray<const char*> vertexConstantNames;
			/* 0x0E0 */ BSTArray<const char*> pixelConstantNames;
			/* 0x0F8 */ BSTArray<const char*> textureNames;
			/* 0x110 */ BSTArray<const char*> unk110;
		};
		static_assert(sizeof(RuntimeData) == 0x128);
		static_assert(offsetof(RuntimeData, className) == 0xB0);
		static_assert(offsetof(RuntimeData, vertexConstantNames) == 0xC8);

		[[nodiscard]] static std::size_t GetRuntimeSize() noexcept(true)
		{ return BSShader::GetRuntimeSize() + sizeof(RuntimeData); }

		[[nodiscard]] RuntimeData& GetImagespaceRuntimeData() noexcept(true)
		{
			return *reinterpret_cast<RuntimeData*>(
				reinterpret_cast<char*>(this) + BSShader::GetRuntimeSize());
		}
		[[nodiscard]] const RuntimeData& GetImagespaceRuntimeData() const noexcept(true)
		{
			return *reinterpret_cast<const RuntimeData*>(
				reinterpret_cast<const char*>(this) + BSShader::GetRuntimeSize());
		}
		[[nodiscard]] ImageSpaceEffect& GetImageSpaceEffect() noexcept(true)
		{ return GetImagespaceRuntimeData().effect; }
		[[nodiscard]] const ImageSpaceEffect& GetImageSpaceEffect() const noexcept(true)
		{ return GetImagespaceRuntimeData().effect; }

	private:
		// The secondary base follows the runtime-sized BSShader base.
		std::byte _runtimeStorage[0x128];
	};
	static_assert(sizeof(BSImagespaceShader) == 0x310);
}
