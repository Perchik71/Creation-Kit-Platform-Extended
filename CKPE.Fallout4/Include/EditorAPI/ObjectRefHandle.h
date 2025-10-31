// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
			}

			class ObjectRefHandle
			{
				std::uint32_t _UniqueId;
			public:
				ObjectRefHandle() : _UniqueId(0) {}
				ObjectRefHandle(std::uint32_t UniqueId) : _UniqueId(UniqueId) {}
				ObjectRefHandle(const ObjectRefHandle& rhs) : _UniqueId(rhs._UniqueId) {}
				
				inline ObjectRefHandle& operator=(const ObjectRefHandle& rhs) noexcept(true) { _UniqueId = rhs._UniqueId; }

				[[nodiscard]] inline std::uint32_t Id() const noexcept(true) { return _UniqueId; }
				[[nodiscard]] Forms::TESForm* Ref() const noexcept(true);

				inline bool operator==(const ObjectRefHandle& rhs) const noexcept(true) { return _UniqueId == rhs._UniqueId; }
				inline bool operator!=(const ObjectRefHandle& rhs) const noexcept(true) { return _UniqueId != rhs._UniqueId; }
			};
			static_assert(sizeof(ObjectRefHandle) == 0x4);

			inline static bool operator==(const ObjectRefHandle& lhs, const ObjectRefHandle& rhs) noexcept(true) { return lhs.Id() == rhs.Id(); }
			inline static bool operator!=(const ObjectRefHandle& lhs, const ObjectRefHandle& rhs) noexcept(true) { return lhs.Id() != rhs.Id(); }
		}
	}
}