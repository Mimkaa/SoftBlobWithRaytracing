#pragma kernel VShader
#define PI 3.14159265358979323846
cbuffer ConstantBuffer : register(b0)
{
    float4x4 World;
    float4x4 Proj;
    float4x4 View;
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

    output.position = mul(mul(mul(position, World), View), Proj);

    output.norm = mul(mul(norm, World), View);
    
    
    output.positionWPT = mul(mul(position, World), View);
    

    
    output.tex = float2(asin(norm.x) / PI + 0.5, asin(norm.y) / PI + 0.5);
    output.color = color;
    
    
    
    return output;
}