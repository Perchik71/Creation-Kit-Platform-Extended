// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
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
				// This of option control the output of the havok collision information generated during the precombine process, which is stuck at mode 1 unpatched.
				// This will not magically make your plugin the same format. - BenRierimanu
				// 0 - 64bit havok little endian[PC or XB1, default setting with CKPE and recommended for PRP and related patches]
				// 1 - 64bit havok big endian[PS4, default setting unpatched. Untested and not recommended unless you can somehow get the files on that platform]
				// 2 - 32bit havok little endian[PC or XB1, default setting when using the user interface generation commands as the XB1 was originally a 32bit platform at one point, will still work, but obsolete]

				using namespace Common;

				auto precomb_flag_1i = static_cast<std::int32_t>(_READ_OPTION_INT("PreCombined", "nGenerationVersion", 2));
				auto precomb_flag_2i = static_cast<std::int32_t>(_READ_OPTION_INT("PreCombined", "nGenerationVersionSCOL", 1));
				auto precomb_flag_3i = static_cast<std::int32_t>(_READ_OPTION_INT("PreCombined", "nGenerationVersionMesh32bit", 1));
				auto precomb_flag_1b = static_cast<std::int8_t>(precomb_flag_1i);
				auto precomb_flag_2b = static_cast<std::int8_t>(precomb_flag_2i);
				auto precomb_flag_3b = static_cast<std::int8_t>(precomb_flag_3i);

				Relocation(ID{ 677726, 1377514 }, 0x13B).Write(std::addressof(precomb_flag_1i), 4);

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					auto rel1 = Relocation(ID(632758));
					rel1.Write<0x38>(std::addressof(precomb_flag_2b), 1);
					rel1.Write<0x44>(std::addressof(precomb_flag_2b), 1);
					rel1.Write<0x54>(std::addressof(precomb_flag_2b), 1);
					rel1.Write<0x67>(std::addressof(precomb_flag_2b), 1);
					rel1.Write<0x6E>(std::addressof(precomb_flag_2i), 4);
					rel1.Write<0x7D>(std::addressof(precomb_flag_2b), 1);

					auto rel2 = Relocation(ID(415101));
					rel2.Write<0xC7>(std::addressof(precomb_flag_3i), 4);
					rel2.Write<0xD6>(std::addressof(precomb_flag_3b), 1);
				}
				else
				{
					Relocation(ID(1462644), 0x29).Write(std::addressof(precomb_flag_2i), 4);

					auto rel1 = Relocation(ID(1990514));
					rel1.Write<0x116>(std::addressof(precomb_flag_3i), 4);
					rel1.Write<0x125>(std::addressof(precomb_flag_3b), 1);
				}

				return true;
			}
		}
	}
}