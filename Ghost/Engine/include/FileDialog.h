#ifndef _FILE_DIALOG_H_
#define _FILE_DIALOG_H_

#include <string>
#include <vector>

#include "Ghost.h"

namespace ghost
{
    struct FileDialogFilter
    {
        const char* _name;
        const char* _pattern;
    };

    class GHOST_API FileDialog
    {
    public:
        static bool OpenFile(void* ownerHandle, const std::vector<FileDialogFilter>& filters,
            bool multiSelect, std::vector<std::string>& outPaths,
            const std::string& initialDir = std::string());

        static bool SaveFile(void* ownerHandle, const std::vector<FileDialogFilter>& filters,
            const std::string& defaultName, const std::string& defaultExtension,
            std::string& outPath, const std::string& initialDir = std::string());
    };
}

#endif
