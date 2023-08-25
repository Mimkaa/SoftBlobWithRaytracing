#define TWIDTH 400
#define THEIGHT 300

RWTexture2D<float4> intermediateBuffer : register(u0);

[numthreads(32, 32, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    // Calculate the value
    float4 calculatedValue = { dispatchThreadID.x, dispatchThreadID.y, 0.0f, 1.0f };

    // Calculate the flat index for the 2D position
    int flatIndex = dispatchThreadID.x + dispatchThreadID.y * TWIDTH;

    // Write the calculated value to the intermediate buffer
    intermediateBuffer[dispatchThreadID.xy] = calculatedValue;
}