uniform float4x4 matWorld;
uniform float4x4 matWorldViewProj;

cbuffer UpdatePerFrame
{
    float4 abcd;
};

Texture2D texAlbedo;
Texture2D texNormal;

SamplerState samplerNormal;

struct vs_input
{
    float4 position : POSITION0;
    float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
};

float4 vs_main(vs_input input) : SV_POSITION
{
    return mul(input.position, matWorld);
}