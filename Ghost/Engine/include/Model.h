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
    public:
        SubMesh()
        {
            _vertexDatas.clear();
            _indices.clear();
        }

        void createVertexBinding(unsigned mask);
        const std::vector<float>& getVertexDatas() const { return _vertexDatas; }
        const std::vector<unsigned>& getIndices() const { return _indices; }
        void setVertexDatas(const std::vector<float>& datas) { _vertexDatas = datas; }
        void setIndices(const std::vector<unsigned>& indices) { _indices = indices; }

    private:
        int _MaterialIndex{ GHOST_INVALID_INDEX };

        std::vector<float> _vertexDatas;
        std::vector<unsigned> _indices;

        VertexBufferPtr _vertexBuffer;
        IndexBufferPtr _indexBuffer;

        VertexBufferBindingPtr _bindings;

        Matrix4x4f _localMatrix;
    };

    class GHOST_API Model final : public Resource
    {
        friend class ModelNode;
        friend class ModelLoadHelper;
    public:
        Model() : Model(RESOURCE_MODEL, "", 0) { }
        Model(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        void createVertexDecl();
        void setMaterial(unsigned subIndex, const MaterialPtr& material);
        const MaterialPtr& getMaterial(unsigned subIndex) const;
        void addSubmesh(const SubMesh& mesh);

        static unsigned getVertexSizeByMask(unsigned mask);

        DECLAR_RESOURCE_TYPE(MODEL);

    protected:
        std::vector<SubMesh> _meshes;
        std::vector<MaterialPtr> _materials;

        unsigned _mask{ VERTEX_NONE };

        VertexDeclarationPtr _vertexDec;
        ConstBufferPtr _objConstBuffer;
    };

    DECLAR_SMART_POINTER(Model);
}

#endif