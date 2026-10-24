// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSTriShape.h>
#include <EditorAPI/B/BSSpinLock.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				class VertexBuffer;
			}

			// 1B0
			class BSDynamicTriShape : public BSTriShape
			{
			public:
				struct DynamicVertexBuffer
				{
					/* 0x00 */ BSGraphics::VertexBuffer* Buffer;
					/* 0x08 */ std::uint32_t Size;
					/* 0x0C */ std::uint32_t pad0C;
				};
				static_assert(sizeof(DynamicVertexBuffer) == 0x10);
				static_assert(offsetof(DynamicVertexBuffer, Size) == 0x8);

				[[nodiscard]] inline std::uint32_t GetDynamicDataSize() const noexcept(true) { return _DynamicDataSize; }
				[[nodiscard]] inline const void* GetDynamicData() const noexcept(true) { return _DynamicData; }
			private:
				/* 0x170 */ std::uint32_t _DynamicDataSize;
				/* 0x174 */ std::uint32_t _unk174;
				/* 0x178 */ BSSpinLock _DynamicDataLock;
				/* 0x180 */ void* _DynamicData;
				/* 0x188 */ NiAPI::NiPointer<NiAPI::NiRefObject> _SegmentData;
				/* 0x190 */ DynamicVertexBuffer _VertexBuffer;
				/* 0x1A0 */ std::uint8_t _unk1A0[0x10];
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSDynamicTriShape, _DynamicDataSize) == 0x170 &&
						offsetof(BSDynamicTriShape, _DynamicDataLock) == 0x178 &&
						offsetof(BSDynamicTriShape, _DynamicData) == 0x180 &&
						offsetof(BSDynamicTriShape, _SegmentData) == 0x188 &&
						offsetof(BSDynamicTriShape, _VertexBuffer) == 0x190 &&
						offsetof(BSDynamicTriShape, _unk1A0) == 0x1A0;
				}
			};
			static_assert(sizeof(BSDynamicTriShape) == 0x1B0);
			static_assert(BSDynamicTriShape::VerifyLayout());
		}
	}
}