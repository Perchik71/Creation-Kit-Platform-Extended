// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixLoadD3DCompiler.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixLoadD3DCompiler::FixLoadD3DCompiler() : Common::Patch()
			{
				SetName("Fixed failed load d3dcompiler");
			}

			bool FixLoadD3DCompiler::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixLoadD3DCompiler::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixLoadD3DCompiler::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixLoadD3DCompiler::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixLoadD3DCompiler::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixLoadD3DCompiler::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					Relocation(ID(795140), 0x40).WriteFill(NOP, 6);
				}
				else
				{
					// Cutting a lot is faster this way
					auto stext = Application::GetSingleton()->GetSegment(Segment::text);
					ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

					// Fixed failed load d3dcompiler.dll
					text.WriteNop(Relocation(ID(2009774), Offset{ 0x115, 0x115, 0x12A }).Address(), 6);
					text.WriteNop(Relocation(ID(1569538), Offset{ 0x123, 0x123, 0x132 }).Address(), 6);
					text.WriteNop(Relocation(ID(2009800), Offset{ 0x3B, 0x3B, 0x2C }).Address(), 6);
				}

				return true;
			}
		}
	}
}