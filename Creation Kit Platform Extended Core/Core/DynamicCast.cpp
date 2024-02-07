// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma warning (disable : 5040)

#include "Engine.h"
#include "DynamicCast.h"
#include "TypeInfo/ms_rtti.h"

extern "C" PVOID __CLRCALL_OR_CDECL __RTDynamicCast(
	PVOID inptr,            // Pointer to polymorphic object
	LONG VfDelta,           // Offset of vfptr in object
	PVOID SrcType,          // Static type of object pointed to by inptr
	PVOID TargetType,       // Desired result of cast
	BOOL isReference)       // TRUE if input is reference, FALSE if input is ptr
	throw(...);

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		DynamicCast* GlobalDynamicCastPtr = nullptr;

		DynamicCast::DynamicCast()
		{
			MSRTTI::Initialize();
		}

		bool DynamicCast::Dump(const char* FileName) const
		{
			FILE* fileStream = _wfsopen(Utils::Utf82Wide((char8_t*)FileName).c_str(), L"wt", _SH_DENYRW);
			if (fileStream)
			{
				Utils::ScopeFileStream file(fileStream);
				MSRTTI::Dump(fileStream);
				return true;
			}

			return false;
		}

		void* DynamicCast::Cast(const void* InPtr, long VfDelta, const char* lpstrFromType,
			const char* lpstrTargetType, bool isReference)
		{
			const MSRTTI::Info* FromInfo = nullptr;
			const MSRTTI::Info* TargetInfo = nullptr;
			
			if (!_CacheFrom.first.compare(lpstrFromType))
				FromInfo = reinterpret_cast<const MSRTTI::Info*>(_CacheFrom.second);
			else
			{
				FromInfo = MSRTTI::Find(lpstrFromType);
				if (FromInfo)
				{
					_CacheFrom.first = lpstrFromType;
					_CacheFrom.second = FromInfo;
				}
			}

			if (!_CacheTarget.first.compare(lpstrTargetType))
				TargetInfo = reinterpret_cast<const MSRTTI::Info*>(_CacheTarget.second);
			else
			{
				TargetInfo = MSRTTI::Find(lpstrTargetType);
				if (TargetInfo)
				{ 
					_CacheTarget.first = lpstrTargetType;
					_CacheTarget.second = TargetInfo;
				}
			}

			if (!FromInfo || !TargetInfo) 
				return nullptr;

			return __RTDynamicCast((PVOID)InPtr, VfDelta,
				(PVOID)FromInfo->Locator->TypeDescriptor.Get(),
				(PVOID)TargetInfo->Locator->TypeDescriptor.Get(),
				isReference);
		}

		void* DynamicCast::CastNoCache(const void* InPtr, long VfDelta,
			const char* lpstrFromType, const char* lpstrTargetType, bool isReference)
		{
			auto FromInfo = MSRTTI::Find(lpstrFromType);
			auto TargetInfo = MSRTTI::Find(lpstrTargetType);

			if (!FromInfo || !TargetInfo)
				return nullptr;

			return __RTDynamicCast((PVOID)InPtr, VfDelta,
				(PVOID)FromInfo->Locator->TypeDescriptor.Get(),
				(PVOID)TargetInfo->Locator->TypeDescriptor.Get(),
				isReference);
		}
	}
}