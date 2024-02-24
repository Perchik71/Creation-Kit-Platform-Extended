//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
* * Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 1)

#include "NiAPI\NiMemoryManager.h" 

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		constexpr auto BSTARRAY_GROW_SIZE = 10;
		constexpr auto BSTARRAY_SHRINK_SIZE = 10;

		template <class _Ty>
		class BSTArray {
			friend class __BSTArrayCheckOffsets;
		public:
			using value_type = _Ty;
			using reference = _Ty&;
			using const_reference = const _Ty&;
			using size_type = uint32_t;
		private:
			_Ty* _Myfirst(VOID) { return (_Ty*)QBuffer(); }
			_Ty* _Mylast(VOID) { return ((_Ty*)QBuffer()) + QSize(); }
			const _Ty* _const_Myfirst(VOID) const { return (_Ty*)QBuffer(); }
			const _Ty* _const_Mylast(VOID) const { return ((_Ty*)QBuffer()) + QSize(); }
		private:
			_Ty* m_Buffer;
			size_type m_AllocSize;
			size_type pad0C;
			size_type m_Size;
			size_type pad14;
		private:
			VOID Deallocate(VOID) {
				NiAPI::NiMemoryManager::Free(NULL, (LPVOID)m_Buffer);
				m_Buffer = NULL;
				m_AllocSize = 0;
				m_Size = 0;
			}

			BOOL Allocate(size_type numEntries) {
				m_Buffer = (_Ty*)NiAPI::NiMemoryManager::Alloc(NULL, sizeof(_Ty) * numEntries);
				if (!m_Buffer) return FALSE;

				for (size_type i = 0; i < numEntries; i++)
					new (&m_Buffer[i]) _Ty;

				m_AllocSize = numEntries;
				m_Size = numEntries;

				return TRUE;
			}
		protected:
			BOOL Shrink(VOID) {
				if (!m_Buffer || m_Size == m_AllocSize)
					return FALSE;

				try {
					size_type newSize = m_Size;
					_Ty* oldArray = m_Buffer;
					_Ty* newArray = (_Ty*)NiAPI::NiMemoryManager::Alloc(NULL, sizeof(_Ty) * newSize);			// Allocate new block
					memmove_s(newArray, sizeof(_Ty) * newSize, m_Buffer, sizeof(_Ty) * newSize);		// Move the old block
					m_Buffer = newArray;
					m_AllocSize = m_Size;
					NiAPI::NiMemoryManager::Free(NULL, (LPVOID)oldArray);			// Free the old block
					return TRUE;
				}
				catch (...) {
					return FALSE;
				}

				return FALSE;
			}

			BOOL Grow(DWORD64 numEntries) {
				if (!m_Buffer) {
					m_Buffer = (_Ty*)NiAPI::NiMemoryManager::Alloc(NULL, sizeof(_Ty) * numEntries);
					m_Size = 0;
					m_AllocSize = numEntries;
					return TRUE;
				}

				try {
					size_type oldSize = m_AllocSize;
					size_type newSize = oldSize + numEntries;
					_Ty* oldArray = m_Buffer;
					_Ty* newArray = (_Ty*)NiAPI::NiMemoryManager::Alloc(NULL, sizeof(_Ty) * newSize);				// Allocate new block
					if (oldArray)
						memmove_s(newArray, sizeof(_Ty) * newSize, m_Buffer, sizeof(_Ty) * m_AllocSize);	// Move the old block
					m_Buffer = newArray;
					m_AllocSize = newSize;

					if (oldArray)
						NiAPI::NiMemoryManager::Free(NULL, (LPVOID)oldArray);		// Free the old block

					for (size_type i = oldSize; i < newSize; i++)					// Allocate the rest of the free blocks
						new (&m_Buffer[i]) _Ty;

					return TRUE;
				}
				catch (...) {
					return FALSE;
				}

				return FALSE;
			}
		public:
			BSTArray(VOID) : m_Buffer(NULL), m_AllocSize(0), m_Size(0) {}
		public:
			inline _Ty* QBuffer(VOID) const { return m_Buffer; }
			inline size_type QAllocSize(VOID) const { return m_AllocSize; }
			inline size_type QSize(VOID) const { return m_Size; }
			inline BOOL QEmpty(VOID) const { return m_Size == 0; }
		public:
			reference at(const size_type Pos) {
				AssertMsg(Pos >= 0 && Pos < QSize(), "Exceeded array bounds");
				return (this->_Myfirst()[Pos]);
			}
			const_reference at(const size_type Pos) const {
				AssertMsg(Pos >= 0 && Pos < QSize(), "Exceeded array bounds");
				return (this->_const_Myfirst()[Pos]);
			}
		public:
			inline reference operator[](const size_type Pos) { return (this->_Myfirst()[Pos]); }
			inline const_reference operator[](const size_type Pos) const { return (this->_const_Myfirst()[Pos]); }
		public:
			inline reference front(VOID) { return (*this->_Myfirst()); }
			inline const_reference const_front(VOID) const { return (*this->_const_Myfirst()); }
			inline reference back(VOID) { return (this->_Mylast()[-1]); }
			inline const_reference const_back(VOID) const { return (this->_const_Mylast()[-1]); }
		public:
			inline VOID Clear(VOID) { Deallocate(); }

			BOOL Resize(size_type numEntries) {
				if (numEntries == m_AllocSize)
					return FALSE;

				if (!m_Buffer) {
					Allocate(numEntries);
					return TRUE;
				}
				if (numEntries < m_AllocSize) {
					// Delete the truncated entries
					for (size_type i = numEntries; i < m_AllocSize; i++)
						delete& m_Buffer[i];
				}

				_Ty* newBlock = (_Ty*)NiAPI::NiMemoryManager::Alloc(NULL, sizeof(_Ty) * numEntries);// Create a new block
				memmove_s(newBlock, sizeof(_Ty) * numEntries, m_Buffer, sizeof(_Ty) * numEntries);	// Move the old memory to the new block
				if (numEntries > m_AllocSize) {														// Fill in new remaining entries
					for (size_type i = m_AllocSize; i < numEntries; i++)
						new (&m_Buffer[i]) _Ty;
				}
				NiAPI::NiMemoryManager::Free(NULL, m_Buffer);										// Free the old block
				m_Buffer = newBlock;																// Assign the new block
				m_AllocSize = numEntries;															// Capacity is now the number of total entries in the block
				m_Size = std::min(m_AllocSize, m_Size);												// Count stays the same, or is truncated to capacity
				return TRUE;
			}

			BOOL Push(const _Ty& entry) {
				if (!m_Buffer || m_Size + 1 > m_AllocSize) {
					if (!Grow(BSTARRAY_GROW_SIZE))
						return FALSE;
				}

				m_Buffer[m_Size] = entry;
				m_Size++;
				return TRUE;
			};

			BOOL Insert(size_type index, const _Ty& entry) {
				if (!m_Buffer)
					return FALSE;

				size_type lastSize = m_Size;
				if (m_Size + 1 > m_AllocSize) {						// Not enough space, grow
					if (!Grow(BSTARRAY_GROW_SIZE))
						return FALSE;
				}

				if (index != lastSize) {							// Not inserting onto the end, need to move everything down
					size_type remaining = m_Size - index;
					memmove_s(&m_Buffer[index + 1], sizeof(_Ty) * remaining, &m_Buffer[index], 
						sizeof(_Ty) * remaining);	// Move the rest up
				}

				m_Buffer[index] = entry;
				m_Size++;
				return TRUE;
			};

			BOOL Remove(size_type index) {
				if (!m_Buffer || index >= m_Size)
					return FALSE;

				// This might not be right for pointer types...
				(&m_Buffer[index])->~_Ty();

				if (index + 1 < m_Size) {
					size_type remaining = m_Size - index;
					memmove_s(&m_Buffer[index], sizeof(_Ty) * remaining, &m_Buffer[index + 1], 
						sizeof(_Ty) * remaining);	// Move the rest up
				}
				m_Size--;

				if (m_AllocSize > m_Size + BSTARRAY_SHRINK_SIZE)
					Shrink();

				return TRUE;
			}

			BOOL GetNthItem(size_type index, _Ty& pT) const {
				if (index < m_Size) {
					pT = m_Buffer[index];
					return TRUE;
				}
				return FALSE;
			}

			DWORD64 GetItemIndex(_Ty& pFind) const {
				for (DWORD64 n = 0; n < m_Size; n++) {
					_Ty& pT = m_Buffer[n];
					if (pT == pFind)
						return n;
				}
				return MAXDWORD;
			}

			DWORD64 GetStrIndex(LPCSTR pFind) const {
				for (DWORD64 n = 0; n < m_Size; n++) {
					_Ty& pT = m_Buffer[n];

					if (!_stricmp(pT->Get(), pFind))
						return n;
				}
				return MAXDWORD;
			}
		};
	}
}

#pragma pack(pop)