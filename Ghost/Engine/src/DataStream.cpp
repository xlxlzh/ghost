#include <cassert>
#include "DataStream.h"

namespace ghost
{
	MemoryDataStream::MemoryDataStream(void* mem, std::size_t size, bool freeOnClose /* = false */, bool readonly /* = false */) :
		DataStream(readonly ? AccessMode::AM_READ : AccessMode::AM_READ_WRITE)
	{
		_data = static_cast<uchar*>(mem);
		_pos = _data;
		_end = _data + size;
		_freeOnClose = freeOnClose;

		assert(_end >= _pos);
	}

	MemoryDataStream::MemoryDataStream(const std::string& name, void* mem, std::size_t size, bool freeOnClose /* = false */, bool readonly /* = false */)
		: DataStream(name, readonly ? AccessMode::AM_READ : AccessMode::AM_READ_WRITE)
	{
		_data = static_cast<uchar*>(mem);
		_pos = _data;
		_end = _data + size;
		_freeOnClose = freeOnClose;

		assert(_end >= _pos);
	}

	std::size_t MemoryDataStream::read(void* buf, std::size_t count)
	{
		std::size_t cnt = count;
		
		if (_pos + cnt > _end)
		{
			cnt = _end - _pos;
		}

		if (cnt == 0)
		{
			return 0;
		}

		memcpy(buf, _pos, cnt);
		_pos += cnt;

		return cnt;
	}

	std::size_t MemoryDataStream::write(const void* buf, size_t count)
	{
		std::size_t sizeWritten = 0;
		if (isWriteable())
		{
			sizeWritten = count;
			
			if (_pos + sizeWritten > _end)
			{
				sizeWritten = _end - _pos;
			}

			if (sizeWritten == 0)
			{
				return 0;
			}

			memcpy(_pos, buf, sizeWritten);
			_pos += sizeWritten;
		}

		return sizeWritten;
	}

	void MemoryDataStream::skip(std::size_t count)
	{
		std::size_t newPos = static_cast<std::size_t>((_pos - _data) + count);
		assert(_data + newPos <= _end);

		_pos = _data + newPos;
	}

	void MemoryDataStream::seek(std::size_t pos)
	{
		assert(_data + pos <= _end);
		_pos = _data + pos;
	}

	std::size_t MemoryDataStream::tell() const
	{
		return _pos - _data;
	}

	bool MemoryDataStream::eof() const
	{
		return _pos >= _end;
	}

	void MemoryDataStream::close()
	{
		_accessMode = AccessMode::AM_READ;
		if (_freeOnClose && _data)
		{
			delete[] _data;
			_data = nullptr;
		}
	}
}