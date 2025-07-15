// Copyright © 2023-2024 aka perchik71. All rights reserved.
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
			class CrashUsingMore16NPCForFaceGen : public Common::Patch
			{
				CrashUsingMore16NPCForFaceGen(const CrashUsingMore16NPCForFaceGen&) = delete;
				CrashUsingMore16NPCForFaceGen& operator=(const CrashUsingMore16NPCForFaceGen&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CrashUsingMore16NPCForFaceGen();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void sub(std::int64_t Texture) noexcept(true);
			};
		}
	}
}