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
			// Package data (1.7.99.0). Sizes are the allocation sizes of the constructors; members are the ones the constructors / readers touch.

			// vtable RVA 0x3059A70, 9 virtual functions. Only the vtable pointer.
			class TESPackageData
			{
			public:
				virtual ~TESPackageData() = default;
			};
			static_assert(sizeof(TESPackageData) == 0x08);

			// vtable RVA 0x3059AD0
			class TESAmbushPackageData : public TESPackageData
			{
			public:
				void* unk08;	// 08 - PLD2 location data, 0 on construction
			};
			static_assert(sizeof(TESAmbushPackageData) == 0x10);

			// vtable RVA 0x305A028, 36 virtual functions
			class TESCustomPackageData : public TESPackageData
			{
			public:
				std::uint8_t unk08[0x10];	// 08
				std::uint32_t unk18;		// 18
				std::uint32_t pad1C;		// 1C
				std::uint32_t unk20;		// 20
				std::uint32_t pad24;		// 24
				void* unk28;				// 28 - 0 on construction
				void* unk30;				// 30
				void* unk38;				// 38 - 0 on construction
				std::uint8_t unk40[0x8];	// 40 - 0 on construction
				void* unk48;				// 48 - 0 on construction
				void* unk50;				// 50 - 0 on construction
				void* unk58;				// 58 - 0 on construction
			};
			static_assert(offsetof(TESCustomPackageData, unk28) == 0x28);
			static_assert(sizeof(TESCustomPackageData) == 0x60);

			// vtable RVA 0x305A6C0
			class TESDialoguePackageData : public TESPackageData
			{
			public:
				float unk08;			// 08 - 1120403456 (100.0f) on construction
				std::uint32_t unk0C;	// 0C
				std::uint32_t unk10;	// 10 - 0 on construction
				std::uint32_t unk14;	// 14
				void* unk18;			// 18 - 0 on construction (PLD2)
				std::uint8_t unk20;		// 20 - 0 on construction
				std::uint8_t unk21;		// 21 - 0 on construction
				std::uint8_t pad22[2];	// 22
				std::uint32_t unk24;	// 24 - 0 on construction
				std::uint8_t unk28;		// 28 - 0 on construction
				std::uint8_t pad29[3];	// 29
				std::uint32_t unk2C;	// 2C - 0 on construction
			};
			static_assert(offsetof(TESDialoguePackageData, unk18) == 0x18);
			static_assert(sizeof(TESDialoguePackageData) == 0x30);

			// vtable RVA 0x305A7D0
			class TESEatPackageData : public TESPackageData
			{
			public:
				void* unk08;	// 08
			};
			static_assert(sizeof(TESEatPackageData) == 0x10);

			// vtable RVA 0x305A870
			class TESEscortPackageData : public TESPackageData
			{
			public:
				void* unk08;			// 08 - PLD2, 0 on construction
				std::uint32_t unk10;	// 10 - 200 on construction
				std::uint32_t pad14;	// 14
			};
			static_assert(offsetof(TESEscortPackageData, unk10) == 0x10);
			static_assert(sizeof(TESEscortPackageData) == 0x18);

			// vtable RVA 0x305A988
			class TESFollowPackageData : public TESPackageData
			{
			public:
				void* unk08;			// 08 - PLD2, 0 on construction
				std::uint32_t unk10;	// 10 - 0 on construction
				std::uint32_t pad14;	// 14
			};
			static_assert(sizeof(TESFollowPackageData) == 0x18);

			// vtable RVA 0x305A9E8
			class TESPatrolPackageData : public TESPackageData
			{
			public:
				void* unk08;	// 08
			};
			static_assert(sizeof(TESPatrolPackageData) == 0x10);

			// vtable RVA 0x305AA88
			class TESUseItemPackageData : public TESPackageData
			{
			public:
				void* unk08;	// 08
			};
			static_assert(sizeof(TESUseItemPackageData) == 0x10);

			// vtable RVA 0x305ABA8
			class TESUseWeaponPackageData : public TESPackageData
			{
			public:
				std::uint16_t unk08;	// 08
				std::uint16_t unk0A;	// 0A
				std::uint32_t unk0C;	// 0C
				std::uint32_t unk10;	// 10
				std::uint32_t unk14;	// 14
				std::uint8_t unk18;		// 18 - 0 on construction
				std::uint8_t unk19;		// 19 - 0 on construction
				std::uint8_t unk1A;		// 1A - 1 on construction
				std::uint8_t unk1B;		// 1B - 1 on construction
				std::uint8_t unk1C;		// 1C - 0 on construction
				std::uint8_t unk1D;		// 1D - 0 on construction
				std::uint16_t unk1E;	// 1E - 1 on construction
				std::uint16_t unk20;	// 20 - 1 on construction
				std::uint16_t unk22;	// 22 - 6 on construction
				float unk24;			// 24 - 1.0 on construction
				float unk28;			// 28 - 2.0 on construction
				std::uint32_t pad2C;	// 2C
				void* unk30;			// 30 - 0 on construction (PLD2)
			};
			static_assert(offsetof(TESUseWeaponPackageData, unk18) == 0x18);
			static_assert(offsetof(TESUseWeaponPackageData, unk30) == 0x30);
			static_assert(sizeof(TESUseWeaponPackageData) == 0x38);
		}
	}
}