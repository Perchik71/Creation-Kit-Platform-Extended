// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiLight.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiPointLight : public NiLight
				{
					float _ConstAttenuation;		// 0x170
					float _LinearAttenuation;		// 0x174
					float _QuadraticAttenuation;	// 0x178
					float _Unk[5];
				public:
					virtual ~NiPointLight() = default;

					[[nodiscard]] inline float GetConstAttenuation() const noexcept(true) { return _ConstAttenuation; }
					[[nodiscard]] inline float GetLinearAttenuation() const noexcept(true) { return _LinearAttenuation; }
					[[nodiscard]] inline float GetQuadraticAttenuation() const noexcept(true) { return _QuadraticAttenuation; }
				};
				static_assert(sizeof(NiPointLight) == 0x190);
			}
		}
	}
}
