// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ConsolePatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		static LPCSTR typeListSkyrim[30] =
		{
			"DEFAULT",
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
			"JOBS",
			"SYSTEM"
		};

		static LPCSTR typeListFO4[34] =
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

		ConsolePatch::ConsolePatch() : Module(GlobalEnginePtr)
		{}

		bool ConsolePatch::HasOption() const
		{
			return false;
		}

		bool ConsolePatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* ConsolePatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* ConsolePatch::GetName() const
		{
			return "Console";
		}

		void ConsolePatch::Log(const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			LogVa(Format, va);
			va_end(va);
		}

		void ConsolePatch::LogVa(const char* Format, va_list Va)
		{
			_CONSOLE(Format, Va);
		}

		void ConsolePatch::LogWarning(int Type, const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			LogWarningVa(Type, Format, va);
			va_end(va);
		}

		void ConsolePatch::LogWarningVa(int Type, const char* Format, va_list& Va)
		{
			LPCSTR* typeList = nullptr;

			if (GlobalEnginePtr->GetEditorVersion() <= (int)Core::EDITOR_SKYRIM_SE_1_6_1130)
				typeList = typeListSkyrim;
			else
				return;

			char buffer[2048];
			_vsnprintf_s(buffer, _TRUNCATE, Format, Va);

			Log("%s: %s", typeList[Type], buffer);
		}

		void ConsolePatch::LogWarningUnknown1(const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			LogVa(Format, va);
			va_end(va);
		}

		void ConsolePatch::LogWarningUnknown2(__int64 Unused, const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			LogVa(Format, va);
			va_end(va);
		}

		void ConsolePatch::LogAssert(const char* File, int Line, const char* Message, ...)
		{
			if (!Message)
				Message = "<No message>";

			char buffer[2048];
			va_list va;

			va_start(va, Message);
			_vsnprintf_s(buffer, _TRUNCATE, Message, va);
			va_end(va);

			Log("ASSERTION: %s (%s line %d)", buffer, File, Line);
		}

		bool ConsolePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool ConsolePatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0),  (uintptr_t)&LogWarning);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1),  (uintptr_t)&LogWarning);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2),  (uintptr_t)&LogWarning);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(5),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(6),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(7),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(8),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(9),  (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(10), (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(11), (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(12), (uintptr_t)&LogAssert);

				return true;
			}
			
			return false;
		}

		bool ConsolePatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}