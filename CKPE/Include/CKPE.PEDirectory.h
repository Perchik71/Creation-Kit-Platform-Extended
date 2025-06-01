// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API PEDirectory
	{
	public:
		enum Name : std::uint32_t
		{
			e_export = 0,		// Export Directory
			e_import,			// Import Directory
			e_resource,			// Resource Directory
			e_exception,		// Exception Directory
			e_security,			// Security Directory
			e_base_reloc,		// Base Relocation Table
			e_debug,			// Debug Directory
			e_architecture,		// Architecture Specific Data
			e_globalptr,		// RVA of GP
			e_tls,				// TLS Directory
			e_load_config,		// Load Configuration Directory
			e_bound_import,		// Bound Import Directory in headers
			e_iat,				// Import Address Table
			e_delay_import,		// Delay Load Import Descriptors
			e_com_descriptor,	// COM Runtime descriptor
			total
		};

		constexpr PEDirectory() noexcept = default;
		constexpr PEDirectory(std::uintptr_t _proxyBase, std::uintptr_t _address, std::uintptr_t _size) noexcept :
			_proxyBase(_proxyBase), _address(_address), _size((std::uint32_t)_size)
		{}

		[[nodiscard]] inline constexpr std::uintptr_t GetAddress() const noexcept(true) { return _address; }
		[[nodiscard]] inline constexpr std::uint32_t GetOffset() const noexcept(true) { return (std::uint32_t)(GetAddress() - _proxyBase); }
		[[nodiscard]] inline constexpr std::uint32_t GetSize() const noexcept(true) { return _size; }
		[[nodiscard]] inline void* GetPointer() const noexcept(true) { return reinterpret_cast<void*>(GetAddress()); }
		template <class T> [[nodiscard]] inline T* GetPointer() const noexcept(true) { return static_cast<T*>(GetPointer()); }
	private:
		std::uintptr_t _proxyBase{ 0 };
		std::uintptr_t _address{ 0 };
		std::uint32_t _size{ 0 };
	};
}