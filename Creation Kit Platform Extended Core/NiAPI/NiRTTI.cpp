#include "NiRTTI.h"
#include "NiObject.h"

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		NiObject* _NI_DYNAMIC_CAST(const NiObject* Src, const char* lpstrFromType)
		{
			if (Src)
				for (auto iter = Src->GetRTTI(); iter; iter = iter->GetParent())
					if (!strcmp(iter->GetNameClass(), lpstrFromType))
						return const_cast<NiObject*>(Src);

			return nullptr;
		}
	}
}