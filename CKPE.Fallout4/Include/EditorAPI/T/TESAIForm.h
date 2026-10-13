// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESPackage;
			}

			// 0x10 bytes, bitfield layout from CommonLibF4 (default 0x03000400 stored by the CK constructor)
			struct AIDATA_GAME
			{
				std::uint32_t aggression : 2;		// 00:00
				std::uint32_t confidence : 3;		// 00:02
				std::uint32_t energy : 8;			// 00:05
				std::uint32_t morality : 2;			// 00:13
				std::uint32_t unused : 3;			// 00:15
				std::uint32_t assistance : 2;		// 00:18
				std::uint32_t useAggroRadius : 1;	// 00:20
				std::uint16_t aggroRadius[3];		// 04
				std::uint32_t noSlowApproach : 1;	// 0C:00
			};
			static_assert(sizeof(AIDATA_GAME) == 0x10);


			// 1.11.240.0: size 0x30, vtable RVA 0x301C2C0, 21 virtual functions
			class TESAIForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESAIForm() = default;

			public:
				AIDATA_GAME aiData; // 08
				std::uint32_t unk18; // 18 - CK only
				std::uint32_t unk1C; // 1C - CK only
				BSSimpleList<Forms::TESPackage*> aiPackList; // 20 - CommonLibF4 PackageList (at 18 there)
			};
			static_assert(sizeof(TESAIForm) == 0x30);
		}
	}
}
