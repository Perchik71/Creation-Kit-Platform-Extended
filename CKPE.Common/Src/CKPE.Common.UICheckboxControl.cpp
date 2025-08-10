// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include "CKPE.Common.UICheckboxControl.h"

#pragma warning(disable: 4312)

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			void CUICheckbox::CreateWnd(const CUIBaseWindow& parent, const CUIBaseControl& control, 
				const std::uint32_t menu_id) noexcept(true)
			{
				CKPE_ASSERT(!m_hWnd);
				CKPE_ASSERT(menu_id);
				CKPE_ASSERT(parent.Is());
				m_Created = false;

				m_hWnd = control.Handle;

				CKPE_ASSERT(m_hWnd);
				m_MenuId = menu_id;
			}

			void CUICheckbox::CreateWnd(const CUIBaseWindow &parent, const std::string &caption, 
				const std::int32_t l, const std::int32_t t, const std::int32_t w, const std::int32_t h, 
				const std::uint32_t menu_id) noexcept(true)
			{
				CKPE_ASSERT(!m_hWnd);
				CKPE_ASSERT(menu_id);
				CKPE_ASSERT(parent.Is());
				m_Created = true;

				m_hWnd = CreateWindowExA(NULL, "button", caption.c_str(), 
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
					l, t, w, h, parent.Handle, 
					(HMENU)menu_id,
					GetModuleHandleA(NULL), NULL);

				CKPE_ASSERT(m_hWnd);

				CheckDlgButton(m_hWnd, menu_id, BST_UNCHECKED);
				m_MenuId = menu_id;
			}

			void CUICheckbox::SetChecked(const bool value) noexcept(true)
			{
				auto State = GetChecked();
				if (State == value)
					return;
				
				CheckDlgButton(Parent(), ID, (!State) ? BST_CHECKED : BST_UNCHECKED);
			}

			bool CUICheckbox::GetChecked() const noexcept(true)
			{
				return IsDlgButtonChecked(Parent(), ID);
			}

			void CUICheckbox::Release() noexcept(true)
			{
				if (m_Created && m_hWnd)
					DestroyWindow(m_hWnd);

				m_hWnd = NULL;
			}
		}
	}
}

