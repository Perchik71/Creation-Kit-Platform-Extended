// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Singleton.h>
#include <EditorAPI/Forms/TESObjectREFR.h>

#pragma once
#pragma pack(push, 1)

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			struct TESHandleManagerTag
			{
				Forms::TESObjectREFR* Refr;
				std::uint32_t Index;
				char pad0C[4];
			};
			static_assert(sizeof(TESHandleManagerTag) == 0x10);

			class TESPointerHandleDetail
			{
			public:
				template<typename TType, std::uint32_t Total>
				class Manager
				{
				private:
					char pad08[0x50];
					std::uint32_t _HeadEntries;
					std::uint32_t _Unk5C;
					std::uint32_t _FreeEntries;
					std::uint32_t _Unk64;
					std::uint32_t _MaxEntries;
					std::uint32_t _TailEntries;
					char pad70[0x10];
					TType _Entries[Total];
				public:
					inline static ISingleton<Manager<TType, Total>> Singleton;

					virtual void Unk00();
					virtual std::uint32_t GetMax_Virtual() const;
					virtual void Unk10();

					inline std::uint32_t GetHead() const { return _HeadEntries; }
					inline std::uint32_t GetTail() const { return _TailEntries; }
					inline std::uint32_t GetFree() const { return _FreeEntries; }
					inline std::uint32_t GetMax() const { return _MaxEntries; }
				};
			};

			using HandleManager = TESPointerHandleDetail::Manager<TESHandleManagerTag, 8388608>;
		}
	}
}

#pragma pack(pop)