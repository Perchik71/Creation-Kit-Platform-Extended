// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BGSRenderWindowReferenceEditModule.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSRenderWindow;
			class BGSRenderWindowEditModuleManager
			{
			public:
				virtual ~BGSRenderWindowEditModuleManager() = default;

				inline BGSRenderWindow* GetRenderWindow() { return _RenderWindow; }
				inline BGSRenderWindowReferenceEditModule* GetReferenceEditModule() { return _ReferenceEditModule; }

				CKPE_READ_PROPERTY(GetRenderWindow) BGSRenderWindow* RenderWindow;
				CKPE_READ_PROPERTY(GetReferenceEditModule) BGSRenderWindowReferenceEditModule* ReferenceEditModule;
			private:
				BGSRenderWindow* _RenderWindow;
				BGSRenderWindowReferenceEditModule* _ReferenceEditModule;
				char pad08[0x28];
			};
		}
	}
}