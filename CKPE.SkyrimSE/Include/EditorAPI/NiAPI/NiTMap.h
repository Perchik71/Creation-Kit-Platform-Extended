// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiMemoryManager.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				template<typename KeyT, typename ValueT>
				class NiTMap
				{
				public:
					struct _Pair
					{
						KeyT key;
						ValueT value;
					};

					struct _Node : _Pair
					{
						_Node* next;

						void Remove(_Node* pred) noexcept(true)
						{
							_Node* pNext = next;

							// become the next entry and return my item
							if (pNext)
							{
								((_Pair*)this)->key = pNext->key;
								((_Pair*)this)->value = pNext->value;
								next = pNext->next;
								NiAPI::NiMemoryManager::Free(nullptr, pNext);
							}
							// tail?
							else
							{
								((_Pair*)this)->key = nullptr;
								((_Pair*)this)->value = nullptr;
								next = nullptr;

								// has precedessor, so tail != head
								if (pred)
								{
									pred->next = nullptr;
									NiAPI::NiMemoryManager::Free(nullptr, this);
								}
							}
						}
					};

					inline _Node* Head(VOID) const noexcept(true) { return const_cast<_Node*>(&m_listHead); }
					inline _Node* Tail(VOID) const noexcept(true)
					{
						_Node* node = const_cast<_Node*>(&m_listHead);

						if (node)
							while (node->next) node = node->next;

						return node;
					}

					class Iterator {
						_Node* m_cur;
					public:
						Iterator() noexcept(true) : m_cur(nullptr) {}
						Iterator(_Node* node) noexcept(true) : m_cur(node) { }
						inline Iterator operator++() noexcept(true) { if (!End()) m_cur = m_cur->next; return *this; }
						inline bool End() noexcept(true) { return m_cur == nullptr; }
						inline const _Pair* operator->() noexcept(true) { return Get(); }
						inline const _Pair* operator*() noexcept(true) { return Get(); }
						inline const Iterator& operator=(const Iterator& rhs) noexcept(true) {
							m_cur = rhs.m_cur;
							return *this;
						}
						inline _Pair* Get() noexcept(true) { return (m_cur) ? (_Pair*)m_cur : nullptr; }
					};

					inline const Iterator Begin() const noexcept(true) { return Iterator(Head()); }
				private:
					_Node m_listHead;	// 00
				};
			}
		}
	}
}