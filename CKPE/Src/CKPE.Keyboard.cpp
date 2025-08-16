// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Keyboard.h>

namespace CKPE
{
	bool Keyboard::IsShiftPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_SHIFT) & 0x8000;
	}

	bool Keyboard::IsControlPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_CONTROL) & 0x8000;
	}

	bool Keyboard::IsAltPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_MENU) & 0x8000;
	}

	bool Keyboard::IsLeftShiftPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_LSHIFT) & 0x8000;
	}

	bool Keyboard::IsLeftControlPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_LCONTROL) & 0x8000;
	}

	bool Keyboard::IsLeftAltPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_LMENU) & 0x8000;
	}

	bool Keyboard::IsRightShiftPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_RSHIFT) & 0x8000;
	}

	bool Keyboard::IsRightControlPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_RCONTROL) & 0x8000;
	}

	bool Keyboard::IsRightAltPressed() noexcept(true)
	{
		return GetAsyncKeyState(VK_RMENU) & 0x8000;
	}
}