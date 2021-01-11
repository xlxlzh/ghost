#include "Mesh.h"
#include "Engine.h"
#include "Matrix4x4.h"

//assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


namespace ghost
{
    class MeshLoadHelper
    {
    public:
        MeshLoadHelper(Mesh* loader) : _loader(loader) { }

        ~MeshLoadHelper()
        {
            _assimpMeshes.clear();
            SAFE_DELETE_ARRAY(_buff);
            _loader = nullptr;
        }

        void processMeshNode(aiNode* node, const aiScene* scene)
        {
            for (unsigned i = 0; i < node->mNumMeshes; ++i)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                _assimpMeshes.push_back(mesh);
            }

            for (unsigned i = 0; i < node->mNumChildren; ++i)
            {
                processMeshNode(node->mChildren[i], scene);
            }
        }

        void processMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<MeshVertex>& vertices = _loader->getVertices();
            std::vector<unsigned>& indices = _loader->getIndices();

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

                vertices.push_back(tmpVertex);
            }

            for (int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (int j = 0; j < face.mNumIndices; ++j)
                    indices.push_back(face.mIndices[j]);
            }
        }

        bool preocessLoad(DataStream& dataStream)
        {
            int meshSize = dataStream.getSize();
            _buff = new unsigned char[meshSize];
            meshSize = dataStream.read(_buff, meshSize);

            if (meshSize)
            {
                Assimp::Importer importer;
                const aiScene* scene = importer.ReadFileFromMemory(_buff, meshSize, aiProcess_Triangulate | aiProcess_GenNormals);
                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
                {
                    SAFE_DELETE_ARRAY(_buff);
                    return false;
                }

                processMeshNode(scene->mRootNode, scene);

                for (auto mesh : _assimpMeshes)
                    processMesh(mesh, scene);
            }

            return true;
        }

    private:
        std::vector<aiMesh*> _assimpMeshes;
        Mesh* _loader;

        unsigned char* _buff = nullptr;
    };

    Mesh::Mesh(int type, const std::string& name, int flags) : Resource(type, name, flags)
    {

    }

    bool Mesh::load(DataStream& dataStream)
    {
        MeshLoadHelper helper(this);

        if (!helper.preocessLoad(dataStream))
            return false;

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

        _objConstBuffer = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(Matrix4x4f), BufferUsage::USAGE_DYNAMIC, "PerObject");

        return true;
    }
}