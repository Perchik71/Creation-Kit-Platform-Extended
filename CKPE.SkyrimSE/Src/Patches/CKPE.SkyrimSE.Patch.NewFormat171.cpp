// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/TESDataHandler.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.NewFormat171.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			constexpr float fPluginVersion = 1.71f;
			constexpr const char* sNotEslCapable = " <NOT ESL Capable - no more IDs remaining>";
			constexpr const char* sEslCapable = " <ESL Capable - %d IDs remaining>";

			uintptr_t pointer_NewFormat171_data = 0;

			NewFormat171::NewFormat171() : Common::Patch()
			{
				SetName("New Format 1.71");
			}

			bool NewFormat171::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* NewFormat171::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bSupportFormat171";
			}

			bool NewFormat171::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> NewFormat171::GetDependencies() const noexcept(true)
			{
				return { "Console", "TESDataHandler" };
			}

			bool NewFormat171::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool NewFormat171::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// change version
				// skip "File %s is a higher version than this EXE can load."
				SafeWrite::Write(__CKPE_OFFSET(0), (std::uint8_t*)&fPluginVersion, 4);

				// make the window title the same as in 1.6.1130
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				pointer_NewFormat171_data = __CKPE_OFFSET(3);

				// Making changes, from 1.6.1130, it's amazing that Bethesda fixed where I used to rule, 
				// I didn't have to look for a long time
				SafeWrite::Write(__CKPE_OFFSET(4), { 0x83, 0xC8, 0x01, 0x90 });
				SafeWrite::Write(__CKPE_OFFSET(5), { 0x83, 0xC8, 0x01, 0x90, 0x90 });

				auto addr = __CKPE_OFFSET(6);
				SafeWrite::WriteNop(addr, 0x34);
				SafeWrite::Write(addr - 3, { 0x4C });
				Detours::DetourCall(addr, (std::uintptr_t)&sub2);

				return true;
			}

			void NewFormat171::sub(char* dst, size_t max_size, char* src)
			{
				strcat_s(dst, max_size, src);

				// Checking for the current active plugin
				auto active_plugin = EditorAPI::TESDataHandler::Singleton.Singleton->ActiveMod;
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

			void NewFormat171::sub2(const void* load_file)
			{
				auto IdPtr = (uint32_t*)((uintptr_t)load_file + 0x290);
				auto DependObjs = ((const EditorAPI::TESFile*)load_file)->GetDependArray();
				auto DependId = *IdPtr >> 24;

				if (DependId >= 0xFE)
				{
					// Usually there is a situation when opening a list of plugins
					*IdPtr &= 0xFFFFFF;
					return;
				}

				if (!DependObjs && DependId)
					Console::LogWarning(Console::FORMS, 
						"Form(%08X) in non-dependent file contains a form with file index bits.", *IdPtr);
			}
		}
	}
}