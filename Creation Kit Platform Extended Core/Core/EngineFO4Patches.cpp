// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "ModuleManager.h"

#include "Patches/FO4/AllowSaveESMandMasterESPF4.h"
#include "Patches/FO4/ReplaceBSPointerHandleAndManagerF4.h"
#include "Patches/FO4/TESFormRE.h"
#include "Patches/FO4/BSResourceLooseFilesPatch.h"
#include "Patches/FO4/TESDataHandlerHook.h"
#include "Patches/FO4/BSArchiveManagerModdedF4.h"
#include "Patches/FO4/RemoveUselessMessagesF4.h"
#include "Patches/FO4/FixVC.h"
#include "Patches/FO4/FixLoadD3DCompiler.h"
#include "Patches/FO4/FixSmoothValue.h"
#include "Patches/FO4/FixCrashNoRootMat.h"
#include "Patches/FO4/FixDeleteTintToRace.h"
#include "Patches/FO4/FixInfiniteLoopPapyrusCompile.h"

#include "Patches/Windows/FO4/MainWindowF4.h"
#include "Patches/Windows/FO4/ObjectWindowF4.h"
#include "Patches/Windows/FO4/CellViewWindowF4.h"
#include "Patches/Windows/FO4/DataWindowF4.h"

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
				new Patches::BSResourceLooseFilesPatch(),
				new Patches::TESDataHandlerPatch(),
				new Patches::BSArchiveManagerModdedPatch(),
				new Patches::RemoveUselessMessagesPatch(),
				new Patches::FixVCPatch(),
				new Patches::FixLoadD3DCompilerPatch(),
				new Patches::FixSmoothValuePatch(),
				new Patches::FixCrashNoRootMatPatch(),
				new Patches::FixDeleteTintToRacePatch(),
				new Patches::FixInfiniteLoopPapyrusCompilePatch(),
				
				new Patches::MainWindow(),
				new Patches::ObjectWindow(),
				new Patches::CellViewWindow(),
				new Patches::DataWindow(),
			});
		}
	}
}