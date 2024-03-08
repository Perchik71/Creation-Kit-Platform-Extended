// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESFormF4.h"
#include "BSEntryString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 112 (0x70)
			// func = 124
			class BGSMaterialSwap : public TESForm
			{
			public: 
				class Object
				{
				public:
					inline const char* GetOriginalMaterialPath() const 
					{ 
						return OriginalMaterial ? OriginalMaterial->Get<char>(true) : nullptr;
					}
					inline const char* GetReplacementMaterialPath() const
					{
						return ReplacementMaterial ? ReplacementMaterial->Get<char>(true) : nullptr;
					}
					inline float GetRemapIndex() const { return fRemapIndex; }
					inline bool HasRemapIndex() const 
					{ 
						// 0x7F7FFFFF - magic hex number. It's not NaN nor infiniti, I don't understand what it is.
						return uRemapIndex == 0x7F7FFFFF; 
					}
				private:
					BSEntryString* OriginalMaterial;
					BSEntryString* ReplacementMaterial;
					union
					{
						float fRemapIndex;
						uint32_t uRemapIndex;
					};
					char pad14[0xC];
				};
			public:
				virtual ~BGSMaterialSwap() = default;
			public:
				inline uint32_t GetMaxCount() const { return _MaxCount; }
				inline uint32_t GetFreeCount() const { return _Free; }
				inline uint32_t GetCount() const { return _MaxCount - _Free; }
				inline const char* GetTreeFolder() const { return _TreeFolder.c_str(); }
				inline Object* At(uint32_t index)
				{
					if (GetCount() <= index) return nullptr;
					
					uint32_t curInd = 0;
					auto EndAddr = _Items + _MaxCount;
					auto StartAddr = _Items;
					do
					{
						if (StartAddr->GetOriginalMaterialPath())
						{
							if (curInd == index)
								return StartAddr;
							else
								curInd++;
						}

						StartAddr++;
					} while (EndAddr > StartAddr);

					return nullptr;
				}
				inline Object* AtByPath(const char* path)
				{
					if (!path) return nullptr;

					auto EndAddr = _Items + _MaxCount;
					auto StartAddr = _Items;
					do
					{
						if (StartAddr->GetOriginalMaterialPath())
						{
							if (!stricmp(path, StartAddr->GetOriginalMaterialPath()))
								return StartAddr;
						}

						StartAddr++;
					} while (EndAddr > StartAddr);

					return nullptr;
				}
			public:
				READ_PROPERTY(GetMaxCount) uint32_t MaxCount;
				READ_PROPERTY(GetFreeCount) uint32_t FreeCount;
				READ_PROPERTY(GetCount) uint32_t Count;
				READ_PROPERTY(GetTreeFolder) const char* TreeFolder;
			private:
				char pad28[0xC];
				uint32_t _MaxCount;			// always 8 (at least for 1 to 7 positions, it is possible that it is a multiple of 8)
				uint32_t _Free;				// free seats
				char pad48[0x1C];
				Object* _Items;
				BSString _TreeFolder;
			};
			static_assert(sizeof(BGSMaterialSwap) == 0x70);
			static_assert(sizeof(BGSMaterialSwap::Object) == 0x20);
		}
	}
}