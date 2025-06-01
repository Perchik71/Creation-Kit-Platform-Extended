// Copyri// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <iw.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Module.h>
#include <CKPE.StringUtils.h>

namespace CKPE
{
	iw::cpu::cpu_info_t _cpu_info{ 0 };
	iw::graphics::graphics_info_t _graphics_info{ 0 };
	iw::system::kernel_info_t _kernel_info{ 0 };
	iw::system::memory_info_t _memory_info{ 0 };

	inline static void CPUUpdate() noexcept(true)
	{
		if (!_cpu_info.size)
			_cpu_info = iw::cpu::cpu_info();
	}

	inline static void GPUUpdate() noexcept(true)
	{
		if (!_graphics_info.videocard_num)
			_graphics_info = iw::graphics::graphics_info();
	}
	
	inline static void OSUpdate() noexcept(true)
	{
		if (!_kernel_info.size)
		{
			_kernel_info = iw::system::kernel_info();
			_memory_info = iw::system::memory_info();
		}
	}

	std::string HardwareInfo::CPU::GetProduct() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::cpu_product(&_cpu_info);
	}

	std::string HardwareInfo::CPU::GetBrand() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::cpu_brand(&_cpu_info);
	}

	std::uint32_t HardwareInfo::CPU::GetTotalCores() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::number_of_procs(&_cpu_info);
	}

	std::uint32_t HardwareInfo::CPU::GetMHz() noexcept(true)
	{
		CPUUpdate();
		return (std::uint32_t)std::round(iw::cpu::mhz(&_cpu_info) * 1000);
	}

	bool HardwareInfo::CPU::IsAMD() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_amd(&_cpu_info);
	}

	bool HardwareInfo::CPU::IsIntel() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_intel(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportHyper() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_hyper(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSSE3() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SSE3(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportPCLMULQDQ() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_PCLMULQDQ(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportMONITOR() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_MONITOR(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSSSE3() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SSSE3(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportFMA() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_FMA(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportCMPXCHG16B() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_CMPXCHG16B(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSSE41() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SSE41(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSSE42() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SSE42(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportMOVBE() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_MOVBE(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportPOPCNT() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_POPCNT(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportAES() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_AES(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportXSAVE() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_XSAVE(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportOSXSAVE() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_OSXSAVE(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportAVX() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_AVX(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportF16C() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_F16C(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportRDRAND() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_RDRAND(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportMSR() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_MSR(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportCX8() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_CX8(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSEP() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SEP(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportCMOV() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_CMOV(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportCLFSH() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_CLFSH(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportMMX() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_MMX(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportFXSR() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_FXSR(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSSE() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SSE(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSSE2() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SSE2(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportFSGSBASE() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_FSGSBASE(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportBMI1() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_BMI1(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportHLE() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_HLE(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportAVX2() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_AVX2(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportBMI2() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_BMI2(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportERMS() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_ERMS(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportINVPCID() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_INVPCID(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportRTM() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_RTM(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportAVX512F() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_AVX512F(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportRDSEED() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_RDSEED(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportADX() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_ADX(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportAVX512PF() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_AVX512PF(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportAVX512ER() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_AVX512ER(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportAVX512CD() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_AVX512CD(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSHA() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SHA(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportPREFETCHWT1() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_PREFETCHWT1(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportLAHF() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_LAHF(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportLZCNT() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_LZCNT(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportABM() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_ABM(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSSE4A() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SSE4a(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportXOP() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_XOP(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportTBM() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_TBM(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportSYSCALL() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_SYSCALL(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportMMXEXT() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_MMXEXT(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupportRDTSCP() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_RDTSCP(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupport3DNOWEXT() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_3DNOWEXT(&_cpu_info);
	}

	bool HardwareInfo::CPU::HasSupport3DNOW() noexcept(true)
	{
		CPUUpdate();
		return iw::cpu::is_support_3DNOW(&_cpu_info);
	}

	std::uint32_t HardwareInfo::GPU::GetCount() noexcept(true)
	{
		GPUUpdate();
		return _graphics_info.videocard_num;
	}

	std::string HardwareInfo::GPU::GetName(std::uint32_t id) noexcept(true)
	{
		GPUUpdate();
		return (_graphics_info.videocard_num > id) ? _graphics_info.videocards[id].name : "";
	}

	std::uint32_t HardwareInfo::GPU::GetVendorID(std::uint32_t id) noexcept(true)
	{
		GPUUpdate();
		return (_graphics_info.videocard_num > id) ? _graphics_info.videocards[id].vendor_id : 0;
	}

	std::uint32_t HardwareInfo::GPU::GetDeviceID(std::uint32_t id) noexcept(true)
	{
		GPUUpdate();
		return (_graphics_info.videocard_num > id) ? _graphics_info.videocards[id].device_id : 0;
	}

	std::uint32_t HardwareInfo::GPU::GetRevision(std::uint32_t id) noexcept(true)
	{
		GPUUpdate();
		return (_graphics_info.videocard_num > id) ? _graphics_info.videocards[id].revision : 0;
	}

	std::uint64_t HardwareInfo::GPU::GetLUID(std::uint32_t id) noexcept(true)
	{
		GPUUpdate();
		return (_graphics_info.videocard_num > id) ? _graphics_info.videocards[id].luid : 0;
	}

	float HardwareInfo::GPU::GetMemorySize(std::uint32_t id) noexcept(true)
	{
		GPUUpdate();
		return (_graphics_info.videocard_num > id) ? _graphics_info.videocards[id].memory : 0.0f;
	}

	std::uint64_t HardwareInfo::OS::GetVersion() noexcept(true)
	{
		OSUpdate();
		return MAKE_EXE_VERSION_EX(_kernel_info.major, _kernel_info.minor, _kernel_info.build, 0);
	}

	std::wstring HardwareInfo::OS::GetVersionByString() noexcept(true)
	{
		OSUpdate();
		return StringUtils::WinCPToUtf16(iw::system::version_name_by_kernel_info(&_kernel_info));
	}

	std::wstring HardwareInfo::OS::GetVersionByStringEx() noexcept(true)
	{
		return StringUtils::WinCPToUtf16(iw::system::os_info());
	}

	HardwareInfo::OS::MemoryInfo HardwareInfo::OS::GetPhysicalMemory() noexcept(true)
	{
		OSUpdate();
		return { _memory_info.physical_total, _memory_info.physical_free };
	}

	HardwareInfo::OS::MemoryInfo HardwareInfo::OS::GetSharedMemory() noexcept(true)
	{
		OSUpdate();
		return { _memory_info.shared_total, _memory_info.shared_free };
	}
}