// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.RelocatorDB.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API Patch
		{
			std::string* _name{ nullptr };
			bool _active{ false };

			Patch(const Patch&) = delete;
			Patch& operator=(const Patch&) = delete;
		protected:
			virtual bool DoActive(RelocatorDB::PatchDB* db) noexcept(true) = 0;
			virtual bool DoQuery() const noexcept(true) = 0;
			virtual void SetName(const char* name) noexcept(true);
			virtual void SetName(const std::string& name) noexcept(true);
		public:
			Patch() noexcept(true);
			virtual ~Patch() noexcept(true);

			[[nodiscard]] virtual bool Active(RelocatorDB::PatchDB* db) noexcept(true);
			[[nodiscard]] virtual bool Query() const noexcept(true);

			[[nodiscard]] virtual std::string GetName() const noexcept(true);
			[[nodiscard]] virtual bool IsActive() const noexcept(true);

			virtual bool HasOption() const noexcept(true) = 0;
			virtual const char* GetOptionName() const noexcept(true) = 0;
			virtual bool HasDependencies() const noexcept(true) = 0;
			virtual std::vector<std::string> GetDependencies() const noexcept(true) = 0;
		};
	}
}