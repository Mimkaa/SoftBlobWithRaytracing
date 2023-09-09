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

void DiffuseDens(float2 cellCoord, float dt, float visc)
{
   
    
    fluidBuff[index1D(cellCoord, inputBuffer[0].y)].denDiv.x = (fluidBuffPrev[index1D(cellCoord, inputBuffer[0].y)].denDiv.x + dt * visc
        * (fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].denDiv.x +
        fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].denDiv.x +
        fluidBuff[index1D(float2(cellCoord.x, cellCoord.y + 1), inputBuffer[0].y)].denDiv.x +
        fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].denDiv.x) / 4) / (1 + dt * visc);
    

}


void DiffuseVels(float2 cellCoord, float dt, float visc)
{
   
    
    fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel.x = (fluidBuffPrev[index1D(cellCoord, inputBuffer[0].y)].vel.x + dt * visc
        * (fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].vel.x +
        fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].vel.x +
        fluidBuff[index1D(float2(cellCoord.x, cellCoord.y + 1), inputBuffer[0].y)].vel.x +
        fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].vel.x) / 4) / (1 + dt * visc);
    
    fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel.y = (fluidBuffPrev[index1D(cellCoord, inputBuffer[0].y)].vel.y + dt * visc
        * (fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].vel.y +
        fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].vel.y +
        fluidBuff[index1D(float2(cellCoord.x, cellCoord.y + 1), inputBuffer[0].y)].vel.y +
        fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].vel.y) / 4) / (1 + dt * visc);
    

}


[numthreads(16, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    
    // current dt;
    float t = inputBuffer[0].x;
    
    DiffuseVels(dispatchThreadID.xy, t, 0.000000001);
    DiffuseDens(dispatchThreadID.xy, t, 0.000000001);
    
    
   
}