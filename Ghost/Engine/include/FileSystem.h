#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "Ghost.h"
#include <string>
#include <vector>

namespace ghost
{
    class GHOST_API FileSystem
    {
    public:
        FileSystem() = default;
        virtual ~FileSystem() { }

        virtual bool SetCurrentDir(const std::string& pathName) = 0;
        virtual std::string GetCurrentDir() const = 0;
        virtual bool CreateDir(const std::string& pathName) = 0;

        virtual bool RenameFile(const std::string& srcFileName, const std::string& dstFileName) = 0;
        virtual bool DeleteFileByName(const std::string& fileName) = 0;

        virtual bool FileExists(const std::string& fileName) const = 0;
        virtual bool DirExists(const std::string& pathName) const = 0;

        virtual void ScanDir(std::vector<std::string>& result, const std::string& pathName, const std::string& filter) = 0;
    };

    DECLAR_SMART_POINTER(FileSystem);

    GHOST_API std::string GetExtension(const std::string& fullPath, bool lowercase = true);
    GHOST_API std::string GetNativePath(const std::string& pathName);
    GHOST_API std::string GetInternalPath(const std::string& pathName);
    GHOST_API std::string AddTrailingSlash(const std::string& pathName);
    GHOST_API std::string RemoveTrailingSlash(const std::string& pathName);
}

#endif
