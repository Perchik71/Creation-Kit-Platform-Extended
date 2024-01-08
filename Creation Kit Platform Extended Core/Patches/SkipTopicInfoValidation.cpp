// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "SkipTopicInfoValidation.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		SkipTopicInfoValidationPatch::SkipTopicInfoValidationPatch() : Module(GlobalEnginePtr)
		{}

		bool SkipTopicInfoValidationPatch::HasOption() const
		{
			return true;
		}

		bool SkipTopicInfoValidationPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* SkipTopicInfoValidationPatch::GetOptionName() const
		{
			return "CreationKit:bSkipTopicInfoValidation";
		}

		const char* SkipTopicInfoValidationPatch::GetName() const
		{
			return "Skip Topic Info Validation";
		}

		bool SkipTopicInfoValidationPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> SkipTopicInfoValidationPatch::GetDependencies() const
		{
			return {};
		}

		bool SkipTopicInfoValidationPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool SkipTopicInfoValidationPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();

			if (verPatch == 1)
			{
				//
				// Skip 'Topic Info' validation during load
				//
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xC3 });

				return true;
			}
			else if (verPatch == 2)
			{
				//
				// Skip 'Topic Info' and refs validation during load
				//
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xE9, 0xF8, 0x01, 0x00, 0x00, 0x90, 0x90 });
				
				return true;
			}

			return false;
		}

		bool SkipTopicInfoValidationPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}