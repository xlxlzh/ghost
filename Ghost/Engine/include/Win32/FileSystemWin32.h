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

        virtual bool SetCurrentDir(const std::string& pathName) override;
        virtual bool CreateDir(const std::string& pathName) override;
        virtual std::string GetCurrentDir() const override;

        virtual bool RenameFile(const std::string& srcFileName, const std::string& dstFileName) override;
        virtual bool DeleteFileByName(const std::string& fileName) override;

        virtual bool FileExists(const std::string& fileName) const override;
        virtual bool DirExists(const std::string& pathName) const override;

        virtual void ScanDir(std::vector<std::string>& result, const std::string& pathName, const std::string& filter) override;
    };

    DECLAR_SMART_POINTER(FileSystemWin32);
}

#endif
