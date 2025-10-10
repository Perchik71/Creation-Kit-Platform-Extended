// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.SafeWrite.h>

namespace CKPE
{
	void SafeWrite::Write(std::uintptr_t address, const std::uint8_t* data, std::size_t size) noexcept(true)
	{
		if (!address) return;

		DWORD d = 0;
		VirtualProtect((LPVOID)address, (SIZE_T)size, PAGE_EXECUTE_READWRITE, &d);
		memcpy((void*)address, (const void*)data, size);
		VirtualProtect((LPVOID)address, (SIZE_T)size, d, &d);
		FlushInstructionCache(GetCurrentProcess(), (LPVOID)address, (SIZE_T)size);
	}

	void SafeWrite::Write(std::uintptr_t address, std::initializer_list<std::uint8_t> data) noexcept(true)
	{
		Write(address, data.begin(), data.size());
	}

	void SafeWrite::WriteSet(std::uintptr_t address, std::uint8_t value, std::size_t size) noexcept(true)
	{
		if (!address) return;

		DWORD d = 0;
		VirtualProtect((LPVOID)address, (SIZE_T)size, PAGE_EXECUTE_READWRITE, &d);
		memset((void*)address, value, size);
		VirtualProtect((LPVOID)address, (SIZE_T)size, d, &d);
		FlushInstructionCache(GetCurrentProcess(), (LPVOID)address, (SIZE_T)size);
	}

	void SafeWrite::WriteNop(std::uintptr_t address, std::size_t size) noexcept(true)
	{
		WriteSet(address, 0x90, size);
	}

	void SafeWrite::WriteStringRef(std::uintptr_t address, const char* new_str) noexcept(true)
	{
		if (!address || !new_str) return;
		Write(address, (std::uint8_t*)&new_str, (std::uint32_t)sizeof(new_str));
	}

	void SafeWrite::WriteJump(std::uintptr_t rav_from, std::uintptr_t rav_to) noexcept(true)
	{
		if (!rav_from || !rav_to) return;

		Write(rav_from, { 0xE9 });
		auto RelOff = (std::uint32_t)(rav_to - (rav_from + 5));
		Write(rav_from + 1, (std::uint8_t*)&RelOff, 4);
	}

	void SafeWrite::WriteCall(std::uintptr_t rav_from, std::uintptr_t rav_to) noexcept(true)
	{
		if (!rav_from || !rav_to) return;

		Write(rav_from, { 0xE8 });
		auto RelOff = (std::uint32_t)(rav_to - (rav_from + 5));
		Write(rav_from + 1, (std::uint8_t*)&RelOff, 4);
	}

	void SafeWrite::WriteMovFromRax(std::uintptr_t rav_from, std::uintptr_t rav_to) noexcept(true)
	{
		if (!rav_from || !rav_to) return;

		Write(rav_from, { 0x48, 0x89, 0x05 });
		auto RelOff = (std::uint32_t)(rav_to - (rav_from + 7));
		Write(rav_from + 3, (uint8_t*)&RelOff, 4);
	}

	ScopeSafeWrite::ScopeSafeWrite(std::uintptr_t target, std::uintptr_t size) noexcept(true) :
		_target(target), _size(size)
	{
		_init = (bool)VirtualProtect((LPVOID)target, (SIZE_T)size, PAGE_EXECUTE_READWRITE, (PDWORD)&_of);
	}

	ScopeSafeWrite::~ScopeSafeWrite() noexcept(true)
	{
		if (_init)
		{
			VirtualProtect((LPVOID)_target, (SIZE_T)_size, _of, (PDWORD)&_of);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)_target, (SIZE_T)_size);
			_init = false;
		}
	}

	bool ScopeSafeWrite::Contain(std::uintptr_t address, std::size_t size) const noexcept(true)
	{
		if (!_init) return false;
		return (_target <= address) && ((address + size) < (_target + _size));
	}

	void ScopeSafeWrite::Write(std::uintptr_t address, const std::uint8_t* data, std::size_t size) const noexcept(true)
	{
		if (Contain(address, size))
			memcpy((void*)address, (const void*)data, size);
	}

	void ScopeSafeWrite::Write(std::uintptr_t address, std::initializer_list<std::uint8_t> data) const noexcept(true)
	{
		Write(address, data.begin(), data.size());
	}

	void ScopeSafeWrite::WriteSet(std::uintptr_t address, std::uint8_t value, std::size_t size) const noexcept(true)
	{
		if (Contain(address, size))
			memset((void*)address, value, size);
	}

	void ScopeSafeWrite::WriteNop(std::uintptr_t address, std::size_t size) const noexcept(true)
	{
		WriteSet(address, 0x90, size);
	}

	void ScopeSafeWrite::WriteStringRef(std::uintptr_t address, const char* new_str) const noexcept(true)
	{
		if (Contain(address, sizeof(new_str)))
			Write(address, (std::uint8_t*)&new_str, sizeof(std::uintptr_t));
	}

	void ScopeSafeWrite::WriteJump(std::uintptr_t rav_from, std::uintptr_t rav_to) const noexcept(true)
	{
		if (Contain(rav_from, 5))
		{
			Write(rav_from, { 0xE9 });
			auto RelOff = (std::uint32_t)(rav_to - (rav_from + 5));
			Write(rav_from + 1, (std::uint8_t*)&RelOff, 4);
		}
	}

	void ScopeSafeWrite::WriteCall(std::uintptr_t rav_from, std::uintptr_t rav_to) const noexcept(true)
	{
		if (Contain(rav_from, 5))
		{
			Write(rav_from, { 0xE8 });
			auto RelOff = (std::uint32_t)(rav_to - (rav_from + 5));
			Write(rav_from + 1, (std::uint8_t*)&RelOff, 4);
		}
	}

	void ScopeSafeWrite::WriteMovFromRax(std::uintptr_t rav_from, std::uintptr_t rav_to) const noexcept(true)
	{
		if (Contain(rav_from, 7))
		{
			Write(rav_from, { 0x48, 0x89, 0x05 });
			auto RelOff = (std::uint32_t)(rav_to - (rav_from + 7));
			Write(rav_from + 3, (uint8_t*)&RelOff, 4);
		}
	}
}