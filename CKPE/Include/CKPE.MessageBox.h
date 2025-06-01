// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once
#undef MessageBox

#include <string>
#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API MessageBox
	{
	public:
		enum Buttons : std::uint32_t
		{
			mbOk = 0,
			mbOkCancel,
			mbYesNo,
			mbYesNoCancel,
		};

		enum Icon : std::uint32_t
		{
			miNone = 0,
			miInfo,
			miQuestion,
			miWarning,
			miError,
		};

		enum Result : std::uint32_t
		{
			mrUnknown = (std::uint32_t)-1,
			mrOk = 0,
			mrCancel,
			mrYes,
			mrNo,
		};

		static Result Open(const std::string& message, const std::string& title = "",
			Buttons buttons = mbOk, Icon icon = miNone) noexcept(true);
		static Result Open(const std::wstring& message, const std::wstring& title = L"",
			Buttons buttons = mbOk, Icon icon = miNone) noexcept(true);
		
		static Result OpenInfo(const std::string& message, Buttons buttons = mbOk) noexcept(true);
		static Result OpenInfo(const std::wstring& message, Buttons buttons = mbOk) noexcept(true);
		static Result OpenQuestion(const std::string& message, Buttons buttons = mbYesNo) noexcept(true);
		static Result OpenQuestion(const std::wstring& message, Buttons buttons = mbYesNo) noexcept(true);
		static Result OpenWarning(const std::string& message, Buttons buttons = mbOk) noexcept(true);
		static Result OpenWarning(const std::wstring& message, Buttons buttons = mbOk) noexcept(true);
		static Result OpenError(const std::string& message, Buttons buttons = mbOk) noexcept(true);
		static Result OpenError(const std::wstring& message, Buttons buttons = mbOk) noexcept(true);

		static Result Open(const std::string& title, Buttons buttons, Icon icon,
			const std::string& formatted_message, ...) noexcept(true);
		static Result Open(const std::wstring& title, Buttons buttons, Icon icon,
			const std::wstring& formatted_message, ...) noexcept(true);

		static Result OpenInfo(Buttons buttons, const std::string& formatted_message, ...) noexcept(true);
		static Result OpenInfo(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true);
		static Result OpenQuestion(Buttons buttons, const std::string& formatted_message, ...) noexcept(true);
		static Result OpenQuestion(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true);
		static Result OpenWarning(Buttons buttons, const std::string& formatted_message, ...) noexcept(true);
		static Result OpenWarning(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true);
		static Result OpenError(Buttons buttons, const std::string& formatted_message, ...) noexcept(true);
		static Result OpenError(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true);
	private:
		constexpr MessageBox() noexcept(true) = default;
		MessageBox(const MessageBox&) = delete;
		MessageBox& operator=(const MessageBox&) = delete;

		static std::uint32_t NativeConvButtons(Buttons buttons) noexcept(true);
		static std::uint32_t NativeConvIcons(Icon icon) noexcept(true);
		static Result NativeConvResult(std::uint32_t result) noexcept(true);

		static Result OpenVa(const std::string& title, Buttons buttons, Icon icon,
			const std::string& formatted_message, void* args) noexcept(true);
		static Result OpenVa(const std::wstring& title, Buttons buttons, Icon icon,
			const std::wstring& formatted_message, void* args) noexcept(true);
	};
}