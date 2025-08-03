// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include "NiAPI/NiTypes.h"
#include "TESObjectREFR.h"
#include "../BGSClasses.h"
#include "../BGSPickHandler.h"
#include "BGSRenderWindowCamera.h"
#include "BSPointerHandleManager.h"
#include "BSTriShape.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
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
					
					// It does not always return 0x3D, it depends on the parent

					switch (Ret->Type)
					{
					case TESObjectREFR::TYPE_ID:	// 0x3D
					case TESForm::ftCharacter:		// 0x3E
						break;
					default:
						return nullptr;
					}

					return Ret;
				}

				struct Pick
				{
					inline static TESObjectREFR* Result;
					inline static TESObjectREFR* (*GetRefFromTriShape)(BSTriShape* TriShape);
					inline static void* (*Update)(BGSRenderWindowReferenceEditModule* EditModule, POINT* MousePos, POINT* MousePos2);
					inline static TESObjectREFR* HKGetRefFromTriShape(BSTriShape* TriShape)
					{
						Result = GetRefFromTriShape(TriShape);
						return Result;
					}
					inline static void* HKUpdate(BGSRenderWindowReferenceEditModule* EditModule, POINT* MousePos, POINT* MousePos2)
					{
						Result = nullptr;
						return Update(EditModule, MousePos, MousePos2);
					}
				};

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

				CKPE_READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				CKPE_READ_PROPERTY(GetWindowSize) SIZE WindowSize;
				CKPE_READ_PROPERTY(GetMousePos) POINT MousePos;
				CKPE_READ_PROPERTY(GetMousePosBefore) POINT MousePosBefore;

				CKPE_READ_PROPERTY(GetEditModuleManager) BGSRenderWindowEditModuleManager<BGSRenderWindow>* EditModuleManager;
				CKPE_READ_PROPERTY(GetBorder) BGSRenderWindowBorder* Border;
				CKPE_READ_PROPERTY(GetCamera) BGSRenderWindowCamera* Camera;
				CKPE_READ_PROPERTY(GetOrthoGrid) BGSRenderOrthoGrid* OrthoGrid;
				CKPE_READ_PROPERTY(GetPickHandler) BGSPickHandler<TESObjectREFR, BGSRenderWindow>* PickHandler;
				CKPE_READ_PROPERTY(GetCurrentCell) TESObjectCELL* CurrentCell;
				CKPE_READ_PROPERTY(GetCurrentCellParentExt) TESObjectCELL* CurrentCellParentExt;
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