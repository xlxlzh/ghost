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
        virtual bool importMeshFromFile(const std::string& meshName) = 0;
    };

    DECLAR_SMART_POINTER(MeshImporter)

    class GHOST_API MeshImporterFactory
    {
    public:
        virtual MeshImporter* createMeshImporter() = 0;
    };

    class GHOST_API MeshImportManager : public SingleTon<MeshImportManager>
    {
    public:
        bool importMeshFromFile(const std::string& meshName);

        void registerMeshImporter(const std::string& ext, MeshImporterFactory* importer);
        void unregisterMeshImporter(const std::string& ext);

    private:
        std::unordered_map<std::string, MeshImporterFactory*> _factories;
    };
}

#endif
