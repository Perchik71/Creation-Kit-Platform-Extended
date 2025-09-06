// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
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
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <EditorAPI/TESPointerHandleDetail.h>
#include <Patches/CKPE.Starfield.Patch.AboutWindow.h>
#include <Patches/CKPE.Starfield.Patch.MainWindow.h>
#include "../../CKPE.Common/resource.h"

#include <commdlg.h>
#include <commctrl.h>

#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QtCore/qfile.h>
#include <QtWidgets/qaction.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qfileiconprovider.h>
#include <QtGui/qwindow.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			std::uintptr_t pointer_MainWindow_sub0 = 0;
			std::uintptr_t pointer_MainWindow_sub1 = 0;

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
						form->ShowEditWindow(Hwnd);
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

				auto ver = FileUtils::GetFileVersion("CKPE.SkyrimSE.dll");
				auto customTitle = std::make_unique<char[]>(100);
				sprintf(customTitle.get(), "[CKPE: v%u.%u build %u]",
					GET_EXE_VERSION_EX_MAJOR(ver), GET_EXE_VERSION_EX_MINOR(ver), GET_EXE_VERSION_EX_BUILD(ver));		
				ZeroMemory(&menuInfo, sizeof(MENUITEMINFO));
				menuInfo.cbSize = sizeof(MENUITEMINFO),
					menuInfo.fMask = MIIM_STRING | MIIM_ID;
				menuInfo.wID = 40016;
				menuInfo.dwTypeData = LPSTR(customTitle.get());
				menuInfo.cch = static_cast<uint32_t>(strlen(menuInfo.dwTypeData));
				CKPE_ASSERT_MSG(InsertMenuItem(_MainMenu, -1, TRUE, &menuInfo), "Failed to create version menuitem");
			}

			MainWindow::MainWindow() : Common::PatchBaseWindow()
			{
				SetName("Main Window");
				Singleton = this;
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
				return { "About Window" };
			}

			bool MainWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool MainWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				pointer_MainWindow_sub0 = Detours::DetourClassJump(__CKPE_OFFSET(1), (std::uintptr_t)&HKInitialize);
				pointer_MainWindow_sub1 = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKInitializeActions);

				return true;
			}

			bool CALLBACK MainWindow::HKInitialize(void* MainWindow, const char* ExeFileName, 
				std::uint32_t Unk01) noexcept(true)
			{
				if (fast_call<bool>(pointer_MainWindow_sub0, MainWindow, ExeFileName, Unk01))
				{
					// Inofficially, (HWND)widget->winId(); still works, note that it enforces 
					// the creation of a native window, though.
					MainWindow::Singleton->m_hWnd = (HWND)((QMainWindow*)MainWindow)->windowHandle()->winId();
					return true;
				}

				return false;
			}

			void CALLBACK MainWindow::HKInitializeActions(void* MainWindow) noexcept(true)
			{
				fast_call<void>(pointer_MainWindow_sub1, MainWindow);

				auto mwnd = (QMainWindow*)MainWindow;
				// Replace icon
				mwnd->setWindowIcon(QIcon("/CKPE/ckpe_ico.png"));

				auto menuBar = mwnd->menuBar();
				if (!menuBar) return;

				// Uppercase
				auto menuActionList = menuBar->actions();
				for (auto menuItem : menuActionList)
					menuItem->setText(menuItem->text().toUpper());

				// Creating an CKPE menu's
				auto menuExtensions = menuBar->addMenu("EXTENSIONS");
				if (menuExtensions)
				{
					static auto actionShowLog = new QAction("Show Log", mwnd);
					static auto actionClearLog = new QAction("Clear Log", mwnd);
					static auto actionAutoscrollLog = new QAction("Autoscroll Log", mwnd);
					actionAutoscrollLog->setCheckable(true);
					actionAutoscrollLog->setChecked(true);

					static auto actionDumpRTTIData = new QAction("Dump RTTI Data", mwnd);
					static auto actionDumpSDMInfo = new QAction("Dump SDM Info", mwnd);
					static auto actionFormInfoOutput = new QAction("Form Info Output", mwnd);

					static auto actionSaveHardcodedForms = new QAction("Save Hardcoded Forms", mwnd);

					menuExtensions->addAction(actionShowLog);
					menuExtensions->addAction(actionClearLog);
					menuExtensions->addAction(actionAutoscrollLog);
					menuExtensions->addSeparator();
					menuExtensions->addAction(actionDumpRTTIData);
					menuExtensions->addAction(actionDumpSDMInfo);
					menuExtensions->addAction(actionFormInfoOutput);
					menuExtensions->addSeparator();
					menuExtensions->addAction(actionSaveHardcodedForms);

					auto menuFileActionList = menuActionList.at(0)->menu()->actions();
					static auto actionSaveAs = menuFileActionList.at(1);

					auto actionThemes = menuFileActionList.at(8);
					if (actionThemes->menu())
					{
						// hide action themes
						actionThemes->setVisible(false);

						if (Common::UI::IsDarkTheme())
							// Fake set theme Plastique Dark 
							actionThemes->menu()->actions().at(3)->activate(QAction::Trigger);
						else
							// Fake set theme Default
							actionThemes->menu()->actions().at(0)->activate(QAction::Trigger);
					}

					// remove Warnings
					auto menuViewActionList = menuActionList.at(2)->menu()->actions();
					menuViewActionList.at(31)->setVisible(false);

					mwnd->connect(actionShowLog, &QAction::triggered, mwnd, []() {
						Common::LogWindow::GetSingleton()->Show(true);
						});
					mwnd->connect(actionClearLog, &QAction::triggered, mwnd, []() {
						Common::LogWindow::GetSingleton()->Clear();
						});
					mwnd->connect(actionAutoscrollLog, &QAction::triggered, mwnd, []() {
						bool flag = actionAutoscrollLog->isChecked();
						Common::LogWindow::GetSingleton()->SetAutoScroll(flag);
						actionAutoscrollLog->setChecked(flag);
						});

					mwnd->connect(actionDumpRTTIData, &QAction::triggered, mwnd, []() {
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
						});
					mwnd->connect(actionDumpSDMInfo, &QAction::triggered, mwnd, []() {
						auto handleManager = EditorAPI::HandleManager::Singleton.GetSingleton();

						auto head = handleManager->GetHead();
						auto free = handleManager->GetFree();
						auto tail = handleManager->GetTail();
						auto maxe = handleManager->GetMax();

						_CONSOLE("Dump SDM Info:\n\tHead: 0x%08X\n\tFree: 0x%08X\n\tTail: 0x%08X\n\tMax: 0x%08X\n\tCapacity: %.2f%%",
							head, free, tail, maxe, ((((long double)(maxe - free)) * 100.0f) / (long double)maxe));
						});
					mwnd->connect(actionFormInfoOutput, &QAction::triggered, mwnd, []() {
						Common::FormInfoOutputWindow Wnd;
						OutputFormInfo((std::uint32_t)Wnd.OpenModal(GetActiveWindow()));
						});

					mwnd->connect(actionSaveHardcodedForms, &QAction::triggered, mwnd, []() {
						for (uint32_t i = 0; i < 4096; i++)
						{
							auto form = EditorAPI::Forms::TESForm::FindFormByFormID(i);
							if (form)
							{
								form->MarkAsChanged(TRUE);
								_CONSOLE("SetFormModified(%08X)", i);
							}
						}
						// Fake save
						actionSaveAs->activate(QAction::Trigger);
						});

					/* for "File"
					0	: Data...
					1	: Save Plugin
					2	: < named separator>
					3	: Version Control...
					4	: Jira Credentials
					5	: < named separator>
					6	: Import / Export
					7	: Preferences...
					8	: Theme(experimental)
					9	: Validate Loaded Data
					A	:
					B	: Compact Active File Form IDs for ...
					C	: Convert Active File to ...
					D	:
					E	: Create Archive
					F	: Upload Plugin and Archive to Bethesda.net
					10	:
					11	: Login to Bethesda.net
					12	: Logout
					13	: < named separator>
					14	: Exit
					*/

					auto actionsMenu = menuActionList[0]->menu()->actions();

					// Remove "< named separator>"
					actionsMenu[0x2]->setText("");
					actionsMenu[0x5]->setText("");
					actionsMenu[0x13]->setText("");
					
					// Change icon
					if (Common::UI::IsDarkTheme())
					{
						/*
							0: Undo
							1: Redo
							2:
							3: Cut Render
							4: Copy Render
							5: Paste Render
							6: Paste in Place
							7: Duplicate
							8: Search && Replace...
							9:
							10: Find Text
							11: Find by Condition
							12:
							13: Render Window Hotkeys
							14: Picking Preferences
							0: Toggle Render Features
							1: Refresh Render Window
							2:
							3: Object Windows
							4: Render Windows
							5: Packin
							6:
							7: Toolbar
							8: Cell View
							9: Statusbar
							10: Scene Info Window
							11: Open Windows...
							12: Preview Window
							13: Preview Animations
							14: Show/Hide
							15: Reference Batch Action Window
							16: Layers
							17: Default Layer Manager
							18: Object Window Layouts
							19:
							20: Material Editor
							21: Material Palette
							22: Material Swap Editor
							23: AVMS
							24:
							25: Current Cell Only
							26:
							27: Isometric
							28: Top
							29:
							30: Depth Biasing
							31: Warnings
							32: Inspector
							33: Properties
							34: Particle Editor
							35: Volumes
							36: Reflection Probe
							37:
							0: Save Current Layout
							1: Save Layout as...
							2: Recent Layouts
							3: Manage Layouts...
							4:
							5: Object Windows
							6: Render Windows
							7: Warnings
							8: Show/Hide
							9: Cell View
							10: Galaxy View
							11: Layers
							12: Material Palette
							13: Properties
							14: Reflection Probe
							15: Picking Preferences
							0: Galaxy View
							1: Block Pattern Editor
							2:
							3: Generate Locations
							4: Gen Overlay Placement Data
							5: Generate Biome Placements
							0: World Spaces...
							1: Cells...
							2:
							3:
							4: Go to Location
							5: World Testing And Reports
							6: View Render Test Failures...
							7: View BetaComment Data
							8:
							9: Run Havok Sim
							10: Animate Lighting and Effects
							11:
							12: Create Local Maps
							13: Adjust Exterior Cells...
							14: Validate Room/Portal Alignment
							15: Align Tanget Space at NIF Intersection
							16: Generate LOS
							17:
							18: Generate Max Height Data
							19:
							20: Force Weather
							21: Toggle Wind
							22:
							23: Export MapMarker Data
							0:
							1: Landscape Editing
							2: Landscape Cutting
							3: Object Palette Editing (Qt)
							4: Heightmap Autopaint
							5: Heightmap Import
							6: Export Landscape...
							7:
							8: Reimport All Blocks
							9: Cell Terrain Data Viewer
							0: Generate Current Cell
							1: Generate Loaded Area
							2: Generate WorldSpace
							3:
							4: Cycle View Mode
							5: Draw Cover
							6: Draw Traversal
							7:
							8: Other Options
							9:
							10: Clean NavMesh Splines
							0: Filtered Dialogue...
							1:
							2: Export Relationship Data
							3:
							4: Export Dialogue...
							5: Export Speech Challenge Scenes...
							6: Export Dialogue For Specific Voices
							7: Export Companion Affinity Event
							8: List Neutral Emotion Dialogue
							9: List Dialogue Needing Review
							10: Quest Voice Asset Totals
							11: Update NPC Body Tint Colors
							12: Update FaceBones Model Availability
							13: Export FaceGen Customization Textures
							14:
							15: Edit Player Dialogue
							16: Quest Aliases
							17: Generate Player Address Lip Files
							0: Settings...
							1:
							2: Papyrus Script Manager
							3: Compile Papyrus Scripts...
							4: Migrate Papyrus Property Data...
							5: Export Papyrus Attach Data...
							6:
							7: Animations...
							8: Creature Creator
							9: Facial Animation...
							10: Camera Paths...
							11: Default Objects...
							12: Validate Forms
							13: Recipe Manager
							14: Workshop Editor
							0: Animation Sound Tag Overrides...
							1: Process Local Voice WAVs
							2: Build Soundbank for Active File
							3: Reload Wwise Data
							0: Refresh Session
							1: About
							0: Contents
							1: Snap Windows
							2:
							3: Third-Party Software Licenses
							4:
							5: About
						*/

						// Show actions menu
						/*for (auto it33 : menuActionList)
						{
							int i = 0;
							for (auto it : it33->menu()->actions())
							{
								_CONSOLE("%d: %s", i, it->text().toStdString().c_str());
								i++;
							}
						}*/

						actionsMenu[0]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic2.png"));
						actionsMenu[1]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic3.png"));
						actionsMenu[3]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic1.png"));
						actionsMenu[7]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic4.png"));

						actionsMenu = menuActionList[1]->menu()->actions();

						actionsMenu[0]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic5.png"));
						actionsMenu[1]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic6.png"));

						actionsMenu = menuActionList[5]->menu()->actions();

						actionsMenu[9]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic11.png"));
						actionsMenu[10]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic44.png"));

						actionsMenu = menuActionList[6]->menu()->actions();
						actionsMenu[1]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic10.png"));

						actionsMenu = menuActionList[8]->menu()->actions();
						actionsMenu[0]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic16.png"));
					}
				}

				auto ver = FileUtils::GetFileVersion("CKPE.Starfield.dll");
				auto versionApp = EditorAPI::BSString::FormatString("[CKPE: v%u.%u build %u]", 
					GET_EXE_VERSION_EX_MAJOR(ver), GET_EXE_VERSION_EX_MINOR(ver), GET_EXE_VERSION_EX_BUILD(ver));

				auto menuCKPEVersion = menuBar->addAction(QString::asprintf(versionApp.c_str()));
				if (menuCKPEVersion)
					mwnd->connect(menuCKPEVersion, &QAction::triggered, mwnd, &AboutWindow::QT5Show);

				auto toolbars = mwnd->findChildren<QToolBar*>();
				if (toolbars.size() > 0)
				{
					auto actionsToolbar = toolbars[0]->actions();

					// fixed tooltip "reate ..."
					actionsToolbar[71]->setToolTip("Create NavMesh Generation Inclusion Volume");
					actionsToolbar[72]->setToolTip("Create NavMesh Generation Exclusion Volume");
				}

				// Get toolbars list
				if (Common::UI::IsDarkTheme())
				{
					if (toolbars.size() > 0)
					{
						//0 : Version Control...
						//1 : Data...
						//2 : Save Plugin
						//3 : Hotload Toggle
						//4 : Force Hotload
						//5 : Go to Selection on PC
						//6 :
						//7 : Preferences...
						//8 : Open the Material Editor
						//9 : Open the Particle Editor
						//10 : Open the Folder Keyword Editor
						//11 : Undo
						//12 : Redo
						//13 :
						//14 : Snap to Grid
						//15 : Snap to Angle
						//16 : Snap to Connect Points
						//17 :
						//18 : Toggle Scale In Place
						//19 : Toggle Local / Global Translation
						//20 :
						//21 : World(Heightmap) Editing
						//22 : Landscape Editing
						//23 : Light Picker
						//24 : Run Havok Sim
						//25 : Animate Lighting and Effects
						//26 : Reset All Particles
						//27 : Pick References Below Terrain
						//28 :
						//29 : Toggle Lights
						//30 : Toggle Sky
						//31 : Toggle Galaxy Mode
						//32 : Toggle Grass
						//33 : Force Weather
						//34 : Toggle Wind
						//35 : Reset Raytracing
						//36 : Toggle Cloud Shadows
						//37 : Toggle Antialiasing
						//38 : Toggle Volumetric Lighting
						//39 : Toggle Lens Flares
						//40 :
						//41 : Filtered Dialogue...
						//42 :
						//43 : Toggle_Navmesh_Mode
						//44 :
						//45 : Create Post Effect Box Volume
						//46 : Create Post Effect Sphere Volume
						//47 : Create Post Effect Cylinder Volume
						//48 :
						//49 : Create Trigger
						//50 : Create Trigger Cylinder
						//51 :
						//52 : Create Sound Emitter
						//53 : Create Acoustic Space Box
						//54 : Create Acoustic Space Sphere
						//55 : Create Acoustic Space Cylinder
						//56 : Create Audio Occlusion Plane
						//57 : Create Audio Occlusion Box
						//58 : Create Audio Occlusion Sphere
						//59 : Create Audio Occlusion Cylinder
						//60 : Create Box Light
						//61 :
						//62 : Create Collision Plane
						//63 : Create Collision Cube
						//64 : Create Collision Sphere
						//65 : Create Collision Cylinder
						//66 : Create Current Plane
						//67 : Create Current Cube
						//68 : Create Current Sphere
						//69 : Create LOD Clip Volume
						//70 :
						//71 : Create NavMesh Generation Inclusion Volume
						//72 : Create NavMesh Generation Exclusion Volume
						//73 :
						//74 : Toggle Power Link Edit Mode
						//75 : Toggle NavMesh Splines Edit Mode
						//76 :
						//77 : SpawnMetricViewer
						//78 : Toggle Light Toolbar
						//79 :
						//80 :

						auto actionsToolbar = toolbars[0]->actions();
						actionsToolbar[5]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic54.png"));
						actionsToolbar[8]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic91.png"));
						actionsToolbar[9]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic92.png"));
						actionsToolbar[10]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic90.png"));
						actionsToolbar[14]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic7.png"));
						actionsToolbar[15]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic8.png"));
						actionsToolbar[16]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic49.png"));
						actionsToolbar[18]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic62.png"));
						actionsToolbar[19]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic57.png"));
						actionsToolbar[21]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic9.png"));
						actionsToolbar[23]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic64.png"));
						actionsToolbar[26]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic65.png"));
						actionsToolbar[27]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic63.png"));
						actionsToolbar[29]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic12.png"));
						actionsToolbar[30]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic13.png"));
						actionsToolbar[31]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic67.png"));
						actionsToolbar[32]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic43.png"));
						actionsToolbar[33]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic59.png"));
						actionsToolbar[34]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic68.png"));
						actionsToolbar[35]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic72.png"));
						actionsToolbar[36]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic66.png"));
						actionsToolbar[37]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic71.png"));
						actionsToolbar[38]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic69.png"));
						actionsToolbar[39]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic70.png"));
						actionsToolbar[43]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic18.png"));
						actionsToolbar[45]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic74.png"));
						actionsToolbar[46]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic75.png"));
						actionsToolbar[47]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic73.png"));
						actionsToolbar[49]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic26.png"));
						actionsToolbar[50]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic76.png"));
						actionsToolbar[52]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic24.png"));
						actionsToolbar[53]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic25.png"));
						actionsToolbar[54]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic78.png"));
						actionsToolbar[55]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic79.png"));
						actionsToolbar[56]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic80.png"));
						actionsToolbar[57]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic81.png"));
						actionsToolbar[58]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic82.png"));
						actionsToolbar[59]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic77.png"));
						actionsToolbar[60]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic50.png"));
						actionsToolbar[62]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic38.png"));
						actionsToolbar[63]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic39.png"));
						actionsToolbar[64]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic40.png"));
						actionsToolbar[65]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic83.png"));
						actionsToolbar[66]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic45.png"));
						actionsToolbar[67]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic46.png"));
						actionsToolbar[68]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic47.png"));
						actionsToolbar[69]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic58.png"));
						actionsToolbar[71]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic84.png"));
						actionsToolbar[72]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic85.png"));
						actionsToolbar[74]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic89.png"));
						actionsToolbar[75]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic86.png"));
						actionsToolbar[77]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic87.png"));
						actionsToolbar[78]->setIcon(QIcon(":/CKPE/qt5menu/ckpe_ic88.png"));
					}
				}
			}

			void CALLBACK MainWindow::ShowForm(std::uint32_t FormID) noexcept(true)
			{
				_try
				{
					auto form = EditorAPI::Forms::TESForm::FindFormByFormID(FormID);
					if (form) form->ShowEditWindow(MainWindow::Singleton->Handle);
				}
				__except (1)
				{
					// skip fatal error
				}
			}
		}
	}
}