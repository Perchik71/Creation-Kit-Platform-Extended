// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiPoint.h

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiPoint2
		{
		public:
			float x;
			float y;
		};

		class NiPoint3
		{
		public:
			const static NiPoint3 ZERO;

			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			inline NiPoint3() {}
			inline NiPoint3(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ) {}
			inline NiPoint3(const NiPoint3& Src) : x(Src.x), y(Src.y), z(Src.z) {}

			inline void Unitize()
			{
				// AKA vector normalization
				float length = sqrt((x * x) + (y * y) + (z * z));

				if (length <= 0.000001f)
				{
					x = 0.0f;
					y = 0.0f;
					z = 0.0f;
				}
				else
				{
					const float invMag = 1.0f / length;

					x *= invMag;
					y *= invMag;
					z *= invMag;
				}
			}

			inline void Normalize()
			{
				Unitize();
			}

			NiPoint3 Cross(const NiPoint3& Pt) const
			{
				return NiPoint3(y * Pt.z - z * Pt.y, z * Pt.x - x * Pt.z, x * Pt.y - y * Pt.x);
			}

			inline __m128 AsXmm() const
			{
				return _mm_setr_ps(x, y, z, 0.0f);
			}

			inline NiPoint3 operator- () const
			{
				return NiPoint3(-x, -y, -z);
			}

			inline NiPoint3 operator- (NiPoint3& Other) const
			{
				return NiPoint3(x - Other.x, y - Other.y, z - Other.z);
			}

			inline NiPoint3 operator+ (NiPoint3& Other) const
			{
				return NiPoint3(x + Other.x, y + Other.y, z + Other.z);
			}

			inline NiPoint3& operator+= (const NiPoint3& Other)
			{
				x += Other.x;
				y += Other.y;
				z += Other.z;
				return *this;
			}

			float operator* (const NiPoint3& Other) const
			{
				return x * Other.x + y * Other.y + z * Other.z;
			}

			inline NiPoint3 operator* (float Scale) const
			{
				return NiPoint3(x * Scale, y * Scale, z * Scale);
			}
		};
		static_assert(sizeof(NiPoint3) == 0xC);

		inline NiPoint3 operator* (float lhs, const NiPoint3& rhs)
		{
			return rhs * lhs;
		}

		inline NiPoint3 operator+ (const NiPoint3& lhs, const NiPoint3& rhs)
		{
			return NiPoint3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		}

		inline NiPoint3 operator- (const NiPoint3& lhs, const NiPoint3& rhs)
		{
			return NiPoint3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}
	}
}