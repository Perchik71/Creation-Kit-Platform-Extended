// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Interfaces
			{
				class IPapyrusScriptsBase
				{
				public:
					virtual ~IPapyrusScriptsBase();
					virtual bool HasScript();	// return true
					virtual std::uint32_t MessageHandler(std::uint32_t msg, std::uintptr_t wparam, std::uintptr_t lparam);
				};
				static_assert(sizeof(IPapyrusScriptsBase) == 0x8);
			}
		}
	}
}