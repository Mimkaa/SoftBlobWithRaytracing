#pragma kernel CSMain
StructuredBuffer<float> inputBuffer : register(t0);

RWTexture2D<float4> intermediateBuffer : register(u0);

float4 palette(float t)
{
    float3 a = { 0.5, 0.500, 0.500, };
    float3 b = { 0.50, 0.50, 0.500 };
    float3 c = { 0.1, 0.1, 0.1 };
    float3 d = { 0.263, 0.416, 0.557 };
    float3 res = a + b * cos(6.28318 * (c * t + d));
   
    return float4(res, 1.0f);

}

[numthreads(16, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    int2 texSize = int2(400, 400);
    
    int2 texCoords = int2(dispatchThreadID.x, dispatchThreadID.y);
    
    // Normalize the texture coordinates to the [0, 1] range and center it
    float2 normalizedTexCoords = texCoords / float2(texSize) * 2 - 1;
    // flip y
    normalizedTexCoords.y = -normalizedTexCoords.y;
    // aspect retio to eliminate distortion
    normalizedTexCoords.x *= texSize.x/texSize.y;
    
    // remembering the center before repeating pattern
    float2 uv0 = normalizedTexCoords;
    
    float4 finalColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    // repeat coords and center it
    for (int i = 0; i < 4.0f; i++)
    {
        // not 2 to budge it a little
        normalizedTexCoords = frac(normalizedTexCoords * 1.5) - 0.5;
 

        // simple circle
    
        float len = length(normalizedTexCoords) * exp(-length(uv0));
    
        float t = inputBuffer[0] / 2000;
    
        // another color 
        // because of some reason it emits more blue???
       //float4 col = palette(len +t*2);
        // with one center
        float4 col = palette(length(uv0) + t * 2 + i*0.5);
   
    
        float d = sin(len * 8.0f + t) / 8.0f;
    
    
    
    
        // make a gradient ring by reversing the minus values with their reverse in the middle with abs
    
        d = abs(d);
    
        // make a threashold with a certain value and make the edges sharp
    
        //len = step(0.1f, len);
    
        // the same but with a gradient between boundaries
    
        //d = smoothstep(0.0f, 0.1f, d);
    
        //inverse with a preasant glow
        d = 0.01 / d;
        
        // enhence the contrast
        
        d = pow(d, 1.2);
    
    
        finalColor += col * d;
    }
    
    intermediateBuffer[dispatchThreadID.xy] = finalColor;
}