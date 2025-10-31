// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include "TESActorValue.h"
//#include "TESObjectCELL.h"
#include "BGSPreviewTransform.h"
#include "BGSSoundTagComponent.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObject : public TESForm
				{
				private:
					/*028*/ char pad28[8];
				public:
					virtual ~TESObject() = default;
				};
			}
		}
	}
}
