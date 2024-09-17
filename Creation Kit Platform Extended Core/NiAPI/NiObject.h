// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiRTTI.h"
#include "NiRefObject.h"

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		class NiStream;
		class NiCloningProcess;

		namespace Templates
		{
			// 10
			template<
				uint32_t REL_RTTI, 
				uint32_t REL_CREATECLONE,
				uint32_t REL_LOADFROMSTREAM,
				uint32_t REL_SAVETOSTREAM,
				uint32_t REL_LINKOBJECT,
				uint32_t REL_REGSTREAM>
			class NiObjectT : public NiRefObject_64
			{
			public:
				// It's everywhere
				inline NiRTTI* GetRTTI() const { return thisVirtualCall<NiRTTI*>(REL_RTTI, this); }

				// Creation of a clone object is mentioned in SKSE and F4SE, however, in SKSE it is specified without a refs, I think this is a mistake
				inline NiObject* CreateClone(NiCloningProcess* Process) const { return thisVirtualCall<NiObject*>(REL_CREATECLONE, this, Process); }
				
				// There is a mention of these functions in SKSE and F4SE, actually this is no exception for CK
				inline void LoadFromStream(NiStream* Stream) const { thisVirtualCall<void>(REL_LOADFROMSTREAM, this, Stream); }
				inline void LinkObject(NiStream* Stream) const { thisVirtualCall<void>(REL_LINKOBJECT, this, Stream); }
				inline bool RegisterStreamables(NiStream* Stream) const { return thisVirtualCall<bool>(REL_REGSTREAM, this, Stream); }
				inline void SaveToStream(NiStream* Stream) const { thisVirtualCall<void>(REL_SAVETOSTREAM, this, Stream); }

				inline void GetViewerRTTI(void(*Callback)(const char*, ...), bool Fully = false) const
				{ 
					auto Info = GetRTTI();

					if (Fully)
						for (auto iter = this->GetRTTI()->GetParent(); iter; iter = iter->GetParent())
							Callback("---- %s ----\n", iter->GetNameClass());
				
					Callback("-- %s --\n", Info->GetNameClass());
				}
			};
		}

		class NiObject : public Templates::NiObjectT<0x10, 0xB8, 0xC0, 0xD8, 0xC8, 0xD0>
		{};
		static_assert(sizeof(NiObject) == 0x10);

		namespace Fallout4
		{
			class NiObject : public Templates::NiObjectT<0x10, 0xD0, 0xD8, 0xF0, 0xC0, 0xC8>
			{};
			static_assert(sizeof(NiObject) == 0x10);
		}

		namespace Starfield
		{
			class NiObject : public Templates::NiObjectT<0x10, 0x148, 0x150, 0x168, 0x158, 0x160>
			{};
			static_assert(sizeof(NiObject) == 0x10);
		}
	}
}