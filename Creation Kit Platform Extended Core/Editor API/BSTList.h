//////////////////////////////////////////
/*
* Fallout 4 Script Extender (F4SE)
* by Ian Patterson, Stephen Abel, and Brendan Borthwick (ianpatt, behippo, and purplelunchbox)
*
* Contact the F4SE Team
*
* Entire Team
* Send email to team [at] f4se [dot] silverlock [dot] org
*
* Ian (ianpatt)
* Send email to ianpatt+f4se [at] gmail [dot] com
*
* Stephen (behippo)
* Send email to gamer [at] silverlock [dot] org
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
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

#include "NiMemoryManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		// 10
		template<class T>
		class BSSimpleList {
			enum {
				eListCount = -3,
				eListEnd = -2,
				eListInvalid = -1,
			};
			struct _Node {
				T* item;	// 00
				_Node* next;	// 04

				T* Item(VOID) const { return item; }
				_Node* Next(VOID) const { return next; }

				T* Remove(_Node* pred) {
					T* pRemoved = item;
					_Node* pNext = next;

					// become the next entry and return my item
					if (pNext) {
						item = pNext->item;
						next = pNext->next;
						NiMemoryManager::Free(NULL, pNext);
					}
					// tail?
					else {
						item = NULL;
						next = NULL;

						// has precedessor, so tail != head
						if (pred) {
							pred->next = NULL;
							NiMemoryManager::Free(NULL, this);
						}
					}
					return pRemoved;
				}
			};

			_Node m_listHead;	// 00
		private:
			_Node* Head(VOID) const { return const_cast<_Node*>(&m_listHead); }
			_Node* Tail(VOID) const {
				_Node* node = const_cast<_Node*>(&m_listHead);

				if (node)
					while (node->next) node = node->next;

				return node;
			}

			template<class Op>
			INT32 FreeNodes(Op& compareOp) {
				INT32 numFreed = 0;

				_Node* pPred = NULL;
				_Node* pCur = Head();

				while (pCur) {
					if (pCur->next) {
						if (compareOp.Accept(pCur->item)) {
							pCur->Remove(pPred);
							numFreed++;
						}
						else {
							pPred = pCur;
							pCur = pCur->next;
						}
					}
					// List Tail?
					else {
						if (compareOp.Accept(pCur->item)) {
							pCur->Remove(pPred);
							numFreed++;
						}
						break;
					}
				}

				return numFreed;
			}

			struct NodePos {
				NodePos(VOID) : node(NULL), index(eListInvalid) {}

				_Node* node;
				INT32 index;
			};


			NodePos GetNthNode(INT32 index) const {
				NodePos pos;
				INT32 n = 0;
				_Node* pCur = Head();

				while (pCur && pCur->Item()) {
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
			static BSSimpleList<T>* Create(VOID) {
				BSSimpleList<T>* p = (BSSimpleList<T>*)NiMemoryManager::Alloc(NULL, sizeof(BSSimpleList<T>));
				Assert(p);

				p->m_listHead.item = NULL;
				p->m_listHead.next = NULL;

				return p;
			}

			VOID Delete(VOID) {
				RemoveAll();
				NiMemoryManager::Free(&m_listHead);
			}

			class Iterator {
				_Node* m_cur;
			public:
				Iterator(VOID) : m_cur(NULL) {}
				Iterator(_Node* node) : m_cur(node) { }
				Iterator operator++(VOID) { if (!End()) m_cur = m_cur->Next(); return *this; }
				BOOL End(VOID) { return m_cur == NULL; }
				const T* operator->(VOID) { return (m_cur) ? m_cur->Item() : NULL; }
				const T* operator*(VOID) { return (m_cur) ? m_cur->Item() : NULL; }
				const Iterator& operator=(const Iterator& rhs) {
					m_cur = rhs.m_cur;
					return *this;
				}
				T* Get(VOID) { return (m_cur) ? m_cur->Item() : NULL; }
			};

			const Iterator Begin(VOID) const { return Iterator(Head()); }

			VOID Insert(T* item) {
				// add new node if we aren't empty
				if (m_listHead.item) {
					// copy head in to new node
					_Node* node = (_Node*)NiMemoryManager::Alloc(NULL, sizeof(_Node));
					Assert(node);

					node->item = m_listHead.item;
					node->next = m_listHead.next;

					m_listHead.next = node;
				}

				m_listHead.item = item;
			}

			VOID Push(T* item) {
				_Node* tail = Tail();

				// add new node if we aren't empty
				if (tail->item) {
					_Node* node = (_Node*)NiMemoryManager::Alloc(NULL, sizeof(_Node));
					Assert(node);

					tail->next = node;

					node->item = item;
					node->next = NULL;
				}
				else
					tail->item = item;
			}

			T* AddFront(VOID) {
				T* item = (T*)NiMemoryManager::Alloc(NULL, sizeof(T));
				if (!item)
					return NULL;

				new (item) T;

				Insert(item);
				return item;
			}

			T* AddBack(VOID) {
				T* item = (T*)NiMemoryManager::Alloc(NULL, sizeof(T));
				if (!item)
					return NULL;

				new (item) T;

				Push(item);
				return item;
			}

			VOID Append(Iterator source) {
				while (!source.End()) {
					Push(source.Get());
					++source;
				}
			}

			INT32 Count(VOID) const {
				NodePos pos = GetNthNode(eListCount);
				return (pos.index > 0) ? pos.index : 0;
			};

			T* GetNthItem(INT32 n) const {
				NodePos pos = GetNthNode(n);
				return (pos.index == n && pos.node) ? pos.node->Item() : NULL;
			}

			T* GetLastItem(VOID) const {
				NodePos pos = GetNthNode(eListEnd);
				return pos.node->Item();
			}

			INT32 AddAt(T* item, INT32 index) {
				if (!m_listHead.item) {
					m_listHead.item = item;
					return 0;
				}

				NodePos pos = GetNthNode(index);
				_Node* pTargetNode = pos.node;
				_Node* newNode = (_Node*)NiMemoryManager::Alloc(NULL, sizeof(newNode));
				if (newNode && pTargetNode) {
					if (index == eListEnd) {
						pTargetNode->next = newNode;
						newNode->item = item;
						newNode->next = NULL;
					}
					else {
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
			VOID Visit(Op& op, _Node* prev = NULL) const {
				const _Node* pCur = (prev) ? prev->next : Head();
				BOOL bContinue = TRUE;
				while (pCur && bContinue) {
					bContinue = op.Accept(pCur->Item());
					if (bContinue) {
						pCur = pCur->next;
					}
				}
			}

			template<class Op>
			T* Find(Op& op) const {
				const _Node* pCur = Head();

				BOOL bFound = FALSE;
				while (pCur && !bFound) {
					if (!pCur->Item())
						pCur = pCur->Next();
					else {
						bFound = op.Accept(pCur->Item());
						if (!bFound)
							pCur = pCur->Next();
					}
				}
				return (bFound && pCur) ? pCur->Item() : NULL;
			}

			template<class Op>
			Iterator Find(Op& op, Iterator prev) const {
				Iterator curIt = (prev.End()) ? Begin() : ++prev;
				BOOL bFound = FALSE;

				while (!curIt.End() && !bFound) {
					const T* pCur = *curIt;
					if (pCur)
						bFound = op.Accept(pCur);
					if (!bFound)
						++curIt;
				}
				return curIt;
			}

			//const _Node* FindString(char* str, Iterator prev) const {
			//	return Find(StringFinder_CI(str), prev);
			//}

			template<class Op>
			INT32 CountIf(Op& op) const {
				INT32 count = 0;
				const _Node* pCur = Head();
				while (pCur) {
					if (pCur->Item() && op.Accept(pCur->Item()))
						count++;
					pCur = pCur->Next();
				}
				return count;
			}

			class AcceptAll {
			public:
				BOOL Accept(T* item) {
					return TRUE;
				}
			};

			VOID RemoveAll(VOID) {
				FreeNodes(AcceptAll());
			}

			T* RemoveNth(INT32 n) {
				T* pRemoved = NULL;
				if (n == 0)
					//pRemoved = m_listHead.RemoveMe();
					pRemoved = m_listHead.Remove(m_listHead);
				else if (n > 0) {
					NodePos nodePos = GetNthNode(n);
					if (nodePos.node && nodePos.index == n) {
						//pRemoved = nodePos.node->RemoveMe();
						pRemoved = nodePos.node->Remove(nodePos.node);
					}
				}
				return pRemoved;
			};

			T* ReplaceNth(INT32 n, T* item) {
				T* pReplaced = NULL;
				NodePos nodePos = GetNthNode(n);
				if (nodePos.node && nodePos.index == n) {
					pReplaced = nodePos.node->item;
					nodePos.node->item = item;
				}
				return pReplaced;
			}

			template<class Op>
			INT32 RemoveIf(Op& op) {
				return FreeNodes(op);
			}

			template<class Op>
			INT32 GetIndexOf(Op& op) {
				INT32 idx = 0;
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

			class AcceptEqual {
			public:
				T* item;

				AcceptEqual(T* a_item) : item(a_item) {}

				BOOL Accept(T* a_item) {
					return *item == *a_item;
				}
			};

			BOOL Contains(T* item) const {
				return Find(AcceptEqual(item)) != NULL;
			}

			VOID Dump(VOID) {
				_MESSAGE("BSSimpleList:");
				_MESSAGE_FMT("> count: %d", Count());

				const _Node* pCur = Head();
				INT32 i = 0;
				while (pCur) {
					_MESSAGE_FMT("* %d :", i);
					//_MESSAGE("\t\titem: %08X", pCur->item);
					if (pCur->item)
						_MESSAGE_FMT("\t\t*item: %d", *pCur->item);
					_MESSAGE_FMT("\t\tnext: %08X", pCur->next);

					i++;
					pCur = pCur->next;
				}
			}
		};

		static_assert(sizeof(BSSimpleList<LPVOID>) == 0x10);
	}
}

#pragma pack(pop)