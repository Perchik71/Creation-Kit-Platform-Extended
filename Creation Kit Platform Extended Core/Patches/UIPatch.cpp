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

		bool UIPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> UIPatch::GetDependencies() const
		{
			return {};
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
				PatchIAT(EditorUI::HKCreateWindowA, "USER32.DLL", "CreateWindowA");
				PatchIAT(EditorUI::HKCreateWindowExA, "USER32.DLL", "CreateWindowExA");
				PatchIAT(EditorUI::HKDestroyWindow, "USER32.DLL", "DestroyWindow");

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