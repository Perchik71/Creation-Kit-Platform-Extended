// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <memory>
#include <span>
#include <string_view>
#include <type_traits>
#include <CKPE.HashUtils.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Detail
			{
				[[nodiscard]] inline static std::uint32_t GenerateCRC32(std::span<const std::uint8_t> data) noexcept(true)
				{
					// CK table hashes start at zero and do not complement the result.
					return HashUtils::CRC32Update(data.data(), (std::uint32_t)data.size(), 0);
				}

				template<class, bool>
				struct BSCRC32;

				template<class Key>
				struct BSCRC32<Key, true>
				{};
			}

			template<class Key>
			struct BSCRC32;

			template <class Key>
			struct BSCRC32 : public Detail::BSCRC32<Key, std::is_arithmetic_v<Key> ||
				std::is_enum_v<Key> || std::is_pointer_v<Key>>
			{
			public:
				[[nodiscard]] std::uint32_t operator()(Key data) const noexcept(true)
				{
					return Detail::GenerateCRC32({ reinterpret_cast<const std::uint8_t*>(std::addressof(data)), sizeof(Key) });
				}
			};

			template <>
			struct BSCRC32<BSFixedString>
			{
			public:
				[[nodiscard]] std::uint32_t operator()(const BSFixedString& data) const noexcept(true)
				{
					return BSCRC32<BSStringCache::Entry*>()(data.data);
				}
			};

			template <>
			struct BSCRC32<std::nullptr_t>
			{
			public:
				[[nodiscard]] std::uint32_t operator()(std::nullptr_t) const noexcept(true)
				{
					return BSCRC32<std::uintptr_t>()(0);
				}
			};

			template <class CharT>
			struct BSCRC32<std::basic_string_view<CharT>>
			{
			public:
				[[nodiscard]] std::uint32_t operator()(std::basic_string_view<CharT> data) const noexcept(true)
				{
					return Detail::GenerateCRC32({ data.data(), data.length() });
				}
			};

			extern template struct BSCRC32<std::int8_t>;
			extern template struct BSCRC32<std::uint8_t>;
			extern template struct BSCRC32<std::int16_t>;
			extern template struct BSCRC32<std::uint16_t>;
			extern template struct BSCRC32<std::int32_t>;
			extern template struct BSCRC32<std::uint32_t>;
			extern template struct BSCRC32<std::int64_t>;
			extern template struct BSCRC32<std::uint64_t>;
		}
	}
}
