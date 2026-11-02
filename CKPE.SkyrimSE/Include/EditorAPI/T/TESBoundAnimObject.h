#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

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
					constexpr static Common::ID RTTI = IDs::RTTI::TESBoundAnimObject;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESBoundAnimObject[0];

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