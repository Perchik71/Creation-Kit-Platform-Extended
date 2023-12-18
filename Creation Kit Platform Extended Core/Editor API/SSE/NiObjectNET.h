// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiObjectNET.h

#pragma once

#include "..\NiObject.h"
#include "BSFixedString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class NiObjectNET : public NiObject
			{
			public:
				virtual ~NiObjectNET() = default;

				BSFixedString m_kName;
				char _pad[0x18];

				inline const BSFixedString* GetName() const
				{
					return &m_kName;
				}

				inline void SetName(BSFixedString Name)
				{
					m_kName = Name;
				}

				inline void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
				{
					if (Recursive)
						__super::GetViewerStrings(Callback, Recursive);

					Callback("-- NiObjectNET --\n");
					Callback("Name = %s\n", GetName()->c_str());
				}
			};
			static_assert(sizeof(NiObjectNET) == 0x30);
		}
	}
}