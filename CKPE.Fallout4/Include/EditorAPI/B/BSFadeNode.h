// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSShaderPropertyLightData.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSTreeNode;
			class BSLeafAnimNode;

			class BSFadeNode : public NiAPI::NiNode
			{
			public:
				/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
				/* 06 */ virtual BSFadeNode* IsFadeNode() noexcept(true) override;
				/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
				/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
				/* 32 */ virtual void UpdateDownwardPass(NiAPI::NiUpdateData& Data, std::uint32_t Flags) noexcept(true) override;
				/* 33 */ virtual void UpdateSelectedDownwardPass(NiAPI::NiUpdateData& Data, std::uint32_t Flags) noexcept(true) override;
				/* 37 */ virtual void UpdateTransformAndBounds(NiAPI::NiUpdateData& Data) noexcept(true) override;
				/* 3B */ virtual void OnVisible(NiAPI::NiCullingProcess& Culler) noexcept(true) override;
				/* 44 */ virtual void UpdateUpwardPass(NiAPI::NiUpdateData& Data) noexcept(true) override;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSShaderMaterial[0];

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
				[[nodiscard]] inline float GetPreviousShaderLODLevel() const noexcept(true) { return _PreviousShaderLODLevel; }
				[[nodiscard]] inline float GetBoundRadius() const noexcept(true) { return _BoundRadius; }
				[[nodiscard]] inline const BSShaderPropertyLightData& GetLightData() const noexcept(true) { return _LightData; }
				[[nodiscard]] inline const BSTArray<FlattenedGeometryData>& GetFlattenedGeometry() const noexcept(true) { return _Geometry; }
			protected:
				// Likely AsTreeNode; the tree override returns this.
				/* 46 */ virtual void sub_230();
				// Likely AsLeafAnimNode; the const distinction is unproved.
				/* 47 */ virtual void sub_238();
				// Likely AsLeafAnimNode; both leaf overrides return this.
				/* 48 */ virtual void sub_240();
			private:
				/* 0x140 */ BSShaderPropertyLightData _LightData;
				/* 0x168 */ BSTArray<FlattenedGeometryData> _Geometry;
				/* 0x180 */ BSTArray<NiAPI::NiBound> _MergedBounds;
				/* 0x198 */ float _NearDistSqr;
				/* 0x19C */ float _FarDistSqr;
				/* 0x1A0 */ float _CurrentFade;
				/* 0x1A4 */ float _CurrentDecalFade;
				/* 0x1A8 */ float _BoundRadius;
				/* 0x1AC */ float _TimeSinceUpdate;
				/* 0x1B0 */ std::int32_t _FrameCounter;
				/* 0x1B4 */ float _PreviousFade;
				/* 0x1B8 */ float _ShaderLODLevel;
				/* 0x1BC */ float _PreviousShaderLODLevel;
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSFadeNode, _LightData) == 0x140 &&
						offsetof(BSFadeNode, _Geometry) == 0x168 &&
						offsetof(BSFadeNode, _MergedBounds) == 0x180 &&
						offsetof(BSFadeNode, _NearDistSqr) == 0x198 &&
						offsetof(BSFadeNode, _FarDistSqr) == 0x19C &&
						offsetof(BSFadeNode, _CurrentFade) == 0x1A0 &&
						offsetof(BSFadeNode, _CurrentDecalFade) == 0x1A4 &&
						offsetof(BSFadeNode, _BoundRadius) == 0x1A8 &&
						offsetof(BSFadeNode, _TimeSinceUpdate) == 0x1AC &&
						offsetof(BSFadeNode, _FrameCounter) == 0x1B0 &&
						offsetof(BSFadeNode, _PreviousFade) == 0x1B4 &&
						offsetof(BSFadeNode, _ShaderLODLevel) == 0x1B8 &&
						offsetof(BSFadeNode, _PreviousShaderLODLevel) == 0x1BC;
				}
			};
			static_assert(sizeof(BSFadeNode) == 0x1C0);
			static_assert(BSFadeNode::VerifyLayout());
		}
	}
}