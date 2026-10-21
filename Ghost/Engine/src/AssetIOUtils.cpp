#include "AssetIOUtils.h"

#include <cstdio>
#include <sstream>

#include "BinaryArchive.h"

namespace ghost
{
    namespace AssetIO
    {
        std::string FormatFloat(float value)
        {
            char buffer[32] = {};
            std::snprintf(buffer, sizeof(buffer), "%.9g", value);
            return buffer;
        }

        bool ParseFloat(const char* text, float& value)
        {
            if (!text)
                return false;

            std::istringstream stream(text);
            if (!(stream >> value))
                return false;

            stream >> std::ws;
            return stream.eof();
        }

        float ParseFloatOrDefault(const char* text, float fallback)
        {
            float value = fallback;
            return ParseFloat(text, value) ? value : fallback;
        }

        bool ParseUInt(const char* text, unsigned& value)
        {
            if (!text)
                return false;

            std::istringstream stream(text);
            if (!(stream >> value))
                return false;

            stream >> std::ws;
            return stream.eof();
        }

        unsigned ParseUIntOrDefault(const char* text, unsigned fallback)
        {
            unsigned value = fallback;
            return ParseUInt(text, value) ? value : fallback;
        }

        bool ParseFloatList(const char* text, std::vector<float>& values)
        {
            values.clear();

            if (!text)
                return true;

            std::istringstream stream(text);
            float value = 0.0f;
            while (stream >> value)
                values.push_back(value);

            return stream.eof();
        }

        bool ParseUIntList(const char* text, std::vector<unsigned>& values)
        {
            values.clear();

            if (!text)
                return true;

            std::istringstream stream(text);
            unsigned value = 0;
            while (stream >> value)
                values.push_back(value);

            return stream.eof();
        }

        std::string FormatFloatList(const std::vector<float>& values)
        {
            std::ostringstream stream;
            for (std::size_t index = 0; index < values.size(); ++index)
            {
                if (index != 0)
                    stream << ' ';
                stream << FormatFloat(values[index]);
            }

            return stream.str();
        }

        std::string FormatUIntList(const std::vector<unsigned>& values)
        {
            std::ostringstream stream;
            for (std::size_t index = 0; index < values.size(); ++index)
            {
                if (index != 0)
                    stream << ' ';
                stream << values[index];
            }

            return stream.str();
        }

        std::string FormatVector3(const Vector3f& value)
        {
            return FormatFloat(value._x) + " " + FormatFloat(value._y) + " " + FormatFloat(value._z);
        }

        bool ParseVector3(const char* text, Vector3f& value)
        {
            if (!text)
                return false;

            std::istringstream stream(text);
            Vector3f result;
            if (!(stream >> result._x >> result._y >> result._z))
                return false;

            value = result;
            return true;
        }

        Vector3f ParseVector3OrDefault(const char* text, const Vector3f& fallback)
        {
            Vector3f value = fallback;
            return ParseVector3(text, value) ? value : fallback;
        }

        std::string FormatColor(const Color& value)
        {
            return FormatFloat(value._r) + " " + FormatFloat(value._g) + " " +
                FormatFloat(value._b) + " " + FormatFloat(value._a);
        }

        bool ParseColor(const char* text, Color& value)
        {
            if (!text)
                return false;

            std::istringstream stream(text);
            Color result = value;
            if (!(stream >> result._r >> result._g >> result._b >> result._a))
                return false;

            value = result;
            return true;
        }

        Color ParseColorOrDefault(const char* text, const Color& fallback)
        {
            Color value = fallback;
            return ParseColor(text, value) ? value : fallback;
        }

        std::string FormatMatrix4x4(const Matrix4x4f& value)
        {
            std::ostringstream stream;
            for (int index = 0; index < 16; ++index)
            {
                if (index != 0)
                    stream << ' ';
                stream << FormatFloat(value._m[index]);
            }

            return stream.str();
        }

        bool ParseMatrix4x4(const char* text, Matrix4x4f& value)
        {
            if (!text)
                return true;

            std::istringstream stream(text);
            Matrix4x4f result = value;
            for (int index = 0; index < 16; ++index)
            {
                if (!(stream >> result._m[index]))
                    return false;
            }

            value = result;
            return true;
        }

        void WriteVector3(BinaryWriter& writer, const Vector3f& value)
        {
            writer.WriteF32(value._x);
            writer.WriteF32(value._y);
            writer.WriteF32(value._z);
        }

        bool ReadVector3(BinaryReader& reader, Vector3f& value)
        {
            return reader.ReadF32(value._x) &&
                reader.ReadF32(value._y) &&
                reader.ReadF32(value._z);
        }

        void WriteColor(BinaryWriter& writer, const Color& value)
        {
            writer.WriteF32(value._r);
            writer.WriteF32(value._g);
            writer.WriteF32(value._b);
            writer.WriteF32(value._a);
        }

        bool ReadColor(BinaryReader& reader, Color& value)
        {
            return reader.ReadF32(value._r) &&
                reader.ReadF32(value._g) &&
                reader.ReadF32(value._b) &&
                reader.ReadF32(value._a);
        }

        void WriteMatrix4x4(BinaryWriter& writer, const Matrix4x4f& value)
        {
            for (int index = 0; index < 16; ++index)
                writer.WriteF32(value._m[index]);
        }

        bool ReadMatrix4x4(BinaryReader& reader, Matrix4x4f& value)
        {
            for (int index = 0; index < 16; ++index)
            {
                if (!reader.ReadF32(value._m[index]))
                    return false;
            }

            return true;
        }
    }
}
