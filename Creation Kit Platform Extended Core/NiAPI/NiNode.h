// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAVObject.h"

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		class NiNode : public NiAVObject 
		{
		public:
			virtual ~NiNode() = default;
		
		};
		static_assert(sizeof(NiNode) == 0x110);

		namespace Fallout4
		{
			class NiNode : public Fallout4::NiAVObject
			{
			public:
				virtual ~NiNode() = default;

			};
			static_assert(sizeof(NiNode) == 0x120);
		}

		namespace Starfield
		{
			class NiNode : public Starfield::NiAVObject
			{
			public:
				virtual ~NiNode() = default;

			};
			static_assert(sizeof(NiNode) == 0x140);
		}
	}
}
