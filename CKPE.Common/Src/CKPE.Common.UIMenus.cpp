// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIMenus.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			bool CUIMenuItem::IsSubMenu() const noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_SUBMENU;
				
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return m_mif.hSubMenu != nullptr;
			}

			void CUIMenuItem::SetText(const std::string& text) noexcept(true)
			{
				MENUITEMINFOA m_mif = {0};
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STRING;
				m_mif.fType = MFT_STRING;
				m_mif.cch = (UINT)text.length();
				m_mif.dwTypeData = const_cast<char*>(&text[0]);

				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			std::string CUIMenuItem::GetText() const noexcept(true)
			{
				std::string str;
			
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STRING;
				m_mif.fType = MFT_STRING;
				m_mif.dwTypeData = nullptr;

				if (GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif))
				{
					m_mif.cch++;
					str.resize(m_mif.cch);
					m_mif.dwTypeData = &str[0];
					GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

					// skip shortcut
					auto found = str.find_first_of('\t');
					if (found != str.npos)
						str.erase(found, str.length() - found);
				}

				return str;
			}

			std::string CUIMenuItem::GetShortCutText() const noexcept(true)
			{
				std::string str;

				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STRING;
				m_mif.fType = MFT_STRING;
				m_mif.dwTypeData = nullptr;

				if (GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif))
				{
					m_mif.cch++;
					str.resize(m_mif.cch);
					m_mif.dwTypeData = &str[0];
					GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

					// skip text
					auto found = str.find_first_of('\t');
					if (found != str.npos)
						str.assign(str.c_str(), found + 1, str.length() - found);
					else
						return "";
				}

				return str;
			}

			void CUIMenuItem::SetID(const std::uint32_t menu_id) noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_ID;
				m_mif.wID = menu_id;

				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			std::uint32_t CUIMenuItem::GetID() const noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_ID;
		
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return m_mif.wID;
			}

			void CUIMenuItem::SetChecked(const bool value) noexcept(true)
			{
				if (GetChecked() == value)
					return;

				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STATE;

				if (value)
					m_mif.fState = (GetEnabled() ? MFS_ENABLED : MFS_DISABLED) | MFS_CHECKED;
				else
					m_mif.fState = (GetEnabled() ? MFS_ENABLED : MFS_DISABLED) | MFS_UNCHECKED;

				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			bool CUIMenuItem::GetChecked() const noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STATE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fState & MFS_CHECKED) == MFS_CHECKED;
			}

			void CUIMenuItem::SetEnabled(const bool value) noexcept(true)
			{
				// fix bug MFS_ENABLED == 0 :(
				//if (GetEnabled() == value)
				//	return;

				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STATE;

				if (value)
					m_mif.fState = (GetChecked() ? MFS_CHECKED : MFS_UNCHECKED) | MFS_ENABLED;
				else
					m_mif.fState = (GetChecked() ? MFS_CHECKED : MFS_UNCHECKED) | MFS_DISABLED;

				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			bool CUIMenuItem::GetEnabled() const noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STATE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fState & MFS_ENABLED) == MFS_ENABLED;
			}

			void CUIMenuItem::SetOwnerDraw(const bool value) noexcept(true)
			{
				if (GetOwnerDraw() == value)
					return;

				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_FTYPE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				if (value)
					m_mif.fType |= MFT_OWNERDRAW;
				else
					m_mif.fType &= ~MFT_OWNERDRAW;

				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			bool CUIMenuItem::GetOwnerDraw() const noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_FTYPE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fType & MFT_OWNERDRAW) == MFT_OWNERDRAW;
			}

			void CUIMenuItem::SetTag(const std::ptrdiff_t data_ptr) noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_DATA;
				m_mif.dwItemData = data_ptr;
		
				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			std::ptrdiff_t CUIMenuItem::GetTag() const noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_DATA;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (LONG_PTR)m_mif.dwItemData;
			}

			bool CUIMenuItem::IsSeparate() const noexcept(true)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_FTYPE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fType & MFT_SEPARATOR) == MFT_SEPARATOR;
			}

			void CUIMenuItem::Remove(CUIMenuItem* MenuItem) noexcept(true)
			{
				DeleteMenu(MenuItem->Menu()->Handle, MenuItem->ID, MenuItem->ByPosition() ? MF_BYPOSITION : MF_BYCOMMAND);
			}

			void CUIMenuItem::Remove(CUIMenuItem& MenuItem) noexcept(true)
			{
				Remove(&MenuItem);
			}

			// CUIMenu

			void CUIMenu::SetHandle(const HMENU value) noexcept(true)
			{
				m_Handle = value;
			}

			HMENU CUIMenu::GetHandle() const noexcept(true)
			{
				return m_Handle;
			}

			CUIMenu CUIMenu::CreateSubMenu() noexcept(true)
			{
				return CreateMenu();
			}

			std::uint32_t CUIMenu::Count() const noexcept(true)
			{
				return GetMenuItemCount(m_Handle);
			}

			void CUIMenu::Remove(const std::uint32_t MenuID) noexcept(true)
			{
				DeleteMenu(m_Handle, MenuID, MF_BYCOMMAND);
			}

			void CUIMenu::RemoveByPos(const std::uint32_t Position) noexcept(true)
			{
				DeleteMenu(m_Handle, Position, MF_BYPOSITION);
			}

			CUIMenuItem CUIMenu::GetItem(const std::uint32_t MenuID) noexcept(true)
			{
				return CUIMenuItem(*this, MenuID, false);
			}

			CUIMenuItem CUIMenu::GetItemByPos(const std::uint32_t Position) noexcept(true)
			{
				return CUIMenuItem(*this, Position, true);
			}

			bool CUIMenu::Insert(const std::string& Text, const std::uint32_t Position, 
				const std::uint32_t MenuID, const bool Enabled, const bool Checked) noexcept(true)
			{
				MENUITEMINFOA minfo = { 0 };
				minfo.cbSize = sizeof(MENUITEMINFOA);
				minfo.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_STATE;
				minfo.wID = (uint32_t)MenuID;
				minfo.cch = static_cast<uint32_t>(Text.length());
				minfo.fType = MFT_STRING;
				minfo.dwTypeData = const_cast<LPSTR>(&Text[0]);
				minfo.fState = (Enabled ? MFS_ENABLED : MFS_DISABLED) | (Checked ? MFS_CHECKED : MFS_UNCHECKED);
				return InsertMenuItemA(m_Handle, Position, true, &minfo);
			}

			bool CUIMenu::Append(const std::string& Text, const std::uint32_t MenuID, 
				const bool Enabled, const bool Checked) noexcept(true)
			{
				return Insert(Text, -1, MenuID, Enabled, Checked);
			}

			bool CUIMenu::Insert(const std::string& Text, const std::uint32_t Position, const std::uint32_t MenuID, 
				const CUIMenu& Menu, const bool Enabled) noexcept(true)
			{
				MENUITEMINFOA minfo = { 0 };
				minfo.cbSize = sizeof(MENUITEMINFOA);
				minfo.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU | MIIM_STATE;
				minfo.hSubMenu = Menu.Handle;
				minfo.wID = (uint32_t)MenuID;
				minfo.cch = static_cast<uint32_t>(Text.length());
				minfo.dwTypeData = const_cast<LPSTR>(&Text[0]);
				minfo.fState = Enabled ? MFS_ENABLED : MFS_DISABLED;
				return InsertMenuItemA(m_Handle, Position, true, &minfo);
			}

			bool CUIMenu::Append(const std::string& Text, const std::uint32_t MenuID, const CUIMenu& Menu, 
				const bool Enabled) noexcept(true)
			{
				return Insert(Text, -1, MenuID, Menu, Enabled);
			}

			bool CUIMenu::InsertSeparator(const std::uint32_t Position) noexcept(true)
			{
				MENUITEMINFOA minfo = { 0 };
				minfo.cbSize = sizeof(MENUITEMINFOA);
				minfo.fMask = MIIM_FTYPE | MIIM_STATE;
				minfo.fType = MFT_SEPARATOR;
				return InsertMenuItemA(m_Handle, Position, true, &minfo);
			}

			bool CUIMenu::AppendSeparator() noexcept(true)
			{
				return InsertSeparator(-1);
			}

			CUIMenu CUIMenu::GetSubMenuItem(const std::uint32_t Position) noexcept(true)
			{
				return GetSubMenu(m_Handle, Position);
			}

			CUIMenuItem CUIMenu::First() noexcept(true)
			{
				return CUIMenuItem(*this, 0, true);
			}

			CUIMenuItem CUIMenu::Last() noexcept(true)
			{
				return CUIMenuItem(*this, Count() - 1, true);
			}
		}
	}
}