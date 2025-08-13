// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <wrl/client.h>
#include <shobjidl_core.h>
#include <map>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.ProgressTaskBar.h>

namespace CKPE
{
	namespace Common
	{
		class ProgressTaskBarImpl
		{
			void* _linkWnd{ nullptr };
			std::uint32_t _progressCompleted{ 0 };
			std::uint32_t _progressTotal{ 0 };
			bool _marquee{ false };
			bool _beg{ false };
			Microsoft::WRL::ComPtr<ITaskbarList3> _context;

			bool SetProgressState(std::uint32_t State) noexcept(true);
			bool SetProgressValue(std::uint32_t Completed, std::uint32_t Total) noexcept(true);
		public:
			ProgressTaskBarImpl(void* LinkWnd, std::uint32_t Total) noexcept(true);
			~ProgressTaskBarImpl() noexcept(true);

			void Begin() noexcept(true);
			void End() noexcept(true);
			void SetMarquee(bool Value) noexcept(true);
			void SetPosition(std::uint32_t Completed) noexcept(true);
			void Step() noexcept(true);
			void SetTotal(std::uint32_t Total) noexcept(true);
			void Reset() noexcept(true);
		};

		bool ProgressTaskBarImpl::SetProgressState(std::uint32_t State) noexcept(true)
		{
			if (_context != nullptr)
				return SUCCEEDED(_context->SetProgressState((HWND)_linkWnd, (TBPFLAG)State));
			return false;
		}

		bool ProgressTaskBarImpl::SetProgressValue(std::uint32_t Completed, std::uint32_t Total) noexcept(true)
		{
			if ((_context != nullptr) && _beg && !_marquee)
				return SUCCEEDED(_context->SetProgressValue((HWND)_linkWnd, Completed, Total));
			return false;
		}

		ProgressTaskBarImpl::ProgressTaskBarImpl(void* LinkWnd, std::uint32_t Total) noexcept(true) : 
			_linkWnd(LinkWnd), _progressTotal(Total)
		{
			if (!IsWindow((HWND)LinkWnd))
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

		ProgressTaskBarImpl::~ProgressTaskBarImpl() noexcept(true)
		{
			End();
		}

		void ProgressTaskBarImpl::Begin() noexcept(true)
		{
			_beg = SetProgressState(_marquee ? TBPF_INDETERMINATE : TBPF_NORMAL);
		}

		void ProgressTaskBarImpl::End() noexcept(true)
		{
			_beg = !SetProgressState(TBPF_NOPROGRESS);
		}

		void ProgressTaskBarImpl::SetMarquee(bool Value) noexcept(true)
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

		void ProgressTaskBarImpl::SetPosition(std::uint32_t Completed) noexcept(true)
		{
			if (Completed < _progressTotal && SetProgressValue(Completed, _progressTotal))
				_progressCompleted = Completed;
		}

		void ProgressTaskBarImpl::Step() noexcept(true)
		{
			SetPosition(_progressCompleted + 1);
		}

		void ProgressTaskBarImpl::SetTotal(std::uint32_t Total) noexcept(true)
		{
			_progressTotal = Total;
			if (_progressCompleted > _progressTotal)
				_progressCompleted = _progressTotal;
			SetProgressValue(_progressCompleted, _progressTotal);
		}

		void ProgressTaskBarImpl::Reset() noexcept(true)
		{
			SetPosition(0);
		}

		static std::map<std::uintptr_t, ProgressTaskBarImpl> ProgressTaskList;

		ProgressTaskBar::ProgressTaskBar(void* LinkWnd, std::uint32_t Total) noexcept(true) : LinkWnd(LinkWnd)
		{
			if (!ProgressTaskList.count((std::uintptr_t)LinkWnd))
				ProgressTaskList.insert(
					std::make_pair<const std::uintptr_t, ProgressTaskBarImpl>
					((std::uintptr_t)LinkWnd, { LinkWnd, Total }));
		}

		ProgressTaskBar::~ProgressTaskBar() noexcept(true)
		{
			End();
		}

		void ProgressTaskBar::Begin() noexcept(true)
		{
			auto it = ProgressTaskList.find((std::uintptr_t)LinkWnd);
			if (it != ProgressTaskList.end())
				it->second.Begin();
		}

		void ProgressTaskBar::End() noexcept(true)
		{
			auto it = ProgressTaskList.find((std::uintptr_t)LinkWnd);
			if (it != ProgressTaskList.end())
				it->second.End();
		}

		void ProgressTaskBar::SetMarquee(bool Value) noexcept(true)
		{
			auto it = ProgressTaskList.find((std::uintptr_t)LinkWnd);
			if (it != ProgressTaskList.end())
				it->second.SetMarquee(Value);
		}

		void ProgressTaskBar::SetPosition(std::uint32_t Completed) noexcept(true)
		{
			auto it = ProgressTaskList.find((std::uintptr_t)LinkWnd);
			if (it != ProgressTaskList.end())
				it->second.SetPosition(Completed);
		}

		void ProgressTaskBar::Step() noexcept(true)
		{
			auto it = ProgressTaskList.find((std::uintptr_t)LinkWnd);
			if (it != ProgressTaskList.end())
				it->second.Step();
		}

		void ProgressTaskBar::SetTotal(std::uint32_t Total) noexcept(true)
		{
			auto it = ProgressTaskList.find((std::uintptr_t)LinkWnd);
			if (it != ProgressTaskList.end())
				it->second.SetTotal(Total);
		}

		void ProgressTaskBar::Reset() noexcept(true)
		{
			auto it = ProgressTaskList.find((std::uintptr_t)LinkWnd);
			if (it != ProgressTaskList.end())
				it->second.Reset();
		}
	}
}