// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.DisableAssertion.h>

namespace CKPE
{
	namespace Fallout4
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool DisableAssertion::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Cutting a lot is faster this way
				auto stext = Application::GetSingleton()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				//
				// Remove assertion message boxes
				//
				text.WriteNop(Relocation(EditorAPI::IDs::Functions::sub_1FFF3E0, 0x50).Address(), 5);
				text.WriteNop(Relocation(EditorAPI::IDs::Functions::sub_1FFF470, 0x46).Address(), 5);
				text.WriteNop(Relocation(EditorAPI::IDs::Functions::sub_1FFF4F0, 0x37).Address(), 5);
				text.WriteNop(Relocation(EditorAPI::IDs::Functions::sub_1FFF570, 0x44).Address(), 5);

				return true;
			}
		}
	}
}