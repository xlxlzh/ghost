#ifndef _MESH_IMPORTER_H_
#define _MESH_IMPORTER_H_

#include "Ghost.h"
#include "SingleTon.h"
#include <string>
#include <unordered_map>

namespace ghost
{
    class GHOST_API MeshImporter
    {
    public:
        virtual bool ImportMeshFromFile(const std::string& meshName) = 0;
    };

    DECLAR_SMART_POINTER(MeshImporter)

    class GHOST_API MeshImporterFactory
    {
    public:
        virtual MeshImporter* CreateMeshImporter() = 0;
    };

    class GHOST_API MeshImportManager : public SingleTon<MeshImportManager>
    {
    public:
        bool ImportMeshFromFile(const std::string& meshName);

        void RegisterMeshImporter(const std::string& ext, MeshImporterFactory* importer);
        void UnregisterMeshImporter(const std::string& ext);

    private:
        std::unordered_map<std::string, MeshImporterFactory*> _factories;
    };
}

#endif
