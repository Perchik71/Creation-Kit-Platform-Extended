// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.AllowCreateBitArray.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			AllowCreateBitArray::AllowCreateBitArray() : Common::Patch()
			{
				SetName("VCBitArray");
			}

			bool AllowCreateBitArray::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AllowCreateBitArray::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AllowCreateBitArray::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> AllowCreateBitArray::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AllowCreateBitArray::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool AllowCreateBitArray::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();
				
				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				for (std::uint32_t i = 0; i < db->GetCount(); i++)
					text.WriteNop(__CKPE_OFFSET(i), 2);

				return true;
			}
		}
	}
}