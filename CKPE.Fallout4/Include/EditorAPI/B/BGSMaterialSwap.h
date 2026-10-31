// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSString.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 112 (0x70)
				// func = 124
				class BGSMaterialSwap : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftMaterialSwap;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMaterialSwap[0];

					class Object
					{
					public:
						[[nodiscard]] inline const char* GetOriginalMaterialPath() const noexcept(true) { return _OriginalMaterial; }
						[[nodiscard]] inline const char* GetReplacementMaterialPath() const noexcept(true) { return _ReplacementMaterial; }
						[[nodiscard]] inline float GetRemapIndex() const noexcept(true) { return fRemapIndex; }
						[[nodiscard]] inline bool HasRemapIndex() const noexcept(true)
						{
							// 0x7F7FFFFF - magic hex number. It's not NaN nor infiniti, I don't understand what it is.
							return uRemapIndex == 0x7F7FFFFF;
						}
					private:
						BSFixedString _OriginalMaterial;
						BSFixedString _ReplacementMaterial;
						union
						{
							float fRemapIndex;
							std::uint32_t uRemapIndex;
						};
						char pad14[0xC];
					};
	
					virtual ~BGSMaterialSwap() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2E8 */ void OnFormIDsRemapped(BSTHashMap<std::uint32_t, std::uint32_t>* FormIDMap) noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;
			
					[[nodiscard]] inline std::uint32_t GetMaxCount() const noexcept(true) { return _MaxCount; }
					[[nodiscard]] inline std::uint32_t GetFreeCount() const noexcept(true) { return _Free; }
					[[nodiscard]] inline std::uint32_t GetCount() const noexcept(true) { return _MaxCount - _Free; }
					[[nodiscard]] inline const char* GetTreeFolder() const noexcept(true) { return _TreeFolder.c_str(); }
					[[nodiscard]] inline Object* At(std::uint32_t index) noexcept(true)
					{
						if (GetCount() <= index) return nullptr;

						std::uint32_t curInd = 0;
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

					inline Object* AtByPath(const char* path) noexcept(true)
					{
						if (!path) return nullptr;

						auto EndAddr = _Items + _MaxCount;
						auto StartAddr = _Items;
						do
						{
							if (StartAddr->GetOriginalMaterialPath())
							{
								if (!_stricmp(path, StartAddr->GetOriginalMaterialPath()))
									return StartAddr;
							}

							StartAddr++;
						} while (EndAddr > StartAddr);

						return nullptr;
					}
				public:
					CKPE_READ_PROPERTY(GetMaxCount) std::uint32_t MaxCount;
					CKPE_READ_PROPERTY(GetFreeCount) std::uint32_t FreeCount;
					CKPE_READ_PROPERTY(GetCount) std::uint32_t Count;
					CKPE_READ_PROPERTY(GetTreeFolder) const char* TreeFolder;
				private:
					char pad28[0xC];
					std::uint32_t _MaxCount;			// always 8 (at least for 1 to 7 positions, it is possible that it is a multiple of 8)
					std::uint32_t _Free;				// free seats
					char pad48[0x1C];
					Object* _Items;
					BSString _TreeFolder;
				};
				static_assert(sizeof(BGSMaterialSwap) == 0x70);
				static_assert(sizeof(BGSMaterialSwap::Object) == 0x20);
			}
		}
	}
}