// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixSmoothValue.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixSmoothValue::FixSmoothValue() : Common::Patch()
			{
				SetName("Fixed Smoothness value to material");
			}

			bool FixSmoothValue::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixSmoothValue::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixSmoothValue::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixSmoothValue::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixSmoothValue::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixSmoothValue::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				const auto target = ID{ 718656, 1601882 };

				// Fixed when the value is different from 0.0 to 1.0. Smoothness value to material (nif)
				Relocation(target, 0xB7).WriteCall(&sub);
				Relocation(target, 0xBC).Write({ 0x66, 0x0F, 0x7E, 0x85, 0x88, 0x00, 0x00, 0x00, 0xEB, 0x18 });

				return true;
			}

			float FixSmoothValue::sub(float a1, float a2) noexcept(true)
			{
				return (a2 > 1.0) ? 1.0 : ((a2 < 0.0) ? 0.0 : a2);
			}
		}
	}
}