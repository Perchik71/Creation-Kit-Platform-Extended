// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "ConsoleWindow.h"
#include "TracerManager.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		TracerManager* GlobalTracerManagerPtr = nullptr;

		TracerManager::TracerManager() : _Rec(false)
		{}

		void TracerManager::Push(const char* _Name, uintptr_t _MemAddress, size_t _Size, uintptr_t _RetAddress)
		{
			if (!_MemAddress || !_Rec)
				return;

			lock.lock();
			{
				TracerInfo info =
				{
					.Name = _Name,
					.RetAddress = _RetAddress,
					.MemAddress = _MemAddress,
					.Size = _Size
				};

				_Items.push_back(info);
			}
			lock.unlock();
		}

		void TracerManager::Pop(uintptr_t _MemAddress)
		{
			if (!_MemAddress || !_Rec)
				return;

			lock.lock();
			{
				for (auto It = _Items.begin(); It != _Items.end(); It++)
				{
					if (It->MemAddress == _MemAddress)
					{
						_Items.erase(It);
						break;
					}
				}
			}
			lock.unlock();
		}

		void TracerManager::Record()
		{
			if (_Rec)
				return;

			lock.lock();
			{
				_Rec = true;
				GlobalConsoleWindowPtr->InputLog("TRACER: Start of recording");
			}
			lock.unlock();
		}

		void TracerManager::Stop()
		{
			if (!_Rec)
				return;

			lock.lock();
			{
				_Rec = false;
				GlobalConsoleWindowPtr->InputLog("TRACER: End of recording");
			}
			lock.unlock();
		}

		void TracerManager::Dump()
		{
			if (_Rec)
				return;

			lock.lock();
			{
				auto Steam = _fsopen((Utils::GetApplicationPath() + "CreationKitPlatformExtendedTracerMemory.log").c_str(),
					"wt+", _SH_DENYWR);

				if (Steam)
				{
					char szBuf[130] = { 0 };
					sprintf_s(szBuf, "Now the \"%llu\" number of lines will be written. Continue?", _Items.size());
					if (MessageBoxA(0, szBuf, "Question", MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
						return;

					{
						size_t TotalMemAllocSize = 0;
						Utils::ScopeFileStream FileStream(Steam);

						fprintf(Steam, "Dumping objects.\n");
						for (auto It = _Items.begin(); It != _Items.end(); It++)
						{
							fprintf(Steam, "%s -> normal block at %p, %llu bytes, where from %p\n",
								(It->Name ? It->Name : "UNKNOWN"), It->MemAddress, It->Size, It->RetAddress);

							fprintf(Steam, "  Data <");
							char* Data = (char*)It->MemAddress;
							for (size_t i = 0; i < It->Size; i++)
								fputc((isprint(Data[i]) ? Data[i] : ' '), Steam);
							fputs("> ", Steam);
							for (size_t i = 0; i < It->Size; i++)
								fprintf(Steam, "%02X ", (unsigned char)(Data[i]));
							fputc('\n', Steam);

							TotalMemAllocSize += It->Size;
						}
						if (TotalMemAllocSize >= (1024 * 1024))
							fprintf(Steam, "Total memory allocated size %.3f Mb.\n", ((double)TotalMemAllocSize / (1024 * 1024)));
						else
							fprintf(Steam, "Total memory allocated size %llu bytes.\n", TotalMemAllocSize);
						fprintf(Steam, "Object dump complete.\n");
					}

					GlobalConsoleWindowPtr->InputLog("TRACER: The memory trace dump is laid out \"CreationKitPlatformExtendedTracerMemory.log\"");
				}
			}
			lock.unlock();
		}

		void TracerManager::Clear()
		{
			if (_Rec)
				return;

			lock.lock();
			{
				_Items.clear();
				GlobalConsoleWindowPtr->InputLog("TRACER: Erasing records");
			}
			lock.unlock();
		}
	}
}