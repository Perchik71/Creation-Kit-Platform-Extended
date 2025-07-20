// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
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
			typedef void(*TNiCollisionObjectClonedWarningSub)(std::int64_t, std::int64_t, std::int64_t);

			static TNiCollisionObjectClonedWarningSub NiCollisionObjectClonedWarningSub;

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

			bool NiCollisionObjectClonedWarning::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool NiCollisionObjectClonedWarning::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Print a warning when a cloned NiCollisionObject has no name specified in its NIF file. 
				// This comes from malformed/ported game assets.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				NiCollisionObjectClonedWarningSub = (TNiCollisionObjectClonedWarningSub)__CKPE_OFFSET(1);

				return true;
			}

			void NiCollisionObjectClonedWarning::sub(std::int64_t SourceNode, std::int64_t DestNode, 
				std::int64_t CloningProcess) noexcept(true)
			{
				const char* sourceNodeName = *(const char**)(SourceNode + 0x10);
				std::int64_t sourceNodeParent = *(std::int64_t*)(SourceNode + 0x30);

				if (*(const char**)(SourceNode + 0x40) && !sourceNodeName && sourceNodeParent)
					Console::LogWarning(10, "Cloning a child node with collision and no name present. Parent is \"%s\".",
						*(const char**)(sourceNodeParent + 0x10));

				NiCollisionObjectClonedWarningSub(SourceNode, DestNode, CloningProcess);
			}
		}
	}
}