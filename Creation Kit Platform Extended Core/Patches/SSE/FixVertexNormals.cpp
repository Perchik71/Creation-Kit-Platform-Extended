// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixVertexNormals.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixVertexNormalsPatch::FixVertexNormalsPatch() : Module(GlobalEnginePtr)
			{}

			bool FixVertexNormalsPatch::HasOption() const
			{
				return false;
			}

			bool FixVertexNormalsPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixVertexNormalsPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixVertexNormalsPatch::GetName() const
			{
				return "Fix Vertex Normals";
			}

			bool FixVertexNormalsPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixVertexNormalsPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for TESObjectLAND vertex normals appearing corrupted in worldspaces with a parent worldspace. 
					// The purpose of this code is unknown and not present in the game itself. TESObjectLAND::LoadVertices.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 2);

					//
					// Fix for TESObjectLAND vertex normals becoming corrupted when saving worldspaces with a parent worldspace. 
					// Invalid memcpy() size supplied.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0x41, 0xB8, 0x63, 0x03, 0x00, 0x00 });

					return true;
				}

				return false;
			}

			bool FixVertexNormalsPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}