// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashNullptrFaceGen.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashNullptrFaceGen_sub = 0;

			CrashNullptrFaceGenPatch::CrashNullptrFaceGenPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashNullptrFaceGenPatch::HasOption() const
			{
				return false;
			}

			bool CrashNullptrFaceGenPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashNullptrFaceGenPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashNullptrFaceGenPatch::GetName() const
			{
				return "Crash Nullptr FaceGen";
			}

			bool CrashNullptrFaceGenPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashNullptrFaceGenPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashNullptrFaceGenPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashNullptrFaceGenPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// In the iterator, that second argument of the function should be a pointer, 
					// if it is null ptr in the log window you will get an ASSERT with an error on nullptr and an immediate CTD.
					// This iterator contains FaceGen parts, most often heads - class BSFaceGenMorphDataHead. 
					// These are nose, eyebrows, eyes, etc.
					//
					// I'll try to skip the code, if you have problems with the display (no eyebrows), 
					// then you have problems with the NPC's faces.

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_CrashNullptrFaceGen_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool CrashNullptrFaceGenPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool CrashNullptrFaceGenPatch::sub(const char* path, uintptr_t* a2, uintptr_t* a3)
			{
				if (!a2 || !a3) return true;
				auto Ret = fastCall<bool>(pointer_CrashNullptrFaceGen_sub, path, a2, a3);
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