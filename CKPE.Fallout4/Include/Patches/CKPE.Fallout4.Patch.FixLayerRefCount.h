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
			// The Layers window "#" column grows every time a primitive is moved: TESObjectREFR::Set3D
			// replaces the 3D of a reference that already has one and still sends the "3D loaded" event,
			// which the Layers window counts as one more reference in the layer. The replace path must not
			// send that event.
			class FixLayerRefCount : public Common::Patch
			{
				FixLayerRefCount(const FixLayerRefCount&) = delete;
				FixLayerRefCount& operator=(const FixLayerRefCount&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixLayerRefCount();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				[[nodiscard]] bool SupportsAddressLibrary() const noexcept(true) override { return true; }
			};
		}
	}
}
