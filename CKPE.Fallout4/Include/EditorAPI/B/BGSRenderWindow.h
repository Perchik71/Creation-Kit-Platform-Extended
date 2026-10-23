// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <CKPE.Singleton.h>
#include <CKPE.Common.Relocation.h>

#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/T/TESObjectCELL.h>
#include <EditorAPI/T/TESObjectREFR.h>
#include <EditorAPI/B/BGSRenderWindowCamera.h>
#include <EditorAPI/B/BSPointerHandleManager.h>
#include <EditorAPI/B/BGSRenderWindowEditModuleManager.h>
#include <EditorAPI/B/BGSPickHandler.h>
#include <EditorAPI/B/BSSettings.h>
#include <EditorAPI/B/BSTEvents.h>
#include <EditorAPI/B/BGSRenderWindowBorder.h>
#include <EditorAPI/B/BGSRenderOrthoGrid.h>
#include <EditorAPI/B/BSCullingProcess.h>
#include <EditorAPI/S/Sky.h>

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

			struct RefSelectDeselectEvent;
			struct CellAttachDetachEvent;

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

				enum class State : std::uint32_t
				{
					Closed = 0,
					Loading = 1,
					Ready = 2,
					Drawing = 3,
				};

				enum WindowFlags : std::uint32_t
				{
					Updating = 0x1,
					InputEnabled = 0x4,
					MemoryDisplayDirty = 0x40,
					MemoryStatsDirty = 0x80,
					InitializeProjection = 0x100,
				};

				inline static ISingleton<BGSRenderWindow> Singleton;

				[[nodiscard]] inline static BGSRenderWindow* GetSingleton() noexcept(true)
				{
					static Common::Relocation<BGSRenderWindow**> singleton{ EditorAPI::IDs::BGSRenderWindow::Singleton };
					return *singleton;
				}

				inline void Update() noexcept(true)
				{
					using func_t = void(*)(BGSRenderWindow*);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::Update };
					func(this);
				}

				inline void Draw() noexcept(true)
				{
					using func_t = void(*)(BGSRenderWindow*);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::Draw };
					func(this);
				}

				inline void UpdateDrawInfo() noexcept(true)
				{
					using func_t = void(*)(BGSRenderWindow*);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::UpdateDrawInfo };
					func(this);
				}

				inline void UpdateTitle() noexcept(true)
				{
					using func_t = void(*)(BGSRenderWindow*);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::UpdateTitle };
					func(this);
				}

				inline void Initialize(HWND window) noexcept(true)
				{
					using func_t = void(*)(BGSRenderWindow*, HWND);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::Initialize };
					func(this, window);
				}

				[[nodiscard]] inline void* PickObject(const POINT& point,
					NiAPI::NiPoint3& position, NiAPI::NiPoint3* normal = nullptr) noexcept(true)
				{
					using func_t = void*(*)(BGSRenderWindow*, const POINT*,
						NiAPI::NiPoint3*, NiAPI::NiPoint3*);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::PickObject };
					return func(this, &point, &position, normal);
				}

				inline void OnMouseMove(std::int32_t x, std::int32_t y) noexcept(true)
				{
					using func_t = void(*)(BGSRenderWindow*, std::int32_t, std::int32_t);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::OnMouseMove };
					func(this, x, y);
				}

				inline void Refresh() noexcept(true)
				{
					if (_State != State::Drawing)
						_State = State::Ready;
				}

				inline static INT_PTR CALLBACK DialogProc(HWND window, UINT message,
					WPARAM wParam, LPARAM lParam) noexcept(true)
				{
					using func_t = INT_PTR(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::DialogProc };
					return func(window, message, wParam, lParam);
				}
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

					// Per render-stats group counters (CK 1.10.162.0, 1.10.982.3 and 1.11.240.0, see RenderWindow::UpdateDrawInfo).
					// "Polys" above is the sum of all groups, "DrawCalls" is group 2 only.
					static constexpr std::uint32_t GroupCount = 9;
					inline static std::uint32_t GroupDrawCalls[GroupCount];
					inline static std::uint32_t GroupPolys[GroupCount];
					inline static bool GroupsValid;
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
				[[nodiscard]] inline NiWindow* GetWindow() const noexcept(true) { return _Window; }
				[[nodiscard]] inline BSCullingProcess* GetCullingProcess() const noexcept(true) { return _CullingProcess; }
				[[nodiscard]] inline BSPortalGraphEntry* GetPortalGraphEntry() const noexcept(true) { return _PortalGraphEntry; }
				[[nodiscard]] inline State GetState() const noexcept(true) { return _State; }
				[[nodiscard]] inline float GetAverageFrameTime() const noexcept(true) { return _AverageFrameTime; }
				[[nodiscard]] inline bool IsInputEnabled() const noexcept(true) { return (_Flags & InputEnabled) != 0; }
				[[nodiscard]] inline Sky* GetSky() const noexcept(true) { return Sky::GetSingleton(); }
				[[nodiscard]] inline BSTEventDispatcher<RefSelectDeselectEvent>& GetSelectionEvents() noexcept(true) { return _SelectionEvents; }
				[[nodiscard]] inline bool HasWireframe() const noexcept(true)
				{
					return _SceneContext &&
						reinterpret_cast<const std::uint8_t*>(_SceneContext)[0x20] != 0;
				}

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
				CKPE_READ_PROPERTY(GetWindow) NiWindow* Window;
				CKPE_READ_PROPERTY(GetCullingProcess) BSCullingProcess* CullingProcess;
				CKPE_READ_PROPERTY(GetPortalGraphEntry) BSPortalGraphEntry* PortalGraphEntry;
				CKPE_READ_PROPERTY(GetState) State WindowState;
				CKPE_READ_PROPERTY(GetAverageFrameTime) float AverageFrameTime;
				CKPE_READ_PROPERTY(HasWireframe) bool Wireframe;

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BGSRenderWindow, _SelectionEvents) == 0x8 &&
						offsetof(BGSRenderWindow, _LoadedEventSink) == 0x60 &&
						offsetof(BGSRenderWindow, _CellEventSink) == 0x68 &&
						offsetof(BGSRenderWindow, _WindowHandle) == 0x70 &&
						offsetof(BGSRenderWindow, _EditModuleManager) == 0xA8 &&
						offsetof(BGSRenderWindow, _OrthoGrid) == 0xB0 &&
						offsetof(BGSRenderWindow, _Border) == 0xB8 &&
						offsetof(BGSRenderWindow, _Camera) == 0xC0 &&
						offsetof(BGSRenderWindow, _Window) == 0xC8 &&
						offsetof(BGSRenderWindow, _PickHandler) == 0xD0 &&
						offsetof(BGSRenderWindow, _CullingProcess) == 0xD8 &&
						offsetof(BGSRenderWindow, _PortalGraphEntry) == 0xE0 &&
						offsetof(BGSRenderWindow, _SceneContext) == 0xF0 &&
						offsetof(BGSRenderWindow, _CurrentCell) == 0xF8 &&
						offsetof(BGSRenderWindow, _WindowSize) == 0x104 &&
						offsetof(BGSRenderWindow, _MousePos) == 0x10C &&
						offsetof(BGSRenderWindow, _State) == 0x11C &&
						offsetof(BGSRenderWindow, _Flags) == 0x138 &&
						offsetof(BGSRenderWindow, _AverageFrameTime) == 0x13C &&
						offsetof(BGSRenderWindow, _DeferredObjects) == 0x148 &&
						offsetof(BGSRenderWindow, _QueuedCell) == 0x160 &&
						offsetof(BGSRenderWindow, _QueuedPosition) == 0x168 &&
						offsetof(BGSRenderWindow, _QueuedRotation) == 0x174;
				}
			private:
				/* 0x008 */ BSTEventDispatcher<RefSelectDeselectEvent> _SelectionEvents;
				/* 0x060 */ BSTEventSink<TESObjectLoadedEvent> _LoadedEventSink;
				/* 0x068 */ BSTEventSink<CellAttachDetachEvent> _CellEventSink;
				/* 0x070 */ HWND _WindowHandle;
				/* 0x078 */ HCURSOR _Cursors[6];
				/* 0x0A8 */ BGSRenderWindowEditModuleManager* _EditModuleManager;
				/* 0x0B0 */ BGSRenderOrthoGrid* _OrthoGrid;
				/* 0x0B8 */ BGSRenderWindowBorder* _Border;
				/* 0x0C0 */ BGSRenderWindowCamera* _Camera;
				/* 0x0C8 */ NiWindow* _Window;
				/* 0x0D0 */ PickHandlerT* _PickHandler;
				/* 0x0D8 */ BSCullingProcess* _CullingProcess;
				/* 0x0E0 */ BSPortalGraphEntry* _PortalGraphEntry;
				/* 0x0E8 */ NiAPI::NiRefObject* _ObjectE8;
				/* 0x0F0 */ void* _SceneContext;
				/* 0x0F8 */ Forms::TESObjectCELL* _CurrentCell;
				/* 0x100 */ char pad100[0x04];
				/* 0x104 */ SIZE _WindowSize;
				/* 0x10C */ POINT _MousePos[2];
				/* 0x11C */ State _State;
				/* 0x120 */ HMENU _ContextMenu;
				/* 0x128 */ NiAPI::NiRefObject* _Object128;
				/* 0x130 */ float _SceneMemory;
				/* 0x134 */ float _SelectionMemory;
				/* 0x138 */ std::uint32_t _Flags;
				/* 0x13C */ float _AverageFrameTime;
				/* 0x140 */ std::uint32_t _FrameTimeSampleCount;
				/* 0x144 */ bool _DeferredCommand;
				/* 0x145 */ char pad145[0x3];
				/* 0x148 */ BSTArray<void*> _DeferredObjects;
				/* 0x160 */ Forms::TESObjectCELL* _QueuedCell;
				/* 0x168 */ NiAPI::NiPoint3 _QueuedPosition;
				/* 0x174 */ NiAPI::NiPoint3 _QueuedRotation;
			};
			static_assert(sizeof(BGSRenderWindow) == 0x180);
			static_assert(BGSRenderWindow::VerifyLayout());
		}
	}
}