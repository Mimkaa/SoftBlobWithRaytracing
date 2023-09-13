#pragma kernel PShader
Texture2D texx : register(t0);
SamplerState samp : register(s0);



static const float3 lightPos = { 0.0f, 0.0f, 0.0f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 BlendPixel(float4 sourceColor, float4 destinationColor)
{
    // Calculate the blended color using alpha blending equation
    float alpha = sourceColor.a;
    float oneMinusAlpha = 1.0 - alpha;

    float4 blendedColor;
    blendedColor.rgb = sourceColor.rgb * alpha + destinationColor.rgb * oneMinusAlpha;
    blendedColor.a = alpha + destinationColor.a * oneMinusAlpha;

    return blendedColor;
}


float4 PShader(float4 position : SV_POSITION, float3 norm : NORMAL, float2 tex : TEXTURE, float4 color : COLOR, float4 positionWPT : POSITIONWPT) : SV_TARGET
{
    
    // Sample the texture using the provided sampler
    
    float4 texColor = texx.Sample(samp, tex);
    float4 finalTexColor = BlendPixel(texColor, float4(1, 0.6039, 0.5412, 1.0f));
    
    //lighting//////////
    
    // fragment to light vector data
    const float3 vToL = lightPos - positionWPT;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, norm)) ;
	
    // specular
    
    // reflected light vector
    const float3 w = norm * dot(vToL, norm);
    const float3 r = w * 2.0f - vToL;
    const float3 specular = att * (diffuseColor * diffuseIntensity) * 1.0f * pow(max(0.0f, dot(normalize(-r), normalize(positionWPT))), 30.0f);

    return finalTexColor * float4(saturate(diffuse + ambient), 1.0f) + float4(specular,1.0f);

}