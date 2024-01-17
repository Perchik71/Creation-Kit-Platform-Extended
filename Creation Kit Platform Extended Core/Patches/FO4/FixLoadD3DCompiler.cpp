// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixLoadD3DCompiler.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixLoadD3DCompilerPatch::FixLoadD3DCompilerPatch() : Module(GlobalEnginePtr)
			{}

			bool FixLoadD3DCompilerPatch::HasOption() const
			{
				return false;
			}

			bool FixLoadD3DCompilerPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixLoadD3DCompilerPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixLoadD3DCompilerPatch::GetName() const
			{
				return "Fixed failed load d3dcompiler";
			}

			bool FixLoadD3DCompilerPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixLoadD3DCompilerPatch::GetDependencies() const
			{
				return {};
			}

			bool FixLoadD3DCompilerPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixLoadD3DCompilerPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fixed failed load d3dcompiler.dll
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);

					return true;
				}

				return false;
			}

			bool FixLoadD3DCompilerPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}