// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class DynamicCast
		{
		public:
			DynamicCast();

			virtual bool Dump(const char* FileName) const;
			virtual void* Cast(const void* InPtr, long VfDelta,
				const char* lpstrFromType, const char* lpstrTargetType, bool isReference = false);
			virtual void* CastNoCache(const void* InPtr, long VfDelta,
				const char* lpstrFromType, const char* lpstrTargetType, bool isReference = false);
		private:
			DynamicCast(const DynamicCast&) = default;
			DynamicCast& operator=(const DynamicCast&) = default;

			std::pair<std::string_view, const void*> _CacheFrom;
			std::pair<std::string_view, const void*> _CacheTarget;
		};

		extern DynamicCast* GlobalDynamicCastPtr;

		inline static void* _DYNAMIC_CAST(const void* InPtr, long VfDelta, const char* lpstrFromType,
			const char* lpstrTargetType, bool isReference = false)
		{
			return GlobalDynamicCastPtr->Cast(InPtr, VfDelta, lpstrFromType, lpstrTargetType, isReference);
		}

		inline static void* _DYNAMIC_CAST2(const void* InPtr, long VfDelta, const char* lpstrFromType,
			const char* lpstrTargetType, bool isReference = false)
		{
			return GlobalDynamicCastPtr->CastNoCache(InPtr, VfDelta, lpstrFromType, lpstrTargetType, isReference);
		}
	}
}