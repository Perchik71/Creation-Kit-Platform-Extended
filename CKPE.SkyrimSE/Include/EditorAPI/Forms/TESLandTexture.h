// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <EditorAPI/BSTList.h>
#include "BGSMaterialType.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
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

					inline BGSTextureSet* GetTexture() const noexcept(true) { return _TextureSet; }
					inline std::uint8_t GetFriction() const noexcept(true) { return _Friction; }
					inline void SetFriction(std::uint8_t v) noexcept(true) { _Friction = v; }
					inline std::uint8_t GetRestitution() const noexcept(true) { return _Restitution; }
					inline void SetRestitution(std::uint8_t v) noexcept(true) { _Restitution = v; }
					inline BGSMaterialType* GetMaterialType() const noexcept(true) { return _MaterialType; }
					inline std::uint8_t GetTextureSpecularExponent() const noexcept(true) { return _TextureSpecularExponent; }
					inline void SetTextureSpecularExponent(std::uint8_t v) noexcept(true) { _TextureSpecularExponent = v; }
					inline bool HasSnow() const noexcept(true) { return _HasSnow; }
					inline void SetSnowVisible(bool v) noexcept(true) { _HasSnow = v; }
					inline TESGrass* GetGrass() const noexcept(true) { return _Grass; }
					inline BSSimpleList<TESGrass>* GetAdditionalGrassList() const noexcept(true) { return _AdditionalGrassList; }

					CKPE_READ_PROPERTY(GetTexture) BGSTextureSet* Texture;
					CKPE_PROPERTY(GetFriction, SetFriction) std::uint8_t Friction;
					CKPE_PROPERTY(GetRestitution, SetRestitution) std::uint8_t Restitution;
					CKPE_READ_PROPERTY(GetMaterialType) BGSMaterialType* MaterialType;
					CKPE_PROPERTY(GetTextureSpecularExponent, SetTextureSpecularExponent) std::uint8_t TextureSpecularExponent;
					CKPE_PROPERTY(HasSnow, SetSnowVisible) bool Snow;
					CKPE_READ_PROPERTY(GetGrass) TESGrass* Grass;
					CKPE_READ_PROPERTY(GetAdditionalGrassList) BSSimpleList<TESGrass>* AdditionalGrassList;
				private:
					BGSTextureSet* _TextureSet;
					std::uint8_t _Friction;
					std::uint8_t _Restitution;
					char pad32[0x6];
					BGSMaterialType* _MaterialType;
					std::uint8_t _TextureSpecularExponent;
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
}