// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class TESPackage;

	class IAIWorldLocationHandle
	{
	public:
		virtual ~IAIWorldLocationHandle() = default;

	protected:
		// PackageLocation: for type 0 looks the object up by its handle (0x18), for type 11 asks the package's IAITarget data
		// (index at 0x18) for it; null for every other type
		virtual void* ResolveReference(const void* Context) = 0;
		virtual void sub_010() = 0;
	};
	static_assert(sizeof(IAIWorldLocationHandle) == 0x8);

	class PackageLocation : public IAIWorldLocationHandle
	{
	public:
		/* 0x008 */ std::uint8_t type;
		/* 0x009 */ std::uint8_t pad09[0x3];
		/* 0x00C */ std::uint32_t unk0C;
		/* 0x010 */ std::uint64_t unk10;
		/* 0x018 */ std::uint64_t location;
	};
	static_assert(sizeof(PackageLocation) == 0x20);
	static_assert(offsetof(PackageLocation, type) == 0x8);
	static_assert(offsetof(PackageLocation, location) == 0x18);

	class PackageTarget
	{
	public:
		/* 0x000 */ std::uint8_t type;
		/* 0x001 */ std::uint8_t pad01[0x7];
		/* 0x008 */ std::uint64_t target;
		/* 0x010 */ std::uint32_t value;
		/* 0x014 */ std::uint8_t pad14[0x4];
		/* 0x018 */ TESPackage* ownerPackage;
	};
	static_assert(sizeof(PackageTarget) == 0x20);
	static_assert(offsetof(PackageTarget, target) == 0x8);
	static_assert(offsetof(PackageTarget, ownerPackage) == 0x18);
}
