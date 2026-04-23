#include "FileSystemWin32.h"
#include <windows.h>
#include "LogManager.h"

namespace ghost
{
    bool FileSystemWin32::SetCurrentDir(const std::string& pathName)
    {
        if (SetCurrentDirectoryA(GetNativePath(pathName).c_str()) == FALSE)
        {
            LogManager::GetInstance()->LogError("setCurrentDir failed.");
            return false;
        }

        return true;
    }

    bool FileSystemWin32::CreateDir(const std::string& pathName)
    {
        bool sucess = CreateDirectoryA(GetNativePath(pathName).c_str(), nullptr) ||
            (GetLastError() == ERROR_ALIAS_EXISTS);

        return sucess;
    }

    std::string FileSystemWin32::GetCurrentDir() const
    {
        char path[MAX_PATH] = { 0 };
        GetCurrentDirectoryA(MAX_PATH, path);
        return path;
    }

    bool FileSystemWin32::RenameFile(const std::string& srcFileName, const std::string& dstFileName)
    {
        return MoveFileA(GetNativePath(srcFileName).c_str(), GetNativePath(dstFileName).c_str());
    }

    bool FileSystemWin32::DeleteFileByName(const std::string& fileName)
    {
        return DeleteFileA(GetNativePath(fileName).c_str());
    }

    bool FileSystemWin32::FileExists(const std::string& fileName) const
    {
        std::string nativeName = GetNativePath(fileName);

        DWORD attributes = GetFileAttributesA(nativeName.c_str());
        if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
            return false;

        return true;
    }

    bool FileSystemWin32::DirExists(const std::string& pathName) const
    {
        std::string nativeName = GetNativePath(pathName);

        DWORD attributes = GetFileAttributesA(nativeName.c_str());
        if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
            return false;

        return true;
    }

    void FileSystemWin32::ScanDir(std::vector<std::string>& result, const std::string& pathName, const std::string& filter)
    {
        result.clear();
        //TODO
        return;
    }
}