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
					BSTArray<std::uint64_t> unk50; // 50 - copied by CopyFrom (8-byte elements)
					BSTArray<std::uint64_t> unk68; // 68 - copied by CopyFrom (8-byte elements)
					std::uint64_t unk80; // 80 - only cleared by the constructor
					BSTArray<std::uint64_t> unk88; // 88 - 8 byte elements (stride found in the indexing code)
					// Three hash tables in the CK 0x38 byte layout (end-of-chain marker object at +18, entries pointer at +30); the form functions only construct them,
					// so the key and value types are not recoverable from this class.
					std::uint8_t hashTableA0[0x38]; // A0 - marker at B8, entries at D0
					std::uint8_t hashTableD8[0x38]; // D8 - marker at F0, entries at 108
					std::uint64_t unk110; // 110 - only cleared by the constructor
					std::uint32_t unk118; // 118 - only cleared by the constructor
					std::uint8_t pad11C[0x4]; // 11C
					std::uint8_t hashTable120[0x38]; // 120 - marker at 138, entries at 150
				};
				static_assert(offsetof(BGSSceneCollection, parentQuest) == 0x30);
				static_assert(offsetof(BGSSceneCollection, scenes) == 0x38);
				static_assert(offsetof(BGSSceneCollection, unk50) == 0x50);
				static_assert(offsetof(BGSSceneCollection, unk68) == 0x68);
				static_assert(offsetof(BGSSceneCollection, hashTableA0) == 0xA0);
				static_assert(offsetof(BGSSceneCollection, hashTableD8) == 0xD8);
				static_assert(offsetof(BGSSceneCollection, hashTable120) == 0x120);
				static_assert(sizeof(BGSSceneCollection) == 0x158);
			}
		}
	}
}
