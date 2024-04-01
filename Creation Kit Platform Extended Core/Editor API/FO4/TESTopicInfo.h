// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESFormF4.h"
#include "..\BGSLocalizedString.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESTopic;
			// size = 0xE0
			// func = 126
			class TESTopicInfo : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftTopicInfo;
		
				virtual ~TESTopicInfo() = default;
			public:
				struct Response
				{
					BGSLocalizedStringIL<BSEntryString> ResponseText;
					Response* Next;
					char pad18[0x2];
					uint8_t Index;

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

#pragma pack(pop)