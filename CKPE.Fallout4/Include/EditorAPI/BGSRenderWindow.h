// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include "NiAPI/NiTypes.h"
#include <EditorAPI/Forms/TESObjectCELL.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include "BGSRenderWindowCamera.h"
#include "BSPointerHandleManager.h"
#include "BGSRenderWindowEditModuleManager.h"
#include "BGSPickHandler.h"
#include "BSSettings.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			struct CKPE_CoordRefr
			{
				EditorAPI::NiAPI::NiPoint3 Pos;
				EditorAPI::Forms::TESObjectREFR* Refr;
			};

			class BGSRenderWindowBorder
			{
			public:
				virtual ~BGSRenderWindowBorder() = default;

				inline HWND GetWindowHandle() const { return _WindowHandle; }
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
				NiNode* _Node;
				class BSDynamicLines* _DynamicLines;
			};

			class BGSRenderWindow
			{
			public:
				typedef BGSPickHandler<Forms::TESObjectREFR, BGSRenderWindow, 0x30> PickHandlerT;

				enum : std::uint8_t
				{
					rwsSnapToGrid = 0x1,
					rwsSnapToAngle = 0x2,
					rwsSnapToConnectPoints = 0x8,
				};

				virtual ~BGSRenderWindow() = default;

				inline static ISingleton<BGSRenderWindow> Singleton;
				inline static Forms::TESForm* GetRef(std::uint32_t UniqueId)
				{
					auto Ret = (Forms::TESObjectREFR*)GetRefFormByUniqueId(UniqueId);
					if (!Ret) return nullptr;
					
					// It does not always return 0x40, it depends on the parent

					switch (Ret->Type)
					{
					case Forms::TESObjectREFR::TYPE_ID:	// 0x40
					case Forms::TESForm::ftCharacter:	// 0x41
						break;
					default:
						return nullptr;
					}

					return Ret;
				}

				inline HWND GetWindowHandle() const { return _WindowHandle; }
				inline SIZE GetWindowSize() const { return _WindowSize; }
				inline POINT GetMousePos() const { return _MousePos[0]; }
				inline POINT GetMousePosBefore() const { return _MousePos[1]; }

				struct Settings
				{
					struct Movement
					{
						inline static ISingleton<EditorAPI::Setting> FlagsSingleton;
						inline static ISingleton<EditorAPI::Setting> SnapGridValueSingleton;
						inline static ISingleton<EditorAPI::Setting> SnapAngleValueSingleton;
						inline static ISingleton<EditorAPI::Setting> ArrowSnapValueSingleton;
						inline static ISingleton<EditorAPI::Setting> ObjectRotateValueSingleton;
						inline static ISingleton<EditorAPI::Setting> ObjectMoveValueSingleton;
						inline static ISingleton<EditorAPI::Setting> CameraRotateValueSingleton;
						inline static ISingleton<EditorAPI::Setting> CameraZoomValueSingleton;
						inline static ISingleton<EditorAPI::Setting> CameraZoomOrthoValueSingleton;
						inline static ISingleton<EditorAPI::Setting> CameraPanValueSingleton;
						inline static ISingleton<EditorAPI::Setting> LandspaceMultValueSingleton;

						inline static uint32_t GetFlags() { return FlagsSingleton.Singleton->GetUnsignedInt(); }
						inline static uint32_t GetSnapGrid() { return SnapGridValueSingleton.Singleton->GetUnsignedInt(); }
						inline static float GetSnapAngle() { return SnapAngleValueSingleton.Singleton->GetFloat(); }
						inline static uint32_t GetArrowSnap() { return ArrowSnapValueSingleton.Singleton->GetUnsignedInt(); }
						inline static float GetObjectRotate() { return ObjectRotateValueSingleton.Singleton->GetFloat(); }
						inline static float GetObjectMove() { return ObjectMoveValueSingleton.Singleton->GetFloat(); }
						inline static float GetCameraRotate() { return CameraRotateValueSingleton.Singleton->GetFloat(); }
						inline static float GetCameraZoom() { return CameraZoomValueSingleton.Singleton->GetFloat(); }
						inline static float GetCameraZoomOrtho() { return CameraZoomOrthoValueSingleton.Singleton->GetFloat(); }
						inline static float GetCameraPan() { return CameraPanValueSingleton.Singleton->GetFloat(); }
						inline static float GetLandspaceMult() { return LandspaceMultValueSingleton.Singleton->GetFloat(); }
					};
				};

				struct DrawInfo
				{
					inline static std::uint32_t DrawCalls;
					inline static std::uint32_t FramePerSecond;
					inline static char TexturesSize[32];
					inline static std::uint32_t Polys;
				};

				struct Pick
				{
					inline static Forms::TESObjectREFR* Result;
					inline static Forms::TESObjectREFR* (*GetRefFromNiNode)(void* ObjNode);
					inline static Forms::TESObjectREFR* HKGetRefFromNiNode(void* ObjNode)
					{
						Result = GetRefFromNiNode(ObjNode);
						return Result;
					}
				};

				inline static bool HasSnapToGrid() { return (Settings::Movement::GetFlags() & rwsSnapToGrid) == rwsSnapToGrid; }
				inline static bool HasSnapToAngle() { return (Settings::Movement::GetFlags() & rwsSnapToAngle) == rwsSnapToAngle; }
				inline static bool HasSnapToConnectPoints() { return (Settings::Movement::GetFlags() & rwsSnapToConnectPoints) == rwsSnapToConnectPoints; }

				inline BGSRenderWindowEditModuleManager* GetEditModuleManager() const { return _EditModuleManager; }
				inline BGSRenderWindowBorder* GetBorder() const { return _Border; }
				inline BGSRenderWindowCamera* GetCamera() const { return _Camera; }
				inline BGSRenderOrthoGrid* GetOrthoGrid() const { return _OrthoGrid; }
				inline PickHandlerT* GetPickHandler() const { return _PickHandler; }
				inline Forms::TESObjectCELL* GetCurrentCell() const { return _CurrentCell; }

				CKPE_READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				CKPE_READ_PROPERTY(GetWindowSize) SIZE WindowSize;
				CKPE_READ_PROPERTY(GetMousePos) POINT MousePos;
				CKPE_READ_PROPERTY(GetMousePosBefore) POINT MousePosBefore;

				CKPE_READ_PROPERTY(GetEditModuleManager) BGSRenderWindowEditModuleManager* EditModuleManager;
				CKPE_READ_PROPERTY(GetBorder) BGSRenderWindowBorder* Border;
				CKPE_READ_PROPERTY(GetCamera) BGSRenderWindowCamera* Camera;
				CKPE_READ_PROPERTY(GetOrthoGrid) BGSRenderOrthoGrid* OrthoGrid;
				CKPE_READ_PROPERTY(GetPickHandler) PickHandlerT* PickHandler;
				CKPE_READ_PROPERTY(GetCurrentCell) Forms::TESObjectCELL* CurrentCell;
			private:
				/* 0x008 */ char pad8[0x8];
				/* 0x010 */ BSTArray<void*> _Tools;
				/* 0x028 */ char pad28[0x48];
				/* 0x070 */ HWND _WindowHandle;
				/* 0x078 */ char pad78[0x30];
				/* 0x0A8 */ BGSRenderWindowEditModuleManager* _EditModuleManager;
				/* 0x0B0 */ BGSRenderOrthoGrid* _OrthoGrid;
				/* 0x0B8 */ BGSRenderWindowBorder* _Border;
				/* 0x0C0 */ BGSRenderWindowCamera* _Camera;
				/* 0x0C8 */ NiWindow* _Window;
				/* 0x0D0 */ PickHandlerT* _PickHandler;
				/* 0x0D8 */ BSCullingProcess* _CullingProcess;
				/* 0x0E0 */ BSPortalGraphEntry* _PortalGraphEntry;
				/* 0x0E8 */ char padE8[0x8];
				/* 0x0F0 */ class Sky* _Sky;
				/* 0x0F8 */ Forms::TESObjectCELL* _CurrentCell;
				/* 0x100 */ char pad100[0x04];
				/* 0x104 */ SIZE _WindowSize;
				/* 0x10C */ POINT _MousePos[2];
				/* 0x11C */ char pad11C[0x64];
			};
			static_assert(sizeof(BGSRenderWindow) == 0x180);
		}
	}
}