

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

VOut VShader(float4 position : POSITION, float3 norm : NORMAL, float2 texx : TEXTURE, float4 color : COLOR)
{
    VOut output;

    output.position = mul(mul(position, World), Proj);

    output.norm = mul(norm, World);
    
    output.tex = texx;
    
    output.color = color;
    
    output.positionWPT = mul(position, World);
    
    return output;
}