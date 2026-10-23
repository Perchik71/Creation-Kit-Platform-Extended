// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSFadeNode : public NiAPI::NiNode
			{
			public:
				struct FlattenedGeometryData
				{
					/* 0x00 */ NiAPI::NiBound Bound;
					/* 0x10 */ NiAPI::NiPointer<NiAPI::NiAVObject> Geometry;
					/* 0x18 */ std::uint32_t Flags;
					/* 0x1C */ std::uint32_t pad1C;
				};
				static_assert(sizeof(FlattenedGeometryData) == 0x20);
				static_assert(offsetof(FlattenedGeometryData, Geometry) == 0x10);

				[[nodiscard]] inline float GetCurrentFade() const noexcept(true) { return _CurrentFade; }
				[[nodiscard]] inline float GetCurrentDecalFade() const noexcept(true) { return _CurrentDecalFade; }
				[[nodiscard]] inline float GetTimeSinceUpdate() const noexcept(true) { return _TimeSinceUpdate; }
				[[nodiscard]] inline std::int32_t GetFrameCounter() const noexcept(true) { return _FrameCounter; }
				[[nodiscard]] inline float GetShaderLODLevel() const noexcept(true) { return _ShaderLODLevel; }
				[[nodiscard]] inline const BSTArray<FlattenedGeometryData>& GetFlattenedGeometry() const noexcept(true) { return _Geometry; }
			protected:
				// These slots are verified; their signatures are not recovered.
				/* 46 */ virtual void Unk46();
				/* 47 */ virtual void Unk47();
				/* 48 */ virtual void Unk48();
			private:
				/* 0x140 */ std::uint8_t _unk140[0x28];
				/* 0x168 */ BSTArray<FlattenedGeometryData> _Geometry;
				/* 0x180 */ BSTArray<NiAPI::NiBound> _MergedBounds;
				/* 0x198 */ float _NearDistSqr;
				/* 0x19C */ float _FarDistSqr;
				/* 0x1A0 */ float _CurrentFade;
				/* 0x1A4 */ float _CurrentDecalFade;
				/* 0x1A8 */ std::uint32_t _unk1A8;
				/* 0x1AC */ float _TimeSinceUpdate;
				/* 0x1B0 */ std::int32_t _FrameCounter;
				/* 0x1B4 */ std::uint32_t _unk1B4;
				/* 0x1B8 */ float _ShaderLODLevel;
				/* 0x1BC */ std::uint32_t _unk1BC;
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSFadeNode, _Geometry) == 0x168 &&
						offsetof(BSFadeNode, _MergedBounds) == 0x180 &&
						offsetof(BSFadeNode, _NearDistSqr) == 0x198 &&
						offsetof(BSFadeNode, _FarDistSqr) == 0x19C &&
						offsetof(BSFadeNode, _CurrentFade) == 0x1A0 &&
						offsetof(BSFadeNode, _CurrentDecalFade) == 0x1A4 &&
						offsetof(BSFadeNode, _TimeSinceUpdate) == 0x1AC &&
						offsetof(BSFadeNode, _FrameCounter) == 0x1B0 &&
						offsetof(BSFadeNode, _ShaderLODLevel) == 0x1B8;
				}
			};
			static_assert(sizeof(BSFadeNode) == 0x1C0);
			static_assert(BSFadeNode::VerifyLayout());
		}
	}
}