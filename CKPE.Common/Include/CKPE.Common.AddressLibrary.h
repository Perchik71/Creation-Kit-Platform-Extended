// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Version.h>
#include <CKPE.Stream.h>
#include <CKPE.Common.Common.h>
#include <vector>

namespace CKPE
{
	namespace Common
	{
		inline constexpr static auto SUPPORT_RUNTIMECOUNT = 4;

		class CKPE_COMMON_API IDDatabase
		{
		public:
			using AddressID = std::uint32_t;
			using VersionID = std::uint32_t;
			using AddressOffset = std::ptrdiff_t;
		private:
			template<typename T>
			requires std::integral<T>
			struct TEntry { T Id, Offset; };
			using Entry_v0 = TEntry<std::uint64_t>;
			using Entry_v1 = TEntry<std::uint32_t>;

			struct Header
			{
				inline static constexpr std::uint32_t MAGICK = 0x434B414C;
				inline static constexpr std::uint32_t CURRENT_VERSION = 0x1;

				std::uint32_t magick{ MAGICK };
				std::uint32_t version{ CURRENT_VERSION };
				std::uint32_t crc32{ (std::uint32_t)-1 };
				std::uint32_t count{ 0 };
			};

			bool Load_v0(Stream& stm, const std::wstring& fname) noexcept(true);
			bool Load_v1(Stream& stm, const Header& hdr, const std::wstring& fname) noexcept(true);

			IDDatabase(const IDDatabase&) = delete;
			IDDatabase(IDDatabase&&) = delete;
			IDDatabase& operator=(const IDDatabase&) = delete;
			IDDatabase& operator=(IDDatabase&&) = delete;
		protected:
			std::vector<Entry_v1>* _entries{ nullptr };
			bool _loaded{ false };
			std::uint8_t _runtime{ 0xFF };
			CKPE::Version _version{};
		public:
			struct VariantID
			{
				struct Variant
				{
					CKPE::Version Version{};
					AddressID ID{ 0 };
					AddressOffset Offset{ 0 };

					constexpr Variant() noexcept = default;
					constexpr Variant(CKPE::Version version, AddressID id, AddressOffset offset = 0) noexcept : Version(version), ID(id), Offset(offset)
					{}
				};

				static constexpr std::size_t MaxVariants = 16;
			private:
				AddressID _defaultID{ 0 };
				AddressOffset _defaultOffset{ 0 };

				std::array<Variant, MaxVariants> _variants{};
				std::size_t _variantCount{ 0 };

			public:
				constexpr VariantID(AddressID id, AddressOffset offset = 0) noexcept : _defaultID(id), _defaultOffset(offset)
				{}

				[[nodiscard]]
				constexpr VariantID For(CKPE::Version version, AddressID id, AddressOffset offset = 0) const noexcept
				{
					VariantID result = *this;

					if (result._variantCount < MaxVariants)
					{
						result._variants[result._variantCount++] =
							Variant{ version, id, offset };
					}

					return result;
				}

				[[nodiscard]]
				constexpr Variant Get(CKPE::Version version) const noexcept
				{
					for (std::size_t i = 0; i < _variantCount; i++)
					{
						if (_variants[i].Version == version)
							return _variants[i];
					}

					return Variant{ version, _defaultID, _defaultOffset };
				}
			};

			IDDatabase();
			virtual ~IDDatabase();

			virtual bool Load(const std::uint8_t a_runtime_index) noexcept(true);
			virtual bool Save(const std::wstring& fname) noexcept(true);
			virtual void Clear() noexcept(true);

			[[nodiscard]] virtual std::uint32_t GetCount() const noexcept(true);

			// Raw RVA for the given stable id, or 0 if unknown / not loaded.
			[[nodiscard]] virtual AddressOffset ResolveOffset(AddressID id) const noexcept(true);
			// Absolute address (CreationKit.exe module base + RVA), or 0 if unknown / not loaded.
			[[nodiscard]] virtual std::uintptr_t Resolve(AddressID id) const noexcept(true);

			[[nodiscard]] virtual AddressOffset ResolveOffset(const VariantID& id) const noexcept(true);
			[[nodiscard]] virtual std::uintptr_t Resolve(const VariantID& id) const noexcept(true);
		};

		class CKPE_COMMON_API AddressLibrary :
			public IDDatabase
		{
			AddressLibrary(const AddressLibrary&) = delete;
			AddressLibrary(AddressLibrary&&) = delete;
			AddressLibrary& operator=(const AddressLibrary&) = delete;
			AddressLibrary& operator=(AddressLibrary&&) = delete;
		public:
			AddressLibrary() noexcept(true) = default;
			virtual ~AddressLibrary() noexcept(true) = default;

			[[nodiscard]] constexpr bool IsLoaded() const noexcept(true) { return _loaded; }
			[[nodiscard]] constexpr uint8_t GetRuntimeIndex() const noexcept(true) { return _runtime; }
			[[nodiscard]] constexpr CKPE::Version GetVersion() const noexcept(true) { return _version; }

			static AddressLibrary* GetSingleton() noexcept(true);
		};

		using IDDB = AddressLibrary;
	}
}
