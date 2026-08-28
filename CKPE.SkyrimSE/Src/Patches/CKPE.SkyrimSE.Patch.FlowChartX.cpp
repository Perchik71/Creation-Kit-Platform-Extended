// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <combaseapi.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FlowChartX.h>

namespace CKPE
{
	namespace SkyrimSE
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

			bool FlowChartX::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FlowChartX::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FlowChartX::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				if (VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1130)
				{
					Relocation(ID(238521), 0x851).WriteCall(&sub);
					Relocation(ID(95666), 0x7ED).WriteCall(&sub);
				}
				else
					Relocation(ID(233736), 0xA5).WriteCall(&sub);
				
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