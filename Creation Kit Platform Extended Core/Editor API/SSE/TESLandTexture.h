// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BGSMaterialType.h"
#include "..\BSTList.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESGrass;
			class BGSTextureSet;

			// size 0x70
			// func 101
			class TESLandTexture : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftLandTexture;
				
			public:
				virtual ~TESLandTexture() = default;

				inline BGSTextureSet* GetTexture() const { return _TextureSet; }
				inline uint8_t GetFriction() const { return _Friction; }
				inline void SetFriction(uint8_t v) { _Friction = v; }
				inline uint8_t GetRestitution() const { return _Restitution; }
				inline void SetRestitution(uint8_t v) { _Restitution = v; }
				inline BGSMaterialType* GetMaterialType() const { return _MaterialType; }
				inline uint8_t GetTextureSpecularExponent() const { return _TextureSpecularExponent; }
				inline void SetTextureSpecularExponent(uint8_t v) { _TextureSpecularExponent = v; }
				inline bool HasSnow() const { return _HasSnow; }
				inline void SetSnowVisible(bool v) { _HasSnow = v; }
				inline TESGrass* GetGrass() const { return _Grass; }
				inline BSSimpleList<TESGrass>* GetAdditionalGrassList() const { return _AdditionalGrassList; }

				READ_PROPERTY(GetTexture) BGSTextureSet* Texture;
				PROPERTY(GetFriction, SetFriction) uint8_t Friction;
				PROPERTY(GetRestitution, SetRestitution) uint8_t Restitution;
				READ_PROPERTY(GetMaterialType) BGSMaterialType* MaterialType;
				PROPERTY(GetTextureSpecularExponent, SetTextureSpecularExponent) uint8_t TextureSpecularExponent;
				PROPERTY(HasSnow, SetSnowVisible) bool Snow;
				READ_PROPERTY(GetGrass) TESGrass* Grass;
				READ_PROPERTY(GetAdditionalGrassList) BSSimpleList<TESGrass>* AdditionalGrassList;
			private:
				BGSTextureSet* _TextureSet;
				uint8_t _Friction;
				uint8_t _Restitution;
				char pad32[0x6];
				BGSMaterialType* _MaterialType;
				uint8_t _TextureSpecularExponent;
				char pad41[0x3];
				bool _HasSnow;
				char pad45[0x3];
				TESGrass* _Grass;
				BSSimpleList<TESGrass>* _AdditionalGrassList;
				char pad58[0x18];
			};
			static_assert(sizeof(TESLandTexture) == 0x70);
		}
	}
}

#pragma pack(pop)