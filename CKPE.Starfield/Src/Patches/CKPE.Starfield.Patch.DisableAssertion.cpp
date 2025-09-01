// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.DisableAssertion.h>

namespace CKPE
{
	namespace Starfield
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

			bool DisableAssertion::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool DisableAssertion::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				//
				// Remove assertion message boxes
				//
				switch (db->GetVersion())
				{
				case 1:
					SafeWrite::WriteNop(__CKPE_OFFSET(0), 5);
					return true;
				default:
					return false;
				}

				return true;
			}
		}
	}
}