// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>			// std::size_t :/
#include <CKPE.Common.h>

namespace CKPE
{
	// thread-safe template versions of vtbl_call()

	template<typename TR>
	__forceinline TR vtbl_call(std::size_t reloff, const void* ths) noexcept(true)
	{
		return (*(TR(__fastcall**)(const void*))(*(__int64*)ths + reloff))(ths);
	}

	template<typename TR, typename T1>
	__forceinline TR vtbl_call(std::size_t reloff, const void* ths, T1 a1) noexcept(true)
	{
		return (*(TR(__fastcall**)(const void*, T1))(*(__int64*)ths + reloff))(ths, a1);
	}

	template<typename TR, typename T1, typename T2>
	__forceinline TR vtbl_call(std::size_t reloff, const void* ths, T1 a1, T2 a2) noexcept(true)
	{
		return (*(TR(__fastcall**)(const void*, T1, T2))(*(__int64*)ths + reloff))(ths, a1, a2);
	}

	template<typename TR, typename T1, typename T2, typename T3>
	__forceinline TR vtbl_call(std::size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3) noexcept(true)
	{
		return (*(TR(__fastcall**)(const void*, T1, T2, T3))(*(__int64*)ths + reloff))(ths, a1, a2, a3);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4>
	__forceinline TR vtbl_call(std::size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4) noexcept(true)
	{
		return (*(TR(__fastcall**)(const void*, T1, T2, T3, T4))(*(__int64*)ths + reloff))(ths, a1, a2, a3, a4);
	}

	// thread-safe template versions of fast_call()

	template<typename TR>
	__forceinline TR fast_call(std::size_t addr) {
		return ((TR(__fastcall*)())(addr))();
	}

	template<typename TR, typename T1>
	__forceinline TR fast_call(std::size_t addr, T1 a1) {
		return ((TR(__fastcall*)(T1))(addr))(a1);
	}

	template<typename TR, typename T1, typename T2>
	__forceinline TR fast_call(std::size_t addr, T1 a1, T2 a2) {
		return ((TR(__fastcall*)(T1, T2))(addr))(a1, a2);
	}

	template<typename TR, typename T1, typename T2, typename T3>
	__forceinline TR fast_call(std::size_t addr, T1 a1, T2 a2, T3 a3) {
		return ((TR(__fastcall*)(T1, T2, T3))(addr))(a1, a2, a3);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4>
	__forceinline TR fast_call(std::size_t addr, T1 a1, T2 a2, T3 a3, T4 a4) {
		return ((TR(__fastcall*)(T1, T2, T3, T4))(addr))(a1, a2, a3, a4);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5>
	__forceinline TR fast_call(std::size_t addr, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
		return ((TR(__fastcall*)(T1, T2, T3, T4, T5))(addr))(a1, a2, a3, a4, a5);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	__forceinline TR fast_call(std::size_t addr, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
		return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6))(addr))(a1, a2, a3, a4, a5, a6);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	__forceinline TR fast_call(std::size_t addr, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
		return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7))(addr))(a1, a2, a3, a4, a5, a6, a7);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	__forceinline TR fast_call(std::size_t addr, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
		return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7, T8))(addr))(a1, a2, a3, a4, a5, a6, a7, a8);
	}

	// thread-safe template versions of this_call()

	template<typename TR>
	__forceinline TR this_call(std::size_t addr, const void* ths) {
		return ((TR(__fastcall*)(const void*))(addr))(ths);
	}

	template<typename TR, typename T1>
	__forceinline TR this_call(std::size_t addr, const void* ths, T1 a1) {
		return ((TR(__fastcall*)(const void*, T1))(addr))(ths, a1);
	}

	template<typename TR, typename T1, typename T2>
	__forceinline TR this_call(std::size_t addr, const void* ths, T1 a1, T2 a2) {
		return ((TR(__fastcall*)(const void*, T1, T2))(addr))(ths, a1, a2);
	}

	template<typename TR, typename T1, typename T2, typename T3>
	__forceinline TR this_call(std::size_t addr, const void* ths, T1 a1, T2 a2, T3 a3) {
		return ((TR(__fastcall*)(const void*, T1, T2, T3))(addr))(ths, a1, a2, a3);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4>
	__forceinline TR this_call(std::size_t addr, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4) {
		return ((TR(__fastcall*)(const void*, T1, T2, T3, T4))(addr))(ths, a1, a2, a3, a4);
	}

	CKPE_API bool CKPE_UserHasAdminRights() noexcept(true);
}