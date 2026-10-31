// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// The render window manager (global): owns the cell grid shown in the render window.
			class RenderWindowManager
			{
			public:
				// Culls (true) or shows (false) the "SoundMarker Node" of every loaded cell. Only the hide branch of the Show Markers handler calls it.
				// CK 1.11.246.0 0x1408426B0.
				inline void SetSoundMarkerNodeCulled(bool Culled) noexcept(true)
				{
					using func_t = decltype(&RenderWindowManager::SetSoundMarkerNodeCulled);
					static Common::Relocation<func_t> func{ Common::ID{ 506883 } };
					return func(this, Culled);
				}
			};
		}
	}
}
