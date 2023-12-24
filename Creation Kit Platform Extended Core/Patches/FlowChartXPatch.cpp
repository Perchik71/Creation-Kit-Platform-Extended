// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FlowChartXPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		FlowChartXPatch::FlowChartXPatch() : Module(GlobalEnginePtr)
		{}

		bool FlowChartXPatch::HasOption() const
		{
			return false;
		}

		bool FlowChartXPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* FlowChartXPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* FlowChartXPatch::GetName() const
		{
			return "FlowChartX Library";
		}

		bool FlowChartXPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> FlowChartXPatch::GetDependencies() const
		{
			return {};
		}

		bool FlowChartXPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
		}

		bool FlowChartXPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				//
				// FlowChartX needs to be registered as a COM server dll (DllRegisterServer), 
				// but it never tells you that administrator permissions are required.
				//
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub_unknown);

				return true;
			}
			else if (lpRelocationDatabaseItem->Version() == 2)
			{
				//
				// Did the developers at Bethesda find this superfluous? lol.
				// Okay...
				//

				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&HKCoGetClassObject);

				return true;
			}

			return false;
		}

		bool FlowChartXPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		void FlowChartXPatch::sub_unknown()
		{
			MessageBoxA(0, "Could not get the FlowChartX class object.\n"
				"Note that the Creation Kit needs to be run as administrator at least one time to register FlowChartX. "
				"If this does not help, then run the console as an administrator and run the command: regsvr32 flowchartx64.dll.\n"
				"Of course, the current directory should be a game directory.", "Error", 0);
		}

		HRESULT FlowChartXPatch::HKCoGetClassObject(_In_ REFCLSID rclsid, _In_ DWORD dwClsContext, _In_opt_ LPVOID pvReserved,
			_In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
		{
			HRESULT hr = CoGetClassObject(rclsid, dwClsContext, pvReserved, riid, ppv);
			if (hr) sub_unknown();
			return hr;
		}
	}
}