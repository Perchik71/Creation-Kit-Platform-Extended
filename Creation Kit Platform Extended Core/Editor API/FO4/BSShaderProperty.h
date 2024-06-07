// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiProperty.h"
#include "NiAPI/NiPointer.h"
#include "BSFadeNode.h"
#include "BSShaderMaterial.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace NiAPI::Fallout4;

			//enum : UInt64
				//{
				//	kShaderFlags_GrayscaleToPalette = 0x0000000080000000ULL,
				//	kShaderFlags_Hair = 0x0000400000000000ULL,
				//	kShaderFlags_Refraction = 0x0000000000008000ULL,
				//	kShaderFlags_EmitColor = 0x0000000000400000ULL,
				//	kShaderFlags_NoExternalEmit = 0x4000000000000000ULL
				//};8180400603

			// 70
			class BSShaderProperty : public NiShadeProperty
			{
			public:
				inline uint64_t GetFlags() { return _Flags; }
				inline void SetFlags(uint64_t v) { _Flags = v; }
				inline NiAPI::NiPointer<BSFadeNode> GetFadeNode() const { return _FadeNode; }
				inline BSShaderMaterial* GetShaderMaterial() const { return _ShaderMaterial; }
			private:
				float						 _Alpha;			// 28
				int32_t						 _Unk2C;			// 2C
				uint64_t					 _Flags;			// 30
				char						 _Pad38[0x10];		// 38
				NiAPI::NiPointer<BSFadeNode> _FadeNode;			// 48
				void*						 _Effects;			// 50 maybe BSEffectShaderMaterial
				BSShaderMaterial*			 _ShaderMaterial;	// 58
				uint32_t					 _LastAccumTime;	// 60
				float						 _LODFade;			// 64
				char						 _Pad68[8];			// 68
			};
			static_assert(sizeof(BSShaderProperty) == 0x70);
		}
	}
}
