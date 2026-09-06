// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
				using namespace Common;

				//
				// Change the default window class name so legacy editors can be opened without using bAllowMultipleEditors
				//
				char* newWindowClass = new char[250];
				sprintf_s(newWindowClass, 250, "Creation Kit %s", StringUtils::Utf16ToWinCP(VersionLists::GetEditorVersionByString()).c_str());
				Relocation(ID{ 42440 }).Write(&newWindowClass, sizeof(newWindowClass));
				Common::ModernTheme::AddSpermanentWindowSubclass(newWindowClass);

				return true;
			}
		}
	}
}