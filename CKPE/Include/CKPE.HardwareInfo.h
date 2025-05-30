// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <cmath>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API HardwareInfo
	{
		constexpr HardwareInfo() noexcept(true) = default;
		HardwareInfo(const HardwareInfo&) = delete;
		HardwareInfo& operator=(const HardwareInfo&) = delete;
	public:
		struct CKPE_API CPU
		{
			[[nodiscard]] static std::string GetProduct() noexcept(true);
			[[nodiscard]] static std::string GetBrand() noexcept(true);

			[[nodiscard]] static std::uint32_t GetTotalCores() noexcept(true);
			[[nodiscard]] static std::uint32_t GetMHz() noexcept(true);

			[[nodiscard]] static bool IsAMD() noexcept(true);
			[[nodiscard]] static bool IsIntel() noexcept(true);

			[[nodiscard]] static bool HasSupportHyper() noexcept(true);
			[[nodiscard]] static bool HasSupportSSE3() noexcept(true);
			[[nodiscard]] static bool HasSupportPCLMULQDQ() noexcept(true);
			[[nodiscard]] static bool HasSupportMONITOR() noexcept(true);
			[[nodiscard]] static bool HasSupportSSSE3() noexcept(true);
			[[nodiscard]] static bool HasSupportFMA() noexcept(true);
			[[nodiscard]] static bool HasSupportCMPXCHG16B() noexcept(true);
			[[nodiscard]] static bool HasSupportSSE41() noexcept(true);
			[[nodiscard]] static bool HasSupportSSE42() noexcept(true);
			[[nodiscard]] static bool HasSupportMOVBE() noexcept(true);
			[[nodiscard]] static bool HasSupportPOPCNT() noexcept(true);
			[[nodiscard]] static bool HasSupportAES() noexcept(true);
			[[nodiscard]] static bool HasSupportXSAVE() noexcept(true);
			[[nodiscard]] static bool HasSupportOSXSAVE() noexcept(true);
			[[nodiscard]] static bool HasSupportAVX() noexcept(true);
			[[nodiscard]] static bool HasSupportF16C() noexcept(true);
			[[nodiscard]] static bool HasSupportRDRAND() noexcept(true);
			[[nodiscard]] static bool HasSupportMSR() noexcept(true);
			[[nodiscard]] static bool HasSupportCX8() noexcept(true);
			[[nodiscard]] static bool HasSupportSEP() noexcept(true);
			[[nodiscard]] static bool HasSupportCMOV() noexcept(true);
			[[nodiscard]] static bool HasSupportCLFSH() noexcept(true);
			[[nodiscard]] static bool HasSupportMMX() noexcept(true);
			[[nodiscard]] static bool HasSupportFXSR() noexcept(true);
			[[nodiscard]] static bool HasSupportSSE() noexcept(true);
			[[nodiscard]] static bool HasSupportSSE2() noexcept(true);
			[[nodiscard]] static bool HasSupportFSGSBASE() noexcept(true);
			[[nodiscard]] static bool HasSupportBMI1() noexcept(true);
			[[nodiscard]] static bool HasSupportHLE() noexcept(true);
			[[nodiscard]] static bool HasSupportAVX2() noexcept(true);
			[[nodiscard]] static bool HasSupportBMI2() noexcept(true);
			[[nodiscard]] static bool HasSupportERMS() noexcept(true);
			[[nodiscard]] static bool HasSupportINVPCID() noexcept(true);
			[[nodiscard]] static bool HasSupportRTM() noexcept(true);
			[[nodiscard]] static bool HasSupportAVX512F() noexcept(true);
			[[nodiscard]] static bool HasSupportRDSEED() noexcept(true);
			[[nodiscard]] static bool HasSupportADX() noexcept(true);
			[[nodiscard]] static bool HasSupportAVX512PF() noexcept(true);
			[[nodiscard]] static bool HasSupportAVX512ER() noexcept(true);
			[[nodiscard]] static bool HasSupportAVX512CD() noexcept(true);
			[[nodiscard]] static bool HasSupportSHA() noexcept(true);
			[[nodiscard]] static bool HasSupportPREFETCHWT1() noexcept(true);
			[[nodiscard]] static bool HasSupportLAHF() noexcept(true);
			[[nodiscard]] static bool HasSupportLZCNT() noexcept(true);
			[[nodiscard]] static bool HasSupportABM() noexcept(true);
			[[nodiscard]] static bool HasSupportSSE4A() noexcept(true);
			[[nodiscard]] static bool HasSupportXOP() noexcept(true);
			[[nodiscard]] static bool HasSupportTBM() noexcept(true);
			[[nodiscard]] static bool HasSupportSYSCALL() noexcept(true);
			[[nodiscard]] static bool HasSupportMMXEXT() noexcept(true);
			[[nodiscard]] static bool HasSupportRDTSCP() noexcept(true);
			[[nodiscard]] static bool HasSupport3DNOWEXT() noexcept(true);
			[[nodiscard]] static bool HasSupport3DNOW() noexcept(true);
		};

		struct CKPE_API GPU
		{
			[[nodiscard]] static std::uint32_t GetCount() noexcept(true);
			[[nodiscard]] static std::string GetName(std::uint32_t id) noexcept(true);
			[[nodiscard]] static std::uint32_t GetVendorID(std::uint32_t id) noexcept(true);
			[[nodiscard]] static std::uint32_t GetDeviceID(std::uint32_t id) noexcept(true);
			[[nodiscard]] static std::uint32_t GetRevision(std::uint32_t id) noexcept(true);
			[[nodiscard]] static std::uint64_t GetLUID(std::uint32_t id) noexcept(true);
			[[nodiscard]] static float GetMemorySize(std::uint32_t id) noexcept(true);
			[[nodiscard]] inline static std::uint32_t GetMemorySizeByUint(std::uint32_t id) noexcept(true)
			{ return (std::uint32_t)std::round(GetMemorySize(id) + 0.4f); };
		};

		struct CKPE_API OS
		{
			struct CKPE_API MemoryInfo
			{
				float Total;
				float Free;

				[[nodiscard]] inline float GetUsed() const noexcept(true) 
				{ return Total - Free; }
				[[nodiscard]] inline std::uint32_t GetTotalByUint() const noexcept(true)
				{ return (std::uint32_t)std::round(Total + 0.4f); }
				[[nodiscard]] inline std::uint32_t GetFreeByUint() const noexcept(true)
				{ return (std::uint32_t)std::round(Free + 0.4f); }
				[[nodiscard]] inline std::uint32_t GetUsedByUint() const noexcept(true)
				{ return (std::uint32_t)std::round(Total - Free); }
			};

			[[nodiscard]] static std::uint64_t GetVersion() noexcept(true);
			[[nodiscard]] static std::wstring GetVersionByString() noexcept(true);
			[[nodiscard]] static std::wstring GetVersionByStringEx() noexcept(true);
			[[nodiscard]] static MemoryInfo GetPhysicalMemory() noexcept(true);
			[[nodiscard]] static MemoryInfo GetSharedMemory() noexcept(true);
		};
	};
}