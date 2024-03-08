// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Singleton.h"
#include "NiAPI/NiTypes.h"
#include "../BGSClasses.h"
#include "../BGSPickHandler.h"
#include "TESObjectCELL.h"
#include "TESObjectREFR.h"
#include "BGSRenderWindowCamera.h"
#include "BSPointerHandleManager.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class BGSRenderWindow
			{
			public:
				typedef BGSPickHandler<TESObjectREFR, BGSRenderWindow, 0x30> PickHandlerT;

				virtual ~BGSRenderWindow() = default;

				inline static Core::ISingleton<BGSRenderWindow> Singleton;
				inline static TESForm* GetRef(uint32_t UniqueId)
				{
					auto Ret = (TESObjectREFR*)GetRefFormByUniqueId(UniqueId);
					return (Ret && Ret->Type == TESObjectREFR::TYPE_ID) ? Ret : nullptr;
				}

				inline HWND GetWindowHandle() const { return _WindowHandle; }
				inline SIZE GetWindowSize() const { return _WindowSize; }
				inline POINT GetMousePos() const { return _MousePos[0]; }
				inline POINT GetMousePosBefore() const { return _MousePos[1]; }

				inline BGSRenderWindowEditModuleManager<BGSRenderWindow>* GetEditModuleManager() const { return _EditModuleManager; }
				inline BGSRenderWindowBorder* GetBorder() const { return _Border; }
				inline BGSRenderWindowCamera* GetCamera() const { return _Camera; }
				inline BGSRenderOrthoGrid* GetOrthoGrid() const { return _OrthoGrid; }
				inline PickHandlerT* GetPickHandler() const { return _PickHandler; }
				inline TESObjectCELL* GetCurrentCell() const { return _CurrentCell; }

				READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				READ_PROPERTY(GetWindowSize) SIZE WindowSize;
				READ_PROPERTY(GetMousePos) POINT MousePos;
				READ_PROPERTY(GetMousePosBefore) POINT MousePosBefore;

				READ_PROPERTY(GetEditModuleManager) BGSRenderWindowEditModuleManager<BGSRenderWindow>* EditModuleManager;
				READ_PROPERTY(GetBorder) BGSRenderWindowBorder* Border;
				READ_PROPERTY(GetCamera) BGSRenderWindowCamera* Camera;
				READ_PROPERTY(GetOrthoGrid) BGSRenderOrthoGrid* OrthoGrid;
				READ_PROPERTY(GetPickHandler) PickHandlerT* PickHandler;
				READ_PROPERTY(GetCurrentCell) TESObjectCELL* CurrentCell;
			private:
				char pad8[0x8];
				BSTArray<void*> _Tools;
				char pad28[0x48];
				HWND _WindowHandle;
				char pad10[0x30];
				BGSRenderWindowEditModuleManager<BGSRenderWindow>* _EditModuleManager;
				BGSRenderOrthoGrid* _OrthoGrid;
				BGSRenderWindowBorder* _Border;
				BGSRenderWindowCamera* _Camera;
				NiWindow* _Window;
				PickHandlerT* _PickHandler;
				BSCullingProcess* _CullingProcess;
				BSPortalGraphEntry* _PortalGraphEntry;
				char padE8[0x8];
				class Sky* _Sky;
				TESObjectCELL* _CurrentCell;
				char pad100[0x04];
				SIZE _WindowSize;
				POINT _MousePos[2];
				char padB4[0x64];
			};
			static_assert(sizeof(BGSRenderWindow) == 0x180);
		}
	}
}

#pragma pack(pop)