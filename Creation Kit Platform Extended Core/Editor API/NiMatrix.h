// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiMatrix.h

#pragma once

#include "NiPoint.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiMatrix3
		{
		public:
			float m_pEntry[3][3];

			inline NiMatrix3(){}
			inline NiMatrix3(const NiMatrix3& Src)
			{
				memcpy(&m_pEntry, Src.m_pEntry, sizeof(m_pEntry));
			}

			template<uint32_t Index>
			NiPoint3 GetCol() const
			{
				static_assert(Index <= 2);

				return NiPoint3(m_pEntry[0][Index], m_pEntry[1][Index], m_pEntry[2][Index]);
			}

			NiMatrix3 Transpose() const
			{
				// Swap [rows, cols] with [cols, rows]. Can only be optimized with AVX.
				NiMatrix3 m;

				m.m_pEntry[0][0] = m_pEntry[0][0];
				m.m_pEntry[0][1] = m_pEntry[1][0];
				m.m_pEntry[0][2] = m_pEntry[2][0];

				m.m_pEntry[1][0] = m_pEntry[0][1];
				m.m_pEntry[1][1] = m_pEntry[1][1];
				m.m_pEntry[1][2] = m_pEntry[2][1];

				m.m_pEntry[2][0] = m_pEntry[0][2];
				m.m_pEntry[2][1] = m_pEntry[1][2];
				m.m_pEntry[2][2] = m_pEntry[2][2];

				return m;
			}

			NiMatrix3 operator* (const NiMatrix3& mat) const
			{
				NiMatrix3 prd;

				prd.m_pEntry[0][0] =
					m_pEntry[0][0] * mat.m_pEntry[0][0] +
					m_pEntry[0][1] * mat.m_pEntry[1][0] +
					m_pEntry[0][2] * mat.m_pEntry[2][0];
				prd.m_pEntry[1][0] =
					m_pEntry[1][0] * mat.m_pEntry[0][0] +
					m_pEntry[1][1] * mat.m_pEntry[1][0] +
					m_pEntry[1][2] * mat.m_pEntry[2][0];
				prd.m_pEntry[2][0] =
					m_pEntry[2][0] * mat.m_pEntry[0][0] +
					m_pEntry[2][1] * mat.m_pEntry[1][0] +
					m_pEntry[2][2] * mat.m_pEntry[2][0];
				prd.m_pEntry[0][1] =
					m_pEntry[0][0] * mat.m_pEntry[0][1] +
					m_pEntry[0][1] * mat.m_pEntry[1][1] +
					m_pEntry[0][2] * mat.m_pEntry[2][1];
				prd.m_pEntry[1][1] =
					m_pEntry[1][0] * mat.m_pEntry[0][1] +
					m_pEntry[1][1] * mat.m_pEntry[1][1] +
					m_pEntry[1][2] * mat.m_pEntry[2][1];
				prd.m_pEntry[2][1] =
					m_pEntry[2][0] * mat.m_pEntry[0][1] +
					m_pEntry[2][1] * mat.m_pEntry[1][1] +
					m_pEntry[2][2] * mat.m_pEntry[2][1];
				prd.m_pEntry[0][2] =
					m_pEntry[0][0] * mat.m_pEntry[0][2] +
					m_pEntry[0][1] * mat.m_pEntry[1][2] +
					m_pEntry[0][2] * mat.m_pEntry[2][2];
				prd.m_pEntry[1][2] =
					m_pEntry[1][0] * mat.m_pEntry[0][2] +
					m_pEntry[1][1] * mat.m_pEntry[1][2] +
					m_pEntry[1][2] * mat.m_pEntry[2][2];
				prd.m_pEntry[2][2] =
					m_pEntry[2][0] * mat.m_pEntry[0][2] +
					m_pEntry[2][1] * mat.m_pEntry[1][2] +
					m_pEntry[2][2] * mat.m_pEntry[2][2];

				return prd;
			}

			NiPoint3 operator* (const NiPoint3& Point) const
			{
				return NiPoint3
				(
					m_pEntry[0][0] * Point.x + m_pEntry[0][1] * Point.y + m_pEntry[0][2] * Point.z,
					m_pEntry[1][0] * Point.x + m_pEntry[1][1] * Point.y + m_pEntry[1][2] * Point.z,
					m_pEntry[2][0] * Point.x + m_pEntry[2][1] * Point.y + m_pEntry[2][2] * Point.z
				);
			}
		};
		static_assert(sizeof(NiMatrix3) == 0x24);

		inline NiPoint3 operator*(const NiPoint3& Point, const NiMatrix3& Mat)
		{
			return NiPoint3
			(
				Point.x * Mat.m_pEntry[0][0] + Point.y * Mat.m_pEntry[1][0] +
				Point.z * Mat.m_pEntry[2][0],
				Point.x * Mat.m_pEntry[0][1] + Point.y * Mat.m_pEntry[1][1] +
				Point.z * Mat.m_pEntry[2][1],
				Point.x * Mat.m_pEntry[0][2] + Point.y * Mat.m_pEntry[1][2] +
				Point.z * Mat.m_pEntry[2][2]
			);
		}
	}
}