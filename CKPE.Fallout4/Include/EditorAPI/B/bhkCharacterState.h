// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <CKPE.Utils.h>

namespace CKPE::Fallout4::EditorAPI
{
	enum class bhkCharacterStateType : std::uint32_t
	{
		OnGround = 0,
		Jumping = 1,
		InAir = 2,
		Climbing = 3,
		Flying = 4,
		Swimming = 5,
		Floating = 6,
		NoRequest = 0xB
	};

	class hknpCharacterState : public hkReferencedObject
	{
	public:
		/* 0x020 */ [[nodiscard]] virtual bhkCharacterStateType GetType() const noexcept(true);

	private:
		/* 0x028 */ virtual void sub_028();
		/* 0x030 */ virtual void sub_030();
		/* 0x038 */ virtual void sub_038();
		/* 0x040 */ virtual void sub_040();
	};
	static_assert(sizeof(hknpCharacterState) == 0x10);

	class bhkCharacterState : public hknpCharacterState
	{
	private:
		/* 0x048 */ virtual void sub_048();
	};
	static_assert(sizeof(bhkCharacterState) == 0x10);

	class bhkCharacterStateOnGround : public bhkCharacterState
	{
	public:
		/* 0x010 */ std::byte unk010[0x8];
	};
	static_assert(sizeof(bhkCharacterStateOnGround) == 0x18);
	static_assert(offsetof(bhkCharacterStateOnGround, unk010) == 0x10);

	class bhkCharacterStateInAir : public bhkCharacterState
	{
	public:
		/* 0x010 */ std::byte unk010[0x8];
	};
	static_assert(sizeof(bhkCharacterStateInAir) == 0x18);
	static_assert(offsetof(bhkCharacterStateInAir, unk010) == 0x10);

	class bhkCharacterStateJumping : public bhkCharacterState {};
	static_assert(sizeof(bhkCharacterStateJumping) == 0x10);

	class bhkCharacterStateClimbing : public bhkCharacterState {};
	static_assert(sizeof(bhkCharacterStateClimbing) == 0x10);

	class bhkCharacterStateFlying : public bhkCharacterState {};
	static_assert(sizeof(bhkCharacterStateFlying) == 0x10);

	class bhkCharacterStateSwimming : public bhkCharacterState {};
	static_assert(sizeof(bhkCharacterStateSwimming) == 0x10);

	class bhkCharacterStateFloating : public bhkCharacterState {};
	static_assert(sizeof(bhkCharacterStateFloating) == 0x10);
}
