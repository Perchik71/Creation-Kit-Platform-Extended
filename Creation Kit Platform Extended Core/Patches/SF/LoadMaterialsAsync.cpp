// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "LoadMaterialsAsync.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			uintptr_t pointer_LoadMaterialsAsyncPatch_sub0 = 0;

			LoadMaterialsAsyncPatch::LoadMaterialsAsyncPatch() : Module(GlobalEnginePtr)
			{}

			bool LoadMaterialsAsyncPatch::HasOption() const
			{
				return false;
			}

			bool LoadMaterialsAsyncPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* LoadMaterialsAsyncPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* LoadMaterialsAsyncPatch::GetName() const
			{
				return "Load Materials Async";
			}

			bool LoadMaterialsAsyncPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> LoadMaterialsAsyncPatch::GetDependencies() const
			{
				return {};
			}

			bool LoadMaterialsAsyncPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0;
			}

			bool LoadMaterialsAsyncPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					pointer_LoadMaterialsAsyncPatch_sub0 = _RELDATA_ADDR(0);
					lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool LoadMaterialsAsyncPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void LoadMaterialsAsyncPatch::sub(void* arg1, void* arg2)
			{
				auto mat_thread = std::thread([](void* arg1, void* arg2) {
					fastCall<void>(pointer_LoadMaterialsAsyncPatch_sub0, arg1, arg2);
					}, arg1, arg2);

				SetThreadPriority(mat_thread.native_handle(), THREAD_PRIORITY_HIGHEST);
				mat_thread.join();
			}
		}
	}
}