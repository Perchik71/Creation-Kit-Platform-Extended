// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Engine;

		class GDIPlusInit
		{
		public:
			GDIPlusInit(Engine* lpEngine);
			~GDIPlusInit();
		private:
			Engine* _engine;
			
			Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
			ULONG_PTR						gdiplusToken;
		};

		extern GDIPlusInit* GlobalGDIPlusInitPtr;
	}
}