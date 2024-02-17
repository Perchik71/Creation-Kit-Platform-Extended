// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#define CKPE_USES_TRACER 0

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class TracerManager
		{
		public:
			struct TracerInfo
			{
				const char* Name;
				uintptr_t RetAddress;
				uintptr_t MemAddress;
				size_t Size;
				bool Active;
			};
		public:
			TracerManager();

			void Push(const char* _Name, uintptr_t _MemAddress, size_t _Size, uintptr_t _RetAddress = (uintptr_t)_ReturnAddress());
			void Pop(uintptr_t _MemAddress);

			inline void Record() { _Rec = true; }
			inline void Stop() { _Rec = false; }

			void Dump();
			void Clear();
		private:
			TracerManager(const TracerManager&) = default;
			TracerManager& operator=(const TracerManager&) = default;

			concurrency::concurrent_vector<TracerInfo> _Items;
			bool _Rec;
		};

		extern TracerManager* GlobalTracerManagerPtr;
	}
}

#if CKPE_USES_TRACER
#define _CKPE_Tracer CreationKitPlatformExtended::Core::GlobalTracerManagerPtr
#define _CKPE_TracerPush(x, y, z) if (_CKPE_Tracer) _CKPE_Tracer->Push(x, (uintptr_t)y, (size_t)z)
#define _CKPE_TracerPop(x) if (_CKPE_Tracer) _CKPE_Tracer->Pop((uintptr_t)x)
#else
#define _CKPE_Tracer
#define _CKPE_TracerPush(x, y, z) 
#define _CKPE_TracerPop(x)
#endif