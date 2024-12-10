// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		typedef int32_t IResult;

		constexpr static IResult RC_OK = 0;
		constexpr static IResult RC_UNSUPPORTED_VERSION_CREATIONKIT = 1;
		constexpr static IResult RC_DEPRECATED_VERSION_CREATIONKIT = 2;
		constexpr static IResult RC_UNKNOWN_APPLICATION = 3;
		constexpr static IResult RC_INITIALIZATION_ENGINE_AGAIN = 4;
		constexpr static IResult RC_UNSUPPORT_VERSION_OS = 5;
		
		inline bool Successed(IResult nErrorNo) { return nErrorNo == RC_OK; }
		inline bool Failed(IResult nErrorNo) { return nErrorNo != RC_OK; }

		const char* GetMessageStringByErrorNo(IResult nErrorNo);
	}
}
