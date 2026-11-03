// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiAVObject.h>
#include <EditorAPI/N/NiMatrix.h>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				// 188
				class NiCamera : public NiAVObject
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::NiCamera;
					constexpr static Common::ID VTABLE = IDs::VTABLE::NiCamera[0];

					virtual ~NiCamera() = default;

					// 110
					NiMatrix4 m_kWorldToCam;
					// 150, a frustum set through the camera is limited: near = max(near, far / m_fMaxFarNearRatio, m_fMinNearPlaneDist)
					NiFrustum m_kViewFrustum;
					// 16C, 0.1 after the constructor
					float m_fMinNearPlaneDist;
					// 170, 10000.0 after the constructor
					float m_fMaxFarNearRatio;
					// 174, left 0, right 1, top 1, bottom 0 after the constructor
					NiRect<float> m_kPort;
					// 184, 1.0 after the constructor
					float m_fLODAdjust;
				};
				static_assert(offsetof(NiCamera, m_kWorldToCam) == 0x110);
				static_assert(offsetof(NiCamera, m_kViewFrustum) == 0x150);
				static_assert(offsetof(NiCamera, m_fMinNearPlaneDist) == 0x16C);
				static_assert(offsetof(NiCamera, m_fMaxFarNearRatio) == 0x170);
				static_assert(offsetof(NiCamera, m_kPort) == 0x174);
				static_assert(offsetof(NiCamera, m_fLODAdjust) == 0x184);
				static_assert(sizeof(NiCamera) == 0x188);
			}
		}
	}
}
