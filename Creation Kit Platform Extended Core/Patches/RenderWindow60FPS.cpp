// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RenderWindow60FPS.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		RenderWindow60FPSPatch::RenderWindow60FPSPatch() : Module(GlobalEnginePtr)
		{}

		bool RenderWindow60FPSPatch::HasOption() const
		{
			return true;
		}

		bool RenderWindow60FPSPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* RenderWindow60FPSPatch::GetOptionName() const
		{
			return "CreationKit:bRenderWindow60FPS";
		}

		const char* RenderWindow60FPSPatch::GetName() const
		{
			return "Render Window 60 FPS";
		}

		bool RenderWindow60FPSPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> RenderWindow60FPSPatch::GetDependencies() const
		{
			return {};
		}

		bool RenderWindow60FPSPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool RenderWindow60FPSPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();
			if ((verPatch == 1) || (verPatch == 2))
			{
				//
				// Force render window to draw at 60fps (SetTimer(10ms))
				//
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { USER_TIMER_MINIMUM });

				if (verPatch == 2)
					// no VSync
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0x33, 0xD2, 0x90 });

				return true;
			} 

			return false;
		}

		bool RenderWindow60FPSPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}