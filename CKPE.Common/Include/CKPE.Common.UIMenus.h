// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>

#include <windows.h>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			class CKPE_COMMON_API CUIMenu;
			class CKPE_COMMON_API CUIMenuItem
			{
			private:
				CUIMenu* m_Menu{ nullptr };
				bool m_ByPos{ true };
				std::uint32_t m_Pos{ 0 };
			public:
				void SetText(const std::string &text) noexcept(true);
				[[nodiscard]] std::string GetText() const noexcept(true);
				[[nodiscard]] std::string GetShortCutText() const noexcept(true);
				void SetID(const std::uint32_t menu_id) noexcept(true);
				[[nodiscard]] std::uint32_t GetID() const noexcept(true);
				void SetChecked(const bool value) noexcept(true);
				[[nodiscard]] bool GetChecked() const noexcept(true);
				void SetEnabled(const bool value) noexcept(true);
				[[nodiscard]] bool GetEnabled() const noexcept(true);
				void SetOwnerDraw(const bool value) noexcept(true);
				[[nodiscard]] bool GetOwnerDraw() const noexcept(true);
				void SetTag(const std::ptrdiff_t data_ptr) noexcept(true);
				[[nodiscard]] std::ptrdiff_t GetTag() const noexcept(true);
				[[nodiscard]] bool IsSeparate() const noexcept(true);
				[[nodiscard]] bool IsSubMenu() const noexcept(true);
				[[nodiscard]] inline CUIMenu* Menu() const noexcept(true) { return m_Menu; };
				[[nodiscard]] inline bool ByPosition() const noexcept(true) { return m_ByPos; };
				[[nodiscard]] std::uint32_t GetPosition() const noexcept(true) { return m_Pos; };
			public:
				static void Remove(CUIMenuItem* MenuItem) noexcept(true);
				static void Remove(CUIMenuItem& MenuItem) noexcept(true);
			public:
				constexpr CUIMenuItem() noexcept(true) = default;
				CUIMenuItem(CUIMenu& Menu, const std::uint32_t Position, const bool ByPosition = true) :
					m_Menu(&Menu), m_Pos(Position), m_ByPos(ByPosition)
				{}
				CUIMenuItem(const CUIMenuItem &base) : 
					m_Menu(base.m_Menu), m_Pos(base.m_Pos), m_ByPos(base.m_ByPos)
				{}
			public:
				CKPE_PROPERTY(GetChecked, SetChecked) const bool Checked;
				CKPE_PROPERTY(GetEnabled, SetEnabled) const bool Enabled;
				CKPE_PROPERTY(GetOwnerDraw, SetOwnerDraw) const bool OwnerDraw;
				CKPE_PROPERTY(GetTag, SetTag) const std::ptrdiff_t Tag;
				CKPE_PROPERTY(GetID, SetID) const std::uint32_t ID;
				// no dll interface (what??? okay)
				//CKPE_PROPERTY(GetText, SetText) std::string Text;
				// no dll interface (what??? okay)
				//CKPE_READ_PROPERTY(GetShortCutText) std::string ShortCut;
			};

			class CKPE_COMMON_API CUIMenu
			{
			private:
				HMENU m_Handle{ nullptr };
			public:
				void SetHandle(const HMENU value) noexcept(true);
				[[nodiscard]] HMENU GetHandle() const noexcept(true);
			public:
				[[nodiscard]] std::uint32_t Count() const noexcept(true);
				bool Insert(const std::string &Text, const std::uint32_t Position, const std::uint32_t MenuID, 
					const bool Enabled = true, const bool Checked = false) noexcept(true);
				bool Append(const std::string &Text, const std::uint32_t MenuID, const bool Enabled = true,
					const bool Checked = false) noexcept(true);
				bool Insert(const std::string& Text, const std::uint32_t Position, const std::uint32_t MenuID, 
					const CUIMenu &Menu, const bool Enabled = true) noexcept(true);
				bool Append(const std::string& Text, const std::uint32_t MenuID, const CUIMenu &Menu, 
					const bool Enabled = true) noexcept(true);
				bool InsertSeparator(const std::uint32_t Position) noexcept(true);
				bool AppendSeparator() noexcept(true);
				void Remove(const std::uint32_t MenuID) noexcept(true);
				void RemoveByPos(const std::uint32_t Position) noexcept(true);
				[[nodiscard]] inline bool IsEmpty() const noexcept(true) { return (bool)Count(); }
				[[nodiscard]] CUIMenuItem GetItem(const std::uint32_t MenuID) noexcept(true);
				[[nodiscard]] CUIMenuItem GetItemByPos(const std::uint32_t Position) noexcept(true);
				[[nodiscard]] CUIMenuItem First() noexcept(true);
				[[nodiscard]] CUIMenuItem Last() noexcept(true);
				[[nodiscard]] CUIMenu GetSubMenuItem(const std::uint32_t Position) noexcept(true);
			public:
				static CUIMenu CreateSubMenu() noexcept(true);
			public:
				constexpr CUIMenu() noexcept(true) = default;
				CUIMenu(const HMENU menu) : m_Handle(menu) {}
				CUIMenu(const CUIMenu& base) : m_Handle(base.m_Handle) {}
			public:
				__declspec(property(get = GetHandle, put = SetHandle)) const HMENU Handle;
			};
		}
	}
}