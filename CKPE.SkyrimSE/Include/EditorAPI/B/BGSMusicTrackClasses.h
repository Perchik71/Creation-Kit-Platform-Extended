// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

#include <EditorAPI/B/BSIMusicTrack.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// Music tracks (1.7.99.0).

			// vtable RVA 0x30CB010, 27 virtual functions
			class BGSMusicTrack : public BSIMusicTrack
			{
			public:
				std::uint32_t unk08;	// 08
				std::uint32_t pad0C;	// 0C
				void* unk10;			// 10 - 0 on construction
				std::uint32_t unk18;	// 18 - 0 on construction
				std::uint32_t pad1C;	// 1C
			};
			static_assert(sizeof(BGSMusicTrack) == 0x20);

			// vtable RVA 0x30CA5E0
			class BGSMusicSilenceTrack : public BGSMusicTrack
			{
			public:
				std::uint32_t unk20;	// 20 - FLTV, 0 on construction
				std::uint32_t pad24;	// 24
				void* unk28;			// 28 - 0 on construction
				void* unk30;			// 30 - 0 on construction
			};
			static_assert(sizeof(BGSMusicSilenceTrack) == 0x38);

			// vtable RVA 0x30CA9B8
			class BGSMusicSingleTrack : public BGSMusicTrack
			{
			public:
				std::uint8_t unk20[0x40];	// 20
				std::uint32_t unk60;		// 60 - 0 on construction
				std::uint32_t unk64;		// 64 - 0 on construction
				std::uint32_t unk68;		// 68 - 0 on construction
				std::uint32_t pad6C;		// 6C
				void* lnam;					// 70 - LNAM, 0 on construction
				std::uint8_t unk78[0x18];	// 78
			};
			static_assert(offsetof(BGSMusicSingleTrack, unk60) == 0x60);
			static_assert(offsetof(BGSMusicSingleTrack, lnam) == 0x70);
			static_assert(sizeof(BGSMusicSingleTrack) == 0x90);

			// vtable RVA 0x30C9E00, 29 virtual functions
			class BGSMusicPaletteTrack : public BGSMusicTrack
			{
			public:
				std::uint8_t unk20[0x54];	// 20
				std::uint32_t unk74;		// 74 - 0 on construction
				std::uint32_t unk78;		// 78 - 0 on construction
				std::uint32_t pad7C;		// 7C
				void* unk80;				// 80 - 0 on construction
				void* unk88;				// 88 - 0 on construction
				std::uint32_t unk90;		// 90 - 0 on construction
				std::uint32_t pad94;		// 94
			};
			static_assert(offsetof(BGSMusicPaletteTrack, unk74) == 0x74);
			static_assert(sizeof(BGSMusicPaletteTrack) == 0x98);
		}
	}
}