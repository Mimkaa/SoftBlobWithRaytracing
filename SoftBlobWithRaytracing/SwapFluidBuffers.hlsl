#pragma kernel CSMain
StructuredBuffer<float4> inputBuffer : register(t0);

RWTexture2D<float4> intermediateBuffer : register(u0);

struct FLUIDSIMCELL
{
    float2 vel;
    float2 denDiv; // desity and divergence
};

RWStructuredBuffer<FLUIDSIMCELL> fluidBuff : register(u1);
RWStructuredBuffer<FLUIDSIMCELL> fluidBuffPrev : register(u2);

float index1D(float2 coord2d, int width)
{
    
    return coord2d.x + coord2d.y * width;
}


[numthreads(16, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    fluidBuffPrev[index1D(dispatchThreadID.xy, inputBuffer[0].y)].vel = fluidBuff[index1D(dispatchThreadID.xy, inputBuffer[0].y)].vel;
    fluidBuffPrev[index1D(dispatchThreadID.xy, inputBuffer[0].y)].denDiv = fluidBuff[index1D(dispatchThreadID.xy, inputBuffer[0].y)].denDiv;

}