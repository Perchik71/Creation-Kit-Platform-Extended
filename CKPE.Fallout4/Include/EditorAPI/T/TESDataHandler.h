// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <algorithm>
#include <CKPE.Singleton.h>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/IDs.h>
#include <EditorAPI/N/NiTObjectArray.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BGSLayer.h>
#include <EditorAPI/B/BGSColorForm.h>
#include <EditorAPI/T/TESObjectARMO.h>
#include <EditorAPI/T/TESObjectREFR.h>
#include <EditorAPI/T/TESObjectCELL.h>
#include <EditorAPI/T/TESObjectLAND.h>
#include <EditorAPI/T/TESObjectLIGH.h>
#include <EditorAPI/B/BGSVoiceType.h>
#include <EditorAPI/T/TESTopic.h>
#include <EditorAPI/T/TESFile.h>
#include <EditorAPI/T/TESRegionDataManager.h>
#include <EditorAPI/C/CheckInFormList.h>
#include <EditorAPI/B/BSTEvents.h>
#include <EditorAPI/B/BSTSingleton.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			template<typename _Ty>
			using NiTPrimitiveArray = NiAPI::NiTObjectArray<_Ty>;
			using TESFormArray = BSTArray<Forms::TESForm*>;

			// 1.11.246.0: size 0x20, vtable RVA 0x302FD70, 3 virtual functions. A list of regions; frees them on destruction
			// when ownsRegionMemory is set (same members as CommonLibF4's TESRegionList).
			class TESRegionList
			{
				BSSimpleList<Forms::TESForm*> Region;						// 08
				std::uint8_t ownsRegionMemory;								// 18
				std::uint8_t pad19[0x7];									// 19
			public:
				virtual ~TESRegionList();

				// Calls Forms::TESRegion::CanCheckIn on every region, false as soon as one refuses (RVA 0x967B00).
				[[nodiscard]] virtual bool CanCheckIn(const Forms::CheckInFormList* Forms) const noexcept(true); // 01
				// Calls BuildCheckInDependencyMessage on every region that cannot be checked in (RVA 0x967B70).
				virtual void BuildCheckInDependencyMessage(const Forms::CheckInFormList* Forms, BSString& Message) const noexcept(true); // 02

				[[nodiscard]] inline const BSSimpleList<Forms::TESForm*>& GetRegions() const noexcept(true) { return Region; }
			};
			static_assert(sizeof(TESRegionList) == 0x20);

			class TESObjectList;
			class TESDataHandler :
				BSTEventSource<BGSHotloadCompletedEvent>,
				BSTSingletonSDM<TESDataHandler>
			{
			public:
				using TESFileList = BSSimpleList<TESFile>;
				using TESFileArray = BSTArray<TESFile*>;

				struct ModList {
					TESFileList modInfoList;							// 00
					TESFileArray loadedMods;							// 10
				};
				static_assert(sizeof(ModList) == 0x28);
				static_assert(offsetof(ModList, loadedMods) == 0x10);

				using UnkArray = BSTArray<LPVOID>;
				TESObjectList* objectList;	
				TESFormArray formArrays[std::to_underlying(Forms::TESForm::ftMax)];		// 0068
				TESRegionList* regionList;												// 0F50
				NiTPrimitiveArray<Forms::TESObjectCELL*> cellList;						// 0F58
				NiTPrimitiveArray<Forms::TESForm*> addonNodes;							// 0F70
				char unkF88[0x18];														// 0F88 - zeroed by the constructor (CommonLibF4: NiTList<TESForm*> badForms)
				std::uint32_t NextFormId;												// 0FA0 - 0x800 on construction
				std::uint32_t UnkDAC;													// 0FA4
				TESFile* activePlugin;													// 0FA8
				ModList modList;														// 0FB0
				BSTArray<TESFile*> smallMods;											// 0FD8 - light (ESL) plugins; loadedMods + smallMods form CommonLibF4's TESFileCollection at FC0
				std::uint8_t unkFF0;													// 0FF0
				std::uint8_t unkFF1;													// 0FF1 - LoadFiles / TESWorldSpace::InitItemImpl gate
				std::uint8_t buildingLOD;												// 0FF2 - set by the LOD build dispatcher for its duration, read by Save
				std::uint8_t unkFF3;													// 0FF3 - LoadFiles: loads the active file's TES info first
				std::uint8_t unkFF4;													// 0FF4 - gates TESForm file reassignment and NavMesh::LoadEdit
				std::uint8_t unkFF5;													// 0FF5 - SaveForm: skips forms of non-master files when set
				std::uint8_t clearingData;												// 0FF6 - set while the TES is destroyed; suppresses TESForm::SetAltered and UI updates
				std::uint8_t unkFF7;													// 0FF7 - 1 on construction
				std::uint8_t unkFF8;													// 0FF8 - gates the LightRadius lookup (sub_140C54630)
				std::uint8_t loadingFiles;												// 0FF9 - set around LoadFiles; the TESForm constructor does not allocate a form ID while set
				std::uint8_t unkFFA;													// 0FFA
				std::uint8_t unkFFB;													// 0FFB - written by LoadFileRecords from its bool argument
				std::uint8_t saveForGame;												// 0FFC - Save: with the game window open posts "FCF" to port 8080, else may launch Beta.exe with the plugin
				std::uint8_t saveForGameAlt;											// 0FFD - Save: posts "FCF" to port 4600 (set by the save handler for mode 4)
				std::uint8_t saveExtraNotify;											// 0FFE - Save: extra notification step (set by the save handler for mode 1)
				std::uint8_t unkFFF;													// 0FFF
				TESRegionDataManager* regionDataManager;								// 1000 - a TESRegionDataManagerEditor
				std::uint8_t unk1008;													// 1008 - cleared by the constructor
				char pad1009[0x7];														// 1009
			public:
				inline const TESFileList* GetMods() const { return &modList.modInfoList; }
				inline const TESFileArray* GetLoadedMods() const { return &modList.loadedMods; }
				inline const BSTArray<TESFile*>* GetSmallMods() const { return &smallMods; }
				inline TESFile* GetActiveMod() const { return activePlugin; }
				inline bool IsActiveMod() const { return activePlugin != nullptr; }

				[[nodiscard]] static TESDataHandler* GetSingleton() noexcept(true)
				{
					static Common::Relocation<TESDataHandler*> singleton{ IDs::TESDataHandler::Instance };
					return singleton.Get();
				}

				[[nodiscard]] static bool HasUserModded() noexcept(true)
				{
					static Common::Relocation<bool> userModded{ IDs::TESDataHandler::UserModded };
					return userModded.Get();
				}

				template <class T>
				[[nodiscard]] BSTArray<T*>& GetFormArray() noexcept  //
					requires(std::derived_from<T, Forms::TESForm> &&
							 !std::is_pointer_v<T> &&
							 !std::is_reference_v<T>)
				{
					CKPE_ASSERT(T::TYPE_ID < Forms::TESForm::ftMax);
					return reinterpret_cast<BSTArray<T*>&>(formArrays[static_cast<std::size_t>(T::TYPE_ID)]);
				}

				inline bool AddForm(Forms::TESForm* form) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::AddForm);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::AddForm };
					return func(this, form);
				}

				inline void RemoveForm(Forms::TESForm* form) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::RemoveForm);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::RemoveForm };
					func(this, form);
				}

				inline bool LoadFiles(bool flag) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::LoadFiles);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::LoadFiles };
					return func(this, flag);
				}

				inline bool LoadFileRecords(TESFile* file, bool flag) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::LoadFileRecords);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::LoadFileRecords };
					return func(this, file, flag);
				}

				// File>Save core: pre-processes the forms, creates the temporary file, saves every form through SaveForm and
				// replaces the target file. FileName is the target plugin name. CK 1.11.246.0 0x1408675F0.
				inline bool Save(const char* FileName) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::Save);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::Save };
					return func(this, FileName);
				}

				// Per-form dispatch of the Save loop: skips deleted forms and forms of other files, then calls the form's
				// save virtual (vtable +0xE8 for master-flagged forms, +0xD8 otherwise). CK 1.11.246.0 0x140859540.
				inline bool SaveForm(Forms::TESForm* Form) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::SaveForm);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::SaveForm };
					return func(this, Form);
				}
			public:
				CKPE_READ_PROPERTY(GetMods) TESFileList* Mods;
				CKPE_READ_PROPERTY(GetLoadedMods) const TESFileArray* LoadedMods;
				CKPE_READ_PROPERTY(GetActiveMod) TESFile* ActiveMod;
			};
			static_assert(sizeof(TESDataHandler) == 0x1010);
			static_assert(offsetof(TESDataHandler, formArrays) == 0x68);
			static_assert(offsetof(TESDataHandler, regionList) == 0xF50);
			static_assert(offsetof(TESDataHandler, cellList) == 0xF58);
			static_assert(offsetof(TESDataHandler, addonNodes) == 0xF70);
			static_assert(offsetof(TESDataHandler, modList) == 0xFB0);
			static_assert(offsetof(TESDataHandler, smallMods) == 0xFD8);
			static_assert(offsetof(TESDataHandler, regionList) == 0xF50);
			static_assert(offsetof(TESDataHandler, NextFormId) == 0xFA0);
			static_assert(offsetof(TESDataHandler, activePlugin) == 0xFA8);
			static_assert(offsetof(TESDataHandler, modList) == 0xFB0);
			static_assert(offsetof(TESDataHandler, smallMods) == 0xFD8);
			static_assert(offsetof(TESDataHandler, buildingLOD) == 0xFF2);
			static_assert(offsetof(TESDataHandler, clearingData) == 0xFF6);
			static_assert(offsetof(TESDataHandler, loadingFiles) == 0xFF9);
			static_assert(offsetof(TESDataHandler, saveForGame) == 0xFFC);
			static_assert(offsetof(TESDataHandler, regionDataManager) == 0x1000);
		}
	}
}