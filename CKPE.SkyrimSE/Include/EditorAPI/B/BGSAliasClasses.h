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
			// Quest aliases (1.7.99.0). Layouts from the constructors and the ALST / ALLS / FNAM / ALFA / ALFE / ALFD / ALEA tag readers.

			// vtable RVA 0x309D6C0, 20 virtual functions
			class BGSBaseAlias
			{
			public:
				virtual ~BGSBaseAlias() = default;

				void* unk08;			// 08 - helper-constructed (string handle, empty on construction)
				void* unk10;			// 10 - constructor argument
				std::uint32_t id;		// 18 - constructor argument (ALST / ALLS)
				std::uint32_t fnam;		// 1C - FNAM flags, 0 on construction
				std::uint16_t unk20;	// 20 - 0 on construction
				std::uint8_t pad22[6];	// 22
			};
			static_assert(offsetof(BGSBaseAlias, id) == 0x18);
			static_assert(offsetof(BGSBaseAlias, fnam) == 0x1C);
			static_assert(sizeof(BGSBaseAlias) == 0x28);

			// vtable RVA 0x309D790
			class BGSLocAlias : public BGSBaseAlias
			{
			public:
				void* unk28;			// 28 - 0 on construction
				std::uint32_t alfa;		// 30 - ALFA, -1 on construction
				std::uint32_t pad34;	// 34
				void* unk38;			// 38 - 0 on construction
				std::uint32_t alfe;		// 40 - ALFE, -1 on construction
				std::uint32_t alfd;		// 44 - ALFD, -1 on construction
				void* unk48;			// 48 - 0 on construction
				std::uint32_t alea;		// 50 - ALEA, -1 on construction
				std::uint32_t pad54;	// 54
				void* unk58;			// 58
			};
			static_assert(offsetof(BGSLocAlias, alfa) == 0x30);
			static_assert(offsetof(BGSLocAlias, alea) == 0x50);
			static_assert(sizeof(BGSLocAlias) == 0x60);

			// vtable RVA 0x30A2F30
			class BGSRefAlias : public BGSBaseAlias
			{
			public:
				std::uint8_t unk28[0x68];	// 28 - ref alias data (eight pointers from 40 are zeroed on construction), layout not resolved
			};
			static_assert(offsetof(BGSRefAlias, unk28) == 0x28);
			static_assert(sizeof(BGSRefAlias) == 0x90);
		}
	}
}