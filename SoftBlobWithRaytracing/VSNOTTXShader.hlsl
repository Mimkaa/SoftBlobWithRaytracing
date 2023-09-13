#pragma kernel VShader
#define PI 3.14159265358979323846
cbuffer ConstantBuffer : register(b0)
{
    float4x4 World;
    float4x4 Proj;
};

struct VOut
{
    float4 position : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXTURE;
    float4 color : COLOR;
    float4 positionWPT : POSITIONWPT;
};

VOut VShader(float4 position : POSITION, float3 norm : NORMAL, float2 tex: TEXTURE, float4 color : COLOR)
{
    VOut output;

    output.position = mul(mul(position, World), Proj);

    output.norm = mul(norm, World);
    

    
    output.tex = float2(asin(norm.x) / PI + 0.5, asin(norm.y) / PI + 0.5);
    output.color = color;
    
    output.positionWPT = mul(position, World);
    
    return output;
}