﻿// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <combaseapi.h>
#include <CKPE.Detours.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FlowChartX.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FlowChartX::FlowChartX() : Common::Patch()
			{
				SetName("FlowChartX Library");
			}

			bool FlowChartX::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FlowChartX::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FlowChartX::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FlowChartX::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FlowChartX::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1;
			}

			bool FlowChartX::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				if (db->GetVersion() != 1)
					return false;

				for (std::uint32_t i = 0; i < db->GetCount(); i++)
					Detours::DetourCall(__CKPE_OFFSET(i), (std::uintptr_t)&sub);

				return true;
			}

			void FlowChartX::sub() noexcept(true)
			{
				MessageBox::OpenError("Could not get the FlowChartX class object.\n"
					"Note that the Creation Kit needs to be run as administrator at least one time to register FlowChartX. "
					"If this does not help, then run the console as an administrator and run the command: regsvr32 flowchartx64.dll.\n"
					"Of course, the current directory should be a game directory.");
			}
		}
	}
}