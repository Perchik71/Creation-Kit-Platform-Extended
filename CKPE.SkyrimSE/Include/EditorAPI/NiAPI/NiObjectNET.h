// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiObjectNET.h

#pragma once

#include <EditorAPI/NiAPI/NiObject.h>
#include <EditorAPI/BSFixedString.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				// 18
				class NiExtraData : public NiObject
				{
				public:
					virtual ~NiExtraData() = 0;

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return name; }
				private:
					BSFixedString name;  // 10
				};
				static_assert(sizeof(NiExtraData) == 0x18);

				// 30
				class NiObjectNET : public NiObject
				{
				public:
					virtual ~NiObjectNET() = default;

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return _ObjectName; }

					inline void GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true)
					{
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
			}
		}
	}
}