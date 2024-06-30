// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#ifdef _CKPE_WITH_QT5
#	include "QtCore/qfile.h"
#	include "QtCore/qresource.h"
#	include "QtGui/qpixmap.h"
#endif // !_CKPE_WITH_QT5
#include "ReplaceImgQSplashWnd.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			ReplaceImgQSplashWndPatch::ReplaceImgQSplashWndPatch() : Module(GlobalEnginePtr)
			{}

			bool ReplaceImgQSplashWndPatch::HasOption() const
			{
				return false;
			}

			bool ReplaceImgQSplashWndPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ReplaceImgQSplashWndPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ReplaceImgQSplashWndPatch::GetName() const
			{
				return "Replace Image QSplashWnd";
			}

			bool ReplaceImgQSplashWndPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ReplaceImgQSplashWndPatch::GetDependencies() const
			{
				return {};
			}

			bool ReplaceImgQSplashWndPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return true;
			}

			bool ReplaceImgQSplashWndPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&sub_QPixmap_Constructor);

					return true;
				}

				return false;
			}

			bool ReplaceImgQSplashWndPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void ReplaceImgQSplashWndPatch::sub_QPixmap_Constructor(__int64 _this, __int64 a1, __int64 a2, __int64 a3)
			{
#ifdef _CKPE_WITH_QT5
				// Qt has a very "ingenious solution" its own resource storage mechanism, 
				// it seems Bethesda thought or planned to use standard system mechanisms, as evidenced by the updated resources.
				// There are two options, the first one should be compiled as source code, or as an external resource,
				// I prefer the second option anyway.

				// The editor has also become a combined hodgepodge of Scaleform, Qt, and WinAPI. 
				// Which complicates GUI control, given Qt's excessive love of keeping the main loop.

				new((QPixmap*)_this) QPixmap(":/CKPE/aboutsf.png");
#endif // !_CKPE_WITH_QT5
			}
		}
	}
}