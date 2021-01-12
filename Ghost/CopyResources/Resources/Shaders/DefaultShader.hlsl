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

    float3 lightDir = normalize(-lightDirRadius.xyz);
    float3 h = normalize(eyeDir + lightDir);

    float ndl = saturate(dot(wnormal, lightDir));
    float ndh = saturate(dot(wnormal, h));

    float specular = pow(ndh, lightColor.w);

    float3 color = ndl * lightColor.xyz + specular * ndl * lightColor.xyz + ambientColor.rgb;
    return float4(color, 1.0);
}