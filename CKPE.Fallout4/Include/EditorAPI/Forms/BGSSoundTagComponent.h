// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x10
				// func = 21
				class BGSSoundTagComponent
				{
				public:
					virtual ~BGSSoundTagComponent() = default;
				public:
					[[nodiscard]] inline std::uint32_t GetTag() const noexcept(true) { return _Tag; }
				public:
					CKPE_READ_PROPERTY(GetTag) std::uint32_t Tag;
				private:
					std::uint32_t _Tag{ 0x43544143 };
				};
				static_assert(sizeof(BGSSoundTagComponent) == 0x10);
			}
		}
	}
}