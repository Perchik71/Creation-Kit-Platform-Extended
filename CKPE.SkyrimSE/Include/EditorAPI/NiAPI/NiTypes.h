// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdio>
#include <EditorAPI/NiAPI/NiColor.h>
#include <EditorAPI/NiAPI/NiMatrix.h>
#include <EditorAPI/NiAPI/NiPoint.h>
#include <EditorAPI/NiAPI/NiTransform.h>
#include <EditorAPI/NiAPI/NiRefObject.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiCamera
				{
				public:
					virtual ~NiCamera();
				};

				class NiWindow
				{
				public:
					virtual ~NiWindow();
				};

				class NiBinaryStream
				{
				protected:
					/*000*/							// --vtbls
					std::int32_t _Pos;				// negative
					char pad0C[4];
					void* _lpfnReadFunc;
					void* _lpfnWriteFunc;
				public:
					virtual ~NiBinaryStream();
			
					[[nodiscard]] virtual bool IsOpen() = 0;
					virtual void Unk10() = 0;
					virtual std::int32_t GetPositionNeg();
					virtual void Cleanup();
					virtual void InitReadWriteFunction(bool mode) = 0;
				};
				static_assert(sizeof(NiBinaryStream) == 0x20);

				class NiFile : public NiBinaryStream 
				{
				public:
					enum FileModes {
						kFileMode_ReadOnly = 0,
						kFileMode_WriteOnly = 1,
						kFileMode_AppendOnly = 2,
					};
				protected:
					// members
					/*000*/							// --vtbls
					std::uint32_t _BufferAllocSize;	// init to 0x800	(sent for reading/writing)
					std::uint32_t _BufferReadSize;	// init to 0		(answered by the function)
					std::uint32_t _BufferOffset;	// init to 0
					std::uint32_t _SizeBuffer;		// init to 0
					char* _Buffer;
					FILE* _Handle; 
					std::uint32_t _Unk;
					std::uint32_t _IsOpen;
				public:
					virtual ~NiFile();

					[[nodiscard]] inline std::uint32_t GetBufferAllocSize() const noexcept(true) { return _BufferAllocSize; }
					[[nodiscard]] inline std::uint32_t GetBufferReadSize() const noexcept(true) { return _BufferReadSize; }
					[[nodiscard]] inline std::uint32_t GetOffsetOfTheBuffer() const noexcept(true) { return _BufferOffset; }
					[[nodiscard]] inline std::uint32_t GetSizeBuffer() const noexcept(true) { return _SizeBuffer; }
					[[nodiscard]] inline FILE* GetHandle() const noexcept(true) { return _Handle; }
					[[nodiscard]] inline virtual bool IsOpen() const noexcept(true) { return (bool)_IsOpen; }
					virtual void Unk10();
					[[nodiscard]] virtual std::uint32_t GetPosition();
					[[nodiscard]] virtual std::uint32_t GetSize();
				};
				static_assert(sizeof(NiFile) == 0x48);

				// 3
				struct NiRGB
				{
					union
					{
						struct { std::uint8_t r, g, b; };
						struct { std::uint8_t v[3]; };
					};
				};

				// 4
				struct NiRGBA
				{
					union
					{
						struct { std::uint8_t r, g, b, a; };
						struct { std::uint8_t v[4]; };
						std::uint32_t c;
					};
				};

				// 10
				template <class T>
				class NiRect 
				{
				public:
					T m_left;		// 00
					T m_right;		// 04
					T m_top;		// 08
					T m_bottom;		// 0C
				};

				// 1C
				class NiFrustum 
				{
				public:
					float	m_fLeft;	// 00
					float	m_fRight;	// 04
					float	m_fTop;		// 08
					float	m_fBottom;	// 0C
					float	m_fNear;	// 10
					float	m_fFar;		// 14
					bool	m_bOrtho;	// 18
				};

				// 10
				class NiQuaternion 
				{
				public:
					// w is first

					float	m_fW;	// 0
					float	m_fX;	// 4
					float	m_fY;	// 8
					float	m_fZ;	// C

					NiQuaternion operator*(const NiQuaternion& q2) const noexcept(true)
					{
						NiQuaternion tmp{};
						tmp.m_fX = m_fX * q2.m_fW + m_fY * q2.m_fZ - m_fZ * q2.m_fY + m_fW * q2.m_fX;
						tmp.m_fY = -m_fX * q2.m_fZ + m_fY * q2.m_fW + m_fZ * q2.m_fX + m_fW * q2.m_fY;
						tmp.m_fZ = m_fX * q2.m_fY - m_fY * q2.m_fX + m_fZ * q2.m_fW + m_fW * q2.m_fZ;
						tmp.m_fW = -m_fX * q2.m_fX - m_fY * q2.m_fY - m_fZ * q2.m_fZ + m_fW * q2.m_fW;
						return tmp;
					}

					NiPoint3 operator* (const NiPoint3& pt) const noexcept(true)
					{
						NiPoint3 tmp{};
						tmp.x = m_fW * m_fW * pt.x + 2 * m_fY * m_fW * pt.z - 2 * m_fZ * m_fW * pt.y + m_fX * m_fX * pt.x + 2 * m_fY * m_fX * pt.y + 2 * m_fZ * m_fX * pt.z - m_fZ * m_fZ * pt.x - m_fY * m_fY * pt.x;
						tmp.y = 2 * m_fX * m_fY * pt.x + m_fY * m_fY * pt.y + 2 * m_fZ * m_fY * pt.z + 2 * m_fW * m_fZ * pt.x - m_fZ * m_fZ * pt.y + m_fW * m_fW * pt.y - 2 * m_fX * m_fW * pt.z - m_fX * m_fX * pt.y;
						tmp.z = 2 * m_fX * m_fZ * pt.x + 2 * m_fY * m_fZ * pt.y + m_fZ * m_fZ * pt.z - 2 * m_fW * m_fY * pt.x - m_fY * m_fY * pt.z + 2 * m_fW * m_fX * pt.y - m_fX * m_fX * pt.z + m_fW * m_fW * pt.z;
						return tmp;
					}

					void SetEulerAngles(float pitch, float roll, float yaw) noexcept(true)
					{
						float t0 = std::cos(yaw * 0.5);
						float t1 = std::sin(yaw * 0.5);
						float t2 = std::cos(roll * 0.5);
						float t3 = std::sin(roll * 0.5);
						float t4 = std::cos(pitch * 0.5);
						float t5 = std::sin(pitch * 0.5);

						m_fW = t0 * t2 * t4 + t1 * t3 * t5;
						m_fX = t0 * t3 * t4 - t1 * t2 * t5;
						m_fY = t0 * t2 * t5 + t1 * t3 * t4;
						m_fZ = t1 * t2 * t4 - t0 * t3 * t5;
					}

					void GetEulerAngles(float& roll, float& pitch, float& yaw) const noexcept(true)
					{
						float ysqr = m_fY * m_fY;

						// roll (x-axis rotation)
						float t0 = +2.0 * (m_fW * m_fX + m_fY * m_fZ);
						float t1 = +1.0 - 2.0 * (m_fX * m_fX + ysqr);
						roll = std::atan2(t0, t1);

						// pitch (y-axis rotation)
						float t2 = +2.0 * (m_fW * m_fY - m_fZ * m_fX);
						t2 = t2 > 1.0 ? 1.0 : t2;
						t2 = t2 < -1.0 ? -1.0 : t2;
						pitch = std::asin(t2);

						// yaw (z-axis rotation)
						float t3 = +2.0 * (m_fW * m_fZ + m_fX * m_fY);
						float t4 = +1.0 - 2.0 * (ysqr + m_fZ * m_fZ);
						yaw = std::atan2(t3, t4);
					}
				};

				// 10
				class NiBound 
				{
				public:
					NiPoint3 m_kCenter;
					union {
						float m_fRadius;
						int m_iRadiusAsInt;
					};
				};

				// 10
				struct NiPlane 
				{
					NiPoint3 m_kNormal;
					float m_fConstant;
				};
			}
		}
	}
}

#pragma warning(default : 26451)