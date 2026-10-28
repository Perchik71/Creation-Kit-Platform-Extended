// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;
			}


			// 1.11.246.0: size 0x20, vtable RVA 0x301A990, 23 virtual functions
			class BGSKeywordForm :
				public BaseFormComponent,  // 000
				public IKeywordFormBase    // 008
			{
			public:
				virtual ~BGSKeywordForm() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSKeywordForm[0];

				[[nodiscard]] bool HasKeyword(const Forms::BGSKeyword* keyword,
					const TBO_InstanceData* data) const noexcept(true) override;
				void CollectAllKeywords(void* keywords,
					const TBO_InstanceData* data) const noexcept(true) override;

				[[nodiscard]] inline Forms::BGSKeyword** GetKeywords() const noexcept(true) { return keywords; }
				[[nodiscard]] inline std::uint32_t GetKeywordCount() const noexcept(true) { return numKeywords; }

				virtual void OnKeywordsChanged() { return; } // 21 - RVA 0x898700
				virtual Forms::BGSKeyword* GetDefaultKeyword() const noexcept(true) { return nullptr; } // 22 - RVA 0x45AB00

				inline void sub_0A8() { OnKeywordsChanged(); }

			public:
				Forms::BGSKeyword** keywords; // 10 - verified: array of BGSKeyword*, see BGSKeywordForm vfuncs
				std::uint32_t numKeywords; // 18
				std::uint32_t listControlID; // 1C - dialog control ID (5320) used with GetDlgItem by the dialog virtuals
			};
			static_assert(sizeof(BGSKeywordForm) == 0x20);
			static_assert(offsetof(BGSKeywordForm, keywords) == 0x10);
			static_assert(offsetof(BGSKeywordForm, numKeywords) == 0x18);
			static_assert(offsetof(BGSKeywordForm, listControlID) == 0x1C);
		}
	}
}
