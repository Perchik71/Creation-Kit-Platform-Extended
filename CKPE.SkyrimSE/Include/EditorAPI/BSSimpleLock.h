#pragma once

#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class SimpleLock
			{
				enum
				{
					kFastSpinThreshold = 10000
				};

				volatile std::int32_t threadID;	// 00
				std::uint32_t lockCount;		// 04

			public:
				SimpleLock() : threadID(0), lockCount(0) {}

				void Lock() noexcept(true);
				void Release() noexcept(true);
			};
			static_assert(sizeof(SimpleLock) == 0x8);
		}
	}
}