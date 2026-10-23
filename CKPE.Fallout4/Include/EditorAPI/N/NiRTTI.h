// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>

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
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiRTTI, _Name) == 0x0 &&
							offsetof(NiRTTI, _Parent) == 0x8;
					}
				};
				static_assert(sizeof(NiRTTI) == 0x10);
				static_assert(NiRTTI::VerifyLayout());

				NiObject* _NI_DYNAMIC_CAST(const NiObject* Src, const char* lpstrFromType);
			}
		}
	}
}