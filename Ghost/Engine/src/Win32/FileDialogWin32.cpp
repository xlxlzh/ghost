#include "FileDialog.h"

#include <windows.h>
#include <shobjidl.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "uuid.lib")

namespace ghost
{
    namespace
    {
        class ComInitializer
        {
        public:
            ComInitializer()
            {
                const HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
                _initialized = SUCCEEDED(result);
            }

            ~ComInitializer()
            {
                if (_initialized)
                    CoUninitialize();
            }

        private:
            bool _initialized = false;
        };

        std::wstring ToWide(const std::string& value)
        {
            if (value.empty())
                return std::wstring();

            const int length = MultiByteToWideChar(CP_ACP, 0, value.c_str(), -1, nullptr, 0);
            if (length <= 1)
                return std::wstring();

            std::wstring result(static_cast<std::size_t>(length), L'\0');
            MultiByteToWideChar(CP_ACP, 0, value.c_str(), -1, &result[0], length);
            result.pop_back();
            return result;
        }

        std::string ToNarrow(const wchar_t* value)
        {
            if (!value)
                return std::string();

            const int length = WideCharToMultiByte(CP_ACP, 0, value, -1, nullptr, 0, nullptr, nullptr);
            if (length <= 1)
                return std::string();

            std::string result(static_cast<std::size_t>(length), '\0');
            WideCharToMultiByte(CP_ACP, 0, value, -1, &result[0], length, nullptr, nullptr);
            result.pop_back();
            return result;
        }

        void SetFilters(IFileDialog* dialog, const std::vector<FileDialogFilter>& filters,
            std::vector<std::wstring>& names, std::vector<std::wstring>& patterns)
        {
            names.clear();
            patterns.clear();

            std::vector<COMDLG_FILTERSPEC> specs;
            names.reserve(filters.size());
            patterns.reserve(filters.size());
            specs.reserve(filters.size());

            for (const FileDialogFilter& filter : filters)
            {
                names.push_back(ToWide(filter._name ? filter._name : ""));
                patterns.push_back(ToWide(filter._pattern ? filter._pattern : ""));
                specs.push_back(COMDLG_FILTERSPEC{ names.back().c_str(), patterns.back().c_str() });
            }

            if (!specs.empty())
                dialog->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());
        }

        void SetInitialFolder(IFileDialog* dialog, const std::string& initialDir)
        {
            if (initialDir.empty())
                return;

            const std::wstring wideDir = ToWide(initialDir);
            if (wideDir.empty())
                return;

            IShellItem* folder = nullptr;
            if (SUCCEEDED(SHCreateItemFromParsingName(wideDir.c_str(), nullptr, IID_PPV_ARGS(&folder))) && folder)
            {
                dialog->SetDefaultFolder(folder);
                folder->Release();
            }
        }

        void AppendPath(IShellItem* item, std::vector<std::string>& outPaths)
        {
            PWSTR filePath = nullptr;
            if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &filePath)) && filePath)
            {
                outPaths.push_back(ToNarrow(filePath));
                CoTaskMemFree(filePath);
            }
        }
    }

    bool FileDialog::OpenFile(void* ownerHandle, const std::vector<FileDialogFilter>& filters,
        bool multiSelect, std::vector<std::string>& outPaths, const std::string& initialDir)
    {
        outPaths.clear();

        ComInitializer comInitializer;

        IFileOpenDialog* dialog = nullptr;
        if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&dialog))) || !dialog)
            return false;

        DWORD options = 0;
        if (SUCCEEDED(dialog->GetOptions(&options)))
        {
            options |= FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST;
            if (multiSelect)
                options |= FOS_ALLOWMULTISELECT;
            dialog->SetOptions(options);
        }

        std::vector<std::wstring> names;
        std::vector<std::wstring> patterns;
        SetFilters(dialog, filters, names, patterns);
        SetInitialFolder(dialog, initialDir);

        const HRESULT showResult = dialog->Show(static_cast<HWND>(ownerHandle));
        if (FAILED(showResult))
        {
            dialog->Release();
            return false;
        }

        if (multiSelect)
        {
            IShellItemArray* items = nullptr;
            if (SUCCEEDED(dialog->GetResults(&items)) && items)
            {
                DWORD count = 0;
                items->GetCount(&count);

                for (DWORD index = 0; index < count; ++index)
                {
                    IShellItem* item = nullptr;
                    if (SUCCEEDED(items->GetItemAt(index, &item)) && item)
                    {
                        AppendPath(item, outPaths);
                        item->Release();
                    }
                }

                items->Release();
            }
        }
        else
        {
            IShellItem* item = nullptr;
            if (SUCCEEDED(dialog->GetResult(&item)) && item)
            {
                AppendPath(item, outPaths);
                item->Release();
            }
        }

        dialog->Release();
        return !outPaths.empty();
    }

    bool FileDialog::SaveFile(void* ownerHandle, const std::vector<FileDialogFilter>& filters,
        const std::string& defaultName, const std::string& defaultExtension,
        std::string& outPath, const std::string& initialDir)
    {
        outPath.clear();

        ComInitializer comInitializer;

        IFileSaveDialog* dialog = nullptr;
        if (FAILED(CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&dialog))) || !dialog)
            return false;

        DWORD options = 0;
        if (SUCCEEDED(dialog->GetOptions(&options)))
        {
            options |= FOS_FORCEFILESYSTEM | FOS_OVERWRITEPROMPT | FOS_PATHMUSTEXIST;
            dialog->SetOptions(options);
        }

        std::vector<std::wstring> names;
        std::vector<std::wstring> patterns;
        SetFilters(dialog, filters, names, patterns);
        SetInitialFolder(dialog, initialDir);

        if (!defaultName.empty())
            dialog->SetFileName(ToWide(defaultName).c_str());

        if (!defaultExtension.empty())
            dialog->SetDefaultExtension(ToWide(defaultExtension).c_str());

        const HRESULT showResult = dialog->Show(static_cast<HWND>(ownerHandle));
        if (FAILED(showResult))
        {
            dialog->Release();
            return false;
        }

        IShellItem* item = nullptr;
        if (SUCCEEDED(dialog->GetResult(&item)) && item)
        {
            std::vector<std::string> paths;
            AppendPath(item, paths);
            item->Release();

            if (!paths.empty())
                outPath = paths.front();
        }

        dialog->Release();
        return !outPath.empty();
    }
}
