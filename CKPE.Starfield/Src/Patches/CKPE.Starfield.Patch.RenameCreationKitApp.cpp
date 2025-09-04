// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <QtCore/qstring.h>

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ModernTheme.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.RenameCreationKitApp.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			static QString& QFixAppend_PluginName(QString& This, QString* PluginName)
			{
				if (((*PluginName)[0] == '/') || ((*PluginName)[0] == '\\'))
					PluginName->remove(0, 1);
				return This.append(PluginName);
			}

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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool RenameCreationKitApp::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 2)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				char* newWindowClass = new char[250];
				sprintf_s(newWindowClass, 250, "Creation Kit %s\0",
					StringUtils::Utf16ToWinCP(VersionLists::GetEditorVersionByString()).c_str());

				SafeWrite::WriteStringRef(__CKPE_OFFSET(0), newWindowClass);
				// In Qt, it would be necessary to give the name of the window initially, for something acceptable
				SafeWrite::Write(__CKPE_OFFSET(1), (uint8_t*)"Creation Kit\0", 13);
				// Cut a useless entry [Branch: <some>, Version: <CKVer>]
				//lpRelocator->PatchNop(_RELDATA_RAV(2), 0xA7);
				SafeWrite::WriteNop(__CKPE_OFFSET(4), 0x6);
				// Cut a useless entry [Admin]
				Detours::DetourClassCall(__CKPE_OFFSET(3), &QString::sprintf);
				// Fix splash
				Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&QFixAppend_PluginName);
				
				Common::ModernTheme::AddSpermanentWindowSubclass(newWindowClass);

				return true;
			}
		}
	}
}