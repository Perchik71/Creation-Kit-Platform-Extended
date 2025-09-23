// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <EditorAPI/BSSimpleLock.h>

#pragma once

namespace CKPE
{
	namespace Fallout4
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
					enum
					{
						kState_RefcountMask = 0x3FFF,
						kState_External = 0x4000,
						kState_Wide = 0x8000
					};

					Entry* left;				// 00
					std::uint16_t flags;		// 08	refcount, flags
					volatile std::uint16_t crc;	// 0A
					std::uint32_t unk1;			// 0C
					// with 4000 flag, is 0
					std::uint32_t len;			// 10
					std::uint32_t unk2;			// 14
					union						// 18
					{
						struct s1
						{
							const char str[1];
						} direct;
						struct s2
						{
							Entry* right;
						} extra;
					};

					std::uint16_t acquire() noexcept(true);

					[[nodiscard]] inline const char* data() const noexcept(true)
					{
						const auto entry = leaf();
						if (entry)
						{
							CKPE_ASSERT(!is_wide());
							return entry->direct.str;
						}
						else
							return nullptr;
					}

					[[nodiscard]] inline const wchar_t* wdata() const noexcept(true)
					{
						const auto entry = leaf();
						if (entry)
						{
							CKPE_ASSERT(is_wide());
							return (const wchar_t*)entry->direct.str;
						}
						else
							return nullptr;
					}

					[[nodiscard]] inline const Entry* leaf() const noexcept(true)
					{
						auto iter = this;
						while (iter && iter->external())
							iter = iter->extra.right;
						return iter;
					}

					[[nodiscard]] inline std::uint32_t length() const noexcept(true)
					{
						const auto entry = leaf();
						return entry ? entry->len : 0;
					}

					[[nodiscard]] inline bool          external() const noexcept(true) { return flags & kState_External; }
					[[nodiscard]] inline bool          is_wide() const noexcept(true) { return flags & kState_Wide; }
					[[nodiscard]] inline std::uint32_t size() const noexcept(true) { return length(); }

					[[nodiscard]] inline const char* c_str() const noexcept(true) { return data() ? data() : ""; }
					[[nodiscard]] inline const wchar_t* c_wstr() const noexcept(true) { return wdata() ? wdata() : L""; }
					[[nodiscard]] inline operator const char*() const noexcept(true) { return c_str(); }
					[[nodiscard]] inline operator const wchar_t*() const noexcept(true) { return c_wstr(); }
				};

				struct Ref
				{
					Entry* data{ nullptr };

					constexpr Ref() noexcept(true) = default;

					inline bool operator==(const Ref& lhs) const noexcept(true) { return data == lhs.data; }
					inline bool operator<(const Ref& lhs) const noexcept(true) { return data < lhs.data; }

					[[nodiscard]] inline std::uint32_t length() const noexcept(true) { return empty() ? 0 : data->length(); }
					[[nodiscard]] inline bool          external() const noexcept(true) { return empty() ? false : data->external(); }
					[[nodiscard]] inline bool          is_wide() const noexcept(true) { return empty() ? false : data->is_wide(); }
					[[nodiscard]] inline bool          empty() const noexcept(true) { return data == nullptr; }
					[[nodiscard]] inline std::uint32_t size() const noexcept(true) { return length(); }

					[[nodiscard]] inline const char* c_str() const noexcept(true) { return empty() ? "" : data->data(); }
					[[nodiscard]] inline const wchar_t* c_wstr() const noexcept(true) { return empty() ? L"" : data->wdata(); }
					[[nodiscard]] inline operator const char*() const noexcept(true) { return c_str(); }
					[[nodiscard]] inline operator const wchar_t*() const noexcept(true) { return c_wstr(); }
				};

				BSStringCache() = default;
				~BSStringCache() = default;

				inline Lock* GetLock(std::uint32_t crc16) { return &locks[crc16 & 0xFF]; }
			private:
				Entry* lut[0x10000];	// 00000
				Lock locks[0x80];		// 80000
				std::uint8_t isInit;	// 80800
			};

			static_assert(sizeof(BSStringCache::Lock) == 0x8);
			static_assert(sizeof(BSStringCache::Ref) == 0x8);
			static_assert(sizeof(BSStringCache::Entry) == 0x20);
		}
	}
}