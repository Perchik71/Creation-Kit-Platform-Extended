// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/ConsoleWindow.h"
#include "Core/FormInfoOutputWindow.h"
#include "Core/TypeInfo/ms_rtti.h"
#include "Version/resource_version2.h"
#include "UITheme/VarCommon.h"
#include "Editor API/BSString.h"
#include "Editor API/SF/TESForm.h"
#include "MainWindowSF.h"
#include "AboutWindowSF.h"
#include "Editor API/SF/TESPointerHandleDetail.h"

#ifdef _CKPE_WITH_QT5
#	include "QtCore/qstring.h"
#	include "QtCore/qlist.h"
#	include "QtCore/qfile.h"
#	include "QtWidgets/qaction.h"
#	include "QtWidgets/qmenu.h"
#	include "QtWidgets/qmenubar.h"
#	include "QtWidgets/qtoolbar.h"
#	include "QtWidgets/qfileiconprovider.h"
#	include "QtGui/qwindow.h"
#endif // !_CKPE_WITH_QT5

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			MainWindow* GlobalMainWindowPtr = nullptr;
			uintptr_t pointer_MainWindow_sub0 = 0;
			uintptr_t pointer_MainWindow_sub1 = 0;

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
				sprintf_s(temp, "[CKPE: V%u.%u BUILD %u]", (pFileInfo->dwFileVersionMS >> 16) & 0xFFFF,
					(pFileInfo->dwFileVersionMS) & 0xFFFF, pFileInfo->dwFileVersionLS);
				result = temp;

				delete[] pbVersionInfo;
				return true;
			}

			void OutputFormInfo(uint32_t FormID)
			{
				if (FormID > 0)
				{
					__try
					{
						auto form = EditorAPI::Starfield::TESForm::GetFormByNumericID(FormID);
						if (form)
						{
							char szBuf[140] = { 0 };
							form->DebugInfo(szBuf, 140);

							_CONSOLE("DebugInfo -> %s type %02X ptr %p",
								szBuf, (uint16_t)form->GetFormType(), form);
						}
					}
					__except (EXCEPTION_EXECUTE_HANDLER)
					{
						_CONSOLE("Form info -> Incorrect FormID");
					}
				}
			}

			MainWindow::MainWindow() : Module(GlobalEnginePtr)
			{}

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
				return { "TESForm" };
			}

#ifdef _CKPE_WITH_QT5
			static HWND WndHandle = NULL;

			bool CALLBACK MainWindow::HKInitialize(QMainWindow* MainWindow, LPCSTR ExeFileName, DWORD Unk01)
			{
				if (Core::fastCall<bool>(pointer_MainWindow_sub0, MainWindow, ExeFileName, Unk01))
				{
					// Inofficially, (HWND)widget->winId(); still works, note that it enforces 
					// the creation of a native window, though.
					WndHandle = (HWND)MainWindow->windowHandle()->winId();
					return true;
				}

				return false;
			}

			void CALLBACK MainWindow::HKInitializeActions(QMainWindow* MainWindow)
			{
				Core::fastCall<void>(pointer_MainWindow_sub1, MainWindow);

				auto menuBar = MainWindow->menuBar();
				if (!menuBar) return;

				// Uppercase
				auto menuActionList = menuBar->actions();
				for (auto menuItem : menuActionList)
					menuItem->setText(menuItem->text().toUpper());

				// Creating an CKPE menu's
				auto menuExtensions = menuBar->addMenu("EXTENSIONS");
				if (menuExtensions)
				{
					static auto actionShowLog = new QAction("Show Log", MainWindow);
					static auto actionClearLog = new QAction("Clear Log", MainWindow);
					static auto actionAutoscrollLog = new QAction("Autoscroll Log", MainWindow);
					actionAutoscrollLog->setCheckable(true);
					actionAutoscrollLog->setChecked(true);

					static auto actionDumpRTTIData = new QAction("Dump RTTI Data", MainWindow);
					static auto actionDumpSDMInfo = new QAction("Dump SDM Info", MainWindow);
					static auto actionFormInfoOutput = new QAction("Form Info Output", MainWindow);

					static auto actionSaveHardcodedForms = new QAction("Save Hardcoded Forms", MainWindow);

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

						if (UITheme::IsDarkTheme())
							// Fake set theme Plastique Dark 
							actionThemes->menu()->actions().at(3)->activate(QAction::Trigger);
						else
							// Fake set theme Default
							actionThemes->menu()->actions().at(0)->activate(QAction::Trigger);
					}

					// remove Warnings
					auto menuViewActionList = menuActionList.at(2)->menu()->actions();
					menuViewActionList.at(31)->setVisible(false);

					MainWindow->connect(actionShowLog, &QAction::triggered, MainWindow, []() {
						if (GlobalConsoleWindowPtr)
							GlobalConsoleWindowPtr->BringToFront();
						});
					MainWindow->connect(actionClearLog, &QAction::triggered, MainWindow, []() {
						if (GlobalConsoleWindowPtr)
							GlobalConsoleWindowPtr->Clear();
						});
					MainWindow->connect(actionAutoscrollLog, &QAction::triggered, MainWindow, []() {
						if (GlobalConsoleWindowPtr)
						{
							bool flag = actionAutoscrollLog->isChecked();
							GlobalConsoleWindowPtr->SetAutoScroll(flag);
							actionAutoscrollLog->setChecked(flag);
						}
						});

					MainWindow->connect(actionDumpRTTIData, &QAction::triggered, MainWindow, []() {
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
						});
					MainWindow->connect(actionDumpSDMInfo, &QAction::triggered, MainWindow, []() {
						auto handleManager = EditorAPI::Starfield::HandleManager::Singleton.GetSingleton();

						auto head = handleManager->GetHead();
						auto free = handleManager->GetFree();
						auto tail = handleManager->GetTail();
						auto maxe = handleManager->GetMax();

						_CONSOLE("Dump SDM Info:\n\tHead: 0x%08X\n\tFree: 0x%08X\n\tTail: 0x%08X\n\tMax: 0x%08X\n\tCapacity: %.2f%%",
							head, free, tail, maxe, ((((long double)(maxe - free)) * 100.0f) / (long double)maxe));
						});
					MainWindow->connect(actionFormInfoOutput, &QAction::triggered, MainWindow, []() {
						FormInfoOutputWindow Wnd;
						OutputFormInfo((uint32_t)Wnd.OpenModal(GetActiveWindow()));
						});

					MainWindow->connect(actionSaveHardcodedForms, &QAction::triggered, MainWindow, []() {
						for (uint32_t i = 0; i < 4096; i++)
						{
							auto form = EditorAPI::Starfield::TESForm::GetFormByNumericID(i);
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
					// Quit handler
					MainWindow->connect(actionsMenu[0x14], &QAction::triggered, MainWindow, []() { Utils::Quit(); });

					// Change icon
					if (UITheme::IsDarkTheme())
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

				EditorAPI::BSString versionApp;
				char modulePath[MAX_PATH];
				GetModuleFileNameA((HMODULE)GlobalEnginePtr->GetInstanceDLL(), modulePath, MAX_PATH);
				GetFileVersion(modulePath, versionApp);

				auto menuCKPEVersion = menuBar->addAction(QString::asprintf(versionApp.c_str()));
				if (menuCKPEVersion)
					MainWindow->connect(menuCKPEVersion, &QAction::triggered, MainWindow, &AboutWindow::QT5Show);

				auto toolbars = MainWindow->findChildren<QToolBar*>();
				if (toolbars.size() > 0)
				{
					auto actionsToolbar = toolbars[0]->actions();
					
					// fixed tooltip "reate ..."
					actionsToolbar[71]->setToolTip("Create NavMesh Generation Inclusion Volume");
					actionsToolbar[72]->setToolTip("Create NavMesh Generation Exclusion Volume");
				}

				// Get toolbars list
				if (UITheme::IsDarkTheme())
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

			HWND CALLBACK MainWindow::GetWindowHandle()
			{
				return WndHandle;
			}
#endif // !_CKPE_WITH_QT5

			void CALLBACK MainWindow::ShowForm(DWORD FormID)
			{
#ifdef _CKPE_WITH_QT5
				_try
				{
					auto form = EditorAPI::Starfield::TESForm::GetFormByNumericID(FormID);
					if (form) form->ShowEditWindow(WndHandle);
				}
				__except (1)
				{
					// skip fatal error
				}
#endif // !_CKPE_WITH_QT5
			}

			bool MainWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool MainWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
#ifdef _CKPE_WITH_QT5
					pointer_MainWindow_sub0 = voltek::detours_function_class_jump(_RELDATA_ADDR(1), &HKInitialize);
					pointer_MainWindow_sub1 = voltek::detours_function_class_jump(_RELDATA_ADDR(0), &HKInitializeActions);
#endif // !_CKPE_WITH_QT5

					return true;
				}

				return false;
			}

			bool MainWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}