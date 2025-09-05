// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.FixQuoteCmdLine.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			FixQuoteCmdLine::FixQuoteCmdLine() : Common::Patch()
			{
				SetName("Fixed quote to cmdline");
			}

			bool FixQuoteCmdLine::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixQuoteCmdLine::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixQuoteCmdLine::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixQuoteCmdLine::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixQuoteCmdLine::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool FixQuoteCmdLine::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				for (uint32_t i = 0; i < db->GetCount(); i++)
					Detours::DetourCall(__CKPE_OFFSET(i), (uintptr_t)&sub);

				return true;
			}

			char* FixQuoteCmdLine::sub(char* str, const char* delim, char** next_token) noexcept(true)
			{
				if (str) 
				{
					while (*str == ' ')
					{
						if (*str == '\0')
							return nullptr;
						str++;
					}
					if (*str == '\"')
						return strtok_s(++str, "\"", next_token);
					else
						return strtok_s(str, " ", next_token);
				}
				else if (next_token && *next_token)
				{
					if (strchr(*next_token, '\"')) 
					{
						char* lpRes = strtok_s(nullptr, "\"", next_token);

						if (lpRes && !StringUtils::Trim(lpRes).length())
							lpRes = strtok_s(nullptr, "\"", next_token);

						return lpRes;
					}
					else
						return strtok_s(nullptr, " ", next_token);
				}
				else
					return strtok_s(str, delim, next_token);
			}
		}
	}
}