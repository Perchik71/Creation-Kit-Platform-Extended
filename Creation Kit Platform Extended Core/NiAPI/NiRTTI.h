// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		class NiObject;

		// 10
		class NiRTTI
		{
		public:
			const char* GetNameClass() const { return _Name; }
			NiRTTI* GetParent() const { return _Parent; }
		private:
			const char* _Name;
			NiRTTI* _Parent;
		};

		NiObject* _NI_DYNAMIC_CAST(const NiObject* Src, const char* lpstrFromType);
	}
}