#include "MeshImporter.h"

namespace ghost
{
    bool MeshImportManager::importMeshFromFile(const std::string& meshName)
    {
        return true;
    }

    void MeshImportManager::registerMeshImporter(const std::string& ext, MeshImporter* importer)
    {

    }

    void MeshImportManager::unregisterMeshImporter(const std::string& ext)
    {

    }
}