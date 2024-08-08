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
		using namespace CreationKitPlatformExtended::Core;

		class FaceGenPatch : public Module
		{
		public:
			FaceGenPatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static void sub(__int64 a1, __int64 a2);
			static void sub_sf(__int64 a1, __int64 a2);

			struct Fallout4
			{
				static void CreateDiffuseCompressDDS(__int64 lpThis, uint32_t TextureId, const char* lpFileName, int32_t Unk1, bool Unk2);
				static void CreateNormalsCompressDDS(__int64 lpThis, uint32_t TextureId, const char* lpFileName, int32_t Unk1, bool Unk2);
				static void CreateSpecularCompressDDS(__int64 lpThis, uint32_t TextureId, const char* lpFileName, int32_t Unk1, bool Unk2);
				static void Execute(bool bShowDone);
				static void ExecuteGUI();
				static void ExecuteCLI(__int64 Arg1, __int64 Arg2, __int64 Arg3, __int64 Arg4);
			};
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			FaceGenPatch(const FaceGenPatch&) = default;
			FaceGenPatch& operator=(const FaceGenPatch&) = default;
		};
	}
}