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
        _size = size;

		assert(_end >= _pos);
	}

	MemoryDataStream::MemoryDataStream(const std::string& name, void* mem, std::size_t size, bool freeOnClose /* = false */, bool readonly /* = false */)
		: DataStream(name, readonly ? AccessMode::AM_READ : AccessMode::AM_READ_WRITE)
	{
		_data = static_cast<uchar*>(mem);
		_pos = _data;
		_end = _data + size;
		_freeOnClose = freeOnClose;
        _size = size;

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

	FileStream::FileStream(const std::string& fileName, AccessMode mode /* = AccessMode::AM_READ */)
		: DataStream(fileName, mode)
	{
		_fileStream = new std::fstream();
		unsigned flag = getStreamFlag(mode);

		if (_fileStream)
		{
			_fileStream->open(fileName.c_str(), flag | std::ios::binary);
            if (_fileStream->is_open())
            {
                _fileStream->seekg(0, std::ios::end);
                _size = _fileStream->tellg();
                _fileStream->seekg(0, std::ios::beg);
            }
		}
	}

	unsigned FileStream::getStreamFlag(AccessMode mode)
	{
		unsigned flag = 0;
		if (mode & AccessMode::AM_READ)
		{
			flag |= std::ios::in;
		}

		if (mode & AccessMode::AM_WRITE)
		{
			flag |= std::ios::out;
		}

		return flag;
	}

	std::size_t FileStream::read(void* buf, std::size_t count)
	{
		if (_fileStream && isReadable())
		{
			_fileStream->read(static_cast<char*>(buf), count);
			return _fileStream->gcount();
		}

		return 0;
	}

	std::size_t FileStream::write(const void* buf, size_t count)
	{
		std::size_t writeCount = 0;
		if (_fileStream && isWriteable())
		{
			_fileStream->write(static_cast<const char*>(buf), static_cast<std::streamsize>(count));
			writeCount = count;
		}

		return writeCount;
	}

	void FileStream::skip(std::size_t count)
	{
		if (_fileStream)
		{
			_fileStream->clear();
			_fileStream->seekg(count, std::ios::cur);
		}
	}

	void FileStream::seek(std::size_t pos)
	{
		if (_fileStream)
		{
			_fileStream->clear();
			_fileStream->seekg(pos, std::ios::beg);
		}
	}

	std::size_t FileStream::tell() const
	{
		if (_fileStream)
		{
			_fileStream->clear();
			return _fileStream->tellg();
		}

		return 0;
	}

	bool FileStream::eof() const
	{
		return _fileStream && _fileStream->eof();
	}

	void FileStream::close()
	{
		if (_fileStream)
		{
			_fileStream->close();
			delete _fileStream;
			_fileStream = nullptr;
		}
	}
}