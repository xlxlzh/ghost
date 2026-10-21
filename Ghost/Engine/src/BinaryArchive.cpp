#include "BinaryArchive.h"

#include <algorithm>
#include <cstring>
#include <limits>

#include "LogManager.h"

namespace ghost
{
    namespace
    {
        constexpr std::size_t kArrayChunkSize = 1024;

        void EncodeU16(uint8_t* out, uint16_t value)
        {
            out[0] = static_cast<uint8_t>(value & 0xffu);
            out[1] = static_cast<uint8_t>((value >> 8) & 0xffu);
        }

        void EncodeU32(uint8_t* out, uint32_t value)
        {
            out[0] = static_cast<uint8_t>(value & 0xffu);
            out[1] = static_cast<uint8_t>((value >> 8) & 0xffu);
            out[2] = static_cast<uint8_t>((value >> 16) & 0xffu);
            out[3] = static_cast<uint8_t>((value >> 24) & 0xffu);
        }

        uint16_t DecodeU16(const uint8_t* in)
        {
            return static_cast<uint16_t>(in[0])
                | static_cast<uint16_t>(in[1] << 8);
        }

        uint32_t DecodeU32(const uint8_t* in)
        {
            return static_cast<uint32_t>(in[0])
                | (static_cast<uint32_t>(in[1]) << 8)
                | (static_cast<uint32_t>(in[2]) << 16)
                | (static_cast<uint32_t>(in[3]) << 24);
        }
    }

    BinaryWriter::BinaryWriter(DataStream& stream) : _stream(stream)
    {
    }

    void BinaryWriter::Fail(const std::string& message)
    {
        if (_ok)
        {
            _ok = false;
            _error = message;
        }
    }

    void BinaryWriter::WriteRaw(const void* data, std::size_t count)
    {
        if (!_ok || count == 0)
            return;

        if (!data)
        {
            Fail("attempted to write a null buffer.");
            return;
        }

        if (_stream.Write(data, count) != count)
        {
            Fail("failed to write to stream '" + _stream.GetName() + "'.");
        }
    }

    void BinaryWriter::WriteU8(uint8_t value)
    {
        WriteRaw(&value, sizeof(value));
    }

    void BinaryWriter::WriteU16(uint16_t value)
    {
        uint8_t buffer[2] = {};
        EncodeU16(buffer, value);
        WriteRaw(buffer, sizeof(buffer));
    }

    void BinaryWriter::WriteU32(uint32_t value)
    {
        uint8_t buffer[4] = {};
        EncodeU32(buffer, value);
        WriteRaw(buffer, sizeof(buffer));
    }

    void BinaryWriter::WriteI32(int32_t value)
    {
        WriteU32(static_cast<uint32_t>(value));
    }

    void BinaryWriter::WriteF32(float value)
    {
        uint32_t bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));
        WriteU32(bits);
    }

    void BinaryWriter::WriteBool(bool value)
    {
        WriteU8(value ? 1u : 0u);
    }

    void BinaryWriter::WriteString(const std::string& value)
    {
        if (!_ok)
            return;

        if (value.size() > std::numeric_limits<uint32_t>::max())
        {
            Fail("string is too large to serialize.");
            return;
        }

        WriteU32(static_cast<uint32_t>(value.size()));
        WriteRaw(value.data(), value.size());
    }

    void BinaryWriter::WriteFloatArray(const float* values, std::size_t count)
    {
        if (!_ok || count == 0)
            return;

        if (!values)
        {
            Fail("attempted to write a null float array.");
            return;
        }

        uint8_t buffer[kArrayChunkSize * sizeof(float)] = {};
        std::size_t offset = 0;
        while (offset < count)
        {
            const std::size_t chunk = std::min(kArrayChunkSize, count - offset);
            for (std::size_t index = 0; index < chunk; ++index)
            {
                uint32_t bits = 0;
                std::memcpy(&bits, &values[offset + index], sizeof(bits));
                EncodeU32(buffer + index * sizeof(float), bits);
            }

            WriteRaw(buffer, chunk * sizeof(float));
            offset += chunk;
        }
    }

    void BinaryWriter::WriteU32Array(const unsigned* values, std::size_t count)
    {
        if (!_ok || count == 0)
            return;

        if (!values)
        {
            Fail("attempted to write a null index array.");
            return;
        }

        uint8_t buffer[kArrayChunkSize * sizeof(uint32_t)] = {};
        std::size_t offset = 0;
        while (offset < count)
        {
            const std::size_t chunk = std::min(kArrayChunkSize, count - offset);
            for (std::size_t index = 0; index < chunk; ++index)
            {
                EncodeU32(buffer + index * sizeof(uint32_t),
                    static_cast<uint32_t>(values[offset + index]));
            }

            WriteRaw(buffer, chunk * sizeof(uint32_t));
            offset += chunk;
        }
    }

    void BinaryWriter::WriteBytes(const void* data, std::size_t count)
    {
        WriteRaw(data, count);
    }

    BinaryReader::BinaryReader(DataStream& stream) : _stream(stream)
    {
    }

    bool BinaryReader::Fail(const std::string& message)
    {
        if (_ok)
        {
            _ok = false;
            _error = message;
        }

        return false;
    }

    std::size_t BinaryReader::RemainingBytes() const
    {
        if (!_stream.IsReadable())
            return 0;

        const std::size_t size = _stream.GetSize();
        const std::size_t position = _stream.Tell();
        return size > position ? size - position : 0;
    }

    bool BinaryReader::EnsureRemaining(std::size_t count, const char* what)
    {
        if (!_ok)
            return false;

        if (count > RemainingBytes())
        {
            return Fail(std::string("not enough bytes remaining for ") + what + ".");
        }

        return true;
    }

    bool BinaryReader::ReadRaw(void* data, std::size_t count)
    {
        if (!_ok)
            return false;

        if (count == 0)
            return true;

        if (!data)
            return Fail("attempted to read into a null buffer.");

        if (_stream.Read(data, count) != count)
        {
            return Fail("failed to read from stream '" + _stream.GetName() + "'.");
        }

        return true;
    }

    bool BinaryReader::ReadU8(uint8_t& value)
    {
        return ReadRaw(&value, sizeof(value));
    }

    bool BinaryReader::ReadU16(uint16_t& value)
    {
        uint8_t buffer[2] = {};
        if (!ReadRaw(buffer, sizeof(buffer)))
            return false;

        value = DecodeU16(buffer);
        return true;
    }

    bool BinaryReader::ReadU32(uint32_t& value)
    {
        uint8_t buffer[4] = {};
        if (!ReadRaw(buffer, sizeof(buffer)))
            return false;

        value = DecodeU32(buffer);
        return true;
    }

    bool BinaryReader::ReadI32(int32_t& value)
    {
        uint32_t raw = 0;
        if (!ReadU32(raw))
            return false;

        value = static_cast<int32_t>(raw);
        return true;
    }

    bool BinaryReader::ReadF32(float& value)
    {
        uint32_t bits = 0;
        if (!ReadU32(bits))
            return false;

        std::memcpy(&value, &bits, sizeof(value));
        return true;
    }

    bool BinaryReader::ReadBool(bool& value)
    {
        uint8_t raw = 0;
        if (!ReadU8(raw))
            return false;

        if (raw > 1)
            return Fail("invalid boolean value in binary asset.");

        value = raw != 0;
        return true;
    }

    bool BinaryReader::ReadString(std::string& value, uint32_t maxLength)
    {
        uint32_t length = 0;
        if (!ReadU32(length))
            return false;

        if (length > maxLength)
            return Fail("string exceeds the maximum allowed length.");

        if (!EnsureRemaining(length, "string"))
            return false;

        value.resize(length);
        if (length > 0 && !ReadRaw(&value[0], length))
            return false;

        return true;
    }

    bool BinaryReader::ReadFloatArray(std::vector<float>& values, uint32_t count)
    {
        static_assert(sizeof(float) == sizeof(uint32_t), "float must be 32-bit.");

        if (count > std::numeric_limits<std::size_t>::max() / sizeof(float))
            return Fail("float array is too large.");

        const std::size_t byteCount = static_cast<std::size_t>(count) * sizeof(float);
        if (!EnsureRemaining(byteCount, "float array"))
            return false;

        values.resize(count);

        uint8_t buffer[kArrayChunkSize * sizeof(float)] = {};
        std::size_t offset = 0;
        while (offset < count)
        {
            const std::size_t chunk = std::min(kArrayChunkSize,
                static_cast<std::size_t>(count) - offset);
            if (!ReadRaw(buffer, chunk * sizeof(float)))
                return false;

            for (std::size_t index = 0; index < chunk; ++index)
            {
                const uint32_t bits = DecodeU32(buffer + index * sizeof(float));
                std::memcpy(&values[offset + index], &bits, sizeof(float));
            }

            offset += chunk;
        }

        return true;
    }

    bool BinaryReader::ReadU32Array(std::vector<unsigned>& values, uint32_t count)
    {
        static_assert(sizeof(unsigned) == sizeof(uint32_t), "unsigned must be 32-bit.");

        if (count > std::numeric_limits<std::size_t>::max() / sizeof(uint32_t))
            return Fail("index array is too large.");

        const std::size_t byteCount = static_cast<std::size_t>(count) * sizeof(uint32_t);
        if (!EnsureRemaining(byteCount, "index array"))
            return false;

        values.resize(count);

        uint8_t buffer[kArrayChunkSize * sizeof(uint32_t)] = {};
        std::size_t offset = 0;
        while (offset < count)
        {
            const std::size_t chunk = std::min(kArrayChunkSize,
                static_cast<std::size_t>(count) - offset);
            if (!ReadRaw(buffer, chunk * sizeof(uint32_t)))
                return false;

            for (std::size_t index = 0; index < chunk; ++index)
                values[offset + index] = static_cast<unsigned>(DecodeU32(buffer + index * sizeof(uint32_t)));

            offset += chunk;
        }

        return true;
    }

    bool BinaryReader::ReadBytes(void* data, std::size_t count)
    {
        return ReadRaw(data, count);
    }

    AssetEncoding DetectAssetEncoding(DataStream& stream)
    {
        const std::size_t start = stream.Tell();

        uint8_t magicBuffer[4] = {};
        const std::size_t magicRead = stream.Read(magicBuffer, sizeof(magicBuffer));
        stream.Seek(start);

        if (magicRead == sizeof(magicBuffer))
        {
            const uint32_t magic = DecodeU32(magicBuffer);
            if (magic == AssetFormat::GRES_MAGIC)
                return AssetEncoding::Gres;

            if (magic == AssetFormat::GMAP_MAGIC)
                return AssetEncoding::Gmap;
        }

        uint8_t textBuffer[64] = {};
        const std::size_t textRead = stream.Read(textBuffer, sizeof(textBuffer));
        stream.Seek(start);

        std::size_t index = 0;
        if (textRead >= 3 &&
            textBuffer[0] == 0xef && textBuffer[1] == 0xbb && textBuffer[2] == 0xbf)
        {
            index = 3;
        }

        while (index < textRead)
        {
            const uint8_t value = textBuffer[index];
            if (value != ' ' && value != '\t' && value != '\r' && value != '\n')
                break;
            ++index;
        }

        if (index < textRead && textBuffer[index] == '<')
            return AssetEncoding::Xml;

        return AssetEncoding::Unknown;
    }

    bool WriteAssetHeader(BinaryWriter& writer, uint32_t magic,
        uint16_t assetType, uint16_t version)
    {
        writer.WriteU32(magic);
        writer.WriteU16(version);
        writer.WriteU16(assetType);
        writer.WriteU32(AssetFormat::ENDIAN_MARKER);
        return writer.IsOk();
    }

    bool ReadAssetHeader(BinaryReader& reader, uint32_t expectedMagic,
        uint16_t expectedAssetType, uint16_t expectedVersion)
    {
        uint32_t magic = 0;
        uint16_t version = 0;
        uint16_t assetType = 0;
        uint32_t endian = 0;

        if (!reader.ReadU32(magic) || !reader.ReadU16(version) ||
            !reader.ReadU16(assetType) || !reader.ReadU32(endian))
        {
            return false;
        }

        if (magic != expectedMagic)
        {
            GHOST_LOG_FORMAT_ERROR("Binary asset magic mismatch.");
            return false;
        }

        if (assetType != expectedAssetType)
        {
            GHOST_LOG_FORMAT_ERROR("Binary asset type mismatch: expected %u, got %u.",
                static_cast<unsigned>(expectedAssetType), static_cast<unsigned>(assetType));
            return false;
        }

        if (endian != AssetFormat::ENDIAN_MARKER)
        {
            GHOST_LOG_FORMAT_ERROR("Binary asset endian marker mismatch.");
            return false;
        }

        if (version != expectedVersion)
        {
            GHOST_LOG_FORMAT_ERROR("Unsupported binary asset version %u (expected %u).",
                static_cast<unsigned>(version), static_cast<unsigned>(expectedVersion));
            return false;
        }

        return true;
    }

    bool ReadStreamAsText(DataStream& stream, std::string& text)
    {
        const std::size_t size = stream.GetSize();
        if (size == 0)
        {
            text.clear();
            return false;
        }

        text.resize(size);
        if (stream.Read(&text[0], size) != size)
        {
            text.clear();
            return false;
        }

        return true;
    }
}
