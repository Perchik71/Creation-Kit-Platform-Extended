// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

#ifdef _CKPE_WITH_QT5
#	include "QtWidgets/qmainwindow.h"
#endif // !_CKPE_WITH_QT5

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			using namespace CreationKitPlatformExtended::Core;

			class MainWindow : public Module
			{
			public:
				MainWindow();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;
#ifdef _CKPE_WITH_QT5
				static BOOL CALLBACK HKInitialize(QMainWindow* MainWindow, LPCSTR ExeFileName, DWORD Unk01);
				static HWND CALLBACK GetWindowHandle();
#endif // !_CKPE_WITH_QT5

				static void CALLBACK ShowForm(DWORD FormID);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				MainWindow(const MainWindow&) = default;
				MainWindow& operator=(const MainWindow&) = default;
			};
		}
	}
}