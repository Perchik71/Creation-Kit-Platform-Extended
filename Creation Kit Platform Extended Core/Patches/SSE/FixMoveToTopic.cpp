// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixMoveToTopic.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixMoveToTopicPatch::FixMoveToTopicPatch() : Module(GlobalEnginePtr)
			{}

			bool FixMoveToTopicPatch::HasOption() const
			{
				return false;
			}

			bool FixMoveToTopicPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixMoveToTopicPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixMoveToTopicPatch::GetName() const
			{
				return "Fix Move to topic";
			}

			bool FixMoveToTopicPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixMoveToTopicPatch::GetDependencies() const
			{
				return {};
			}

			bool FixMoveToTopicPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixMoveToTopicPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when using "Move to topic" in a quest dialogue view. 
					// Any unresolved/unused Topic actions default to "Unknown action",
					// but a null pointer is used while trying to get the type.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 5);

					return true;
				}

				return false;
			}

			bool FixMoveToTopicPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}