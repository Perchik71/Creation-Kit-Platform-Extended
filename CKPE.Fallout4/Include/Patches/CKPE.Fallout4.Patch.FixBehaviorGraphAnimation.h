// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			// Statics whose nif carries a BSBehaviorGraphExtraData (e.g. the fire barrels, Autoplay.hkx) never
			// animate in the render window: the reference animation init creates the behavior graph and, when
			// that succeeds, skips TESObjectREFR::InitNonNPCAnimation, the only place that starts the nif's
			// NiControllerManager sequence. The Creation Kit never updates behavior graphs, so nothing else
			// starts it. This patch starts the sequence after the graph was created, for STAT / MSTT references
			// whose controller manager has no running sequence. (Same code in all supported runtimes, 1.10.162.0 to 1.11.246.0.)
			class FixBehaviorGraphAnimation : public Common::Patch
			{
				FixBehaviorGraphAnimation(const FixBehaviorGraphAnimation&) = delete;
				FixBehaviorGraphAnimation& operator=(const FixBehaviorGraphAnimation&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixBehaviorGraphAnimation();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				[[nodiscard]] bool SupportsAddressLibrary() const noexcept(true) override { return true; }
			};
		}
	}
}
