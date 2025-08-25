// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Singleton.h"
#include "TESObjectREFR.h"

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			struct TESHandleManagerTag
			{
				TESObjectREFR* Refr;
				uint32_t Index;
				char pad0C[4];
			};
			static_assert(sizeof(TESHandleManagerTag) == 0x10);

			class TESPointerHandleDetail
			{
			public:
				template<typename TType, uint32_t Total>
				class Manager
				{
				private:
					char pad08[0x50];
					uint32_t _HeadEntries;
					uint32_t _Unk5C;
					uint32_t _FreeEntries;
					uint32_t _Unk64;
					uint32_t _MaxEntries;
					uint32_t _TailEntries;
					char pad70[0x10];
					TType _Entries[Total];
				public:
					inline static Core::ISingleton<Manager<TType, Total>> Singleton;

					virtual void Unk00();
					virtual uint32_t GetMax_Virtual() const;
					virtual void Unk10();

					inline uint32_t GetHead() const { return _HeadEntries; }
					inline uint32_t GetTail() const { return _TailEntries; }
					inline uint32_t GetFree() const { return _FreeEntries; }
					inline uint32_t GetMax() const { return _MaxEntries; }
				};
			};

			using HandleManager = TESPointerHandleDetail::Manager<TESHandleManagerTag, 8388608>;
		}
	}
}

#pragma pack(pop)