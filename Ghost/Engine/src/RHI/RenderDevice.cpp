#include "RenderDevice.h"

#include <algorithm>
#include <cstdint>
#include <utility>

namespace ghost
{
    namespace
    {
        std::uint64_t HashBytes(std::uint64_t hash, const void* data, std::size_t size)
        {
            const unsigned char* bytes = static_cast<const unsigned char*>(data);
            for (std::size_t index = 0; index < size; ++index)
            {
                hash ^= bytes[index];
                hash *= 1099511628211ull;
            }
            return hash;
        }

        std::uint64_t HashString(std::uint64_t hash, const std::string& value)
        {
            const std::uint64_t size = value.size();
            hash = HashBytes(hash, &size, sizeof(size));
            return HashBytes(hash, value.data(), value.size());
        }

        std::string MakeCompiledShaderKey(const ShaderResourcePtr& source,
            const std::vector<ShaderEntryDefinition>& entries,
            const std::unordered_map<std::string, std::string>& defines)
        {
            std::uint64_t hash = 14695981039346656037ull;
            hash = HashString(hash, source->GetName());
            hash = HashBytes(hash, source->GetRawdata(), static_cast<std::size_t>(source->GetRawdataSize()));

            std::vector<std::pair<std::string, std::string>> sortedDefines(defines.begin(), defines.end());
            std::sort(sortedDefines.begin(), sortedDefines.end(),
                [](const std::pair<std::string, std::string>& lhs,
                    const std::pair<std::string, std::string>& rhs)
                {
                    return lhs.first < rhs.first;
                });

            for (const std::pair<std::string, std::string>& define : sortedDefines)
            {
                hash = HashString(hash, define.first);
                hash = HashString(hash, define.second);
            }

            std::vector<ShaderEntryDefinition> sortedEntries(entries);
            std::sort(sortedEntries.begin(), sortedEntries.end(),
                [](const ShaderEntryDefinition& lhs, const ShaderEntryDefinition& rhs)
                {
                    if (lhs.type != rhs.type)
                        return lhs.type < rhs.type;
                    return lhs.entry < rhs.entry;
                });

            for (const ShaderEntryDefinition& entry : sortedEntries)
            {
                hash = HashBytes(hash, &entry.type, sizeof(entry.type));
                hash = HashString(hash, entry.entry);
            }

            return source->GetName() + ":" + std::to_string(hash);
        }
    }

    RenderTargetPtr RenderDevice::CreateSingleRenderTarget(unsigned w, unsigned h, GhostColorFormat format, bool srv, bool msaa, bool depth /* = true */)
    {
        return CreateRenderTargets(w, h, 1, &format, srv, msaa, depth);
    }

    CompiledShaderPtr RenderDevice::GetOrCreateCompiledShader(const ShaderResourcePtr& source,
        const std::vector<ShaderEntryDefinition>& entries,
        const std::unordered_map<std::string, std::string>& defines)
    {
        if (!source || !source->GetRawdata() || source->GetRawdataSize() <= 0 || entries.empty())
            return nullptr;

        const std::string key = MakeCompiledShaderKey(source, entries, defines);

        auto found = _compiledShaders.find(key);
        if (found != _compiledShaders.end())
            return found->second;

        CompiledShaderPtr compiledShader = GHOST_MAKE_SMART_POINTER(CompiledShader);
        for (const ShaderEntryDefinition& entry : entries)
        {
            if (entry.type >= SHADER_TYPE_NUM || entry.entry.empty())
                return nullptr;

            if (!CompileShader(entry.type, entry.entry.c_str(), defines, *source, *compiledShader))
                return nullptr;
        }

        _compiledShaders[key] = compiledShader;
        return compiledShader;
    }
}