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
		namespace Starfield
		{
			using namespace CreationKitPlatformExtended::Core;

			class ReplaceImgQSplashWndPatch : public Module
			{
			public:
				ReplaceImgQSplashWndPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static void sub_QPixmap_Constructor(__int64 _this, __int64 a1, __int64 a2, __int64 a3);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				ReplaceImgQSplashWndPatch(const ReplaceImgQSplashWndPatch&) = default;
				ReplaceImgQSplashWndPatch& operator=(const ReplaceImgQSplashWndPatch&) = default;
			};
		}
	}
}