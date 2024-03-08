// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiNode.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		using namespace NiAPI;

		// it's not in RTTI, I named it the way I see it.

		template<typename _FormT, typename _RenderClass>
		class BGSPickNode
		{
		protected:
			BGSPickNode* _next;
			BGSPickNode* _prev;
			uint32_t _index;
		public:
			inline BGSPickNode* GetNext() const { return _next; }
			inline BGSPickNode* GetPrev() const { return _prev; }
			inline uint32_t GetIndex() const { return _index; }
			inline _FormT* GetRef() const
			{
				auto Renderer = _RenderClass::Singleton.Singleton;
				if (!Renderer) return nullptr;
				return (_FormT*)Renderer->GetRef(_index);
			}

			READ_PROPERTY(GetNext) BGSPickNode* Next;
			READ_PROPERTY(GetPrev) BGSPickNode* Prev;
			READ_PROPERTY(GetIndex) uint32_t Index;
			READ_PROPERTY(GetRef) _FormT* Ref;
		};

		template<typename _FormT, typename _RenderClass>
		class BGSPickIterator 
		{
		private:
			BGSPickNode<_FormT, _RenderClass>* m_cur;
		public:
			BGSPickIterator() : m_cur(nullptr) {}
			BGSPickIterator(BGSPickNode<_FormT, _RenderClass>* node) : m_cur(node) { }

			inline BGSPickIterator operator++(int) { BGSPickIterator temp = *this; if (!End()) m_cur = m_cur->Next; return temp; }
			inline BGSPickIterator operator--(int) { BGSPickIterator temp = *this; if (!End()) m_cur = m_cur->Prev; return temp; }
			inline BGSPickIterator& operator++() { if (!End()) m_cur = m_cur->Next; return *this; }
			inline BGSPickIterator& operator--() { if (!End()) m_cur = m_cur->Prev; return *this; }
			
			inline _FormT* operator->() { return (m_cur) ? m_cur->Ref : nullptr; }
			inline _FormT* operator*() { return (m_cur) ? m_cur->Ref : nullptr; }

			inline bool End() { return m_cur == nullptr; }

			inline const BGSPickIterator& operator=(const BGSPickIterator& rhs)
			{
				m_cur = rhs.m_cur;
				return *this;
			}

			inline _FormT* Get(VOID) { return (m_cur) ? m_cur->Ref : nullptr; }
		};

		template<typename _FormT, typename _RenderClass>
		class BGSPickList : public BGSPickNode<_FormT, _RenderClass>
		{
		public:
			inline BGSPickNode<_FormT, _RenderClass>* GetFirst() const { return this->_next; }
			inline BGSPickNode<_FormT, _RenderClass>* GetEnd() const { return this->_prev; }
			inline BGSPickIterator<_FormT, _RenderClass> GetBeginIterator() { return this->_next; }
			inline BGSPickIterator<_FormT, _RenderClass> GetLastIterator() { return this->_prev; }

			READ_PROPERTY(GetFirst) BGSPickNode<_FormT, _RenderClass>* First;
			READ_PROPERTY(GetEnd) BGSPickNode<_FormT, _RenderClass>* End;
		};

		template<typename _FormT, typename _RenderClass, int OffsetToList = 0x28>
		class BGSPickHandler
		{
		public:
			inline uint32_t GetCount() const { return _Count; }
			inline NiPoint3 GetCenter() const { return _Center; }
			inline BGSPickList<_FormT, _RenderClass>* GetItems() { return _Items; }

			inline bool Has(const _FormT* lpForm) const
			{
				if (!_Items || !_Count) return false;

				auto It = _Items->First;
				for (uint32_t i = 0; i < _Count; i++, It = It->Next)
				{
					if (It->Ref == lpForm)
						return true;
				}

				return false;
			}

			READ_PROPERTY(GetCount) uint32_t Count;
			READ_PROPERTY(GetCenter) NiPoint3 Center;
			READ_PROPERTY(GetItems) BGSPickList<_FormT, _RenderClass>* Items;
		private:
			uint32_t _Count;
			NiPoint3 _Center;
			char pad10[OffsetToList];
			BGSPickList<_FormT, _RenderClass>* _Items;
		};
	}
}