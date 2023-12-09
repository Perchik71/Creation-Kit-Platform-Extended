// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashLightingShader.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixCrashLightingShaderPatch::FixCrashLightingShaderPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashLightingShaderPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashLightingShaderPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashLightingShaderPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashLightingShaderPatch::GetName() const
			{
				return "Fix Crash Lighting Shader";
			}

			bool FixCrashLightingShaderPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashLightingShaderPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when BSLightingShader::SetupMaterial(MULTIINDEXTRISHAPESNOW) incorrectly casts a 
					// BSLightingShaderMaterial to BSLightingShaderMaterialSnow. 
					// Force the shader sparkle params to zero instead (xor xmm0, xmm1, xmm2, eax).
					//
					auto addr = lpRelocationDatabaseItem->At(0);
					lpRelocator->PatchNop(addr, 24);
					lpRelocator->Patch(addr, { 0x0F, 0x57, 0xC0, 0x0F, 0x57, 0xC9, 0x0F, 0x57, 0xD2, 0x33, 0xC0 });

					return true;
				}

				return false;
			}

			bool FixCrashLightingShaderPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}