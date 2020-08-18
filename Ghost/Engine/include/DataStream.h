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

		const std::string& getName() { return _name; }
		AccessMode getAccessMode() const { return _accessMode; }

		virtual bool isReadable() const { return (_accessMode & AM_READ) != 0; }
		virtual bool isWriteable() const { return (_accessMode & AM_WRITE) != 0; }

		virtual std::size_t read(void* buf, std::size_t count) = 0;

		virtual std::size_t write(const void* buf, size_t count)
		{
			(void)buf;
			(void)count;
			return 0;
		}

		virtual void skip(std::size_t count) = 0;

		virtual void seek(std::size_t pos) = 0;
		virtual std::size_t tell() const = 0;

		virtual bool eof() const = 0;
		virtual void close() = 0;

        virtual bool isOpened() const = 0;

		std::size_t getSize() const { return _size; }

	protected:
		AccessMode _accessMode;
		std::string _name;
		unsigned _size;
	};

	class GHOST_API MemoryDataStream : public DataStream
	{
	public:
		MemoryDataStream(void* mem, std::size_t size, bool freeOnClose = false, bool readonly = false);
		MemoryDataStream(const std::string& name, void* mem, std::size_t size, bool freeOnClose = false, bool readonly = false);

		uchar* getPtr() { return _data; }
		uchar* getCurrentPtr() { return _pos; }

		virtual std::size_t read(void* buf, std::size_t count) override;
		virtual std::size_t write(const void* buf, size_t count) override;

		virtual void skip(std::size_t count) override;

		virtual void seek(std::size_t pos) override;
		virtual std::size_t tell() const override;

		virtual bool eof() const override;
		virtual void close() override;

        virtual bool isOpened() const override;

	protected:
		uchar* _data;
		uchar* _pos;
		uchar* _end;

		bool _freeOnClose;
	};

	class GHOST_API FileStream : public DataStream
	{
	public:
		FileStream(const std::string& fileName, AccessMode mode = AccessMode::AM_READ);

		virtual std::size_t read(void* buf, std::size_t count) override;
		virtual std::size_t write(const void* buf, size_t count) override;

		virtual void skip(std::size_t count) override;

		virtual void seek(std::size_t pos) override;
		virtual std::size_t tell() const override;

		virtual bool eof() const override;
		virtual void close() override;

        virtual bool isOpened() const override;

	protected:
		unsigned getStreamFlag(AccessMode mode);

	protected:
		std::fstream* _fileStream;
	};
}

#endif