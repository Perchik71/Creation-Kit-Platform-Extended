// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;

			class MemoryLeakInPreviewWindowPatch : public Module
			{
			public:
				MemoryLeakInPreviewWindowPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;

				static void HKCreateRenderTarget_CK1130(void* This, uint32_t TargetIndex, const void* Properties);
				static void HKCreateDepthStencil_CK1130(void* This, uint32_t TargetIndex, const void* Properties);
				static void HKCreateCubemapRenderTarget_CK1130(void* This, uint32_t TargetIndex, const void* Properties);

				inline static void (*CreateRenderTarget_CK1130)(void* This, uint32_t TargetIndex, const void* Properties);
				inline static void (*CreateDepthStencil_CK1130)(void* This, uint32_t TargetIndex, const void* Properties);
				inline static void (*CreateCubemapRenderTarget_CK1130)(void* This, uint32_t TargetIndex, const void* Properties);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				MemoryLeakInPreviewWindowPatch(const MemoryLeakInPreviewWindowPatch&) = default;
				MemoryLeakInPreviewWindowPatch& operator=(const MemoryLeakInPreviewWindowPatch&) = default;
			};
		}
	}
}