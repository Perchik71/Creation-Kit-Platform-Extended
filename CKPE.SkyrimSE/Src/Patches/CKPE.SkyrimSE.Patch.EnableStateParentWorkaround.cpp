// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.EnableStateParentWorkaround.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_EnableStateParentWorkaround_sub1 = 0;
			std::uintptr_t pointer_EnableStateParentWorkaround_sub2 = 0;
			std::uintptr_t pointer_EnableStateParentWorkaround_sub3 = 0;
			std::uintptr_t pointer_EnableStateParentWorkaround_sub4 = 0;

			EnableStateParentWorkaround::EnableStateParentWorkaround() : Common::Patch()
			{
				SetName("Enable State Parent Workaround");
			}

			bool EnableStateParentWorkaround::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* EnableStateParentWorkaround::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bEnableStateParentWorkaround";
			}

			bool EnableStateParentWorkaround::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> EnableStateParentWorkaround::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool EnableStateParentWorkaround::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool EnableStateParentWorkaround::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Workaround for "Select Enable State Parent" selecting objects outside of the current cell or worldspace
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&sub);

				pointer_EnableStateParentWorkaround_sub1 = __CKPE_OFFSET(1);
				pointer_EnableStateParentWorkaround_sub2 = __CKPE_OFFSET(2);
				pointer_EnableStateParentWorkaround_sub3 = __CKPE_OFFSET(3);
				pointer_EnableStateParentWorkaround_sub4 = __CKPE_OFFSET(4);

				return true;
			}

			void EnableStateParentWorkaround::sub(std::int64_t RenderWindowInstance, 
				std::uint32_t* UntypedPointerHandle, bool Select) noexcept(true)
			{
				// The caller of this function already holds a reference to the pointer
				std::int64_t parentRefr = ((std::int64_t(__fastcall*)(std::int64_t))
					pointer_EnableStateParentWorkaround_sub1)(*(std::int64_t*)(RenderWindowInstance + 0xB8));

				std::int64_t childRefr;
				((void(__fastcall*)(std::int64_t*, uint32_t*))pointer_EnableStateParentWorkaround_sub2)
					(&childRefr, UntypedPointerHandle);

				if (childRefr)
				{
					// Only select child forms if they are in the same parent cell
					if (*(std::int64_t*)(childRefr + 0x70) == *(std::int64_t*)(parentRefr + 0x70))
						((void(__fastcall*)(std::int64_t, uint32_t*, bool))pointer_EnableStateParentWorkaround_sub3)
						(RenderWindowInstance, UntypedPointerHandle, Select);
					else
						Console::LogWarning(Console::SYSTEM, 
							"Not selecting child refr (%08X) because parent cells don't match (%p != %p)\n",
							*(std::uint32_t*)(childRefr + 0x14), *(std::int64_t*)(childRefr + 0x70), 
							*(std::int64_t*)(parentRefr + 0x70));
				}

				((void(__fastcall*)(std::int64_t*))pointer_EnableStateParentWorkaround_sub4)(&childRefr);
			}
		}
	}
}