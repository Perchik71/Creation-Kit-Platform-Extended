// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "NiAPI/NiTArray.h"
#include "Core/Singleton.h"
#include "Editor API/BSTArray.h"
#include "TESForm.h"

//#include "QtCore/qarraydata.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class TESDataHandler
			{
			public:
				/*class TESFormArray
				{
					char pad00[8];
					uint32_t Index;
					char pad0C[4];
					BSTArray<TESForm*> Items;
				};*/

				inline static Core::ISingleton<bool> UserModdedSingleton;
				inline static Core::ISingleton<TESDataHandler> Singleton;
			};
			//static_assert(sizeof(TESDataHandler::TESFormArray) == 0x20)

			/*class TESDataHandler_CKPEIntf
			{
			public:
				virtual TESDataHandler* GetSingleton();
				virtual TESForm* GetFormById(uint32_t FormId);
			};*/
		}
	}
}

#pragma pack(pop)