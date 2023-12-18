// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashTestRadius.h"

#include "Editor API/SSE/BSEffectShaderMaterial.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashTestRadius_sub1 = 0;
			uintptr_t pointer_CrashTestRadius_sub2 = 0;

			CrashTestRadiusPatch::CrashTestRadiusPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashTestRadiusPatch::HasOption() const
			{
				return false;
			}

			bool CrashTestRadiusPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashTestRadiusPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashTestRadiusPatch::GetName() const
			{
				return "Crash Test Radius";
			}

			bool CrashTestRadiusPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashTestRadiusPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when trying to use "Test Radius" on a reference's "3D Data" dialog tab. 
					// This code wasn't correctly ported to BSGeometry from NiGeometry during the LE->SSE transition. 
					// Flags & materials need to be fixed as a result.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);
					lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0x48, 0x8B, 0xC1, 0x90, 0x90 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0x48, 0x8B, 0xC1, 0x90, 0x90 });

					pointer_CrashTestRadius_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));
					pointer_CrashTestRadius_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));

					return true;
				}

				return false;
			}

			bool CrashTestRadiusPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CrashTestRadiusPatch::sub(__int64 a1, BSShaderProperty* Property)
			{
				if (Property)
				{
					Property->ulFlags |= (1ull << BSShaderProperty::BSSP_FLAG_TWO_SIDED);		// Sphere is only 1 sided
					Property->ulFlags &= ~(1ull << BSShaderProperty::BSSP_FLAG_ZBUFFER_WRITE);	// Transparency is used

					// Fix material alpha. A copy must be made because it uses a global pointer by default.
					auto oldShaderMaterial = static_cast<BSEffectShaderMaterial*>(Property->pMaterial);
					auto newShaderMaterial = static_cast<BSEffectShaderMaterial*>(oldShaderMaterial->CreateNew());

					newShaderMaterial->CopyMembers(oldShaderMaterial);
					newShaderMaterial->kBaseColor.a = 0.5f;

					((void(__fastcall*)(BSShaderProperty*, BSEffectShaderMaterial*, bool))pointer_CrashTestRadius_sub1)(Property, newShaderMaterial, false);
				}

				((void(__fastcall*)(__int64, BSShaderProperty*))pointer_CrashTestRadius_sub2)(a1 + 0x128, Property);
			}
		}
	}
}