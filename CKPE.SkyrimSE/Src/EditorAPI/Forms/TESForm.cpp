#include <CKPE.Utils.h>
#include <CKPE.Common.RTTI.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/Forms/TESFullName.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				void TESForm::MarkAsDeleted(bool Value) const noexcept(true)
				{
					vtbl_call<void>(0x160, this, Value);
				}

				void TESForm::MarkAsChanged(bool Value) const noexcept(true)
				{
					vtbl_call<void>(0x168, this, Value);
				}

				void TESForm::EditFormToWindow(std::int64_t hParentWnd, __int64 a1, __int64 a2) noexcept(true)
				{
					vtbl_call<const char*>(0x2D0, this, hParentWnd, a1, a2);
				}

				void TESForm::SetNewFormID(std::uint32_t NewIndex, bool Unk) noexcept(true)
				{
					vtbl_call<char*>(0x310, this, NewIndex, Unk);
				}

				bool TESForm::CanPreview() const noexcept(true)
				{
					return vtbl_call<bool>(0x188, this);
				}

				void TESForm::DebugInfo(char* Buffer, uint32_t BufferSize) const noexcept(true)
				{
					vtbl_call<void>(0xF8, this, Buffer, BufferSize);
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