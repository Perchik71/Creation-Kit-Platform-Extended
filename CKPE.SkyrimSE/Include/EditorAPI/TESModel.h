// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BaseFormComponent.h>
#include <EditorAPI/BSFixedString.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class TESModelTextureSwap;
			class TESModel : public BaseFormComponent
			{
				// members
				BSFixedString model;
				std::uint32_t model_unk10[10];
			public:
				virtual ~TESModel() = default;

				// add
				[[nodiscard]] virtual const char* GetModel() const noexcept(true);
				virtual void sub78();
				virtual void SetModel(const char* a_model);
				virtual TESModelTextureSwap* GetAsModelTextureSwap() noexcept(true) { return nullptr; }
				virtual void sub90();
				virtual void sub98();
			};
			static_assert(sizeof(TESModel) == 0x38);
		}
	}
}