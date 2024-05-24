// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "ProgressTaskBar.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		ProgressTaskBar::ProgressTaskBar(HWND LinkWnd, DWORD Total) : _linkWnd(LinkWnd), _marquee(false), _beg(false),
			_progressCompleted(0), _progressTotal(Total)
		{
			if (!IsWindow(LinkWnd))
				_MESSAGE("IProgressTaskBar could not be created, because LinkWnd isn't window.");
			else
			{
				if (SUCCEEDED(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(_context.GetAddressOf()))))
				{
					if (FAILED(_context->HrInit()))
					{
						_context = nullptr;
						_MESSAGE("ITaskbarList::HrInit() has failed.");
					}
					else
						SetProgressState(TBPF_NOPROGRESS);
				}
				else
					_MESSAGE("ITaskbarList could not be created.");
			}
		}

		ProgressTaskBar::~ProgressTaskBar()
		{
			End();
		}

		void ProgressTaskBar::Begin()
		{
			_beg = SetProgressState(_marquee ? TBPF_INDETERMINATE : TBPF_NORMAL);
		}

		void ProgressTaskBar::End()
		{
			_beg = !SetProgressState(TBPF_NOPROGRESS);
		}

		void ProgressTaskBar::SetMarquee(bool Value)
		{
			if (!_beg || (Value == _marquee))
				return;

			if (Value)
				_marquee = SetProgressState(TBPF_INDETERMINATE);
			else
			{
				_marquee = !SetProgressState(TBPF_NORMAL);
				SetProgressValue(_progressCompleted, _progressTotal);
			}
		}

		void ProgressTaskBar::SetPosition(DWORD Completed)
		{
			if (Completed < _progressTotal && SetProgressValue(Completed, _progressTotal))
				_progressCompleted = Completed;
		}

		void ProgressTaskBar::Step()
		{
			SetPosition(_progressCompleted + 1);
		}

		void ProgressTaskBar::SetTotal(DWORD Total)
		{
			_progressTotal = Total;
			if (_progressCompleted > _progressTotal)
				_progressCompleted = _progressTotal;
			SetProgressValue(_progressCompleted, _progressTotal);
		}

		void ProgressTaskBar::Reset()
		{
			SetPosition(0);
		}

		bool ProgressTaskBar::SetProgressState(DWORD State)
		{
			if (_context != nullptr)
				return SUCCEEDED(_context->SetProgressState(_linkWnd, (TBPFLAG)State));
			return false;
		}

		bool ProgressTaskBar::SetProgressValue(DWORD Completed, DWORD Total)
		{
			if ((_context != nullptr) && _beg && !_marquee)
				return SUCCEEDED(_context->SetProgressValue(_linkWnd, Completed, Total));
			return false;
		}
	}
}