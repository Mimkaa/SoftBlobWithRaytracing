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
    if (coord2d.x < 0)
    {
        coord2d.x = 0;
    }
    else if (coord2d.x > width - 1)
    {
        coord2d.x = width - 1;

    }
    
    if (coord2d.y < 0)
    {
        coord2d.y = 0;
    }
    else if (coord2d.y > width - 1)
    {
        coord2d.y = width - 1;

    }

    return coord2d.x + coord2d.y * width;
}

void CalculateScalars(float2 cellCoord)
{
    float divergence = (fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].vel.x 
    - fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].vel.x +
    fluidBuff[index1D(float2(cellCoord.x, cellCoord.y + 1), inputBuffer[0].y)].vel.y -
     fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].vel.y) / 2;
    
    fluidBuff[index1D(cellCoord, inputBuffer[0].y)].denDiv.y = ((fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].denDiv.y
    - fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].denDiv.y +
    fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].denDiv.y +
     fluidBuff[index1D(float2(cellCoord.x, cellCoord.y + 1), inputBuffer[0].y)].denDiv.y) - divergence) / 4;
    

}


[numthreads(16, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    
    // current dt;
    float t = inputBuffer[0].x;
    
    CalculateScalars(float2(dispatchThreadID.xy));
    
    
   
}