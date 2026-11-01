// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiRefObject.h>
#include <EditorAPI/N/NiPointer.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSMultiBoundRoom;
			class BSPortalGraph;

			namespace NiAPI
			{
				class NiLight;
				class NiAVObject;
			}

			class BSLight : public NiAPI::NiRefObject
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLight[0];

				enum class Shape : std::uint32_t
				{
					None = 0,
					ScreenQuad = 1,
					Sphere = 2,
					Hemisphere = 3,
					Ovoid = 4,
					Box = 5,
					Cone = 6
				};
			private:
				float _LODDimmer;									// 0x10
				float _Luminance;									// 0x14
				std::uint32_t _Unk18;								// 0x18
				char _pad1C[0x74];									// 0x1C
				NiAPI::NiPoint3 _LightPosition;							// 0x90
				float _unk9C;											// 0x9C
				NiAPI::NiPoint3 _LightBoxPosition;						// 0xA0
				NiAPI::NiPoint3 _Radius;								// 0xAC
				NiAPI::NiPointer<NiAPI::NiLight> _Light;				// 0xB8
				void* _AssociationHead;								// 0xC0
				void* _AssociationTail;								// 0xC8
				std::uint32_t _AssociationCount;						// 0xD0
				std::uint32_t _PadD4;								// 0xD4
				void* _AssociationAllocator;							// 0xD8
				std::uint32_t _AssociationLock[2];					// 0xE0
				std::uint64_t _UnkE8;								// 0xE8
				BSTArray<BSMultiBoundRoom*> _BoundRooms;				// 0xF0
				BSTArray<void*> _Portals;								// 0x108 BSPortal
				BSTArray<void*> _Unk1;									// 0x120
				NiAPI::NiPointer<BSPortalGraph> _PortalGraph;			// 0x138
				std::uint64_t _unk140;									// 0x140
				NiAPI::NiPointer<NiAPI::NiAVObject> _ObjectNode;		// 0x148
				std::uint64_t _Unk150[4];							// 0x150
				bool _PointLight;									// 0x170
				bool _AmbientLight;									// 0x171
				bool _DynamicLight;									// 0x172
				bool _PortalStrict;									// 0x173
				std::uint8_t _Unk174;								// 0x174
				bool _AffectLand;									// 0x175
				bool _AffectWater;									// 0x176
				bool _LODFade;										// 0x177
				bool _Specular;										// 0x178
				bool _AttenuationOnly;								// 0x179
				bool _IgnoreRoughness;								// 0x17A
				bool _IgnoreRim;									// 0x17B
				bool _Occluded;										// 0x17C
				std::uint8_t _Unk17D[3];								// 0x17D
				Shape _Shape;										// 0x180
				std::uint32_t _Unk184;								// 0x184
				std::uint64_t _Unk188;								// 0x188
			public:
				virtual ~BSLight() = default;
				[[nodiscard]] inline Shape GetShape() const noexcept(true) { return _Shape; }
				[[nodiscard]] inline float GetLODDimmer() const noexcept(true) { return _LODDimmer; }
				[[nodiscard]] inline float GetLuminance() const noexcept(true) { return _Luminance; }
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSLight, _LODDimmer) == 0x10 &&
						offsetof(BSLight, _Light) == 0xB8 &&
						offsetof(BSLight, _AssociationHead) == 0xC0 &&
						offsetof(BSLight, _BoundRooms) == 0xF0 &&
						offsetof(BSLight, _PointLight) == 0x170 &&
						offsetof(BSLight, _Shape) == 0x180;
				}
			private:
				// Writes the light's properties (LODDimmer, Luminance, the flags and the shape name) to a property dump
				/* 0x010 */ virtual void DumpProperties(void* Dump) const;
				/* 0x018 */ virtual void SetSourceLight(NiAPI::NiLight* Light);
				// False here; true in the BSShadow*Light classes
				/* 0x020 */ [[nodiscard]] virtual bool IsShadowLight() const;
				// Copies 64 bytes of transform data to Out: this+0x20 here (Index unused), the Index-th 0xF0 byte entry of the shadow light's array in BSShadowLight
				/* 0x028 */ virtual void GetTransformData(std::uint32_t Index, void* Out) const;
			};
			static_assert(sizeof(BSLight) == 0x190);
			static_assert(BSLight::VerifyLayout());
		}
	}
}
