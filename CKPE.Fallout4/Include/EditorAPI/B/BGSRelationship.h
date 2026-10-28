// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESNPC;
				class BGSAssociationType;

				// 1.11.240.0: size 0x48, vtable RVA 0x303D528, 124 virtual functions
				class BGSRelationship :
					public TESForm   // 000
				{
				public:
					virtual ~BGSRelationship() = default;

					constexpr static std::uint8_t TYPE_ID = ftRelationship;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSRelationship[0];
				public:
					TESNPC* npc1; // 28
					TESNPC* npc2; // 30
					BGSAssociationType* assocType; // 38
					std::uint32_t packedData; // 40
					std::uint32_t unk44; // 44
				};
				static_assert(sizeof(BGSRelationship) == 0x48);
			}
		}
	}
}
