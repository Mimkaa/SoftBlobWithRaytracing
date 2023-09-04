#pragma kernel CSMain
StructuredBuffer<float4> inputBuffer : register(t0);

RWTexture2D<float4> intermediateBuffer : register(u0);

struct FLUIDSIMCELL
{
    float2 vel;
    float2 denDiv; // desity and divergence
};

RWStructuredBuffer<FLUIDSIMCELL> fluidBuff : register(u1);

float index1D(float2 coord2d, int width)
{
    
    return coord2d.x + coord2d.y * width;
}


[numthreads(16, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    // clear divergence
    float dx = (fluidBuff[index1D(float2(dispatchThreadID.x + 1, dispatchThreadID.y), inputBuffer[0].y)].denDiv.y 
    - fluidBuff[index1D(float2(dispatchThreadID.x - 1, dispatchThreadID.y), inputBuffer[0].y)].denDiv.y) / 2;
    
    float dy = (fluidBuff[index1D(float2(dispatchThreadID.x, dispatchThreadID.y + 1), inputBuffer[0].y)].denDiv.y 
    - fluidBuff[index1D(float2(dispatchThreadID.x, dispatchThreadID.y - 1), inputBuffer[0].y)].denDiv.y) / 2;
    
    fluidBuff[index1D(dispatchThreadID.xy, inputBuffer[0].y)].vel -= float2(dx, dy);
    
    
    // current dt;
    float t = inputBuffer[0].x / 2000;
    
    fluidBuff[index1D(float2(200, 200), inputBuffer[0].y)].vel = float2(0.0f, 1.0f);
    fluidBuff[index1D(float2(200, 200), inputBuffer[0].y)].denDiv.x = 50.0f;
    
    
    intermediateBuffer[dispatchThreadID.xy] = float4(float3(1.0f, 1.0f, 1.0f) * fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].denDiv.x, 1.0f);
   

}