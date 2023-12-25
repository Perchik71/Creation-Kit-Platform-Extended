// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

#include "Editor API/BSTArray.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;
			using namespace CreationKitPlatformExtended::EditorAPI;
			using namespace CreationKitPlatformExtended::EditorAPI::SkyrimSpectialEdition;

			class DeferredDlgPatch : public Module
			{
			public:
				DeferredDlgPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				template<typename T, bool Stable = false>
				static void ArrayQuickSortRecursive(BSTArray<T>& Array, 
					int(*SortFunction)(const void*, const void*))
				{
					auto compare = [SortFunction](const T& A, const T& B)
					{
						return SortFunction(A, B) == -1;
					};

					if constexpr (Stable)
						std::stable_sort(&Array[0], &Array[Array.QSize()], compare);
					else
						std::sort(&Array[0], &Array[Array.QSize()], compare);
				}

				static void SortDialogueInfo(__int64 TESDataHandler, uint32_t FormType, 
					int(*SortFunction)(const void*, const void*));
				static void UpdateTreeView(void* Thisptr, HWND ControlHandle);
				static void UpdateCellList(void* Thisptr, HWND ControlHandle, __int64 Unknown);
				static void UpdateObjectList(void* Thisptr, HWND* ControlHandle);
				static LRESULT PickScriptsToCompileDlgProc(void* This, UINT Message, WPARAM wParam, LPARAM lParam);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				DeferredDlgPatch(const DeferredDlgPatch&) = default;
				DeferredDlgPatch& operator=(const DeferredDlgPatch&) = default;
			};
		}
	}
}