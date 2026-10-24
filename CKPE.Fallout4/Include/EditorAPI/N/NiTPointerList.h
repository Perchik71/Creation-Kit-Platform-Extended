// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				template<typename T>
				class NiTPointerList
				{
				public:
					struct Item
					{
						/* 0x00 */ Item* Next;
						/* 0x08 */ Item* Previous;
						/* 0x10 */ T Value;
					};

					[[nodiscard]] inline const Item* GetHead() const noexcept(true) { return _Head; }
					[[nodiscard]] inline std::uint32_t GetSize() const noexcept(true) { return _Size; }
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiTPointerList, _Head) == 0x0 &&
							offsetof(NiTPointerList, _Tail) == 0x8 &&
							offsetof(NiTPointerList, _Size) == 0x10;
					}
				private:
					/* 0x00 */ Item* _Head;
					/* 0x08 */ Item* _Tail;
					/* 0x10 */ std::uint32_t _Size;
					/* 0x14 */ std::uint32_t _pad14;
				};
			}
		}
	}
}
