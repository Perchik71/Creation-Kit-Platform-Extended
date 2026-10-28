// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShader.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSLightingShader : public BSShader
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShader[0];

				struct RuntimeData
				{
					std::uint32_t Unk00;								// 00
					std::uint32_t CurrentTechniqueID;				// 04
					char Unk08[0x38];								// 08
					std::uint32_t FrameID;							// 40
					std::uint32_t Pad44;								// 44
				};
				static_assert(sizeof(RuntimeData) == 0x48);
				static_assert(offsetof(RuntimeData, CurrentTechniqueID) == 0x4);
				static_assert(offsetof(RuntimeData, FrameID) == 0x40);

				BSLightingShader() = delete;
				[[nodiscard]] static std::size_t GetRuntimeSize() noexcept(true)
				{ return BSShader::GetRuntimeSize() + sizeof(RuntimeData); }
				[[nodiscard]] inline RuntimeData& GetRuntimeData() noexcept(true)
				{
					return *reinterpret_cast<RuntimeData*>(
						reinterpret_cast<char*>(this) + BSShader::GetRuntimeSize());
				}
				[[nodiscard]] inline const RuntimeData& GetRuntimeData() const noexcept(true)
				{
					return *reinterpret_cast<const RuntimeData*>(
						reinterpret_cast<const char*>(this) + BSShader::GetRuntimeSize());
				}
			private:
				char _VersionStorage[0x48];							// 1E8
			};
			static_assert(sizeof(BSLightingShader) == 0x230);
		}
	}
}
