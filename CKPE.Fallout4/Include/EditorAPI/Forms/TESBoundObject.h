// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/Forms/TESObject.h>
#include <EditorAPI/Forms/TESCellUseList.h>
#include <EditorAPI/BGSMod.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 0x90
				class TESBoundObject : public TESObject, public TESCellUseList 
				{
				public:
					//constexpr static std::uint8_t TYPE_ID = ftVoice;			// need knows

					struct TESBound { std::int16_t x, y, z; };
				private:
					/*040*/ TESBound _BoundMin;
					/*046*/ TESBound _BoundMax;
					/*04C*/ char pad4C[4];
					/*050*/ BGSMod::Template::Items _templateItems;	// ???
					/*070*/ BGSPreviewTransform _PreviewTransform;
					/*080*/ BGSSoundTagComponent _SoundTag;
				public:
					[[nodiscard]] inline TESBound GetBoundMin() const noexcept(true) { return _BoundMin; }
					[[nodiscard]] inline TESBound GetBoundMax() const noexcept(true) { return _BoundMax; }

					/*
					Declare TESForm 
					[[nodiscard]] inline const BGSPreviewTransform* GetPreviewTransform() const noexcept(true) { return &_PreviewTransform; }
					[[nodiscard]] inline const BGSSoundTagComponent* GetSoundTagComponent() const noexcept(true) { return &_SoundTag; }*/
				public:
					CKPE_READ_PROPERTY(GetBoundMin) TESBound BoundMin;
					CKPE_READ_PROPERTY(GetBoundMax) TESBound BoundMax;
				};
				static_assert(sizeof(TESBoundObject) == 0x90);

				// 0x90
				class TESBoundAnimObject : public TESBoundObject
				{};
				static_assert(sizeof(TESBoundAnimObject) == 0x90);

				// 0x90
				class TESBoundTreeObject : public TESBoundObject 
				{};
				static_assert(sizeof(TESBoundTreeObject) == 0x90);
			}
		}
	}
}
