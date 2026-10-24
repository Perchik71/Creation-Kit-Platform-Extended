// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	class IProcedure
	{
	public:
		virtual ~IProcedure() = default;

	protected:
		// Preserve the interface without inventing callable procedure signatures.
		virtual void sub_008() = 0;
		virtual void sub_010() = 0;
		virtual void sub_018() = 0;
		virtual void sub_020() = 0;
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
	};
	static_assert(sizeof(IProcedure) == 0x8);

	class BGSProcedureBase : public IProcedure
	{
	protected:
		virtual void sub_108() = 0;
		virtual void sub_110() = 0;

	public:
		/* 0x008 */ std::uint8_t unk08[0x30];
	};
	static_assert(sizeof(BGSProcedureBase) == 0x38);
	static_assert(offsetof(BGSProcedureBase, unk08) == 0x8);
}
