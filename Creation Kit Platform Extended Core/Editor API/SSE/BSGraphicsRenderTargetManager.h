// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSGraphicsRenderTargetManager_CK.h

#pragma once
#include "BSGraphicsTypes.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			using namespace BSGraphics;

			class BSGraphics_CK
			{
			public:
				class Renderer
				{
				public:
					char _pad0[0xAF0];
					RenderTargetData pRenderTargets[RENDER_TARGET_COUNT];
					DepthStencilData pDepthStencils[DEPTH_STENCIL_COUNT];
					CubemapRenderTargetData pCubemapRenderTargets[RENDER_TARGET_CUBEMAP_COUNT];

					static Renderer* QInstance();
					void SetResourceName(ID3D11DeviceChild* Resource, const char* Format, ...);
					void CreateRenderTarget(uint32_t TargetIndex, const char* Name, const RenderTargetProperties* Properties);
					void CreateDepthStencil(uint32_t TargetIndex, const char* Name, const DepthStencilTargetProperties* Properties);
					void CreateCubemapRenderTarget(uint32_t TargetIndex, const char* Name, const CubeMapRenderTargetProperties* Properties);

					void DestroyRenderTarget(uint32_t TargetIndex);
					void DestroyDepthStencil(uint32_t TargetIndex);
					void DestroyCubemapRenderTarget(uint32_t TargetIndex);
				};
				//static_assert_offset(Renderer, pRenderTargets, 0xAF0);
				//static_assert_offset(Renderer, pDepthStencils, 0x2050);
				//static_assert_offset(Renderer, pCubemapRenderTargets, 0x2770);
			};

			class BSGraphicsRenderTargetManager_CK
			{
			public:
				RenderTargetProperties pRenderTargetDataA[RENDER_TARGET_COUNT];
				DepthStencilTargetProperties pDepthStencilTargetDataA[DEPTH_STENCIL_COUNT];
				CubeMapRenderTargetProperties pCubeMapRenderTargetDataA[RENDER_TARGET_CUBEMAP_COUNT];

				void CreateRenderTarget(uint32_t TargetIndex, const RenderTargetProperties* Properties);
				void CreateDepthStencil(uint32_t TargetIndex, const DepthStencilTargetProperties* Properties);
				void CreateCubemapRenderTarget(uint32_t TargetIndex, const CubeMapRenderTargetProperties* Properties);

				const char* GetRenderTargetName(uint32_t Index);
				const char* GetDepthStencilName(uint32_t Index);
				const char* GetCubemapRenderTargetName(uint32_t Index);
			};
			//static_assert_offset(BSGraphicsRenderTargetManager_CK, pRenderTargetDataA, 0x0);
			//static_assert_offset(BSGraphicsRenderTargetManager_CK, pDepthStencilTargetDataA, 0xC78);
			//static_assert_offset(BSGraphicsRenderTargetManager_CK, pCubeMapRenderTargetDataA, 0xD38);
		}
	}
}