// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>

// Microsoft: It's probably cool when you need to make several includes instead of one 
// Perchik71: NO

#include <shlobj_core.h>
#include <gdiplusenums.h>
#include <gdiplustypes.h>
#include <gdiplus.h>

#include <CKPE.GDIPlus.h>

namespace CKPE
{
	static GDIPlus _sgdiplus;
	static Gdiplus::GdiplusStartupInput	_sgdiplusStartupInput;
	static ULONG_PTR _sgdiplusToken;

	GDIPlus::GDIPlus() noexcept(true)
	{
		Gdiplus::GdiplusStartup(&_sgdiplusToken, &_sgdiplusStartupInput, nullptr);
	}

	GDIPlus::~GDIPlus() noexcept(true)
	{
		Gdiplus::GdiplusShutdown(_sgdiplusToken);
	}

	const GDIPlus* GDIPlus::GetSingleton() noexcept(true)
	{
		return &_sgdiplus;
	}
}