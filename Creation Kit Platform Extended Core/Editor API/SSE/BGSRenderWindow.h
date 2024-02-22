// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\NiTypes.h"
#include "TESForm.h"
#include "Editor API/SSE/BSPointerHandleManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class BGSPickHandler;
			class BGSRenderWindow;

			class BGSRenderWindowReferenceEditModule
			{
			public:
				virtual ~BGSRenderWindowReferenceEditModule() = default;

			private:

			};

			// size 0x40
			// func 2
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
			static_assert(sizeof(BGSRenderWindowEditModuleManager) == 0x40);

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

			// size 0x100
			// func 1
			class BGSRenderWindowCamera
			{
			public:
				enum ViewModeT : uint32_t
				{
					Perspective = 0,
					Orthographic
				};

				enum CameraModeT : uint32_t
				{
					Front = 0,
					Left,
					Top,
					Free,
					North,
					West
				};

				inline static const char* CameraModeS[8] =
				{
					"Front view",
					"Left view",
					"Top view",
					"Free camera",
					"North view",
					"West view",
					"Top view",
					"Free camera",
				};
			public:
				virtual ~BGSRenderWindowCamera() = default;

				inline NiPoint3 GetPosition() const { return *(NiPoint3*)(((char*)(*_Data)) + 0x6C); }
				inline ViewModeT GetViewMode() const { return _ViewMode; }
				inline const char* GetViewModeByStr() const { return _ViewMode == Perspective ? "perspective" : "orthographic"; }
				inline CameraModeT GetCameraMode() const { return _CameraMode; }
				inline const char* GetCameraModeByStr() const { return CameraModeS[(int)_CameraMode]; }

				READ_PROPERTY(GetPosition) NiPoint3 Position;
				READ_PROPERTY(GetViewMode) ViewModeT ViewMode;
				READ_PROPERTY(GetCameraMode) CameraModeT CameraMode;
			private:
				NiNode** _Data;
				NiCamera* _Camera;
				ViewModeT _ViewMode;
				CameraModeT _CameraMode;
				float _Nums[14];
				BGSRenderWindow* _RenderWindow;
				float _Nums2;
				char pad64[0x9C];
			};
			static_assert(sizeof(BGSRenderWindowCamera) == 0x100);

			// size 0x100
			// func 2
			class BGSRenderWindow
			{
			public:
				virtual ~BGSRenderWindow() = default;
				virtual void Refresh(const uint32_t& flag);		// flag always equal 2

				inline static BGSRenderWindow** Instance;
				inline static BGSRenderWindow* GetInstance() { return *Instance; }
				inline static TESForm* GetRef(uint32_t UniqueId)
				{
					auto Ret = GetRefFormByUniqueId(UniqueId);
					return (Ret && Ret->FormType == 0x3D) ? Ret : nullptr;
				}

				inline HWND GetWindowHandle() const { return _WindowHandle; }
				inline SIZE GetWindowSize() const { return _WindowSize; }
				inline POINT GetMousePos() const { return _MousePos[0]; }
				inline POINT GetMousePosBefore() const { return _MousePos[1]; }

				inline BGSRenderWindowEditModuleManager* GetEditModuleManager() const { return _EditModuleManager; }
				inline BGSRenderWindowBorder* GetBorder() const { return _Border; }
				inline BGSRenderWindowCamera* GetCamera() const { return _Camera; }
				inline BGSRenderOrthoGrid* GetOrthoGrid() const { return _OrthoGrid; }
				inline BGSPickHandler* GetPickHandler() const { return _PickHandler; }
				inline TESForm* GetCurrentCell() const { return _CurrentCell[0]; }
				inline TESForm* GetCurrentCellParentExt() const { return _CurrentCell[1]; }

				READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				READ_PROPERTY(GetWindowSize) SIZE WindowSize;
				READ_PROPERTY(GetMousePos) POINT MousePos;
				READ_PROPERTY(GetMousePosBefore) POINT MousePosBefore;

				READ_PROPERTY(GetEditModuleManager) BGSRenderWindowEditModuleManager* EditModuleManager;
				READ_PROPERTY(GetBorder) BGSRenderWindowBorder* Border;
				READ_PROPERTY(GetCamera) BGSRenderWindowCamera* Camera;
				READ_PROPERTY(GetOrthoGrid) BGSRenderOrthoGrid* OrthoGrid;
				READ_PROPERTY(GetPickHandler) BGSPickHandler* PickHandler;
				READ_PROPERTY(GetCurrentCell) TESForm* CurrentCell;
				READ_PROPERTY(GetCurrentCellParentExt) TESForm* CurrentCellParentExt;
			private:
				struct SceneTag
				{
					class ShadowSceneNode* _ShadowSceneNode;
					NiNode* _UnkNode[2];
					class Sky* _Sky;
				};

				HWND _WindowHandle;
				char pad10[0x30];
				BGSRenderWindowEditModuleManager* _EditModuleManager;
				BGSRenderOrthoGrid* _OrthoGrid;
				BGSRenderWindowBorder* _Border;
				BGSRenderWindowCamera* _Camera;
				NiWindow* _Window;
				BGSPickHandler* _PickHandler;
				BSCullingProcess* _CullingProcess;
				BSPortalGraphEntry* _PortalGraphEntry;
				SceneTag* _Scene;
				TESForm* _CurrentCell[2];
				char pad98[0x04];
				SIZE _WindowSize;
				POINT _MousePos[2];
				char padB4[0x4C];
			};
			static_assert(sizeof(BGSRenderWindow) == 0x100);

			// it's not in RTTI, I named it the way I see it.
			class BGSPickHandler
			{
			public:
				class Node
				{
				protected:
					Node* _next;
					Node* _prev;
					uint32_t _index;
				public:
					inline Node* GetNext() const { return _next; }
					inline Node* GetPrev() const { return _prev; }
					inline uint32_t GetIndex() const { return _index; }
					inline TESForm* GetRef() const
					{
						auto Renderer = BGSRenderWindow::GetInstance();
						if (!Renderer) return nullptr;
						return Renderer->GetRef(_index);
					}

					READ_PROPERTY(GetNext) Node* Next;
					READ_PROPERTY(GetPrev) Node* Prev;
					READ_PROPERTY(GetIndex) uint32_t Index;
					READ_PROPERTY(GetRef) TESForm* Ref;
				};

				class List : public Node
				{
				public:
					inline Node* GetFirst() const { return _next; }
					inline Node* GetEnd() const { return _prev; }

					READ_PROPERTY(GetFirst) Node* First;
					READ_PROPERTY(GetEnd) Node* End;
				};
			public:
				inline uint32_t GetCount() const { return _Count; }
				inline NiPoint3 GetCenter() const { return _Center; }
				inline List* GetItems() { return _Items; }

				inline bool Has(const TESForm* lpForm) const
				{
					if (!_Items || !_Count) return false;

					auto It = _Items->First;
					for (uint32_t i = 0; i < _Count; i++, It = It->Next)
					{
						if (It->Ref == lpForm) 
							return true;
					}

					return false;
				}

				READ_PROPERTY(GetCount) uint32_t Count;
				READ_PROPERTY(GetCenter) NiPoint3 Center;
				READ_PROPERTY(GetItems) List* Items;
			private:
				uint32_t _Count;
				NiPoint3 _Center;
				char pad10[0x28];
				List* _Items;
			};
		}
	}
}