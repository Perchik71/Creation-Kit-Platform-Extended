// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSTArray.h"
#include "BSFixedString.h"
#include "NiAPI/NiTypes.h"
#include "NiAPI/NiTSimpleArray.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				// https://github.com/CharmedBaryon/CommonLibSSE-NG/blob/main/include/RE/E/ErrorCodes.h
				enum class ErrorCode
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

				enum class SeekMode
				{
					kBeginning = 0,
					kCurrent = 1,
					kEnd = 2
				};

				class Stream;
				class StreamBase;

				class StreamBase
				{
					std::uint64_t _TotalSize;
					std::uint64_t _Flags;
				public:
					virtual ~StreamBase();
					virtual ErrorCode Open() = 0;
					virtual void Close() = 0;
					virtual std::uint32_t GetKey();		// return 0;
					virtual ErrorCode GetInfo();		// return kUnsupported;
				};
				static_assert(sizeof(StreamBase) == 0x18);

				class Stream : public StreamBase
				{
				public:
					virtual ~Stream();
					virtual void Clone() = 0;
					virtual ErrorCode ReadBuffer(void* buffer, std::uint64_t size, std::uint64_t& read) const = 0;
					virtual ErrorCode ReadBufferAt(void* buffer, std::uint64_t size, std::uint64_t pos, std::uint64_t& read) const;
					virtual ErrorCode WriteBuffer(const void* buffer, std::uint64_t size, std::uint64_t& written) const = 0;
					virtual ErrorCode Seek(std::uint64_t offset, SeekMode mode, std::uint64_t& pos) const = 0;
					virtual ErrorCode SetEndOfStream();																		// return kUnsupported;
					virtual ErrorCode PrefetchAt(std::uint64_t bytes, std::uint64_t offset, std::uint32_t priority) const;	// return kUnsupported;
					virtual ErrorCode StartTaggedPrioritizedRead(void* buffer, std::uint64_t bytes, std::uint64_t offset, 
						std::uint32_t priority, volatile std::uint32_t* completionTag, std::uint32_t& completionTagWaitValue, 
						void* a_eventFlag) const;																			// return kUnsupported;
					virtual ErrorCode WaitTags(volatile std::uint32_t* completionTag, void* eventFlag, 
						std::uint32_t completionTagWaitValue) const;														// return kUnsupported;                                                                                                    // 0D
					virtual ErrorCode PrefetchAll(std::uint32_t priority) const;											// return kUnsupported;
					virtual bool GetFileName(BSFixedString& dst) const;
					virtual ErrorCode CreateAsync();																		// return kUnsupported;
					virtual bool      QTaggedPrioritizedReadSupported() const;
					virtual bool      Unk1() const;
					virtual ErrorCode Unk2() const;
					virtual ErrorCode Unk3() const;
				};
				static_assert(sizeof(Stream) == 0x18);

				struct LooseFileStreamInfo
				{
					char _pad20[0x10];
					char FileName[256];
				};
				static_assert(sizeof(LooseFileStreamInfo) == 0x110);

				struct LooseFileStreamBase
				{
					BSFixedString DataPath;
					BSFixedString RelativeDataSourcePath;
					BSFixedString ShortFileName;
					LooseFileStreamInfo Info;
					char _pad130[0x28];

					virtual ~LooseFileStreamBase();
				};
				static_assert(sizeof(LooseFileStreamBase) == 0x158);

				class LooseFileStream : public Stream, public LooseFileStreamBase
				{
				public:
					virtual ~LooseFileStream();
				};
				static_assert(sizeof(LooseFileStream) == 0x170);
			}

			class BSResourceNiBinaryStream : public NiAPI::NiBinaryStream
			{
				BSResource::Stream* _Stream;
				void* _Unk28;
			public:

				inline const BSResource::Stream* GetStream() const noexcept(true) { return _Stream; }
			};
			static_assert(sizeof(BSResourceNiBinaryStream) == 0x20);
		}
	}
}