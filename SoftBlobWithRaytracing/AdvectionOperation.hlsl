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
        coord2d.x = width - 1;
    }
    else if (coord2d.x > width - 1)
    {
        coord2d.x = 0;

    }
    
    if (coord2d.y < 0)
    {
        coord2d.y = width - 1;
    }
    else if (coord2d.y > width - 1)
    {
        coord2d.y = 0;

    }
    return coord2d.x + coord2d.y * width;
}




float lerp(float a, float b, float c)
{
    return a + (b - a) * c;
}

void AdvectVel(float2 cellCoord, float dt)
{
    float2 velComesFrom = cellCoord - fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel * dt;
    int2 integerPart = floor(velComesFrom);
    float2 fractPart = frac(velComesFrom);
    
    float upperX = lerp(fluidBuff[index1D(float2(integerPart.x, integerPart.y), inputBuffer[0].y)].vel.x, fluidBuff[index1D(float2(integerPart.x + 1, integerPart.y), inputBuffer[0].y)].vel.x, fractPart.x);
    float lowwerX = lerp(fluidBuff[index1D(float2(integerPart.x, integerPart.y + 1), inputBuffer[0].y)].vel.x, fluidBuff[index1D(float2(integerPart.x + 1, integerPart.y + 1), inputBuffer[0].y)].vel.x, fractPart.x);
    
    float centerX = lerp(upperX, lowwerX, fractPart.y);
    
    float upperY = lerp(fluidBuff[index1D(float2(integerPart.x, integerPart.y), inputBuffer[0].y)].vel.y, fluidBuff[index1D(float2(integerPart.x + 1, integerPart.y), inputBuffer[0].y)].vel.y, fractPart.x);
    float lowwerY = lerp(fluidBuff[index1D(float2(integerPart.x, integerPart.y + 1), inputBuffer[0].y)].vel.y, fluidBuff[index1D(float2(integerPart.x + 1, integerPart.y + 1), inputBuffer[0].y)].vel.y, fractPart.x);
    
    float centerY = lerp(upperY, lowwerY, fractPart.y);
    
    fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel = float2(centerX, centerY);
     
    
}

void AdvectDen(float2 cellCoord, float dt)
{
    float2 velComesFrom = cellCoord - fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel * dt;
    int2 integerPart = floor(velComesFrom);
    float2 fractPart = frac(velComesFrom);
    
    float upperX = lerp(fluidBuff[index1D(float2(integerPart.x, integerPart.y), inputBuffer[0].y)].denDiv.x, fluidBuff[index1D(float2(integerPart.x + 1, integerPart.y), inputBuffer[0].y)].denDiv.x, fractPart.x);
    float lowwerX = lerp(fluidBuff[index1D(float2(integerPart.x, integerPart.y + 1), inputBuffer[0].y)].denDiv.x, fluidBuff[index1D(float2(integerPart.x + 1, integerPart.y + 1), inputBuffer[0].y)].denDiv.x, fractPart.x);
    
    float centerX = lerp(upperX, lowwerX, fractPart.y);
    
    fluidBuff[index1D(cellCoord, inputBuffer[0].y)].denDiv.x = centerX;
}

void ReflectFromBoundaries(float2 cellCoord)
{
    if (cellCoord.x == inputBuffer[0].y - 2)
    {
        fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel.x *= -1;
       

    }
    else if (cellCoord.x == 2)
    {
        fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel.x *= -1;
        

    }
    
    if (cellCoord.y == inputBuffer[0].y - 2)
    {
        fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel.y *= -1;
        

    }
    else if (cellCoord.y == 2)
    {
        fluidBuff[index1D(cellCoord, inputBuffer[0].y)].vel.y *= -1;
        

    }

}


[numthreads(16, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    
	
    
    // current dt;
    float t = inputBuffer[0].x;
	
    AdvectVel(float2(dispatchThreadID.xy), t);
    AdvectDen(float2(dispatchThreadID.xy), t);
    ReflectFromBoundaries(float2(dispatchThreadID.xy));
    
   

}