// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiLight.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiPointLight : public NiLight
				{
					float _ConstAttenuation;		// 0x170
					float _LinearAttenuation;		// 0x174
					float _QuadraticAttenuation;	// 0x178
					float _Unk[5];
				public:
					virtual ~NiPointLight() = default;
					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true) override;
					/* 1A */ virtual NiObject* CreateClone(NiCloningProcess& Process) noexcept(true) override;
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true) override;
					/* 1C */ virtual void LinkObject(NiStream& Stream) noexcept(true) override;
					/* 1D */ virtual bool RegisterStreamables(NiStream& Stream) noexcept(true) override;
					/* 1E */ virtual void SaveBinary(NiStream& Stream) noexcept(true) override;
					/* 1F */ virtual bool IsEqual(NiObject* Object) noexcept(true) override;
					/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;

					[[nodiscard]] inline float GetConstAttenuation() const noexcept(true) { return _ConstAttenuation; }
					[[nodiscard]] inline float GetLinearAttenuation() const noexcept(true) { return _LinearAttenuation; }
					[[nodiscard]] inline float GetQuadraticAttenuation() const noexcept(true) { return _QuadraticAttenuation; }
				};
				static_assert(sizeof(NiPointLight) == 0x190);
			}
		}
	}
}
