// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSRenderOrthoGrid
			{
			public:
				virtual ~BGSRenderOrthoGrid() = default;

				[[nodiscard]] inline NiAPI::NiNode* GetNode() const noexcept(true) { return _Node; }
				[[nodiscard]] inline float GetSpacing() const noexcept(true) { return _Spacing; }
				CKPE_READ_PROPERTY(GetNode) NiAPI::NiNode* Node;
				CKPE_READ_PROPERTY(GetSpacing) float Spacing;

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BGSRenderOrthoGrid, _Node) == 0x10 &&
						offsetof(BGSRenderOrthoGrid, _Spacing) == 0x24 &&
						offsetof(BGSRenderOrthoGrid, _Entries) == 0x38;
				}
			private:
				/* 0x008 */ NiAPI::NiRefObject* _Object08;
				/* 0x010 */ NiAPI::NiNode* _Node;
				/* 0x018 */ class BSDynamicLines* _DynamicLines;
				/* 0x020 */ char pad020[0x4];
				/* 0x024 */ float _Spacing;
				/* 0x028 */ NiAPI::NiColorA _Color;
				/* 0x038 */ BSTArray<void*> _Entries;
				/* 0x050 */ char pad050[0x18];
			};
			static_assert(sizeof(BGSRenderOrthoGrid) == 0x68);
			static_assert(BGSRenderOrthoGrid::VerifyLayout());
		}
	}
}
