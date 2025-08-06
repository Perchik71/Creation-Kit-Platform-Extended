// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "resource.h"
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.D3D11ImagespaceAA.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		D3D11ImagespaceAA::D3D11ImagespaceAA() :
			_Init(false), _hWindow(nullptr), _Enabled(true)
		{
			_PShaderEffectFXAA = std::make_unique<D3D11PixelShader>(D3D11ShaderEngine::Singleton.GetSingleton(), "PS_FXAA_D3D11ImagespaceAA");
			_PShaderEffectTAA = std::make_unique<D3D11PixelShader>(D3D11ShaderEngine::Singleton.GetSingleton(), "PS_TAA_D3D11ImagespaceAA");
			_PostFrame = std::make_unique<D3D11ShaderTexture>(D3D11ShaderEngine::Singleton.GetSingleton(), "PostFrame_D3D11ImagespaceAA");
			_VShader = std::make_unique<D3D11VertexShader>(D3D11ShaderEngine::Singleton.GetSingleton(), "VS_D3D11ImagespaceAA");
			_BackRes = std::make_unique<D3D11ShaderResourceView>(D3D11ShaderEngine::Singleton.GetSingleton(), "BackRes_D3D11ImagespaceAA");
			_BackSmp = std::make_unique<D3D11SamplerState>(D3D11ShaderEngine::Singleton.GetSingleton(), "BackSmp_D3D11ImagespaceAA");
			_State = std::make_unique<D3D11State>(D3D11ShaderEngine::Singleton.GetSingleton(), "State_D3D11ImagespaceAA");
			_Frame = std::make_unique<D3D11ShaderTexture>(D3D11ShaderEngine::Singleton.GetSingleton(), "Frame_D3D11ImagespaceAA");
			_RTVFrame = std::make_unique<D3D11RenderTargetView>(D3D11ShaderEngine::Singleton.GetSingleton(), "RTVFrame_D3D11ImagespaceAA");
			_DataBuffer = std::make_unique<D3D11ShaderBuffer>(D3D11ShaderEngine::Singleton.GetSingleton(), "DataBuffer_D3D11ImagespaceAA");
		}

		bool D3D11ImagespaceAA::Install(HWND hWindow)
		{
			if (_Init)
				return true;

			CD3D11_SAMPLER_DESC DescSmp(D3D11_DEFAULT);

			auto inst = GetModuleHandleA("CKPE.SkyrimSE.dll");

			_Init =
				_PShaderEffectFXAA->LoadFromResource((HMODULE)inst, IDS_FXAA_PS) &&
				_PShaderEffectTAA->LoadFromResource((HMODULE)inst, IDS_TAA_PS) &&
				_VShader->LoadFromResource((HMODULE)inst, IDS_FXAA_VS) &&
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

		void D3D11ImagespaceAA::Draw(IDXGISwapChain* SwapChain) const
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

				auto This = const_cast<D3D11ImagespaceAA*>(this);
				This->_Data.tilesize.x = 1.0f / FrameDesc->Width;
				This->_Data.tilesize.y = 1.0f / FrameDesc->Height;

				if (!_DataBuffer->Create(reinterpret_cast<const void*>(&(This->_Data)), sizeof(D3D11ImagespaceAA::Data),
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
					auto Eng = D3D11ShaderEngine::Singleton.GetSingleton();

					Eng->DeviceContext()->RSSetViewports(1, &vp);

					// Setup shader and vertex buffers
					Eng->DeviceContext()->GSSetShader(nullptr, nullptr, 0);
					Eng->DeviceContext()->HSSetShader(nullptr, nullptr, 0);
					Eng->DeviceContext()->DSSetShader(nullptr, nullptr, 0);
					Eng->DeviceContext()->CSSetShader(nullptr, nullptr, 0);

					// Setup blend state
					const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
					Eng->DeviceContext()->OMSetBlendState(nullptr, blend_factor, 0xffffffff);
					Eng->DeviceContext()->OMSetDepthStencilState(nullptr, 0);
					Eng->DeviceContext()->RSSetState(nullptr);
					Eng->DeviceContext()->RSSetScissorRects(0, nullptr);

					ComPtr<ID3D11RenderTargetView> OldRTV;
					Eng->DeviceContext()->OMGetRenderTargets(1, OldRTV.GetAddressOf(), nullptr);
					Eng->DeviceContext()->OMSetRenderTargets(1, _RTVFrame->GetAddressOf(), nullptr);

					_BackRes->Create(_Frame->Get(), &ResFrameDesc);
					_BackRes->Bind(CKPE_PIXEL_SHADER, 0);
					_BackSmp->Bind(CKPE_PIXEL_SHADER, 0);
					_DataBuffer->Bind(CKPE_PIXEL_SHADER, 0);

					_VShader->Bind();
					_PShaderEffectFXAA->Bind();

					Eng->DeviceContext()->Draw(4, 0);
					Eng->DeviceContext()->OMSetRenderTargets(1, OldRTV.GetAddressOf(), nullptr);

					_BackRes->Unbind();
					_BackRes->Create(_PostFrame->Get(), &ResFrameDesc);
					
					_BackRes->Bind(CKPE_PIXEL_SHADER, 0);
					_PShaderEffectTAA->Bind();

					Eng->DeviceContext()->Draw(4, 0);
				}
				catch (const std::exception& e)
				{
					_ERROR("[ERROR] D3D11ImagespaceAA: %s", e.what());
				}
				
				_State->PopState();
			}
		}
	}
}