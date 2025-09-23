// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>

namespace CKPE
{
	namespace Fallout4
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool Console::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 2)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();
				
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&LogWarningVa);
				Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&LogWarningUnknown1);
				Detours::DetourJump(__CKPE_OFFSET(2), (std::uintptr_t)&LogWarningUnknown1);
				Detours::DetourJump(__CKPE_OFFSET(3), (std::uintptr_t)&LogWarningUnknown1);
				Detours::DetourJump(__CKPE_OFFSET(4), (std::uintptr_t)&LogWarningUnknown1);
				Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&LogWarningUnknown2);
				Detours::DetourJump(__CKPE_OFFSET(6), (std::uintptr_t)&LogAssert);

				return true;
			}

			void Console::Log(const char* Format, ...) noexcept(true)
			{
				va_list va;
				va_start(va, Format);
				LogVa(Format, va);
				va_end(va);
			}

			void Console::LogVa(const char* Format, va_list& Va) noexcept(true)
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

			void Console::LogWarningVa(MsgType Type, const char* Format, va_list& Va) noexcept(true)
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