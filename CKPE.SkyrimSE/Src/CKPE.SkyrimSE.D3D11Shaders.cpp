// Copyright © 2025 aka perchik71.All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.D3D11Shaders.h>

#include <comdef.h>
#include <d3dcompiler.h>
#include <ScreenGrab11.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		static bool CheckHrResult(HRESULT hr, const char* ClassName)
		{
			bool Result = SUCCEEDED(hr);
			if (!Result)
				_ERROR("An error has occurred in the \"%s\" class. Message: \"%s\"", ClassName, _com_error(hr).ErrorMessage());
			return Result;
		}

		D3D11Object::D3D11Object(D3D11ShaderEngine* Engine, const char* Name) :
			_Engine(Engine), _Name(Name)
		{}

		D3D11State::D3D11State(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Object(Engine, Name)
		{}

		void D3D11State::PushState() noexcept(true)
		{
			// https://github.com/fholger/vrperfkit/blob/a52f8a45d330d0b66206aee85165db715e4482cd/src/d3d11/d3d11_helper.h

			Engine()->DeviceContext()->VSGetShader(VertexShader.ReleaseAndGetAddressOf(), nullptr, nullptr);
			Engine()->DeviceContext()->PSGetShader(PixelShader.ReleaseAndGetAddressOf(), nullptr, nullptr);
			Engine()->DeviceContext()->CSGetShader(ComputeShader.ReleaseAndGetAddressOf(), nullptr, nullptr);
			Engine()->DeviceContext()->IAGetInputLayout(InputLayout.ReleaseAndGetAddressOf());
			Engine()->DeviceContext()->IAGetPrimitiveTopology(&Topology);
			Engine()->DeviceContext()->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, VertexBuffers, Strides, Offsets);
			Engine()->DeviceContext()->IAGetIndexBuffer(IndexBuffer.ReleaseAndGetAddressOf(), &Format, &Offset);
			Engine()->DeviceContext()->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, RenderTargets, DepthStencil.GetAddressOf());
			Engine()->DeviceContext()->RSGetState(RasterizerState.ReleaseAndGetAddressOf());
			Engine()->DeviceContext()->OMGetDepthStencilState(DepthStencilState.ReleaseAndGetAddressOf(), &StencilRef);
			Engine()->DeviceContext()->RSGetViewports(&NumViewports, nullptr);
			Engine()->DeviceContext()->RSGetViewports(&NumViewports, Viewports);
			Engine()->DeviceContext()->VSGetConstantBuffers(0, 1, VSConstantBuffer.GetAddressOf());
			Engine()->DeviceContext()->PSGetConstantBuffers(0, 1, PSConstantBuffer.GetAddressOf());
			Engine()->DeviceContext()->CSGetConstantBuffers(0, 1, CSConstantBuffer.GetAddressOf());
			Engine()->DeviceContext()->CSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, CSShaderResources);
			Engine()->DeviceContext()->CSGetUnorderedAccessViews(0, D3D11_1_UAV_SLOT_COUNT, CSUavs);

			// Added
			Engine()->DeviceContext()->RSGetScissorRects(&NumScissorRects, nullptr);
			Engine()->DeviceContext()->RSGetScissorRects(&NumScissorRects, RSScissorRects);
			Engine()->DeviceContext()->RSGetState(RS.GetAddressOf());
			Engine()->DeviceContext()->OMGetBlendState(BlendState.GetAddressOf(), BlendFactor, &SampleMask);
		}

		void D3D11State::PopState() noexcept(true)
		{
			// https://github.com/fholger/vrperfkit/blob/a52f8a45d330d0b66206aee85165db715e4482cd/src/d3d11/d3d11_helper.h

			Engine()->DeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
			Engine()->DeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);
			Engine()->DeviceContext()->CSSetShader(ComputeShader.Get(), nullptr, 0);
			Engine()->DeviceContext()->IASetInputLayout(InputLayout.Get());
			Engine()->DeviceContext()->IASetPrimitiveTopology(Topology);
			Engine()->DeviceContext()->IASetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, VertexBuffers, Strides, Offsets);

			for (int i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; ++i)
				if (VertexBuffers[i])
					VertexBuffers[i]->Release();

			Engine()->DeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), Format, Offset);
			Engine()->DeviceContext()->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, RenderTargets, DepthStencil.Get());

			for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
				if (RenderTargets[i])
					RenderTargets[i]->Release();

			Engine()->DeviceContext()->RSSetState(RasterizerState.Get());
			Engine()->DeviceContext()->OMSetDepthStencilState(DepthStencilState.Get(), StencilRef);
			Engine()->DeviceContext()->RSSetViewports(NumViewports, Viewports);
			Engine()->DeviceContext()->VSSetConstantBuffers(0, 1, VSConstantBuffer.GetAddressOf());
			Engine()->DeviceContext()->PSSetConstantBuffers(0, 1, PSConstantBuffer.GetAddressOf());
			Engine()->DeviceContext()->CSSetConstantBuffers(0, 1, CSConstantBuffer.GetAddressOf());
			Engine()->DeviceContext()->CSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, CSShaderResources);

			for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; ++i)
				if (CSShaderResources[i])
					CSShaderResources[i]->Release();

			UINT initial = 0;
			Engine()->DeviceContext()->CSSetUnorderedAccessViews(0, D3D11_1_UAV_SLOT_COUNT, CSUavs, &initial);

			for (int i = 0; i < D3D11_1_UAV_SLOT_COUNT; ++i)
				if (CSUavs[i])
					CSUavs[i]->Release();

			// Added
			Engine()->DeviceContext()->RSSetScissorRects(NumScissorRects, RSScissorRects);
			Engine()->DeviceContext()->RSSetState(RS.Get());
			Engine()->DeviceContext()->OMSetBlendState(BlendState.Get(), BlendFactor, SampleMask);
		}

		D3D11ShaderTexture::D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Object(Engine, Name)
		{}

		D3D11ShaderTexture::D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name, const D3D11_TEXTURE2D_DESC* Desc) :
			D3D11Object(Engine, Name)
		{
			CKPE_ASSERT(Create(Desc));
		}

		D3D11ShaderTexture::D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Texture2D* Texture,
			uint32_t BindFlags) : D3D11Object(Engine, Name)
		{
			CKPE_ASSERT(Create(Texture, BindFlags));
		}

		D3D11ShaderTexture::D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource,
			uint32_t BindFlags) : D3D11Object(Engine, Name)
		{
			CKPE_ASSERT(Create(Resource, BindFlags));
		}

		bool D3D11ShaderTexture::SaveTextureToFileAsDDS(const char* FileName) const noexcept(true)
		{
			if (!FileName || !_Texture)
				return false;

			int len = MultiByteToWideChar(CP_ACP, 0, FileName, -1, nullptr, 0);
			if (len <= 0)
				return false;

			auto str = std::make_unique<wchar_t[]>((size_t)len + 1);
			if (!str)
				return false;

			MultiByteToWideChar(CP_ACP, 0, FileName, len, str.get(), len);
			return CheckHrResult(DirectX::SaveDDSTextureToFile(Engine()->DeviceContext(), Get(), str.get()), Name().c_str());
		}

		void D3D11ShaderTexture::DebugInfo() const noexcept(true)
		{
			_MESSAGE("[DBG] Texture \"%s\" info: Size(%ux%u) Format(%X) Usage(%X) BindFlags(%X)",
				Name().c_str(), _Desc.Width, _Desc.Height, _Desc.Format, _Desc.Usage, _Desc.BindFlags);
		}

		bool D3D11ShaderTexture::Create(const D3D11_TEXTURE2D_DESC* Desc) noexcept(true)
		{
			if (!Desc)
			{
				_ERROR("D3D11ShaderTexture: \"%s\" description nullptr", Name().c_str());
				return false;
			}

			memcpy(&_Desc, Desc, sizeof(D3D11_TEXTURE2D_DESC));
			return CheckHrResult(Engine()->Device()->CreateTexture2D(&_Desc, nullptr, ReleaseAndGetAddressOf()), Name().c_str());
		}

		bool D3D11ShaderTexture::Create(const ID3D11Texture2D* Texture, std::uint32_t BindFlags) noexcept(true)
		{
			if (!Texture)
			{
				_ERROR("TextureShader: \"%s\" texture nullptr", Name().c_str());
				return false;
			}

			auto Tex = const_cast<ID3D11Texture2D*>(Texture);
			Tex->GetDesc(&_Desc);
			_Desc.BindFlags = BindFlags;

			if (Create(&_Desc))
			{
				Engine()->DeviceContext()->CopyResource(Get(), Tex);
				return true;
			}

			return false;
		}

		bool D3D11ShaderTexture::Create(const ID3D11Resource* Resource, std::uint32_t BindFlags) noexcept(true)
		{
			if (!Resource)
			{
				_ERROR("TextureShader: \"%s\" resource nullptr", Name().c_str());
				return false;
			}

			D3D11_RESOURCE_DIMENSION dimType;
			auto Res = const_cast<ID3D11Resource*>(Resource);
			Res->GetType(&dimType);
			if (dimType != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				return false;

			ComPtr<ID3D11Texture2D> Tex;
			auto hr = Res->QueryInterface(IID_ID3D11Texture2D, reinterpret_cast<void**>(Tex.GetAddressOf()));
			if (CheckHrResult(hr, Name().c_str()))
				return Create(Tex.Get(), BindFlags);

			return false;
		}

		bool D3D11ShaderTexture::CopyFrom(const D3D11ShaderTexture* Texture) noexcept(true)
		{
			if (!Texture) return false;
			return CopyFrom(Texture->Get());
		}

		bool D3D11ShaderTexture::CopyFrom(const ID3D11Texture2D* Texture) noexcept(true)
		{
			if (!Texture) return false;
			Engine()->DeviceContext()->CopyResource(Get(), const_cast<ID3D11Texture2D*>(Texture));
			return true;
		}

		bool D3D11ShaderTexture::CopyFrom(const ID3D11Resource* Resource) noexcept(true)
		{
			if (!Resource)
				return false;

			D3D11_RESOURCE_DIMENSION dimType;
			auto Res = const_cast<ID3D11Resource*>(Resource);
			Res->GetType(&dimType);
			if (dimType != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				return false;

			ComPtr<ID3D11Texture2D> Tex;
			auto hr = Res->QueryInterface(IID_ID3D11Texture2D, reinterpret_cast<void**>(Tex.GetAddressOf()));
			if (!CheckHrResult(hr, Name().c_str()))
				return false;

			Engine()->DeviceContext()->CopyResource(Get(), Tex.Get());

			return true;
		}

		D3D11RenderTargetView::D3D11RenderTargetView(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Object(Engine, Name)
		{}

		D3D11RenderTargetView::D3D11RenderTargetView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource,
			const D3D11_RENDER_TARGET_VIEW_DESC* Desc) : D3D11Object(Engine, Name)
		{
			CKPE_ASSERT(Create(Resource, Desc));
		}

		D3D11RenderTargetView::D3D11RenderTargetView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11RenderTargetView* View) :
			D3D11Object(Engine, Name)
		{
			CKPE_ASSERT(Create(View));
		}

		void D3D11RenderTargetView::DebugInfo() const noexcept(true)
		{
			_MESSAGE("[DBG] Render target \"%s\" info: Format(%X) Buffer([%u:%u] Off:%u Width:%u) ViewDimension(%X)", Name().c_str(),
				_Desc.Format, _Desc.Buffer.FirstElement, _Desc.Buffer.NumElements, _Desc.Buffer.ElementOffset, _Desc.Buffer.ElementWidth,
				_Desc.ViewDimension);
		}

		bool D3D11RenderTargetView::Create(const ID3D11Resource* Resource, const D3D11_RENDER_TARGET_VIEW_DESC* Desc) noexcept(true)
		{
			if (!Resource || !Desc)
				return false;

			memcpy(&_Desc, Desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
			return CheckHrResult(Engine()->Device()->CreateRenderTargetView(const_cast<ID3D11Resource*>(Resource), &_Desc,
				ReleaseAndGetAddressOf()), Name().c_str());
		}

		bool D3D11RenderTargetView::Create(const ID3D11RenderTargetView* View) noexcept(true)
		{
			if (!View)
				return false;

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			auto rtv = const_cast<ID3D11RenderTargetView*>(View);
			rtv->GetDesc(&rtvDesc);

			ComPtr<ID3D11Resource> Res;
			rtv->GetResource(Res.GetAddressOf());

			return Create(Res.Get(), &rtvDesc);
		}

		D3D11DepthStencilView::D3D11DepthStencilView(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Object(Engine, Name)
		{}

		D3D11DepthStencilView::D3D11DepthStencilView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource,
			const D3D11_DEPTH_STENCIL_VIEW_DESC* Desc) : D3D11Object(Engine, Name)
		{
			CKPE_ASSERT(Create(Resource, Desc));
		}

		D3D11DepthStencilView::D3D11DepthStencilView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11DepthStencilView* View) :
			D3D11Object(Engine, Name)
		{
			CKPE_ASSERT(Create(View));
		}

		void D3D11DepthStencilView::DebugInfo() const noexcept(true)
		{
			_MESSAGE("[DBG] Depth and stencil \"%s\" info: Format(%X) Flags(%X) ViewDimension(%X)",
				Name().c_str(), _Desc.Format, _Desc.Flags, _Desc.ViewDimension);
		}

		bool D3D11DepthStencilView::Create(const ID3D11Resource* Resource, const D3D11_DEPTH_STENCIL_VIEW_DESC* Desc) noexcept(true)
		{
			if (!Resource || !Desc)
				return false;

			memcpy(&_Desc, Desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			return CheckHrResult(Engine()->Device()->CreateDepthStencilView(const_cast<ID3D11Resource*>(Resource), &_Desc,
				ReleaseAndGetAddressOf()), Name().c_str());
		}

		bool D3D11DepthStencilView::Create(const ID3D11DepthStencilView* View) noexcept(true)
		{
			if (!View)
				return false;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			auto rtv = const_cast<ID3D11DepthStencilView*>(View);
			rtv->GetDesc(&dsvDesc);

			ComPtr<ID3D11Resource> Res;
			rtv->GetResource(Res.GetAddressOf());

			return Create(Res.Get(), &dsvDesc);
		}

		D3D11ShaderBuffer::D3D11ShaderBuffer(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Object(Engine, Name), IPType(0)
		{}

		D3D11ShaderBuffer::D3D11ShaderBuffer(D3D11ShaderEngine* Engine, const char* Name, const void* Buffer,
			std::uint32_t Size, std::uint32_t BindFlags, D3D11_USAGE Usage) : D3D11Object(Engine, Name), IPType(0)
		{
			CKPE_ASSERT(Create(Buffer, Size, BindFlags, Usage));
		}

		bool D3D11ShaderBuffer::Create(const void* Buffer, std::uint32_t Size, std::uint32_t BindFlags,
			D3D11_USAGE Usage, std::uint32_t CPUAccess) noexcept(true)
		{
			if (!Buffer || !Size)
				return false;

			CD3D11_BUFFER_DESC cbDesc(Size, BindFlags, Usage, CPUAccess);
			memcpy(&_Desc, &cbDesc, sizeof(D3D11_BUFFER_DESC));

			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
			InitData.pSysMem = Buffer;

			return CheckHrResult(Engine()->Device()->CreateBuffer(&_Desc, &InitData, ReleaseAndGetAddressOf()), Name().c_str());
		}

		void D3D11ShaderBuffer::Bind(std::uint32_t Type, std::uint32_t BindID) noexcept(true)
		{
			if ((Type & CKPE_PIXEL_SHADER) == CKPE_PIXEL_SHADER)
			{
				Engine()->DeviceContext()->PSSetConstantBuffers(BindID, 1, GetAddressOf());
				BindId[0] = BindID;
			}

			if ((Type & CKPE_VERTEX_SHADER) == CKPE_VERTEX_SHADER)
			{
				Engine()->DeviceContext()->VSSetConstantBuffers(BindID, 1, GetAddressOf());
				BindId[1] = BindID;
			}

			if ((Type & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
			{
				Engine()->DeviceContext()->CSSetConstantBuffers(BindID, 1, GetAddressOf());
				BindId[2] = BindID;
			}

			IPType = Type;
		}

		void D3D11ShaderBuffer::Unbind() noexcept(true)
		{
			if ((IPType & CKPE_PIXEL_SHADER) == CKPE_PIXEL_SHADER)
				Engine()->DeviceContext()->PSSetConstantBuffers(BindId[0], 0, nullptr);

			if ((IPType & CKPE_VERTEX_SHADER) == CKPE_VERTEX_SHADER)
				Engine()->DeviceContext()->VSSetConstantBuffers(BindId[1], 0, nullptr);

			if ((IPType & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
				Engine()->DeviceContext()->CSSetConstantBuffers(BindId[2], 0, nullptr);

			IPType = 0;
		}

		D3D11UnorderedAccessView::D3D11UnorderedAccessView(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Object(Engine, Name), IPType(0)
		{}

		D3D11UnorderedAccessView::D3D11UnorderedAccessView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource,
			const D3D11_UNORDERED_ACCESS_VIEW_DESC* Desc) : D3D11Object(Engine, Name), IPType(0)
		{
			CKPE_ASSERT(Create(Resource, Desc));
		}

		D3D11UnorderedAccessView::D3D11UnorderedAccessView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11UnorderedAccessView* View) :
			D3D11Object(Engine, Name), IPType(0)
		{
			CKPE_ASSERT(Create(View));
		}

		void D3D11UnorderedAccessView::DebugInfo() const noexcept(true)
		{
			_MESSAGE("[DBG] Unordered access \"%s\" info: Format(%X) Buffer([%u:%u] Flags:%X) ViewDimension(%X)",
				Name().c_str(), _Desc.Format, _Desc.Buffer.FirstElement, _Desc.Buffer.NumElements, _Desc.Buffer.Flags, _Desc.ViewDimension);
		}

		bool D3D11UnorderedAccessView::Create(const ID3D11Resource* Resource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* Desc) noexcept(true)
		{
			if (!Resource || !Desc)
				return false;

			memcpy(&_Desc, Desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
			return CheckHrResult(Engine()->Device()->CreateUnorderedAccessView(const_cast<ID3D11Resource*>(Resource), &_Desc,
				ReleaseAndGetAddressOf()), Name().c_str());
		}

		bool D3D11UnorderedAccessView::Create(const ID3D11UnorderedAccessView* View) noexcept(true)
		{
			if (!View)
				return false;

			D3D11_UNORDERED_ACCESS_VIEW_DESC srvDesc;
			auto rtv = const_cast<ID3D11UnorderedAccessView*>(View);
			rtv->GetDesc(&srvDesc);

			ComPtr<ID3D11Resource> Res;
			rtv->GetResource(Res.GetAddressOf());

			return Create(Res.Get(), &srvDesc);
		}

		void D3D11UnorderedAccessView::Bind(std::uint32_t Type, std::uint32_t BindID) noexcept(true)
		{
			if ((Type & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
			{
				Engine()->DeviceContext()->CSSetUnorderedAccessViews(BindID, 1, GetAddressOf(), &Counts);
				BindId = BindID;
			}

			IPType = Type;
		}

		void D3D11UnorderedAccessView::Unbind() noexcept(true)
		{
			if ((IPType & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
				Engine()->DeviceContext()->CSSetUnorderedAccessViews(BindId, 0, nullptr, nullptr);

			IPType = 0;
		}

		D3D11ShaderResourceView::D3D11ShaderResourceView(D3D11ShaderEngine* Engine, const char* Name) : 
			D3D11Object(Engine, Name), IPType(0)
		{}

		D3D11ShaderResourceView::D3D11ShaderResourceView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource,
			const D3D11_SHADER_RESOURCE_VIEW_DESC* Desc) : D3D11Object(Engine, Name), IPType(0)
		{
			CKPE_ASSERT(Create(Resource, Desc));
		}

		D3D11ShaderResourceView::D3D11ShaderResourceView(D3D11ShaderEngine* Engine, const char* Name, 
			const ID3D11ShaderResourceView* View) : D3D11Object(Engine, Name), IPType(0)
		{
			CKPE_ASSERT(Create(View));
		}

		void D3D11ShaderResourceView::DebugInfo() const noexcept(true)
		{
			_MESSAGE("[DBG] Resource \"%s\" info: Format(%X) Buffer([%u:%u] Off:%u Width:%u) ViewDimension(%X)", Name().c_str(),
				_Desc.Format, _Desc.Buffer.FirstElement, _Desc.Buffer.NumElements, _Desc.Buffer.ElementOffset, _Desc.Buffer.ElementWidth,
				_Desc.ViewDimension);
		}

		bool D3D11ShaderResourceView::Create(const ID3D11Resource* Resource, const D3D11_SHADER_RESOURCE_VIEW_DESC* Desc) noexcept(true)
		{
			if (!Resource || !Desc)
				return false;

			memcpy(&_Desc, Desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			return CheckHrResult(Engine()->Device()->CreateShaderResourceView(const_cast<ID3D11Resource*>(Resource), &_Desc,
				ReleaseAndGetAddressOf()), Name().c_str());
		}

		bool D3D11ShaderResourceView::Create(const ID3D11ShaderResourceView* View) noexcept(true)
		{
			if (!View)
				return false;

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			auto rv = const_cast<ID3D11ShaderResourceView*>(View);
			rv->GetDesc(&srvDesc);

			ComPtr<ID3D11Resource> Res;
			rv->GetResource(Res.GetAddressOf());

			return Create(Res.Get(), &srvDesc);
		}

		void D3D11ShaderResourceView::Bind(std::uint32_t Type, std::uint32_t BindID) noexcept(true)
		{
			if ((Type & CKPE_PIXEL_SHADER) == CKPE_PIXEL_SHADER)
			{
				Engine()->DeviceContext()->PSSetShaderResources(BindID, 1, GetAddressOf());
				BindId[0] = BindID;
			}

			if ((Type & CKPE_VERTEX_SHADER) == CKPE_VERTEX_SHADER)
			{
				Engine()->DeviceContext()->VSSetShaderResources(BindID, 1, GetAddressOf());
				BindId[1] = BindID;
			}

			if ((Type & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
			{
				Engine()->DeviceContext()->CSSetShaderResources(BindID, 1, GetAddressOf());
				BindId[2] = BindID;
			}

			IPType = Type;
		}

		void D3D11ShaderResourceView::Unbind() noexcept(true)
		{
			if ((IPType & CKPE_PIXEL_SHADER) == CKPE_PIXEL_SHADER)
				Engine()->UnbindResource(IPType, BindId[0]);

			if ((IPType & CKPE_VERTEX_SHADER) == CKPE_VERTEX_SHADER)
				Engine()->UnbindResource(IPType, BindId[1]);

			if ((IPType & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
				Engine()->UnbindResource(IPType, BindId[2]);

			IPType = 0;
		}

		D3D11SamplerState::D3D11SamplerState(D3D11ShaderEngine* Engine, const char* Name) : 
			D3D11Object(Engine, Name), IPType(0)
		{}

		D3D11SamplerState::D3D11SamplerState(D3D11ShaderEngine* Engine, const char* Name, const D3D11_SAMPLER_DESC* Desc) :
			D3D11Object(Engine, Name), IPType(0)
		{
			CKPE_ASSERT(Create(Desc));
		}

		void D3D11SamplerState::DebugInfo() const noexcept(true)
		{
			_MESSAGE("[DBG] Sampler \"%s\" info: Address(%Xx%Xx%X) BorderColor(%.3f,%.3f,%.3f,%.3f) ComparisonFunc(%X)"
				" Filter(%X) MaxAnisotropy(%u) LOD(%.3f:%.3f) MipLODBias(%.3f)", Name().c_str(),
				_Desc.AddressU, _Desc.AddressV, _Desc.AddressW, _Desc.BorderColor[0], _Desc.BorderColor[1],
				_Desc.BorderColor[2], _Desc.BorderColor[3], _Desc.ComparisonFunc, _Desc.Filter, _Desc.MaxAnisotropy,
				_Desc.MinLOD, _Desc.MaxLOD, _Desc.MipLODBias);
		}

		bool D3D11SamplerState::Create(const D3D11_SAMPLER_DESC* Desc) noexcept(true)
		{
			if (!Desc)
				return false;

			memcpy(&_Desc, Desc, sizeof(D3D11_SAMPLER_DESC));
			return CheckHrResult(Engine()->Device()->CreateSamplerState(&_Desc, ReleaseAndGetAddressOf()), Name().c_str());
		}

		void D3D11SamplerState::Bind(std::uint32_t Type, std::uint32_t BindID) noexcept(true)
		{
			if ((Type & CKPE_PIXEL_SHADER) == CKPE_PIXEL_SHADER)
			{
				Engine()->DeviceContext()->PSSetSamplers(BindID, 1, GetAddressOf());
				BindId[0] = BindID;
			}

			if ((Type & CKPE_VERTEX_SHADER) == CKPE_VERTEX_SHADER)
			{
				Engine()->DeviceContext()->VSSetSamplers(BindID, 1, GetAddressOf());
				BindId[1] = BindID;
			}

			if ((Type & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
			{
				Engine()->DeviceContext()->CSSetSamplers(BindID, 1, GetAddressOf());
				BindId[2] = BindID;
			}

			IPType = Type;
		}

		void D3D11SamplerState::Unbind() noexcept(true)
		{
			if ((IPType & CKPE_PIXEL_SHADER) == CKPE_PIXEL_SHADER)
				Engine()->DeviceContext()->PSSetSamplers(BindId[0], 0, nullptr);

			if ((IPType & CKPE_VERTEX_SHADER) == CKPE_VERTEX_SHADER)
				Engine()->DeviceContext()->VSSetSamplers(BindId[1], 0, nullptr);

			if ((IPType & CKPE_COMPUTE_SHADER) == CKPE_COMPUTE_SHADER)
				Engine()->DeviceContext()->CSSetSamplers(BindId[2], 0, nullptr);

			IPType = 0;
		}

		D3D11Shader::D3D11Shader(D3D11ShaderEngine* Engine, const char* Name) : D3D11Object(Engine, Name)
		{}

		bool D3D11Shader::LoadFromResource(HMODULE Module, std::uint32_t ResourceID) noexcept(true)
		{
			return LoadFromResource(Module, MAKEINTRESOURCEA(ResourceID));
		}

		bool D3D11Shader::LoadFromResource(HMODULE Module, const char* ResourceName) noexcept(true)
		{
			if (!ResourceName)
				return false;

			HRSRC hResource = FindResourceA(Module, ResourceName, "SHADER");
			if (!hResource)
			{
				_WARNING("Resource no found");
				return false;
			}

			std::uint32_t Size = SizeofResource(Module, hResource);
			if (Size >= (std::uint32_t)std::numeric_limits<std::int32_t>::max())
			{
				_WARNING("Size of resource exceeds 2 gigs");
				return false;
			}

			auto hResourceMemory = LoadResource(Module, hResource);
			if (!hResourceMemory)
			{
				_ERROR("hResourceMemory == nullptr");
				return false;
			}

			StreamBinary.Write(LockResource(hResourceMemory), (std::int32_t)Size);
			StreamBinary.SetPosition(0);

			bool Successed = Size == StreamBinary.GetSize();
			if (!Successed)
			{
				StreamBinary.Clear();
				_ERROR("An error occurred while reading the resource");
			}
			else
				_MESSAGE("Resource was loaded successfully");

			FreeResource(hResourceMemory);
			return Successed;
		}

		bool D3D11Shader::LoadFromStream(Stream& Steam) noexcept(true)
		{
			return StreamBinary.LoadFromStream(Steam);
		}

		bool D3D11Shader::LoadFromMemory(const void* Buf, std::int32_t Size) noexcept(true)
		{
			StreamBinary.Clear();
			return StreamBinary.Write(Buf, Size) == Size;
		}

		bool D3D11Shader::LoadFromFile(const char* FileName) noexcept(true)
		{
			if (!FileName)
				return false;

			return StreamBinary.LoadFromFile(FileName);
		}

		bool D3D11Shader::LoadFromFile(const wchar_t* FileName) noexcept(true)
		{
			if (!FileName)
				return false;

			return StreamBinary.LoadFromFile(FileName);
		}

		bool D3D11Shader::CompileFromFile(const char* FileName, const char* TypeShader, const char* EntryPoint) noexcept(true)
		{
			if (!FileName || !TypeShader || !EntryPoint)
				return false;

			int len = MultiByteToWideChar(CP_ACP, 0, FileName, -1, nullptr, 0);
			if (len <= 0)
				return false;

			auto str = std::make_unique<wchar_t[]>((std::size_t)len + 1);
			if (!str)
				return false;

			MultiByteToWideChar(CP_ACP, 0, FileName, len, str.get(), len);
			return CompileFromFile(str.get(), TypeShader, EntryPoint);
		}

		bool D3D11Shader::CompileFromFile(const wchar_t* FileName, const char* TypeShader, const char* EntryPoint) noexcept(true)
		{
			if (!FileName || !TypeShader || !EntryPoint)
				return false;

			ComPtr<ID3DBlob> shaderBlob = nullptr;
			ComPtr<ID3DBlob> errorBlob = nullptr;
			HRESULT hr = D3DCompileFromFile(FileName, NULL, NULL, EntryPoint, TypeShader,
				D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL2, 0, shaderBlob.GetAddressOf(), 
				errorBlob.GetAddressOf());

			if (FAILED(hr))
			{
				if (errorBlob)
					_ERROR("Compiler shader returned error: %s", (char*)errorBlob->GetBufferPointer());

				return false;
			}

			_MESSAGE("Compiler shader: %s (%p:%u)", Name().c_str(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());

			return LoadFromMemory(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
		}

		D3D11PixelShader::D3D11PixelShader(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Shader(Engine, Name)
		{}

		void D3D11PixelShader::Bind() noexcept(true)
		{
			Engine()->DeviceContext()->PSSetShader(_PixelShader.Get(), nullptr, 0);
		}

		void D3D11PixelShader::Unbind() noexcept(true)
		{
			Engine()->DeviceContext()->PSSetShader(nullptr, nullptr, 0);
		}

		bool D3D11PixelShader::Install() noexcept(true)
		{
			if (IsEmpty())
				return false;

			// create the shader object
			auto hr = Engine()->Device()->CreatePixelShader(StreamBinary.Data(), StreamBinary.GetSize(),
				nullptr, _PixelShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				_ERROR("CreatePixelShader: \"%s\" error has occurred: \"%s\"", Name().c_str(), _com_error(hr).ErrorMessage());
				return false;
			}

			_MESSAGE("PixelShader \"%s\" initialization was successful", Name().c_str());

			return true;
		}

		void D3D11PixelShader::Shutdown() noexcept(true)
		{
			_PixelShader.Reset();
		}

		D3D11VertexShader::D3D11VertexShader(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Shader(Engine, Name)
		{
			_InputBuffer = std::make_unique<D3D11ShaderBuffer>(Engine, Name);
		}

		void D3D11VertexShader::Bind() noexcept(true)
		{
			auto dc = Engine()->DeviceContext();
			dc->IASetPrimitiveTopology(_Topology);
			dc->IASetInputLayout(_InputLayout.Get());
			dc->IASetVertexBuffers(0, 1, _InputBuffer->GetAddressOf(), &_StrideBuffer, &_OffsetBuffer);
			dc->VSSetShader(_VertexShader.Get(), nullptr, 0);
		}

		void D3D11VertexShader::Unbind() noexcept(true)
		{
			auto dc = Engine()->DeviceContext();
			dc->IASetPrimitiveTopology(_Topology);
			dc->IASetInputLayout(nullptr);
			dc->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
			dc->VSSetShader(nullptr, nullptr, 0);
		}

		bool D3D11VertexShader::Install() noexcept(true)
		{
			if (IsEmpty())
				return false;

			// create the shader object
			auto hr = Engine()->Device()->CreateVertexShader(StreamBinary.Data(), StreamBinary.GetSize(), 
				nullptr, _VertexShader.GetAddressOf());
			if (FAILED(hr))
			{
				_ERROR("CreateVertexShader: \"%s\" error has occurred: \"%s\"", Name().c_str(), _com_error(hr).ErrorMessage());
				return false;
			}

			D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
			{
				{ "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hr = Engine()->Device()->CreateInputLayout(inputElementDesc, _ARRAYSIZE(inputElementDesc), StreamBinary.Data(), 
				StreamBinary.GetSize(), _InputLayout.GetAddressOf());
			if (FAILED(hr))
			{
				_ERROR("CreateInputLayout: \"%s\" error has occurred: \"%s\"", Name().c_str(), _com_error(hr).ErrorMessage());
				return false;
			}

			_MESSAGE("VertexShader \"%s\" initialization was successful", Name().c_str());

			return true;
		}

		void D3D11VertexShader::Shutdown() noexcept(true)
		{
			_VertexShader.Reset();
		}

		bool D3D11VertexShader::InitData(D3D_PRIMITIVE_TOPOLOGY Topology, const std::initializer_list<DirectX::XMFLOAT2>& InputData) noexcept(true)
		{
			if (!InputData.size())
				return false;

			_Topology = Topology;
			_SizeBuffer = (uint32_t)(InputData.size() >> 1);
			_StrideBuffer = sizeof(DirectX::XMFLOAT2) + sizeof(DirectX::XMFLOAT2);
			_OffsetBuffer = 0;

			if (!_InputBuffer->Create(reinterpret_cast<const void*>(InputData.begin()),
				_StrideBuffer * _SizeBuffer, D3D11_BIND_VERTEX_BUFFER))
				return false;

			_MESSAGE("VertexShader \"%s\" initialization data was successful", Name().c_str());

			return true;
		}

		D3D11ComputeShader::D3D11ComputeShader(D3D11ShaderEngine* Engine, const char* Name) :
			D3D11Shader(Engine, Name)
		{}

		void D3D11ComputeShader::Dispatch(uint32_t ThreadGroupCountX, uint32_t ThreadGroupCountY, uint32_t ThreadGroupCountZ) noexcept(true)
		{
			Engine()->DeviceContext()->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
		}

		void D3D11ComputeShader::Bind() noexcept(true)
		{
			Engine()->DeviceContext()->CSSetShader(_ComputeShader.Get(), nullptr, 0);
		}

		void D3D11ComputeShader::Unbind() noexcept(true)
		{
			Engine()->DeviceContext()->CSSetShader(nullptr, nullptr, 0);
		}

		bool D3D11ComputeShader::Install() noexcept(true)
		{
			if (IsEmpty())
				return false;

			// create the shader object
			auto hr = Engine()->Device()->CreateComputeShader(StreamBinary.Data(), StreamBinary.GetSize(),
				nullptr, _ComputeShader.GetAddressOf());
			if (FAILED(hr))
			{
				_ERROR("CreateComputeShader: \"%s\" error has occurred: \"%s\"", Name().c_str(), _com_error(hr).ErrorMessage());
				return false;
			}

			_MESSAGE("ComputeShader \"%s\" initialization was successful", Name().c_str());

			return true;
		}

		void D3D11ComputeShader::Shutdown() noexcept(true)
		{
			_ComputeShader.Reset();
		}

		D3D11ShaderEngine::D3D11ShaderEngine(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) :
			_Device(Device), _DeviceContext(DeviceContext)
		{
			CD3D11_TEXTURE2D_DESC Tex2D(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 1, 1);
			_EmptyTexture = std::make_unique<D3D11ShaderTexture>(this, "Empty Texture", &Tex2D);
			CD3D11_SHADER_RESOURCE_VIEW_DESC ResView(D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
				DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
			_EmptyResource = std::make_unique<D3D11ShaderResourceView>(this, "Empty", _EmptyTexture->Get(), &ResView);
			Singleton = this;
		}

		void D3D11ShaderEngine::UnbindResource(uint32_t Type, uint32_t BindID) noexcept(true)
		{
			_EmptyResource->Bind(Type, BindID);
		}
	}
}