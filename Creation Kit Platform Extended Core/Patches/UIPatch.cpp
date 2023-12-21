// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "UIPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::EditorAPI;

		UIPatch::UIPatch() : Module(GlobalEnginePtr)
		{}

		bool UIPatch::HasOption() const
		{
			return false;
		}

		bool UIPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* UIPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* UIPatch::GetName() const
		{
			return "UI";
		}

		bool UIPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool UIPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				//
				// UI
				//
				PatchIAT(EditorUI::HKCreateDialogParamA, "USER32.DLL", "CreateDialogParamA");
				PatchIAT(EditorUI::HKDialogBoxParamA, "USER32.DLL", "DialogBoxParamA");
				PatchIAT(EditorUI::HKEndDialog, "USER32.DLL", "EndDialog");
				PatchIAT(EditorUI::HKSendMessageA, "USER32.DLL", "SendMessageA");

				return true;
			}
			
			return false;
		}

		bool UIPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}