// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ESLTip.h"
#include "Patches/ConsolePatch.h"
#include "Editor API/FO4/TESDataHandler.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

			constexpr const char* sNotEslCapable = " <NOT ESL Capable - no more IDs remaining>";
			constexpr const char* sEslCapable = " <ESL Capable - %d IDs remaining>";

			ESLTipPatch::ESLTipPatch() : Module(GlobalEnginePtr)
			{}

			bool ESLTipPatch::HasOption() const
			{
				return false;
			}

			bool ESLTipPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ESLTipPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ESLTipPatch::GetName() const
			{
				return "ESL Tip";
			}

			bool ESLTipPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> ESLTipPatch::GetDependencies() const
			{
				return { "Console", "TESDataHandler" };
			}

			bool ESLTipPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool ESLTipPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// make the window title the same as in 1.6.1130
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool ESLTipPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void ESLTipPatch::sub(char* dst, size_t max_size, char* src)
			{
				strcat_s(dst, max_size, src);

				// Checking for the current active plugin
				auto active_plugin = TESDataHandler::Instance->ActiveMod;
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