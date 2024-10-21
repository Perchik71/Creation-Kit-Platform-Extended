// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/RegistratorWindow.h"
#include "RenderWindowSF.h"
#include "MainWindowSF.h"
#ifdef _CKPE_WITH_QT5
#	include "Editor API/SF/BGSRenderWindowSF.h"
#	include <QtGui/qpalette.h>
#endif // !_CKPE_WITH_QT5

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
#ifdef _CKPE_WITH_QT5
			using namespace CreationKitPlatformExtended::EditorAPI::Starfield;
#endif // !_CKPE_WITH_QT5

			RenderWindow* GlobalRenderWindowPtr;
			uintptr_t pointer_RenderWindow_sub0;

			RenderWindow::RenderWindow() : Module(GlobalEnginePtr)
			{
				Assert(!GlobalRenderWindowPtr);
				GlobalRenderWindowPtr = this;
			}

			bool RenderWindow::HasOption() const
			{
				return false;
			}

			bool RenderWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RenderWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* RenderWindow::GetName() const
			{
				return "Render Window";
			}

			bool RenderWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> RenderWindow::GetDependencies() const
			{
				return {};
			}

			bool RenderWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool RenderWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&pointer_RenderWindow_sub0 =
						voltek::detours_function_class_jump(_RELDATA_ADDR(0), (uintptr_t)&Initialize);

					return true;
				}

				return false;
			}

			bool RenderWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void* CALLBACK RenderWindow::Initialize(__int64 unk1, __int64 unk2)
			{
#ifdef _CKPE_WITH_QT5
				/*EditorAPI::Starfield::GlobalRenderWindowPtr = fastCall<BGSRenderWindow*>(pointer_RenderWindow_sub0,
					unk1, unk2);

				if (EditorAPI::Starfield::GlobalRenderWindowPtr)
				{
					QPalette p;
					p.setColor(QPalette::Window, QColor(0x00C0C0C0));
					EditorAPI::Starfield::GlobalRenderWindowPtr->setAutoFillBackground(true);
					EditorAPI::Starfield::GlobalRenderWindowPtr->setPalette(p);

					EditorAPI::Starfield::GlobalRenderWindowPtr->
				}*/

				return EditorAPI::Starfield::GlobalRenderWindowPtr;
#endif // !_CKPE_WITH_QT5
			}
		}
	}
}