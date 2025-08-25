// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstdint>
#include <EditorAPI/BSSimpleLock.h>

#pragma once

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			class BSStringCache
			{
			public:
				struct Lock
				{
					SimpleLock lock;
				};

				struct Entry
				{
					enum : std::uint8_t
					{
						kExternal = 1 << 1,
					};

					Entry* left;  // 00
					union
					{
						std::uint32_t len;	// Number of bytes (even for wchar_t).
						Entry* right;
					};  // 08
					volatile std::uint32_t refCount;  // 10
					std::uint32_t          flags;     // 14

					std::uint32_t acquire() noexcept(true);

					template <class T>
					[[nodiscard]] inline const T* data() const noexcept(true)
					{
						const auto entry = leaf();
						if (entry)
							return reinterpret_cast<const T*>(entry + 1);
						else
							return nullptr;
					}

					[[nodiscard]] inline const Entry* leaf() const noexcept(true)
					{
						auto iter = this;
						while (iter && iter->external())
							iter = iter->right;
						return iter;
					}

					[[nodiscard]] inline std::uint32_t length() const noexcept(true)
					{
						const auto entry = leaf();
						return entry ? entry->len : 0;
					}

					[[nodiscard]] inline bool          external() const noexcept(true) { return flags & kExternal; }
					[[nodiscard]] inline std::uint32_t size()	  const noexcept(true) { return length(); }

					template <class T>
					[[nodiscard]] inline const T* c_str() const noexcept(true) { return data<T>(); }
					template <class T>
					[[nodiscard]] inline operator const T*() const { return data<T>() ? data<T>() : ""; }
				};

				struct Ref
				{
					Entry* data{ nullptr };

					constexpr Ref() noexcept(true) = default;

					inline bool operator==(const Ref& lhs) const noexcept(true) { return data == lhs.data; }
					inline bool operator<(const Ref& lhs) const noexcept(true) { return data < lhs.data; }

					template <class T>
					[[nodiscard]] inline const T* c_str() const noexcept(true) { return data->data<T>(); }
					template <class T>
					[[nodiscard]] inline operator const T*() const { return data->data<T>() ? data->data<T>() : ""; }
				};

				BSStringCache() = default;
				~BSStringCache() = default;

				inline Lock* GetLock(std::uint32_t crc16) { return &locks[crc16 & 0xFF]; }
			private:
				Entry* lut[0x40000];
				Lock locks[0x100];
				std::uint8_t isInit;
			};

			static_assert(sizeof(BSStringCache::Lock) == 0x8);
			static_assert(sizeof(BSStringCache::Ref) == 0x8);
			static_assert(sizeof(BSStringCache::Entry) == 0x18);
		}
	}
}