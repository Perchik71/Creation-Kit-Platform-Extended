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

#pragma once

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			class CUIMenu;
			class CUIMenuItem
			{
			private:
				CUIMenu* m_Menu;
				BOOL m_ByPos;
				UINT m_Pos;
			public:
				VOID SetText(const std::string &text);
				std::string GetText(VOID) const;
				std::string GetShortCutText(VOID) const;
				VOID SetID(const UINT menu_id);
				UINT GetID(VOID) const;
				VOID SetChecked(const BOOL value);
				BOOL GetChecked(VOID) const;
				VOID SetEnabled(const BOOL value);
				BOOL GetEnabled(VOID) const;
				VOID SetOwnerDraw(const BOOL value);
				BOOL GetOwnerDraw(VOID) const;
				VOID SetTag(const LONG_PTR data_ptr);
				LONG_PTR GetTag(VOID) const;
				BOOL IsSeparate(VOID) const;
				BOOL IsSubMenu(VOID) const;
				inline CUIMenu* Menu(VOID) const { return m_Menu; };
				inline BOOL ByPosition(VOID) const { return m_ByPos; };
				UINT GetPosition(VOID) const { return m_Pos; };
				VOID Click(VOID) const;
			public:
				static VOID Remove(CUIMenuItem* MenuItem);
				static VOID Remove(CUIMenuItem& MenuItem);
			public:
				CUIMenuItem(VOID) :
					m_Menu(NULL), m_Pos(0), m_ByPos(TRUE)
				{}
				CUIMenuItem(CUIMenu& Menu, const UINT Position, const BOOL ByPosition = TRUE) :
					m_Menu(&Menu), m_Pos(Position), m_ByPos(ByPosition)
				{}
				CUIMenuItem(const CUIMenuItem &base) : 
					m_Menu(base.m_Menu), m_Pos(base.m_Pos), m_ByPos(base.m_ByPos)
				{}
			public:
				__declspec(property(get = GetChecked, put = SetChecked)) const BOOL Checked;
				__declspec(property(get = GetEnabled, put = SetEnabled)) const BOOL Enabled;
				__declspec(property(get = GetOwnerDraw, put = SetOwnerDraw)) const BOOL OwnerDraw;
				__declspec(property(get = GetTag, put = SetTag)) const LONG_PTR Tag;
				__declspec(property(get = GetID, put = SetID)) const UINT ID;
				__declspec(property(get = GetText, put = SetText)) std::string Text;
				__declspec(property(get = GetShortCutText)) std::string ShortCut;
			};

			class CUIMenu
			{
			private:
				HMENU m_Handle;
			public:
				VOID SetHandle(const HMENU value);
				HMENU GetHandle(VOID) const;
			public:
				UINT Count(VOID) const;
				BOOL Insert(const std::string &Text, const UINT Position, const UINT MenuID, const BOOL Enabled = TRUE, const BOOL Checked = FALSE);
				BOOL Append(const std::string &Text, const UINT MenuID, const BOOL Enabled = TRUE, const BOOL Checked = FALSE);
				BOOL Insert(const std::string& Text, const UINT Position, const CUIMenu &Menu, const BOOL Enabled = TRUE);
				BOOL Append(const std::string& Text, const CUIMenu &Menu, const BOOL Enabled = TRUE);
				BOOL InsertSeparator(const UINT Position);
				BOOL AppendSeparator(VOID);
				VOID Remove(const UINT MenuID);
				VOID RemoveByPos(const UINT Position);
				inline BOOL IsEmpty(void) const { return (BOOL)Count(); }
				CUIMenuItem GetItem(const UINT MenuID);
				CUIMenuItem GetItemByPos(const UINT Position);
				CUIMenuItem First(VOID);
				CUIMenuItem Last(VOID);
				CUIMenu GetSubMenuItem(const UINT Position);
			public:
				static CUIMenu CreateSubMenu(VOID);
			public:
				CUIMenu(VOID) : m_Handle(NULL) {}
				CUIMenu(const HMENU menu) : m_Handle(menu) {}
				CUIMenu(const CUIMenu& base) : m_Handle(base.m_Handle) {}
			public:
				__declspec(property(get = GetHandle, put = SetHandle)) const HMENU Handle;
			};
		}
	}
}