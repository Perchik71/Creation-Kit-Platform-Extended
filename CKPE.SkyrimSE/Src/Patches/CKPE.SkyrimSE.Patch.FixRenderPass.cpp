// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixRenderPass.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class BSRenderPass
			{
			public:
				const static int MaxLightInArrayC = 16;

				/*0x00*/ void* m_Shader;
				/*0x08*/ void* m_ShaderProperty;
				/*0x10*/ void* m_Geometry;
				/*0x18*/ std::uint32_t m_PassEnum;
				/*0x1C*/ std::uint8_t m_AccumulationHint;
				/*0x1D*/ std::uint8_t m_ExtraParam;
				struct
				{
					std::uint8_t Index : 7;					// Also referred to as "texture degrade level"
					bool SingleLevel : 1;
				} /*0x1E*/ m_LODMode;
				/*0x1F*/ std::uint8_t m_NumLights;
				/*0x20*/ std::uint16_t m_Word20;			// Not used?
				/*0x28*/ BSRenderPass* m_Next;
				/*0x30*/ BSRenderPass* m_PassGroupNext;
				/*0x38*/ void** m_SceneLights;
				/*0x40*/ std::uint32_t m_CachePoolId;		// Maximum of 2

				void SetLights(std::uint8_t NumLights, void** SceneLights)
				{
					CKPE_ASSERT_MSG(NumLights <= MaxLightInArrayC, "MaxLightInArrayC is too small");

					m_NumLights = NumLights;

					for (std::uint32_t i = 0; i < MaxLightInArrayC; i++)
						m_SceneLights[i] = nullptr;

					for (std::uint32_t i = 0; i < NumLights; i++)
						m_SceneLights[i] = SceneLights[i];
				}

				void Set(void* Shader, void* ShaderProperty, void* Geometry, std::uint32_t PassEnum,
					std::uint8_t NumLights, void** SceneLights)
				{
					m_PassEnum = PassEnum;
					m_Shader = Shader;
					m_ShaderProperty = ShaderProperty;
					m_Geometry = Geometry;
					m_AccumulationHint = 0;
					m_ExtraParam = 0;
					m_LODMode.Index = 3;
					m_LODMode.SingleLevel = false;
					m_NumLights = NumLights;
					m_Word20 = 0;
					m_Next = nullptr;
					m_PassGroupNext = nullptr;
					m_CachePoolId = 0;

					CKPE_ASSERT(m_SceneLights);
					SetLights(NumLights, SceneLights);
				}
			};
			static_assert(sizeof(BSRenderPass) == 0x48, "sizeof(BSRenderPass) != 0x48");

			FixRenderPass::FixRenderPass() : Common::Patch()
			{
				SetName("Fix RenderPass");
			}

			bool FixRenderPass::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixRenderPass::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixRenderPass::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixRenderPass::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixRenderPass::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRenderPass::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when too much geometry is present in the scene (usually with navmesh). 
				// The CK runs out of render pass cache entries.
				// Dynamically allocate them instead.
				//
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&InitSDM);
				Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&KillSDM);
				Detours::DetourJump(__CKPE_OFFSET(2), (std::uintptr_t)&AllocatePass);
				Detours::DetourJump(__CKPE_OFFSET(3), (std::uintptr_t)&DeallocatePass);

				return true;
			}

			void FixRenderPass::InitSDM()
			{
				// Intentionally left empty
			}

			void FixRenderPass::KillSDM()
			{
				// Intentionally left empty
			}

			void* FixRenderPass::AllocatePass(void* Shader, void* ShaderProperty, void* Geometry,
				std::uint32_t PassEnum, std::uint8_t NumLights, void** SceneLights)
			{
				std::uint32_t size = 0xC8;
				void* data = EditorAPI::NiAPI::NiMemoryManager::Alloc(nullptr, size, 8);
				memset(data, 0, size);

				auto pass = reinterpret_cast<BSRenderPass*>(data);
				auto lights = reinterpret_cast<void**>(&pass[1]);

				pass->m_SceneLights = lights;
				pass->Set(Shader, ShaderProperty, Geometry, PassEnum, NumLights, SceneLights);
				pass->m_CachePoolId = 0xFEFEDEAD;

				return pass;
			}

			void FixRenderPass::DeallocatePass(void* Pass)
			{
				EditorAPI::NiAPI::NiMemoryManager::Free(nullptr, Pass);
			}
		}
	}
}