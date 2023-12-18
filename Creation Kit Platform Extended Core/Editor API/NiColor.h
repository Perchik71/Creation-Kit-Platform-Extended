// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiColor.h

#pragma once

#include <xmmintrin.h>

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiColor
		{
		public:
			const static NiColor BLACK;
			const static NiColor WHITE;

			float r;
			float g;
			float b;

			inline NiColor(float initR = 0.0f, float initG = 0.0f, float initB = 0.0f)
			{
				Set(initR, initG, initB);
			}

			inline void Set(float R = 0.0f, float G = 0.0f, float B = 0.0f)
			{
				r = R;
				g = G;
				b = B;
			}
		};

		class NiColorA
		{
		public:
			const static NiColorA BLACK;
			const static NiColorA WHITE;
			const static NiColorA RED;
			const static NiColorA GREEN;
			const static NiColorA BLUE;

			float r;
			float g;
			float b;
			float a;

			inline NiColorA(float initR = 0.0f, float initG = 0.0f, float initB = 0.0f, float initA = 0.0f)
			{
				Set(initR, initG, initB, initA);
			}

			inline NiColorA(const NiColor& Src, float A)
			{
				Set(Src.r, Src.g, Src.b, A);
			}

			inline void Set(float R = 0.0f, float G = 0.0f, float B = 0.0f, float A = 0.0f)
			{
				r = R;
				g = G;
				b = B;
				a = A;
			}

			inline const float* Data() const
			{
				return &r;
			}

			inline __m128 AsXmm() const
			{
				return _mm_load_ps(Data());
			}
		};
		static_assert(sizeof(NiColorA) == 0x10);
	}
}