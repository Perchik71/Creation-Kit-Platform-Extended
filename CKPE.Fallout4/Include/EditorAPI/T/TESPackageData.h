// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <windows.h>

#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class TESPackage;
	class TESForm;

	class TESPackageData
	{
	public:
		virtual ~TESPackageData() = default;

		constexpr static Common::ID VTABLE = IDs::VTABLE::TESPackageData[0];
	public:
		// The base implementations (vtable 0x1430081A0) are no-ops, IsDifferent returns true and ProcessDialogMessage false.
		virtual void CopyFrom(const TESPackageData* Source) = 0; // 01
		[[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const = 0; // 02
		// Resolves the form ids read by Load into pointers through the owning package
		virtual void ResolveReferences(TESForm* Owner) = 0; // 03
		virtual void Save() = 0; // 04 - writes the package data chunk
		virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) = 0; // 05

		virtual void UpdateDataInDialogBox(HWND hDialogBox) = 0; // 06

	protected:
		virtual void sub_038() = 0; // 07 - no-op in the base and every derived type

	public:
		virtual void UpdateDataFromDialogBox(HWND hDialogBox) = 0; // 08
	};
	static_assert(sizeof(TESPackageData) == 0x8);

	class TESAmbushPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESAmbushPackageData[0];

		/* 0x008 */ void* unk08;
	};
	static_assert(sizeof(TESAmbushPackageData) == 0x10);

	class TESDialoguePackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x018 */ virtual void ResolveReferences(TESForm* Owner) override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
		/* 0x030 */ virtual void UpdateDataInDialogBox(HWND hDialogBox) override;
		/* 0x040 */ virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESDialoguePackageData[0];

		/* 0x008 */ std::uint8_t unk08[0x28];
	};
	static_assert(sizeof(TESDialoguePackageData) == 0x30);

	class TESEatPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESEatPackageData[0];

		/* 0x008 */ void* unk08;
	};
	static_assert(sizeof(TESEatPackageData) == 0x10);

	class TESEscortPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
		/* 0x030 */ virtual void UpdateDataInDialogBox(HWND hDialogBox) override;
		/* 0x040 */ virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESEscortPackageData[0];

		/* 0x008 */ std::uint8_t unk08[0x10];
	};
	static_assert(sizeof(TESEscortPackageData) == 0x18);

	class TESFindPackageData : public TESPackageData
	{
	public:
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
	protected:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESFindPackageData[0];

		virtual void sub_048() = 0;

	public:
		/* 0x008 */ std::uint8_t unk08[0x8];
	};
	static_assert(sizeof(TESFindPackageData) == 0x10);

	class TESFollowPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
		/* 0x030 */ virtual void UpdateDataInDialogBox(HWND hDialogBox) override;
		/* 0x040 */ virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESFollowPackageData[0];

		/* 0x008 */ std::uint8_t unk08[0x10];
	};
	static_assert(sizeof(TESFollowPackageData) == 0x18);

	class TESPatrolPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
		/* 0x030 */ virtual void UpdateDataInDialogBox(HWND hDialogBox) override;
		/* 0x040 */ virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESPatrolPackageData[0];

		/* 0x008 */ std::uint8_t unk08[0x8];
	};
	static_assert(sizeof(TESPatrolPackageData) == 0x10);

	class TESUseItemPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESUseItemPackageData[0];

		/* 0x008 */ void* unk08;
	};
	static_assert(sizeof(TESUseItemPackageData) == 0x10);

	class TESUseWeaponPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x018 */ virtual void ResolveReferences(TESForm* Owner) override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
		/* 0x030 */ virtual void UpdateDataInDialogBox(HWND hDialogBox) override;
		/* 0x040 */ virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESUseWeaponPackageData[0];

		/* 0x008 */ std::uint8_t unk08[0x30];
	};
	static_assert(sizeof(TESUseWeaponPackageData) == 0x38);

	class TESCustomPackageData : public TESPackageData
	{
	public:
		/* 0x008 */ virtual void CopyFrom(const TESPackageData* Source) override;
		/* 0x010 */ [[nodiscard]] virtual bool IsDifferent(const TESPackageData* Other) const override;
		/* 0x018 */ virtual void ResolveReferences(TESForm* Owner) override;
		/* 0x020 */ virtual void Save() override;
		/* 0x028 */ virtual bool ProcessDialogMessage(HWND hDialogBox, UINT uMsg, WPARAM wParam) override;
	protected:
		virtual void sub_048() = 0;
		virtual void sub_050() = 0;
		virtual void sub_058() = 0;
		virtual void InitializeDataListColumns() = 0; // sets the list view columns Name, (blank), Value, Public
		virtual void sub_068() = 0;
		virtual void sub_070() = 0;
		virtual void sub_078() = 0;
		virtual void sub_080() = 0;
		virtual void sub_088() = 0;
		virtual void sub_090() = 0;
		virtual void sub_098() = 0;
		virtual void sub_0A0() = 0;
		virtual void PopulateTreeView() = 0; // clears the tree (TVM_DELETEITEM on the root) and fills it
		virtual void sub_0B0() = 0;
		virtual void InitializeDialogControls() = 0; // fills the combo box (control 5315) and the Description / Value columns (control 5307)
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
		virtual void GetDescriptionText() = 0; // prepends "Package Template (%08X) '%s', " when a template is set, then forwards to the embedded object

	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::TESCustomPackageData[0];

		/* 0x008 */ std::uint8_t unk08[0x50];
		/* 0x058 */ TESPackage* ownerPackage;
	};
	static_assert(sizeof(TESCustomPackageData) == 0x60);
	static_assert(offsetof(TESCustomPackageData, ownerPackage) == 0x58);
}
