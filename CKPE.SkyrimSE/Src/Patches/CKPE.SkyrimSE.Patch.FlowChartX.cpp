// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <combaseapi.h>
#include <CKPE.Detours.h>
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

			bool FlowChartX::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FlowChartX::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				switch (db->GetVersion())
				{
				case 1:
					//
					// FlowChartX needs to be registered as a COM server dll (DllRegisterServer), 
					// but it never tells you that administrator permissions are required.
					//
					Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&sub);
					return true;
				case 2:
					//
					// Did the developers at Bethesda find this superfluous? lol.
					// Okay...
					//
					Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&HKCoGetClassObject);
					return true;
				default:
					return false;
				}

				return true;
			}

			void FlowChartX::sub() noexcept(true)
			{
				MessageBox::OpenError("Could not get the FlowChartX class object.\n"
					"Note that the Creation Kit needs to be run as administrator at least one time to register FlowChartX. "
					"If this does not help, then run the console as an administrator and run the command: regsvr32 flowchartx64.dll.\n"
					"Of course, the current directory should be a game directory.");
			}

			std::int64_t FlowChartX::HKCoGetClassObject(void* rclsid, std::uint32_t dwClsContext, void* pvReserved,
				void* riid, void** ppv) noexcept(true)
			{
				HRESULT hr = CoGetClassObject((REFCLSID)rclsid, dwClsContext, pvReserved, (REFIID)riid, ppv);
				if (FAILED(hr)) sub();
				return hr;
			}
		}
	}
}