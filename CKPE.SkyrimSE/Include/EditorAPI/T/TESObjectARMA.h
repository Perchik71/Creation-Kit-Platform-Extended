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
					std::uint8_t unk50[0x178];	// 50 - armor addon data, layout not resolved
				};
				static_assert(offsetof(TESObjectARMA, unk50) == 0x50);
				static_assert(sizeof(TESObjectARMA) == 0x1C8);
			}
		}
	}
}