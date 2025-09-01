// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "QtCore/qfile.h"
#include "QtCore/qresource.h"
#include "QtGui/qpixmap.h"

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.ReplaceImgQSplashWnd.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			ReplaceImgQSplashWnd::ReplaceImgQSplashWnd() : Common::Patch()
			{
				SetName("Replace Image QSplashWnd");
			}

			bool ReplaceImgQSplashWnd::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ReplaceImgQSplashWnd::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ReplaceImgQSplashWnd::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ReplaceImgQSplashWnd::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ReplaceImgQSplashWnd::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool ReplaceImgQSplashWnd::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&sub_QPixmap_Constructor);

				return true;
			}

			void ReplaceImgQSplashWnd::sub_QPixmap_Constructor(std::int64_t a1, std::int64_t a2, std::int64_t a3,
				std::int64_t a4) noexcept(true)
			{
				// Qt has a very "ingenious solution" its own resource storage mechanism, 
				// it seems Bethesda thought or planned to use standard system mechanisms, as evidenced by the updated resources.
				// There are two options, the first one should be compiled as source code, or as an external resource,
				// I prefer the second option anyway.

				// The editor has also become a combined hodgepodge of Scaleform, Qt, and WinAPI. 
				// Which complicates GUI control, given Qt's excessive love of keeping the main loop.

				switch (std::rand() % 4)
				{
				case 0:
					new((QPixmap*)a1) QPixmap(":/CKPE/aboutwnd_1.png");
					break;
				case 1:
					new((QPixmap*)a1) QPixmap(":/CKPE/aboutwnd_2.png");
					break;
				case 2:
					new((QPixmap*)a1) QPixmap(":/CKPE/aboutwnd_3.png");
					break;
				case 3:
					new((QPixmap*)a1) QPixmap(":/CKPE/aboutwnd_4.png");
					break;
				}
			}
		}
	}
}