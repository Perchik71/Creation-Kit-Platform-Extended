// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include "NiAPI/NiNode.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			class BGSRenderWindow;

			// size 0x88
			// func 1
			class BGSRenderWindowCamera
			{
			private:
				struct DataInfo
				{
					char pad00[0x6C];
					NiPoint3 Position;
				};
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

				[[nodiscard]] inline NiNode* GetNode() const noexcept(true) { return _Node; }
				[[nodiscard]] inline NiMatrix3 GetRotate() const noexcept(true) { return _Node->GetLocalTransform().m_Rotate; }
				[[nodiscard]] inline NiPoint3 GetPosition() const noexcept(true) { return _Node->GetLocalTransform().m_Translate; }
				[[nodiscard]] inline ViewModeT GetViewMode() const noexcept(true) { return _ViewMode; }
				[[nodiscard]] inline const char* GetViewModeByStr() const noexcept(true) { return _ViewMode == Perspective ? "perspective" : "orthographic"; }
				[[nodiscard]] inline CameraModeT GetCameraMode() const noexcept(true) { return _CameraMode; }
				[[nodiscard]] inline const char* GetCameraModeByStr() const noexcept(true) { return CameraModeS[(int)_CameraMode]; }

				CKPE_READ_PROPERTY(GetNode) NiNode* Node;
				CKPE_READ_PROPERTY(GetRotate) NiMatrix3 Rotate;
				CKPE_READ_PROPERTY(GetPosition) NiPoint3 Position;
				CKPE_READ_PROPERTY(GetViewMode) ViewModeT ViewMode;
				CKPE_READ_PROPERTY(GetCameraMode) CameraModeT CameraMode;
			private:
				NiNode* _Node;
				NiCamera* _Camera;
				ViewModeT _ViewMode;
				CameraModeT _CameraMode;
				float _Nums[14];
				BGSRenderWindow* _RenderWindow;
				float _Nums2;
				char pad64[0x20];
			};
			static_assert(sizeof(BGSRenderWindowCamera) == 0x88);
		}
	}
}