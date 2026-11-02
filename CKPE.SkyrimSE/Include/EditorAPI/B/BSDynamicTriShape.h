// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/BSDynamicTriShape.h

#pragma once

#include <EditorAPI/B/BSSpinLock.h>
#include <EditorAPI/B/BSGraphicsTypes.h>
#include <EditorAPI/B/BSTriShape.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSDynamicTriShape : public BSTriShape
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSDynamicTriShape;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSDynamicTriShape[0];

				virtual ~BSDynamicTriShape() = default;

				void* pDynamicData;
				BSSpinLock DynamicDataAccessSpinLock;
				std::uint32_t DynamicDataSize;
				std::uint32_t uiFrameCount;
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

				std::uint32_t QDynamicDataSize() const
				{
					return DynamicDataSize;
				}
			};
			static_assert(sizeof(BSDynamicTriShape) == 0x180);
		}
	}
}