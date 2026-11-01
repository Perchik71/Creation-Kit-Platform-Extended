// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiObjectNET.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkExtraData : public NiAPI::NiExtraData
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
		/* 21 */ virtual void ProcessClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
		/* 0x018 */ std::uint8_t unk018[0x18];
	};
	static_assert(sizeof(bhkExtraData) == 0x30);
}
