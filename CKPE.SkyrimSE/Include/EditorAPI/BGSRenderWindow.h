// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <CKPE.Singleton.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include "BGSClasses.h"
#include "BGSPickHandler.h"
#include "BGSRenderWindowCamera.h"
#include "BSPointerHandleManager.h"
#include "BSTriShape.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// size 0xE0
			// func 2
			class BGSRenderWindow
			{
			public:
				virtual ~BGSRenderWindow() = default;
				virtual void Refresh(const std::uint32_t& flag);		// flag always equal 2

				inline static ISingleton<BGSRenderWindow> Singleton;
				inline static Forms::TESForm* GetRef(std::uint32_t UniqueId)
				{
					auto Ret = (Forms::TESObjectREFR*)GetRefFormByUniqueId(UniqueId);
					
					// It does not always return 0x3D, it depends on the parent

					switch (Ret->Type)
					{
					case Forms::TESObjectREFR::TYPE_ID:	// 0x3D
					case Forms::TESForm::ftCharacter:	// 0x3E
						break;
					default:
						return nullptr;
					}

					return Ret;
				}

				struct Pick
				{
					inline static Forms::TESObjectREFR* Result;
					inline static Forms::TESObjectREFR* (*GetRefFromTriShape)(BSTriShape* TriShape);
					inline static void* (*Update)(BGSRenderWindowReferenceEditModule* EditModule, POINT* MousePos, POINT* MousePos2);
					inline static Forms::TESObjectREFR* HKGetRefFromTriShape(BSTriShape* TriShape) noexcept(true)
					{
						Result = GetRefFromTriShape(TriShape);
						return Result;
					}
					inline static void* HKUpdate(BGSRenderWindowReferenceEditModule* EditModule, 
						POINT* MousePos, POINT* MousePos2) noexcept(true)
					{
						Result = nullptr;
						return Update(EditModule, MousePos, MousePos2);
					}
				};

				[[nodiscard]] constexpr inline HWND GetWindowHandle() const noexcept(true) { return _WindowHandle; }
				
				[[nodiscard]] constexpr inline SIZE GetWindowSize() const noexcept(true) 
				{ 
					return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
						difference.v1_5._WindowSize : difference.v1_6._WindowSize;
				}

				[[nodiscard]] constexpr inline POINT GetMousePos() const noexcept(true) 
				{
					return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
						difference.v1_5._MousePos[0] : difference.v1_6._MousePos[0];
				}

				[[nodiscard]] constexpr inline POINT GetMousePosBefore() const noexcept(true)
				{ 
					return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
						difference.v1_5._MousePos[1] : difference.v1_6._MousePos[1];
				}

				[[nodiscard]] inline BGSRenderWindowEditModuleManager<BGSRenderWindow>* GetEditModuleManager() const noexcept(true) { return _EditModuleManager; }
				[[nodiscard]] inline BGSRenderWindowBorder* GetBorder() const noexcept(true) { return _Border; }
				[[nodiscard]] inline BGSRenderWindowCamera* GetCamera() const noexcept(true) { return _Camera; }
				[[nodiscard]] inline BGSRenderOrthoGrid* GetOrthoGrid() const noexcept(true) { return _OrthoGrid; }
				[[nodiscard]] inline BGSPickHandler<Forms::TESObjectREFR, BGSRenderWindow>* GetPickHandler() const noexcept(true) { return _PickHandler; }
				
				[[nodiscard]] inline Forms::TESObjectCELL* GetCurrentCell() const noexcept(true) 
				{ 
					return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
						difference.v1_5._CurrentCell : difference.v1_6._CurrentCell[0];
				}

				CKPE_READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				CKPE_READ_PROPERTY(GetWindowSize) SIZE WindowSize;
				CKPE_READ_PROPERTY(GetMousePos) POINT MousePos;
				CKPE_READ_PROPERTY(GetMousePosBefore) POINT MousePosBefore;

				CKPE_READ_PROPERTY(GetEditModuleManager) BGSRenderWindowEditModuleManager<BGSRenderWindow>* EditModuleManager;
				CKPE_READ_PROPERTY(GetBorder) BGSRenderWindowBorder* Border;
				CKPE_READ_PROPERTY(GetCamera) BGSRenderWindowCamera* Camera;
				CKPE_READ_PROPERTY(GetOrthoGrid) BGSRenderOrthoGrid* OrthoGrid;
				CKPE_READ_PROPERTY(GetPickHandler) BGSPickHandler<Forms::TESObjectREFR, BGSRenderWindow>* PickHandler;
				CKPE_READ_PROPERTY(GetCurrentCell) Forms::TESObjectCELL* CurrentCell;
				CKPE_READ_PROPERTY(GetCurrentCellParentExt) Forms::TESObjectCELL* CurrentCellParentExt;
			private:
				struct SceneData
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
				BGSPickHandler<Forms::TESObjectREFR, BGSRenderWindow>* _PickHandler;
				BSCullingProcess* _CullingProcess;
				BSPortalGraphEntry* _PortalGraphEntry;
				SceneData* _Scene;

				union
				{
					struct _v1_5
					{
						Forms::TESObjectCELL* _CurrentCell;
						char pad98[0x04];
						SIZE _WindowSize;
						POINT _MousePos[2];
						char pad__[0x28];
					} v1_5;
					struct _v1_6
					{
						Forms::TESObjectCELL* _CurrentCell[2];
						char pad98[0x04];
						SIZE _WindowSize;
						POINT _MousePos[2];
						char pad__[0x28];
					} v1_6;
				} difference;
			};
			static_assert(sizeof(BGSRenderWindow) == 0xE0);
		}
	}
}