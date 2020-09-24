uniform float4x4 matWorld;
uniform float4x4 matWorldViewProj;

cbuffer UpdatePerFrame
{
    float4x4 TestConstBuffer;
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

float4 ps_main() : SV_TARGET
{
    return float4(1.0, 0.0, 0.0, 1.0);
}