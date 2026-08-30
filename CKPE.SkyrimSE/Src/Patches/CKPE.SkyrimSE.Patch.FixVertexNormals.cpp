// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool FixVertexNormals::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixVertexNormals::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixVertexNormals::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for TESObjectLAND vertex normals appearing corrupted in worldspaces with a parent worldspace. 
				// The purpose of this code is unknown and not present in the game itself. TESObjectLAND::LoadVertices.
				//
				Relocation(ID(338273), 0x177).WriteFill(NOP, 2);

				//
				// Fix for TESObjectLAND vertex normals becoming corrupted when saving worldspaces with a parent worldspace. 
				// Invalid memcpy() size supplied.
				//
				Relocation(ID(60495), 0x146).Write({ 0x41, 0xB8, 0x63, 0x03, 0x00, 0x00 });

				return true;
			}
		}
	}
}