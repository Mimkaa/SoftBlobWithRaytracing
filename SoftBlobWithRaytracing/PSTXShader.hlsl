
Texture2D texx : register(t0);
SamplerState samp : register(s0);


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 tex : TEXCOORD) : SV_TARGET
{
    // Sample the texture using the provided sampler
    float4 texColor = texx.Sample(samp, tex);

    return texColor;
}