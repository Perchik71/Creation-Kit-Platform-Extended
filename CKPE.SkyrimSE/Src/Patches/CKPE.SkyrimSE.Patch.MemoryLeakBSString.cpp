// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
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

			bool MemoryLeakBSString::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MemoryLeakBSString::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// According to the indications of trace...
				// Bethesda does not free up memory when adding to the end of a string.
				// A stupid mistake, it seems to have been done on purpose, out of inexperience.
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&Append);

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