// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSProcedure.h>

namespace CKPE::Fallout4::EditorAPI
{
	// The concrete package procedures. Each one is a BGSProcedureTyped<...>: it adds no data members to BGSProcedureBase (the deleting
	// destructors free 56 bytes, BGSProcedureFollowTo 64) and overrides the parameter accessors (slots 0x88-0xA0 and 0x118) with per-class tables;
	// see the notes for the names of those functions.

	class BGSProcedureHeadtrack : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureHeadtrack[0];
	};
	static_assert(sizeof(BGSProcedureHeadtrack) == 0x38);

	class BGSProcedurePlayIdle : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedurePlayIdle[0];
	};
	static_assert(sizeof(BGSProcedurePlayIdle) == 0x38);

	class BGSProcedureRange : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureRange[0];
	};
	static_assert(sizeof(BGSProcedureRange) == 0x38);

	class BGSProcedureStayAway : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureStayAway[0];
	};
	static_assert(sizeof(BGSProcedureStayAway) == 0x38);

	class BGSProcedureDone : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureDone[0];
	};
	static_assert(sizeof(BGSProcedureDone) == 0x38);

	class BGSProcedureAcquire : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureAcquire[0];
	};
	static_assert(sizeof(BGSProcedureAcquire) == 0x38);

	class BGSProcedureActivate : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureActivate[0];
	};
	static_assert(sizeof(BGSProcedureActivate) == 0x38);

	class BGSProcedureDialogue : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureDialogue[0];
	};
	static_assert(sizeof(BGSProcedureDialogue) == 0x38);

	class BGSProcedureDialogueActivate : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureDialogueActivate[0];
	};
	static_assert(sizeof(BGSProcedureDialogueActivate) == 0x38);

	class BGSProcedureEat : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureEat[0];
	};
	static_assert(sizeof(BGSProcedureEat) == 0x38);

	class BGSProcedureEscort : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureEscort[0];
	};
	static_assert(sizeof(BGSProcedureEscort) == 0x38);

	class BGSProcedureFind : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureFind[0];
	};
	static_assert(sizeof(BGSProcedureFind) == 0x38);

	class BGSProcedureFlee : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureFlee[0];
	};
	static_assert(sizeof(BGSProcedureFlee) == 0x38);

	class BGSProcedureFlightGrab : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureFlightGrab[0];
	};
	static_assert(sizeof(BGSProcedureFlightGrab) == 0x38);

	class BGSProcedureFollow : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureFollow[0];
	};
	static_assert(sizeof(BGSProcedureFollow) == 0x38);

	class BGSProcedureFollowTo : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureFollowTo[0];
		/* 0x038 */ std::uint64_t unk38;
	};
	static_assert(sizeof(BGSProcedureFollowTo) == 0x40);

	class BGSProcedureForceGreet : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureForceGreet[0];
	};
	static_assert(sizeof(BGSProcedureForceGreet) == 0x38);

	class BGSProcedureGuard : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureGuard[0];
	};
	static_assert(sizeof(BGSProcedureGuard) == 0x38);

	class BGSProcedureGuardArea : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureGuardArea[0];
	};
	static_assert(sizeof(BGSProcedureGuardArea) == 0x38);

	class BGSProcedureHoldPosition : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureHoldPosition[0];
	};
	static_assert(sizeof(BGSProcedureHoldPosition) == 0x38);

	class BGSProcedureHover : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureHover[0];
	};
	static_assert(sizeof(BGSProcedureHover) == 0x38);

	class BGSProcedureKeepAnEyeOn : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureKeepAnEyeOn[0];
	};
	static_assert(sizeof(BGSProcedureKeepAnEyeOn) == 0x38);

	class BGSProcedureLock : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureLock[0];
	};
	static_assert(sizeof(BGSProcedureLock) == 0x38);

	class BGSProcedureUnlock : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureUnlock[0];
	};
	static_assert(sizeof(BGSProcedureUnlock) == 0x38);

	class BGSProcedureOrbit : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureOrbit[0];
	};
	static_assert(sizeof(BGSProcedureOrbit) == 0x38);

	class BGSProcedurePatrol : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedurePatrol[0];
	};
	static_assert(sizeof(BGSProcedurePatrol) == 0x38);

	class BGSProcedurePursue : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedurePursue[0];
	};
	static_assert(sizeof(BGSProcedurePursue) == 0x38);

	class BGSProcedureSandbox : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureSandbox[0];
	};
	static_assert(sizeof(BGSProcedureSandbox) == 0x38);

	class BGSProcedureSay : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureSay[0];
	};
	static_assert(sizeof(BGSProcedureSay) == 0x38);

	class BGSProcedureSit : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureSit[0];
	};
	static_assert(sizeof(BGSProcedureSit) == 0x38);

	class BGSProcedureSleep : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureSleep[0];
	};
	static_assert(sizeof(BGSProcedureSleep) == 0x38);

	class BGSProcedureTravel : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureTravel[0];
	};
	static_assert(sizeof(BGSProcedureTravel) == 0x38);

	class BGSProcedureUseIdleMarker : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureUseIdleMarker[0];
	};
	static_assert(sizeof(BGSProcedureUseIdleMarker) == 0x38);

	class BGSProcedureUseMagic : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureUseMagic[0];
	};
	static_assert(sizeof(BGSProcedureUseMagic) == 0x38);

	class BGSProcedureUseWeapon : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureUseWeapon[0];
	};
	static_assert(sizeof(BGSProcedureUseWeapon) == 0x38);

	class BGSProcedureWait : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureWait[0];
	};
	static_assert(sizeof(BGSProcedureWait) == 0x38);

	class BGSProcedureWander : public BGSProcedureBase
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProcedureWander[0];
	};
	static_assert(sizeof(BGSProcedureWander) == 0x38);

}