// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/B/BSPortalGraphEntry.h>
#include <EditorAPI/N/NiRTTI.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiCamera;
			}

			class BSCullingProcess
			{
			public:
				enum class CullingType : std::uint32_t
				{
					Normal = 0,
					AllPass = 1,
					AllFail = 2,
					IgnoreMultiBounds = 3,
					ForceMultiBoundsNoUpdate = 4,
				};

				// NiCullingProcess has GetRTTI at slot zero, not a destructor.
				[[nodiscard]] inline NiAPI::NiRTTI* GetRTTI() const noexcept(true)
				{
					return CKPE::vtbl_call<NiAPI::NiRTTI*>(0, this);
				}

				[[nodiscard]] inline BSPortalGraphEntry* GetPortalGraphEntry() const noexcept(true) { return _PortalGraphEntry; }
				[[nodiscard]] inline NiAPI::NiCamera* GetCamera() const noexcept(true) { return _Camera; }
				[[nodiscard]] inline CullingType GetCullingType() const noexcept(true) { return _CullingType; }
				[[nodiscard]] inline bool IsRecursingToGeometry() const noexcept(true) { return _RecurseToGeometry; }
				CKPE_READ_PROPERTY(GetPortalGraphEntry) BSPortalGraphEntry* PortalGraphEntry;
				CKPE_READ_PROPERTY(GetCamera) NiAPI::NiCamera* Camera;
				CKPE_READ_PROPERTY(GetCullingType) CullingType CullMode;

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSCullingProcess, _Camera) == 0x18 &&
						offsetof(BSCullingProcess, _PortalGraphEntry) == 0x158 &&
						offsetof(BSCullingProcess, _CullingType) == 0x160 &&
						offsetof(BSCullingProcess, _TypeStack) == 0x164 &&
						offsetof(BSCullingProcess, _TypeStackIndex) == 0x18C &&
						offsetof(BSCullingProcess, _CompoundFrustum) == 0x190 &&
						offsetof(BSCullingProcess, _Accumulator) == 0x198 &&
						offsetof(BSCullingProcess, _RecurseToGeometry) == 0x1A0;
				}
			private:
				/* 0x000 */ const void* _Vtable;
				/* 0x008 */ bool _UseVirtualAppend;
				/* 0x009 */ char pad009[0x7];
				/* 0x010 */ void* _VisibleSet;
				/* 0x018 */ NiAPI::NiCamera* _Camera;
				/* 0x020 */ char pad020[0x138];
				/* 0x158 */ BSPortalGraphEntry* _PortalGraphEntry;
				/* 0x160 */ CullingType _CullingType;
				/* 0x164 */ CullingType _TypeStack[10];
				/* 0x18C */ std::uint32_t _TypeStackIndex;
				/* 0x190 */ class BSCompoundFrustum* _CompoundFrustum;
				/* 0x198 */ NiAPI::NiRefObject* _Accumulator;
				/* 0x1A0 */ bool _RecurseToGeometry;
				/* 0x1A1 */ char pad1A1[0x7];
			};
			static_assert(sizeof(BSCullingProcess) == 0x1A8);
			static_assert(BSCullingProcess::VerifyLayout());
		}
	}
}
