// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BSPointerHandleManager.h>
#include <EditorAPI/ObjectRefHandle.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			Forms::TESForm* ObjectRefHandle::Ref() const noexcept(true) { return GetRefFormByUniqueId(_UniqueId); }
		}
	}
}