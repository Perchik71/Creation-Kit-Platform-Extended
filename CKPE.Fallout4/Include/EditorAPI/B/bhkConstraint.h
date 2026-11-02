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
	class bhkConstraint : public bhkRefObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkConstraint[0];

	private:
		/* 38 */ virtual void UnkConstraint38() noexcept(true);
		/* 39 */ virtual void UnkConstraint39() noexcept(true);
		/* 3A */ virtual void UnkConstraint3A() noexcept(true);
		/* 3B */ virtual void UnkConstraint3B() noexcept(true);
		/* 3C */ virtual void UnkConstraint3C() noexcept(true);
	};
	static_assert(sizeof(bhkConstraint) == 0x20);

	class bhkBallAndSocketConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkBallAndSocketConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkBallAndSocketConstraint[0];
	};
	static_assert(sizeof(bhkBallAndSocketConstraint) == 0x20);

	class bhkHingeConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkHingeConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkHingeConstraint[0];
	};
	static_assert(sizeof(bhkHingeConstraint) == 0x20);

	class bhkHingeLimitsConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkHingeLimitsConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkHingeLimitsConstraint[0];
	};
	static_assert(sizeof(bhkHingeLimitsConstraint) == 0x20);

	class bhkLimitedHingeConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkLimitedHingeConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkLimitedHingeConstraint[0];
	};
	static_assert(sizeof(bhkLimitedHingeConstraint) == 0x20);

	class bhkGenericConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkGenericConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkGenericConstraint[0];
	};
	static_assert(sizeof(bhkGenericConstraint) == 0x20);

	class bhkFixedConstraint : public bhkGenericConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkFixedConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkFixedConstraint[0];
	};
	static_assert(sizeof(bhkFixedConstraint) == 0x20);

	class bhkBreakableConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkBreakableConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkBreakableConstraint[0];
	};
	static_assert(sizeof(bhkBreakableConstraint) == 0x20);

	class bhkMalleableConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkMalleableConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkMalleableConstraint[0];
	};
	static_assert(sizeof(bhkMalleableConstraint) == 0x20);

	class bhkRagdollConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkRagdollConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkRagdollConstraint[0];
	};
	static_assert(sizeof(bhkRagdollConstraint) == 0x20);

	class bhkRagdollLimitsConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkRagdollLimitsConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkRagdollLimitsConstraint[0];
	};
	static_assert(sizeof(bhkRagdollLimitsConstraint) == 0x20);

	class bhkPrismaticConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkPrismaticConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkPrismaticConstraint[0];
	};
	static_assert(sizeof(bhkPrismaticConstraint) == 0x20);

	class bhkStiffSpringConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkStiffSpringConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkStiffSpringConstraint[0];
	};
	static_assert(sizeof(bhkStiffSpringConstraint) == 0x20);

	class bhkWheelConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkWheelConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkWheelConstraint[0];
	};
	static_assert(sizeof(bhkWheelConstraint) == 0x20);

	class bhkPointToPathConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkPointToPathConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkPointToPathConstraint[0];
	};
	static_assert(sizeof(bhkPointToPathConstraint) == 0x20);

	class bhkGroupConstraint : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkGroupConstraint;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkGroupConstraint[0];

		/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
		/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
		/* 0x020 */ std::uint32_t unk020;
		/* 0x024 */ std::uint8_t pad024[0x4];
	};
	static_assert(sizeof(bhkGroupConstraint) == 0x28);
	static_assert(offsetof(bhkGroupConstraint, unk020) == 0x20);

	class bhkConstraintChain : public bhkConstraint
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkConstraintChain;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkConstraintChain[0];

	private:
		/* 3D */ virtual void UnkConstraintChain3D() noexcept(true);
	};
	static_assert(sizeof(bhkConstraintChain) == 0x20);

	class bhkBallSocketConstraintChain : public bhkConstraintChain
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkBallSocketConstraintChain;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkBallSocketConstraintChain[0];
	};
	static_assert(sizeof(bhkBallSocketConstraintChain) == 0x20);
}
