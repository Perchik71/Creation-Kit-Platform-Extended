// Copyright � 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/PluginManager.h"
#include "Core/ConsoleWindow.h"
#include "Core/TypeInfo/ms_rtti.h"
#include "Editor API/EditorUI.h"
#include "Editor API/BSString.h"
#include "Editor API/FO4/BSPointerHandleManager.h"
#include "Patches/Windows/FO4/ObjectWindowF4.h"
#include "Patches/Windows/FO4/CellViewWindowF4.h"
#include "Patches/FO4/FixFog.h"
#include "Patches/ConsolePatch.h"
#include "MainWindowF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

			uintptr_t pointer_MainWindow_data = 0;

			struct VersionControlListItem
			{
				char* EditorId;
				uint32_t FileOffset;
				char Type[4];
				uint32_t FileLength;
				char GroupType[4];
				uint32_t FormId;
				uint32_t VersionControlId;
				char _pad0[0x8];
			};
			static_assert(sizeof(VersionControlListItem) == 0x28);
			Array<VersionControlListItem> formList;

			bool GetFileVersion(LPCSTR pszFilePath, EditorAPI::BSString& result)
			{
				DWORD dwSize = 0;
				BYTE* pbVersionInfo = NULL;
				VS_FIXEDFILEINFO* pFileInfo = NULL;
				UINT puLenFileInfo = 0;

				// Get the version information for the file requested
				dwSize = GetFileVersionInfoSize(pszFilePath, NULL);
				if (dwSize == 0)
					return false;

				pbVersionInfo = new BYTE[dwSize];
				if (!pbVersionInfo)
					return false;

				if (!GetFileVersionInfo(pszFilePath, 0, dwSize, pbVersionInfo)) {
					delete[] pbVersionInfo;
					return false;
				}

				if (!VerQueryValue(pbVersionInfo, TEXT("\\"), (LPVOID*)&pFileInfo, &puLenFileInfo)) {
					delete[] pbVersionInfo;
					return false;
				}

				char temp[100];
				sprintf_s(temp, "%d.%d build %d", (pFileInfo->dwFileVersionMS >> 16) & 0xFFFF,
					(pFileInfo->dwFileVersionMS) & 0xFFFF, pFileInfo->dwFileVersionLS);
				result = temp;

				delete[] pbVersionInfo;
				return true;
			}

			char newMainWindowClass[250];
			MainWindow* GlobalMainWindowPtr = nullptr;

			bool MainWindow::HasOption() const
			{
				return false;
			}

			bool MainWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* MainWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* MainWindow::GetName() const
			{
				return "Main Window";
			}

			bool MainWindow::HasDependencies() const
			{
				return true;
			}

			Array<String> MainWindow::GetDependencies() const
			{
				return { "Console", "Object Window", "Fix Fog" };
			}

			bool MainWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool MainWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc = 
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), &MainWindow::HKWndProc);
					
					pointer_MainWindow_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool MainWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			MainWindow::MainWindow() : BaseWindow(), Classes::CUIMainWindow(),
				ExtensionMenuHandle(NULL)
			{
				Assert(!GlobalMainWindowPtr);

				sprintf_s(newMainWindowClass, 250, "Creation Kit %s",
					Core::allowedEditorVersionStr[(int)Core::GlobalEnginePtr->GetEditorVersion()].data());

				GlobalMainWindowPtr = this;
			}

			HWND MainWindow::GetToolbarHandle()
			{
				return *(HWND*)pointer_MainWindow_data;
			}

			void MainWindow::CreateExtensionMenu(HWND MainWindow, HMENU MainMenu)
			{
				// Creating a submenu to open the hidden functions of the Creation Kit
				ExtensionMenuHideFunctionsHandle = CreateMenu();
				Classes::CUIMenu ExtMenuHideFunctions = ExtensionMenuHideFunctionsHandle;
				ExtMenuHideFunctions.Append("Dialogue", UI_EXTMENU_IMPORT_DIALOGUE);
				ExtMenuHideFunctions.Append("Quest Stages", UI_EXTMENU_IMPORT_QUESTSTAGES);
				ExtMenuHideFunctions.Append("Quest Objectives", UI_EXTMENU_IMPORT_QUESTOBJECTIVES);
				ExtMenuHideFunctions.Append("Names", UI_EXTMENU_IMPORT_NAMES);
				ExtMenuHideFunctions.Append("Topics", UI_EXTMENU_IMPORT_TOPICS);
				ExtMenuHideFunctions.Append("Script Messageboxes", UI_EXTMENU_IMPORT_SCRIPTSMSGBOX);
				ExtMenuHideFunctions.Append("Game Settings", UI_EXTMENU_IMPORT_GAMESETTINGS);
				ExtMenuHideFunctions.Append("Descriptions", UI_EXTMENU_IMPORT_DESCRIPTIONS);
				ExtMenuHideFunctions.Append("Faction Rank Names", UI_EXTMENU_IMPORT_FACTIONRANKNAMES);
				ExtMenuHideFunctions.Append("Ammo", UI_EXTMENU_IMPORT_AMMO);
				ExtMenuHideFunctions.Append("Body Part Data", UI_EXTMENU_IMPORT_BODYPARTDATA);

				// Create extended menu options
				ExtensionMenuHandle = CreateMenu();

				Classes::CUIMenu ExtMenu = ExtensionMenuHandle;
				ExtMenu.Append("Show Log", UI_EXTMENU_SHOWLOG);
				ExtMenu.Append("Clear Log", UI_EXTMENU_CLEARLOG);
				ExtMenu.Append("Autoscroll Log", UI_EXTMENU_AUTOSCROLL, true, true);
				ExtMenu.AppendSeparator();
				ExtMenu.Append("Import", UI_EXTMENU_IMPORT, ExtMenuHideFunctions);
				ExtMenu.AppendSeparator();
				ExtMenu.Append("Dump RTTI Data", UI_EXTMENU_DUMPRTTI);
				ExtMenu.Append("Dump SDM Info", UI_EXTMENU_SDM);
				ExtMenu.AppendSeparator();
				ExtMenu.Append("Save Hardcoded Forms", UI_EXTMENU_HARDCODEDFORMS);

				MENUITEMINFO menuInfo
				{
					.cbSize = sizeof(MENUITEMINFO),
					.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_STRING,
					.wID = UI_EXTMENU_ID,
					.hSubMenu = ExtensionMenuHandle,
					.dwTypeData = const_cast<LPSTR>("Extensions"),
					.cch = static_cast<uint32_t>(strlen(menuInfo.dwTypeData))
				};

				AssertMsg(InsertMenuItem(MainMenu, -1, TRUE, &menuInfo), "Failed to create extension submenu");

				// Create plug-ins menu options
				auto Plugins = GlobalEnginePtr->GetUserPluginsManager();
				Plugins->CreatePluginsMenu(MainMenu, UI_EXTMENU_ID + 1);
			}

			LRESULT CALLBACK MainWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				Classes::CUIMenuItem MenuItem;

				switch (Message)
				{
				case WM_CREATE:
				{
					auto createInfo = reinterpret_cast<const CREATESTRUCT*>(lParam);

					if (!strcmp(createInfo->lpszName, newMainWindowClass) && !strcmp(createInfo->lpszClass, newMainWindowClass))
					{
						// Initialize the original window before adding anything
						LRESULT status = CallWindowProc(GlobalMainWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
						GlobalMainWindowPtr->m_hWnd = Hwnd;

						// Grass is always enabled by default, make the UI buttons match
						CheckMenuItem(GetMenu(Hwnd), EditorAPI::EditorUI::UI_EDITOR_TOGGLEGRASS, MF_CHECKED);
						SendMessageA(GetDlgItem(Hwnd, EditorAPI::EditorUI::UI_EDITOR_TOOLBAR), TB_CHECKBUTTON,
							EditorAPI::EditorUI::UI_EDITOR_TOGGLEGRASS_BUTTON, TRUE);

						// Same for fog
						CheckMenuItem(GetMenu(Hwnd), EditorAPI::EditorUI::UI_EDITOR_TOGGLEFOG,
							FixFogPatch::IsFogEnabled() ? MF_CHECKED : MF_UNCHECKED);

						// Create custom menu controls
						GlobalMainWindowPtr->MainMenu = createInfo->hMenu;
						GlobalMainWindowPtr->CreateExtensionMenu(Hwnd, createInfo->hMenu);

						// All main menus change to uppercase letters
						for (UINT i = 0; i < GlobalMainWindowPtr->MainMenu.Count(); i++) 
						{
							MenuItem = GlobalMainWindowPtr->MainMenu.GetItemByPos(i);
							MenuItem.Text = EditorAPI::BSString(MenuItem.Text).UpperCase().c_str();
						}

						Classes::CUIMenu ViewMenu = GlobalMainWindowPtr->MainMenu.GetSubMenuItem(2);

						// How annoying is this window Warnings, delete from the menu.
						ViewMenu.Remove(0xA043);

						// Fix show/hide object window
						MenuItem = ViewMenu.GetItemByPos(2);
						MenuItem.Checked = TRUE;

						// Fix display text hotkey toggle sound marker
						ViewMenu.GetItem(40677).Text = "Sound Marker\tCtrl-N";
						// Deprecated
						ViewMenu.GetItem(290).Enabled = FALSE;
						ViewMenu.GetItem(40032).Enabled = FALSE;
						
						return status;
					}
				}
				break;
				default:
					if (GlobalMainWindowPtr->Handle == Hwnd)
					{
						switch (Message)
						{
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

							SendMessageA(GetDlgItem(Hwnd, EditorAPI::EditorUI::UI_EDITOR_STATUSBAR), SB_SETPARTS,
								spacing.size(), reinterpret_cast<LPARAM>(spacing.data()));
						}
						break;
						case WM_COMMAND:
						{
							const uint32_t param = LOWORD(wParam);
							switch (param)
							{
							case EditorAPI::EditorUI::UI_EDITOR_TOGGLEFOG:
							{
								FixFogPatch::ToggleFog();
							}
							return 0;
							case EditorAPI::EditorUI::UI_EDITOR_TOGGLECELLVIEW:
							{
								GlobalCellViewWindowPtr->Visible = !GlobalCellViewWindowPtr->Visible;
								auto MenuItem = GlobalMainWindowPtr->MainMenu.GetItem(EditorAPI::EditorUI::UI_EDITOR_TOGGLECELLVIEW);
								MenuItem.Checked = !MenuItem.Checked;
							}
							return 0;
							case EditorAPI::EditorUI::UI_EDITOR_OPENFORMBYID:
							{
								auto form = GetFormByNumericID(static_cast<uint32_t>(lParam));
								if (form)
									(*(void(__fastcall**)(TESForm*, HWND, __int64, __int64))(*(__int64*)form + 0x340))
									  (form, Hwnd, 0, 1);
							}
							return 0;
							case UI_EXTMENU_SHOWLOG:
							{
								GlobalConsoleWindowPtr->BringToFront();
							}
							return 0;
							case UI_EXTMENU_CLEARLOG:
							{
								GlobalConsoleWindowPtr->Clear();
							}
							return 0;
							case EditorAPI::EditorUI::UI_EDITOR_TOGGLEOBJECTWND:
							{
								for (auto Wnd : ObjectWindows) {
									Wnd.second->ObjectWindow.Visible = !Wnd.second->ObjectWindow.Visible;
									if (Wnd.second->ObjectWindow.Visible)
										Wnd.second->ObjectWindow.Foreground();
								}

								// Change the checkbox
								MenuItem = GlobalMainWindowPtr->MainMenu.GetItem(EditorAPI::EditorUI::UI_EDITOR_TOGGLEOBJECTWND);
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

								GetMenuItemInfo(GlobalMainWindowPtr->GetExtensionMenuHandle(), param, FALSE, &info);

								bool doCheck = !((info.fState & MFS_CHECKED) == MFS_CHECKED);

								if (doCheck)
									info.fState |= MFS_CHECKED;
								else
									info.fState &= ~MFS_CHECKED;

								GlobalConsoleWindowPtr->SetAutoScroll(doCheck);
								SetMenuItemInfo(GlobalMainWindowPtr->GetExtensionMenuHandle(), param, FALSE, &info);
							}
							return 0;
							case UI_EXTMENU_DUMPRTTI:
							{
								char filePath[MAX_PATH] = {};
								OPENFILENAME ofnData
								{
									.lStructSize = sizeof(OPENFILENAME),
									.lpstrFilter = "Text Files (*.txt)\0*.txt\0\0",
									.lpstrFile = filePath,
									.nMaxFile = ARRAYSIZE(filePath),
									.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
									.lpstrDefExt = "txt"
								};

								if (FILE* f; GetSaveFileName(&ofnData) && fopen_s(&f, filePath, "w") == 0)
								{
									MSRTTI::Dump(f);
									fclose(f);
								}
							}
							return 0;
							case UI_EXTMENU_SDM:
							{
								bool ExtremlyMode = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
								if (ExtremlyMode)
								{
									auto head = EditorAPI::Fallout4::BSPointerHandleManagerInterface_Extended::GetHead();
									auto tail = EditorAPI::Fallout4::BSPointerHandleManagerInterface_Extended::GetTail();

									ConsolePatch::Log("Dump SDM Info:\n\tHead: 0x%08X\n\tTail: 0x%08X\n\tMax: 0x%08X\n\tCapacity: %.2f%%",
										head, tail, EditorAPI::Fallout4::BSUntypedPointerHandle_Extended::MAX_HANDLE_COUNT,
										((((long double)head) * 100.0f) / 
											(long double)EditorAPI::Fallout4::BSUntypedPointerHandle_Extended::MAX_HANDLE_COUNT));
								}
								else
									MessageBoxA(0, "Not implemented for the original version of the manager", "Info", MB_OK | MB_ICONINFORMATION);
							}
							return 0;
							case UI_EXTMENU_HARDCODEDFORMS:
							{
								for (uint32_t i = 0; i < 4096; i++)
								{
									auto form = GetFormByNumericID(i);
									if (form)
									{
										form->MarkAsChanged(TRUE); 
										ConsolePatch::Log("SetFormModified(%08X)", i);
									}
								}

								// Fake the click on "Save"
								PostMessageA(Hwnd, WM_COMMAND, 40127, 0);
							}
							return 0;
							default:
							{
								bool Continue = true;
								auto Plugins = GlobalEnginePtr->GetUserPluginsManager();
								auto Ret = Plugins->ProcessingMenuMessages(Hwnd, Message, wParam, lParam, Continue);
								if (!Continue) return Ret;
								return CallWindowProc(GlobalMainWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
							}
							}
						}
						break;
						case WM_SHOWWINDOW:
						{
							// Getting additional child Windows		
							GlobalMainWindowPtr->FindToolWindow();
						}
						break;
						case WM_GETMINMAXINFO:
						{
							// https://social.msdn.microsoft.com/Forums/vstudio/en-US/fb4de52d-66d4-44da-907c-0357d6ba894c/swmaximize-is-same-as-fullscreen?forum=vcgeneral

							RECT WorkArea;
							SystemParametersInfoA(SPI_GETWORKAREA, 0, &WorkArea, 0);
							((MINMAXINFO*)lParam)->ptMaxSize.x = (WorkArea.right - WorkArea.left);
							((MINMAXINFO*)lParam)->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
							((MINMAXINFO*)lParam)->ptMaxPosition.x = WorkArea.left;
							((MINMAXINFO*)lParam)->ptMaxPosition.y = WorkArea.top;
						}
						break;
						case WM_SETTEXT:
						{
							// Continue normal execution but with a custom string
							char customTitle[1024];
							EditorAPI::BSString versionApp;

							char modulePath[MAX_PATH];
							GetModuleFileNameA((HMODULE)GlobalEnginePtr->GetInstanceDLL(), modulePath, MAX_PATH);
							GetFileVersion(modulePath, versionApp);

							sprintf_s(customTitle, "%s [CKPE: v%s]", (LPCSTR)lParam, versionApp.c_str());

							return CallWindowProc(GlobalMainWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, reinterpret_cast<LPARAM>(customTitle));
						}
						break;
						}
					}
				}

				return CallWindowProc(GlobalMainWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}