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

#pragma once

#include <stdint.h>
#include <string>
#include <bitset>

namespace iw
{
	namespace system
	{
		struct kernel_info_t
		{
			uint32_t size;
			uint32_t major;
			uint32_t minor; 
			uint32_t build;
			bool work;
		};

		[[nodiscard]] std::string __stdcall version_name();
		[[nodiscard]] std::string __stdcall version_name_by_kernel_info(kernel_info_t* kernel_info);

		enum determ_osver_method : size_t
		{
			// Use cmd /c ver
			spec_determ_osver_method = 0,
			// Use Rtl.. if Win8 determ in this case use spec_determ_osver_method
			rtl_determ_osver_method = 1,
			// Get file version kernel32.dll
			kernel32_determ_osver_method = 2,
		};

		[[nodiscard]] kernel_info_t __stdcall kernel_info(determ_osver_method method = kernel32_determ_osver_method);
		// Returned full text version windows
		[[nodiscard]] std::string __stdcall os_info();

		struct memory_info_t
		{
			float physical_total;
			float physical_free;
			float shared_total;
			float shared_free;
		};

		[[nodiscard]] memory_info_t __stdcall memory_info();
	}

	namespace cpu
	{
		enum cpu_vendor : size_t
		{
			cpu_unknown = 0,
			cpu_intel = 1,
			cpu_amd = 2,
		};

		struct cpu_info_t
		{
			uint32_t size;
			int32_t ids, idsex;
			int32_t data[3][4];
			int32_t dataex[4][4];
			char vendor_text[20];
			char brand[50];
			cpu_vendor vendor;
			int32_t ecx_1;
			int32_t edx_1;
			int32_t ebx_7;
			int32_t ecx_7;
			int32_t ecx_81;
			int32_t edx_81;

			uint32_t number_of_procs;
			float mhz;
		};

		cpu_info_t __stdcall cpu_info();

		[[nodiscard]] inline static std::string __stdcall cpu_product(cpu_info_t* cpu_info) noexcept(true) { return cpu_info->vendor_text; }
		[[nodiscard]] inline static std::string __stdcall cpu_brand(cpu_info_t* cpu_info) noexcept(true) { return cpu_info->brand; }

		[[nodiscard]] inline static uint32_t __stdcall number_of_procs(cpu_info_t* cpu_info) noexcept(true) { return cpu_info->number_of_procs; }
		[[nodiscard]] inline static float __stdcall mhz(cpu_info_t* cpu_info) noexcept(true) { return cpu_info->mhz; }

		[[nodiscard]] inline static bool __stdcall is_intel(cpu_info_t* cpu_info) noexcept(true) { return cpu_info->vendor == cpu_intel; }
		[[nodiscard]] inline static bool __stdcall is_amd(cpu_info_t* cpu_info) noexcept(true) { return cpu_info->vendor == cpu_amd; }

		[[nodiscard]] inline static bool __stdcall is_support_hyper(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 28)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SSE3(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 0)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_PCLMULQDQ(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 1)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_MONITOR(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 3)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SSSE3(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 9)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_FMA(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 12)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_CMPXCHG16B(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 13)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SSE41(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 19)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SSE42(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 20)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_MOVBE(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 22)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_POPCNT(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 23)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_AES(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 25)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_XSAVE(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 26)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_OSXSAVE(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 27)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_AVX(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 28)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_F16C(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 29)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_RDRAND(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_1 & (1 << 30)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_MSR(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 5)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_CX8(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 8)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SEP(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 11)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_CMOV(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 15)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_CLFSH(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 19)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_MMX(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 23)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_FXSR(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 24)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SSE(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 25)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SSE2(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->edx_1 & (1 << 26)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_FSGSBASE(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 0)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_BMI1(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 3)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_HLE(cpu_info_t* cpu_info) noexcept(true) { return is_intel(cpu_info) && ((cpu_info->ebx_7 & (1 << 4)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_AVX2(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 5)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_BMI2(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 8)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_ERMS(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 9)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_INVPCID(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 10)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_RTM(cpu_info_t* cpu_info) noexcept(true) { return is_intel(cpu_info) && ((cpu_info->ebx_7 & (1 << 11)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_AVX512F(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 16)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_RDSEED(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 18)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_ADX(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 19)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_AVX512PF(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 26)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_AVX512ER(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 27)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_AVX512CD(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 28)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_SHA(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ebx_7 & (1 << 29)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_PREFETCHWT1(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_7 & (1 << 0)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_LAHF(cpu_info_t* cpu_info) noexcept(true) { return (cpu_info->ecx_81 & (1 << 0)) != 0; }
		[[nodiscard]] inline static bool __stdcall is_support_LZCNT(cpu_info_t* cpu_info) noexcept(true) { return is_intel(cpu_info) && ((cpu_info->ecx_81 & (1 << 5)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_ABM(cpu_info_t* cpu_info) noexcept(true) { return is_amd(cpu_info) && ((cpu_info->ecx_81 & (1 << 5)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_SSE4a(cpu_info_t* cpu_info) noexcept(true) { return is_amd(cpu_info) && ((cpu_info->ecx_81 & (1 << 6)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_XOP(cpu_info_t* cpu_info) noexcept(true) { return is_amd(cpu_info) && ((cpu_info->ecx_81 & (1 << 11)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_TBM(cpu_info_t* cpu_info) noexcept(true) { return is_amd(cpu_info) && ((cpu_info->ecx_81 & (1 << 21)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_SYSCALL(cpu_info_t* cpu_info) noexcept(true) { return is_intel(cpu_info) && ((cpu_info->edx_81 & (1 << 11)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_MMXEXT(cpu_info_t* cpu_info) noexcept(true) { return is_amd(cpu_info) && ((cpu_info->edx_81 & (1 << 22)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_RDTSCP(cpu_info_t* cpu_info) noexcept(true) { return is_intel(cpu_info) && ((cpu_info->edx_81 & (1 << 27)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_3DNOWEXT(cpu_info_t* cpu_info) noexcept(true) { return is_amd(cpu_info) && ((cpu_info->edx_81 & (1 << 30)) != 0); }
		[[nodiscard]] inline static bool __stdcall is_support_3DNOW(cpu_info_t* cpu_info) noexcept(true) { return is_amd(cpu_info) && ((cpu_info->edx_81 & (1 << 31)) != 0); }
	}

	namespace graphics
	{
		struct display_info_t
		{
			char name[20];
			char videocard[40];
			char id[60];
			char key[119];
			bool primary;
		};

		struct display_setting_t
		{
			uint8_t devid;
			uint16_t width;
			uint16_t height;
			uint8_t bpp;
			uint16_t frequency;
		};

		struct videocard_info_t
		{
			char name[40];
			uint32_t vendor_id;
			uint32_t device_id;
			uint32_t revision;
			uint64_t luid;
			float memory;
		};

		struct graphics_info_t
		{
			uint8_t display_num;
			display_info_t displays[8];
			uint8_t videocard_num;
			videocard_info_t videocards[8];
		};

		[[nodiscard]] graphics_info_t __stdcall graphics_info();
		[[nodiscard]] int16_t __stdcall display_settings(graphics_info_t* graphics_info, uint8_t index, display_setting_t* settings, uint16_t count);
		[[nodiscard]] bool __stdcall display_setting_apply(graphics_info_t* graphics_info, uint8_t index, display_setting_t* setting);
	}
}