// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "ModuleManager.h"

#include "Patches/SF/TESFormRESF.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		void Starfield_AppendPatches(ModuleManager* PatchesManager)
		{
			namespace Patches = CreationKitPlatformExtended::Patches::Starfield;

			PatchesManager->Append({
				new Patches::TESFormREPatch(),
			});
		}
	}
}