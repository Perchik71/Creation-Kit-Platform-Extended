/***************************************************************************************************
  FXAA Fast approXimate Anti-Aliasing (Optimized Version)
***************************************************************************************************/

#define FXAA_EDGE_THRESHOLD      (1.0 / 16.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0 / 16.0)
#define FXAA_SEARCH_STEPS        16
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0 / 4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_TRIM         (1.0 / 4.0)
#define FXAA_SUBPIX_CAP          (3.0 / 4.0) 

#define FXAA_SUBPIX_TRIM_SCALE   (1.0 / (1.0 - FXAA_SUBPIX_TRIM))

struct InputData
{
    float4 ClipPosition : SV_POSITION;
    float2 UV           : TEXCOORD0;
};

Texture2D ColorBuffer      : register(t0);
SamplerState linearSampler : register(s0);

cbuffer dataBuffer : register(b0)
{
    float2 tilesize; // Inverse screen resolution (1.0 / Width, 1.0 / Height)
};

// Faster luminance function using standard Rec. 709 weights
float helper_Luma(float3 rgb)
{
    return dot(rgb, float3(0.2126, 0.7152, 0.0722));
}

float3 fxaa(InputData Data)
{
    // Gather 4-pixel footprint directly using modern hardware GatherRed/GatherGreen methods, 
    // or standard lookups optimized for cache lines.
    float3 rgbM = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0).rgb;
    
    // Explicit 4-tap offset checks
    float3 rgbN = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(0, -1)).rgb;
    float3 rgbE = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(1,  0)).rgb;
    float3 rgbW = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(-1, 0)).rgb;
    float3 rgbS = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(0,  1)).rgb;
    
    float lumaM = helper_Luma(rgbM);
    float lumaN = helper_Luma(rgbN);
    float lumaE = helper_Luma(rgbE);
    float lumaW = helper_Luma(rgbW);
    float lumaS = helper_Luma(rgbS);
    
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
    
    // Early exit condition for non-edges or dark regions
    if (range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
    {
        return rgbM;
    }
    
    // Sub-pixel filtering calculation
    float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
    float rangeL = abs(lumaL - lumaM);
    float blendL = max(0.0, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
    blendL = min(FXAA_SUBPIX_CAP, blendL);
    
    // Read diagonal neighborhood
    float3 rgbNW = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(-1, -1)).rgb;
    float3 rgbNE = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(1, -1)).rgb;
    float3 rgbSW = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(-1, 1)).rgb;
    float3 rgbSE = ColorBuffer.SampleLevel(linearSampler, Data.UV, 0.0, int2(1, 1)).rgb;
    
    float3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS + rgbNW + rgbNE + rgbSW + rgbSE;
    rgbL *= (1.0 / 9.0);
    
    float lumaNW = helper_Luma(rgbNW);
    float lumaNE = helper_Luma(rgbNE);
    float lumaSW = helper_Luma(rgbSW);
    float lumaSE = helper_Luma(rgbSE);
    
    // Edge detection filters
    float edgeVert = abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +
                     abs((0.50 * lumaW)  + (-1.0 * lumaM) + (0.50 * lumaE))  +
                     abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));
                     
    float edgeHorz = abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +
                     abs((0.50 * lumaN)  + (-1.0 * lumaM) + (0.50 * lumaS))  +
                     abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    
    bool horzSpan = edgeHorz >= edgeVert;
    float lengthSign = horzSpan ? -tilesize.y : -tilesize.x;
    
    if (!horzSpan)
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }
    
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;
    
    if (gradientN < gradientS)
    {
        lumaN = lumaS;
        gradientN = gradientS;
        lengthSign *= -1.0;
    }
        
    float2 posN;
    posN.x = Data.UV.x + (horzSpan ? 0.0 : lengthSign * 0.5);
    posN.y = Data.UV.y + (horzSpan ? lengthSign * 0.5 : 0.0);
    
    gradientN *= FXAA_SEARCH_THRESHOLD;
    
    float2 posP = posN;
    float2 offNP = horzSpan ? float2(tilesize.x, 0.0) : float2(0.0, tilesize.y);
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;
    
    posN -= offNP;
    posP += offNP;
    
    // Explicitly unroll loop hints for scalar execution units on modern GPUs
    [unroll]
    for (int i = 0; i < FXAA_SEARCH_STEPS; i++)
    {
        if (!doneN) lumaEndN = helper_Luma(ColorBuffer.SampleLevel(linearSampler, posN.xy, 0.0).rgb);
        if (!doneP) lumaEndP = helper_Luma(ColorBuffer.SampleLevel(linearSampler, posP.xy, 0.0).rgb);
        
        doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
        
        if (doneN && doneP) break;
        if (!doneN) posN -= offNP;
        if (!doneP) posP += offNP;
    }
    
    float dstN = horzSpan ? Data.UV.x - posN.x : Data.UV.y - posN.y;
    float dstP = horzSpan ? posP.x - Data.UV.x : posP.y - Data.UV.y;
    bool directionN = dstN < dstP;
    lumaEndN = directionN ? lumaEndN : lumaEndP;
    
    if (((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
        lengthSign = 0.0;
    
    float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5 + (dstN * (-1.0 / spanLength))) * lengthSign;
    
    float3 rgbF = ColorBuffer.SampleLevel(linearSampler, float2(
        Data.UV.x + (horzSpan ? 0.0 : subPixelOffset),
        Data.UV.y + (horzSpan ? subPixelOffset : 0.0)), 0.0).rgb;
    
    return lerp(rgbF, rgbL, blendL);

}

float4 main(InputData Data) : SV_Target
{
    return float4(fxaa(Data), 1.0);
}
