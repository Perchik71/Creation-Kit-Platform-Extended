// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <mmsystem.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.TableID.h>
#include <CKPE.HashUtils.h>
#include <CKPE.Exception.h>

namespace CKPE
{
	namespace Common
	{
		static TableID GlobalTableID;

		TableID::TableID(std::uintptr_t base) noexcept(true) :
			_table(new std::vector<std::uint32_t>)
		{
			Rebase(base);
		}

		TableID::~TableID() noexcept(true)
		{
			if (_table)
			{
				delete _table;
				_table = nullptr;
			}
		}

		void TableID::Rebase(std::uintptr_t base) noexcept(true)
		{
			if (!base)
				_base = (std::uintptr_t)GetModuleHandleA(nullptr);
			else
				_base = base;
		}

		bool TableID::Open(const std::string& fname) noexcept(true)
		{
			MemoryStream mstm;
			bool r = mstm.LoadFromFile(fname);
			if (r) return Open(mstm);
			return r;
		}

		bool TableID::Open(const std::wstring& fname) noexcept(true)
		{
			MemoryStream mstm;
			bool r = mstm.LoadFromFile(fname);
			if (r) return Open(mstm);
			return r;
		}

		bool TableID::Open(Stream& stm) noexcept(true)
		{
			try
			{
				if (!_table)
					throw std::runtime_error("TableID::Open Table no init");

				auto msize = stm.GetSize();
				if (msize < sizeof(TableIDHeader))
					throw std::runtime_error("TableID::Open stream data no contain header tableid");

				stm.SetPosition(0);

				TableIDHeader _header;
				if (stm.Read(&_header, sizeof(TableIDHeader)) != sizeof(TableIDHeader))
					throw std::runtime_error("TableID::Open stream data I/O error read");

				if (_header.fourcc != MAKEFOURCC('C', 'K', 'T', 'I'))
					throw std::runtime_error("TableID::Open stream data no tableid");

				if (_header.count)
				{
					std::size_t dsize = _header.count * sizeof(std::uint32_t);
					if (dsize != (msize - sizeof(TableIDHeader)))
						throw RuntimeError("TableID::Open stream data tableid invalid size ({} / {})", 
							dsize, (msize - sizeof(TableIDHeader)));

					_table->resize((std::size_t)_header.count);
					if (_table->empty())
						throw std::runtime_error("TableID::Open out of memory");

					if (stm.Read(_table->data(), (std::uint32_t)dsize) != (std::uint32_t)dsize)
						throw std::runtime_error("TableID::Open stream data I/O error read");

					auto mcrc32 = HashUtils::CRC32Buffer(_table->data(), (std::uint32_t)dsize);
					if (mcrc32 != _header.crc32)
					{
						_table->clear();

						throw RuntimeError("TableID::Open stream data tableid invalid crc32 ({:x} / {:x})",
							mcrc32, _header.crc32);
					}
				}
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());

				return false;
			}

			return true;
		}

		void TableID::Clear() noexcept(true)
		{
			if (_table)
				_table->clear();
		}

		std::uint32_t TableID::Rva(std::uint32_t id) const noexcept(true)
		{
			if (_table)
				return _table->size() > id ? _table->at(id) : 0;
			return 0;
		}

		std::uintptr_t TableID::Offset(std::uint32_t id) const noexcept(true)
		{
			auto mrva = Rva(id);
			return mrva ? (_base + mrva) : 0;
		}

		TableID* TableID::GetSingleton() noexcept(true)
		{
			return &GlobalTableID;
		}
	}
}