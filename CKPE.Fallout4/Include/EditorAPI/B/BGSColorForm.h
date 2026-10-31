// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiFlags.h>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESCondition.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// It has been announced that the RAW color is no longer supported

				// size = 0x68
				// func = 124
				class BGSColorForm : public TESForm, public TESFullName
				{
				public:
					static constexpr std::uint32_t FLAG_PLAYABLE		= (std::uint32_t)1 << 0;
					static constexpr std::uint32_t FLAG_REMAPCOLOR		= (std::uint32_t)1 << 1;
					static constexpr std::uint32_t FLAG_EXTENDED_LUT	= (std::uint32_t)1 << 2;
				public:
					constexpr static std::uint8_t TYPE_ID = ftColorForm;

					virtual ~BGSColorForm() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					inline bool HasPlayable() const noexcept(true) { return _Flags.Has(FLAG_PLAYABLE); }
					inline void SetPlayable(bool v) noexcept(true)
					{
						if (HasPlayable() == v)
							return;

						if (!v)
							_Flags.Set(FLAG_PLAYABLE);
						else
							_Flags.Unset(FLAG_PLAYABLE);
					}
					inline bool HasRemappingIndex() const noexcept(true) { return _Flags.Has(FLAG_REMAPCOLOR); }
					inline void SetRemappingIndex(bool v) noexcept(true)
					{
						if (HasRemappingIndex() == v)
							return;

						if (!v)
							_Flags.Set(FLAG_REMAPCOLOR);
						else
							_Flags.Unset(FLAG_REMAPCOLOR);
					}
					inline bool HasExtendedLUT() const noexcept(true) { return _Flags.Has(FLAG_EXTENDED_LUT); }
					inline void SetExtendedLUT(bool v) noexcept(true)
					{
						if (HasExtendedLUT() == v)
							return;

						if (!v)
							_Flags.Set(FLAG_EXTENDED_LUT);
						else
							_Flags.Unset(FLAG_EXTENDED_LUT);
					}
					inline float GetRemappingIndex() const noexcept(true) { return _RemappingIndex; }
					inline void SetRemappingIndex(float v) noexcept(true)
					{
						SetRemappingIndex(true);
						_RemappingIndex = v;
					}
					inline NiAPI::NiRGBA GetRawColor() const noexcept(true) { return _Raw; }
					inline void SetRawColor(const NiAPI::NiRGBA& Color) noexcept(true)
					{
						SetRemappingIndex(false);
						_RemappingIndex = 0.0f;
						_Raw = Color;
					}
				public:
					CKPE_READ_PROPERTY(GetName) const char* Name;
					CKPE_PROPERTY(GetRemappingIndex, SetRemappingIndex) float RemappingIndex;
					CKPE_PROPERTY(GetRawColor, SetRawColor) NiAPI::NiRGBA RawColor;
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(BGSColorForm, _Conditions) == 0x48 &&
							offsetof(BGSColorForm, _Flags) == 0x60;
					}
				private:
					union
					{
						float _RemappingIndex;
						NiAPI::NiRGBA _Raw;
					};
					char pad44[0x4];
					TESCondition _Conditions;
					NiAPI::NiTFlags<std::uint32_t, BGSColorForm> _Flags;
				};
				static_assert(sizeof(BGSColorForm) == 0x68);
				static_assert(BGSColorForm::VerifyLayout());
			}
		}
	}
}