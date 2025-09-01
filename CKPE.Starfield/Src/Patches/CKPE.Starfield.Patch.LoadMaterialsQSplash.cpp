// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "QtWidgets/qwidget.h"
#include "QtWidgets/qsplashscreen.h"

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.LoadMaterialsQSplash.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			LoadMaterialsQSplash::LoadMaterialsQSplash() : Common::Patch()
			{
				SetName("Load Materials QSplash");
			}

			bool LoadMaterialsQSplash::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* LoadMaterialsQSplash::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool LoadMaterialsQSplash::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> LoadMaterialsQSplash::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool LoadMaterialsQSplash::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool LoadMaterialsQSplash::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Bethesda error: finish() must be called after show()
				// READ DOC: https://doc.qt.io/qt-6/qsplashscreen.html#finish
				// Erase error, replace it with our own
				auto offset = __CKPE_OFFSET(0);
				Detours::DetourCall(offset, (std::uintptr_t)&hk_finish);
				SafeWrite::WriteNop(offset + 5, 0xA);

				return true;
			}

			void LoadMaterialsQSplash::hk_finish(std::int64_t logoWin, std::int64_t mainWin) noexcept(true)
			{
				// Show main window
				((QWidget*)mainWin)->show();
				((QSplashScreen*)logoWin)->finish((QWidget*)mainWin);
			}
		}
	}
}