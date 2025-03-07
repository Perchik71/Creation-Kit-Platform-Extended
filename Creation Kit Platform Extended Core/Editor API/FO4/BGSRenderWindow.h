// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Singleton.h"
#include "NiAPI/NiTypes.h"
#include "../BGSClasses.h"
#include "../BGSPickHandler.h"
#include "../Settings.h"
#include "TESObjectCELL.h"
#include "TESObjectREFR.h"
#include "BGSRenderWindowCamera.h"
#include "BSPointerHandleManager.h"
#include "BGSRenderWindowEditModuleManager.h"

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

				enum : uint8_t {
					rwsSnapToGrid = 0x1,
					rwsSnapToAngle = 0x2,
					rwsSnapToConnectPoints = 0x8,
				};

				virtual ~BGSRenderWindow() = default;

				inline static Core::ISingleton<BGSRenderWindow> Singleton;
				inline static TESForm* GetRef(uint32_t UniqueId)
				{
					auto Ret = (TESObjectREFR*)GetRefFormByUniqueId(UniqueId);
					if (!Ret) return nullptr;
					
					// It does not always return 0x40, it depends on the parent

					switch (Ret->Type)
					{
					case TESObjectREFR::TYPE_ID:	// 0x40
					case TESForm::ftCharacter:		// 0x41
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
						inline static Core::ISingleton<EditorAPI::Setting> FlagsSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> SnapGridValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> SnapAngleValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> ArrowSnapValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> ObjectRotateValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> ObjectMoveValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> CameraRotateValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> CameraZoomValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> CameraZoomOrthoValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> CameraPanValueSingleton;
						inline static Core::ISingleton<EditorAPI::Setting> LandspaceMultValueSingleton;

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
					inline static UINT DrawCalls;
					inline static UINT FramePerSecond;
					inline static char TexturesSize[32];
					inline static UINT Polys;
				};

				inline static bool HasSnapToGrid() { return (Settings::Movement::GetFlags() & rwsSnapToGrid) == rwsSnapToGrid; }
				inline static bool HasSnapToAngle() { return (Settings::Movement::GetFlags() & rwsSnapToAngle) == rwsSnapToAngle; }
				inline static bool HasSnapToConnectPoints() { return (Settings::Movement::GetFlags() & rwsSnapToConnectPoints) == rwsSnapToConnectPoints; }

				inline BGSRenderWindowEditModuleManager* GetEditModuleManager() const { return _EditModuleManager; }
				inline BGSRenderWindowBorder* GetBorder() const { return _Border; }
				inline BGSRenderWindowCamera* GetCamera() const { return _Camera; }
				inline BGSRenderOrthoGrid* GetOrthoGrid() const { return _OrthoGrid; }
				inline PickHandlerT* GetPickHandler() const { return _PickHandler; }
				inline TESObjectCELL* GetCurrentCell() const { return _CurrentCell; }

				READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				READ_PROPERTY(GetWindowSize) SIZE WindowSize;
				READ_PROPERTY(GetMousePos) POINT MousePos;
				READ_PROPERTY(GetMousePosBefore) POINT MousePosBefore;

				READ_PROPERTY(GetEditModuleManager) BGSRenderWindowEditModuleManager* EditModuleManager;
				READ_PROPERTY(GetBorder) BGSRenderWindowBorder* Border;
				READ_PROPERTY(GetCamera) BGSRenderWindowCamera* Camera;
				READ_PROPERTY(GetOrthoGrid) BGSRenderOrthoGrid* OrthoGrid;
				READ_PROPERTY(GetPickHandler) PickHandlerT* PickHandler;
				READ_PROPERTY(GetCurrentCell) TESObjectCELL* CurrentCell;
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
				/* 0x0F8 */ TESObjectCELL* _CurrentCell;
				/* 0x100 */ char pad100[0x04];
				/* 0x104 */ SIZE _WindowSize;
				/* 0x10C */ POINT _MousePos[2];
				/* 0x11C */ char pad11C[0x64];
			};
			static_assert(sizeof(BGSRenderWindow) == 0x180);
		}
	}
}

#pragma pack(pop)