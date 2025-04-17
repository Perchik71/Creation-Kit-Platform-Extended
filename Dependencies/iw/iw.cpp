/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
Copyright (c) 2025 Alex (Perchik71).

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <wrl/client.h>
#include <comdef.h>
#include <wbemidl.h>
#include <string.h>
#include <intrin.h>
#include <powerbase.h>
#include <memory>
#include <dxgi.h>

#include "iw.h"

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "powrprof.lib")
#pragma comment(lib, "dxgi.lib")

namespace iw
{
	namespace util
	{
		static std::string __stdcall to_ansi(const std::wstring& str) noexcept(true)
		{
			std::string r;
			auto il = WideCharToMultiByte(CP_ACP, 0, str.c_str(), (int)str.length(), NULL, 0, NULL, NULL);
			if (il > 0)
			{
				r.resize(il);
				WideCharToMultiByte(CP_ACP, 0, str.c_str(), (int)str.length(), const_cast<char*>(r.data()), (int)r.length(), NULL, NULL);
			}
			return r;
		}

		static std::wstring __stdcall to_unicode(const std::string& str) noexcept(true)
		{
			std::wstring r;
			auto il = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), NULL, 0);
			if (il > 0)
			{
				r.resize(il);
				MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), const_cast<wchar_t*>(r.data()), (int)r.length());
			}
			return r;
		}
	}

	namespace system
	{
		class scope_com
		{
			bool Init;
		public:
			scope_com()
			{
				Init = SUCCEEDED(CoInitializeEx(0, COINIT_MULTITHREADED));
				if (Init)
				{
					auto hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
						RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
					if (FAILED(hr) && (hr != RPC_E_TOO_LATE))
					{
						CoUninitialize();
						Init = false;
					}
				}
			}

			~scope_com()
			{
				if (Init)
				{
					CoUninitialize();
					Init = false;
				}
			}

			[[nodiscard]] inline bool is_init() const noexcept(true) { return Init; }
		};

		static constexpr const char* OS_VER_UNKNOWN = "Unknown Windows";

		std::string __stdcall version_name()
		{
			ULONG uReturn = 0;
			std::string ret = OS_VER_UNKNOWN;
			scope_com com;

			Microsoft::WRL::ComPtr<IWbemLocator> pLoc;
			Microsoft::WRL::ComPtr<IWbemServices> pSvc;
			Microsoft::WRL::ComPtr<IEnumWbemClassObject> pEnumerator;	

			if (!com.is_init())
				goto ver_determ_broken;

			if (FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)pLoc.GetAddressOf())))
				goto ver_determ_broken;

			if (FAILED(pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, pSvc.GetAddressOf())))
				goto ver_determ_broken;

			if (FAILED(CoSetProxyBlanket(pSvc.Get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
				RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
				goto ver_determ_broken;

			if (FAILED(pSvc->ExecQuery(_bstr_t("WQL"), _bstr_t("SELECT * FROM Win32_OperatingSystem"),
				WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, pEnumerator.GetAddressOf())))
				goto ver_determ_broken;

			if (pEnumerator)
			{
				Microsoft::WRL::ComPtr<IWbemClassObject> pclsObj;
				pEnumerator->Next(WBEM_INFINITE, 1, pclsObj.GetAddressOf(), &uReturn);
				if (!uReturn)
					goto ver_determ_broken;

				VARIANT vtProp;
				VariantInit(&vtProp);
				// Get the value of the Name property
				pclsObj->Get(L"Name", 0, &vtProp, 0, 0);

				auto i = WideCharToMultiByte(CP_ACP, 0, vtProp.bstrVal, -1, NULL, NULL, NULL, NULL);
				if (i > 0)
				{
					ret.resize(i + 1);
					WideCharToMultiByte(CP_ACP, 0, vtProp.bstrVal, (int)ret.size(), const_cast<char*>(ret.data()), (int)ret.size(), NULL, NULL);
					ret[i] = '\0';

					auto it = ret.find_first_of('|');
					if (it != std::string::npos)
						ret.erase(it, -1);
				}

				VariantClear(&vtProp);
			}

		ver_determ_broken:
			return ret;
		}

		std::string __stdcall version_name_by_kernel_info(kernel_info_t* kernel_info)
		{
			if (!kernel_info)
			ver_determ_broken:
				return OS_VER_UNKNOWN;

			bool suc = false;
			char buffer[128];
			ZeroMemory(buffer, _ARRAYSIZE(buffer));

			strcat_s(buffer, "Microsoft Windows ");
			
			if ((kernel_info->major == 5) && (kernel_info->minor == 0)) { strcat(buffer, "2000"); suc = true; }
			else if ((kernel_info->major == 5) && (kernel_info->minor == 1)) { strcat(buffer, "XP"); suc = true; }
			else if ((kernel_info->major == 5) && (kernel_info->minor == 2)) { strcat(buffer, "XP x64"); suc = true; }
			else if ((kernel_info->major == 6) && (kernel_info->minor == 0)) { strcat(buffer, kernel_info->work ? "Vista" : "Server 2008"); suc = true; }
			else if ((kernel_info->major == 6) && (kernel_info->minor == 1)) { strcat(buffer, kernel_info->work ? "7" : "Server 2008 R2"); suc = true; }
			else if ((kernel_info->major == 6) && (kernel_info->minor == 2)) { strcat(buffer, kernel_info->work ? "8" : "Server 2012"); suc = true; }
			else if ((kernel_info->major == 6) && (kernel_info->minor == 3)) { strcat(buffer, kernel_info->work ? "8.1" : "Server 2012 R2"); suc = true; }
			else if ((kernel_info->major == 10) && (kernel_info->minor >= 0))
			{
				if (kernel_info->build <= 22000)
					strcat(buffer, "10");
				else
					strcat(buffer, "11");

				if (!kernel_info->work)
					strcat(buffer, " Server");

				suc = true;
			}

			if (!suc) 
				goto ver_determ_broken;

			return buffer;
		}

		static void __stdcall get_os_spec(uint32_t* major, uint32_t* minor, uint32_t* build)
		{
			if (!major || !minor || !build)
				return;

			std::string str_ver;

			wchar_t cmd_exe[256];
			GetEnvironmentVariableW(L"COMSPEC", cmd_exe, 256);

			size_t				it, it_l;
			DWORD               read;
			SECURITY_ATTRIBUTES sa;
			STARTUPINFOW        si;
			PROCESS_INFORMATION pi;
			HANDLE              hReadOut = NULL;   // pipe handles for redirecting STDOUT
			HANDLE              hWriteOut = NULL;
			char                buf[255];

			std::wstring scmd = cmd_exe;
			scmd += L" /c ver";

			// Prepare security attributes for pipes.
			// Note - inheritance flag is ON!
			memset(&sa, 0, sizeof(sa));
			sa.nLength = sizeof(sa);
			sa.bInheritHandle = TRUE;
			sa.lpSecurityDescriptor = NULL;

			// Create pipe for output redirection
			if (!CreatePipe(&hReadOut, &hWriteOut, &sa, 0))
				goto invalid_error;

			// Prepare child process startup data
			// Force child process to use hWriteOut as stdout
			memset(&si, 0, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			si.wShowWindow = SW_HIDE;
			si.hStdOutput = hWriteOut;
			si.hStdError = hWriteOut;

			// Spawn child process.
			if (!CreateProcessW(NULL, const_cast<wchar_t*>(scmd.data()), NULL, NULL, TRUE,
				0, NULL, NULL, &si, &pi))
			{
				CloseHandle(hReadOut);
				CloseHandle(hWriteOut);

				goto invalid_error;
			}

			// Wait until child process exits.
			WaitForSingleObject(pi.hProcess, INFINITE);
			// Close pipe ends, which i're not going to use
			CloseHandle(hWriteOut);  // child process will write here

			// Read process output.
			while (TRUE)
			{
				if (!ReadFile(hReadOut, buf, sizeof(buf) - 1, &read, NULL))
					// It might be "normal" error. Spawned app just closed the
					// stdout, and in this case ReadFile returned FALSE.
					break;

				// Test for the "normal" end of the output
				if (0 == read)
					// Nothing to read, child processs has closed the output
					break;

				// Write all data from redirected stdout to our own stdout.
				buf[read] = 0;
				str_ver += buf;
			}

			//  Close all remaining handles
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			CloseHandle(hReadOut);

			it = str_ver.find_first_of('[');
			if (it == std::string::npos)
				goto invalid_error;

			it_l = str_ver.find_first_of(' ', it + 1);
			if (it_l == std::string::npos)
				goto invalid_error;

			if (sscanf(str_ver.c_str() + it_l + 1, "%u.%u.%u", major, minor, build) != 3)
				goto invalid_error;

			return;
		invalid_error:

			*major = 0;
			*minor = 0;
			*build = 0;
		}

		static void __stdcall get_os_rtl(uint32_t* major, uint32_t* minor, uint32_t* build, bool* work)
		{
			if (!major || !minor || !build || !work)
				return;

			LONG(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW) = nullptr;
			OSVERSIONINFOEXW osInfo = { 0 };

			auto dll = GetModuleHandleA("ntdll");
			if (!dll)
				goto ver_win_home_maybe;

			*(FARPROC*)&RtlGetVersion = GetProcAddress(dll, "RtlGetVersion");
			if (RtlGetVersion)
			{
				osInfo.dwOSVersionInfoSize = sizeof(osInfo);
				RtlGetVersion(&osInfo);

				*work = osInfo.wProductType == VER_NT_WORKSTATION;
				*major = osInfo.dwMajorVersion;
				*minor = osInfo.dwMinorVersion;
				*build = osInfo.dwBuildNumber;

				if ((osInfo.dwMajorVersion == 6) && (osInfo.dwMinorVersion == 2))
					// There is a problem with stripped-down versions of OS, the function returns that it is still Win 8.
					goto ver_win_home_maybe;

				return;
			}

		ver_win_home_maybe:
			*work = true;
			get_os_spec(major, minor, build);
		}

		static void __stdcall get_os_kernel32(uint32_t* major, uint32_t* minor, uint32_t* build)
		{
			if (!major || !minor || !build)
				return;

			std::string path;
			VS_FIXEDFILEINFO* file_version = NULL;
			unsigned int file_version_len;

			path.resize(GetSystemDirectoryA(NULL, 0) - 1);
			GetSystemDirectoryA(&path[0], static_cast<UINT>(path.size() + 1));
			path += "\\kernel32.dll";

			const auto ver_info_len = GetFileVersionInfoSizeA(path.c_str(), NULL);
			auto ver_info = std::make_unique<std::uint8_t[]>(ver_info_len);
			if (!GetFileVersionInfoA(path.c_str(), 0, ver_info_len, ver_info.get()))
				goto ver_win_home_maybe;

			if (!VerQueryValueA(ver_info.get(), "", reinterpret_cast<void**>(&file_version), &file_version_len))
				goto ver_win_home_maybe;

			*major = HIWORD(file_version->dwProductVersionMS);
			*minor = LOWORD(file_version->dwProductVersionMS);
			*build = HIWORD(file_version->dwProductVersionLS);

			return;
		ver_win_home_maybe:
			get_os_spec(major, minor, build);
		}

		kernel_info_t __stdcall kernel_info(determ_osver_method method)
		{
			kernel_info_t kernel_info;
			ZeroMemory(&kernel_info, sizeof(kernel_info));
			kernel_info.size = sizeof(kernel_info);
			kernel_info.work = true;

			switch (method)
			{
			case iw::system::rtl_determ_osver_method:
				get_os_rtl(&kernel_info.major, &kernel_info.minor, &kernel_info.build, &kernel_info.work);
				break;
			case iw::system::kernel32_determ_osver_method:
				get_os_kernel32(&kernel_info.major, &kernel_info.minor, &kernel_info.build);
				break;
			default:
				get_os_spec(&kernel_info.major, &kernel_info.minor, &kernel_info.build);
				break;
			}

			return kernel_info;
		}
		
		std::string __stdcall os_info()
		{
			char buffer[256];
			kernel_info_t info = kernel_info();
			std::string s = version_name();
			sprintf_s(buffer, "%s v%u.%u.%u", s.c_str(), info.major, info.minor, info.build);
			return buffer;
		}

		memory_info_t __stdcall memory_info()
		{
			memory_info_t memory_info;
			ZeroMemory(&memory_info, sizeof(memory_info));

			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (GlobalMemoryStatusEx(&statex))
			{
				memory_info.physical_total = (float)((double)statex.ullTotalPhys / 1073741824.0);
				memory_info.physical_free = (float)((double)statex.ullAvailPhys / 1073741824.0);
				memory_info.shared_total = (float)((double)statex.ullTotalPageFile / 1073741824.0);
				memory_info.shared_free = (float)((double)statex.ullAvailPageFile / 1073741824.0);
			}

			return memory_info;
		}
	}

	namespace cpu
	{
		cpu_info_t __stdcall cpu_info()
		{
			cpu_info_t info;
			ZeroMemory(&info, sizeof(info));
			info.size = sizeof(info);

			// Calling __cpuid with 0x0 as the function_id argument
			// gets the number of the highest valid function ID.
			int32_t cpui[4];
			__cpuid(cpui, 0);
			info.ids = cpui[0];

			__cpuidex(info.data[0], 0, 0);
			__cpuidex(info.data[1], 1, 0);
			if (info.ids >= 7)
				__cpuidex(info.data[2], 7, 0);

			// Capture vendor string
			*reinterpret_cast<int*>(info.vendor_text) = info.data[0][1];
			*reinterpret_cast<int*>(info.vendor_text + 4) = info.data[0][3];
			*reinterpret_cast<int*>(info.vendor_text + 8) = info.data[0][2];

			if (!strcmp(info.vendor_text, "GenuineIntel"))
				info.vendor = iw::cpu::cpu_intel;
			else if (!strcmp(info.vendor_text, "AuthenticAMD"))
				info.vendor = iw::cpu::cpu_amd;

			// load bitset with flags for function 0x00000001
			if (info.ids >= 1)
			{
				info.ecx_1 = info.data[1][2];
				info.edx_1 = info.data[1][3];
			}

			// load bitset with flags for function 0x00000007
			if (info.ids >= 7)
			{
				info.ebx_7 = info.data[2][1];
				info.ecx_7 = info.data[2][2];
			}

			// Calling __cpuid with 0x80000000 as the function_id argument
			// gets the number of the highest valid extended ID.
			__cpuid(cpui, 0x80000000);
			info.idsex = cpui[0];

			// load bitset with flags for function 0x80000001
			if (info.idsex >= 0x80000001)
			{
				__cpuidex(info.dataex[0], 0x80000001, 0);

				info.ecx_81 = info.dataex[0][2];
				info.edx_81 = info.dataex[0][3];
			}

			// Interpret CPU brand string if reported
			if (info.idsex >= 0x80000004)
			{
				__cpuidex(info.dataex[1], 0x80000002, 0);
				__cpuidex(info.dataex[2], 0x80000003, 0);
				__cpuidex(info.dataex[3], 0x80000004, 0);

				memcpy(info.brand, info.dataex[1], sizeof(cpui));
				memcpy(info.brand + 16, info.dataex[2], sizeof(cpui));
				memcpy(info.brand + 32, info.dataex[3], sizeof(cpui));
			}

			typedef struct _PROCESSOR_POWER_INFORMATION {
				ULONG Number;
				ULONG MaxMhz;
				ULONG CurrentMhz;
				ULONG MhzLimit;
				ULONG MaxIdleState;
				ULONG CurrentIdleState;
			} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

			info.number_of_procs = (info.data[1][1] >> 16) & 0xff;
			auto ppi = std::make_unique<PROCESSOR_POWER_INFORMATION[]>(info.number_of_procs);
	
			ZeroMemory(ppi.get(), _ARRAYSIZE(ppi));

			if (!CallNtPowerInformation(POWER_INFORMATION_LEVEL::ProcessorInformation, NULL, 0, (PVOID)ppi.get(),
				sizeof(PROCESSOR_POWER_INFORMATION) * info.number_of_procs))
				info.mhz = (float)ppi.get()[0].MaxMhz / 1000.0f;

			return info;
		}
	}

	namespace graphics
	{
		graphics_info_t __stdcall graphics_info()
		{
			graphics_info_t info;
			ZeroMemory(&info, sizeof(info));

			DISPLAY_DEVICEA device;
			ZeroMemory(&device, sizeof(DISPLAY_DEVICEA));
			device.cb = sizeof(DISPLAY_DEVICEA);

			uint32_t uIndex = 0;
			bool suc = EnumDisplayDevicesA(NULL, uIndex, &device, 0);
			while (suc)
			{
				auto display = info.displays + info.display_num;

				if (!((device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) ||
					(device.StateFlags & DISPLAY_DEVICE_REMOVABLE) ||
					(info.display_num >= _ARRAYSIZE(info.displays)))
					goto continue_s;
	
				info.display_num++;

				strcpy_s(display->name, device.DeviceName);
				strcpy_s(display->videocard, device.DeviceString);
				strcpy_s(display->id, device.DeviceID);
				strcpy_s(display->key, device.DeviceKey);

				display->primary = (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) == DISPLAY_DEVICE_PRIMARY_DEVICE;
				
			continue_s:
				suc = EnumDisplayDevicesA(NULL, ++uIndex, &device, 0);
			}

			Microsoft::WRL::ComPtr<IDXGIFactory> Factory;
			if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)Factory.GetAddressOf())))
			{
				for (UINT nIndex = 0; ; nIndex++)
				{
					if (info.videocard_num >= _ARRAYSIZE(info.videocards))
						break;

					Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter;
					if (SUCCEEDED(Factory->EnumAdapters(nIndex++, Adapter.GetAddressOf())))
					{
						DXGI_ADAPTER_DESC adInfo;
						ZeroMemory(&adInfo, sizeof(adInfo));
						if (SUCCEEDED(Adapter->GetDesc(&adInfo)))
						{
							auto videocard = info.videocards + info.videocard_num;

							strcpy_s(videocard->name, iw::util::to_ansi(adInfo.Description).c_str());
							videocard->vendor_id = adInfo.VendorId;
							videocard->device_id = adInfo.DeviceId;
							videocard->revision = adInfo.Revision;
							videocard->luid = *(uint64_t*)&adInfo.AdapterLuid;
							videocard->memory = (float)((double)adInfo.DedicatedVideoMemory / 1073741824.0);

							info.videocard_num++;
						}
					}
					else break;
				}
			}

			return info;
		}

		int16_t __stdcall display_settings(graphics_info_t* graphics_info, uint8_t index, display_setting_t* settings, uint16_t count)
		{
			if (!graphics_info || (graphics_info->display_num >= index))
				return -1;

			int16_t ret = 0;
			DEVMODEA dm;
			ZeroMemory(&dm, sizeof(DEVMODEA));
			dm.dmSize = sizeof(DEVMODEA);

			for (uint32_t uIndex = 0; EnumDisplaySettingsA(graphics_info->displays[index].name, uIndex++, &dm);)
			{
				if ((dm.dmBitsPerPel != 32) || (dm.dmDisplayFrequency < 60) ||
					((dm.dmPelsWidth < 1280) && (dm.dmPelsHeight < 1024)))
					continue;

				if (settings && (count > (uint16_t)ret))
				{
					auto setting = settings + ret;

					setting->devid = index;
					setting->width = (uint16_t)dm.dmPelsWidth;
					setting->height = (uint16_t)dm.dmPelsHeight;
					setting->bpp = (uint8_t)dm.dmBitsPerPel;
					setting->frequency = (uint16_t)dm.dmDisplayFrequency;

					ret++;
				}
				else if (!settings)
					ret++;
			}

			return ret;
		}

		bool __stdcall display_setting_apply(graphics_info_t* graphics_info, uint8_t index, display_setting_t* setting)
		{
			if (!graphics_info || (graphics_info->display_num >= index) || !setting || (setting->devid != index))
				return false;

			DEVMODEA dm;
			ZeroMemory(&dm, sizeof(DEVMODEA));
			dm.dmSize = sizeof(DEVMODEA);
			dm.dmBitsPerPel = setting->bpp;
			dm.dmDisplayFrequency = setting->frequency;
			dm.dmPelsWidth = setting->width;
			dm.dmPelsHeight = setting->height;
			dm.dmFields = DM_DISPLAYFREQUENCY | DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			auto hr = ChangeDisplaySettingsExA(graphics_info->displays[index].name, &dm, NULL, CDS_TEST, NULL);
			if (hr == DISP_CHANGE_SUCCESSFUL)
			{
				bool Result = ChangeDisplaySettingsExA(graphics_info->displays[index].name, &dm, NULL,
					CDS_FULLSCREEN, NULL) == DISP_CHANGE_SUCCESSFUL;
				return Result;
			}

			return false;
		}
	}
}