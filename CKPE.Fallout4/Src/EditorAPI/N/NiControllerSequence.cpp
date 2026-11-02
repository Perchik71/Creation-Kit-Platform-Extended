// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Relocation.h>
#include <EditorAPI/N/NiControllerSequence.h>
#include <EditorAPI/IDs.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				bool NiControllerSequence::Activate(std::uint8_t Priority, bool StartOver, float Weight, float EaseInTime,
					NiControllerSequence* TimeSync, bool Transition) noexcept(true)
				{
					using Function = bool(*)(NiControllerSequence*, std::uint8_t, bool, float, float, NiControllerSequence*, bool);
					static Common::Relocation<Function> function{ EditorAPI::IDs::Functions::NiControllerSequence__Activate };
					return function(this, Priority, StartOver, Weight, EaseInTime, TimeSync, Transition);
				}
			}
		}
	}
}
