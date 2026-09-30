// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstdint>
#include <atomic>
#include <type_traits>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/BSSpinLock.h>
#include <EditorAPI/IDs.h>

#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			struct BSStringPool
			{
				class Entry
				{
				public:
					enum
					{
						kWide = 1 << 15,
						kRefCountMask = 0x7FFF,
						kLengthMask = 0xFFFFFF
					};

					static inline void release(const char*& a_entry) { release8(a_entry); }
					static inline void release(const wchar_t*& a_entry) { release16(a_entry); }

					static inline void release8(const char*& a_entry)
					{
						using func_t = decltype(Entry::release8);
						Common::Relocation<func_t> func{ StringCache::StringPool_AnsiRelease };
						func(a_entry);
					}

					static inline void release16(const wchar_t*& a_entry)
					{
						using func_t = decltype(Entry::release16);
						Common::Relocation<func_t> func{ StringCache::StringPool_WideRelease };
						func(a_entry);
					}

					inline void acquire()
					{
						std::atomic_ref flags{ _flags };
						std::uint16_t   expected{ 0 };
						do {
							expected = flags;
							if ((expected & kRefCountMask) >= kRefCountMask)
								break;
						} while (!flags.compare_exchange_weak(expected, static_cast<std::uint16_t>(expected + 1)));
					}

					[[nodiscard]] constexpr std::uint16_t crc() const noexcept(true) { return _crc; }

					template <class T>
					[[nodiscard]] const T* data() const noexcept(true);

					template <>
					[[nodiscard]] inline const char* data<char>() const noexcept(true)
					{
						return u8();
					}

					template <>
					[[nodiscard]] inline const wchar_t* data<wchar_t>() const noexcept(true)
					{
						return u16();
					}

					[[nodiscard]] constexpr std::uint32_t length() const noexcept(true) { return _length & kLengthMask; }
					[[nodiscard]] constexpr std::uint32_t size() const noexcept(true) { return length(); }

					[[nodiscard]] inline const char* u8() const noexcept(true)
					{
						assert(!wide());
						return reinterpret_cast<const char*>(this + 1);
					}

					[[nodiscard]] inline const wchar_t* u16() const noexcept(true)
					{
						assert(wide());
						return reinterpret_cast<const wchar_t*>(this + 1);
					}

					[[nodiscard]] constexpr bool wide() const noexcept(true) { return static_cast<bool>(_flags & kWide); }

					// members
					Entry* _left;
					std::uint16_t _flags;
					volatile std::uint16_t _crc;
					union
					{
						std::uint32_t _length;
						Entry* _right;
					};
				};
				static_assert(sizeof(Entry) == 0x18);
			};
			static_assert(std::is_empty_v<BSStringPool>);

			struct BucketTable
			{
				enum HashMask
				{
					kEntryIndexMask = 0xFFFF,
					kLockIndexMask = 0x7F
				};

				static BucketTable* GetSingleton() noexcept(true)
				{
					using func_t = decltype(&BucketTable::GetSingleton);
					Common::Relocation<func_t> func{ StringCache::BucketTable_Singleton };
					return func();
				}

				// members
				BSStringPool::Entry* buckets[0x10000];			// 00000 - index using hash & kEntryIndexMask
				mutable BSSpinLock   locks[0x10000 / 0x800];	// 80000 - index using hash & kLockIndexMask
				bool				 initialized;				// 80100
			};
			static_assert(sizeof(BucketTable) == 0x80108);
		}
	}
}