// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashLightingShader.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixCrashLightingShader::FixCrashLightingShader() : Common::Patch()
			{
				SetName("Fix Crash Lighting Shader");
			}

			bool FixCrashLightingShader::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashLightingShader::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashLightingShader::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashLightingShader::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashLightingShader::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashLightingShader::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when BSLightingShader::SetupMaterial(MULTIINDEXTRISHAPESNOW) incorrectly casts a 
				// BSLightingShaderMaterial to BSLightingShaderMaterialSnow. 
				// Force the shader sparkle params to zero instead (xor xmm0, xmm1, xmm2, eax).
				//
				auto addr = __CKPE_OFFSET(0);
				SafeWrite::WriteNop(addr, 24);
				SafeWrite::Write(addr, { 0x0F, 0x57, 0xC0, 0x0F, 0x57, 0xC9, 0x0F, 0x57, 0xD2, 0x33, 0xC0 });

				return true;
			}
		}
	}
}