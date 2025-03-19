// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Shaders.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class ImagespaceAA
		{
		public:
			ImagespaceAA();

			virtual bool Install(HWND hWindow);
			virtual void Draw(IDXGISwapChain* SwapChain) const;
		private:
			std::unique_ptr<D3D11PixelShader> _PShader;
			std::unique_ptr<D3D11VertexShader> _VShader;
			std::unique_ptr<D3D11ShaderTexture> _BackTex;
			std::unique_ptr<D3D11ShaderResourceView> _BackRes;
			std::unique_ptr<D3D11SamplerState> _BackSmp;
			std::unique_ptr<D3D11State> _State;
			HWND _hWindow;
			bool _Init;
		};
	}
}
