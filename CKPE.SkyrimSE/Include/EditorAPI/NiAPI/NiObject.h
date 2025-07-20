// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/NiAPI/NiRTTI.h>
#include <EditorAPI/NiAPI/NiRefObject.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiStream;
				class NiCloningProcess;

				namespace Templates
				{
					// 10
					template<
						typename T,
						std::uint32_t REL_RTTI,
						std::uint32_t REL_CREATECLONE,
						std::uint32_t REL_LOADFROMSTREAM,
						std::uint32_t REL_SAVETOSTREAM,
						std::uint32_t REL_LINKOBJECT,
						std::uint32_t REL_REGSTREAM>
					class NiObjectT : public T
					{
					public:
						// It's everywhere
						inline NiRTTI* GetRTTI() const noexcept(true) { return vtbl_call<NiRTTI*>(REL_RTTI, this); }

						// Creation of a clone object is mentioned in SKSE and F4SE, however, in SKSE it is specified without a refs, I think this is a mistake
						inline NiObject* CreateClone(NiCloningProcess* Process) const noexcept(true) { return vtbl_call<NiObject*>(REL_CREATECLONE, this, Process); }

						// There is a mention of these functions in SKSE and F4SE, actually this is no exception for CK
						inline void LoadFromStream(NiStream* Stream) const noexcept(true) { vtbl_call<void>(REL_LOADFROMSTREAM, this, Stream); }
						inline void LinkObject(NiStream* Stream) const noexcept(true) { vtbl_call<void>(REL_LINKOBJECT, this, Stream); }
						inline bool RegisterStreamables(NiStream* Stream) const noexcept(true) { return vtbl_call<bool>(REL_REGSTREAM, this, Stream); }
						inline void SaveToStream(NiStream* Stream) const noexcept(true) { vtbl_call<void>(REL_SAVETOSTREAM, this, Stream); }

						inline void GetViewerRTTI(void(*Callback)(const char*, ...), bool Fully = false) const noexcept(true)
						{
							auto Info = GetRTTI();

							if (Fully)
								for (auto iter = this->GetRTTI()->GetParent(); iter; iter = iter->GetParent())
									Callback("---- %s ----\n", iter->GetNameClass());

							Callback("-- %s --\n", Info->GetNameClass());
						}
					};
				}

				class NiObject : public Templates::NiObjectT<NiRefObject64, 0x10, 0xB8, 0xC0, 0xD8, 0xC8, 0xD0>
				{};
				static_assert(sizeof(NiObject) == 0x10);
			}
		}
	}
}