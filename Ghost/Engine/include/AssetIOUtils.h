#ifndef _ASSET_IO_UTILS_H_
#define _ASSET_IO_UTILS_H_

#include <string>
#include <vector>

#include "Ghost.h"
#include "Vector3.h"
#include "Color.h"
#include "Matrix4x4.h"

namespace ghost
{
    class BinaryWriter;
    class BinaryReader;

    namespace AssetIO
    {
        GHOST_API std::string FormatFloat(float value);
        GHOST_API bool ParseFloat(const char* text, float& value);
        GHOST_API float ParseFloatOrDefault(const char* text, float fallback);
        GHOST_API bool ParseUInt(const char* text, unsigned& value);
        GHOST_API unsigned ParseUIntOrDefault(const char* text, unsigned fallback);

        GHOST_API bool ParseFloatList(const char* text, std::vector<float>& values);
        GHOST_API bool ParseUIntList(const char* text, std::vector<unsigned>& values);
        GHOST_API std::string FormatFloatList(const std::vector<float>& values);
        GHOST_API std::string FormatUIntList(const std::vector<unsigned>& values);

        GHOST_API std::string FormatVector3(const Vector3f& value);
        GHOST_API bool ParseVector3(const char* text, Vector3f& value);
        GHOST_API Vector3f ParseVector3OrDefault(const char* text, const Vector3f& fallback);
        GHOST_API std::string FormatColor(const Color& value);
        GHOST_API bool ParseColor(const char* text, Color& value);
        GHOST_API Color ParseColorOrDefault(const char* text, const Color& fallback);
        GHOST_API std::string FormatMatrix4x4(const Matrix4x4f& value);
        GHOST_API bool ParseMatrix4x4(const char* text, Matrix4x4f& value);

        GHOST_API void WriteVector3(BinaryWriter& writer, const Vector3f& value);
        GHOST_API bool ReadVector3(BinaryReader& reader, Vector3f& value);
        GHOST_API void WriteColor(BinaryWriter& writer, const Color& value);
        GHOST_API bool ReadColor(BinaryReader& reader, Color& value);
        GHOST_API void WriteMatrix4x4(BinaryWriter& writer, const Matrix4x4f& value);
        GHOST_API bool ReadMatrix4x4(BinaryReader& reader, Matrix4x4f& value);
    }
}

#endif
