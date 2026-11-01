// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESActorBase.h>
#include <EditorAPI/B/TESRaceForm.h>
#include <EditorAPI/B/BGSOverridePackCollection.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x2E8, vtable RVA 0x30AFCC8 (NPC_)
				class TESNPC :
					public TESActorBase,				// 000
					public TESRaceForm,					// 178
					public BGSOverridePackCollection	// 188
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftNPC;

					virtual ~TESNPC() = default;
				public:
					std::uint8_t unk1B0[0x138];	// 1B0 - NPC data, layout not resolved
				};
				static_assert(offsetof(TESNPC, unk1B0) == 0x1B0);
				static_assert(sizeof(TESNPC) == 0x2E8);
			}
		}
	}
}