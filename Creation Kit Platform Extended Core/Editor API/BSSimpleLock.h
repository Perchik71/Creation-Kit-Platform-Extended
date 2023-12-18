#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class SimpleLock
		{
			enum
			{
				kFastSpinThreshold = 10000
			};

			volatile LONG threadID;	// 00
			ULONG lockCount;		// 04

		public:
			SimpleLock() : threadID(0), lockCount(0) {}

			void Lock(void);
			void Release(void);
		};
		static_assert(sizeof(SimpleLock) == 0x8);
	}
}