// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSSoundTagComponent : public BaseFormComponent
			{
				std::uint32_t _Tag{ 0x43544143 };
			public:
				constexpr BGSSoundTagComponent() = default;
				BGSSoundTagComponent(std::uint32_t tag) : _Tag(tag) {}
				BGSSoundTagComponent(const BGSSoundTagComponent& Rhs) : _Tag(Rhs._Tag) {}

				BGSSoundTagComponent& operator=(const BGSSoundTagComponent& Rhs) noexcept(true) { _Tag = Rhs._Tag; return *this; }

				constexpr bool operator==(const BGSSoundTagComponent& Rhs) const noexcept(true) { return _Tag == Rhs._Tag; }
				constexpr bool operator!=(const BGSSoundTagComponent& Rhs) const noexcept(true) { return _Tag != Rhs._Tag; }

				virtual ~BGSSoundTagComponent() = default;
			public:
				[[nodiscard]] inline std::uint32_t GetTag() const noexcept(true) { return _Tag; }
				inline void SetTag(std::uint32_t tag) noexcept(true) { _Tag = tag; }
			public:
				CKPE_PROPERTY(GetTag, SetTag) std::uint32_t Tag;
			};
			static_assert(sizeof(BGSSoundTagComponent) == 0x10);
		}
	}
}