#pragma once

#include "BSString.h"
#include "BSFixedString.h"
#include "BaseFormComponent.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class TESTexture : public BaseFormComponent
			{
				// members
				BSFixedString textureName;
				std::uint32_t model_unk10[6];
			public:
				virtual ~TESTexture() = default;

				// add
				virtual const char* sub70() const noexcept(true);
				[[nodiscard]] virtual std::uint32_t GetMaxAllowedSize() noexcept(true) { return 0; }
				[[nodiscard]] virtual const char* GetAsNormalFile(BSString& a_out) const noexcept(true);
				[[nodiscard]] virtual const char* GetDefaultPath() const noexcept(true) { return "Data\\Textures\\"; }
				[[nodiscard]] virtual const char* GetCaptionForOpenDialog() const noexcept(true) { return "Add Image File"; }
				virtual void sub98() const noexcept(true);
			};
			static_assert(sizeof(TESTexture) == 0x28);
		}
	}
}