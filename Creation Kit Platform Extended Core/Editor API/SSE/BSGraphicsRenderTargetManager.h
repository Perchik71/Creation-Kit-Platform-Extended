// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSGraphicsRenderTargetManager_CK.h

#pragma once
#include "BSGraphicsTypes.h"

#define CHECK_RESULT(ReturnVar, Statement) do { (ReturnVar) = (Statement); AssertMsgVa(SUCCEEDED(ReturnVar), "Renderer target '%s' creation failed. HR = 0x%X.", Name, (ReturnVar)); } while (0)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			using namespace BSGraphics;

			static uintptr_t pointer_Renderer = 0;
			static uintptr_t pointer_D3D11Device = 0;

			template<uint32_t RenderTargetCount, uint32_t DepthStencilsCount, 
				uint32_t CubemapRenderTargetCount>
			class BSRenderer
			{
			public:
				char _pad0[0xAF0];
				RenderTargetData pRenderTargets[RenderTargetCount];
				DepthStencilData pDepthStencils[DepthStencilsCount];
				CubemapRenderTargetData pCubemapRenderTargets[CubemapRenderTargetCount];

				inline static BSRenderer* QInstance()
				{
					return reinterpret_cast<BSRenderer*>(pointer_Renderer);
				}

				void SetResourceName(ID3D11DeviceChild* Resource, const char* Format, ...)
				{
					if (!Resource)
						return;

					char buffer[1024];
					va_list va;

					va_start(va, Format);
					int len = _vsnprintf_s(buffer, _TRUNCATE, Format, va);
					va_end(va);

					Resource->SetPrivateData(WKPDID_D3DDebugObjectName, len, buffer);
				}

				void CreateRenderTarget(uint32_t TargetIndex, const char* Name, 
					const RenderTargetProperties* Properties)
				{
					Assert(TargetIndex < RenderTargetCount && TargetIndex != RENDER_TARGET_NONE);

					auto device = *reinterpret_cast<ID3D11Device**>(pointer_D3D11Device);
					auto data = &pRenderTargets[TargetIndex];

					auto hr = S_OK;
					auto dxgiFormat = static_cast<DXGI_FORMAT>(Properties->eFormat);

					if (Properties->iMipLevel == -1)
					{
						D3D11_TEXTURE2D_DESC texDesc;
						ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
						texDesc.Width = Properties->uiWidth;
						texDesc.Height = Properties->uiHeight;
						texDesc.MipLevels = (Properties->bAllowMipGeneration == false);
						texDesc.ArraySize = 1;
						texDesc.Format = dxgiFormat;
						texDesc.SampleDesc.Count = 1;
						texDesc.SampleDesc.Quality = 0;
						texDesc.Usage = D3D11_USAGE_DEFAULT;
						texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
						texDesc.CPUAccessFlags = 0;
						texDesc.MiscFlags = 0;

						if (Properties->bSupportUnorderedAccess)
							texDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

						if (Properties->bAllowMipGeneration)
							texDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

						CHECK_RESULT(hr, device->CreateTexture2D(&texDesc, nullptr, &data->Texture));
						CHECK_RESULT(hr, device->CreateRenderTargetView(data->Texture, nullptr, &data->RTV));
						CHECK_RESULT(hr, device->CreateShaderResourceView(data->Texture, nullptr, &data->SRV));

						SetResourceName(data->Texture, "%s TEX2D", Name);
						SetResourceName(data->RTV, "%s RTV", Name);
						SetResourceName(data->SRV, "%s SRV", Name);

						if (Properties->bCopyable)
						{
							texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

							CHECK_RESULT(hr, device->CreateTexture2D(&texDesc, nullptr, &data->TextureCopy));
							CHECK_RESULT(hr, device->CreateShaderResourceView(data->TextureCopy, nullptr, &data->SRVCopy));

							SetResourceName(data->TextureCopy, "%s COPY TEX2D", Name);
							SetResourceName(data->SRVCopy, "%s COPY SRV", Name);
						}

						if (Properties->bSupportUnorderedAccess)
						{
							D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
							ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
							uavDesc.Format = dxgiFormat;
							uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
							uavDesc.Texture2D.MipSlice = 0;

							CHECK_RESULT(hr, device->CreateUnorderedAccessView(data->Texture, &uavDesc, &data->UAV));

							SetResourceName(data->UAV, "%s UAV", Name);
						}
					}
					else
					{
						ID3D11Texture2D* textureTarget = pRenderTargets[Properties->uiTextureTarget].Texture;

						AssertMsg(textureTarget, "Can't create a render texture on a specified mip level because the texture has not been created.");

						D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
						ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
						rtvDesc.Format = dxgiFormat;
						rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
						rtvDesc.Texture2D.MipSlice = 0;

						CHECK_RESULT(hr, device->CreateRenderTargetView(textureTarget, &rtvDesc, &data->RTV));

						D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
						ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
						srvDesc.Format = dxgiFormat;
						srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						srvDesc.Texture2D.MostDetailedMip = Properties->iMipLevel;
						srvDesc.Texture2D.MipLevels = 1;

						CHECK_RESULT(hr, device->CreateShaderResourceView(textureTarget, &srvDesc, &data->SRV));

						if (Properties->bSupportUnorderedAccess)
						{
							D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
							ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
							uavDesc.Format = dxgiFormat;
							uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
							uavDesc.Texture2D.MipSlice = Properties->iMipLevel;

							CHECK_RESULT(hr, device->CreateUnorderedAccessView(textureTarget, &uavDesc, &data->UAV));
						}

						SetResourceName(data->RTV, "%s MIP%d RTV", Name, Properties->iMipLevel);
						SetResourceName(data->SRV, "%s MIP%d SRV", Name, Properties->iMipLevel);
						SetResourceName(data->UAV, "%s MIP%d UAV", Name, Properties->iMipLevel);
					}
				}

				void CreateDepthStencil(uint32_t TargetIndex, const char* Name, 
					const DepthStencilTargetProperties* Properties)
				{
					Assert(TargetIndex < DepthStencilsCount && TargetIndex != RENDER_TARGET_NONE);

					auto device = *reinterpret_cast<ID3D11Device**>(pointer_D3D11Device);
					auto data = &pDepthStencils[TargetIndex];
					auto hr = S_OK;

					// Create backing texture
					D3D11_TEXTURE2D_DESC texDesc;
					ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
					texDesc.Width = Properties->uiWidth;
					texDesc.Height = Properties->uiHeight;
					texDesc.MipLevels = 1;
					texDesc.ArraySize = Properties->uiArraySize;
					texDesc.SampleDesc.Count = 1;
					texDesc.SampleDesc.Quality = 0;
					texDesc.Usage = D3D11_USAGE_DEFAULT;
					texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
					texDesc.CPUAccessFlags = 0;
					texDesc.MiscFlags = 0;

					if (Properties->Use16BitsDepth)
						texDesc.Format = DXGI_FORMAT_R16_TYPELESS;
					else
						texDesc.Format = DXGI_FORMAT_R32_TYPELESS;

					if (Properties->Stencil)
						texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

					CHECK_RESULT(hr, device->CreateTexture2D(&texDesc, nullptr, &data->Texture));
					SetResourceName(data->Texture, "%s TEX2D", Name);

					// Depth stencil 1 (read / write / main)
					D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc1;
					ZeroMemory(&dsvDesc1, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
					dsvDesc1.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					dsvDesc1.Flags = 0;
					dsvDesc1.Texture2D.MipSlice = 0;

					if (Properties->Use16BitsDepth)
						dsvDesc1.Format = DXGI_FORMAT_D16_UNORM;
					else
						dsvDesc1.Format = DXGI_FORMAT_D32_FLOAT;

					if (Properties->Stencil)
						dsvDesc1.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

					// Depth stencil 2 (read only / copy)
					D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc2;
					ZeroMemory(&dsvDesc2, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
					dsvDesc2.Format = dsvDesc1.Format;
					dsvDesc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					dsvDesc2.Flags = D3D11_DSV_READ_ONLY_DEPTH;
					dsvDesc2.Texture2D.MipSlice = 0;

					if (Properties->Stencil)
						dsvDesc2.Flags |= D3D11_DSV_READ_ONLY_STENCIL;

					for (uint32_t i = 0; i < Properties->uiArraySize; i++)
					{
						if (Properties->uiArraySize > 1)
						{
							dsvDesc1.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
							dsvDesc1.Texture2DArray.MipSlice = 0;
							dsvDesc1.Texture2DArray.ArraySize = 1;
							dsvDesc1.Texture2DArray.FirstArraySlice = i;

							dsvDesc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
							dsvDesc2.Texture2DArray.MipSlice = 0;
							dsvDesc2.Texture2DArray.ArraySize = 1;
							dsvDesc2.Texture2DArray.FirstArraySlice = i;
						}

						CHECK_RESULT(hr, device->CreateDepthStencilView(data->Texture, &dsvDesc1, &data->Views[i]));
						CHECK_RESULT(hr, device->CreateDepthStencilView(data->Texture, &dsvDesc2, &data->ReadOnlyViews[i]));

						SetResourceName(data->Views[i], "%s DSV PRI SLICE%u", Name, i);
						SetResourceName(data->ReadOnlyViews[i], "%s DSV SEC SLICE%u", Name, i);
					}

					// Shader resource access
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
					srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MostDetailedMip = 0;
					srvDesc.Texture2D.MipLevels = 1;

					if (Properties->Use16BitsDepth)
						srvDesc.Format = DXGI_FORMAT_R16_UNORM;
					else
						srvDesc.Format = DXGI_FORMAT_R32_FLOAT;

					if (Properties->Stencil)
						srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

					if (Properties->uiArraySize > 1)
					{
						srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY;
						srvDesc.Texture2DArray.MostDetailedMip = 0;
						srvDesc.Texture2DArray.MipLevels = 1;
						srvDesc.Texture2DArray.FirstArraySlice = 0;
						srvDesc.Texture2DArray.ArraySize = Properties->uiArraySize;
					}

					CHECK_RESULT(hr, device->CreateShaderResourceView(data->Texture, &srvDesc, &data->DepthSRV));

					if (Properties->Stencil)
					{
						srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
						CHECK_RESULT(hr, device->CreateShaderResourceView(data->Texture, &srvDesc, &data->StencilSRV));
					}

					SetResourceName(data->DepthSRV, "%s DEPTH SRV", Name);
					SetResourceName(data->StencilSRV, "%s STENCIL SRV", Name);
				}

				void CreateCubemapRenderTarget(uint32_t TargetIndex, const char* Name, 
					const CubeMapRenderTargetProperties* Properties)
				{
					Assert(TargetIndex < CubemapRenderTargetCount);

					auto device = *reinterpret_cast<ID3D11Device**>(pointer_D3D11Device);
					auto data = &pCubemapRenderTargets[TargetIndex];
					auto hr = S_OK;

					D3D11_TEXTURE2D_DESC texDesc;
					ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
					texDesc.Width = Properties->uiWidth;
					texDesc.Height = Properties->uiHeight;
					texDesc.MipLevels = 1;
					texDesc.ArraySize = 6;
					texDesc.Format = static_cast<DXGI_FORMAT>(Properties->eFormat);
					texDesc.SampleDesc.Count = 1;
					texDesc.SampleDesc.Quality = 0;
					texDesc.Usage = D3D11_USAGE_DEFAULT;
					texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
					texDesc.CPUAccessFlags = 0;
					texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

					CHECK_RESULT(hr, device->CreateTexture2D(&texDesc, nullptr, &data->Texture));
					SetResourceName(data->Texture, "%s TEX2D", Name);

					// Create a separate render target for each side
					D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
					ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
					rtvDesc.Format = texDesc.Format;
					rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					rtvDesc.Texture2DArray.MipSlice = 0;
					rtvDesc.Texture2DArray.FirstArraySlice = 0;
					rtvDesc.Texture2DArray.ArraySize = 1;

					for (uint32_t i = 0; i < 6; i++)
					{
						rtvDesc.Texture2DArray.FirstArraySlice = i;

						CHECK_RESULT(hr, device->CreateRenderTargetView(data->Texture, &rtvDesc, &data->CubeSideRTV[i]));
						SetResourceName(data->Texture, "%s SIDE %u RTV", Name, i);
					}

					// Shader sampler
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
					srvDesc.Format = texDesc.Format;
					srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
					srvDesc.TextureCube.MostDetailedMip = 0;
					srvDesc.TextureCube.MipLevels = 1;

					CHECK_RESULT(hr, device->CreateShaderResourceView(data->Texture, &srvDesc, &data->SRV));
					SetResourceName(data->Texture, "%s SRV", Name);
				}

				void DestroyRenderTarget(uint32_t TargetIndex)
				{
					auto data = &pRenderTargets[TargetIndex];

					if (data->Texture)
						data->Texture->Release();

					if (data->TextureCopy)
						data->TextureCopy->Release();

					if (data->RTV)
						data->RTV->Release();

					if (data->SRV)
						data->SRV->Release();

					if (data->SRVCopy)
						data->SRVCopy->Release();

					if (data->UAV)
						data->UAV->Release();

					memset(data, 0, sizeof(RenderTargetData));
				}

				void DestroyDepthStencil(uint32_t TargetIndex)
				{
					auto data = &pDepthStencils[TargetIndex];

					if (data->Texture)
						data->Texture->Release();

					for (ID3D11DepthStencilView* view : data->Views)
					{
						if (view)
							view->Release();
					}

					for (ID3D11DepthStencilView* view : data->ReadOnlyViews)
					{
						if (view)
							view->Release();
					}

					if (data->DepthSRV)
						data->DepthSRV->Release();

					if (data->StencilSRV)
						data->StencilSRV->Release();

					memset(data, 0, sizeof(DepthStencilData));
				}

				void DestroyCubemapRenderTarget(uint32_t TargetIndex)
				{
					auto data = &pCubemapRenderTargets[TargetIndex];

					if (data->Texture)
						data->Texture->Release();

					for (ID3D11RenderTargetView* view : data->CubeSideRTV)
					{
						if (view)
							view->Release();
					}

					if (data->SRV)
						data->SRV->Release();

					memset(data, 0, sizeof(CubemapRenderTargetData));
				}
			};

			template<uint32_t RenderTargetCount, uint32_t DepthStencilCount,
				uint32_t CubemapRenderTargetCount, const char** RenderTargetNames, 
				const char** DepthStencilNames, const char** CubemapNames, 
				typename RendererT = BSRenderer<RenderTargetCount, DepthStencilCount, CubemapRenderTargetCount>>
			class BSGraphicsRenderTargetManager
			{
			public:
				RenderTargetProperties pRenderTargetDataA[RenderTargetCount];
				DepthStencilTargetProperties pDepthStencilTargetDataA[DepthStencilCount];
				CubeMapRenderTargetProperties pCubeMapRenderTargetDataA[CubemapRenderTargetCount];

				void CreateRenderTarget(uint32_t TargetIndex,
					const RenderTargetProperties* Properties)
				{
					AssertMsg(TargetIndex < RenderTargetCount && TargetIndex != RENDER_TARGET_NONE, "Wrong target index");
					AssertMsg(TargetIndex != 0, "Framebuffer properties come from the renderer");

					RendererT::QInstance()->DestroyRenderTarget(TargetIndex);
					pRenderTargetDataA[TargetIndex] = *Properties;
					RendererT::QInstance()->CreateRenderTarget(TargetIndex,
						GetRenderTargetName(TargetIndex), Properties);
				}

				void CreateDepthStencil(uint32_t TargetIndex,
					const DepthStencilTargetProperties* Properties)
				{
					AssertMsg(TargetIndex < DepthStencilCount && TargetIndex != DEPTH_STENCIL_TARGET_NONE, "Wrong target index");

					RendererT::QInstance()->DestroyDepthStencil(TargetIndex);
					pDepthStencilTargetDataA[TargetIndex] = *Properties;
					RendererT::QInstance()->CreateDepthStencil(TargetIndex,
						GetDepthStencilName(TargetIndex), Properties);
				}

				void CreateCubemapRenderTarget(uint32_t TargetIndex,
					const CubeMapRenderTargetProperties* Properties)
				{
					AssertMsg(TargetIndex < CubemapRenderTargetCount, "Wrong target index");

					RendererT::QInstance()->DestroyCubemapRenderTarget(TargetIndex);
					pCubeMapRenderTargetDataA[TargetIndex] = *Properties;
					RendererT::QInstance()->CreateCubemapRenderTarget(TargetIndex,
						GetCubemapRenderTargetName(TargetIndex), Properties);
				}

				const char* GetRenderTargetName(uint32_t Index)
				{
					Assert(Index < RenderTargetCount);

					if (Index == RENDER_TARGET_NONE)
						return "RENDER_TARGET_NONE";

					return RenderTargetNames[Index];
				}

				const char* GetDepthStencilName(uint32_t Index)
				{
					Assert(Index < DepthStencilCount);

					if (Index == DEPTH_STENCIL_TARGET_NONE)
						return "DEPTH_STENCIL_TARGET_NONE";

					return DepthStencilNames[Index];
				}

				const char* GetCubemapRenderTargetName(uint32_t Index)
				{
					Assert(Index < CubemapRenderTargetCount);

					if (Index == RENDER_TARGET_CUBEMAP_NONE)
						return "RENDER_TARGET_CUBEMAP_NONE";

					return CubemapNames[Index];
				}
			};

			namespace BSGraphics
			{
				namespace BSShaderRenderTargets
				{
					extern const char* Debug_RenderTargetNames[RENDER_TARGET_COUNT];
					extern const char* Debug_RenderTargetNames_1130[RENDER_TARGET_COUNT_1130];
					extern const char* Debug_DepthStencilNames[DEPTH_STENCIL_COUNT];
					extern const char* Debug_CubemapNames[RENDER_TARGET_CUBEMAP_COUNT];
				}
			}

			typedef
				BSGraphicsRenderTargetManager<RENDER_TARGET_COUNT, DEPTH_STENCIL_COUNT, RENDER_TARGET_CUBEMAP_COUNT,
				BSGraphics::BSShaderRenderTargets::Debug_RenderTargetNames, 
				BSGraphics::BSShaderRenderTargets::Debug_DepthStencilNames, 
				BSGraphics::BSShaderRenderTargets::Debug_CubemapNames>
				BSGraphicsRenderTargetManager_CK;

			typedef  
				BSGraphicsRenderTargetManager<RENDER_TARGET_COUNT_1130, DEPTH_STENCIL_COUNT, RENDER_TARGET_CUBEMAP_COUNT,
				BSGraphics::BSShaderRenderTargets::Debug_RenderTargetNames_1130, 
				BSGraphics::BSShaderRenderTargets::Debug_DepthStencilNames, 
				BSGraphics::BSShaderRenderTargets::Debug_CubemapNames>
				BSGraphicsRenderTargetManager_CK1130;
		}
	}
}