// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/TESWater.h

#pragma once

#include <EditorAPI/Forms/TESWaterForm.h>
#include "BSTArray.h"
#include "BSTriShape.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class TESWaterObject
			{
			public:
				char _pad0[0x20];
				BSTriShape* m_TriShape;
				char _pad1[0x8];
				Forms::TESForm* m_BaseWaterForm;
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