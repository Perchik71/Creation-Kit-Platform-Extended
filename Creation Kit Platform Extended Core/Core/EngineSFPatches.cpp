// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "ModuleManager.h"

#include "Patches/SF/TESFormRESF.h"
#include "Patches/SF/ReplaceImgQSplashWnd.h"
#include "Patches/SF/RunAppWithoutNet.h"
#include "Patches/SF/TESDataHandlerHookSF.h"
#include "Patches/SF/TESPointerHandleDetailSF.h"
#include "Patches/SF/UIDefferSF.h"
#include "Patches/SF/RTDynamicCastCrash.h"
#include "Patches/SF/OptimizationLoadSF.h"
#include "Patches/SF/RemoveUselessMessagesSF.h"

#include "Patches/Windows/SF/DataWindowSF.h"
#include "Patches/Windows/SF/AboutWindowSF.h"
#include "Patches/Windows/SF/MainWindowSF.h"
#include "Patches/Windows/SF/ObjectWindowSF.h"
#include "Patches/Windows/SF/ProgressWindowSF.h"
#include "Patches/Windows/SF/CellViewWindowSF.h"
#include "Patches/Windows/SF/LayersWindowSF.h"
#include "Patches/Windows/SF/RenderWindowSF.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		void Starfield_AppendPatches(ModuleManager* PatchesManager)
		{
			namespace Patches = CreationKitPlatformExtended::Patches::Starfield;

			PatchesManager->Append({
				new Patches::TESFormREPatch(),
				new Patches::ReplaceImgQSplashWndPatch(),
				new Patches::RunAppWithoutNetPatch(),
				new Patches::TESDataHandlerPatch(),
				new Patches::TESPointerHandleDetailSFPatch(),
				new Patches::UIDefferPatch(),
				new Patches::RTDynamicCastCrashPatch(),
				new Patches::OptimizationLoadPatch(),
				new Patches::RemoveUselessMessagesPatch(),

				new Patches::DataWindow(),
				new Patches::AboutWindow(),
				new Patches::MainWindow(),
				new Patches::ObjectWindow(),
				new Patches::ProgressWindow(),
				new Patches::CellViewWindow(),
				new Patches::LayersWindow(),
				//new Patches::RenderWindow(),
			});
		}
	}
}