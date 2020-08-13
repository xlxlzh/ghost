#ifndef _FILESYSTEM_WIN32_H_
#define _FILESYSTEM_WIN32_H_

#include "FileSystem.h"

namespace ghost
{
    class GHOST_API FileSystemWin32 : public FileSystem
    {
    public:
        FileSystemWin32() = default;
        virtual ~FileSystemWin32() { }

        virtual bool setCurrentDir(const std::string& pathName) override;
        virtual bool createDir(const std::string& pathName) override;
        virtual std::string getCurrentDir() const override;

        virtual bool renameFile(const std::string& srcFileName, const std::string& dstFileName) override;
        virtual bool deleteFile(const std::string& fileName);

        virtual bool fileExists(const std::string& fileName) const override;
        virtual bool dirExists(const std::string& pathName) const override;

        virtual void scanDir(std::vector<std::string>& result, const std::string& pathName, const std::string& filter) override;
    };

    DECLAR_SMART_POINTER(FileSystemWin32);
}

#endif
