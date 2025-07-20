// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	// thread-safe template versions of thisVirtualCall()

	template<typename TR>
	__forceinline TR vtbl_call(size_t reloff, const void* ths) 
	{
		return (*(TR(__fastcall**)(const void*))(*(__int64*)ths + reloff))(ths);
	}

	template<typename TR, typename T1>
	__forceinline TR vtbl_call(size_t reloff, const void* ths, T1 a1) 
	{
		return (*(TR(__fastcall**)(const void*, T1))(*(__int64*)ths + reloff))(ths, a1);
	}

	template<typename TR, typename T1, typename T2>
	__forceinline TR vtbl_call(size_t reloff, const void* ths, T1 a1, T2 a2) 
	{
		return (*(TR(__fastcall**)(const void*, T1, T2))(*(__int64*)ths + reloff))(ths, a1, a2);
	}

	template<typename TR, typename T1, typename T2, typename T3>
	__forceinline TR vtbl_call(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3) 
	{
		return (*(TR(__fastcall**)(const void*, T1, T2, T3))(*(__int64*)ths + reloff))(ths, a1, a2, a3);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4>
	__forceinline TR vtbl_call(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4)
	{
		return (*(TR(__fastcall**)(const void*, T1, T2, T3, T4))(*(__int64*)ths + reloff))(ths, a1, a2, a3, a4);
	}
}