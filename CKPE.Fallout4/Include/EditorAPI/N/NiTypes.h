// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdio>
#include <cstddef>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiMatrix.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiTransform.h>
#include <EditorAPI/B/BSSystemFile.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSShaderAccumulator;

			namespace NiAPI
			{
				class NiCamera;
				class NiAVObject;
				template<typename T> class NiPointer;

				struct NiWindowStorage162
				{
					/* 0x000 */ void* VTable;
					/* 0x008 */ NiAVObject* SceneRoot;
					/* 0x010 */ std::uint8_t unk010[0x18];
					/* 0x028 */ BSShaderAccumulator* Accumulator;
					/* 0x030 */ std::uint8_t unk030[0x170];
					/* 0x1A0 */ NiCamera* Camera;
					/* 0x1A8 */ std::uint8_t unk1A8[0x28];
					/* 0x1D0 */ std::uint32_t RenderWindowID;
					/* 0x1D4 */ std::uint8_t unk1D4[0x3C];
				};
				static_assert(sizeof(NiWindowStorage162) == 0x210);
				static_assert(offsetof(NiWindowStorage162, SceneRoot) == 0x8);
				static_assert(offsetof(NiWindowStorage162, Accumulator) == 0x28);
				static_assert(offsetof(NiWindowStorage162, Camera) == 0x1A0);
				static_assert(offsetof(NiWindowStorage162, RenderWindowID) == 0x1D0);

				struct NiWindowStorage982
				{
					/* 0x000 */ void* VTable;
					/* 0x008 */ std::uint8_t unk008[0x8];
					/* 0x010 */ NiAVObject* SceneRoot;
					/* 0x018 */ std::uint8_t unk018[0x18];
					/* 0x030 */ BSShaderAccumulator* Accumulator;
					/* 0x038 */ std::uint8_t unk038[0x178];
					/* 0x1B0 */ NiCamera* Camera;
					/* 0x1B8 */ std::uint8_t unk1B8[0x28];
					/* 0x1E0 */ std::uint32_t RenderWindowID;
					/* 0x1E4 */ std::uint8_t unk1E4[0x3C];
				};
				static_assert(sizeof(NiWindowStorage982) == 0x220);
				static_assert(offsetof(NiWindowStorage982, SceneRoot) == 0x10);
				static_assert(offsetof(NiWindowStorage982, Accumulator) == 0x30);
				static_assert(offsetof(NiWindowStorage982, Camera) == 0x1B0);
				static_assert(offsetof(NiWindowStorage982, RenderWindowID) == 0x1E0);

				class NiWindow
				{
				public:
					using Storage162 = NiWindowStorage162;
					using Storage982 = NiWindowStorage982;

					~NiWindow() = default;
				protected:
					// Slot zero is not a destructor.
					/* 00 */ virtual void Unk00();
				public:
					/* 01 */ virtual bool SetSceneRoot(NiAVObject* Root);
					/* 02 */ virtual void CreateCamera(NiPointer<NiCamera>& Camera);
					/* 03 */ virtual bool Resize(std::uint32_t Width, std::uint32_t Height);
					/* 04 */ virtual bool UpdateCameraFrustum(NiCamera* Camera,
						std::int32_t Width, std::int32_t Height, float FarPlane,
						float FieldOfView);
				public:
					[[nodiscard]] inline NiAVObject* GetSceneRoot() const noexcept(true)
					{ return ReadPointer<NiAVObject>(offsetof(Storage162, SceneRoot), offsetof(Storage982, SceneRoot)); }
					[[nodiscard]] inline NiCamera* GetCamera() const noexcept(true)
					{ return ReadPointer<NiCamera>(offsetof(Storage162, Camera), offsetof(Storage982, Camera)); }
					[[nodiscard]] inline BSShaderAccumulator* GetAccumulator() const noexcept(true)
					{ return ReadPointer<BSShaderAccumulator>(offsetof(Storage162, Accumulator), offsetof(Storage982, Accumulator)); }
					[[nodiscard]] static std::size_t GetAllocationSize() noexcept(true)
					{
						return VersionLists::GetRuntimeIndex() == 0 ?
							sizeof(Storage162) : sizeof(Storage982);
					}
				private:
					template<typename T>
					[[nodiscard]] T* ReadPointer(std::size_t Old, std::size_t Current) const noexcept(true)
					{
						const auto Offset = VersionLists::GetRuntimeIndex() == 0 ? Old : Current;
						return *reinterpret_cast<T* const*>(
							reinterpret_cast<const std::uint8_t*>(this) + Offset);
					}

					// Storage is 982+; 162 accessors use its smaller layout.
					/* 0x008 */ std::uint8_t _unk008[0x8];
					/* 0x010 */ NiAVObject* _SceneRoot;
					/* 0x018 */ std::uint32_t _unk018;
					/* 0x01C */ std::uint32_t _pad01C;
					/* 0x020 */ std::uint8_t _unk020[0x10];
					/* 0x030 */ BSShaderAccumulator* _Accumulator;
					/* 0x038 */ std::uint8_t _unk038[0x178];
					/* 0x1B0 */ NiCamera* _Camera;
					/* 0x1B8 */ std::uint8_t _unk1B8[0x28];
					/* 0x1E0 */ std::uint32_t _RenderWindowID;
					/* 0x1E4 */ std::uint8_t _unk1E4[0x3C];
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiWindow, _SceneRoot) == 0x10 &&
							offsetof(NiWindow, _Accumulator) == 0x30 &&
							offsetof(NiWindow, _Camera) == 0x1B0 &&
							offsetof(NiWindow, _RenderWindowID) == 0x1E0;
					}
				};
				static_assert(sizeof(NiWindow) == 0x220);
				static_assert(NiWindow::VerifyLayout());

				class NiBinaryStream
				{
				public:
					struct BufferInfo
					{
						/* 0x00 */ void* Buffer;
						/* 0x08 */ std::uint64_t TotalSize;
						/* 0x10 */ std::uint64_t BufferSize;
						/* 0x18 */ std::uint64_t BufferReadSize;
						/* 0x20 */ std::uint64_t BufferPosition;
						/* 0x28 */ std::uint64_t StreamPosition;
					};
					static_assert(sizeof(BufferInfo) == 0x30);
					static_assert(offsetof(BufferInfo, StreamPosition) == 0x28);
				protected:
					/*000*/							// --vtbls
					std::uint64_t _Pos;			
				public:
					virtual ~NiBinaryStream();
			
					[[nodiscard]] virtual bool IsOpen() = 0;
					virtual void Seek(std::ptrdiff_t numBytes) = 0;   
					[[nodiscard]] virtual std::uint64_t GetPosition();
					virtual void GetBufferInfo(BufferInfo& Info);
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
					using NiBinaryStream::Seek;

					virtual void Seek(std::int64_t Offset, std::uint32_t SeekMode);	// 07

					[[nodiscard]] inline bool IsOpen() const noexcept(true) { return (bool)_IsOpen; }
					[[nodiscard]] virtual std::uint64_t GetSize();
				};
				static_assert(sizeof(NiFile) == 0x150);

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
				static_assert(sizeof(NiRect<float>) == 0x10);
				static_assert(offsetof(NiRect<float>, m_left) == 0x0);
				static_assert(offsetof(NiRect<float>, m_right) == 0x4);
				static_assert(offsetof(NiRect<float>, m_top) == 0x8);
				static_assert(offsetof(NiRect<float>, m_bottom) == 0xC);

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
				static_assert(sizeof(NiFrustum) == 0x1C);
				static_assert(offsetof(NiFrustum, m_fLeft) == 0x0);
				static_assert(offsetof(NiFrustum, m_fRight) == 0x4);
				static_assert(offsetof(NiFrustum, m_fTop) == 0x8);
				static_assert(offsetof(NiFrustum, m_fBottom) == 0xC);
				static_assert(offsetof(NiFrustum, m_fNear) == 0x10);
				static_assert(offsetof(NiFrustum, m_fFar) == 0x14);
				static_assert(offsetof(NiFrustum, m_bOrtho) == 0x18);

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
				static_assert(sizeof(NiQuaternion) == 0x10);
				static_assert(offsetof(NiQuaternion, m_fW) == 0x0);
				static_assert(offsetof(NiQuaternion, m_fX) == 0x4);
				static_assert(offsetof(NiQuaternion, m_fY) == 0x8);
				static_assert(offsetof(NiQuaternion, m_fZ) == 0xC);

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
				static_assert(sizeof(NiBound) == 0x10);
				static_assert(offsetof(NiBound, m_kCenter) == 0x0);
				static_assert(offsetof(NiBound, m_fRadius) == 0xC);

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