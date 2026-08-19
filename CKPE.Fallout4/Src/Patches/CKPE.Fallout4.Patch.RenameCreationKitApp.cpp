// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ModernTheme.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.RenameCreationKitApp.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			RenameCreationKitApp::RenameCreationKitApp() : Common::Patch()
			{
				SetName("Rename Creation Kit App");
			}

			bool RenameCreationKitApp::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RenameCreationKitApp::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RenameCreationKitApp::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RenameCreationKitApp::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RenameCreationKitApp::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool RenameCreationKitApp::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				std::uintptr_t address = 0;

				if (db)
				{
					if (db->GetVersion() != 1)
						return false;

					address = __CKPE_OFFSET(0);
				}
				else
				{
					address = Common::AddressLibrary::GetSingleton()->Resolve(854440);
				}

				if (!address)
					return false;

				//
				// Change the default window class name so legacy editors can be opened without using bAllowMultipleEditors
				//
				char* newWindowClass = new char[250];
				sprintf_s(newWindowClass, 250, "Creation Kit %s",
					StringUtils::Utf16ToWinCP(VersionLists::GetEditorVersionByString()).c_str());
				SafeWrite::WriteStringRef(address, newWindowClass);
				Common::ModernTheme::AddSpermanentWindowSubclass(newWindowClass);

				return true;
			}
		}
	}
}