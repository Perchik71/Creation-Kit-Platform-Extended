// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.AllowCreateBitArray.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			AllowCreateBitArray::AllowCreateBitArray() : Common::Patch()
			{
				SetName("VCBitArray");
			}

			bool AllowCreateBitArray::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AllowCreateBitArray::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AllowCreateBitArray::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> AllowCreateBitArray::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AllowCreateBitArray::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool AllowCreateBitArray::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool AllowCreateBitArray::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				auto target = ID(236297);
				Relocation(target, Offset{ 0x551, 0x512 }).WriteFill(NOP, 2);
				Relocation(target, Offset{ 0x55A, 0x51B }).WriteFill(NOP, 2);
				Relocation(target, Offset{ 0x565, 0x526 }).WriteFill(NOP, 2);
				Relocation(target, Offset{ 0x578, 0x539 }).WriteFill(NOP, 2);

				return true;
			}
		}
	}
}