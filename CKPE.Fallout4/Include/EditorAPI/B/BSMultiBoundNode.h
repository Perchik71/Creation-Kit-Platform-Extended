// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSNiNode.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSMultiBound;
			class BSMultiBoundRoom;

			class BSMultiBoundNode : public BSNiNode
			{
			public:
				/* 46 */ [[nodiscard]] virtual BSMultiBoundRoom* GetMultiBoundRoom() noexcept(true);
				/* 47 */ [[nodiscard]] virtual bool IsPointWithin(const NiAPI::NiPoint3* Point) noexcept(true);
				/* 48 */ [[nodiscard]] virtual std::uint32_t CheckMultiBound(const BSMultiBound* Bound) noexcept(true);
				/* 49 */ [[nodiscard]] virtual std::uint32_t CheckBound(const NiAPI::NiBound* Bound) noexcept(true);

				[[nodiscard]] inline const NiAPI::NiPointer<BSMultiBound>& GetMultiBound() const noexcept(true) { return _MultiBound; }
				[[nodiscard]] inline std::uint32_t GetCullingMode() const noexcept(true) { return _CullingMode; }
			private:
				/* 0x140 */ NiAPI::NiPointer<BSMultiBound> _MultiBound;
				/* 0x148 */ std::uint32_t _CullingMode;
				/* 0x14C */ float _LastAccumTime;
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSMultiBoundNode, _MultiBound) == 0x140 &&
						offsetof(BSMultiBoundNode, _CullingMode) == 0x148 &&
						offsetof(BSMultiBoundNode, _LastAccumTime) == 0x14C;
				}
			};
			static_assert(sizeof(BSMultiBoundNode) == 0x150);
			static_assert(BSMultiBoundNode::VerifyLayout());
		}
	}
}
