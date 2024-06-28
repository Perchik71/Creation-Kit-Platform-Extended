// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ConsolePatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		static LPCSTR typeListSkyrim[] =
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

		static LPCSTR typeListFO4[] =
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

		static LPCSTR typeListSF[] =
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
			"AVMS",
		};

		static LPCSTR* currentTypeList = nullptr;

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

		bool ConsolePatch::HasDependencies() const
		{
			return false;
		}

		Array<String> ConsolePatch::GetDependencies() const
		{
			return {};
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
			_CONSOLEVA(Format, Va);
		}

		void ConsolePatch::LogWarning(int Type, const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			LogWarningVa(Type, Format, va);
			va_end(va);
		}

		void ConsolePatch::LogWarningWithDebugInfo1(const char* File, int Line, int Type, const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			LogWarningWithDebugInfo1Va(File, Line, Type, Format, va);
			va_end(va);
		}

		void ConsolePatch::LogWarningWithDebugInfo1Va(const char* File, int Line, int Type, const char* Format, va_list& Va)
		{
			// This is redundant
			UNREFERENCED_PARAMETER(File);
			UNREFERENCED_PARAMETER(Line);
			
			if (!currentTypeList)
				return;

			char buffer[2048];
			_vsnprintf_s(buffer, _TRUNCATE, Format, Va);

			Log("%s: %s", currentTypeList[Type], buffer);
		}

		void ConsolePatch::LogWarningWithDebugInfo2(__int64 Unused, const char* File, int Line, const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			LogWarningWithDebugInfo2Va(Unused, File, Line, Format, va);
			va_end(va);
		}

		void ConsolePatch::LogWarningWithDebugInfo2Va(__int64 Unused, const char* File, int Line, const char* Format, va_list& Va)
		{
			// This is redundant
			UNREFERENCED_PARAMETER(Unused);
			UNREFERENCED_PARAMETER(File);
			UNREFERENCED_PARAMETER(Line);

			LogVa(Format, Va);
		}

		void ConsolePatch::LogWarningVa(int Type, const char* Format, va_list& Va)
		{
			if (!currentTypeList)
				return;

			char buffer[2048];
			_vsnprintf_s(buffer, _TRUNCATE, Format, Va);

			Log("%s: %s", currentTypeList[Type], buffer);
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
			if (GlobalEnginePtr->GetEditorVersion() <= (int)Core::EDITOR_SKYRIM_SE_LAST)
				currentTypeList = typeListSkyrim;
			else if (GlobalEnginePtr->GetEditorVersion() <= (int)Core::EDITOR_FALLOUT_C4_LAST)
				currentTypeList = typeListFO4;
			else if (GlobalEnginePtr->GetEditorVersion() <= (int)Core::EDITOR_STARFIELD_LAST)
				currentTypeList = typeListSF;

			auto verPatch = lpRelocationDatabaseItem->Version();
			if (verPatch == 1)
			{
				lpRelocator->DetourJump(_RELDATA_RAV(0),  (uintptr_t)&LogWarning);
				lpRelocator->DetourJump(_RELDATA_RAV(1),  (uintptr_t)&LogWarning);
				lpRelocator->DetourJump(_RELDATA_RAV(2),  (uintptr_t)&LogWarning);
				lpRelocator->DetourJump(_RELDATA_RAV(3),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(4),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(5),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(6),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(7),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(8),  (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(9),  (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourJump(_RELDATA_RAV(10), (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourCall(_RELDATA_RAV(11), (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourJump(_RELDATA_RAV(12), (uintptr_t)&LogAssert);
										
				return true;			
			}							
			else if (verPatch == 2)		
			{							
				lpRelocator->DetourJump(_RELDATA_RAV(0), (uintptr_t)&LogWarningVa);
				lpRelocator->DetourJump(_RELDATA_RAV(1), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(2), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(3), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(4), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourCall(_RELDATA_RAV(5), (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourJump(_RELDATA_RAV(6), (uintptr_t)&LogAssert);

				return true;
			}
			else if (verPatch == 3)
			{
				lpRelocator->DetourJump(_RELDATA_RAV(0), (uintptr_t)&LogWarning);
				lpRelocator->DetourJump(_RELDATA_RAV(1), (uintptr_t)&LogWarningWithDebugInfo1Va);
				lpRelocator->DetourJump(_RELDATA_RAV(2), (uintptr_t)&LogWarningWithDebugInfo1Va);
				lpRelocator->DetourJump(_RELDATA_RAV(3), (uintptr_t)&LogWarningWithDebugInfo2);
				lpRelocator->DetourJump(_RELDATA_RAV(4), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(5), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(6), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(7), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(8), (uintptr_t)&LogWarningUnknown1);
				lpRelocator->DetourJump(_RELDATA_RAV(9), (uintptr_t)&LogWarningUnknown2);
				lpRelocator->DetourJump(_RELDATA_RAV(10), (uintptr_t)&LogWarningUnknown2);

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