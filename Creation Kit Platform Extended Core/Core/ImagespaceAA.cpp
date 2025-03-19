// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "ImagespaceAA.h"
#include "resource.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		ImagespaceAA::ImagespaceAA() :
			_Init(false), _hWindow(nullptr)
		{
			_PShader = std::make_unique<D3D11PixelShader>(GlobalD3D11ShaderEngine, "PS_ImagespaceAA");
			_VShader = std::make_unique<D3D11VertexShader>(GlobalD3D11ShaderEngine, "VS_ImagespaceAA");
			_BackTex = std::make_unique<D3D11ShaderTexture>(GlobalD3D11ShaderEngine, "BackTex_ImagespaceAA");
			_BackRes = std::make_unique<D3D11ShaderResourceView>(GlobalD3D11ShaderEngine, "BackRes_ImagespaceAA");
			_BackSmp = std::make_unique<D3D11SamplerState>(GlobalD3D11ShaderEngine, "BackSmp_ImagespaceAA");
			_State   = std::make_unique<D3D11State>(GlobalD3D11ShaderEngine, "State_ImagespaceAA");
		}

		bool ImagespaceAA::Install(HWND hWindow)
		{
			if (_Init)
				return true;

			CD3D11_SAMPLER_DESC DescSmp(D3D11_DEFAULT);

			_Init =
				_PShader->LoadFromResource((HMODULE)GlobalEnginePtr->GetInstanceDLL(), IDS_FXAA_PS) &&
				_VShader->LoadFromResource((HMODULE)GlobalEnginePtr->GetInstanceDLL(), IDS_FXAA_VS) &&
				//_PShader->CompileFromFile("ps_aa.hlsl", "ps_5_0", "main") && 
				//_VShader->CompileFromFile("vs_aa.hlsl", "vs_5_0", "main") &&
				_VShader->InitData(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, {
					{ -1.0f, 1.0f }, { 0.0f, 0.0f },	// point at top-left
					{ 1.0f, 1.0f }, { 1.0f, 0.0f },		// point at top-right
					{ -1.0f, -1.0f }, { 0.0f, 1.0f },	// point at bottom-left
					{ 1.0f, -1.0f }, { 1.0f, 1.0f },	// point at bottom-right
					}) && _PShader->Install() && _VShader->Install() && _BackSmp->Create(&DescSmp);

			_hWindow = hWindow;

			return _Init;
		}

		void ImagespaceAA::Draw(IDXGISwapChain* SwapChain) const
		{
			if (!_Init) return;

			ComPtr<ID3D11Texture2D> Back;
			SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)Back.GetAddressOf());
			if (Back)
			{
				_State->PushState();

				try 
				{
					RECT ClientRect;
					GetClientRect(_hWindow, &ClientRect);

					// Setup viewport
					D3D11_VIEWPORT vp;
					ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
					vp.Width = ClientRect.right;
					vp.Height = ClientRect.bottom;
					vp.MaxDepth = 1.0f;
					GlobalD3D11ShaderEngine->DeviceContext()->RSSetViewports(1, &vp);

					// Setup shader and vertex buffers
					GlobalD3D11ShaderEngine->DeviceContext()->GSSetShader(nullptr, nullptr, 0);
					GlobalD3D11ShaderEngine->DeviceContext()->HSSetShader(nullptr, nullptr, 0);
					GlobalD3D11ShaderEngine->DeviceContext()->DSSetShader(nullptr, nullptr, 0);
					GlobalD3D11ShaderEngine->DeviceContext()->CSSetShader(nullptr, nullptr, 0);

					// Setup blend state
					const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
					GlobalD3D11ShaderEngine->DeviceContext()->OMSetBlendState(nullptr, blend_factor, 0xffffffff);
					GlobalD3D11ShaderEngine->DeviceContext()->OMSetDepthStencilState(nullptr, 0);
					GlobalD3D11ShaderEngine->DeviceContext()->RSSetState(nullptr);
					GlobalD3D11ShaderEngine->DeviceContext()->RSSetScissorRects(0, nullptr);

					_BackTex->Create(Back.Get(), D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
					CD3D11_SHADER_RESOURCE_VIEW_DESC ResDesc(
						D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
						_BackTex->GetDesc()->Format);

					_BackRes->Create(_BackTex->Get(), &ResDesc);
					_BackRes->Bind(CKPE_PIXEL_SHADER, 0);
					_BackSmp->Bind(CKPE_PIXEL_SHADER, 0);

					_VShader->Bind();
					_PShader->Bind();

					GlobalD3D11ShaderEngine->DeviceContext()->Draw(4, 0);
				}
				catch (const std::exception& e)
				{
					_ERROR("[ERROR] ImagespaceAA: %s", e.what());
				}
				
				_State->PopState();
			}
		}
	}
}