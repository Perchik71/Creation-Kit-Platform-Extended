// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/N/NiCamera.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			class BGSRenderWindow;

			// size 0x88
			// func 1
			class BGSRenderWindowCamera
			{
			public:
				enum ViewModeT : std::uint32_t
				{
					Perspective = 0,
					Orthographic
				};

				enum CameraModeT : std::uint32_t
				{
					Front = 0,
					Left,
					Top,
					Free,
					North,
					West
				};

				inline static const char* CameraModeS[8] =
				{
					"Front view",
					"Left view",
					"Top view",
					"Free camera",
					"North view",
					"West view",
					"Top view",
					"Free camera",
				};
			public:
				virtual ~BGSRenderWindowCamera() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSRenderWindowCamera[0];

				[[nodiscard]] inline NiNode* GetNode() const noexcept(true) { return _Node; }
				[[nodiscard]] inline NiCamera* GetCamera() const noexcept(true) { return _Camera; }
				[[nodiscard]] inline NiMatrix43 GetRotate() const noexcept(true) { return _Node->GetLocalTransform().m_Rotate; }
				[[nodiscard]] inline NiPoint3 GetPosition() const noexcept(true) { return _Node->GetLocalTransform().m_Translate; }
				[[nodiscard]] inline ViewModeT GetViewMode() const noexcept(true) { return _ViewMode; }
				[[nodiscard]] inline const char* GetViewModeByStr() const noexcept(true) { return _ViewMode == Perspective ? "perspective" : "orthographic"; }
				[[nodiscard]] inline CameraModeT GetCameraMode() const noexcept(true) { return _CameraMode; }
				[[nodiscard]] inline const char* GetCameraModeByStr() const noexcept(true) { return CameraModeS[std::to_underlying(_CameraMode)]; }
				[[nodiscard]] inline const NiFrustum& GetOrthoFrustum() const noexcept(true) { return _OrthoFrustum; }
				[[nodiscard]] inline const NiFrustum& GetPerspectiveFrustum() const noexcept(true) { return _PerspectiveFrustum; }
				[[nodiscard]] inline float GetOrthoScale() const noexcept(true) { return _OrthoScale; }
				[[nodiscard]] inline const NiPoint3& GetPivot() const noexcept(true) { return _Pivot; }
				[[nodiscard]] inline bool HasPivot() const noexcept(true) { return _HasPivot; }
				[[nodiscard]] inline bool IsZoomingToPivot() const noexcept(true) { return _ZoomToPivot; }

				CKPE_READ_PROPERTY(GetNode) NiNode* Node;
				CKPE_READ_PROPERTY(GetCamera) NiCamera* Camera;
				CKPE_READ_PROPERTY(GetRotate) NiMatrix43 Rotate;
				CKPE_READ_PROPERTY(GetPosition) NiPoint3 Position;
				CKPE_READ_PROPERTY(GetViewMode) ViewModeT ViewMode;
				CKPE_READ_PROPERTY(GetCameraMode) CameraModeT CameraMode;
				CKPE_READ_PROPERTY(GetOrthoFrustum) const NiFrustum& OrthoFrustum;
				CKPE_READ_PROPERTY(GetPerspectiveFrustum) const NiFrustum& PerspectiveFrustum;
				CKPE_READ_PROPERTY(GetOrthoScale) float OrthoScale;
				CKPE_READ_PROPERTY(GetPivot) const NiPoint3& Pivot;

				inline void Update() noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::Update.Address(), this);
				}

				inline void SetPosition(const NiPoint3& Position) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::SetPosition.Address(), this, &Position);
				}

				inline void SetRotation(const NiMatrix43& Rotation) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::SetRotation.Address(), this, &Rotation);
				}

				inline void LookAt(const NiPoint3& Target) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::LookAt.Address(), this, &Target);
				}

				inline void FramePoint(const NiPoint3& Center, float Distance) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::FramePoint.Address(), this, &Center, Distance);
				}

				inline void SetPivot(const NiPoint3& Pivot, float DistanceScale,
					bool Reposition) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::SetPivot.Address(),
						this, &Pivot, DistanceScale, Reposition);
				}

				inline void ClearPivot() noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::ClearPivot.Address(), this);
				}

				inline void Pan(float X, float Y) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::Pan.Address(), this, X, Y);
				}

				inline void Zoom(float Amount) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::Zoom.Address(), this, Amount);
				}

				inline void RotateBy(float X, float Y, float Z) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::RotateBy.Address(), this, X, Y, Z);
				}

				inline void MoveLocal(float X, float Y, float Z) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::MoveLocal.Address(), this, X, Y, Z);
				}

				inline void MoveWorld(float X, float Y, float Z) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::MoveWorld.Address(), this, X, Y, Z);
				}

				inline void SetViewMode(ViewModeT Mode) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::SetViewMode.Address(), this, Mode);
				}

				// This stores the framing mode; FramePoint applies its orientation.
				inline void SetCameraMode(CameraModeT Mode) noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::SetCameraMode.Address(), this, Mode);
				}

				inline void UpdateOrthoFrustum() noexcept(true)
				{
					CKPE::fast_call<void>(EditorAPI::IDs::BGSRenderWindowCamera::UpdateOrthoFrustum.Address(), this);
				}

			private:
				/* 0x008 */ NiNode* _Node;
				/* 0x010 */ NiCamera* _Camera;
				/* 0x018 */ ViewModeT _ViewMode;
				/* 0x01C */ CameraModeT _CameraMode;
				/* 0x020 */ NiFrustum _OrthoFrustum;
				/* 0x03C */ NiFrustum _PerspectiveFrustum;
				/* 0x058 */ BGSRenderWindow* _RenderWindow;
				/* 0x060 */ float _OrthoScale;
				/* 0x064 */ std::uint32_t _Unknown64;
				/* 0x068 */ NiPoint3 _Pivot;
				/* 0x074 */ bool _HasPivot;
				/* 0x075 */ bool _ZoomToPivot;
				/* 0x076 */ std::uint8_t _Pad76[0x2];
				/* 0x078 */ NiPoint3 _PivotDirection;
				/* 0x084 */ std::uint8_t _Pad84[0x4];

			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BGSRenderWindowCamera, _Node) == 0x8 &&
						offsetof(BGSRenderWindowCamera, _Camera) == 0x10 &&
						offsetof(BGSRenderWindowCamera, _ViewMode) == 0x18 &&
						offsetof(BGSRenderWindowCamera, _CameraMode) == 0x1C &&
						offsetof(BGSRenderWindowCamera, _OrthoFrustum) == 0x20 &&
						offsetof(BGSRenderWindowCamera, _PerspectiveFrustum) == 0x3C &&
						offsetof(BGSRenderWindowCamera, _RenderWindow) == 0x58 &&
						offsetof(BGSRenderWindowCamera, _OrthoScale) == 0x60 &&
						offsetof(BGSRenderWindowCamera, _Pivot) == 0x68 &&
						offsetof(BGSRenderWindowCamera, _HasPivot) == 0x74 &&
						offsetof(BGSRenderWindowCamera, _ZoomToPivot) == 0x75 &&
						offsetof(BGSRenderWindowCamera, _PivotDirection) == 0x78;
				}
			};
			static_assert(sizeof(BGSRenderWindowCamera) == 0x88);
			static_assert(BGSRenderWindowCamera::VerifyLayout());
		}
	}
}