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
			// Perk entries (1.7.99.0). Sizes come from the constructors and the members each one initialises; roles are not resolved.

			// vtable RVA 0x309FFE8, 25 virtual functions
			class BGSPerkEntry
			{
			public:
				virtual ~BGSPerkEntry() = default;

				std::uint8_t unk08;		// 08 - 0 on construction
				std::uint8_t unk09;		// 09 - 0 on construction
				std::uint8_t pad0A[6];	// 0A
			};
			static_assert(offsetof(BGSPerkEntry, unk08) == 0x08);
			static_assert(sizeof(BGSPerkEntry) == 0x10);

			// vtable RVA 0x30A01E8
			class BGSAbilityPerkEntry : public BGSPerkEntry
			{
			public:
				void* unk10;	// 10 - 0 on construction (PRKF / DATA)
			};
			static_assert(sizeof(BGSAbilityPerkEntry) == 0x18);

			// vtable RVA 0x30A00E8
			class BGSQuestPerkEntry : public BGSPerkEntry
			{
			public:
				void* unk10;			// 10 - 0 on construction
				std::uint16_t unk18;	// 18 - 0 on construction
				std::uint8_t pad1A[6];	// 1A
			};
			static_assert(sizeof(BGSQuestPerkEntry) == 0x20);

			// vtable RVA 0x30A02E8
			class BGSEntryPointPerkEntry : public BGSPerkEntry
			{
			public:
				std::uint8_t unk10;		// 10 - 0 on construction
				std::uint8_t unk11;		// 11 - 0 on construction
				std::uint8_t unk12;		// 12 - 0 on construction
				std::uint8_t pad13[5];	// 13
				void* unk18;			// 18 - 0 on construction
				void* unk20;			// 20 - 0 on construction
				void* unk28;			// 28 - 0 on construction
				void* unk30;			// 30 - helper-constructed
			};
			static_assert(offsetof(BGSEntryPointPerkEntry, unk10) == 0x10);
			static_assert(sizeof(BGSEntryPointPerkEntry) == 0x38);

			// Entry point function data (EPFD). The base only holds the vtable pointer.
			// vtable RVA 0x3099F38, 18 virtual functions
			class BGSEntryPointFunctionData
			{
			public:
				virtual ~BGSEntryPointFunctionData() = default;
			};
			static_assert(sizeof(BGSEntryPointFunctionData) == 0x08);

			// vtable RVA 0x3099FF0
			class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData
			{
			public:
				std::uint32_t unk08;	// 08 - 0 on construction
				std::uint32_t pad0C;	// 0C
			};
			static_assert(sizeof(BGSEntryPointFunctionDataOneValue) == 0x10);

			// vtable RVA 0x309A0A8
			class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData
			{
			public:
				std::uint32_t unk08;	// 08 - 0 on construction
				std::uint32_t unk0C;	// 0C - 0 on construction
			};
			static_assert(sizeof(BGSEntryPointFunctionDataTwoValue) == 0x10);

			// vtable RVA 0x309A160
			class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData
			{
			public:
				void* unk08;	// 08 - 0 on construction
			};
			static_assert(sizeof(BGSEntryPointFunctionDataLeveledList) == 0x10);

			// vtable RVA 0x309A218
			class BGSEntryPointFunctionDataSpellItem : public BGSEntryPointFunctionData
			{
			public:
				void* unk08;	// 08 - 0 on construction
			};
			static_assert(sizeof(BGSEntryPointFunctionDataSpellItem) == 0x10);

			// vtable RVA 0x309A388
			class BGSEntryPointFunctionDataText : public BGSEntryPointFunctionData
			{
			public:
				void* unk08[2];	// 08 - helper-constructed string object
			};
			static_assert(sizeof(BGSEntryPointFunctionDataText) == 0x18);

			// vtable RVA 0x309A2D0
			class BGSEntryPointFunctionDataBooleanGraphVariable : public BGSEntryPointFunctionData
			{
			public:
				void* unk08[2];	// 08 - helper-constructed string object
			};
			static_assert(sizeof(BGSEntryPointFunctionDataBooleanGraphVariable) == 0x18);

			// vtable RVA 0x309A440
			class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData
			{
			public:
				void* unk08[2];			// 08 - helper-constructed string object
				void* unk18;			// 18 - set from the constructor argument
				void* unk20;			// 20 - 0 on construction
				std::uint16_t unk28;	// 28 - 0 on construction
				std::uint16_t unk2A;	// 2A - set from the constructor argument
				std::uint32_t pad2C;	// 2C
			};
			static_assert(offsetof(BGSEntryPointFunctionDataActivateChoice, unk18) == 0x18);
			static_assert(sizeof(BGSEntryPointFunctionDataActivateChoice) == 0x30);
		}
	}
}