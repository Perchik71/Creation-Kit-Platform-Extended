// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Exception.h>
#include <EditorAPI/N/NiMemoryManager.h>

#include <limits.h>
#include <memory.h>
#include <iterator>
#include <algorithm>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				constexpr auto NITARRAY_GROW_SIZE = 10;
				constexpr auto NITARRAY_SHRINK_SIZE = 10;

				template <class T, std::uint16_t GROW = NITARRAY_GROW_SIZE, std::uint16_t SHRINK = NITARRAY_SHRINK_SIZE>
				class NiTArray
				{
				public:
					using value_type = T;
					using size_type = std::uint16_t;
					using difference_type = std::ptrdiff_t;
					using reference = value_type&;
					using const_reference = const value_type&;
					using pointer = value_type*;
					using const_pointer = const value_type*;
					using iterator = pointer;
					using const_iterator = const_pointer;
					using reverse_iterator = std::reverse_iterator<iterator>;
					using const_reverse_iterator = std::reverse_iterator<const_iterator>;
				private:
					T* _data;
					size_type len;
					size_type _capacity;

					void Deallocate()
					{
						NiAPI::NiMemoryManager::Free(nullptr, (void*)_data);
						_data = nullptr;
						_capacity = 0;
						len = 0;
					}

					bool Allocate(size_type numEntries)
					{
						_data = (T*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(value_type) * numEntries);
						if (!_data) return false;

						for (size_type i = 0; i < numEntries; i++)
							new (&_data[i]) value_type;

						_capacity = numEntries;
						len = numEntries;

						return true;
					}

					bool Shrink()
					{
						if (!_data || len == _capacity)
							return false;

						try
						{
							size_type newSize = len;
							value_type* oldArray = _data;
							value_type* newArray = (value_type*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(value_type) * newSize);	// Allocate new block
							memmove_s(newArray, sizeof(value_type) * newSize, _data, sizeof(value_type) * newSize);						// Move the old block
							_data = newArray;
							_capacity = len;
							NiAPI::NiMemoryManager::Free(nullptr, (void*)oldArray);														// Free the old block
							return true;
						}
						catch (...)
						{
							return false;
						}

						return false;
					}

					bool Grow(size_type numEntries)
					{
						if (!_data)
						{
							_data = (value_type*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(value_type) * numEntries);
							len = 0;
							_capacity = numEntries;
							return true;
						}

						try
						{
							size_type oldSize = _capacity;
							size_type newSize = oldSize + numEntries;
							value_type* oldArray = _data;
							value_type* newArray = (value_type*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(value_type) * newSize);	// Allocate new block
							if (oldArray)
								memmove_s(newArray, sizeof(value_type) * newSize, _data, sizeof(value_type) * _capacity);					// Move the old block
							_data = newArray;
							_capacity = newSize;

							if (oldArray)
								NiAPI::NiMemoryManager::Free(nullptr, (void*)oldArray);	// Free the old block

							for (size_type i = oldSize; i < newSize; i++)				// Allocate the rest of the free blocks
								new (&_data[i]) value_type;

							return true;
						}
						catch (...)
						{
							return false;
						}

						return false;
					}

					[[nodiscard]] inline value_type* _Myfirst() noexcept(true) { return (value_type*)data(); }
					[[nodiscard]] inline value_type* _Mylast() noexcept(true) { return ((value_type*)data()) + size(); }
					[[nodiscard]] inline const value_type* _const_Myfirst() const noexcept(true) { return (value_type*)data(); }
					[[nodiscard]] inline const value_type* _const_Mylast() const noexcept(true) { return ((value_type*)data()) + size(); }
				public:
					NiTArray() = default;
					~NiTArray() { clear(); };

					[[nodiscard]] inline constexpr reference operator[](const size_type Pos) noexcept(true) { return (this->_Myfirst()[Pos]); }
					[[nodiscard]] inline constexpr const_reference operator[](const size_type Pos) const noexcept(true) { return (this->_const_Myfirst()[Pos]); }

					[[nodiscard]] inline constexpr pointer data() noexcept(true) { return static_cast<pointer>(_data); }
					[[nodiscard]] inline constexpr const_pointer data() const noexcept(true) { return static_cast<const_pointer>(_data); }
					[[nodiscard]] inline constexpr size_type size() const noexcept(true) { return len; }
					[[nodiscard]] inline constexpr size_type capacity() const noexcept(true) { return _capacity; }
					[[nodiscard]] inline constexpr size_type max_size() const noexcept(true) { return std::numeric_limits<size_type>::max(); }
					[[nodiscard]] inline constexpr bool empty() const noexcept(true) { return len == 0; }
					[[nodiscard]] inline constexpr reference front() noexcept(true) { return (*this->_Myfirst()); }
					[[nodiscard]] inline constexpr const_reference const_front() const noexcept(true) { return (*this->_const_Myfirst()); }
					[[nodiscard]] inline constexpr reference back() noexcept(true) { return (this->_Mylast()[-1]); }
					[[nodiscard]] inline constexpr const_reference const_back() const noexcept(true) { return (this->_const_Mylast()[-1]); }
					[[nodiscard]] inline constexpr iterator begin() noexcept(true) { return data(); }
					[[nodiscard]] inline constexpr const_iterator begin() const noexcept(true) { return data(); }
					[[nodiscard]] inline constexpr const_iterator cbegin() const noexcept(true) { return begin(); }
					[[nodiscard]] inline constexpr iterator end() noexcept(true) { return begin() + size(); }
					[[nodiscard]] inline constexpr const_iterator end() const noexcept(true) { return begin() + size(); }
					[[nodiscard]] inline constexpr const_iterator cend() const noexcept(true) { return end(); }
					[[nodiscard]] inline constexpr reverse_iterator rbegin() noexcept(true) { return reverse_iterator(end()); }
					[[nodiscard]] inline constexpr const_reverse_iterator rbegin() const noexcept(true) { return rbegin(); }
					[[nodiscard]] inline constexpr const_reverse_iterator crbegin() const noexcept(true) { return rbegin(); }
					[[nodiscard]] inline constexpr reverse_iterator rend() noexcept(true) { return reverse_iterator(begin()); }
					[[nodiscard]] inline constexpr const_reverse_iterator rend() const noexcept(true) { return rend(); }
					[[nodiscard]] inline constexpr const_reverse_iterator crend() const noexcept(true) { return rend(); }

					[[nodiscard]] reference at(const size_type Pos)
					{
						if (size() <= Pos)
							throw RuntimeError("bounds check failed in NiTArray::at()");
						return (this->_Myfirst()[Pos]);
					}
					[[nodiscard]] const_reference at(const size_type Pos) const
					{
						if (size() <= Pos)
							throw RuntimeError("bounds check failed in NiTArray::at()");
						return (this->_const_Myfirst()[Pos]);
					}

					inline bool resize(size_type num) noexcept(true)
					{
						if (num == _capacity)
							return false;

						if (!_data)
						{
							Allocate(num);
							return true;
						}

						if (num < _capacity)
						{
							// Delete the truncated entries
							for (size_type i = num; i < _capacity; i++)
								(&_data[i])->~value_type();
						}

						value_type* newBlock = (value_type*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(value_type) * num);	// Create a new block
						memmove_s(newBlock, sizeof(value_type) * num, _data, sizeof(value_type) * num);							// Move the old memory to the new block
						if (num > _capacity)																						// Fill in new remaining entries
						{
							for (size_type i = _capacity; i < num; i++)
								new (&_data[i]) value_type;
						}
						NiAPI::NiMemoryManager::Free(nullptr, _data);							// Free the old block
						_data = newBlock;														// Assign the new block
						_capacity = num;															// _capacity is now the number of total entries in the block
						len = std::min(_capacity, len);											// Count stays the same, or is truncated to _capacity
						return true;
					}

					inline bool resize(size_type num, const value_type& value) noexcept(true)
					{
						if (resize(num))
							std::fill(begin(), end(), value);
					}

					inline void clear() noexcept(true) { Deallocate(); }

					void push_back(value_type const& entry)
					{
						if (!_data || len + 1 > _capacity)
						{
							if (!Grow(GROW))
								throw RuntimeError("out of memory BSTArray::push_back()");
						}

						_data[len] = entry;
						len++;
					}

					void push_back(value_type&& entry)
					{
						push_back(std::move(entry));
					}

					bool Insert(size_type index, const value_type& entry) noexcept(true)
					{
						if (!_data)
							return false;

						size_type lastSize = len;
						if (len + 1 > _capacity)								// Not enough space, grow
						{
							if (!Grow(GROW))
								return false;
						}

						if (index != lastSize)								// Not inserting onto the end, need to move everything down
						{
							size_type remaining = len - index;
							memmove_s(&_data[index + 1], sizeof(value_type) * remaining, &_data[index],
								sizeof(value_type) * remaining);			// Move the rest up
						}

						_data[index] = entry;
						len++;
						return true;
					};

					bool Remove(size_type index) noexcept(true)
					{
						if (!_data || index >= len)
							return false;

						// This might not be right for pointer types...
						(&_data[index])->~value_type();

						if (index + 1 < len)
						{
							size_type remaining = len - index;
							memmove_s(&_data[index], sizeof(value_type) * remaining, &_data[index + 1],
								sizeof(value_type) * remaining);	// Move the rest up
						}
						len--;

						if (_capacity > len + SHRINK)
							Shrink();

						return true;
					}
				};
				static_assert(sizeof(NiTArray<int32_t>) == 0x10);
			}
		}
	}
}