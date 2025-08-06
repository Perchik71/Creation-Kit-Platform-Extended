// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <windows.h>
#include "NiAPI/NiNode.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BGSRenderWindowReferenceEditModule
			{
			public:
				virtual ~BGSRenderWindowReferenceEditModule() = default;
			};

			// size 0x40
			// func 2
			template<typename _Ty>
			class BGSRenderWindowEditModuleManager
			{
			public:
				virtual ~BGSRenderWindowEditModuleManager() = default;

				inline _Ty* GetRenderWindow() { return _RenderWindow; }
				inline BGSRenderWindowReferenceEditModule* GetReferenceEditModule() { return _ReferenceEditModule; }

				CKPE_READ_PROPERTY(GetRenderWindow) _Ty* RenderWindow;
				CKPE_READ_PROPERTY(GetReferenceEditModule) BGSRenderWindowReferenceEditModule* ReferenceEditModule;
			private:
				_Ty* _RenderWindow;
				BGSRenderWindowReferenceEditModule* _ReferenceEditModule;
				char pad08[0x28];
			};
			static_assert(sizeof(BGSRenderWindowEditModuleManager<void>) == 0x40);

			class BGSRenderWindowBorder
			{
			public:
				virtual ~BGSRenderWindowBorder() = default;

				[[nodiscard]] inline HWND GetWindowHandle() const noexcept(true) { return _WindowHandle; }
				CKPE_READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
			private:
				HWND _WindowHandle;
			};

			class BSPortalGraphEntry
			{
			public:
				virtual ~BSPortalGraphEntry() = default;
			};

			class BSCullingProcess
			{
			public:
				virtual ~BSCullingProcess() = default;
			};

			class BGSRenderOrthoGrid
			{
			public:
				virtual ~BGSRenderOrthoGrid() = default;
			private:
				char pad08[0x8];
				NiAPI::NiNode* _Node;
				class BSDynamicLines* _DynamicLines;
			};
		}
	}
}