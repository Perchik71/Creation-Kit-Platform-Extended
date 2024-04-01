// This tutorial is about using the CKPE plugin system.
// To clarify how you can use this and how to write your plugins.
// 
// LICENSE
// This file for a test project, and serves as a guide for creating plugins.
// 
// You can also use the assets of this test project, some platform classes are passed with their interfaces 
// without implementation, so that they can be used inside the plugin, while you will not necessarily need 
// to open the code to the public. But you cannot use CKPE assets, in which case you must disclose 
// the code and indicate authorship. And also you can't re-license, only GPLv3.

#pragma once

namespace CreationKitPlatformExtended
{
	template<typename _Ty>
	using BaseString = std::basic_string<_Ty, std::char_traits<_Ty>, voltek::allocator<_Ty>>;
	using String = BaseString<char>;
	using WideString = BaseString<wchar_t>;

	template<typename _Ty>
	using Array = std::vector<_Ty, voltek::allocator<_Ty>>;

	template<typename _Ty>
	using List = std::list<_Ty, voltek::allocator<_Ty>>;

	template<typename _Ty>
	using ConcurrencyArray = concurrency::concurrent_vector<_Ty, voltek::allocator<_Ty>>;

	template<typename _kTy, typename _Ty, typename _Pr = std::less<_kTy>>
	using Map = std::map<_kTy, _Ty, _Pr, voltek::allocator<std::pair<const _kTy, _Ty>>>;

	template<typename _kTy, typename _Ty, typename _Hasher = std::hash<_kTy>,
		typename _Equal = std::equal_to<_kTy>>
	using UnorderedMap = std::unordered_map<_kTy, _Ty, _Hasher, _Equal, 
		voltek::allocator<std::pair<const _kTy, _Ty>>>;

	template<typename _kTy, typename _Ty, typename _Hasher = std::hash<_kTy>, 
		typename _Equal = std::equal_to<_kTy>>
	using ConcurrencyMap = concurrency::concurrent_unordered_map<_kTy, _Ty, 
		_Hasher, _Equal, voltek::allocator<std::pair<const _kTy, _Ty>>>;

	template<typename _kTy, typename _Pr = std::less<_kTy>>
	using Set = std::set<_kTy, _Pr, voltek::allocator<_kTy>>;

	template<typename _kTy, typename _Hasher = std::hash<_kTy>, 
		typename _Equal = std::equal_to<_kTy>>
	using UnorderedSet = std::unordered_set<_kTy, _Hasher, _Equal, voltek::allocator<_kTy>>;
}