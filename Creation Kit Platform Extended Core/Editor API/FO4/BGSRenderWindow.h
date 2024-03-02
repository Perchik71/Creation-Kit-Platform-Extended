// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Singleton.h"
#include "NiAPI/NiTypes.h"
#include "NiAPI/NiNode.h"
#include "TESObjectREFR.h"
#include "BSPointerHandleManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class BGSRenderWindow
			{
			public:
				virtual ~BGSRenderWindow() = default;

				inline static Core::ISingleton<BGSRenderWindow> Singleton;
				inline static TESForm* GetRef(uint32_t UniqueId)
				{
					auto Ret = (TESObjectREFR*)GetRefFormByUniqueId(UniqueId);
					return (Ret && Ret->Type == TESObjectREFR::TYPE_ID) ? Ret : nullptr;
				}
			};
		}
	}
}