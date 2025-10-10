// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BSResourceArchive2.h>
#include <EditorAPI/BSFixedString.h>
#include <EditorAPI/NiAPI/NiFlags.h>
#include <EditorAPI/NiAPI/NiPointer.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				// https://github.com/CharmedBaryon/CommonLibSSE-NG/blob/main/include/RE/E/ErrorCodes.h
				enum class EResultError : std::uint32_t
				{
					kNone = 0,
					kNotExist = 1,
					kInvalidPath = 2,
					kFileError = 3,
					kInvalidType = 4,
					kMemoryError = 5,
					kBusy = 6,
					kInvalidParam = 7,
					kUnsupported = 8
				};

				enum class SeekMode : std::uint32_t
				{
					kBeginning = 0,
					kCurrent = 1,
					kEnd = 2
				};

				class StreamBase
				{
				protected:
					std::uint32_t _totalSize{ 0 };						// 08
					NiAPI::NiTFlags<std::uint32_t, StreamBase> _flags;	// 0C
				public:
					StreamBase() noexcept(true) = default;
					StreamBase(const StreamBase& stm) noexcept(true) :
						_totalSize(stm._totalSize)
					{
						_flags.Flags = stm._flags.Flags & 1;
					}

					StreamBase& operator=(const StreamBase& stm) noexcept(true)
					{
						_totalSize = stm._totalSize;
						_flags.Flags = stm._flags.Flags & 1;

						return *this;
					}

					virtual ~StreamBase() {}
					virtual EResultError DoOpen() noexcept(true) = 0;
					virtual void DoClose() noexcept(true) = 0;
					[[nodiscard]] virtual std::uint64_t DoGetKey() const noexcept(true) { return 0xFFFFFFFF00000000; }
					virtual EResultError DoGetInfo(void*) noexcept(true) { return EResultError::kUnsupported; }

					[[nodiscard]] bool IsWritable() const noexcept(true) { return _flags.Has(1); }

					std::uint32_t IncRef() noexcept(true)
					{
						auto atomic = std::atomic_ref(_flags.Flags);
						std::uint32_t expected = 0;
						std::uint32_t desired = 0;
						do 
						{
							expected = _flags.Flags;
							desired = expected + 0x1000;
						} while (!atomic.compare_exchange_weak(expected, desired));
						return desired & 0xFFFFF000;
					}

					std::uint32_t DecRef() noexcept(true)
					{
						auto atomic = std::atomic_ref(_flags.Flags);
						std::uint32_t expected = 0;
						std::uint32_t desired = 0;
						do 
						{
							expected = _flags.Flags;
							desired = expected - 0x1000;
						} while (!atomic.compare_exchange_weak(expected, desired));
						return desired & 0xFFFFF000;
					}
				};
				static_assert(sizeof(StreamBase) == 0x10);

				class AsyncStream : public StreamBase
				{
				protected:
					std::uint32_t _minPacketSize = 0;		// 10
				public:
					struct PacketAlignedBuffer
					{
					public:
						std::uint64_t resultOffset = 0;     // 00
						std::uint32_t bufferSize = 0;       // 08
						void* packetBuffer = nullptr;       // 10
						std::uint32_t dataRequestSize = 0;  // 18
						std::uint32_t dataSize = 0;         // 1C
						void* dataStart = nullptr;          // 20
					};
					static_assert(sizeof(PacketAlignedBuffer) == 0x28);

					virtual void DoClone(void*) const noexcept(true) = 0;
					virtual EResultError DoStartRead(void* buffer, std::uint64_t bytes, std::uint64_t offset) const noexcept(true) = 0;
					virtual EResultError DoStartPacketAlignedBufferedRead(PacketAlignedBuffer* buffer, std::uint64_t bytes, std::uint64_t offset) const noexcept(true) = 0;
					virtual EResultError DoStartWrite(const void* buffer, std::uint64_t bytes, std::uint64_t offset) const noexcept(true) = 0;
					virtual EResultError DoTruncate(std::uint64_t bytes) const noexcept(true) = 0;
					virtual EResultError DoPrefetchAt(std::uint64_t bytes, std::uint64_t pos, std::uint32_t priority) const noexcept(true) 
					{
						return EResultError::kUnsupported;
					}

					virtual EResultError DoWait(std::uint64_t& a_transferred, bool a_block) noexcept(true) = 0;					
				};
				static_assert(sizeof(AsyncStream) == 0x18);

				class Stream : public StreamBase
				{
				public:
					virtual void DoClone(void*) const noexcept(true) = 0;
					virtual EResultError DoRead(void* buffer, std::uint64_t bytes, std::uint64_t& read) const noexcept(true) = 0;
					
					virtual EResultError DoReadAt(void* buffer, std::uint64_t bytes, std::uint64_t pos, std::uint64_t& read) const noexcept(true)
					{
						return EResultError::kUnsupported;
					}

					virtual EResultError DoWrite(const void* buffer, std::uint64_t bytes, std::uint64_t& write) const noexcept(true) = 0;
					virtual EResultError DoSeek(std::int64_t offset, SeekMode whence, std::uint64_t& pos) const noexcept(true) = 0;
					virtual EResultError DoSetEndOfStream() noexcept(true) { return EResultError::kUnsupported; }
					
					virtual EResultError DoPrefetchAt(std::uint64_t bytes, std::uint64_t offset, std::uint32_t priority) const noexcept(true) 
					{
						return EResultError::kUnsupported;
					} 

					virtual EResultError DoStartTaggedPrioritizedRead(void* buffer, std::uint64_t bytes, std::uint64_t offset,
						std::uint32_t priority, std::uint32_t* completionTagOut, std::uint32_t& completionTagWaitValue,
						void* unk) const noexcept(true) 
					{
						return EResultError::kUnsupported;
					}

					virtual EResultError DoWaitTags(std::uint32_t* completionTag, std::uint32_t completionTagWaitValue, void* unk)
						const noexcept(true)
					{
						return EResultError::kUnsupported;
					}

					virtual EResultError DoPrefetchAll(std::uint32_t priority) const noexcept(true)
					{
						return EResultError::kUnsupported;
					}

					virtual bool DoGetName(BSFixedString& result) const noexcept(true)
					{
						return false;
					}

					virtual EResultError DoCreateAsync(void*) const noexcept(true)
					{
						return EResultError::kUnsupported;
					}       
					
					[[nodiscard]] virtual bool DoQTaggedPrioritizedReadSupported() const noexcept(true) { return false; }
					virtual EResultError DoCreateOp(void*, const char*) const noexcept(true) { return EResultError::kUnsupported; }
					[[nodiscard]] virtual bool DoGetIsFromArchive() const noexcept(true) { return false; }                                                           // 13
				};
				static_assert(sizeof(Stream) == 0x10);
			}
		}
	}
}