// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixNiSkinInstance.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer1 = 0;

			FixNiSkinInstancePatch::FixNiSkinInstancePatch() : Module(GlobalEnginePtr)
			{}

			bool FixNiSkinInstancePatch::HasOption() const
			{
				return false;
			}

			bool FixNiSkinInstancePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixNiSkinInstancePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixNiSkinInstancePatch::GetName() const
			{
				return "NiSkinInstance Fix";
			}

			bool FixNiSkinInstancePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixNiSkinInstancePatch::GetDependencies() const
			{
				return {};
			}

			bool FixNiSkinInstancePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixNiSkinInstancePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					voltek::detours_function_class_vtable(_RELDATA_ADDR(0), &NiSkinInstance_LinkObject, 25);
					voltek::detours_function_class_vtable(_RELDATA_ADDR(1), &NiSkinInstance_LinkObject, 25);
					pointer1 = _RELDATA_ADDR(2);

					return true;
				}

				return false;
			}

			bool FixNiSkinInstancePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixNiSkinInstancePatch::NiSkinInstance_LinkObject(__int64 SkinInstance, __int64 Stream)
			{
				((void(__fastcall*)(__int64, __int64))pointer1)(SkinInstance, Stream);

				// SkinInstance->RootParent can't be null
				__int64 rootParent = *(__int64*)(SkinInstance + 0x20);
				const char* nifPath = (const char*)(Stream + 0x108);

				AssertMsgVa(rootParent, "A mesh's NiSkinInstance is missing a skeleton root node. This is a fatal error. NIF path is \"%s\".", nifPath);
			}
		}
	}
}