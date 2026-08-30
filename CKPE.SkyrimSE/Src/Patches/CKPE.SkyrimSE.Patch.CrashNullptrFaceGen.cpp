// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashNullptrFaceGen.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TCrashNullptrFaceGenSub = bool(const char*, std::uintptr_t*, std::uintptr_t*);

			static std::function<TCrashNullptrFaceGenSub> CrashNullptrFaceGenSub;

			CrashNullptrFaceGen::CrashNullptrFaceGen() : Common::Patch()
			{
				SetName("Crash Nullptr FaceGen");
			}

			bool CrashNullptrFaceGen::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashNullptrFaceGen::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashNullptrFaceGen::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashNullptrFaceGen::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashNullptrFaceGen::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashNullptrFaceGen::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashNullptrFaceGen::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// In the iterator, that second argument of the function should be a pointer, 
				// if it is null ptr in the log window you will get an ASSERT with an error on nullptr and an immediate CTD.
				// This iterator contains FaceGen parts, most often heads - class BSFaceGenMorphDataHead. 
				// These are nose, eyebrows, eyes, etc.
				//
				// I'll try to skip the code, if you have problems with the display (no eyebrows), 
				// then you have problems with the NPC's faces.
				
				CrashNullptrFaceGenSub = reinterpret_cast<TCrashNullptrFaceGenSub*>(Relocation(ID(107031), 0x7C).WriteCall(&sub));

				return true;
			}

			bool CrashNullptrFaceGen::sub(const char* path, std::uintptr_t* a2, std::uintptr_t* a3) noexcept(true)
			{
				if (!a2 || !a3) return true;
				auto Ret = CrashNullptrFaceGenSub(path, a2, a3);
				if (!*a2)
				{
					if (path)
						_CONSOLE("[ERROR] An empty FaceGen pointer was received, the path to the file: <%s>. Skip.", path);
					else
						_CONSOLE("[ERROR] Empty pointer to the model, no file path. Skip.");

					Ret = true;
				}
				return Ret;
			}
		}
	}
}