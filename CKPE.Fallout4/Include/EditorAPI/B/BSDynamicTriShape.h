// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSTriShape.h>
#include <EditorAPI/B/BSSpinLock.h>

struct ID3D11Buffer;

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
				/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
				/* 0B */ virtual BSDynamicTriShape* IsDynamicTriShape() noexcept(true) override;
				/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
				/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
				/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSDynamicTriShape[0];

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
				[[nodiscard]] inline std::uint32_t GetLastUpdatedFrame() const noexcept(true) { return _LastUpdatedFrame; }
			private:
				/* 0x170 */ std::uint32_t _DynamicDataSize;
				/* 0x174 */ std::uint32_t _LastUpdatedFrame;
				/* 0x178 */ BSSpinLock _DynamicDataLock;
				/* 0x180 */ void* _DynamicData;
				/* 0x188 */ NiAPI::NiPointer<NiAPI::NiRefObject> _SegmentData;
				/* 0x190 */ DynamicVertexBuffer _VertexBuffer;
				/* 0x1A0 */ ID3D11Buffer* _D3DVertexBuffer;
				/* 0x1A8 */ std::uint32_t _VertexBufferOffset;
				/* 0x1AC */ std::uint32_t _pad1AC;
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSDynamicTriShape, _DynamicDataSize) == 0x170 &&
						offsetof(BSDynamicTriShape, _LastUpdatedFrame) == 0x174 &&
						offsetof(BSDynamicTriShape, _DynamicDataLock) == 0x178 &&
						offsetof(BSDynamicTriShape, _DynamicData) == 0x180 &&
						offsetof(BSDynamicTriShape, _SegmentData) == 0x188 &&
						offsetof(BSDynamicTriShape, _VertexBuffer) == 0x190 &&
						offsetof(BSDynamicTriShape, _D3DVertexBuffer) == 0x1A0 &&
						offsetof(BSDynamicTriShape, _VertexBufferOffset) == 0x1A8;
				}
			};
			static_assert(sizeof(BSDynamicTriShape) == 0x1B0);
			static_assert(BSDynamicTriShape::VerifyLayout());
		}
	}
}