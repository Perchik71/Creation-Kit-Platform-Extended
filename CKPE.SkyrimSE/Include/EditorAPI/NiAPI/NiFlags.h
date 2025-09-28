// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				template <class Integral, class>
				class NiTFlags
				{
				public:
					using integral_type = Integral;

					constexpr NiTFlags() noexcept(true) {}

					integral_type Flags{ 0 };

					inline void Set(integral_type v) noexcept(true)
					{
						Flags |= v;
					}

					inline void Unset(integral_type v) noexcept(true)
					{
						Flags &= ~v;
					}

					[[nodiscard]] inline bool Has(integral_type v) const noexcept(true)
					{
						return (Flags & v) != 0;
					}
				};
			}
		}
	}
}
