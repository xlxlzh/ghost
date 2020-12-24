#ifndef _MESH_H_
#define _MESH_H_

#include "Resource.h"

//assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Vector3.h"
#include "Vector2.h"

namespace ghost
{
    struct GHOST_API MeshVertex
    {
        Vector3f postion;
        Vector3f normal;
        Vector2f uv;
    };

    //We only just use a model contain one submesh, next to do others.
    class GHOST_API Mesh : public Resource
    {
    public:
        Mesh() : Mesh(RESOURCE_MESH, "", 0) { }
        Mesh(int type, const std::string& name, int flags);

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_MESH; }

    private:
        void _processMeshNode(aiNode* node, const aiScene* scene);
        void _processMesh(aiMesh* mesh, const aiScene* scene);

    private:
        std::vector<aiMesh*> _assimpMeshes;

        std::vector<MeshVertex> _vertices;
        std::vector<unsigned> _indices;
    };
}

#endif
