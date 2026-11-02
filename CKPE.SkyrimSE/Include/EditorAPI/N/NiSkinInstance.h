// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiSkinInstance.h

#pragma once

#include <windows.h>
#include <EditorAPI/N/NiRefObject.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				
				class NiSkinInstance : public NiRefObject64
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::NiSkinInstance;
					constexpr static Common::ID VTABLE = IDs::VTABLE::NiSkinInstance[0];

					class NiSkinData* m_spSkinData;
					void* m_spSkinPartition;
					char _pad0[0x28];
					void* m_pvBoneMatrices;
					void* m_pvPrevBoneMatrices;
					char _pad1[0x8];
					CRITICAL_SECTION m_csLock;

					virtual ~NiSkinInstance() = default;
				};
				static_assert(sizeof(NiSkinInstance) == 0x88);
			}
		}
	}
}