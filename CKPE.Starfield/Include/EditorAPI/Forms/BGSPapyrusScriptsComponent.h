// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"
#include "../Interfaces/IPapyrusScriptsBase.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSPapyrusScriptsComponent : public BaseFormComponent
				{
				public:
					inline ~BGSPapyrusScriptsComponent() {}

					Interfaces::IPapyrusScriptsBase PapyrusScriptsIntf;
				private:
					char pad40__[0xA0];
				};
				static_assert(sizeof(BGSPapyrusScriptsComponent) == 0xE8);
			}
		}
	}
}