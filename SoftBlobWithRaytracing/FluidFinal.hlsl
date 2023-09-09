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

float3 palette(float t)
{
    float3 a = { 0.5, 0.500, 0.500, };
    float3 b = { 0.50, 0.50, 0.500 };
    float3 c = { 0.1, 0.1, 0.1 };
    float3 d = { 0.263, 0.416, 0.557 };
    //float3 a = { 0.610, 0.498, 0.650 };
    //float3 b = { 0.388, 0.498, 0.350};
    //float3 c = { 0.530, 0.498, 0.620 };
    //float3 d = { 3.438, 3.012, 4.025 };
    float3 res = a + b * cos(6.28318 * (c * t + d));
   
    return res;

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
    float t = inputBuffer[0].x ;
    float noise = inputBuffer[0].w ;
   // float rotAngle = ((noise(t) + 1)/2) * 3.14159;
    
    float2 inputVector = { 0.0f, 0.3f };
    // Calculate sine and cosine of the angle
    float sinAngle = sin(noise);
    float cosAngle = cos(noise);

    // Calculate the rotated vector
    float2 rotatedVector;
    rotatedVector.x = inputVector.x * cosAngle - inputVector.y * sinAngle;
    rotatedVector.y = inputVector.x * sinAngle + inputVector.y * cosAngle;
    
    if (inputBuffer[0].z < 0.55 & int(inputBuffer[1].w) == 0)
    {
        fluidBuff[index1D(float2(200, 200), inputBuffer[0].y)].denDiv.x = 50.0f;
        fluidBuff[index1D(float2(200, 200), inputBuffer[0].y)].vel = rotatedVector;
    }
    else
    {
        fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].denDiv.x *= 0.98;
        
    }
    
   
    
    float3 col = palette(t);
    float sum = (col.x + col.y + col.z) / 3;
    if (sum < 0.3)
    {
        col *= float3(2, 2, 2);
    }
    intermediateBuffer[dispatchThreadID.xy] = float4(col* fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].denDiv.x, 1.0f);
    
    float len = length(fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].vel);
    if (len > 0.5)
    {
        fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].vel /= len;
        fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].vel *= len;
        

    }
    // dencity fading
    
    if (int(inputBuffer[1].w) == 1)
    {
        float val = fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].denDiv.x;
       
        
        fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].denDiv.x *= 0.99;
        fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].vel.x *= 0.0;
        fluidBuff[index1D(float2(dispatchThreadID.xy), inputBuffer[0].y)].vel.y *= 0.0;
    }
    
    
    
    

}