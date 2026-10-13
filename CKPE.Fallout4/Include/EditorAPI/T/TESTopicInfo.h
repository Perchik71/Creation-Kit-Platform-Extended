// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESTopic;
				// size = 0xE0
				// func = 126
				class TESTopicInfo :
					public TESForm   // 000
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTopicInfo;

					virtual ~TESTopicInfo() = default;
					// CK 1.11.240.0: 126 virtual functions (TESForm has 124)
					virtual void sub_3E0() { return; } // 124 - RVA 0xB6A6D0
					virtual void sub_3E8() { return; } // 125 - RVA 0xB6A7B0
				public:
					struct Response
					{
						BGSLocalizedStringIL ResponseText;
						Response* Next;
						char pad18[0x2];
						std::uint8_t Index;

						inline bool End() const { return Next == nullptr; }
					};

					inline TESTopic* GetParentTopic() const { return _ParentTopic; }
					inline Response* GetFirstResponse() const { return _First; }
				private:
					char pad28[0x8];
					TESTopic* _ParentTopic;
					char pad38[0x30];
					Response* _First;
					char pad70[0x70];
				};
				static_assert(sizeof(TESTopicInfo) == 0xE0);
			}
		}
	}
}