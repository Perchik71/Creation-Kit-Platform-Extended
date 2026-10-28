// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <functional>
#include <type_traits>

#include <EditorAPI/M/MemoryManager.h>
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

			template <std::size_t S, std::size_t A>
			class BSTScatterTableHeapAllocator
			{
			public:
				using size_type = std::uint32_t;
				using propagate_on_container_move_assignment = std::true_type;

				BSTScatterTableHeapAllocator() = default;
				BSTScatterTableHeapAllocator(const BSTScatterTableHeapAllocator&) = delete;

				BSTScatterTableHeapAllocator(BSTScatterTableHeapAllocator&& a_rhs) noexcept(true) :
					_entries(std::exchange(a_rhs._entries, nullptr))
				{}

				~BSTScatterTableHeapAllocator() = default;
				BSTScatterTableHeapAllocator& operator=(const BSTScatterTableHeapAllocator&) = delete;

				BSTScatterTableHeapAllocator& operator=(BSTScatterTableHeapAllocator&& a_rhs) noexcept(true)
				{
					if (this != std::addressof(a_rhs))
					{
						assert(_entries == nullptr);
						_entries = std::exchange(a_rhs._entries, nullptr);
					}

					return *this;
				}

				[[nodiscard]] static constexpr size_type min_size() noexcept(true) { return 1u << 3; }

				[[nodiscard]] void* allocate_bytes(std::size_t a_bytes)
				{
					assert(a_bytes % S == 0);
					return malloc(a_bytes);
				}

				void deallocate_bytes(void* a_ptr) { free(a_ptr); }

				[[nodiscard]] void* get_entries() const noexcept(true) { return _entries; }
				void                set_entries(void* a_entries) noexcept(true) { _entries = static_cast<std::byte*>(a_entries); }

			private:
				// members
				std::uint64_t _pad00{ 0 };
				std::uint64_t _pad08{ 0 };
				std::byte* _entries{ nullptr };
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

			template <class Hash, class KeyEqual, class Traits, template <std::size_t, std::size_t> class Allocator, std::size_t TableSize = 0x38>
			class BSTScatterTable
			{
			public:
				using key_type = typename Traits::key_type;
				using mapped_type = typename Traits::mapped_type;
				using value_type = typename Traits::value_type;
				using size_type = std::uint32_t;
				using reference = value_type&;
				using const_reference = const value_type&;
				using pointer = value_type*;
				using const_pointer = const value_type*;

				static_assert(std::is_invocable_r_v<size_type, const Hash&, const key_type&>);
				static_assert(std::is_invocable_r_v<bool, const KeyEqual&, const key_type&, const key_type&>);

				struct entry_type
				{
					value_type value;
					entry_type* next;	// nullptr: free entry, sentinel: end of chain

					[[nodiscard]] bool has_value() const noexcept(true) { return next != nullptr; }
				};

				using allocator_type = Allocator<sizeof(entry_type), alignof(entry_type)>;

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

				[[nodiscard]] iterator begin() noexcept(true) { return iterator(_allocator._entries, _allocator._entries + _capacity); }
				[[nodiscard]] const_iterator begin() const noexcept(true) { return const_iterator(_allocator._entries, _allocator._entries + _capacity); }
				[[nodiscard]] iterator end() noexcept(true) { return iterator(_allocator._entries + _capacity, _allocator._entries + _capacity); }
				[[nodiscard]] const_iterator end() const noexcept(true) { return const_iterator(_allocator._entries + _capacity, _allocator._entries + _capacity); }

				[[nodiscard]] iterator find(const key_type& Key) noexcept(true) { return do_find<iterator>(Key); }
				[[nodiscard]] const_iterator find(const key_type& Key) const noexcept(true) { return do_find<const_iterator>(Key); }
				[[nodiscard]] bool contains(const key_type& Key) const noexcept(true) { return find(Key) != end(); }

			private:
				template <class Iter>
				[[nodiscard]] Iter do_find(const key_type& Key) const noexcept(true)
				{
					const auto last = _allocator._entries + _capacity;
					if (!_allocator._entries || empty())
						return Iter(last, last);

					auto entry = _allocator._entries + (static_cast<size_type>(Hash()(Key)) & (_capacity - 1));
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
				allocator_type _allocator;
			};

			template <class Key, class T, class Hash = BSCRC32<Key>, class KeyEq = std::equal_to<Key>>
			using BSTHashMap = BSTScatterTable<Hash, KeyEq, BSTScatterTableTraits<Key, T>, BSTScatterTableHeapAllocator>;

			template <class Key, class Hash = BSCRC32<Key>, class KeyEq = std::equal_to<Key>>
			using BSTSet = BSTScatterTable<Hash, KeyEq, BSTSetTraits<Key>, BSTScatterTableHeapAllocator>;

			static_assert(sizeof(BSTHashMap<std::uint32_t, void*>) == 0x38);
			static_assert(sizeof(BSTScatterTable<BSCRC32<std::uint32_t>, std::equal_to<std::uint32_t>, BSTScatterTableTraits<std::uint32_t, void*>, BSTScatterTableHeapAllocator>) == 0x38);
		}
	}
}
