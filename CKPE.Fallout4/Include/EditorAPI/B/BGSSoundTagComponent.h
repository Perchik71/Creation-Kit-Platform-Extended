// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// CK 1.11.240.0: size 0x8 (only the vtable pointer; the scalar deleting destructor frees 8 bytes),
			// 21 virtual functions. The constructor only stores the vtable, the destructor takes the global spin lock.
			// The dword that follows the component inside TESBoundObject (0x88) belongs to TESBoundObject, not to the component.
			class BGSSoundTagComponent :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSSoundTagComponent() = default;
			};
			static_assert(sizeof(BGSSoundTagComponent) == 0x8);
		}
	}
}
