// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiMatrix.h

#pragma once

#include <EditorAPI/NiAPI/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			constexpr static auto M_PI = 3.14159265358979323846;

			namespace NiAPI
			{
				class NiMatrix3
				{
				public:
					float m_pEntry[3][3];

					inline NiMatrix3() noexcept(true) {}
					inline NiMatrix3(const NiMatrix3& Src) noexcept(true)
					{
						memcpy(&m_pEntry, Src.m_pEntry, sizeof(m_pEntry));
					}

					template<uint32_t Index>
					NiPoint3 GetCol() const noexcept(true)
					{
						static_assert(Index <= 2);

						return NiPoint3(m_pEntry[0][Index], m_pEntry[1][Index], m_pEntry[2][Index]);
					}

					NiMatrix3 Transpose() const noexcept(true)
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

					NiMatrix3 operator*(const NiMatrix3& mat) const noexcept(true)
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

					NiPoint3 operator*(const NiPoint3& Point) const noexcept(true)
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

				inline NiPoint3 operator*(const NiPoint3& Point, const NiMatrix3& Mat) noexcept(true)
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

				// 30
				class NiMatrix43
				{
				public:
					union
					{
						float data[3][4];
						float arr[12];
					};

					NiMatrix43 operator*(const NiMatrix43& rhs) const noexcept(true)
					{
						NiMatrix43 tmp{};
						tmp.data[0][0] =
							data[0][0] * rhs.data[0][0] +
							data[0][1] * rhs.data[1][0] +
							data[0][2] * rhs.data[2][0];
						tmp.data[1][0] =
							data[1][0] * rhs.data[0][0] +
							data[1][1] * rhs.data[1][0] +
							data[1][2] * rhs.data[2][0];
						tmp.data[2][0] =
							data[2][0] * rhs.data[0][0] +
							data[2][1] * rhs.data[1][0] +
							data[2][2] * rhs.data[2][0];
						tmp.data[0][1] =
							data[0][0] * rhs.data[0][1] +
							data[0][1] * rhs.data[1][1] +
							data[0][2] * rhs.data[2][1];
						tmp.data[1][1] =
							data[1][0] * rhs.data[0][1] +
							data[1][1] * rhs.data[1][1] +
							data[1][2] * rhs.data[2][1];
						tmp.data[2][1] =
							data[2][0] * rhs.data[0][1] +
							data[2][1] * rhs.data[1][1] +
							data[2][2] * rhs.data[2][1];
						tmp.data[0][2] =
							data[0][0] * rhs.data[0][2] +
							data[0][1] * rhs.data[1][2] +
							data[0][2] * rhs.data[2][2];
						tmp.data[1][2] =
							data[1][0] * rhs.data[0][2] +
							data[1][1] * rhs.data[1][2] +
							data[1][2] * rhs.data[2][2];
						tmp.data[2][2] =
							data[2][0] * rhs.data[0][2] +
							data[2][1] * rhs.data[1][2] +
							data[2][2] * rhs.data[2][2];
						return tmp;
					}

					NiMatrix43 Transpose() const noexcept(true)
					{
						NiMatrix43 result{};
						result.data[0][0] = data[0][0];
						result.data[0][1] = data[1][0];
						result.data[0][2] = data[2][0];
						result.data[0][3] = data[0][3];
						result.data[1][0] = data[0][1];
						result.data[1][1] = data[1][1];
						result.data[1][2] = data[2][1];
						result.data[1][3] = data[1][3];
						result.data[2][0] = data[0][2];
						result.data[2][1] = data[1][2];
						result.data[2][2] = data[2][2];
						result.data[2][3] = data[2][3];
						return result;
					}

					NiPoint3 operator*(const NiPoint3& pt) const
					{
						return NiPoint3
						(
							data[0][0] * pt.x + data[0][1] * pt.y + data[0][2] * pt.z,
							data[1][0] * pt.x + data[1][1] * pt.y + data[1][2] * pt.z,
							data[2][0] * pt.x + data[2][1] * pt.y + data[2][2] * pt.z
						);
					}

					void GetEulerAngles(float* heading, float* attitude, float* bank) const noexcept(true)
					{
						if (data[1][0] > 0.998) { // singularity at north pole
							*heading = atan2(data[0][2], data[2][2]);
							*attitude = (float)(M_PI / 2);
							*bank = 0;
						}
						else if (data[1][0] < -0.998) { // singularity at south pole
							*heading = atan2(data[0][2], data[2][2]);
							*attitude = (float)(-M_PI / 2);
							*bank = 0;
						}
						else {
							*heading = atan2(-data[2][0], data[0][0]);
							*bank = atan2(-data[1][2], data[1][1]);
							*attitude = asin(data[1][0]);
						}
					}
					void SetEulerAngles(float heading, float attitude, float bank) noexcept(true)
					{
						double ch = cos(heading);
						double sh = sin(heading);
						double ca = cos(attitude);
						double sa = sin(attitude);
						double cb = cos(bank);
						double sb = sin(bank);

						data[0][0] = ch * ca;
						data[0][1] = sh * sb - ch * sa * cb;
						data[0][2] = ch * sa * sb + sh * cb;
						data[1][0] = sa;
						data[1][1] = ca * cb;
						data[1][2] = -ca * sb;
						data[2][0] = -sh * ca;
						data[2][1] = sh * sa * cb + ch * sb;
						data[2][2] = -sh * sa * sb + ch * cb;
					}
				};

				class NiMatrix4
				{
				public:
					float m_pEntry[4][4];
				};
			}
		}
	}
}