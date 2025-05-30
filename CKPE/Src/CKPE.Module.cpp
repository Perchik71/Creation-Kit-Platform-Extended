// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.Module.h>

namespace CKPE
{
	Module::Module(const void* m) noexcept(true) :
		_handle(m)
	{}

	bool Module::Is64() noexcept(true)
	{
		auto* base = (const char*)(std::uintptr_t(_handle) & ~3);
		auto* dosHeader = (const IMAGE_DOS_HEADER*)base;
		auto* ntHeader = (const IMAGE_NT_HEADERS*)(base + dosHeader->e_lfanew);
		// FileHeader is PE32/64 independent
		return ntHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;
	}

	const void* Module::GetResourceLibraryViaProcAddress(const char* exportName) noexcept(true)
	{
		auto* base = (const char*)(uintptr_t(_handle) & ~3);
		auto* dosHeader = (const IMAGE_DOS_HEADER*)base;
		auto* ntHeader = (const IMAGE_NT_HEADERS*)(base + dosHeader->e_lfanew);
		auto* exportTable = (const IMAGE_EXPORT_DIRECTORY*)(base + 
				ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

		auto* exportAddresses = (const std::uint32_t*)(base + exportTable->AddressOfFunctions);			// RVA array
		auto* exportNameOrdinals = (const std::uint16_t*)(base + exportTable->AddressOfNameOrdinals);	// index in to exportAddresses
		auto* exportNames = (const std::uint32_t*)(base + exportTable->AddressOfNames);					// RVA array

		const void* result = nullptr;

		for (std::uint32_t i = 0; i < exportTable->NumberOfNames; i++)
		{
			std::uint32_t nameRVA = exportNames[i];
			auto* name = (const char*)(base + nameRVA);

			if (!strcmp(exportName, name))
			{
				std::uint32_t addrIdx = exportNameOrdinals[i];
				if (addrIdx < exportTable->NumberOfFunctions)
				{
					std::uint32_t addrRVA = exportAddresses[addrIdx];
					result = (const void*)(base + addrRVA);

					break;
				}
			}
		}

		return result;
	}
}