// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;

				// Native check-in selection list; not BSSimpleList<TESForm>.
				struct CheckInFormList
				{
					struct Node
					{
						Node* Next;
						Node* Previous;
						TESForm* Form;
					};
					static_assert(sizeof(Node) == 0x18);

					Node* Head;
					Node* Tail;
					std::uint32_t Count;
					char pad14[0x4];
				};
				static_assert(sizeof(CheckInFormList) == 0x18);
			}
		}
	}
}
