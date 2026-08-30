// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSString.h>
#include <Patches/CKPE.SkyrimSE.Patch.MemoryLeakBSString.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			MemoryLeakBSString::MemoryLeakBSString() : Common::Patch()
			{
				SetName("Memory Leak class BSString");
			}

			bool MemoryLeakBSString::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* MemoryLeakBSString::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool MemoryLeakBSString::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> MemoryLeakBSString::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool MemoryLeakBSString::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool MemoryLeakBSString::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MemoryLeakBSString::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// According to the indications of trace...
				// Bethesda does not free up memory when adding to the end of a string.
				// A stupid mistake, it seems to have been done on purpose, out of inexperience.
				Relocation(ID(551397)).WriteJump(&Append);

				return true;
			}

			void* MemoryLeakBSString::Append(void* DestStr, const char* SrcStr) noexcept(true)
			{
				if (!SrcStr || !DestStr)
					return DestStr;

				((EditorAPI::BSString*)DestStr)->Append(SrcStr);
				return DestStr;
			}
		}
	}
}