// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <limits.h>
#include <memory.h>
#include <iterator>
#include <CKPE.Exception.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			constexpr auto BSTARRAY_GROW_SIZE = 10;
			constexpr auto BSTARRAY_SHRINK_SIZE = 10;

			template <class _Ty, std::uint32_t GROW = BSTARRAY_GROW_SIZE, std::uint32_t SHRINK = BSTARRAY_SHRINK_SIZE>
			class BSTArray {
				friend class __BSTArrayCheckOffsets;
			public:
				using value_type = _Ty;
				using size_type = std::uint32_t;
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
				[[nodiscard]] inline _Ty* _Myfirst() noexcept(true) { return (_Ty*)data(); }
				[[nodiscard]] inline _Ty* _Mylast() noexcept(true) { return ((_Ty*)data()) + size(); }
				[[nodiscard]] inline const _Ty* _const_Myfirst() const noexcept(true) { return (_Ty*)data(); }
				[[nodiscard]] inline const _Ty* _const_Mylast() const noexcept(true) { return ((_Ty*)data()) + size(); }
			private:
				_Ty* m_Buffer{ nullptr };
				size_type m_AllocSize{ 0 };
				size_type pad0C{ 0 };
				size_type m_Size{ 0 };
				size_type pad14{ 0 };
			private:
				void Deallocate() 
				{
					NiAPI::NiMemoryManager::Free(nullptr, (void*)m_Buffer);
					m_Buffer = nullptr;
					m_AllocSize = 0;
					m_Size = 0;
				}

				bool Allocate(size_type numEntries) 
				{
					m_Buffer = (_Ty*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Ty) * numEntries);
					if (!m_Buffer) return false;

					for (size_type i = 0; i < numEntries; i++)
						new (&m_Buffer[i]) _Ty;

					m_AllocSize = numEntries;
					m_Size = numEntries;

					return true;
				}
			protected:
				bool Shrink()
				{
					if (!m_Buffer || m_Size == m_AllocSize)
						return false;

					try
					{
						size_type newSize = m_Size;
						_Ty* oldArray = m_Buffer;
						_Ty* newArray = (_Ty*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Ty) * newSize);	// Allocate new block
						memmove_s(newArray, sizeof(_Ty) * newSize, m_Buffer, sizeof(_Ty) * newSize);			// Move the old block
						m_Buffer = newArray;
						m_AllocSize = m_Size;
						NiAPI::NiMemoryManager::Free(nullptr, (void*)oldArray);									// Free the old block
						return true;
					}
					catch (...) 
					{
						return false;
					}

					return false;
				}

				bool Grow(std::uint64_t numEntries)
				{
					if (!m_Buffer)
					{
						m_Buffer = (_Ty*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Ty) * (std::uint32_t)numEntries);
						m_Size = 0;
						m_AllocSize = (std::uint32_t)numEntries;
						return true;
					}

					try 
					{
						size_type oldSize = m_AllocSize;
						size_type newSize = oldSize + (std::uint32_t)numEntries;
						_Ty* oldArray = m_Buffer;
						_Ty* newArray = (_Ty*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Ty) * newSize);	// Allocate new block
						if (oldArray)
							memmove_s(newArray, sizeof(_Ty) * newSize, m_Buffer, sizeof(_Ty) * m_AllocSize);	// Move the old block
						m_Buffer = newArray;
						m_AllocSize = newSize;

						if (oldArray)
							NiAPI::NiMemoryManager::Free(nullptr, (void*)oldArray);	// Free the old block

						for (size_type i = oldSize; i < newSize; i++)					// Allocate the rest of the free blocks
							new (&m_Buffer[i]) _Ty;

						return true;
					}
					catch (...) 
					{
						return false;
					}

					return false;
				}
			public:
				BSTArray() noexcept(true) = default;
				~BSTArray() { clear(); }
			public:
				[[nodiscard]] inline constexpr reference operator[](const size_type Pos) noexcept(true) { return (this->_Myfirst()[Pos]); }
				[[nodiscard]] inline constexpr const_reference operator[](const size_type Pos) const noexcept(true) { return (this->_const_Myfirst()[Pos]); }

				[[nodiscard]] inline constexpr pointer       data() noexcept(true) { return static_cast<pointer>(m_Buffer); }
				[[nodiscard]] inline constexpr const_pointer data() const noexcept(true) { return static_cast<const_pointer>(m_Buffer); }
				[[nodiscard]] inline constexpr size_type size() const noexcept(true) { return m_Size; }
				[[nodiscard]] inline constexpr size_type capacity() const noexcept(true) { return m_AllocSize; }
				[[nodiscard]] inline constexpr size_type max_size() const noexcept(true) { return std::numeric_limits<size_type>::max(); }
				[[nodiscard]] inline constexpr bool empty() const noexcept(true) { return m_Size == 0; }
				[[nodiscard]] inline constexpr reference front() noexcept(true) { return (*this->_Myfirst()); }
				[[nodiscard]] inline constexpr const_reference const_front() const noexcept(true) { return (*this->_const_Myfirst()); }
				[[nodiscard]] inline constexpr reference back() noexcept(true) { return (this->_Mylast()[-1]); }
				[[nodiscard]] inline constexpr const_reference const_back() const noexcept(true) { return (this->_const_Mylast()[-1]); }
				[[nodiscard]] inline constexpr iterator       begin() noexcept(true) { return data(); }
				[[nodiscard]] inline constexpr const_iterator begin() const noexcept(true) { return data(); }
				[[nodiscard]] inline constexpr const_iterator cbegin() const noexcept(true) { return begin(); }
				[[nodiscard]] inline constexpr iterator       end() noexcept(true) { return begin() + size(); }
				[[nodiscard]] inline constexpr const_iterator end() const noexcept(true) { return begin() + size(); }
				[[nodiscard]] inline constexpr const_iterator cend() const noexcept(true) { return end(); }
				[[nodiscard]] inline constexpr reverse_iterator       rbegin() noexcept(true) { return reverse_iterator(end()); }
				[[nodiscard]] inline constexpr const_reverse_iterator rbegin() const noexcept(true) { return rbegin(); }
				[[nodiscard]] inline constexpr const_reverse_iterator crbegin() const noexcept(true) { return rbegin(); }
				[[nodiscard]] inline constexpr reverse_iterator       rend() noexcept(true) { return reverse_iterator(begin()); }
				[[nodiscard]] inline constexpr const_reverse_iterator rend() const noexcept(true) { return rend(); }
				[[nodiscard]] inline constexpr const_reverse_iterator crend() const noexcept(true) { return rend(); }

				[[nodiscard]] reference at(const size_type Pos)
				{
					if (size() <= Pos)
						throw std::out_of_range("bounds check failed in BSTArray::at()");

					//CKPE_ASSERT_MSG(Pos >= 0 && Pos < QSize(), "Exceeded array bounds");
					return (this->_Myfirst()[Pos]);
				}
				[[nodiscard]] const_reference at(const size_type Pos) const
				{
					if (size() <= Pos)
						throw std::out_of_range("bounds check failed in BSTArray::at()");

					//CKPE_ASSERT_MSG(Pos >= 0 && Pos < QSize(), "Exceeded array bounds");
					return (this->_const_Myfirst()[Pos]);
				}				
			public:
				inline bool resize(size_type num) noexcept(true)
				{ 
					if (num == m_AllocSize)
						return false;

					if (!m_Buffer)
					{
						Allocate(num);
						return true;
					}

					if (num < m_AllocSize)
					{
						// Delete the truncated entries
						for (size_type i = num; i < m_AllocSize; i++)
							delete& m_Buffer[i];
					}

					_Ty* newBlock = (_Ty*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Ty) * num);	// Create a new block
					memmove_s(newBlock, sizeof(_Ty) * num, m_Buffer, sizeof(_Ty) * num);				// Move the old memory to the new block
					if (num > m_AllocSize)																// Fill in new remaining entries
					{
						for (size_type i = m_AllocSize; i < num; i++)
							new (&m_Buffer[i]) _Ty;
					}
					NiAPI::NiMemoryManager::Free(nullptr, m_Buffer);									// Free the old block
					m_Buffer = newBlock;																// Assign the new block
					m_AllocSize = num;																	// Capacity is now the number of total entries in the block
					m_Size = std::min(m_AllocSize, m_Size);												// Count stays the same, or is truncated to capacity
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
					if (!m_Buffer || m_Size + 1 > m_AllocSize)
					{
						if (!Grow(GROW))
							throw RuntimeError("out of memory BSTArray::push_back()");
					}

					m_Buffer[m_Size] = entry;
					m_Size++;
				}

				void push_back(value_type&& entry)
				{
					push_back(std::move(entry));
				}

				bool Insert(size_type index, const _Ty& entry) noexcept(true)
				{
					if (!m_Buffer)
						return false;

					size_type lastSize = m_Size;
					if (m_Size + 1 > m_AllocSize)						// Not enough space, grow
					{						
						if (!Grow(GROW))
							return false;
					}

					if (index != lastSize)								// Not inserting onto the end, need to move everything down
					{							
						size_type remaining = m_Size - index;
						memmove_s(&m_Buffer[index + 1], sizeof(_Ty) * remaining, &m_Buffer[index],
							sizeof(_Ty) * remaining);					// Move the rest up
					}

					m_Buffer[index] = entry;
					m_Size++;
					return true;
				};

				bool Remove(size_type index) noexcept(true)
				{
					if (!m_Buffer || index >= m_Size)
						return false;

					// This might not be right for pointer types...
					(&m_Buffer[index])->~_Ty();

					if (index + 1 < m_Size) 
					{
						size_type remaining = m_Size - index;
						memmove_s(&m_Buffer[index], sizeof(_Ty) * remaining, &m_Buffer[index + 1],
							sizeof(_Ty) * remaining);	// Move the rest up
					}
					m_Size--;

					if (m_AllocSize > m_Size + SHRINK)
						Shrink();

					return true;
				}

				bool GetNthItem(size_type index, _Ty& pT) const noexcept(true)
				{
					if (index < m_Size) 
					{
						pT = m_Buffer[index];
						return true;
					}
					return false;
				}

				size_type GetItemIndex(_Ty& pFind) const noexcept(true)
				{
					for (size_type n = 0; n < m_Size; n++) 
					{
						_Ty& pT = m_Buffer[n];
						if (pT == pFind)
							return n;
					}
					return std::numeric_limits<size_type>::max();
				}

				size_type GetStrIndex(const char* pFind) const noexcept(true)
				{
					for (size_type n = 0; n < m_Size; n++) 
					{
						_Ty& pT = m_Buffer[n];

						if (!_stricmp(pT->Get(), pFind))
							return n;
					}
					return std::numeric_limits<size_type>::max();
				}
			};
			static_assert(sizeof(BSTArray<void*>) == 0x18);
		}
	}
}