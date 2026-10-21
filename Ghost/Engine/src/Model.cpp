#include "Model.h"
#include "Engine.h"
#include "FileSystem.h"
#include "AssetIOUtils.h"
#include "BinaryArchive.h"
#include "LogManager.h"
#include "Vector3.h"
#include "Vector2.h"

#include <limits>
#include <utility>

#include "tinyxml2.h"

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
            SAFE_DELETE_ARRAY(_buff);
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

            SubMesh subMesh;
            subMesh._localMatrix = localMatrix;

            subMesh._mask = VERTEX_POSITION;
            if (hasNormal)
                subMesh._mask |= VERTEX_NORMAL;
            if (hasUv)
                subMesh._mask |= VERTEX_TEXCOORD;
            if (hasTangent)
                subMesh._mask |= VERTEX_TANGENT;

            unsigned eleSize = Model::GetVertexSizeByMask(subMesh._mask) / sizeof(float);
            subMesh._vertexDatas.resize(eleSize * mesh->mNumVertices);

            unsigned offset = 0;
            for (unsigned i = 0; i < mesh->mNumVertices; ++i)
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

                if (hasTangent)
                {
                    const aiVector3D& tangent = mesh->mTangents[i];
                    subMesh._vertexDatas[offset++] = tangent.x;
                    subMesh._vertexDatas[offset++] = tangent.y;
                    subMesh._vertexDatas[offset++] = tangent.z;
                }

                if (hasUv)
                {
                    subMesh._vertexDatas[offset++] = mesh->mTextureCoords[0][i].x;
                    subMesh._vertexDatas[offset++] = mesh->mTextureCoords[0][i].y;
                }
            }

            for (unsigned i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned j = 0; j < face.mNumIndices; ++j)
                    subMesh._indices.push_back(face.mIndices[j]);
            }

            _loader->_meshes.emplace_back(std::move(subMesh));
        }

        bool preocessLoad(DataStream& dataStream)
        {
            int meshSize = static_cast<int>(dataStream.GetSize());
            if (meshSize <= 0)
                return false;

            _buff = new unsigned char[meshSize];
            meshSize = static_cast<int>(dataStream.Read(_buff, meshSize));
            if (meshSize <= 0)
                return false;

            std::string formatHint = GetExtension(dataStream.GetName());
            if (!formatHint.empty() && formatHint[0] == '.')
                formatHint.erase(formatHint.begin());

            Assimp::Importer importer;
            importer.SetPropertyFloat("PP_GSN_MAX_SMOOTHING_ANGLE", 80.0f);

            const aiScene* scene = importer.ReadFileFromMemory(_buff, meshSize,
                aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices,
                formatHint.c_str());
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                return false;
            }

            processNode(scene->mRootNode, scene, Matrix4x4f());
            return true;
        }

    private:
        std::vector<aiMesh*> _assimpMeshes;
        Model* _loader;

        unsigned char* _buff = nullptr;
    };

    ModelPtr ModelHelper::ImportModel(const std::string& filename)
    {
        FileStream stream(filename, AM_READ);
        if (!stream.IsOpened())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::ImportModel: failed to open '%s'.", filename.c_str());
            return nullptr;
        }

        auto model = std::make_shared<Model>();
        model->SetName(filename);

        ModelLoadHelper helper(model.get());
        if (!helper.preocessLoad(stream))
        {
            stream.Close();
            return nullptr;
        }

        stream.Close();

        model->_materials.resize(model->_meshes.size());
        model->_loaded = true;
        return model;
    }

    bool ModelHelper::ExportModel(const ModelPtr& model, const std::string& filename)
    {
        if (!model)
            return false;

        FileStream stream(filename, AM_WRITE);
        if (!stream.IsOpened())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::ExportModel: failed to open '%s'.", filename.c_str());
            return false;
        }

        const bool saved = model->Save(stream);
        stream.Close();
        return saved;
    }

    bool ModelHelper::ExportModelXml(const ModelPtr& model, const std::string& filename)
    {
        if (!model)
            return false;

        FileStream stream(filename, AM_WRITE);
        if (!stream.IsOpened())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::ExportModelXml: failed to open '%s'.", filename.c_str());
            return false;
        }

        const bool saved = model->SaveXml(stream);
        stream.Close();
        return saved;
    }

    bool ModelHelper::LoadNativeModel(Model& model, DataStream& dataStream)
    {
        switch (DetectAssetEncoding(dataStream))
        {
        case AssetEncoding::Gres:
            return LoadNativeModelBinary(model, dataStream);

        case AssetEncoding::Xml:
            return LoadNativeModelXml(model, dataStream);

        case AssetEncoding::Unknown:
        {
            ModelLoadHelper helper(&model);
            if (!helper.preocessLoad(dataStream))
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: failed to import source model '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            model._materials.resize(model._meshes.size());
            model._loaded = true;
            return true;
        }

        case AssetEncoding::Gmap:
        default:
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: '%s' is not a model asset.",
                dataStream.GetName().c_str());
            return false;
        }
    }

    bool ModelHelper::LoadNativeModelBinary(Model& model, DataStream& dataStream)
    {
        BinaryReader reader(dataStream);
        if (!ReadAssetHeader(reader, AssetFormat::GRES_MAGIC,
            static_cast<uint16_t>(RESOURCE_MODEL), AssetFormat::MODEL_VERSION))
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: invalid header in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        uint32_t subMeshCount = 0;
        if (!reader.ReadU32(subMeshCount))
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: failed to read submesh count in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        if (subMeshCount > 65535)
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: submesh count exceeds the allowed limit.");
            return false;
        }

        const unsigned knownMask =
            VERTEX_POSITION | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_TEXCOORD;

        std::vector<SubMesh> subMeshes;
        subMeshes.reserve(subMeshCount);

        for (uint32_t meshIndex = 0; meshIndex < subMeshCount; ++meshIndex)
        {
            uint32_t mask = 0;
            uint32_t vertexCount = 0;
            uint32_t indexCount = 0;

            if (!reader.ReadU32(mask) ||
                !reader.ReadU32(vertexCount) ||
                !reader.ReadU32(indexCount))
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: failed to read submesh header in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            const unsigned vertexSize = Model::GetVertexSizeByMask(mask);
            const unsigned floatsPerVertex = vertexSize / sizeof(float);

            if ((mask & knownMask) == 0 || (mask & ~knownMask) != 0 ||
                (mask & VERTEX_POSITION) == 0 || floatsPerVertex == 0)
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: invalid vertex mask in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            if (vertexCount == 0 || indexCount == 0)
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: empty submesh in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            SubMesh subMesh;
            subMesh._mask = mask;

            if (!AssetIO::ReadMatrix4x4(reader, subMesh._localMatrix))
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: failed to read local matrix in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            const uint64_t floatCount = static_cast<uint64_t>(vertexCount) * floatsPerVertex;
            const uint64_t vertexBytes = floatCount * sizeof(float);
            const uint64_t indexBytes = static_cast<uint64_t>(indexCount) * sizeof(uint32_t);

            if (floatCount > std::numeric_limits<uint32_t>::max() ||
                vertexBytes + indexBytes > reader.RemainingBytes())
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: invalid vertex/index counts in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            if (!reader.ReadFloatArray(subMesh._vertexDatas, static_cast<uint32_t>(floatCount)) ||
                !reader.ReadU32Array(subMesh._indices, indexCount))
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: failed to read mesh data in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            for (unsigned index : subMesh._indices)
            {
                if (index >= vertexCount)
                {
                    GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: index out of range in '%s'.",
                        dataStream.GetName().c_str());
                    return false;
                }
            }

            subMeshes.push_back(std::move(subMesh));
        }

        if (!reader.IsOk())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelBinary: %s", reader.GetError().c_str());
            return false;
        }

        model._meshes.swap(subMeshes);
        model._materials.assign(model._meshes.size(), nullptr);
        model._loaded = true;
        return true;
    }

    bool ModelHelper::LoadNativeModelXml(Model& model, DataStream& dataStream)
    {
        std::string text;
        if (!ReadStreamAsText(dataStream, text))
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelXml: empty or unreadable stream '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLDocument document;
        if (document.Parse(text.c_str(), text.size()) != tinyxml2::XML_SUCCESS)
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelXml: XML parse error in '%s'.", dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLElement* root = document.FirstChildElement("Model");
        if (!root)
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModelXml: missing <Model> root in '%s'.", dataStream.GetName().c_str());
            return false;
        }

        const int version = root->IntAttribute("version", 1);
        if (version > 1)
        {
            GHOST_LOG_FORMAT_WARNNING("ModelHelper::LoadNativeModelXml: unsupported version %d in '%s'.",
                version, dataStream.GetName().c_str());
        }

        std::vector<SubMesh> subMeshes;

        for (tinyxml2::XMLElement* element = root->FirstChildElement("SubMesh");
            element != nullptr;
            element = element->NextSiblingElement("SubMesh"))
        {
            const int maskValue = element->IntAttribute("mask", -1);
            const int vertexCount = element->IntAttribute("vertexCount", -1);
            const int indexCount = element->IntAttribute("indexCount", -1);

            if (maskValue < 0 || vertexCount < 0 || indexCount < 0)
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: invalid SubMesh attributes in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            const unsigned mask = static_cast<unsigned>(maskValue);
            const unsigned vertexSize = Model::GetVertexSizeByMask(mask);
            const unsigned floatsPerVertex = vertexSize / sizeof(float);

            if (vertexSize == 0 || floatsPerVertex == 0)
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: invalid vertex mask in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            SubMesh subMesh;
            subMesh._mask = mask;

            tinyxml2::XMLElement* matrixElement = element->FirstChildElement("LocalMatrix");
            if (matrixElement)
            {
                if (!AssetIO::ParseMatrix4x4(matrixElement->GetText(), subMesh._localMatrix))
                {
                    GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: invalid LocalMatrix in '%s'.",
                        dataStream.GetName().c_str());
                    return false;
                }
            }
            else
            {
                subMesh._localMatrix.Identify();
            }

            tinyxml2::XMLElement* verticesElement = element->FirstChildElement("Vertices");
            if (!AssetIO::ParseFloatList(verticesElement ? verticesElement->GetText() : nullptr, subMesh._vertexDatas))
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: invalid Vertices in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            if (subMesh._vertexDatas.size() != static_cast<std::size_t>(vertexCount) * floatsPerVertex)
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: vertex count mismatch in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            tinyxml2::XMLElement* indicesElement = element->FirstChildElement("Indices");
            if (!AssetIO::ParseUIntList(indicesElement ? indicesElement->GetText() : nullptr, subMesh._indices))
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: invalid Indices in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            if (subMesh._indices.size() != static_cast<std::size_t>(indexCount))
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: index count mismatch in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            for (unsigned index : subMesh._indices)
            {
                if (index >= static_cast<unsigned>(vertexCount))
                {
                    GHOST_LOG_FORMAT_ERROR("ModelHelper::LoadNativeModel: index out of range in '%s'.",
                        dataStream.GetName().c_str());
                    return false;
                }
            }

            subMeshes.push_back(std::move(subMesh));
        }

        model._meshes.swap(subMeshes);
        model._materials.assign(model._meshes.size(), nullptr);
        model._loaded = true;
        return true;
    }

    bool ModelHelper::SaveNativeModel(const Model& model, DataStream& dataStream)
    {
        if (!dataStream.IsWriteable())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: stream is not writable.");
            return false;
        }

        if (model._meshes.size() > std::numeric_limits<uint32_t>::max())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: too many submeshes.");
            return false;
        }

        BinaryWriter writer(dataStream);
        if (!WriteAssetHeader(writer, AssetFormat::GRES_MAGIC,
            static_cast<uint16_t>(RESOURCE_MODEL), AssetFormat::MODEL_VERSION))
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: failed to write header.");
            return false;
        }

        writer.WriteU32(static_cast<uint32_t>(model._meshes.size()));

        for (const SubMesh& subMesh : model._meshes)
        {
            const unsigned mask = subMesh._mask;
            const unsigned vertexSize = Model::GetVertexSizeByMask(mask);
            const unsigned floatsPerVertex = vertexSize / sizeof(float);

            if (floatsPerVertex == 0 || subMesh._vertexDatas.empty() ||
                subMesh._indices.empty() ||
                subMesh._vertexDatas.size() % floatsPerVertex != 0)
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: invalid vertex data.");
                return false;
            }

            const std::size_t vertexCount = subMesh._vertexDatas.size() / floatsPerVertex;
            const std::size_t indexCount = subMesh._indices.size();

            if (vertexCount > std::numeric_limits<uint32_t>::max() ||
                indexCount > std::numeric_limits<uint32_t>::max())
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: mesh is too large.");
                return false;
            }

            writer.WriteU32(mask);
            writer.WriteU32(static_cast<uint32_t>(vertexCount));
            writer.WriteU32(static_cast<uint32_t>(indexCount));

            AssetIO::WriteMatrix4x4(writer, subMesh._localMatrix);

            writer.WriteFloatArray(subMesh._vertexDatas.data(), subMesh._vertexDatas.size());
            writer.WriteU32Array(subMesh._indices.data(), subMesh._indices.size());
        }

        if (!writer.IsOk())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: %s", writer.GetError().c_str());
            return false;
        }

        return true;
    }

    bool ModelHelper::SaveNativeModelXml(const Model& model, DataStream& dataStream)
    {
        if (!dataStream.IsWriteable())
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModelXml: stream is not writable.");
            return false;
        }

        tinyxml2::XMLDocument document;
        document.InsertFirstChild(document.NewDeclaration());

        tinyxml2::XMLElement* root = document.NewElement("Model");
        root->SetAttribute("version", 1);
        document.InsertEndChild(root);

        for (const SubMesh& subMesh : model._meshes)
        {
            const unsigned mask = subMesh._mask;
            const unsigned vertexSize = Model::GetVertexSizeByMask(mask);
            const unsigned floatsPerVertex = vertexSize / sizeof(float);

            if (floatsPerVertex == 0 || subMesh._vertexDatas.size() % floatsPerVertex != 0)
            {
                GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: invalid vertex data.");
                return false;
            }

            const unsigned vertexCount = static_cast<unsigned>(subMesh._vertexDatas.size()) / floatsPerVertex;
            const unsigned indexCount = static_cast<unsigned>(subMesh._indices.size());

            tinyxml2::XMLElement* element = document.NewElement("SubMesh");
            element->SetAttribute("mask", static_cast<int>(mask));
            element->SetAttribute("vertexCount", static_cast<int>(vertexCount));
            element->SetAttribute("indexCount", static_cast<int>(indexCount));

            tinyxml2::XMLElement* matrixElement = document.NewElement("LocalMatrix");
            matrixElement->SetText(AssetIO::FormatMatrix4x4(subMesh._localMatrix).c_str());
            element->InsertEndChild(matrixElement);

            tinyxml2::XMLElement* verticesElement = document.NewElement("Vertices");
            verticesElement->SetText(AssetIO::FormatFloatList(subMesh._vertexDatas).c_str());
            element->InsertEndChild(verticesElement);

            tinyxml2::XMLElement* indicesElement = document.NewElement("Indices");
            indicesElement->SetText(AssetIO::FormatUIntList(subMesh._indices).c_str());
            element->InsertEndChild(indicesElement);

            root->InsertEndChild(element);
        }

        tinyxml2::XMLPrinter printer;
        document.Print(&printer);

        const std::size_t length = std::strlen(printer.CStr());
        if (dataStream.Write(printer.CStr(), length) != length)
        {
            GHOST_LOG_FORMAT_ERROR("ModelHelper::SaveNativeModel: write failed.");
            return false;
        }

        return true;
    }

    Model::Model(int type, const std::string& name, int flag) : Resource(type, name, flag)
    {

    }

    bool Model::Load(DataStream& dataStream)
    {
        return ModelHelper::LoadNativeModel(*this, dataStream);
    }

    bool Model::Save(DataStream& dataStream)
    {
        return ModelHelper::SaveNativeModel(*this, dataStream);
    }

    bool Model::SaveXml(DataStream& dataStream)
    {
        return ModelHelper::SaveNativeModelXml(*this, dataStream);
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
