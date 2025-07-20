// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include <limits.h>
#include <memory.h>
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
				using reference = _Ty&;
				using const_reference = const _Ty&;
				using size_type = std::uint32_t;
			private:
				[[nodiscard]] _Ty* _Myfirst() { return (_Ty*)QBuffer(); }
				[[nodiscard]] _Ty* _Mylast() { return ((_Ty*)QBuffer()) + QSize(); }
				[[nodiscard]] const _Ty* _const_Myfirst() const { return (_Ty*)QBuffer(); }
				[[nodiscard]] const _Ty* _const_Mylast() const { return ((_Ty*)QBuffer()) + QSize(); }
			private:
				_Ty* m_Buffer;
				size_type m_AllocSize;
				size_type pad0C;
				size_type m_Size;
				size_type pad14;
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
				BSTArray() noexcept(true) : m_Buffer(nullptr), m_AllocSize(0), m_Size(0) {}
			public:
				[[nodiscard]] inline _Ty* QBuffer() const noexcept(true) { return m_Buffer; }
				[[nodiscard]] inline size_type QAllocSize() const noexcept(true) { return m_AllocSize; }
				[[nodiscard]] inline size_type QSize() const noexcept(true) { return m_Size; }
				[[nodiscard]] inline bool QEmpty() const noexcept(true) { return m_Size == 0; }
			public:
				[[nodiscard]] reference at(const size_type Pos) noexcept(true)
				{
					AssertMsg(Pos >= 0 && Pos < QSize(), "Exceeded array bounds");
					return (this->_Myfirst()[Pos]);
				}
				[[nodiscard]] const_reference at(const size_type Pos) const noexcept(true)
				{
					AssertMsg(Pos >= 0 && Pos < QSize(), "Exceeded array bounds");
					return (this->_const_Myfirst()[Pos]);
				}
			public:
				inline reference operator[](const size_type Pos) noexcept(true) { return (this->_Myfirst()[Pos]); }
				inline const_reference operator[](const size_type Pos) const noexcept(true) { return (this->_const_Myfirst()[Pos]); }
			public:
				inline reference front() noexcept(true) { return (*this->_Myfirst()); }
				inline const_reference const_front() const noexcept(true) { return (*this->_const_Myfirst()); }
				inline reference back() noexcept(true) { return (this->_Mylast()[-1]); }
				inline const_reference const_back() const noexcept(true) { return (this->_const_Mylast()[-1]); }
			public:
				inline void Clear() noexcept(true) { Deallocate(); }

				bool Resize(size_type numEntries) noexcept(true)
				{
					if (numEntries == m_AllocSize)
						return false;

					if (!m_Buffer) 
					{
						Allocate(numEntries);
						return true;
					}

					if (numEntries < m_AllocSize) 
					{
						// Delete the truncated entries
						for (size_type i = numEntries; i < m_AllocSize; i++)
							delete& m_Buffer[i];
					}

					_Ty* newBlock = (_Ty*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Ty) * numEntries);	// Create a new block
					memmove_s(newBlock, sizeof(_Ty) * numEntries, m_Buffer, sizeof(_Ty) * numEntries);		// Move the old memory to the new block
					if (numEntries > m_AllocSize)															// Fill in new remaining entries
					{															
						for (size_type i = m_AllocSize; i < numEntries; i++)
							new (&m_Buffer[i]) _Ty;
					}
					NiAPI::NiMemoryManager::Free(nullptr, m_Buffer);										// Free the old block
					m_Buffer = newBlock;																	// Assign the new block
					m_AllocSize = numEntries;																// Capacity is now the number of total entries in the block
					m_Size = std::min(m_AllocSize, m_Size);													// Count stays the same, or is truncated to capacity
					return true;
				}

				bool Push(const _Ty& entry) noexcept(true)
				{
					if (!m_Buffer || m_Size + 1 > m_AllocSize) 
					{
						if (!Grow(GROW))
							return false;
					}

					m_Buffer[m_Size] = entry;
					m_Size++;
					return true;
				};

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
		}
	}
}

#pragma pack(pop)