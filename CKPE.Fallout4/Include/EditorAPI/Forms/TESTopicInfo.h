// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include "..\BGSLocalizedString.h"

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
				class TESTopicInfo : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTopicInfo;

					virtual ~TESTopicInfo() = default;
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