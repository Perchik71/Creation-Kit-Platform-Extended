// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/bhkRefObject.h>
#include <cstddef>
#include <cstdint>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkAction : public bhkRefObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkAction[0];

	private:
		/* 38 */ virtual void UnkAction38() noexcept(true);
	};
	static_assert(sizeof(bhkAction) == 0x20);

	class bhkUnaryAction : public bhkAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkUnaryAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkUnaryAction[0];
	};
	static_assert(sizeof(bhkUnaryAction) == 0x20);

	class bhkBinaryAction : public bhkAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkBinaryAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkBinaryAction[0];

	private:
		/* 39 */ virtual void UnkBinaryAction39() noexcept(true);
		/* 3A */ virtual void UnkBinaryAction3A() noexcept(true);
		/* 3B */ virtual void UnkBinaryAction3B() noexcept(true);
	};
	static_assert(sizeof(bhkBinaryAction) == 0x20);

	class bhkDashpotAction : public bhkBinaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkDashpotAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkDashpotAction[0];
	};
	static_assert(sizeof(bhkDashpotAction) == 0x20);

	class bhkAngularDashpotAction : public bhkBinaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkAngularDashpotAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkAngularDashpotAction[0];
	};
	static_assert(sizeof(bhkAngularDashpotAction) == 0x20);

	class bhkSpringAction : public bhkBinaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkSpringAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkSpringAction[0];
	};
	static_assert(sizeof(bhkSpringAction) == 0x20);

	class bhkMotorAction : public bhkUnaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkMotorAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkMotorAction[0];
	};
	static_assert(sizeof(bhkMotorAction) == 0x20);

	class bhkMouseSpringAction : public bhkUnaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkMouseSpringAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkMouseSpringAction[0];
	};
	static_assert(sizeof(bhkMouseSpringAction) == 0x20);

	class bhkOrientHingedBodyAction : public bhkUnaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkOrientHingedBodyAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkOrientHingedBodyAction[0];

	private:
		/* 39 */ virtual void UnkOrientHingedBodyAction39() noexcept(true);
	};
	static_assert(sizeof(bhkOrientHingedBodyAction) == 0x20);

	class bhkLiquidAction : public bhkUnaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkLiquidAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkLiquidAction[0];

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
		constexpr static Common::ID RTTI = IDs::RTTI::bhkStickyContactAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkStickyContactAction[0];

		/* 0x020 */ std::uint8_t unk020[0x8];
	};
	static_assert(sizeof(bhkStickyContactAction) == 0x28);
	static_assert(offsetof(bhkStickyContactAction, unk020) == 0x20);
}
