// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/bhkRefObject.h>
#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkAction : public bhkRefObject
	{
	private:
		/* 38 */ virtual void UnkAction38() noexcept(true);
	};
	static_assert(sizeof(bhkAction) == 0x20);

	class bhkUnaryAction : public bhkAction {};
	static_assert(sizeof(bhkUnaryAction) == 0x20);

	class bhkBinaryAction : public bhkAction
	{
	private:
		/* 39 */ virtual void UnkBinaryAction39() noexcept(true);
		/* 3A */ virtual void UnkBinaryAction3A() noexcept(true);
		/* 3B */ virtual void UnkBinaryAction3B() noexcept(true);
	};
	static_assert(sizeof(bhkBinaryAction) == 0x20);

	class bhkDashpotAction : public bhkBinaryAction {};
	static_assert(sizeof(bhkDashpotAction) == 0x20);

	class bhkAngularDashpotAction : public bhkBinaryAction {};
	static_assert(sizeof(bhkAngularDashpotAction) == 0x20);

	class bhkSpringAction : public bhkBinaryAction {};
	static_assert(sizeof(bhkSpringAction) == 0x20);

	class bhkMotorAction : public bhkUnaryAction {};
	static_assert(sizeof(bhkMotorAction) == 0x20);

	class bhkMouseSpringAction : public bhkUnaryAction {};
	static_assert(sizeof(bhkMouseSpringAction) == 0x20);

	class bhkOrientHingedBodyAction : public bhkUnaryAction
	{
	private:
		/* 39 */ virtual void UnkOrientHingedBodyAction39() noexcept(true);
	};
	static_assert(sizeof(bhkOrientHingedBodyAction) == 0x20);

	class bhkLiquidAction : public bhkUnaryAction
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x18];
		/* 0x038 */ float unk038[0x6];
		/* 0x050 */ std::uint8_t unk050;
		/* 0x051 */ std::uint8_t pad051[0x7];
	};
	static_assert(sizeof(bhkLiquidAction) == 0x58);
	static_assert(offsetof(bhkLiquidAction, unk038) == 0x38);
	static_assert(offsetof(bhkLiquidAction, unk050) == 0x50);

	class bhkStickyContactAction : public bhkUnaryAction
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x8];
	};
	static_assert(sizeof(bhkStickyContactAction) == 0x28);
	static_assert(offsetof(bhkStickyContactAction, unk020) == 0x20);
}
