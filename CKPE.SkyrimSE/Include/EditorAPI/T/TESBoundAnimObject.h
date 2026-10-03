#pragma once

#include <EditorAPI/T/TESBoundObject.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESBoundAnimObject : public TESBoundObject
				{
				public:
					virtual ~TESBoundAnimObject() = default;

					// override (TESBoundObject)
					bool IsBoundAnimObject() noexcept(true) override { return true; }
					bool ReplaceModel(const char* a_str) override;
				};
				static_assert(sizeof(TESBoundAnimObject) == 0x50);
			}
		}
	}
}