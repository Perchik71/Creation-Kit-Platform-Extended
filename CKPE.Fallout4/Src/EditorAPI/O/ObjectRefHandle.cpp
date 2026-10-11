// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSPointerHandleManager.h>
#include <EditorAPI/O/ObjectRefHandle.h>

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