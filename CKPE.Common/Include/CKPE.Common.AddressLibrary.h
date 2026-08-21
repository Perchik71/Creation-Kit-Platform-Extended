// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Version.h>
#include <CKPE.Common.Common.h>
#include <vector>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API AddressLibrary
		{
		public:
			using AddressID = std::uint64_t;
			using VersionID = std::uint64_t;
			using AddressOffset = std::ptrdiff_t;
		private:
			struct Entry
			{
				std::uint64_t Id;
				std::uint64_t Offset;
			};
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
		private:
			std::vector<Entry>* _entries{ nullptr };
			bool _loaded{ false };
			CKPE::Version _version{};

			AddressLibrary(const AddressLibrary&) = delete;
			AddressLibrary& operator=(const AddressLibrary&) = delete;
		public:
			AddressLibrary() noexcept(true);
			virtual ~AddressLibrary() noexcept(true);

			// Returns false (and logs the reason) on any missing/malformed/unsorted file - never throws
			virtual bool Load(const std::wstring& fname) noexcept(true);
			virtual void Clear() noexcept(true);

			[[nodiscard]] constexpr bool IsLoaded() const noexcept(true) { return _loaded; }
			[[nodiscard]] virtual std::uint32_t GetCount() const noexcept(true);

			// Raw RVA for the given stable id, or 0 if unknown / not loaded.
			[[nodiscard]] virtual std::uint64_t ResolveOffset(AddressID id) const noexcept(true);
			// Absolute address (CreationKit.exe module base + RVA), or 0 if unknown / not loaded.
			[[nodiscard]] virtual std::uintptr_t Resolve(AddressID id) const noexcept(true);

			void SetVersion(const CKPE::Version& version) noexcept(true);
			[[nodiscard]] CKPE::Version GetVersion() const noexcept(true);

			[[nodiscard]] virtual std::uint64_t ResolveOffset(const VariantID& id) const noexcept(true);
			[[nodiscard]] virtual std::uintptr_t Resolve(const VariantID& id) const noexcept(true);

			static AddressLibrary* GetSingleton() noexcept(true);
		};
	}
}
