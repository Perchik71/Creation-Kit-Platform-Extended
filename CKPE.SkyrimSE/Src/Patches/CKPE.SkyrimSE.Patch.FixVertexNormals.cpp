// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixVertexNormals.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixVertexNormals::FixVertexNormals() : Common::Patch()
			{
				SetName("Fix Vertex Normals");
			}

			bool FixVertexNormals::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixVertexNormals::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixVertexNormals::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixVertexNormals::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixVertexNormals::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixVertexNormals::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for TESObjectLAND vertex normals appearing corrupted in worldspaces with a parent worldspace. 
				// The purpose of this code is unknown and not present in the game itself. TESObjectLAND::LoadVertices.
				//
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 2);

				//
				// Fix for TESObjectLAND vertex normals becoming corrupted when saving worldspaces with a parent worldspace. 
				// Invalid memcpy() size supplied.
				//
				SafeWrite::Write(__CKPE_OFFSET(1), { 0x41, 0xB8, 0x63, 0x03, 0x00, 0x00 });

				return true;
			}
		}
	}
}