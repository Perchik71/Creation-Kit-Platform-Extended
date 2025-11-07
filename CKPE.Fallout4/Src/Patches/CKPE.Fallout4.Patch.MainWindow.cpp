// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Utils.h>
#include <CKPE.FileUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.Common.FormInfoOutputWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.UIMenus.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/TES.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <EditorAPI/BSPointerHandleManager.h>
#include <Patches/CKPE.Fallout4.Patch.ObjectWindow.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>
#include <Patches/CKPE.Fallout4.Patch.RenderWindow.h>
#include <Patches/CKPE.Fallout4.Patch.CellViewWindow.h>
#include <Patches/CKPE.Fallout4.Patch.MainWindow.h>
#include "..\CKPE.Common\resource.h"

#include <commdlg.h>
#include <commctrl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_MainWindow_sub1 = 0;
			std::uintptr_t pointer_MainWindow_sub2 = 0;
			std::uintptr_t pointer_MainWindow_sub3 = 0;

			extern std::uint32_t* pointer_ReplaceBSPointerHandleAndManager_data1;
			extern std::uint32_t* pointer_ReplaceBSPointerHandleAndManager_data2;

			struct VersionControlListItem
			{
				char* EditorId;
				std::uint32_t FileOffset;
				char Type[4];
				std::uint32_t FileLength;
				char GroupType[4];
				std::uint32_t FormId;
				std::uint32_t VersionControlId;
				char _pad0[0x8];
			};
			static_assert(sizeof(VersionControlListItem) == 0x28);
			std::vector<VersionControlListItem> formList;

			static HMENU ExtensionMenuHandle = nullptr;
			static HMENU ExtensionMenuHideFunctionsHandle = nullptr;
			static char newMainWindowClass[250];

			static void OutputRefrAdditionalInfo(EditorAPI::Forms::TESObjectREFR* Refr)
			{
				auto Extras = Refr->GetExtraData();
				if (Extras)
					Extras->Dump([](const char* fmt, ...) {
						va_list ap;
						va_start(ap, fmt);
						_CONSOLEVA(fmt, ap);
						va_end(ap);
						});
			}

			static void OutputFormInfo(std::uint32_t FormID) noexcept(true)
			{
				if (FormID > 0)
				{
					__try
					{
						auto form = EditorAPI::Forms::TESForm::FindFormByFormID(FormID);
						if (form)
						{
							char szBuf[140] = { 0 };
							form->DebugInfo(szBuf, 140);

							_CONSOLE("DebugInfo -> %s type %02X ptr %p",
								szBuf, (std::uint16_t)form->Type, form);

							if (form->Type == EditorAPI::Forms::TESObjectREFR::TYPE_ID)
								OutputRefrAdditionalInfo((EditorAPI::Forms::TESObjectREFR*)form);
						}
					}
					__except (EXCEPTION_EXECUTE_HANDLER)
					{
						_CONSOLE("DebugInfo -> Incorrect FormID");
					}
				}
			}

			static void ShowDialogEdit(HWND Hwnd, LPARAM lParam)
			{
				__try
				{
					auto form = EditorAPI::Forms::TESForm::FindFormByFormID(static_cast<std::uint32_t>(lParam));
					if (form)
						form->EditFormToWindow(Hwnd, false, true, 0);
				}
				__except (1)
				{
					// skip fatal error
				}
			}

			static void CreateExtensionMenu(HWND _MainWindow, HMENU _MainMenu) noexcept(true)
			{
				// Creating a submenu to open the hidden functions of the Creation Kit
				ExtensionMenuHideFunctionsHandle = CreateMenu();
				Common::UI::CUIMenu ExtMenuHideFunctions = ExtensionMenuHideFunctionsHandle;
				ExtMenuHideFunctions.Append("Dialogue", MainWindow::UI_EXTMENU_IMPORT_DIALOGUE);
				ExtMenuHideFunctions.Append("Quest Stages", MainWindow::UI_EXTMENU_IMPORT_QUESTSTAGES);
				ExtMenuHideFunctions.Append("Quest Objectives", MainWindow::UI_EXTMENU_IMPORT_QUESTOBJECTIVES);
				ExtMenuHideFunctions.Append("Names", MainWindow::UI_EXTMENU_IMPORT_NAMES);
				ExtMenuHideFunctions.Append("Topics", MainWindow::UI_EXTMENU_IMPORT_TOPICS);
				ExtMenuHideFunctions.Append("Script Messageboxes", MainWindow::UI_EXTMENU_IMPORT_SCRIPTSMSGBOX);
				ExtMenuHideFunctions.Append("Game Settings", MainWindow::UI_EXTMENU_IMPORT_GAMESETTINGS);
				ExtMenuHideFunctions.Append("Descriptions", MainWindow::UI_EXTMENU_IMPORT_DESCRIPTIONS);
				ExtMenuHideFunctions.Append("Faction Rank Names", MainWindow::UI_EXTMENU_IMPORT_FACTIONRANKNAMES);
				ExtMenuHideFunctions.Append("Ammo", MainWindow::UI_EXTMENU_IMPORT_AMMO);
				ExtMenuHideFunctions.Append("Body Part Data", MainWindow::UI_EXTMENU_IMPORT_BODYPARTDATA);

				// Create extended menu options
				ExtensionMenuHandle = CreateMenu();

				Common::UI::CUIMenu ExtMenu = ExtensionMenuHandle;
				ExtMenu.Append("Show Log", MainWindow::UI_EXTMENU_SHOWLOG);
				ExtMenu.Append("Clear Log", MainWindow::UI_EXTMENU_CLEARLOG);
				ExtMenu.Append("Autoscroll Log", MainWindow::UI_EXTMENU_AUTOSCROLL, true, true);
				ExtMenu.AppendSeparator();
				ExtMenu.Append("Import", MainWindow::UI_EXTMENU_IMPORT, ExtMenuHideFunctions);
				ExtMenu.AppendSeparator();
				ExtMenu.Append("Dump RTTI Data", MainWindow::UI_EXTMENU_DUMPRTTI);
				ExtMenu.Append("Dump SDM Info", MainWindow::UI_EXTMENU_SDM);
				ExtMenu.Append("Form Info Output", MainWindow::UI_EXTMENU_FORMINFOOUTPUT);
				ExtMenu.Append("Toggle Anti-aliasing", MainWindow::UI_EXTMENU_TOGGLE_ANTIALIASING, true, true);

#if CKPE_USES_TRACER
				// Create tracer menu
				auto TracerMenuHandle = CreateMenu();
				Classes::CUIMenu TracerMenu = TracerMenuHandle;

				TracerMenu.Append("Record", UI_EXTMENU_TRACER_RECORD);
				TracerMenu.Append("Clear", UI_EXTMENU_TRACER_CLEAR);
				TracerMenu.Append("Dump", UI_EXTMENU_TRACER_DUMP);
				ExtMenu.Append("Tracer Memory", UI_EXTMENU_TRACER, TracerMenu);
#endif

				ExtMenu.AppendSeparator();
				ExtMenu.Append("Save Hardcoded Forms", MainWindow::UI_EXTMENU_HARDCODEDFORMS);

				MENUITEMINFO menuInfo
				{
					.cbSize = sizeof(MENUITEMINFO),
					.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_STRING,
					.wID = MainWindow::UI_EXTMENU_ID,
					.hSubMenu = ExtensionMenuHandle,
					.dwTypeData = const_cast<LPSTR>("Extensions"),
					.cch = static_cast<std::uint32_t>(strlen(menuInfo.dwTypeData))
				};

				CKPE_ASSERT_MSG(InsertMenuItem(_MainMenu, -1, TRUE, &menuInfo), "Failed to create extension submenu");

				//// Create plug-ins menu options
				//auto Plugins = GlobalEnginePtr->GetUserPluginsManager();
				//Plugins->CreatePluginsMenu(MainMenu, UI_EXTMENU_ID + 1);

				auto ver = FileUtils::GetFileVersion("CKPE.Fallout4.dll");
				auto customTitle = std::make_unique<char[]>(100);
				sprintf(customTitle.get(), "[CKPE: v%u.%u build %u]",
					GET_EXE_VERSION_EX_MAJOR(ver), GET_EXE_VERSION_EX_MINOR(ver), GET_EXE_VERSION_EX_BUILD(ver));		
				ZeroMemory(&menuInfo, sizeof(MENUITEMINFO));
				menuInfo.cbSize = sizeof(MENUITEMINFO),
					menuInfo.fMask = MIIM_STRING | MIIM_ID;
				menuInfo.wID = 40016;
				menuInfo.dwTypeData = LPSTR(customTitle.get());
				menuInfo.cch = static_cast<std::uint32_t>(strlen(menuInfo.dwTypeData));
				CKPE_ASSERT_MSG(InsertMenuItem(_MainMenu, -1, TRUE, &menuInfo), "Failed to create version menuitem");
			}

			void MainWindow::DoOpenFormByIdHandler(std::uint32_t id) noexcept(true)
			{
				MainWindow::Singleton->Perform(WM_COMMAND, Common::EditorUI::UI_EDITOR_OPENFORMBYID, id);
			}

			void MainWindow::FogToggling() noexcept(true)
			{
				fast_call<void>(pointer_MainWindow_sub2);

				auto Toggle = EditorAPI::Sky::Setting_FogEnabled->GetBool();
				auto MenuItem = MainWindow::Singleton->MainMenu.GetItem(Common::EditorUI::UI_EDITOR_TOGGLEFOG);

				MenuItem.Checked = Toggle;
			}

			void MainWindow::MarkerToggling() noexcept(true)
			{
				auto Toggle = EditorAPI::TES::Setting_ShowMarkers->GetBool();
				EditorAPI::TES::Setting_ShowMarkers->SetBool(!Toggle);

				fast_call<void>(pointer_MainWindow_sub3);

				auto MenuItem = MainWindow::Singleton->MainMenu.GetItem(Common::EditorUI::UI_EDITOR_TOGGLEMAKERS);
				MenuItem.Checked = !Toggle;
			}

			MainWindow::MainWindow() : PatchMainWindow()
			{
				SetName("Main Window");
				Singleton = this;
				sprintf_s(newMainWindowClass, 250, "Creation Kit %s",
					StringUtils::Utf16ToWinCP(VersionLists::GetEditorVersionByString()).c_str());
			}

			bool MainWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* MainWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool MainWindow::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> MainWindow::GetDependencies() const noexcept(true)
			{
				return { "TES", "Console", "Object Window", "TESForm" };
			}

			bool MainWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool MainWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);
				
				pointer_MainWindow_sub1 = __CKPE_OFFSET(1);
				pointer_MainWindow_sub2 = Detours::DetourClassJump(__CKPE_OFFSET(2), (std::uintptr_t)&FogToggling);
				
				// only erase it first
				auto rva = __CKPE_OFFSET(3);
				SafeWrite::WriteNop(rva + 0x4, 7);
				pointer_MainWindow_sub3 = Detours::DetourClassJump(rva, (std::uintptr_t)&MarkerToggling);
				SafeWrite::WriteNop(rva + 0x12, 7);

				Common::LogWindow::GetSingleton()->OnOpenFormById = DoOpenFormByIdHandler;

				return true;
			}

			INT_PTR CALLBACK MainWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
				case WM_CREATE:
				{
					auto createInfo = reinterpret_cast<const CREATESTRUCT*>(lParam);

					if (!strcmp(createInfo->lpszName, newMainWindowClass) && 
						!strcmp(createInfo->lpszClass, newMainWindowClass))
					{
						// Initialize the original window before adding anything
						LRESULT status = CallWindowProc(MainWindow::Singleton->GetOldWndProc(), 
							Hwnd, Message, wParam, lParam);
						MainWindow::Singleton->m_hWnd = Hwnd;

						// Grass is always enabled by default, make the UI buttons match
						CheckMenuItem(GetMenu(Hwnd), Common::EditorUI::UI_EDITOR_TOGGLEGRASS, MF_CHECKED);
						SendMessageA(GetDlgItem(Hwnd, Common::EditorUI::UI_EDITOR_TOOLBAR), TB_CHECKBUTTON,
							Common::EditorUI::UI_EDITOR_TOGGLEGRASS_BUTTON, TRUE);

						// Same for fog
						CheckMenuItem(GetMenu(Hwnd), Common::EditorUI::UI_EDITOR_TOGGLEFOG,
							EditorAPI::Sky::Setting_FogEnabled->GetBool() ? MF_CHECKED : MF_UNCHECKED);

						// Create custom menu controls
						MainWindow::Singleton->MainMenu = createInfo->hMenu;
						CreateExtensionMenu(Hwnd, createInfo->hMenu);

						// All main menus change to uppercase letters
						for (UINT i = 0; i < MainWindow::Singleton->MainMenu.Count(); i++)
						{
							auto MenuItem = MainWindow::Singleton->MainMenu.GetItemByPos(i);
							MenuItem.SetText(EditorAPI::BSString(MenuItem.GetText().c_str()).UpperCase().c_str());
						}

						auto ViewMenu = MainWindow::Singleton->MainMenu.GetSubMenuItem(2);

						// How annoying is this window Warnings, delete from the menu.
						ViewMenu.Remove(0xA04D);
						// Deprecated
						ViewMenu.Remove(0x122);		// Leaves
						ViewMenu.Remove(0x9C60);	// Isometric

						// Fix show/hide object window
						auto MenuItem = ViewMenu.GetItemByPos(2);
						MenuItem.Checked = TRUE;

						// Fix display text hotkey toggle sound marker
						ViewMenu.GetItem(40677).SetText("Sound Marker\tCtrl-N");

						return status;
					}
				}
				break;
				default:
					if (MainWindow::Singleton->Handle == Hwnd)
					{
						switch (Message)
						{
						case WM_NOTIFY:
							if (((LPNMHDR)lParam)->code == TTN_GETDISPINFO)
							{
								LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
								if (lpttt->hdr.idFrom == UI_EXTMENU_TOGGLE_ANTIALIASING)
								{
									lpttt->hinst = (HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL();
									lpttt->lpszText = MAKEINTRESOURCE(IDST_ANTIALIASING_TIPS);
									return TRUE;
								}
							}
							break;
						case WM_SIZE:
						{
							// Scale the status bar segments to fit the window size
							auto scale = [&](int Width)
								{
									return static_cast<int>(Width * (LOWORD(lParam) / 1024.0f));
								};

							std::array<int, 4> spacing
							{
								scale(150),	// 150
								scale(300),	// 225
								scale(600),	// 500
								-1,			// -1
							};

							SendMessageA(GetDlgItem(Hwnd, Common::EditorUI::UI_EDITOR_STATUSBAR), SB_SETPARTS,
								spacing.size(), reinterpret_cast<LPARAM>(spacing.data()));
						}
						break;
						case WM_COMMAND:
						{
							const std::uint32_t param = LOWORD(wParam);
							switch (param)
							{
							case Common::EditorUI::UI_EDITOR_TOGGLEFOG:
							{
								FogToggling();
							}
							return 0;
							case Common::EditorUI::UI_EDITOR_TOGGLEMAKERS:
							{
								MarkerToggling();
							}
							return 0;
							case Common::EditorUI::UI_EDITOR_TOGGLECELLVIEW:
							{
								auto wnd = CellViewWindow::Singleton.GetSingleton();
								wnd->Visible = !wnd->Visible;
								auto MenuItem =
									MainWindow::Singleton->MainMenu.GetItem(Common::EditorUI::UI_EDITOR_TOGGLECELLVIEW);
								MenuItem.Checked = !MenuItem.Checked;
							}
							return 0;
							case Common::EditorUI::UI_EDITOR_OPENFORMBYID:
							{
								ShowDialogEdit(Hwnd, lParam);
							}
							return 0;
							case UI_EXTMENU_SHOWLOG:
							{
								Common::LogWindow::GetSingleton()->Show(true);
							}
							return 0;
							case UI_EXTMENU_CLEARLOG:
							{
								Common::LogWindow::GetSingleton()->Clear();
							}
							return 0;
							case UI_EXTMENU_FORMINFOOUTPUT:
							{
								Common::FormInfoOutputWindow Wnd;
								OutputFormInfo((std::uint32_t)Wnd.OpenModal(Hwnd));
							}
							return 0;
							case Common::EditorUI::UI_EDITOR_TOGGLEOBJECTWND:
							{
								for (auto& Wnd : ObjectWindows) 
								{
									Wnd.second->ObjectWindow.Visible = !Wnd.second->ObjectWindow.Visible;
									if (Wnd.second->ObjectWindow.Visible)
										Wnd.second->ObjectWindow.Foreground();
								}

								// Change the checkbox
								auto MenuItem =
									MainWindow::Singleton->MainMenu.GetItem(Common::EditorUI::UI_EDITOR_TOGGLEOBJECTWND);
								MenuItem.Checked = !MenuItem.Checked;
							}
							return 0;
							case UI_EXTMENU_AUTOSCROLL:
							{
								MENUITEMINFO info
								{
									.cbSize = sizeof(MENUITEMINFO),
									.fMask = MIIM_STATE
								};

								GetMenuItemInfo(ExtensionMenuHandle, param, FALSE, &info);

								bool doCheck = !((info.fState & MFS_CHECKED) == MFS_CHECKED);

								if (doCheck)
									info.fState |= MFS_CHECKED;
								else
									info.fState &= ~MFS_CHECKED;

								Common::LogWindow::GetSingleton()->SetAutoScroll(doCheck);
								SetMenuItemInfo(ExtensionMenuHandle, param, FALSE, &info);
							}
							return 0;
							case UI_EXTMENU_DUMPRTTI:
							{
								wchar_t filePath[MAX_PATH] = {};
								OPENFILENAMEW ofnData
								{
									.lStructSize = sizeof(OPENFILENAME),
									.lpstrFilter = L"Text Files (*.txt)\0*.txt\0\0",
									.lpstrFile = filePath,
									.nMaxFile = ARRAYSIZE(filePath),
									.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
									.lpstrDefExt = L"txt"
								};

								if (GetSaveFileNameW(&ofnData))
									Common::RTTI::GetSingleton()->Dump(ofnData.lpstrFile);
							}
							return 0;
							case UI_EXTMENU_SDM:
							{
								bool ExtremlyMode = EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId;

								if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_982_3)
								{
									if (ExtremlyMode == 1)
									{
										auto head = *pointer_ReplaceBSPointerHandleAndManager_data2;
										auto tail = *pointer_ReplaceBSPointerHandleAndManager_data1;

										Console::LogWarning(Console::SYSTEM,
											"Dump SDM Info:\n\tHead: 0x%08X\n\tTail: 0x%08X\n\tMax: 0x%08X\n\tCapacity: %.2f%%",
											head, tail, EditorAPI::BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT,
											((((long double)head) * 100.0f) /
												(long double)EditorAPI::BSUntypedPointerHandle_Extended_NG::MAX_HANDLE_COUNT));
									}
									else
									{
										auto head = *pointer_ReplaceBSPointerHandleAndManager_data2;
										auto tail = *pointer_ReplaceBSPointerHandleAndManager_data1;

										Console::LogWarning(Console::SYSTEM,
											"Dump SDM Info:\n\tHead: 0x%08X\n\tTail: 0x%08X\n\tMax: 0x%08X\n\tCapacity: %.2f%%",
											head, tail, EditorAPI::BSUntypedPointerHandle_Original::MAX_HANDLE_COUNT,
											((((long double)head) * 100.0f) /
												(long double)EditorAPI::BSUntypedPointerHandle_Original::MAX_HANDLE_COUNT));
									}
								}
								else
								{
									if (ExtremlyMode == 1)
									{
										auto head = EditorAPI::BSPointerHandleManager_Extended::GetHead();
										auto tail = EditorAPI::BSPointerHandleManager_Extended::GetTail();

										Console::LogWarning(Console::SYSTEM,
											"Dump SDM Info:\n\tHead: 0x%08X\n\tTail: 0x%08X\n\tMax: 0x%08X\n\tCapacity: %.2f%%",
											head, tail, EditorAPI::BSUntypedPointerHandle_Extended::MAX_HANDLE_COUNT,
											((((long double)head) * 100.0f) /
												(long double)EditorAPI::BSUntypedPointerHandle_Extended::MAX_HANDLE_COUNT));
									}
									else
									{
										auto head = EditorAPI::BSPointerHandleManager_Original::GetHead();
										auto tail = EditorAPI::BSPointerHandleManager_Original::GetTail();

										Console::LogWarning(Console::SYSTEM,
											"Dump SDM Info:\n\tHead: 0x%08X\n\tTail: 0x%08X\n\tMax: 0x%08X\n\tCapacity: %.2f%%",
											head, tail, EditorAPI::BSUntypedPointerHandle_Original::MAX_HANDLE_COUNT,
											((((long double)head) * 100.0f) /
												(long double)EditorAPI::BSUntypedPointerHandle_Original::MAX_HANDLE_COUNT));
									}
								}
							}
							return 0;
							case UI_EXTMENU_HARDCODEDFORMS:
							{
								for (std::uint32_t i = 0; i < 4096; i++)
								{
									auto form = EditorAPI::Forms::TESForm::FindFormByFormID(i);
									if (form)
									{
										form->MarkAsChanged(true);
										Console::LogWarning(Console::FORMS, "SetFormModified(%08X)", i);
									}
								}

								// Fake the click on "Save"
								PostMessageA(Hwnd, WM_COMMAND, 40127, 0);
							}
							return 0;
							case UI_EXTMENU_TOGGLE_ANTIALIASING:
							{
								MENUITEMINFO info
								{
									.cbSize = sizeof(MENUITEMINFO),
									.fMask = MIIM_STATE
								};

								GetMenuItemInfo(ExtensionMenuHandle, param, FALSE, &info);

								bool doCheck = !((info.fState & MFS_CHECKED) == MFS_CHECKED);
								if (RenderWindow::Singleton->GetImagespaceAA())
								{
									RenderWindow::Singleton->SetAntiAliasingEnabled(doCheck);

									if (doCheck)
										info.fState |= MFS_CHECKED;
									else
										info.fState &= ~MFS_CHECKED;

									SetMenuItemInfo(ExtensionMenuHandle, param, FALSE, &info);
									SendMessageA(MainWindow::Singleton->Toolbar.Handle, TB_CHECKBUTTON,
										UI_EXTMENU_TOGGLE_ANTIALIASING, MAKELPARAM(doCheck, 0));
								}
								else
									SendMessageA(MainWindow::Singleton->Toolbar.Handle, TB_CHECKBUTTON,
										UI_EXTMENU_TOGGLE_ANTIALIASING, MAKELPARAM(TRUE, 0));
							}
							return 0;
							/*default:
							{
								bool Continue = true;
								auto Plugins = GlobalEnginePtr->GetUserPluginsManager();
								auto Ret = Plugins->ProcessingMenuMessages(Hwnd, Message, wParam, lParam, Continue);
								if (!Continue) return Ret;
								return CallWindowProc(MainWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
							}*/
							}
						}
						break;
						case WM_SHOWWINDOW:
						{
							// Getting additional child Windows		
							MainWindow::Singleton->FindToolWindow();
						}
						break;
						case WM_GETMINMAXINFO:
						{
							// https://social.msdn.microsoft.com/Forums/vstudio/en-US/fb4de52d-66d4-44da-907c-0357d6ba894c/swmaximize-is-same-as-fullscreen?forum=vcgeneral

							RECT WorkArea{};
							SystemParametersInfoA(SPI_GETWORKAREA, 0, &WorkArea, 0);
							((MINMAXINFO*)lParam)->ptMaxSize.x = (WorkArea.right - WorkArea.left);
							((MINMAXINFO*)lParam)->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
							((MINMAXINFO*)lParam)->ptMaxPosition.x = WorkArea.left;
							((MINMAXINFO*)lParam)->ptMaxPosition.y = WorkArea.top;
						}
						break;
						}
					}
				}

				return CallWindowProc(MainWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}