// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/N/NiRefObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSPortalGraphEntry : public NiAPI::NiRefObject
			{
			public:
				[[nodiscard]] inline class BSPortalGraph* GetPortalGraph() const noexcept(true) { return _PortalGraph; }
				[[nodiscard]] inline bool HasVisibleUnboundSpace() const noexcept(true) { return _VisibleUnboundSpace; }
				[[nodiscard]] inline bool IsAccumulationEnabled() const noexcept(true) { return _EnableAccumulation; }
				CKPE_READ_PROPERTY(GetPortalGraph) class BSPortalGraph* PortalGraph;

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSPortalGraphEntry, _PortalGraph) == 0x10 &&
						offsetof(BSPortalGraphEntry, _RoomAccumulation) == 0x18 &&
						offsetof(BSPortalGraphEntry, _RoomRoot) == 0x30 &&
						offsetof(BSPortalGraphEntry, _VisibleUnboundSpace) == 0x140 &&
						offsetof(BSPortalGraphEntry, _EnableAccumulation) == 0x141 &&
						offsetof(BSPortalGraphEntry, _WorldID) == 0x148;
				}
			private:
				/* 0x010 */ class BSPortalGraph* _PortalGraph;
				/* 0x018 */ BSTArray<class BSMultiBoundRoom*> _RoomAccumulation;
				/* 0x030 */ class BSMultiBoundRoom* _RoomRoot;
				/* 0x038 */ char pad038[0x38];
				/* 0x070 */ char pad070[0xD0];
				/* 0x140 */ bool _VisibleUnboundSpace;
				/* 0x141 */ bool _EnableAccumulation;
				/* 0x142 */ char pad142[0x6];
				/* 0x148 */ std::uint32_t _WorldID;
				/* 0x14C */ char pad14C[0x4];
			};
			static_assert(sizeof(BSPortalGraphEntry) == 0x150);
			static_assert(BSPortalGraphEntry::VerifyLayout());
		}
	}
}
