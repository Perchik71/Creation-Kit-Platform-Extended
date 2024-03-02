// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Singleton.h"
#include "NiAPI/NiTypes.h"
#include "TESObjectREFR.h"
#include "../BGSClasses.h"
#include "../BGSPickHandler.h"
#include "BGSRenderWindowCamera.h"
#include "BSPointerHandleManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			// size 0x100
			// func 2
			class BGSRenderWindow
			{
			public:
				virtual ~BGSRenderWindow() = default;
				virtual void Refresh(const uint32_t& flag);		// flag always equal 2

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
				inline BGSPickHandler<TESObjectREFR, BGSRenderWindow>* GetPickHandler() const { return _PickHandler; }
				inline TESObjectCELL* GetCurrentCell() const { return _CurrentCell[0]; }
				inline TESObjectCELL* GetCurrentCellParentExt() const { return _CurrentCell[1]; }

				READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				READ_PROPERTY(GetWindowSize) SIZE WindowSize;
				READ_PROPERTY(GetMousePos) POINT MousePos;
				READ_PROPERTY(GetMousePosBefore) POINT MousePosBefore;

				READ_PROPERTY(GetEditModuleManager) BGSRenderWindowEditModuleManager<BGSRenderWindow>* EditModuleManager;
				READ_PROPERTY(GetBorder) BGSRenderWindowBorder* Border;
				READ_PROPERTY(GetCamera) BGSRenderWindowCamera* Camera;
				READ_PROPERTY(GetOrthoGrid) BGSRenderOrthoGrid* OrthoGrid;
				READ_PROPERTY(GetPickHandler) BGSPickHandler<TESObjectREFR, BGSRenderWindow>* PickHandler;
				READ_PROPERTY(GetCurrentCell) TESObjectCELL* CurrentCell;
				READ_PROPERTY(GetCurrentCellParentExt) TESObjectCELL* CurrentCellParentExt;
			private:
				struct SceneTag
				{
					class ShadowSceneNode* _ShadowSceneNode;
					NiNode* _UnkNode[2];
					class Sky* _Sky;
				};

				HWND _WindowHandle;
				char pad10[0x30];
				BGSRenderWindowEditModuleManager<BGSRenderWindow>* _EditModuleManager;
				BGSRenderOrthoGrid* _OrthoGrid;
				BGSRenderWindowBorder* _Border;
				BGSRenderWindowCamera* _Camera;
				NiWindow* _Window;
				BGSPickHandler<TESObjectREFR, BGSRenderWindow>* _PickHandler;
				BSCullingProcess* _CullingProcess;
				BSPortalGraphEntry* _PortalGraphEntry;
				SceneTag* _Scene;
				TESObjectCELL* _CurrentCell[2];
				char pad98[0x04];
				SIZE _WindowSize;
				POINT _MousePos[2];
				char padB4[0x4C];
			};
			static_assert(sizeof(BGSRenderWindow) == 0x100);
		}
	}
}