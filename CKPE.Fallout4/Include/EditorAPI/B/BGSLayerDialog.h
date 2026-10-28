// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <windows.h>
#include <commctrl.h>
#include <EditorAPI/B/BSTEvents.h>
#include <EditorAPI/T/TreeListControl.h>

namespace CKPE::Fallout4::EditorAPI
{
	struct CellAttachDetachEvent;
	namespace Forms { class BGSLayer; }

	class BGSLayerDialog :
		public BSTEventSink<TESObjectLoadedEvent>,
		public BSTEventSink<CellAttachDetachEvent>
	{
	public:
		enum ControlID : std::uint32_t
		{
			kNewLayer = 1124,
			kAddSelection = 3682,
			kFilter = 2581,
			kExpandAll = 5593,
			kCollapseAll = 5594,
			kTreeBody = 6086
		};
		enum : std::uint16_t { kDialogResource = 468 };

		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSLayerDialog[0];

		/* 0x000 */ virtual ~BGSLayerDialog();
		/* 0x008 */ EventResult ReceiveEvent(TESObjectLoadedEvent* Event,
			void* Dispatcher) override;
		EventResult ReceiveEvent(CellAttachDetachEvent* Event,
			void* Dispatcher) override;

		/* 0x010 */ std::uint8_t pad010[0x08];
		/* 0x018 */ HWND WindowHandle;
		/* 0x020 */ HIMAGELIST ImageList;
		/* 0x028 */ std::uint8_t pad028[0x04];
		/* 0x02C */ std::int32_t ClientWidth;
		/* 0x030 */ std::int32_t ClientHeight;
		/* 0x034 */ std::uint8_t pad034[0x04];
		/* 0x038 */ BSFixedString FilterText;
		/* 0x040 */ std::uint8_t pad040[0x70];
		/* 0x0B0 */ Forms::BGSLayer* ActiveLayer;
		/* 0x0B8 */ TreeListControl* TreeList;
	};
	static_assert(sizeof(BGSLayerDialog) == 0xC0);
	static_assert(offsetof(BGSLayerDialog, WindowHandle) == 0x18);
	static_assert(offsetof(BGSLayerDialog, FilterText) == 0x38);
	static_assert(offsetof(BGSLayerDialog, ActiveLayer) == 0xB0);
	static_assert(offsetof(BGSLayerDialog, TreeList) == 0xB8);
}
