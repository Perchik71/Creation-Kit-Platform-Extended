// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Patches/ConsolePatch.h"
#include "NiCollisionObjectClonedWarning.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_NiCollisionObjectClonedWarning_sub = 0;

			NiCollisionObjectClonedWarningPatch::NiCollisionObjectClonedWarningPatch() : Module(GlobalEnginePtr)
			{}

			bool NiCollisionObjectClonedWarningPatch::HasOption() const
			{
				return false;
			}

			bool NiCollisionObjectClonedWarningPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* NiCollisionObjectClonedWarningPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* NiCollisionObjectClonedWarningPatch::GetName() const
			{
				return "NiCollisionObject Cloned Warning";
			}

			bool NiCollisionObjectClonedWarningPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool NiCollisionObjectClonedWarningPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					patch:

					//
					// Print a warning when a cloned NiCollisionObject has no name specified in its NIF file. 
					// This comes from malformed/ported game assets.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_NiCollisionObjectClonedWarning_sub = lpRelocationDatabaseItem->At(1);

					return true;
				} 
				else if (lpRelocationDatabaseItem->Version() == 2)
				{
					// Delete new code
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 0x69);
					goto patch;
				}

				return false;
			}

			bool NiCollisionObjectClonedWarningPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void NiCollisionObjectClonedWarningPatch::sub(__int64 SourceNode, __int64 DestNode, __int64 CloningProcess)
			{
				const char* sourceNodeName = *(const char**)(SourceNode + 0x10);
				__int64 sourceNodeParent = *(__int64*)(SourceNode + 0x30);

				if (*(const char**)(SourceNode + 0x40) && !sourceNodeName && sourceNodeParent)
					ConsolePatch::LogWarning(10, "Cloning a child node with collision and no name present. Parent is \"%s\".", 
						*(const char**)(sourceNodeParent + 0x10));

				((void(__fastcall*)(__int64, __int64, __int64))pointer_NiCollisionObjectClonedWarning_sub)
					(SourceNode, DestNode, CloningProcess);
			}
		}
	}
}