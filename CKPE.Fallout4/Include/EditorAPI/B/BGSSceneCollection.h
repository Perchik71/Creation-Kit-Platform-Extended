// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESQuest;
				class BGSScene;

				// 1.11.240.0: size 0x158, vtable RVA 0x2FB6F48, 124 virtual functions
				class BGSSceneCollection :
					public TESForm   // 000
				{
				public:
					virtual ~BGSSceneCollection() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftSceneCollection;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSceneCollection[0];
				public:
					std::uint32_t unk28; // 28
					std::uint32_t unk2C; // 2C
					TESQuest* parentQuest; // 30 - QNAM
					BSTArray<BGSScene*> scenes; // 38 - SNAM
					BSTArray<std::uint64_t> unk50; // 50
					BSTArray<std::uint64_t> unk68; // 68
					std::uint64_t unk80; // 80
					BSTArray<void*> unk88; // 88 - element type not identified yet
					std::uint8_t unkA0[0xC]; // A0
					std::uint8_t unkAC[0x8]; // AC
					std::uint32_t unkB4; // B4
					std::uint64_t unkB8; // B8
					std::uint32_t unkC0; // C0
					std::uint8_t unkC4[0xC]; // C4
					std::uint64_t unkD0; // D0
					std::uint8_t unkD8[0xC]; // D8
					std::uint8_t unkE4[0x8]; // E4
					std::uint32_t unkEC; // EC
					std::uint64_t unkF0; // F0
					std::uint32_t unkF8; // F8
					std::uint8_t unkFC[0xC]; // FC
					std::uint64_t unk108; // 108
					std::uint64_t unk110; // 110
					std::uint32_t unk118; // 118
					std::uint8_t unk11C[0x10]; // 11C
					std::uint8_t unk12C[0x8]; // 12C
					std::uint32_t unk134; // 134
					std::uint64_t unk138; // 138
					std::uint32_t unk140; // 140
					std::uint8_t unk144[0xC]; // 144
					std::uint64_t unk150; // 150
				};
				static_assert(offsetof(BGSSceneCollection, parentQuest) == 0x30);
				static_assert(offsetof(BGSSceneCollection, scenes) == 0x38);
				static_assert(offsetof(BGSSceneCollection, unk50) == 0x50);
				static_assert(offsetof(BGSSceneCollection, unk68) == 0x68);
				static_assert(sizeof(BGSSceneCollection) == 0x158);
			}
		}
	}
}
