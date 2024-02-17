// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "TracerManager.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		TracerManager* GlobalTracerManagerPtr = nullptr;

		TracerManager::TracerManager() : _Rec(false)
		{
			_Items.grow_by(200);
		}

		void TracerManager::Push(const char* _Name, uintptr_t _MemAddress, size_t _Size, uintptr_t _RetAddress)
		{
			if (!_MemAddress || !_Rec)
				return;

			TracerInfo info = 
			{
				.Name = _Name,
				.RetAddress = _RetAddress,
				.MemAddress = _MemAddress,
				.Size = _Size,
				.Active = true
			};

			_Items.push_back(info);
		}

		void TracerManager::Pop(uintptr_t _MemAddress)
		{
			if (!_MemAddress || !_Rec)
				return;

			for (auto It = _Items.begin(); It != _Items.end(); It++)
			{
				if (It->MemAddress == _MemAddress)
				{
					It->Active = false;
					break; 
				}
			}
		}

		void TracerManager::Dump()
		{
			auto Steam = _fsopen((Utils::GetApplicationPath() + "CreationKitPlatformExtendedTracerMemory.log").c_str(), 
				"wt+", _SH_DENYWR);

			if (Steam)
			{
				{
					size_t TotalMemAllocSize = 0;
					Utils::ScopeFileStream FileStream(Steam);

					fprintf(Steam, "Dumping objects.\n");
					for (auto It = _Items.begin(); It != _Items.end(); It++)
						if (It->Active)
						{
							fprintf(Steam, "%s -> normal block at %p, %llu bytes, where from %p\n",
								(It->Name ? It->Name : "UNKNOWN"), It->MemAddress, It->Size, It->RetAddress);
							
							fprintf(Steam, "  Data <");
							char* Data = (char*)It->MemAddress;
							for (auto i = 0; i < It->Size; i++)
								fputc((isprint(Data[i]) ? Data[i] : ' '), Steam);
							fputs("> ", Steam);
							for (auto i = 0; i < It->Size; i++)
								fprintf(Steam, "%02X ", (unsigned char)(Data[i]));
							fputc('\n', Steam);

							TotalMemAllocSize += It->Size;
						}
					fprintf(Steam, "Total memory allocated size %llu.\n", TotalMemAllocSize);
					fprintf(Steam, "Object dump complete.\n");
				}

				MessageBoxA(0, "The memory trace dump is laid out \"CreationKitPlatformExtendedTracerMemory.log\"", 
					"Info", MB_OK | MB_ICONINFORMATION);
			}
		}

		void TracerManager::Clear()
		{
			_Items.clear();
		}
	}
}