// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSString.h"
#include "NiAPI/NiTypes.h"
#include "NiAPI/NiPointer.h"
#include "BSResource.h"
#include "BSFixedString.h"
#include "BSResourceLooseFiles.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				struct InfoEx
				{
					LooseFileStream* resFile;
					std::uint64_t unk08;
					BSEntryString* fileName;
					std::uint64_t unk18;
				};

				class Archive2
				{
				public:
					static void Initialize() noexcept(true);
					static void GetFileSizeStr(std::uint64_t fileSize, BSString& fileSizeStr) noexcept(true);
					static EResultError HKLoadStreamArchive(void* arrayDataList, LooseFileStream*& resFile, void* Unk1, std::uint32_t Unk2) noexcept(true);
					static EResultError HKLoadStreamArchiveEx(void* arrayDataList, InfoEx* infoRes, void* Unk1, std::uint32_t Unk2) noexcept(true);
					static void HKLoadArchive(const char* fileName, void* Unk1, void* Unk2, std::uint32_t Unk3) noexcept(true);
					static void LoadArchive(const char* fileName) noexcept(true);
					static bool IsAvailableForLoad(const char* fileName) noexcept(true);
					static void LoadPrimaryArchive() noexcept(true);

					class ReaderStream : public Stream
					{
						Stream* _source{ nullptr };								// 10
						void* _unk18{ nullptr };								// 18
						const std::uint64_t startOffset{ 0 };					// 20
						BSFixedString name;										// 28
						std::uint32_t currentRelativeOffset{ 0 };				// 30
						std::uint32_t compressedSize{ 0 };						// 34
						std::uint32_t uncompressedSize{ 0 };					// 38
						NiAPI::NiTFlags<std::uint32_t, ReaderStream> flags;		// 3C
					public:
						inline static std::uint64_t RTTI = 0;

						ReaderStream() noexcept(true) = default;

						virtual ~ReaderStream();
						virtual EResultError DoOpen() noexcept(true) override;
						virtual void DoClose() noexcept(true) override;
						virtual std::uint64_t DoGetKey() const noexcept(true) override;
						virtual EResultError DoGetInfo(void*) noexcept(true) override;														// return kUnsupported
						virtual void DoClone(void* p) const noexcept(true) override;
						virtual EResultError DoRead(void* Buffer, std::uint64_t Bytes, std::uint64_t& Read) const noexcept(true) override;
						virtual EResultError DoReadAt(void*, std::uint64_t, std::uint64_t, std::uint64_t&) const noexcept(true) override;	// return kUnsupported
						virtual EResultError DoWrite(const void*, std::uint64_t, std::uint64_t&) const noexcept(true) override;				// return kUnsupported
						virtual EResultError DoSeek(std::int64_t p, SeekMode m, std::uint64_t& np) const noexcept(true) override;
						virtual EResultError DoSetEndOfStream() noexcept(true) override;													// return kUnsupported
						virtual EResultError DoPrefetchAt(uint64_t, uint64_t, uint32_t) const noexcept(true) override;						// return kUnsupported		
						virtual EResultError DoStartTaggedPrioritizedRead(void* buffer, std::uint64_t bytes, std::uint64_t offset,
							std::uint32_t priority, std::uint32_t* completionTagOut, std::uint32_t& completionTagWaitValue,
							void* unk) const noexcept(true) override;																		// return kUnsupported
						virtual EResultError DoWaitTags(std::uint32_t* completionTag, std::uint32_t completionTagWaitValue,
							void* unk) const noexcept(true) override;																		// return kUnsupported
						virtual EResultError DoPrefetchAll(std::uint32_t a) const noexcept(true) override;
						virtual bool DoGetName(BSFixedString& result) const noexcept(true) override;
						virtual EResultError DoCreateAsync(void* unk) const noexcept(true) override;
						virtual bool DoQTaggedPrioritizedReadSupported() const noexcept(true) override;										// return false
						virtual EResultError DoCreateOp(void*, const char*) const noexcept(true) override;									// return kUnsupported
						virtual bool DoGetIsFromArchive() const noexcept(true) override;													// return true
					};
				};
				static_assert(sizeof(Archive2::ReaderStream) == 0x40);
			}
		}
	}
}