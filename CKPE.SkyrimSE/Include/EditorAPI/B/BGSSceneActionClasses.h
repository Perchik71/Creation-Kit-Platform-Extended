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
			// Scene actions (1.7.99.0), 25 virtual functions in the base.

			// vtable RVA 0x30A6E60
			class BGSSceneAction
			{
			public:
				virtual ~BGSSceneAction() = default;

				std::uint8_t unk08[0x10];	// 08
				void* unk18;				// 18 - 0 on construction
				std::uint32_t unk20;		// 20 - -1 on construction
				std::uint16_t unk24;		// 24 - 0 on construction
				std::uint16_t unk26;		// 26 - 0 on construction
				std::uint32_t fnam;			// 28 - FNAM flags, 0 on construction
				std::uint32_t unk2C;		// 2C
			};
			static_assert(offsetof(BGSSceneAction, unk18) == 0x18);
			static_assert(offsetof(BGSSceneAction, fnam) == 0x28);
			static_assert(sizeof(BGSSceneAction) == 0x30);

			// vtable RVA 0x30A6F60
			class BGSSceneActionDialogue : public BGSSceneAction
			{
			public:
				void* unk30;				// 30 - 0 on construction
				std::uint32_t htid;			// 38 - HTID, -1 on construction
				float unk3C;				// 3C - 10.0 on construction
				float unk40;				// 40 - 1.0 on construction
				std::uint32_t unk44;		// 44 - 0 on construction
				std::uint32_t demo;			// 48 - DEMO, 0 on construction
				std::uint32_t deva;			// 4C - DEVA, 0 on construction
			};
			static_assert(offsetof(BGSSceneActionDialogue, htid) == 0x38);
			static_assert(sizeof(BGSSceneActionDialogue) == 0x50);

			// vtable RVA 0x30A7698
			class BGSSceneActionTimer : public BGSSceneAction
			{
			public:
				std::uint32_t unk30;		// 30 - 0 on construction
				std::uint32_t pad34;		// 34
				std::uint8_t unk38[0x8];	// 38
			};
			static_assert(sizeof(BGSSceneActionTimer) == 0x40);
		}
	}
}