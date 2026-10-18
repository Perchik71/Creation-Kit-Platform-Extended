// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/T/TESFile.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// The CK's TESVersionControl (tesversioncontrol.cpp): a memory-mapped editor of one plugin file, with slack
			// space past the end of the file so that records can grow. The CK keeps it on the stack (no virtual
			// functions). It is used by the internal version control check-in and by the batch tools (delocalization,
			// tagification, DLC conversion). Layout and functions are identical in every supported CK runtime.
			class TESVersionControl
			{
			public:
				// The CK's default extra mapping (15 MB).
				inline static constexpr std::uint32_t DefaultSlack = 15728640;

				TESFile* file{ nullptr };		// 00 the file, closed while it is mapped
				HANDLE fileHandle{ nullptr };		// 08 exclusive read/write handle
				HANDLE mappingHandle{ nullptr };		// 10
				std::uint32_t size{ 0 };		// 18 logical size of the file
				std::uint32_t capacity{ 0 };		// 1C size + slack
				void* base{ nullptr };		// 20 mapped view
				void* current{ nullptr };		// 28 current record
				void* unk30{ nullptr };		// 30
				void* unk38{ nullptr };		// 38
				void* worldSpaceGroup{ nullptr };		// 40 cached top group of WRLD forms
				void* cellGroup{ nullptr };		// 48 cached top group of CELL forms
				void* unk50{ nullptr };		// 50
				std::uint32_t slack{ DefaultSlack };		// 58 bytes mapped past the end of the file for growing records
				std::uint32_t pad5C{ 0 };		// 5C

				explicit TESVersionControl(std::uint32_t Slack = DefaultSlack) noexcept(true) { slack = Slack; }
				TESVersionControl(const TESVersionControl&) = delete;
				TESVersionControl& operator=(const TESVersionControl&) = delete;
				~TESVersionControl() noexcept(true) { Close(); }

				[[nodiscard]] inline TESFile* GetFile() const noexcept(true) { return file; }
				[[nodiscard]] inline bool IsOpen() const noexcept(true) { return file && base; }

				// Closes the file, opens it exclusively and maps it. The file must be a master (ESM) with form groups;
				// it only works without version control while OpenFileGate() is set. CK 1.7.99.0 0x1414736B0.
				inline bool OpenFile(TESFile* File) noexcept(true)
				{
					using func_t = decltype(&TESVersionControl::OpenFile);
					static Common::Relocation<func_t> func{ Common::ID(87119) };
					return func(this, File);
				}

				// Flushes and unmaps the file, truncates it to its logical size and clears the object. Safe to call twice.
				// CK 1.7.99.0 0x141472790.
				inline void Close() noexcept(true)
				{
					using func_t = decltype(&TESVersionControl::Close);
					static Common::Relocation<func_t> func{ Common::ID(349537) };
					return func(this);
				}

				// Rebuilds the worldspace's OFST subrecord: re-saves the form into the CK's form buffer, appends the
				// offset table and splices the record into the mapped file. CK 1.7.99.0 0x141478420.
				inline bool UpdateWorldSpaceOffsets(Forms::TESForm* WorldSpace) noexcept(true)
				{
					using func_t = decltype(&TESVersionControl::UpdateWorldSpaceOffsets);
					static Common::Relocation<func_t> func{ Common::ID(453080) };
					return func(this, WorldSpace);
				}

				// While non-zero OpenFile works without bUseVersionControl; the CK's batch tools set it around their work.
				// CK 1.7.99.0 0x143ABA2D0.
				[[nodiscard]] inline static std::uint8_t& OpenFileGate() noexcept(true)
				{
					static Common::Relocation<std::uint8_t*> data{ Common::ID(225075) };
					return *data.Get();
				}

				// Set by the batch tools while they work on a file. CK 1.7.99.0 0x143B6B049.
				[[nodiscard]] inline static std::uint8_t& BatchMode() noexcept(true)
				{
					static Common::Relocation<std::uint8_t*> data{ Common::ID(399158) };
					return *data.Get();
				}

				// The file the batch tools write to; the form re-save localizes FormIDs against it. CK 1.7.99.0 0x143B6B050.
				[[nodiscard]] inline static TESFile*& TargetFile() noexcept(true)
				{
					static Common::Relocation<TESFile**> data{ Common::ID(234947) };
					return *data.Get();
				}
			};
			static_assert(sizeof(TESVersionControl) == 0x60);
			static_assert(offsetof(TESVersionControl, base) == 0x20);
			static_assert(offsetof(TESVersionControl, slack) == 0x58);
		}
	}
}
