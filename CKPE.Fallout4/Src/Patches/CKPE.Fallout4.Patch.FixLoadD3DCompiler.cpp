// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
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
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				// Fixed failed load d3dcompiler.dll
				for (std::uint32_t nId = 0; nId < db->GetCount(); nId++)
					text.WriteNop(__CKPE_OFFSET(nId), 6);

				return true;
			}
		}
	}
}