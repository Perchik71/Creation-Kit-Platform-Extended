// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashUsingMore16NPCForFaceGen.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef const char* (*TCrashUsingMore16NPCForFaceGenSub)(std::int64_t Texture);

			static TCrashUsingMore16NPCForFaceGenSub CrashUsingMore16NPCForFaceGenSub;

			CrashUsingMore16NPCForFaceGen::CrashUsingMore16NPCForFaceGen() : Common::Patch()
			{
				SetName("Crash Using More 16 NPC For FaceGen");
			}

			bool CrashUsingMore16NPCForFaceGen::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashUsingMore16NPCForFaceGen::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashUsingMore16NPCForFaceGen::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> CrashUsingMore16NPCForFaceGen::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool CrashUsingMore16NPCForFaceGen::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashUsingMore16NPCForFaceGen::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix crash when using more than 16 NPC face tint masks during FaceGen
				//
				CrashUsingMore16NPCForFaceGenSub = (TCrashUsingMore16NPCForFaceGenSub)__CKPE_OFFSET(0);

				auto addr = __CKPE_OFFSET(1);
				SafeWrite::Write(addr, { 0x48, 0x8B, 0x4C, 0x24, 0x68, 0xE8, 0xCB, 0xFF,
					0xFF, 0xFF, 0xE9, 0x7D, 0x01, 0x00, 0x00 });
				Detours::DetourCall((uintptr_t)addr + 5, (uintptr_t)&sub);

				return true;
			}

			void CrashUsingMore16NPCForFaceGen::sub(std::int64_t Texture) noexcept(true)
			{
				const char* texName = CrashUsingMore16NPCForFaceGenSub(*(__int64*)Texture);

				Console::LogWarning(23, "Exceeded limit of 16 tint masks. Skipping texture: %s", texName);
			}
		}
	}
}