// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/bhkRefObject.h>
#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkConstraint : public bhkRefObject
	{
	private:
		/* 38 */ virtual void UnkConstraint38() noexcept(true);
		/* 39 */ virtual void UnkConstraint39() noexcept(true);
		/* 3A */ virtual void UnkConstraint3A() noexcept(true);
		/* 3B */ virtual void UnkConstraint3B() noexcept(true);
		/* 3C */ virtual void UnkConstraint3C() noexcept(true);
	};
	static_assert(sizeof(bhkConstraint) == 0x20);

	class bhkBallAndSocketConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkBallAndSocketConstraint) == 0x20);

	class bhkHingeConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkHingeConstraint) == 0x20);

	class bhkHingeLimitsConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkHingeLimitsConstraint) == 0x20);

	class bhkLimitedHingeConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkLimitedHingeConstraint) == 0x20);

	class bhkGenericConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkGenericConstraint) == 0x20);

	class bhkFixedConstraint : public bhkGenericConstraint {};
	static_assert(sizeof(bhkFixedConstraint) == 0x20);

	class bhkBreakableConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkBreakableConstraint) == 0x20);

	class bhkMalleableConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkMalleableConstraint) == 0x20);

	class bhkRagdollConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkRagdollConstraint) == 0x20);

	class bhkRagdollLimitsConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkRagdollLimitsConstraint) == 0x20);

	class bhkPrismaticConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkPrismaticConstraint) == 0x20);

	class bhkStiffSpringConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkStiffSpringConstraint) == 0x20);

	class bhkWheelConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkWheelConstraint) == 0x20);

	class bhkPointToPathConstraint : public bhkConstraint {};
	static_assert(sizeof(bhkPointToPathConstraint) == 0x20);

	class bhkGroupConstraint : public bhkConstraint
	{
	public:
		/* 0x020 */ std::uint32_t unk020;
		/* 0x024 */ std::uint8_t pad024[0x4];
	};
	static_assert(sizeof(bhkGroupConstraint) == 0x28);
	static_assert(offsetof(bhkGroupConstraint, unk020) == 0x20);

	class bhkConstraintChain : public bhkConstraint
	{
	private:
		/* 3D */ virtual void UnkConstraintChain3D() noexcept(true);
	};
	static_assert(sizeof(bhkConstraintChain) == 0x20);

	class bhkBallSocketConstraintChain : public bhkConstraintChain {};
	static_assert(sizeof(bhkBallSocketConstraintChain) == 0x20);
}
