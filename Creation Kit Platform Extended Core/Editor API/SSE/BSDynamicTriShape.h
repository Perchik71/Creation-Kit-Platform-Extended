// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/BSDynamicTriShape.h

#pragma once

#include "../BSSpinLock.h"
#include "BSGraphicsTypes.h"
#include "BSTriShape.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class BSDynamicTriShape : public BSTriShape
			{
			public:
				virtual ~BSDynamicTriShape() = default;

				void* pDynamicData;
				BSSpinLock DynamicDataAccessSpinLock;
				uint32_t DynamicDataSize;
				uint32_t uiFrameCount;
				BSGraphics::DynamicTriShapeDrawData DrawData;
				char _pad0[0x4];

				void* LockDynamicData()
				{
					DynamicDataAccessSpinLock.Acquire();
					return pDynamicData;
				}

				const void* LockDynamicDataForRead()
				{
					return LockDynamicData();
				}

				void UnlockDynamicData()
				{
					DynamicDataAccessSpinLock.Release();
				}

				uint32_t QDynamicDataSize() const
				{
					return DynamicDataSize;
				}
			};
			static_assert(sizeof(BSDynamicTriShape) == 0x180);
		}
	}
}