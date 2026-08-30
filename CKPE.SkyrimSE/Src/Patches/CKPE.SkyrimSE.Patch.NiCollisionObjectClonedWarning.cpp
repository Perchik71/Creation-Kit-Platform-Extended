// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.NiCollisionObjectClonedWarning.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TNiCollisionObjectClonedWarningSub = void(std::int64_t, std::int64_t, std::int64_t);

			static std::function<TNiCollisionObjectClonedWarningSub> NiCollisionObjectClonedWarningSub;

			NiCollisionObjectClonedWarning::NiCollisionObjectClonedWarning() : Common::Patch()
			{
				SetName("NiCollisionObject Cloned Warning");
			}

			bool NiCollisionObjectClonedWarning::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* NiCollisionObjectClonedWarning::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool NiCollisionObjectClonedWarning::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> NiCollisionObjectClonedWarning::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool NiCollisionObjectClonedWarning::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool NiCollisionObjectClonedWarning::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool NiCollisionObjectClonedWarning::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Print a warning when a cloned NiCollisionObject has no name specified in its NIF file. 
				// This comes from malformed/ported game assets.
				//		
				NiCollisionObjectClonedWarningSub = reinterpret_cast<TNiCollisionObjectClonedWarningSub*>(
					Relocation(ID{ 759144, 1017004 }, 0xC9).WriteCall(&sub));

				return true;
			}

			void NiCollisionObjectClonedWarning::sub(std::int64_t SourceNode, std::int64_t DestNode, 
				std::int64_t CloningProcess) noexcept(true)
			{
				auto sourceNodeName = *(const char**)(SourceNode + 0x10);
				auto sourceNodeParent = *(std::int64_t*)(SourceNode + 0x30);

				if (*(const char**)(SourceNode + 0x40) && !sourceNodeName && sourceNodeParent)
					Console::LogWarning(Console::MODELS, "Cloning a child node with collision and no name present. Parent is \"%s\".",
						*(const char**)(sourceNodeParent + 0x10));

				NiCollisionObjectClonedWarningSub(SourceNode, DestNode, CloningProcess);
			}
		}
	}
}