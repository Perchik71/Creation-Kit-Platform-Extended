// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectREFR;
				class BGSOpenCloseForm
				{
				public:
					virtual ~BGSOpenCloseForm();

					virtual void HandleOpenStart(TESObjectREFR* target, TESObjectREFR* activator, bool arg3);
					virtual void HandleCloseStart(TESObjectREFR* target, TESObjectREFR* activator, bool arg3);
					virtual void HandleOpenFinish(TESObjectREFR* target, TESObjectREFR* activator);
					virtual void HandleCloseFinish(TESObjectREFR* target, TESObjectREFR* activator);
				};
				static_assert(sizeof(BGSOpenCloseForm) == 0x8);
			}
		}
	}
}