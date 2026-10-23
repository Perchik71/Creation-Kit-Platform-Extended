// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Fallout4.VersionLists.h>
#include <CKPE.Utils.h>
#include <EditorAPI/N/NiRefObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSIStream;
			class BSShaderMaterial;
			class BSRenderPass;

			class BSReloadShaderI
			{
			public:
				virtual void ReloadShaders(BSIStream* Stream) = 0;
			};
			static_assert(sizeof(BSReloadShaderI) == 0x8);

			class BSShader : public NiAPI::NiRefObject, public BSReloadShaderI
			{
			public:
				struct RuntimeData
				{
					char VertexShaders[0x38];						// 000
					char HullShaders[0x38];							// 038
					char DomainShaders[0x38];						// 070
					char PixelShaders[0x38];							// 0A8
					char ComputeShaders[0x38];						// 0E0
					const char* FxpFilename;							// 118
				};
				static_assert(sizeof(RuntimeData) == 0x120);
				static_assert(offsetof(RuntimeData, FxpFilename) == 0x118);

				BSShader() = delete;
				[[nodiscard]] static std::size_t GetRuntimeDataOffset() noexcept(true)
				{
					return VersionLists::GetEditorVersion() ==
						VersionLists::EDITOR_FALLOUT_C4_1_10_162_0 ? 0x20 : 0xC8;
				}
				[[nodiscard]] static std::size_t GetRuntimeSize() noexcept(true)
				{ return GetRuntimeDataOffset() + sizeof(RuntimeData); }
				[[nodiscard]] inline RuntimeData& GetRuntimeData() noexcept(true)
				{
					return *reinterpret_cast<RuntimeData*>(
						reinterpret_cast<char*>(this) + GetRuntimeDataOffset());
				}
				[[nodiscard]] inline const RuntimeData& GetRuntimeData() const noexcept(true)
				{
					return *reinterpret_cast<const RuntimeData*>(
						reinterpret_cast<const char*>(this) + GetRuntimeDataOffset());
				}
				[[nodiscard]] inline std::int32_t GetShaderType() const noexcept(true) { return _ShaderType; }
				inline bool SetupTechnique(std::uint32_t Technique) noexcept(true)
				{ return CKPE::vtbl_call<bool>(0x10, this, Technique); }
				inline void RestoreTechnique(std::uint32_t Technique) noexcept(true)
				{ CKPE::vtbl_call<void>(0x18, this, Technique); }
				inline void SetupMaterial(const BSShaderMaterial* Material) noexcept(true)
				{ CKPE::vtbl_call<void>(0x20, this, Material); }
				inline void RestoreMaterial(const BSShaderMaterial* Material) noexcept(true)
				{ CKPE::vtbl_call<void>(0x28, this, Material); }
				inline void SetupGeometry(BSRenderPass* Pass) noexcept(true)
				{ CKPE::vtbl_call<void>(0x38, this, Pass); }
				inline void RestoreGeometry(BSRenderPass* Pass) noexcept(true)
				{ CKPE::vtbl_call<void>(0x40, this, Pass); }
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{ return offsetof(BSShader, _ShaderType) == 0x18; }
			private:
				std::int32_t _ShaderType;							// 018
				std::uint32_t _Pad1C;								// 01C
				// 982+ inserts three 0x38 critical-section wrappers before the maps.
				char _VersionStorage[0x1C8];							// 020
			};
			static_assert(sizeof(BSShader) == 0x1E8);
			static_assert(BSShader::VerifyLayout());
		}
	}
}
