// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class ProgressTaskBar
		{
		public:
			ProgressTaskBar(HWND LinkWnd, DWORD Total = 95);
			~ProgressTaskBar();

			void Begin();
			void End();
			void SetMarquee(bool Value);
			void SetPosition(DWORD Completed);
			void Step();
			void SetTotal(DWORD Total);
			void Reset();
		protected:
			bool SetProgressState(DWORD State);
			bool SetProgressValue(DWORD Completed, DWORD Total);
		private:
			HWND _linkWnd;
			DWORD _progressCompleted;
			DWORD _progressTotal;
			bool _marquee;
			bool _beg;
			Microsoft::WRL::ComPtr<ITaskbarList3> _context;
		};
	}
}

