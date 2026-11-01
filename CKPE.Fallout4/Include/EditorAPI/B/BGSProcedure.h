// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class IProcedure
	{
	public:
		virtual ~IProcedure() = default;

		constexpr static Common::ID VTABLE = IDs::VTABLE::IProcedure[0];
	protected:
		// Preserve the interface without inventing callable procedure signatures.
		virtual void CopyFrom(const IProcedure* Source) = 0; // 01 - dynamic-casts the source to BGSProcedureBase and copies the parameter list
		[[nodiscard]] virtual bool IsDifferent(const IProcedure* Other) const = 0; // 02 - true when the type, parameter count or parameters differ
		virtual void Save() = 0; // 03 - writes the procedure chunk (type name from slot 32, then the parameters)
		virtual void Load(void* File) = 0; // 04 - reads the procedure chunk
		virtual void sub_028() = 0;
		virtual void sub_030() = 0;
		virtual void sub_038() = 0;
		virtual void sub_040() = 0;
		virtual void sub_048() = 0;
		virtual void sub_050() = 0;
		virtual void sub_058() = 0;
		virtual void sub_060() = 0;
		virtual void sub_068() = 0;
		virtual void sub_070() = 0;
		virtual void sub_078() = 0;
		virtual void sub_080() = 0;
		// BGSProcedureBase family: GetParamCount() - number of expected parameters (a constant per class); the BGSProcedureTree* classes use this slot differently
		virtual void sub_088() = 0;
		// BGSProcedureBase family: GetExpectedParamType(Index) - parameter type descriptor, asserts when Index >= GetParamCount()
		virtual void sub_090() = 0;
		// BGSProcedureBase family: GetExpectedParamName(Index) - parameter name
		virtual void sub_098() = 0;
		// BGSProcedureBase family: IsParamRequired(Index)
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
		[[nodiscard]] virtual const void* GetTypeName() const = 0; // 32 - pointer to the interned type name string, used by Save and IsDifferent
	};
	static_assert(sizeof(IProcedure) == 0x8);

	class BGSProcedureBase : public IProcedure
	{
	public:
		/* 0x008 */ void CopyFrom(const IProcedure* Source) override;
		/* 0x010 */ [[nodiscard]] bool IsDifferent(const IProcedure* Other) const override;
		/* 0x018 */ void Save() override;
		/* 0x020 */ void Load(void* File) override;
	protected:
		virtual void sub_108() = 0;
		virtual void sub_110() = 0;
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureBase[0];

		/* 0x008 */ std::uint8_t unk08[0x30];
	};
	static_assert(sizeof(BGSProcedureBase) == 0x38);
	static_assert(offsetof(BGSProcedureBase, unk08) == 0x8);
}
