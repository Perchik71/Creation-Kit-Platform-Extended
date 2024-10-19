// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <QtGui/qwindow.h>
#include <QtWidgets/qwidget.h>

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			// This QWidget 
			class BGSRenderWindow : public QWidget
			{
			public:
				~BGSRenderWindow() = default;

			};

			extern BGSRenderWindow* GlobalRenderWindowPtr;
		}
	}
}

#pragma pack(pop)
