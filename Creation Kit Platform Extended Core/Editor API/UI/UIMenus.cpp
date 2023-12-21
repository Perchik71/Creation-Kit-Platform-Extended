//////////////////////////////////////////
/*
* Copyright (c) 2020-2021 Perchik71 <email:perchik71@outlook.com>
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

#include "UIMenus.h"
//#include "MainWindow.h"

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			BOOL CUIMenuItem::IsSubMenu(VOID) const
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_SUBMENU;
				
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return m_mif.hSubMenu != NULL;
			}

			VOID CUIMenuItem::Click(VOID) const
			{
				//PostMessageA(MainWindow::GetWindow(), WM_COMMAND, GetID(), 0);
			}

			VOID CUIMenuItem::SetText(const std::string& text)
			{
				MENUITEMINFOA m_mif = {0};
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STRING;
				m_mif.fType = MFT_STRING;
				m_mif.cch = (UINT)text.length();
				m_mif.dwTypeData = const_cast<char*>(&text[0]);

				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			std::string CUIMenuItem::GetText(VOID) const
			{
				std::string str;
			
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STRING;
				m_mif.fType = MFT_STRING;
				m_mif.dwTypeData = NULL;

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

			std::string CUIMenuItem::GetShortCutText(VOID) const
			{
				std::string str;

				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STRING;
				m_mif.fType = MFT_STRING;
				m_mif.dwTypeData = NULL;

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

			VOID CUIMenuItem::SetID(const UINT menu_id)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_ID;
				m_mif.wID = menu_id;

				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			UINT CUIMenuItem::GetID(VOID) const
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_ID;
		
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return m_mif.wID;
			}

			VOID CUIMenuItem::SetChecked(const BOOL value)
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

			BOOL CUIMenuItem::GetChecked(VOID) const
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STATE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fState & MFS_CHECKED) == MFS_CHECKED;
			}

			VOID CUIMenuItem::SetEnabled(const BOOL value)
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

			BOOL CUIMenuItem::GetEnabled(VOID) const
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_STATE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fState & MFS_ENABLED) == MFS_ENABLED;
			}

			VOID CUIMenuItem::SetOwnerDraw(const BOOL value)
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

			BOOL CUIMenuItem::GetOwnerDraw(VOID) const
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_FTYPE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fType & MFT_OWNERDRAW) == MFT_OWNERDRAW;
			}

			VOID CUIMenuItem::SetTag(const LONG_PTR data_ptr)
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_DATA;
				m_mif.dwItemData = data_ptr;
		
				SetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);
			}

			LONG_PTR CUIMenuItem::GetTag(VOID) const
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_DATA;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (LONG_PTR)m_mif.dwItemData;
			}

			BOOL CUIMenuItem::IsSeparate(VOID) const
			{
				MENUITEMINFOA m_mif = { 0 };
				m_mif.cbSize = sizeof(MENUITEMINFOA);
				m_mif.fMask = MIIM_FTYPE;
				GetMenuItemInfoA(m_Menu->Handle, m_Pos, m_ByPos, &m_mif);

				return (m_mif.fType & MFT_SEPARATOR) == MFT_SEPARATOR;
			}

			VOID CUIMenuItem::Remove(CUIMenuItem* MenuItem)
			{
				Assert(MenuItem && IsMenu(MenuItem->Menu()->Handle));
				DeleteMenu(MenuItem->Menu()->Handle, MenuItem->ID, MenuItem->ByPosition() ? MF_BYPOSITION : MF_BYCOMMAND);
			}

			VOID CUIMenuItem::Remove(CUIMenuItem& MenuItem)
			{
				Remove(&MenuItem);
			}

			// CUIMenu

			VOID CUIMenu::SetHandle(const HMENU value)
			{
				Assert(IsMenu(value));
				m_Handle = value;
			}

			HMENU CUIMenu::GetHandle(VOID) const
			{
				return m_Handle;
			}

			CUIMenu CUIMenu::CreateSubMenu(VOID)
			{
				return CreateMenu();
			}

			UINT CUIMenu::Count(VOID) const
			{
				return GetMenuItemCount(m_Handle);
			}

			VOID CUIMenu::Remove(const UINT MenuID)
			{
				Assert(IsMenu(m_Handle));
				DeleteMenu(m_Handle, MenuID, MF_BYCOMMAND);
			}

			VOID CUIMenu::RemoveByPos(const UINT Position)
			{
				Assert(IsMenu(m_Handle));
				DeleteMenu(m_Handle, Position, MF_BYPOSITION);
			}

			CUIMenuItem CUIMenu::GetItem(const UINT MenuID)
			{
				return CUIMenuItem(*this, MenuID, FALSE);
			}

			CUIMenuItem CUIMenu::GetItemByPos(const UINT Position)
			{
				return CUIMenuItem(*this, Position, TRUE);
			}

			BOOL CUIMenu::Insert(const std::string& Text, const UINT Position, const UINT MenuID, const BOOL Enabled, const BOOL Checked)
			{
				MENUITEMINFOA minfo = { 0 };
				minfo.cbSize = sizeof(MENUITEMINFOA);
				minfo.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_STATE;
				minfo.wID = (UINT)MenuID;
				minfo.cch = (UINT)Text.length();
				minfo.fType = MFT_STRING;
				minfo.dwTypeData = const_cast<char*>(&Text[0]);
				minfo.fState = (Enabled ? MFS_ENABLED : MFS_DISABLED) | (Checked ? MFS_CHECKED : MFS_UNCHECKED);
				return InsertMenuItemA(m_Handle, Position, TRUE, &minfo);
			}

			BOOL CUIMenu::Append(const std::string& Text, const UINT MenuID, const BOOL Enabled, const BOOL Checked)
			{
				return Insert(Text, -1, MenuID, Enabled, Checked);
			}

			BOOL CUIMenu::Insert(const std::string& Text, const UINT Position, const CUIMenu& Menu, const BOOL Enabled)
			{
				MENUITEMINFOA minfo = { 0 };
				minfo.cbSize = sizeof(MENUITEMINFOA);
				minfo.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_SUBMENU | MIIM_STATE;
				minfo.fType = MFT_STRING;
				minfo.hSubMenu = Menu.Handle;
				minfo.cch = (UINT)Text.length();
				minfo.dwTypeData = const_cast<char*>(&Text[0]);
				minfo.fState = Enabled ? MFS_ENABLED : MFS_DISABLED;
				return InsertMenuItemA(m_Handle, Position, TRUE, &minfo);
			}

			BOOL CUIMenu::Append(const std::string& Text, const CUIMenu& Menu, const BOOL Enabled)
			{
				return Insert(Text, -1, Menu, Enabled);
			}

			BOOL CUIMenu::InsertSeparator(const UINT Position)
			{
				MENUITEMINFOA minfo = { 0 };
				minfo.cbSize = sizeof(MENUITEMINFOA);
				minfo.fMask = MIIM_FTYPE | MIIM_STATE;
				minfo.fType = MFT_SEPARATOR;
				return InsertMenuItemA(m_Handle, Position, TRUE, &minfo);
			}

			BOOL CUIMenu::AppendSeparator(VOID)
			{
				return InsertSeparator(-1);
			}

			CUIMenu CUIMenu::GetSubMenuItem(const UINT Position)
			{
				return GetSubMenu(m_Handle, Position);
			}

			CUIMenuItem CUIMenu::First(VOID)
			{
				return CUIMenuItem(*this, 0, TRUE);
			}

			CUIMenuItem CUIMenu::Last(VOID)
			{
				return CUIMenuItem(*this, Count() - 1, TRUE);
			}
		}
	}
}