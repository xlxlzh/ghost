#ifndef _MESH_H_
#define _MESH_H_

#include "Resource.h"

#include "Vector3.h"
#include "Vector2.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "VertexBufferBinding.h"
#include "ConstBuffer.h"

namespace ghost
{
    enum VertexMask : unsigned
    {
        VERTEX_NONE             = 0x0,
        VERTEX_POSITION         = 0x1,
        VERTEX_NORMAL           = 0x2,
        VERTEX_TANGENT          = 0x4,
        VERTEX_TEXCOORD         = 0x8,
    };

    struct GHOST_API MeshVertex
    {
        Vector3f postion;
        Vector3f normal;
        Vector2f uv;
    };

    //We only just use a model contain one submesh, next to do others.
    class GHOST_API Mesh : public Resource
    {
        friend class MeshNode;
        friend class MeshLoadHelper;
    public:
        Mesh() : Mesh(RESOURCE_MESH, "", 0) { }
        Mesh(int type, const std::string& name, int flags);

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_MESH; }
        
        std::vector<Vector3f>& getVertices() { return _positions; }
        std::vector<unsigned>& getIndices() { return _indices; }

        static unsigned getVertexSizeByMask(unsigned mask);

    private:
        std::vector<Vector3f> _positions;
        std::vector<Vector3f> _normals;
        std::vector<Vector3f> _tangents;
        std::vector<Vector2f> _uv;
        std::vector<unsigned> _indices;

        std::vector<char> _rawDatas;

        unsigned _mask{ VERTEX_NONE };

        VertexBufferPtr _vertexBuffer;
        IndexBufferPtr _indexBuffer;

        VertexBufferBindingPtr _bindings;

        VertexDeclarationPtr _vertexDec;

        ConstBufferPtr _objConstBuffer;
    };

    DECLAR_SMART_POINTER(Mesh);
}

#endif
