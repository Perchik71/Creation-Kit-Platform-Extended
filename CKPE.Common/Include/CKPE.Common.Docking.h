// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.CriticalSection.h>
#include <CKPE.Common.Common.h>
#include <cstdint>
#include <map>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API DockingWindow
		{
			DockingWindow(const DockingWindow&) = delete;
			DockingWindow& operator=(const DockingWindow&) = delete;
		protected:
			std::uintptr_t _Wnd{ 0 };
		public:
			constexpr DockingWindow() noexcept(true) = default;
			DockingWindow(std::uintptr_t hWnd);
			virtual ~DockingWindow() = default;
		
			[[nodiscard]] virtual std::uintptr_t GetWindow() const noexcept(true);
		};

		class CKPE_COMMON_API DockingHoverWindow : public DockingWindow
		{
			DockingHoverWindow(const DockingHoverWindow&) = delete;
			DockingHoverWindow& operator=(const DockingHoverWindow&) = delete;
		protected:
			DockingWindow* _link_wnd{ nullptr };
		public:
			DockingHoverWindow();
			virtual ~DockingHoverWindow();

			virtual void Show() const noexcept(true);
			virtual void Show(std::int32_t x, std::int32_t y, std::int32_t wx, std::int32_t wy) const noexcept(true);
			virtual void Hide() const noexcept(true);
			virtual void Move(std::int32_t x, std::int32_t y, std::int32_t wx, std::int32_t wy) const noexcept(true);

			virtual DockingWindow* GetLinkWindow() const noexcept(true);
			virtual void SetLinkWindow(DockingWindow* wnd) noexcept(true);
		};

		class CKPE_COMMON_API DockingFrameWindow : public DockingWindow
		{
			DockingFrameWindow(const DockingFrameWindow&) = delete;
			DockingFrameWindow& operator=(const DockingFrameWindow&) = delete;
		protected:
			DockingWindow* _Container{ nullptr };
			std::uintptr_t _OldStyles{ 0 };
			std::uintptr_t _OldExStyles{ 0 };
			std::uint32_t _Flags{ 0 };

			void RestoreWindowStyles() noexcept(true);
			void ApplyDockingWindowStyles() noexcept(true);
			void SetFlag(std::uint32_t f) noexcept(true);
			void UnsetFlag(std::uint32_t f) noexcept(true);
		public:
			enum : std::uint32_t
			{
				EF_NONE = 0,
				EF_DOCKSTYLES = 1 << 0,
				EF_FLOATING = 1 << 1,
				EF_MOVING = 1 << 2,
			};

			enum : std::uint32_t
			{
				EFR_DOCKLEFT	= 1 << 10,
				EFR_DOCKTOP		= 1 << 11,
				EFR_DOCKRIGHT	= 1 << 12,
				EFR_DOCKBOTTOM	= 1 << 13,
			};

			DockingFrameWindow(std::uintptr_t hWnd);
			virtual ~DockingFrameWindow();
			
			virtual bool Undock() noexcept(true);
			virtual bool Dock(std::uintptr_t hWnd) noexcept(true);

			[[nodiscard]] virtual bool HasDocking() const noexcept(true);
			[[nodiscard]] virtual DockingWindow* GetDockContainer() const noexcept(true);

			[[nodiscard]] virtual std::uint32_t GetFlags() const noexcept(true);
			[[nodiscard]] virtual bool HasFlag(std::uint32_t f) const noexcept(true);
		};

		class CKPE_COMMON_API DockingRootWindow : public DockingFrameWindow
		{
			DockingRootWindow(const DockingRootWindow&) = delete;
			DockingRootWindow& operator=(const DockingRootWindow&) = delete;
		public:
			DockingRootWindow();

			virtual bool SetWindow(std::uintptr_t hWnd) noexcept(true);
		};

		class CKPE_COMMON_API DockingManager : public DockingRootWindow
		{
			CriticalSection _Guard;
			std::map<std::uintptr_t, DockingFrameWindow*>* _Container;
			DockingHoverWindow _HoverWindow;

			DockingManager(const DockingManager&) = delete;
			DockingManager& operator=(const DockingManager&) = delete;
		public:
			enum : std::uint32_t
			{
				E_FRAME = 0
			};

			DockingManager();
			virtual ~DockingManager();

			virtual void FreeAll() noexcept(true);
			virtual bool AddWindow(std::uintptr_t hWnd, std::uint32_t style = E_FRAME) noexcept(true);
			virtual bool RemoveWindow(std::uintptr_t hWnd) noexcept(true);

			[[nodiscard]] virtual std::size_t Count() const noexcept(true);
			[[nodiscard]] virtual DockingWindow* At(std::uintptr_t hWnd) const noexcept(true);
			[[nodiscard]] virtual DockingWindow* AtByIndex(std::size_t id) const noexcept(true);
		};
	}
}