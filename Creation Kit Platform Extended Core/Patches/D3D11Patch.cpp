// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/D3D11Proxy.h"
#include "D3D11Patch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		decltype(&CreateDXGIFactory) ptrCreateDXGIFactory = nullptr;
		decltype(&D3D11CreateDeviceAndSwapChain) ptrD3D11CreateDeviceAndSwapChain = nullptr;
		ID3D11Device2* pointer_d3d11Device2Intf = nullptr;
		ID3D11Device* pointer_d3d11DeviceIntf = nullptr;
		IDXGISwapChain* pointer_dxgiSwapChain = nullptr;
		ID3D11DeviceContext* pointer_d3d11DeviceContext = nullptr;

		D3D11Patch::D3D11Patch() : Module(GlobalEnginePtr), moduleDXGI(NULL), moduleD3D11(NULL)
		{}

		bool D3D11Patch::HasOption() const
		{
			return true;
		}

		bool D3D11Patch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* D3D11Patch::GetOptionName() const
		{
			return "CreationKit:bD3D11Patch";
		}

		const char* D3D11Patch::GetName() const
		{
			return "D3D11 Patch";
		}

		bool D3D11Patch::HasDependencies() const
		{
			return false;
		}

		Array<String> D3D11Patch::GetDependencies() const
		{
			return {};
		}

		bool D3D11Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			if (eEditorCurrentVersion < EDITOR_STARFIELD_1_12_32_0)
			{
				auto SystemVersion = _engine->GetSystemVersion();
				// Win 8.1 and newer
				return ((SystemVersion.MajorVersion > 6) || ((SystemVersion.MajorVersion == 6) && (SystemVersion.MinorVersion == 3)));
			}

			return false;
		}

		bool D3D11Patch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				auto Section = GlobalEnginePtr->GetSection(SECTION_TEXT);
				auto Patterns = voltek::find_patterns(Section.base, Section.end - Section.base, "81 ? ? ? ? ? 00 B0 00 00");

				if (Patterns.size() != 1)
				{
					_ERROR("Can't find the D3D11 level check.");
					return false;
				}

				auto addr = Patterns[0];
				if (((uint8_t*)addr)[10] != 0x74)
				{
					_ERROR("Can't find the D3D11 level check.");
					return false;
				}

				{
					ScopeRelocator text;
					((uint8_t*)addr)[10] = 0xEB;
				}

				// Grab the original function pointers
				moduleDXGI = LoadLibraryExA("dxgi.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				Assert(moduleDXGI);
				moduleD3D11 = LoadLibraryExA("d3d11.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				Assert(moduleD3D11);

				*(FARPROC*)&ptrCreateDXGIFactory = GetProcAddress(moduleDXGI, "CreateDXGIFactory1");
				if (!ptrCreateDXGIFactory)
					*(FARPROC*)&ptrCreateDXGIFactory = GetProcAddress(moduleDXGI, "CreateDXGIFactory");
				*(FARPROC*)&ptrD3D11CreateDeviceAndSwapChain = GetProcAddress(moduleD3D11, "D3D11CreateDeviceAndSwapChain");

				AssertMsg(ptrCreateDXGIFactory, "CreateDXGIFactory import not found");
				AssertMsg(ptrD3D11CreateDeviceAndSwapChain, "D3D11CreateDeviceAndSwapChain import not found");

				PatchIAT(HKCreateDXGIFactory, "dxgi.dll", "CreateDXGIFactory");
				PatchIAT(HKD3D11CreateDeviceAndSwapChain, "d3d11.dll", "D3D11CreateDeviceAndSwapChain");

				return true;
			}

			return false;
		}

		bool D3D11Patch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		HRESULT WINAPI D3D11Patch::HKCreateDXGIFactory(REFIID riid, void** ppFactory)
		{
			if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory3), ppFactory)))
				return S_OK;

			if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory2), ppFactory)))
				return S_OK;

			return ptrCreateDXGIFactory(__uuidof(IDXGIFactory), ppFactory);
		}

		HRESULT WINAPI D3D11Patch::HKD3D11CreateDeviceAndSwapChain(
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
			ID3D11DeviceContext** ppImmediateContext)
		{
			//
			// From MSDN:
			//
			// If the Direct3D 11.1 runtime is present on the computer and pFeatureLevels is set to NULL,
			// this function won't create a D3D_FEATURE_LEVEL_11_1 device. To create a D3D_FEATURE_LEVEL_11_1
			// device, you must explicitly provide a D3D_FEATURE_LEVEL array that includes
			// D3D_FEATURE_LEVEL_11_1. If you provide a D3D_FEATURE_LEVEL array that contains
			// D3D_FEATURE_LEVEL_11_1 on a computer that doesn't have the Direct3D 11.1 runtime installed,
			// this function immediately fails with E_INVALIDARG.
			//
			const D3D_FEATURE_LEVEL testFeatureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
			};

			D3D_FEATURE_LEVEL level;
			HRESULT hr;

			for (INT i = 0; i < ARRAYSIZE(testFeatureLevels); i++) 
			{
				hr = NsightHack_D3D11CreateDeviceAndSwapChain(
					pAdapter,
					DriverType,
					Software,
					Flags,
					&testFeatureLevels[i],
					1,
					SDKVersion,
					pSwapChainDesc,
					ppSwapChain,
					ppDevice,
					&level,
					ppImmediateContext);

				if (SUCCEEDED(hr)) 
				{
					if (pFeatureLevel)
						*pFeatureLevel = level;

					break;
				}
			}

			if (FAILED(hr)) 
				AssertMsg(FALSE, "DirectX11 device creation failed. Creation Kit will now exit.");
			
			_MESSAGE("Created D3D11 device with feature level %X...", level);
			if (SUCCEEDED((*ppDevice)->QueryInterface<ID3D11Device2>(&pointer_d3d11Device2Intf))) 
			{
				// Force DirectX11.2 in case we use features later (11.3+ requires Win10 or higher)
				ID3D11Device2* proxyDevice = new D3D11DeviceProxy(*ppDevice);
				ID3D11DeviceContext2* proxyContext = new D3D11DeviceContextProxy(*ppImmediateContext);

				pointer_d3d11DeviceIntf = proxyDevice;
				*ppDevice = proxyDevice;
				pointer_d3d11DeviceContext = proxyContext;
				*ppImmediateContext = proxyContext;

				_MESSAGE("Force DirectX11.2");
			}
			else 
			{
				pointer_d3d11DeviceIntf = *ppDevice;
				pointer_d3d11DeviceContext = *ppImmediateContext;
			}

			(*ppDevice)->SetExceptionMode(D3D11_RAISE_FLAG_DRIVER_INTERNAL_ERROR);
			pointer_dxgiSwapChain = *ppSwapChain;

			return hr;
		}

		HRESULT WINAPI D3D11Patch::NsightHack_D3D11CreateDeviceAndSwapChain(
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
			ID3D11DeviceContext** ppImmediateContext)
		{
			//
			// Nvidia NSight checks the return address of D3D11CreateDeviceAndSwapChain to see if it's
			// a blacklisted directx dll. "d3dx9_42.dll" happens to be in that list. So, now I need to
			// generate code which spoofs the return address to something random.
			//
			// NOTE: Do NOT touch rcx, rdx, r8, r9
			//
			class d3djmp : public Xbyak::CodeGenerator {
			public:
				d3djmp() : Xbyak::CodeGenerator() {
					push(rbx);
					push(rsi);
					push(rdi);
					sub(rsp, 0x60);
					mov(rax, qword[rsp + 0xD8]);
					mov(r10, qword[rsp + 0xD0]);
					mov(r11, qword[rsp + 0xC8]);
					mov(rbx, qword[rsp + 0xC0]);
					mov(rdi, qword[rsp + 0xB8]);
					mov(rsi, qword[rsp + 0xA0]);
					mov(qword[rsp + 0x58], rax);
					mov(eax, dword[rsp + 0xB0]);
					mov(qword[rsp + 0x50], r10);
					mov(qword[rsp + 0x48], r11);
					mov(qword[rsp + 0x40], rbx);
					mov(qword[rsp + 0x38], rdi);
					mov(dword[rsp + 0x30], eax);
					mov(eax, dword[rsp + 0xA8]);
					mov(dword[rsp + 0x28], eax);
					mov(qword[rsp + 0x20], rsi);

					mov(rax, (uintptr_t)ptrD3D11CreateDeviceAndSwapChain);
					call(rax);

					add(rsp, 0x60);
					pop(rdi);
					pop(rsi);
					pop(rbx);
					ret();
				}
			} hack;

			auto newPtr = hack.getCode<decltype(&D3D11CreateDeviceAndSwapChain)>();
			return newPtr(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
		}
	}
}