#ifndef _BINARY_ARCHIVE_H_
#define _BINARY_ARCHIVE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "Ghost.h"
#include "DataStream.h"

namespace ghost
{
    namespace AssetFormat
    {
        constexpr uint32_t GRES_MAGIC = 0x53455247u;
        constexpr uint32_t GMAP_MAGIC = 0x50414D47u;
        constexpr uint32_t ENDIAN_MARKER = 0x01020304u;

        constexpr uint16_t MODEL_VERSION = 1;
        constexpr uint16_t MATERIAL_VERSION = 1;
        constexpr uint16_t SCENE_VERSION = 1;

        constexpr const char* GRES_EXTENSION = ".gres";
        constexpr const char* GMAP_EXTENSION = ".gmap";
        constexpr const char* XML_EXTENSION = ".xml";
    }

    enum class AssetEncoding
    {
        Xml,
        Gres,
        Gmap,
        Unknown
    };

    class GHOST_API BinaryWriter
    {
    public:
        explicit BinaryWriter(DataStream& stream);

        void WriteU8(uint8_t value);
        void WriteU16(uint16_t value);
        void WriteU32(uint32_t value);
        void WriteI32(int32_t value);
        void WriteF32(float value);
        void WriteBool(bool value);
        void WriteString(const std::string& value);
        void WriteFloatArray(const float* values, std::size_t count);
        void WriteU32Array(const unsigned* values, std::size_t count);
        void WriteBytes(const void* data, std::size_t count);

        bool IsOk() const { return _ok; }
        const std::string& GetError() const { return _error; }

    private:
        void Fail(const std::string& message);
        void WriteRaw(const void* data, std::size_t count);

        DataStream& _stream;
        bool _ok = true;
        std::string _error;
    };

    class GHOST_API BinaryReader
    {
    public:
        explicit BinaryReader(DataStream& stream);

        bool ReadU8(uint8_t& value);
        bool ReadU16(uint16_t& value);
        bool ReadU32(uint32_t& value);
        bool ReadI32(int32_t& value);
        bool ReadF32(float& value);
        bool ReadBool(bool& value);
        bool ReadString(std::string& value, uint32_t maxLength);
        bool ReadFloatArray(std::vector<float>& values, uint32_t count);
        bool ReadU32Array(std::vector<unsigned>& values, uint32_t count);
        bool ReadBytes(void* data, std::size_t count);

        bool IsOk() const { return _ok; }
        const std::string& GetError() const { return _error; }
        std::size_t RemainingBytes() const;

    private:
        bool Fail(const std::string& message);
        bool ReadRaw(void* data, std::size_t count);
        bool EnsureRemaining(std::size_t count, const char* what);

        DataStream& _stream;
        bool _ok = true;
        std::string _error;
    };

    GHOST_API AssetEncoding DetectAssetEncoding(DataStream& stream);

    GHOST_API bool WriteAssetHeader(BinaryWriter& writer, uint32_t magic,
        uint16_t assetType, uint16_t version);

    GHOST_API bool ReadAssetHeader(BinaryReader& reader, uint32_t expectedMagic,
        uint16_t expectedAssetType, uint16_t expectedVersion);

    GHOST_API bool ReadStreamAsText(DataStream& stream, std::string& text);
}

#endif
