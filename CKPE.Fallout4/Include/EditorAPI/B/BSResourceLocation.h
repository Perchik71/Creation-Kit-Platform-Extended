// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSResource.h>

namespace CKPE::Fallout4::EditorAPI::BSResource
{
	class LocationTraverser;

	class Location
	{
	public:
		virtual ~Location();
		virtual EResultError DoMount() noexcept(true);						// 01
		virtual void DoUnmount() noexcept(true);								// 02
		virtual EResultError DoCreateStream(const char* path, void* result,
			Location*& location, bool writable, LocationTraverser* traverser) = 0; // 03
		virtual EResultError DoCreateAsyncStream(const char* path, void* result,
			Location*& location, bool writable, LocationTraverser* traverser);	// 04
		virtual EResultError DoTraversePrefix(const char* prefix,
			LocationTraverser& traverser) = 0;								// 05
		virtual EResultError DoGetInfo(const char* path, void* result,
			LocationTraverser* traverser);									// 06
		virtual EResultError DoGetInfoWithLocation(const char* path,
			void* result, Location*& location);								// 07
		virtual EResultError DoDelete(const char* path);						// 08
		[[nodiscard]] virtual const char* DoGetName() const;					// 09
		[[nodiscard]] virtual std::uint32_t DoQBufferHint() const;			// 0A
		[[nodiscard]] virtual std::uint32_t DoGetMinimumAsyncPacketSize() const; // 0B

		/* 0x008 */ bool mounted;
		/* 0x009 */ std::byte pad009[0x7];
	};
	static_assert(sizeof(Location) == 0x10);
	static_assert(offsetof(Location, mounted) == 0x8);

	class LooseFileLocation : public Location
	{
	public:
		virtual ~LooseFileLocation() override;
		virtual EResultError DoCreateStream(const char* path, void* result,
			Location*& location, bool writable,
			LocationTraverser* traverser) override;
		virtual EResultError DoCreateAsyncStream(const char* path, void* result,
			Location*& location, bool writable,
			LocationTraverser* traverser) override;
		virtual EResultError DoTraversePrefix(const char* prefix,
			LocationTraverser& traverser) override;
		virtual EResultError DoGetInfo(const char* path, void* result,
			LocationTraverser* traverser) override;
		virtual EResultError DoGetInfoWithLocation(const char* path,
			void* result, Location*& location) override;
		virtual EResultError DoDelete(const char* path) override;
		[[nodiscard]] virtual const char* DoGetName() const override;
		[[nodiscard]] virtual std::uint32_t
			DoGetMinimumAsyncPacketSize() const override;

		/* 0x010 */ BSFixedString path;
		/* 0x018 */ std::uint32_t minimumAsyncPacketSize;
		/* 0x01C */ std::uint32_t unk01C;
	};
	static_assert(sizeof(LooseFileLocation) == 0x20);
	static_assert(offsetof(LooseFileLocation, path) == 0x10);
	static_assert(offsetof(LooseFileLocation, minimumAsyncPacketSize) == 0x18);
}
