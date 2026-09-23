#pragma once

namespace CKPE
{
	namespace SkyrimSE
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

					// add
					[[nodiscard]] virtual TESObjectCELL* GetSaveParentCell() noexcept(true) = 0;
				};
				static_assert(sizeof(TESChildCell) == 0x8);
			}
		}
	}
}