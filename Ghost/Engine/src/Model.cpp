#include "Model.h"
#include "Engine.h"
#include "Vector3.h"
#include "Vector2.h"

//assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace ghost
{
    class ModelLoadHelper
    {
    public:
        ModelLoadHelper(Model* loader) : _loader(loader) { }

        ~ModelLoadHelper()
        {
            _loader = nullptr;
        }

        void processNode(aiNode* node, const aiScene* scene, Matrix4x4f globalMatrix)
        {
            if (node->mParent == nullptr)
            {
                globalMatrix = node->mTransformation[0];
                globalMatrix.Transpose();
            }
            else
            {
                Matrix4x4f localMat = node->mTransformation[0];
                localMat.Transpose();
                globalMatrix = localMat * globalMatrix;
            }

            for (unsigned i = 0; i < node->mNumMeshes; ++i)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                processMesh(mesh, scene, globalMatrix);
            }

            for (unsigned i = 0; i < node->mNumChildren; ++i)
            {
                processNode(node->mChildren[i], scene, globalMatrix);
            }
        }

        void processMesh(aiMesh* mesh, const aiScene* scene, const Matrix4x4f& localMatrix)
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

            SubMesh subMesh;
            subMesh._localMatrix = localMatrix;
            unsigned eleSize = Model::GetVertexSizeByMask(_loader->_mask) / sizeof(float);
            subMesh._vertexDatas.resize(eleSize * mesh->mNumVertices);

            unsigned offset = 0;
            for (int i = 0; i < mesh->mNumVertices; ++i)
            {
                offset = eleSize * i;

                const aiVector3D& pos = mesh->mVertices[i];
                subMesh._vertexDatas[offset++] = pos.x;
                subMesh._vertexDatas[offset++] = pos.y;
                subMesh._vertexDatas[offset++] = pos.z;

                if (hasNormal)
                {
                    const aiVector3D& normal = mesh->mNormals[i];
                    subMesh._vertexDatas[offset++] = normal.x;
                    subMesh._vertexDatas[offset++] = normal.y;
                    subMesh._vertexDatas[offset++] = normal.z;
                }

                if (hasUv)
                {
                    subMesh._vertexDatas[offset++] = mesh->mTextureCoords[0][i].x;
                    subMesh._vertexDatas[offset++] = mesh->mTextureCoords[0][i].y;
                }
            }

            for (int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (int j = 0; j < face.mNumIndices; ++j)
                    subMesh._indices.push_back(face.mIndices[j]);
            }

            _loader->_meshes.emplace_back(subMesh);
        }

        bool preocessLoad(DataStream& dataStream)
        {
            int meshSize = dataStream.GetSize();
            _buff = new unsigned char[meshSize];
            meshSize = dataStream.Read(_buff, meshSize);

            if (meshSize > 0)
            {
                Assimp::Importer importer;
                const aiScene* scene = importer.ReadFileFromMemory(_buff, meshSize, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
                {
                    SAFE_DELETE_ARRAY(_buff);
                    return false;
                }

                processNode(scene->mRootNode, scene, Matrix4x4f());
            }

            return true;
        }

    private:
        std::vector<aiMesh*> _assimpMeshes;
        Model* _loader;

        unsigned char* _buff = nullptr;
    };

    void SubMesh::CreateVertexBinding(unsigned mask)
    {
        if (!_vertexDatas.empty())
        {
            unsigned vSize = Model::GetVertexSizeByMask(mask);
            unsigned numVertex = _vertexDatas.size() * sizeof(float) / vSize;

            _vertexBuffer = Engine::GetInstance()->GetRenderDevice()->CreateVertexBuffer(
                vSize, numVertex, ResourceUsage::USAGE_DYNAMIC);

            _vertexBuffer->WriteData(0, vSize * numVertex, &_vertexDatas[0], true);

            _bindings = std::make_shared<VertexBufferBinding>();
            _bindings->SetBinding(0, _vertexBuffer);

            _indexBuffer = Engine::GetInstance()->GetRenderDevice()->CreateIndexBuffer(
                IndexBuffer::INDEX_32BIT, _indices.size(), ResourceUsage::USAGE_DYNAMIC);
            _indexBuffer->WriteData(0, sizeof(unsigned int) * _indices.size(), &_indices[0], true);
        }
    }

    Model::Model(int type, const std::string& name, int flag) : Resource(type, name, flag)
    {

    }

    bool Model::Load(DataStream& dataStream)
    {
        ModelLoadHelper helper(this);

        if (!helper.preocessLoad(dataStream))
            return false;

        for (auto& subMesh : _meshes)
        {
            subMesh.CreateVertexBinding(_mask);
        }

        _materials.resize(_meshes.size());

        CreateVertexDecl();

        return true;
    }

    void Model::Save(DataStream& dataStream)
    {
        //TODO
    }

    void Model::CreateVertexDecl()
    {
        _vertexDec = Engine::GetInstance()->GetRenderDevice()->CreateVertexDeclaration();

        unsigned offset = 0;
        //if (_mask & VERTEX_POSITION)
        {
            _vertexDec->AddElement(0, offset, VET_FLOAT_3, VES_POSITION);
            offset += VertexElement::GetTypeSize(VET_FLOAT_3);
        }
        //if (_mask & VERTEX_NORMAL)
        {
            _vertexDec->AddElement(0, offset, VET_FLOAT_3, VES_NORMAL);
            offset += VertexElement::GetTypeSize(VET_FLOAT_3);
        }
        //if (_mask & VERTEX_TEXCOORD)
        {
            _vertexDec->AddElement(0, offset, VET_FLOAT_2, VES_TEXTURE_COORDINATES);
            offset += VertexElement::GetTypeSize(VET_FLOAT_2);
        }
    }

    void Model::SetMaterial(unsigned subIndex, const MaterialPtr& material)
    {
        assert(subIndex >= 0 && subIndex < _meshes.size());

        _materials[subIndex] = material;
        _meshes[subIndex]._MaterialIndex = subIndex;
    }

    const MaterialPtr& Model::GetMaterial(unsigned subIndex) const 
    {
        assert(subIndex >= 0 && subIndex < _meshes.size());

        return _materials[subIndex];
    }

    void Model::AddSubmesh(const SubMesh& mesh)
    {
        _meshes.emplace_back(mesh);
    }

    unsigned Model::GetVertexSizeByMask(unsigned mask)
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
}