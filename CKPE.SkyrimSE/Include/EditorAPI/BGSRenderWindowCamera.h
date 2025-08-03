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
				enum ViewModeT : uint32_t
				{
					Perspective = 0,
					Orthographic
				};

				enum CameraModeT : uint32_t
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

				inline NiPoint3 GetPosition() const { return _Data->Position; }
				inline ViewModeT GetViewMode() const { return _ViewMode; }
				inline const char* GetViewModeByStr() const { return _ViewMode == Perspective ? "perspective" : "orthographic"; }
				inline CameraModeT GetCameraMode() const { return _CameraMode; }
				inline const char* GetCameraModeByStr() const { return CameraModeS[(int)_CameraMode]; }

				CKPE_READ_PROPERTY(GetPosition) NiPoint3 Position;
				CKPE_READ_PROPERTY(GetViewMode) ViewModeT ViewMode;
				CKPE_READ_PROPERTY(GetCameraMode) CameraModeT CameraMode;
			private:
				DataInfo* _Data;
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