//////////////////////////////////////////
/*
* Fallout 4 Script Extender (F4SE)
* by Ian Patterson, Stephen Abel, and Brendan Borthwick (ianpatt, behippo, and purplelunchbox)
*
* Contact the F4SE Team
*
* Entire Team
* Send email to team [at] f4se [dot] silverlock [dot] org
*
* Ian (ianpatt)
* Send email to ianpatt+f4se [at] gmail [dot] com
*
* Stephen (behippo)
* Send email to gamer [at] silverlock [dot] org
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 1)

#include "NiColor.h"
#include "NiMatrix.h"
#include "NiPoint.h"
#include "NiTransform.h"
#include "NiArray.h"
#include "NiTArray.h"
#include "NiRefObject.h"

namespace CreationKitPlatformExtended
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

		class NiBinaryStream {
		public:
			virtual ~NiBinaryStream();
		public:
			virtual bool IsValid() = 0;
		};

		class NiFile : public NiBinaryStream {
		public:
			virtual ~NiFile();
		public:
			enum FileModes {
				kFileMode_ReadOnly = 0,
				kFileMode_WriteOnly = 1,
				kFileMode_AppendOnly = 2,
			};
		protected:
			// members
			/*000*/ // --vtbls
			/*008*/ uint64_t _Pos;				// init to 0
			/*010*/ uint64_t _BufferAllocSize;	// init to 0x800	(sent for reading/writing)
			/*018*/ uint64_t _BufferReadSize;	// init to 0		(answered by the function)
			/*020*/ uint64_t _BufferOffset;		// init to 0
			/*028*/ uint64_t _SizeBuffer;		// init to 0
			/*030*/ void* _Buffer;				// allocated/deallocated on form heap by constructor/destructor
			/*038*/ uint64_t _dwUnk38;
			/*040*/ HANDLE _FileHandle;
			/*048*/ char _FileName[MAX_PATH];
			/*14C*/ uint32_t _Good;				// true if file is exist or no error
		public:
			inline uint64_t GetPosition() const { return _Pos; }
			inline uint64_t GetOffsetOfTheBuffer() const { return _BufferOffset; }
			inline uint64_t GetSizeBuffer() const { return _SizeBuffer; }
			inline HANDLE GetHandle() const { return _FileHandle; }
			inline const char* GetFileName() const { return _FileName; }
			inline bool IsGood() const { return (bool)_Good; }
		public:
			virtual bool IsValid() { return IsGood(); }
		};

		// 3
		struct NiRGB 
		{
			union
			{
				struct { uint8_t r, g, b; };
				struct { uint8_t v[3]; };
			};
		};

		// 4
		struct NiRGBA 
		{
			union
			{
				struct { uint8_t r, g, b, a; };
				struct { uint8_t v[4]; };
				uint32_t c;
			};
		};

		// 10
		template <class T>
		class NiRect {
		public:
			T m_left;		// 00
			T m_right;		// 04
			T m_top;		// 08
			T m_bottom;		// 0C
		};

		// 1C
		class NiFrustum {
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
		class NiQuaternion {
		public:
			// w is first

			float	m_fW;	// 0
			float	m_fX;	// 4
			float	m_fY;	// 8
			float	m_fZ;	// C

			NiQuaternion operator*(const NiQuaternion& q2) const {
				NiQuaternion tmp;
				tmp.m_fX = m_fX * q2.m_fW + m_fY * q2.m_fZ - m_fZ * q2.m_fY + m_fW * q2.m_fX;
				tmp.m_fY = -m_fX * q2.m_fZ + m_fY * q2.m_fW + m_fZ * q2.m_fX + m_fW * q2.m_fY;
				tmp.m_fZ = m_fX * q2.m_fY - m_fY * q2.m_fX + m_fZ * q2.m_fW + m_fW * q2.m_fZ;
				tmp.m_fW = -m_fX * q2.m_fX - m_fY * q2.m_fY - m_fZ * q2.m_fZ + m_fW * q2.m_fW;
				return tmp;
			}

			NiPoint3 operator* (const NiPoint3& pt) const {
				NiPoint3 tmp;
				tmp.x = m_fW * m_fW * pt.x + 2 * m_fY * m_fW * pt.z - 2 * m_fZ * m_fW * pt.y + m_fX * m_fX * pt.x + 2 * m_fY * m_fX * pt.y + 2 * m_fZ * m_fX * pt.z - m_fZ * m_fZ * pt.x - m_fY * m_fY * pt.x;
				tmp.y = 2 * m_fX * m_fY * pt.x + m_fY * m_fY * pt.y + 2 * m_fZ * m_fY * pt.z + 2 * m_fW * m_fZ * pt.x - m_fZ * m_fZ * pt.y + m_fW * m_fW * pt.y - 2 * m_fX * m_fW * pt.z - m_fX * m_fX * pt.y;
				tmp.z = 2 * m_fX * m_fZ * pt.x + 2 * m_fY * m_fZ * pt.y + m_fZ * m_fZ * pt.z - 2 * m_fW * m_fY * pt.x - m_fY * m_fY * pt.z + 2 * m_fW * m_fX * pt.y - m_fX * m_fX * pt.z + m_fW * m_fW * pt.z;
				return tmp;
			}

			void SetEulerAngles(float pitch, float roll, float yaw) {
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

			void GetEulerAngles(float& roll, float& pitch, float& yaw) {
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
		class NiBound {
		public:
			NiPoint3 m_kCenter;
			union {
				float m_fRadius;
				int m_iRadiusAsInt;
			};
		};

		// 10
		struct NiPlane {
			NiPoint3 m_kNormal;
			float m_fConstant;
		};
	}
}

#pragma warning(default : 26451)
#pragma pack(pop)
