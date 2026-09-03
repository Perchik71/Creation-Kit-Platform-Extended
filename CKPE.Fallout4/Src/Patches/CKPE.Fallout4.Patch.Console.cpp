// Copyright © 2023-2025 aka CKPE team. All rights reserved.
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
				using namespace Common;

				Relocation(ID{ 105048, 1494590 }).WriteJump(LogWarningVa);
				Relocation(ID{ 429701, 1636009 }).WriteJump(LogWarningUnknown1);
				Relocation(ID{ 185107, 1956945 }).WriteJump(LogWarningUnknown1);
				Relocation(ID{ 378705, 1589438 }).WriteJump(LogWarningUnknown1);
				Relocation(ID{ 323410, 1589059 }).WriteJump(LogWarningUnknown1);
				Relocation(ID{ 501160, 1942406 }, Offset{ 0x7B3, 0x744 }).WriteCall(LogWarningUnknown2);
				Relocation(ID{ 334052, 1653769 }).WriteJump(LogAssert);

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