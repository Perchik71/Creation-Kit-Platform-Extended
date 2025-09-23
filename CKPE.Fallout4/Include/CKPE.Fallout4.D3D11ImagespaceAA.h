// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "CKPE.Fallout4.D3D11Shaders.h"
#include <DirectXMath.h>

namespace CKPE
{
	namespace Fallout4
	{
		class D3D11ImagespaceAA
		{
#pragma pack(push, 4)
			struct Data
			{
				DirectX::XMFLOAT2 tilesize;
				DirectX::XMFLOAT2 jitter;
			};
#pragma pack(pop)
		public:
			D3D11ImagespaceAA();

			virtual bool Install(HWND hWindow);
			virtual void Draw(IDXGISwapChain* SwapChain) const;

			inline virtual bool IsInit() const noexcept(true) { return _Init; }
			inline virtual bool IsEnabled() const noexcept(true) { return _Enabled; }
			inline virtual HWND GetHandle() const noexcept(true) { return _hWindow; }
			inline virtual void SetEnabled(bool Enabled) noexcept(true) { _Enabled = Enabled; }
		private:
			std::unique_ptr<D3D11PixelShader> _PShaderEffectFXAA;
			std::unique_ptr<D3D11PixelShader> _PShaderEffectTAA;
			std::unique_ptr<D3D11VertexShader> _VShader;
			std::unique_ptr<D3D11ShaderResourceView> _BackRes;
			std::unique_ptr<D3D11SamplerState> _BackSmp;
			std::unique_ptr<D3D11State> _State;
			std::unique_ptr<D3D11ShaderTexture> _Frame;
			std::unique_ptr<D3D11ShaderTexture> _PostFrame;
			std::unique_ptr<D3D11RenderTargetView> _RTVFrame;
			std::unique_ptr<D3D11ShaderBuffer> _DataBuffer;
			Data _Data;
			HWND _hWindow;
			bool _Init;
			bool _Enabled;
		};
	}
}
