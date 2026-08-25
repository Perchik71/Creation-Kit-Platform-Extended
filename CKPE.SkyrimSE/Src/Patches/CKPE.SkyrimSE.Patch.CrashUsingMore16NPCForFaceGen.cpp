// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TCrashUsingMore16NPCForFaceGenSub = const char*(std::int64_t Texture);

			static std::function<TCrashUsingMore16NPCForFaceGenSub> CrashUsingMore16NPCForFaceGenSub;

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

			bool CrashUsingMore16NPCForFaceGen::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashUsingMore16NPCForFaceGen::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashUsingMore16NPCForFaceGen::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix crash when using more than 16 NPC face tint masks during FaceGen
				//
				CrashUsingMore16NPCForFaceGenSub = Relocation<TCrashUsingMore16NPCForFaceGenSub>(ID(315919)).Get();

				auto addr = Relocation(ID(105866), 0x1A0).Address();
				SafeWrite::Write(addr, { 0x48, 0x8B, 0x4C, 0x24, 0x68, 0xE8, 0xCB, 0xFF,
					0xFF, 0xFF, 0xE9, 0x7D, 0x01, 0x00, 0x00 });
				Detours::DetourCall((std::uintptr_t)addr + 5, (std::uintptr_t)&sub);

				return true;
			}

			void CrashUsingMore16NPCForFaceGen::sub(std::int64_t Texture) noexcept(true)
			{
				const char* texName = CrashUsingMore16NPCForFaceGenSub(*(std::uintptr_t*)Texture);

				Console::LogWarning(Console::FACEGEN, "Exceeded limit of 16 tint masks. Skipping texture: %s", texName);
			}
		}
	}
}