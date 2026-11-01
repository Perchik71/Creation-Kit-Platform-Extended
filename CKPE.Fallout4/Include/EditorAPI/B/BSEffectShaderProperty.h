// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShaderProperty.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSEffectShaderProperty : public BSShaderProperty
			{
			public:
				/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
				/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
				/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
				/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
				/* 22 */ virtual void PostLinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
				void* Unk70;										// 70
				NiAPI::NiColor* ExternalEmittance;					// 78
				std::uint32_t Unk80;									// 80
				std::uint32_t Pad84;									// 84
			};
			static_assert(sizeof(BSEffectShaderProperty) == 0x88);
			static_assert(offsetof(BSEffectShaderProperty, ExternalEmittance) == 0x78);
		}
	}
}
