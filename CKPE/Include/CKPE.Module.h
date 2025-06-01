// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <CKPE.Segment.h>
#include <CKPE.PEDirectory.h>

//	x-------	major
//	-xxx----	minor
//	----xxxx	build

#define MAKE_EXE_VERSION(major, minor, build)	((((major) & 0xF) << 28) | (((minor) & 0xFFF) << 16) | ((build) & 0xFFFF))

#define GET_EXE_VERSION_MAJOR(a)	(((a) & 0xF0000000ul) >> 28)
#define GET_EXE_VERSION_MINOR(a)	(((a) & 0x0FFF0000ul) >> 16)
#define GET_EXE_VERSION_BUILD(a)	(((a) & 0x0000FFFFul))

// xxxx------------ major
// ----xxxx-------- minor
// --------xxxx---- build
// ------------xxxx revision

#define MAKE_EXE_VERSION_EX(major, minor, build, revision) \
	((((std::uint64_t)(major) & 0xFFFF) << 48) | \
	 (((std::uint64_t)(minor) & 0xFFFF) << 32) | \
	 (((std::uint64_t)(build) & 0xFFFF) << 16) | \
	 ((std::uint64_t)(revision) & 0xFFFF))

#define GET_EXE_VERSION_EX_MAJOR(a)		((std::uint16_t)(((a) & 0xFFFF000000000000ull) >> 48))
#define GET_EXE_VERSION_EX_MINOR(a)		((std::uint16_t)(((a) & 0x0000FFFF00000000ull) >> 32))
#define GET_EXE_VERSION_EX_BUILD(a)		((std::uint16_t)(((a) & 0x00000000FFFF0000ull) >> 16))
#define GET_EXE_VERSION_EX_REVISION(a)	((std::uint16_t)( (a) & 0x000000000000FFFFull))

namespace CKPE
{
	class CKPE_API Module
	{
		const void* _handle{ nullptr };
		std::wstring* _fname{ nullptr };
		std::array<Segment, Segment::total> _segments;
		std::array<PEDirectory, PEDirectory::total> _directories;

		constexpr Module() noexcept(true) = default;
		Module(const Module&) = delete;
		Module& operator=(const Module&) = delete;
	protected:
		void LoadSegments();
		void LoadDirectories();
	public:
		Module(const void* m) noexcept(true);

		void LoadInfo();

		[[nodiscard]] bool Is64() const noexcept(true);
		[[nodiscard]] inline std::wstring GetFileName() const noexcept(true) { return _fname ? *_fname : L""; }
		[[nodiscard]] inline constexpr virtual std::uintptr_t GetBase() const noexcept(true) { return (std::uintptr_t)_handle; }
		[[nodiscard]] inline constexpr Segment GetSegment(Segment::Name a_segment) const noexcept(true) { return _segments[a_segment]; }
		[[nodiscard]] inline constexpr PEDirectory GetPEDirectory(PEDirectory::Name a_segment) const noexcept(true) { return _directories[a_segment]; }
		[[nodiscard]] inline void* GetPointer() const noexcept(true) { return reinterpret_cast<void*>(GetBase()); }
		template <class T> [[nodiscard]] inline T* GetPointer() const noexcept(true) { return static_cast<T*>(GetPointer()); }

		class CKPE_API Resource
		{
			const Module* _module{ nullptr };

			Resource(const Resource&) = delete;
			Resource& operator=(const Resource&) = delete;
		public:
			inline constexpr Resource(const Module* m) noexcept(true) : _module(m) {};
			[[nodiscard]] const void* GetProcAddress(const char* name) const noexcept(true);
		} Resources{ this };

		[[nodiscard]] const void* GetProcAddress(const char* name) const noexcept(true);

		friend class Application;
	};

	class CKPE_API ScopeLoadedModule
	{
		const Module* _m;
	public:
		ScopeLoadedModule(const Module& m) noexcept(true);
		~ScopeLoadedModule() noexcept(true);
	};
}
