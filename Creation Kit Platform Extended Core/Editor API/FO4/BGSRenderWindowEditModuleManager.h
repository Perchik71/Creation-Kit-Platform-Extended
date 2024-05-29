#pragma once

#include "BGSRenderWindowReferenceEditModule.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class BGSRenderWindow;
			class BGSRenderWindowEditModuleManager
			{
			public:
				virtual ~BGSRenderWindowEditModuleManager() = default;

				inline BGSRenderWindow* GetRenderWindow() { return _RenderWindow; }
				inline BGSRenderWindowReferenceEditModule* GetReferenceEditModule() { return _ReferenceEditModule; }

				READ_PROPERTY(GetRenderWindow) BGSRenderWindow* RenderWindow;
				READ_PROPERTY(GetReferenceEditModule) BGSRenderWindowReferenceEditModule* ReferenceEditModule;
			private:
				BGSRenderWindow* _RenderWindow;
				BGSRenderWindowReferenceEditModule* _ReferenceEditModule;
				char pad08[0x28];
			};
		}
	}
}