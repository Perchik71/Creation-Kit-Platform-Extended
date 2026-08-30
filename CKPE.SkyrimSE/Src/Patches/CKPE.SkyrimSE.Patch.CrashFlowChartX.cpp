// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashFlowChartX.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef void* (*TCrashFlowChartXSub)(void*);

			static TCrashFlowChartXSub CrashFlowChartXSub;

			CrashFlowChartX::CrashFlowChartX() : Common::Patch()
			{
				SetName("Crash FlowChartX");
			}

			bool CrashFlowChartX::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashFlowChartX::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashFlowChartX::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashFlowChartX::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashFlowChartX::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashFlowChartX::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashFlowChartX::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				if (VersionLists::GetEditorVersion() < VersionLists::EDITOR_SKYRIM_SE_1_6_1130)
					//
					// Fix for crash when using FlowChartX functionality to grab current topic info id in a dialogue view. 
					// The broken code path returns a VARIANT of type VT_UI8 (21) with an invalid 8-byte pointer in the buffer. 
					// This code path is never taken in CK32.
					//
					// This hook also fixes broken graph layout where every topic would draw on top of the other.
					//
					Relocation(ID(347454), 0x1F).WriteCall(&sub);
				else
					// Bethesda has changed the code, let's make a jump and omit the entire code
					Relocation(ID(347454)).WriteJump(&sub);

				return true;
			}

			void* CrashFlowChartX::sub([[maybe_unused]] void* a1) noexcept(true)
			{
				// Patch flowchartx64.dll every time - it's a COM dll and I have no idea if it gets reloaded
				auto flowchartBase = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("flowchartx64.dll"));
				if (flowchartBase)
				{
					CKPE_ASSERT_MSG(*(std::uint8_t*)(flowchartBase + 0x5FF89) == 0x48 && *(std::uint8_t*)(flowchartBase + 0x5FF8A) == 0x8B,
						"Unknown FlowChartX64.dll version");

					// Prevent the XML element <Tag Type="14">16745094784</Tag> from being written
					SafeWrite::WriteNop(flowchartBase + 0x5FF9A, 60);
				}

				// Return null so the branch directly after is never taken
				return nullptr;
			}
		}
	}
}