// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

#include "Editor API/SSE/TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;
			using namespace CreationKitPlatformExtended::EditorAPI::SkyrimSpectialEdition;

			class FixRenderPassPatch : public Module
			{
			public:
				FixRenderPassPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static void InitSDM();
				static void KillSDM();
				static void* AllocatePass(void* Shader, void* ShaderProperty, void* Geometry,
					uint32_t PassEnum, uint8_t NumLights, void** SceneLights);
				static void DeallocatePass(void* Pass);

				static void sub(TESForm* Class, HWND WindowHandle);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				FixRenderPassPatch(const FixRenderPassPatch&) = default;
				FixRenderPassPatch& operator=(const FixRenderPassPatch&) = default;
			};
		}
	}
}