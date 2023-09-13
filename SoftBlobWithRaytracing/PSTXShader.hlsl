#pragma kernel PShader
Texture2D texx : register(t0);
SamplerState samp : register(s0);


float4 PShader(float4 position : SV_POSITION, float4 norm : NORMAL, float2 tex : TEXTURE, float4 color : COLOR, float4 positionWPT : POSITIONWPT) : SV_TARGET
{
    // Sample the texture using the provided sampler
    float4 texColor = texx.Sample(samp, tex);

    return texColor;
}