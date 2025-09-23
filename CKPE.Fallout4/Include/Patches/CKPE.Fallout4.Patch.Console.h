// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>
#include <cstdarg>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class Console : public Common::Patch
			{
				Console(const Console&) = delete;
				Console& operator=(const Console&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				enum MsgType : std::int32_t
				{
					DEFAULT,
					SYSTEM,
					COMBAT,
					ANIMATION,
					AI,
					SCRIPTS,
					SAVELOAD,
					DIALOGUE,
					QUESTS,
					PACKAGES,
					EDITOR,
					MODELS,
					TEXTURES,
					PLUGINS,
					MASTERFILE,
					FORMS,
					MAGIC,
					SHADERS,
					RENDERING,
					PATHFINDING,
					MENUS,
					AUDIO,
					CELLS,
					HAVOK,
					FACEGEN,
					WATER,
					INGAME,
					MEMORY,
					PERFORMANCE,
					LOOTJOY,
					VATS,
					DISMEMBER,
					COMPANION,
					WORKSHOP
				};

				Console();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void Log(const char* Format, ...) noexcept(true);
				static void LogVa(const char* Format, va_list& Va) noexcept(true);
				static void LogWarning(MsgType Type, const char* Format, ...) noexcept(true);
				static void LogWarningVa(MsgType Type, const char* Format, va_list& Va) noexcept(true);
				static void LogWarningUnknown1(const char* Format, ...) noexcept(true);
				static void LogWarningUnknown2(__int64 Unused, const char* Format, ...) noexcept(true);
				static void LogAssert(const char* File, int Line, const char* Message, ...) noexcept(true);
			};
		}
	}
}