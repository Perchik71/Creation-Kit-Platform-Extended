﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
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
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::Core;

			class FixRecursiveSortingPatch : public Module
			{
			public:
				FixRecursiveSortingPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				template<typename T, bool Stable = false>
				static void sub(EditorAPI::BSTArray<T>& Array, int32_t(*SortFunction)(void*, void*))
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
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				FixRecursiveSortingPatch(const FixRecursiveSortingPatch&) = default;
				FixRecursiveSortingPatch& operator=(const FixRecursiveSortingPatch&) = default;
			};
		}
	}
}