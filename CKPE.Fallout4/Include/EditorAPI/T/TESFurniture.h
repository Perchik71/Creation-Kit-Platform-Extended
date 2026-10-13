// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectACTI.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSAttachParentArray.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;

				// 1.11.240.0: size 0x2A0, vtable RVA 0x308AC68, 154 virtual functions
				class TESFurniture :
					public TESObjectACTI   // 000
				{
				public:
					virtual ~TESFurniture() = default;

				public:
					BSTArray<void*> unk1E8; // 1E8 - element type not identified yet
					TESModel unk200; // 200
					BSTArray<void*> entryPointDataArray; // 238 - BSTArray<EntryPointData>, by position (same member order and sizes as the game after the last base)
					BGSAttachParentArray attachParents; // 250
					std::int8_t wbData; // 270 - WorkbenchData::type, by position (same member order and sizes as the game after the last base)
					std::uint8_t unk271[0x3]; // 271
					std::uint32_t furnFlags; // 274 - by position (same member order and sizes as the game after the last base)
					TESForm* associatedForm; // 278 - by position (same member order and sizes as the game after the last base)
					BSTArray<void*> markersArray; // 280 - BSTArray<BSFurnitureMarker>, by position (same member order and sizes as the game after the last base)
					std::uint16_t unk298; // 298
					std::uint8_t unk29A; // 29A
					std::uint8_t unk29B[0x5]; // 29B
				};
				static_assert(sizeof(TESFurniture) == 0x2A0);
			}
		}
	}
}
