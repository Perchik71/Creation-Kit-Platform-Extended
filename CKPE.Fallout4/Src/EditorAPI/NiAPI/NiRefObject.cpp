// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiRefObject.h

#include <windows.h>
#include <EditorAPI/NiAPI/NiRefObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				NiRefObject::NiRefObject() noexcept(true) : m_uiRefCount(0)
				{
					InterlockedIncrement(&m_uiRefCount);
				}

				NiRefObject::~NiRefObject() noexcept(true)
				{
					InterlockedDecrement(&m_uiRefCount);
				}

				void NiRefObject::DeleteThis() noexcept(true)
				{
					if (this)
						this->~NiRefObject();
				}

				std::uint32_t NiRefObject::IncRefCount() noexcept(true)
				{
					return (std::uint32_t)InterlockedIncrement(&m_uiRefCount);
				}

				std::uint32_t NiRefObject::DecRefCount() noexcept(true)
				{
					std::uint32_t count = (std::uint32_t)InterlockedDecrement(&m_uiRefCount);

					if (count <= 0)
						DeleteThis();

					return count;
				}

				void NiRefObject::GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true)
				{
					Callback("-- NiRefObject --\n");
					Callback("This = 0x%p\n", this);
					Callback("Ref Count = %u\n", m_uiRefCount);
				}

				NiRefObject64::NiRefObject64() noexcept(true) : m_uiRefCount(0)
				{
					InterlockedIncrement64(&m_uiRefCount);
				}

				NiRefObject64::~NiRefObject64() noexcept(true)
				{
					InterlockedDecrement64(&m_uiRefCount);
				}

				void NiRefObject64::DeleteThis() noexcept(true)
				{
					if (this)
						this->~NiRefObject64();
				}

				std::uint64_t NiRefObject64::IncRefCount() noexcept(true)
				{
					return (std::uint64_t)InterlockedIncrement64(&m_uiRefCount);
				}

				std::uint64_t NiRefObject64::DecRefCount() noexcept(true)
				{
					std::uint64_t count = (std::uint64_t)InterlockedDecrement64(&m_uiRefCount);

					if (count <= 0)
						DeleteThis();

					return count;
				}

				void NiRefObject64::GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true)
				{
					Callback("-- NiRefObject --\n");
					Callback("This = 0x%p\n", this);
					Callback("Ref Count = %llu\n", m_uiRefCount);
				}
			}
		}
	}
}