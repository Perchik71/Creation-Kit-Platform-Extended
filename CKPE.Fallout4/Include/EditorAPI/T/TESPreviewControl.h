// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESRenderControl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESPreviewControl : public TESRenderControl
			{
			public:
				/* 0x000 */ ~TESPreviewControl() override;
				/* 0x010 */ void DetachObject(NiAPI::NiAVObject* Object) noexcept(true) override;

			protected:
				/* 0x048 */ void Unk_048() noexcept(true) override;

			public:
				/* 0x050 */ virtual void SetPreviewObject(NiAPI::NiAVObject* Object,
					bool RememberObject) noexcept(true);

			protected:
				/* 0x058 */ virtual void Unk_058() noexcept(true);
				/* 0x060 */ virtual void Unk_060() noexcept(true);

			private:
				/* 0x0B0 */ char padB0[0x30];

			public:
				/* 0x0E0 */ NiAPI::NiPointer<NiAPI::NiNode> PreviewScene;
				/* 0x0E8 */ NiAPI::NiAVObject* PreviewObject;

			private:
				/* 0x0F0 */ char padF0[0x18];
			};
			static_assert(sizeof(TESPreviewControl) == 0x108);
			static_assert(offsetof(TESPreviewControl, PreviewScene) == 0xE0);
			static_assert(offsetof(TESPreviewControl, PreviewObject) == 0xE8);
		}
	}
}
