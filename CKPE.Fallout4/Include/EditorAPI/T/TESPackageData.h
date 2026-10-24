// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class TESPackage;

	class TESPackageData
	{
	public:
		virtual ~TESPackageData() = default;

	protected:
		virtual void sub_008() = 0;
		virtual void sub_010() = 0;
		virtual void sub_018() = 0;
		virtual void sub_020() = 0;
		virtual void sub_028() = 0;
		virtual void sub_030() = 0;
		virtual void sub_038() = 0;
		virtual void sub_040() = 0;
	};
	static_assert(sizeof(TESPackageData) == 0x8);

	class TESAmbushPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ void* unk08;
	};
	static_assert(sizeof(TESAmbushPackageData) == 0x10);

	class TESDialoguePackageData : public TESPackageData
	{
	public:
		/* 0x008 */ std::uint8_t unk08[0x28];
	};
	static_assert(sizeof(TESDialoguePackageData) == 0x30);

	class TESEatPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ void* unk08;
	};
	static_assert(sizeof(TESEatPackageData) == 0x10);

	class TESEscortPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ std::uint8_t unk08[0x10];
	};
	static_assert(sizeof(TESEscortPackageData) == 0x18);

	class TESFindPackageData : public TESPackageData
	{
	protected:
		virtual void sub_048() = 0;

	public:
		/* 0x008 */ std::uint8_t unk08[0x8];
	};
	static_assert(sizeof(TESFindPackageData) == 0x10);

	class TESFollowPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ std::uint8_t unk08[0x10];
	};
	static_assert(sizeof(TESFollowPackageData) == 0x18);

	class TESPatrolPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ std::uint8_t unk08[0x8];
	};
	static_assert(sizeof(TESPatrolPackageData) == 0x10);

	class TESUseItemPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ void* unk08;
	};
	static_assert(sizeof(TESUseItemPackageData) == 0x10);

	class TESUseWeaponPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ std::uint8_t unk08[0x30];
	};
	static_assert(sizeof(TESUseWeaponPackageData) == 0x38);

	class TESCustomPackageData : public TESPackageData
	{
	protected:
		virtual void sub_048() = 0;
		virtual void sub_050() = 0;
		virtual void sub_058() = 0;
		virtual void sub_060() = 0;
		virtual void sub_068() = 0;
		virtual void sub_070() = 0;
		virtual void sub_078() = 0;
		virtual void sub_080() = 0;
		virtual void sub_088() = 0;
		virtual void sub_090() = 0;
		virtual void sub_098() = 0;
		virtual void sub_0A0() = 0;
		virtual void sub_0A8() = 0;
		virtual void sub_0B0() = 0;
		virtual void sub_0B8() = 0;
		virtual void sub_0C0() = 0;
		virtual void sub_0C8() = 0;
		virtual void sub_0D0() = 0;
		virtual void sub_0D8() = 0;
		virtual void sub_0E0() = 0;
		virtual void sub_0E8() = 0;
		virtual void sub_0F0() = 0;
		virtual void sub_0F8() = 0;
		virtual void sub_100() = 0;
		virtual void sub_108() = 0;
		virtual void sub_110() = 0;
		virtual void sub_118() = 0;

	public:
		/* 0x008 */ std::uint8_t unk08[0x50];
		/* 0x058 */ TESPackage* ownerPackage;
	};
	static_assert(sizeof(TESCustomPackageData) == 0x60);
	static_assert(offsetof(TESCustomPackageData, ownerPackage) == 0x58);
}
