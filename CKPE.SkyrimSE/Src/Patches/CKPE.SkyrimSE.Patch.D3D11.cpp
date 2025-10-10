// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <shlobj_core.h>
#include <dxgi.h>
#include <d3d11_2.h>
#include <xbyak.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <CKPE.Utils.h>
#include <CKPE.Asserts.h>
#include <CKPE.Detours.h>
#include <CKPE.Patterns.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.D3D11Proxy.h>
#include <CKPE.SkyrimSE.D3D11Shaders.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.D3D11.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			decltype(&CreateDXGIFactory) ptrCreateDXGIFactory = nullptr;
			decltype(&D3D11CreateDeviceAndSwapChain) ptrD3D11CreateDeviceAndSwapChain = nullptr;
			ID3D11Device2* pointer_d3d11Device2Intf = nullptr;
			ID3D11Device* pointer_d3d11DeviceIntf = nullptr;
			IDXGISwapChain* pointer_dxgiSwapChain = nullptr;
			ID3D11DeviceContext* pointer_d3d11DeviceContext = nullptr;
			D3D11ShaderEngine* pointer_D3D11ShaderEngine = nullptr;
			std::uintptr_t gGlobAddrDeviceContext = 0;
			ImFont* imguiFonts[3];

			static const ImWchar GlobalFontRanges[] =
			{
				0x0020, 0xFFFF, // ALL
				0,
			};

			static HRESULT WINAPI HKCreateDXGIFactory(REFIID riid, void** ppFactory) noexcept(true)
			{
				if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory3), ppFactory)))
					return S_OK;

				if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory2), ppFactory)))
					return S_OK;

				return ptrCreateDXGIFactory(__uuidof(IDXGIFactory), ppFactory);
			}

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
				ID3D11DeviceContext** ppImmediateContext) noexcept(true)
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

			static std::uintptr_t old_Present = 0;

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
				ID3D11DeviceContext** ppImmediateContext) noexcept(true)
			{
				auto SwapChainDesc = const_cast<DXGI_SWAP_CHAIN_DESC*>(pSwapChainDesc);
				SwapChainDesc->BufferDesc.RefreshRate.Numerator = 60;
				SwapChainDesc->BufferDesc.RefreshRate.Denominator =
					(UINT)_READ_OPTION_BOOL("CreationKit", "bRenderWindowVSync", true);

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
					CKPE_ASSERT_MSG(FALSE, "DirectX11 device creation failed. Creation Kit will now exit.");

				_MESSAGE("Created D3D11 device with feature level %X...", level);
				if (SUCCEEDED((*ppDevice)->QueryInterface<ID3D11Device2>(&pointer_d3d11Device2Intf)))
				{
					// Force DirectX11.2 in case we use features later (11.3+ requires Win10 or higher)
					ID3D11Device2* proxyDevice = new Common::D3D11DeviceProxy(*ppDevice);
					ID3D11DeviceContext2* proxyContext = new Common::D3D11DeviceContextProxy(*ppImmediateContext);

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
				std::string ps(path);

				if (CKPE_UserUseWine())
				{
					imguiFonts[0] = io.Fonts->AddFontFromFileTTF((ps + "\\tahoma.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
					imguiFonts[1] = io.Fonts->AddFontFromFileTTF((ps + "\\tahomabd.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
					imguiFonts[2] = io.Fonts->AddFontFromFileTTF((ps + "\\tahoma.ttf").c_str(), 10.0f, nullptr, GlobalFontRanges);
				}
				else
				{
					imguiFonts[0] = io.Fonts->AddFontFromFileTTF((ps + "\\consola.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
					imguiFonts[1] = io.Fonts->AddFontFromFileTTF((ps + "\\consolab.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
					imguiFonts[2] = io.Fonts->AddFontFromFileTTF((ps + "\\consola.ttf").c_str(), 10.0f, nullptr, GlobalFontRanges);
				}

				if (!imguiFonts[0] || !imguiFonts[1] || !imguiFonts[2])
					return E_FAIL;
				
				ImGui_ImplWin32_Init(pSwapChainDesc->OutputWindow);
				ImGui_ImplDX11_Init(*ppDevice, *ppImmediateContext);

				pointer_D3D11ShaderEngine = new D3D11ShaderEngine(*ppDevice, *ppImmediateContext);
				gGlobAddrDeviceContext = (std::uintptr_t)ppImmediateContext;

				return hr;
			}

			D3D11::D3D11() : Common::Patch()
			{
				SetName("D3D11 Patch");
			}

			D3D11::~D3D11()
			{
				ImGui_ImplDX11_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();
			}

			bool D3D11::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* D3D11::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool D3D11::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> D3D11::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool D3D11::DoQuery() const noexcept(true)
			{
				// Win 8.1 and newer
				auto verOs = HardwareInfo::OS::GetVersion();
				return ((GET_EXE_VERSION_EX_MAJOR(verOs) > 6) ||
					((GET_EXE_VERSION_EX_MAJOR(verOs) == 6) && (GET_EXE_VERSION_EX_MINOR(verOs) == 3)));
			}

			bool D3D11::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				auto Section = _interface->GetApplication()->GetSegment(Segment::text);
				auto Patterns = Patterns::FindsByMask(Section.GetAddress(), Section.GetSize(), "81 ? ? ? ? ? 00 B0 00 00");		
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

				SafeWrite::Write(addr + 10, { 0xEB });

				// Grab the original function pointers
				auto moduleDXGI = LoadLibraryExA("dxgi.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				CKPE_ASSERT(moduleDXGI);
				auto moduleD3D11 = LoadLibraryExA("d3d11.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				CKPE_ASSERT(moduleD3D11);

				*(FARPROC*)&ptrCreateDXGIFactory = GetProcAddress(moduleDXGI, "CreateDXGIFactory1");
				if (!ptrCreateDXGIFactory)
					*(FARPROC*)&ptrCreateDXGIFactory = GetProcAddress(moduleDXGI, "CreateDXGIFactory");
				*(FARPROC*)&ptrD3D11CreateDeviceAndSwapChain = GetProcAddress(moduleD3D11, "D3D11CreateDeviceAndSwapChain");

				CKPE_ASSERT_MSG(ptrCreateDXGIFactory, "CreateDXGIFactory import not found");
				CKPE_ASSERT_MSG(ptrD3D11CreateDeviceAndSwapChain, "D3D11CreateDeviceAndSwapChain import not found");
				
				Detours::DetourIAT(base, "dxgi.dll", "CreateDXGIFactory", (std::uintptr_t)HKCreateDXGIFactory);
				Detours::DetourIAT(base, "d3d11.dll", "D3D11CreateDeviceAndSwapChain", (std::uintptr_t)HKD3D11CreateDeviceAndSwapChain);
				
				return true;
			}
		}
	}
}