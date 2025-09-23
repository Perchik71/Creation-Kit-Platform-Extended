// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API Segment
	{
	public:
		enum Name : std::uint32_t
		{
			//textbss,
			text = 0,
			interpr,
			idata,
			rdata,
			data,
			pdata,
			tls,
			total
		};

		constexpr Segment() noexcept(true) = default;
		constexpr Segment(std::uintptr_t _proxyBase, std::uintptr_t _address, std::uintptr_t _size) noexcept(true) :
			_proxyBase(_proxyBase), _address(_address), _size((std::uint32_t)_size)
		{}

		[[nodiscard]] inline constexpr std::uintptr_t GetAddress() const noexcept(true) { return _address; }
		[[nodiscard]] inline constexpr std::uintptr_t GetEndAddress() const noexcept(true) { return _address + _size; }
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