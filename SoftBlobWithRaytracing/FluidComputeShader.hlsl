#pragma kernel CSMain
StructuredBuffer<float4> inputBuffer : register(t0);

RWTexture2D<float4> intermediateBuffer : register(u0);

struct FLUIDSIMCELL
{
    float2 vel;
    float2 denDiv;// desity and divergence
};

RWStructuredBuffer<FLUIDSIMCELL> fluidBuff : register(u1);

float index1D(float2 coord2d, int width)
{
    if (coord2d.x < 0)
    {
        coord2d.x = 0;
    }
    else if(coord2d.x > width-1)
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

void DiffuseDens(float2 cellCoord, float dt, float visc, int it_num)
{
   
    
        fluidBuff[index1D(cellCoord, inputBuffer[0].y)].denDiv.x = (fluidBuff[index1D(cellCoord, inputBuffer[0].y)].denDiv.x + dt * visc
        * (fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].denDiv.x +
        fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].denDiv.x +
        fluidBuff[index1D(float2(cellCoord.x , cellCoord.y + 1), inputBuffer[0].y)].denDiv.x +
        fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].denDiv.x) / 4) / (1 + dt * visc);
    

}

void CalculateScalars(float2 cellCoord)
{
    float divergence = (fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].vel.x 
    - fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].vel.x +
    fluidBuff[index1D(float2(cellCoord.x, cellCoord.y + 1), inputBuffer[0].y)].vel.y +
     fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].vel.y) / 2;
    
    fluidBuff[index1D(cellCoord, inputBuffer[0].y)].denDiv.y = ((fluidBuff[index1D(float2(cellCoord.x - 1, cellCoord.y), inputBuffer[0].y)].denDiv.y
    - fluidBuff[index1D(float2(cellCoord.x + 1, cellCoord.y), inputBuffer[0].y)].denDiv.y +
    fluidBuff[index1D(float2(cellCoord.x, cellCoord.y - 1), inputBuffer[0].y)].denDiv.y +
     fluidBuff[index1D(float2(cellCoord.x, cellCoord.y + 1), inputBuffer[0].y)].denDiv.y) - divergence) / 4;
    

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
    float lowwerX = lerp(fluidBuff[index1D(float2(integerPart.x, integerPart.y+1), inputBuffer[0].y)].vel.x, fluidBuff[index1D(float2(integerPart.x + 1, integerPart.y+1), inputBuffer[0].y)].vel.x, fractPart.x);
    
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
    
    fluidBuff[0].denDiv = float2(1.0f, 0.0f);
    fluidBuff[0].vel = float2(1.0f, 0.0f);
    
    
    int2 texSize = int2(inputBuffer[0].y, inputBuffer[0].z);
    
    int2 texCoords = int2(dispatchThreadID.x, dispatchThreadID.y);
    
    // Normalize the texture coordinates to the [0, 1] range and center it
    float2 normalizedTexCoords = texCoords / float2(texSize) * 2 - 1;
    // flip y
    normalizedTexCoords.y = -normalizedTexCoords.y;
    // aspect retio to eliminate distortion
    normalizedTexCoords.x *= texSize.x / texSize.y;
    
    float2 uv = normalizedTexCoords;

    
    // current dt;
    float t = inputBuffer[0].x / 2000;
    
    float4 col = palette(length(uv) + t * 2);
    
    
    intermediateBuffer[dispatchThreadID.xy] = col;
}