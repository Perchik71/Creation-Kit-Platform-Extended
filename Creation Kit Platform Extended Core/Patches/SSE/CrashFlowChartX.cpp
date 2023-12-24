// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashFlowChartX.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			CrashFlowChartXPatch::CrashFlowChartXPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashFlowChartXPatch::HasOption() const
			{
				return false;
			}

			bool CrashFlowChartXPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashFlowChartXPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashFlowChartXPatch::GetName() const
			{
				return "Crash FlowChartX";
			}

			bool CrashFlowChartXPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashFlowChartXPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashFlowChartXPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashFlowChartXPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when using FlowChartX functionality to grab current topic info id in a dialogue view. 
					// The broken code path returns a VARIANT of type VT_UI8 (21) with an invalid 8-byte pointer in the buffer. 
					// This code path is never taken in CK32.
					//
					// This hook also fixes broken graph layout where every topic would draw on top of the other.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}
				else if (lpRelocationDatabaseItem->Version() == 2)
				{
					//
					// Bethesda has changed the code, let's make a jump and omit the entire code
					//
					auto addr = lpRelocationDatabaseItem->At(0);
					lpRelocator->DetourCall(addr, (uintptr_t)&sub);
					lpRelocator->Patch((uintptr_t)addr + 5, { 0xEB, 0x7B, 0x90 });
				}

				return false;
			}

			bool CrashFlowChartXPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void* CrashFlowChartXPatch::sub(void* a1)
			{
				// Patch flowchartx64.dll every time - it's a COM dll and I have no idea if it gets reloaded
				uintptr_t flowchartBase = (uintptr_t)GetModuleHandleA("flowchartx64.dll");

				if (flowchartBase)
				{
					AssertMsg(*(uint8_t*)(flowchartBase + 0x5FF89) == 0x48 && *(uint8_t*)(flowchartBase + 0x5FF8A) == 0x8B, "Unknown FlowChartX64.dll version");

					// Prevent the XML element <Tag Type="14">16745094784</Tag> from being written
					Utils::PatchMemoryNop(flowchartBase + 0x5FF9A, 60);
				}

				// Return null so the branch directly after is never taken
				return nullptr;
			}
		}
	}
}