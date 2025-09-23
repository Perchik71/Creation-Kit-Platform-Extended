// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiObject;
				class NiRTTI
				{
				public:
					const char* GetNameClass() const noexcept(true) { return _Name; }
					NiRTTI* GetParent() const noexcept(true) { return _Parent; }
				private:
					const char* _Name;
					NiRTTI* _Parent;
				};

				NiObject* _NI_DYNAMIC_CAST(const NiObject* Src, const char* lpstrFromType);
			}
		}
	}
}