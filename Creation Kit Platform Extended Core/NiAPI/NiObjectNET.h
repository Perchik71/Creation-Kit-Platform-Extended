// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiObjectNET.h

#pragma once

#include "NiRefObject.h"
#include "Editor API/FO4/BSEntryString.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		// 20
		class NiExtraData
		{
		public:
			virtual ~NiExtraData() = 0;
		private:
			char pad08[0x18];
		};
		static_assert(sizeof(NiExtraData) == 0x20);

		// 30
		class NiObjectNET : public NiRefObject_64
		{
		public:
			virtual ~NiObjectNET() = default;

			inline const char* GetName() const
			{
				return _ObjectName;
			}

			inline void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
			{
				if (Recursive)
					__super::GetViewerStrings(Callback, Recursive);

				Callback("-- NiObjectNET --\n");
				Callback("Name = %s\n", _ObjectName);
			}
		private:
			char* _ObjectName;
			char _pad18[0x8];
			NiExtraData** _ExtraData;
			uint16_t _ExtraDataListLen;		
			uint16_t _ExtraDataListCapacity;
			char _pad2C[0x4];
		};
		static_assert(sizeof(NiObjectNET) == 0x30);

		namespace Fallout4
		{
			class NiObjectNET : public NiRefObject_64
			{
			public:
				virtual ~NiObjectNET() = default;

				inline const char* GetName() const
				{
					return _ObjectName ? _ObjectName->Get<char>(TRUE) : "UNKNOWN";
				}

				inline void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
				{
					if (Recursive)
						__super::GetViewerStrings(Callback, Recursive);

					Callback("-- NiObjectNET --\n");
					Callback("Name = %s\n", GetName());
				}
			private:
				EditorAPI::Fallout4::BSEntryString* _ObjectName;
				char _pad18[0x10];
			};
			static_assert(sizeof(NiObjectNET) == 0x28);
		}
	}
}

#pragma pack(pop)