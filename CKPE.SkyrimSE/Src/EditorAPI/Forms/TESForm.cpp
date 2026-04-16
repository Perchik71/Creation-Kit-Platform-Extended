#include <CKPE.Utils.h>
#include <CKPE.Common.RTTI.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/TESFullName.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				const char* TESForm::TryGetFullName() const noexcept(true)
				{
					TESFullName* fullname = (TESFullName*)_DYNAMIC_CAST(this, 0, "class TESForm", "class TESFullName");
					return fullname ? fullname->GetFullName() : "";
				}
			}
		}
	}
}