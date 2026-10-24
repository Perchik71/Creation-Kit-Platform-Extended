// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiRefObject.h

#pragma once

#include <cstdint>
#include <cstddef>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				// 10
				class __declspec(align(8)) NiRefObject
				{
				protected:
					volatile long m_uiRefCount;
				public:
					NiRefObject() noexcept(true);
					virtual ~NiRefObject() noexcept(true);

					virtual void DeleteThis() noexcept(true);

					std::uint32_t IncRefCount() noexcept(true);
					std::uint32_t DecRefCount() noexcept(true);

					void GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true);
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiRefObject, m_uiRefCount) == 0x8;
					}
				};
				static_assert(sizeof(NiRefObject) == 0x10);
				static_assert(NiRefObject::VerifyLayout());

				// 10
				class __declspec(align(8)) NiRefObject64
				{
				protected:
					volatile long long m_uiRefCount;
				public:
					NiRefObject64() noexcept(true);
					virtual ~NiRefObject64() noexcept(true);

					virtual void DeleteThis() noexcept(true);

					std::uint64_t IncRefCount() noexcept(true);
					std::uint64_t DecRefCount() noexcept(true);

					void GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true);
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiRefObject64, m_uiRefCount) == 0x8;
					}
				};
				static_assert(sizeof(NiRefObject64) == 0x10);
				static_assert(NiRefObject64::VerifyLayout());
			}
		}
	}
}