// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/A/ActorValueInfo.h>
#include "TESForm.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// AVIF records use ActorValueInfo, not a separate TESActorValue type.
				using TESActorValue = ActorValueInfo;
				static_assert(sizeof(TESActorValue) == 0x1A8);

				class TESActorValueArray : public BaseFormComponent 
				{
				public:
					struct ActorValueInfoT 
					{
						TESActorValue* ActorValueForm;
						float Value;
					};
				public:
					BSTArray<ActorValueInfoT>* Array;
				};
				static_assert(sizeof(TESActorValueArray) == 0x10);
				static_assert(sizeof(TESActorValueArray::ActorValueInfoT) == 0x10);
			}
		}
	}
}