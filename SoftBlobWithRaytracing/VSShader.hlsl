

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

VOut VShader(float4 position : POSITION, float3 norm : NORMAL, float2 texx : TEXTURE, float4 color : COLOR)
{
    VOut output;

    output.position = mul(mul(mul(position, World), View), Proj);

    output.norm = mul(mul(norm, World), View);
    
    output.tex = texx;
    
    output.color = color;
    
    output.positionWPT = mul(mul(position, World), View);
    
    return output;
}