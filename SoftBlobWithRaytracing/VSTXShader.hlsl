#pragma kernel VShader
cbuffer ConstantBuffer : register(b0)
{
    float4x4 World;
    float4x4 Proj;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 tex : TEXCOORD)
{
    VOut output;

    output.position = mul(mul(position, World), Proj);
    
    output.color = color;
    
    output.tex = tex;

    return output;
}