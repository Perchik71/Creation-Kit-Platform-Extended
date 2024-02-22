// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "NewFormat171.h"
#include "Patches/ConsolePatch.h"
#include "Editor API/SSE/TESFile.h"
#include "Editor API/SSE/TESDataHandler.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI::SkyrimSpectialEdition;

			constexpr float fPluginVersion = 1.71f;
			constexpr const char* sNotEslCapable = " <NOT ESL Capable - no more IDs remaining>";
			constexpr const char* sEslCapable = " <ESL Capable - %d IDs remaining>";

			//uintptr_t pointer_NewFormat171_sub = 0;
			uintptr_t pointer_NewFormat171_data = 0;

			NewFormat171Patch::NewFormat171Patch() : Module(GlobalEnginePtr)
			{}

			bool NewFormat171Patch::HasOption() const
			{
				return true;
			}

			bool NewFormat171Patch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* NewFormat171Patch::GetOptionName() const
			{
				return "CreationKit:bSupportFormat171";
			}

			const char* NewFormat171Patch::GetName() const
			{
				return "New Format 1.71";
			}

			bool NewFormat171Patch::HasDependencies() const
			{
				return true;
			}

			Array<String> NewFormat171Patch::GetDependencies() const
			{
				return { "Console", "TESDataHandler" };
			}

			bool NewFormat171Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool NewFormat171Patch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// change version
					// skip "File %s is a higher version than this EXE can load."
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), (uint8_t*)&fPluginVersion, 4);

					// make the window title the same as in 1.6.1130
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);
					//pointer_NewFormat171_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_NewFormat171_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));

					// Making changes, from 1.6.1130, it's amazing that Bethesda fixed where I used to rule, 
					// I didn't have to look for a long time
					lpRelocator->Patch(lpRelocationDatabaseItem->At(4), { 0x83, 0xC8, 0x01, 0x90 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(5), { 0x83, 0xC8, 0x01, 0x90, 0x90 });

					auto addr = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6));
					Utils::PatchMemoryNop(addr, 0x34);
					Utils::PatchMemory(addr - 3, { 0x4C });
					Utils::DetourCall(addr, (uintptr_t)&sub2);

					return true;
				}

				return false;
			}

			bool NewFormat171Patch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void NewFormat171Patch::sub(char* dst, size_t max_size, char* src)
			{
				strcat_s(dst, max_size, src);

				// Checking for the current active plugin
				auto active_plugin = TESDataHandler::GetInstance()->ActiveMod;
				if (active_plugin && (*((uintptr_t*)pointer_NewFormat171_data)))
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

			void NewFormat171Patch::sub2(const void* load_file)
			{
				auto IdPtr = (uint32_t*)((uintptr_t)load_file + 0x290);
				auto DependObjs = ((const TESFile*)load_file)->GetDependArray();
				auto DependId = *IdPtr >> 24;

				if (DependId >= 0xFE)
				{
					// Usually there is a situation when opening a list of plugins
					*IdPtr &= 0xFFFFFF;
					return;
				}

				if (!DependObjs && DependId)
					ConsolePatch::Log("Form(%08X) in non-dependent file contains a form with file index bits.", *IdPtr);
			}
		}
	}
}