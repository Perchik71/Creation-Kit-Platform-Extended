// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API CriticalSection
	{
		void* _handle{ nullptr };

		CriticalSection(const CriticalSection&) = delete;
		CriticalSection& operator=(const CriticalSection&) = delete;
	public:
		CriticalSection() noexcept(true);
		~CriticalSection() noexcept(true);

		bool TryLock() const noexcept(true);
		void Lock() const noexcept(true);
		void Unlock() const noexcept(true);
	};

	class CKPE_API ScopeCriticalSection
	{
		const CriticalSection* _section{ nullptr };

		ScopeCriticalSection(const ScopeCriticalSection&) = delete;
		ScopeCriticalSection& operator=(const ScopeCriticalSection&) = delete;
	public:
		ScopeCriticalSection(const CriticalSection& section) noexcept(true);
		~ScopeCriticalSection() noexcept(true);
	};
}