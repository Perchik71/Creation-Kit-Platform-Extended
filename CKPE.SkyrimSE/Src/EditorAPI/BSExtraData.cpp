// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <EditorAPI/BSExtraData.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			bool BSExtraData::operator==(const BSExtraData& rhs) const noexcept(true)
			{
				return !operator!=(rhs);
			}

			bool BSExtraData::operator!=(const BSExtraData& rhs) const noexcept(true)
			{
				if (GetType() != rhs.GetType()) return true;
				else return IsNotEqual(&rhs);
			}
		}
	}
}