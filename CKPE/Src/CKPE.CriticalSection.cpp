// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.CriticalSection.h>

namespace CKPE
{
	CriticalSection::CriticalSection() noexcept(true)
	{
		_handle = new CRITICAL_SECTION;
		if (_handle)
			InitializeCriticalSection((PCRITICAL_SECTION)_handle);
	}

	CriticalSection::~CriticalSection() noexcept(true)
	{
		if (_handle)
		{
			DeleteCriticalSection((PCRITICAL_SECTION)_handle);
			_handle = nullptr;
		}
	}

	bool CriticalSection::TryLock() const noexcept(true)
	{
		return (_handle) ? TryEnterCriticalSection((PCRITICAL_SECTION)_handle) : true;
	}

	void CriticalSection::Lock() const noexcept(true)
	{
		if (_handle)
			EnterCriticalSection((PCRITICAL_SECTION)_handle);
	}

	void CriticalSection::Unlock() const noexcept(true)
	{
		if (_handle)
			LeaveCriticalSection((PCRITICAL_SECTION)_handle);
	}

	ScopeCriticalSection::ScopeCriticalSection(const CriticalSection& section) noexcept(true) :
		_section(&section)
	{
		_section->Lock();
	}

	ScopeCriticalSection::~ScopeCriticalSection() noexcept(true)
	{
		_section->Unlock();
	}
}