// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TEnableStateParentWorkaround_sub1 = std::int64_t(std::int64_t);
			using TEnableStateParentWorkaround_sub2 = void(std::int64_t*, uint32_t*);
			using TEnableStateParentWorkaround_sub3 = void(std::int64_t, uint32_t*, bool);
			using TEnableStateParentWorkaround_sub4 = void(std::int64_t*);

			static std::function<TEnableStateParentWorkaround_sub1> EnableStateParentWorkaround_sub1;
			static std::function<TEnableStateParentWorkaround_sub2> EnableStateParentWorkaround_sub2;
			static std::function<TEnableStateParentWorkaround_sub3> EnableStateParentWorkaround_sub3;
			static std::function<TEnableStateParentWorkaround_sub4> EnableStateParentWorkaround_sub4;

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

			bool EnableStateParentWorkaround::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool EnableStateParentWorkaround::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool EnableStateParentWorkaround::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Workaround for "Select Enable State Parent" selecting objects outside of the current cell or worldspace
				//
				Relocation(ID(336790), 0x203).WriteCall(&sub);

				EnableStateParentWorkaround_sub1 = Relocation<TEnableStateParentWorkaround_sub1>(ID(111020)).Get();
				EnableStateParentWorkaround_sub2 = Relocation<TEnableStateParentWorkaround_sub2>(ID(597218)).Get();
				EnableStateParentWorkaround_sub3 = Relocation<TEnableStateParentWorkaround_sub3>(ID(355147)).Get();
				EnableStateParentWorkaround_sub4 = Relocation<TEnableStateParentWorkaround_sub4>(ID(356605)).Get();

				return true;
			}

			void EnableStateParentWorkaround::sub(std::int64_t RenderWindowInstance, 
				std::uint32_t* UntypedPointerHandle, bool Select) noexcept(true)
			{
				// The caller of this function already holds a reference to the pointer
				std::int64_t parentRefr = EnableStateParentWorkaround_sub1(*(std::int64_t*)(RenderWindowInstance + 0xB8));

				std::int64_t childRefr;
				EnableStateParentWorkaround_sub2(&childRefr, UntypedPointerHandle);

				if (childRefr)
				{
					// Only select child forms if they are in the same parent cell
					if (*(std::int64_t*)(childRefr + 0x70) == *(std::int64_t*)(parentRefr + 0x70))
						EnableStateParentWorkaround_sub3(RenderWindowInstance, UntypedPointerHandle, Select);
					else
						Console::LogWarning(Console::SYSTEM, 
							"Not selecting child refr (%08X) because parent cells don't match (%p != %p)\n",
							*(std::uint32_t*)(childRefr + 0x14), *(std::int64_t*)(childRefr + 0x70), 
							*(std::int64_t*)(parentRefr + 0x70));
				}

				EnableStateParentWorkaround_sub4(&childRefr);
			}
		}
	}
}