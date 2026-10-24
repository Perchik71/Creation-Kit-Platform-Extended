// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/N/NiObject.h>

namespace CKPE::Fallout4::EditorAPI::NiAPI
{
	class NiCamera;
	class NiBound;
	class NiVisibleArray;

	class NiAccumulator : public NiObject
	{
	public:
		NiAccumulator() = delete;

		/* 2A */ virtual void StartAccumulating(const NiCamera* Camera);
		/* 2B */ virtual void FinishAccumulating();
		/* 2C */ virtual void RegisterObjectArray(NiVisibleArray* Objects);
		/* 2D */ virtual void StartGroupingAlphas(const NiBound* Bound,
			bool SortByClosestPoint);
		/* 2E */ virtual void StopGroupingAlphas();
		/* 2F */ virtual void RegisterObject(BSGeometry* Geometry);

		/* 0x010 */ const NiCamera* camera;
	};
	static_assert(sizeof(NiAccumulator) == 0x18);
	static_assert(offsetof(NiAccumulator, camera) == 0x10);

	class NiBackToFrontAccumulator : public NiAccumulator
	{
	public:
		/* 0x018 */ std::byte _pad018[0x18];
		/* 0x030 */ std::int32_t numItems;
		/* 0x034 */ std::int32_t maxItems;
		/* 0x038 */ BSGeometry** items;
		/* 0x040 */ float* depths;
		/* 0x048 */ std::int32_t currentItem;
		/* 0x04C */ std::byte _pad04C[4];
	};
	static_assert(sizeof(NiBackToFrontAccumulator) == 0x50);
	static_assert(offsetof(NiBackToFrontAccumulator, items) == 0x38);
	static_assert(offsetof(NiBackToFrontAccumulator, depths) == 0x40);

	class NiAlphaAccumulator : public NiBackToFrontAccumulator
	{
	public:
		/* 0x050 */ bool observeNoSortHint;
		/* 0x051 */ bool sortByClosestPoint;
		/* 0x052 */ bool interfaceSort;
		/* 0x053 */ std::byte _pad053[5];
	};
	static_assert(sizeof(NiAlphaAccumulator) == 0x58);
	static_assert(offsetof(NiAlphaAccumulator, observeNoSortHint) == 0x50);
}
