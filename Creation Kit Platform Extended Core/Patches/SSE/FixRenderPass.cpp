// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "NiAPI/NiMemoryManager.h"
#include "FixRenderPass.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
#pragma pack(push, 8)
			class BSRenderPass
			{
			public:
				const static int MaxLightInArrayC = 16;

				/*0x00*/ void* m_Shader;
				/*0x08*/ void* m_ShaderProperty;
				/*0x10*/ void* m_Geometry;
				/*0x18*/ uint32_t m_PassEnum;
				/*0x1C*/ uint8_t m_AccumulationHint;
				/*0x1D*/ uint8_t m_ExtraParam;
				struct
				{
					uint8_t Index : 7;						// Also referred to as "texture degrade level"
					bool SingleLevel : 1;
				} /*0x1E*/ m_LODMode;
				/*0x1F*/ uint8_t m_NumLights;
				/*0x20*/ uint16_t m_Word20;					// Not used?
				/*0x28*/ BSRenderPass* m_Next;
				/*0x30*/ BSRenderPass* m_PassGroupNext;
				/*0x38*/ void** m_SceneLights;
				/*0x40*/ uint32_t m_CachePoolId;			// Maximum of 2

				void SetLights(uint8_t NumLights, void** SceneLights)
				{
					AssertMsg(NumLights <= MaxLightInArrayC, "MaxLightInArrayC is too small");

					m_NumLights = NumLights;

					for (uint32_t i = 0; i < MaxLightInArrayC; i++)
						m_SceneLights[i] = nullptr;

					for (uint32_t i = 0; i < NumLights; i++)
						m_SceneLights[i] = SceneLights[i];
				}

				void Set(void* Shader, void* ShaderProperty, void* Geometry, uint32_t PassEnum,
					uint8_t NumLights, void** SceneLights)
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

					Assert(m_SceneLights);
					SetLights(NumLights, SceneLights);
				}
			};
#pragma pack(pop)
			static_assert(sizeof(BSRenderPass) == 0x48, "sizeof(BSRenderPass) != 0x48");

			FixRenderPassPatch::FixRenderPassPatch() : Module(GlobalEnginePtr)
			{}

			bool FixRenderPassPatch::HasOption() const
			{
				return false;
			}

			bool FixRenderPassPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixRenderPassPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixRenderPassPatch::GetName() const
			{
				return "Fix RenderPass";
			}

			bool FixRenderPassPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixRenderPassPatch::GetDependencies() const
			{
				return {};
			}

			bool FixRenderPassPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRenderPassPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when too much geometry is present in the scene (usually with navmesh). 
					// The CK runs out of render pass cache entries.
					// Dynamically allocate them instead.
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&InitSDM);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&KillSDM);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2), (uintptr_t)&AllocatePass);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3), (uintptr_t)&DeallocatePass);

					return true;
				}

				return false;
			}

			bool FixRenderPassPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixRenderPassPatch::InitSDM()
			{
				// Intentionally left empty
			}

			void FixRenderPassPatch::KillSDM()
			{
				// Intentionally left empty
			}

			void* FixRenderPassPatch::AllocatePass(void* Shader, void* ShaderProperty, void* Geometry,
				uint32_t PassEnum, uint8_t NumLights, void** SceneLights)
			{
				uint32_t size = 0xC8;
				void* data = NiAPI::NiMemoryManager::Alloc(nullptr, size, 8);
				memset(data, 0, size);

				auto pass = reinterpret_cast<BSRenderPass*>(data);
				auto lights = reinterpret_cast<void**>(&pass[1]);

				pass->m_SceneLights = lights;
				pass->Set(Shader, ShaderProperty, Geometry, PassEnum, NumLights, SceneLights);
				pass->m_CachePoolId = 0xFEFEDEAD;

				return pass;
			}

			void FixRenderPassPatch::DeallocatePass(void* Pass)
			{
				NiAPI::NiMemoryManager::Free(nullptr, Pass);
			}
		}
	}
}