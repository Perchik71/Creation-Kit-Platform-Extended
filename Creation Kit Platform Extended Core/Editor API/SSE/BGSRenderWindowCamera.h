// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiNode.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			using namespace NiAPI;

			class BGSRenderWindow;

			// size 0x88
			// func 1
			class BGSRenderWindowCamera
			{
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

				inline NiPoint3 GetPosition() const { return *(NiPoint3*)(((char*)(*_Data)) + 0x6C); }
				inline ViewModeT GetViewMode() const { return _ViewMode; }
				inline const char* GetViewModeByStr() const { return _ViewMode == Perspective ? "perspective" : "orthographic"; }
				inline CameraModeT GetCameraMode() const { return _CameraMode; }
				inline const char* GetCameraModeByStr() const { return CameraModeS[(int)_CameraMode]; }

				READ_PROPERTY(GetPosition) NiPoint3 Position;
				READ_PROPERTY(GetViewMode) ViewModeT ViewMode;
				READ_PROPERTY(GetCameraMode) CameraModeT CameraMode;
			private:
				NiNode** _Data;
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