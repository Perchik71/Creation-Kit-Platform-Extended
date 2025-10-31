// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdio>
#include <EditorAPI/NiAPI/NiColor.h>
#include <EditorAPI/NiAPI/NiMatrix.h>
#include <EditorAPI/NiAPI/NiPoint.h>
#include <EditorAPI/NiAPI/NiTransform.h>
#include <EditorAPI/BSSystemFile.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiCamera
				{
				public:
					virtual ~NiCamera() = default;
				};

				class NiWindow
				{
				public:
					virtual ~NiWindow() = default;
				};

				class NiBinaryStream
				{
				protected:
					/*000*/							// --vtbls
					std::uint64_t _Pos;			
				public:
					virtual ~NiBinaryStream();
			
					[[nodiscard]] virtual bool IsOpen() = 0;
					virtual void Seek(std::ptrdiff_t numBytes) = 0;   
					[[nodiscard]] virtual std::uint64_t GetPosition();
					virtual void Cleanup();
					virtual std::uint64_t DoRead(void* buffer, std::uint64_t bytes) = 0;
					virtual std::uint64_t DoWrite(const void* buffer, std::uint64_t bytes) = 0;
				};
				static_assert(sizeof(NiBinaryStream) == 0x10);

				class NiFile : public NiBinaryStream 
				{
				public:
					enum FileModes : std::uint32_t 
					{
						kFileMode_ReadOnly = 0,
						kFileMode_WriteOnly = 1,
						kFileMode_AppendOnly = 2,
					};
				protected:
					// members
					/*000*/							// --vtbls
					std::uint64_t _BufferAllocSize;	// init to 0x800	(sent for reading/writing)
					std::uint64_t _BufferReadSize;	// init to 0		(answered by the function)
					std::uint64_t _Pos;				// init to 0
					std::uint64_t _CurrentFilePos;	// init to 0
					std::uint8_t* _Buffer;
					BSSystemFile _SysFile;
					char _FileName[256];
					FileModes _Mode;
					std::uint32_t _IsOpen;
				public:
					virtual ~NiFile();

					[[nodiscard]] inline std::uint64_t GetBufferAllocSize() const noexcept(true) { return _BufferAllocSize; }
					[[nodiscard]] inline std::uint64_t GetBufferReadSize() const noexcept(true) { return _BufferReadSize; }
					[[nodiscard]] inline std::uint64_t GetCurrentPos() const noexcept(true) { return _Pos; }
					[[nodiscard]] inline std::uint64_t GetCurrentFilePos() const noexcept(true) { return _CurrentFilePos; }
					[[nodiscard]] inline HANDLE GetHandle() const noexcept(true) { return _SysFile.Handle; }
					[[nodiscard]] inline const char* GetFileName() const noexcept(true) { return _FileName; }
					[[nodiscard]] inline virtual bool IsOpen() const noexcept(true) { return (bool)_IsOpen; }
					[[nodiscard]] virtual std::uint64_t GetSize();
				};
				static_assert(sizeof(NiFile) == 0x150);

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
						std::int32_t m_iRadiusAsInt;
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