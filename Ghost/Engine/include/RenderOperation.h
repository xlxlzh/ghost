#ifndef _RENDEROPERATION_H_
#define _RENDEROPERATION_H_

#include "Ghost.h"
#include "RenderCommon.h"
#include "VertexDeclaration.h"
#include "VertexBufferBinding.h"
#include "IndexBuffer.h"

namespace ghost
{
    struct GHOST_API RenderOperation
    {
        VertexDeclarationPtr _vertexDecl;
        VertexBufferBindingPtr _vertexBinding;
        IndexBufferPtr _indexBuffer;

        bool _useIndex;

        PrimitiveType _primitiveType;

        RenderOperation() :
            _vertexDecl(nullptr),
            _vertexBinding(nullptr),
            _indexBuffer(nullptr),
            _useIndex(true),
            _primitiveType(PRIMITIVE_TRIANGLELIST)
        {

        }
    };
}


#endif
