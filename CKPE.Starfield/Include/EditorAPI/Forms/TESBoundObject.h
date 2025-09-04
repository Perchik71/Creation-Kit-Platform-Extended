// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESObject.h"
#include "BGSMod.h"
#include "BGSObjectPlacementDefaults.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESBoundObject : public TESObject
				{
				public:
					NiAPI::NiPoint3            BoundMin;           // 1A8
					NiAPI::NiPoint3            BoundMax;           // 1B4
					BGSMod::Template::Items    TemplateItems;      // 1C0
					BGSPreviewTransform        PreviewTransform;   // 210
					BGSObjectPlacementDefaults PlacementDefaults;  // 298
				};
				static_assert(offsetof(TESBoundObject, PreviewTransform) == 0x218);
				static_assert(offsetof(TESBoundObject, PlacementDefaults) == 0x298);
				static_assert(sizeof(TESBoundObject) == 0x2F0);
			}
		}
	}
}