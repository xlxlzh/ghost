#ifndef _MODEL_H_
#define _MODEL_H_

#include "Ghost.h"
#include "Resource.h"
#include "Material.h"
#include "VertexBufferBinding.h"
#include "VertexDeclaration.h"
#include "ConstBuffer.h"
#include "IndexBuffer.h"
#include "Matrix4x4.h"

namespace ghost
{
    class ModelHelper;

    enum VertexMask : unsigned
    {
        VERTEX_NONE = 0x0,
        VERTEX_POSITION = 0x1,
        VERTEX_NORMAL = 0x2,
        VERTEX_TANGENT = 0x4,
        VERTEX_TEXCOORD = 0x8,
    };

    class GHOST_API SubMesh
    {
        friend class ModelNode;
        friend class ModelLoadHelper;
        friend class Model;
        friend class ModelHelper;
    public:
        SubMesh()
        {
            _vertexDatas.clear();
            _indices.clear();
        }

        const std::vector<float>& GetVertexDatas() const { return _vertexDatas; }
        const std::vector<unsigned>& GetIndices() const { return _indices; }
        void SetVertexDatas(const std::vector<float>& datas) { _vertexDatas = datas; }
        void SetIndices(const std::vector<unsigned>& indices) { _indices = indices; }
        unsigned GetMask() const { return _mask; }
        void SetMask(unsigned mask) { _mask = mask; }
        const Matrix4x4f& GetLocalMatrix() const { return _localMatrix; }
        void SetLocalMatrix(const Matrix4x4f& matrix) { _localMatrix = matrix; }

    private:
        int _MaterialIndex{ GHOST_INVALID_INDEX };

        std::vector<float> _vertexDatas;
        std::vector<unsigned> _indices;
        unsigned _mask{ VERTEX_NONE };

        Matrix4x4f _localMatrix;
    };

    class GHOST_API Model final : public Resource
    {
        friend class ModelNode;
        friend class ModelLoadHelper;
        friend class ModelHelper;
    public:
        Model() : Model(RESOURCE_MODEL, "", 0) { }
        Model(int type, const std::string& name, int flag);

        virtual bool Load(DataStream& dataStream) override;
        virtual bool Save(DataStream& dataStream) override;
        virtual bool SaveXml(DataStream& dataStream) override;
        
        void SetMaterial(unsigned subIndex, const MaterialPtr& material);
        const MaterialPtr& GetMaterial(unsigned subIndex) const;
        unsigned GetSubMeshCount() const { return static_cast<unsigned>(_meshes.size()); }
        void AddSubmesh(const SubMesh& mesh);
        const SubMesh& GetSubMesh(unsigned subIndex) const { return _meshes[subIndex]; }

        static unsigned GetVertexSizeByMask(unsigned mask);

        DECLAR_RESOURCE_TYPE(MODEL);

    protected:
        std::vector<SubMesh> _meshes;
        std::vector<MaterialPtr> _materials;
    };

    DECLAR_SMART_POINTER(Model);
    
    class GHOST_API ModelHelper
    {
    public:
        static ModelPtr ImportModel(const std::string& filename);
        static bool ExportModel(const ModelPtr& model, const std::string& filename);
        static bool ExportModelXml(const ModelPtr& model, const std::string& filename);

        static bool LoadNativeModel(Model& model, DataStream& dataStream);
        static bool LoadNativeModelBinary(Model& model, DataStream& dataStream);
        static bool LoadNativeModelXml(Model& model, DataStream& dataStream);
        static bool SaveNativeModel(const Model& model, DataStream& dataStream);
        static bool SaveNativeModelXml(const Model& model, DataStream& dataStream);
    };
}

#endif