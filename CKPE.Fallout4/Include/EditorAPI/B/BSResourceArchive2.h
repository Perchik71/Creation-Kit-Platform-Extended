// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSString.h>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/B/BSResource.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSResourceLooseFiles.h>
#include <EditorAPI/B/BSResourceEntryDB.h>
#include <EditorAPI/B/BSTEvents.h>
#include <EditorAPI/B/BSReadWriteLock.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				extern bool NoTextureLoad;

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
					class RegisteredEvent;
					struct ClearRegistryEvent;

					template<std::uint32_t ArchiveCount>
					class TIndex :
						public BSTEventSink<RegisteredEvent>,
						public BSTEventSink<ClearRegistryEvent>
					{
					public:
						inline static constexpr std::uint32_t MaxArchives = ArchiveCount;

						virtual ~TIndex() override;
						virtual EventResult ReceiveEvent(RegisteredEvent* event,
							void* dispatcher) override;
						virtual EventResult ReceiveEvent(ClearRegistryEvent* event,
							void* dispatcher) override;

						/* 0x0010 */ std::byte unk0010[0x20];
						/* 0x0030 */ void* streams[MaxArchives];
						void* asyncStreams[MaxArchives];
						BSHashDB nameIDs[MaxArchives];
						std::uint32_t dataFileCount;
						std::uint32_t padCount;
						void* pager;
						void* rootPage;
						std::uint64_t unkTree18;
						std::uint64_t unkTree20;
						BSReadWriteLock lock;
					};
					// CK 1.11.246 expands the archive registry from 256 to 1024.
					using Index = TIndex<0x400>;
					using Index256 = TIndex<0x100>;
					static_assert(sizeof(Index) == 0x7060);
					static_assert(sizeof(Index256) == 0x1C60);
					static_assert(offsetof(Index, streams) == 0x30);
					static_assert(offsetof(Index, asyncStreams) == 0x2030);
					static_assert(offsetof(Index, nameIDs) == 0x4030);
					static_assert(offsetof(Index, dataFileCount) == 0x7030);
					static_assert(offsetof(Index, pager) == 0x7038);
					static_assert(offsetof(Index, lock) == 0x7058);
					static_assert(offsetof(Index256, asyncStreams) == 0x830);
					static_assert(offsetof(Index256, nameIDs) == 0x1030);
					static_assert(offsetof(Index256, dataFileCount) == 0x1C30);
					static_assert(offsetof(Index256, lock) == 0x1C58);

					static void Initialize() noexcept(true);
					static void GetFileSizeStr(std::uint64_t fileSize, BSString& fileSizeStr) noexcept(true);
					static EResultError HKLoadStreamArchive(void* arrayDataList, LooseFileStream*& resFile, void* Unk1, std::uint32_t Unk2) noexcept(true);
					static EResultError HKLoadStreamArchiveEx(void* arrayDataList, InfoEx* infoRes, void* Unk1, std::uint32_t Unk2) noexcept(true);
					static EResultError HKLoadStreamArchiveEx2(void* arrayDataList, void* Unk1, void* Unk2, std::uint32_t Unk3, const char* fileName) noexcept(true);
					static void HKLoadArchive(const char* fileName, void* Unk1, void* Unk2, std::uint32_t Unk3) noexcept(true);
					static void LoadArchive(const char* fileName) noexcept(true);
					static bool IsAvailableForLoad(const char* fileName) noexcept(true);
					static void LoadPrimaryArchive() noexcept(true);

					struct OG_NG
					{
						class ReaderStream : public BSResource::OG_NG::Stream
						{
							Stream* _source{ nullptr };								// 10
							void* _context{ nullptr };								// 18
							const std::uint64_t startOffset{ 0 };					// 20
							BSFixedString name;										// 28
							std::uint32_t currentRelativeOffset{ 0 };				// 30
							std::uint32_t compressedSize{ 0 };						// 34
							std::uint32_t uncompressedSize{ 0 };					// 38
							NiAPI::NiTFlags<std::uint32_t, ReaderStream> flags;		// 3C
						public:
							inline static std::uint64_t RTTI = 0;

							ReaderStream() noexcept(true) = default;

							[[nodiscard]] const Stream* GetSource() const noexcept(true) { return _source; }
							[[nodiscard]] const BSFixedString& GetName() const noexcept(true) { return name; }
							[[nodiscard]] std::uint64_t GetStartOffset() const noexcept(true) { return startOffset; }
							[[nodiscard]] std::uint32_t GetRelativeOffset() const noexcept(true) { return currentRelativeOffset; }
							[[nodiscard]] std::uint32_t GetCompressedSize() const noexcept(true) { return compressedSize; }
							[[nodiscard]] std::uint32_t GetUncompressedSize() const noexcept(true) { return uncompressedSize; }
							[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
							{
								return offsetof(ReaderStream, _source) == 0x10 &&
									offsetof(ReaderStream, _context) == 0x18 &&
									offsetof(ReaderStream, startOffset) == 0x20 &&
									offsetof(ReaderStream, name) == 0x28 &&
									offsetof(ReaderStream, flags) == 0x3C;
							}

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
							virtual EResultError DoPrefetchAt(std::uint64_t, std::uint64_t, std::uint32_t) const noexcept(true) override;		// return kUnsupported		
							virtual EResultError DoStartTaggedPrioritizedRead(void* buffer, std::uint64_t bytes, std::uint64_t offset,
								std::uint32_t priority, std::uint32_t* completionTagOut, std::uint32_t& completionTagWaitValue,
								void* unk) const noexcept(true) override;																		// return kUnsupported
							virtual EResultError DoWaitTags(std::uint32_t* completionTag, std::uint32_t completionTagWaitValue,
								void* unk) const noexcept(true) override;																		// return kUnsupported
							virtual EResultError DoPrefetchAll(std::uint32_t a) const noexcept(true) override;
							virtual bool GetFileName(BSFixedString& result) const noexcept(true) override;
							virtual EResultError DoCreateAsync(void* unk) const noexcept(true) override;
							virtual bool DoQTaggedPrioritizedReadSupported() const noexcept(true) override;										// return false
							virtual EResultError DoCreateOp(void*, const char*) const noexcept(true) override;									// return kUnsupported
							virtual bool DoGetIsFromArchive() const noexcept(true) override;													// return true
						};
					};

					struct AE
					{
						class ReaderStream : public BSResource::AE::Stream
						{
							Stream* _source{ nullptr };								// 18
							void* _context{ nullptr };								// 20
							const std::uint64_t startOffset{ 0 };					// 28
							BSFixedString name;										// 30
							std::uint32_t currentRelativeOffset{ 0 };				// 38
							std::uint32_t compressedSize{ 0 };						// 3C
							std::uint32_t uncompressedSize{ 0 };					// 40
							NiAPI::NiTFlags<std::uint32_t, ReaderStream> flags;		// 44
						public:
							inline static std::uint64_t RTTI = 0;

							ReaderStream() noexcept(true) = default;

							[[nodiscard]] const Stream* GetSource() const noexcept(true) { return _source; }
							[[nodiscard]] const BSFixedString& GetName() const noexcept(true) { return name; }
							[[nodiscard]] std::uint64_t GetStartOffset() const noexcept(true) { return startOffset; }
							[[nodiscard]] std::uint32_t GetRelativeOffset() const noexcept(true) { return currentRelativeOffset; }
							[[nodiscard]] std::uint32_t GetCompressedSize() const noexcept(true) { return compressedSize; }
							[[nodiscard]] std::uint32_t GetUncompressedSize() const noexcept(true) { return uncompressedSize; }
							[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
							{
								return offsetof(ReaderStream, _source) == 0x18 &&
									offsetof(ReaderStream, _context) == 0x20 &&
									offsetof(ReaderStream, startOffset) == 0x28 &&
									offsetof(ReaderStream, name) == 0x30 &&
									offsetof(ReaderStream, flags) == 0x44;
							}

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
							virtual EResultError DoPrefetchAt(std::uint64_t, std::uint64_t, std::uint32_t) const noexcept(true) override;		// return kUnsupported		
							virtual EResultError DoStartTaggedPrioritizedRead(void* buffer, std::uint64_t bytes, std::uint64_t offset,
								std::uint32_t priority, std::uint32_t* completionTagOut, std::uint32_t& completionTagWaitValue,
								void* unk) const noexcept(true) override;																		// return kUnsupported
							virtual EResultError DoWaitTags(std::uint32_t* completionTag, std::uint32_t completionTagWaitValue,
								void* unk) const noexcept(true) override;																		// return kUnsupported
							virtual EResultError DoPrefetchAll(std::uint32_t a) const noexcept(true) override;
							virtual bool GetFileName(BSFixedString& result) const noexcept(true) override;
							virtual EResultError DoCreateAsync(void* unk) const noexcept(true) override;
							virtual bool DoQTaggedPrioritizedReadSupported() const noexcept(true) override;										// return false
							virtual EResultError DoCreateOp(void*, const char*) const noexcept(true) override;									// return kUnsupported
							virtual bool DoGetIsFromArchive() const noexcept(true) override;													// return true
						};
					};
				};
				static_assert(sizeof(Archive2::OG_NG::ReaderStream) == 0x40);
				static_assert(sizeof(Archive2::AE::ReaderStream) == 0x48);
				static_assert(Archive2::OG_NG::ReaderStream::VerifyLayout());
				static_assert(Archive2::AE::ReaderStream::VerifyLayout());
			}
		}
	}
}