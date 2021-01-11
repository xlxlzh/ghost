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
    public:
        Mesh() : Mesh(RESOURCE_MESH, "", 0) { }
        Mesh(int type, const std::string& name, int flags);

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_MESH; }
        
        std::vector<MeshVertex>& getVertices() { return _vertices; }
        std::vector<unsigned>& getIndices() { return _indices; }

    private:
        std::vector<MeshVertex> _vertices;
        std::vector<unsigned> _indices;

        VertexBufferPtr _vertexBuffer;
        IndexBufferPtr _indexBuffer;

        VertexBufferBindingPtr _bindings;

        VertexDeclarationPtr _vertexDec;

        ConstBufferPtr _objConstBuffer;
    };

    DECLAR_SMART_POINTER(Mesh);
}

#endif
