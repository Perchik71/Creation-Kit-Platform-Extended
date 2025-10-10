// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include "NiAPI/NiNode.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			// It's not in RTTI, I named it the way I see it.

			template<typename _FormT, typename _RenderClass>
			class BGSPickNode
			{
			protected:
				BGSPickNode* _next;
				BGSPickNode* _prev;
				std::uint32_t _index;
			public:
				[[nodiscard]] inline BGSPickNode* GetNext() const noexcept(true) { return _next; }
				[[nodiscard]] inline BGSPickNode* GetPrev() const noexcept(true) { return _prev; }
				[[nodiscard]] inline std::uint32_t GetIndex() const noexcept(true) { return _index; }
				[[nodiscard]] inline _FormT* GetRef() const noexcept(true)
				{
					auto Renderer = _RenderClass::Singleton.Singleton;
					if (!Renderer) return nullptr;
					return (_FormT*)Renderer->GetRef(_index);
				}

				CKPE_READ_PROPERTY(GetNext) BGSPickNode* Next;
				CKPE_READ_PROPERTY(GetPrev) BGSPickNode* Prev;
				CKPE_READ_PROPERTY(GetIndex) std::uint32_t Index;
				CKPE_READ_PROPERTY(GetRef) _FormT* Ref;
			};

			template<typename _FormT, typename _RenderClass>
			class BGSPickIterator
			{
			private:
				BGSPickNode<_FormT, _RenderClass>* m_cur;
			public:
				BGSPickIterator() noexcept(true) : m_cur(nullptr) {}
				BGSPickIterator(BGSPickNode<_FormT, _RenderClass>* node) noexcept(true) : m_cur(node) { }

				inline BGSPickIterator operator++(std::int32_t) noexcept(true) { BGSPickIterator temp = *this; if (!End()) m_cur = m_cur->Next; return temp; }
				inline BGSPickIterator operator--(std::int32_t) noexcept(true) { BGSPickIterator temp = *this; if (!End()) m_cur = m_cur->Prev; return temp; }
				inline BGSPickIterator& operator++() noexcept(true) { if (!End()) m_cur = m_cur->Next; return *this; }
				inline BGSPickIterator& operator--() noexcept(true) { if (!End()) m_cur = m_cur->Prev; return *this; }

				inline _FormT* operator->() noexcept(true) { return (m_cur) ? m_cur->Ref : nullptr; }
				inline _FormT* operator*() noexcept(true) { return (m_cur) ? m_cur->Ref : nullptr; }

				[[nodiscard]] inline bool End() noexcept(true) { return m_cur == nullptr; }

				inline const BGSPickIterator& operator=(const BGSPickIterator& rhs)
				{
					m_cur = rhs.m_cur;
					return *this;
				}

				inline _FormT* Get() noexcept(true) { return (m_cur) ? m_cur->Ref : nullptr; }
			};

			template<typename _FormT, typename _RenderClass>
			class BGSPickList : public BGSPickNode<_FormT, _RenderClass>
			{
			public:
				inline BGSPickNode<_FormT, _RenderClass>* GetFirst() const { return this->_next; }
				inline BGSPickNode<_FormT, _RenderClass>* GetEnd() const { return this->_prev; }
				inline BGSPickIterator<_FormT, _RenderClass> GetBeginIterator() { return this->_next; }
				inline BGSPickIterator<_FormT, _RenderClass> GetLastIterator() { return this->_prev; }

				CKPE_READ_PROPERTY(GetFirst) BGSPickNode<_FormT, _RenderClass>* First;
				CKPE_READ_PROPERTY(GetEnd) BGSPickNode<_FormT, _RenderClass>* End;
			};

			template<typename _FormT, typename _RenderClass, std::int32_t OffsetToList = 0x28>
			class BGSPickHandler
			{
			public:
				[[nodiscard]] inline std::uint32_t GetCount() const noexcept(true) { return _Count; }
				[[nodiscard]] inline NiPoint3 GetCenter() const noexcept(true) { return _Center; }
				[[nodiscard]] inline BGSPickList<_FormT, _RenderClass>* GetItems() noexcept(true) { return _Items; }
				[[nodiscard]] inline bool Has(const _FormT* lpForm) const noexcept(true)
				{
					if (!_Items || !_Count) return false;

					auto It = _Items->First;
					for (std::uint32_t i = 0; i < _Count; i++, It = It->Next)
						if (It->Ref == lpForm)
							return true;

					return false;
				}

				CKPE_READ_PROPERTY(GetCount) uint32_t Count;
				CKPE_READ_PROPERTY(GetCenter) NiPoint3 Center;
				CKPE_READ_PROPERTY(GetItems) BGSPickList<_FormT, _RenderClass>* Items;
			private:
				std::uint32_t _Count;
				NiPoint3 _Center;
				char pad10[OffsetToList];
				BGSPickList<_FormT, _RenderClass>* _Items;
			};
		}
	}
}