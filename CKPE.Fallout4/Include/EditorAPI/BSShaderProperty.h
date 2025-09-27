// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiProperty.h"
#include "NiAPI/NiPointer.h"
#include "NiAPI/NiFlags.h"
#include "BSFadeNode.h"
#include "BSShaderMaterial.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			//enum : UInt64
				//{
				//	kShaderFlags_GrayscaleToPalette = 0x0000000080000000ULL,
				//	kShaderFlags_Hair = 0x0000400000000000ULL,
				//	kShaderFlags_Refraction = 0x0000000000008000ULL,
				//	kShaderFlags_EmitColor = 0x0000000000400000ULL,
				//	kShaderFlags_NoExternalEmit = 0x4000000000000000ULL
				//};8180400603

			// 70
			class BSShaderProperty : public NiAPI::NiShadeProperty
			{
			public:
				inline std::uint64_t GetFlags() const noexcept(true) { return _Flags.Flags; }
				inline void SetFlags(std::uint64_t v) noexcept(true) { _Flags.Flags = v; }
				inline NiAPI::NiPointer<BSFadeNode> GetFadeNode() const noexcept(true) { return _FadeNode; }
				inline BSShaderMaterial* GetShaderMaterial() const noexcept(true) { return _ShaderMaterial; }
			private:
				float												_Alpha;				// 28
				std::int32_t										_Unk2C;				// 2C
				NiAPI::NiTFlags<std::uint64_t, BSShaderProperty>	_Flags;				// 30
				char												_Pad38[0x10];		// 38
				NiAPI::NiPointer<BSFadeNode>						_FadeNode;			// 48
				void*												_Effects;			// 50 maybe BSEffectShaderMaterial
				BSShaderMaterial*									_ShaderMaterial;	// 58
				std::uint32_t										_LastAccumTime;		// 60
				float												_LODFade;			// 64
				char												_Pad68[8];			// 68
			};
			static_assert(sizeof(BSShaderProperty) == 0x70);
		}
	}
}
