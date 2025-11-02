// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/TESDataHandler.h>
#include <Patches/CKPE.Fallout4.Patch.ESLTip.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			constexpr const char* sNotEslCapable = " <NOT ESL Capable - no more IDs remaining>";
			constexpr const char* sEslCapable = " <ESL Capable - %d IDs remaining>";

			ESLTip::ESLTip() : Common::Patch()
			{
				SetName("ESL Tip");
			}

			bool ESLTip::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ESLTip::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ESLTip::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> ESLTip::GetDependencies() const noexcept(true)
			{
				return { "Console", "TESDataHandler" };
			}

			bool ESLTip::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool ESLTip::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&sub);

				return true;
			}

			void ESLTip::sub(char* dst, std::size_t max_size, char* src) noexcept(true)
			{
				strcat_s(dst, max_size, src);

				// Checking for the current active plugin
				auto active_plugin = EditorAPI::TESDataHandler::Singleton.Singleton->ActiveMod;
				if (active_plugin)
				{
					auto total = active_plugin->GetHeaderInfo().numRecords;
					auto avail = 0xFFF - total;
					if (avail >= 0xFFF)
						strcat_s(dst, max_size, sNotEslCapable);
					else
					{
						char szBuf[64] = { 0 };
						sprintf_s(szBuf, sEslCapable, avail);
						strcat_s(dst, max_size, szBuf);
					}
				}
			}
		}
	}
}