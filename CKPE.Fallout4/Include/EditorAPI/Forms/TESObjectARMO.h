// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESBoundObject.h"
#include "TESCellUseList.h"
#include "TESFullName.h"
#include "BGSKeyword.h"
#include "BGSModelMaterialSwap.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x3C0
				// func = 154
				class TESObjectARMO : public TESBoundObject, public TESFullName
				{
				public:
					virtual ~TESObjectARMO() = default;
				public:
					inline BGSModelMaterialSwap& GetMaleModel() { return _MaleModel; }
					inline BGSModelMaterialSwap& GetWomanModel() { return _WomanModel; }
					inline TESForm* GetRace() { return _RaceForm; }
					inline BGSKeyword** GetKeywords() { return _Keywords; }
					inline std::uint32_t GetKeywordCount() const { return _KeywordCount; }
				private:
					char padA8[0x8];
					TESForm* _RaceForm;
					char padB8[0x48];
					BGSModelMaterialSwap _MaleModel;
					BGSModelMaterialSwap _WomanModel;
					// Old data (Message icons and model)
					char pad1A0[0xB0];
					// TESModelRDT
					char pad250[0x80];
					BGSKeyword** _Keywords;
					std::uint32_t _KeywordCount;
					char pad2DC[0xE4];
				};
				static_assert(sizeof(TESObjectARMO) == 0x3C0);
			}
		}
	}
}