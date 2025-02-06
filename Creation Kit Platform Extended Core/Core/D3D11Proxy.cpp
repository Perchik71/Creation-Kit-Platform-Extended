#include "D3D11Proxy.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		D3D11DeviceProxy::D3D11DeviceProxy(ID3D11Device *Device)
		{
			HRESULT hr = Device->QueryInterface<ID3D11Device2>(&m_Device);

			AssertMsg(SUCCEEDED(hr), "D3D11.2 interface is not supported");
			Assert(m_Device->Release() > 0);

			ID3D11DeviceContext2 *temp;
			m_Device->GetImmediateContext2(&temp);

			m_ContextProxy = new D3D11DeviceContextProxy(temp);
			m_MipLODBias = std::min(5.f, std::max(-5.f, 
				_READ_OPTION_FLOAT("Graphics", "fMipLODBias", 0.f)));
			m_MaxAnisotropy = std::min(16ul, std::max(0ul,
				_READ_OPTION_UINT("Graphics", "uMaxAnisotropy", 0)));
		}

		D3D11DeviceProxy::D3D11DeviceProxy(ID3D11Device2 *Device)
		{
			m_Device = Device;

			ID3D11DeviceContext2 *temp;
			m_Device->GetImmediateContext2(&temp);

			m_ContextProxy = new D3D11DeviceContextProxy(temp);
			m_MipLODBias = std::min(5.f, std::max(-5.f,
				_READ_OPTION_FLOAT("Graphics", "fMipLODBias", 0.f)));
			m_MaxAnisotropy = std::min(16ul, std::max(0ul,
				_READ_OPTION_UINT("Graphics", "uMaxAnisotropy", 0)));
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::QueryInterface(REFIID riid, void **ppvObj)
		{
			return m_Device->QueryInterface(riid, ppvObj);
		}

		ULONG STDMETHODCALLTYPE D3D11DeviceProxy::AddRef()
		{
			return m_Device->AddRef();
		}

		ULONG STDMETHODCALLTYPE D3D11DeviceProxy::Release()
		{
			ULONG refCount = m_Device->Release();

			if (refCount <= 0)
			{
				m_ContextProxy->Release();
				m_ContextProxy = NULL;

				m_Device = NULL;
				delete this;
			}

			return refCount;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateBuffer(const D3D11_BUFFER_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer)
		{
			return m_Device->CreateBuffer(pDesc, pInitialData, ppBuffer);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateTexture1D(const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture1D **ppTexture1D)
		{
			return m_Device->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateTexture2D(const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D)
		{
			return m_Device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateTexture3D(const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D **ppTexture3D)
		{
			return m_Device->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateShaderResourceView(ID3D11Resource *pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D11ShaderResourceView **ppSRView)
		{
			return m_Device->CreateShaderResourceView(pResource, pDesc, ppSRView);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateUnorderedAccessView(ID3D11Resource *pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D11UnorderedAccessView **ppUAView)
		{
			return m_Device->CreateUnorderedAccessView(pResource, pDesc, ppUAView);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateRenderTargetView(ID3D11Resource *pResource, const D3D11_RENDER_TARGET_VIEW_DESC *pDesc, ID3D11RenderTargetView **ppRTView)
		{
			return m_Device->CreateRenderTargetView(pResource, pDesc, ppRTView);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateDepthStencilView(ID3D11Resource *pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D11DepthStencilView **ppDepthStencilView)
		{
			return m_Device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout)
		{
			return m_Device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader)
		{
			HRESULT hr = m_Device->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);

			//if (SUCCEEDED(hr))
			//	BSGraphics::Renderer::QInstance()->RegisterShaderBytecode(*ppVertexShader, pShaderBytecode, BytecodeLength);

			return hr;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateGeometryShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader)
		{
			return m_Device->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateGeometryShaderWithStreamOutput(const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, const UINT *pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader)
		{
			return m_Device->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader)
		{
			HRESULT hr = m_Device->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);

			//if (SUCCEEDED(hr))
			//	BSGraphics::Renderer::QInstance()->RegisterShaderBytecode(*ppPixelShader, pShaderBytecode, BytecodeLength);

			return hr;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateHullShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader)
		{
			return m_Device->CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateDomainShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppDomainShader)
		{
			return m_Device->CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateComputeShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11ComputeShader **ppComputeShader)
		{
			HRESULT hr = m_Device->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);

			//if (SUCCEEDED(hr))
			//	BSGraphics::Renderer::QInstance()->RegisterShaderBytecode(*ppComputeShader, pShaderBytecode, BytecodeLength);

			return hr;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateClassLinkage(ID3D11ClassLinkage **ppLinkage)
		{
			return m_Device->CreateClassLinkage(ppLinkage);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateBlendState(const D3D11_BLEND_DESC *pBlendStateDesc, ID3D11BlendState **ppBlendState)
		{
			return m_Device->CreateBlendState(pBlendStateDesc, ppBlendState);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D11DepthStencilState **ppDepthStencilState)
		{
			return m_Device->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateRasterizerState(const D3D11_RASTERIZER_DESC *pRasterizerDesc, ID3D11RasterizerState **ppRasterizerState)
		{
			if (pRasterizerDesc)
			{
				auto RasterizerDesc = const_cast<D3D11_RASTERIZER_DESC*>(pRasterizerDesc);
				RasterizerDesc->AntialiasedLineEnable = true;
				RasterizerDesc->MultisampleEnable = true;

				return m_Device->CreateRasterizerState(RasterizerDesc, ppRasterizerState);
			}

			return m_Device->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateSamplerState(const D3D11_SAMPLER_DESC *pSamplerDesc, ID3D11SamplerState **ppSamplerState)
		{
			if (pSamplerDesc)
			{
				auto SamplerDesc = const_cast<D3D11_SAMPLER_DESC*>(pSamplerDesc);

				if (SamplerDesc->Filter == D3D11_FILTER_ANISOTROPIC)
					SamplerDesc->MaxAnisotropy = m_MaxAnisotropy;

				SamplerDesc->MipLODBias = m_MipLODBias;		// mipmap level bias value
				SamplerDesc->MinLOD = 0.0f;					// alternative minimum mipmap level
				SamplerDesc->MaxLOD = D3D11_FLOAT32_MAX;	// alternative maximum mipmap level
			}
			
			return m_Device->CreateSamplerState(pSamplerDesc, ppSamplerState);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateQuery(const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery)
		{
			return m_Device->CreateQuery(pQueryDesc, ppQuery);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreatePredicate(const D3D11_QUERY_DESC *pPredicateDesc, ID3D11Predicate **ppPredicate)
		{
			return m_Device->CreatePredicate(pPredicateDesc, ppPredicate);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateCounter(const D3D11_COUNTER_DESC *pCounterDesc, ID3D11Counter **ppCounter)
		{
			return m_Device->CreateCounter(pCounterDesc, ppCounter);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext **ppDeferredContext)
		{
			HRESULT hr = m_Device->CreateDeferredContext(ContextFlags, ppDeferredContext);

			if (SUCCEEDED(hr))
				*ppDeferredContext = new D3D11DeviceContextProxy(*ppDeferredContext);

			return hr;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::OpenSharedResource(HANDLE hResource, REFIID ReturnedInterface, void **ppResource)
		{
			return m_Device->OpenSharedResource(hResource, ReturnedInterface, ppResource);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CheckFormatSupport(DXGI_FORMAT Format, UINT *pFormatSupport)
		{
			return m_Device->CheckFormatSupport(Format, pFormatSupport);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels)
		{
			return m_Device->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
		}

		void STDMETHODCALLTYPE D3D11DeviceProxy::CheckCounterInfo(D3D11_COUNTER_INFO *pCounterInfo)
		{
			m_Device->CheckCounterInfo(pCounterInfo);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CheckCounter(const D3D11_COUNTER_DESC *pDesc, D3D11_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength)
		{
			return m_Device->CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CheckFeatureSupport(D3D11_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize)
		{
			return m_Device->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData)
		{
			return m_Device->GetPrivateData(guid, pDataSize, pData);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::SetPrivateData(REFGUID guid, UINT DataSize, const void *pData)
		{
			return m_Device->SetPrivateData(guid, DataSize, pData);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::SetPrivateDataInterface(REFGUID guid, const IUnknown *pData)
		{
			return m_Device->SetPrivateDataInterface(guid, pData);
		}

		UINT STDMETHODCALLTYPE D3D11DeviceProxy::GetCreationFlags()
		{
			return m_Device->GetCreationFlags();
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::GetDeviceRemovedReason()
		{
			return m_Device->GetDeviceRemovedReason();
		}

		void STDMETHODCALLTYPE D3D11DeviceProxy::GetImmediateContext(ID3D11DeviceContext **ppImmediateContext)
		{
			m_ContextProxy->AddRef();
			*ppImmediateContext = m_ContextProxy;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::SetExceptionMode(UINT RaiseFlags)
		{
			return m_Device->SetExceptionMode(RaiseFlags);
		}

		UINT STDMETHODCALLTYPE D3D11DeviceProxy::GetExceptionMode()
		{
			return m_Device->GetExceptionMode();
		}

		D3D_FEATURE_LEVEL STDMETHODCALLTYPE D3D11DeviceProxy::GetFeatureLevel()
		{
			return m_Device->GetFeatureLevel();
		}

		// ID3D11Device1
		void STDMETHODCALLTYPE D3D11DeviceProxy::GetImmediateContext1(ID3D11DeviceContext1 **ppImmediateContext)
		{
			m_ContextProxy->AddRef();
			*ppImmediateContext = m_ContextProxy;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateDeferredContext1(UINT ContextFlags, ID3D11DeviceContext1 **ppDeferredContext)
		{
			HRESULT hr = m_Device->CreateDeferredContext1(ContextFlags, ppDeferredContext);

			if (SUCCEEDED(hr))
				*ppDeferredContext = new D3D11DeviceContextProxy(*ppDeferredContext);

			return hr;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateBlendState1(const D3D11_BLEND_DESC1 *pBlendStateDesc, ID3D11BlendState1 **ppBlendState)
		{
			return m_Device->CreateBlendState1(pBlendStateDesc, ppBlendState);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateRasterizerState1(const D3D11_RASTERIZER_DESC1 *pRasterizerDesc, ID3D11RasterizerState1 **ppRasterizerState)
		{
			return m_Device->CreateRasterizerState1(pRasterizerDesc, ppRasterizerState);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateDeviceContextState(UINT Flags, const D3D_FEATURE_LEVEL *pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, REFIID EmulatedInterface, D3D_FEATURE_LEVEL *pChosenFeatureLevel, ID3DDeviceContextState **ppContextState)
		{
			return m_Device->CreateDeviceContextState(Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::OpenSharedResource1(HANDLE hResource, REFIID returnedInterface, void **ppResource)
		{
			return m_Device->OpenSharedResource1(hResource, returnedInterface, ppResource);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::OpenSharedResourceByName(LPCWSTR lpName, DWORD dwDesiredAccess, REFIID returnedInterface, void **ppResource)
		{
			return m_Device->OpenSharedResourceByName(lpName, dwDesiredAccess, returnedInterface, ppResource);
		}

		// ID3D11Device2
		void STDMETHODCALLTYPE D3D11DeviceProxy::GetImmediateContext2(ID3D11DeviceContext2 **ppImmediateContext)
		{
			m_ContextProxy->AddRef();
			*ppImmediateContext = m_ContextProxy;
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CreateDeferredContext2(UINT ContextFlags, ID3D11DeviceContext2 **ppDeferredContext)
		{
			return m_Device->CreateDeferredContext2(ContextFlags, ppDeferredContext);
		}

		void STDMETHODCALLTYPE D3D11DeviceProxy::GetResourceTiling(ID3D11Resource *pTiledResource, UINT *pNumTilesForEntireResource, D3D11_PACKED_MIP_DESC *pPackedMipDesc, D3D11_TILE_SHAPE *pStandardTileShapeForNonPackedMips, UINT *pNumSubresourceTilings, UINT FirstSubresourceTilingToGet, D3D11_SUBRESOURCE_TILING *pSubresourceTilingsForNonPackedMips)
		{
			m_Device->GetResourceTiling(pTiledResource, pNumTilesForEntireResource, pPackedMipDesc, pStandardTileShapeForNonPackedMips, pNumSubresourceTilings, FirstSubresourceTilingToGet, pSubresourceTilingsForNonPackedMips);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceProxy::CheckMultisampleQualityLevels1(DXGI_FORMAT Format, UINT SampleCount, UINT Flags, UINT *pNumQualityLevels)
		{
			return m_Device->CheckMultisampleQualityLevels1(Format, SampleCount, Flags, pNumQualityLevels);
		}

		D3D11DeviceContextProxy::D3D11DeviceContextProxy(ID3D11DeviceContext *Context)
		{
			HRESULT hr = Context->QueryInterface<ID3D11DeviceContext2>(&m_Context);

			AssertMsg(SUCCEEDED(hr), "D3D11.2 interface is not supported");
			Assert(m_Context->Release() > 0);

			// Grab ID3DUserDefinedAnnotation for profiling markers
			hr = m_Context->QueryInterface<ID3DUserDefinedAnnotation>(&m_UserAnnotation);

			if (!SUCCEEDED(hr))
				m_UserAnnotation = NULL;
		}

		D3D11DeviceContextProxy::D3D11DeviceContextProxy(ID3D11DeviceContext2 *Context)
		{
			m_Context = Context;

			// Grab ID3DUserDefinedAnnotation for profiling markers
			HRESULT hr = m_Context->QueryInterface<ID3DUserDefinedAnnotation>(&m_UserAnnotation);

			if (!SUCCEEDED(hr))
				m_UserAnnotation = NULL;
		}

		// IUnknown
		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::QueryInterface(REFIID riid, void **ppvObj)
		{
			Assert(FALSE);
			return E_NOTIMPL;
		}

		ULONG STDMETHODCALLTYPE D3D11DeviceContextProxy::AddRef()
		{
			return m_Context->AddRef();
		}

		ULONG STDMETHODCALLTYPE D3D11DeviceContextProxy::Release()
		{
			ULONG refCount = m_Context->Release();

			if (refCount <= 0)
			{
				if (m_UserAnnotation)
				{
					m_UserAnnotation->Release();
					m_UserAnnotation = NULL;
				}

				m_Context = NULL;
				delete this;
			}

			return refCount;
		}

		// ID3D11DeviceChild
		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GetDevice(ID3D11Device **ppDevice)
		{
			//AssertMsg(false, "TODO: This call must be proxied");
			m_Context->GetDevice(ppDevice);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData)
		{
			return m_Context->GetPrivateData(guid, pDataSize, pData);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::SetPrivateData(REFGUID guid, UINT DataSize, const void *pData)
		{
			return m_Context->SetPrivateData(guid, DataSize, pData);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::SetPrivateDataInterface(REFGUID guid, const IUnknown *pData)
		{
			return m_Context->SetPrivateDataInterface(guid, pData);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
		{
			m_Context->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_Context->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSSetShader(ID3D11PixelShader *pPixelShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances)
		{
			m_Context->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_Context->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSSetShader(ID3D11VertexShader *pVertexShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances)
		{
			m_Context->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
		{
			//ProfileCounterInc("Draw Calls");

			m_Context->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::Draw(UINT VertexCount, UINT StartVertexLocation)
		{
			//ProfileCounterInc("Draw Calls");

			m_Context->Draw(VertexCount, StartVertexLocation);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::Map(ID3D11Resource *pResource, UINT Subresource, D3D11_MAP MapType, UINT MapFlags, D3D11_MAPPED_SUBRESOURCE *pMappedResource)
		{
			return m_Context->Map(pResource, Subresource, MapType, MapFlags, pMappedResource);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::Unmap(ID3D11Resource *pResource, UINT Subresource)
		{
			m_Context->Unmap(pResource, Subresource);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
		{
			m_Context->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IASetInputLayout(ID3D11InputLayout *pInputLayout)
		{
			m_Context->IASetInputLayout(pInputLayout);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IASetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets)
		{
			m_Context->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IASetIndexBuffer(ID3D11Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
		{
			m_Context->IASetIndexBuffer(pIndexBuffer, Format, Offset);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
		{
			//ProfileCounterInc("Draw Calls");

			m_Context->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
		{
			//ProfileCounterInc("Draw Calls");

			m_Context->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
		{
			m_Context->GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSSetShader(ID3D11GeometryShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances)
		{
			m_Context->GSSetShader(pShader, ppClassInstances, NumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology)
		{
			m_Context->IASetPrimitiveTopology(Topology);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_Context->VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_Context->VSSetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::Begin(ID3D11Asynchronous *pAsync)
		{
			m_Context->Begin(pAsync);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::End(ID3D11Asynchronous *pAsync)
		{
			m_Context->End(pAsync);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::GetData(ID3D11Asynchronous *pAsync, void *pData, UINT DataSize, UINT GetDataFlags)
		{
			return m_Context->GetData(pAsync, pData, DataSize, GetDataFlags);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::SetPredication(ID3D11Predicate *pPredicate, BOOL PredicateValue)
		{
			m_Context->SetPredication(pPredicate, PredicateValue);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_Context->GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_Context->GSSetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMSetRenderTargets(UINT NumViews, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView)
		{
			m_Context->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMSetRenderTargetsAndUnorderedAccessViews(UINT NumRTVs, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts)
		{
			m_Context->OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMSetBlendState(ID3D11BlendState *pBlendState, const FLOAT BlendFactor[4], UINT SampleMask)
		{
			m_Context->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMSetDepthStencilState(ID3D11DepthStencilState *pDepthStencilState, UINT StencilRef)
		{
			m_Context->OMSetDepthStencilState(pDepthStencilState, StencilRef);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::SOSetTargets(UINT NumBuffers, ID3D11Buffer *const *ppSOTargets, const UINT *pOffsets)
		{
			m_Context->SOSetTargets(NumBuffers, ppSOTargets, pOffsets);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DrawAuto()
		{
			//ProfileCounterInc("Draw Calls");

			m_Context->DrawAuto();
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DrawIndexedInstancedIndirect(ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
		{
			//ProfileCounterInc("Draw Calls");

			m_Context->DrawIndexedInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DrawInstancedIndirect(ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
		{
			//ProfileCounterInc("Draw Calls");

			m_Context->DrawInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
		{
			//ProfileCounterInc("Dispatch Calls");

			m_Context->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DispatchIndirect(ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
		{
			//ProfileCounterInc("Dispatch Calls");

			m_Context->DispatchIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::RSSetState(ID3D11RasterizerState *pRasterizerState)
		{
			m_Context->RSSetState(pRasterizerState);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::RSSetViewports(UINT NumViewports, const D3D11_VIEWPORT *pViewports)
		{
			m_Context->RSSetViewports(NumViewports, pViewports);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::RSSetScissorRects(UINT NumRects, const D3D11_RECT *pRects)
		{
			m_Context->RSSetScissorRects(NumRects, pRects);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CopySubresourceRegion(ID3D11Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource *pSrcResource, UINT SrcSubresource, const D3D11_BOX *pSrcBox)
		{
			m_Context->CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CopyResource(ID3D11Resource *pDstResource, ID3D11Resource *pSrcResource)
		{
			m_Context->CopyResource(pDstResource, pSrcResource);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::UpdateSubresource(ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
		{
			m_Context->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CopyStructureCount(ID3D11Buffer *pDstBuffer, UINT DstAlignedByteOffset, ID3D11UnorderedAccessView *pSrcView)
		{
			m_Context->CopyStructureCount(pDstBuffer, DstAlignedByteOffset, pSrcView);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ClearRenderTargetView(ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4])
		{
			m_Context->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ClearUnorderedAccessViewUint(ID3D11UnorderedAccessView *pUnorderedAccessView, const UINT Values[4])
		{
			m_Context->ClearUnorderedAccessViewUint(pUnorderedAccessView, Values);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ClearUnorderedAccessViewFloat(ID3D11UnorderedAccessView *pUnorderedAccessView, const FLOAT Values[4])
		{
			m_Context->ClearUnorderedAccessViewFloat(pUnorderedAccessView, Values);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ClearDepthStencilView(ID3D11DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
		{
			m_Context->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GenerateMips(ID3D11ShaderResourceView *pShaderResourceView)
		{
			m_Context->GenerateMips(pShaderResourceView);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::SetResourceMinLOD(ID3D11Resource *pResource, FLOAT MinLOD)
		{
			m_Context->SetResourceMinLOD(pResource, MinLOD);
		}

		FLOAT STDMETHODCALLTYPE D3D11DeviceContextProxy::GetResourceMinLOD(ID3D11Resource *pResource)
		{
			return m_Context->GetResourceMinLOD(pResource);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ResolveSubresource(ID3D11Resource *pDstResource, UINT DstSubresource, ID3D11Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format)
		{
			m_Context->ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ExecuteCommandList(ID3D11CommandList *pCommandList, BOOL RestoreContextState)
		{
			m_Context->ExecuteCommandList(pCommandList, RestoreContextState);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_Context->HSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSSetShader(ID3D11HullShader *pHullShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances)
		{
			m_Context->HSSetShader(pHullShader, ppClassInstances, NumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_Context->HSSetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
		{
			m_Context->HSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_Context->DSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSSetShader(ID3D11DomainShader *pDomainShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances)
		{
			m_Context->DSSetShader(pDomainShader, ppClassInstances, NumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_Context->DSSetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
		{
			m_Context->DSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_Context->CSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSSetUnorderedAccessViews(UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts)
		{
			m_Context->CSSetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSSetShader(ID3D11ComputeShader *pComputeShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances)
		{
			m_Context->CSSetShader(pComputeShader, ppClassInstances, NumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_Context->CSSetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
		{
			m_Context->CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_Context->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews)
		{
			m_Context->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSGetShader(ID3D11PixelShader **ppPixelShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances)
		{
			m_Context->PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers)
		{
			m_Context->PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSGetShader(ID3D11VertexShader **ppVertexShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances)
		{
			m_Context->VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_Context->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IAGetInputLayout(ID3D11InputLayout **ppInputLayout)
		{
			m_Context->IAGetInputLayout(ppInputLayout);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IAGetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets)
		{
			m_Context->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IAGetIndexBuffer(ID3D11Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset)
		{
			m_Context->IAGetIndexBuffer(pIndexBuffer, Format, Offset);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_Context->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSGetShader(ID3D11GeometryShader **ppGeometryShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances)
		{
			m_Context->GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::IAGetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY *pTopology)
		{
			m_Context->IAGetPrimitiveTopology(pTopology);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews)
		{
			m_Context->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers)
		{
			m_Context->VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GetPredication(ID3D11Predicate **ppPredicate, BOOL *pPredicateValue)
		{
			m_Context->GetPredication(ppPredicate, pPredicateValue);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews)
		{
			m_Context->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers)
		{
			m_Context->GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMGetRenderTargets(UINT NumViews, ID3D11RenderTargetView **ppRenderTargetViews, ID3D11DepthStencilView **ppDepthStencilView)
		{
			m_Context->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMGetRenderTargetsAndUnorderedAccessViews(UINT NumRTVs, ID3D11RenderTargetView **ppRenderTargetViews, ID3D11DepthStencilView **ppDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView **ppUnorderedAccessViews)
		{
			m_Context->OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMGetBlendState(ID3D11BlendState **ppBlendState, FLOAT BlendFactor[4], UINT *pSampleMask)
		{
			m_Context->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::OMGetDepthStencilState(ID3D11DepthStencilState **ppDepthStencilState, UINT *pStencilRef)
		{
			m_Context->OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::SOGetTargets(UINT NumBuffers, ID3D11Buffer **ppSOTargets)
		{
			m_Context->SOGetTargets(NumBuffers, ppSOTargets);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::RSGetState(ID3D11RasterizerState **ppRasterizerState)
		{
			m_Context->RSGetState(ppRasterizerState);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::RSGetViewports(UINT *pNumViewports, D3D11_VIEWPORT *pViewports)
		{
			m_Context->RSGetViewports(pNumViewports, pViewports);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::RSGetScissorRects(UINT *pNumRects, D3D11_RECT *pRects)
		{
			m_Context->RSGetScissorRects(pNumRects, pRects);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews)
		{
			m_Context->HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSGetShader(ID3D11HullShader **ppHullShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances)
		{
			m_Context->HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers)
		{
			m_Context->HSGetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_Context->HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews)
		{
			m_Context->DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSGetShader(ID3D11DomainShader **ppDomainShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances)
		{
			m_Context->DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers)
		{
			m_Context->DSGetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_Context->DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews)
		{
			m_Context->CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSGetUnorderedAccessViews(UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView **ppUnorderedAccessViews)
		{
			m_Context->CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSGetShader(ID3D11ComputeShader **ppComputeShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances)
		{
			m_Context->CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers)
		{
			m_Context->CSGetSamplers(StartSlot, NumSamplers, ppSamplers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_Context->CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ClearState()
		{
			m_Context->ClearState();
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::Flush()
		{
			m_Context->Flush();
		}

		UINT STDMETHODCALLTYPE D3D11DeviceContextProxy::GetContextFlags()
		{
			return m_Context->GetContextFlags();
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::FinishCommandList(BOOL RestoreDeferredContextState, ID3D11CommandList **ppCommandList)
		{
			return m_Context->FinishCommandList(RestoreDeferredContextState, ppCommandList);
		}

		D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE D3D11DeviceContextProxy::GetType()
		{
			return m_Context->GetType();
		}

		// ID3D11DeviceContext1
		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CopySubresourceRegion1(ID3D11Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource *pSrcResource, UINT SrcSubresource, const D3D11_BOX *pSrcBox, UINT CopyFlags)
		{
			m_Context->CopySubresourceRegion1(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox, CopyFlags);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::UpdateSubresource1(ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch, UINT CopyFlags)
		{
			m_Context->UpdateSubresource1(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch, CopyFlags);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DiscardResource(ID3D11Resource *pResource)
		{
			m_Context->DiscardResource(pResource);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DiscardView(ID3D11View *pResourceView)
		{
			m_Context->DiscardView(pResourceView);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSSetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers, const UINT *pFirstConstant, const UINT *pNumConstants)
		{
			m_Context->VSSetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSSetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers, const UINT *pFirstConstant, const UINT *pNumConstants)
		{
			m_Context->HSSetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSSetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers, const UINT *pFirstConstant, const UINT *pNumConstants)
		{
			m_Context->DSSetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSSetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers, const UINT *pFirstConstant, const UINT *pNumConstants)
		{
			m_Context->GSSetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSSetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers, const UINT *pFirstConstant, const UINT *pNumConstants)
		{
			m_Context->PSSetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSSetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers, const UINT *pFirstConstant, const UINT *pNumConstants)
		{
			m_Context->CSSetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::VSGetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers, UINT *pFirstConstant, UINT *pNumConstants)
		{
			m_Context->VSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::HSGetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers, UINT *pFirstConstant, UINT *pNumConstants)
		{
			m_Context->HSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DSGetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers, UINT *pFirstConstant, UINT *pNumConstants)
		{
			m_Context->DSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::GSGetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers, UINT *pFirstConstant, UINT *pNumConstants)
		{
			m_Context->GSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::PSGetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers, UINT *pFirstConstant, UINT *pNumConstants)
		{
			m_Context->PSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CSGetConstantBuffers1(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers, UINT *pFirstConstant, UINT *pNumConstants)
		{
			m_Context->CSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::SwapDeviceContextState(ID3DDeviceContextState *pState, ID3DDeviceContextState **ppPreviousState)
		{
			m_Context->SwapDeviceContextState(pState, ppPreviousState);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::ClearView(ID3D11View *pView, const FLOAT Color[4], const D3D11_RECT *pRect, UINT NumRects)
		{
			m_Context->ClearView(pView, Color, pRect, NumRects);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::DiscardView1(ID3D11View *pResourceView, const D3D11_RECT *pRects, UINT NumRects)
		{
			m_Context->DiscardView1(pResourceView, pRects, NumRects);
		}

		// ID3D11DeviceContext2
		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::UpdateTileMappings(ID3D11Resource *pTiledResource, UINT NumTiledResourceRegions, const D3D11_TILED_RESOURCE_COORDINATE *pTiledResourceRegionStartCoordinates, const D3D11_TILE_REGION_SIZE *pTiledResourceRegionSizes, ID3D11Buffer *pTilePool, UINT NumRanges, const UINT *pRangeFlags, const UINT *pTilePoolStartOffsets, const UINT *pRangeTileCounts, UINT Flags)
		{
			return m_Context->UpdateTileMappings(pTiledResource, NumTiledResourceRegions, pTiledResourceRegionStartCoordinates, pTiledResourceRegionSizes, pTilePool, NumRanges, pRangeFlags, pTilePoolStartOffsets, pRangeTileCounts, Flags);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::CopyTileMappings(ID3D11Resource *pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE *pDestRegionStartCoordinate, ID3D11Resource *pSourceTiledResource, const D3D11_TILED_RESOURCE_COORDINATE *pSourceRegionStartCoordinate, const D3D11_TILE_REGION_SIZE *pTileRegionSize, UINT Flags)
		{
			return m_Context->CopyTileMappings(pDestTiledResource, pDestRegionStartCoordinate, pSourceTiledResource, pSourceRegionStartCoordinate, pTileRegionSize, Flags);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::CopyTiles(ID3D11Resource *pTiledResource, const D3D11_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate, const D3D11_TILE_REGION_SIZE *pTileRegionSize, ID3D11Buffer *pBuffer, UINT64 BufferStartOffsetInBytes, UINT Flags)
		{
			m_Context->CopyTiles(pTiledResource, pTileRegionStartCoordinate, pTileRegionSize, pBuffer, BufferStartOffsetInBytes, Flags);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::UpdateTiles(ID3D11Resource *pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE *pDestTileRegionStartCoordinate, const D3D11_TILE_REGION_SIZE *pDestTileRegionSize, const void *pSourceTileData, UINT Flags)
		{
			m_Context->UpdateTiles(pDestTiledResource, pDestTileRegionStartCoordinate, pDestTileRegionSize, pSourceTileData, Flags);
		}

		HRESULT STDMETHODCALLTYPE D3D11DeviceContextProxy::ResizeTilePool(ID3D11Buffer *pTilePool, UINT64 NewSizeInBytes)
		{
			return m_Context->ResizeTilePool(pTilePool, NewSizeInBytes);
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::TiledResourceBarrier(ID3D11DeviceChild *pTiledResourceOrViewAccessBeforeBarrier, ID3D11DeviceChild *pTiledResourceOrViewAccessAfterBarrier)
		{
			m_Context->TiledResourceBarrier(pTiledResourceOrViewAccessBeforeBarrier, pTiledResourceOrViewAccessAfterBarrier);
		}

		BOOL STDMETHODCALLTYPE D3D11DeviceContextProxy::IsAnnotationEnabled()
		{
			if (m_UserAnnotation)
				return m_UserAnnotation->GetStatus();

			return FALSE;

		#if 0
			return m_Context->IsAnnotationEnabled();
		#endif
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::SetMarkerInt(LPCWSTR pLabel, INT Data)
		{
			if (m_UserAnnotation)
				m_UserAnnotation->SetMarker(pLabel);

		#if 0
			m_Context->SetMarkerInt(pLabel, Data);
		#endif
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::BeginEventInt(LPCWSTR pLabel, INT Data)
		{
			if (m_UserAnnotation)
				m_UserAnnotation->BeginEvent(pLabel);

		#if 0
			m_Context->BeginEventInt(pLabel, Data);
		#endif
		}

		void STDMETHODCALLTYPE D3D11DeviceContextProxy::EndEvent()
		{
			if (m_UserAnnotation)
				m_UserAnnotation->EndEvent();

		#if 0
			m_Context->EndEvent();
		#endif
		}
	}
}