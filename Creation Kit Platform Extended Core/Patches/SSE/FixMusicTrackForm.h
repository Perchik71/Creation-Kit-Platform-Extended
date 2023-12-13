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

			class FixMusicTrackFormPatch : public Module
			{
			public:
				FixMusicTrackFormPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;

				static size_t sub(const char* String);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				FixMusicTrackFormPatch(const FixMusicTrackFormPatch&) = default;
				FixMusicTrackFormPatch& operator=(const FixMusicTrackFormPatch&) = default;
			};
		}
	}
}