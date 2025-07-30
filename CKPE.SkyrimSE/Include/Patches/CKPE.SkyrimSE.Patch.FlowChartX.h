// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class FlowChartX : public Common::Patch
			{
				FlowChartX(const FlowChartX&) = delete;
				FlowChartX& operator=(const FlowChartX&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FlowChartX();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void sub() noexcept(true);
				static std::int64_t HKCoGetClassObject(void* rclsid, std::uint32_t dwClsContext, void* pvReserved,
					void* riid, void** ppv) noexcept(true);
			};
		}
	}
}