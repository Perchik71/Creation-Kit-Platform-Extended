// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "LoadMaterialsQSplash.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			static constexpr const char* MESSAGE = "Initializing Materials...";

			HANDLE g_LoadMaterialsQSplashPatch_wait = NULL;
			uintptr_t pointer_LoadMaterialsQSplashPatch_sub0 = 0;

			LoadMaterialsQSplashPatch::LoadMaterialsQSplashPatch() : Module(GlobalEnginePtr)
			{}

			bool LoadMaterialsQSplashPatch::HasOption() const
			{
				return false;
			}

			bool LoadMaterialsQSplashPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* LoadMaterialsQSplashPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* LoadMaterialsQSplashPatch::GetName() const
			{
				return "Load Materials QSplash";
			}

			bool LoadMaterialsQSplashPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> LoadMaterialsQSplashPatch::GetDependencies() const
			{
				return {};
			}

			bool LoadMaterialsQSplashPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0;
			}

			bool LoadMaterialsQSplashPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
#ifdef _CKPE_WITH_QT5
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Bethesda error: finish() must be called after show()
					// READ DOC: https://doc.qt.io/qt-6/qsplashscreen.html#finish
					// Erase error, replace it with our own
					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&hk_finish);
					lpRelocator->PatchNop(_RELDATA_RAV(0) + 5, 0xA);

					//*(uintptr_t*)&pointer_LoadMaterialsQSplashPatch_sub0 =
					//	voltek::detours_function_class_jump(_RELDATA_ADDR(1), (uintptr_t)&sub);

					return true;
				}
#endif // !_CKPE_WITH_QT5

				return false;
			}

			bool LoadMaterialsQSplashPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

#ifdef _CKPE_WITH_QT5
			void LoadMaterialsQSplashPatch::hk_finish(QSplashScreen* logoWin, QWidget* mainWin)
			{
				//logoWin->showMessage(MESSAGE, 1, Qt::red);
				// Instead of QtCore::ProcessMessage, since CKPE Qt is not initialized
				//Utils::ProcessMessage();
				// Wait loading materials
				//g_LoadMaterialsQSplashPatch_wait = CreateEvent(NULL, TRUE, FALSE, NULL);
				//ResetEvent(g_LoadMaterialsQSplashPatch_wait);
				//WaitForSingleObject(g_LoadMaterialsQSplashPatch_wait, INFINITE);
				//CloseHandle(g_LoadMaterialsQSplashPatch_wait);
				// Show main window
				mainWin->show();
				logoWin->finish(mainWin);
			}
#endif // !_CKPE_WITH_QT5

			void LoadMaterialsQSplashPatch::sub(void* arg1, void* arg2)
			{
				//_CONSOLE(MESSAGE);
				// To be processed faster by the process
				SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
				// Load
				LoadMaterials(arg1, arg2);
				//_CONSOLE("%s %s", MESSAGE,  ? "SUCCESS" : "FATAL");
				// Return priority
				SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
				// End loading
				SetEvent(g_LoadMaterialsQSplashPatch_wait);
			}

			bool LoadMaterialsQSplashPatch::LoadMaterials(void* arg1, void* arg2)
			{
				__try
				{
					fastCall<void>(pointer_LoadMaterialsQSplashPatch_sub0, arg1, arg2);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					return false;
				}

				return true;
			}
		}
	}
}