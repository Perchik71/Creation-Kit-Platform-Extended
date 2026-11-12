// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.MainWindow.h>
#include <Patches/CKPE.SkyrimSE.Patch.ShowHideWindow.h>
#include <EditorAPI/B/BSSettings.h>
#include "resource.h"

#include <commdlg.h>
#include <commctrl.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t ShowHideRefreshChecksOrig;

			class ToolbarShowHide :
				public ISingleton<ToolbarShowHide>
			{
				HWND hWnd{ nullptr };
				HWND hParentWnd{ nullptr };
				ImageList imageList{};
			public:
				enum class Command
				{
					kMarkers = 0x2000,//0x9C5E,
					kLightMarkers,//				= 0x9EB8,
					kSoundMarkers,//				= 0x9EE5,
					kMultiboundMarkers,
					kOcclusionMarkers,//			= 0x9EB5,
					kPortalAndRoomMarkers,
					kCollisionMarkers,//			= 0x9CFD,//
					kTerrain,
					kWater,
					kStatics,
					kActors,
					kDynamics,
					kDecals
				}; // lights radius 0x9C5F, tree 0x9E20, Grass 0xA003

				ToolbarShowHide(HWND a_parentWnd) :
					ISingleton<ToolbarShowHide>(this)
				{
					if (!a_parentWnd)
						return;

					auto inst = GetModuleHandleA("CKPE.SkyrimSE.dll");

					(void)imageList.InitFromResource((THandle)inst, IDB_TOOLBAR_SHOWHIDE, L"PNG");

					// Инициализация общих элементов управления
					INITCOMMONCONTROLSEX icex{};
					icex.dwSize = sizeof(icex);
					icex.dwICC = ICC_BAR_CLASSES;
					InitCommonControlsEx(&icex);

					// Создание панели инструментов
					hWnd = CreateWindowExA(
						0,
						TOOLBARCLASSNAMEA,
						nullptr,
						// стили (TBSTYLE_WRAPABLE для правой стороны)
						WS_CHILD | CCS_RIGHT | CCS_ADJUSTABLE | CCS_NORESIZE | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE,
						0, 0, 0, 0,
						a_parentWnd,
						nullptr,
						GetModuleHandleA(nullptr), nullptr);

					if (!hWnd)
						return;

					hParentWnd = a_parentWnd;

					// Добавление кнопок
					static std::array<TBBUTTON, 15> tbButtons
					{ {
						{ -1, -1,																TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_SEP,	{}, 0L, 0 },
						{ MAKELONG(0, 0),  std::to_underlying(Command::kMarkers),				TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 1),  std::to_underlying(Command::kLightMarkers),			TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 2),  std::to_underlying(Command::kSoundMarkers),			TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 3),  std::to_underlying(Command::kMultiboundMarkers),		TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 4),  std::to_underlying(Command::kOcclusionMarkers),		TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 5),  std::to_underlying(Command::kPortalAndRoomMarkers),	TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 6),  std::to_underlying(Command::kCollisionMarkers),		TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ -1, -1,																TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_SEP,	{}, 0L, 0 },
						{ MAKELONG(0, 7),  std::to_underlying(Command::kTerrain),				TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 8),  std::to_underlying(Command::kWater),					TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 9),  std::to_underlying(Command::kStatics),				TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 10), std::to_underlying(Command::kActors),				TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 11), std::to_underlying(Command::kDynamics),				TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
						{ MAKELONG(0, 12), std::to_underlying(Command::kDecals),				TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_CHECK,	{}, 0L, 0 },
					} };

					// Создаём список изображений (иконок)
					SendMessageA(hWnd, TB_SETIMAGELIST, (WPARAM)0, (LPARAM)imageList.Handle);
					SendMessageA(hWnd, TB_LOADIMAGES, (WPARAM)TRUE, (LPARAM)imageList.Handle);

					// Добавление кнопок
					SendMessageA(hWnd, TB_BUTTONSTRUCTSIZE, static_cast<WPARAM>(sizeof(TBBUTTON)), 0);
					SendMessageA(hWnd, TB_ADDBUTTONS, static_cast<WPARAM>(tbButtons.size()), reinterpret_cast<LPARAM>(tbButtons.data()));
				}

				~ToolbarShowHide()
				{
					if (hWnd)
					{
						DestroyWindow(hWnd);
						hWnd = nullptr;
					}

					hParentWnd = nullptr;
				}

				void Check(Command a_cmd, bool a_check) const noexcept(true)
				{
					auto oldState = SendMessageA(hWnd, TB_GETSTATE, (WPARAM)std::to_underlying(a_cmd), (LPARAM)0);

					if (a_check)
						oldState |= TBSTATE_CHECKED;
					else
						oldState &= ~TBSTATE_CHECKED;

					SendMessageA(hWnd, TB_SETSTATE, (WPARAM)std::to_underlying(a_cmd), (LPARAM)oldState);
				}

				void NewSize(std::int32_t a_w, std::int32_t a_h) const noexcept(true)
				{
					constexpr std::uint32_t maxWidth = 40;
					auto margin_y = MainWindow::Singleton->Toolbar.Height;
					auto margin_b = MainWindow::Singleton->Statusbar.Height;

					// Вычисляем позицию справа:
					int x = a_w - maxWidth;
					if (x < 0) x = 0;

					SetWindowPos(hWnd, nullptr, x, margin_y - 1, maxWidth, a_h - (margin_y + margin_b - 1), SWP_NOACTIVATE);
				}

				void Update() const noexcept(true)
				{
					RECT rc;
					if (GetClientRect(hParentWnd, std::addressof(rc)))
					{
						NewSize(rc.right, rc.bottom);
						ShowWindow(hWnd, TRUE);
					}
				}

				void RefreshChecks() const noexcept(true)
				{
					using namespace EditorAPI::IDs::BGSRenderShowHideDlg;

					auto CheckToolbarBtn = [&](Command a_cmd, EditorAPI::Setting* a_setting)
						{
							if (a_setting)
								Check(a_cmd, a_setting->GetBool());
						};

					CheckToolbarBtn(Command::kMarkers, Common::Relocation<EditorAPI::Setting*>(Settings::ShowMarkers).Get());
					CheckToolbarBtn(Command::kLightMarkers, Common::Relocation<EditorAPI::Setting*>(Settings::ShowLightMarkers).Get());
					CheckToolbarBtn(Command::kSoundMarkers, Common::Relocation<EditorAPI::Setting*>(Settings::ShowSoundMarkers).Get());
					CheckToolbarBtn(Command::kMultiboundMarkers, Common::Relocation<EditorAPI::Setting*>(Settings::ShowMultibounds).Get());
					CheckToolbarBtn(Command::kOcclusionMarkers, Common::Relocation<EditorAPI::Setting*>(Settings::ShowOcclusionPlanes).Get());
					CheckToolbarBtn(Command::kPortalAndRoomMarkers, Common::Relocation<EditorAPI::Setting*>(Settings::ShowPortalsAndRooms).Get());
					CheckToolbarBtn(Command::kCollisionMarkers, Common::Relocation<EditorAPI::Setting*>(Settings::ShowCollisionObjects).Get());
					CheckToolbarBtn(Command::kTerrain, Common::Relocation<EditorAPI::Setting*>(Settings::ShowTerrain).Get());
					CheckToolbarBtn(Command::kWater, Common::Relocation<EditorAPI::Setting*>(Settings::ShowWater).Get());
					CheckToolbarBtn(Command::kStatics, Common::Relocation<EditorAPI::Setting*>(Settings::ShowStatics).Get());
					CheckToolbarBtn(Command::kActors, Common::Relocation<EditorAPI::Setting*>(Settings::ShowActors).Get());
					CheckToolbarBtn(Command::kDynamics, Common::Relocation<EditorAPI::Setting*>(Settings::ShowDynamics).Get());
					CheckToolbarBtn(Command::kDecals, Common::Relocation<EditorAPI::Setting*>(Settings::ShowDecals).Get());

					auto state = Common::Relocation<EditorAPI::Setting*>(Settings::ShowMarkers).Get()->GetBool();
					auto EnabledToolbarBtn = [&](int a_cmd, uint16_t a_idMenu)
						{
							auto oldState = SendMessageA(hWnd, TB_GETSTATE, (WPARAM)a_cmd, (LPARAM)0);
							if (state)
								oldState |= TBSTATE_ENABLED;
							else
								oldState &= ~TBSTATE_ENABLED;

							SendMessageA(hWnd, TB_SETSTATE, (WPARAM)a_cmd, (LPARAM)oldState);

							if (a_idMenu)
							{
								auto hMenu = GetMenu(hParentWnd);
								if (hMenu) EnableMenuItem(hMenu, a_idMenu, state ? MF_ENABLED : MF_GRAYED);
							}
						};

					EnabledToolbarBtn(std::to_underlying(Command::kLightMarkers),			0x9EB8);
					EnabledToolbarBtn(std::to_underlying(Command::kSoundMarkers),			0x9EE5);
					EnabledToolbarBtn(std::to_underlying(Command::kMultiboundMarkers),		0);
					EnabledToolbarBtn(std::to_underlying(Command::kOcclusionMarkers),		0x9EB5);
					EnabledToolbarBtn(std::to_underlying(Command::kPortalAndRoomMarkers),	0);
					EnabledToolbarBtn(std::to_underlying(Command::kCollisionMarkers),		0);
				}

				bool ProcessHandlerFromParentWindow(UINT a_message, WPARAM a_wParam, LPARAM a_lParam, LRESULT& a_result) noexcept(true)
				{
					if (a_message == WM_SIZE)
						NewSize(LOWORD(a_lParam), HIWORD(a_lParam));
					else if (a_message == WM_NOTIFY)
					{
						LPNMHDR pnmh = (LPNMHDR)a_lParam;

						if (pnmh->code == TTN_GETDISPINFO)
						{
							LPNMTTDISPINFOA pttdi = (LPNMTTDISPINFOA)a_lParam;

							static std::array<std::string_view, 13> tbTooltips
							{ {
								"Markers"sv,
								"Light Markers"sv,
								"Sound Markers"sv,
								"Multibounds"sv,
								"Occlusion Planes"sv,
								"Portals and Rooms"sv,
								"Collision Objects"sv,
								"Terrain"sv,
								"Water"sv,
								"Statics"sv,
								"Actors"sv,
								"Dynamics"sv,
								"Decals"sv,
							} };

							constexpr auto minId = std::to_underlying(Command::kMarkers);
							constexpr auto maxId = std::to_underlying(Command::kDecals);

							if ((pnmh->idFrom >= minId) && (pnmh->idFrom <= maxId))
							{
								auto tbId = pnmh->idFrom - minId;
								pttdi->lpszText = const_cast<char*>(tbTooltips[tbId].data());

								a_result = 0;
								return true;
							}
						}
					}
					else if (a_message == WM_COMMAND)
					{
						constexpr auto minId = std::to_underlying(Command::kMarkers);
						constexpr auto maxId = std::to_underlying(Command::kDecals);

						int CmdId = static_cast<int>(a_wParam);
						if ((CmdId >= minId) && (CmdId <= maxId))
						{
							using namespace EditorAPI::IDs::BGSRenderShowHideDlg;

							typedef void(*func_t)();
							struct Data
							{
								EditorAPI::Setting* setting;
								func_t functor;
								uint16_t idMenu;
								uint16_t idDlgItem;
							};

							std::array<Data, 13> datas{{
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowMarkers).Get(),
									nullptr,
									0x9C5E,
									0x3EF
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowLightMarkers).Get(),
									nullptr,
									0x9EB8,
									0x3EE
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowSoundMarkers).Get(),
									nullptr,
									0x9EE5,
									0x3F5
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowMultibounds).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateMultiboundsVisibilty).Get(),
									0,
									0x3FB
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowOcclusionPlanes).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateOcclusionPlanesVisibilty).Get(),
									0x9EB5,
									0x3FD
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowPortalsAndRooms).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdatePortalsAndRoomsVisibilty).Get(),
									0,
									0x3FE
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowCollisionObjects).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateCollisionObjectsVisibilty).Get(),
									0,
									0x13EC
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowTerrain).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateTerrainVisibilty).Get(),
									0,
									0x3F3
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowWater).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateWaterVisibilty).Get(),
									0,
									0x3F4
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowStatics).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateStaticsVisibilty).Get(),
									0,
									0xFC0
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowActors).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateActorsVisibilty).Get(),
									0,
									0x3FA
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowDynamics).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateDynamicVisibilty).Get(),
									0,
									0x3FC
								},
								{
									Common::Relocation<EditorAPI::Setting*>(Settings::ShowDecals).Get(),
									Common::Relocation<func_t>(EditorAPI::IDs::BGSRenderWindow::CellUpdateDecalsVisibilty).Get(),
									0,
									0x3FF
								},
								}};

							auto CheckToolbarBtn = [&](int a_cmd, EditorAPI::Setting* a_setting, uint16_t a_idMenu)
								{
									if (!a_setting) return;

									auto state = a_setting->GetBool();
									Check(static_cast<Command>(a_cmd), state);

									auto hMenu = GetMenu(hParentWnd);
									if (hMenu) CheckMenuItem(hMenu, a_idMenu, state ? MF_CHECKED : MF_UNCHECKED);
								};

							auto& data = datas[static_cast<std::size_t>(CmdId) - minId];
							auto state = !data.setting->GetBool();
							data.setting->SetBool(state);
							CheckToolbarBtn(CmdId, data.setting, data.idMenu);

							if (CmdId == minId)
							{
								auto EnabledToolbarBtn = [&](int a_cmd, uint16_t a_idMenu)
									{
										auto oldState = SendMessageA(hWnd, TB_GETSTATE, (WPARAM)a_cmd, (LPARAM)0);
										if (state)
											oldState |= TBSTATE_ENABLED;
										else
											oldState &= ~TBSTATE_ENABLED;

										SendMessageA(hWnd, TB_SETSTATE, (WPARAM)a_cmd, (LPARAM)oldState);

										if (a_idMenu)
										{
											auto hMenu = GetMenu(hParentWnd);
											if (hMenu) EnableMenuItem(hMenu, a_idMenu, state ? MF_ENABLED : MF_GRAYED);
										}
									};

								EnabledToolbarBtn(std::to_underlying(Command::kLightMarkers),			datas[1].idMenu);
								EnabledToolbarBtn(std::to_underlying(Command::kSoundMarkers),			datas[2].idMenu);
								EnabledToolbarBtn(std::to_underlying(Command::kMultiboundMarkers),		datas[3].idMenu);
								EnabledToolbarBtn(std::to_underlying(Command::kOcclusionMarkers),		datas[4].idMenu);
								EnabledToolbarBtn(std::to_underlying(Command::kPortalAndRoomMarkers),	datas[5].idMenu);
								EnabledToolbarBtn(std::to_underlying(Command::kCollisionMarkers),		datas[6].idMenu);
							}
							else
							{
								Common::Relocation<HWND*> Hwnd{ EditorAPI::IDs::BGSRenderShowHideDlg::Hwnd };
								CheckDlgButton(*Hwnd, datas[static_cast<std::size_t>(CmdId) - minId].idDlgItem, state ? BST_CHECKED : BST_UNCHECKED);
							}

							if (!data.functor)
							{
								typedef void (*func_t)();
								const Common::Relocation<func_t> func{ EditorAPI::IDs::BGSRenderWindow::ToggleMarkersVisibilty };
								func();
							}
							else
							{
								data.functor();

								Common::Relocation<HWND*> Hwnd{ EditorAPI::IDs::BGSRenderShowHideDlg::Hwnd };
								if (ShowHideRefreshChecksOrig && (*Hwnd.Get()))
									fast_call<void>(ShowHideRefreshChecksOrig);
							}
						}
					}

					return false;
				}
			};

			std::unique_ptr<ToolbarShowHide> g_toolbarShowHide{};

			ShowHideWindow::ShowHideWindow() : Common::PatchBaseWindow()
			{
				SetName("ShowHide Window");
				Singleton = this;
			}

			bool ShowHideWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ShowHideWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ShowHideWindow::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> ShowHideWindow::GetDependencies() const noexcept(true)
			{
				return { "Main Window" };
			}

			bool ShowHideWindow::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool ShowHideWindow::IsInitToolbarCKPE() noexcept(true)
			{
				return g_toolbarShowHide != nullptr;
			}

			void ShowHideWindow::UpdateSizesToolbarCKPE() noexcept(true)
			{
				if (g_toolbarShowHide)
					g_toolbarShowHide->Update();
			}

			void ShowHideWindow::InitToolbarCKPE(HWND a_parentHwnd) noexcept(true)
			{
				if (IsWindow(a_parentHwnd))
					g_toolbarShowHide = std::make_unique<ToolbarShowHide>(a_parentHwnd);
			}

			bool ShowHideWindow::ProcessHandlerFromParentWindow(UINT Message, WPARAM wParam, LPARAM lParam, LRESULT &result) noexcept(true)
			{
				if (g_toolbarShowHide)
				{
					if (g_toolbarShowHide->ProcessHandlerFromParentWindow(Message, wParam, lParam, result))
						return true;
				}

				return false;
			}

			bool ShowHideWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ShowHideWindow::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				*(std::uintptr_t*)&_oldWndProc = Relocation(EditorAPI::IDs::BGSRenderShowHideDlg::WndProc).WriteJump(&HKWndProc);

				// Hook refresh checks
				auto relHideShowDlg = Relocation(EditorAPI::IDs::BGSRenderShowHideDlg::RefreshChecks);
				relHideShowDlg.WriteFill<4>(Common::NOP, 0xE);
				ShowHideRefreshChecksOrig = relHideShowDlg.WriteJump(&ShowHideRefreshChecks);

				return true;
			}

			INT_PTR CALLBACK ShowHideWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
				case WM_COMMAND:
				{
					// Fixed markers toggled
					if (wParam == 0x3EF)
					{
						using namespace EditorAPI::IDs::BGSRenderShowHideDlg;
						auto markers = Common::Relocation<EditorAPI::Setting*>(Settings::ShowMarkers).Get();
						markers->SetBool(!markers->GetBool());
					}
					else
					{
						auto result = CallWindowProc(ShowHideWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);

						struct Data
						{
							uint16_t idDlgItem;
							ToolbarShowHide::Command command;

							bool operator==(const Data& a_rhs) const noexcept(true)
							{
								return (idDlgItem == a_rhs.idDlgItem);
							}

							bool operator==(const uint16_t a_rhs) const noexcept(true)
							{
								return (a_rhs == idDlgItem);
							}
						};

						std::array<Data, 13> datas{{
							{ 0x3EF,  ToolbarShowHide::Command::kMarkers },
							{ 0x3EE,  ToolbarShowHide::Command::kLightMarkers },
							{ 0x3F5,  ToolbarShowHide::Command::kSoundMarkers },
							{ 0x3FB,  ToolbarShowHide::Command::kMultiboundMarkers },
							{ 0x3FD,  ToolbarShowHide::Command::kOcclusionMarkers },
							{ 0x3FE,  ToolbarShowHide::Command::kPortalAndRoomMarkers },
							{ 0x13EC, ToolbarShowHide::Command::kCollisionMarkers },
							{ 0x3F3,  ToolbarShowHide::Command::kTerrain },
							{ 0x3F4,  ToolbarShowHide::Command::kWater },
							{ 0xFC0,  ToolbarShowHide::Command::kStatics },
							{ 0x3FA,  ToolbarShowHide::Command::kActors },
							{ 0x3FC,  ToolbarShowHide::Command::kDynamics },
							{ 0x3FF,  ToolbarShowHide::Command::kDecals },
						}};

						auto it = std::find(datas.begin(), datas.end(), static_cast<uint16_t>(wParam));
						if ((it != datas.end()) && g_toolbarShowHide) g_toolbarShowHide->Check(it->command, IsDlgButtonChecked(Hwnd, it->idDlgItem) == BST_CHECKED);

						return result;
					}
				}
				break;
				}

				return CallWindowProc(ShowHideWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void ShowHideWindow::ShowHideRefreshChecks() noexcept(true)
			{
				if (g_toolbarShowHide)
					g_toolbarShowHide->RefreshChecks();

				Common::Relocation<HWND*> Hwnd{ EditorAPI::IDs::BGSRenderShowHideDlg::Hwnd };
				if (ShowHideRefreshChecksOrig && (*Hwnd.Get()))
					fast_call<void>(ShowHideRefreshChecksOrig);
			}
		}
	}
}