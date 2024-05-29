#pragma once

#include "NiAPI/NiPoint.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::NiAPI;

			class BGSRenderWindow;
			class BGSRenderWindowReferenceEditModule
			{
			public:
				virtual ~BGSRenderWindowReferenceEditModule() = default;

				void MoveSelectObjects(NiPoint3* NewPosition);
				void MoveSelectObjectsExtremly(NiPoint3* NewPosition);
			private:
				BGSRenderWindow* _Renderer;
				char pad10[0x60];
				NiPoint3 _TempPosition;
				char pad7C[0x4];
			};
			static_assert(sizeof(BGSRenderWindowReferenceEditModule) == 0x80);
		}
	}
}