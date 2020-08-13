#include "FileSystem.h"
#include <algorithm>

namespace ghost
{
    std::string GetNativePath(const std::string& pathName)
    {
#ifdef _WIN32
        std::string newPath = pathName;
        std::replace(newPath.begin(), newPath.end(), '\\', '/');
        return newPath;
#else
        return pathName
#endif // _WIN32
    }

    std::string GetExtension(const std::string& fullPath, bool lowercase /* = true */)
    {
        return std::string();
    }
}