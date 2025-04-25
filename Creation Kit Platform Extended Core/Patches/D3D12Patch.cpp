// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/Shaders.h"
#include "D3D12Patch.h"

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx12.h>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		extern decltype(&CreateDXGIFactory) ptrCreateDXGIFactory;
		decltype(&D3D12CreateDevice) ptrD3D12CreateDevice = nullptr;
		ID3D12Device* pointer_d3d12DeviceIntf = nullptr;
		extern IDXGISwapChain* pointer_dxgiSwapChain;
		extern ImFont* imguiFonts[3];

		static const ImWchar GlobalFontRanges[] =
		{
			0x0020, 0xFFFF, // ALL
			0,
		};

		D3D12Patch::D3D12Patch() : Module(GlobalEnginePtr), moduleDXGI(NULL), moduleD3D12(NULL)
		{}

		bool D3D12Patch::HasOption() const
		{
			return false;
		}

		bool D3D12Patch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* D3D12Patch::GetOptionName() const
		{
			return nullptr;
		}

		const char* D3D12Patch::GetName() const
		{
			return "D3D12 Patch";
		}

		bool D3D12Patch::HasDependencies() const
		{
			return false;
		}

		Array<String> D3D12Patch::GetDependencies() const
		{
			return {};
		}

		bool D3D12Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			if (eEditorCurrentVersion >= EDITOR_STARFIELD_1_14_78_0)
			{
				auto SystemVersion = _engine->GetSystemVersion();
				// Win 11 and newer
				return (SystemVersion.MajorVersion == 10) && (SystemVersion.BuildNubmer >= 22000);
			}

			return false;
		}

		bool D3D12Patch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				// Grab the original function pointers
				moduleDXGI = LoadLibraryExA("dxgi.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				Assert(moduleDXGI);
				moduleD3D12 = LoadLibraryExA("d3d12.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				Assert(moduleD3D12);

				*(FARPROC*)&ptrCreateDXGIFactory = GetProcAddress(moduleDXGI, "CreateDXGIFactory2");
				AssertMsg(ptrCreateDXGIFactory, "CreateDXGIFactory import not found");
				*(FARPROC*)&ptrD3D12CreateDevice = GetProcAddress(moduleD3D12, "D3D12CreateDevice");
				AssertMsg(ptrD3D12CreateDevice, "D3D12CreateDevice import not found");

				PatchIAT(HKCreateDXGIFactory, "dxgi.dll", "CreateDXGIFactory");
				PatchIAT(HKD3D12CreateDevice, "d3d12.dll", "D3D12CreateDevice");

				return true;
			}

			return false;
		}

		bool D3D12Patch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			ImGui_ImplDX12_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			return false;
		}

		HRESULT WINAPI D3D12Patch::HKCreateDXGIFactory(REFIID riid, void** ppFactory)
		{
			if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory3), ppFactory)))
				return S_OK;

			if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory2), ppFactory)))
				return S_OK;

			return ptrCreateDXGIFactory(__uuidof(IDXGIFactory), ppFactory);
		}

		HRESULT WINAPI D3D12Patch::HKD3D12CreateDevice(IUnknown* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, 
			REFIID riid, void** ppDevice)
		{
			auto hr = ptrD3D12CreateDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);
			if (SUCCEEDED(hr))
			{
				pointer_d3d12DeviceIntf = (ID3D12Device*)(*ppDevice);

				//ImGui_ImplWin32_Init(pSwapChainDesc->OutputWindow);
				//ImGui_ImplDX12_Init(*ppDevice, *ppImmediateContext);

				IMGUI_CHECKVERSION();
				ImGui::CreateContext();

				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
				io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
				io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;		// Hide cursor

				io.Fonts->AddFontDefault();

				char path[MAX_PATH];
				if (FAILED(SHGetFolderPath(NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, path)))
					return E_FAIL;
				String ps(path);

				imguiFonts[0] = io.Fonts->AddFontFromFileTTF((ps + "\\consola.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
				imguiFonts[1] = io.Fonts->AddFontFromFileTTF((ps + "\\consolab.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
				imguiFonts[2] = io.Fonts->AddFontFromFileTTF((ps + "\\consola.ttf").c_str(), 10.0f, nullptr, GlobalFontRanges);
				if (!imguiFonts[0] || !imguiFonts[1] || !imguiFonts[2])
					return E_FAIL;
			}

			return hr;
		}
	}
}