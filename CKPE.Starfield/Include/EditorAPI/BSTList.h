// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Asserts.h>
#include <CKPE.Common.Interface.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			// 10
			template<class T>
			class BSSimpleList
			{
				enum : std::int32_t
				{
					eListCount = -3,
					eListEnd = -2,
					eListInvalid = -1,
				};
				struct _Node {
					T* item;		// 00
					_Node* next;	// 08

					[[nodiscard]] inline T* Item() const noexcept(true) { return item; }
					[[nodiscard]] inline _Node* Next() const noexcept(true) { return next; }

					T* Remove(_Node* pred) noexcept(true)
					{
						T* pRemoved = item;
						_Node* pNext = next;

						// become the next entry and return my item
						if (pNext)
						{
							item = pNext->item;
							next = pNext->next;
							NiAPI::NiMemoryManager::Free(nullptr, pNext);
						}
						// tail?
						else
						{
							item = nullptr;
							next = nullptr;

							// has precedessor, so tail != head
							if (pred) {
								pred->next = nullptr;
								NiAPI::NiMemoryManager::Free(nullptr, this);
							}
						}
						return pRemoved;
					}
				};

				_Node m_listHead;	// 00
			private:
				[[nodiscard]] _Node* Head() const noexcept(true) { return const_cast<_Node*>(&m_listHead); }
				[[nodiscard]] _Node* Tail() const noexcept(true)
				{
					_Node* node = const_cast<_Node*>(&m_listHead);

					if (node)
						while (node->next) node = node->next;

					return node;
				}

				template<class Op>
				[[nodiscard]] std::int32_t FreeNodes(Op& compareOp) noexcept(true)
				{
					std::int32_t numFreed = 0;

					_Node* pPred = nullptr;
					_Node* pCur = Head();

					while (pCur) 
					{
						if (pCur->next)
						{
							if (compareOp.Accept(pCur->item)) 
							{
								pCur->Remove(pPred);
								numFreed++;
							}
							else 
							{
								pPred = pCur;
								pCur = pCur->next;
							}
						}
						// List Tail?
						else 
						{
							if (compareOp.Accept(pCur->item)) 
							{
								pCur->Remove(pPred);
								numFreed++;
							}
							break;
						}
					}

					return numFreed;
				}

				struct NodePos 
				{
					NodePos() noexcept(true) : node(nullptr), index(eListInvalid) {}

					_Node* node;
					std::int32_t index;
				};


				[[nodiscard]] NodePos GetNthNode(std::int32_t index) const noexcept(true) 
				{
					NodePos pos{};
					std::int32_t n = 0;
					_Node* pCur = Head();

					while (pCur && pCur->Item()) 
					{
						if (n == index) break;
						if (eListEnd == index && !pCur->Next()) break;
						pCur = pCur->Next();
						++n;
					}

					pos.node = pCur;
					pos.index = n;

					return pos;
				}
			public:
				// Allocate list as a single empty node
				static BSSimpleList<T>* Create() noexcept(true)
				{
					BSSimpleList<T>* p = (BSSimpleList<T>*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(BSSimpleList<T>));
					CKPE_ASSERT(p);

					p->m_listHead.item = nullptr;
					p->m_listHead.next = nullptr;

					return p;
				}

				void Delete() noexcept(true)
				{
					RemoveAll();
					NiAPI::NiMemoryManager::Free(&m_listHead);
				}

				class Iterator 
				{
					_Node* m_cur;
				public:
					Iterator() noexcept(true) : m_cur(nullptr) {}
					Iterator(_Node* node) noexcept(true) : m_cur(node) { }
					Iterator operator++() noexcept(true) { if (!End()) m_cur = m_cur->Next(); return *this; }
					[[nodiscard]] bool End() noexcept(true) { return m_cur == nullptr; }
					const T* operator->() noexcept(true) { return (m_cur) ? m_cur->Item() : nullptr; }
					const T* operator*() noexcept(true) { return (m_cur) ? m_cur->Item() : nullptr; }
					const Iterator& operator=(const Iterator& rhs) noexcept(true)
					{
						m_cur = rhs.m_cur;
						return *this;
					}
					[[nodiscard]] T* Get() noexcept(true) { return (m_cur) ? m_cur->Item() : nullptr; }
				};

				[[nodiscard]] const Iterator Begin() const noexcept(true) { return Iterator(Head()); }

				void Insert(T* item) noexcept(true)
				{
					// add new node if we aren't empty
					if (m_listHead.item)
					{
						// copy head in to new node
						_Node* node = (_Node*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Node));
						Assert(node);

						node->item = m_listHead.item;
						node->next = m_listHead.next;

						m_listHead.next = node;
					}

					m_listHead.item = item;
				}

				void Push(T* item) noexcept(true)
				{
					_Node* tail = Tail();

					// add new node if we aren't empty
					if (tail->item) {
						_Node* node = (_Node*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(_Node));
						CKPE_ASSERT(node);

						tail->next = node;

						node->item = item;
						node->next = nullptr;
					}
					else
						tail->item = item;
				}

				T* AddFront() noexcept(true)
				{
					T* item = (T*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(T));
					if (!item)
						return nullptr;

					new (item) T;

					Insert(item);
					return item;
				}

				T* AddBack() noexcept(true)
				{
					T* item = (T*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(T));
					if (!item)
						return nullptr;

					new (item) T;

					Push(item);
					return item;
				}

				void Append(Iterator source) noexcept(true)
				{
					while (!source.End())
					{
						Push(source.Get());
						++source;
					}
				}

				[[nodiscard]] std::int32_t Count() const noexcept(true)
				{
					NodePos pos = GetNthNode(eListCount);
					return (pos.index > 0) ? pos.index : 0;
				};

				[[nodiscard]] T* GetNthItem(std::int32_t n) const noexcept(true)
				{
					NodePos pos = GetNthNode(n);
					return (pos.index == n && pos.node) ? pos.node->Item() : nullptr;
				}

				[[nodiscard]] T* GetLastItem() const noexcept(true)
				{
					NodePos pos = GetNthNode(eListEnd);
					return pos.node->Item();
				}

				std::int32_t AddAt(T* item, std::int32_t index) noexcept(true)
				{
					if (!m_listHead.item) 
					{
						m_listHead.item = item;
						return 0;
					}

					NodePos pos = GetNthNode(index);
					_Node* pTargetNode = pos.node;
					_Node* newNode = (_Node*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(newNode));
					if (newNode && pTargetNode) 
					{
						if (index == eListEnd) 
						{
							pTargetNode->next = newNode;
							newNode->item = item;
							newNode->next = nullptr;
						}
						else 
						{
							newNode->item = pTargetNode->item;
							newNode->next = pTargetNode->next;
							pTargetNode->item = item;
							pTargetNode->next = newNode;
						}
						return pos.index;
					}
					return eListInvalid;
				}

				template<class Op>
				void Visit(Op& op, _Node* prev = nullptr) const noexcept(true)
				{
					const _Node* pCur = (prev) ? prev->next : Head();
					bool bContinue = true;
					while (pCur && bContinue) 
					{
						bContinue = op.Accept(pCur->Item());
						if (bContinue) 
						{
							pCur = pCur->next;
						}
					}
				}

				template<class Op>
				[[nodiscard]] T* Find(Op& op) const noexcept(true)
				{
					const _Node* pCur = Head();

					bool bFound = false;
					while (pCur && !bFound) {
						if (!pCur->Item())
							pCur = pCur->Next();
						else {
							bFound = op.Accept(pCur->Item());
							if (!bFound)
								pCur = pCur->Next();
						}
					}
					return (bFound && pCur) ? pCur->Item() : nullptr;
				}

				template<class Op>
				[[nodiscard]] Iterator Find(Op& op, Iterator prev) const noexcept(true)
				{
					Iterator curIt = (prev.End()) ? Begin() : ++prev;
					bool bFound = false;

					while (!curIt.End() && !bFound)
					{
						const T* pCur = *curIt;
						if (pCur)
							bFound = op.Accept(pCur);
						if (!bFound)
							++curIt;
					}
					return curIt;
				}

				template<class Op>
				[[nodiscard]] std::int32_t CountIf(Op& op) const noexcept(true)
				{
					std::int32_t count = 0;
					const _Node* pCur = Head();
					while (pCur) 
					{
						if (pCur->Item() && op.Accept(pCur->Item()))
							count++;
						pCur = pCur->Next();
					}
					return count;
				}

				class AcceptAll
				{
				public:
					[[nodiscard]] inline bool Accept(T* item) const noexcept(true)
					{
						return true;
					}
				};

				void RemoveAll() noexcept(true)
				{
					FreeNodes(AcceptAll());
				}

				T* RemoveNth(std::int32_t n) noexcept(true)
				{
					T* pRemoved = nullptr;
					if (n == 0)
						pRemoved = m_listHead.Remove(m_listHead);
					else if (n > 0) {
						NodePos nodePos = GetNthNode(n);
						if (nodePos.node && nodePos.index == n)
							pRemoved = nodePos.node->Remove(nodePos.node);
					}
					return pRemoved;
				};

				T* ReplaceNth(std::int32_t n, T* item) noexcept(true)
				{
					T* pReplaced = nullptr;
					NodePos nodePos = GetNthNode(n);
					if (nodePos.node && nodePos.index == n) 
					{
						pReplaced = nodePos.node->item;
						nodePos.node->item = item;
					}
					return pReplaced;
				}

				template<class Op>
				[[nodiscard]] std::int32_t RemoveIf(Op& op) noexcept(true)
				{
					return FreeNodes(op);
				}

				template<class Op>
				[[nodiscard]] std::int32_t GetIndexOf(Op& op) noexcept(true)
				{
					std::int32_t idx = 0;
					const _Node* pCur = Head();
					while (pCur && pCur->Item() && !op.Accept(pCur->Item())) {
						idx++;
						pCur = pCur->Next();
					}

					if (pCur && pCur->Item())
						return idx;
					else
						return -1;
				}

				class AcceptEqual 
				{
				public:
					T* item;

					AcceptEqual(T* a_item) noexcept(true) : item(a_item) {}

					[[nodiscard]] inline bool Accept(T* a_item) const noexcept(true)
					{
						return *item == *a_item;
					}
				};

				[[nodiscard]] inline bool Contains(T* item) const noexcept(true)
				{
					return Find(AcceptEqual(item)) != nullptr;
				}

				void Dump() noexcept(true)
				{
					_MESSAGE("BSSimpleList:");
					_MESSAGE("> count: %d", Count());

					const _Node* pCur = Head();
					std::int32_t i = 0;
					while (pCur) 
					{
						_MESSAGE("* %d :", i);

						if (pCur->item)
							_MESSAGE("\t\t*item: %d", *pCur->item);
						_MESSAGE("\t\tnext: %08X", pCur->next);

						i++;
						pCur = pCur->next;
					}
				}
			};

			static_assert(sizeof(BSSimpleList<void*>) == 0x10);
		}
	}
}