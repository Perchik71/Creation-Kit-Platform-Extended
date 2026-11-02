// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// Interface of the package procedures: only the vtable pointer; the virtuals are not resolved.
			struct IProcedure
			{
				void* vftable;	// 00
			};
			static_assert(sizeof(IProcedure) == 0x08);

			// 1.7.99.0: size 0x38, vtable RVA 0x304A658, 34 virtual functions. Base of the package procedures.
			class BGSProcedureBase : public IProcedure
			{
			public:
				std::uint8_t unk08[0x20];	// 08 - helper-constructed object
				std::uint32_t unk28;		// 28 - 0 on construction
				std::uint32_t pad2C;		// 2C
				void* unk30;				// 30 - 0 on construction
			};
			static_assert(offsetof(BGSProcedureBase, unk28) == 0x28);
			static_assert(sizeof(BGSProcedureBase) == 0x38);

			// The concrete package procedures. Each one is a BGSProcedureTyped<BGSProcedureBase, ...>: it adds no data members to BGSProcedureBase
			// (35 virtual functions, one more than the base) except BGSProcedureFollowTo, which is 0x40 bytes.

			// vtable RVA 0x304B270
			class BGSProcedureAcquire : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureAcquire) == 0x38);

			// vtable RVA 0x304B800
			class BGSProcedureActivate : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureActivate) == 0x38);

			// vtable RVA 0x304BCD8
			class BGSProcedureDialogue : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureDialogue) == 0x38);

			// vtable RVA 0x304C1D8
			class BGSProcedureDialogueActivate : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureDialogueActivate) == 0x38);

			// vtable RVA 0x304A908
			class BGSProcedureDone : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureDone) == 0x38);

			// vtable RVA 0x304C710
			class BGSProcedureEat : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureEat) == 0x38);

			// vtable RVA 0x304CC70
			class BGSProcedureEscort : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureEscort) == 0x38);

			// vtable RVA 0x304D258
			class BGSProcedureFind : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureFind) == 0x38);

			// vtable RVA 0x304D758
			class BGSProcedureFlee : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureFlee) == 0x38);

			// vtable RVA 0x304DCA8
			class BGSProcedureFlightGrab : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureFlightGrab) == 0x38);

			// vtable RVA 0x304E250
			class BGSProcedureFollow : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureFollow) == 0x38);

			// vtable RVA 0x304EA88
			class BGSProcedureForceGreet : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureForceGreet) == 0x38);

			// vtable RVA 0x304EEF0
			class BGSProcedureGuard : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureGuard) == 0x38);

			// vtable RVA 0x304F330
			class BGSProcedureHoldPosition : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureHoldPosition) == 0x38);

			// vtable RVA 0x304F770
			class BGSProcedureHover : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureHover) == 0x38);

			// vtable RVA 0x304FC18
			class BGSProcedureKeepAnEyeOn : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureKeepAnEyeOn) == 0x38);

			// vtable RVA 0x30500D0
			class BGSProcedureLock : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureLock) == 0x38);

			// vtable RVA 0x30508F8
			class BGSProcedureOrbit : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureOrbit) == 0x38);

			// vtable RVA 0x3050E68
			class BGSProcedurePatrol : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedurePatrol) == 0x38);

			// vtable RVA 0x3051378
			class BGSProcedurePursue : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedurePursue) == 0x38);

			// vtable RVA 0x3051988
			class BGSProcedureSandbox : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureSandbox) == 0x38);

			// vtable RVA 0x3051EE8
			class BGSProcedureSay : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureSay) == 0x38);

			// vtable RVA 0x30544B0
			class BGSProcedureShout : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureShout) == 0x38);

			// vtable RVA 0x3054A70
			class BGSProcedureSit : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureSit) == 0x38);

			// vtable RVA 0x3054D30
			class BGSProcedureSleep : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureSleep) == 0x38);

			// vtable RVA 0x3055260
			class BGSProcedureTravel : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureTravel) == 0x38);

			// vtable RVA 0x3050390
			class BGSProcedureUnlock : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureUnlock) == 0x38);

			// vtable RVA 0x3055CF0
			class BGSProcedureUseIdleMarker : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureUseIdleMarker) == 0x38);

			// vtable RVA 0x30562D8
			class BGSProcedureUseMagic : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureUseMagic) == 0x38);

			// vtable RVA 0x3056A38
			class BGSProcedureUseWeapon : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureUseWeapon) == 0x38);

			// vtable RVA 0x3057000
			class BGSProcedureWait : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureWait) == 0x38);

			// vtable RVA 0x3057468
			class BGSProcedureWander : public BGSProcedureBase
			{
			public:
			};
			static_assert(sizeof(BGSProcedureWander) == 0x38);

			// vtable RVA 0x304E510
			class BGSProcedureFollowTo : public BGSProcedureBase
			{
			public:
				std::uint8_t unk38[0x8];	// 38 - byte @38 is zeroed on construction
			};
			static_assert(sizeof(BGSProcedureFollowTo) == 0x40);
		}
	}
}