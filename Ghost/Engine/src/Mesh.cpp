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
            bool hasNormal = mesh->HasNormals();
            bool hasTangent = mesh->HasTangentsAndBitangents();
            bool hasUv = mesh->mTextureCoords[0];

            _loader->_mask = VERTEX_POSITION;
            if (hasNormal)
                _loader->_mask |= VERTEX_NORMAL;
            if (hasUv)
                _loader->_mask |= VERTEX_TEXCOORD;
            if (hasTangent)
                _loader->_mask |= VERTEX_TANGENT;

            unsigned vSize = Mesh::getVertexSizeByMask(_loader->_mask);
            _loader->_rawDatas.resize(vSize * mesh->mNumVertices);

            unsigned offset = 0;
            for (int i = 0; i < mesh->mNumVertices; ++i)
            {
                Vector3f tmpPos, tmpNormal;
                Vector2f tmpUV;

                aiVector3D pos = mesh->mVertices[i];
                tmpPos._x = pos.x;
                tmpPos._y = pos.y;
                tmpPos._z = pos.z;
                _loader->_positions.push_back(tmpPos);
                memcpy(&_loader->_rawDatas[offset], &tmpPos, sizeof(Vector3f));
                offset += sizeof(Vector3f);

                if (hasNormal)
                {
                    aiVector3D normal = mesh->mNormals[i];
                    tmpNormal._x = normal.x;
                    tmpNormal._y = normal.y;
                    tmpNormal._z = normal.z;

                    memcpy(&_loader->_rawDatas[offset], &tmpNormal, sizeof(Vector3f));
                    offset += sizeof(Vector3f);

                    _loader->_normals.push_back(tmpNormal);
                }

                if (hasUv)
                {
                    tmpUV._x = mesh->mTextureCoords[0][i].x;
                    tmpUV._y = mesh->mTextureCoords[0][i].y;

                    memcpy(&_loader->_rawDatas[offset], &tmpUV, sizeof(Vector2f));
                    offset += sizeof(Vector2f);

                    _loader->_uv.push_back(tmpUV);
                }
            }

            for (int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (int j = 0; j < face.mNumIndices; ++j)
                    _loader->_indices.push_back(face.mIndices[j]);
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
                const aiScene* scene = importer.ReadFileFromMemory(_buff, meshSize, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
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

    unsigned Mesh::getVertexSizeByMask(unsigned mask)
    {
        unsigned vSize = 0;
        if (mask & VERTEX_POSITION)
            vSize += sizeof(Vector3f);
        if (mask & VERTEX_NORMAL)
            vSize += sizeof(Vector3f);
        if (mask & VERTEX_TANGENT)
            vSize += sizeof(Vector3f);
        if (mask & VERTEX_TEXCOORD)
            vSize += sizeof(Vector2f);

        return vSize;
    }

    bool Mesh::load(DataStream& dataStream)
    {
        MeshLoadHelper helper(this);

        if (!helper.preocessLoad(dataStream))
            return false;

        if (!_positions.empty())
        {
            unsigned vSize = getVertexSizeByMask(_mask);

            _vertexBuffer = Engine::getInstance()->getRenderDevice()->createVertexBuffer(
                vSize, _positions.size(), ResourceUsage::USAGE_DYNAMIC);
            
            _vertexBuffer->writeData(0, vSize * _positions.size(), &_rawDatas[0], true);

            _bindings = std::make_shared<VertexBufferBinding>();
            _bindings->setBinding(0, _vertexBuffer);

            _indexBuffer = Engine::getInstance()->getRenderDevice()->createIndexBuffer(
            IndexBuffer::INDEX_32BIT, _indices.size(), ResourceUsage::USAGE_DYNAMIC);
            _indexBuffer->writeData(0, sizeof(unsigned int) * _indices.size(), &_indices[0], true);

            _vertexDec = Engine::getInstance()->getRenderDevice()->createVertexDeclaration();

            unsigned offset = 0;
            //if (_mask & VERTEX_POSITION)
            {
                _vertexDec->addElement(0, offset, VET_FLOAT_3, VES_POSITION);
                offset += VertexElement::getTypeSize(VET_FLOAT_3);
            }
            //if (_mask & VERTEX_NORMAL)
            {
                _vertexDec->addElement(0, offset, VET_FLOAT_3, VES_NORMAL);
                offset += VertexElement::getTypeSize(VET_FLOAT_3);
            }
            //if (_mask & VERTEX_TEXCOORD)
            {
                _vertexDec->addElement(0, offset, VET_FLOAT_2, VES_TEXTURE_COORDINATES);
                offset += VertexElement::getTypeSize(VET_FLOAT_2);
            }
            
        }

        _objConstBuffer = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(Matrix4x4f), ResourceUsage::USAGE_DYNAMIC, "PerObject");

        return true;
    }
}