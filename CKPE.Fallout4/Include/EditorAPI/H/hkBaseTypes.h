// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	class hkContainerHeapAllocator;

	template<class T>
	struct hkArrayBase
	{
		enum : std::uint32_t
		{
			kCapacityMask = 0x3FFFFFFF,
			kFlagMask = 0xC0000000,
			kDontDeallocateFlag = 0x80000000
		};

		/* 0x000 */ T* m_data;
		/* 0x008 */ std::int32_t m_size;
		/* 0x00C */ std::int32_t m_capacityAndFlags;

		[[nodiscard]] std::uint32_t GetCapacity() const noexcept(true)
		{
			return static_cast<std::uint32_t>(m_capacityAndFlags) & kCapacityMask;
		}
	};

	template<class T, class Allocator = hkContainerHeapAllocator>
	struct hkArray : hkArrayBase<T> {};

	template<class T>
	struct hkSmallArray
	{
		/* 0x000 */ T* m_data;
		/* 0x008 */ std::uint16_t m_size;
		/* 0x00A */ std::uint16_t m_capacityAndFlags;
		/* 0x00C */ std::uint32_t pad00C;
	};

	template<class T>
	struct hkSimpleArray
	{
		/* 0x000 */ T* m_data;
		/* 0x008 */ std::int32_t m_size;
		/* 0x00C */ std::uint32_t pad00C;
	};

	template<class T>
	struct hkRefPtr
	{
		/* 0x000 */ T* m_ptr;
	};

	struct hkStringPtr
	{
		/* 0x000 */ const char* m_data;
	};

	struct alignas(16) hkVector4f
	{
		/* 0x000 */ float m_quad[4];
	};
	using hkVector4 = hkVector4f;

	struct alignas(16) hkQuaternionf
	{
		/* 0x000 */ hkVector4f m_vec;
	};
	using hkQuaternion = hkQuaternionf;

	struct alignas(16) hkRotationf
	{
		/* 0x000 */ hkVector4f m_col0;
		/* 0x010 */ hkVector4f m_col1;
		/* 0x020 */ hkVector4f m_col2;
	};
	using hkRotation = hkRotationf;

	using hkMatrix3f = hkRotationf;
	using hkMatrix3 = hkMatrix3f;

	struct alignas(16) hkMatrix4f
	{
		/* 0x000 */ hkVector4f m_col0;
		/* 0x010 */ hkVector4f m_col1;
		/* 0x020 */ hkVector4f m_col2;
		/* 0x030 */ hkVector4f m_col3;
	};
	using hkMatrix4 = hkMatrix4f;

	struct alignas(16) hkTransformf
	{
		/* 0x000 */ hkRotationf m_rotation;
		/* 0x030 */ hkVector4f m_translation;
	};
	using hkTransform = hkTransformf;

	struct alignas(16) hkQsTransformf
	{
		/* 0x000 */ hkVector4f m_translation;
		/* 0x010 */ hkQuaternionf m_rotation;
		/* 0x020 */ hkVector4f m_scale;
	};
	using hkQsTransform = hkQsTransformf;

	struct hkBool
	{
		/* 0x000 */ std::uint8_t m_bool;
	};

	struct hkHalf
	{
		/* 0x000 */ std::uint16_t m_value;
	};

	template<class Enum, class Storage>
	struct hkEnum
	{
		/* 0x000 */ Storage m_storage;
	};

	template<class Enum, class Storage>
	struct hkFlags
	{
		/* 0x000 */ Storage m_storage;
	};

	template<class Storage, Storage Invalid, class Discriminant>
	struct hkHandle
	{
		/* 0x000 */ Storage m_value;
	};

	static_assert(sizeof(hkArrayBase<void*>) == 0x10);
	static_assert(sizeof(hkArray<void*>) == 0x10);
	static_assert(sizeof(hkSmallArray<void*>) == 0x10);
	static_assert(sizeof(hkSimpleArray<void*>) == 0x10);
	static_assert(offsetof(hkArrayBase<void*>, m_capacityAndFlags) == 0xC);
	static_assert(sizeof(hkRefPtr<void>) == 0x8);
	static_assert(sizeof(hkStringPtr) == 0x8);
	static_assert(sizeof(hkVector4f) == 0x10);
	static_assert(sizeof(hkQuaternionf) == 0x10);
	static_assert(sizeof(hkRotationf) == 0x30);
	static_assert(sizeof(hkMatrix4f) == 0x40);
	static_assert(sizeof(hkTransformf) == 0x40);
	static_assert(offsetof(hkTransformf, m_translation) == 0x30);
	static_assert(sizeof(hkQsTransformf) == 0x30);
	static_assert(offsetof(hkQsTransformf, m_scale) == 0x20);
	static_assert(sizeof(hkBool) == 0x1);
	static_assert(sizeof(hkHalf) == 0x2);
	static_assert(sizeof(hkEnum<int, std::uint8_t>) == 0x1);
	static_assert(sizeof(hkFlags<int, std::uint32_t>) == 0x4);
	static_assert(sizeof(hkHandle<std::uint16_t, 0xFFFF, void>) == 0x2);
}
