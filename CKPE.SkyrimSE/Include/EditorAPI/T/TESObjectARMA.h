// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESObject.h>
#include <EditorAPI/B/TESRaceForm.h>
#include <EditorAPI/B/BGSBipedObjectForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x1C8, vtable RVA 0x30FBE88
				// The CK TESObject occupies 0x30 bytes; TESObject here is 0x28, so the remaining 8 bytes are modelled as a base.
				struct TESObjectARMAPad28
				{
					std::uint8_t pad28[8];	// 28
				};

				class TESObjectARMA :
					public TESObject,				// 00
					public TESObjectARMAPad28,		// 28
					public TESRaceForm,				// 30
					public BGSBipedObjectForm		// 40
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftARMA;

					virtual ~TESObjectARMA() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
				public:
					std::uint8_t dnam[0x10];	// 50 - DNAM
					std::uint8_t unk60[0x48];	// 60 - MO2S/MO2T/MOD2
					std::uint8_t unkA8[0x48];	// A8 - MO3S/MO3T/MOD3
					std::uint8_t unkF0[0x48];	// F0 - MO4S/MO4T/MOD4
					std::uint8_t unk138[0x48];	// 138 - MO5S/MO5T/MOD5
					void* nam0;	// 180 - NAM0
					void* nam1;	// 188 - NAM1
					void* nam2;	// 190 - NAM2
					void* nam3;	// 198 - NAM3
					std::uint8_t pad1A0[0x18];	// 1A0
					void* sndd;	// 1B8 - SNDD
					void* onam;	// 1C0 - ONAM
				};
				static_assert(offsetof(TESObjectARMA, dnam) == 0x50);
				static_assert(offsetof(TESObjectARMA, unk60) == 0x60);
				static_assert(offsetof(TESObjectARMA, unkA8) == 0xA8);
				static_assert(offsetof(TESObjectARMA, unkF0) == 0xF0);
				static_assert(offsetof(TESObjectARMA, unk138) == 0x138);
				static_assert(offsetof(TESObjectARMA, nam0) == 0x180);
				static_assert(offsetof(TESObjectARMA, nam1) == 0x188);
				static_assert(offsetof(TESObjectARMA, nam2) == 0x190);
				static_assert(offsetof(TESObjectARMA, nam3) == 0x198);
				static_assert(offsetof(TESObjectARMA, sndd) == 0x1B8);
				static_assert(offsetof(TESObjectARMA, onam) == 0x1C0);
				static_assert(sizeof(TESObjectARMA) == 0x1C8);
			}
		}
	}
}