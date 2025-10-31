#include <CKPE.Utils.h>
#include <CKPE.Common.RTTI.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/Forms/TESFullName.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				void TESForm::MarkAsDeleted(bool Value) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x198, this, Value);
				}

				void TESForm::MarkAsChanged(bool Value) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x1A0, this, Value);
				}

				void TESForm::EditFormToWindow(std::int64_t hParentWnd, std::int64_t a1, std::int64_t a2) noexcept(true)
				{
					CKPE::vtbl_call<const char*>(0x340, this, hParentWnd, a1, a2);
				}

				void TESForm::SetNewFormID(std::uint32_t NewIndex, bool Unk) noexcept(true)
				{
					CKPE::vtbl_call<void>(0x398, this, NewIndex, Unk);
				}

				bool TESForm::CanPreview() const noexcept(true)
				{
					return CKPE::vtbl_call<bool>(0x1D0, this);
				}

				void TESForm::DebugInfo(char* Buffer, uint32_t BufferSize) const noexcept(true)
				{
					CKPE::vtbl_call<void>(0x128, this, Buffer, BufferSize);
				}

				const char* TESForm::GetEditorID_orig() const noexcept(true)
				{
					return CKPE::vtbl_call<const char*>(0x238, this);
				}

				const char* TESForm::TryGetFullName() const noexcept(true)
				{
					TESFullName* fullname = (TESFullName*)_DYNAMIC_CAST(this, 0, "class TESForm", "class TESFullName");
					return fullname ? fullname->GetFullName() : "";
				}
			}
		}
	}
}