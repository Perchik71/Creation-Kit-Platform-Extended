// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixQuoteCmdLineSF.h"
#include "Editor API/BSString.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			FixQuoteCmdLinePatch::FixQuoteCmdLinePatch() : Module(GlobalEnginePtr)
			{}

			bool FixQuoteCmdLinePatch::HasOption() const
			{
				return false;
			}

			bool FixQuoteCmdLinePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixQuoteCmdLinePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixQuoteCmdLinePatch::GetName() const
			{
				return "Fixed quote to cmdline";
			}

			bool FixQuoteCmdLinePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixQuoteCmdLinePatch::GetDependencies() const
			{
				return {};
			}

			bool FixQuoteCmdLinePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0;
			}

			bool FixQuoteCmdLinePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					for (uint32_t i = 0; i < lpRelocationDatabaseItem->Count(); i++)
						lpRelocator->DetourCall(_RELDATA_RAV(i), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool FixQuoteCmdLinePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			char* FixQuoteCmdLinePatch::sub(char* str, const char* delim, char** next_token)
			{
				if (str) {
					while (*str == ' ') {
						if (*str == '\0')
							return nullptr;
						str++;
					}
					if (*str == '\"')
						return strtok_s(++str, "\"", next_token);
					else
						return strtok_s(str, " ", next_token);
				}
				else if (next_token && *next_token) {
					if (strchr(*next_token, '\"')) {
						char* lpRes = strtok_s(nullptr, "\"", next_token);

						if (lpRes && !Utils::Trim(lpRes).length())
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