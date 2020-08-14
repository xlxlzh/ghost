#include "FileSystem.h"
#include <algorithm>
#include <cctype>

namespace ghost
{
    std::string GetNativePath(const std::string& pathName)
    {
#ifdef _WIN32
        std::string newPath = pathName;
        std::replace(newPath.begin(), newPath.end(), '/', '\\');
        return newPath;
#else
        return pathName
#endif // _WIN32
    }

    std::string GetInternalPath(const std::string& pathName)
    {
        std::string newPath = pathName;
        std::replace(newPath.begin(), newPath.end(), '\\', '/');
        return newPath;
    }

    std::string AddTrailingSlash(const std::string& pathName)
    {
        std::string newPath = pathName;
        newPath.erase(newPath.begin(), std::find_if(newPath.begin(), newPath.end(),
        [](int c)->bool
        {
            return !std::isspace(c);
        }
        ));

        newPath.erase(std::find_if(newPath.rbegin(), newPath.rend(),
        [](int c)->bool
        {
            return !std::isspace(c);
        }
        ).base(), newPath.end());

        std::replace(newPath.begin(), newPath.end(), '\\', '/');
        if (!newPath.empty() && newPath.back() != '/')
            newPath += '/';

        return newPath;
    }

    std::string RemoveTrailingSlash(const std::string& pathName)
    {
        std::string newPath = pathName;
        newPath.erase(newPath.begin(), std::find_if(newPath.begin(), newPath.end(),
            [](int c)->bool
        {
            return !std::isspace(c);
        }
        ));

        newPath.erase(std::find_if(newPath.rbegin(), newPath.rend(),
            [](int c)->bool
        {
            return !std::isspace(c);
        }
        ).base(), newPath.end());

        std::replace(newPath.begin(), newPath.end(), '\\', '/');
        if (!newPath.empty() && newPath.back() == '/')
            newPath.pop_back();

        return newPath;
    }

    std::string GetExtension(const std::string& fullPath, bool lowercase /* = true */)
    {
        std::string ext;

        auto extPos = fullPath.find_last_of('.');
        if (extPos != std::string::npos)
        {
            ext = fullPath.substr(extPos);
            if (lowercase)
                std::transform(ext.begin(), ext.end(), ext.begin(), std::tolower);
        }

        return ext;
    }
}