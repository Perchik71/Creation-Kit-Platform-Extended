// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "CKPE.Common.UIBaseWindow.h"

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			class CKPE_COMMON_API CUICheckbox : public CUIBaseControl
			{
			private:
				bool m_Created;
				bool m_Checked;
				std::uint32_t m_MenuId;
			public:
				CUICheckbox(void) noexcept(true) : CUIBaseControl(), m_Created(false), m_Checked(false), m_MenuId(0) {}
				CUICheckbox(const HWND hWnd) noexcept(true) : CUIBaseControl(hWnd), m_Created(false), m_Checked(false), m_MenuId(0) {}
				CUICheckbox(const CUICheckbox& base) noexcept(true) : CUIBaseControl(base), m_Created(false), m_Checked(false), m_MenuId(0) {}

				void CreateWnd(const CUIBaseWindow &parent, const CUIBaseControl& control, const std::uint32_t menu_id) noexcept(true);
				void CreateWnd(const CUIBaseWindow &parent, const std::string &caption, const std::int32_t l, 
					const std::int32_t t, const std::int32_t w, const std::int32_t h, const std::uint32_t menu_id) noexcept(true);
				void SetChecked(const bool value) noexcept(true);
				bool GetChecked() const noexcept(true);
				void Release() noexcept(true);
			public:
				CKPE_PROPERTY(GetChecked, SetChecked) bool Checked;
			};
		}
	}
}