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
    float3 lightDir;
    float3 lightColor;
};

Texture2D texAlbedo;
Texture2D texNormal;

struct vs_input
{
    float3 position : POSITION0;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
};

struct ps_input
{
    float4 position : SV_POSITION;
    float3 wpos : POSITION;
    float3 wnormal : NORMAL;
};

ps_input vs_main(vs_input input)
{
    ps_input pInput;
    pInput.position = mul(float4(input.position, 1.0), matWorldViewProj);
    pInput.wnormal = mul(input.normal ,(float3x3)matWorldInverseTranspose);
    pInput.wpos = mul(float4(input.position, 1.0), matWorld).xyz;
    return pInput;
}

float4 ps_main(ps_input pInput) : SV_TARGET
{
    float3 eyeDir = normalize(cameraPos.xyz - pInput.wpos);
    float3 wnormal = normalize(pInput.wnormal);
    float diffuse = saturate(dot(eyeDir, wnormal));

    float3 color = diffuse * lightColor;
    return float4(color, 1.0);
}