// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "PapyrusEditorLimit.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		PapyrusEditorLimitPatch::PapyrusEditorLimitPatch() : Module(GlobalEnginePtr)
		{}

		bool PapyrusEditorLimitPatch::HasOption() const
		{
			return false;
		}

		bool PapyrusEditorLimitPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* PapyrusEditorLimitPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* PapyrusEditorLimitPatch::GetName() const
		{
			return "Papyrus Editor Limit";
		}

		bool PapyrusEditorLimitPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> PapyrusEditorLimitPatch::GetDependencies() const
		{
			return {};
		}

		bool PapyrusEditorLimitPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool PapyrusEditorLimitPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				// Raise the papyrus script editor text limit to 500k characters from 64k
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

				return true;
			}
			
			return false;
		}

		bool PapyrusEditorLimitPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		bool PapyrusEditorLimitPatch::sub(HWND RichEditControl, const char* Text)
		{
			SendMessageA(RichEditControl, EM_LIMITTEXT, 500000, 0);
			return SetWindowTextA(RichEditControl, Text);
		}
	}
}