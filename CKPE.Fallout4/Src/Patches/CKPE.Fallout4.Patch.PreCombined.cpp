// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.PreCombined.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			PreCombined::PreCombined() : Common::Patch()
			{
				SetName("PreCombined");
			}

			bool PreCombined::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* PreCombined::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool PreCombined::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> PreCombined::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool PreCombined::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool PreCombined::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				std::int32_t precomb_flag = (std::int32_t)_READ_OPTION_INT("PreCombined", "nGenerationVersion", 0);

				// This of option control the output of the havok collision information generated during the precombine process, which is stuck at mode 1 unpatched.
				// This will not magically make your plugin the same format. - BenRierimanu
				// 0 - 64bit havok little endian[PC or XB1, default setting with CKPE and recommended for PRP and related patches]
				// 1 - 64bit havok big endian[PS4, default setting unpatched.Untested and not recommended unless you can somehow get the files on that platform]
				// 2 - 32bit havok little endian[PC or XB1, default setting when using the user interface generation commands as the XB1 was originally a 32bit platform at one point, will still work, but obsolete]

				SafeWrite::Write(__CKPE_OFFSET(0), (std::uint8_t*)&precomb_flag, 4);

				if (verPatch == 2)
					// push <precomb_flag>
					// pop rcx
					SafeWrite::Write(__CKPE_OFFSET(1), { 0x6A, (std::uint8_t)precomb_flag, 0x59 });
				else
					SafeWrite::Write(__CKPE_OFFSET(1), (std::uint8_t*)&precomb_flag, 4);

				SafeWrite::Write(__CKPE_OFFSET(2), (std::uint8_t*)&precomb_flag, 4);

				return true;
			}
		}
	}
}