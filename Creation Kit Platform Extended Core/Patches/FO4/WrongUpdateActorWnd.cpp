// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "WrongUpdateActorWnd.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			WrongUpdateActorWndPatch::WrongUpdateActorWndPatch() : Module(GlobalEnginePtr)
			{}

			bool WrongUpdateActorWndPatch::HasOption() const
			{
				return false;
			}

			bool WrongUpdateActorWndPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* WrongUpdateActorWndPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* WrongUpdateActorWndPatch::GetName() const
			{
				return "Wrong Update Actor Window";
			}

			bool WrongUpdateActorWndPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> WrongUpdateActorWndPatch::GetDependencies() const
			{
				return {};
			}

			bool WrongUpdateActorWndPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool WrongUpdateActorWndPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					ScopeRelocator text;
					// ACTOR: Wrong InvalidateRect
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 6);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 0xF);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(3), 0x1D);

					return true;
				}

				return false;
			}

			bool WrongUpdateActorWndPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}