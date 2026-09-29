#pragma once

#include "TESObject.h"
#include <EditorAPI/BSTList.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				struct NiNPShortPoint3
				{
					// members
					std::int16_t x;  // 0
					std::int16_t y;  // 2
					std::int16_t z;  // 4
				};
				static_assert(sizeof(NiNPShortPoint3) == 0x6);
			}

			namespace Forms
			{
				class Actor;
				class BGSVoiceType;
				class TESObjectCell;

				struct TESCellUseList
				{
					char pad0[8];
					BSSimpleList<TESObjectCell> cells;
				};
				static_assert(sizeof(TESCellUseList) == 0x18);

				class TESBoundObject : public TESObject, public TESCellUseList
				{
				public:
					struct BOUND_DATA  // OBND
					{
						// members
						NiAPI::NiNPShortPoint3 boundMin;  // 0
						NiAPI::NiNPShortPoint3 boundMax;  // 6
					};
					static_assert(sizeof(BOUND_DATA) == 0xC);
				protected:
					// members
					BOUND_DATA		boundData;	// 20 - OBND
					std::uint32_t	pad2C;		// 2C
				public:
					virtual ~TESBoundObject() = default;
	
					[[nodiscard]] inline BOUND_DATA GetBoundData() const noexcept(true) { return boundData; }

					// override (TESObject)
					void LoadObjectBound(TESFile* pFile) override;
					bool IsBoundObject() const noexcept(true) override { return true; }
					NiAPI::NiAVObject* Clone3D(TESObjectREFR* a_ref, bool a_arg3) noexcept(true) override;
					bool ReplaceModel() noexcept(true) override;

					// add
					virtual void SetObjectVoiceType(BGSVoiceType* a_voiceType);													// 73 - { return; }
					[[nodiscard]] virtual BGSVoiceType* GetObjectVoiceType() const;												// 74 - { return 0; }
					virtual NiAPI::NiAVObject* Clone3D(TESObjectREFR* a_ref);													// 75 - { Clone3D(a_ref, false); }
					virtual bool ReplaceModel(const char* a_str);																// 76
					virtual bool GetActivateText(TESObjectREFR* a_activator, BSString& a_dst);									// 77
					virtual bool CalculateDoFavor(Actor* a_activator, bool a_arg2, TESObjectREFR* a_toActivate, float a_arg3);	// 78
					virtual void HandleRemoveItemFromContainer(TESObjectREFR* a_container);										// 79 - { return; }
					virtual void OnRemove3D(NiAPI::NiAVObject* a_obj3D);														// 7A - { return; }
					virtual void OnCheckModels();																				// 7B - { return; }
					virtual void OnCopyReference();																				// 7C - { return; }
					virtual void OnFinishScale();
				};
				static_assert(sizeof(TESBoundObject) == 0x50);
			}
		}
	}
}