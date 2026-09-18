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
			namespace PreGenCombined
			{
				enum class Mode
				{
					kGenerateForPC = 0,
					kGenerateForPS4 = 2,
				};

				static Mode GenerateMode = Mode::kGenerateForPC;
				static std::uint32_t* PreGenCombinedMode_orig = nullptr;

				static void SetPreGenCombinedMode() noexcept(true)
				{
					if (PreGenCombinedMode_orig)
						*PreGenCombinedMode_orig = std::to_underlying(GenerateMode);
				}
			};

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
				using namespace Common;

				std::int32_t precomb_flag = std::to_underlying(PreGenCombined::Mode::kGenerateForPC);
				auto precomb_mode = _READ_OPTION_INT("PreCombined", "bGenerateForPC", true);
				if (!precomb_mode)
				{
					PreGenCombined::GenerateMode = PreGenCombined::Mode::kGenerateForPS4;
					precomb_flag = std::to_underlying(PreGenCombined::GenerateMode);
				}
				auto precomb_flag_b = static_cast<std::int8_t>(precomb_flag);

				if (PreGenCombined::GenerateMode == PreGenCombined::Mode::kGenerateForPS4)
					// For PS4 no needs patch
					return true;

				Relocation(ID{ 63416, 1696443 }).WriteJump(&PreGenCombined::SetPreGenCombinedMode);
				PreGenCombined::PreGenCombinedMode_orig = Relocation<std::uint32_t*>(ID{ 975208, 1623094 }).Get();

				Relocation(ID{ 677726, 1377514 }, 0x13B).Write(std::addressof(precomb_flag), 4);

				if (VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					Relocation(ID(1462644), 0x29).Write(std::addressof(precomb_flag), 4);

					auto rel1 = Relocation(ID(1990514));
					rel1.Write<0x116>(std::addressof(precomb_flag), 4);
					rel1.Write<0x125>(std::addressof(precomb_flag_b), 1);
				}
				else
				{
					auto rel1 = Relocation(ID(632758));
					rel1.Write<0x38>(std::addressof(precomb_flag_b), 1);
					rel1.Write<0x44>(std::addressof(precomb_flag_b), 1);
					rel1.Write<0x54>(std::addressof(precomb_flag_b), 1);
					rel1.Write<0x67>(std::addressof(precomb_flag_b), 1);
					rel1.Write<0x6E>(std::addressof(precomb_flag), 4);
					rel1.Write<0x7D>(std::addressof(precomb_flag_b), 1);

					auto rel2 = Relocation(ID(415101));
					rel2.Write<0xC7>(std::addressof(precomb_flag), 4);
					rel2.Write<0xD6>(std::addressof(precomb_flag_b), 1);
				}

				return true;
			}
		}
	}
}