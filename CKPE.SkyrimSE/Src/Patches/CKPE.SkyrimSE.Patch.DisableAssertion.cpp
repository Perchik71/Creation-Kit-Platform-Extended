// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.DisableAssertion.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			DisableAssertion::DisableAssertion() : Common::Patch()
			{
				SetName("Disable Assertion");
			}

			bool DisableAssertion::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* DisableAssertion::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bDisableAssertions";
			}

			bool DisableAssertion::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> DisableAssertion::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool DisableAssertion::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool DisableAssertion::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool DisableAssertion::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Remove assertion message boxes
				//
				Relocation(ID(347247), 0x59).WriteFill(NOP, 5);
				Relocation(ID(346760), 0x4E).WriteFill(NOP, 5);

				return true;
			}
		}
	}
}