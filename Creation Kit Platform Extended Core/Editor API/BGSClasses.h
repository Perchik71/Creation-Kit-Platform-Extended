// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiNode.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		using namespace NiAPI;

		class BGSRenderWindowReferenceEditModule
		{
		public:
			virtual ~BGSRenderWindowReferenceEditModule() = default;

		private:

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

			READ_PROPERTY(GetRenderWindow) _Ty* RenderWindow;
			READ_PROPERTY(GetReferenceEditModule) BGSRenderWindowReferenceEditModule* ReferenceEditModule;
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

			inline HWND GetWindowHandle() const { return _WindowHandle; }
			READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
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
			NiNode* _Node;
			class BSDynamicLines* _DynamicLines;
		};
	}
}