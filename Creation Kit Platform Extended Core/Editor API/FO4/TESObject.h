//////////////////////////////////////////
/*
* Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 4)

#include "TESFormF4.h"
#include "TESActorValue.h"
#include "TESObjectCELL.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESObject : public TESForm {
			private:
				/*028*/ CHAR pad28[8];
			public:
				virtual ~TESObject(VOID) = default;
			};

			// 0x90
			class TESBoundObject : public TESObject, public TESCellUseList {
			public:
				struct TESBound { WORD x, y, z; };
			private:
				/*040*/ TESBound Bound[2];
				/*04C*/ CHAR pad4C[4];
				/*050*/ BGSMod::Template::Items _templateItems;	// ???
				/*070*/ BGSPreviewTransform _PreviewTransform;
				/*080*/ BGSSoundTagComponent _SoundTag;
			public:
				inline TESBound GetBound1() const { return Bound[0]; }
				inline TESBound GetBound2() const { return Bound[1]; }
				inline const BGSPreviewTransform* GetPreviewTransform() const { return &_PreviewTransform; }
				inline const BGSSoundTagComponent* GetSoundTagComponent() const { return &_SoundTag; }
			public:
				READ_PROPERTY(GetBound1) TESBound Bound1;
				READ_PROPERTY(GetBound2) TESBound Bound2;
				READ_PROPERTY(GetPreviewTransform) const BGSPreviewTransform* PreviewTransform;
				READ_PROPERTY(GetSoundTagComponent) const BGSSoundTagComponent* SoundTagComponent;
			};
			static_assert(sizeof(TESBoundObject) == 0x90);

			// 0x90
			class TESBoundAnimObject : public TESBoundObject {
			public:
			};
			static_assert(sizeof(TESBoundAnimObject) == 0x90);

			// 0x90
			class TESBoundTreeObject : public TESBoundObject {
			public:
			};
			static_assert(sizeof(TESBoundTreeObject) == 0x90);

			// 0x40
			class TESObjectStaticInfo {
			public:
				struct DirectionalMaterialT {
					/*00*/ TESForm* DirectionalMaterialForm;
					/*08*/ float MaxAngle;
				};
				struct FlutterDataT {
					/*00*/ float LeafAmplitude;
					/*04*/ float LeafFrequency;
				};
			private:
				/*000*/ TESActorValueArray arrActorValueList;
				/*010*/ TESForcedLocRef _ForcedLocRef;
				/*020*/ CHAR __pad20[8];
			public:
				/*028*/ DirectionalMaterialT DirectionalMaterial;
				/*034*/ FlutterDataT FlutterData;
			public:
				inline const BSTArray<TESActorValueArray::ActorValueInfoT>* GetActorValues(VOID) const { return arrActorValueList.Array; }
				inline const TESForcedLocRef* GetForcedLocRef(VOID) const { return &_ForcedLocRef; }
			public:
				READ_PROPERTY(GetActorValues) const BSTArray<TESActorValueArray::ActorValueInfoT>* ActorValues;
				READ_PROPERTY(GetForcedLocRef) const TESForcedLocRef* ForcedLocRef;
			};
			static_assert(sizeof(TESObjectStaticInfo) == 0x3C);
		}
	}
}

#pragma pack(pop)
