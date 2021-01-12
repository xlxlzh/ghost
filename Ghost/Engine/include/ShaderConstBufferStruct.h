#ifndef _SHADERCONSTBUFFERSTRUCT_H_
#define _SHADERCONSTBUFFERSTRUCT_H_

#include "Ghost.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace ghost
{
    struct GHOST_API PerObject
    {
        Matrix4x4f _matMVP;
        Matrix4x4f _matWorld;
        Matrix4x4f _matWorldInverseTranspose;
    };

    struct GHOST_API PerFrame
    {
        Vector4f _cameraPos;
    };

    struct GHOST_API MainLight
    {
        //xyz: lightdir
        //w: ligth radius, use for spot and point light
        Vector4f _lightDirAndRadius;
        Vector4f _lightColor;
    };

    struct GHOST_API SceneGlobalParams
    {
        Vector4f _ambientColor;
    };
}

#endif
