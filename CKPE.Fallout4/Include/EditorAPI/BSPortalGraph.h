// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSTArray.h"
#include "NiAPI/NiRefObject.h"
#include "NiAPI/NiPointer.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiAVObject;
				class NiNode;
			}

			class BSPortalGraph : public NiAPI::NiRefObject
			{
				char _pad[0x30];
				BSTArray<void*> _unk40;
				BSTArray<void*> _unk58;
				NiAPI::NiPointer<NiAPI::NiNode> _PortalSharedNode;		// 70
				BSTArray<NiAPI::NiPointer<NiAPI::NiAVObject>> unk78;    // 78
				BSTArray<NiAPI::NiPointer<NiAPI::NiAVObject>> unk90;    // 90
				BSTArray<void*> _unkA8;									// A8
				std::uint32_t _CellID;									// C0
			public:
				virtual ~BSPortalGraph() = default;

				[[nodiscard]] inline std::uint32_t GetCellFormID() const noexcept(true) { return _CellID; }
			};
			static_assert(sizeof(BSPortalGraph) == 0xC8);
		}
	}
}
