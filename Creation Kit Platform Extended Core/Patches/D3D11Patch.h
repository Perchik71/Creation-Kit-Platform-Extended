// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class D3D11Patch : public Module
		{
		public:
			D3D11Patch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static HRESULT WINAPI HKCreateDXGIFactory(REFIID riid, void** ppFactory);
			static HRESULT WINAPI HKD3D11CreateDeviceAndSwapChain(
				IDXGIAdapter* pAdapter,
				D3D_DRIVER_TYPE DriverType,
				HMODULE Software,
				UINT Flags,
				const D3D_FEATURE_LEVEL* pFeatureLevels,
				UINT FeatureLevels,
				UINT SDKVersion,
				const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
				IDXGISwapChain** ppSwapChain,
				ID3D11Device** ppDevice,
				D3D_FEATURE_LEVEL* pFeatureLevel,
				ID3D11DeviceContext** ppImmediateContext);
			static HRESULT WINAPI NsightHack_D3D11CreateDeviceAndSwapChain(
				IDXGIAdapter* pAdapter,
				D3D_DRIVER_TYPE DriverType,
				HMODULE Software,
				UINT Flags,
				const D3D_FEATURE_LEVEL* pFeatureLevels,
				UINT FeatureLevels,
				UINT SDKVersion,
				const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
				IDXGISwapChain** ppSwapChain,
				ID3D11Device** ppDevice,
				D3D_FEATURE_LEVEL* pFeatureLevel,
				ID3D11DeviceContext** ppImmediateContext);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			D3D11Patch(const D3D11Patch&) = default;
			D3D11Patch& operator=(const D3D11Patch&) = default;

			HMODULE moduleDXGI;
			HMODULE moduleD3D11;
		};
	}
}