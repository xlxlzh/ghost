#include "MeshImporter.h"
#include "FileSystem.h"
#include "LogManager.h"

namespace ghost
{
    bool MeshImportManager::importMeshFromFile(const std::string& meshName)
    {
        std::string ext = GetExtension(meshName);
        if (ext.empty())
        {
            GHOST_LOG_FORMAT_ERROR("Import meshName [%s] is wrong", meshName.c_str());
            return false;
        }

        auto fac = _factories.find(ext);
        if (fac == _factories.end())
        {
            GHOST_LOG_FORMAT_ERROR("Import meshName [%s] with format [%s] is not support", meshName.c_str(), ext.c_str());
            return false;
        }
            
        auto importer = fac->second->createMeshImporter();

        return importer->importMeshFromFile(meshName);
    }

    void MeshImportManager::registerMeshImporter(const std::string& ext, MeshImporterFactory* importer)
    {

    }

    void MeshImportManager::unregisterMeshImporter(const std::string& ext)
    {

    }
}