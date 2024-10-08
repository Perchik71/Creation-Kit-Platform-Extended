﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/ConsoleWindow.h"
#include "Core/FormInfoOutputWindow.h"
#include "Core/TypeInfo/ms_rtti.h"
#include "Version/resource_version2.h"
#include "UITheme/VarCommon.h"
#include "Editor API/SF/TESForm.h"
#include "MainWindowSF.h"
#include "AboutWindowSF.h"
#include "Editor API/SF/TESPointerHandleDetail.h"

#ifdef _CKPE_WITH_QT5
#	include "QtCore/qstring.h"
#	include "QtCore/qlist.h"
#	include "QtWidgets/qaction.h"
#	include "QtWidgets/qmenu.h"
#	include "QtWidgets/qmenubar.h"
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

			BOOL CALLBACK MainWindow::HKInitialize(QMainWindow* MainWindow, LPCSTR ExeFileName, DWORD Unk01)
			{
				BOOL bResult = Core::fastCall<BOOL>(pointer_MainWindow_sub0, MainWindow, ExeFileName, Unk01);
				if (bResult)
				{
					//auto application = qApp;

					// Inofficially, (HWND)widget->winId(); still works, note that it enforces 
					// the creation of a native window, though.
					WndHandle = (HWND)MainWindow->windowHandle()->winId();

					auto menuBar = MainWindow->menuBar();
					if (!menuBar) return false;
					
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
					}

					auto menuCKPEVersion = menuBar->addAction(QString::asprintf("[CKPE: V%u.%u BUILD %u]", 
						VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD));
					if (menuCKPEVersion)
						MainWindow->connect(menuCKPEVersion, &QAction::triggered, MainWindow, &AboutWindow::QT5Show);
				}
				return bResult;
			}

			HWND CALLBACK MainWindow::GetWindowHandle()
			{
				return WndHandle;
			}
#endif // !_CKPE_WITH_QT5

			void CALLBACK MainWindow::ShowForm(DWORD FormID)
			{
#ifdef _CKPE_WITH_QT5
				auto form = EditorAPI::Starfield::TESForm::GetFormByNumericID(FormID);
				if (form) form->ShowEditWindow(WndHandle);
#endif // !_CKPE_WITH_QT5
			}

			bool MainWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_STARFIELD_LAST;
			}

			bool MainWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
#ifdef _CKPE_WITH_QT5
					pointer_MainWindow_sub0 = voltek::detours_function_class_jump(_RELDATA_ADDR(0), &HKInitialize);
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