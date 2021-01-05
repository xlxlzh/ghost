#include "Mesh.h"
#include "Engine.h"

//assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


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

        if (!_vertices.empty())
        {
            _vertexBuffer = Engine::getInstance()->getRenderDevice()->createVertexBuffer(
                sizeof(MeshVertex), _vertices.size(), BufferUsage::USAGE_DYNAMIC);
            _vertexBuffer->writeData(0, sizeof(MeshVertex) * _vertices.size(), &_vertices[0], true);

            _bindings = std::make_shared<VertexBufferBinding>();
            _bindings->setBinding(0, _vertexBuffer);

            _indexBuffer = Engine::getInstance()->getRenderDevice()->createIndexBuffer(
            IndexBuffer::INDEX_32BIT, _indices.size(), BufferUsage::USAGE_DYNAMIC);
            _indexBuffer->writeData(0, sizeof(unsigned int) * _indices.size(), &_indices[0], true);

            _vertexDec = Engine::getInstance()->getRenderDevice()->createVertexDeclaration();

            unsigned offset = 0;
            _vertexDec->addElement(0, offset, VET_FLOAT_3, VES_POSITION);
            offset += VertexElement::getTypeSize(VET_FLOAT_3);
            _vertexDec->addElement(0, offset, VET_FLOAT_3, VES_NORMAL);
            offset += VertexElement::getTypeSize(VET_FLOAT_3);
            _vertexDec->addElement(0, offset, VET_FLOAT_2, VES_TEXTURE_COORDINATES);
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