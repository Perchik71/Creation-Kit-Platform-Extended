// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

#ifdef _CKPE_WITH_QT5
#	include "QtWidgets/qwidget.h"
#	include "QtWidgets/qsplashscreen.h"
#endif // !_CKPE_WITH_QT5

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			using namespace CreationKitPlatformExtended::Core;

			class LoadMaterialsQSplashPatch : public Module
			{
			public:
				LoadMaterialsQSplashPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

#ifdef _CKPE_WITH_QT5
				static void hk_finish(QSplashScreen* logoWin, QWidget* mainWin);
#endif // !_CKPE_WITH_QT5

				static void sub(void* arg1, void* arg2);
				static bool LoadMaterials(void* arg1, void* arg2);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				LoadMaterialsQSplashPatch(const LoadMaterialsQSplashPatch&) = default;
				LoadMaterialsQSplashPatch& operator=(const LoadMaterialsQSplashPatch&) = default;
			};
		}
	}
}