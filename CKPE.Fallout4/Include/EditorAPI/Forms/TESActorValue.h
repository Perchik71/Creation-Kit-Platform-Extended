// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BSFixedString.h>
#include "TESForm.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESActorValue : public TESForm 
				{
				private:
					constexpr static std::uint8_t TYPE_ID = ftActorValueInfo;

					enum { eTypeID = FormType::ftActorValueInfo };
				public:
					//	/*68*/ TESActorValue* _ParentActorValue;		// ???
				};
				//static_assert(sizeof(TESActorValue) == 0x70);

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