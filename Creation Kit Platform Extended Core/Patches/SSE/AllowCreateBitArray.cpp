// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "AllowCreateBitArray.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			AllowCreateBitArrayPatch::AllowCreateBitArrayPatch() : Module(GlobalEnginePtr)
			{}

			bool AllowCreateBitArrayPatch::HasOption() const
			{
				return false;
			}

			bool AllowCreateBitArrayPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* AllowCreateBitArrayPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* AllowCreateBitArrayPatch::GetName() const
			{
				return "VCBitArray";
			}

			bool AllowCreateBitArrayPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> AllowCreateBitArrayPatch::GetDependencies() const
			{
				return {};
			}

			bool AllowCreateBitArrayPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool AllowCreateBitArrayPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Cutting a lot is faster this way
					ScopeRelocator text;

					for (uint32_t i = 0; i < lpRelocationDatabaseItem->Count(); i++)
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(i), 2);

					return true;
				}

				return false;
			}

			bool AllowCreateBitArrayPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}