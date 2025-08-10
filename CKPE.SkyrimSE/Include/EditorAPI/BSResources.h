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
	namespace SkyrimSE
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

				class Location;
				class LooseFileLocation;
				class Stream;
				class StreamBase;

				class StreamBase
				{
					std::uint32_t _TotalSize;
					std::uint32_t _Unk0C;				// Added in 1.6.XXXX
					std::uint32_t _Flags;
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
					LooseFileLocation* _Location;		// Added in 1.6.XXXX
					std::uint32_t _filePos;
					void* _Buffer;
				public:
					virtual ~Stream();
					virtual void Clone() = 0;
					virtual ErrorCode ReadBuffer(void* buffer, std::uint32_t size, std::uint32_t& read) const = 0;
					virtual ErrorCode WriteBuffer(const void* buffer, std::uint32_t size, std::uint32_t& written) const = 0;
					virtual ErrorCode Seek(std::uint32_t offset, SeekMode mode, std::uint32_t& pos) const = 0;
					virtual ErrorCode SetEndOfStream();				// return kUnsupported;
					virtual bool GetFileName(BSFixedString& dst) const;
					virtual ErrorCode CreateAsync();				// return kUnsupported;
				};
				static_assert(sizeof(Stream) == 0x30);

				class Location
				{
					std::uint32_t Unk08;
				public:
					virtual ~Location();
				};
				static_assert(sizeof(Location) == 0x10);

				class LooseFileLocation : public Location
				{
					BSFixedString _Path;
					std::uint32_t Unk18;
					std::uint32_t Unk1C;
				public:
					virtual ~LooseFileLocation();

					[[nodiscard]] inline const char* GetFileName() const noexcept(true) { return _Path.c_str(); }
					[[nodiscard]] inline BSFixedString GetFileNameBS() const noexcept(true) { return _Path; }
				};
				static_assert(sizeof(LooseFileLocation) == 0x20);

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

					virtual ~LooseFileStreamBase();
				};
				static_assert(sizeof(LooseFileStreamBase) == 0x130);

				struct StreamBuffer
				{
					char pad00[8];
					LooseFileStreamInfo* StreamInfo;
					char pad10[0x20];
					void* buffer;
					char pad38[8];
				};

				//`class BSResource::ArchiveStream`: VTable[0x00000000033360E0, 0x0000000000000000 offset, 13 functions] `. ? AVArchiveStream@BSResource@@`
				//`class BSResource::CompressedArchiveStream`: VTable[0x0000000003336168, 0x0000000000000000 offset, 13 functions] `. ? AVCompressedArchiveStream@BSResource@@`

				/*`class BSResource::`anonymous namespace'::LooseFileStream`: VTable [0x00000000032515B0, 0x0000000000000000 offset, 1 functions] `.?AVLooseFileStream@?A0x21d31999@BSResource@@`
					#0: flags(4d) `struct BSResource::`anonymous namespace'::LooseFileStreamBase`:`.?AULooseFileStreamBase@?A0x21d31999@BSResource@@`
					#1: flags(40) `class BSResource::Stream`:`. ? AVStream@BSResource@@`
					#0: flags(40) `class BSResource::StreamBase`:`. ? AVStreamBase@BSResource@@`*/
				class LooseFileStream : public LooseFileStreamBase, public Stream
				{
				public:
					virtual ~LooseFileStream();
				};
				static_assert(sizeof(LooseFileStream) == 0x160);
			}

			class BSResourceNiBinaryStream : public NiAPI::NiBinaryStream
			{
				BSResource::Stream* _Stream;
				void* _Unk28;
			public:

				inline const BSResource::Stream* GetStream() const noexcept(true) { return _Stream; }
			};
			static_assert(sizeof(BSResourceNiBinaryStream) == 0x30);
		}
	}
}