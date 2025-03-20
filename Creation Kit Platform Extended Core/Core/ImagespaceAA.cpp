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
			_Init(false), _hWindow(nullptr), _Enabled(true)
		{
			_PShaderEffectFXAA = std::make_unique<D3D11PixelShader>(GlobalD3D11ShaderEngine, "PS_FXAA_ImagespaceAA");
			_PShaderEffectTAA = std::make_unique<D3D11PixelShader>(GlobalD3D11ShaderEngine, "PS_TAA_ImagespaceAA");
			_PostFrame = std::make_unique<D3D11ShaderTexture>(GlobalD3D11ShaderEngine, "PostFrame_ImagespaceAA");
			_VShader = std::make_unique<D3D11VertexShader>(GlobalD3D11ShaderEngine, "VS_ImagespaceAA");
			_BackRes = std::make_unique<D3D11ShaderResourceView>(GlobalD3D11ShaderEngine, "BackRes_ImagespaceAA");
			_BackSmp = std::make_unique<D3D11SamplerState>(GlobalD3D11ShaderEngine, "BackSmp_ImagespaceAA");
			_State = std::make_unique<D3D11State>(GlobalD3D11ShaderEngine, "State_ImagespaceAA");
			_Frame = std::make_unique<D3D11ShaderTexture>(GlobalD3D11ShaderEngine, "Frame_ImagespaceAA");
			_RTVFrame = std::make_unique<D3D11RenderTargetView>(GlobalD3D11ShaderEngine, "RTVFrame_ImagespaceAA");
			_DataBuffer = std::make_unique<D3D11ShaderBuffer>(GlobalD3D11ShaderEngine, "DataBuffer_ImagespaceAA");
		}

		bool ImagespaceAA::Install(HWND hWindow)
		{
			if (_Init)
				return true;

			CD3D11_SAMPLER_DESC DescSmp(D3D11_DEFAULT);

			_Init =
				_PShaderEffectFXAA->LoadFromResource((HMODULE)GlobalEnginePtr->GetInstanceDLL(), IDS_FXAA_PS) &&
				_PShaderEffectTAA->LoadFromResource((HMODULE)GlobalEnginePtr->GetInstanceDLL(), IDS_TAA_PS) &&
				_VShader->LoadFromResource((HMODULE)GlobalEnginePtr->GetInstanceDLL(), IDS_FXAA_VS) &&
				//_PShader->CompileFromFile("ps_aa.hlsl", "ps_5_0", "main") && 
				//_VShader->CompileFromFile("vs_aa.hlsl", "vs_5_0", "main") &&
				_VShader->InitData(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, {
					{ -1.0f, 1.0f }, { 0.0f, 0.0f },	// point at top-left
					{ 1.0f, 1.0f }, { 1.0f, 0.0f },		// point at top-right
					{ -1.0f, -1.0f }, { 0.0f, 1.0f },	// point at bottom-left
					{ 1.0f, -1.0f }, { 1.0f, 1.0f },	// point at bottom-right
					}) && 
					_PShaderEffectFXAA->Install() && _PShaderEffectTAA->Install() &&
					_VShader->Install() && _BackSmp->Create(&DescSmp);
			
			_hWindow = hWindow;
			_Data.jitter.x = std::max(0.0f, std::min(_READ_OPTION_FLOAT("Graphics", "fTAAJitter", 0.5f), 0.5f));
			_Data.jitter.y = _Data.jitter.x;

			return _Init;
		}

		void ImagespaceAA::Draw(IDXGISwapChain* SwapChain) const
		{
			if (!_Init || !_Enabled) return;

			ComPtr<ID3D11Texture2D> Back;
			SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)Back.GetAddressOf());
			if (Back)
			{
				if (!_Frame->Create(Back.Get(), D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
					return;

				auto FrameDesc = _Frame->GetDesc();
				if (!_PostFrame->Create(FrameDesc))
					return;
				
				CD3D11_SHADER_RESOURCE_VIEW_DESC ResFrameDesc(D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D, FrameDesc->Format);
				CD3D11_RENDER_TARGET_VIEW_DESC RTVDesc(D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D, FrameDesc->Format);

				if (!_RTVFrame->Create(_PostFrame->Get(), &RTVDesc))
					return;

				auto This = const_cast<ImagespaceAA*>(this);
				This->_Data.tilesize.x = 1.0f / FrameDesc->Width;
				This->_Data.tilesize.y = 1.0f / FrameDesc->Height;

				if (!_DataBuffer->Create(reinterpret_cast<const void*>(&(This->_Data)), sizeof(ImagespaceAA::Data),
						D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE))
					return;

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

					ComPtr<ID3D11RenderTargetView> OldRTV;
					GlobalD3D11ShaderEngine->DeviceContext()->OMGetRenderTargets(1, OldRTV.GetAddressOf(), nullptr);
					GlobalD3D11ShaderEngine->DeviceContext()->OMSetRenderTargets(1, _RTVFrame->GetAddressOf(), nullptr);

					_BackRes->Create(_Frame->Get(), &ResFrameDesc);
					_BackRes->Bind(CKPE_PIXEL_SHADER, 0);
					_BackSmp->Bind(CKPE_PIXEL_SHADER, 0);
					_DataBuffer->Bind(CKPE_PIXEL_SHADER, 0);

					_VShader->Bind();
					_PShaderEffectFXAA->Bind();

					GlobalD3D11ShaderEngine->DeviceContext()->Draw(4, 0);
					GlobalD3D11ShaderEngine->DeviceContext()->OMSetRenderTargets(1, OldRTV.GetAddressOf(), nullptr);

					_BackRes->Unbind();
					_BackRes->Create(_PostFrame->Get(), &ResFrameDesc);
					
					_BackRes->Bind(CKPE_PIXEL_SHADER, 0);
					_PShaderEffectTAA->Bind();

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