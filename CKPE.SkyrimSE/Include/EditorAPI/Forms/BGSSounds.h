// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/BSTArray.h>
#include <EditorAPI/TESFullName.h>
#include <CKPE.Float16.h>
#include "TESForm.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundCategory;
				class BGSSoundDescriptorForm;

				class BSISoundCategory
				{
					enum : std::uint32_t
					{
						scfMuteWhenSubmerged = 1 << 0,
						scfShouldAppearOnMenu = 1 << 1,
					};

					std::uint32_t _flags;
					std::uint32_t _pad0C;
					BGSSoundCategory* _parent;
					std::uint32_t _pad18;
					float16_t _static_volume_multiplier;
					float16_t _default_menu_volume;
					float _unk_v[2];					// sets 1.0
					char _pad28[0x18];
				public:
					virtual ~BSISoundCategory() = default;

					[[nodiscard]] inline bool HasMuteWhenSubmerged() const noexcept(true) 
					{ return (_flags & scfMuteWhenSubmerged) == scfMuteWhenSubmerged; }
					[[nodiscard]] inline bool HasShouldAppearOnMenu() const noexcept(true)
					{ return (_flags & scfShouldAppearOnMenu) == scfShouldAppearOnMenu; }
					[[nodiscard]] inline BGSSoundCategory* GetParent() const noexcept(true) { return _parent; }
					[[nodiscard]] inline float16_t GetStaticVolumeMultiplier() const noexcept(true) 
					{ return _static_volume_multiplier; }
					[[nodiscard]] inline float16_t GetDefaultMenuVolume() const noexcept(true)
					{ return _default_menu_volume; }
				};
				static_assert(sizeof(BSISoundCategory) == 0x40);

				/*`class BSISoundCategory`: VTable[0x0000000003018530, 0x0000000000000000 offset, 12 functions] `. ? AVBSISoundCategory@@`
					`class BGSSoundCategory`: VTable[0x00000000030185B0, 0x0000000000000000 offset, 101 functions] `. ? AVBGSSoundCategory@@`
					#1: flags(40) `class TESForm`:`. ? AVTESForm@@`
					#0: flags(42) `class BaseFormComponent`:`. ? AVBaseFormComponent@@`
					#1: flags(40) `class TESFullName`:`. ? AVTESFullName@@`
					#0: flags(42) `class BaseFormComponent`:`. ? AVBaseFormComponent@@`
					#0: flags(40) `class BSISoundCategory`:`. ? AVBSISoundCategory@@`*/
				class BGSSoundCategory : public TESForm, public TESFullName, public BSISoundCategory
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftSoundCategory;

					virtual ~BGSSoundCategory() = default;
				};
				static_assert(sizeof(BGSSoundCategory) == 0x80);

				class BSISoundOutputModel
				{
				public:
					virtual ~BSISoundOutputModel() = default;

					// In RTTI: BGSSoundOutput::DynamicAttenuationCharacteristics but use only in class BSISoundOutputModel.
					// Violations in inheritance, Idk how to explain to the compiler so that it knows about 
					// the structure of the class before it is implemented in both classes.
					struct DynamicAttenuationCharacteristics
					{
						float MinDistance;
						float MaxDistance;
						std::uint8_t Curve[5];
					};

					// https://en.wikipedia.org/wiki/Surround_sound
					struct InputChannelParam
					{
						std::uint8_t FrontLeft;				// L
						std::uint8_t FrontRight;			// R
						std::uint8_t FrontCenter;			// C
						std::uint8_t LowFrequencyEffects;	// LFE
						std::uint8_t RearLeft;				// RL
						std::uint8_t RearRight;				// RR
						std::uint8_t BackLeft;				// BL
						std::uint8_t BackRight;				// BR
					};

					struct InputChannels
					{
						InputChannelParam Mono;
						InputChannelParam Stereo[2];
					};

					[[nodiscard]] inline bool HasUsesHRTF() const noexcept(true)
					{ return _Mode == sommUsesHRTF; }
					[[nodiscard]] inline bool HasDefinedSpeakerOutput() const noexcept(true)
					{ return _Mode == sommDefinedSpeakerOutput; }
					[[nodiscard]] inline bool HasAttenuatesWithDistance() const noexcept(true)
					{ return (_MainParms._Flags & somfAttenuatesWithDistance) == somfAttenuatesWithDistance; }
					[[nodiscard]] inline bool HasAllowsRumble() const noexcept(true)
					{ return (_MainParms._Flags & somfAllowsRumble) == somfAllowsRumble; }
					[[nodiscard]] inline bool GetReverbSendProcent() const noexcept(true)
					{ return _MainParms._ReverbSend; }
					[[nodiscard]] inline InputChannels* GetInputChannels() const noexcept(true)
					{ return _InputChannels; }
					[[nodiscard]] inline DynamicAttenuationCharacteristics* GetDynamicAttenuationCharacteristics() const noexcept(true)
					{ return _DynamicAttenuationCharacteristics; }
				private:
					enum : std::uint32_t
					{
						somfAttenuatesWithDistance = 1 << 0,
						somfAllowsRumble = 1 << 1,
					};

					enum Mode : std::uint32_t
					{
						sommUsesHRTF = 0,
						sommDefinedSpeakerOutput,
					};

					struct MainParms 
					{
						std::uint32_t _Flags : 24;
						std::uint32_t _ReverbSend : 8;
					};
					MainParms _MainParms;
					Mode _Mode;
					DynamicAttenuationCharacteristics* _DynamicAttenuationCharacteristics;
					InputChannels* _InputChannels;
				};
				
				static_assert(sizeof(BSISoundOutputModel) == 0x20);

				/*`class BSISoundOutputModel`: VTable[0x0000000003019DB8, 0x0000000000000000 offset, 10 functions] `. ? AVBSISoundOutputModel@@`
				`class BGSSoundOutput`: VTable[0x0000000003019E58, 0x0000000000000000 offset, 101 functions] `. ? AVBGSSoundOutput@@`
					#1: flags(40) `class TESForm`:`. ? AVTESForm@@`
					#0: flags(40) `class BaseFormComponent`:`. ? AVBaseFormComponent@@`
					#0: flags(40) `class BSISoundOutputModel`:`. ? AVBSISoundOutputModel@@`*/
				class BGSSoundOutput : public TESForm, public BSISoundOutputModel
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftSoundOutput;

					virtual ~BGSSoundOutput() = default;
				};
				static_assert(sizeof(BGSSoundOutput) == 0x48);

				// size 0x78
				// func 27
				class BGSStandardSoundDef
				{
				public:
					struct SoundPlaybackCharacteristics
					{
						virtual ~SoundPlaybackCharacteristics() = default;

						char pad08[0x10];
					};

					virtual ~BGSStandardSoundDef() = default;

					[[nodiscard]] inline BGSSoundCategory* GetSoundCategory() const noexcept(true) { return _SoundCategory; }
					[[nodiscard]] inline BGSSoundOutput* GetSoundOutput() const noexcept(true) { return _SoundOutput; }
					[[nodiscard]] inline BGSSoundDescriptorForm* GetDescriptor() const noexcept(true) { return _Descriptor; }
					[[nodiscard]] inline std::uint32_t GetAudioCount() const noexcept(true) { return _SoundPaths.size(); }
					[[nodiscard]] inline const char* GetAudioPathByAt(std::uint32_t idx) const noexcept(true) { return _SoundPaths[idx]; }
				private:
					BGSSoundCategory* _SoundCategory;
					char pad10[0x8];
					BSTArray<const char*> _SoundPaths;
					BSTArray<void*> _SoundCaches;
					SoundPlaybackCharacteristics _SoundPlaybackCharacteristics;
					BGSSoundOutput* _SoundOutput;
					char pad68[0x8];
					BGSSoundDescriptorForm* _Descriptor;
					char pad78[0x8];
				};
				static_assert(sizeof(BGSStandardSoundDef::SoundPlaybackCharacteristics) == 0x18);
				static_assert(sizeof(BGSStandardSoundDef) == 0x80);
	
				// func 3
				class BSISoundDescriptor
				{
					std::int64_t _unk;
					BGSStandardSoundDef* _sound;
				public:
					virtual ~BSISoundDescriptor() = default;

					[[nodiscard]] inline BGSStandardSoundDef* GetSound() const noexcept(true) { return _sound; }
				};
				static_assert(sizeof(BSISoundDescriptor) == 0x18);

				// size 0x40
				// func 101
				//`class BGSSoundDescriptorForm`: VTable[0x0000000003019478, 0x0000000000000000 offset, 102 functions] `. ? AVBGSSoundDescriptorForm@@`
				//	#1: flags(40) `class TESForm`:`. ? AVTESForm@@`
				//	#0: flags(40) `class BaseFormComponent`:`. ? AVBaseFormComponent@@`
				//	#0: flags(40) `class BSISoundDescriptor`:`. ? AVBSISoundDescriptor@@`
				class BGSSoundDescriptorForm : public TESForm, public BSISoundDescriptor
				{
				public:
					constexpr static uint8_t TYPE_ID = ftSoundDescriptor;
				public:
					virtual ~BGSSoundDescriptorForm() = default;
				};
				static_assert(sizeof(BGSSoundDescriptorForm) == 0x40);
			}
		}
	}
}