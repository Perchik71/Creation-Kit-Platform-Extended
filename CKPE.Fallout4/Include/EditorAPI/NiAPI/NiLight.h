// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAVObject.h"
#include "NiColor.h"
#include "NiPoint.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiLight : public NiAVObject
				{
					NiColor _Ambient;		// 0x120
					NiColor _Diffuse;		// 0x12C
					NiPoint3 _Radius;		// 0x138
					float _Fade;			// 0x144
					float _unk1[5];
					float _Time;			// 0x15C
					float _unk2[4];
				public:
					virtual ~NiLight() = default;

					[[nodiscard]] inline NiColor GetAmbient() const noexcept(true) { return _Ambient; }
					[[nodiscard]] inline NiColor GetDiffuse() const noexcept(true) { return _Diffuse; }
					[[nodiscard]] inline NiPoint3 GetRadius() const noexcept(true) { return _Radius; }
					[[nodiscard]] inline NiColor& GetAmbient() noexcept(true) { return _Ambient; }
					[[nodiscard]] inline NiColor& GetDiffuse() noexcept(true) { return _Diffuse; }
					[[nodiscard]] inline NiPoint3& GetRadius() noexcept(true) { return _Radius; }
					[[nodiscard]] inline float GetFade() const noexcept(true) { return _Fade; }
					[[nodiscard]] inline float GetTime() const noexcept(true) { return _Time; }
				};
				static_assert(sizeof(NiLight) == 0x170);
			}
		}
	}
}