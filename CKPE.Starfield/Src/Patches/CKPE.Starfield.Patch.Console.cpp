// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.Console.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			const char* typeList[] =
			{
				"DEFAULT",
				"SYSTEM",
				"COMBAT",
				"ANIMATION",
				"AI",
				"SCRIPTS",
				"SAVELOAD",
				"DIALOGUE",
				"QUESTS",
				"PACKAGES",
				"EDITOR",
				"MODELS",
				"TEXTURES",
				"PLUGINS",
				"MASTERFILE",
				"FORMS",
				"MAGIC",
				"SHADERS",
				"RENDERING",
				"PATHFINDING",
				"MENUS",
				"AUDIO",
				"CELLS",
				"HAVOK",
				"FACEGEN",
				"WATER",
				"INGAME",
				"MEMORY",
				"PERFORMANCE",
				"LOOTJOY",
				"VATS",
				"DISMEMBER",
				"COMPANION",
				"WORKSHOP",
				"GALAXY",
				"TERRAIN",
				"PLANETS",
				"PROCGEN",
				"LODGENERATION",
				"MATERIALS",
				"PARTICLE_ENGINE",
				"BINDING",
				"SHIP_BUILDER",
				"HOUDINI",
				"SEQUENCE",
				"MORPH",
				"VFX",
				"BNET",
				"BPS",
				"HOTLOADING",
				"AVMS"
			};

			Console::Console() : Common::Patch()
			{
				SetName("Console");
			}

			bool Console::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* Console::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool Console::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> Console::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool Console::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool Console::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 3)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();
				
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&LogWarning);
				Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&LogAssertVa1);
				Detours::DetourJump(__CKPE_OFFSET(2), (std::uintptr_t)&LogAssertVa1);
				Detours::DetourJump(__CKPE_OFFSET(3), (std::uintptr_t)&LogAssertVa2);
				Detours::DetourJump(__CKPE_OFFSET(4), (std::uintptr_t)&LogWarningUnknown1);
				//Detours::DetourJump(__CKPE_OFFSET(5), (std::uintptr_t)&LogWarningUnknown1);
				Detours::DetourJump(__CKPE_OFFSET(6), (std::uintptr_t)&LogWarningUnknown1);
				Detours::DetourJump(__CKPE_OFFSET(7), (std::uintptr_t)&LogWarningUnknown1);
				Detours::DetourJump(__CKPE_OFFSET(8), (std::uintptr_t)&LogWarningUnknown1);

				if (_READ_OPTION_BOOL("Log", "bAllowOutputNetworkActivity", false))
				{
					// this spam [BDK] --> It looks like Bethesda is constantly sending user info to its server
					Detours::DetourJump(__CKPE_OFFSET(9), (std::uintptr_t)&LogWarningUnknown1);
					Detours::DetourJump(__CKPE_OFFSET(10), (std::uintptr_t)&LogWarningUnknown1);
					Detours::DetourJump(__CKPE_OFFSET(11), (std::uintptr_t)&LogWarningUnknown1);
					Detours::DetourJump(__CKPE_OFFSET(12), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(13), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(14), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(15), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(16), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(17), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(18), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(19), (std::uintptr_t)&LogWarningUnknown2);
					Detours::DetourJump(__CKPE_OFFSET(20), (std::uintptr_t)&LogWarningUnknown2);
				}

				Detours::DetourJump(__CKPE_OFFSET(21), (std::uintptr_t)&LogWarningUnknown2);

				return true;
			}

			void Console::Log(const char* Format, ...) noexcept(true)
			{
				va_list va;
				va_start(va, Format);
				LogVa(Format, va);
				va_end(va);
			}

			void Console::LogVa(const char* Format, va_list Va) noexcept(true)
			{
				_CONSOLEVA(Format, Va);
			}

			void Console::LogWarning(MsgType Type, const char* Format, ...) noexcept(true)
			{
				va_list va;
				va_start(va, Format);
				LogWarningVa(Type, Format, va);
				va_end(va);
			}

			void Console::LogWarningVa(MsgType Type, const char* Format, va_list Va) noexcept(true)
			{
				char buffer[1024];
				_vsnprintf_s(buffer, _TRUNCATE, Format, Va);

				Log("[%s] %s", typeList[Type], buffer);
			}

			void Console::LogWarningUnknown1(const char* Format, ...) noexcept(true)
			{
				va_list va;
				va_start(va, Format);
				LogVa(Format, va);
				va_end(va);
			}

			void Console::LogWarningUnknown2(__int64 Unused, const char* Format, ...) noexcept(true)
			{
				va_list va;
				va_start(va, Format);
				LogVa(Format, va);
				va_end(va);
			}

			void Console::LogAssertVa1(const char* File, int Line, MsgType Type, const char* Format,
				va_list& Va) noexcept(true)
			{
				// This is redundant
				UNREFERENCED_PARAMETER(File);
				UNREFERENCED_PARAMETER(Line);

				char buffer[2048];
				_vsnprintf_s(buffer, _TRUNCATE, Format, Va);

				Log("%s: %s", typeList[Type], buffer);
			}

			void Console::LogAssertVa2(__int64 Unused, const char* File, int Line, const char* Format,
				va_list& Va) noexcept(true)
			{
				// This is redundant
				UNREFERENCED_PARAMETER(Unused);
				UNREFERENCED_PARAMETER(File);
				UNREFERENCED_PARAMETER(Line);

				LogVa(Format, Va);
			}

			void Console::LogAssert(const char* File, int Line, const char* Message, ...) noexcept(true)
			{
				if (!Message || !Message[0])
					Message = "<No message>";

				char buffer[1024];
				va_list va;

				va_start(va, Message);
				_vsnprintf_s(buffer, _TRUNCATE, Message, va);
				va_end(va);

				Log("[ASSERTION] %s (%s line %d)", buffer, File, Line);
			}
		}
	}
}