// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class FixRenderPass : public Common::Patch
			{
				FixRenderPass(const FixRenderPass&) = delete;
				FixRenderPass& operator=(const FixRenderPass&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixRenderPass();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				
				static void InitSDM();
				static void KillSDM();
				static void* AllocatePass(void* Shader, void* ShaderProperty, void* Geometry,
					std::uint32_t PassEnum, std::uint8_t NumLights, void** SceneLights);
				static void DeallocatePass(void* Pass);
			};
		}
	}
}