#ifndef _DATA_STREAM_H_
#define _DATA_STREAM_H_

#include <fstream>
#include "Ghost.h"

namespace ghost
{
	enum AccessMode
	{
		AM_READ = 0x01,
		AM_WRITE = 0x02,
		AM_READ_WRITE = AM_READ | AM_WRITE
	};

	using uchar = unsigned char;

	class GHOST_API DataStream
	{
	public:
		DataStream(AccessMode mode = AccessMode::AM_READ) : _accessMode(mode), _size(0) { }
		DataStream(const std::string& name, AccessMode mode = AccessMode::AM_READ) : _accessMode(mode), _name(name), _size(0) { }

		virtual ~DataStream() { }

		const std::string& GetName() { return _name; }
		AccessMode GetAccessMode() const { return _accessMode; }

		virtual bool IsReadable() const { return (_accessMode & AM_READ) != 0; }
		virtual bool IsWriteable() const { return (_accessMode & AM_WRITE) != 0; }

		virtual std::size_t Read(void* buf, std::size_t count) = 0;

		virtual std::size_t Write(const void* buf, size_t count)
		{
			(void)buf;
			(void)count;
			return 0;
		}

		virtual void Skip(std::size_t count) = 0;

		virtual void Seek(std::size_t pos) = 0;
		virtual std::size_t Tell() const = 0;

		virtual bool Eof() const = 0;
		virtual void Close() = 0;

        virtual bool IsOpened() const = 0;

		std::size_t GetSize() const { return _size; }

	protected:
		AccessMode _accessMode;
		std::string _name;
		unsigned _size;
	};

    DECLAR_SMART_POINTER(DataStream);

	class GHOST_API MemoryDataStream : public DataStream
	{
	public:
		MemoryDataStream(void* mem, std::size_t size, bool freeOnClose = false, bool readonly = false);
		MemoryDataStream(const std::string& name, void* mem, std::size_t size, bool freeOnClose = false, bool readonly = false);

		uchar* GetPtr() { return _data; }
		uchar* GetCurrentPtr() { return _pos; }

		virtual std::size_t Read(void* buf, std::size_t count) override;
		virtual std::size_t Write(const void* buf, size_t count) override;

		virtual void Skip(std::size_t count) override;

		virtual void Seek(std::size_t pos) override;
		virtual std::size_t Tell() const override;

		virtual bool Eof() const override;
		virtual void Close() override;

        virtual bool IsOpened() const override;

	protected:
		uchar* _data;
		uchar* _pos;
		uchar* _end;

		bool _freeOnClose;
	};

    DECLAR_SMART_POINTER(MemoryDataStream);

	class GHOST_API FileStream : public DataStream
	{
	public:
		FileStream(const std::string& fileName, AccessMode mode = AccessMode::AM_READ);

		virtual std::size_t Read(void* buf, std::size_t count) override;
		virtual std::size_t Write(const void* buf, size_t count) override;

		virtual void Skip(std::size_t count) override;

		virtual void Seek(std::size_t pos) override;
		virtual std::size_t Tell() const override;

		virtual bool Eof() const override;
		virtual void Close() override;

        virtual bool IsOpened() const override;

	protected:
		unsigned GetStreamFlag(AccessMode mode);

	protected:
		std::fstream* _fileStream;
	};

    DECLAR_SMART_POINTER(FileStream);
}

#endif