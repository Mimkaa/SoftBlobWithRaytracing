Texture2D texx : register(t0);
SamplerState samp : register(s0);


float4 PShader(float4 position : SV_POSITION, float4 norm : NORMAL, float4 color : COLOR) : SV_TARGET
{
   
    float2 tex = float2(norm.x / 2 + 0.5, norm.y / 2 + 0.5);
    
    // Sample the texture using the provided sampler
    float4 texColor = texx.Sample(samp, tex);

    return texColor;
}