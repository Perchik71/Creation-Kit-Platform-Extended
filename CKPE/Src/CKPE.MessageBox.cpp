// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <stdarg.h>
#include <CKPE.MessageBox.h>

namespace CKPE
{
	std::uint32_t MessageBox::NativeConvButtons(Buttons buttons) noexcept(true)
	{
		switch (buttons)
		{
		case CKPE::MessageBox::mbOkCancel:
			return MB_OKCANCEL;
		case CKPE::MessageBox::mbYesNo:
			return MB_YESNO;
		case CKPE::MessageBox::mbYesNoCancel:
			return MB_YESNOCANCEL;
		default:
			return 0;
		}
	}

	std::uint32_t MessageBox::NativeConvIcons(Icon icon) noexcept(true)
	{
		switch (icon)
		{
		case CKPE::MessageBox::miInfo:
			return MB_ICONINFORMATION;
		case CKPE::MessageBox::miQuestion:
			return MB_ICONQUESTION;
		case CKPE::MessageBox::miWarning:
			return MB_ICONWARNING;
		case CKPE::MessageBox::miError:
			return MB_ICONERROR;
		default:
			return 0;
		}
	}

	MessageBox::Result MessageBox::NativeConvResult(std::uint32_t result) noexcept(true)
	{
		switch (result)
		{
		case IDOK:
			return CKPE::MessageBox::mrOk;
		case IDCANCEL:
			return CKPE::MessageBox::mrCancel;
		case IDYES:
			return CKPE::MessageBox::mrYes;
		case IDNO:
			return CKPE::MessageBox::mrNo;
		default:
			return CKPE::MessageBox::mrUnknown;
		}
	}

	MessageBox::Result MessageBox::OpenVa(const std::string& title, Buttons buttons, Icon icon,
		const std::string& formatted_message, void* args) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		auto len = _vscprintf(formatted_message.c_str(), (va_list)args);
		if (len > 0)
		{
			std::string s;
			s.resize((std::size_t)len);
			if (!s.empty())
			{
				vsprintf(s.data(), formatted_message.c_str(), (va_list)args);
				r = Open(s, title, buttons, icon);
			}
		}
		return r;
	}

	MessageBox::Result MessageBox::OpenVa(const std::wstring& title, Buttons buttons, Icon icon,
		const std::wstring& formatted_message, void* args) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		auto len = _vscwprintf(formatted_message.c_str(), (va_list)args);
		if (len > 0)
		{
			std::wstring s;
			s.resize((std::size_t)len);
			if (!s.empty())
			{
				_vswprintf(s.data(), formatted_message.c_str(), (va_list)args);
				r = Open(s, title, buttons, icon);
			}
		}
		return r;
	}

	MessageBox::Result MessageBox::Open(const std::string& message, const std::string& title, 
		Buttons buttons, Icon icon) noexcept(true)
	{
		return NativeConvResult(MessageBoxA(0, 
			message.c_str(), 
			title.c_str(), 
			NativeConvButtons(buttons) | NativeConvIcons(icon)));
	}

	MessageBox::Result MessageBox::Open(const std::wstring& message, const std::wstring& title, 
		Buttons buttons, Icon icon) noexcept(true)
	{
		return NativeConvResult(MessageBoxW(0,
			message.c_str(),
			title.c_str(),
			NativeConvButtons(buttons) | NativeConvIcons(icon)));
	}

	MessageBox::Result MessageBox::OpenInfo(const std::string& message, Buttons buttons) noexcept(true)
	{
		return Open(message, "Info", buttons, Icon::miInfo);
	}

	MessageBox::Result MessageBox::OpenInfo(const std::wstring& message, Buttons buttons) noexcept(true)
	{
		return Open(message, L"Info", buttons, Icon::miInfo);
	}

	MessageBox::Result MessageBox::OpenQuestion(const std::string& message, Buttons buttons) noexcept(true)
	{
		return Open(message, "Question", buttons, Icon::miQuestion);
	}

	MessageBox::Result MessageBox::OpenQuestion(const std::wstring& message, Buttons buttons) noexcept(true)
	{
		return Open(message, L"Question", buttons, Icon::miQuestion);
	}

	MessageBox::Result MessageBox::OpenWarning(const std::string& message, Buttons buttons) noexcept(true)
	{
		return Open(message, "Warning", buttons, Icon::miWarning);
	}

	MessageBox::Result MessageBox::OpenWarning(const std::wstring& message, Buttons buttons) noexcept(true)
	{
		return Open(message, L"Warning", buttons, Icon::miWarning);
	}

	MessageBox::Result MessageBox::OpenError(const std::string& message, Buttons buttons) noexcept(true)
	{
		return Open(message, "Error", buttons, Icon::miError);
	}

	MessageBox::Result MessageBox::OpenError(const std::wstring& message, Buttons buttons) noexcept(true)
	{
		return Open(message, L"Error", buttons, Icon::miError);
	}

	MessageBox::Result MessageBox::Open(const std::string& title, Buttons buttons, Icon icon, 
		const std::string& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa(title, buttons, icon, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::Open(const std::wstring& title, Buttons buttons, Icon icon, 
		const std::wstring& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa(title, buttons, icon, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenInfo(Buttons buttons, const std::string& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa("Info", buttons, Icon::miInfo, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenInfo(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa(L"Info", buttons, Icon::miInfo, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenQuestion(Buttons buttons, const std::string& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa("Question", buttons, Icon::miQuestion, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenQuestion(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa(L"Question", buttons, Icon::miQuestion, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenWarning(Buttons buttons, const std::string& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa("Warning", buttons, Icon::miWarning, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenWarning(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa(L"Warning", buttons, Icon::miWarning, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenError(Buttons buttons, const std::string& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa("Error", buttons, Icon::miError, formatted_message, ap);
		va_end(ap);
		return r;
	}

	MessageBox::Result MessageBox::OpenError(Buttons buttons, const std::wstring& formatted_message, ...) noexcept(true)
	{
		MessageBox::Result r{ mrUnknown };
		va_list ap;
		va_start(ap, &formatted_message);
		r = OpenVa(L"Error", buttons, Icon::miError, formatted_message, ap);
		va_end(ap);
		return r;
	}
}
