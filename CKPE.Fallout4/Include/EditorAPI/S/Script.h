// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESQuest;

				struct SCRIPT_HEADER
				{
					std::uint32_t variableCount; // 00
					std::uint32_t refObjectCount; // 04
					std::uint32_t dataSize; // 08
					std::uint32_t lastID; // 0C
					bool isQuestScript; // 10
					bool isMagicEffectScript; // 11
					bool isCompiled; // 12
				};
				static_assert(sizeof(SCRIPT_HEADER) == 0x14);

				// 1.11.240.0: size 0x78, vtable RVA 0x3066FB8, 127 virtual functions
				class Script :
					public TESForm   // 000
				{
				public:
					virtual ~Script() = default;

					virtual void sub_3E0() { return; } // 124 - RVA 0xBE2580
					virtual void sub_3E8() { return; } // 125 - RVA 0xBE2610
					virtual void sub_3F0() { return; } // 126 - RVA 0xBE4A60

				public:
					SCRIPT_HEADER header; // 28
					std::uint32_t unk3C; // 3C
					char* text; // 40
					std::byte* data; // 48
					TESQuest* parentQuest; // 50
					BSSimpleList<void*> refObjects; // 58 - BSSimpleList<SCRIPT_REFERENCED_OBJECT*>
					BSSimpleList<void*> variables; // 68 - BSSimpleList<ScriptVariable*>
				};
				static_assert(sizeof(Script) == 0x78);
			}
		}
	}
}
