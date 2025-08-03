// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiColor.h

#pragma once

#include <xmmintrin.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiColor
				{
				public:
					float r;
					float g;
					float b;

					inline NiColor(float initR = 0.0f, float initG = 0.0f, float initB = 0.0f) noexcept(true)
					{
						Set(initR, initG, initB);
					}

					inline void Set(float R = 0.0f, float G = 0.0f, float B = 0.0f) noexcept(true)
					{
						r = R;
						g = G;
						b = B;
					}
				};
				static_assert(sizeof(NiColor) == 0xC);

				class NiColorA
				{
				public:
					float r;
					float g;
					float b;
					float a;

					inline NiColorA(float initR = 0.0f, float initG = 0.0f, float initB = 0.0f, 
						float initA = 0.0f) noexcept(true)
					{
						Set(initR, initG, initB, initA);
					}

					inline NiColorA(const NiColor& Src, float A) noexcept(true)
					{
						Set(Src.r, Src.g, Src.b, A);
					}

					inline void Set(float R = 0.0f, float G = 0.0f, float B = 0.0f, float A = 0.0f) noexcept(true)
					{
						r = R;
						g = G;
						b = B;
						a = A;
					}

					[[nodiscard]] inline const float* Data() const noexcept(true) { return &r; }
					[[nodiscard]] inline __m128 AsXmm() const noexcept(true) { return _mm_load_ps(Data()); }
				};
				static_assert(sizeof(NiColorA) == 0x10);
			}
		}
	}
}