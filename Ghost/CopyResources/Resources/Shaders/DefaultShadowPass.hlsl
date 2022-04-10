cbuffer PerObject
{
    float4x4 matWorldViewProj;
    float4x4 matWorld;
    float4x4 matWorldInverseTranspose;
};

cbuffer PerFrame
{
    float4 cameraPos;
};

cbuffer MainLight
{
    float4 lightDirRadius;
    float4 lightColor;
};

cbuffer SceneGlobalParams
{
    float4 ambientColor;
};

Texture2D texAlbedo;
Texture2D texNormal;

struct vs_input
{
    float3 position : POSITION0;
};

struct ps_input
{
    float4 position : SV_POSITION;
};

ps_input vs_main(vs_input input)
{
    ps_input pInput;
    pInput.position = mul(float4(input.position, 1.0), matWorldViewProj);
    return pInput;
}

float4 ps_main(ps_input pInput) : SV_TARGET
{
    return float4(0.0, 0.0, 0.0, 1.0);
}