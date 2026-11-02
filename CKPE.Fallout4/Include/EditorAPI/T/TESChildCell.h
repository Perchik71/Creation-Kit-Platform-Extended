#pragma once

#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL;
				class TESChildCell
				{
				public:
					virtual ~TESChildCell();

					constexpr static Common::ID RTTI = IDs::RTTI::TESChildCell;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESChildCell[0];

					// add
					[[nodiscard]] virtual TESObjectCELL* GetSaveParentCell() noexcept(true) = 0;
				};
				static_assert(sizeof(TESChildCell) == 0x8);
			}
		}
	}
}