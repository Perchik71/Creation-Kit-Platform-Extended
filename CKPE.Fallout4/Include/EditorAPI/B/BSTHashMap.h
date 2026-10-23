// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <functional>
#include <type_traits>
#include <EditorAPI/C/CRC.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			template <class First, class Second>
			struct BSTTuple
			{
				First first;
				Second second;
			};

			template <class Key, class T>
			struct BSTScatterTableTraits
			{
				using key_type = Key;
				using mapped_type = T;
				using value_type = BSTTuple<const Key, T>;
				[[nodiscard]] static const key_type& unwrap_key(const value_type& Value) noexcept(true) { return Value.first; }
			};

			template <class Key>
			struct BSTSetTraits
			{
				using key_type = Key;
				using mapped_type = void;
				using value_type = Key;
				[[nodiscard]] static const key_type& unwrap_key(const value_type& Value) noexcept(true) { return Value; }
			};

			// Chained scatter table as laid out by the Creation Kit: 0x38 bytes (entries pointer at +30, modification count at +20). CommonLibF4's
			// game table is 0x30 bytes (entries at +28); TableSize 0x30 selects that layout, but no Creation Kit table was found using it.
			// The table lives in game memory: it can be queried and walked, but not grown or modified from here.
			template <class Hash, class KeyEqual, class Traits, std::size_t TableSize = 0x38>
			class BSTScatterTable
			{
			public:
				using key_type = typename Traits::key_type;
				using mapped_type = typename Traits::mapped_type;
				using value_type = typename Traits::value_type;
				using size_type = std::uint32_t;

				struct entry_type
				{
					value_type value;
					entry_type* next;	// nullptr: free entry, sentinel: end of chain

					[[nodiscard]] bool has_value() const noexcept(true) { return next != nullptr; }
				};

				template <class U>
				class iterator_base
				{
				public:
					using iterator_category = std::forward_iterator_tag;
					using value_type = std::remove_const_t<U>;
					using difference_type = std::ptrdiff_t;
					using pointer = U*;
					using reference = U&;

					iterator_base() noexcept(true) = default;
					[[nodiscard]] reference operator*() const noexcept(true) { return _first->value; }
					[[nodiscard]] pointer operator->() const noexcept(true) { return std::addressof(_first->value); }
					[[nodiscard]] bool operator==(const iterator_base& Rhs) const noexcept(true) { return _first == Rhs._first; }
					[[nodiscard]] bool operator!=(const iterator_base& Rhs) const noexcept(true) { return _first != Rhs._first; }

					iterator_base& operator++() noexcept(true)
					{
						do { ++_first; } while (_first != _last && !_first->has_value());
						return *this;
					}

					iterator_base operator++(int) noexcept(true)
					{
						iterator_base tmp{ *this };
						operator++();
						return tmp;
					}

				private:
					friend class BSTScatterTable;

					iterator_base(entry_type* First, entry_type* Last) noexcept(true) : _first(First), _last(Last)
					{
						if (_first != _last && !_first->has_value())
							operator++();
					}

					entry_type* _first{ nullptr };
					entry_type* _last{ nullptr };
				};

				using iterator = iterator_base<value_type>;
				using const_iterator = iterator_base<const value_type>;

				[[nodiscard]] bool empty() const noexcept(true) { return size() == 0; }
				[[nodiscard]] size_type size() const noexcept(true) { return _capacity - _free; }
				[[nodiscard]] size_type capacity() const noexcept(true) { return _capacity; }

				[[nodiscard]] iterator begin() noexcept(true) { return iterator(_entries, _entries + _capacity); }
				[[nodiscard]] const_iterator begin() const noexcept(true) { return const_iterator(_entries, _entries + _capacity); }
				[[nodiscard]] iterator end() noexcept(true) { return iterator(_entries + _capacity, _entries + _capacity); }
				[[nodiscard]] const_iterator end() const noexcept(true) { return const_iterator(_entries + _capacity, _entries + _capacity); }

				[[nodiscard]] iterator find(const key_type& Key) noexcept(true) { return do_find<iterator>(Key); }
				[[nodiscard]] const_iterator find(const key_type& Key) const noexcept(true) { return do_find<const_iterator>(Key); }
				[[nodiscard]] bool contains(const key_type& Key) const noexcept(true) { return find(Key) != end(); }

			private:
				template <class Iter>
				[[nodiscard]] Iter do_find(const key_type& Key) const noexcept(true)
				{
					const auto last = _entries + _capacity;
					if (!_entries || empty())
						return Iter(last, last);

					auto entry = _entries + (static_cast<size_type>(Hash()(Key)) & (_capacity - 1));
					if (entry->has_value())
					{
						do
						{
							if (KeyEqual()(Traits::unwrap_key(entry->value), Key))
								return Iter(entry, last);
							entry = entry->next;
						} while (entry != reinterpret_cast<const entry_type*>(_sentinel));
					}

					return Iter(last, last);
				}

			private:
				std::uint64_t _pad00{ 0 };				// 00
				std::uint32_t _pad08{ 0 };				// 08
				size_type _capacity{ 0 };				// 0C - total number of slots, a power of two
				size_type _free{ 0 };					// 10 - number of free slots
				size_type _good{ 0 };					// 14 - last free index
				std::uintptr_t _sentinel{ 0 };			// 18 - end of chain marker (points at the bytes DE AD BE EF)
				std::uint8_t _padAllocator[TableSize - 0x28]{};	// 20 - 16 bytes in the CK's 0x38 layout (modification count at 20, entries at 30), 8 in the game's 0x30 one
				entry_type* _entries{ nullptr };		// 28
			};

			template <class Key, class T, class Hash = BSCRC32<Key>, class KeyEq = std::equal_to<Key>>
			using BSTHashMap = BSTScatterTable<Hash, KeyEq, BSTScatterTableTraits<Key, T>>;

			template <class Key, class Hash = BSCRC32<Key>, class KeyEq = std::equal_to<Key>>
			using BSTSet = BSTScatterTable<Hash, KeyEq, BSTSetTraits<Key>>;

			static_assert(sizeof(BSTHashMap<std::uint32_t, void*>) == 0x38);
			static_assert(sizeof(BSTScatterTable<BSCRC32<std::uint32_t>, std::equal_to<std::uint32_t>, BSTScatterTableTraits<std::uint32_t, void*>, 0x30>) == 0x30);
		}
	}
}
