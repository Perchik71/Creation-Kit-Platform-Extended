// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ModernTheme.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.RenameCreationKitApp.h>

namespace CKPE
{
	namespace SkyrimSE
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RenameCreationKitApp::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Change the default window class name so legacy editors can be opened without using bAllowMultipleEditors
				//
				char* newWindowClass = new char[250];
				sprintf_s(newWindowClass, 250, "Creation Kit %s", 
					StringUtils::Utf16ToWinCP(VersionLists::GetEditorVersionByString()).c_str());
				SafeWrite::WriteStringRef(__CKPE_OFFSET(0), newWindowClass);
				Common::ModernTheme::AddSpermanentWindowSubclass(newWindowClass);

				return true;
			}
		}
	}
}