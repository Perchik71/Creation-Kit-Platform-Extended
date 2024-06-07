// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESNPC;
			class BSDynamicTriShape;
		}
	}

	namespace Patches
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::Core;

			using namespace EditorAPI::Fallout4;

			class FixUsesReadyFaceGenPatch : public Module
			{
			public:
				struct ActorData
				{
					TESNPC* Actor;			// 00
					bool GenerationAgain;	// 08
					bool NoUseGenerator;	// 09
					uint8_t Count;			// 0A
				};

				FixUsesReadyFaceGenPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static bool sub(ActorData* Data, BSDynamicTriShape* TriShape);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				FixUsesReadyFaceGenPatch(const FixUsesReadyFaceGenPatch&) = default;
				FixUsesReadyFaceGenPatch& operator=(const FixUsesReadyFaceGenPatch&) = default;
			};
			static_assert(sizeof(FixUsesReadyFaceGenPatch::ActorData) == 0x10);
		}
	}
}