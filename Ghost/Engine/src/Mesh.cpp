#include "Mesh.h"


namespace ghost
{
    Mesh::Mesh(int type, const std::string& name, int flags) : Resource(type, name, flags)
    {

    }

    bool Mesh::load(DataStream& dataStream)
    {
        int meshSize = dataStream.getSize();
        unsigned char* buff = new unsigned char[meshSize];
        meshSize = dataStream.read(buff, meshSize);

        if (meshSize)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFileFromMemory(buff, meshSize, aiProcess_Triangulate | aiProcess_GenNormals);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
                return false;

            _processMeshNode(scene->mRootNode, scene);

            for (auto mesh : _assimpMeshes)
                _processMesh(mesh, scene);
        }

        return true;
    }

    void Mesh::_processMeshNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            _assimpMeshes.push_back(mesh);
        }

        for (unsigned i = 0; i < node->mNumChildren; ++i)
        {
            _processMeshNode(node->mChildren[i], scene);
        }
    }

    void Mesh::_processMesh(aiMesh* mesh, const aiScene* scene)
    {
        for (int i = 0; i < mesh->mNumVertices; ++i)
        {
            MeshVertex tmpVertex;
            
            aiVector3D pos = mesh->mVertices[i];

            if (mesh->HasNormals())
            {
                aiVector3D normal = mesh->mNormals[i];
                tmpVertex.normal._x = normal.x;
                tmpVertex.normal._y = normal.y;
                tmpVertex.normal._z = normal.z;
            }
            else
            {
                tmpVertex.normal = Vector3f(0.0, 1.0, 0.0);
            }
            

            tmpVertex.postion._x = pos.x;
            tmpVertex.postion._y = pos.y;
            tmpVertex.postion._z = pos.z;

            if (mesh->mTextureCoords[0])
            {
                tmpVertex.uv._x = mesh->mTextureCoords[0][i].x;
                tmpVertex.uv._y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                tmpVertex.uv = Vector2f();
            }

            _vertices.push_back(tmpVertex);
        }

        for (int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; ++j)
                _indices.push_back(face.mIndices[j]);
        }
    }
}