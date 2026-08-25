// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstddef>
#include <initializer_list>
#include <CKPE.PluginAPI.PluginAPI.h>

// Header-only convenience wrapper around CKPEAddressLibraryInterface, mirroring the
// CKPE::Common::Relocation / ID / Offset syntax used internally by CKPE's own patches.
//
// One-time setup, once you have your CKPEPluginInterface*:
//
//   CKPE::PluginAPI::Relocation::Init(intfc->QueryInterface(CKPE::PluginAPI::kInterface_AddressLibrary));
//
// After that:
//
//   using namespace CKPE::PluginAPI;
//   Relocation(ID{ 12345 }, Offset{ 0x10 }).WriteCall(&MyHook);
//   Relocation(ID{ 12345 }).Address();

namespace CKPE
{
	namespace PluginAPI
	{
		class ID
		{
			std::uint64_t _id{ 0 };
		public:
			constexpr ID() noexcept = default;
			constexpr explicit ID(std::uint64_t a_id) noexcept : _id(a_id) {}

			[[nodiscard]] constexpr std::uint64_t Value() const noexcept { return _id; }
		};

		class Offset
		{
			std::ptrdiff_t _offset{ 0 };
		public:
			constexpr Offset() noexcept = default;
			constexpr explicit Offset(std::ptrdiff_t a_offset) noexcept : _offset(a_offset) {}

			[[nodiscard]] constexpr std::ptrdiff_t Value() const noexcept { return _offset; }
		};

		class Relocation
		{
			std::uintptr_t _impl{ 0 };

			static inline CKPEAddressLibraryInterface* s_iface{ nullptr };
		public:
			static void Init(void* a_iface) noexcept { s_iface = reinterpret_cast<CKPEAddressLibraryInterface*>(a_iface); }
			[[nodiscard]] static bool IsInitialized() noexcept { return s_iface != nullptr; }
			[[nodiscard]] static bool IsAddressLibraryLoaded() noexcept { return s_iface && s_iface->IsLoaded(); }

			constexpr Relocation() noexcept = default;

			explicit constexpr Relocation(std::uintptr_t a_address) noexcept :
				_impl(a_address)
			{}

			explicit Relocation(const ID& a_id, const Offset& a_offset = Offset{}) noexcept :
				_impl(s_iface ? s_iface->Resolve(a_id.Value(), a_offset.Value()) : 0)
			{}

			[[nodiscard]] constexpr std::uintptr_t Address() const noexcept { return _impl; }
			[[nodiscard]] constexpr operator bool() const noexcept { return _impl != 0; }

			void Write(const std::uint8_t* a_data, std::size_t a_count) const noexcept
			{
				if (_impl && s_iface)
					s_iface->Write(_impl, a_data, a_count);
			}

			void Write(std::initializer_list<std::uint8_t> a_data) const noexcept
			{
				Write(a_data.begin(), a_data.size());
			}

			void WriteFill(std::uint8_t a_value, std::size_t a_count) const noexcept
			{
				if (_impl && s_iface)
					s_iface->WriteSet(_impl, a_value, a_count);
			}

			void WriteNop(std::size_t a_count) const noexcept
			{
				if (_impl && s_iface)
					s_iface->WriteNop(_impl, a_count);
			}

			std::uintptr_t WriteJump(std::uintptr_t a_dst) const noexcept
			{
				return (_impl && s_iface) ? s_iface->DetourJump(_impl, a_dst) : 0;
			}

			template <class F>
			std::uintptr_t WriteJump(F a_dst) const noexcept
			{
				return WriteJump(*reinterpret_cast<std::uintptr_t*>(&a_dst));
			}

			std::uintptr_t WriteCall(std::uintptr_t a_dst) const noexcept
			{
				return (_impl && s_iface) ? s_iface->DetourCall(_impl, a_dst) : 0;
			}

			template <class F>
			std::uintptr_t WriteCall(F a_dst) const noexcept
			{
				return WriteCall(*reinterpret_cast<std::uintptr_t*>(&a_dst));
			}
		};
	}
}
