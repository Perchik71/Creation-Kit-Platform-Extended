// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>
#include <CKPE.Stream.h>

namespace CKPE
{
#ifndef CKPE_GRAPHICS
	typedef void* THandle;
#endif // !CKPE_GRAPHICS

	class CKPE_API GDIPlus
	{
		GDIPlus(const GDIPlus&) = delete;
		GDIPlus& operator=(const GDIPlus&) = delete;
	public:
		GDIPlus() noexcept(true);
		virtual ~GDIPlus() noexcept(true);

		static THandle LoadImageFromFile(const std::wstring& fname) noexcept(true);
		static THandle LoadImageFromStream(Stream& stream) noexcept(true);
		static THandle LoadImageFromResource(THandle hInst, std::uint32_t dwResId,
			const std::wstring& ResType) noexcept(true);

		[[nodiscard]] static GDIPlus* GetSingleton() noexcept(true);
	};
}