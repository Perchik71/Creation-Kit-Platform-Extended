// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/BSTriShape.h

#pragma once

#include "NiAPI/NiTypes.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
#pragma pack(push, 4)
			class BSFile : public NiAPI::NiFile
			{
			protected:
				char pad48[0x1C];
				char _FileName[260];
				char pad168[0x8];
				std::uint32_t _ReadWriteBytes;			// Until it's zero again, it doesn't read any
				std::uint32_t _Size;
			public:
				virtual ~BSFile() = default;

				[[nodiscard]] virtual bool Reopen();
				virtual bool Unk48() = 0;
				// upd 0x174
				[[nodiscard]] virtual std::uint32_t GetSizeWithUpdate();	
				virtual bool Unk58();
				virtual bool Unk60();
				virtual bool Unk68();
				virtual bool Unk70();
				virtual bool Unk78();
				[[nodiscard]] virtual bool Exists();
				[[nodiscard]] virtual std::uint32_t ReadBuffer(void* buffer, std::uint32_t size);
				[[nodiscard]] virtual std::uint32_t WriteBuffer(void* buffer, std::uint32_t size);

				[[nodiscard]] inline const char* GetFileName() const noexcept(true) { return _FileName; }
				[[nodiscard]] inline std::uint32_t GetReadWriteBytesCount() const noexcept(true) { return _ReadWriteBytes; }
			};
			static_assert(sizeof(BSFile) == 0x178);
#pragma pack(pop)
		}
	}
}
