// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "GDIPlusInit.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		GDIPlusInit* GlobalGDIPlusInitPtr = nullptr;

		GDIPlusInit::GDIPlusInit(Engine* lpEngine) : _engine(lpEngine)
		{
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		}

		GDIPlusInit::~GDIPlusInit()
		{
			Gdiplus::GdiplusShutdown(gdiplusToken);
		}
	}
}