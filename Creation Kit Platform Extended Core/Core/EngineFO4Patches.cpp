// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "ModuleManager.h"

#include "Patches/FO4/AllowSaveESMandMasterESPF4.h"
#include "Patches/FO4/ReplaceBSPointerHandleAndManagerF4.h"
#include "Patches/FO4/TESFormRE.h"

#include "Patches/Windows/FO4/MainWindowF4.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		void Fallout4_AppendPatches(ModuleManager* PatchesManager)
		{
			namespace Patches = CreationKitPlatformExtended::Patches::Fallout4;

			PatchesManager->Append({
				new Patches::AllowSaveESMandMasterESPPatch(),
				new Patches::ReplaceBSPointerHandleAndManagerPatch(),
				new Patches::TESFormREPatch(),

				new Patches::MainWindow(),
			});
		}
	}
}