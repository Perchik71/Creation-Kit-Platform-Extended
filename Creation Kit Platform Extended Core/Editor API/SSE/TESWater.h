// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/TESWater.h

#pragma once

#include "../BSTArray.h"
#include "TESForm.h"
#include "BSTriShape.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESWaterObject
			{
			public:
				char _pad0[0x20];
				BSTriShape* m_TriShape;
				char _pad1[0x8];
				TESForm* m_BaseWaterForm;
			};

			class TESWaterRoot
			{
			public:
				char _pad0[0x28];
				BSTArray<TESWaterObject*> m_WaterObjects;

				inline static TESWaterRoot* (*SingletonFunc)();
				inline static TESWaterRoot* Singleton() { return SingletonFunc(); }
			};
		}
	}
}