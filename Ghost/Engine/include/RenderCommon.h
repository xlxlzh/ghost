#ifndef _RENDER_COMMON_H_
#define _RENDER_COMMON_H_

#include "Ghost.h"

namespace ghost
{
    enum RendersystemType
    {
        RENDER_D3D11,
        RENDER_D3D12,
        RENDER_OPENGL4,
        RENDER_VULKAN
    };

    enum MSAA
    {
        _1x,
        _2x,
        _4x,
        _8x,
        _16x,
    };

    GHOST_API unsigned GetMSAASampleCount(MSAA ms);
    GHOST_API MSAA GetMSAATypeBySampleCount(unsigned sampleCount);

    enum TargetClear
    {
        CLEAR_COLOR = 0x01,
        CLEAR_DEPTH = 0x02,
        CLEAR_STENCIL = 0x04,
        CLEAR_DEPTH_STENCIL = CLEAR_DEPTH | CLEAR_STENCIL,
        CLEAR_ALL = CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL
    };

    enum PrimitiveType
    {
        PRIMITIVE_UNDEFINED = 0,
        PRIMITIVE_POINTLIST = 1,

        PRIMITIVE_LINELIST = 2,
        PRIMITIVE_LINESTRIP = 3,

        PRIMITIVE_TRIANGLELIST = 4,
        PRIMITIVE_TRIANGLESTRIP = 5
    };

    enum CullMode
    {
        CULL_CW,
        CULL_CCW,
        CULL_NONE,
    };

    enum FillMode
    {
        FILL_SOLID,
        FILL_WIREFRAME,
    };

    enum CompareFunction
    {
        COMPARISON_NEVER,
        COMPARISON_ALWAYS,
        COMPARISON_LESS,
        COMPARISON_LESS_EQUAL,
        COMPARISON_EQUAL,
        COMPARISON_NOT_EQUAL,
        COMPARISON_GREATER_EQUAL,
        COMPARISON_GREATER
    };
}

#endif
