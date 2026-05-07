#include <filesystem>
#include "FileSystemStd.h"
#include "LogManager.h"

namespace ghost
{
    namespace fs = std::filesystem;
    
    bool FileSystemStd::SetCurrentDir(const std::string& pathName)
    {
        fs::current_path(GetNativePath(pathName).c_str());
        return true;
    }
    
    std::string FileSystemStd::GetCurrentDir() const
    {
        return fs::current_path().string();
    }
    
    bool FileSystemStd::CreateDir(const std::string& pathName)
    {
        return fs::create_directory(GetNativePath(pathName).c_str());
    }
    
    bool FileSystemStd::RenameFile(const std::string& srcFileName, const std::string& dstFileName)
    {
        fs::rename(GetNativePath(srcFileName).c_str(), GetNativePath(dstFileName).c_str());
        return true;
    }
    
    bool FileSystemStd::DeleteFileByName(const std::string& fileName)
    {
        return fs::remove(GetNativePath(fileName).c_str());
    }
    
    bool FileSystemStd::FileExists(const std::string& fileName) const
    {
        return fs::exists(GetNativePath(fileName).c_str());
    }
    
    bool FileSystemStd::DirExists(const std::string& pathName) const
    {
        return fs::exists(pathName.c_str());
    }
    
    void FileSystemStd::ScanDir(std::vector<std::string>& result, const std::string& pathName, const std::string& filter)
    {
        //TODO
    }
}
